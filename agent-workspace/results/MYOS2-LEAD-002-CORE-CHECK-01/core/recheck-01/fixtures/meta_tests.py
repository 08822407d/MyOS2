# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: M01-M12 of the recheck contract. Old side = frozen verifier extracted from a1e7c2277705 and
#   run in its own subprocess (or its Phase A record where the old entry needs the full old replay);
#   new side = revised modules in this directory. Every altered input is labelled HARNESS_META_TEST;
#   none of them is a MyOS2 original-function result. M12 is the real same-source run supplied as input.
# change (batch 2): M09 also requires V08 = INCOMPLETE_EVIDENCE with its limited-model layer listed when the
#   static stage is missing, and adds variant B (static stage reported failed after writing its output).
# --------------------------------------------------------------------------------------------------
"""Usage: RECHECK_ROOT=<parent> python3 meta_tests.py <canonical run.json> <old_counterexamples.json> <out.json>"""
import copy
import json
import os
import re
import subprocess
import sys

import harness2 as H
import frozen
import evaluate2 as E
import identity2
import run_recheck
import make_results2 as MR
import readback2

ROOT = os.environ.get("RECHECK_ROOT") or sys.exit("set RECHECK_ROOT")
PY = sys.executable
META = "HARNESS_META_TEST"
EVID = H.RESULTS_ROOT + "core/evidence.md"
LINE = re.compile("^命令 `<CORE_WORK>/(fx_\\w+)/\\1 (\\w+)`：退出码 (-?\\d+)，超时 (\\w+)，重复一致 (\\w+)，stderr (\\d+) 字节。stdout：$")


# ---------------------------------------------------------------- inputs
def delivered():
    """Fixture records rebuilt from the delivered core/evidence.md @ a1e7c2277705 (stdout blocks + exit codes)."""
    lines = H.blob(H.PINS["core_frozen"], EVID).decode("utf-8").split("\n")
    recs = {}
    for i, ln in enumerate(lines):
        m = LINE.match(ln)
        if not m:
            continue
        fx, case, rc = m.group(1), m.group(2), int(m.group(3))
        j = i + 1
        while not lines[j].startswith("```"):
            j += 1
        k = j + 1
        block = []
        while lines[k] != "```":
            block.append(lines[k])
            k += 1
        recs.setdefault(fx, {"status": "BUILT", "cases": {}, "_meta": META + " rebuilt from delivered evidence"})
        recs[fx]["cases"][case] = run_rec("\n".join(block) + "\n", rc)
    return recs


def run_rec(stdout, rc, **flags):
    r = {"returncode": rc, "timed_out": False, "collect_timed_out": False, "terminal_state": "exited",
         "reaped_confirmed": True, "stdout": stdout, "stderr": ""}
    r.update({k: v for k, v in flags.items() if k in r})
    return {"status": flags.get("status", "RAN"), "run": r, "repeat_identical": flags.get("repeat_identical", True),
            "repeat_returncode": flags.get("repeat_returncode", rc)}


def events_of(stdout):
    return [json.loads(x) for x in stdout.splitlines() if x.strip()]


def to_stdout(evs):
    return "".join((e if isinstance(e, str) else json.dumps(e, separators=(",", ":"))) + "\n" for e in evs)


OLD_EVAL = r'''
import json, sys, evaluate as E, build as B
spec = json.load(open(sys.argv[1]))
fx = spec["fx"]
for f in fx.values():
    for c in f.get("cases", {}).values():
        if isinstance(c.get("run"), dict) and "stdout" in c["run"]:
            c["run"]["events"] = B.parse_events(c["run"]["stdout"])
try:
    r = getattr(E, spec["fn"])(*[fx.get(a, {}) for a in spec["args"]])
    out = {"verdict": r.get("verdict"), "sub": {k: v.get("verdict") for k, v in r.get("subcases", {}).items()}}
except Exception as e:
    out = {"exception": "%s: %s" % (type(e).__name__, e)}
print(json.dumps(out))
'''
OLD_ARGS = {"v02": ["fx_wait"], "v03": ["fx_wait"], "v04": ["fx_sched"], "v05": ["fx_sched"], "v06": ["fx_sched"],
            "v09": ["fx_wait"], "v14": ["fx_jiffies", "fx_jiffies_control"]}


class Ctx:
    def __init__(self):
        self.base = H.fresh_dir(ROOT, "meta-")
        self.fz = os.path.join(self.base, "frozen")
        os.makedirs(self.fz)
        self.manifest = frozen.extract(self.fz)
        self.n = 0

    def tmp(self, name):
        self.n += 1
        d = os.path.join(self.base, "%02d_%s" % (self.n, name))
        os.makedirs(d)
        return d

    def old(self, fn, fx):
        d = self.tmp("old_" + fn)
        spec = os.path.join(d, "spec.json")
        json.dump({"fn": fn, "args": OLD_ARGS[fn], "fx": fx}, open(spec, "w"))
        r = H.run_pg([PY, "-c", OLD_EVAL, spec], 60, cwd=self.fz,
                     env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=d))
        try:
            return json.loads(r["stdout"].strip().splitlines()[-1])
        except Exception:  # noqa: BLE001
            return {"harness_error": r["stderr"][-500:], "returncode": r["returncode"]}


def new_brief(e):
    return {"status": e.get("status"), "result": e.get("result"), "reasons": (e.get("evidence_reasons") or [e.get("reason")])[:4]}


# ---------------------------------------------------------------- M01-M05 (evidence)
def m01(c):
    empty = {k: {"status": "BUILT", "cases": {}} for k in ("fx_wait", "fx_sched", "fx_prims", "fx_jiffies", "fx_jiffies_control")}
    old_v05 = c.old("v05", {"fx_sched": {"cases": {}}})
    A1 = ("import json, run_all as R\nempty = %s\nd = R.evaluate(empty)\n"
          "print(json.dumps({k: (v.get('verdict') if 'verdict' in v else 'composite') for k, v in d.items()}))" % json.dumps(empty))
    d = c.tmp("old_dispatch")
    r = H.run_pg([PY, "-c", A1], 60, cwd=c.fz, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=d))
    old_dispatch = json.loads(r["stdout"].strip().splitlines()[-1])
    new_v05 = E.v05({"cases": {}})
    new_all = E.evaluate_all(empty, None)
    behaviour = [k for k, v in new_all.items() if v.get("result") is not None]
    return {"id": "M01", "input_label": META, "requirement": "no valid observation -> no OBSERVED_AS_PREDICTED / NO_FAILURE / COUNTEREVIDENCE",
            "old": {"v05({'cases': {}})": old_v05, "run_all.evaluate(empty)": old_dispatch},
            "new": {"v05({'cases': {}})": new_brief(new_v05), "evaluate_all(empty)": {k: new_brief(v) for k, v in new_all.items()}},
            "old_summary": "v05 -> %s; dispatch false results: %s" % (old_v05.get("verdict"), sorted(set(old_dispatch.values()))),
            "new_summary": "v05 -> %s; behaviour verdicts on empty input: %d" % (new_v05.get("status"), len(behaviour)),
            "old_side_status": "failure_demonstrated", "met": new_v05.get("result") is None and not behaviour}


def m02(c, dv):
    base = dv["fx_sched"]["cases"]["v05_state_not_in_mask"]["run"]["stdout"]
    evs = events_of(base)
    var = {
        "drop_label_group_task_new": [e for e in evs if not (e.get("label") == "task_new_via_wake_up_process" or
                                                              e.get("step") == "task_new_via_wake_up_process")],
        "drop_q_of_uninterruptible_mask": [e for e in evs if not (e.get("ev") == "q" and e.get("step") == "uninterruptible_mask_interruptible")],
        "truncate_last_two_events": evs[:-2],
    }
    out, met = {}, True
    for k, v in var.items():
        fx = {"fx_sched": {"status": "BUILT", "cases": {"v05_state_not_in_mask": run_rec(to_stdout(v), 0)}}}
        o, n = c.old("v05", fx), E.v05(fx)
        out[k] = {"events_kept": len(v), "old": o, "new": new_brief(n)}
        met = met and n.get("result") is None and n.get("status") == "INCOMPLETE_EVIDENCE"
    ctrl = E.v05({"fx_sched": dv["fx_sched"]})
    out["control_delivered_five_groups"] = {"new": new_brief(ctrl)}
    met = met and ctrl.get("status") == "VALID"
    return {"id": "M02", "input_label": META + " (derived from delivered V05 events)", "requirement": "missing label/association detected",
            "variants": out, "old_summary": {k: v["old"].get("verdict") for k, v in out.items() if "old" in v},
            "new_summary": {k: v["new"]["status"] for k, v in out.items()}, "old_side_status": "failure_demonstrated", "met": met}


def m03(c, dv):
    v04 = events_of(dv["fx_sched"]["cases"]["v04_noncurrent_wake"]["run"]["stdout"])
    v06 = events_of(dv["fx_sched"]["cases"]["v06_double_wake"]["run"]["stdout"])
    dup_ret = v04 + [dict([e for e in v04 if e["ev"] == "ret"][0], ret=1)]
    bad_type = [dict(e, task_state=str(e["task_state"])) if e.get("step") == "after" else e for e in v04]
    unparsable = v04[:1] + ["{garbage line " + META] + v04[1:]
    dup_q = v06[:2] + [dict(v06[1], task_occ=2)] + v06[2:]
    var = {"duplicate_conflicting_ret": ("v04", "v04_noncurrent_wake", dup_ret),
           "wrong_field_type": ("v04", "v04_noncurrent_wake", bad_type),
           "unparsable_line": ("v04", "v04_noncurrent_wake", unparsable),
           "duplicate_q_dict_override": ("v06", "v06_double_wake", dup_q)}
    out, met = {}, True
    for k, (fn, case, evs) in var.items():
        fx = {"fx_sched": {"status": "BUILT", "cases": {case: run_rec(to_stdout(evs), 0)}}}
        o = c.old(fn, fx)
        n = getattr(E, fn)(fx)
        out[k] = {"old": o, "new": new_brief(n)}
        met = met and n.get("result") is None and n.get("status") == "INVALID_EVIDENCE"
    return {"id": "M03", "input_label": META, "requirement": "duplicates/conflicts/type errors/unparsable lines give an invalid-evidence state",
            "variants": out, "old_summary": {k: v["old"].get("verdict") for k, v in out.items()},
            "new_summary": {k: v["new"]["status"] for k, v in out.items()}, "old_side_status": "failure_demonstrated", "met": met}


def m04(c, dv):
    base = dv["fx_sched"]["cases"]["v04_noncurrent_wake"]
    var = {"timed_out": dict(timed_out=True), "collect_timed_out": dict(collect_timed_out=True),
           "case_status_ERROR_with_output": dict(status="ERROR"), "repeat_not_identical": dict(repeat_identical=False),
           "terminal_state_killed": dict(terminal_state="timeout_group_killed")}
    out, met = {}, True
    for k, fl in var.items():
        rec = run_rec(base["run"]["stdout"], 0, **fl)
        fx = {"fx_sched": {"status": "BUILT", "cases": {"v04_noncurrent_wake": rec}}}
        o, n = c.old("v04", fx), E.v04(fx)
        out[k] = {"old": o, "new": new_brief(n)}
        met = met and n.get("result") is None and n.get("status") in ("INVALID_EVIDENCE", "ERROR")
    return {"id": "M04", "input_label": META, "requirement": "unreliable runs are not behaviour evidence; not reported as kernel counterevidence",
            "variants": out, "old_summary": {k: v["old"].get("verdict") for k, v in out.items()},
            "new_summary": {k: v["new"]["status"] for k, v in out.items()}, "old_side_status": "failure_demonstrated", "met": met}


def m05(c, dv):
    w = dv["fx_wait"]
    out, met = {}, True

    def with_case(case, stdout, rc):
        fx = {"fx_wait": copy.deepcopy(w)}
        fx["fx_wait"]["cases"][case] = run_rec(stdout, rc)
        return fx
    v03d = w["cases"]["v03_second_wake_direct"]["run"]["stdout"]
    var = {"v02_valid_events_exit_42": ("v02", with_case("v02_single", w["cases"]["v02_single"]["run"]["stdout"], 42)),
           "v03_stop_event_exit_0": ("v03", with_case("v03_second_wake_direct", v03d, 0)),
           "v09_msleep_step_cap_exit_0": ("v09", with_case("v09_msleep_bounded", w["cases"]["v09_msleep_bounded"]["run"]["stdout"], 0)),
           "v03_exit_42_without_stop_event": ("v03", with_case("v03_second_wake_direct", to_stdout(events_of(v03d)[:-1]), 42))}
    for k, (fn, fx) in var.items():
        o, n = c.old(fn, fx), getattr(E, fn)(fx)
        out[k] = {"old": o, "new": new_brief(n)}
        met = met and n.get("result") is None and n.get("status") in ("INVALID_EVIDENCE", "INCOMPLETE_EVIDENCE")
    ctrl = {"exit_0_v02": E.v02({"fx_wait": w}), "exit_42_v03": E.v03({"fx_wait": w}), "exit_43_v09": E.v09({"fx_wait": w})}
    out["controls_delivered"] = {k: new_brief(v) for k, v in ctrl.items()}
    met = met and all(v.get("status") == "VALID" and v.get("result") for v in ctrl.values())
    return {"id": "M05", "input_label": META + " (exit codes altered on delivered records)",
            "requirement": "exit code checked together with events/case contract; 0/42/43 controls accepted",
            "variants": out, "old_summary": {k: v["old"].get("verdict") for k, v in out.items() if "old" in v},
            "new_summary": {k: (v["new"]["status"] if "new" in v else {kk: vv["status"] for kk, vv in v.items()}) for k, v in out.items()},
            "old_side_status": "failure_demonstrated", "met": met}


# ---------------------------------------------------------------- M06-M09 (entry / generator)
class Counter:
    def __init__(self):
        self.calls = {"h00": 0, "stage": 0, "fixtures": 0}

    def h00(self, *a):
        self.calls["h00"] += 1
        return {"dynamic_safety_ok": True, "transport_ok": True, "probes": [], "note": META}

    def stage(self, name, *a):
        self.calls["stage"] += 1
        return {"script": name, "returncode": 0, "timed_out": False, "note": META}

    def fixtures(self, *a):
        self.calls["fixtures"] += 1
        return {}


def rr_env(c, tag):
    d = c.tmp(tag)
    os.environ["RECHECK_ROOT"] = d
    return d


def m06(c, oldc):
    out, met = {}, True
    variants = {"gate_false": lambda: {"gate": {"ok": False, "core_review_record": False}, "note": META},
                "identity_raises": lambda: (_ for _ in ()).throw(RuntimeError(META + " identity failure")),
                "real_identity_on_bad_repo": lambda: identity2.safe_check(repo=os.path.join(c.base, "no-such-repo"))}
    for k, fn in variants.items():
        rr_env(c, "m06_" + k)
        cnt = Counter()
        outp = os.path.join(os.environ["RECHECK_ROOT"], "run.json")
        code = run_recheck.main(["--out", outp], hooks={"identity": fn, "h00": cnt.h00, "stage": cnt.stage, "fixtures": cnt.fixtures})
        rec = json.load(open(outp))
        out[k] = {"exit_code": code, "status": rec.get("status"), "stand_in_calls": cnt.calls,
                  "record_dynamic_calls": rec.get("dynamic_calls"), "infrastructure_errors": rec.get("infrastructure_errors")}
        met = met and code == 3 and not any(cnt.calls.values()) and not any(rec["dynamic_calls"].values())
    old = {k: (v.get("json") or {}).get("dynamic_entry_calls") for k, v in (oldc.get("A2_R02_gate_not_enforced") or {}).items()}
    return {"id": "M06", "input_label": META, "requirement": "identity false/exception -> zero dynamic calls, accurate blocked/error record",
            "variants": out, "old": {"phase_A_dynamic_entry_calls": old}, "old_summary": old,
            "new_summary": {k: [v["exit_code"], v["status"], v["stand_in_calls"]] for k, v in out.items()},
            "old_side_status": "failure_demonstrated (Phase A A2)", "met": met}


def m07(c, oldc):
    out, met = {}, True
    fail = lambda *a: {"dynamic_safety_ok": False, "transport_ok": True, "note": META,
                       "probes": [{"probe": "group_kill", "as_expected": False, "terminal_state": "timeout_kill_error", "gates_dynamic": True}]}

    def boom(*a):
        raise TimeoutError(META + " H00 probe timed out")
    for k, h in (("h00_probe_failed", fail), ("h00_raised_timeout", boom)):
        rr_env(c, "m07_" + k)
        cnt = Counter()
        outp = os.path.join(os.environ["RECHECK_ROOT"], "run.json")
        code = run_recheck.main(["--out", outp], hooks={"h00": h, "fixtures": cnt.fixtures, "stages": ["v01_structure.py"]})
        rec = json.load(open(outp))
        v01j = None
        p = os.path.join(rec["workdir"], "stages", "v01.json")
        if os.path.exists(p):
            v01j = json.load(open(p))
        doc = MR.build_doc(rec, None, v01j, None, None, None, None, MR.frozen_old_results())
        cases = {x["case_id"]: [x["evidence_status"], x["result"]] for x in doc["cases"]}
        out[k] = {"exit_code": code, "status": rec.get("status"), "fixtures_stand_in_calls": cnt.calls["fixtures"],
                  "stage_v01": {kk: rec["stages"]["v01_structure.py"].get(kk) for kk in ("returncode", "timed_out", "source")},
                  "report_generated": True, "generator_errors": doc["generator_errors"], "cases": cases}
        met = met and cnt.calls["fixtures"] == 0 and rec["stages"]["v01_structure.py"]["returncode"] == 0 \
            and cases.get("V01", [None])[0] == "VALID" and cases.get("V02", [None])[0] == "BLOCKED" and code == 2 \
            and not doc["generator_errors"]
    old = (oldc.get("A2_R02_gate_not_enforced") or {}).get("h00_fails", {}).get("json")
    return {"id": "M07", "input_label": META, "requirement": "failed/timed-out H00 blocks dynamic fixtures; independent read-only check continues; partial report",
            "variants": out, "old": {"phase_A_h00_fails": old}, "old_summary": old and old.get("calls"),
            "new_summary": {k: [v["exit_code"], v["fixtures_stand_in_calls"], v["cases"].get("V01"), v["cases"].get("V02")] for k, v in out.items()},
            "old_side_status": "failure_demonstrated (Phase A A2 h00_fails)", "met": met}


def m08(c, canon, oldc):
    wd = canon["workdir"]
    v00 = json.load(open(os.path.join(wd, "stages", "v00.json")))
    v01 = json.load(open(os.path.join(wd, "stages", "v01.json")))
    static = json.load(open(os.path.join(wd, "stages", "static.json")))
    a46 = json.load(open(os.path.join(wd, "stages", "a46.json")))
    fr = MR.frozen_old_results()

    def doc_with(v00x, v01x):
        run = copy.deepcopy(canon)
        run["evaluation"]["V00"] = E.v00(v00x)
        run["evaluation"]["V01"] = E.v01(v01x)
        d = MR.build_doc(run, v00x, v01x, static, a46, None, None, fr)
        by = {x["case_id"]: x for x in d["cases"]}
        return {"V00": [by["V00"]["evidence_status"], by["V00"]["result"], by["V00"].get("failing_anchors")],
                "V01": [by["V01"]["evidence_status"], by["V01"]["result"],
                        [ch[0] for ch in by["V01"].get("checks", []) if not ch[3]]],
                "CA-06": d["ca_rulings"]["CA-06"]["ruling"], "CA-05": d["ca_rulings"]["CA-05"]["ruling"],
                "counts": d["counts"]["results_for_valid_evidence"], "generator_errors": d["generator_errors"]}
    v01_paths = copy.deepcopy(v01)
    for k in v01_paths["referenced_paths"]:
        v01_paths["referenced_paths"][k]["exists_taskbook"] = False
    v01_parse = copy.deepcopy(v01)
    v01_parse["parse"]["map"] = {"ok": False, "error": META}
    v00_fix46 = copy.deepcopy(v00)
    v00_a35 = copy.deepcopy(v00)
    for a in v00_fix46["anchors"]:
        if a["id"] == "A46":
            a["verdict_mech"] = "MATCH_IN_DEFINITION"
    for a in v00_a35["anchors"]:
        if a["id"] == "A35":
            a["verdict_mech"] = "QUOTE_NOT_FOUND"
    res = {"real_base": doc_with(v00, v01), "v01_referenced_paths_missing": doc_with(v00, v01_paths),
           "v01_parse_failure": doc_with(v00, v01_parse), "v00_A46_changed_to_match": doc_with(v00_fix46, v01),
           "v00_A35_changed_to_quote_not_found": doc_with(v00_a35, v01)}
    met = (res["v01_referenced_paths_missing"]["V01"][1] == "COUNTEREVIDENCE"
           and res["v01_parse_failure"]["V01"][1] == "COUNTEREVIDENCE"
           and res["v00_A46_changed_to_match"]["V00"][1] == "OBSERVED_AS_PREDICTED"
           and res["v00_A35_changed_to_quote_not_found"]["CA-06"] != res["real_base"]["CA-06"]
           and res["v00_A35_changed_to_quote_not_found"]["V00"][1] == "COUNTEREVIDENCE"
           and res["real_base"]["counts"] != res["v00_A46_changed_to_match"]["counts"]
           and all(not v["generator_errors"] for v in res.values()))
    old = {k: (v.get("case_results") or {}).get("V01") for k, v in (oldc.get("A5_R03_old_make_results") or {}).items()}
    return {"id": "M08", "input_label": META + " (derived from this run's real V00/V01 records)",
            "requirement": "no hard-coded NO_FAILURE / fixed conclusions in YAML, text or CA summary",
            "variants": res, "old": {"phase_A_old_make_results_V01": old}, "old_summary": old,
            "new_summary": {k: [v["V00"][1], v["V01"][1], v["CA-06"]] for k, v in res.items()},
            "old_side_status": "failure_demonstrated (Phase A A5: V01 stays NO_FAILURE_IN_SCOPE)", "met": met}


def _doc_of(rec):
    wd = rec["workdir"]
    ld = lambda n: json.load(open(os.path.join(wd, "stages", n))) if os.path.exists(os.path.join(wd, "stages", n)) else None
    doc = MR.build_doc(rec, ld("v00.json"), ld("v01.json"), ld("static.json"), ld("a46.json"), None, None, MR.frozen_old_results())
    text_ok = True
    try:
        MR.render(dict(MR.header(doc), **doc))
    except Exception as e:  # noqa: BLE001
        text_ok = "%s: %s" % (type(e).__name__, e)
    cases = {x["case_id"]: [x["evidence_status"], x["result"]] for x in doc["cases"]}
    cas = {k: v["ruling"] for k, v in doc["ca_rulings"].items()}
    return doc, text_ok, cases, cas


def m09(c, oldc, canon):
    rr_env(c, "m09_partial")
    outp = os.path.join(os.environ["RECHECK_ROOT"], "run.json")
    inj = {"broken_fixture": "fx_prims", "case_error": ["fx_sched", "v06_double_wake"], "skip_stage": "static_checks.py"}
    code = run_recheck.main(["--out", outp], inject=inj)
    rec = json.load(open(outp))
    doc, text_ok, cases, cas = _doc_of(rec)
    lnr = doc["counts"]["layers_not_executed"]
    met_a = (code == 2 and text_ok is True and not doc["generator_errors"]
             and cases["V12"][0] == "BLOCKED" and cases["V13"][0] == "BLOCKED" and cases["V06"][0] == "ERROR"
             and cases["V02"][0] == "VALID" and cas["CA-07"].startswith("UNDETERMINED") and cas["CA-01"].startswith("UNDETERMINED")
             and cas["CA-05"].startswith("UNDETERMINED")
             # batch 2: without static criteria V08 gives no case-level verdict and its limited-model layer is listed
             and cases["V08"] == ["INCOMPLETE_EVIDENCE", None]
             and "V08.limited_model_reachability" in lnr and "V08.host_original_slice_function_level" not in lnr
             and "V14.source_and_build_reference" in lnr
             and cas["CA-02"].startswith("FUNCTION_LEVEL_SUPPORTED") and "UNDETERMINED_STATIC_INPUT_MISSING" in cas["CA-02"])
    # variant B (batch 2): the static stage really runs and writes static.json, but its exit code is reported
    # as 1; fixture records are the M12 run's real records (no rebuild) and H00 is a stand-in
    rr_env(c, "m09_static_stage_failed")
    outb = os.path.join(os.environ["RECHECK_ROOT"], "run.json")

    def stage_fail_static(name, fz, sdir):
        r = run_recheck.run_stage(name, fz, sdir)
        if name == "static_checks.py":
            r = dict(r, returncode_real=r.get("returncode"), returncode=1,
                     note=META + " exit code reported as 1 after the real stage wrote static.json")
        return r
    h00_ok = lambda *a: {"dynamic_safety_ok": True, "transport_ok": True, "probes": [], "note": META + " stand-in; real H00 in the M12 run"}
    fx_reuse = lambda *a: copy.deepcopy(canon["fixtures"])
    code_b = run_recheck.main(["--out", outb], hooks={"h00": h00_ok, "stage": stage_fail_static, "fixtures": fx_reuse})
    recb = json.load(open(outb))
    docb, text_ok_b, cases_b, cas_b = _doc_of(recb)
    static_file_left = os.path.exists(os.path.join(recb["workdir"], "stages", "static.json"))
    canon_cases = {k: [v.get("status"), v.get("result")] for k, v in (canon.get("evaluation") or {}).items()}
    var_b = {"exit_code": code_b, "run_status": recb.get("status"), "yaml_renders": text_ok_b,
             "static_stage_real_returncode": (recb["stages"].get("static_checks.py") or {}).get("returncode_real"),
             "static_json_left_in_stage_dir": static_file_left, "stage_outputs_usable": recb.get("stage_outputs_usable"),
             "cases": cases_b, "ca_rulings": cas_b, "layers_not_executed": docb["counts"]["layers_not_executed"],
             "infrastructure_errors": recb.get("infrastructure_errors"), "generator_errors": docb["generator_errors"]}
    met_b = (code_b == 2 and text_ok_b is True and not docb["generator_errors"] and static_file_left
             and (recb.get("stage_outputs_usable") or {}).get("static") is False
             and cases_b["V08"] == ["INCOMPLETE_EVIDENCE", None] and cases_b["V14"][0] == "INCOMPLETE_EVIDENCE"
             and "UNDETERMINED_STATIC_INPUT_MISSING" in cas_b["CA-02"] and cas_b["CA-05"].startswith("UNDETERMINED")
             and all(cases_b[k] == canon_cases.get(k) for k in ("V02", "V03", "V04", "V05", "V06", "V07", "V09", "V10",
                                                                 "V11", "V12", "V13", "V00", "V01")))
    old = {k: {"returncode": v.get("returncode"), "generated": v.get("generated"),
               "error_tail": (v.get("stderr_tail") or "").strip().splitlines()[-1:] if v.get("stderr_tail") else []}
           for k, v in (oldc.get("A5_R03_old_make_results") or {}).items() if k in ("missing_static_stage", "fixture_blocked_compile")}
    return {"id": "M09", "input_label": META + " (real partial run with injected compile failure, case exception and skipped stage;"
                                             " variant B: static stage reported failed after writing its output)",
            "requirement": "no crash; full list of not-executed layers, affected CA, still-valid items; complete control = M12",
            "injection": inj, "exit_code": code, "run_status": rec.get("status"), "yaml_renders": text_ok,
            "cases": cases, "ca_rulings": cas, "layers_not_executed": lnr,
            "infrastructure_errors": rec.get("infrastructure_errors"), "generator_errors": doc["generator_errors"],
            "variant_static_stage_failed": var_b, "met_variants": {"partial_run": bool(met_a), "static_stage_failed": bool(met_b)},
            "old": old, "old_summary": old,
            "new_summary": {"exit": code, "cases": cases, "ca": cas,
                            "static_stage_failed": {"exit": code_b, "V08": cases_b["V08"], "V14": cases_b["V14"],
                                                    "CA-02": cas_b["CA-02"], "CA-05": cas_b["CA-05"]}},
            "old_side_status": "failure_demonstrated (Phase A A5: FileNotFoundError / KeyError)", "met": bool(met_a and met_b)}


# ---------------------------------------------------------------- M10 (identity with a scratch clone)
def git_in(repo, args, env=None, inp=None):
    return subprocess.run(["git", "-C", repo, *args], input=inp, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                          env=env, timeout=120, check=True).stdout.decode().strip()


def make_commit(clone, base_full, path, content, msg):
    idx = os.path.join(clone, ".git", "meta.index")
    env = dict(os.environ, GIT_AUTHOR_NAME=META, GIT_AUTHOR_EMAIL="meta@invalid", GIT_COMMITTER_NAME=META,
               GIT_COMMITTER_EMAIL="meta@invalid", GIT_INDEX_FILE=idx)
    blob = git_in(clone, ["hash-object", "-w", "--stdin"], env, content)
    git_in(clone, ["read-tree", base_full], env)
    git_in(clone, ["update-index", "--add", "--cacheinfo", "100644,%s,%s" % (blob, path)], env)
    tree = git_in(clone, ["write-tree"], env)
    return git_in(clone, ["commit-tree", tree, "-p", base_full, "-m", msg], env)


def m10(c, oldc):
    before = H.git("count-objects", "-v").stdout.decode()
    d = c.tmp("m10_clone")
    clone = os.path.join(d, "repo")
    # The session repository is shallow, where `git clone --shared` silently copies only branch-reachable
    # objects. Build the scratch repository by hand instead: an empty repo whose alternates file points
    # read-only at the session object store, plus a copy of the shallow boundary; every new object goes
    # into the scratch repo only.
    subprocess.run(["git", "init", "--quiet", clone], check=True, timeout=60)
    main_git = H.git("rev-parse", "--absolute-git-dir").stdout.decode().strip()
    with open(os.path.join(clone, ".git", "objects", "info", "alternates"), "w") as f:
        f.write(os.path.join(main_git, "objects") + "\n")
    if os.path.exists(os.path.join(main_git, "shallow")):
        with open(os.path.join(main_git, "shallow"), "rb") as s, open(os.path.join(clone, ".git", "shallow"), "wb") as t:
            t.write(s.read())
    subprocess.run(["git", "-C", clone, "remote", "add", "origin", H.REPO], check=True, timeout=60)
    base = H.full(H.PINS["core_frozen"], clone)
    succ = make_commit(clone, base, H.RECHECK_PREFIX + "META_SUCCESSOR.txt", META.encode(), META + " legit successor")
    pchg = make_commit(clone, base, H.RESULTS_ROOT + "pilot/result.yaml", b"changed: " + META.encode() + b"\n", META + " pilot changed")
    cchg = make_commit(clone, base, H.RESULTS_ROOT + "core/results.yaml", b"changed: " + META.encode() + b"\n", META + " core changed")
    master = H.full(H.PINS["master"], clone)
    cases = {"legit_successor_head": dict(head=succ, branch_name=H.WORK_BRANCH, remote_head=succ),
             "legit_equal_head": dict(head=base, branch_name=H.WORK_BRANCH, remote_head=base),
             "legit_local_ahead_of_remote": dict(head=succ, branch_name=H.WORK_BRANCH, remote_head=base),
             "wrong_branch_name": dict(head=base, branch_name="master", remote_head=base),
             "non_descendant_head": dict(head=master, branch_name=H.WORK_BRANCH, remote_head=master),
             "pilot_original_changed": dict(head=pchg, branch_name=H.WORK_BRANCH, remote_head=pchg),
             "core_original_changed": dict(head=cchg, branch_name=H.WORK_BRANCH, remote_head=cchg),
             "remote_reset_to_master": dict(head=base, branch_name=H.WORK_BRANCH, remote_head=master),
             "remote_ahead_of_local": dict(head=base, branch_name=H.WORK_BRANCH, remote_head=succ)}
    expect = {"legit_successor_head": True, "legit_equal_head": True, "legit_local_ahead_of_remote": True}
    out, met = {}, True
    for k, kw in cases.items():
        r = identity2.safe_check(repo=clone, **kw)
        g = r.get("gate") or {}
        out[k] = {"gate_ok": g.get("ok"), "closed_by": sorted(x for x, v in g.items() if x != "ok" and not v),
                  "error": r.get("error"), "remote_relation": (r.get("execution") or {}).get("remote_relation"),
                  "originals": {kk: (r.get("originals") or {}).get(kk) for kk in ("changed_or_missing", "pilot_changed_since_pilot_head")}}
        met = met and bool(g.get("ok")) == expect.get(k, False)
    real = identity2.safe_check()
    after = H.git("count-objects", "-v").stdout.decode()
    old = (oldc.get("A3_R04_old_identity_and_readback_probe") or {}).get("json") or {}
    return {"id": "M10", "input_label": META + " (local commits only in a --shared scratch clone; no remote writes)",
            "requirement": "legit successor replays; wrong branch / non-descendant / changed originals / bad remote are rejected",
            "variants": out, "session_repo_is_shallow": H.git("rev-parse", "--is-shallow-repository").stdout.decode().strip(),
            "scratch_repo_method": "git init + alternates (read-only) + copied shallow file; objects written only to scratch",
            "real_repo_gate_ok": (real.get("gate") or {}).get("ok"),
            "real_repo_execution": real.get("execution"), "main_repo_object_store_unchanged": before == after,
            "old": {"phase_A_old_identity_remote_heads_match": old.get("old_identity_remote_heads_match"),
                    "phase_A_old_identity_gate": old.get("old_identity_gate")},
            "old_summary": "old identity has no head/branch/originals inputs; it flags the legit advance as a mismatch and the run continues anyway",
            "new_summary": {k: [v["gate_ok"], v["closed_by"]] for k, v in out.items()},
            "old_side_status": "failure_demonstrated (Phase A A3/A4 misblock without enforcement)", "met": met and before == after}


# ---------------------------------------------------------------- M11 (readback CLI)
def m11(c, oldc):
    d = c.tmp("m11")
    pf = H.RESULTS_ROOT + "pilot/result.yaml"
    here = os.path.dirname(os.path.abspath(__file__))
    runs = {"with_field_object": [H.PINS["pilot_head"], pf, "transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4"],
            "without_field_object": [H.PINS["pilot_head"], pf],
            "with_field_branch_mode": [H.PINS["pilot_head"], pf, "transport_marker=MYOS2-CLOUD-PILOT-20260925-K7P4", "--mode", "branch"],
            "missing_path": [H.PINS["pilot_head"], H.RESULTS_ROOT + "pilot/does-not-exist.yaml"],
            "wrong_field_value": [H.PINS["pilot_head"], pf, "transport_marker=WRONG-" + META],
            "malformed_field": [H.PINS["pilot_head"], pf, "transport_marker"]}
    expect = {"with_field_object": 0, "without_field_object": 0, "with_field_branch_mode": 0, "missing_path": 1,
              "wrong_field_value": 1, "malformed_field": 2}
    out, met = {}, True
    for k, args in runs.items():
        o = os.path.join(d, k + ".json")
        r = H.run_pg([PY, os.path.join(here, "readback2.py"), *args, "--out", o], 120, cwd=here,
                     env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1"))
        j = json.load(open(o)) if os.path.exists(o) else None
        out[k] = {"argv_tail": args[2:] + ["--out", "<file>"], "exit": r["returncode"], "stderr_tail": r["stderr"][-300:],
                  "ok": j and j.get("ok"), "mode": j and j.get("mode"), "relation": j and j.get("branch_head_relation"),
                  "channels": j and [{kk: ch.get(kk) for kk in ("http_code", "curl_exit", "timed_out", "byte_identical", "field_ok", "ok")}
                                     for ch in j["channels"]]}
        met = met and r["returncode"] == expect[k]
    # timeout path, injected into the transport runner (in-process, HARNESS_META_TEST)
    real = H.run_pg

    def fake(cmd, timeout, **kw):
        if cmd and cmd[0] == "curl":
            return {"returncode": -9, "timed_out": True, "terminal_state": "timeout_group_killed", "stdout": "", "stderr": ""}
        return real(cmd, timeout, **kw)
    H.run_pg = fake
    try:
        t = readback2.remote_read(H.PINS["pilot_head"], pf, "object", workdir=c.tmp("m11_timeout"))
    finally:
        H.run_pg = real
    out["injected_curl_timeout"] = {"ok": t["ok"], "channels": [{kk: ch.get(kk) for kk in ("timed_out", "transport_ok", "ok")} for ch in t["channels"]]}
    met = met and t["ok"] is False
    old = oldc.get("A3_R04_old_readback_cli") or {}
    return {"id": "M11", "input_label": "real network reads + one " + META + " injected timeout",
            "requirement": "--out parsed with/without field=value; HTTP/timeout/object/bytes/field all in status",
            "variants": out, "old": {k: {"returncode": v.get("returncode"), "stderr_last": (v.get("stderr_tail") or "").strip().splitlines()[-1:]}
                                     for k, v in old.items()},
            "old_summary": {k: v.get("returncode") for k, v in old.items()},
            "new_summary": {k: v.get("exit", v.get("ok")) for k, v in out.items()},
            "old_side_status": "failure_demonstrated (Phase A A3 CLI ValueError)", "met": met}


# ---------------------------------------------------------------- M12 (same-source replay facts)
OLD_EXPAND = r'''
import json, os, sys, hashlib, build as B
out = {}
for n in ("fx_wait", "fx_sched", "fx_prims", "fx_jiffies"):
    p = os.path.join(sys.argv[1], n + ".expanded.c")
    B.expand(n + ".c", p)
    out[n] = hashlib.sha256(open(p, "rb").read()).hexdigest()[:16]
print(json.dumps(out))
'''


def m12(c, canon):
    d = c.tmp("m12_old_expand")
    r = H.run_pg([PY, "-c", OLD_EXPAND, d], 120, cwd=c.fz, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", CORE_WORK=d))
    old = json.loads(r["stdout"].strip().splitlines()[-1]) if r["returncode"] == 0 else {"error": r["stderr"][-400:]}
    new = {n: ((canon.get("fixtures") or {}).get(n) or {}).get("expanded_source", {}).get("sha256_segments", [None])[0]
           for n in ("fx_wait", "fx_sched", "fx_prims", "fx_jiffies")}
    ev = canon.get("evaluation") or {}
    summary = {k: [v.get("status"), v.get("result")] for k, v in ev.items()}
    same = all(old.get(k) == new.get(k) and new.get(k) for k in new)
    a46 = ev.get("V00_A46_correction") or {}
    v00 = ev.get("V00") or {}
    met = (canon.get("status") == "COMPLETE" and same and all(v[0] == "VALID" for v in summary.values())
           and any(x.get("id") == "A46" for x in v00.get("failing_anchors", [])) and a46.get("result") == "OBSERVED_AS_PREDICTED")
    return {"id": "M12", "input_label": "REAL same-source run (not a meta test)",
            "requirement": "frozen sources/predictions, full valid data -> scoped conclusions; old A46 still fails; A46-C/ASM checked separately",
            "canonical_status": canon.get("status"), "canonical_exit": canon.get("exit_code"), "evaluation": summary,
            "expanded_source_sha256_first_segment": {"frozen_old_builder": old, "revised_builder": new, "identical": same},
            "old_A46_failing": v00.get("failing_anchors"), "a46_correction": [a46.get("status"), a46.get("result")],
            "old_summary": "n/a (this row is the real replay)", "new_summary": summary,
            "old_side_status": "not_applicable", "met": bool(met)}


def main():
    canon = json.load(open(sys.argv[1]))
    oldc = json.load(open(sys.argv[2]))
    outp = sys.argv[3]
    root0 = os.environ["RECHECK_ROOT"]
    c = Ctx()
    res = {"check": "META_TESTS", "workdir": c.base, "frozen_manifest_names_match": c.manifest["names_match_expected"], "tests": []}
    dv = delivered()
    res["delivered_records"] = {k: sorted(v["cases"]) for k, v in dv.items()}
    steps = [("M01", lambda: m01(c)), ("M02", lambda: m02(c, dv)), ("M03", lambda: m03(c, dv)), ("M04", lambda: m04(c, dv)),
             ("M05", lambda: m05(c, dv)), ("M06", lambda: m06(c, oldc)), ("M07", lambda: m07(c, oldc)),
             ("M08", lambda: m08(c, canon, oldc)), ("M09", lambda: m09(c, oldc, canon)), ("M10", lambda: m10(c, oldc)),
             ("M11", lambda: m11(c, oldc)), ("M12", lambda: m12(c, canon))]
    for mid, fn in steps:
        try:
            res["tests"].append(fn())
        except Exception as e:  # noqa: BLE001
            import traceback
            res["tests"].append({"id": mid, "met": False, "harness_error": "%s: %s" % (type(e).__name__, e),
                                 "trace_tail": traceback.format_exc()[-1500:]})
        finally:
            os.environ["RECHECK_ROOT"] = root0
    res["met"] = {t["id"]: t.get("met") for t in res["tests"]}
    res["all_met"] = all(res["met"].values())
    H.emit(res, outp)
    print(H.dump(res["met"]))
    return 0 if res["all_met"] else 1


if __name__ == "__main__":
    sys.exit(main())
