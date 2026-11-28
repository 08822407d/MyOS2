# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-MM-BASELINE-04 (mm-baseline-04)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file written for this batch; no code copied from earlier packages
# purpose: binding and boundary check required by section 2 and 6 of the 17 contract.
#   start : contract/review fields at the recorded lead object; the lead branch only added files since
#           then; work branch head contains ee9e6a738224; no file of ee9e6a738224 changed; time/master
#           pins on the remote; PR base not containing the work head (i.e. not merged into master).
#   stage : start checks + the index adds files only under mm-baseline-04/, at most two new .py files,
#           hygiene of the staged new files (no 40-hex, no secret pattern, no NUL/ZIP, YAML/JSON parse,
#           provenance header in every new file).
#   docs  : stage checks + the results commit given as argument is an ancestor of HEAD and the docs
#           batch changes no file of the results commit; MANIFEST lists exactly the delivered files.
#   Read-only on git (reads the index); prints one JSON document; never writes into the repository.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 mm_scope.py start|stage|docs [<results_commit_short12>]"""
import json
import re
import subprocess
import sys

import yaml

REPO = "/home/user/MyOS2"
PINS = {"time": "a039d9803ade", "master": "de3bb1df906a", "lead": "c2176ad4da02",
        "reviewed": "ee9e6a738224", "integration_results": "b7fa83583e35"}
WORK_BRANCH = "claude/dazzling-cori-q0dnyt"
LEAD_BRANCH = "agent/MYOS2-LEAD-002"
RESULTS_ROOT = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
PREFIX = RESULTS_ROOT + "mm-baseline-04/"
LEAD_DIR = "agent-workspace/lead/MYOS2-LEAD-002/"
CONTRACT = LEAD_DIR + "17-memory-baseline-contract.md"
REVIEW = LEAD_DIR + "reviews/CORE-SCHED-INTEGRATION-03-review.md"
HEX40 = re.compile(r"\b[0-9a-f]{40}\b")
SECRET = re.compile(r"(gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|AKIA[0-9A-Z]{16}|"
                    r"-----BEGIN [A-Z ]*PRIVATE KEY-----|[Aa]uthorization:\s*\S+)")
MAX_NEW_SCRIPTS = 2


def git(*args, check=True):
    p = subprocess.run(["git", "-C", REPO] + list(args), capture_output=True)
    if check and p.returncode != 0:
        raise RuntimeError("git %s failed: %s" % (" ".join(args[:3]), p.stderr.decode()[:200]))
    return p


def out(*args):
    return git(*args).stdout.decode("utf-8", "replace")


def s12(rev):
    return out("rev-parse", rev + "^{commit}").strip()[:12]


def front_matter(text):
    m = re.match(r"^---\n(.*?)\n---\n", text, re.S)
    return yaml.safe_load(m.group(1)) if m else {}


def check_lead(res):
    remote = out("ls-remote", "origin", "refs/heads/" + LEAD_BRANCH).split("\t")[0].strip()
    res["lead_remote_head_short12"] = remote[:12]
    rel = "equal" if remote[:12] == PINS["lead"] else (
        "descendant" if git("merge-base", "--is-ancestor", PINS["lead"], remote, check=False).returncode == 0 else "other")
    res["lead_relation"] = rel
    ok = rel in ("equal", "descendant")
    if rel == "descendant":                      # later lead commits may only add files
        st = [l for l in out("diff", "--name-status", PINS["lead"], remote).split("\n") if l]
        res["lead_later_changes"] = st
        ok = ok and all(l.startswith("A\t") for l in st)
    c = front_matter(out("show", "%s:%s" % (PINS["lead"], CONTRACT)))
    r = front_matter(out("show", "%s:%s" % (PINS["lead"], REVIEW)))
    want_c = {"followup_id": "CORE-MM-BASELINE-04", "reviewed_execution_short12": PINS["reviewed"],
              "integration_results_frozen_short12": PINS["integration_results"], "kernel_short12": PINS["time"],
              "execution_branch": WORK_BRANCH, "initial_execution_pr": 17, "allowed_write_prefix": PREFIX,
              "kernel_change_authorized": False, "kernel_patch_production_authorized": False,
              "host_fixture_execution_authorized": False, "full_kernel_build_authorized": False,
              "candidate_A_adopted": False, "candidate_B_adopted": False,
              "required_review_record": "CORE-SCHED-INTEGRATION-03-REVIEW-001"}
    want_r = {"record_id": "CORE-SCHED-INTEGRATION-03-REVIEW-001", "reviewed_commit_short12": PINS["reviewed"],
              "results_frozen_short12": PINS["integration_results"], "acceptance_verdict": "RETURN",
              "returned_items": ["IR-01", "IR-02"], "next_followup": "CORE-MM-BASELINE-04",
              "kernel_change_authorized": False}
    res["contract_fields"] = {k: {"want": v, "got": c.get(k), "ok": c.get(k) == v} for k, v in want_c.items()}
    res["review_fields"] = {k: {"want": v, "got": r.get(k), "ok": r.get(k) == v} for k, v in want_r.items()}
    return ok and all(x["ok"] for x in res["contract_fields"].values()) and all(x["ok"] for x in res["review_fields"].values())


def check_pins(res):
    ok = True
    for name, ref in (("time", "time"), ("master", "master")):
        h = out("ls-remote", "origin", "refs/heads/" + ref).split("\t")[0].strip()[:12]
        res["remote_%s_short12" % name] = h
        ok = ok and h == PINS[name]
    head = s12("HEAD")
    res["head_short12"] = head
    res["head_contains_reviewed"] = git("merge-base", "--is-ancestor", PINS["reviewed"], "HEAD", check=False).returncode == 0
    res["master_contains_reviewed"] = git("merge-base", "--is-ancestor", PINS["reviewed"], "origin/master",
                                          check=False).returncode == 0
    return ok and res["head_contains_reviewed"]


def changed_vs_reviewed(res, cached):
    args = ["diff", "--name-status", "--no-renames"] + (["--cached"] if cached else []) + [PINS["reviewed"]]
    st = [l.split("\t") for l in out(*args).split("\n") if l]
    if not cached:
        st += [["??", p] for p in out("ls-files", "--others", "--exclude-standard").split("\n") if p]
    res["changes_vs_reviewed"] = ["%s\t%s" % (a, b) for a, b in st]
    bad = [x for x in st if x[0] not in ("A", "??") or not x[1].startswith(PREFIX)]
    res["changes_outside_rule"] = ["%s\t%s" % (a, b) for a, b in bad]
    return [b for a, b in st if a in ("A", "??")], not bad


def hygiene(paths, res):
    rows, ok = [], True
    for p in paths:
        blob = git("show", ":" + p, check=False).stdout if git("ls-files", "--error-unmatch", p, check=False).returncode == 0 \
            else open(REPO + "/" + p, "rb").read()
        row = {"path": p[len(PREFIX):], "bytes": len(blob)}
        row["nul_or_zip"] = b"\x00" in blob or blob[:4] == b"PK\x03\x04"
        text = blob.decode("utf-8", "replace")
        row["hex40"] = bool(HEX40.search(text))
        row["secret_pattern"] = bool(SECRET.search(text))
        row["model_marker"] = "unknown_or_not_attestable" in text
        if p.endswith((".yaml", ".yml")):
            try:
                yaml.safe_load(text); row["parse"] = "yaml_ok"
            except Exception as e:  # noqa: BLE001
                row["parse"] = "yaml_error:%s" % type(e).__name__
        elif p.endswith(".json"):
            try:
                json.loads(text); row["parse"] = "json_ok"
            except Exception as e:  # noqa: BLE001
                row["parse"] = "json_error:%s" % type(e).__name__
        if p.endswith(".py"):
            row["provenance_header"] = text.startswith("# ---- provenance")
        bad = row["nul_or_zip"] or row["hex40"] or row["secret_pattern"] or str(row.get("parse", "ok")).find("error") >= 0 \
            or row.get("provenance_header") is False or (p.endswith((".md", ".yaml", ".py")) and not row["model_marker"])
        row["ok"] = not bad
        ok = ok and row["ok"]
        rows.append(row)
    res["new_files"] = rows
    scripts = [p for p in paths if p.endswith(".py")]
    res["new_scripts"] = [p[len(PREFIX):] for p in scripts]
    return ok and len(scripts) <= MAX_NEW_SCRIPTS


def manifest_list(paths, res):
    m = PREFIX + "MANIFEST.md"
    blob = git("show", ":" + m, check=False)
    text = blob.stdout.decode("utf-8") if blob.returncode == 0 else ""
    listed = set(re.findall(r"`((?:scripts|records)/[^`\s]+|[A-Za-z0-9_.-]+\.(?:md|yaml))`", text))
    actual = {p[len(PREFIX):] for p in paths}
    res["manifest_missing"] = sorted(actual - listed - {"MANIFEST.md"})
    res["manifest_extra"] = sorted(x for x in listed - actual if "/" in x or x.endswith((".md", ".yaml")))
    return not res["manifest_missing"] and not res["manifest_extra"]


def main(argv):
    mode = argv[0] if argv else "start"
    res = {"check": "MM_BASELINE_04_SCOPE", "mode": mode, "pins": PINS, "work_branch": WORK_BRANCH, "prefix": PREFIX}
    ok = check_lead(res)
    ok = check_pins(res) and ok
    if mode == "start":
        _, clean = changed_vs_reviewed(res, cached=False)
        res["start_note"] = "worktree compared with ee9e6a738224; only untracked additions under the prefix allowed"
        ok = ok and clean
    else:
        paths, clean = changed_vs_reviewed(res, cached=True)
        ok = ok and clean and hygiene(paths, res)
        if mode == "docs":
            rc = argv[1]
            res["results_commit_short12"] = rc
            res["results_is_ancestor_of_head"] = git("merge-base", "--is-ancestor", rc, "HEAD", check=False).returncode == 0
            later = [l for l in out("diff", "--cached", "--name-status", "--no-renames", rc).split("\n") if l]
            res["changes_since_results"] = later
            res["results_files_modified"] = [l for l in later if not l.startswith("A\t")]
            ok = ok and res["results_is_ancestor_of_head"] and not res["results_files_modified"] and manifest_list(paths, res)
    res["all_ok"] = bool(ok)
    text = json.dumps(res, ensure_ascii=False, indent=1)
    if HEX40.search(text):
        sys.stderr.write("REFUSED: 40-hex in output\n")
        return 3
    print(text)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
