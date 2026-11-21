# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-MM-BASELINE-04 (mm-baseline-04)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file. Read-only reuse (import, no copy) of analyse()/enclosing()/show() from
#   ../../scheduler-integration-03/scripts/scan_index.py at ee9e6a738224: they give top-level brace spans,
#   a comment mask and the #if stack per line of a file at time a039d9803ade. The lexer is heuristic
#   (see that package's limits); this file adds its own strict per-kind rules and does not use the
#   older verify_anchors.py leniencies (file-scope naming, comment-above-define).
# purpose: mechanical consistency check of facts-and-dependencies.yaml and the Markdown files:
#   (1) every anchor: 1-5 verbatim lines at the stated line range of the file at the time pin, checked
#       by its declared kind (in_body / in_body_comment / macro / type_member / build), with the
#       recorded #if conditions equal to the computed non-guard #if stack;
#   (2) every "[VERIFIED path::symbol]" block in the given Markdown files: consecutive verbatim lines
#       strictly inside the body of that symbol;
#   (3) structure: unique IDs, no dangling references, caps (24 capabilities, 3-6 per subsystem,
#       40 edges, 80 anchors, 6 candidates), allowed enum values, runtime NOT_RUN everywhere,
#       counts block equals recomputed counts, every concern has premise/path/counter-evidence/unknown;
#   (4) queries: re-run every recorded git-grep / size query at the time pin, compare return code and
#       hit counts with the recorded expectation, and resolve edge hit references; return code >= 2
#       is reported as a tool failure, never as zero hits;
#   (5) M00 arithmetic over the NR-6 value table (charges per step, totals, span conservation).
#   Writes the query records to <records_dir>/queries.json and the check result to <out.json>.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 mm_facts_check.py <facts-and-dependencies.yaml> <records_dir> <out.json> [<file.md> ...]"""
import json
import os
import re
import subprocess
import sys

import yaml

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.normpath(os.path.join(HERE, "..", "..", "scheduler-integration-03", "scripts")))
import scan_index as S  # noqa: E402  (read-only reuse, see header)

PIN = S.PIN
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
TAG = re.compile(r"\[VERIFIED ([^\]:]+)::([^\]]+)\]\s*\n```[\w+-]*\n(.*?)\n```", re.S)
KINDS = {"in_body", "in_body_comment", "macro", "type_member", "build"}
EDGE_KINDS = {"call", "data", "init_order", "config", "build"}
IMPL = {"declaration_only", "partial_body", "connected_body", "not_located_in_scope", "not_assessed"}
CORR = {"not_assessed", "static_concern", "bounded_prior_observation"}
CONC = {"not_assessed", "source_only"}
SUBSYSTEMS = ["mm.page_alloc", "mm.kmalloc", "mm.vm_map", "mm.fault"]


EXTRA_ATTRS = {"__alloc_size", "__malloc", "__must_check", "__always_inline", "noinline", "__init", "__ref"}


def span_name(a, x):
    """Function name of a span; re-derived from the header text when the reused lexer picked an
    attribute macro such as __alloc_size(1) (kept strict: first identifier followed by '(' that is
    not an attribute macro)."""
    if x["kind"] != "function" or x["name"] not in EXTRA_ATTRS:
        return x["name"]
    head = " ".join(a["clines"][x["start"] - 1:x["brace_line"]])
    for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", head):
        if m.group(1) not in EXTRA_ATTRS and m.group(1) not in S.ATTRIBUTE_MACROS:
            return m.group(1)
    return x["name"]


def code_chars(a, ln):
    """Non-blank characters of line ln that are outside comments."""
    line, mask = a["lines"][ln - 1], a["mask"][ln - 1]
    return [c for c, m in zip(line, mask) if not m and not c.isspace()]


def cpp_at(a, ln):
    return [e["text"] for e in a["cpp"][ln - 1] if not e.get("guard")] if a and ln - 1 < len(a["cpp"]) else []


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
    while q and q[-1] == "":
        q.pop()
    s, e = an["lines"]
    res["quote_lines"] = len(q)
    if not 1 <= len(q) <= 5 or e - s + 1 != len(q) or lines[s - 1:e] != q:
        res["reason"] = "quote is not the verbatim lines %d-%d (or not 1-5 lines)" % (s, e)
        return res
    if an["kind"] == "build":
        if not an["path"].endswith((".cmake", "CMakeLists.txt")):
            res["reason"] = "build anchor outside a CMake file"
            return res
        word = re.compile(r"(?<![\w])%s(?![\w])" % re.escape(an["symbol"]))
        named = bool(word.search(q[0]))
        if not named:
            for k in range(s - 1, max(0, s - 21), -1):
                m = re.match(r"\s*(?:set|file)\s*\(\s*(?:GLOB_RECURSE\s+|GLOB\s+)?(\w+)", lines[k - 1])
                if m:
                    named = m.group(1) == an["symbol"]
                    break
        res["ok"] = named and an.get("cpp_conditions", []) == []
        res["reason"] = None if res["ok"] else "symbol not named by the quote or its set()/file() command"
        return res
    a = S.analyse(an["path"])
    cpp = cpp_at(a, s)
    res["cpp_computed"] = cpp
    if cpp != an.get("cpp_conditions", []):
        res["reason"] = "recorded #if conditions differ from computed %s" % cpp
        return res
    if an["kind"] == "macro":
        ok = re.match(r"^\s*#\s*define\s+%s\b" % re.escape(an["symbol"]), q[0]) is not None
        for k in range(s, e):                 # following lines must be continuation lines of the define
            ok = ok and lines[k - 1].rstrip().endswith("\\")
        res["ok"] = ok
        res["reason"] = None if ok else "first line is not '#define %s' or lines are not one macro" % an["symbol"]
        return res
    want = "type" if an["kind"] == "type_member" else "function"
    spans = [x for x in a["syms"] if span_name(a, x) == an["symbol"] and x["kind"] == want]
    inside = [x for x in spans if x["brace_line"] < s and e <= x["end"]]
    if not inside:
        res["reason"] = "lines not strictly inside the body of %s %s (spans %s)" % (
            want, an["symbol"], [[x["start"], x["brace_line"], x["end"]] for x in spans])
        return res
    res["span"] = [inside[0]["start"], inside[0]["brace_line"], inside[0]["end"]]
    code = [code_chars(a, ln) for ln in range(s, e + 1)]
    if an["kind"] == "in_body_comment":
        res["ok"] = all(not c for c in code)
        res["reason"] = None if res["ok"] else "kind in_body_comment but some quoted line has active text"
    else:
        res["ok"] = any(code)
        res["reason"] = None if res["ok"] else "kind %s but every quoted line is comment" % an["kind"]
    return res


def check_md_tags(path):
    rows = []
    for m in TAG.finditer(open(path, encoding="utf-8").read()):
        p, sym, body = m.group(1).strip(), m.group(2).strip(), m.group(3)
        q = body.split("\n")
        text = S.show(p)
        row = {"tag": "%s::%s" % (p, sym), "quote_lines": len(q), "ok": False}
        if text is None:
            row["reason"] = "file not readable"
            rows.append(row)
            continue
        lines = text.split("\n")
        hits = [i + 1 for i in range(len(lines) - len(q) + 1) if lines[i:i + len(q)] == q]
        a = S.analyse(p)
        good = [h for h in hits for x in a["syms"]
                if span_name(a, x) == sym and x["brace_line"] < h and h + len(q) - 1 <= x["end"]]
        if not good and any(re.match(r"^\s*#\s*define\s+%s\b" % re.escape(sym), lines[h - 1]) for h in hits):
            good = hits
        row.update(found_at=hits, ok=bool(good) and 1 <= len(q) <= 5)
        if not row["ok"]:
            row["reason"] = "not verbatim consecutive lines inside %s" % sym
        rows.append(row)
    return rows


def run_query(q):
    if q.get("type") == "size":
        hits = []
        rc = 0
        for p in q["paths"]:
            r = subprocess.run(["git", "-C", S.REPO, "cat-file", "-s", "%s:%s" % (PIN, p)], capture_output=True)
            rc = max(rc, r.returncode)
            hits.append({"path": p, "bytes": int(r.stdout) if r.returncode == 0 else None})
        return {"id": q["id"], "type": "size", "returncode": rc, "hits": hits}
    cmd = ["git", "-C", S.REPO, "grep"] + q["args"] + [PIN, "--"] + q["paths"]
    r = subprocess.run(cmd, capture_output=True)
    hits = []
    for ln in r.stdout.decode("utf-8", "replace").split("\n"):
        m = re.match(r"^%s:([^:]+):(\d+):(.*)$" % PIN, ln)
        if not m:
            continue
        path, line, text = m.group(1), int(m.group(2)), m.group(3)
        a = S.analyse(path) if path.endswith((".c", ".h", ".S")) else None
        active = bool(code_chars(a, line)) if a else True
        enc = S.enclosing(a, line) if a else None
        hits.append({"path": path, "line": line, "text": text.strip()[:160], "active": active,
                     "enclosing": span_name(a, enc) if enc else None})
    shown = " ".join(["git", "grep"] + q["args"] + ["<time>", "--"] + q["paths"])
    return {"id": q["id"], "command": shown, "returncode": r.returncode, "tool_failure": r.returncode >= 2,
            "stderr": r.stderr.decode()[:200], "hits_total": len(hits), "hits_active": sum(h["active"] for h in hits),
            "hits": hits}


def m00_check(tbl):
    out = []
    for v in tbl["variants"]:
        charge, idle, last, cur, errs = {}, 0, tbl["span"][0], tbl["start_current"], []
        for st in v["steps"]:
            seg = st["at"] - last
            if st["current"] != cur:
                errs.append("%s: current %s but previous selection was %s" % (st["step"], st["current"], cur))
            if st["current"] == tbl["idle"]:
                idle += seg
                if st["charge"] != 0:
                    errs.append("%s: idle charged" % st["step"])
            elif st["charge"] != seg:
                errs.append("%s: charge %s != interval %s" % (st["step"], st["charge"], seg))
            charge[st["current"]] = charge.get(st["current"], 0) + (0 if st["current"] == tbl["idle"] else st["charge"])
            last, cur = st["at"], st["selected"]
        charge.pop(tbl["idle"], None)
        got = dict(charge, idle_interval=idle)
        span = tbl["span"][1] - tbl["span"][0]
        conserved = sum(charge.values()) + idle == span
        out.append({"variant": v["id"], "computed": got, "expected": v["expected"],
                    "match": got == v["expected"], "conserved": conserved, "errors": errs,
                    "ok": got == v["expected"] and conserved and not errs})
    return out


def main(argv):
    facts, recdir, outp, mds = argv[0], argv[1], argv[2], argv[3:]
    d = yaml.safe_load(open(facts, encoding="utf-8"))
    res = {"check": "MM_BASELINE_04_FACTS", "pin_time": PIN}
    anchors = [check_anchor(a) for a in d["anchors"]]
    res["anchors"] = {"total": len(anchors), "ok": sum(r["ok"] for r in anchors),
                      "by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(KINDS)},
                      "failures": [r for r in anchors if not r["ok"]]}
    res["markdown_tags"] = {}
    for md in mds:
        rows = check_md_tags(md)
        res["markdown_tags"][os.path.basename(md)] = {"total": len(rows), "ok": sum(r["ok"] for r in rows),
                                                     "failures": [r for r in rows if not r["ok"]]}
    # ---- structure ----
    errs = []
    ids = {}
    for sec in ("subsystems", "capabilities", "anchors", "edges", "link_breaks", "concerns", "dependency_risks",
                "queries", "next_validation_candidates", "gaps", "old_inputs_index", "refuted_claims"):
        for x in d.get(sec) or []:
            key = x.get("id") or x.get("proposed_id")
            if key in ids:
                errs.append("duplicate id %s (%s, %s)" % (key, ids[key], sec))
            ids[key] = sec
    anc = {a["id"] for a in d["anchors"]}
    qids = {q["id"] for q in d["queries"]}
    edg = {e["id"] for e in d["edges"]}
    con = {c["id"] for c in d["concerns"]}
    drk = {r["id"] for r in d["dependency_risks"]}
    caps = {c["id"] for c in d["capabilities"]}
    for c in d["capabilities"]:
        for k, pool in (("anchors", anc), ("edges", edg), ("concerns", con), ("dependency_risks", drk), ("queries", qids)):
            for r in c.get(k) or []:
                if r not in pool:
                    errs.append("%s.%s -> missing %s" % (c["id"], k, r))
        ie, ce, ru, cc = c["implementation_evidence"], c["correctness_evidence"], c["runtime_evidence"], c["concurrency_evidence"]
        if ie["value"] not in IMPL or not ie.get("reason"):
            errs.append("%s implementation_evidence" % c["id"])
        if ce["value"] not in CORR or (ce["value"] == "static_concern" and not c.get("concerns")):
            errs.append("%s correctness_evidence" % c["id"])
        if ru != "NOT_RUN":
            errs.append("%s runtime_evidence %s" % (c["id"], ru))
        if cc["value"] not in CONC:
            errs.append("%s concurrency_evidence" % c["id"])
        if c["subsystem"] not in SUBSYSTEMS:
            errs.append("%s subsystem" % c["id"])
        for f in ("purpose", "entries", "normal_path", "failure_path", "source_condition", "not_covered"):
            if not c.get(f):
                errs.append("%s missing %s" % (c["id"], f))
        if c["source_condition"].get("resolution") not in ("not_conditional", "resolved", "unknown", "partially_resolved"):
            errs.append("%s source_condition.resolution" % c["id"])
    for e in d["edges"]:
        if e["kind"] not in EDGE_KINDS:
            errs.append("%s kind" % e["id"])
        if not e.get("anchors") and not e.get("hits"):
            errs.append("%s has no evidence" % e["id"])
        for r in e.get("anchors") or []:
            if r not in anc:
                errs.append("%s -> missing anchor %s" % (e["id"], r))
        for h in e.get("hits") or []:
            if h["query"] not in qids:
                errs.append("%s -> missing query %s" % (e["id"], h["query"]))
        for end in ("from", "to"):
            if e[end] not in caps and e[end] not in SUBSYSTEMS and not e[end].startswith("external:"):
                errs.append("%s %s endpoint %s neither capability, subsystem nor external:" % (e["id"], end, e[end]))
        for s in e.get("also_applies_to") or []:
            if s not in SUBSYSTEMS and s not in caps:
                errs.append("%s also_applies_to %s" % (e["id"], s))
    capd = {c["id"]: c for c in d["capabilities"]}
    for e in d["edges"]:                      # edge <-> capability listing must agree both ways
        for end in ("from", "to"):
            if e[end] in capd and e["id"] not in (capd[e[end]].get("edges") or []):
                errs.append("%s not listed in edges of its endpoint %s" % (e["id"], e[end]))
    eby = {e["id"]: e for e in d["edges"]}
    for c in d["capabilities"]:
        for r in c.get("edges") or []:
            if r in eby and c["id"] not in (eby[r]["from"], eby[r]["to"]):
                errs.append("%s lists %s but is not one of its endpoints" % (c["id"], r))
    for c in d["concerns"]:
        if c.get("status") != "static_concern":
            errs.append("%s status" % c["id"])
        for f in ("claim", "premise", "path", "counter_evidence", "unknown"):
            if not c.get(f):
                errs.append("%s missing %s" % (c["id"], f))
        for r in c.get("anchors") or []:
            if r not in anc:
                errs.append("%s -> missing anchor %s" % (c["id"], r))
        for r in c.get("queries") or []:
            if r not in qids:
                errs.append("%s -> missing query %s" % (c["id"], r))
        if c.get("capability") not in caps:
            errs.append("%s capability %s" % (c["id"], c.get("capability")))
    for r in d["dependency_risks"]:
        for f in ("depends_on", "affects", "evidence_class", "unknown"):
            if not r.get(f):
                errs.append("%s missing %s" % (r["id"], f))
    for b in d["link_breaks"] + d["refuted_claims"]:
        if not (b.get("anchors") or b.get("queries")):
            errs.append("%s without evidence" % b["id"])
        for r in b.get("anchors") or []:
            if r not in anc:
                errs.append("%s -> missing anchor %s" % (b["id"], r))
        for r in b.get("queries") or []:
            if r not in qids:
                errs.append("%s -> missing query %s" % (b["id"], r))
    for v in d["next_validation_candidates"]:
        if v.get("status") != "NOT_RUN":
            errs.append("%s status" % v["id"])
    per = {s: sum(1 for c in d["capabilities"] if c["subsystem"] == s) for s in SUBSYSTEMS}
    caps_ok = len(d["capabilities"]) <= 24 and all(3 <= n <= 6 for n in per.values())
    limits = {"capabilities": [len(d["capabilities"]), 24], "edges": [len(d["edges"]), 40],
              "anchors": [len(d["anchors"]), 80], "candidates": [len(d["next_validation_candidates"]), 6],
              "per_subsystem": per}
    if not caps_ok or len(d["edges"]) > 40 or len(d["anchors"]) > 80 or len(d["next_validation_candidates"]) > 6:
        errs.append("limit exceeded %s" % limits)
    if [s["id"] for s in d["subsystems"]] != SUBSYSTEMS:
        errs.append("subsystem rows are not exactly the four fixed subsystems")
    recount = {"subsystems": len(d["subsystems"]), "capabilities": len(d["capabilities"]), "anchors": len(d["anchors"]),
               "edges": len(d["edges"]), "link_breaks": len(d["link_breaks"]), "concerns": len(d["concerns"]),
               "dependency_risks": len(d["dependency_risks"]), "queries": len(d["queries"]),
               "next_validation_candidates": len(d["next_validation_candidates"]), "gaps": len(d["gaps"]),
               "refuted_claims": len(d["refuted_claims"]),
               "gaps_open": sum(1 for g in d["gaps"] if g.get("status") == "open"),
               "capabilities_per_subsystem": per,
               "implementation_evidence": {k: sum(1 for c in d["capabilities"] if c["implementation_evidence"]["value"] == k)
                                           for k in sorted(IMPL)}}
    if d.get("counts") != recount:
        errs.append("counts block differs from recount")
    res["structure"] = {"errors": errs, "limits": limits, "recount": recount, "ok": not errs}
    # ---- queries ----
    qrecs = [run_query(q) for q in d["queries"]]
    os.makedirs(recdir, exist_ok=True)
    qtext = json.dumps({"pin_time": PIN, "queries": qrecs}, ensure_ascii=False, indent=1)
    qerr = []
    by = {r["id"]: r for r in qrecs}
    for q in d["queries"]:
        r = by[q["id"]]
        if r.get("tool_failure"):
            qerr.append("%s tool failure rc=%s" % (q["id"], r["returncode"]))
            continue
        if q.get("type") == "size":
            if [h["bytes"] for h in r["hits"]] != q["expected_bytes"]:
                qerr.append("%s sizes %s" % (q["id"], [h["bytes"] for h in r["hits"]]))
            continue
        if r["returncode"] != q["expected_rc"] or r["hits_total"] != q["expected_hits_total"] \
                or r["hits_active"] != q["expected_hits_active"]:
            qerr.append("%s rc/total/active %s/%s/%s" % (q["id"], r["returncode"], r["hits_total"], r["hits_active"]))
        if q["expect"] == "negative_active" and r["hits_active"] != 0:
            qerr.append("%s expected no active hit" % q["id"])
    for e in d["edges"]:
        for h in e.get("hits") or []:
            r = by.get(h["query"])
            if not r or not any(x["path"] == h["path"] and x["line"] == h["line"] and x["active"]
                                and (h.get("enclosing") is None or x["enclosing"] == h["enclosing"]) for x in r["hits"]):
                qerr.append("%s hit %s:%s not found as active hit of %s" % (e["id"], h["path"], h["line"], h["query"]))
    res["queries"] = {"total": len(qrecs), "tool_failures": sum(1 for r in qrecs if r.get("tool_failure")),
                      "errors": qerr, "ok": not qerr}
    # ---- M00 ----
    m00 = m00_check(d["m00_value_table"])
    res["m00"] = {"variants": m00, "ok": all(x["ok"] for x in m00)}
    res["all_ok"] = (res["anchors"]["ok"] == res["anchors"]["total"] and res["structure"]["ok"] and res["queries"]["ok"]
                     and res["m00"]["ok"] and all(v["ok"] == v["total"] for v in res["markdown_tags"].values()))
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text) or HEX40.search(qtext):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    with open(os.path.join(recdir, "queries.json"), "w", encoding="utf-8") as fh:
        fh.write(qtext + "\n")
    with open(outp, "w", encoding="utf-8") as fh:
        fh.write(text + "\n")
    print(json.dumps({"anchors": [res["anchors"]["ok"], res["anchors"]["total"]],
                      "markdown_tags": {k: [v["ok"], v["total"]] for k, v in res["markdown_tags"].items()},
                      "structure_errors": len(errs), "query_errors": len(qerr), "m00_ok": res["m00"]["ok"],
                      "all_ok": res["all_ok"]}))
    return 0 if res["all_ok"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
