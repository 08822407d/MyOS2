"""Pre-publish scope and hygiene check for the core delivery (run with files staged).
Prints nothing that depends on the size of evidence.md, so embedding its own output and re-running
must reproduce identical output. Usage: python3 final_check.py <results_commit_short12>
"""
import re
import sys

import yaml

import harness as H

BASE = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/"
CORE = BASE + "core/"
DOCS = ["MANIFEST.md", "evidence.md", "results.yaml"]
TAG = "[" + "VERIFIED"
SECRET = re.compile(r"(gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|AKIA[0-9A-Z]{16}|"
                    r"-----BEGIN [A-Z ]*PRIVATE KEY-----|[Aa]uthorization:\s*\S+)")


def fm(text):
    return yaml.safe_load(text[4:text.index("\n---\n", 4)])


rc = sys.argv[1]
res = {"check": "FINAL_SCOPE_CORE"}
names = [n for n in H._git("diff", "--cached", "--name-status", H.commit("master")).stdout.decode().split("\n") if n]
res["index_vs_master_count"] = len(names)
res["outside_pilot_core"] = [n for n in names if not n.split("\t", 1)[1].startswith((BASE + "pilot/", CORE))]
res["non_added"] = [n for n in names if not n.startswith("A\t")]
res["worktree_outside"] = [l for l in H._git("status", "--porcelain", "--untracked-files=all").stdout.decode().split("\n")
                           if l and not l[3:].startswith(CORE)]
res["pilot_changed_since_pilot_head"] = [n for n in H._git("diff", "--cached", "--name-only", H.commit("pilot_head"), "--",
                                                           BASE + "pilot/").stdout.decode().split("\n") if n]
full = H._git("rev-parse", "--verify", rc + "^{commit}").stdout.decode().strip()
res["results_yaml_changed_since_results_commit"] = H._git("diff", "--cached", "--quiet", full, "--",
                                                          CORE + "results.yaml", check=False).returncode != 0
staged = [n.split("\t", 1)[1] for n in names if n.split("\t", 1)[1].startswith(CORE)]
res["core_files_staged"] = len(staged)
res["binary_like_core_files"] = [p for p in staged if b"\0" in H._git("show", ":" + p).stdout]
res["hex40_hits"] = {p.replace(CORE, ""): len(H.HEX40.findall(H._git("show", ":" + p).stdout)) for p in staged
                     if H.HEX40.search(H._git("show", ":" + p).stdout)}
res["secret_pattern_hits"] = [p.replace(CORE, "") for p in staged if SECRET.search(H._git("show", ":" + p).stdout.decode("utf-8", "replace"))]
res["docs"] = {}
for d in DOCS:
    t = H._git("show", ":" + CORE + d).stdout.decode("utf-8")
    meta = yaml.safe_load(t) if d.endswith(".yaml") else fm(t)
    # line numbers of every upper-case pass token other than the scoped ceiling token, for a reader to
    # judge; only numbers are printed so that embedding this output does not add new tokens
    bare_pass = sorted({t.count("\n", 0, m.start()) + 1 for m in re.finditer(r"\bPASS\w*", t)
                        if m.group(0) != "PASS_PENDING_LOCAL"})
    res["docs"][d] = {"yaml_parses": isinstance(meta, dict), "first_key": next(iter(meta)),
                      "packet_id": meta.get("packet_id"), "verified_tag_count": t.count(TAG),
                      "other_pass_token_lines": bare_pass}
man = fm(H._git("show", ":" + CORE + "MANIFEST.md").stdout.decode("utf-8"))
res["manifest_self_check_consistent"] = man["self_check"]["verified_claims"] == sum(v["verified_tag_count"] for v in res["docs"].values())
conv = H.blob("master", "agent-workspace/conventions.md").decode("utf-8")
res["startup_selfcheck_quote_in_conventions"] = man.get("startup_selfcheck_quote", "\0") in conv
ok = (not res["outside_pilot_core"] and not res["non_added"] and not res["worktree_outside"]
      and not res["pilot_changed_since_pilot_head"] and not res["results_yaml_changed_since_results_commit"]
      and not res["binary_like_core_files"] and not res["hex40_hits"] and not res["secret_pattern_hits"]
      and all(v["yaml_parses"] and v["packet_id"] == "MYOS2-LEAD-002-CORE-CHECK-01" for v in res["docs"].values())
      and res["manifest_self_check_consistent"] and res["startup_selfcheck_quote_in_conventions"])
res["all_as_expected"] = bool(ok)
print(H.dump(res))
sys.exit(0 if ok else 1)
