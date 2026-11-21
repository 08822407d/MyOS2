# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-USER-VFS-BASELINE-05 (user-vfs-baseline-05)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file. Read-only reuse (import, no copy) of check_anchor()/check_md_tags()/run_query()/
#   span_name()/cpp_at() from ../../mm-baseline-04/scripts/mm_facts_check.py at de7c96ede546, which in
#   turn imports scan_index.py from scheduler-integration-03 (heuristic lexer; see those packages).
#   One local rule is added: a function whose header is MYOS_SYSCALL_DEFINEn(name, ...) is named
#   sys_<name> (the reused lexer names it by the macro). Bytecode writing is disabled before import.
# purpose: mechanical consistency check of this package, not a semantic proof:
#   (1) anchors: 1-5 verbatim lines at the time pin, per declared kind, #if stack recorded = computed;
#   (2) "[VERIFIED path::symbol]" blocks in the given Markdown files;
#   (3) structure: unique IDs, resolvable references, caps (20 capabilities, 4-10 per main object,
#       45 edges, 90 anchors, 6 candidates all NOT_RUN), enums, runtime NOT_RUN, the six chains,
#       indirect-call triples, counts block = recount;
#   (4) queries re-run at the pin (rc / total / active, negative_active, edge and indirect hit refs);
#   (5) lexical body checks (structure_checks) on the pinned files;
#   (6) value tables: VT-MR01 key-floor rule arithmetic, VT-FR01 length formula;
#   (7) deferred-findings.yaml fields, statuses, owner_action_now, references into the facts file;
#   (8) mm-04-disposition.md front matter: every supersedes_scope / reviewed_retained fragment is on
#       the stated line of the frozen object de7c96ede546;
#   (9) no 40-hex in the package text or in the outputs.
# ---------------------------------------------------------------------------------------------------
"""Usage: python3 uv_facts_check.py <package_dir> <out.json> [<file.md> ...]
(run from anywhere; queries.json is written next to <out.json>)"""
import sys
sys.dont_write_bytecode = True

import json  # noqa: E402
import os  # noqa: E402
import re  # noqa: E402
import subprocess  # noqa: E402

import yaml  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.normpath(os.path.join(HERE, "..", "..", "mm-baseline-04", "scripts")))
import mm_facts_check as M  # noqa: E402  (read-only reuse, see header)

S = M.S
PIN = S.PIN
REVIEWED = "de7c96ede546"
MM_PKG = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/mm-baseline-04/"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
SYS = re.compile(r"MYOS_SYSCALL_DEFINE\d\s*\(\s*(\w+)")
ORIG_SPAN_NAME = M.span_name
MAIN_OBJECTS = ["sched.forkexec", "fs.vfs"]
OBJECTS = MAIN_OBJECTS + ["boundary"]
CHAINS = ["C1", "C2", "C3", "C4", "C5", "C6"]
SEG_STATUS = {"connected", "connected_with_concern", "connected_under_config_text", "build_text_only", "endpoint", "broken"}
F_STATUS = {"deferred_owner_not_ready", "needs_evidence", "superseded_interpretation"}
F_KINDS = {"source_fact", "conditional_inference", "limited_host_observation", "policy_proposal", "evidence_gap",
           "documentation_erratum"}
LIMITS = {"capabilities": 20, "edges": 45, "anchors": 90, "candidates": 6}


def uv_span_name(a, x):
    n = ORIG_SPAN_NAME(a, x)
    if x["kind"] == "function" and re.match(r"MYOS_SYSCALL_DEFINE\d$", n):
        m = SYS.search(" ".join(a["clines"][x["start"] - 1:x["brace_line"]]))
        if m:
            return "sys_" + m.group(1)
    return n


M.span_name = uv_span_name


def active_text(a, ln):
    return "".join(c if not m else " " for c, m in zip(a["lines"][ln - 1], a["mask"][ln - 1]))


def structure_check(sc):
    a = S.analyse(sc["path"])
    spans = [x for x in a["syms"] if x["kind"] == "function" and uv_span_name(a, x) == sc["symbol"]]
    res = {"id": sc["id"], "path": sc["path"], "symbol": sc["symbol"], "check": sc["check"], "ok": False}
    if len(spans) != 1:
        res["reason"] = "expected exactly one function span, got %d" % len(spans)
        return res
    x = spans[0]
    body = [active_text(a, ln) for ln in range(x["brace_line"] + 1, x["end"])]
    tail = a["lines"][x["brace_line"] - 1]
    brace_rest = tail[tail.find("{") + 1:] if "{" in tail else ""
    body = [brace_rest] + body
    res["span"] = [x["start"], x["brace_line"], x["end"]]
    if sc["check"] == "empty_body":
        res["active_chars"] = sum(len(t.strip()) for t in body)
        res["ok"] = res["active_chars"] == 0
    else:
        n = sum(len(re.findall(sc["regex"], t)) for t in body)
        res["active_matches"] = n
        res["ok"] = n == 0 if sc["check"] == "no_active_match" else n == sc["expected"]
    return res


def vt_mr01(t):
    out = []
    for r in t["rows"]:
        q = [list(x) for x in r["queue_before"]]
        errs = []
        for st in r["steps"]:
            head = q[0][1] if q else None
            key = max(st["own_key"], head) if head is not None else st["own_key"]
            if key != st["expect_key"]:
                errs.append("%s: key %s != expected %s" % (st["task"], key, st["expect_key"]))
            idx = next((i for i, (_, k) in enumerate(q) if k >= key), len(q))
            q.insert(idx, [st["task"], key])
        names = [n for n, _ in q] + ["IDLE"]
        ok = not errs and names == r["expect_queue_after"] and names[0] == r["expect_head"]
        out.append({"row": r["id"], "computed_queue": names, "errors": errs, "ok": ok})
    return out


def vt_fr01(t):
    ps, out = t["page_size"], []
    for r in t["rows"]:
        start, end = r["start"], r["start"] + r["count"]
        pos, lens = start - start % ps, []
        while pos < end:
            inpage = max(pos, start) % ps
            lens.append(ps - inpage if pos + ps < end else end % ps - inpage)
            pos += ps
        ok = lens == r["expect_lens"] and sum(lens) == r["expect_sum_len"]
        out.append({"row": r["id"], "lens": lens, "sum_len": sum(lens), "intended_total": r["intended_total"],
                    "differs_from_intended": sum(lens) != r["intended_total"], "ok": ok})
    return out


def front_matter(path):
    text = open(path, encoding="utf-8").read()
    if not text.startswith("---\n"):
        return None
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


def check_supersedes(md):
    fm = front_matter(md) or {}
    rows = []
    for field in ("supersedes_scope", "reviewed_retained"):
        for s in fm.get(field) or []:
            r = subprocess.run(["git", "-C", S.REPO, "show", "%s:%s%s" % (REVIEWED, MM_PKG, s["path"])], capture_output=True)
            lines = r.stdout.decode("utf-8", "replace").split("\n") if r.returncode == 0 else []
            ok = 1 <= s["line"] <= len(lines) and s["fragment"] in lines[s["line"] - 1]
            rows.append({"field": field, "path": s["path"], "line": s["line"], "action": s.get("action"), "ok": ok})
    return {"total": len(rows), "ok": sum(r["ok"] for r in rows), "failures": [r for r in rows if not r["ok"]],
            "superseded": sum(1 for r in rows if r["field"] == "supersedes_scope"),
            "has_scope": any(r["field"] == "supersedes_scope" for r in rows)}


def main(argv):
    pkg, outp, mds = argv[0], argv[1], argv[2:]
    d = yaml.safe_load(open(os.path.join(pkg, "facts-and-dependencies.yaml"), encoding="utf-8"))
    f = yaml.safe_load(open(os.path.join(pkg, "deferred-findings.yaml"), encoding="utf-8"))
    res = {"check": "USER_VFS_BASELINE_05_FACTS", "pin_time": PIN}
    an = [M.check_anchor(a) for a in d["anchors"]]
    res["anchors"] = {"total": len(an), "ok": sum(r["ok"] for r in an),
                      "by_kind": {k: sum(1 for a in d["anchors"] if a["kind"] == k) for k in sorted(M.KINDS)},
                      "failures": [r for r in an if not r["ok"]]}
    res["markdown_tags"] = {}
    for md in mds:
        rows = M.check_md_tags(md)
        res["markdown_tags"][os.path.basename(md)] = {"total": len(rows), "ok": sum(r["ok"] for r in rows),
                                                     "failures": [r for r in rows if not r["ok"]]}
    # ---- structure ----
    errs, ids = [], {}
    for sec in ("objects", "capabilities", "chains", "edges", "link_breaks", "dependency_risks", "next_validation_candidates",
                "old_inputs_index", "structure_checks", "value_tables", "anchors", "queries", "gaps"):
        for x in d.get(sec) or []:
            if x["id"] in ids:
                errs.append("duplicate id %s (%s, %s)" % (x["id"], ids[x["id"]], sec))
            ids[x["id"]] = sec
    anc = {a["id"] for a in d["anchors"]}
    qid = {q["id"] for q in d["queries"]}
    scs = {s["id"] for s in d["structure_checks"]}
    vts = {v["id"] for v in d["value_tables"]}
    caps = {c["id"]: c for c in d["capabilities"]}
    eby = {e["id"]: e for e in d["edges"]}
    fids = {x["id"] for x in f["findings"]}
    ev_pools = (("anchors", anc), ("queries", qid), ("structure_checks", scs), ("value_tables", vts))

    def refs(owner, obj):
        for k, pool in ev_pools:
            for r in obj.get(k) or []:
                if r not in pool:
                    errs.append("%s.%s -> missing %s" % (owner, k, r))

    for c in d["capabilities"]:
        refs(c["id"], c)
        for r in c["edges"]:
            if r not in eby:
                errs.append("%s.edges -> missing %s" % (c["id"], r))
        for r in c["findings"]:
            if r not in fids:
                errs.append("%s.findings -> missing %s" % (c["id"], r))
        for ch in c["chains"]:
            if ch not in CHAINS:
                errs.append("%s chain %s" % (c["id"], ch))
        if c["id_status"] != "proposed_id" or c["object"] not in OBJECTS:
            errs.append("%s id_status/object" % c["id"])
        ie, ce, cc = c["implementation_evidence"], c["correctness_evidence"], c["concurrency_evidence"]
        if ie["value"] not in M.IMPL or not ie.get("reason"):
            errs.append("%s implementation_evidence" % c["id"])
        if ce["value"] not in M.CORR or (ce["value"] == "static_concern" and not c["findings"]):
            errs.append("%s correctness_evidence" % c["id"])
        if c["runtime_evidence"] != "NOT_RUN":
            errs.append("%s runtime_evidence" % c["id"])
        if cc["value"] not in M.CONC:
            errs.append("%s concurrency_evidence" % c["id"])
        for k in ("purpose", "entries", "normal_path", "failure_path", "source_condition", "not_covered", "anchors"):
            if not c.get(k):
                errs.append("%s missing %s" % (c["id"], k))
        if c["source_condition"].get("resolution") not in ("not_conditional", "resolved", "partially_resolved", "unknown"):
            errs.append("%s source_condition.resolution" % c["id"])
    for e in d["edges"]:
        if e["kind"] not in M.EDGE_KINDS:
            errs.append("%s kind" % e["id"])
        if not e.get("anchors") and not e.get("hits"):
            errs.append("%s has no evidence" % e["id"])
        refs(e["id"], e)
        for end in ("from", "to"):
            if e[end] not in caps and e[end] not in OBJECTS and not e[end].startswith("external:"):
                errs.append("%s %s endpoint %s" % (e["id"], end, e[end]))
            if e[end] in caps and e["id"] not in caps[e[end]]["edges"]:
                errs.append("%s not listed by endpoint %s" % (e["id"], e[end]))
        for s in e.get("also_applies_to") or []:
            if s not in caps and s not in OBJECTS:
                errs.append("%s also_applies_to %s" % (e["id"], s))
            if s in caps and e["id"] not in caps[s]["edges"]:
                errs.append("%s not listed by also_applies_to %s" % (e["id"], s))
        if "indirect" in e:
            for k in ("call_site", "binding", "receiver"):
                v = e["indirect"].get(k)
                if v is None or (isinstance(v, str) and v not in anc):
                    errs.append("%s indirect.%s missing or unresolved" % (e["id"], k))
    for c in d["capabilities"]:
        for r in c["edges"]:
            e = eby.get(r)
            if e and c["id"] not in (e["from"], e["to"]) and c["id"] not in (e.get("also_applies_to") or []):
                errs.append("%s lists %s but is not an endpoint" % (c["id"], r))
    chains = {x["id"]: x for x in d["chains"]}
    if sorted(chains) != CHAINS:
        errs.append("chains are not exactly C1..C6")
    for ch in d["chains"]:
        for k in ("title", "entry", "capabilities", "normal_path", "failure_cleanup", "evidence", "coverage_limits", "segments"):
            if not ch.get(k):
                errs.append("%s missing %s" % (ch["id"], k))
        refs(ch["id"], ch["evidence"])
        for c in ch["capabilities"]:
            if c not in caps:
                errs.append("%s capability %s" % (ch["id"], c))
        for sg in ch["segments"]:
            if sg["status"] not in SEG_STATUS:
                errs.append("%s segment status %s" % (ch["id"], sg["status"]))
            for r in sg["evidence"]:
                if r not in anc and r not in qid and r not in scs:
                    errs.append("%s segment evidence %s" % (ch["id"], r))
    for b in d["link_breaks"]:
        if not (b.get("anchors") or b.get("queries") or b.get("structure_checks")):
            errs.append("%s without evidence" % b["id"])
        refs(b["id"], b)
    for r in d["dependency_risks"]:
        for k in ("depends_on", "affects", "what", "evidence_class", "unknown"):
            if not r.get(k):
                errs.append("%s missing %s" % (r["id"], k))
        for a in r["affects"]:
            if a not in caps:
                errs.append("%s affects %s" % (r["id"], a))
    for v in d["next_validation_candidates"]:
        if v.get("status") != "NOT_RUN":
            errs.append("%s status" % v["id"])
    for o in d["old_inputs_index"]:
        for m in o["mapped_to"]:
            if m not in caps and m not in OBJECTS:
                errs.append("%s mapped_to %s" % (o["id"], m))
    per = {o: sum(1 for c in d["capabilities"] if c["object"] == o) for o in OBJECTS}
    limits = {"capabilities": [len(d["capabilities"]), 20], "edges": [len(d["edges"]), 45],
              "anchors": [len(d["anchors"]), 90], "candidates": [len(d["next_validation_candidates"]), 6],
              "per_object": per}
    if (len(d["capabilities"]) > 20 or len(d["edges"]) > 45 or len(d["anchors"]) > 90
            or len(d["next_validation_candidates"]) > 6 or not all(4 <= per[o] <= 10 for o in MAIN_OBJECTS)):
        errs.append("limit exceeded %s" % limits)
    for o in MAIN_OBJECTS:
        if not any(c["object"] == o for c in d["capabilities"]):
            errs.append("object %s has no capability" % o)
    impl = {}
    for c in d["capabilities"]:
        impl[c["implementation_evidence"]["value"]] = impl.get(c["implementation_evidence"]["value"], 0) + 1
    recount = {"objects": len(d["objects"]), "capabilities": len(d["capabilities"]), "capabilities_per_object": per,
               "anchors": len(d["anchors"]), "anchors_by_kind": res["anchors"]["by_kind"],
               "edges": len(d["edges"]), "edges_by_kind": {k: sum(1 for e in d["edges"] if e["kind"] == k) for k in sorted(M.EDGE_KINDS)},
               "queries": len(d["queries"]), "structure_checks": len(d["structure_checks"]), "value_tables": len(d["value_tables"]),
               "chains": len(d["chains"]), "link_breaks": len(d["link_breaks"]), "dependency_risks": len(d["dependency_risks"]),
               "next_validation_candidates": len(d["next_validation_candidates"]), "gaps": len(d["gaps"]),
               "gaps_open": sum(1 for g in d["gaps"] if g["status"] == "open"), "old_inputs_index": len(d["old_inputs_index"]),
               "implementation_evidence": {k: impl.get(k, 0) for k in sorted(M.IMPL)}}
    if d.get("counts") != recount:
        errs.append("counts block differs from recount")
    res["structure"] = {"errors": errs, "limits": limits, "recount": recount, "ok": not errs}
    # ---- deferred findings ----
    ferr = []
    if f.get("owner_action_now") != "none" or f.get("owner_decisions_requested_now") != []:
        ferr.append("header owner_action_now / owner_decisions_requested_now")
    if len(f.get("resume_order") or []) != 5 or not f.get("empty_set_statement"):
        ferr.append("resume_order or empty_set_statement")
    for x in f["findings"]:
        for k in ("qualified_id", "source_ref", "kind", "status", "owner_action_now", "claim", "premises", "impact",
                  "counter_evidence", "unknown", "applicable_source", "evidence", "future_verification",
                  "reopen_conditions", "blocks", "capabilities"):
            if x.get(k) in (None, "", []) and not (k in ("counter_evidence", "unknown") and x.get(k) == []):
                ferr.append("%s missing %s" % (x["id"], k))
        if x["qualified_id"] != "CORE-USER-VFS-BASELINE-05::" + x["id"]:
            ferr.append("%s qualified_id" % x["id"])
        if x["status"] not in F_STATUS or x["owner_action_now"] != "none" or not set(x["kind"]) <= F_KINDS:
            ferr.append("%s status/owner_action_now/kind" % x["id"])
        for k, pool in ev_pools:
            for r in x["evidence"].get(k) or []:
                if r not in pool:
                    ferr.append("%s evidence %s -> missing %s" % (x["id"], k, r))
        for c in x["capabilities"]:
            if c not in caps:
                ferr.append("%s capability %s" % (x["id"], c))
        for cand in x["future_verification"]["candidates"]:
            if cand not in ids:
                ferr.append("%s candidate %s" % (x["id"], cand))
    for c in d["capabilities"]:
        for r in c["findings"]:
            fx = next((x for x in f["findings"] if x["id"] == r), None)
            if fx and c["id"] not in fx["capabilities"]:
                ferr.append("%s lists %s but the finding does not list it" % (c["id"], r))
    for x in f["errata"]:
        if x["kind"] != ["documentation_erratum"] or x["status"] not in F_STATUS or x["owner_action_now"] != "none":
            ferr.append("%s erratum fields" % x["qualified_id"])
    fcount = {"findings": len(f["findings"]), "errata": len(f["errata"]), "status_changes": len(f["status_changes"]),
              "cross_links": len(f["cross_links"]),
              "findings_by_status": {s: sum(1 for x in f["findings"] if x["status"] == s) for s in ("deferred_owner_not_ready", "needs_evidence")}}
    if f.get("counts") != fcount:
        ferr.append("deferred-findings counts differ from recount")
    res["deferred_findings"] = {"errors": ferr, "recount": fcount, "ok": not ferr}
    # ---- queries ----
    qrecs = [M.run_query(q) for q in d["queries"]]
    by = {r["id"]: r for r in qrecs}
    qerr = []
    for q in d["queries"]:
        r = by[q["id"]]
        if r.get("tool_failure"):
            qerr.append("%s tool failure rc=%s" % (q["id"], r["returncode"]))
            continue
        if q.get("type") == "size":
            if [h["bytes"] for h in r["hits"]] != q["expected_bytes"]:
                qerr.append("%s sizes" % q["id"])
            continue
        if (r["returncode"], r["hits_total"], r["hits_active"]) != (q["expected_rc"], q["expected_hits_total"], q["expected_hits_active"]):
            qerr.append("%s rc/total/active %s/%s/%s" % (q["id"], r["returncode"], r["hits_total"], r["hits_active"]))
        if q["expect"] == "negative_active" and r["hits_active"]:
            qerr.append("%s expected no active hit" % q["id"])

    def hit_ok(h):
        r = by.get(h["query"])
        return bool(r) and any(x["path"] == h["path"] and x["line"] == h["line"] and x["active"]
                               and x["enclosing"] == h["enclosing"] for x in r["hits"])

    for e in d["edges"]:
        hs = list(e.get("hits") or []) + [v for v in (e.get("indirect") or {}).values() if isinstance(v, dict)]
        for h in hs:
            if not hit_ok(h):
                qerr.append("%s hit %s:%s not an active hit of %s" % (e["id"], h["path"], h["line"], h["query"]))
    res["queries"] = {"total": len(qrecs), "tool_failures": sum(1 for r in qrecs if r.get("tool_failure")),
                      "errors": qerr, "ok": not qerr}
    # ---- lexical body checks and value tables ----
    scr = [structure_check(s) for s in d["structure_checks"]]
    res["structure_checks"] = {"total": len(scr), "ok": sum(r["ok"] for r in scr), "rows": scr}
    vt = {v["id"]: v for v in d["value_tables"]}
    m1, f1 = vt_mr01(vt["VT-MR01"]), vt_fr01(vt["VT-FR01"])
    res["value_tables"] = {"VT-MR01": m1, "VT-FR01": f1, "ok": all(x["ok"] for x in m1 + f1)}
    # ---- U00 supersedes_scope ----
    disp = os.path.join(pkg, "mm-04-disposition.md")
    res["supersedes_scope"] = check_supersedes(disp) if os.path.exists(disp) else {"total": 0, "ok": 0, "failures": [], "has_scope": False}
    # ---- 40-hex in package text ----
    hexhits = []
    for root, _, files in os.walk(pkg):
        for name in files:
            p = os.path.join(root, name)
            if HEX40.search(open(p, encoding="utf-8", errors="replace").read()):
                hexhits.append(os.path.relpath(p, pkg))
    res["hex40_in_package"] = hexhits
    res["all_ok"] = (res["anchors"]["ok"] == res["anchors"]["total"] and res["structure"]["ok"]
                     and res["deferred_findings"]["ok"] and res["queries"]["ok"]
                     and res["structure_checks"]["ok"] == res["structure_checks"]["total"] and res["value_tables"]["ok"]
                     and res["supersedes_scope"]["has_scope"] and res["supersedes_scope"]["ok"] == res["supersedes_scope"]["total"]
                     and all(v["ok"] == v["total"] for v in res["markdown_tags"].values()) and not hexhits)
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
                      "markdown_tags": {k: [v["ok"], v["total"]] for k, v in res["markdown_tags"].items()},
                      "structure_errors": len(errs), "deferred_findings_errors": len(ferr), "query_errors": len(qerr),
                      "structure_checks": [res["structure_checks"]["ok"], res["structure_checks"]["total"]],
                      "value_tables_ok": res["value_tables"]["ok"],
                      "supersedes_scope": [res["supersedes_scope"]["ok"], res["supersedes_scope"]["total"]],
                      "hex40_in_package": len(hexhits), "all_ok": res["all_ok"]}, ensure_ascii=False))
    return 0 if res["all_ok"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
