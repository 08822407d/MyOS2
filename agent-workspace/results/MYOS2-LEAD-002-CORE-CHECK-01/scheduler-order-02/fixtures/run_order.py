# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file. Reuses, from a verified 0d62c4d19711 extraction (frozen_0d62.py), build2.Builder.expand
#   with build2.CFLAGS / COMPILE_LIMIT_S (60 s) / RUN_LIMIT_S (5 s) and harness2.run_pg. build2's own
#   build_and_run is NOT used because it keeps only the first run's full output.
# purpose: one isolated build of fixtures/fx_order.c (original pick_next_task_myos and list primitives
#   copied verbatim from time a039d9803ade), then every scenario W01-W08 run twice in its own process;
#   both runs are kept in full (stdout, stderr, exit code, timeout, terminal state). A failed or timed-out
#   compile runs nothing; a stale binary cannot exist because the build directory is new.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 run_order.py <frozen extraction dir> <work parent> <observations dir>"""
import os
import shutil
import sys

import common4 as C
import frozen_0d62 as FZ

HERE = os.path.dirname(os.path.abspath(__file__))
SCENARIOS = ["W01", "W02", "W03", "W04", "W05", "W06", "W07", "W08"]
REPS = 2


def main(frozen, parent, obs):
    H, B = FZ.import_frozen(frozen)
    wd = C.fresh_dir(parent, "order-")
    bd = os.path.join(wd, "build")
    os.makedirs(bd)
    tpl = os.path.join(bd, "fx_order.c")
    shutil.copy(os.path.join(HERE, "fx_order.c"), tpl)
    for n in ("fx_common.h", "fx_list.inc.c"):
        shutil.copy(os.path.join(frozen, "core", n), bd)
    rec = {"check": "SCHED_ORDER_02_RUN", "workdir": wd, "frozen_extraction": os.path.realpath(frozen),
           "template": C.file_id(tpl), "template_source": "scheduler-order-02/fixtures/fx_order.c",
           "copied_next_to_template": {n: C.file_id(os.path.join(bd, n)) for n in ("fx_common.h", "fx_list.inc.c")},
           "limits": {"compile_s": B.COMPILE_LIMIT_S, "run_s": B.RUN_LIMIT_S, "walk_links": 16}, "cflags": B.CFLAGS,
           "compiler": shutil.which("gcc")}
    tv = lambda cmd: ((H.run_pg(cmd, 10)["stdout"] or "").splitlines() or [None])[0]
    rec["tools"] = {"gcc": tv([rec["compiler"], "--version"]) if rec["compiler"] else None, "python3": sys.version.split()[0],
                    "git": tv(["git", "--version"])}
    builder = B.Builder(os.path.join(frozen, "core"))
    csrc = os.path.join(bd, "fx_order.expanded.c")
    exe = os.path.join(bd, "fx_order")
    try:
        rec["extraction"] = builder.expand(tpl, csrc)
        rec["expanded_source"] = C.file_id(csrc)
    except Exception as e:  # noqa: BLE001
        rec["status"] = "BLOCKED_EXTRACTION"
        rec["extraction_error"] = "%s: %s" % (type(e).__name__, e)
        C.emit(rec, os.path.join(obs, "runs.json"))
        return 2
    shutil.copy(csrc, os.path.join(obs, "fx_order.expanded.c"))
    if not rec["compiler"]:
        rec["status"] = "BLOCKED_NO_COMPILER"
        C.emit(rec, os.path.join(obs, "runs.json"))
        return 2
    comp = H.run_pg([rec["compiler"], *B.CFLAGS, "-I", bd, "-o", exe, csrc], B.COMPILE_LIMIT_S, cwd=bd)
    comp["binary_exists"] = os.path.exists(exe)
    rec["compile"] = comp
    if comp["timed_out"] or comp["returncode"] != 0 or not comp["binary_exists"]:
        rec["status"] = "BLOCKED_COMPILE"
        C.emit(rec, os.path.join(obs, "runs.json"))
        return 2
    rec["binary"] = {"bytes": os.path.getsize(exe), "committed": False}
    rec["status"] = "BUILT"
    rec["scenarios"] = {}
    for sc in SCENARIOS:
        runs = []
        for rep in range(1, REPS + 1):
            r = H.run_pg([exe, sc], B.RUN_LIMIT_S, cwd=bd)
            r["rep"] = rep
            r["cmd"] = ["<build>/fx_order", sc]
            runs.append(r)
        rec["scenarios"][sc] = {"runs": runs,
                                "stdout_identical": runs[0]["stdout"] == runs[1]["stdout"],
                                "stderr_identical": runs[0]["stderr"] == runs[1]["stderr"],
                                "returncode_identical": runs[0]["returncode"] == runs[1]["returncode"]}
    C.emit(rec, os.path.join(obs, "runs.json"))
    print(C.dump({"status": rec["status"], "compile_rc": comp["returncode"], "compile_stderr_bytes": len(comp["stderr"]),
                  "scenarios": {k: [[r["returncode"], r["terminal_state"]] for r in v["runs"]] + [v["stdout_identical"]]
                                for k, v in rec["scenarios"].items()}}))
    return 0


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:4]))
