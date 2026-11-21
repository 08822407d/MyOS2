# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-USER-RETURN-SIGNAL-09 (user-return-signal-09)
# executor: Claude Code desktop app, local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd
#   (MYOS2-A-CC02); model: claude-opus-5-5 as declared by the runtime (not independently attestable)
# source: new file. Its anchor/tag/query/body-regex routines are copied and trimmed from
#   ../user-syscall-stdio-08/scripts/ss_check.py (same session); that script is neither imported nor
#   run, so the 08 check is not repeated. The boundary mode is rewritten for RC08-01: it compares
#   full git object ids internally, records explicit booleans, and exits non-zero on any violation.
#   Read-only reuse (import, no copy) of show()/analyse()/enclosing() from
#   ../scheduler-integration-03/scripts/scan_index.py, REPO rebound to the given clone root.
#   Bytecode writing is disabled first.
# purpose: mechanical consistency check of this package, not a semantic proof:
#   (1) anchors: 1-5 verbatim lines at the time pin inside the named function / initializer / type /
#       #define (with continuation lines) / asm range, or build text naming the symbol; recorded #if
#       conditions = computed non-guard stack; the free-text role only gets format checks (not empty,
#       not "markdown", no empty or dangling brackets / list separators, paired full-width brackets,
#       even backticks, no role a proper prefix of another) - its meaning is reviewed, not checked;
#   (2) "[VERIFIED path::symbol]" blocks in user-return-signal.md and stdio-08-disposition.md;
#   (3) queries re-run with git grep at the pin (return code, total and active hit counts); every
#       hit reference must be an active hit of its query, and a reference that carries "expect" must
#       also contain that text in the hit line (guards against citing a neighbouring line);
#   (4) structure checks: active-regex counts in one C function body;
#   (5) value tables recomputed from source text (#define values and membership in the active
#       syscall table);
#   (6) structure of facts-and-boundaries.yaml, return-paths.dot (same IDs as the facts) and
#       deferred-findings.yaml (references incl. 05/07/08 IDs read from the base commit, limits, counts,
#       candidates vs covers);
#   (7) boundary: full-object comparison of the base tree with the index, additions only under the
#       allowed prefix, nothing unstaged/untracked, remote execution head unchanged; exit 2 otherwise;
#   (8) no 40-hex run in the package text or in the outputs.
# It never checks out, fetches, compiles or runs repository code.
# ---------------------------------------------------------------------------------------------------
"""Usage: PYTHONDONTWRITEBYTECODE=1 python3 -B -P rs_check.py <repo_root> check <package_dir> <out.json>
       PYTHONDONTWRITEBYTECODE=1 python3 -B -P rs_check.py <repo_root> boundary <out.json> --staged
       PYTHONDONTWRITEBYTECODE=1 python3 -B -P rs_check.py <repo_root> boundary <out.json> --commit <rev>
(queries.json is written next to <out.json> in check mode; boundary exits 2 on any violated condition)"""
import sys
sys.dont_write_bytecode = True

import importlib.util  # noqa: E402
import json  # noqa: E402
import os  # noqa: E402
import re  # noqa: E402
import subprocess  # noqa: E402

import yaml  # noqa: E402

PIN = "a039d9803ade"
BASE = "8983a9235c8a"                    # reviewed execution head (08 reviewed); all its files frozen
EXEC = "claude/dazzling-cori-q0dnyt"
PKG = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
PREFIX = PKG + "user-return-signal-09/"
SELF = "CORE-USER-RETURN-SIGNAL-09::"
EXT = {"CORE-USER-VFS-BASELINE-05": [PKG + "user-vfs-baseline-05/facts-and-dependencies.yaml",
                                     PKG + "user-vfs-baseline-05/deferred-findings.yaml"],
       "CORE-USER-BOUNDARIES-07": [PKG + "user-boundaries-07/facts-and-boundaries.yaml",
                                   PKG + "user-boundaries-07/deferred-findings.yaml"],
       "CORE-USER-SYSCALL-STDIO-08": [PKG + "user-syscall-stdio-08/facts-and-boundaries.yaml",
                                      PKG + "user-syscall-stdio-08/deferred-findings.yaml"]}
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
TAG = re.compile(r"\[VERIFIED ([^\]:]+)::([^\]]+)\]\s*\n```[\w+-]*\n(.*?)\n```", re.S)
SYS = re.compile(r"MYOS_SYSCALL_DEFINE\d\s*\(\s*(\w+)")
ASM_START = re.compile(r"^\s*SYM_(?:CODE|FUNC)_START\((\w+)\)")
ASM_BREAK = re.compile(r"^\s*(?:SYM_(?:CODE|FUNC)_START\(|\.pushsection|\.popsection)")
KINDS = {"in_body", "initializer", "type_member", "macro", "asm_range", "build"}
THEMES = ("D00", "D01", "D02", "D03")
ROW_SECTIONS = ("return_frame_transitions", "frame_fields", "restart_conditions", "fact_notes")
LIMITS = {"capabilities": 15, "edges": 40, "anchors": 75, "candidates": 4}    # ceilings from contract 23
F_STATUS = {"deferred_owner_not_ready", "needs_evidence", "superseded_interpretation"}
F_KINDS = {"source_fact", "conditional_inference", "limited_host_observation", "policy_proposal",
           "evidence_gap", "documentation_erratum"}
GAP_STATUS = {"open", "narrowed", "closed_static"}
EDGE_KINDS = {"call", "data", "init_order", "config", "build"}
EDGE_STATUS = {"connected", "connected_with_concern", "conditional", "blocked", "endpoint", "unknown"}
REPO = None
S = None
COMMANDS = []


def load_scan_index(repo):
    p = os.path.join(repo, PKG, "scheduler-integration-03", "scripts", "scan_index.py")
    spec = importlib.util.spec_from_file_location("scan_index", p)
    mod = importlib.util.module_from_spec(spec)
    sys.modules["scan_index"] = mod
    spec.loader.exec_module(mod)
    frozen = mod.REPO
    mod.REPO = repo
    return mod, frozen


def git(*args, check=False):
    COMMANDS.append(" ".join(["git"] + list(args)))
    p = subprocess.run(["git", "-C", REPO] + list(args), capture_output=True)
    if check and p.returncode != 0:
        raise SystemExit("git %s failed rc=%d" % (" ".join(args), p.returncode))
    return p


def span_name(a, x):
    n = x["name"]
    if x["kind"] == "function" and re.match(r"MYOS_SYSCALL_DEFINE\d$", n or ""):
        m = SYS.search(" ".join(a["clines"][x["start"] - 1:x["brace_line"]]))
        if m:
            return "sys_" + m.group(1)
    return n


def active(a, ln):
    return "".join(c if not m else " " for c, m in zip(a["lines"][ln - 1], a["mask"][ln - 1]))


def asm_range(lines, sym):
    """(start, end, bound) of an assembly symbol: from SYM_*_START(sym) to SYM_*_END(sym) when present,
    otherwise to the line before the next symbol start or section switch (bound = next_symbol)."""
    start = next((i + 1 for i, l in enumerate(lines) if (m := ASM_START.match(l)) and m.group(1) == sym), None)
    if start is None:
        return None
    end_re = re.compile(r"^\s*SYM_(?:CODE|FUNC)_END\(%s\)" % re.escape(sym))
    for i in range(start, len(lines)):
        if end_re.match(lines[i]):
            return start, i + 1, "explicit_end"
        if ASM_BREAK.match(lines[i]):
            return start, i, "next_symbol"
    return start, len(lines), "end_of_file"


def in_span(a, sym, kind, s, e):
    if kind == "asm_range":
        r = asm_range(a["lines"], sym)
        return (r is not None and r[0] < s and e <= r[1]), (list(r) if r else None)
    want = {"in_body": "function", "initializer": "initializer", "type_member": "type"}.get(kind)
    spans = [x for x in a["syms"] if x["kind"] == want and span_name(a, x) == sym and x["brace_line"] < s and e <= x["end"]]
    return bool(spans), ([spans[0]["start"], spans[0]["brace_line"], spans[0]["end"]] if spans else None)


def check_anchor(an):
    res = {"id": an["id"], "path": an["path"], "symbol": an["symbol"], "kind": an["kind"], "ok": False}
    if an["kind"] not in KINDS:
        res["reason"] = "unknown kind"
        return res
    text = S.show(an["path"])
    if text is None:
        res["reason"] = "file not readable at pin"
        return res
    lines = text.split("\n")
    q = an["quote"].split("\n")
    s, e = an["lines"]
    if not 1 <= len(q) <= 5 or e - s + 1 != len(q) or lines[s - 1:e] != q:
        res["reason"] = "quote is not the verbatim lines %d-%d (or not 1-5 lines)" % (s, e)
        return res
    if an["kind"] == "build":
        named = re.search(r"(?<![\w])(?:-D)?%s(?![\w])" % re.escape(an["symbol"]), "\n".join(q)) is not None
        res["ok"] = named and an.get("cpp_conditions", []) == []
        res["reason"] = None if res["ok"] else "symbol not named in the build text"
        return res
    a = S.analyse(an["path"])
    cpp = [x["text"] for x in a["cpp"][s - 1] if not x.get("guard")]
    res["cpp_computed"] = cpp
    if cpp != an.get("cpp_conditions", []):
        res["reason"] = "recorded #if conditions differ from computed %s" % cpp
        return res
    has_active = any(active(a, ln).strip() for ln in range(s, e + 1))
    if an["kind"] == "macro":
        ok = re.match(r"^\s*#\s*define\s+%s\b" % re.escape(an["symbol"]), q[0]) is not None
        for k in range(s, e):
            ok = ok and lines[k - 1].rstrip().endswith("\\")
        res["ok"] = ok
        res["reason"] = None if ok else "not one #define %s with continuation lines" % an["symbol"]
        return res
    ok, span = in_span(a, an["symbol"], an["kind"], s, e)
    res["span"] = span
    res["ok"] = ok and has_active
    if not ok:
        res["reason"] = "lines not inside %s %s" % (an["kind"], an["symbol"])
    elif not has_active:
        res["reason"] = "every quoted line is comment"
    return res


def check_md_tags(path):
    rows = []
    for m in TAG.finditer(open(path, encoding="utf-8").read()):
        p, sym, body = m.group(1).strip(), m.group(2).strip(), m.group(3)
        q = body.split("\n")
        row = {"tag": "%s::%s" % (p, sym), "quote_lines": len(q), "ok": False}
        text = S.show(p)
        if text is None:
            row["reason"] = "file not readable"
            rows.append(row)
            continue
        lines = text.split("\n")
        hits = [i + 1 for i in range(len(lines) - len(q) + 1) if lines[i:i + len(q)] == q]
        if p.endswith(".S"):
            r = asm_range(lines, sym)
            good = [h for h in hits if r and r[0] < h and h + len(q) - 1 <= r[1]]
        elif p.endswith((".c", ".h")):
            a = S.analyse(p)
            good = [h for h in hits for x in a["syms"]
                    if span_name(a, x) == sym and x["brace_line"] < h and h + len(q) - 1 <= x["end"]]
            if not good:
                good = [h for h in hits if re.match(r"^\s*#\s*define\s+%s\b" % re.escape(sym), lines[h - 1])]
        else:                                   # build text: the symbol must be named in the block
            good = hits if re.search(r"(?<![\w])(?:-D)?%s(?![\w])" % re.escape(sym), body) else []
        row.update(found_at=sorted(set(hits)), ok=bool(good) and 1 <= len(q) <= 5)
        if not row["ok"]:
            row["reason"] = "not verbatim consecutive lines inside %s" % sym
        rows.append(row)
    return rows


def run_query(q):
    cmd = ["grep"] + q["args"] + [PIN, "--"] + q["paths"]
    r = git(*cmd)
    hits = []
    for ln in r.stdout.decode("utf-8", "replace").split("\n"):
        m = re.match(r"^%s:([^:]+):(\d+):(.*)$" % PIN, ln)
        if not m:
            continue
        path, line, text = m.group(1), int(m.group(2)), m.group(3)
        if path.endswith((".c", ".h", ".S")):
            a = S.analyse(path)
            act = bool(active(a, line).strip())
            enc = S.enclosing(a, line)
            encn = span_name(a, enc) if enc and not path.endswith(".S") else None
        elif path.endswith((".cmake", "CMakeLists.txt", ".sh")):
            act, encn = not text.lstrip().startswith("#"), None     # '#' comment lines in CMake/shell text
        else:
            act, encn = True, None
        hits.append({"path": path, "line": line, "text": text.strip()[:200], "active": act, "enclosing": encn})
    shown = " ".join(["git", "grep"] + q["args"] + ["<time>", "--"] + q["paths"])
    return {"id": q["id"], "command": shown, "returncode": r.returncode, "tool_failure": r.returncode >= 2,
            "stderr": r.stderr.decode()[:200], "hits_total": len(hits), "hits_active": sum(h["active"] for h in hits),
            "hits": hits}


def func_body(a, sym, near=None):
    """active text of the body of the one function span named sym (the one containing line `near` if given)."""
    spans = [x for x in a["syms"] if x["kind"] == "function" and span_name(a, x) == sym
             and (near is None or x["start"] <= near <= x["end"])]
    if len(spans) != 1:
        return None, len(spans)
    x = spans[0]
    body = [active(a, ln) for ln in range(x["brace_line"], x["end"] + 1)]
    head = a["lines"][x["brace_line"] - 1]
    body[0] = "".join(c if not m else " " for c, m in zip(head, a["mask"][x["brace_line"] - 1]))[head.find("{") + 1:]
    tail = body[-1]
    body[-1] = tail[:tail.rfind("}")] if "}" in tail else tail
    return body, [x["start"], x["brace_line"], x["end"]]


def structure_check(sc):
    res = {"id": sc["id"], "kind": sc["kind"], "path": sc["path"], "symbol": sc["symbol"], "ok": False}
    body, span = func_body(S.analyse(sc["path"]), sc["symbol"])
    if body is None:
        res["reason"] = "expected exactly one function span, got %d" % span
        return res
    n = sum(len(re.findall(sc["regex"], t)) for t in body)
    res.update(span=span, regex=sc["regex"], active_matches=n, expected=sc["expected_count"], ok=n == sc["expected_count"])
    return res


def defines(path):
    """object-like #define NAME value on active lines (value = rest of the line without a trailing comment)."""
    a = S.analyse(path)
    out = {}
    for ln in range(1, len(a["lines"]) + 1):
        m = re.match(r"^\s*#\s*define\s+(\w+)\s+(.+?)\s*$", active(a, ln))
        if m:
            out[m.group(1)] = m.group(2)
    return out


def num(v):
    lits = re.findall(r"0[xX][0-9a-fA-F]+|\b\d+\b", v)
    lit = lits[-1]
    return int(lit, 16) if lit.lower().startswith("0x") else int(lit)


def systab():
    """dispatch table recomputed from syscalls_64.h, unistd_64.h and the MYOS_SYSCALL_DEFINEn sites."""
    tab = "mykernel/arch/x86_64/include/asm/syscalls_64.h"
    nrs = {k: int(v) for k, v in defines("mykernel/arch/x86_64/include/asm/unistd_64.h").items()
           if k.startswith("__NR_") and re.match(r"^\d+$", v)}
    a = S.analyse(tab)
    pat = re.compile(r"__SYSCALL\(\s*([^,]+?)\s*,\s*(\w+)\s*\)")
    rows, commented = [], []
    for i in range(1, len(a["lines"]) + 1):
        m = pat.search(active(a, i))
        if m:
            rows.append({"table_line": i, "nr_expr": m.group(1), "sym": m.group(2)})
        elif (mc := pat.search(a["lines"][i - 1])):
            commented.append({"table_line": i, "nr_expr": mc.group(1), "sym": mc.group(2)})

    def value(expr):
        m = re.match(r"^(__NR_\w+)(?:\s*-\s*(\d+))?$", expr)
        return nrs[m.group(1)] - int(m.group(2) or 0) if m and m.group(1) in nrs else None
    defs = {}
    g = git("grep", "-n", "-E", r"MYOS_SYSCALL_DEFINE[0-6]\s*\(\s*\w+", PIN, "--", "mykernel")
    for ln in g.stdout.decode("utf-8", "replace").split("\n"):
        mm = re.match(r"^%s:([^:]+):(\d+):(.*)$" % PIN, ln)
        if not mm or not mm.group(1).endswith((".c", ".h")):
            continue
        path, line = mm.group(1), int(mm.group(2))
        t = active(S.analyse(path), line)
        m2 = re.search(r"MYOS_SYSCALL_DEFINE([0-6])\s*\(\s*(\w+)", t)
        if m2 and not re.match(r"^\s*#\s*define\b", t):          # skip comments and the macro definitions
            defs.setdefault("sys_" + m2.group(2), []).append({"path": path, "line": line, "argc": int(m2.group(1))})
    out = []
    for r in rows:
        dd = defs.get(r["sym"], [])
        row = dict(r, nr=value(r["nr_expr"]), definitions=len(dd))
        if len(dd) == 1:
            body, _ = func_body(S.analyse(dd[0]["path"]), r["sym"], near=dd[0]["line"])
            text = "\n".join(body) if body is not None else ""
            row.update(def_path=dd[0]["path"], def_line=dd[0]["line"], argc=dd[0]["argc"],
                       dummy=bool(re.search(r"\bALERT_DUMMY_SYSCALL\s*\(", text)),
                       returns=len(re.findall(r"\breturn\b", text)))
        out.append(row)
    nums = [r["nr"] for r in out]
    summary = {"entries": len(out), "nr_syscalls": nrs.get("__NR_syscalls"),
               "unique_numbers": len(set(nums)) == len(nums), "all_resolved": None not in nums,
               "all_below_nr_syscalls": None not in nums and all(n < nrs.get("__NR_syscalls", 0) for n in nums),
               "all_defined_once": all(r["definitions"] == 1 for r in out),
               "without_active_return": sorted(r["sym"] for r in out if r.get("returns") == 0),
               "dummy": sorted(r["sym"] for r in out if r.get("dummy"))}
    return {"rows": out, "commented_entries": commented, "summary": summary}


def parse_num(v):
    """value of a simple numeric #define (decimal/hex, optional sign, parentheses, integer suffix)."""
    m = re.fullmatch(r"\(?\s*(-?)\s*(0[xX][0-9a-fA-F]+|\d+)[uUlL]*\s*\)?", (v or "").strip())
    if not m:
        return v
    n = int(m.group(2), 16) if m.group(2).lower().startswith("0x") else int(m.group(2))
    return -n if m.group(1) else n


def vt_defines(t):
    """#define values and membership of __NR_* names in the active syscall table, recomputed."""
    got = {}
    for name, path in (t.get("defines") or {}).items():
        got[name] = parse_num(defines(path).get(name))
    rows = systab()["rows"]
    for name in t.get("table_membership") or []:
        got["in_active_table:" + name] = any(r["nr_expr"] == name for r in rows)
    want = t.get("values") or {}
    diff = sorted(k for k in set(got) | set(want) if got.get(k) != want.get(k))
    return {"values": got, "differs": diff, "ok": not diff}


def ext_ids():
    """every 'id' found in the 05 and 07 facts and findings files at the base commit, qualified."""
    out = set()

    def walk(prefix, x):
        if isinstance(x, dict):
            if isinstance(x.get("id"), str):
                out.add(prefix + "::" + x["id"])
            for v in x.values():
                walk(prefix, v)
        elif isinstance(x, list):
            for v in x:
                walk(prefix, v)
    for prefix, paths in EXT.items():
        for p in paths:
            r = git("show", BASE + ":" + p, check=True)
            walk(prefix, yaml.safe_load(r.stdout.decode("utf-8")))
    return out


def structure(d, f, ext):
    errs, ids = [], {}
    for sec in ("capabilities", "edges", "anchors", "queries", "structure_checks", "value_tables",
                "gap_status", "next_validation_candidates") + ROW_SECTIONS + ("sr08_dispositions",):
        for x in d.get(sec) or []:
            if x["id"] in ids:
                errs.append("duplicate id %s (%s, %s)" % (x["id"], ids[x["id"]], sec))
            ids[x["id"]] = sec
    anc = {a["id"] for a in d["anchors"]}
    qid = {q["id"] for q in d["queries"]}
    scs = {s["id"] for s in d["structure_checks"]}
    vts = {v["id"] for v in d["value_tables"]}
    caps = {c["id"] for c in d["capabilities"]}
    eby = {e["id"]: e for e in d["edges"]}
    fids = {x["id"] for x in f["findings"]} | {x["id"] for x in f.get("errata") or []}

    def ref_ok(ref):
        if ref in anc or ref in qid or ref in scs or ref in vts:
            return True
        if ref.startswith(SELF):
            return ref[len(SELF):] in fids
        if "::" in ref:
            return ref.split("::")[0] not in EXT or ref in ext
        return False

    def ev(owner, ref):
        if isinstance(ref, dict):
            if ref.get("query") not in qid:
                errs.append("%s hit -> missing query %s" % (owner, ref.get("query")))
        elif not ref_ok(ref):
            errs.append("%s -> unresolved evidence %s" % (owner, ref))

    for c in d["capabilities"]:
        for k in ("purpose", "entries", "normal_path", "failure_path", "unknown", "not_covered", "evidence"):
            if not c.get(k):
                errs.append("%s missing %s" % (c["id"], k))
        if c.get("id_status") != "proposed_id" or c.get("runtime_evidence") != "NOT_RUN":
            errs.append("%s id_status/runtime_evidence" % c["id"])
        if c.get("refines") and not ref_ok(c["refines"]):
            errs.append("%s refines %s unresolved" % (c["id"], c["refines"]))
        for r in c["evidence"]:
            ev(c["id"], r)
        for r in c.get("edges") or []:
            if r not in eby:
                errs.append("%s edge %s missing" % (c["id"], r))
        for r in c.get("findings") or []:
            if r not in fids and not ref_ok(r):
                errs.append("%s finding %s missing" % (c["id"], r))
    for e in d["edges"]:
        if e["kind"] not in EDGE_KINDS or e.get("status") not in EDGE_STATUS:
            errs.append("%s kind/status" % e["id"])
        if not e.get("evidence") and "indirect" not in e:
            errs.append("%s has no evidence" % e["id"])
        for r in e.get("evidence") or []:
            ev(e["id"], r)
        for end in ("from", "to"):
            v = e[end]
            if v not in caps and not v.startswith("external:") and not ("::" in v and ref_ok(v)):
                errs.append("%s %s endpoint %s" % (e["id"], end, v))
            if v in caps and e["id"] not in (next(c for c in d["capabilities"] if c["id"] == v).get("edges") or []):
                errs.append("%s not listed by endpoint %s" % (e["id"], v))
        if "indirect" in e:
            for k in ("call_site", "binding", "receiver"):
                refs = e["indirect"].get(k)
                if not refs:
                    errs.append("%s indirect.%s missing" % (e["id"], k))
                for r in refs or []:
                    ev(e["id"] + ".indirect." + k, r)
    for g in d["gap_status"]:
        for k in ("original", "coverage", "evidence", "closed_static_parts", "remaining_unknown", "status_from", "status_to", "runtime"):
            if g.get(k) in (None, "", []) and not (k == "closed_static_parts" and g.get("status_to") == "open"):
                errs.append("%s missing %s" % (g["id"], k))
        if g.get("status_to") not in GAP_STATUS or g.get("runtime") != "NOT_RUN":
            errs.append("%s status_to/runtime" % g["id"])
        if g.get("qualified_id") and not ref_ok(g["qualified_id"]):
            errs.append("%s qualified_id unresolved" % g["id"])
        for r in g.get("evidence") or []:
            ev(g["id"], r)
    for v in d["next_validation_candidates"]:
        if v.get("status") != "NOT_RUN" or not v.get("authorization_needed"):
            errs.append("%s status/authorization" % v["id"])
    for sec in ROW_SECTIONS:
        for x in d.get(sec) or []:
            if not x.get("evidence"):
                errs.append("%s missing evidence" % x["id"])
            for r in x.get("evidence") or []:
                ev(x["id"], r)
            for r in x.get("concerns") or []:
                if r not in fids and not ref_ok(r):
                    errs.append("%s concern %s unresolved" % (x["id"], r))
    for x in d.get("sr08_dispositions") or []:
        if x.get("id") not in ("SR08-01", "SR08-02") or x.get("verdict") not in ("accept", "partial_accept", "dispute"):
            errs.append("%s id/verdict" % x.get("id"))
        if not x.get("locations") or not x.get("evidence"):
            errs.append("%s missing locations/evidence" % x.get("id"))
        for r in x.get("evidence") or []:
            ev(x["id"], r)
        for r in x.get("affected_ids") or []:
            if not ref_ok(r):
                errs.append("%s affected id %s unresolved" % (x["id"], r))
    recount = {"capabilities": len(d["capabilities"]), "edges": len(d["edges"]),
               "edges_indirect": sum(1 for e in d["edges"] if "indirect" in e), "anchors": len(d["anchors"]),
               "anchors_by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(KINDS)},
               "anchors_by_theme": {t: sum(1 for a in d["anchors"] if a["theme"] == t) for t in THEMES},
               "queries": len(d["queries"]), "structure_checks": len(d["structure_checks"]),
               "value_tables": len(d["value_tables"]), "gap_status": len(d["gap_status"]),
               "next_validation_candidates": len(d["next_validation_candidates"]),
               "sr08_dispositions": len(d.get("sr08_dispositions") or [])}
    for sec in ROW_SECTIONS:
        recount[sec] = len(d.get(sec) or [])
    limits = {"capabilities": [len(d["capabilities"]), LIMITS["capabilities"]], "edges": [len(d["edges"]), LIMITS["edges"]],
              "anchors": [len(d["anchors"]), LIMITS["anchors"]],
              "candidates": [len(d["next_validation_candidates"]), LIMITS["candidates"]]}
    if any(v[0] > v[1] for v in limits.values()):
        errs.append("limit exceeded %s" % limits)
    if d.get("counts") != recount:
        errs.append("counts block differs from recount")
    return {"errors": errs, "limits": limits, "recount": recount, "ok": not errs}


def deferred(f, d, ext):
    ferr = []
    anc = {a["id"] for a in d["anchors"]}
    qid = {q["id"] for q in d["queries"]}
    scs = {s["id"] for s in d["structure_checks"]}
    vts = {v["id"] for v in d["value_tables"]}
    if f.get("owner_action_now") != "none" or f.get("owner_decisions_requested_now") != []:
        ferr.append("header owner fields")

    def resolve(owner, refs):
        for r in refs or []:
            if isinstance(r, dict):                       # line hit {query, path, line}; checked in check()
                if r.get("query") not in qid:
                    ferr.append("%s hit -> missing query %s" % (owner, r.get("query")))
                continue
            if r in anc or r in qid or r in scs or r in vts:
                continue
            if "::" in r and (r.split("::")[0] not in EXT or r in ext or r.startswith(SELF)):
                continue
            ferr.append("%s evidence %s unresolved" % (owner, r))

    def item(x, extra=()):
        for k in ("qualified_id", "source_ref", "kind", "status", "owner_action_now", "claim", "premises", "impact",
                  "counter_evidence", "unknown", "evidence", "future_verification", "reopen_conditions", "blocks") + extra:
            if x.get(k) in (None, "", []) and not (k in ("counter_evidence", "unknown") and x.get(k) == []):
                ferr.append("%s missing %s" % (x["id"], k))
        if x["qualified_id"] != SELF + x["id"]:
            ferr.append("%s qualified_id" % x["id"])
        if x["status"] not in F_STATUS or x["owner_action_now"] != "none" or not set(x["kind"]) <= F_KINDS:
            ferr.append("%s status/owner_action_now/kind" % x["id"])
        resolve(x["id"], x.get("evidence"))
        for r in x.get("related") or []:
            if "::" not in r or (r.split("::")[0] in EXT and r not in ext):
                ferr.append("%s related %s not fully qualified or unresolved" % (x["id"], r))

    covers = {}
    for c in d["next_validation_candidates"]:
        for t in c.get("covers") or []:
            if t.startswith("RS-"):
                covers.setdefault(t, set()).add(c["id"])
    ids = {x["id"] for x in f["findings"]}
    for t in sorted(set(covers) - ids):
        ferr.append("candidate covers unknown finding %s" % t)
    for x in f["findings"]:
        item(x)
        got = set((x.get("future_verification") or {}).get("candidates") or [])
        if got != covers.get(x["id"], set()):
            ferr.append("%s candidates %s differ from covers %s" % (x["id"], sorted(got), sorted(covers.get(x["id"], set()))))
    for x in f.get("evidence_appends") or []:
        if x.get("target") not in ext or x.get("status_change") != "none" or not x.get("evidence") or not x.get("added_facts"):
            ferr.append("%s append target/status/evidence/facts" % x.get("target"))
        resolve(x.get("target"), x.get("evidence"))
    quoted = 0
    for x in f.get("errata") or []:
        item(x, ("target", "original_locations", "correction"))
        if x.get("kind") != ["documentation_erratum"] or x.get("target") not in ext:
            ferr.append("%s erratum kind/target" % x["id"])
        for loc in x.get("original_locations") or []:
            p = git("show", BASE + ":" + loc["path"])
            blob = re.sub(r"\s+", " ", p.stdout.decode("utf-8", "replace"))
            quoted += 1
            if p.returncode != 0 or re.sub(r"\s+", " ", loc["text"]) not in blob:
                ferr.append("%s original text not found in %s at %s" % (x["id"], loc["path"], BASE))
    fcount = {"findings": len(f["findings"]), "evidence_appends": len(f.get("evidence_appends") or []),
              "errata": len(f.get("errata") or []),
              "findings_by_status": {s: sum(1 for x in f["findings"] if x["status"] == s) for s in sorted(F_STATUS)}}
    if f.get("counts") != fcount:
        ferr.append("deferred-findings counts differ from recount")
    return {"errors": ferr, "recount": fcount, "errata_quotes_checked": quoted, "ok": not ferr}


def dot_check(pkg, d):
    """return-paths.dot must use the same IDs as the facts: every facts edge appears once with the same
    endpoints, every capability is a node, and nothing in the graph is absent from the facts."""
    text = open(os.path.join(pkg, "return-paths.dot"), encoding="utf-8").read()
    nodes = set(re.findall(r'^\s*"([^"]+)"\s*\[', text, flags=re.M))
    edges, errs = {}, []
    for m in re.finditer(r'^\s*"([^"]+)"\s*->\s*"([^"]+)"\s*\[([^\]]*)\]', text, flags=re.M):
        a, b, attrs = m.groups()
        mid = re.search(r'\bid\s*=\s*"([^"]+)"', attrs)
        if not mid:
            errs.append("edge %s -> %s has no id" % (a, b))
        elif mid.group(1) in edges:
            errs.append("edge id %s repeated" % mid.group(1))
        else:
            edges[mid.group(1)] = (a, b)
    fe = {e["id"]: (e["from"], e["to"]) for e in d["edges"]}
    errs += ["facts edge %s missing or different in dot" % k for k, v in fe.items() if edges.get(k) != v]
    errs += ["dot edge %s not in facts" % k for k in edges if k not in fe]
    caps = {c["id"] for c in d["capabilities"]}
    errs += ["capability %s is not a dot node" % c for c in sorted(caps - nodes)]
    ends = {x for v in fe.values() for x in v}
    errs += ["dot node %s not in facts" % n for n in sorted(nodes - caps - ends)]
    return {"nodes": len(nodes), "edges": len(edges), "errors": errs, "ok": not errs}


def role_check(anchors):
    """format checks on the free-text role of each anchor (its meaning is not checked mechanically)."""
    errs = []
    roles = {a["id"]: a.get("role") for a in anchors}
    for i, r in roles.items():
        if not isinstance(r, str) or not r.strip() or r.strip() == "markdown":
            errs.append("%s role empty or placeholder" % i)
            continue
        bad = [t for t in ("（）", "（、", "、）", "、、", " ，") if t in r]
        if bad:
            errs.append("%s role has empty or dangling separators %s" % (i, bad))
        if r.lstrip().startswith("|") or r.rstrip().endswith(("| 、", "|")):
            errs.append("%s role looks like a table fragment" % i)
        if r.count("（") != r.count("）"):
            errs.append("%s role has unpaired full-width brackets" % i)
        if r.count("`") % 2:
            errs.append("%s role has an odd number of backticks" % i)
    for i, r in roles.items():
        for j, t in roles.items():
            if i != j and isinstance(r, str) and isinstance(t, str) and t != r and t.startswith(r):
                errs.append("%s role is a proper prefix of %s role" % (i, j))
    return {"total": len(roles), "errors": errs, "ok": not errs}


def check(argv):
    pkg, outp = argv[0], argv[1]
    d = yaml.safe_load(open(os.path.join(pkg, "facts-and-boundaries.yaml"), encoding="utf-8"))
    f = yaml.safe_load(open(os.path.join(pkg, "deferred-findings.yaml"), encoding="utf-8"))
    res = {"check": "USER_RETURN_SIGNAL_09", "pin_time": PIN, "base": BASE}
    an = [check_anchor(a) for a in d["anchors"]]
    res["anchors"] = {"total": len(an), "ok": sum(r["ok"] for r in an),
                      "by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(KINDS)},
                      "failures": [r for r in an if not r["ok"]]}
    tags = []
    per_file = {}
    for name in ("user-return-signal.md", "stdio-08-disposition.md"):
        rows = check_md_tags(os.path.join(pkg, name))
        per_file[name] = [len(rows), sum(r["ok"] for r in rows)]
        tags += rows
    res["markdown_tags"] = {"files": per_file, "total": len(tags), "ok": sum(r["ok"] for r in tags),
                            "failures": [r for r in tags if not r["ok"]]}
    qrecs = [run_query(q) for q in d["queries"]]
    by = {r["id"]: r for r in qrecs}
    qerr = []
    for q in d["queries"]:
        r = by[q["id"]]
        if r["tool_failure"]:
            qerr.append("%s tool failure rc=%s" % (q["id"], r["returncode"]))
            continue
        if (r["returncode"], r["hits_total"], r["hits_active"]) != (q["expected_rc"], q["expected_hits_total"], q["expected_hits_active"]):
            qerr.append("%s rc/total/active %s/%s/%s" % (q["id"], r["returncode"], r["hits_total"], r["hits_active"]))
        if q["expect"] == "negative_active" and r["hits_active"]:
            qerr.append("%s expected no active hit" % q["id"])
        if q["expect"] == "comment_only" and (r["hits_active"] or not r["hits_total"]):
            qerr.append("%s expected comment-only hits" % q["id"])

    def hit_ok(h):
        r = by.get(h["query"])
        return bool(r) and any(x["path"] == h["path"] and x["line"] == h["line"] and x["active"]
                               and (not h.get("expect") or h["expect"] in x["text"]) for x in r["hits"])
    hit_refs = []
    for sec in ("capabilities", "edges", "gap_status", "sr08_dispositions") + ROW_SECTIONS:
        for x in d.get(sec) or []:
            for ref in (x.get("evidence") or []) + [r for k in ("call_site", "binding", "receiver")
                                                   for r in ((x.get("indirect") or {}).get(k) or [])]:
                if isinstance(ref, dict):
                    hit_refs.append(ref)
                    if not hit_ok(ref):
                        qerr.append("%s hit %s:%s not an active hit of %s%s" % (x["id"], ref["path"], ref["line"], ref["query"],
                                                                             " containing %r" % ref["expect"] if ref.get("expect") else ""))
    for sec in ("findings", "evidence_appends", "errata"):
        for x in f.get(sec) or []:
            for ref in x.get("evidence") or []:
                if isinstance(ref, dict):
                    hit_refs.append(ref)
                    if not hit_ok(ref):
                        qerr.append("%s hit %s:%s not an active hit of %s%s" % (x.get("id") or x.get("target"), ref["path"], ref["line"], ref["query"],
                                                                             " containing %r" % ref["expect"] if ref.get("expect") else ""))
    res["queries"] = {"total": len(qrecs), "tool_failures": sum(1 for r in qrecs if r["tool_failure"]),
                      "hit_refs_checked": len(hit_refs), "hit_refs_with_expect": sum(1 for h in hit_refs if h.get("expect")),
                      "errors": qerr, "ok": not qerr}
    res["anchor_roles"] = role_check(d["anchors"])
    scr = [structure_check(s) for s in d["structure_checks"]]
    res["structure_checks"] = {"total": len(scr), "ok": sum(r["ok"] for r in scr), "rows": scr}
    fn = {"defines": vt_defines}
    vres = {v["id"]: fn[v["type"]](v) for v in d["value_tables"]}
    res["value_tables"] = dict(vres, ok=all(v["ok"] for v in vres.values()))
    ext = ext_ids()
    res["external_ids_loaded"] = len(ext)
    res["structure"] = structure(d, f, ext)
    res["deferred_findings"] = deferred(f, d, ext)
    res["dot"] = dot_check(pkg, d)
    hexhits = []
    for root, _, files in os.walk(pkg):
        for name in files:
            p = os.path.join(root, name)
            if p.endswith((".md", ".yaml", ".py", ".json")) and HEX40.search(open(p, encoding="utf-8", errors="replace").read()):
                hexhits.append(os.path.relpath(p, pkg))
    res["hex40_in_package"] = hexhits
    res["all_ok"] = (res["anchors"]["ok"] == res["anchors"]["total"] and res["markdown_tags"]["ok"] == res["markdown_tags"]["total"]
                     and res["markdown_tags"]["total"] > 0 and res["queries"]["ok"]
                     and res["structure_checks"]["ok"] == res["structure_checks"]["total"]
                     and res["value_tables"]["ok"] and res["structure"]["ok"] and res["deferred_findings"]["ok"]
                     and res["dot"]["ok"] and res["anchor_roles"]["ok"] and not hexhits)
    res["scan_index_repo_rebound"] = True
    qtext = json.dumps({"pin_time": PIN, "queries": qrecs}, ensure_ascii=False, indent=1)
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text) or HEX40.search(qtext):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(os.path.join(os.path.dirname(os.path.abspath(outp)), "queries.json"), "w", encoding="utf-8") as fh:
        fh.write(qtext + "\n")
    with open(outp, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    print(json.dumps({"anchors": [res["anchors"]["ok"], res["anchors"]["total"]],
                      "markdown_tags": [res["markdown_tags"]["ok"], res["markdown_tags"]["total"]],
                      "queries": [len(qrecs), len(qerr)], "hit_refs": len(hit_refs),
                      "hit_refs_with_expect": res["queries"]["hit_refs_with_expect"], "role_errors": len(res["anchor_roles"]["errors"]),
                      "structure_checks": [res["structure_checks"]["ok"], res["structure_checks"]["total"]],
                      "value_tables_ok": res["value_tables"]["ok"], "structure_errors": len(res["structure"]["errors"]),
                      "deferred_findings_errors": len(res["deferred_findings"]["errors"]), "dot_errors": len(res["dot"]["errors"]),
                      "hex40_in_package": len(hexhits), "all_ok": res["all_ok"]}, ensure_ascii=False))
    return 0 if res["all_ok"] else 1


def tree_of(rev):
    rows = {}
    for rec in git("ls-tree", "-r", "-z", "--full-tree", rev, check=True).stdout.decode().split("\0"):
        if rec:
            meta, path = rec.split("\t", 1)
            mode, _typ, obj = meta.split()
            rows[path] = (mode, obj)            # full object id; compared in memory, never written out
    return rows


def boundary(argv):
    """--staged: compare the index with BASE; --commit REV: compare commit REV with BASE.
    Exits 2 unless every explicit condition holds (RC08-01: the booleans and the exit code are the gate)."""
    outp, mode = argv[0], argv[1]
    base = tree_of(BASE)
    base_full = git("rev-parse", BASE + "^{commit}", check=True).stdout.decode().strip()
    if mode == "--staged":
        target = {}
        for rec in git("ls-files", "-s", "-z", check=True).stdout.decode().split("\0"):
            if rec:
                meta, path = rec.split("\t", 1)
                fmode, obj, _st = meta.split()
                target[path] = (fmode, obj)
        diff_args = ["diff", "--cached", "--name-status", "--no-renames", BASE]
        label = "index (staged)"
    else:
        rev = argv[2]
        target = tree_of(rev)
        diff_args = ["diff", "--name-status", "--no-renames", BASE, rev]
        label = "commit " + git("rev-parse", "--short=12", rev, check=True).stdout.decode().strip()
    changes = []
    for ln in git(*diff_args, check=True).stdout.decode().splitlines():
        st, path = ln.split("\t", 1)
        changes.append({"status": st, "path": path})
    differing = [p for p in base if p in target and target[p] != base[p]]
    missing = [p for p in base if p not in target]
    added = sorted(p for p in target if p not in base)
    rem = git("ls-remote", "origin", "refs/heads/" + EXEC, check=True).stdout.decode().split()
    remote_full = rem[0] if rem else ""
    head_full = git("rev-parse", "HEAD", check=True).stdout.decode().strip()
    unstaged = git("diff", "--name-status", "--no-renames").stdout.decode().splitlines()
    untracked = git("ls-files", "--others", "--exclude-standard").stdout.decode().splitlines()
    checks = {"base_files_identical_by_full_object": not differing and not missing,
              "has_additions": bool(added),
              "additions_only_under_prefix": bool(added) and all(p.startswith(PREFIX) for p in added),
              "diff_only_added_under_prefix": bool(changes) and all(c["status"] == "A" and c["path"].startswith(PREFIX) for c in changes),
              "nothing_unstaged": not unstaged, "nothing_untracked": not untracked}
    if mode == "--staged":
        checks["head_is_base"] = head_full == base_full
        checks["remote_exec_head_is_base"] = remote_full == base_full
    else:
        rev_full = git("rev-parse", argv[2] + "^{commit}", check=True).stdout.decode().strip()
        checks["remote_exec_head_is_target"] = remote_full == rev_full
    ok = all(checks.values())
    res = {"record": "D04_WRITE_AREA_BOUNDARY", "base": BASE, "target": label,
           "comparison": "mode and full object id of every path, compared in memory; only paths, counts and 12-character commit ids are written",
           "base_files": len(base), "base_files_identical": len(base) - len(differing) - len(missing),
           "differing": differing, "missing": missing, "added": added, "changes_vs_base": changes,
           "unstaged": unstaged, "untracked": untracked, "remote_exec_head": remote_full[:12], "head": head_full[:12],
           "checks": checks, "ok": ok, "commands_run": sorted(set(COMMANDS))}
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(outp, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    print(json.dumps({"base_files": len(base), "added": len(added), "checks": checks, "ok": ok}, ensure_ascii=False))
    return 0 if ok else 2


def main(argv):
    global REPO, S
    REPO, mode = os.path.realpath(argv[0]), argv[1]
    S, frozen = load_scan_index(REPO)
    if S.PIN != PIN:
        sys.stderr.write("REFUSED: scan_index PIN %s != %s\n" % (S.PIN, PIN))
        return 4
    sys.stderr.write(json.dumps({"scan_index_repo_frozen": frozen, "scan_index_repo_rebound": REPO, "pin": S.PIN}) + "\n")
    return {"check": check, "boundary": boundary}[mode](argv[2:])


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
