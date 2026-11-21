# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: bind every reuse of the old verifier to the frozen commit a1e7c2277705. Old fixture files
#   are extracted byte-for-byte from that commit into a fresh directory (never from the work tree),
#   and the extracted set is checked against the frozen manifest below.
# --------------------------------------------------------------------------------------------------
"""Extract the frozen core verifier (core/fixtures/ at pin core_frozen) into a new directory."""
import os

import harness2 as H

OLD_FIX = H.RESULTS_ROOT + "core/fixtures/"
# Names of the twenty frozen files at a1e7c2277705; extract() records bytes and all four SHA-256
# segments of what it actually wrote, and reports whether the listed names match.
EXPECTED_FILES = ["build.py", "build_evidence.py", "evaluate.py", "final_check.py", "fx_common.h",
                  "fx_jiffies.c", "fx_list.inc.c", "fx_prims.c", "fx_sched.c", "fx_wait.c",
                  "h00_hardening.py", "harness.py", "locate.py", "make_results.py", "readback.py",
                  "run_all.py", "static_checks.py", "v00_anchors.py", "v00_semantic_review.yaml",
                  "v01_structure.py"]


def list_frozen(repo=None):
    out = H.git("ls-tree", "--name-only", H.pin("core_frozen", repo), OLD_FIX, repo=repo).stdout.decode().split("\n")
    return sorted(os.path.basename(p) for p in out if p)


def extract(dest, repo=None):
    """Write every frozen fixture file into dest (must be a fresh empty dir). Returns a manifest."""
    if os.listdir(dest):
        raise RuntimeError("extract target not empty")
    names = list_frozen(repo)
    man = {"commit_short12": H.short12(H.PINS["core_frozen"], repo), "path": OLD_FIX,
           "names_match_expected": names == sorted(EXPECTED_FILES), "files": []}
    for n in names:
        data = H.blob(H.PINS["core_frozen"], OLD_FIX + n, repo)
        with open(os.path.join(dest, n), "wb") as f:
            f.write(data)
        man["files"].append({"name": n, "bytes": len(data), "sha256_segments": H.sha_segments(data)})
    return man
