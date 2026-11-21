# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; structure follows consumer-fix-02/fixtures/final_check3.py @ 0d62c4d19711 (unchanged)
# purpose: pre-publish check with the files staged: versus 0d62 only additions under scheduler-order-02/
#   (every 0d62 file unchanged); versus the merge base with remote master only additions under pilot/,
#   core/ or scheduler-order-02/; hygiene of every staged new file (no 40-hex, no secret pattern, no NUL,
#   JSON parses, Python compiles with a provenance header); results.yaml reproduced byte for byte from
#   observations/; the committed expanded source matches runs.json. Docs mode adds front matter,
#   MANIFEST data_summary and file-list consistency. Output does not depend on evidence.md size.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 final_check4.py results
       python3 final_check4.py docs <results_commit_short12>"""
import json
import os
import re
import sys

import yaml

import common4 as C
import make_results4

P = C.PREFIX
TAG = "[" + "VERIFIED"
SECRET = re.compile(r"(gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|AKIA[0-9A-Z]{16}|"
                    r"-----BEGIN [A-Z ]*PRIVATE KEY-----|[Aa]uthorization:\s*\S+)")
HERE = os.path.dirname(os.path.abspath(__file__))


def lines(*args):
    return [x for x in C.git(*args).stdout.decode().split("\n") if x]


def fm(text):
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


def data_summary(res):
    s = res.get("summary") or {}
    return {"scenarios_completed": s.get("scenarios_completed"), "candidate_P": s.get("candidate_P"),
            "P_violations_by_step": s.get("P_violations_by_step"), "all_structure_ok": s.get("all_structure_ok"),
            "all_accounting_matches_oracle": s.get("all_accounting_matches_oracle"), "lead_derivations_all_agree": s.get("lead_derivations_all_agree"),
            "selected_task_not_queue_minimum": s.get("selected_task_not_queue_minimum"), "controls_all_met": s.get("controls_all_met"),
            "pick_next_task_myos_sha256_0": ((res.get("input_identity") or {}).get("pick_next_task_myos") or {}).get("sha256_segments", [None])[0]}


def main(argv):
    mode = argv[0] if argv else None
    if mode not in ("results", "docs") or (mode == "docs" and len(argv) < 2):
        sys.stderr.write(__doc__ + "\n")
        return 2
    res = {"check": "FINAL_SCOPE_SCHEDULER_ORDER_02", "mode": mode}
    head = C.full("HEAD")
    res["branch"] = C.git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    res["head_short12"] = C.short12("HEAD")
    rb = C.git("ls-remote", "origin", "refs/heads/" + C.WORK_BRANCH).stdout.decode().split("\t")[0].strip()
    res["remote_branch_equals_head"] = rb == head
    rm = C.git("ls-remote", "origin", "refs/heads/master").stdout.decode().split("\t")[0].strip()
    res["remote_master_equals_pin"] = rm == C.full(C.PINS["master"])
    base = C.git("merge-base", "HEAD", rm if rm and C.git("cat-file", "-e", rm + "^{commit}", check=False).returncode == 0
                 else C.full(C.PINS["master"])).stdout.decode().strip()
    res["diff_base"] = "merge-base(HEAD, remote master) = %s" % C.short12(base)
    names = lines("diff", "--cached", "--name-status", base)
    res["index_vs_base_count"] = len(names)
    allowed = tuple(C.RESULTS_ROOT + x for x in ("pilot/", "core/", "scheduler-order-02/"))
    res["outside_allowed_dirs"] = [n for n in names if not n.split("\t", 1)[1].startswith(allowed)]
    res["non_added_vs_base"] = [n for n in names if not n.startswith("A\t")]
    since = lines("diff", "--cached", "--name-status", C.full(C.PINS["reviewed"]))
    res["since_0d62_not_addition_under_prefix"] = [n for n in since if not n.startswith("A\t" + P)]
    st = [x for x in C.git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n") if x]
    res["worktree_outside_prefix"] = [x for x in st if not x[3:].startswith(P)]
    res["prefix_unstaged_modifications"] = [x[3:].replace(P, "") for x in st if x[3:].startswith(P) and x[0] != "?" and x[1] != " "]
    res["prefix_untracked"] = [x[3:].replace(P, "") for x in st if x[3:].startswith(P) and x[0] == "?"]
    files = sorted(n.split("\t", 1)[1] for n in since if n.split("\t", 1)[1].startswith(P))
    rel = [p[len(P):] for p in files]
    res["prefix_files_staged"] = rel
    res["new_or_changed_vs_head"] = [n.replace(P, "") for n in lines("diff", "--cached", "--name-status", "HEAD")]
    blobs = {p[len(P):]: C.git("show", ":" + p).stdout for p in files}
    txt = {k: b.decode("utf-8", "replace") for k, b in blobs.items()}
    res["binary_like"] = [k for k, b in blobs.items() if b"\0" in b]
    res["hex40_hits"] = {k: len(C.HEX40.findall(t)) for k, t in txt.items() if C.HEX40.search(t)}
    res["secret_pattern_hits"] = [k for k, t in txt.items() if SECRET.search(t)]
    res["json_parse_failures"], res["python_syntax_failures"], res["python_without_provenance_header"] = [], [], []
    for k, b in blobs.items():
        if k.endswith(".json"):
            try:
                json.loads(b.decode("utf-8"))
            except Exception:  # noqa: BLE001
                res["json_parse_failures"].append(k)
        if k.endswith(".py"):
            try:
                compile(b.decode("utf-8"), k, "exec")
            except SyntaxError:
                res["python_syntax_failures"].append(k)
            if not b.startswith(b"# ---- provenance (metadata comment)"):
                res["python_without_provenance_header"].append(k)
    res["c_template_has_provenance_header"] = blobs.get("fixtures/fx_order.c", b"").startswith(b"/* ---- provenance (metadata comment)")
    obs = os.path.join(HERE, "..", "observations")
    try:
        res["results_yaml_reproduced_from_observations"] = make_results4.render(make_results4.generate(obs)).encode("utf-8") == blobs.get("results.yaml")
    except Exception as e:  # noqa: BLE001
        res["results_yaml_reproduced_from_observations"] = "%s: %s" % (type(e).__name__, e)
    runs = json.loads(blobs.get("observations/runs.json", b"{}").decode("utf-8"))
    res["expanded_source_matches_runs_record"] = (runs.get("expanded_source") or {}).get("sha256_segments") == C.sha_segments(blobs.get("observations/fx_order.expanded.c", b""))
    res["template_matches_runs_record"] = (runs.get("template") or {}).get("sha256_segments") == C.sha_segments(blobs.get("fixtures/fx_order.c", b""))
    ry = yaml.safe_load(txt.get("results.yaml") or "{}")
    res["results_yaml"] = {"packet_id": ry.get("packet_id"), "followup_id": ry.get("followup_id"),
                           "scenarios_completed": len((ry.get("summary") or {}).get("scenarios_completed") or [])}
    ok = (res["branch"] == C.WORK_BRANCH and res["remote_branch_equals_head"] and not res["outside_allowed_dirs"] and not res["non_added_vs_base"]
          and not res["since_0d62_not_addition_under_prefix"] and not res["worktree_outside_prefix"] and not res["prefix_unstaged_modifications"]
          and not any(u.startswith("observations/") or u == "results.yaml" for u in res["prefix_untracked"])
          and not res["binary_like"] and not res["hex40_hits"] and not res["secret_pattern_hits"] and not res["json_parse_failures"]
          and not res["python_syntax_failures"] and not res["python_without_provenance_header"] and res["c_template_has_provenance_header"]
          and res["results_yaml_reproduced_from_observations"] is True and res["expanded_source_matches_runs_record"]
          and res["template_matches_runs_record"] and res["results_yaml"]["packet_id"] == C.PACKET and res["results_yaml"]["followup_id"] == C.FOLLOWUP)
    if mode == "docs":
        rc = C.full(argv[1])
        res["results_commit"] = C.short12(rc)
        res["results_yaml_changed_since_results_commit"] = C.git("diff", "--cached", "--quiet", rc, "--", P + "results.yaml", check=False).returncode != 0
        res["non_added_since_results_commit"] = [n for n in lines("diff", "--cached", "--name-status", rc) if not n.startswith("A\t")]
        res["docs"] = {}
        for d in ("MANIFEST.md", "evidence.md", "results.yaml"):
            t = txt.get(d, "")
            try:
                meta = yaml.safe_load(t) if d.endswith(".yaml") else fm(t)
            except Exception:  # noqa: BLE001
                meta = None
            bare = sorted({t.count("\n", 0, m.start()) + 1 for m in re.finditer(r"\bPASS\w*", t) if m.group(0) != "PASS_PENDING_LOCAL"})
            res["docs"][d] = {"yaml_parses": isinstance(meta, dict), "packet_id": (meta or {}).get("packet_id") if isinstance(meta, dict) else None,
                              "verified_tag_count": t.count(TAG), "other_pass_token_lines": bare}
        man = fm(txt["MANIFEST.md"]) if txt.get("MANIFEST.md") else {}
        ev = fm(txt["evidence.md"]) if txt.get("evidence.md") else {}
        res["manifest_self_check_consistent"] = (man.get("self_check") or {}).get("verified_claims") == sum(v["verified_tag_count"] for v in res["docs"].values())
        want, got = data_summary(ry), man.get("data_summary") or {}
        res["manifest_data_summary_mismatches"] = sorted(k for k in set(want) | set(got) if want.get(k) != got.get(k))
        res["manifest_file_list_matches_staged"] = sorted(man.get("delivered_files") or []) == rel
        res["evidence_results_commit_matches"] = ev.get("results_commit_short12") == res["results_commit"]
        ok = (ok and not res["prefix_untracked"] and not res["results_yaml_changed_since_results_commit"] and not res["non_added_since_results_commit"]
              and all(v["yaml_parses"] and v["packet_id"] == C.PACKET and not v["other_pass_token_lines"] for v in res["docs"].values())
              and res["manifest_self_check_consistent"] and not res["manifest_data_summary_mismatches"]
              and res["manifest_file_list_matches_staged"] and res["evidence_results_commit_matches"])
    res["all_as_expected"] = bool(ok)
    text = C.dump(res)
    if C.HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    print(text)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
