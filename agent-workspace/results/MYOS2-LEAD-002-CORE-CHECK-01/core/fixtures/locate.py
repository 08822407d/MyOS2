"""Definition-boundary locators used by V00 (anchor check) and by fixture extraction.

All functions work on the exact file text (str, split on "\\n"); line numbers are 1-based.
Views are length-preserving: comments/strings are blanked with spaces, newlines kept, so offsets
map back to the original text. Every locator returns a list of candidate ranges with the method
used; callers decide which candidate (if any) contains a quote. Heuristic locators say so.
"""
import re


# ---------------------------------------------------------------- views
def strip_c(text):
    """Blank C/C++ comments and string/char literal contents (keep quotes and newlines)."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i)); i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append("".join(ch if ch == "\n" else " " for ch in text[i:j])); i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            body = "".join(ch if ch == "\n" else " " for ch in text[i + 1:j - 1])
            out.append(c + body + (text[j - 1] if j - i >= 2 else "")); i = j
        else:
            out.append(c); i += 1
    return "".join(out)


def blank_pp(view):
    """Blank preprocessor directive lines (and their backslash continuations) in a C view."""
    lines = view.split("\n")
    cont = False
    for k, ln in enumerate(lines):
        if cont or ln.lstrip().startswith("#"):
            cont = ln.rstrip().endswith("\\")
            lines[k] = " " * len(ln)
        else:
            cont = False
    return "\n".join(lines)


def strip_hash_comments(text, quote_chars="\""):
    """Blank '#' comments and quoted-string contents (CMake / shell style, line oriented)."""
    out = []
    for ln in text.split("\n"):
        buf, q, i = [], None, 0
        while i < len(ln):
            ch = ln[i]
            if q:
                if ch == "\\" and i + 1 < len(ln):
                    buf.append("  "); i += 2; continue
                buf.append(ch if ch == q else " ")
                if ch == q:
                    q = None
            elif ch in quote_chars:
                q = ch; buf.append(ch)
            elif ch == "#" and not ln[max(0, i - 1):i] == "$":
                buf.append(" " * (len(ln) - i)); break
            else:
                buf.append(ch)
            i += 1
        out.append("".join(buf))
    return "\n".join(out)


def strip_cmake(text):
    """CMake: blank # comments; blank quoted strings even when they span lines."""
    out, i, n, inq = [], 0, len(text), False
    while i < n:
        ch = text[i]
        if inq:
            if ch == "\\" and i + 1 < n:
                out.append("  " if text[i + 1] != "\n" else " \n"); i += 2; continue
            if ch == "\"":
                inq = False; out.append(ch)
            else:
                out.append("\n" if ch == "\n" else " ")
        elif ch == "\"":
            inq = True; out.append(ch)
        elif ch == "#":
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i)); i = j; continue
        else:
            out.append(ch)
        i += 1
    return "".join(out)


def line_of(text, off):
    return text.count("\n", 0, off) + 1


def match_close(view, open_off, o="{", c="}"):
    depth = 0
    for k in range(open_off, len(view)):
        if view[k] == o:
            depth += 1
        elif view[k] == c:
            depth -= 1
            if depth == 0:
                return k
    return -1


def depth_at(view, off, o="{", c="}"):
    return view.count(o, 0, off) - view.count(c, 0, off)


# ---------------------------------------------------------------- C family
ATTR = re.compile(r"\s*([A-Za-z_]\w*)\s*(\()?")


def c_function(text, name):
    """Candidates for a C function *definition* named `name` at brace depth 0."""
    view = blank_pp(strip_c(text))
    cands = []
    for m in re.finditer(r"(?<![\w$])" + re.escape(name) + r"\s*\(", view):
        if depth_at(view, m.start()) != 0:
            continue
        po = m.end() - 1
        pc = match_close(view, po, "(", ")")
        if pc < 0:
            continue
        k = pc + 1
        while True:  # skip attribute-like tokens, e.g. __releases(rq->lock)
            a = ATTR.match(view, k)
            if not a:
                break
            if a.group(2):
                e = match_close(view, a.end() - 1, "(", ")")
                if e < 0:
                    break
                k = e + 1
            else:
                k = a.end()
        while k < len(view) and view[k] in " \t\n":
            k += 1
        if k < len(view) and view[k] == "{":
            close = match_close(view, k)
            cands.append({"kind": "c_function", "name_line": line_of(text, m.start()),
                          "start": head_start(view, line_of(text, m.start())),
                          "body_open": line_of(text, k), "end": line_of(text, close),
                          "method": "comment/string/preprocessor-blanked view; depth-0 name(...) followed by {; brace match"})
    return cands


def head_start(view, name_line):
    """Walk back over declaration-specifier lines (e.g. PREFIX_STATIC_INLINE / return type)."""
    vl = view.split("\n")
    s = name_line
    while s > 1:
        prev = vl[s - 2].strip()
        if not prev or prev.endswith((";", "}", "{", ")")) or prev.startswith("#"):
            break
        s -= 1
    return s


def c_macro(text, name):
    """Candidates for #define name (object- or function-like), with continuation lines."""
    view = strip_c(text)
    raw, vl = text.split("\n"), view.split("\n")
    pat = re.compile(r"^\s*#\s*define\s+" + re.escape(name) + r"(?![\w])")
    cands = []
    for k, ln in enumerate(vl):
        if pat.match(ln):  # match in comment-blanked view => active (not commented) directive
            e = k
            while raw[e].rstrip().endswith("\\") and e + 1 < len(raw):
                e += 1
            cands.append({"kind": "c_macro", "start": k + 1, "end": e + 1,
                          "method": "#define line in comment-blanked view + backslash continuations"})
    return cands


def c_struct(text, tag):
    view = blank_pp(strip_c(text))
    cands = []
    for m in re.finditer(r"\bstruct\s+" + re.escape(tag) + r"\s*\{", view):
        o = m.end() - 1
        close = match_close(view, o)
        cands.append({"kind": "c_struct", "start": line_of(text, m.start()), "end": line_of(text, close),
                      "method": "struct tag { ... } brace match"})
    return cands


def c_macro_generated_initializer(text, macro, arg):
    """e.g. DEFINE_SCHED_CLASS(myos_rt) = { ... };"""
    view = blank_pp(strip_c(text))
    cands = []
    for m in re.finditer(r"\b" + re.escape(macro) + r"\s*\(\s*" + re.escape(arg) + r"\s*\)\s*=\s*\{", view):
        o = m.end() - 1
        close = match_close(view, o)
        cands.append({"kind": "macro_generated_initializer", "start": line_of(text, m.start()),
                      "end": line_of(text, close),
                      "method": "%s(%s) = { ... } brace match" % (macro, arg)})
    return cands


def pp_stack(text, line_no):
    """Active preprocessor conditional directives enclosing line_no (C family)."""
    vl = strip_c(text).split("\n")
    stack = []
    for k in range(0, min(line_no - 1, len(vl))):
        s = vl[k].strip()
        m = re.match(r"#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)", s)
        if not m:
            continue
        d = m.group(1)
        if d in ("if", "ifdef", "ifndef"):
            stack.append("#%s%s" % (d, m.group(2).rstrip()))
        elif d in ("elif", "else") and stack:
            stack[-1] = stack[-1] + " -> #%s%s" % (d, m.group(2).rstrip())
        elif d == "endif" and stack:
            stack.pop()
    return stack


# ---------------------------------------------------------------- CMake / ld / shell / asm
def cmake_command(text, symbol):
    """set(SYMBOL ...), file(<mode> SYMBOL ...), or a command named SYMBOL(...)."""
    view = strip_cmake(text)
    pats = [r"\bset\s*\(\s*" + re.escape(symbol) + r"\b",
            r"\bfile\s*\(\s*[A-Z_]+\s+" + re.escape(symbol) + r"\b",
            r"(?<![\w$])" + re.escape(symbol) + r"\s*\("]
    cands = []
    for pat in pats:
        for m in re.finditer(pat, view):
            po = view.index("(", m.start())
            pc = match_close(view, po, "(", ")")
            cands.append({"kind": "cmake_command", "start": line_of(text, m.start()),
                          "end": line_of(text, pc), "pattern": pat,
                          "method": "CMake view (comments/strings blanked); command ( ... ) paren match"})
    uniq = {(c["start"], c["end"]): c for c in cands}
    return sorted(uniq.values(), key=lambda c: c["start"])


def ld_assignment(text, symbol):
    view = strip_c(text)
    cands = []
    for m in re.finditer(r"(?m)^[ \t]*" + re.escape(symbol) + r"\s*=", view):
        semi = view.index(";", m.start())
        cands.append({"kind": "ld_symbol_assignment", "start": line_of(text, m.start()),
                      "end": line_of(text, semi), "brace_depth": depth_at(view, m.start()),
                      "method": "linker-script assignment '<sym> = ...;' (comments blanked)"})
    return cands


def shell_functions(text):
    """Heuristic shell function ranges: one-liners 'f() { ...; }' and blocks closed by '}' at col 0."""
    raw = text.split("\n")
    view = strip_hash_comments(text, quote_chars="\"'").split("\n")
    fns = []
    for k, ln in enumerate(view):
        m = re.match(r"^\s*(?:function\s+)?([A-Za-z_][\w-]*)\s*\(\)\s*\{(.*)$", ln)
        if not m:
            continue
        if m.group(2).rstrip().endswith("}"):
            fns.append((m.group(1), k + 1, k + 1))
            continue
        e = k + 1
        while e < len(raw) and raw[e].rstrip() != "}":
            e += 1
        fns.append((m.group(1), k + 1, e + 1))
    return fns


def asm_symbol(text, symbol):
    raw = text.split("\n")
    view = strip_c(text).split("\n")
    cands = []
    for k, ln in enumerate(view):
        if re.match(r"^\s*SYM_\w+_START\w*\(\s*" + re.escape(symbol) + r"\s*\)", ln) or \
                re.match(r"^" + re.escape(symbol) + r":", ln):
            e = k + 1
            end_kind = "EOF"
            while e < len(view):
                s = view[e]
                if re.match(r"^\s*SYM_\w+_END\w*\(\s*" + re.escape(symbol) + r"\s*\)", s):
                    end_kind = "SYM_*_END"; break
                if re.match(r"^\s*SYM_\w+_START", s) or re.match(r"^[A-Za-z_.$][\w.$]*:", s):
                    end_kind = "next SYM_*_START or column-0 label"; e -= 1; break
                e += 1
            cands.append({"kind": "asm_symbol", "start": k + 1, "end": min(e + 1, len(raw)),
                          "end_rule": end_kind,
                          "method": "heuristic: from SYM_*_START(sym)/sym: to its END, next start/label, or EOF"})
    return cands


def find_contiguous(lines, quote):
    n = len(quote)
    return [i + 1 for i in range(0, len(lines) - n + 1) if lines[i:i + n] == quote]


def lines_blank_in_view(view, start, n, raw=None):
    """Quote lines that carry text in the raw file but are blank in the comment-blanked view."""
    vl = view.split("\n")
    rl = raw.split("\n") if raw is not None else None
    return [start + j for j in range(n)
            if not vl[start - 1 + j].strip() and (rl is None or rl[start - 1 + j].strip())]
