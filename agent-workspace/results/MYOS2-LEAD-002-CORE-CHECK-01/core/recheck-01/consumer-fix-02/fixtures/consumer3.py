# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file. Reuses evaluate2.py and make_results2.py from a b843d475367a extraction (frozen
#   bytes, imported via frozen_b843.import_frozen); the frozen files are not edited.
# change (S01): bind() wraps the frozen evaluate2.validate_case in this process. The wrapper first keeps
#   the frozen verdict, then adds the run-record fields the b843 producer writes and the consumer uses:
#   a missing field is INCOMPLETE_EVIDENCE [MISSING_FIELD], a wrong type INVALID_EVIDENCE [WRONG_TYPE], an
#   explicit failure value the frozen check ignored INVALID_EVIDENCE [EXPLICIT_FAILURE]. Nothing is
#   defaulted to exited/True/no-stderr any more.
# change (S02): v01_strict() compares referenced_paths, report_inputs_read, hex40_hits_in_scope_files,
#   parse, completion_flags and the MANIFEST tag-count table with the sets in expected_objects.json
#   (taken from frozen inputs). Missing members -> INCOMPLETE_EVIDENCE, extra/duplicate/renamed members or
#   wrong member types -> INVALID_EVIDENCE; with complete sets the frozen v01 decides (an exists=False
#   member stays a VALID finding, an empty error list stays a legal result).
# change (S03): read_json() gives every input a state (OK, MISSING, EMPTY, JSON_SYNTAX_ERROR,
#   NOT_UTF8, WRONG_TOP_TYPE) instead of raising; schema checks decide per file which consumed parts are
#   usable; consume() recomputes every item from the raw records that are usable now and never takes the
#   cached run.evaluation as a current verdict.
# change (S04, found by N05 of this round, not in the 13 contract): the frozen evaluate2.judge returns
#   ERROR/COMPARATOR_INVALID when a complete, valid observation equals the fixed alternative value of its
#   negative control; bind() also wraps judge so that such an observation is a VALID finding whenever the
#   alternative value differs from the prediction (a truly meaningless control still gives ERROR).
# --------------------------------------------------------------------------------------------------
"""Data-layer consumer for the recheck-01 observations (no C, no H00, no tree scan)."""
import copy
import datetime
import json
import os

import common3 as C
import frozen_b843 as FB

E = MR = None
_ORIG_VALIDATE = None
HERE = os.path.dirname(os.path.abspath(__file__))
INPUTS = ["run.json", "v00.json", "v01.json", "static.json", "a46.json", "old_counterexamples.json", "meta_tests.json"]
STAGE_OF = {"v00.json": "v00_anchors.py", "v01.json": "v01_structure.py", "static.json": "static_checks.py", "a46.json": "a46_check.py"}
ITEMS = ["V00", "V01", "V02", "V03", "V04", "V05", "V06", "V07", "V08", "V09", "V10", "V11", "V12", "V13", "V14_model", "V00_A46_correction"]
DYNAMIC_ITEMS = ["V02", "V03", "V04", "V05", "V06", "V07", "V08", "V09", "V10", "V11", "V12", "V13", "V14_model"]
DEPENDS = {  # which reported items consume which input (for affected_by_input)
    "run.json": DYNAMIC_ITEMS + ["V14", "stage completion of V00/V01/V08-static/V14-source/A46"],
    "v00.json": ["V00", "V14.source_and_build_reference", "CA-05", "CA-06"],
    "v01.json": ["V01"],
    "static.json": ["V08.limited_model_reachability", "V14.source_and_build_reference", "CA-02 limited-model part", "CA-05"],
    "a46.json": ["V00_A46_correction"],
    "old_counterexamples.json": ["r_items"],
    "meta_tests.json": ["m_items"],
}
REQ_CASE = {"status": str, "repeat_identical": bool, "repeat_returncode": int, "repeat_terminal_state": str}
REQ_RUN = {"returncode": int, "timed_out": bool, "collect_timed_out": bool, "output_lost": bool, "terminal_state": str,
           "reaped_confirmed": bool, "stdout": str, "stderr": str}
MISSING, WRONG, FAIL = "MISSING_FIELD", "WRONG_TYPE", "EXPLICIT_FAILURE"


def bind(fixdir):
    """Import the frozen modules and install the strict case validator and the judge correction
    (in this process only; the frozen files are not changed)."""
    global E, MR, _ORIG_VALIDATE, _ORIG_JUDGE
    E, MR = FB.import_frozen(fixdir)
    if _ORIG_VALIDATE is None:
        _ORIG_VALIDATE = E.validate_case
        E.validate_case = strict_validate_case
        _ORIG_JUDGE = E.judge
        E.judge = judge_with_meaningful_negcontrol
    return E, MR


_ORIG_JUDGE = None


def judge_with_meaningful_negcontrol(checks, wrong):
    """S04 (found by N05 in this round): the frozen judge feeds the OBSERVED value against a fixed
    alternative value; when a complete, valid observation equals that alternative (e.g. V04 ret=1,
    V01 hex40 count 1) it returns ERROR/COMPARATOR_INVALID instead of COUNTEREVIDENCE. The comparator is
    only meaningless when an alternative value equals the prediction itself; otherwise such an
    observation is a finding."""
    r = _ORIG_JUDGE(checks, wrong)
    if r.get("status") != "ERROR" or not str(r.get("reason", "")).startswith("COMPARATOR_INVALID") or not checks:
        return r
    pred = {}
    for label, _, p in checks:
        pred.setdefault(label, []).append(p)
    meaningful = all(label in pred and all(p != w for p in pred[label]) for label, _, w in wrong)
    if not meaningful:
        return r
    mism = [c for c in checks if c[1] != c[2]]
    return {"status": "VALID", "result": "OBSERVED_AS_PREDICTED" if not mism else "COUNTEREVIDENCE",
            "checks": [{"label": a, "observed": b, "predicted": c, "match": b == c} for a, b, c in checks],
            "negcontrol_wrong_prediction_detected": False,
            "negcontrol_note": "observation equals the alternative value of the negative control; the alternative differs from "
                               "the prediction, so the comparator discriminates and the observation is judged as a finding",
            "negcontrol": [{"label": a, "observed": b, "wrong_prediction": c} for a, b, c in wrong]}


def expected(path=None):
    with open(path or os.path.join(HERE, "expected_objects.json"), encoding="utf-8") as f:
        return json.load(f)


def worst(a, b):
    order = E.STATUS_ORDER
    if a is None:
        return b
    return a if order.index(a) <= order.index(b) else b


def _typed(v, t):
    return type(v) is t  # bool is not accepted where int is required, and vice versa


# ---------------------------------------------------------------- S01: run-record metadata
def run_metadata_problems(fx_rec, case):
    """Problems in the fields a RAN case must carry. Records the frozen validator rejects early
    (fixture not BUILT, case not RAN, run record absent) are left to it."""
    if not isinstance(fx_rec, dict) or fx_rec.get("status") != "BUILT":
        return []
    c = (fx_rec.get("cases") or {}).get(case)
    if not isinstance(c, dict) or c.get("status") != "RAN":
        return []
    out = []

    def need(obj, where, spec):
        for k, t in spec.items():
            if k not in obj:
                out.append(("INCOMPLETE_EVIDENCE", MISSING, "%s.%s" % (where, k), "%s.%s missing" % (where, k)))
            elif not _typed(obj[k], t):
                out.append(("INVALID_EVIDENCE", WRONG, "%s.%s" % (where, k),
                            "%s.%s has type %s, expected %s" % (where, k, type(obj[k]).__name__, t.__name__)))
    need(c, "case", REQ_CASE)
    run = c.get("run")
    if isinstance(run, dict):
        need(run, "run", REQ_RUN)
        if run.get("output_lost") is True:
            out.append(("INVALID_EVIDENCE", FAIL, "run.output_lost", "run.output_lost=True"))
    if isinstance(c.get("repeat_terminal_state"), str) and c["repeat_terminal_state"] != "exited":
        out.append(("INVALID_EVIDENCE", FAIL, "case.repeat_terminal_state", "repeat run terminal_state=%r" % c["repeat_terminal_state"]))
    return out


def strict_validate_case(fx_rec, fixture, case):
    out = _ORIG_VALIDATE(fx_rec, fixture, case)
    probs = run_metadata_problems(fx_rec, case)
    out["metadata_problems"] = [{"status": s, "category": cat, "field": f} for s, cat, f, _ in probs]
    for s, cat, _, msg in probs:
        out["reasons"].append("[%s] %s" % (cat, msg))
        out["status"] = worst(out["status"], s)
    if probs:
        out["index"] = None
    return out


# ---------------------------------------------------------------- S02: V01 object sets
def v01_collection_problems(v, exp):
    x = exp["v01"]
    out = []

    def add(status, cat, coll, detail):
        out.append({"status": status, "category": cat, "collection": coll, "detail": detail})
    tops = {"parse": dict, "issue_ids_expected_CA01_CA07": bool, "anchor_refs": dict, "case_refs": dict,
            "contract09_anchor_refs_dangling": list, "referenced_paths": dict, "report_inputs_read": list,
            "manifest_self_check": dict, "startup_selfcheck_quote_in_master_conventions": bool, "completion_flags": dict,
            "hex40_hits_in_scope_files": dict}
    for k, t in tops.items():
        if k not in v:
            add("INCOMPLETE_EVIDENCE", "MISSING_COLLECTION", k, "key absent")
        elif not _typed(v[k], t):
            add("INVALID_EVIDENCE", "WRONG_TYPE", k, "type %s, expected %s" % (type(v[k]).__name__, t.__name__))
    if out:
        return out

    def keyset(coll, have, want):
        miss, extra = sorted(set(want) - set(have)), sorted(set(have) - set(want))
        if miss:
            add("INCOMPLETE_EVIDENCE", "MISSING_OBJECT", coll, miss)
        if extra:
            add("INVALID_EVIDENCE", "UNEXPECTED_OBJECT", coll, extra)
    rp = v["referenced_paths"]
    keyset("referenced_paths", rp.keys(), x["referenced_paths"].keys())
    for k, p in x["referenced_paths"].items():
        m = rp.get(k)
        if k not in rp:
            continue
        if not isinstance(m, dict) or m.get("path") != p:
            add("INVALID_EVIDENCE", "MEMBER_IDENTITY", "referenced_paths", k)
        elif "exists_taskbook" not in m:
            add("INCOMPLETE_EVIDENCE", "MISSING_FIELD", "referenced_paths", "%s.exists_taskbook" % k)
        elif not _typed(m["exists_taskbook"], bool):
            add("INVALID_EVIDENCE", "WRONG_TYPE", "referenced_paths", "%s.exists_taskbook" % k)
    ri = v["report_inputs_read"]
    paths = []
    for n, m in enumerate(ri):
        if not isinstance(m, dict) or not isinstance(m.get("path"), str):
            add("INVALID_EVIDENCE", "MEMBER_IDENTITY", "report_inputs_read", "element %d" % n)
            continue
        paths.append(m["path"])
        if "exists" not in m:
            add("INCOMPLETE_EVIDENCE", "MISSING_FIELD", "report_inputs_read", "%s.exists" % m["path"])
        elif not _typed(m["exists"], bool):
            add("INVALID_EVIDENCE", "WRONG_TYPE", "report_inputs_read", "%s.exists" % m["path"])
    dups = sorted({p for p in paths if paths.count(p) > 1})
    if dups:
        add("INVALID_EVIDENCE", "DUPLICATE_OBJECT", "report_inputs_read", dups)
    keyset("report_inputs_read", paths, x["report_inputs_read"])
    hx = v["hex40_hits_in_scope_files"]
    keyset("hex40_hits_in_scope_files", hx.keys(), x["hex40_scope_file_keys"])
    bad = sorted(k for k, n in hx.items() if not (_typed(n, int) and n >= 0))
    if bad:
        add("INVALID_EVIDENCE", "WRONG_TYPE", "hex40_hits_in_scope_files", bad)
    keyset("parse", v["parse"].keys(), x["parse_keys"])
    bad = sorted(k for k, m in v["parse"].items() if not (isinstance(m, dict) and _typed(m.get("ok"), bool)))
    if bad:
        add("INVALID_EVIDENCE", "WRONG_TYPE", "parse", bad)
    keyset("completion_flags", v["completion_flags"].keys(), x["completion_flag_keys"])
    tc = v["manifest_self_check"].get("tag_counts_per_scope_file")
    if not isinstance(tc, dict):
        add("INCOMPLETE_EVIDENCE", "MISSING_COLLECTION", "manifest_self_check.tag_counts_per_scope_file", "absent or not a mapping")
    else:
        keyset("manifest_self_check.tag_counts_per_scope_file", tc.keys(), x["manifest_scope_files"])
    return out


def v01_strict(v01j, stage_rec, exp):
    res = {"case_id": "V01"}
    if stage_rec is not None and (stage_rec.get("returncode") != 0 or stage_rec.get("timed_out")):
        return E.v01(v01j, stage_rec)
    if not isinstance(v01j, dict):
        return dict(res, status="INCOMPLETE_EVIDENCE", result=None, evidence_reasons=["v01 record missing or not a mapping"])
    probs = v01_collection_problems(v01j, exp)
    if probs:
        st = None
        for p in probs:
            st = worst(st, p["status"])
        return dict(res, status=st, result=None, collection_problems=probs,
                    evidence_reasons=["[%s] %s: %s" % (p["category"], p["collection"], p["detail"]) for p in probs][:8])
    try:
        r = E.v01(v01j, stage_rec)
    except Exception as e:  # noqa: BLE001 - a reader failure, not a finding
        return dict(res, status="ERROR", result=None, evidence_reasons=["frozen v01 raised %s: %s" % (type(e).__name__, e)])
    r["collection_check"] = "complete against expected_objects.json"
    return r


# ---------------------------------------------------------------- S03: inputs, schemas, consumption
def read_json(path):
    """(object or None, state record). Never raises for a bad input file."""
    name = os.path.basename(path)
    st = {"file": name, "state": None, "error": None, "bytes": None, "sha256_segments": None}
    if not os.path.exists(path):
        st["state"] = "MISSING"
        return None, st
    with open(path, "rb") as f:
        data = f.read()
    st["bytes"], st["sha256_segments"] = len(data), C.sha_segments(data)
    if not data.strip():
        st["state"] = "EMPTY"
        return None, st
    try:
        obj = json.loads(data.decode("utf-8"))
    except UnicodeDecodeError as e:
        st["state"], st["error"] = "NOT_UTF8", str(e)[:200]
        return None, st
    except ValueError as e:
        st["state"], st["error"] = "JSON_SYNTAX_ERROR", "%s: %s" % (type(e).__name__, str(e)[:200])
        return None, st
    if not isinstance(obj, dict):
        st["state"], st["error"] = "WRONG_TOP_TYPE", "top-level %s, expected object" % type(obj).__name__
        return None, st
    st["state"] = "OK"
    return obj, st


def state_status(state):
    return "INCOMPLETE_EVIDENCE" if state == "MISSING" else "INVALID_EVIDENCE"


def v00_schema(v, exp):
    a = v.get("anchors")
    if not isinstance(a, list) or not all(isinstance(x, dict) for x in a):
        return ["anchors absent or not a list of objects"]
    ids = [x.get("id") for x in a]
    p = []
    want = exp["v00"]["anchor_ids"]
    if sorted({i for i in ids if ids.count(i) > 1}):
        p.append("duplicate anchor ids %s" % sorted({i for i in ids if ids.count(i) > 1}))
    if sorted(set(want) - set(ids)):
        p.append("missing anchors %s" % sorted(set(want) - set(ids)))
    if sorted(set(ids) - set(want)):
        p.append("unexpected anchors %s" % sorted(set(ids) - set(want)))
    for x in a:
        miss = [k for k in exp["v00"]["consumed_anchor_keys"] if k not in x]
        if miss:
            p.append("%s missing %s" % (x.get("id"), miss))
    if not _typed(v.get("tag_count_regex"), int):
        p.append("tag_count_regex absent or not int")
    return p


def a46_schema(v, exp):
    a = v.get("anchors")
    if not isinstance(a, list) or not all(isinstance(x, dict) for x in a):
        return ["anchors absent or not a list of objects"]
    ids = [x.get("id") for x in a]
    p = []
    if sorted(ids) != sorted(exp["a46"]["anchor_ids"]):
        p.append("anchor ids %s, expected %s" % (ids, exp["a46"]["anchor_ids"]))
    p += ["%s verdict_mech absent" % x.get("id") for x in a if not isinstance(x.get("verdict_mech"), str)]
    if not isinstance(v.get("old_A46_verdict_in_frozen_v00"), str):
        p.append("old_A46_verdict_in_frozen_v00 absent")
    return p


def static_parts(v, exp):
    """{'V08': problems, 'V14': problems} for the keys the frozen consumers read (all must be lists)."""
    out = {}
    for part, keys in exp["static"].items():
        s = v.get(part)
        if not isinstance(s, dict):
            out[part] = ["%s absent or not an object" % part]
            continue
        out[part] = ["%s.%s %s" % (part, k, "absent" if k not in s else "not a list") for k in keys if not isinstance(s.get(k), list)]
    return out


def run_schema(run):
    p = {"fixtures": [], "stages": []}
    fx, sg = run.get("fixtures"), run.get("stages")
    if not isinstance(fx, dict):
        p["fixtures"].append("fixtures absent or not an object")
    else:
        want = sorted({f for f, _ in E.CONTRACTS})
        p["fixtures"] += ["fixture %s absent" % f for f in want if f not in fx]
        p["fixtures"] += ["fixture %s not an object" % f for f in want if f in fx and not isinstance(fx[f], dict)]
    if not isinstance(sg, dict):
        p["stages"].append("stages absent or not an object")
    return p


def consume(obs_dir, exp=None):
    """Re-judge the recorded observations in obs_dir. Returns (report dict, exit code)."""
    exp = exp or expected()
    objs, states = {}, {}
    for n in INPUTS:
        objs[n], states[n] = read_json(os.path.join(obs_dir, n))
    run = objs["run.json"]
    schema = {n: [] for n in INPUTS}
    reasons = {n: None for n in INPUTS}   # why an input is not usable now
    for n in INPUTS:
        if states[n]["state"] != "OK":
            reasons[n] = "%s %s%s" % (n, states[n]["state"], (": " + states[n]["error"]) if states[n]["error"] else "")
    rs = run_schema(run) if run is not None else {"fixtures": [reasons["run.json"]], "stages": [reasons["run.json"]]}
    schema["run.json"] = rs["fixtures"] + rs["stages"] if run is not None else []
    fx = run.get("fixtures") if run is not None and not rs["fixtures"] else None
    src_stages = run.get("stages") if run is not None and not rs["stages"] else None
    usable, cstages = {}, {}
    static_ok = {"V08": False, "V14": False}
    for n, stage in STAGE_OF.items():
        rec = (src_stages or {}).get(stage) if src_stages is not None else None
        why = None
        if src_stages is None:
            why = "stage completion record unavailable (%s)" % (reasons["run.json"] or "run.json stages unusable")
        elif not isinstance(rec, dict) or rec.get("returncode") != 0 or rec.get("timed_out") is not False:
            why = "source stage %s did not complete per run.json" % stage
        elif reasons[n]:
            why = reasons[n]
        else:
            o = objs[n]
            if n == "v00.json":
                schema[n] = v00_schema(o, exp)
            elif n == "a46.json":
                schema[n] = a46_schema(o, exp)
            elif n == "static.json":
                parts = static_parts(o, exp)
                schema[n] = parts["V08"] + parts["V14"]
                static_ok = {k: not v for k, v in parts.items()}
            if schema[n] and n != "static.json":
                why = "%s schema: %s" % (n, "; ".join(schema[n])[:300])
            if n == "static.json" and not any(static_ok.values()):
                why = "%s schema: %s" % (n, "; ".join(schema[n])[:300])
        usable[n] = why is None
        if why is None and n == "static.json" and not all(static_ok.values()):
            reasons[n] = "%s schema: %s" % (n, "; ".join(schema[n])[:300])
        elif why is not None:
            reasons[n] = why
        cstages[stage] = (dict(rec, consumer_usable=True) if why is None
                          else {"returncode": None, "timed_out": None, "status": "CONSUMER_UNUSABLE", "reason": why})
    # ---- consumer evaluation from the raw records usable now (cached run.evaluation is not used)
    ev = E.evaluate_all(fx if fx is not None else {}, objs["static.json"] if usable["static.json"] and static_ok["V08"] else None)
    if fx is None:
        why = reasons["run.json"] or "run.json fixtures unusable: %s" % "; ".join(schema["run.json"])
        st = state_status(states["run.json"]["state"]) if states["run.json"]["state"] != "OK" else "INCOMPLETE_EVIDENCE"
        for k in DYNAMIC_ITEMS:
            ev[k] = {"case_id": k, "status": st, "result": None, "evidence_reasons": ["dynamic raw observations unavailable: " + why]}
    elif ev["V08"].get("status") == "INCOMPLETE_EVIDENCE" and not (usable["static.json"] and static_ok["V08"]):
        ev["V08"].setdefault("evidence_reasons", []).append("static input: %s" % reasons["static.json"])

    def gated(n, fn, *a):
        if usable[n]:
            return fn(objs[n], cstages[STAGE_OF[n]], *a)
        st = state_status(states[n]["state"]) if states[n]["state"] not in ("OK",) else "INCOMPLETE_EVIDENCE"
        return {"status": st, "result": None, "evidence_reasons": ["not usable: %s" % reasons[n]]}
    ev["V00"] = dict({"case_id": "V00"}, **gated("v00.json", E.v00))
    if usable["v01.json"]:
        ev["V01"] = v01_strict(objs["v01.json"], cstages["v01_structure.py"], exp)
    else:
        ev["V01"] = dict({"case_id": "V01"}, **gated("v01.json", None))
    ev["V00_A46_correction"] = dict({"case_id": "V00_A46_correction"}, **gated("a46.json", E.a46r))
    # ---- frozen generator on the consumer view (stages = consumer usability, evaluation = consumer)
    crun = {k: (run or {}).get(k) for k in ("status", "exit_code", "execution_complete", "verification_findings",
                                           "infrastructure_errors", "dynamic_calls", "workdir", "meta_injection",
                                           "stage_outputs_present", "stage_outputs_usable", "identity", "h00", "frozen_manifest")}
    crun["stages"] = cstages
    crun["evaluation"] = ev
    doc = MR.build_doc(crun, objs["v00.json"] if usable["v00.json"] else None, objs["v01.json"] if usable["v01.json"] else None,
                       objs["static.json"] if usable["static.json"] and static_ok["V14"] else None,
                       objs["a46.json"] if usable["a46.json"] else None,
                       objs["old_counterexamples.json"], objs["meta_tests.json"], MR.frozen_old_results())
    src = {"note_template": "facts recorded by the source run; this consumer did not re-execute anything",
           "run": doc.pop("run", None), "identity": doc.pop("identity", None), "h00": doc.pop("h00", None),
           "frozen_verifier": doc.pop("frozen_verifier", None),
           "identity_read_time_utc": ((run or {}).get("identity") or {}).get("read_time_utc") if run else None,
           "execution_head_short12": (((run or {}).get("identity") or {}).get("execution") or {}).get("head_short12") if run else None}
    stages_view = doc.pop("stages", None)
    cached = (run or {}).get("evaluation") if isinstance((run or {}).get("evaluation"), dict) else {}
    cmp_cached = {k: {"cached_in_run_json": [(cached.get(k) or {}).get("status"), (cached.get(k) or {}).get("result")] if k in cached else None,
                      "consumer_now": [(ev.get(k) or {}).get("status"), (ev.get(k) or {}).get("result")]} for k in ITEMS}
    for v in cmp_cached.values():
        v["same"] = v["cached_in_run_json"] == v["consumer_now"]
    items = {k: [(ev.get(k) or {}).get("status"), (ev.get(k) or {}).get("result")] for k in ITEMS}
    bad_inputs = [n for n in INPUTS if reasons[n]]
    not_valid = {k: ((ev.get(k) or {}).get("evidence_reasons") or [(ev.get(k) or {}).get("reason")])[:3] for k, v in items.items() if v[0] != "VALID"}
    case_states = {c["case_id"]: c.get("evidence_status") for c in doc.get("cases", []) if isinstance(c, dict)}
    complete = not bad_inputs and not not_valid and not doc.get("generator_errors") and all(v == "VALID" for v in case_states.values())
    cv = {"kind": "RECORDED_OBSERVATION_REVALIDATION", "status": "COMPLETE" if complete else "PARTIAL",
          "exit_code_template": "0 complete; 2 partial (report written); 4 consumer failure",
          "consumer_date_utc": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d"),
          "consumer_programs": "consumer-fix-02/fixtures/consumer3.py + make_results3.py; frozen evaluate2/make_results2 from %s"
                               % C.short12(C.PINS["reviewed_input"]),
          "inputs": [states[n] for n in INPUTS], "input_problems": {n: reasons[n] for n in bad_inputs},
          "schema_problems": {n: p for n, p in schema.items() if p},
          "dynamic_raw_observations_available": fx is not None,
          "stage_usability": {s: {"usable": r.get("consumer_usable", False), "reason": r.get("reason")} for s, r in cstages.items()},
          "static_parts_usable": static_ok,
          "affected_by_input": {n: DEPENDS[n] for n in bad_inputs},
          "items": items, "items_not_valid": not_valid,
          "cached_evaluation_vs_consumer": {"differs": sorted(k for k, v in cmp_cached.items() if not v["same"]), "detail": cmp_cached}}
    head = {"task_id": C.PACKET, "packet_id": C.PACKET, "followup_id": C.FOLLOWUP, "phase": "evidence_consumer_recheck",
            "record_type": "recorded_observation_revalidation",
            "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
            "execution_model_selection": "unknown_or_not_attestable",
            "status": "consumer_%s" % cv["status"], "acceptance_ceiling": "PASS_PENDING_LOCAL", "results_are_generated": True}
    out = dict(head)
    out["consumer_validation"] = cv
    out["source_execution"] = src
    out["consumer_stage_view"] = stages_view
    out.update(doc)
    out["diff_vs_recheck01_frozen_results"] = diff_vs_efb(out)
    return out, (0 if complete else 2)


def diff_vs_efb(doc):
    import yaml
    old = yaml.safe_load(C.blob(C.PINS["frozen_results"], C.R01 + "results.yaml").decode("utf-8"))
    oc = {c["case_id"]: c for c in old.get("cases", [])}
    nc = {c["case_id"]: c for c in doc.get("cases", []) if isinstance(c, dict)}
    fields = ("evidence_status", "result", "layers", "checks")
    cases = {k: {f: (oc.get(k) or {}).get(f) == (nc.get(k) or {}).get(f) for f in fields} for k in sorted(set(oc) | set(nc))}
    oca = {k: v.get("ruling") for k, v in (old.get("ca_rulings") or {}).items()}
    nca = {k: (v or {}).get("ruling") for k, v in (doc.get("ca_rulings") or {}).items() if isinstance(v, dict)}
    return {"source": "recheck-01 results.yaml @ %s" % C.short12(C.PINS["frozen_results"]),
            "cases_identical": sorted(k for k, v in cases.items() if all(v.values())),
            "cases_different": {k: [f for f, same in v.items() if not same] for k, v in cases.items() if not all(v.values())},
            "ca_rulings": {k: {"efb": oca.get(k), "now": nca.get(k), "same": oca.get(k) == nca.get(k)} for k in sorted(set(oca) | set(nca))},
            "counts_same": old.get("counts") == doc.get("counts"),
            "diff_vs_frozen_core_same": old.get("diff_vs_frozen_core") == doc.get("diff_vs_frozen_core"),
            "r_items_same": old.get("r_items") == doc.get("r_items"), "m_items_same": old.get("m_items") == doc.get("m_items")}
