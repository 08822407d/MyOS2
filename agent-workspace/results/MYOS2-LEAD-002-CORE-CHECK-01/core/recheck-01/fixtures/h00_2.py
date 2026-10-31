# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/h00_hardening.py @ a1e7c2277705 (frozen original unchanged)
# change (R02/R04): probes use harness2.run_pg; added fast-exit, group-already-gone (injected) and
#   collection-timeout (escaped grandchild) probes with explicit terminal states; the readback probe
#   reads the frozen pilot file by object (and by branch with successor check) instead of requiring the
#   branch head to stay at the pilot head. Returns dynamic_safety_ok / transport_ok for the entry gate.
# --------------------------------------------------------------------------------------------------
"""Driver-safety probes. run(workdir, frozen_dir) -> record; never raises."""
import os
import signal
import subprocess
import sys
import time

import harness2 as H
import build2
import readback2 as R

PROBE = ("import os,sys,time\npid=os.fork()\nif pid==0:\n    open(sys.argv[1],'w').write('%d %d'%(os.getpid(),os.getpgrp()))\n"
         "    time.sleep(3); os._exit(0)\ntime.sleep(3)\n")
ESCAPE = ("import os,sys,time\npid=os.fork()\nif pid==0:\n    os.setsid()\n    open(sys.argv[1],'w').write('%d'%os.getpid())\n"
          "    time.sleep(4); os._exit(0)\ntime.sleep(3)\n")


def _state(pid):
    try:
        with open("/proc/%d/stat" % pid) as f:
            return f.read().rsplit(")", 1)[1].split()[0]
    except OSError:
        return "ABSENT"


def _wait_file(p, limit=2.0):
    t0 = time.monotonic()
    while time.monotonic() - t0 < limit:
        if os.path.exists(p) and os.path.getsize(p) > 0:
            return open(p).read().split()
        time.sleep(0.02)
    return None


def probe_group_kill(w):
    pidf = os.path.join(w, "gk.pid")
    by = subprocess.Popen(["sleep", "8"], start_new_session=True)
    r = H.run_pg([sys.executable, "-c", PROBE, pidf], 0.3, cwd=w)
    got = _wait_file(pidf)
    cpid, cpg = (int(got[0]), int(got[1])) if got else (None, None)
    states = []
    for _ in range(30):
        s = _state(cpid) if cpid else "UNKNOWN"
        states.append(s)
        if s in ("ABSENT",):
            break
        time.sleep(0.1)
    by_alive = by.poll() is None
    by.terminate()
    by.wait(timeout=5)
    ok = (r["terminal_state"] == "timeout_group_killed" and r["reaped_confirmed"] and r["elapsed_s"] < 2.0
          and cpg == r["pgid"] and r["pgid_differs_from_own"] and states[-1] in ("ABSENT", "Z") and by_alive)
    return {"probe": "group_kill", "terminal_state": r["terminal_state"], "kill": r["kill"], "elapsed_s": r["elapsed_s"],
            "returncode": r["returncode"], "reaped_confirmed": r["reaped_confirmed"], "child_in_group": cpg == r["pgid"],
            "child_state_last": states[-1], "bystander_alive": by_alive, "as_expected": bool(ok), "gates_dynamic": True}


def probe_fast_exit(w):
    r = H.run_pg(["true"], 5, cwd=w)
    ok = r["terminal_state"] == "exited" and r["returncode"] == 0 and r["reaped_confirmed"] and not r["timed_out"]
    return {"probe": "fast_exit", "terminal_state": r["terminal_state"], "returncode": r["returncode"],
            "reaped_confirmed": r["reaped_confirmed"], "as_expected": bool(ok), "gates_dynamic": True}


def probe_group_gone(w):
    def gone(pgid, sig):
        raise ProcessLookupError("HARNESS_META_TEST injected: group already gone")
    r = H.run_pg(["sleep", "1"], 0.2, cwd=w, _killpg=gone)
    ok = (r["terminal_state"] == "timeout_group_already_gone" and r["kill"] == "group_already_gone"
          and r["reaped_confirmed"] and r["returncode"] == 0)
    return {"probe": "group_already_gone_injected", "terminal_state": r["terminal_state"], "kill": r["kill"],
            "returncode": r["returncode"], "reaped_confirmed": r["reaped_confirmed"], "elapsed_s": r["elapsed_s"],
            "note": "killpg replaced by a stand-in raising ProcessLookupError; the real sleep then exits by itself",
            "as_expected": bool(ok), "gates_dynamic": True}


def probe_collect_timeout(w):
    pidf = os.path.join(w, "esc.pid")
    r = H.run_pg([sys.executable, "-c", ESCAPE, pidf], 0.3, cwd=w, collect_timeout=1.0)
    got = _wait_file(pidf)
    gpid = int(got[0]) if got else None
    esc_before = _state(gpid) if gpid else "UNKNOWN"
    cleaned = None
    if gpid and esc_before not in ("ABSENT", "Z"):
        os.kill(gpid, signal.SIGKILL)  # the escaped grandchild was created by this probe; exact pid only
        cleaned = True
    ok = (r["timed_out"] and r["collect_timed_out"] and r["output_lost"]
          and r["terminal_state"] == "timeout_group_killed_collect_timeout" and r["elapsed_s"] < 4.0)
    return {"probe": "collect_timeout_escaped_grandchild", "terminal_state": r["terminal_state"], "kill": r["kill"],
            "collect_timed_out": r["collect_timed_out"], "output_lost": r["output_lost"],
            "reaped_confirmed": r["reaped_confirmed"], "returncode": r["returncode"], "elapsed_s": r["elapsed_s"],
            "escaped_grandchild_state_before_cleanup": esc_before, "escaped_grandchild_killed_by_exact_pid": cleaned,
            "as_expected": bool(ok), "gates_dynamic": True}


def probe_compile_failure(w, frozen_dir):
    d = os.path.join(w, "cf")
    os.makedirs(d)
    tpl = os.path.join(d, "broken.c")
    open(tpl, "w").write("int main(void) { return 0 }\n")
    stale = os.path.join(d, "broken")
    open(stale, "w").write("#!/bin/sh\necho STALE\n")
    os.chmod(stale, 0o755)
    rec = build2.Builder(frozen_dir).build_and_run("broken", tpl, ["c1", "c2"], d)
    ok = (rec["status"] == "BLOCKED_COMPILE" and not os.path.exists(stale)
          and all(v["status"] == "BLOCKED" for v in rec["cases"].values()) and rec["compile"]["returncode"] != 0)
    return {"probe": "compile_failure", "status": rec["status"], "compile_returncode": rec["compile"]["returncode"],
            "stale_binary_exists_after": os.path.exists(stale), "cases": {k: v["status"] for k, v in rec["cases"].items()},
            "as_expected": bool(ok), "gates_dynamic": True}


def probe_case_exception(w, frozen_dir):
    d = os.path.join(w, "ce")
    os.makedirs(d)
    tpl = os.path.join(d, "ok.c")
    open(tpl, "w").write('#include <stdio.h>\nint main(int c,char**v){printf("{\\"case\\":\\"%s\\"}\\n",v[1]);return 0;}\n')
    rec = build2.Builder(frozen_dir).build_and_run("ok", tpl, ["boom", "fine"], d, inject_case_error="boom")
    ok = rec["cases"]["boom"]["status"] == "ERROR" and rec["cases"]["fine"]["status"] == "RAN" \
        and rec["cases"]["fine"]["run"]["returncode"] == 0
    return {"probe": "case_exception_isolated", "boom": rec["cases"]["boom"], "fine_status": rec["cases"]["fine"]["status"],
            "as_expected": bool(ok), "gates_dynamic": True}


def probe_readback(w):
    pf = H.RESULTS_ROOT + "pilot/result.yaml"
    miss = R.remote_read(H.PINS["pilot_head"], H.RESULTS_ROOT + "pilot/does-not-exist.yaml", "object", workdir=w, tag="miss")
    obj = R.remote_read(H.PINS["pilot_head"], pf, "object", field="transport_marker",
                        value="MYOS2-CLOUD-PILOT-20260925-K7P4", workdir=w, tag="obj")
    br = R.remote_read(H.PINS["pilot_head"], pf, "branch", field="transport_marker",
                       value="MYOS2-CLOUD-PILOT-20260925-K7P4", workdir=w, tag="br")
    ok = (not miss["ok"]) and obj["ok"] and br["ok"]
    return {"probe": "readback_criteria", "missing_path": miss, "frozen_pilot_by_object": obj,
            "frozen_pilot_by_branch": br, "as_expected": bool(ok), "gates_dynamic": False}


def run(workdir, frozen_dir):
    rec = {"check": "H00_2", "probes": []}
    fns = [probe_group_kill, probe_fast_exit, probe_group_gone, probe_collect_timeout,
           lambda w: probe_compile_failure(w, frozen_dir), lambda w: probe_case_exception(w, frozen_dir), probe_readback]
    names = ["group_kill", "fast_exit", "group_already_gone_injected", "collect_timeout_escaped_grandchild",
             "compile_failure", "case_exception_isolated", "readback_criteria"]
    for n, fn in zip(names, fns):
        d = os.path.join(workdir, n)
        os.makedirs(d)
        try:
            rec["probes"].append(fn(d))
        except Exception as e:  # noqa: BLE001
            rec["probes"].append({"probe": n, "error": "%s: %s" % (type(e).__name__, e), "as_expected": False,
                                  "gates_dynamic": n != "readback_criteria"})
    rec["dynamic_safety_ok"] = all(p["as_expected"] for p in rec["probes"] if p.get("gates_dynamic"))
    rec["transport_ok"] = all(p["as_expected"] for p in rec["probes"] if not p.get("gates_dynamic"))
    return rec
