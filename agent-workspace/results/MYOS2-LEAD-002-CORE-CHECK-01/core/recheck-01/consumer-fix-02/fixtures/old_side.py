# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: S01-S03 of the 13 contract on the UNMODIFIED b843 programs and observations, before any fix.
#   Each experiment runs in its own Python process (30 s limit) with the b843 extraction as working
#   directory; altered inputs are written only to a fresh directory and labelled HARNESS_META_TEST.
#   Only evaluate2 (S01, S02) and the make_results2 CLI (S03) are called; nothing compiles or runs C.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 old_side.py <frozen extraction dir> <work parent> <out.json>"""
import copy
import json
import os
import shutil
import sys

import yaml

import common3 as C

META = "HARNESS_META_TEST"
S01_KEYS = ["collect_timed_out", "terminal_state", "reaped_confirmed", "stderr"]
S02_SETS = {"referenced_paths": {}, "report_inputs_read": [], "hex40_hits_in_scope_files": {}}
GEN_INPUTS = ["run.json", "v00.json", "v01.json", "static.json", "a46.json", "old_counterexamples.json", "meta_tests.json"]
CALL = r'''
import json, sys, evaluate2 as E
spec = json.load(open(sys.argv[1]))
arg = json.load(open(spec["input"]))
fn = getattr(E, spec["fn"])
r = fn(arg, spec.get("stage_rec")) if spec["fn"] in ("v01", "v00", "a46r") else fn(arg)
print(json.dumps({"status": r.get("status"), "result": r.get("result"), "constituents": r.get("constituents"),
                  "reasons": (r.get("evidence_reasons") or [r.get("reason")])[:6],
                  "checks": [[c["label"], c["observed"], c["predicted"], c["match"]] for c in r.get("checks", [])]}))
'''


def file_id(path):
    with open(path, "rb") as f:
        d = f.read()
    return {"bytes": len(d), "sha256_segments": C.sha_segments(d)}


def call_old(frozen, d, fn, obj, stage_rec=None):
    inp = os.path.join(d, "input.json")
    with open(inp, "w", encoding="utf-8") as f:
        json.dump(obj, f, ensure_ascii=False)
    spec = os.path.join(d, "spec.json")
    with open(spec, "w", encoding="utf-8") as f:
        json.dump({"fn": fn, "input": inp, "stage_rec": stage_rec}, f)
    r = C.run_py(["-c", CALL, spec], cwd=os.path.join(frozen, "fixtures"))
    return {"input": file_id(inp), "returncode": r["returncode"], "timed_out": r["timed_out"],
            "returned": C.last_json(r["stdout"]), "stderr_tail": C.tail(r["stderr"])}


def summarize_report(path):
    """Fields of a generated results.yaml that S03 is about (None when no report was written)."""
    if not os.path.exists(path):
        return None
    doc = yaml.safe_load(open(path, encoding="utf-8"))
    by = {c["case_id"]: c for c in doc.get("cases", [])}
    pick = lambda cid: {"evidence_status": by.get(cid, {}).get("evidence_status"), "result": by.get(cid, {}).get("result"),
                        "layers": by.get(cid, {}).get("layers")}
    v08 = pick("V08")
    v08["limited_model_reachability"] = (by.get("V08", {}).get("result_basis") or {}).get("limited_model_reachability")
    return {"status": doc.get("status"), "run_status": (doc.get("run") or {}).get("status"), "V08": v08, "V14": pick("V14"),
            "CA-02": (doc.get("ca_rulings") or {}).get("CA-02", {}).get("ruling"),
            "CA-05": (doc.get("ca_rulings") or {}).get("CA-05", {}).get("ruling"),
            "generator_errors": doc.get("generator_errors"), "counts_evidence_status": (doc.get("counts") or {}).get("evidence_status")}


def cli_old(frozen, d, obs_dir):
    out = os.path.join(d, "results.yaml")
    r = C.run_py(["make_results2.py", obs_dir, out], cwd=os.path.join(frozen, "fixtures"))
    return {"argv": ["make_results2.py", "<obs>", "<out.yaml>"], "returncode": r["returncode"], "timed_out": r["timed_out"],
            "stdout_tail": C.tail(r["stdout"]), "stderr_tail": C.tail(r["stderr"]), "report_written": os.path.exists(out),
            "report": summarize_report(out), "report_file": file_id(out) if os.path.exists(out) else None, "_out": out}


def main(frozen, parent, outp):
    wd = C.fresh_dir(parent, "old-side-")
    obs = os.path.join(frozen, "observations")
    run = json.load(open(os.path.join(obs, "run.json"), encoding="utf-8"))
    v01 = json.load(open(os.path.join(obs, "v01.json"), encoding="utf-8"))
    res = {"check": "OLD_SIDE_S01_S03", "workdir": wd, "frozen_programs": "b843d475367a recheck-01/fixtures (extraction %s)" % frozen,
           "input_label": META + " (altered copies of the b843 observations; originals untouched)"}
    n = [0]

    def sub(name):
        n[0] += 1
        d = os.path.join(wd, "%02d_%s" % (n[0], name))
        os.makedirs(d)
        return d
    # S01 --------------------------------------------------------------------------------------
    fx = copy.deepcopy(run["fixtures"])
    s01 = {"case": "fx_sched/v04_noncurrent_wake", "deleted_run_keys": S01_KEYS,
           "control_unmodified": call_old(frozen, sub("s01_control"), "v04", fx)}
    fx_del = copy.deepcopy(fx)
    for k in S01_KEYS:
        del fx_del["fx_sched"]["cases"]["v04_noncurrent_wake"]["run"][k]
    s01["keys_left_in_run"] = sorted(fx_del["fx_sched"]["cases"]["v04_noncurrent_wake"]["run"])
    s01["deleted_four_keys"] = call_old(frozen, sub("s01_deleted"), "v04", fx_del)
    got = (s01["deleted_four_keys"]["returned"] or {})
    s01["lead_prediction"] = ["VALID", "OBSERVED_AS_PREDICTED"]
    s01["reproduced"] = [got.get("status"), got.get("result")] == s01["lead_prediction"]
    res["S01"] = s01
    # S02 --------------------------------------------------------------------------------------
    srec = run["stages"].get("v01_structure.py")
    s02 = {"stage_rec_from_run_json": {k: srec.get(k) for k in ("returncode", "timed_out")},
           "control_unmodified": call_old(frozen, sub("s02_control"), "v01", v01, srec), "variants": {}}
    for k, empty in S02_SETS.items():
        v = copy.deepcopy(v01)
        v[k] = copy.deepcopy(empty)
        s02["variants"]["empty_" + k] = call_old(frozen, sub("s02_" + k), "v01", v, srec)
    v = copy.deepcopy(v01)
    v.update(copy.deepcopy(S02_SETS))
    s02["variants"]["empty_all_three"] = call_old(frozen, sub("s02_all"), "v01", v, srec)
    s02["lead_prediction"] = ["VALID", "NO_FAILURE_IN_SCOPE"]
    s02["reproduced"] = {k: [(x["returned"] or {}).get("status"), (x["returned"] or {}).get("result")] == s02["lead_prediction"]
                         for k, x in s02["variants"].items()}
    res["S02"] = s02
    # S03 --------------------------------------------------------------------------------------
    s03 = {"entry": "frozen make_results2.py CLI (generate -> _load -> build_doc -> render)"}
    d = sub("s03_control_complete")
    od = os.path.join(d, "obs")
    os.makedirs(od)
    for f in GEN_INPUTS:
        shutil.copy(os.path.join(obs, f), od)
    ctl = cli_old(frozen, d, od)
    frozen_yaml = C.blob(C.PINS["frozen_results"], C.R01 + "results.yaml")
    out = ctl.pop("_out")
    ctl["report_identical_to_efb_results_yaml"] = ctl["report_written"] and open(out, "rb").read() == frozen_yaml
    s03["control_complete_copy"] = ctl
    d = sub("s03_static_omitted")
    od = os.path.join(d, "obs")
    os.makedirs(od)
    for f in GEN_INPUTS:
        if f != "static.json":
            shutil.copy(os.path.join(obs, f), od)
    om = cli_old(frozen, d, od)
    om.pop("_out")
    om["inputs_present"] = sorted(os.listdir(od))
    s03["static_omitted"] = om
    d = sub("s03_static_truncated")
    od = os.path.join(d, "obs")
    os.makedirs(od)
    for f in GEN_INPUTS:
        shutil.copy(os.path.join(obs, f), od)
    with open(os.path.join(od, "static.json"), "w") as f:
        f.write("{")
    tr = cli_old(frozen, d, od)
    tr.pop("_out")
    tr["static_json_content"] = "{"
    s03["static_truncated_json"] = tr
    rep = om.get("report") or {}
    s03["lead_prediction"] = {"static_omitted": "V14 source layer missing evidence, V08 still cached VALID with limited-model verdict",
                              "static_truncated_json": "JSONDecodeError before section error collection; no partial report"}
    s03["reproduced"] = {
        "static_omitted": bool(rep) and (rep.get("V14") or {}).get("evidence_status") != "VALID"
        and (rep.get("V08") or {}).get("evidence_status") == "VALID"
        and (rep.get("V08") or {}).get("limited_model_reachability") == "COUNTEREVIDENCE_IN_LIMITED_MODEL",
        "static_truncated_json": tr["returncode"] not in (0, None) and not tr["report_written"]
        and any("JSONDecodeError" in x for x in tr["stderr_tail"])}
    res["S03"] = s03
    res["all_three_reproduced"] = bool(s01["reproduced"] and all(s02["reproduced"].values()) and all(s03["reproduced"].values()))
    C.emit(res, outp)
    print(C.dump({"S01": s01["reproduced"], "S02": s02["reproduced"], "S03": s03["reproduced"]}))
    return 0


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:4]))
