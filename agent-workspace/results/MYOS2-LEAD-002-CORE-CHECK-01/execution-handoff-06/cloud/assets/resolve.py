"""Scratch helper (not delivered): resolve anchor specs to verbatim line ranges at the time pin."""
import re
import sys
sys.dont_write_bytecode = True
sys.path.insert(0, "/home/user/MyOS2/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/mm-baseline-04/scripts")
import mm_facts_check as M  # noqa: E402

S = M.S
SYS = re.compile(r"MYOS_SYSCALL_DEFINE\d\s*\(\s*(\w+)")


ORIG_SPAN_NAME = M.span_name


def uv_span_name(a, x):
    n = ORIG_SPAN_NAME(a, x)
    if x["kind"] == "function" and re.match(r"MYOS_SYSCALL_DEFINE\d$", n):
        head = " ".join(a["clines"][x["start"] - 1:x["brace_line"]])
        m = SYS.search(head)
        if m:
            return "sys_" + m.group(1)
    return n


def resolve(spec):
    aid, path, sym, kind, needle, n = spec[:6]
    occ = spec[6] if len(spec) > 6 else 1
    text = S.show(path)
    if text is None:
        raise SystemExit("unreadable %s" % path)
    lines = text.split("\n")
    if kind == "build":
        cands = [i + 1 for i, l in enumerate(lines) if needle in l]
    elif kind == "macro":
        cands = [i + 1 for i, l in enumerate(lines) if re.match(r"^\s*#\s*define\s+%s\b" % re.escape(sym), l)]
    else:
        a = S.analyse(path)
        want = "type" if kind == "type_member" else "function"
        spans = [x for x in a["syms"] if uv_span_name(a, x) == sym and x["kind"] == want]
        if not spans:
            raise SystemExit("%s: no span %s %s in %s" % (aid, want, sym, path))
        cands = []
        for x in spans:
            for ln in range(x["brace_line"] + 1, x["end"]):
                if lines[ln - 1].strip().startswith(needle.strip()):
                    cands.append(ln)
    if len(cands) < occ:
        raise SystemExit("%s: needle not found (%d cands): %r" % (aid, len(cands), needle))
    s = cands[occ - 1]
    e = s + n - 1
    quote = "\n".join(lines[s - 1:e])
    an = {"id": aid, "path": path, "symbol": sym, "kind": kind, "lines": [s, e], "quote": quote}
    if kind != "build":
        a = S.analyse(path)
        an["cpp_conditions"] = M.cpp_at(a, s)
    else:
        an["cpp_conditions"] = []
    return an, len(cands)


M.span_name = uv_span_name   # syscall bodies get sys_<name> everywhere (check_anchor, run_query)
