# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; derived from recheck-01/fixtures/final_check2.py @ b843d475367a (frozen, unchanged)
# purpose: pre-publish check with the files staged. Scope: versus b843 only additions under
#   consumer-fix-02/ (so every b843 file is unchanged); versus the merge base with remote master only
#   additions under pilot/ or core/; hygiene of every staged consumer-fix-02 file; results.yaml and
#   expected_objects.json reproduced byte for byte; docs mode adds front-matter, MANIFEST data_summary
#   and file-list consistency. Output does not depend on the size of evidence.md, so embedding it and
#   re-running gives identical output.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 final_check3.py results
       python3 final_check3.py docs <results_commit_short12>"""
import json
import os
import re
import sys

import yaml

import common3 as C
import expected_objects3
import make_summary3

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
    rov = res.get("recorded_observation_revalidation") or {}
    return {"S01_S03_reproduced_on_b843": s.get("S01_S03_reproduced_on_b843"), "N_met": s.get("N_met"), "all_N_met": s.get("all_N_met"),
            "S04_old_new": s.get("S04_old_new"), "revalidation_consumer_status": (rov.get("consumer_facts") or {}).get("status"),
            "revalidation_cases_different_from_efb": sorted(((rov.get("diff_vs_recheck01_frozen_results") or {}).get("cases_different") or {}).keys()),
            "revalidation_source_run_status": (rov.get("source_execution_facts") or {}).get("run_status")}


def main(argv):
    mode = argv[0] if argv else None
    if mode not in ("results", "docs") or (mode == "docs" and len(argv) < 2):
        sys.stderr.write(__doc__ + "\n")
        return 2
    res = {"check": "FINAL_SCOPE_CONSUMER_FIX_02", "mode": mode}
    head = C.full("HEAD")
    res["branch"] = C.git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    res["head_short12"] = C.short12("HEAD")
    rb = C.git("ls-remote", "origin", "refs/heads/" + C.WORK_BRANCH).stdout.decode().split("\t")[0].strip()
    res["remote_branch_equals_head"] = rb == head
    rm = C.git("ls-remote", "origin", "refs/heads/master").stdout.decode().split("\t")[0].strip()
    res["remote_master_equals_pin"] = rm == C.pin("master")
    base = C.git("merge-base", "HEAD", rm if rm and C.git("cat-file", "-e", rm + "^{commit}", check=False).returncode == 0
                 else C.pin("master")).stdout.decode().strip()
    res["diff_base"] = "merge-base(HEAD, remote master) = %s" % C.short12(base)
    names = lines("diff", "--cached", "--name-status", base)
    res["index_vs_base_count"] = len(names)
    res["outside_pilot_core"] = [n for n in names if not n.split("\t", 1)[1].startswith((C.RESULTS_ROOT + "pilot/", C.RESULTS_ROOT + "core/"))]
    res["non_added_vs_base"] = [n for n in names if not n.startswith("A\t")]
    since = lines("diff", "--cached", "--name-status", C.pin("reviewed_input"))
    res["since_b843_not_addition_under_prefix"] = [n for n in since if not n.startswith("A\t" + P)]
    st = [x for x in C.git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n") if x]
    res["worktree_outside_prefix"] = [x for x in st if not x[3:].startswith(P)]
    res["prefix_unstaged_modifications"] = [x[3:].replace(P, "") for x in st if x[3:].startswith(P) and x[0] != "?" and x[1] != " "]
    res["prefix_untracked"] = [x[3:].replace(P, "") for x in st if x[3:].startswith(P) and x[0] == "?"]
    files = sorted(n.split("\t", 1)[1] for n in since if n.split("\t", 1)[1].startswith(P))
    rel = [p[len(P):] for p in files]
    res["prefix_files_staged"] = rel
    res["new_or_changed_vs_head"] = [n.replace(P, "") for n in lines("diff", "--cached", "--name-status", "HEAD")]
    blobs = {p[len(P):]: C.git("show", ":" + p).stdout for p in files}
    res["binary_like"] = [k for k, b in blobs.items() if b"\0" in b]
    res["hex40_hits"] = {k: len(C.HEX40.findall(b.decode("utf-8", "replace"))) for k, b in blobs.items() if C.HEX40.search(b.decode("utf-8", "replace"))}
    res["secret_pattern_hits"] = [k for k, b in blobs.items() if SECRET.search(b.decode("utf-8", "replace"))]
    res["json_parse_failures"] = []
    res["python_syntax_failures"] = []
    res["python_without_provenance_header"] = []
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
    obs = os.path.join(HERE, "..", "observations")
    try:
        res["results_yaml_reproduced_from_observations"] = make_summary3.render(make_summary3.generate(obs)).encode("utf-8") == blobs.get("results.yaml")
    except Exception as e:  # noqa: BLE001
        res["results_yaml_reproduced_from_observations"] = "%s: %s" % (type(e).__name__, e)
    res["expected_objects_reproduced_from_frozen_inputs"] = (json.loads(blobs.get("fixtures/expected_objects.json", b"{}").decode("utf-8"))
                                                           == json.loads(json.dumps(expected_objects3.extract(), ensure_ascii=False)))
    ct = json.loads(blobs.get("observations/consumer_tests.json", b"{}").decode("utf-8"))
    n6 = [t for t in ct.get("tests", []) if t.get("id") == "N06"]
    n6y = blobs.get("observations/n06_revalidation.yaml", b"")
    res["n06_report_matches_consumer_tests_record"] = bool(n6) and (n6[0].get("report_file") or {}).get("sha256_segments") == C.sha_segments(n6y)
    ry = yaml.safe_load(blobs.get("results.yaml", b"").decode("utf-8") or "{}")
    res["results_yaml"] = {"packet_id": ry.get("packet_id"), "followup_id": ry.get("followup_id"), "all_N_met": (ry.get("summary") or {}).get("all_N_met")}
    ok = (res["branch"] == C.WORK_BRANCH and res["remote_branch_equals_head"] and not res["outside_pilot_core"] and not res["non_added_vs_base"]
          and not res["since_b843_not_addition_under_prefix"] and not res["worktree_outside_prefix"] and not res["prefix_unstaged_modifications"]
          and not any(u.startswith("observations/") or u == "results.yaml" for u in res["prefix_untracked"])
          and not res["binary_like"] and not res["hex40_hits"] and not res["secret_pattern_hits"] and not res["json_parse_failures"]
          and not res["python_syntax_failures"] and not res["python_without_provenance_header"]
          and res["results_yaml_reproduced_from_observations"] is True and res["expected_objects_reproduced_from_frozen_inputs"]
          and res["n06_report_matches_consumer_tests_record"] and res["results_yaml"]["packet_id"] == C.PACKET
          and res["results_yaml"]["followup_id"] == C.FOLLOWUP and res["results_yaml"]["all_N_met"] is True)
    if mode == "docs":
        rc = C.full(argv[1])
        res["results_commit"] = C.short12(rc)
        res["results_yaml_changed_since_results_commit"] = C.git("diff", "--cached", "--quiet", rc, "--", P + "results.yaml", check=False).returncode != 0
        res["non_added_since_results_commit"] = [n for n in lines("diff", "--cached", "--name-status", rc) if not n.startswith("A\t")]
        res["docs"] = {}
        for d in ("MANIFEST.md", "evidence.md", "results.yaml"):
            t = blobs.get(d, b"").decode("utf-8")
            try:
                meta = yaml.safe_load(t) if d.endswith(".yaml") else fm(t)
            except Exception:  # noqa: BLE001
                meta = None
            bare = sorted({t.count("\n", 0, m.start()) + 1 for m in re.finditer(r"\bPASS\w*", t) if m.group(0) != "PASS_PENDING_LOCAL"})
            res["docs"][d] = {"yaml_parses": isinstance(meta, dict), "packet_id": (meta or {}).get("packet_id") if isinstance(meta, dict) else None,
                              "verified_tag_count": t.count(TAG), "other_pass_token_lines": bare}
        man = fm(blobs.get("MANIFEST.md", b"").decode("utf-8")) if blobs.get("MANIFEST.md") else {}
        ev = fm(blobs.get("evidence.md", b"").decode("utf-8")) if blobs.get("evidence.md") else {}
        res["manifest_self_check_consistent"] = (man.get("self_check") or {}).get("verified_claims") == sum(v["verified_tag_count"] for v in res["docs"].values())
        want, got = data_summary(ry), man.get("data_summary") or {}
        res["manifest_data_summary_mismatches"] = sorted(k for k in set(want) | set(got) if want.get(k) != got.get(k))
        res["manifest_file_list_matches_staged"] = sorted(man.get("delivered_files") or []) == rel
        res["evidence_results_commit_matches"] = ev.get("results_commit_short12") == res["results_commit"]
        ok = (ok and not res["prefix_untracked"] and not res["results_yaml_changed_since_results_commit"]
              and not res["non_added_since_results_commit"]
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
