# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file. N05 re-implements the M01-M05/M08/M09 input transformations of
#   recheck-01/fixtures/meta_tests.py @ b843d475367a (m01..m05, m08, m09) on the real b843 run.json
#   records instead of importing meta_tests (its import requires RECHECK_ROOT and its m06-m12 entries
#   run C/H00 work). No old batch entry (run_all/run_recheck/meta_tests main) is called.
# purpose: N01-N06 of the 13 contract. Old side = b843 evaluate2 / make_results2 CLI from the frozen
#   extraction; new side = consumer3 / make_results3. Every experiment is its own Python process with a
#   30 s limit; altered inputs live only in a fresh directory and are labelled HARNESS_META_TEST.
#   N06 re-judges the unaltered b843 observations (RECORDED_OBSERVATION_REVALIDATION).
# --------------------------------------------------------------------------------------------------
"""Usage: python3 consumer_tests.py <frozen extraction dir> <work parent> <out.json> <n06_report.yaml>"""
import copy
import json
import os
import re
import shutil
import sys

import yaml

import common3 as C
import old_side as OS

HERE = os.path.dirname(os.path.abspath(__file__))
META = "HARNESS_META_TEST"
GEN = OS.GEN_INPUTS
NEW_CALL = r'''
import json, re, sys
sys.path.insert(0, sys.argv[2])
import consumer3 as K
E, MR = K.bind(sys.argv[3])
spec = json.load(open(sys.argv[1]))
arg = json.load(open(spec["input"]))
if spec["fn"] == "v01":
    r = K.v01_strict(arg, spec.get("stage_rec"), K.expected())
elif spec["fn"] == "evaluate_all":
    allv = E.evaluate_all(arg, None)
    r = {"status": None, "result": None, "all": {k: [v.get("status"), v.get("result")] for k, v in allv.items()}}
else:
    r = getattr(E, spec["fn"])(arg)
reasons = r.get("evidence_reasons") or ([r.get("reason")] if r.get("reason") else [])
cats = sorted(set(re.findall(r"\[([A-Z_]+)\]", " ".join(str(x) for x in reasons))) |
              {p["category"] for p in r.get("collection_problems", [])})
print(json.dumps({"status": r.get("status"), "result": r.get("result"), "categories": cats, "reasons": reasons[:6],
                  "all": r.get("all"), "constituents": r.get("constituents"),
                  "checks": [[c["label"], c["observed"], c["predicted"], c["match"]] for c in r.get("checks", [])]}))
'''


class Ctx:
    def __init__(self, frozen, parent):
        self.frozen = os.path.realpath(frozen)
        self.base = C.fresh_dir(parent, "consumer-tests-")
        self.n = 0
        obs = os.path.join(self.frozen, "observations")
        self.obs = obs
        self.run = json.load(open(os.path.join(obs, "run.json"), encoding="utf-8"))
        self.v01 = json.load(open(os.path.join(obs, "v01.json"), encoding="utf-8"))
        self.v00 = json.load(open(os.path.join(obs, "v00.json"), encoding="utf-8"))
        self.v01_stage = self.run["stages"]["v01_structure.py"]

    def sub(self, name):
        self.n += 1
        d = os.path.join(self.base, "%03d_%s" % (self.n, name))
        os.makedirs(d)
        return d

    def old(self, name, fn, obj, stage_rec=None):
        if fn == "evaluate_all":  # per-item statuses of the b843 dispatcher (old_side.CALL returns one status)
            d = self.sub("old_" + name)
            inp = os.path.join(d, "input.json")
            with open(inp, "w", encoding="utf-8") as f:
                json.dump(obj, f, ensure_ascii=False)
            code = ("import json, sys, evaluate2 as E; a = json.load(open(sys.argv[1])); "
                    "print(json.dumps({'all': {k: [v.get('status'), v.get('result')] for k, v in E.evaluate_all(a, None).items()}}))")
            r = C.run_py(["-c", code, inp], cwd=os.path.join(self.frozen, "fixtures"))
            return {"input": OS.file_id(inp), "returncode": r["returncode"], "timed_out": r["timed_out"],
                    "returned": C.last_json(r["stdout"]), "stderr_tail": C.tail(r["stderr"])}
        return OS.call_old(self.frozen, self.sub("old_" + name), fn, obj, stage_rec)

    def new(self, name, fn, obj, stage_rec=None):
        d = self.sub("new_" + name)
        inp = os.path.join(d, "input.json")
        with open(inp, "w", encoding="utf-8") as f:
            json.dump(obj, f, ensure_ascii=False)
        spec = os.path.join(d, "spec.json")
        with open(spec, "w", encoding="utf-8") as f:
            json.dump({"fn": fn, "input": inp, "stage_rec": stage_rec}, f)
        r = C.run_py(["-c", NEW_CALL, spec, HERE, os.path.join(self.frozen, "fixtures")], cwd=HERE)
        return {"input": OS.file_id(inp), "returncode": r["returncode"], "timed_out": r["timed_out"],
                "returned": C.last_json(r["stdout"]), "stderr_tail": C.tail(r["stderr"])}

    def obs_copy(self, name, omit=(), replace=None, objects=None):
        """Fresh observations copy; omit files, replace file text, or replace parsed objects."""
        d = self.sub(name)
        od = os.path.join(d, "obs")
        os.makedirs(od)
        for f in GEN:
            if f in omit:
                continue
            if objects and f in objects:
                with open(os.path.join(od, f), "w", encoding="utf-8") as fh:
                    json.dump(objects[f], fh, ensure_ascii=False)
            else:
                shutil.copy(os.path.join(self.obs, f), od)
        for f, text in (replace or {}).items():
            with open(os.path.join(od, f), "w", encoding="utf-8") as fh:
                fh.write(text)
        return d, od

    def cli_old(self, d, od):
        r = OS.cli_old(self.frozen, d, od)
        r.pop("_out")
        return r

    def cli_new(self, d, od, frozen_fix=None):
        out = os.path.join(d, "consumer.yaml")
        r = C.run_py(["make_results3.py", "--frozen", frozen_fix or os.path.join(self.frozen, "fixtures"), od, out], cwd=HERE)
        rec = {"argv": ["make_results3.py", "--frozen", "<b843 fixtures>", "<obs>", "<out.yaml>"], "returncode": r["returncode"],
               "timed_out": r["timed_out"], "stderr_tail": C.tail(r["stderr"]), "report_written": os.path.exists(out),
               "error_record_written": os.path.exists(out + ".error.json")}
        rec["report"] = summarize_new(out) if rec["report_written"] else None
        if rec["report_written"]:
            rec["report_file"] = OS.file_id(out)
        return rec


def summarize_new(path):
    d = yaml.safe_load(open(path, encoding="utf-8"))
    cv = d["consumer_validation"]
    by = {c["case_id"]: c for c in d.get("cases", [])}
    pick = lambda k: {"evidence_status": by[k].get("evidence_status"), "result": by[k].get("result"), "layers": by[k].get("layers")}
    return {"status": d.get("status"), "source_run_status": ((d.get("source_execution") or {}).get("run") or {}).get("status"),
            "input_problems": cv["input_problems"], "dynamic_raw_observations_available": cv["dynamic_raw_observations_available"],
            "items": cv["items"], "cases": {k: [v.get("evidence_status"), v.get("result")] for k, v in by.items()},
            "V08": pick("V08"), "V14": pick("V14"), "V01": pick("V01"),
            "ca_rulings": {k: v.get("ruling") for k, v in (d.get("ca_rulings") or {}).items()},
            "cached_differs": cv["cached_evaluation_vs_consumer"]["differs"], "affected_by_input": cv["affected_by_input"],
            "generator_errors": d.get("generator_errors"), "counts": d.get("counts")}


def st(x):
    r = (x or {}).get("returned") or {}
    return [r.get("status"), r.get("result")]


# ---------------------------------------------------------------- N01: run metadata (S01 family)
def n01(c):
    case = "v04_noncurrent_wake"
    base = copy.deepcopy(c.run["fixtures"])

    def variant(mut):
        fx = copy.deepcopy(base)
        mut(fx["fx_sched"]["cases"][case])
        return fx
    var = {}
    for k in OS.S01_KEYS:
        var["delete_run." + k] = ("missing", lambda cs, k=k: cs["run"].pop(k))
    var["delete_all_four"] = ("missing", lambda cs: [cs["run"].pop(k) for k in OS.S01_KEYS])
    var["delete_case.repeat_terminal_state"] = ("missing", lambda cs: cs.pop("repeat_terminal_state"))
    var["delete_run.output_lost"] = ("missing", lambda cs: cs["run"].pop("output_lost"))
    var["explicit_collect_timed_out_true"] = ("explicit", lambda cs: cs["run"].update(collect_timed_out=True))
    var["explicit_terminal_state_killed"] = ("explicit", lambda cs: cs["run"].update(terminal_state="timeout_group_killed"))
    var["explicit_reaped_confirmed_false"] = ("explicit", lambda cs: cs["run"].update(reaped_confirmed=False))
    var["explicit_stderr_nonempty"] = ("explicit", lambda cs: cs["run"].update(stderr=META + " stderr"))
    var["explicit_output_lost_true"] = ("explicit", lambda cs: cs["run"].update(output_lost=True))
    var["explicit_repeat_terminal_state_killed"] = ("explicit", lambda cs: cs.update(repeat_terminal_state="timeout_group_killed"))
    var["wrong_type_reaped_confirmed_string"] = ("wrong_type", lambda cs: cs["run"].update(reaped_confirmed="true"))
    out, met = {}, True
    for name, (kind, mut) in var.items():
        fx = variant(mut)
        o, n = c.old("n01_" + name, "v04", fx), c.new("n01_" + name, "v04", fx)
        out[name] = {"kind": kind, "old": st(o), "new": st(n), "new_categories": (n["returned"] or {}).get("categories"),
                     "new_reasons": (n["returned"] or {}).get("reasons")}
        want = {"missing": "INCOMPLETE_EVIDENCE", "explicit": "INVALID_EVIDENCE", "wrong_type": "INVALID_EVIDENCE"}[kind]
        cat = {"missing": "MISSING_FIELD", "explicit": None, "wrong_type": "WRONG_TYPE"}[kind]
        ok = st(n) == [want, None] and (cat is None or cat in (out[name]["new_categories"] or []))
        out[name]["met"] = ok
        met = met and ok
    o, n = c.old("n01_control", "v04", base), c.new("n01_control", "v04", base)
    out["control_complete_real_record"] = {"kind": "control", "old": st(o), "new": st(n), "met": st(n) == ["VALID", "OBSERVED_AS_PREDICTED"]}
    met = met and out["control_complete_real_record"]["met"]
    return {"id": "N01", "input_label": META + " (fields removed/changed on the real b843 V04 record)",
            "requirement": "missing run metadata is never VALID; error class recorded; explicit failures still rejected; complete control valid",
            "variants": out, "met": bool(met)}


# ---------------------------------------------------------------- N02: V01 object sets (S02 family)
def n02(c):
    v = c.v01
    var = {}
    var["drop_one_referenced_path"] = ("missing", lambda x: x["referenced_paths"].pop("map.validation_contract"))
    var["empty_referenced_paths"] = ("missing", lambda x: x.update(referenced_paths={}))
    var["drop_one_report_input"] = ("missing", lambda x: x["report_inputs_read"].pop(3))
    var["empty_report_inputs_read"] = ("missing", lambda x: x.update(report_inputs_read=[]))
    var["drop_one_hex40_scope_file"] = ("missing", lambda x: x["hex40_hits_in_scope_files"].pop("MANIFEST.md"))
    var["empty_hex40_hits_in_scope_files"] = ("missing", lambda x: x.update(hex40_hits_in_scope_files={}))
    var["empty_all_three"] = ("missing", lambda x: x.update(referenced_paths={}, report_inputs_read=[], hex40_hits_in_scope_files={}))
    var["duplicate_report_input"] = ("invalid", lambda x: x["report_inputs_read"].append(dict(x["report_inputs_read"][0])))
    var["renamed_referenced_path_member"] = ("invalid", lambda x: x["referenced_paths"]["map.report"].update(path=META + "/other.md"))
    out, met = {}, True
    for name, (kind, mut) in var.items():
        x = copy.deepcopy(v)
        mut(x)
        o, n = c.old("n02_" + name, "v01", x, c.v01_stage), c.new("n02_" + name, "v01", x, c.v01_stage)
        want = "INCOMPLETE_EVIDENCE" if kind == "missing" else "INVALID_EVIDENCE"
        out[name] = {"kind": kind, "old": st(o), "new": st(n), "new_categories": (n["returned"] or {}).get("categories"),
                     "met": st(n) == [want, None]}
        met = met and out[name]["met"]
    ctl = {"control_complete_empty_error_lists": (lambda x: None, ["VALID", "NO_FAILURE_IN_SCOPE"]),
           "control_member_exists_false": (lambda x: x["referenced_paths"]["map.report"].update(exists_taskbook=False),
                                           ["VALID", "COUNTEREVIDENCE"]),
           "control_complete_hex40_hit_found": (lambda x: x["hex40_hits_in_scope_files"].update({"MANIFEST.md": 1}),
                                                ["VALID", "COUNTEREVIDENCE"])}
    for name, (mut, want) in ctl.items():
        x = copy.deepcopy(v)
        mut(x)
        o, n = c.old("n02_" + name, "v01", x, c.v01_stage), c.new("n02_" + name, "v01", x, c.v01_stage)
        out[name] = {"kind": "control", "old": st(o), "old_reasons": (o["returned"] or {}).get("reasons"), "new": st(n), "met": st(n) == want}
        met = met and out[name]["met"]
    out["control_complete_empty_error_lists"]["empty_error_lists_in_record"] = {
        "anchor_refs.dangling": v["anchor_refs"]["dangling"], "case_refs.dangling": v["case_refs"]["dangling"],
        "contract09_anchor_refs_dangling": v["contract09_anchor_refs_dangling"], "p9_wording_hits_in_scope_files": v["p9_wording_hits_in_scope_files"]}
    return {"id": "N02", "input_label": META + " (members removed/duplicated/changed on the real b843 v01.json)",
            "requirement": "missing checked objects are missing evidence, not 'no failure'; legal empty error lists stay valid; a real False is a finding",
            "variants": out, "met": bool(met)}


# ---------------------------------------------------------------- N03: complete entry, missing files
def n03(c):
    out = {}
    d, od = c.obs_copy("n03_control_full")
    ctl = {"old": c.cli_old(d, od), "new": c.cli_new(d, od)}
    out["control_full"] = ctl
    base_cases = ctl["new"]["report"]["cases"]
    for name, omit in (("omit_static", ("static.json",)), ("omit_v01", ("v01.json",))):
        d, od = c.obs_copy("n03_" + name, omit=omit)
        out[name] = {"inputs_present": sorted(os.listdir(od)), "old": c.cli_old(d, od), "new": c.cli_new(d, od)}
    a = out["omit_static"]["new"]["report"]
    b = out["omit_v01"]["new"]["report"]
    indep_static = [k for k in base_cases if k not in ("V08", "V14")]
    indep_v01 = [k for k in base_cases if k != "V01"]
    checks = {
        "control_complete_exit0": ctl["new"]["returncode"] == 0 and ctl["new"]["report"]["status"] == "consumer_COMPLETE",
        "omit_static_partial_exit2": out["omit_static"]["new"]["returncode"] == 2 and a["status"] == "consumer_PARTIAL",
        "omit_static_V08_not_valid_function_layer_kept": a["V08"]["evidence_status"] == "INCOMPLETE_EVIDENCE" and a["V08"]["result"] is None
        and a["V08"]["layers"]["host_original_slice_function_level"] == "EXECUTED" and a["V08"]["layers"]["limited_model_reachability"] != "EXECUTED",
        "omit_static_CA02_function_level_only": a["ca_rulings"]["CA-02"].startswith("FUNCTION_LEVEL_SUPPORTED")
        and "UNDETERMINED_STATIC_INPUT_MISSING" in a["ca_rulings"]["CA-02"],
        "omit_static_V14_source_layer_not_executed": a["V14"]["layers"]["source_and_build_reference"] != "EXECUTED",
        "omit_static_cache_not_used": "V08" in a["cached_differs"],
        "omit_static_independent_items_unchanged": all(a["cases"][k] == base_cases[k] for k in indep_static),
        "omit_v01_partial_exit2": out["omit_v01"]["new"]["returncode"] == 2 and b["status"] == "consumer_PARTIAL",
        "omit_v01_V01_not_valid": b["cases"]["V01"] == ["INCOMPLETE_EVIDENCE", None] and "V01" in b["cached_differs"],
        "omit_v01_independent_items_unchanged": all(b["cases"][k] == base_cases[k] for k in indep_v01)
        and b["ca_rulings"] == ctl["new"]["report"]["ca_rulings"],
        "source_run_status_kept_as_history": a["source_run_status"] == "COMPLETE" and b["source_run_status"] == "COMPLETE"}
    return {"id": "N03", "input_label": META + " (b843 observations copied with one file omitted)",
            "requirement": "no cached verdict hides a missing original; V08 keeps only the independent function layer; independent items unaffected",
            "variants": out, "checks": checks, "met": all(checks.values())}


# ---------------------------------------------------------------- N04: complete entry, corrupt files
def n04(c):
    out, checks = {}, {}
    bad = {"syntax_open_brace": "{", "empty_file": "", "wrong_top_type_list": "[]"}
    for f in ("static.json", "v01.json"):
        for kind, text in bad.items():
            name = "%s_%s" % (f.split(".")[0], kind)
            d, od = c.obs_copy("n04_" + name, replace={f: text})
            o, n = c.cli_old(d, od), c.cli_new(d, od)
            out[name] = {"file": f, "content": text, "old": o, "new": n}
            r = n["report"] or {}
            aff = ["V08", "V14"] if f == "static.json" else ["V01"]
            others = [k for k in (r.get("cases") or {}) if k not in aff]
            checks[name] = (n["returncode"] == 2 and n["report_written"] and r.get("status") == "consumer_PARTIAL"
                            and f in r.get("input_problems", {}) and all((r["cases"][k][1] is None) for k in aff)
                            and all(r["cases"][k][0] == "VALID" for k in others))
    d, od = c.obs_copy("n04_run_unparsable", replace={"run.json": "{"})
    o, n = c.cli_old(d, od), c.cli_new(d, od)
    out["run_unparsable_control"] = {"file": "run.json", "content": "{", "old": o, "new": n}
    r = n["report"] or {}
    checks["run_unparsable_control"] = (n["returncode"] == 2 and r.get("dynamic_raw_observations_available") is False
                                        and all(v[0] != "VALID" and v[1] is None for v in r.get("items", {}).values()))
    # consumer failure is not a partial report: a frozen module that differs from b843 stops the reader
    d = c.sub("n04_consumer_failure_control")
    fz = os.path.join(d, "frozen")
    shutil.copytree(c.frozen, fz)
    with open(os.path.join(fz, "fixtures", "evaluate2.py"), "a") as f:
        f.write("\n# " + META + " altered copy\n")
    n = c.cli_new(d, os.path.join(c.obs), frozen_fix=os.path.join(fz, "fixtures"))
    out["consumer_failure_control"] = {"alteration": "comment appended to a copy of frozen evaluate2.py", "new": n}
    checks["consumer_failure_control"] = n["returncode"] == 4 and not n["report_written"] and n["error_record_written"]
    return {"id": "N04", "input_label": META + " (b843 observations copied with one file corrupted)",
            "requirement": "error/partial report written with matching exit code; no behaviour verdict for affected items; no dynamic claim without run data",
            "variants": out, "checks": checks, "met": all(checks.values())}


# ---------------------------------------------------------------- N05: regression on M01-M05/M08/M09 data
def ev_list(stdout):
    return [json.loads(x) for x in stdout.splitlines() if x.strip()]


def to_stdout(evs):
    return "".join((e if isinstance(e, str) else json.dumps(e, separators=(",", ":"))) + "\n" for e in evs)


def n05(c):
    fx0 = c.run["fixtures"]
    out, met = {}, True

    def with_case(fixture, case, stdout=None, **upd):
        fx = copy.deepcopy(fx0)
        cs = fx[fixture]["cases"][case]
        if stdout is not None:
            cs["run"]["stdout"] = stdout
        for k, v in upd.items():
            (cs["run"] if k in cs["run"] else cs)[k] = v
        return fx
    v05 = ev_list(fx0["fx_sched"]["cases"]["v05_state_not_in_mask"]["run"]["stdout"])
    v04 = ev_list(fx0["fx_sched"]["cases"]["v04_noncurrent_wake"]["run"]["stdout"])
    v06 = ev_list(fx0["fx_sched"]["cases"]["v06_double_wake"]["run"]["stdout"])
    v03d = fx0["fx_wait"]["cases"]["v03_second_wake_direct"]["run"]["stdout"]
    reject = {  # (M source, evaluator, input, accepted statuses)
        "M01_empty_cases": ("M01", "evaluate_all", {k: {"status": "BUILT", "cases": {}} for k in fx0}, None),
        "M02_drop_label_task_new": ("M02", "v05", with_case("fx_sched", "v05_state_not_in_mask", to_stdout(
            [e for e in v05 if not (e.get("label") == "task_new_via_wake_up_process" or e.get("step") == "task_new_via_wake_up_process")])),
            ("INCOMPLETE_EVIDENCE",)),
        "M02_drop_q_uninterruptible": ("M02", "v05", with_case("fx_sched", "v05_state_not_in_mask", to_stdout(
            [e for e in v05 if not (e.get("ev") == "q" and e.get("step") == "uninterruptible_mask_interruptible")])), ("INCOMPLETE_EVIDENCE",)),
        "M02_truncate_last_two": ("M02", "v05", with_case("fx_sched", "v05_state_not_in_mask", to_stdout(v05[:-2])), ("INCOMPLETE_EVIDENCE",)),
        "M03_duplicate_conflicting_ret": ("M03", "v04", with_case("fx_sched", "v04_noncurrent_wake", to_stdout(
            v04 + [dict([e for e in v04 if e["ev"] == "ret"][0], ret=1)])), ("INVALID_EVIDENCE",)),
        "M03_wrong_field_type": ("M03", "v04", with_case("fx_sched", "v04_noncurrent_wake", to_stdout(
            [dict(e, task_state=str(e["task_state"])) if e.get("step") == "after" else e for e in v04])), ("INVALID_EVIDENCE",)),
        "M03_unparsable_line": ("M03", "v04", with_case("fx_sched", "v04_noncurrent_wake", to_stdout(
            v04[:1] + ["{garbage line " + META] + v04[1:])), ("INVALID_EVIDENCE",)),
        "M03_duplicate_q": ("M03", "v06", with_case("fx_sched", "v06_double_wake", to_stdout(v06[:2] + [dict(v06[1], task_occ=2)] + v06[2:])),
                            ("INVALID_EVIDENCE",)),
        "M04_timed_out": ("M04", "v04", with_case("fx_sched", "v04_noncurrent_wake", timed_out=True), ("INVALID_EVIDENCE",)),
        "M04_collect_timed_out": ("M04", "v04", with_case("fx_sched", "v04_noncurrent_wake", collect_timed_out=True), ("INVALID_EVIDENCE",)),
        "M04_case_status_ERROR": ("M04", "v04", with_case("fx_sched", "v04_noncurrent_wake", status="ERROR"), ("ERROR",)),
        "M04_repeat_not_identical": ("M04", "v04", with_case("fx_sched", "v04_noncurrent_wake", repeat_identical=False), ("INVALID_EVIDENCE",)),
        "M04_terminal_state_killed": ("M04", "v04", with_case("fx_sched", "v04_noncurrent_wake", terminal_state="timeout_group_killed"),
                                      ("INVALID_EVIDENCE",)),
        "M05_v02_exit_42": ("M05", "v02", with_case("fx_wait", "v02_single", returncode=42, repeat_returncode=42), ("INVALID_EVIDENCE",)),
        "M05_v03_stop_event_exit_0": ("M05", "v03", with_case("fx_wait", "v03_second_wake_direct", returncode=0, repeat_returncode=0),
                                      ("INVALID_EVIDENCE",)),
        "M05_v09_step_cap_exit_0": ("M05", "v09", with_case("fx_wait", "v09_msleep_bounded", returncode=0, repeat_returncode=0),
                                    ("INVALID_EVIDENCE",)),
        "M05_v03_exit_42_without_stop_event": ("M05", "v03", with_case("fx_wait", "v03_second_wake_direct", to_stdout(ev_list(v03d)[:-1])),
                                               ("INVALID_EVIDENCE", "INCOMPLETE_EVIDENCE")),
    }
    for name, (src, fn, fx, want) in reject.items():
        o, n = c.old("n05_" + name, fn, fx), c.new("n05_" + name, fn, fx)
        if fn == "evaluate_all":
            vals = ((n["returned"] or {}).get("all") or {}).values()
            ok = bool(vals) and all(v[1] is None and v[0] != "VALID" for v in vals)
            rec = {"m_source": src, "old_all": (o["returned"] or {}).get("all"), "new_all": (n["returned"] or {}).get("all"), "met": ok}
        else:
            ok = st(n)[1] is None and st(n)[0] in want
            rec = {"m_source": src, "old": st(o), "new": st(n), "met": ok}
        out[name] = rec
        met = met and ok
    accept = {
        "control_exit_0_v02": ("v02", fx0, ["VALID", "OBSERVED_AS_PREDICTED"]),
        "control_exit_42_v03": ("v03", fx0, ["VALID", "OBSERVED_AS_PREDICTED"]),
        "control_exit_43_v09": ("v09", fx0, ["VALID", "OBSERVED_AS_PREDICTED"]),
        "valid_but_different_from_prediction_v04_ret_1": ("v04", with_case("fx_sched", "v04_noncurrent_wake", to_stdout(
            [dict(e, ret=1) if e.get("ev") == "ret" else e for e in v04])), ["VALID", "COUNTEREVIDENCE"]),
    }
    for name, (fn, fx, want) in accept.items():
        o, n = c.old("n05_" + name, fn, fx), c.new("n05_" + name, fn, fx)
        out[name] = {"old": st(o), "old_reasons": (o["returned"] or {}).get("reasons"), "new": st(n), "met": st(n) == want}
        met = met and out[name]["met"]
    # through the real read/generate entry (make_results3 CLI): M08/M09-derived data and N02/N03 transforms
    cli = {}
    run = copy.deepcopy(c.run)
    v01x = copy.deepcopy(c.v01)
    v01x["referenced_paths"]["map.report"]["exists_taskbook"] = False
    v00a = copy.deepcopy(c.v00)
    for a in v00a["anchors"]:
        if a["id"] == "A46":
            a["verdict_mech"] = "MATCH_IN_DEFINITION"
    v00b = copy.deepcopy(c.v00)
    for a in v00b["anchors"]:
        if a["id"] == "A35":
            a["verdict_mech"] = "QUOTE_NOT_FOUND"
    m09 = copy.deepcopy(c.run)
    m09["fixtures"]["fx_prims"] = {"fixture": "fx_prims", "status": "BLOCKED_COMPILE",
                                   "cases": {k: {"status": "BLOCKED", "reason": META + " compile failed; nothing executed"}
                                             for k in ("v12_add_test_negative", "v13_trylock")}}
    m09["fixtures"]["fx_sched"]["cases"]["v06_double_wake"] = {"status": "ERROR", "reason": META + " injected case exception"}
    m09["stages"]["static_checks.py"] = {"script": "static_checks.py", "status": "NOT_RUN", "reason": META + " skipped stage"}
    m09b = copy.deepcopy(c.run)
    m09b["stages"]["static_checks.py"] = dict(m09b["stages"]["static_checks.py"], returncode=1)
    v01e = copy.deepcopy(c.v01)
    v01e["referenced_paths"] = {}
    v01d = copy.deepcopy(c.v01)
    v01d["report_inputs_read"].append(dict(v01d["report_inputs_read"][0]))
    cases = {
        "M08_v01_member_exists_false": ({"v01.json": v01x}, (), lambda r: r["cases"]["V01"] == ["VALID", "COUNTEREVIDENCE"]),
        "M08_v00_A46_changed_to_match": ({"v00.json": v00a}, (), lambda r: r["cases"]["V00"] == ["VALID", "OBSERVED_AS_PREDICTED"]
                                         and r["counts"]["results_for_valid_evidence"].get("COUNTEREVIDENCE") == 1),
        "M08_v00_A35_changed_to_quote_not_found": ({"v00.json": v00b}, (), lambda r: r["cases"]["V00"] == ["VALID", "COUNTEREVIDENCE"]
                                                   and r["ca_rulings"]["CA-06"] == "CONTRADICTED_OR_UNSUPPORTED_AT_SOURCE_LEVEL"),
        "M09_partial_compile_case_skipped_stage": ({"run.json": m09}, ("static.json",), lambda r: r["cases"]["V12"][0] == "BLOCKED"
                                                   and r["cases"]["V13"][0] == "BLOCKED" and r["cases"]["V06"][0] == "ERROR"
                                                   and r["cases"]["V08"] == ["INCOMPLETE_EVIDENCE", None] and r["cases"]["V14"][0] == "INCOMPLETE_EVIDENCE"
                                                   and all(r["ca_rulings"][k].startswith("UNDETERMINED") for k in ("CA-01", "CA-05", "CA-07"))
                                                   and r["cases"]["V02"][0] == "VALID"),
        "M09B_static_stage_failed_output_present": ({"run.json": m09b}, (), lambda r: r["cases"]["V08"] == ["INCOMPLETE_EVIDENCE", None]
                                                    and r["cases"]["V14"][0] == "INCOMPLETE_EVIDENCE" and r["cases"]["V04"][0] == "VALID"),
        "N02_through_cli_empty_referenced_paths": ({"v01.json": v01e}, (), lambda r: r["cases"]["V01"] == ["INCOMPLETE_EVIDENCE", None]),
        "N02_through_cli_duplicate_report_input": ({"v01.json": v01d}, (), lambda r: r["cases"]["V01"] == ["INVALID_EVIDENCE", None]),
    }
    for name, (objs, omit, pred) in cases.items():
        d, od = c.obs_copy("n05_cli_" + name, omit=omit, objects=objs)
        o, n = c.cli_old(d, od), c.cli_new(d, od)
        r = n["report"] or {}
        ok = bool(r) and pred(r)
        cli[name] = {"old_cli": {"returncode": o["returncode"], "report_written": o["report_written"], "summary": o.get("report")},
                     "new_cli": {"returncode": n["returncode"], "status": r.get("status"), "cases": r.get("cases"),
                                 "ca_rulings": r.get("ca_rulings"), "input_problems": r.get("input_problems")}, "met": ok}
        met = met and ok
    out["through_cli"] = cli
    return {"id": "N05", "input_label": META + " (M01-M05/M08/M09 transformations re-applied to the real b843 records)",
            "requirement": "old rejections still hold, legal 0/42/43 and a valid-but-different record still accepted; N02/N03 transforms through the real entry",
            "variants": out, "met": bool(met)}


# ---------------------------------------------------------------- N06: recorded observation revalidation
def n06(c, report_out):
    d, od = c.obs_copy("n06_unaltered_b843_observations")
    n = c.cli_new(d, od)
    src = os.path.join(d, "consumer.yaml")
    shutil.copy(src, report_out)
    doc = yaml.safe_load(open(src, encoding="utf-8"))
    dif = doc["diff_vs_recheck01_frozen_results"]
    by = {x["case_id"]: x for x in doc["cases"]}
    v00 = by["V00"]
    checks = {"exit0_consumer_complete": n["returncode"] == 0 and doc["status"] == "consumer_COMPLETE",
              "all_cases_identical_to_efb": not dif["cases_different"],
              "ca_rulings_identical": all(v["same"] for v in dif["ca_rulings"].values()),
              "counts_identical": dif["counts_same"], "r_m_items_identical": dif["r_items_same"] and dif["m_items_same"],
              "old_A46_failure_kept": any(a.get("id") == "A46" for a in (v00.get("failing_anchors") or [])),
              "A46_C_ASM_listed_separately": [a[0] for a in by["V00_A46_correction"]["anchors"]] == ["A46-C", "A46-ASM"],
              "not_run_layers_not_upgraded": all(v != "EXECUTED" for k, v in by["V14"]["layers"].items() if k == "existing_elf")
              and doc["counts"]["layers_not_executed"] == yaml.safe_load(C.blob(C.PINS["frozen_results"], C.R01 + "results.yaml"))["counts"]["layers_not_executed"],
              "cached_equals_consumer": not doc["consumer_validation"]["cached_evaluation_vs_consumer"]["differs"]}
    return {"id": "N06", "kind": "RECORDED_OBSERVATION_REVALIDATION",
            "source_execution": {"identity_read_time_utc": doc["source_execution"]["identity_read_time_utc"],
                                 "execution_head_short12": doc["source_execution"]["execution_head_short12"],
                                 "workdir": (doc["source_execution"]["run"] or {}).get("workdir")},
            "consumer_check": {"date_utc": doc["consumer_validation"]["consumer_date_utc"], "programs": doc["consumer_validation"]["consumer_programs"]},
            "report_file": OS.file_id(report_out), "cli": {"returncode": n["returncode"]},
            "diff_vs_recheck01_frozen_results": dif, "checks": checks, "met": all(checks.values())}


def main(frozen, parent, outp, n06_out):
    c = Ctx(frozen, parent)
    res = {"check": "CONSUMER_TESTS_N01_N06", "workdir": c.base, "frozen_extraction": c.frozen, "tests": []}
    for tid, fn in (("N01", lambda: n01(c)), ("N02", lambda: n02(c)), ("N03", lambda: n03(c)), ("N04", lambda: n04(c)),
                    ("N05", lambda: n05(c)), ("N06", lambda: n06(c, n06_out))):
        try:
            res["tests"].append(fn())
        except Exception as e:  # noqa: BLE001 - one failing test is reported, the others still run
            import traceback
            res["tests"].append({"id": tid, "met": False, "harness_error": "%s: %s" % (type(e).__name__, e),
                                 "trace_tail": traceback.format_exc()[-1500:]})
    # S04: complete, valid observations that equal a negative-control alternative value (found by N02/N05)
    T = {t["id"]: t for t in res["tests"]}
    res["S04_valid_observation_equal_to_negcontrol_value"] = {
        k: {kk: v.get(kk) for kk in ("old", "old_reasons", "new")} for k, v in
        (("N02.control_complete_hex40_hit_found", (T.get("N02", {}).get("variants") or {}).get("control_complete_hex40_hit_found", {})),
         ("N05.valid_but_different_from_prediction_v04_ret_1", (T.get("N05", {}).get("variants") or {}).get("valid_but_different_from_prediction_v04_ret_1", {})))}
    res["met"] = {t["id"]: t.get("met") for t in res["tests"]}
    res["all_met"] = all(res["met"].values())
    C.emit(res, outp)
    print(C.dump(res["met"]))
    return 0 if res["all_met"] else 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:5]))
