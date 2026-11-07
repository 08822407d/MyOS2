# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; replaces run_all.identity() of core/fixtures/run_all.py @ a1e7c2277705
# change (R02/R04): the gate now separates (a) frozen inputs and receipts, (b) frozen pilot/core
#   originals, (c) the moving execution-branch head. A legitimate successor head is accepted when it
#   descends from the reviewed core head and every original is byte-identical; a wrong branch, a
#   non-descendant head, a changed original or a receipt mismatch closes the gate. Any exception
#   also closes the gate. Parameters allow meta tests to inject heads/branches/repos (no remote writes).
# --------------------------------------------------------------------------------------------------
"""Identity and authorisation gate for the recheck."""
import datetime

import yaml

import harness2 as H

LEAD = "agent-workspace/lead/MYOS2-LEAD-002/"
TECH = ["07-scheduler-wakeup-timer-audit.md", "07-core-audit-map.yaml", "MANIFEST.md",
        "09-local-verification-contract.md", "10-cloud-pilot-and-github-handoff.md", "11-core-verification-cloud.md"]
CORE_REVIEW = LEAD + "reviews/CORE-CHECK-01-core-review.md"
PILOT_REVIEW = LEAD + "reviews/CORE-CHECK-01-pilot-review.md"
CONTRACT12 = LEAD + "12-core-check-recheck-contract.md"


def _front(data):
    t = data.decode("utf-8")
    return yaml.safe_load(t[4:t.index("\n---\n", 4)])


def _tree(rev, prefix, repo=None):
    out = H.git("ls-tree", "-r", H.full(rev, repo), prefix, repo=repo).stdout.decode().split("\n")
    res = {}
    for ln in out:
        if "\t" in ln:
            meta, path = ln.split("\t", 1)
            res[path] = meta.split()[2]  # blob object id (internal only)
    return res


def check(repo=None, head=None, branch_name=None, remote_head=None, check_remote=True):
    """Return the identity record; rec['gate']['ok'] is the only value callers may act on."""
    rec = {"check": "IDENTITY2", "read_time_utc": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
           "gate": {}, "notes": []}
    gate = rec["gate"]
    rec["pins"] = {k: H.short12(v, repo) for k, v in H.PINS.items()}
    # (a) receipts and frozen technical inputs
    cr = _front(H.blob(H.PINS["lead_head"], CORE_REVIEW, repo))
    c12 = _front(H.blob(H.PINS["lead_head"], CONTRACT12, repo))
    s = lambda k: H.short12(H.PINS[k], repo)
    gate["core_review_record"] = (cr.get("record_id") == "CORE-CHECK-01-CORE-REVIEW-001" and cr.get("packet_id") == H.PACKET
                                  and cr.get("reviewed_pr") == 17 and cr.get("reviewed_branch") == H.WORK_BRANCH
                                  and str(cr.get("reviewed_commit_short12")) == s("core_frozen")
                                  and str(cr.get("frozen_results_commit_short12")) == s("core_results")
                                  and str(cr.get("pilot_head_short12")) == s("pilot_head")
                                  and cr.get("followup_id") == "CORE-CHECK-01-RECHECK-01"
                                  and cr.get("allowed_execution_branch") == H.WORK_BRANCH
                                  and cr.get("allowed_write_prefix") == H.RECHECK_PREFIX)
    ir = cr.get("input_refs") or {}
    gate["core_review_input_refs"] = (str((ir.get("kernel") or {}).get("short12")) == s("time")
                                      and str((ir.get("workspace") or {}).get("short12")) == s("master")
                                      and str((ir.get("technical_taskbook") or {}).get("short12")) == s("taskbook")
                                      and str((ir.get("pilot_review") or {}).get("short12")) == s("pilot_review"))
    gate["contract12_binding"] = (c12.get("followup_id") == "CORE-CHECK-01-RECHECK-01" and c12.get("execution_pr") == 17
                                  and c12.get("execution_branch") == H.WORK_BRANCH
                                  and str(c12.get("reviewed_core_short12")) == s("core_frozen")
                                  and c12.get("allowed_write_prefix") == H.RECHECK_PREFIX)
    rec["core_review_fields"] = {k: cr.get(k) for k in ("record_id", "disposition", "reviewed_pr", "reviewed_branch",
                                                         "reviewed_commit_short12", "frozen_results_commit_short12",
                                                         "pilot_head_short12", "followup_id", "allowed_write_prefix")}
    same = {f: H.blob(H.PINS["taskbook"], LEAD + f, repo) == H.blob(H.PINS["lead_head"], LEAD + f, repo) for f in TECH}
    same["reviews/CORE-CHECK-01-pilot-review.md"] = (H.blob(H.PINS["pilot_review"], PILOT_REVIEW, repo)
                                                    == H.blob(H.PINS["lead_head"], PILOT_REVIEW, repo))
    rec["technical_inputs_unchanged"] = same
    gate["technical_inputs_unchanged"] = all(same.values())
    gate["lead_head_descends_from_taskbook"] = H.is_ancestor(H.PINS["taskbook"], H.PINS["lead_head"], repo)
    # (b)+(c) execution branch and frozen originals
    cur_branch = branch_name if branch_name is not None else H.git("rev-parse", "--abbrev-ref", "HEAD", repo=repo).stdout.decode().strip()
    head_full = H.full(head or "HEAD", repo)
    rec["execution"] = {"branch": cur_branch, "head_short12": H.short12(head_full, repo)}
    gate["on_execution_branch"] = cur_branch == H.WORK_BRANCH
    gate["head_descends_from_reviewed_core"] = H.is_ancestor(H.PINS["core_frozen"], head_full, repo)
    frozen_orig = {p: b for p, b in _tree(H.PINS["core_frozen"], H.RESULTS_ROOT, repo).items()
                   if not p.startswith(H.RECHECK_PREFIX)}
    at_head = {p: b for p, b in _tree(head_full, H.RESULTS_ROOT, repo).items() if not p.startswith(H.RECHECK_PREFIX)}
    changed = sorted(p for p in frozen_orig if at_head.get(p) != frozen_orig[p])
    added = sorted(p for p in at_head if p not in frozen_orig)
    pilot_frozen = _tree(H.PINS["pilot_head"], H.RESULTS_ROOT + "pilot/", repo)
    pilot_changed = sorted(p for p in pilot_frozen if at_head.get(p) != pilot_frozen[p])
    rec["originals"] = {"frozen_files": len(frozen_orig), "changed_or_missing": changed, "added_outside_recheck": added,
                        "pilot_changed_since_pilot_head": pilot_changed}
    gate["originals_unchanged"] = not changed and not added and not pilot_changed
    if check_remote:
        if remote_head is None:
            ls = H.git("ls-remote", "origin", "refs/heads/" + H.WORK_BRANCH, repo=repo).stdout.decode().split("\t")[0].strip()
            remote_full = ls or None
        else:
            remote_full = H.full(remote_head, repo)
        known = remote_full is not None and H.git("cat-file", "-e", remote_full + "^{commit}", check=False, repo=repo).returncode == 0
        rel = "unknown"
        if known:
            if remote_full == head_full:
                rel = "equal"
            elif H.is_ancestor(remote_full, head_full, repo):
                rel = "remote_is_ancestor_of_head"
            elif H.is_ancestor(head_full, remote_full, repo):
                rel = "remote_is_descendant_of_head"
            else:
                rel = "diverged"
        rec["execution"]["remote_relation"] = rel
        gate["remote_on_same_line"] = rel in ("equal", "remote_is_ancestor_of_head")
        # a remote branch reset to an older commit is also an ancestor of HEAD; require that the
        # reviewed core head is still part of the remote branch history
        gate["remote_descends_from_reviewed_core"] = bool(known and H.is_ancestor(H.PINS["core_frozen"], remote_full, repo))
        for name, key in (("master", "master"), ("time", "time"), ("agent/MYOS2-LEAD-002", "lead_head")):
            r = H.git("ls-remote", "origin", "refs/heads/" + name, repo=repo).stdout.decode().split("\t")[0].strip()
            rec.setdefault("remote_refs_equal_pins", {})[name] = (r == H.pin(key, repo)) if r else None
        if rec["remote_refs_equal_pins"].get("time") is False:
            rec["notes"].append("time branch moved; all source reads stay bound to the pinned object")
    st = H.git("status", "--porcelain", "--untracked-files=all", repo=repo).stdout.decode().split("\n")
    outside = [l for l in st if l and not l[3:].startswith(H.RECHECK_PREFIX)]
    rec["worktree_changes_outside_recheck"] = outside
    gate["worktree_clean_outside_recheck"] = not outside if head is None else True
    gate["ok"] = all(v for k, v in gate.items() if k != "ok")
    return rec


def safe_check(**kw):
    """Exceptions close the gate (never raise past the entry point)."""
    try:
        return check(**kw)
    except Exception as e:  # noqa: BLE001
        return {"check": "IDENTITY2", "gate": {"ok": False}, "error": "%s: %s" % (type(e).__name__, e)}
