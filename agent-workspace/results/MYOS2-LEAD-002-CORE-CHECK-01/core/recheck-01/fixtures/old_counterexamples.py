# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: Phase A - run the FROZEN old verifier (extracted from a1e7c2277705) on the minimal
#   counterexamples R01-R04 stated by the core-review, BEFORE any repair, and save real returns.
#   Every old-side experiment runs in its own bounded subprocess with cwd = the extracted frozen dir,
#   so old module names never mix with the revised ones. Injected inputs are HARNESS_META_TEST.
# --------------------------------------------------------------------------------------------------
"""Usage: RECHECK_ROOT=<parent dir> python3 old_counterexamples.py <out.json>
Creates a fresh directory under RECHECK_ROOT, extracts the frozen verifier there, runs:
  A1 R01  evaluate.v05({"cases": {}}) and the old dispatch entry run_all.evaluate on empty cases
  A2 R02  old run_all.main with counting stand-ins: identity gate false / identity raises / H00 fails
  A3 R04  old identity() and old h00.probe_readback() against the current (advanced) branch head;
          old readback.py CLI with --out and no field=value
  A4      full replay of the frozen old run_all.py (real, bounded) -> old-format outputs
  A5 R03  old make_results.py on the real replay outputs and on derived variants
  A6      old run_pg with an injected ProcessLookupError (group already gone)
"""
import json
import os
import shutil
import sys

import harness2 as H
import frozen

ROOT = os.environ.get("RECHECK_ROOT") or sys.exit("set RECHECK_ROOT")
PY = sys.executable


def sub(frozen_dir, code, env_extra=None, timeout=120, args=()):
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE="1", **(env_extra or {}))
    r = H.run_pg([PY, "-c", code, *args], timeout, cwd=frozen_dir, env=env)
    out = {k: r[k] for k in ("returncode", "timed_out", "terminal_state", "reaped_confirmed", "elapsed_s")}
    out["stdout"] = r["stdout"][-6000:]
    out["stderr_tail"] = r["stderr"][-3000:]
    try:
        out["json"] = json.loads(r["stdout"].strip().splitlines()[-1]) if r["stdout"].strip() else None
    except (ValueError, IndexError):
        out["json"] = None
    return out


A1 = r'''
import json, evaluate as E, run_all as R
v05 = E.v05({"cases": {}})
empty = {k: {"status": "BUILT", "cases": {}} for k in ("fx_wait","fx_sched","fx_prims","fx_jiffies","fx_jiffies_control")}
disp = R.evaluate(empty)
summ = {k: (v.get("verdict") if "verdict" in v else {kk: vv.get("verdict") for kk, vv in v.items() if isinstance(vv, dict)}) for k, v in disp.items()}
print(json.dumps({"v05_empty_return": v05, "dispatch_evaluate_on_empty_cases": summ}, ensure_ascii=False))
'''

A2 = r'''
import json, sys, run_all as R, harness as H
mode = sys.argv[1]
calls = {"stage_script": [], "fixtures": 0, "evaluate": 0}
def ident():
    if mode == "identity_raises":
        raise RuntimeError("HARNESS_META_TEST injected identity failure")
    return {"gate": {"ALLOW_CORE_bound_to_this_execution": mode != "gate_false"}, "note": "HARNESS_META_TEST"}
def stage(name):
    calls["stage_script"].append(name)
    bad = (mode == "h00_fails" and name == "h00_hardening.py")
    return {"script": name, "returncode": 1 if bad else 0, "timed_out": bad, "elapsed_s": 0, "stderr": "", "stdout_tail": ""}
def fx():
    calls["fixtures"] += 1
    return {}
real_eval = R.evaluate
def ev(f):
    calls["evaluate"] += 1
    return real_eval(f)
R.identity, R.stage_script, R.fixtures, R.evaluate = ident, stage, fx, ev
R.tools = lambda: {"stub": "HARNESS_META_TEST"}
err = None
try:
    R.main()
except BaseException as e:
    err = "%s: %s" % (type(e).__name__, e)
print(json.dumps({"mode": mode, "calls": calls, "main_exception": err,
                  "dynamic_entry_calls": len([s for s in calls["stage_script"] if s == "h00_hardening.py"]) + calls["fixtures"]}))
'''

A3 = r'''
import json, run_all as R, h00_hardening as HH
idt = R.identity()
rb = HH.probe_readback()
print(json.dumps({"old_identity_remote_heads_match": idt.get("remote_heads_match"), "old_identity_gate": idt.get("gate"),
                  "old_probe_readback": {"as_expected": rb["as_expected"],
                     "frozen_pilot_file": {k: rb["existing_frozen_pilot_file"][k] for k in ("remote_head_equals_commit", "expected_bytes", "ok")},
                     "frozen_pilot_channels": [{k: c[k] for k in ("http_code", "transport_ok", "byte_identical", "field_ok", "ok")} for c in rb["existing_frozen_pilot_file"]["channels"]],
                     "missing_path_ok": rb["missing_path"]["ok"]}}))
'''

A6 = r'''
import json, os, harness as H
def gone(pgid, sig):
    raise ProcessLookupError("HARNESS_META_TEST injected: group already gone")
os.killpg = gone
err = None; r = None
try:
    r = H.run_pg(["sleep", "2"], 0.2)
except BaseException as e:
    err = "%s: %s" % (type(e).__name__, e)
print(json.dumps({"old_run_pg_returned": r is not None, "exception": err}))
'''


def main():
    out_path = sys.argv[1]
    base = H.fresh_dir(ROOT, "phaseA-")
    fz = os.path.join(base, "frozen_fixtures")
    os.makedirs(fz)
    res = {"phase": "A_old_counterexamples", "workdir": base, "label": "old verifier = frozen a1e7c2277705",
           "frozen_manifest": frozen.extract(fz)}
    w = lambda n: os.path.join(base, n)
    # A1 - R01
    os.makedirs(w("a1"))
    res["A1_R01_empty_observation"] = sub(fz, A1, {"CORE_WORK": w("a1")})
    # A2 - R02 (counting stand-ins; nothing dynamic really runs)
    res["A2_R02_gate_not_enforced"] = {}
    for mode in ("gate_false", "identity_raises", "h00_fails"):
        os.makedirs(w("a2_" + mode))
        res["A2_R02_gate_not_enforced"][mode] = sub(fz, A2, {"CORE_WORK": w("a2_" + mode)}, args=(mode,))
    # A3 - R04 old identity / old readback probe / old readback CLI
    os.makedirs(w("a3"))
    res["A3_R04_old_identity_and_readback_probe"] = sub(fz, A3, {"CORE_WORK": w("a3")}, timeout=180)
    cli = {}
    pilot_file = H.RESULTS_ROOT + "pilot/result.yaml"
    for label, extra in (("no_field_with_out", []), ("with_field_with_out", ["transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4"])):
        outf = w("a3_cli_%s.json" % label)
        r = H.run_pg([PY, "readback.py", H.WORK_BRANCH, H.PINS["pilot_head"], pilot_file, *extra, "--out", outf], 120,
                     cwd=fz, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=w("a3")))
        cli[label] = {"argv_tail": [*extra, "--out", "<file>"], "returncode": r["returncode"], "timed_out": r["timed_out"],
                      "stderr_tail": r["stderr"][-1200:], "out_file_written": os.path.exists(outf),
                      "out_ok": (json.load(open(outf)).get("ok") if os.path.exists(outf) else None)}
    res["A3_R04_old_readback_cli"] = cli
    # A4 - full replay of the frozen old run_all.py (real run, bounded)
    rep = w("a4_replay")
    os.makedirs(rep)
    r = H.run_pg([PY, "run_all.py"], 1200, cwd=fz, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=rep))
    res["A4_old_replay"] = {"returncode": r["returncode"], "timed_out": r["timed_out"], "terminal_state": r["terminal_state"],
                            "elapsed_s": r["elapsed_s"], "stdout": r["stdout"][-4000:], "stderr_tail": r["stderr"][-2000:],
                            "outputs": sorted(os.listdir(rep))}
    try:
        cr = json.load(open(os.path.join(rep, "core_run.json")))
        h0 = json.load(open(os.path.join(rep, "h00.json")))
        res["A4_old_replay"]["identity_remote_heads_match"] = cr["identity"].get("remote_heads_match")
        res["A4_old_replay"]["identity_gate"] = cr["identity"].get("gate")
        res["A4_old_replay"]["stage_returncodes"] = {k: v["returncode"] for k, v in cr["stages"].items()}
        res["A4_old_replay"]["h00_all_as_expected"] = h0.get("all_as_expected")
        res["A4_old_replay"]["h00_readback_probe_as_expected"] = [p.get("as_expected") for p in h0["probes"] if p["probe"] == "readback_criteria"]
        res["A4_old_replay"]["fixtures_called_after_failed_h00"] = bool(cr.get("fixtures"))
        res["A4_old_replay"]["evaluation"] = {k: (v.get("verdict") if "verdict" in v else {kk: vv.get("verdict") for kk, vv in v.items() if isinstance(vv, dict)})
                                              for k, v in cr["evaluation"].items()}
    except Exception as e:  # noqa: BLE001
        res["A4_old_replay"]["summary_error"] = "%s: %s" % (type(e).__name__, e)
    # A5 - R03 old make_results on the real replay outputs and on derived variants
    variants = {}

    def run_make(tag, mutate=None, drop=None):
        d = w("a5_" + tag)
        shutil.copytree(rep, d, ignore=shutil.ignore_patterns("fx_*", "h00"))
        if drop:
            os.remove(os.path.join(d, drop))
        if mutate:
            mutate(d)
        outy = os.path.join(d, "results_generated.yaml")
        r = H.run_pg([PY, "make_results.py", outy], 120, cwd=fz, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=d))
        rec = {"returncode": r["returncode"], "stdout_tail": r["stdout"][-600:], "stderr_tail": r["stderr"][-1500:],
               "generated": os.path.exists(outy)}
        if os.path.exists(outy):
            import yaml
            y = yaml.safe_load(open(outy, encoding="utf-8"))
            rec["case_results"] = {c["case_id"]: [c["result"], c["execution_status"]] for c in y["cases"]}
            rec["ca_rulings"] = {c["issue_id"]: c["ruling"] for c in y["ca_rulings"]}
            rec["v01_observed_referenced_paths_exist"] = [c for c in y["cases"] if c["case_id"] == "V01"][0]["observations"].get("referenced_paths_exist")
            rec["counts"] = y.get("counts")
        variants[tag] = rec

    def v01_false(d):
        p = os.path.join(d, "v01.json")
        j = json.load(open(p))
        for k in j["referenced_paths"]:
            j["referenced_paths"][k]["exists_taskbook"] = False
        j["parse"]["map"] = {"ok": False, "error": "HARNESS_META_TEST injected parse failure"}
        j["_meta"] = "HARNESS_META_TEST derived from real replay v01.json"
        json.dump(j, open(p, "w"))

    def blocked_prims(d):
        p = os.path.join(d, "core_run.json")
        j = json.load(open(p))
        j["fixtures"]["fx_prims"] = {"fixture": "fx_prims", "status": "BLOCKED_COMPILE",
                                     "cases": {"v12_add_test_negative": {"status": "BLOCKED"}, "v13_trylock": {"status": "BLOCKED"}},
                                     "extraction": [], "_meta": "HARNESS_META_TEST"}
        j["evaluation"]["V12"] = {"verdict": "BLOCKED"}
        j["evaluation"]["V13"] = {"verdict": "BLOCKED"}
        json.dump(j, open(p, "w"))

    if os.path.exists(os.path.join(rep, "core_run.json")):
        run_make("baseline_real_replay")
        run_make("v01_paths_missing_and_parse_failed", mutate=v01_false)
        run_make("missing_static_stage", drop="static.json")
        run_make("fixture_blocked_compile", mutate=blocked_prims)
    res["A5_R03_old_make_results"] = variants
    # A6 - old run_pg when the process group is already gone (injected)
    os.makedirs(w("a6"))
    res["A6_old_run_pg_group_gone"] = sub(fz, A6, {"CORE_WORK": w("a6")})
    H.emit(res, out_path)
    print(H.dump({"workdir": base, "A1": res["A1_R01_empty_observation"]["json"],
                  "A2": {k: v["json"] for k, v in res["A2_R02_gate_not_enforced"].items()},
                  "A3": res["A3_R04_old_identity_and_readback_probe"]["json"], "A3cli": cli,
                  "A4": {k: v for k, v in res["A4_old_replay"].items() if k not in ("stdout", "stderr_tail")},
                  "A5": {k: {kk: vv for kk, vv in v.items() if kk in ("returncode", "generated", "case_results", "ca_rulings", "stderr_tail")} for k, v in variants.items()},
                  "A6": res["A6_old_run_pg_group_gone"]["json"]}))


if __name__ == "__main__":
    main()
