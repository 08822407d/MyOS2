# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; derived from core/fixtures/final_check.py @ a1e7c2277705 (frozen, unchanged)
# purpose: pre-publish scope, hygiene and consistency check, run with the files staged. It is a
#   publication tool and takes no part in the verification runs.
# change vs frozen final_check.py: two modes (results commit / docs commit); diff base is the merge
#   base with the current remote master; tracked files inside recheck-01 must equal the index (untracked
#   files are listed; allowed in results mode except direct observations/ inputs, refused in docs mode);
#   results.yaml must be reproduced byte for byte from observations/; MANIFEST data_summary, file list
#   and V/M/CA table rows are compared with results.yaml. Output never depends on the size of
#   evidence.md, so embedding it in evidence.md and re-running reproduces identical output.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 final_check2.py results
       python3 final_check2.py docs <results_commit_short12>"""
import json
import os
import re
import sys

import yaml

import harness2 as H
import make_results2 as MR

REC = H.RECHECK_PREFIX
DOCS = ["MANIFEST.md", "evidence.md", "results.yaml"]
TAG = "[" + "VERIFIED"
SECRET = re.compile(r"(gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|AKIA[0-9A-Z]{16}|"
                    r"-----BEGIN [A-Z ]*PRIVATE KEY-----|[Aa]uthorization:\s*\S+)")
HERE = os.path.dirname(os.path.abspath(__file__))


def lines(*args):
    return [x for x in H.git(*args).stdout.decode().split("\n") if x]


def staged(path):
    return H.git("show", ":" + path).stdout


def fm(text):
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


def data_summary(res):
    run, cnt, dif = res.get("run") or {}, res.get("counts") or {}, res.get("diff_vs_frozen_core") or {}
    a46 = [c for c in res.get("cases", []) if c.get("case_id") == "V00_A46_correction"]
    return {"run_status": run.get("status"), "run_exit_code": run.get("exit_code"),
            "execution_complete": run.get("execution_complete"), "verification_findings": run.get("verification_findings"),
            "infrastructure_errors": len(run.get("infrastructure_errors") or []), "dynamic_calls": run.get("dynamic_calls"),
            "identity_gate_ok": ((res.get("identity") or {}).get("gate") or {}).get("ok"),
            "h00_dynamic_safety_ok": (res.get("h00") or {}).get("dynamic_safety_ok"),
            "top_level_cases": cnt.get("top_level_cases"), "evidence_status": cnt.get("evidence_status"),
            "results_for_valid_evidence": cnt.get("results_for_valid_evidence"),
            "layers_not_executed": cnt.get("layers_not_executed"),
            "case_results_differing_from_frozen_core": [k for k, v in (dif.get("case_results") or {}).items() if not v.get("same")],
            "ca_rulings_differing_from_frozen_core": [k for k, v in (dif.get("ca_rulings") or {}).items() if not v.get("same")],
            "a46_correction": [a46[0].get("evidence_status"), a46[0].get("result"), [x[0] for x in a46[0].get("anchors", [])]] if a46 else None,
            "m_items_met": {k: v.get("met") for k, v in (res.get("m_items") or {}).items()},
            "generator_errors": len(res.get("generator_errors") or [])}


def table_rows(text, res):
    cases = {c["case_id"]: c for c in res.get("cases", [])}
    mets = {k: v.get("met") for k, v in (res.get("m_items") or {}).items()}
    cas = {k: v.get("ruling") for k, v in (res.get("ca_rulings") or {}).items()}
    out = {"V_rows": 0, "M_rows": 0, "CA_rows": 0, "mismatches": []}
    for ln in text.split("\n"):
        m = re.match(r"^\| (V\d\d) \| (\w+) \| (\w+|-) \|", ln)
        if m:
            out["V_rows"] += 1
            c = cases.get(m.group(1)) or {}
            if [m.group(2), m.group(3)] != [c.get("evidence_status"), c.get("result") or "-"]:
                out["mismatches"].append(m.group(1))
        m = re.match(r"^\| (M\d\d) \|.*\| (true|false) \|$", ln)
        if m:
            out["M_rows"] += 1
            if (m.group(2) == "true") != mets.get(m.group(1)):
                out["mismatches"].append(m.group(1))
        m = re.match(r"^\| (CA-\d\d) \| `([^`|]+)` \|", ln)
        if m:
            out["CA_rows"] += 1
            if m.group(2) != cas.get(m.group(1)):
                out["mismatches"].append(m.group(1))
    return out


def main(argv):
    mode = argv[0] if argv else None
    if mode not in ("results", "docs") or (mode == "docs" and len(argv) < 2):
        sys.stderr.write(__doc__ + "\n")
        return 2
    res = {"check": "FINAL_SCOPE_RECHECK_01", "mode": mode}
    head = H.full("HEAD")
    res["branch"] = H.git("rev-parse", "--abbrev-ref", "HEAD").stdout.decode().strip()
    res["head_short12"] = H.short12(head)
    rb = H.git("ls-remote", "origin", "refs/heads/" + H.WORK_BRANCH).stdout.decode().split("\t")[0].strip()
    res["remote_branch_equals_head"] = rb == head
    rm = H.git("ls-remote", "origin", "refs/heads/master").stdout.decode().split("\t")[0].strip()
    res["remote_master_equals_pin"] = rm == H.pin("master")
    base = H.git("merge-base", "HEAD", rm if rm and H.git("cat-file", "-e", rm + "^{commit}", check=False).returncode == 0
                 else H.pin("master")).stdout.decode().strip()
    res["diff_base"] = "merge-base(HEAD, remote master) = %s" % H.short12(base)
    names = lines("diff", "--cached", "--name-status", base)
    res["index_vs_base_count"] = len(names)
    res["outside_pilot_core"] = [n for n in names if not n.split("\t", 1)[1].startswith((H.RESULTS_ROOT + "pilot/", H.RESULTS_ROOT + "core/"))]
    res["non_added_vs_base"] = [n for n in names if not n.startswith("A\t")]
    since_core = lines("diff", "--cached", "--name-status", H.pin("core_frozen"))
    res["since_core_frozen_outside_recheck"] = [n for n in since_core if not n.split("\t", 1)[1].startswith(REC)]
    res["since_core_frozen_non_added"] = [n for n in since_core if not n.startswith("A\t")]
    res["pilot_changed_since_pilot_head"] = lines("diff", "--cached", "--name-only", H.pin("pilot_head"), "--", H.RESULTS_ROOT + "pilot/")
    st = [x for x in H.git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n") if x]
    res["worktree_outside_recheck"] = [x for x in st if not x[3:].startswith(REC)]
    # tracked files must equal the index (the checked bytes are the committed bytes); untracked files are
    # listed: allowed in results mode (they belong to the later docs commit), not allowed in docs mode
    res["recheck_unstaged_modifications"] = [x[3:].replace(REC, "") for x in st if x[3:].startswith(REC) and x[0] != "?" and x[1] != " "]
    res["recheck_untracked"] = [x[3:].replace(REC, "") for x in st if x[3:].startswith(REC) and x[0] == "?"]
    files = sorted(n.split("\t", 1)[1] for n in since_core if n.split("\t", 1)[1].startswith(REC))
    rel = [p[len(REC):] for p in files]
    res["recheck_files_staged"] = rel
    res["new_or_changed_vs_head"] = [n.replace(REC, "") for n in lines("diff", "--cached", "--name-status", "HEAD")]
    blobs = {p[len(REC):]: staged(p) for p in files}
    res["binary_like"] = [k for k, b in blobs.items() if b"\0" in b]
    res["hex40_hits"] = {k: len(H.HEX40.findall(b)) for k, b in blobs.items() if H.HEX40.search(b)}
    res["secret_pattern_hits"] = [k for k, b in blobs.items() if SECRET.search(b.decode("utf-8", "replace"))]
    res["json_parse_failures"] = []
    for k, b in blobs.items():
        if k.endswith(".json"):
            try:
                json.loads(b.decode("utf-8"))
            except Exception:  # noqa: BLE001
                res["json_parse_failures"].append(k)
    res["python_syntax_failures"] = []
    res["python_without_provenance_header"] = []
    for k, b in blobs.items():
        if k.endswith(".py"):
            try:
                compile(b.decode("utf-8"), k, "exec")
            except SyntaxError:
                res["python_syntax_failures"].append(k)
            if not b.startswith(b"# ---- provenance (metadata comment)"):
                res["python_without_provenance_header"].append(k)
    ry = blobs.get("results.yaml", b"")
    try:
        regen = MR.render(MR.generate(os.path.join(HERE, "..", "observations"))).encode("utf-8")
        res["results_yaml_reproduced_from_observations"] = regen == ry
    except Exception as e:  # noqa: BLE001
        res["results_yaml_reproduced_from_observations"] = "%s: %s" % (type(e).__name__, e)
    ryd = yaml.safe_load(ry.decode("utf-8")) if ry else {}
    res["results_yaml"] = {"packet_id": ryd.get("packet_id"), "status": ryd.get("status"),
                           "generator_errors": len(ryd.get("generator_errors") or [])}
    ok = (res["branch"] == H.WORK_BRANCH and res["remote_branch_equals_head"] and not res["outside_pilot_core"]
          and not res["non_added_vs_base"] and not res["since_core_frozen_outside_recheck"]
          and not res["since_core_frozen_non_added"] and not res["pilot_changed_since_pilot_head"]
          and not res["worktree_outside_recheck"] and not res["recheck_unstaged_modifications"]
          and not any(u.startswith("observations/") and u.count("/") == 1 for u in res["recheck_untracked"])
          and not res["binary_like"] and not res["hex40_hits"] and not res["secret_pattern_hits"]
          and not res["json_parse_failures"] and not res["python_syntax_failures"]
          and not res["python_without_provenance_header"] and res["results_yaml_reproduced_from_observations"] is True
          and res["results_yaml"]["packet_id"] == H.PACKET and res["results_yaml"]["generator_errors"] == 0)
    if mode == "docs":
        rc = H.full(argv[1])
        res["results_commit"] = H.short12(rc)
        res["results_yaml_changed_since_results_commit"] = H.git("diff", "--cached", "--quiet", rc, "--", REC + "results.yaml",
                                                                 check=False).returncode != 0
        res["non_added_since_results_commit"] = [n for n in lines("diff", "--cached", "--name-status", rc) if not n.startswith("A\t")]
        res["docs"] = {}
        for d in DOCS:
            t = blobs.get(d, b"").decode("utf-8")
            try:
                meta = yaml.safe_load(t) if d.endswith(".yaml") else fm(t)
            except Exception:  # noqa: BLE001
                meta = None
            # line numbers of every upper-case pass token other than the scoped ceiling token; only numbers
            # are printed so that embedding this output does not add new tokens
            bare = sorted({t.count("\n", 0, m.start()) + 1 for m in re.finditer(r"\bPASS\w*", t) if m.group(0) != "PASS_PENDING_LOCAL"})
            res["docs"][d] = {"yaml_parses": isinstance(meta, dict), "first_key": next(iter(meta)) if isinstance(meta, dict) else None,
                              "packet_id": (meta or {}).get("packet_id") if isinstance(meta, dict) else None,
                              "verified_tag_count": t.count(TAG), "other_pass_token_lines": bare}
        man_t = blobs.get("MANIFEST.md", b"").decode("utf-8")
        man = fm(man_t) if man_t else {}
        ev_t = blobs.get("evidence.md", b"").decode("utf-8")
        ev = fm(ev_t) if ev_t else {}
        res["manifest_self_check_consistent"] = (man.get("self_check") or {}).get("verified_claims") == sum(
            v["verified_tag_count"] for v in res["docs"].values())
        conv = H.blob(H.PINS["master"], "agent-workspace/conventions.md").decode("utf-8")
        res["startup_selfcheck_quote_in_conventions"] = bool(man.get("startup_selfcheck_quote")) and man["startup_selfcheck_quote"] in conv
        want = data_summary(ryd)
        got = man.get("data_summary") or {}
        res["manifest_data_summary_mismatches"] = sorted(k for k in set(want) | set(got) if want.get(k) != got.get(k))
        res["manifest_file_list_matches_staged"] = sorted(man.get("delivered_files") or []) == rel
        res["manifest_tables"] = table_rows(man_t, ryd)
        res["evidence_results_commit_matches"] = ev.get("results_commit_short12") == res["results_commit"]
        ok = (ok and not res["recheck_untracked"]
              and not res["results_yaml_changed_since_results_commit"] and not res["non_added_since_results_commit"]
              and all(v["yaml_parses"] and v["packet_id"] == H.PACKET and not v["other_pass_token_lines"] for v in res["docs"].values())
              and res["manifest_self_check_consistent"] and res["startup_selfcheck_quote_in_conventions"]
              and not res["manifest_data_summary_mismatches"] and res["manifest_file_list_matches_staged"]
              and res["manifest_tables"]["V_rows"] == 15 and res["manifest_tables"]["M_rows"] == 12
              and res["manifest_tables"]["CA_rows"] == 7 and not res["manifest_tables"]["mismatches"]
              and res["evidence_results_commit_matches"])
    res["all_as_expected"] = bool(ok)
    text = H.dump(res)
    if H.HEX40.search(text.encode()):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    print(text)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
