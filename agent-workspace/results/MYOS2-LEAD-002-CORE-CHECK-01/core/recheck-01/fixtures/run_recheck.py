# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/run_all.py @ a1e7c2277705 (frozen original unchanged)
# change (R02/R04): order is extract-frozen -> identity gate -> H00 -> read-only stages -> dynamic
#   fixtures -> evaluation. A closed or failed identity gate stops the run before any dynamic entry
#   (h00 and fixtures are both counted). A failed/timed-out H00 safety probe blocks dynamic fixtures
#   while independent read-only stages continue. The record separates execution_complete,
#   verification_findings and infrastructure_errors. Exit codes: 0 complete (findings allowed),
#   2 partial, 3 identity/authorisation blocked, 4 internal error before a record could be completed.
#   hooks/inject parameters exist only for HARNESS_META_TEST runs.
# change (batch 2): static_checks output reaches evaluation only when that stage completed.
# --------------------------------------------------------------------------------------------------
"""Usage: RECHECK_ROOT=<parent> python3 run_recheck.py [--out FILE]
Creates a fresh work directory under RECHECK_ROOT; never reuses or cleans an existing one."""
import json
import os
import platform
import shutil
import sys

import harness2 as H
import frozen
import identity2
import h00_2
import build2
import evaluate2 as E

STAGES = ["v00_anchors.py", "v01_structure.py", "static_checks.py"]
STAGE_OF = {"v00": "v00_anchors.py", "v01": "v01_structure.py", "static": "static_checks.py", "a46": "a46_check.py"}
FIXTURES = {
    "fx_wait": ["v02_single", "v03_second_wake_direct", "v03_second_wake_via_complete", "v03_all_two_waiters",
                "v09_schedule_timeout_values", "v09_uninterruptible_wrapper", "v09_msleep_bounded",
                "v10_done_preset_fast_path", "v10_infinite_notify_during_schedule", "v10_finite_timeout_no_notifier",
                "v11_wait_then_notify_then_reuse"],
    "fx_sched": ["v04_noncurrent_wake", "v05_state_not_in_mask", "v06_double_wake", "v07_cpu_metadata",
                 "v08_pick_combinations", "v08_sequence_idle_requeue", "v08_sequence_idle_switched_out_blocked",
                 "v08_vruntime_requeue_order"],
    "fx_prims": ["v12_add_test_negative", "v13_trylock"],
    "fx_jiffies": ["v14"],
    "fx_jiffies_control": ["v14_control"],
}
LIMIT = 600


def tools():
    def ver(cmd):
        if not shutil.which(cmd[0]):
            return None
        r = H.run_pg(cmd, 10)
        return (r["stdout"] or r["stderr"]).splitlines()[0] if (r["stdout"] or r["stderr"]) else None
    import yaml
    return {"cpu_arch": platform.machine(), "python3": sys.version.split()[0], "gcc": ver(["gcc", "--version"]),
            "git": ver(["git", "--version"]), "curl": ver(["curl", "--version"]), "pyyaml": yaml.__version__}


def run_stage(name, frozen_dir, stage_dir, extra_args=(), script_dir=None):
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=stage_dir)
    path = os.path.join(script_dir or frozen_dir, name)
    r = H.run_pg([sys.executable, path, *extra_args], LIMIT, cwd=script_dir or frozen_dir, env=env)
    return {"script": name, "source": "frozen a1e7c2277705" if not script_dir else "recheck-01", "returncode": r["returncode"],
            "timed_out": r["timed_out"], "terminal_state": r["terminal_state"], "elapsed_s": r["elapsed_s"],
            "stderr_tail": r["stderr"][-1500:], "stdout_tail": r["stdout"][-800:]}


def run_fixtures(frozen_dir, fx_dir, inject):
    b = build2.Builder(frozen_dir)
    recs = {}
    tpl = lambda n: os.path.join(frozen_dir, n + ".c")
    for name in ("fx_wait", "fx_sched"):
        recs[name] = b.build_and_run(name, tpl(name), FIXTURES[name], os.path.join(fx_dir, name),
                                     inject_case_error=(inject.get("case_error") or (None, None))[1]
                                     if (inject.get("case_error") or (None,))[0] == name else None)
    if platform.machine() != "x86_64":
        recs["fx_prims"] = {"fixture": "fx_prims", "status": "BLOCKED_NOT_X86_64",
                            "cases": {c: {"status": "BLOCKED", "reason": "host is not x86-64"} for c in FIXTURES["fx_prims"]}}
    else:
        t = tpl("fx_prims")
        if inject.get("broken_fixture") == "fx_prims":
            t = os.path.join(fx_dir, "fx_prims_broken.c")
            os.makedirs(fx_dir, exist_ok=True)
            shutil.copy(tpl("fx_prims"), t)
            with open(t, "a") as f:
                f.write("\nthis is not C HARNESS_META_TEST\n")
        recs["fx_prims"] = b.build_and_run("fx_prims", t, FIXTURES["fx_prims"], os.path.join(fx_dir, "fx_prims"))
    d = os.path.join(fx_dir, "fx_jiffies")
    os.makedirs(d, exist_ok=True)
    lds = H.blob(H.PINS["time"], "mykernel/arch/x86_64/kernel.lds").decode("utf-8")
    cand = b.L.ld_assignment(lds, "jiffies")
    line = lds.split("\n")[cand[0]["start"] - 1]
    with open(os.path.join(d, "alias.ld"), "w") as f:
        f.write(line + "\n")
    recs["fx_jiffies"] = b.build_and_run("fx_jiffies", tpl("fx_jiffies"), FIXTURES["fx_jiffies"], d,
                                         extra_inputs=[os.path.join(d, "alias.ld")])
    recs["fx_jiffies"]["alias_ld"] = {"source": "time:mykernel/arch/x86_64/kernel.lds", "line": cand[0]["start"], "text": line}
    recs["fx_jiffies_control"] = b.build_and_run("fx_jiffies_control", tpl("fx_jiffies"), FIXTURES["fx_jiffies_control"],
                                                 os.path.join(fx_dir, "fx_jiffies_control"), extra_cflags=["-DCONTROL_SEPARATE"])
    return recs


def load(p):
    try:
        with open(p, encoding="utf-8") as f:
            return json.load(f), None
    except Exception as e:  # noqa: BLE001
        return None, "%s: %s" % (type(e).__name__, e)


def main(argv=None, hooks=None, inject=None):
    argv = sys.argv[1:] if argv is None else argv
    hooks, inject = hooks or {}, inject or {}
    root = os.environ.get("RECHECK_ROOT")
    if not root:
        sys.stderr.write("set RECHECK_ROOT\n")
        return 4
    wd = H.fresh_dir(root, "run-")
    out = argv[argv.index("--out") + 1] if "--out" in argv else os.path.join(wd, "run.json")
    res = {"check": "RECHECK_RUN", "workdir": wd, "meta_injection": inject or None,
           "dynamic_calls": {"h00": 0, "fixtures": 0}, "stages": {}, "infrastructure_errors": []}
    code = 4
    try:
        res["tools"] = tools()
        fz = os.path.join(wd, "frozen")
        os.makedirs(fz)
        res["frozen_manifest"] = frozen.extract(fz)
        if not res["frozen_manifest"]["names_match_expected"]:
            res["infrastructure_errors"].append("frozen verifier file set differs from expected list")
            res["status"] = "ERROR_FROZEN_INPUT"
            code = 4
            return code
        try:
            ident = (hooks.get("identity") or identity2.safe_check)()
        except Exception as e:  # noqa: BLE001 - any identity failure closes the gate
            ident = {"gate": {"ok": False}, "error": "%s: %s" % (type(e).__name__, e)}
        if not isinstance(ident, dict):
            ident = {"gate": {"ok": False}, "error": "identity returned %s" % type(ident).__name__}
        res["identity"] = ident
        if not (ident.get("gate") or {}).get("ok"):
            res["status"] = "ERROR_IDENTITY" if ident.get("error") else "BLOCKED_IDENTITY"
            res["infrastructure_errors"].append("identity gate closed: %s" % (ident.get("error") or
                                                [k for k, v in (ident.get("gate") or {}).items() if not v]))
            res["execution_complete"] = False
            code = 3
            return code
        # H00 (dynamic, counted)
        res["dynamic_calls"]["h00"] += 1
        h00dir = os.path.join(wd, "h00")
        os.makedirs(h00dir)
        try:
            h00 = (hooks.get("h00") or h00_2.run)(h00dir, fz)
        except Exception as e:  # noqa: BLE001
            h00 = {"error": "%s: %s" % (type(e).__name__, e), "dynamic_safety_ok": False, "transport_ok": False}
        res["h00"] = h00
        safety = bool(h00.get("dynamic_safety_ok"))
        if not safety:
            res["infrastructure_errors"].append("H00 dynamic safety not established; dynamic fixtures blocked")
        # read-only stages (independent of H00)
        sdir = os.path.join(wd, "stages")
        os.makedirs(sdir)
        for s in hooks.get("stages", STAGES):
            if inject.get("skip_stage") == s:
                res["stages"][s] = {"script": s, "status": "NOT_RUN", "reason": "HARNESS_META_TEST skipped stage"}
                continue
            res["stages"][s] = (hooks.get("stage") or run_stage)(s, fz, sdir)
        here = os.path.dirname(os.path.abspath(__file__))
        if "v00_anchors.py" in res["stages"] and res["stages"]["v00_anchors.py"].get("returncode") == 0:
            res["stages"]["a46_check.py"] = run_stage("a46_check.py", fz, sdir, (fz, os.path.join(sdir, "v00.json"),
                                                      os.path.join(sdir, "a46.json")), script_dir=here)
        # dynamic fixtures (counted) only when H00 established safety
        if safety:
            res["dynamic_calls"]["fixtures"] += 1
            try:
                fx = (hooks.get("fixtures") or run_fixtures)(fz, os.path.join(wd, "fixtures"), inject)
            except Exception as e:  # noqa: BLE001
                fx = {n: {"fixture": n, "status": "ERROR", "reason": "%s: %s" % (type(e).__name__, e), "cases": {}} for n in FIXTURES}
        else:
            fx = {n: {"fixture": n, "status": "BLOCKED_SAFETY", "cases": {c: {"status": "BLOCKED", "reason": "H00"} for c in cs}}
                  for n, cs in FIXTURES.items()}
        res["fixtures"] = fx
        # evaluation
        outs = {}
        for key, fname in (("v00", "v00.json"), ("v01", "v01.json"), ("static", "static.json"), ("a46", "a46.json")):
            outs[key], err = load(os.path.join(sdir, fname))
            if err:
                res.setdefault("stage_output_errors", {})[key] = err
        # a stage output is evidence only when its stage completed (exit 0, no timeout); a file left behind
        # by a failed stage is not used (V00/V01/A46 evaluators receive the stage record and check it)
        done = lambda s: (res["stages"].get(s) or {}).get("returncode") == 0 and not (res["stages"].get(s) or {}).get("timed_out")
        res["stage_outputs_usable"] = {k: outs[k] is not None and done(STAGE_OF[k]) for k in outs}
        ev = E.evaluate_all(fx, outs["static"] if res["stage_outputs_usable"]["static"] else None)
        ev["V00"] = E.v00(outs["v00"], res["stages"].get("v00_anchors.py"))
        ev["V01"] = E.v01(outs["v01"], res["stages"].get("v01_structure.py"))
        ev["V00_A46_correction"] = E.a46r(outs["a46"], res["stages"].get("a46_check.py"))
        res["evaluation"] = ev
        res["stage_outputs_present"] = {k: v is not None for k, v in outs.items()}
        # summary
        statuses = {k: v.get("status") for k, v in ev.items()}
        res["verification_findings"] = sorted(k for k, v in ev.items() if v.get("status") == "VALID" and v.get("result") == "COUNTEREVIDENCE")
        for k, st in statuses.items():
            if st != "VALID":
                res["infrastructure_errors"].append("%s evidence status %s" % (k, st))
        for s, r in res["stages"].items():
            if r.get("status") == "NOT_RUN" or r.get("returncode") != 0 or r.get("timed_out"):
                res["infrastructure_errors"].append("stage %s not completed" % s)
        res["execution_complete"] = not res["infrastructure_errors"]
        res["status"] = "COMPLETE" if res["execution_complete"] else "PARTIAL"
        code = 0 if res["execution_complete"] else 2
        return code
    except Exception as e:  # noqa: BLE001
        res["status"] = "ERROR_INTERNAL"
        res["infrastructure_errors"].append("internal %s: %s" % (type(e).__name__, e))
        code = 4
        return code
    finally:
        res["exit_code"] = code
        H.emit(res, out)


if __name__ == "__main__":
    sys.exit(main())
