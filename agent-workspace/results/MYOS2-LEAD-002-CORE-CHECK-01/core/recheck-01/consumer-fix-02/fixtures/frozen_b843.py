# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file; same idea as recheck-01/fixtures/frozen.py @ b843d475367a, retargeted to b843
# purpose: write the recheck-01 programs and observations exactly as stored in commit b843d475367a
#   into a fresh directory (never from the work tree), with name list, bytes and SHA-256 per file.
#   Every consumer-fix-02 program that reuses an old module imports it from such an extraction.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 frozen_b843.py <new empty dir>
Writes <dir>/fixtures/*.py, <dir>/observations/**, <dir>/frozen_manifest.json."""
import os
import sys

import common3 as C

FIXTURES = ["a46_check.py", "build2.py", "build_evidence2.py", "compare_runs2.py", "evaluate2.py", "final_check2.py",
            "frozen.py", "h00_2.py", "harness2.py", "identity2.py", "make_results2.py", "meta_tests.py",
            "old_counterexamples.py", "readback2.py", "run_recheck.py"]
OBSERVATIONS = ["a46.json", "after_batch1/identity_after_push.json", "after_batch1/readback_results.branch.json",
                "after_batch1/readback_results.object.json", "after_batch2/identity_after_push.json",
                "after_batch2/readback_results.branch.json", "after_batch2/readback_results.object.json",
                "meta_tests.json", "old_counterexamples.json", "rerun_vs_batch1.json", "run.json", "static.json",
                "v00.json", "v01.json"]
# modules that consumer-fix-02 actually imports or runs (others are extracted for the record only)
USED = ["evaluate2.py", "make_results2.py", "harness2.py", "identity2.py", "readback2.py"]


def extract(dest):
    if os.path.exists(dest) and os.listdir(dest):
        raise RuntimeError("refusing non-empty destination: %s" % dest)
    os.makedirs(dest, exist_ok=True)
    src = C.tree(C.PINS["reviewed_input"], C.R01)
    head = C.tree("HEAD", C.R01)
    names_fx = sorted(p[len(C.R01) + len("fixtures/"):] for p in src if p.startswith(C.R01 + "fixtures/"))
    names_obs = sorted(p[len(C.R01) + len("observations/"):] for p in src if p.startswith(C.R01 + "observations/"))
    man = {"commit_short12": C.short12(C.PINS["reviewed_input"]), "root": C.R01, "dest": os.path.realpath(dest),
           "fixture_names_match_expected": names_fx == sorted(FIXTURES),
           "observation_names_match_expected": names_obs == sorted(OBSERVATIONS),
           "files": []}
    for sub, names in (("fixtures", FIXTURES), ("observations", OBSERVATIONS)):
        for n in names:
            path = "%s%s/%s" % (C.R01, sub, n)
            data = C.blob(C.PINS["reviewed_input"], path)
            out = os.path.join(dest, sub, n)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with open(out, "wb") as f:
                f.write(data)
            man["files"].append({"path": "%s/%s" % (sub, n), "bytes": len(data), "sha256_segments": C.sha_segments(data),
                                 "unchanged_at_head": head.get(path) == src.get(path), "used_by_consumer_fix_02": n in USED})
    man["all_unchanged_at_head"] = all(f["unchanged_at_head"] for f in man["files"])
    C.emit(man, os.path.join(dest, "frozen_manifest.json"))
    return man


def verify(dest):
    """Re-check an extraction against commit b843 (used by every CLI that imports from it)."""
    bad = []
    for sub, names in (("fixtures", FIXTURES), ("observations", OBSERVATIONS)):
        for n in names:
            p = os.path.join(dest, sub, n)
            want = C.blob(C.PINS["reviewed_input"], "%s%s/%s" % (C.R01, sub, n))
            try:
                with open(p, "rb") as f:
                    if f.read() != want:
                        bad.append("%s/%s" % (sub, n))
            except OSError:
                bad.append("%s/%s (missing)" % (sub, n))
    return bad


def import_frozen(fixdir):
    """Put the extracted b843 fixtures first on sys.path and import the reused modules from there."""
    fixdir = os.path.realpath(fixdir)
    bad = verify(os.path.dirname(fixdir))
    bad = [b for b in bad if b.startswith("fixtures/")]
    if bad:
        raise RuntimeError("frozen extraction differs from b843: %s" % bad)
    sys.path.insert(0, fixdir)
    import evaluate2
    import make_results2
    for m in (evaluate2, make_results2):
        if os.path.dirname(os.path.realpath(m.__file__)) != fixdir:
            raise RuntimeError("module %s not imported from the frozen extraction" % m.__name__)
    return evaluate2, make_results2


if __name__ == "__main__":
    m = extract(sys.argv[1])
    print(C.dump({k: m[k] for k in ("commit_short12", "fixture_names_match_expected", "observation_names_match_expected",
                                    "all_unchanged_at_head")}))
    sys.exit(0 if m["fixture_names_match_expected"] and m["observation_names_match_expected"] else 1)
