# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file (own binding for this task; consumer-fix-02/fixtures/guard3.py @ 0d62c4d19711 is
#   not widened or reused)
# purpose: before compiling and before pushing: the RECHECK-02 closing review and the 15 contract at the
#   pinned lead object; the lead only appended since b0aa54db1b70; 77faf51e9a43 -> 0d62c4d19711 only
#   added files; HEAD, index and work tree differ from 0d62 only by additions under scheduler-order-02/
#   (so every 0d62 file is byte-identical); time still equals a039d9803ade; the remote branch is on the
#   same line and contains 0d62. Read-only Git and ls-remote.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 guard4.py <out.json>   (exit 0 gate open, 3 gate closed)"""
import datetime
import sys

import yaml

import common4 as C

REVIEW = C.LEAD + "reviews/CORE-CHECK-01-recheck-02-review.md"
CONTRACT = C.LEAD + "15-scheduler-ordering-witness-contract.md"
ANALYSIS = C.LEAD + "14-scheduler-ordering-followup.md"


def front(data):
    t = data.decode("utf-8")
    return yaml.safe_load(t[4:t.index("\n---\n", 4)])


def ns(a, b=None, cached=False):
    args = ["diff", "--name-status"] + (["--cached"] if cached else []) + [C.full(a)] + ([C.full(b)] if b else [])
    return [x for x in C.git(*args).stdout.decode().split("\n") if x]


def check():
    s = lambda k: C.short12(C.PINS[k])
    rec = {"check": "GUARD4", "read_time_utc": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
           "pins": {k: s(k) for k in C.PINS}, "gate": {}}
    g = rec["gate"]
    rv = front(C.blob(C.PINS["lead"], REVIEW))
    ct = front(C.blob(C.PINS["lead"], CONTRACT))
    rec["review_fields"] = {k: rv.get(k) for k in ("record_id", "status", "acceptance_verdict", "reviewed_pr", "reviewed_branch",
                                                   "reviewed_commit_short12", "results_frozen_short12", "remaining_return_items_in_contract13")}
    g["review_binding"] = (rv.get("record_id") == "CORE-CHECK-01-RECHECK-02-REVIEW-001" and rv.get("packet_id") == C.PACKET
                           and rv.get("reviewed_pr") == 17 and rv.get("reviewed_branch") == C.WORK_BRANCH
                           and str(rv.get("reviewed_commit_short12")) == s("reviewed")
                           and str(rv.get("results_frozen_short12")) == s("consumer_results")
                           and rv.get("acceptance_verdict") == "PASS_PENDING_LOCAL" and rv.get("status") == "BOUNDED_RECHECK_CLOSED"
                           and rv.get("remaining_return_items_in_contract13") == [])
    rec["contract_fields"] = {k: ct.get(k) for k in ("followup_id", "required_review_record", "reviewed_execution_short12",
                                                     "consumer_results_frozen_short12", "kernel_short12", "execution_branch",
                                                     "initial_execution_pr", "allowed_write_prefix", "kernel_change_authorized",
                                                     "phase3_authorized", "acceptance_ceiling")}
    g["contract_binding"] = (ct.get("followup_id") == C.FOLLOWUP and ct.get("packet_id") == C.PACKET
                             and ct.get("required_review_record") == rv.get("record_id")
                             and str(ct.get("reviewed_execution_short12")) == s("reviewed")
                             and str(ct.get("consumer_results_frozen_short12")) == s("consumer_results")
                             and str(ct.get("kernel_short12")) == s("time") and ct.get("execution_branch") == C.WORK_BRANCH
                             and ct.get("initial_execution_pr") == 17 and ct.get("allowed_write_prefix") == C.PREFIX
                             and ct.get("kernel_change_authorized") is False)
    g["analysis14_present"] = C.git("cat-file", "-e", "%s:%s" % (C.full(C.PINS["lead"]), ANALYSIS), check=False).returncode == 0
    ld = ns(C.PINS["lead_prev"], C.PINS["lead"])
    rec["lead_changes_since_previous_pin"] = [x.replace(C.LEAD, "") for x in ld]
    g["lead_advanced_by_additions_only"] = C.is_ancestor(C.PINS["lead_prev"], C.PINS["lead"]) and all(x.startswith("A\t" + C.LEAD) for x in ld)
    cc = ns(C.PINS["consumer_results"], C.PINS["reviewed"])
    rec["consumer_results_to_reviewed"] = [x.replace(C.RESULTS_ROOT, "") for x in cc]
    g["reviewed_chain"] = C.is_ancestor(C.PINS["consumer_results"], C.PINS["reviewed"]) and all(x.startswith("A\t") for x in cc)
    branch = C.git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    rec["execution"] = {"branch": branch, "head_short12": C.short12("HEAD")}
    g["on_execution_branch"] = branch == C.WORK_BRANCH
    g["head_descends_from_0d62"] = C.is_ancestor(C.PINS["reviewed"], "HEAD")
    committed = ns(C.PINS["reviewed"], "HEAD")
    staged = ns(C.PINS["reviewed"], cached=True)
    st = [x for x in C.git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n") if x]
    rec["frozen_0d62_files"] = {"under_results_root": len(C.tree(C.PINS["reviewed"], C.RESULTS_ROOT)), "whole_tree": len(C.tree(C.PINS["reviewed"]))}
    rec["changes_vs_0d62"] = {"committed": [x.replace(C.PREFIX, "<so02>/") for x in committed],
                              "staged_not_additions_under_prefix": [x for x in staged if not x.startswith("A\t" + C.PREFIX)],
                              "worktree_outside_prefix": [x for x in st if not x[3:].startswith(C.PREFIX)]}
    g["0d62_files_unchanged_only_additions_under_prefix"] = (all(x.startswith("A\t" + C.PREFIX) for x in committed)
                                                            and not rec["changes_vs_0d62"]["staged_not_additions_under_prefix"]
                                                            and not rec["changes_vs_0d62"]["worktree_outside_prefix"])
    rb = C.git("ls-remote", "origin", "refs/heads/" + C.WORK_BRANCH).stdout.decode().split("\t")[0].strip()
    known = bool(rb) and C.git("cat-file", "-e", rb + "^{commit}", check=False).returncode == 0
    head = C.full("HEAD")
    rel = "unknown"
    if known:
        rel = ("equal" if rb == head else "remote_is_ancestor_of_head" if C.is_ancestor(rb, head)
               else "remote_is_descendant_of_head" if C.is_ancestor(head, rb) else "diverged")
    rec["execution"]["remote_relation"] = rel
    g["remote_on_same_line_and_contains_0d62"] = rel in ("equal", "remote_is_ancestor_of_head") and known and C.is_ancestor(C.PINS["reviewed"], rb)
    rec["remote_refs_equal_pins"] = {}
    for name, key in (("master", "master"), ("time", "time"), ("agent/MYOS2-LEAD-002", "lead")):
        r = C.git("ls-remote", "origin", "refs/heads/" + name).stdout.decode().split("\t")[0].strip()
        rec["remote_refs_equal_pins"][name] = (r == C.full(C.PINS[key])) if r else None
    g["kernel_source_equals_pin"] = rec["remote_refs_equal_pins"]["time"] is True
    rec["note_template"] = "a lead branch that moved past the pin is an expected append and does not close the gate"
    g["ok"] = all(v for k, v in g.items() if k != "ok")
    return rec


if __name__ == "__main__":
    try:
        r = check()
    except Exception as e:  # noqa: BLE001 - any failure closes the gate
        r = {"check": "GUARD4", "gate": {"ok": False}, "error": "%s: %s" % (type(e).__name__, e)}
    C.emit(r, sys.argv[1])
    print(C.dump(r["gate"]))
    sys.exit(0 if r["gate"].get("ok") else 3)
