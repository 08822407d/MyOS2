# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: binding and protection gate for this round. It checks (a) the recheck-01 review receipt and
#   the 13 contract at the pinned lead object, (b) that the lead branch only appended files since the
#   previous pin, (c) the recheck-01 commit chain a1e7 -> f9ae -> efb -> b843 and that efb -> b843 only
#   added files, (d) that HEAD, the index and the work tree differ from b843 only by additions under
#   consumer-fix-02/ (so every b843 file is byte-identical), (e) the remote branch relation. The frozen
#   recheck-01 identity2 is run from a b843 extraction as an auxiliary check of the pilot/core originals
#   only; it does not replace (d). Read-only Git and ls-remote; no C, no H00, no tree scan.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 guard3.py <frozen extraction dir> <out.json>   (exit 0 gate open, 3 gate closed)"""
import datetime
import os
import sys

import yaml

import common3 as C

REVIEW = C.LEAD + "reviews/CORE-CHECK-01-recheck-01-review.md"
CONTRACT13 = C.LEAD + "13-core-evidence-consumer-recheck.md"
UNCHANGED_SINCE_PREVIOUS_LEAD = ["12-core-check-recheck-contract.md", "reviews/CORE-CHECK-01-core-review.md",
                                 "reviews/CORE-CHECK-01-pilot-review.md", "07-scheduler-wakeup-timer-audit.md",
                                 "07-core-audit-map.yaml", "MANIFEST.md", "09-local-verification-contract.md",
                                 "10-cloud-pilot-and-github-handoff.md", "11-core-verification-cloud.md"]
TECH = ["07-scheduler-wakeup-timer-audit.md", "07-core-audit-map.yaml", "MANIFEST.md", "09-local-verification-contract.md",
        "10-cloud-pilot-and-github-handoff.md", "11-core-verification-cloud.md"]


def front(data):
    t = data.decode("utf-8")
    return yaml.safe_load(t[4:t.index("\n---\n", 4)])


def name_status(a, b=None, cached=False):
    args = ["diff", "--name-status"] + (["--cached"] if cached else []) + [C.full(a)] + ([C.full(b)] if b else [])
    return [x for x in C.git(*args).stdout.decode().split("\n") if x]


def check(frozen_dir):
    s = lambda k: C.short12(C.PINS[k])
    rec = {"check": "GUARD3", "read_time_utc": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
           "pins": {k: s(k) for k in C.PINS}, "gate": {}}
    g = rec["gate"]
    rv = front(C.blob(C.PINS["lead_recheck02"], REVIEW))
    c13 = front(C.blob(C.PINS["lead_recheck02"], CONTRACT13))
    rec["review_fields"] = {k: rv.get(k) for k in ("record_id", "disposition", "reviewed_pr", "reviewed_branch", "reviewed_commit_short12",
                                                   "results_frozen_short12", "first_batch_short12", "previous_core_short12",
                                                   "followup_id", "followup_allowed_write_prefix")}
    g["review_receipt_binding"] = (rv.get("record_id") == "CORE-CHECK-01-RECHECK-01-REVIEW-001" and rv.get("packet_id") == C.PACKET
                                   and rv.get("reviewed_pr") == 17 and rv.get("reviewed_branch") == C.WORK_BRANCH
                                   and str(rv.get("reviewed_commit_short12")) == s("reviewed_input")
                                   and str(rv.get("results_frozen_short12")) == s("frozen_results")
                                   and str(rv.get("first_batch_short12")) == s("batch1")
                                   and str(rv.get("previous_core_short12")) == s("core_frozen")
                                   and rv.get("followup_id") == C.FOLLOWUP and rv.get("followup_allowed_write_prefix") == C.PREFIX)
    rec["contract13_fields"] = {k: c13.get(k) for k in ("followup_id", "required_review_record", "execution_branch", "execution_pr",
                                                        "reviewed_input_short12", "frozen_results_short12", "allowed_write_prefix",
                                                        "old_files_must_remain_unchanged", "kernel_fixture_execution_authorized",
                                                        "full_tree_scan_authorized")}
    g["contract13_binding"] = (c13.get("followup_id") == C.FOLLOWUP and c13.get("required_review_record") == rv.get("record_id")
                               and c13.get("execution_branch") == C.WORK_BRANCH and c13.get("execution_pr") == 17
                               and str(c13.get("reviewed_input_short12")) == s("reviewed_input")
                               and str(c13.get("frozen_results_short12")) == s("frozen_results")
                               and c13.get("allowed_write_prefix") == C.PREFIX and c13.get("old_files_must_remain_unchanged") is True
                               and c13.get("kernel_fixture_execution_authorized") is False and c13.get("full_tree_scan_authorized") is False)
    # (b) lead branch only appended since the previous pin; technical inputs unchanged since the taskbook
    lead_diff = name_status(C.PINS["lead_recheck01"], C.PINS["lead_recheck02"])
    rec["lead_changes_since_previous_pin"] = [x.replace(C.LEAD, "") for x in lead_diff]
    g["lead_advanced_by_additions_only"] = (C.is_ancestor(C.PINS["lead_recheck01"], C.PINS["lead_recheck02"])
                                            and all(x.startswith("A\t" + C.LEAD) for x in lead_diff))
    same_prev = {f: C.blob(C.PINS["lead_recheck01"], C.LEAD + f) == C.blob(C.PINS["lead_recheck02"], C.LEAD + f)
                 for f in UNCHANGED_SINCE_PREVIOUS_LEAD}
    same_tb = {f: C.blob(C.PINS["taskbook"], C.LEAD + f) == C.blob(C.PINS["lead_recheck02"], C.LEAD + f) for f in TECH}
    rec["inputs_unchanged"] = {"since_previous_lead_pin": same_prev, "technical_since_taskbook": same_tb}
    g["inputs_unchanged"] = all(same_prev.values()) and all(same_tb.values())
    # (c) recheck-01 commit chain and the efb -> b843 relation
    eb = name_status(C.PINS["frozen_results"], C.PINS["reviewed_input"])
    rec["efb_to_b843"] = [x.replace(C.R01, "") for x in eb]
    ry = C.R01 + "results.yaml"
    g["results_chain"] = (C.is_ancestor(C.PINS["core_frozen"], C.PINS["batch1"]) and C.is_ancestor(C.PINS["batch1"], C.PINS["frozen_results"])
                          and C.is_ancestor(C.PINS["frozen_results"], C.PINS["reviewed_input"])
                          and len(eb) == 6 and all(x.startswith("A\t" + C.R01) for x in eb)
                          and C.blob(C.PINS["frozen_results"], ry) == C.blob(C.PINS["reviewed_input"], ry))
    # (d) execution branch, HEAD, index and work tree versus b843
    branch = C.git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    rec["execution"] = {"branch": branch, "head_short12": C.short12("HEAD")}
    g["on_execution_branch"] = branch == C.WORK_BRANCH
    g["head_descends_from_b843"] = C.is_ancestor(C.PINS["reviewed_input"], "HEAD")
    committed = name_status(C.PINS["reviewed_input"], "HEAD")
    staged = name_status(C.PINS["reviewed_input"], cached=True)
    st = [x for x in C.git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n") if x]
    rec["b843_files"] = {"under_recheck01": len(C.tree(C.PINS["reviewed_input"], C.R01)),
                         "under_results_root": len(C.tree(C.PINS["reviewed_input"], C.RESULTS_ROOT)),
                         "whole_tree": len(C.tree(C.PINS["reviewed_input"]))}
    rec["changes_vs_b843"] = {"committed": [x.replace(C.PREFIX, "<cf02>/") for x in committed],
                              "staged_not_additions_under_prefix": [x for x in staged if not x.startswith("A\t" + C.PREFIX)],
                              "worktree_outside_prefix": [x for x in st if not x[3:].startswith(C.PREFIX)]}
    g["b843_files_unchanged_only_additions_under_prefix"] = (all(x.startswith("A\t" + C.PREFIX) for x in committed)
                                                            and not rec["changes_vs_b843"]["staged_not_additions_under_prefix"]
                                                            and not rec["changes_vs_b843"]["worktree_outside_prefix"])
    # (e) remote branch relation
    rb = C.git("ls-remote", "origin", "refs/heads/" + C.WORK_BRANCH).stdout.decode().split("\t")[0].strip()
    known = bool(rb) and C.git("cat-file", "-e", rb + "^{commit}", check=False).returncode == 0
    head = C.full("HEAD")
    rel = "unknown"
    if known:
        rel = ("equal" if rb == head else "remote_is_ancestor_of_head" if C.is_ancestor(rb, head)
               else "remote_is_descendant_of_head" if C.is_ancestor(head, rb) else "diverged")
    rec["execution"]["remote_relation"] = rel
    g["remote_on_same_line_and_contains_b843"] = rel in ("equal", "remote_is_ancestor_of_head") and C.is_ancestor(C.PINS["reviewed_input"], rb)
    rec["remote_refs_equal_pins"] = {}
    for name, key in (("master", "master"), ("time", "time"), ("agent/MYOS2-LEAD-002", "lead_recheck02")):
        r = C.git("ls-remote", "origin", "refs/heads/" + name).stdout.decode().split("\t")[0].strip()
        rec["remote_refs_equal_pins"][name] = (r == C.pin(key)) if r else None
    g["kernel_source_branch_equals_pin"] = rec["remote_refs_equal_pins"]["time"] is True
    # auxiliary: frozen recheck-01 identity2 from the b843 extraction (pilot/core originals vs a1e7)
    code = ("import json, identity2 as I; r = I.safe_check(); "
            "print(json.dumps({'gate': r.get('gate'), 'error': r.get('error'), 'originals': r.get('originals')}))")
    aux = C.run_py(["-c", code], cwd=os.path.join(frozen_dir, "fixtures"), timeout=60)
    rec["auxiliary_frozen_identity2"] = C.last_json(aux["stdout"]) or {"error": C.tail(aux["stderr"]), "returncode": aux["returncode"]}
    rec["auxiliary_frozen_identity2_note"] = "protects pilot/core originals against a1e7c2277705 only; b843 files are covered by gate b843_files_unchanged_only_additions_under_prefix"
    g["auxiliary_identity2_open"] = ((rec["auxiliary_frozen_identity2"] or {}).get("gate") or {}).get("ok") is True
    g["ok"] = all(v for k, v in g.items() if k != "ok")
    return rec


if __name__ == "__main__":
    try:
        r = check(sys.argv[1])
    except Exception as e:  # noqa: BLE001 - any failure closes the gate
        r = {"check": "GUARD3", "gate": {"ok": False}, "error": "%s: %s" % (type(e).__name__, e)}
    C.emit(r, sys.argv[2])
    print(C.dump(r["gate"]))
    sys.exit(0 if r["gate"].get("ok") else 3)
