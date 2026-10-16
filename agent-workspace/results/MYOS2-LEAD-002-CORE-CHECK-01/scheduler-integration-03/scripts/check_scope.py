# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-INTEGRATION-03 (scheduler-integration-03)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; no code copied from earlier packages
# purpose: start/pre-push binding and scope check required by section 5 of the 16 contract:
#   contract and review fields at the pinned lead object; lead advanced by additions only; work branch
#   contains f36b89b8a53a and every f36 file is unchanged; index/worktree add files only under
#   scheduler-integration-03/; time and master equal their pins on the remote. Modes "stage" and
#   "docs" add hygiene of the staged new files (no 40-hex, no secret pattern, no NUL, YAML/JSON parse,
#   Python provenance header) and, for docs, MANIFEST file-list and tag-count consistency.
#   Read-only on git except reading the index; prints JSON; never writes into the repository.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 check_scope.py start|stage|docs [<results_commit_short12>]"""
import json
import re
import subprocess
import sys

import yaml

REPO = "/home/user/MyOS2"
PINS = {"time": "a039d9803ade", "master": "de3bb1df906a", "lead_prev": "6706013a079a", "lead": "40bc4faa4202",
        "reviewed": "f36b89b8a53a", "witness_results": "5078686e8267"}
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
LEAD_BRANCH = "agent/MYOS2-LEAD-002"
PACKET, FOLLOWUP = "MYOS2-LEAD-002-CORE-CHECK-01", "CORE-SCHED-INTEGRATION-03"
RESULTS_ROOT = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
PREFIX = RESULTS_ROOT + "scheduler-integration-03/"
LEAD_DIR = "agent-workspace/lead/MYOS2-LEAD-002/"
CONTRACT = LEAD_DIR + "16-scheduler-integration-contract.md"
REVIEW = LEAD_DIR + "reviews/CORE-SCHED-ORDER-02-review.md"
ORDER02_MANIFEST = RESULTS_ROOT + "scheduler-order-02/MANIFEST.md"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
SECRET = re.compile(r"(gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|AKIA[0-9A-Z]{16}|"
                    r"-----BEGIN [A-Z ]*PRIVATE KEY-----|[Aa]uthorization:\s*\S+)")
TAG = "[" + "VERIFIED "


def git(*args, check=True):
    p = subprocess.run(["git", "-C", REPO] + list(args), capture_output=True)
    if check and p.returncode != 0:
        raise RuntimeError("git %s failed: %s" % (" ".join(args[:3]), p.stderr.decode()[:200]))
    return p


def out(*args):
    return git(*args).stdout.decode("utf-8", "replace")


def full(rev):
    return out("rev-parse", rev + "^{commit}").strip()


def s12(rev):
    return full(rev)[:12]


def lines(*args):
    return [x for x in out(*args).split("\n") if x]


def is_ancestor(a, b):
    return git("merge-base", "--is-ancestor", a, b, check=False).returncode == 0


def remote(ref):
    r = out("ls-remote", "origin", ref).split("\t")[0].strip()
    return r


def front_matter(text):
    if not text.startswith("---\n"):
        return None
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


def main(argv):
    mode = argv[0] if argv else None
    if mode not in ("start", "stage", "docs") or (mode == "docs" and len(argv) < 2):
        sys.stderr.write(__doc__ + "\n")
        return 2
    res = {"check": "SCOPE_INTEGRATION_03", "mode": mode, "pins": PINS}
    gate = {}
    # lead objects: pinned task object, and the remote lead head only appends after it
    c = front_matter(out("show", "%s:%s" % (full(PINS["lead"]), CONTRACT))) or {}
    r = front_matter(out("show", "%s:%s" % (full(PINS["lead"]), REVIEW))) or {}
    want_c = {"packet_id": PACKET, "followup_id": FOLLOWUP, "required_review_record": "CORE-SCHED-ORDER-02-REVIEW-001",
              "reviewed_execution_short12": PINS["reviewed"], "witness_results_frozen_short12": PINS["witness_results"],
              "kernel_short12": PINS["time"], "execution_branch": WORK_BRANCH, "initial_execution_pr": 17,
              "allowed_write_prefix": PREFIX, "acceptance_ceiling": "PASS_PENDING_LOCAL",
              "kernel_change_authorized": False, "kernel_patch_production_authorized": False,
              "host_fixture_execution_authorized": False}
    want_r = {"record_id": "CORE-SCHED-ORDER-02-REVIEW-001", "packet_id": PACKET, "followup_id": "CORE-SCHED-ORDER-02",
              "reviewed_pr": 17, "reviewed_branch": WORK_BRANCH, "reviewed_commit_short12": PINS["reviewed"],
              "results_frozen_short12": PINS["witness_results"], "acceptance_verdict": "PASS_PENDING_LOCAL",
              "remaining_return_items_in_contract15": [], "next_followup": FOLLOWUP}
    res["contract_fields"] = {k: [c.get(k), c.get(k) == v] for k, v in want_c.items()}
    res["review_fields"] = {k: [r.get(k), r.get(k) == v] for k, v in want_r.items()}
    gate["contract_binding"] = all(v[1] for v in res["contract_fields"].values())
    gate["review_binding"] = all(v[1] for v in res["review_fields"].values())
    added = lines("diff", "--name-status", full(PINS["lead_prev"]), full(PINS["lead"]))
    res["lead_changes_prev_to_pin"] = [x.replace(LEAD_DIR, "") for x in added]
    gate["lead_pin_advanced_by_additions_only"] = bool(added) and all(x.startswith("A\t" + LEAD_DIR) for x in added)
    rl = remote("refs/heads/" + LEAD_BRANCH)
    res["remote_lead_short12"] = rl[:12] if rl else None
    after = lines("diff", "--name-status", full(PINS["lead"]), rl) if rl and git("cat-file", "-e", rl + "^{commit}", check=False).returncode == 0 else ["<remote lead object not local>"]
    res["remote_lead_changes_after_pin"] = [x.replace(LEAD_DIR, "") for x in after]
    gate["remote_lead_appends_only_after_pin"] = bool(rl) and is_ancestor(full(PINS["lead"]), rl) and all(x.startswith("A\t" + LEAD_DIR) for x in after)
    # execution branch: contains f36, every f36 file unchanged, only additions under the new prefix
    res["branch"] = out("rev-parse", "--abbrev-ref", "HEAD").strip()
    res["head_short12"] = s12("HEAD")
    gate["on_execution_branch"] = res["branch"] == WORK_BRANCH
    gate["head_contains_f36"] = is_ancestor(full(PINS["reviewed"]), full("HEAD"))
    since = lines("diff", "--cached", "--name-status", full(PINS["reviewed"]))
    res["index_vs_f36"] = [x.replace(PREFIX, "<prefix>") for x in since]
    gate["f36_files_unchanged_only_additions_under_prefix"] = all(x.startswith("A\t" + PREFIX) for x in since)
    st = [x for x in out("status", "--porcelain", "--untracked-files=all").split("\n") if x]
    res["worktree_outside_prefix"] = [x for x in st if not x[3:].startswith(PREFIX)]
    gate["worktree_clean_outside_prefix"] = not res["worktree_outside_prefix"]
    rb = remote("refs/heads/" + WORK_BRANCH)
    res["remote_branch_short12"] = rb[:12] if rb else None
    res["remote_branch_relation"] = ("equal" if rb == full("HEAD") else "behind_head" if rb and is_ancestor(rb, full("HEAD"))
                                     else "not_ancestor_or_missing")
    gate["remote_on_same_line"] = res["remote_branch_relation"] in ("equal", "behind_head") and is_ancestor(full(PINS["reviewed"]), rb)
    rt, rm = remote("refs/heads/time"), remote("refs/heads/master")
    res["remote_time_equals_pin"] = rt == full(PINS["time"])
    res["remote_master_equals_pin"] = rm == full(PINS["master"])
    gate["kernel_source_equals_pin"] = res["remote_time_equals_pin"]
    om = out("show", "%s:%s" % (full(PINS["reviewed"]), ORDER02_MANIFEST))
    res["order02_manifest_at_f36"] = {"bytes": len(om.encode()), "followup_id": (front_matter(om) or {}).get("followup_id")}
    gate["order02_manifest_present"] = res["order02_manifest_at_f36"]["followup_id"] == "CORE-SCHED-ORDER-02"
    if mode in ("stage", "docs"):
        files = sorted(x.split("\t", 1)[1] for x in since)
        rel = [p[len(PREFIX):] for p in files]
        blobs = {p[len(PREFIX):]: git("show", ":" + p).stdout for p in files}
        txt = {k: b.decode("utf-8", "replace") for k, b in blobs.items()}
        h = {"staged_files": rel,
             "untracked_in_prefix": [x[3:].replace(PREFIX, "") for x in st if x.startswith("??") and x[3:].startswith(PREFIX)],
             "unstaged_in_prefix": [x[3:].replace(PREFIX, "") for x in st if x[3:].startswith(PREFIX) and x[0] != "?" and x[1] != " "],
             "binary_like": [k for k, b in blobs.items() if b"\0" in b],
             "hex40_hits": {k: len(HEX40.findall(t)) for k, t in txt.items() if HEX40.search(t)},
             "secret_pattern_hits": [k for k, t in txt.items() if SECRET.search(t)],
             "json_parse_failures": [], "yaml_parse_failures": [], "python_failures": []}
        for k, t in txt.items():
            try:
                if k.endswith(".json"):
                    json.loads(t)
                elif k.endswith(".yaml"):
                    yaml.safe_load(t)
                elif k.endswith(".md") and not isinstance(front_matter(t), dict):
                    h["yaml_parse_failures"].append(k + " (front matter)")
            except Exception as e:  # noqa: BLE001
                (h["json_parse_failures"] if k.endswith(".json") else h["yaml_parse_failures"]).append("%s: %s" % (k, type(e).__name__))
            if k.endswith(".py"):
                try:
                    compile(t, k, "exec")
                except SyntaxError:
                    h["python_failures"].append(k + " (syntax)")
                if not t.startswith("# ---- provenance (metadata comment)"):
                    h["python_failures"].append(k + " (no provenance header)")
        res["hygiene"] = h
        gate["hygiene"] = not (h["binary_like"] or h["hex40_hits"] or h["secret_pattern_hits"] or h["json_parse_failures"]
                               or h["yaml_parse_failures"] or h["python_failures"] or h["unstaged_in_prefix"])
        if mode == "docs":
            rc = full(argv[1])
            res["results_commit_short12"] = rc[:12]
            res["non_added_since_results_commit"] = [x for x in lines("diff", "--cached", "--name-status", rc) if not x.startswith("A\t")]
            man = front_matter(txt.get("MANIFEST.md", "")) or {}
            tags = {k: txt[k].count(TAG) for k in txt if k.endswith(".md")}
            res["verified_tags_by_file"] = tags
            res["manifest_self_check"] = man.get("self_check")
            res["manifest_file_list_matches_staged"] = sorted(man.get("delivered_files") or []) == rel
            gate["docs_consistency"] = (not h["untracked_in_prefix"] and not res["non_added_since_results_commit"]
                                        and res["manifest_file_list_matches_staged"]
                                        and (man.get("self_check") or {}).get("verified_claims") == sum(tags.values())
                                        and all((front_matter(txt[k]) or {}).get("packet_id") == PACKET for k in txt if k.endswith(".md")))
    gate["ok"] = all(gate.values())
    res["gate"] = gate
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    print(text)
    return 0 if gate["ok"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
