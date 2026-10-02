"""H00: driver hardening probes required by the pilot review sec.3, run before V00-V14.

1. Process-group timeout: a parent that forks a sleeping child holding the stdout pipe is limited to
   0.3 s; the whole group must be killed, collection must finish promptly, the child must be gone.
2. Contrast: the pilot-era runner (kill parent only, unbounded communicate) on the same probe,
   itself bounded by run_pg(10 s); shows why the pilot runner was not a process-tree guarantee.
3. Unrelated process safety: an independent sleeper in its own session must survive probe 1.
4. Compile failure: a stale binary is present, the fixture source is broken; nothing may run and
   the stale binary must be gone. A Python exception inside one case must not abort the others.
5. Readback criteria: a missing path must fail (HTTP 404 => ok False); an existing frozen pilot file
   at the reviewed pilot head must pass all criteria.
"""
import os
import signal
import subprocess
import sys
import time

import harness as H
import build as B
import readback as R

W = os.path.join(H.WORK, "h00")
os.makedirs(W, exist_ok=True)
PROBE = (
    "import os,sys,time\n"
    "pid=os.fork()\n"
    "if pid==0:\n"
    "    open(sys.argv[1],'w').write('%d %d'%(os.getpid(),os.getpgrp()))\n"
    "    time.sleep(3); os._exit(0)\n"
    "time.sleep(3)\n"
)
OLD_RUNNER = (  # verbatim logic of pilot common.run_limited timeout branch
    "import os,subprocess,sys,time,json\n"
    "t0=time.monotonic()\n"
    "p=subprocess.Popen([sys.executable,'-c',sys.argv[1],sys.argv[2]],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n"
    "try:\n"
    "    out,err=p.communicate(timeout=0.3); to=False\n"
    "except subprocess.TimeoutExpired:\n"
    "    to=True; p.kill(); t_kill=time.monotonic()-t0\n"
    "    time.sleep(0.2)\n"
    "    cpid=int(open(sys.argv[2]).read().split()[0])\n"
    "    alive_after_parent_kill=os.path.exists('/proc/%d'%cpid)\n"
    "    out,err=p.communicate()\n"
    "print(json.dumps({'timed_out':to,'elapsed_s':round(time.monotonic()-t0,3),\n"
    "  'child_alive_0p2s_after_parent_kill':alive_after_parent_kill}))\n"
)


def proc_state(pid):
    try:
        with open("/proc/%d/stat" % pid) as f:
            return f.read().split(")")[-1].split()[0]
    except OSError:
        return "ABSENT"


def probe_group_kill():
    pidfile = os.path.join(W, "child1.pid")
    if os.path.exists(pidfile):
        os.remove(pidfile)
    bystander = subprocess.Popen(["sleep", "8"], start_new_session=True)
    r = H.run_pg([sys.executable, "-c", PROBE, pidfile], 0.3, cwd=W)
    cpid, cpg = (int(x) for x in open(pidfile).read().split())
    states = []
    for _ in range(20):  # bounded wait for reparent+reap, max ~2 s
        s = proc_state(cpid)
        states.append(s)
        if s == "ABSENT":
            break
        time.sleep(0.1)
    by_alive = bystander.poll() is None
    bystander.terminate(); bystander.wait(timeout=5)
    ok = (r["timed_out"] and r["group_killed"] and not r["collect_timed_out"] and r["elapsed_s"] < 2.0
          and cpg == r["pgid"] and r["pgid"] != r["own_pgid"] and states[-1] in ("ABSENT", "Z") and by_alive)
    r.pop("stdout"); r.pop("stderr")
    return {"probe": "group_kill", "run": r, "child_pid_pgid_match_group": cpg == r["pgid"],
            "child_state_samples": states, "bystander_alive_after_probe": by_alive,
            "expected": "timed_out & group_killed & elapsed<2s & child gone/zombie & bystander alive",
            "as_expected": bool(ok), "label": "TIMEOUT_GROUP_KILLED" if ok else "UNEXPECTED"}


def probe_old_runner():
    pidfile = os.path.join(W, "child2.pid")
    if os.path.exists(pidfile):
        os.remove(pidfile)
    r = H.run_pg([sys.executable, "-c", OLD_RUNNER, PROBE, pidfile], 10, cwd=W)
    ev = B.parse_events(r["stdout"])
    d = ev[0] if ev else {}
    weak = bool(d) and d.get("child_alive_0p2s_after_parent_kill") and d.get("elapsed_s", 0) >= 2.5
    return {"probe": "pilot_runner_contrast", "outer_timed_out": r["timed_out"], "result": d,
            "expected": "pilot runner waits for the orphaned child (~3 s) and child survives parent kill",
            "weakness_reproduced": bool(weak)}


def probe_compile_failure():
    d = os.path.join(W, "cf")
    os.makedirs(d, exist_ok=True)
    tpl = os.path.join(d, "broken.c")
    open(tpl, "w").write("int main(void) { return 0 }\n")  # missing ';'
    stale = os.path.join(d, "broken")
    open(stale, "w").write("#!/bin/sh\necho STALE\n")
    os.chmod(stale, 0o755)
    rec = B.build_and_run("broken", tpl, ["c1", "c2"], d)
    ok = (rec["status"] == "BLOCKED_COMPILE" and not os.path.exists(stale)
          and all(v["status"] == "BLOCKED" for v in rec["cases"].values())
          and rec["compile"]["returncode"] != 0)
    comp = {k: rec["compile"][k] for k in ("returncode", "timed_out", "binary_exists")}
    comp["stderr_first_line"] = rec["compile"]["stderr"].splitlines()[0] if rec["compile"]["stderr"] else ""
    return {"probe": "compile_failure", "status": rec["status"], "compile": comp,
            "stale_binary_exists_after": os.path.exists(stale), "cases": rec["cases"],
            "as_expected": bool(ok)}


def probe_case_exception():
    d = os.path.join(W, "ce")
    os.makedirs(d, exist_ok=True)
    tpl = os.path.join(d, "ok.c")
    open(tpl, "w").write('#include <stdio.h>\n#include <string.h>\nint main(int c,char**v){'
                         'printf("{\\"case\\":\\"%s\\"}\\n",v[1]);return 0;}\n')
    orig = H.run_pg
    calls = {"n": 0}

    def flaky(cmd, timeout, **kw):  # raise inside the first case's run only
        if cmd and cmd[-1] == "boom":
            calls["n"] += 1
            raise RuntimeError("injected failure")
        return orig(cmd, timeout, **kw)
    H.run_pg = flaky
    try:
        rec = B.build_and_run("ok", tpl, ["boom", "fine"], d)
    finally:
        H.run_pg = orig
    ok = rec["cases"]["boom"]["status"] == "ERROR" and rec["cases"]["fine"]["status"] == "RAN" \
        and rec["cases"]["fine"]["run"]["returncode"] == 0
    return {"probe": "case_exception_isolated", "boom": rec["cases"]["boom"],
            "fine_status": rec["cases"]["fine"]["status"],
            "fine_events": rec["cases"]["fine"].get("run", {}).get("events"), "as_expected": bool(ok)}


def probe_readback():
    miss = R.remote_read("claude/dazzling-cori-q0dnyt", H.PINS["pilot_head"],
                         "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/does-not-exist.yaml",
                         tag="h00miss")
    good = R.remote_read("claude/dazzling-cori-q0dnyt", H.PINS["pilot_head"],
                         "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/pilot/result.yaml",
                         "transport_marker", "MYOS2-CLOUD-PILOT-20260925-K7P4", tag="h00good")
    return {"probe": "readback_criteria", "missing_path": miss, "existing_frozen_pilot_file": good,
            "as_expected": (not miss["ok"]) and good["ok"]}


if __name__ == "__main__":
    res = {"check": "H00", "probes": []}
    for fn in (probe_group_kill, probe_old_runner, probe_compile_failure, probe_case_exception, probe_readback):
        try:
            res["probes"].append(fn())
        except Exception as e:  # record and continue
            res["probes"].append({"probe": fn.__name__, "error": "%s: %s" % (type(e).__name__, e),
                                  "as_expected": False})
    res["all_as_expected"] = all(p.get("as_expected", p.get("weakness_reproduced", False))
                                 for p in res["probes"])
    H.emit(res, os.path.join(H.WORK, "h00.json"))
    sys.exit(0 if res["all_as_expected"] else 1)
