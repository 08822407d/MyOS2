# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-USER-BOUNDARIES-07 (user-boundaries-07)
# executor: Claude Code desktop app, local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd
#   (MYOS2-A-CC02); model: claude-opus-5-5 as declared by the runtime (not independently attestable)
# source: new file. Read-only reuse (import, no copy) of show()/analyse() from
#   ../../scheduler-integration-03/scripts/scan_index.py (heuristic C lexer: comment mask, #if stack,
#   top-level brace spans). Its fixed REPO (/home/user/MyOS2) is rebound to the given clone root, as
#   in execution-handoff-06/local/scripts/run_uv05_recheck.py. Bytecode writing is disabled first.
# purpose: mechanical consistency check of this package, not a semantic proof:
#   (1) anchors: 1-5 verbatim lines at the time pin; per kind: C function body / initializer / type
#       member (C spans), macro (#define + continuation lines), asm_range (lines between
#       SYM_*_START(sym) and its SYM_*_END, or the next symbol/section line when no END exists),
#       build (CMake text naming the symbol); recorded #if conditions = computed non-guard stack;
#   (2) "[VERIFIED path::symbol]" blocks in the Markdown files, with the same span rules;
#   (3) queries re-run with git grep at the pin (return code, total and active hit counts; a hit is
#       active when it has non-comment text: C lexer mask for .c/.h/.S, '#' lines in CMake/shell);
#   (4) structure checks: asm instruction sequences (comments stripped) and active-regex counts in
#       one C function body;
#   (5) value tables recomputed from source text: pt_regs layout vs committed generated offsets and
#       PUSH_REGS/POP_REGS/entry push order; inactive_task_frame vs __switch_to_asm; flag values;
#   (6) structure of facts-and-boundaries.yaml and deferred-findings.yaml (IDs, references, limits;
#       each finding's future_verification.candidates equal to the candidates whose covers name it;
#       each erratum's quoted original text present, whitespace-normalised, in the base commit);
#   (7) boundary: files of a base commit unchanged in the index or a commit, additions only under
#       the allowed prefix, remote head of the execution branch (git ls-remote, read-only);
#   (8) no 40-hex run in the package text or in the outputs.
# It never checks out, fetches, compiles or runs repository code.
# ---------------------------------------------------------------------------------------------------
"""Usage: PYTHONDONTWRITEBYTECODE=1 python3 -B -P ub_check.py <repo_root> check <package_dir> <out.json>
       PYTHONDONTWRITEBYTECODE=1 python3 -B -P ub_check.py <repo_root> boundary <out.json> --staged
(queries.json is written next to <out.json> in check mode)"""
import sys
sys.dont_write_bytecode = True

import importlib.util  # noqa: E402
import json  # noqa: E402
import os  # noqa: E402
import re  # noqa: E402
import subprocess  # noqa: E402

import yaml  # noqa: E402

PIN = "a039d9803ade"
BASE = "b12572f14eae"                    # reviewed execution head, all files frozen
EXEC = "claude/dazzling-cori-q0dnyt"
PKG = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
PREFIX = PKG + "user-boundaries-07/"
UV05 = PKG + "user-vfs-baseline-05/facts-and-dependencies.yaml"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
TAG = re.compile(r"\[VERIFIED ([^\]:]+)::([^\]]+)\]\s*\n```[\w+-]*\n(.*?)\n```", re.S)
SYS = re.compile(r"MYOS_SYSCALL_DEFINE\d\s*\(\s*(\w+)")
ASM_START = re.compile(r"^\s*SYM_(?:CODE|FUNC)_START\((\w+)\)")
ASM_BREAK = re.compile(r"^\s*(?:SYM_(?:CODE|FUNC)_START\(|\.pushsection|\.popsection)")
KINDS = {"in_body", "initializer", "type_member", "macro", "asm_range", "build"}
LIMITS = {"capabilities": 15, "edges": 40, "anchors": 75, "candidates": 4}
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


def asm_macro_range(lines, name):
    start = next((i + 1 for i, l in enumerate(lines) if re.match(r"^\s*\.macro\s+%s\b" % re.escape(name), l)), None)
    if start is None:
        return None
    end = next((i + 1 for i in range(start, len(lines)) if re.match(r"^\s*\.endm\b", lines[i])), len(lines))
    return start, end, "endm"


def norm(s):
    s = re.sub(r"\s+", " ", s.strip())
    return re.sub(r"\s*,\s*", ",", s)


def asm_instructions(a, start, end):
    out = []
    for ln in range(start + 1, end + 1):
        t = norm(active(a, ln))
        if not t or ASM_START.match(t) or re.match(r"^SYM_(?:CODE|FUNC)_END\(", t):
            continue
        out.append([ln, t])
    return out


def in_span(a, path, sym, kind, s, e):
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
        res["ok"] = re.search(r"(?<![\w])%s(?![\w])" % re.escape(an["symbol"]), "\n".join(q)) is not None \
            and an.get("cpp_conditions", []) == []
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
    ok, span = in_span(a, an["path"], an["symbol"], an["kind"], s, e)
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
        good = []
        if p.endswith(".S"):
            r = asm_range(lines, sym) or asm_macro_range(lines, sym)
            good = [h for h in hits if r and r[0] < h and h + len(q) - 1 <= r[1]]
        else:
            a = S.analyse(p)
            good = [h for h in hits for x in a["syms"]
                    if span_name(a, x) == sym and x["brace_line"] < h and h + len(q) - 1 <= x["end"]]
            if not good:
                good = [h for h in hits if re.match(r"^\s*#\s*define\s+%s\b" % re.escape(sym), lines[h - 1])]
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


def structure_check(sc):
    res = {"id": sc["id"], "kind": sc["kind"], "path": sc["path"], "symbol": sc["symbol"], "ok": False}
    a = S.analyse(sc["path"])
    if sc["kind"] == "asm_sequence":
        r = asm_range(a["lines"], sc["symbol"])
        if r is None:
            res["reason"] = "symbol not found"
            return res
        got = asm_instructions(a, r[0], r[1])
        res.update(range=list(r), instructions=[t for _, t in got])
        res["ok"] = [t for _, t in got] == [norm(x) for x in sc["expected"]] and r[2] == sc["expected_bound"]
        if not res["ok"]:
            res["reason"] = "instruction sequence or bound differs"
        return res
    spans = [x for x in a["syms"] if x["kind"] == "function" and span_name(a, x) == sc["symbol"]]
    if len(spans) != 1:
        res["reason"] = "expected exactly one function span, got %d" % len(spans)
        return res
    x = spans[0]
    body = [active(a, ln) for ln in range(x["brace_line"], x["end"] + 1)]
    head = a["lines"][x["brace_line"] - 1]
    body[0] = "".join(c if not m else " " for c, m in zip(head, a["mask"][x["brace_line"] - 1]))[head.find("{") + 1:]
    tail = body[-1]
    body[-1] = tail[:tail.rfind("}")] if "}" in tail else tail
    n = sum(len(re.findall(sc["regex"], t)) for t in body)
    res.update(span=[x["start"], x["brace_line"], x["end"]], regex=sc["regex"], active_matches=n,
               expected=sc["expected_count"], ok=n == sc["expected_count"])
    return res


def defines(path):
    """object-like #define NAME value (value = rest of the line without a trailing comment)."""
    out = {}
    for l in S.show(path).split("\n"):
        m = re.match(r"^\s*#\s*define\s+(\w+)\s+(.+?)\s*(?:/\*.*)?$", l)
        if m:
            out[m.group(1)] = m.group(2)
    return out


def num(v):
    """last numeric literal of a #define value: 0x.. hex, 0.. octal or decimal."""
    lits = re.findall(r"0[xX][0-9a-fA-F]+|\b\d+\b", v)
    lit = lits[-1]
    return int(lit, 16) if lit.lower().startswith("0x") else int(lit, 8) if re.match(r"^0[0-7]+$", lit) else int(lit)


def vt_ptregs(t):
    text = S.show(t["struct_path"])
    body = text[text.index("typedef struct pt_regs {"):text.index("} pt_regs_s;")]
    fields = re.findall(r"^\s*reg_t\s+(\w+);", body, re.M)
    off = {f: 8 * i for i, f in enumerate(fields)}
    gen = defines(t["offsets_path"])
    rows = []
    for macro, field in t["asm_macros"].items():
        rows.append({"macro": macro, "field": field, "struct_offset": off.get(field),
                     "generated": int(gen[macro]) if macro in gen else None,
                     "ok": macro in gen and int(gen[macro]) == off.get(field)})
    lines = S.show(t["entry_path"]).split("\n")
    a = S.analyse(t["entry_path"])

    def macro_regs(name):
        r = asm_macro_range(lines, name)
        return [m.group(1) for _, ins in asm_instructions(a, r[0], r[1] - 1)
                if (m := re.match(r"^(?:pushq|popq) %(\w+)$", ins))]
    regmap = {"rdi": "di", "rsi": "si", "rdx": "dx", "rcx": "cx", "rax": "ax", "rbx": "bx", "rbp": "bp"}
    push = [regmap.get(x, x) for x in macro_regs("PUSH_REGS")]
    pop = [regmap.get(x, x) for x in macro_regs("POP_REGS")]
    r = asm_range(lines, "entry_SYSCALL_64")
    seq = [ins for _, ins in asm_instructions(a, r[0], r[1])]
    pushes = [ins for ins in seq if ins.startswith("pushq") or ins == "PUSH_REGS"]
    n_slots = len([p for p in pushes if p != "PUSH_REGS"]) + len(push)
    sysret = [ins for _, ins in asm_instructions(a, *asm_range(lines, "syscall_return_via_sysret")[:2])]
    addq = next((int(m.group(1), 0) for ins in sysret if (m := re.match(r"^addq \$(0x[0-9a-f]+|\d+),%rsp$", ins))), None)
    res = {"fields": fields, "field_count": len(fields), "rows": rows,
           "ptregs_size_generated": int(gen.get("PTREGS_SIZE", -1)), "ptregs_size_struct": 8 * len(fields),
           "push_regs_order": push, "pop_regs_order": pop,
           "push_regs_reversed_equals_first_fields": list(reversed(push)) == fields[:len(push)],
           "pop_regs_equals_first_fields": pop == fields[:len(pop)],
           "entry_push_sequence": pushes, "entry_slots": n_slots,
           "sysret_addq": addq, "sysret_skip_expected": off["sp"] - off["irq_nr"]}
    res["ok"] = (all(x["ok"] for x in rows) and res["ptregs_size_generated"] == res["ptregs_size_struct"]
                 and res["push_regs_reversed_equals_first_fields"] and res["pop_regs_equals_first_fields"]
                 and n_slots == len(fields) and addq == res["sysret_skip_expected"])
    return res


def vt_kframe(t):
    text = S.show(t["types_path"])
    body = text[text.index("struct inactive_task_frame {"):text.index("struct fork_frame {")]
    fields = re.findall(r"^\s*reg_t\s+(\w+);", body, re.M)
    ff = text[text.index("struct fork_frame {"):]
    ff_members = re.findall(r"^\s*\w+\s+(\w+);", ff[:ff.index("};")], re.M)
    lines = S.show(t["entry_path"]).split("\n")
    a = S.analyse(t["entry_path"])
    r = asm_range(lines, "__switch_to_asm")
    seq = [ins for _, ins in asm_instructions(a, r[0], r[1])]
    regmap = {"rbx": "bx", "rbp": "bp"}
    push = [regmap.get(m.group(1), m.group(1)) for ins in seq if (m := re.match(r"^pushq %(\w+)$", ins))]
    pop = [regmap.get(m.group(1), m.group(1)) for ins in seq if (m := re.match(r"^popq %(\w+)$", ins))]
    src = S.show(t["offsets_src_path"])
    mapping = {m.group(1): (m.group(2), m.group(3)) for m in re.finditer(r"OFFSET\((\w+),\s*(\w+),\s*([\w.]+)\)", src)}
    gen = defines(t["offsets_path"])
    res = {"frame_fields": fields, "fork_frame_members": ff_members, "switch_push": push, "switch_pop": pop,
           "pop_equals_first_fields": pop == fields[:len(pop)],
           "push_reversed_equals_pop": list(reversed(push)) == pop,
           "ret_addr_after_popped": fields[len(pop):] == ["ret_addr"],
           "regs_offset_in_fork_frame": 8 * len(fields),
           "rsp_after_pops_and_ret": 8 * (len(pop) + 1),
           "symbolic_offsets": {k: list(mapping.get(k, ("?", "?"))) for k in t["symbols"]},
           "generated_values_unverified": {k: gen.get(k) for k in t["symbols"]}}
    res["ok"] = (res["pop_equals_first_fields"] and res["push_reversed_equals_pop"] and res["ret_addr_after_popped"]
                 and ff_members == ["frame", "regs"] and res["regs_offset_in_fork_frame"] == res["rsp_after_pops_and_ret"]
                 and all(k in mapping for k in t["symbols"]))
    return res


def vt_flags(t):
    vals = {}
    for name, path in t["constants"].items():
        d = defines(path)
        vals[name] = num(d[name]) if name in d else None
    fm = S.show(t["open_fmode_path"]).split("\n")
    i = next(k for k, l in enumerate(fm) if "#define OPEN_FMODE(flag)" in l)
    body = " ".join(fm[i:i + 4])
    found = all(x in body for x in t["open_fmode_parts"])
    open_fmode_values = sorted({((f + 1) & 3) for f in range(4)})
    res = {"values": vals, "o_creat_equals_lookup_rcu": vals["O_CREAT"] == vals["LOOKUP_RCU"],
           "open_fmode_line": i + 1, "open_fmode_text_found": found, "open_fmode_access_values": open_fmode_values,
           "can_write_reachable_by_open_fmode": vals["FMODE_CAN_WRITE"] in open_fmode_values
           or vals["FMODE_CAN_WRITE"] == vals["FMODE_NONOTIFY"]}
    res["ok"] = res["o_creat_equals_lookup_rcu"] == t["expect_alias"] and res["open_fmode_text_found"] \
        and res["can_write_reachable_by_open_fmode"] == t["expect_can_write_reachable"]
    return res


def uv05_anchor_ids():
    d = yaml.safe_load(open(os.path.join(REPO, UV05), encoding="utf-8"))
    return {"CORE-USER-VFS-BASELINE-05::" + a["id"] for a in d["anchors"]} | \
           {"CORE-USER-VFS-BASELINE-05::" + q["id"] for q in d["queries"]}


def structure(d, f, ext):
    errs, ids = [], {}
    for sec in ("capabilities", "edges", "anchors", "queries", "structure_checks", "value_tables",
                "gap_status", "next_validation_candidates", "frame_paths", "chain_links"):
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
    fids = {x["id"] for x in f["findings"]}

    def ev(owner, ref):
        if isinstance(ref, dict):
            if ref.get("query") not in qid:
                errs.append("%s hit -> missing query %s" % (owner, ref.get("query")))
            return
        if ref in anc or ref in qid or ref in scs or ref in vts:
            return
        if "::" in ref:
            if ref.startswith("CORE-USER-VFS-BASELINE-05::") and ref not in ext:
                errs.append("%s -> unresolved external %s" % (owner, ref))
            return
        errs.append("%s -> missing evidence %s" % (owner, ref))

    for c in d["capabilities"]:
        for k in ("purpose", "entries", "normal_path", "failure_path", "unknown", "not_covered", "evidence"):
            if not c.get(k):
                errs.append("%s missing %s" % (c["id"], k))
        if c.get("id_status") != "proposed_id" or c.get("runtime_evidence") != "NOT_RUN":
            errs.append("%s id_status/runtime_evidence" % c["id"])
        for r in c["evidence"]:
            ev(c["id"], r)
        for r in c.get("edges") or []:
            if r not in eby:
                errs.append("%s edge %s missing" % (c["id"], r))
        for r in c.get("findings") or []:
            if r not in fids:
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
            if v not in caps and "::" not in v and not v.startswith("external:"):
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
        for r in g.get("evidence") or []:
            ev(g["id"], r)
    for v in d["next_validation_candidates"]:
        if v.get("status") != "NOT_RUN" or not v.get("authorization_needed"):
            errs.append("%s status/authorization" % v["id"])
    for p in d.get("frame_paths") or []:
        for k in ("frame_writer", "built_when", "sp_taker", "consumer", "evidence"):
            if not p.get(k):
                errs.append("%s missing %s" % (p["id"], k))
        for r in p.get("evidence") or []:
            ev(p["id"], r)
    for cl in d.get("chain_links") or []:
        for r in cl.get("evidence") or []:
            ev(cl["id"], r)
    recount = {"capabilities": len(d["capabilities"]), "edges": len(d["edges"]),
               "edges_indirect": sum(1 for e in d["edges"] if "indirect" in e), "anchors": len(d["anchors"]),
               "anchors_by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(KINDS)},
               "anchors_by_theme": {t: sum(1 for a in d["anchors"] if a["theme"] == t) for t in ("B01", "B02", "B03")},
               "queries": len(d["queries"]), "structure_checks": len(d["structure_checks"]),
               "value_tables": len(d["value_tables"]), "gap_status": len(d["gap_status"]),
               "next_validation_candidates": len(d["next_validation_candidates"]),
               "frame_paths": len(d.get("frame_paths") or []), "chain_links": len(d.get("chain_links") or [])}
    limits = {"capabilities": [len(d["capabilities"]), LIMITS["capabilities"]], "edges": [len(d["edges"]), LIMITS["edges"]],
              "anchors": [len(d["anchors"]), LIMITS["anchors"]],
              "candidates": [len(d["next_validation_candidates"]), LIMITS["candidates"]]}
    if any(v[0] > v[1] for v in limits.values()):
        errs.append("limit exceeded %s" % limits)
    if d.get("counts") != recount:
        errs.append("counts block differs from recount")
    return {"errors": errs, "limits": limits, "recount": recount, "ok": not errs}


def deferred(f, d):
    ferr = []
    anc = {a["id"] for a in d["anchors"]}
    qid = {q["id"] for q in d["queries"]}
    scs = {s["id"] for s in d["structure_checks"]}
    vts = {v["id"] for v in d["value_tables"]}
    if f.get("owner_action_now") != "none" or f.get("owner_decisions_requested_now") != []:
        ferr.append("header owner fields")

    def resolve(owner, refs):
        for r in refs or []:
            if r not in anc and r not in qid and r not in scs and r not in vts and "::" not in r:
                ferr.append("%s evidence %s unresolved" % (owner, r))

    def item(x, extra=()):
        for k in ("qualified_id", "source_ref", "kind", "status", "owner_action_now", "claim", "premises", "impact",
                  "counter_evidence", "unknown", "evidence", "future_verification", "reopen_conditions", "blocks") + extra:
            if x.get(k) in (None, "", []) and not (k in ("counter_evidence", "unknown") and x.get(k) == []):
                ferr.append("%s missing %s" % (x["id"], k))
        if x["qualified_id"] != "CORE-USER-BOUNDARIES-07::" + x["id"]:
            ferr.append("%s qualified_id" % x["id"])
        if x["status"] not in F_STATUS or x["owner_action_now"] != "none" or not set(x["kind"]) <= F_KINDS:
            ferr.append("%s status/owner_action_now/kind" % x["id"])
        resolve(x["id"], x.get("evidence"))
        for r in x.get("related") or []:
            if "::" not in r:
                ferr.append("%s related %s not fully qualified" % (x["id"], r))

    # future_verification.candidates must equal the candidates whose covers name the finding
    covers = {}
    for c in d["next_validation_candidates"]:
        for t in c.get("covers") or []:
            if t.startswith("UB-"):
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
        if "::" not in x["target"] or x.get("status_change") not in ("none",) or not x.get("evidence"):
            ferr.append("%s append target/status/evidence" % x["target"])
        resolve(x["target"], x.get("evidence"))
    # errata: the quoted original text must still occur in the frozen file at BASE (whitespace-normalised)
    quoted = 0
    for x in f.get("errata") or []:
        item(x, ("target", "original_locations", "correction"))
        if x.get("kind") != ["documentation_erratum"] or "::" not in (x.get("target") or ""):
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


def check(argv):
    pkg, outp = argv[0], argv[1]
    d = yaml.safe_load(open(os.path.join(pkg, "facts-and-boundaries.yaml"), encoding="utf-8"))
    f = yaml.safe_load(open(os.path.join(pkg, "deferred-findings.yaml"), encoding="utf-8"))
    res = {"check": "USER_BOUNDARIES_07", "pin_time": PIN}
    an = [check_anchor(a) for a in d["anchors"]]
    res["anchors"] = {"total": len(an), "ok": sum(r["ok"] for r in an),
                      "by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(KINDS)},
                      "failures": [r for r in an if not r["ok"]]}
    md = os.path.join(pkg, "user-entry-console.md")
    tags = check_md_tags(md)
    res["markdown_tags"] = {"file": os.path.basename(md), "total": len(tags), "ok": sum(r["ok"] for r in tags),
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

    def hit_ok(h):
        r = by.get(h["query"])
        return bool(r) and any(x["path"] == h["path"] and x["line"] == h["line"] and x["active"] for x in r["hits"])
    hit_refs = []
    for sec in ("capabilities", "edges", "gap_status", "frame_paths", "chain_links"):
        for x in d.get(sec) or []:
            for ref in (x.get("evidence") or []) + [r for k in ("call_site", "binding", "receiver")
                                                   for r in ((x.get("indirect") or {}).get(k) or [])]:
                if isinstance(ref, dict):
                    hit_refs.append(ref)
                    if not hit_ok(ref):
                        qerr.append("%s hit %s:%s not an active hit of %s" % (x["id"], ref["path"], ref["line"], ref["query"]))
    res["queries"] = {"total": len(qrecs), "tool_failures": sum(1 for r in qrecs if r["tool_failure"]),
                      "hit_refs_checked": len(hit_refs), "errors": qerr, "ok": not qerr}
    scr = [structure_check(s) for s in d["structure_checks"]]
    res["structure_checks"] = {"total": len(scr), "ok": sum(r["ok"] for r in scr), "rows": scr}
    vt = {v["id"]: v for v in d["value_tables"]}
    fn = {"ptregs_layout": vt_ptregs, "kframe_layout": vt_kframe, "flag_values": vt_flags}
    vres = {k: fn[v["type"]](v) for k, v in vt.items()}
    res["value_tables"] = dict(vres, ok=all(v["ok"] for v in vres.values()))
    res["structure"] = structure(d, f, uv05_anchor_ids())
    res["deferred_findings"] = deferred(f, d)
    hexhits = []
    for root, _, files in os.walk(pkg):
        for name in files:
            p = os.path.join(root, name)
            if p.endswith((".md", ".yaml", ".py", ".json")) and HEX40.search(open(p, encoding="utf-8", errors="replace").read()):
                hexhits.append(os.path.relpath(p, pkg))
    res["hex40_in_package"] = hexhits
    res["all_ok"] = (res["anchors"]["ok"] == res["anchors"]["total"] and res["markdown_tags"]["ok"] == res["markdown_tags"]["total"]
                     and res["queries"]["ok"] and res["structure_checks"]["ok"] == res["structure_checks"]["total"]
                     and res["value_tables"]["ok"] and res["structure"]["ok"] and res["deferred_findings"]["ok"] and not hexhits)
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
                      "structure_checks": [res["structure_checks"]["ok"], res["structure_checks"]["total"]],
                      "value_tables_ok": res["value_tables"]["ok"], "structure_errors": len(res["structure"]["errors"]),
                      "deferred_findings_errors": len(res["deferred_findings"]["errors"]),
                      "hex40_in_package": len(hexhits), "all_ok": res["all_ok"]}, ensure_ascii=False))
    return 0 if res["all_ok"] else 1


def tree_of(rev):
    rows = {}
    for rec in git("ls-tree", "-r", "-z", "--full-tree", rev, check=True).stdout.decode().split("\0"):
        if rec:
            meta, path = rec.split("\t", 1)
            mode, _typ, obj = meta.split()
            rows[path] = (mode, obj[:12])
    return rows


def boundary(argv):
    outp = argv[0]
    base = tree_of(BASE)
    idx = {}
    for rec in git("ls-files", "-s", "-z", check=True).stdout.decode().split("\0"):
        if rec:
            meta, path = rec.split("\t", 1)
            mode, obj, _st = meta.split()
            idx[path] = (mode, obj[:12])
    changes = []
    for ln in git("diff", "--cached", "--name-status", "--no-renames", BASE, check=True).stdout.decode().splitlines():
        st, path = ln.split("\t", 1)
        changes.append({"status": st, "path": path})
    diff = [p for p in base if p in idx and idx[p] != base[p]]
    missing = [p for p in base if p not in idx]
    rem = git("ls-remote", "origin", "refs/heads/" + EXEC, check=True).stdout.decode().split()
    res = {"record": "B04_WRITE_AREA_BOUNDARY", "base": BASE, "target": "index (staged)",
           "base_files": len(base), "base_files_identical": len(base) - len(diff) - len(missing),
           "differing": diff, "missing": missing, "changes_vs_base": changes,
           "only_additions_under_prefix": bool(changes) and all(c["status"] == "A" and c["path"].startswith(PREFIX) for c in changes),
           "unstaged": git("diff", "--name-status", "--no-renames").stdout.decode().splitlines(),
           "untracked": git("ls-files", "--others", "--exclude-standard").stdout.decode().splitlines(),
           "remote_exec_head": rem[0][:12] if rem else None,
           "head": git("rev-parse", "--short=12", "HEAD").stdout.decode().strip(),
           "commands_run": sorted(set(COMMANDS))}
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(outp, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    print(json.dumps({k: res[k] for k in ("base_files", "base_files_identical", "only_additions_under_prefix",
                                          "remote_exec_head", "head")}, ensure_ascii=False))
    return 0


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
