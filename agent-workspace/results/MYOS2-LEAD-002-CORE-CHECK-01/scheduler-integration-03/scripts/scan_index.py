# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-INTEGRATION-03 (scheduler-integration-03)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; no code copied from earlier packages
# purpose: D01 of the 16 contract. Targeted, read-only text search of the pinned kernel object
#   (time a039d9803ade, scope mykernel/) for the fixed field/entry/list terms, using `git grep`
#   against the commit object (no checkout, no build, no execution of repository code).
#   Every hit gets: path, line, verbatim text, enclosing top-level symbol, whether each match
#   sits inside a comment, the enclosing preprocessor conditions, a mechanical kind and one of the
#   contract's four classes. Also records local pointer aliases, one-identifier macro aliases,
#   definition sites of the macros/helpers the chains need (or "unlocated"), and direct call /
#   reference edges for the seed entries. A missing hit is never turned into "no other writer".
# --------------------------------------------------------------------------------------------------
"""Usage: python3 scan_index.py <out.json>"""
import json
import re
import subprocess
import sys

REPO = "/home/user/MyOS2"
PIN = "a039d9803ade"            # time, kernel source pin (short id only)
SCOPE = "mykernel"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")

# (id, group, mode, pattern, scope) -- mode: word = git grep -w -F; regex = git grep -E; fixed = -F
QUERIES = [
    ("F01", "field", "word", "running_lhdr", SCOPE),
    ("F02", "field", "word", "last_jiffies", SCOPE),
    ("F03", "field", "word", "vruntime", SCOPE),
    ("F04", "field", "word", "run_list", SCOPE),
    ("F05", "field", "regex", r"(\.|->)myos\b", SCOPE),          # member access to rq->myos
    ("F06", "field", "word", "myos_rq", SCOPE),                  # local alias name used in myos_rt.c
    ("F07", "field", "word", "myos_rt_sched_class", SCOPE),
    ("F08", "field", "word", "time_slice", SCOPE),               # read by the switch condition
    ("F09", "field", "word", "jiffies_64", SCOPE),               # linker alias target of jiffies
    ("F10", "field", "regex", r"\b(sum_exec_runtime|exec_start|prev_sum_exec_runtime)\b", SCOPE),  # other time accounting
    ("F11", "field", "regex", r"\b(nr_switches|nvcsw|nivcsw)\b", SCOPE),                        # switch counters
    ("F12", "field", "word", "on_rq", SCOPE),                     # candidate membership flag
    ("E01", "entry", "word", "set_task_cpu", SCOPE),
    ("E02", "entry", "word", "try_to_wake_up", SCOPE),
    ("E03", "entry", "word", "wake_up_process", SCOPE),
    ("E04", "entry", "word", "wake_up_new_task", SCOPE),
    ("E05", "entry", "word", "__sched_fork", SCOPE),
    ("E06", "entry", "word", "sched_fork", SCOPE),
    ("E07", "entry", "word", "init_idle", SCOPE),
    ("E08", "entry", "word", "sched_init", SCOPE),
    ("E09", "entry", "word", "__schedule", SCOPE),
    ("E10", "entry", "word", "pick_next_task_myos", SCOPE),
    ("E11", "entry", "word", "pick_next_task", SCOPE),            # class hook / macro alias
    ("E12", "entry", "word", "__pick_next_task", SCOPE),
    ("E13", "entry", "word", "for_each_class", SCOPE),
    ("E14", "entry", "word", "DEFINE_SCHED_CLASS", SCOPE),
    ("E15", "entry", "regex", r"sched_class\s*=[^=]", SCOPE),     # assignments of a task's class
    ("E16", "entry", "word", "wake_up_state", SCOPE),
    ("E17", "entry", "word", "resched_curr", SCOPE),
    ("E18", "entry", "word", "schedule", SCOPE),
    ("E19", "entry", "word", "dup_task_struct", SCOPE),
    ("E20", "entry", "word", "kernel_clone", SCOPE),
    ("E21", "entry", "word", "kernel_thread", SCOPE),
    ("L01", "list", "word", "list_header_add_to_head", SCOPE),
    ("L02", "list", "word", "list_header_add_to_tail", SCOPE),
    ("L03", "list", "word", "list_header_remove_head", SCOPE),
    ("L04", "list", "word", "list_header_remove_tail", SCOPE),
    ("L05", "list", "word", "list_header_delete_node", SCOPE),
    ("L06", "list", "word", "list_header_contains", SCOPE),
    ("L07", "list", "word", "list_header_is_empty", SCOPE),
    ("L08", "list", "word", "INIT_LIST_HEADER_S", SCOPE),
    ("L09", "list", "word", "list_add_to_prev", SCOPE),
    ("L10", "list", "word", "list_add_to_next", SCOPE),
    ("L11", "list", "word", "list_header_foreach", SCOPE),
]
# tree-wide users of the generic list primitives are counted, but only hits in these files are
# classified one by one (the scheduler queue is reached only through F01/F04 lines)
LIST_DETAIL_FILES = ("mykernel/lib/list/double_list.h", "mykernel/sched/")
# helpers/macros the five chains need; a missing definition is reported as unlocated
DEFINITIONS = ["DEFINE_SCHED_CLASS", "for_each_class", "per_cpu", "cpu_rq", "this_rq", "get_current",
               "need_resched", "test_tsk_need_resched", "set_tsk_need_resched", "container_of", "WRITE_ONCE",
               "list_header_foreach", "INIT_LIST_S", "INIT_LIST_HEADER_S", "LIST_HEAD_INIT", "list_del_init",
               "__list_add_between", "__list_add_valid", "TASK_RUNNING", "TASK_WAKING", "TASK_NEW", "TASK_NORMAL",
               "TASK_UNINTERRUPTIBLE", "BUG_ON", "RR_TIMESLICE", "runqueues", "smp_processor_id", "select_task_rq",
               "task_is_running", "context_switch", "switch_to", "__schedule_loop", "set_special_state",
               "del_timer_sync", "timer_delete_sync", "list_is_empty_entry",
               # names that appear only in comments/extern declarations; expected to stay unlocated
               "__set_task_cpu", "myos_wake_up_new_task", "fair_sched_class", "idle_sched_class", "rt_sched_class",
               "dl_sched_class", "stop_sched_class", "HAVE_ARCH_BUG_ON", "ttwu_queue", "ttwu_state_match"]
# known -D macros of mykernel/scripts/options_flags.cmake (CMAKE_C_FLAGS); DEBUG/RELEASE depend on build type
CMAKE_DEFINES = ["__KERNEL__", "__x86_64__", "CONFIG_FLATMEM", "CONFIG_NR_CPUS", "CONFIG_64BIT",
                 "CONFIG_PHYS_ADDR_T_64BIT", "CONFIG_ZONE_DMA", "CONFIG_ZONE_DMA32", "CONFIG_SLUB",
                 "CONFIG_ARCH_HAS_SYSCALL_WRAPPER", "CONFIG_BUG", "GRUB2_BOOTUP_SUPPORT",
                 "CONFIG_HYPERVISOR_GUEST", "CONFIG_KVM_GUEST"]
BUILD_TYPE_DEPENDENT = ["DEBUG", "RELEASE"]
CLASS_RULES = [
    "declaration_or_comment: every match on the line is inside a comment, or the code part is a declaration "
    "(extern, prototype ending in ';', struct/union member) or a commented-out line",
    "visible_active_text: a match in code that is not a declaration and has no enclosing unresolved "
    "preprocessor condition (include guards ignored; conditions on CMAKE_DEFINES count as resolved)",
    "conditional_unresolved: as visible_active_text, but at least one enclosing #if/#ifdef/#ifndef/#else "
    "depends on a macro that is not in CMAKE_DEFINES (e.g. DEBUG, file-local defines)",
    "unlocated: a needed definition with no definition site found by this script (see definitions[])",
]


def git(*args):
    p = subprocess.run(["git", "-C", REPO] + list(args), capture_output=True)
    return p


def show(path, cache={}):
    if path not in cache:
        p = git("show", "%s:%s" % (PIN, path))
        cache[path] = p.stdout.decode("utf-8", "replace") if p.returncode == 0 else None
    return cache[path]


def lex(text):
    """Return (code, mask): code has comments and literal contents blanked (newlines kept);
    mask[i] is True where text[i] is inside a comment."""
    out, mask = [], []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i)); mask.extend([True] * (j - i)); i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            seg = text[i:j]
            out.append("".join(ch if ch == "\n" else " " for ch in seg)); mask.extend([True] * (j - i)); i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + " " * (j - i - 2) + (text[j - 1] if j - i >= 2 else "")); mask.extend([False] * (j - i)); i = j
        else:
            out.append(c); mask.append(False); i += 1
    return "".join(out), mask


IDENT = re.compile(r"[A-Za-z_]\w*")
# attribute-like macros that may precede or follow a function name in a definition header
ATTRIBUTE_MACROS = {"__printf", "__releases", "__acquires", "__must_hold", "__aligned", "__attribute__",
                    "__section", "__scanf", "__cond_acquires", "__cond_releases"}
DIRECTIVE = re.compile(r"^\s*#\s*(\w*)(.*)$")      # any preprocessor line; only conditionals change the stack


def analyse(path):
    """Per file: lines, comment mask per line, cpp condition stack per line, top-level symbol spans."""
    text = show(path)
    if text is None:
        return None
    code, mask = lex(text)
    lines, clines = text.split("\n"), code.split("\n")
    lmask, pos = [], 0
    for ln in lines:
        lmask.append(mask[pos:pos + len(ln)]); pos += len(ln) + 1
    stack, cpp, guard = [], [], None
    first_directives = []
    depth, header, header_start, spans, cur = 0, "", None, [], None
    i = 0
    while i < len(clines):
        cl = clines[i]
        m = DIRECTIVE.match(cl)
        if m:
            kind, rest = m.group(1), m.group(2).strip()
            if len(first_directives) < 2:
                first_directives.append((kind, rest, i))
                if len(first_directives) == 2 and first_directives[0][0] == "ifndef" and first_directives[1][0] == "define" \
                        and first_directives[1][1].split()[:1] == [first_directives[0][1]]:
                    guard = first_directives[0][2]
                    for e in stack:                       # the include guard opened one directive earlier
                        if e["line"] == guard + 1:
                            e["guard"] = True
            if kind in ("if", "ifdef", "ifndef"):
                stack.append({"line": i + 1, "text": ("#%s %s" % (kind, rest)).strip(), "guard": False})
            elif kind in ("elif", "else") and stack:
                stack[-1] = dict(stack[-1], text="#%s %s (after %s)" % (kind, rest, stack[-1]["text"].split(" (after")[0]))
            elif kind == "endif" and stack:
                stack.pop()
            cpp.append([dict(s) for s in stack])
            while cl.rstrip().endswith("\\") and i + 1 < len(clines):   # macro continuation lines
                i += 1; cl = clines[i]; cpp.append([dict(s) for s in stack])
            i += 1
            continue
        cpp.append([dict(s) for s in stack])
        for ch in cl:
            if ch == "{":
                if depth == 0:
                    cur = {"start": header_start if header_start is not None else i + 1, "brace_line": i + 1,
                           "header": header + " " + cl.split("{")[0]}
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0 and cur:
                    cur["end"] = i + 1; spans.append(cur); cur = None; header, header_start = "", None
            elif ch == ";" and depth == 0:
                header, header_start = "", None
        if depth == 0 and cl.strip() and not cl.strip().endswith((";", "}")):
            if header_start is None:
                header_start = i + 1
            header += " " + cl.strip()
        elif depth == 0 and cl.strip().endswith((";", "}")):
            header, header_start = "", None
        i += 1
    syms = []
    for s in spans:
        h = re.sub(r"\s+", " ", s["header"]).strip()
        name, kind = None, "block"
        if "=" in h.split("(")[0] or (h.endswith("=") or "= " in h and "(" not in h.split("=")[0]):
            left = h.split("=")[0].strip()
            mm = re.search(r"(\w+\s*\([^()]*\)|\w+)\s*(__\w+\([^()]*\)\s*)*$", left)
            name, kind = (mm.group(1).replace(" ", "") if mm else left), "initializer"
        elif "(" in h:
            cands = [m.group(1) for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", h) if m.group(1) not in ATTRIBUTE_MACROS]
            name, kind = (cands[0] if cands else h), "function"
        elif re.match(r"^(typedef\s+)?(struct|union|enum)\b", h):
            mm = re.match(r"^(?:typedef\s+)?(struct|union|enum)\s*(\w*)", h)
            name, kind = ("%s %s" % (mm.group(1), mm.group(2))).strip(), "type"
        syms.append({"name": name, "kind": kind, "start": s["start"], "brace_line": s["brace_line"], "end": s["end"]})
    return {"lines": lines, "clines": clines, "mask": lmask, "cpp": cpp, "syms": syms}


def enclosing(a, ln):
    best = None
    for s in a["syms"]:
        if s["start"] <= ln <= s["end"] and (best is None or s["start"] >= best["start"]):
            best = s
    return best


def file_scope_declaration(a, ln):
    """True when the file-scope statement containing line ln is an extern/prototype/typedef/forward
    declaration (ends with ';' before any '{' and is not an initialised definition)."""
    cl = a["clines"]
    i = ln - 1
    while i > 0:
        prev = cl[i - 1].strip()
        if prev.endswith((";", "}", "{")) or DIRECTIVE.match(cl[i - 1]) or not prev:
            break
        i -= 1
    stmt, j = "", i
    while j < len(cl) and not DIRECTIVE.match(cl[j]):
        stmt += " " + cl[j].strip()
        if ";" in cl[j] or "{" in cl[j]:
            break
        j += 1
    stmt = stmt.strip()
    if "{" in stmt.split(";")[0]:
        return False
    if not stmt.endswith(";"):
        return False
    return stmt.startswith(("extern ", "typedef ", "struct ", "union ", "enum ")) or ("(" in stmt and "=" not in stmt)


def cond_resolution(entry):
    names = set(IDENT.findall(entry["text"])) - {"if", "ifdef", "ifndef", "elif", "else", "defined", "after", "endif"}
    if not names:
        return "resolved"
    if names <= set(CMAKE_DEFINES):
        return "resolved_by_cmake_defines"
    return "unresolved:" + ",".join(sorted(names - set(CMAKE_DEFINES)))


def classify(a, ln, pattern, mode):
    raw, code, msk = a["lines"][ln - 1], a["clines"][ln - 1], a["mask"][ln - 1]
    rx = re.compile(r"(?<!\w)%s(?!\w)" % re.escape(pattern)) if mode == "word" else re.compile(pattern if mode == "regex" else re.escape(pattern))
    spans = [(m.start(), m.end()) for m in rx.finditer(raw)]
    in_comment = bool(spans) and all(all(msk[k] for k in range(s, min(e, len(msk)))) for s, e in spans)
    sym = enclosing(a, ln)
    conds = [c for c in a["cpp"][ln - 1] if not c["guard"]]
    cstate = [dict(c, resolution=cond_resolution(c)) for c in conds]
    cs = code.strip()
    if in_comment or not cs:
        kind = "comment"
    elif re.match(r"^#\s*define\b", cs):
        kind = "macro_definition"
    elif sym is None and file_scope_declaration(a, ln):
        kind = "declaration"
    elif sym is not None and sym["kind"] == "type":
        kind = "declaration"
    elif sym is not None and sym["kind"] == "function" and sym["name"] == pattern and ln <= sym["brace_line"] and "(" in cs:
        kind = "definition_header"
    elif sym is None and ln < len(a["lines"]) and re.search(r"(?<!\w)%s\s*\(" % re.escape(pattern), cs) and not cs.endswith(";"):
        nxt = enclosing(a, ln + 1)
        kind = "definition_header" if nxt and nxt["name"] == pattern else "active"
    else:
        kind = "active"
    if kind in ("comment", "declaration"):
        cls = "declaration_or_comment"
    elif any(c["resolution"].startswith("unresolved") for c in cstate):
        cls = "conditional_unresolved"
    else:
        cls = "visible_active_text"
    follows_call = bool(re.search(r"(?<!\w)%s\s*\(" % re.escape(pattern), code)) if mode == "word" else None
    return {"in_comment": in_comment, "kind": kind, "class": cls, "symbol": sym["name"] if sym else None,
            "symbol_kind": sym["kind"] if sym else None, "symbol_lines": [sym["start"], sym["end"]] if sym else None,
            "cpp_conditions": cstate, "call_syntax": follows_call}


ALIAS = re.compile(r"^\s*(?:const\s+)?[A-Za-z_]\w*\s*\*\s*([A-Za-z_]\w*)\s*=\s*(&\s*\(?[^;]+?\)?)\s*;")
MACRO_ALIAS = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*$")


def run_query(q, files_cache):
    qid, group, mode, pat, scope = q
    flag = {"word": ["-w", "-F"], "regex": ["-E"], "fixed": ["-F"]}[mode]
    argv = ["git", "grep", "-n", "-I", "--no-color"] + flag + ["-e", pat, PIN, "--", scope]
    p = git(*argv[1:])
    out = p.stdout.decode("utf-8", "replace")
    rows = []
    for line in out.split("\n"):
        if not line:
            continue
        _, path, ln, text = line.split(":", 3)
        rows.append((path, int(ln), text))
    res = {"id": qid, "group": group, "mode": mode, "pattern": pat, "scope": scope, "argv": argv,
           "returncode": p.returncode, "returncode_meaning": {0: "hits", 1: "no match"}.get(p.returncode, "error"),
           "stderr": p.stderr.decode("utf-8", "replace").strip(), "lines": len(rows),
           "files": len({r[0] for r in rows}), "hits": [], "not_detailed": []}
    for path, ln, text in rows:
        if group == "list" and not path.startswith(LIST_DETAIL_FILES):
            res["not_detailed"].append("%s:%d" % (path, ln))
            continue
        a = files_cache.setdefault(path, analyse(path))
        if a is None:
            res["hits"].append({"path": path, "line": ln, "text": text.strip(), "class": "unlocated", "note": "file not readable at pin"})
            continue
        h = {"path": path, "line": ln, "text": text.strip()[:220]}
        h.update(classify(a, ln, pat, mode))
        m = ALIAS.match(a["clines"][ln - 1])
        if m and not h["in_comment"]:
            h["local_alias"] = {"alias": m.group(1), "of": re.sub(r"\s+", " ", a["lines"][ln - 1].split("=", 1)[1].strip().rstrip(";"))}
        m = MACRO_ALIAS.match(a["clines"][ln - 1])
        if m:
            h["macro_alias"] = {"name": m.group(1), "expands_to": m.group(2)}
        res["hits"].append(h)
    return res


def find_definitions(name, files_cache):
    p = git("grep", "-n", "-I", "--no-color", "-w", "-F", "-e", name, PIN, "--", SCOPE)
    sites = []
    for line in p.stdout.decode("utf-8", "replace").split("\n"):
        if not line:
            continue
        _, path, ln, text = line.split(":", 3)
        ln = int(ln)
        a = files_cache.setdefault(path, analyse(path))
        if a is None:
            continue
        code = a["clines"][ln - 1]
        conds = [dict(c, resolution=cond_resolution(c)) for c in a["cpp"][ln - 1] if not c["guard"]]
        if re.match(r"^\s*#\s*define\s+%s\b" % re.escape(name), code):
            sites.append({"path": path, "line": ln, "form": "macro", "text": text.strip()[:200], "cpp_conditions": conds})
            continue
        s = enclosing(a, ln)
        nxt = enclosing(a, ln + 1) if ln < len(a["lines"]) else None
        for cand in (s, nxt):
            if cand and cand["name"] == name and cand["start"] <= ln <= cand["brace_line"]:
                sites.append({"path": path, "line": ln, "form": cand["kind"], "symbol_lines": [cand["start"], cand["end"]],
                              "text": text.strip()[:200], "cpp_conditions": conds})
                break
        else:
            if re.search(r"\bDEFINE_PER_CPU\w*\s*\([^,]+,\s*%s\s*\)" % re.escape(name), code):
                sites.append({"path": path, "line": ln, "form": "per_cpu_variable", "text": text.strip()[:200], "cpp_conditions": conds})
    uniq = {(s["path"], s["line"]): s for s in sites}
    return {"name": name, "grep_returncode": p.returncode, "sites": list(uniq.values()),
            "status": "located" if uniq else "unlocated"}


def main(out):
    files = {}
    res = {"check": "D01_TARGETED_INDEX", "pin_time": PIN, "scope": SCOPE,
           "git_version": subprocess.run(["git", "--version"], capture_output=True).stdout.decode().strip(),
           "python_version": sys.version.split()[0],
           "method": "git grep on the commit object; files read with git show <pin>:<path>; nothing checked out, compiled or run",
           "classification_rules": CLASS_RULES, "cmake_defines_assumed": CMAKE_DEFINES,
           "build_type_dependent_macros": BUILD_TYPE_DEPENDENT,
           "list_detail_files": list(LIST_DETAIL_FILES),
           "limit_note": "direct text search only: an alias, macro expansion, function pointer or computed access that does "
                         "not spell the searched token is not covered; absence of a hit is not absence of a writer or caller",
           "queries": [run_query(q, files) for q in QUERIES]}
    res["definitions"] = [find_definitions(n, files) for n in DEFINITIONS]
    seeds = {q[3] for q in QUERIES if q[1] == "entry" and q[2] == "word"} | {q[3] for q in QUERIES if q[1] == "list"}
    edges = []
    for q in res["queries"]:
        if q["pattern"] not in seeds:
            continue
        for h in q["hits"]:
            if h.get("class") in ("visible_active_text", "conditional_unresolved") and h["kind"] == "active":
                edges.append({"callee": q["pattern"], "caller_symbol": h["symbol"], "path": h["path"], "line": h["line"],
                              "edge": "call" if h["call_syntax"] else "reference", "class": h["class"], "text": h["text"]})
    res["direct_edges"] = edges
    res["aliases"] = {"local": sorted({(h["path"], h["line"], h["local_alias"]["alias"], h["local_alias"]["of"])
                                       for q in res["queries"] for h in q["hits"] if "local_alias" in h}),
                      "macro": sorted({(h["path"], h["line"], h["macro_alias"]["name"], h["macro_alias"]["expands_to"])
                                       for q in res["queries"] for h in q["hits"] if "macro_alias" in h})}
    cnt = {}
    for q in res["queries"]:
        for h in q["hits"]:
            cnt[h["class"]] = cnt.get(h["class"], 0) + 1
    res["counts"] = {"queries": len(res["queries"]), "query_errors": [q["id"] for q in res["queries"] if q["returncode"] not in (0, 1)],
                     "queries_without_hits": [q["id"] for q in res["queries"] if q["lines"] == 0],
                     "lines_total": sum(q["lines"] for q in res["queries"]),
                     "hits_detailed": sum(len(q["hits"]) for q in res["queries"]),
                     "hits_not_detailed_list_users": sum(len(q["not_detailed"]) for q in res["queries"]),
                     "by_class": dict(sorted(cnt.items())), "files_read": len(files),
                     "definitions_unlocated": [d["name"] for d in res["definitions"] if d["status"] == "unlocated"],
                     "direct_edges": len(edges)}
    text = json.dumps(res, ensure_ascii=False, indent=1, sort_keys=False)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(out, "w", encoding="utf-8") as f:
        f.write(text + "\n")
    print(json.dumps(res["counts"], ensure_ascii=False))
    return 1 if res["counts"]["query_errors"] else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
