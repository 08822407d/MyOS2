"""V01: structure and reference integrity of the frozen technical inputs (taskbook pin).

Parses 07 YAML and the front matter of 07/09/10/11/MANIFEST with PyYAML; checks CA-01..CA-07,
A-anchor references against the report's actual tag set, V-case references against the 09 headings,
referenced paths, the MANIFEST self_check arithmetic over its own six scope_files, completion flags,
40-hex runs, and the protocol-P9 wording list. Old-batch denominators use only MANIFEST scope_files.
"""
import os
import re

import yaml

import harness as H
import v00_anchors as V0

LEAD = "agent-workspace/lead/MYOS2-LEAD-002/"
FILES = {"report": LEAD + "07-scheduler-wakeup-timer-audit.md", "map": LEAD + "07-core-audit-map.yaml",
         "contract09": LEAD + "09-local-verification-contract.md", "pilot10": LEAD + "10-cloud-pilot-and-github-handoff.md",
         "core11": LEAD + "11-core-verification-cloud.md", "manifest": LEAD + "MANIFEST.md"}
BANNED = ["可直接" + "编译", "可直接" + "运行", "已" + "验证", "已" + "测试"]


def front(text):
    assert text.startswith("---\n")
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


def refs_in(obj, pat):
    out = []
    if isinstance(obj, dict):
        for v in obj.values():
            out += refs_in(v, pat)
    elif isinstance(obj, list):
        for v in obj:
            out += refs_in(v, pat)
    elif isinstance(obj, str):
        out += re.findall(pat, obj)
    return out


def main():
    res = {"check": "V01", "taskbook_short12": H.short12("taskbook"), "parse": {}}
    txt = {k: H.blob("taskbook", p).decode("utf-8") for k, p in FILES.items()}
    parsed = {}
    for k, t in txt.items():
        try:
            parsed[k] = yaml.safe_load(t) if k == "map" else front(t)
            res["parse"][k] = {"ok": isinstance(parsed[k], dict), "top_keys": len(parsed[k])}
        except Exception as e:  # noqa: BLE001
            res["parse"][k] = {"ok": False, "error": "%s: %s" % (type(e).__name__, e)}
    m = parsed["map"]
    report_ids = [a["id"] for a in V0.parse_report(txt["report"])]
    v_ids = re.findall(r"^### (V\d\d)｜", txt["contract09"], re.M)
    res["contract09_case_ids"] = v_ids
    issues = m.get("issues", [])
    ids = [i["issue_id"] for i in issues]
    res["issue_ids"] = ids
    res["issue_ids_expected_CA01_CA07"] = ids == ["CA-%02d" % k for k in range(1, 8)]
    a_in_issues = [a for i in issues for a in i.get("anchors", [])]
    a_in_rel = [a for r in m.get("relations", []) for a in r.get("anchors", [])]
    res["anchor_refs"] = {
        "in_issues": len(a_in_issues), "in_relations": len(a_in_rel),
        "dangling": sorted(set(a_in_issues + a_in_rel) - set(report_ids)),
        "report_anchors_not_referenced_by_any_issue": sorted(set(report_ids) - set(a_in_issues)),
        "duplicates_within_an_issue": {i["issue_id"]: [a for a in set(i.get("anchors", []))
                                                      if i["anchors"].count(a) > 1] for i in issues
                                       if len(set(i.get("anchors", []))) != len(i.get("anchors", []))}}
    lc = [c for i in issues for c in i.get("local_cases", [])]
    res["case_refs"] = {"dangling": sorted(set(lc) - set(v_ids)),
                        "cases_not_referenced_by_any_issue": sorted(set(v_ids) - set(lc))}
    # 09 text references to anchors (Axx / Axx-Ayy ranges) must exist
    rng = []
    for a, b in re.findall(r"A(\d\d)-A(\d\d)", txt["contract09"]):
        rng += ["A%02d" % k for k in range(int(a), int(b) + 1)]
    singles = re.findall(r"(?<![\w-])(A\d\d)(?![\d-])", txt["contract09"])
    res["contract09_anchor_refs"] = sorted(set(rng + singles))
    res["contract09_anchor_refs_dangling"] = sorted(set(rng + singles) - set(report_ids))
    # referenced paths
    paths = {"map.report": m.get("report"), "map.validation_contract": m.get("validation_contract")}
    for i in issues:
        if i.get("old_claim_ref"):
            paths["map.%s.old_claim_ref" % i["issue_id"]] = i["old_claim_ref"].split()[0]
    res["referenced_paths"] = {k: {"path": p, "exists_taskbook": H.exists("taskbook", p)} for k, p in paths.items()}
    ir = parsed["report"].get("inputs_read", [])
    res["report_inputs_read"] = [{"path": p, "exists": H.exists("time", p) if not p.startswith("agent-workspace/")
                                  else H.exists("taskbook", p)} for p in ir if isinstance(p, str) and "/" in p and " " not in p]
    # MANIFEST self_check over its own six scope files
    man = parsed["manifest"]
    sc = man.get("self_check", {})
    counts = {}
    for f in sc.get("scope_files", []):
        p = LEAD + f
        counts[f] = H.blob("taskbook", p).decode("utf-8").count("[" + "VERIFIED") if H.exists("taskbook", p) else "MISSING"
    res["manifest_self_check"] = {
        "scope_files": sc.get("scope_files"), "tag_counts_per_scope_file": counts,
        "automated_total": sum(v for v in counts.values() if isinstance(v, int)),
        "declared_verified_claims": sc.get("verified_claims"),
        "declared_reconfirmed_plus_downgraded": (sc.get("quotes_reconfirmed", 0) or 0) + (sc.get("downgraded_to_inferred", 0) or 0),
        "map_expected_quote_count": (m.get("validation") or {}).get("source_quote_count_in_report_expected"),
    }
    msc = res["manifest_self_check"]
    msc["automated_equals_declared"] = msc["automated_total"] == msc["declared_verified_claims"]
    msc["arithmetic_ok"] = msc["declared_reconfirmed_plus_downgraded"] == msc["declared_verified_claims"]
    conv = H.blob("master", "agent-workspace/conventions.md").decode("utf-8")
    res["startup_selfcheck_quote_in_master_conventions"] = man.get("startup_selfcheck_quote", "\0") in conv
    res["completion_flags"] = {k: man.get(k) for k in ("whole_002R_complete", "whole_003R_complete",
                                                       "whole_007R_complete", "whole_wave2_complete",
                                                       "acceptance_verdict", "kernel_modified")}
    res["completion_flags"]["map.replacement_for_002R_003R_007R"] = m.get("replacement_for_002R_003R_007R")
    scope_all = [LEAD + f for f in sc.get("scope_files", [])]
    res["hex40_hits_in_scope_files"] = {p.split("/")[-1]: len(H.HEX40.findall(H.blob("taskbook", p)))
                                        for p in scope_all if H.exists("taskbook", p)}
    res["p9_wording_hits_in_scope_files"] = {}
    for p in scope_all:
        if not H.exists("taskbook", p):
            continue
        lines = H.blob("taskbook", p).decode("utf-8").split("\n")
        hits = [{"line": n + 1, "phrase": b} for n, l in enumerate(lines) for b in BANNED if b in l]
        if hits:
            res["p9_wording_hits_in_scope_files"][p.split("/")[-1]] = hits
    H.emit(res, os.path.join(H.WORK, "v01.json"), echo=False)
    print(H.dump({k: res[k] for k in ("parse", "issue_ids_expected_CA01_CA07", "anchor_refs", "case_refs",
                                      "contract09_anchor_refs_dangling", "manifest_self_check",
                                      "startup_selfcheck_quote_in_master_conventions", "completion_flags",
                                      "hex40_hits_in_scope_files", "p9_wording_hits_in_scope_files")}))


if __name__ == "__main__":
    main()
