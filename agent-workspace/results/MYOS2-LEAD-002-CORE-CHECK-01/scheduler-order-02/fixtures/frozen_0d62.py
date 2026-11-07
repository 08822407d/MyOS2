# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: write the few frozen files this task reuses, exactly as stored in commit 0d62c4d19711, into a
#   fresh directory (never from the work tree) and verify them byte for byte before any import:
#     core/fixtures/fx_common.h, fx_list.inc.c   - shared fixture header and verbatim list primitives
#     core/fixtures/locate.py                     - locator used by the expansion
#     core/fixtures/fx_sched.c                    - reference only (type choices; its pick() helper is NOT used)
#     core/recheck-01/fixtures/build2.py          - Builder.expand (//@@ORIG expansion), CFLAGS, limits
#     core/recheck-01/fixtures/harness2.py        - run_pg (bounded process-group runs), pins for 'time'
# --------------------------------------------------------------------------------------------------
"""Usage: python3 frozen_0d62.py <new empty dir>"""
import os
import sys

import common4 as C

FILES = {
    "core/fixtures/fx_common.h": "core", "core/fixtures/fx_list.inc.c": "core", "core/fixtures/locate.py": "core",
    "core/fixtures/fx_sched.c": "core", "core/recheck-01/fixtures/build2.py": "recheck", "core/recheck-01/fixtures/harness2.py": "recheck",
}
ROLE = {"core/fixtures/fx_common.h": "fixture header (copied next to the template)",
        "core/fixtures/fx_list.inc.c": "verbatim list primitives via //@@ORIG (copied next to the template)",
        "core/fixtures/locate.py": "locator used by build2.Builder", "core/fixtures/fx_sched.c": "reference only: type choices; not compiled",
        "core/recheck-01/fixtures/build2.py": "Builder.expand, CFLAGS, COMPILE_LIMIT_S, RUN_LIMIT_S",
        "core/recheck-01/fixtures/harness2.py": "run_pg (bounded runs), blob/short12 of the time pin"}


def extract(dest):
    if os.path.exists(dest) and os.listdir(dest):
        raise RuntimeError("refusing non-empty destination: %s" % dest)
    man = {"commit_short12": C.short12(C.PINS["reviewed"]), "dest": os.path.realpath(dest), "files": []}
    for rel, sub in FILES.items():
        data = C.blob(C.PINS["reviewed"], C.RESULTS_ROOT + rel)
        out = os.path.join(dest, sub, os.path.basename(rel))
        os.makedirs(os.path.dirname(out), exist_ok=True)
        with open(out, "wb") as f:
            f.write(data)
        same_head = C.git("diff", "--quiet", C.full(C.PINS["reviewed"]), "HEAD", "--", C.RESULTS_ROOT + rel, check=False).returncode == 0
        man["files"].append({"path": rel, "bytes": len(data), "sha256_segments": C.sha_segments(data),
                             "unchanged_at_head": same_head, "role": ROLE[rel]})
    C.emit(man, os.path.join(dest, "frozen_manifest.json"))
    return man


def verify(dest):
    bad = []
    for rel, sub in FILES.items():
        p = os.path.join(dest, sub, os.path.basename(rel))
        try:
            with open(p, "rb") as f:
                if f.read() != C.blob(C.PINS["reviewed"], C.RESULTS_ROOT + rel):
                    bad.append(rel)
        except OSError:
            bad.append(rel + " (missing)")
    return bad


def import_frozen(dest):
    """Verify, then import harness2/build2 from the extraction (recheck dir first on sys.path)."""
    bad = verify(dest)
    if bad:
        raise RuntimeError("frozen extraction differs from 0d62: %s" % bad)
    rdir = os.path.realpath(os.path.join(dest, "recheck"))
    sys.path.insert(0, rdir)
    import harness2
    import build2
    for m in (harness2, build2):
        if os.path.dirname(os.path.realpath(m.__file__)) != rdir:
            raise RuntimeError("module %s not imported from the frozen extraction" % m.__name__)
    return harness2, build2


if __name__ == "__main__":
    m = extract(sys.argv[1])
    print(C.dump({"commit_short12": m["commit_short12"], "files": [[f["path"], f["bytes"], f["unchanged_at_head"]] for f in m["files"]]}))
