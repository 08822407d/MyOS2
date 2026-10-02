# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/make_results.py @ a1e7c2277705 (frozen original unchanged)
# change (R03): every case status/result, layer status, count and CA ruling is computed from the
#   supplied run data with explicit criteria; missing stages, blocked fixtures, errors and partial runs
#   produce UNDETERMINED/NOT_RUN entries instead of fixed text or a crash. Fixed explanatory text is kept
#   only under keys named *_template. Section failures are collected in generator_errors.
# change (batch 2): stage outputs (v00/static/a46) are used only when their stage completed; V08 layers
#   follow its function-level subresult and its complete evidence separately; CA-02 keeps the
#   function-level part when only the static criteria are missing; CA-06 and the V14 source layer need
#   VALID V00 evidence.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 make_results2.py <observations_dir> <out.yaml>
Reads run.json, v00.json, v01.json, static.json, a46.json, old_counterexamples.json, meta_tests.json
from the directory (each optional) and the frozen old results.yaml from Git (pin core_results)."""
import json
import os
import re
import sys

import yaml

import harness2 as H

ORDER = ["V00", "V01", "V02", "V03", "V04", "V05", "V06", "V07", "V08", "V09", "V10", "V11", "V12", "V13", "V14"]
LAYER_TEMPLATES = {
    "V00": {"source_match": "V00"}, "V01": {"structure": "V01"},
    "V02": {"host_original_slice": "V02"}, "V03": {"host_original_slice": "V03"},
    "V04": {"host_original_slice": "V04", "smp_or_real_scheduler": None},
    "V05": {"host_original_slice": "V05", "irq_or_signal_context": None},
    "V06": {"host_original_slice_serial": "V06", "concurrent_wakeups": None},
    "V07": {"host_original_slice": "V07", "ap_migration": None},
    "V08": {"host_original_slice_function_level": "V08_function", "limited_model_reachability": "V08", "global_reachability": None},
    "V09": {"host_original_slice_stub_timers": "V09", "real_timer_expiry": None},
    "V10": {"host_original_slice_scripted_interleaving": "V10", "real_timer_or_scheduler": None},
    "V11": {"local_sequence_scripted_interleaving": "V11", "real_context_switch": None},
    "V12": {"host_original_asm_single_thread": "V12", "multicore_atomicity": None},
    "V13": {"host_original_slice_serial": "V13", "smp_stress": None},
    "V14": {"source_and_build_reference": "V14_source", "host_link_model": "V14_model", "existing_elf": None},
}
NOT_RUN_REASON_TEMPLATE = "outside the authorised scope of this host recheck (no kernel run, no QEMU, no SMP/IRQ, no ELF build)"
CA_RULES = {
    "CA-01": {"V04": "OBSERVED_AS_PREDICTED", "V05": "OBSERVED_AS_PREDICTED", "V06": "NO_FAILURE_IN_SCOPE", "V07": "OBSERVED_AS_PREDICTED"},
    "CA-03": {"V09": "OBSERVED_AS_PREDICTED", "V10": "OBSERVED_AS_PREDICTED", "V11": "OBSERVED_AS_PREDICTED"},
    "CA-04": {"V02": "OBSERVED_AS_PREDICTED", "V03": "OBSERVED_AS_PREDICTED", "V11": "OBSERVED_AS_PREDICTED"},
    "CA-07": {"V12": "OBSERVED_AS_PREDICTED", "V13": "OBSERVED_AS_PREDICTED"},
}
CA06_ANCHORS = ["A01", "A35", "A36", "A37"]
V14_SOURCE_ANCHORS = ["A28", "A29", "A30", "A31"]
ACCEPT_MECH = {"MATCH_IN_DEFINITION", "MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE"}


def _load(d, name):
    p = os.path.join(d, name)
    if not os.path.exists(p):
        return None
    with open(p, encoding="utf-8") as f:
        return json.load(f)


def compact(e):
    if not isinstance(e, dict):
        return e
    keep = ("status", "result", "constituents", "evidence_reasons", "reason", "result_basis", "failing_anchors",
            "contract_mismatch_vectors", "observations_beyond_prediction", "layers", "extra_observation_requeue_order",
            "mechanical_verdict_counts", "semantic_verdict_counts", "semantic_source")
    out = {k: e[k] for k in keep if k in e}
    if "checks" in e:
        out["checks"] = [[c["label"], c["observed"], c["predicted"], c["match"]] for c in e["checks"]]
    if "subresults" in e:
        out["subresults"] = {k: {"status": v.get("status"), "result": v.get("result"),
                                 "checks": [[c["label"], c["observed"], c["predicted"], c["match"]] for c in v.get("checks", [])]}
                             for k, v in e["subresults"].items()}
    return out


def v14_source(v00, static):
    """Source/build-reference layer of V14, computed from V00 anchors and static checks."""
    if not isinstance(v00, dict) or not isinstance(static, dict) or "V14" not in static:
        return {"status": "INCOMPLETE_EVIDENCE", "result": None, "evidence_reasons": ["v00 or static output missing"]}
    anc = {a.get("id"): a for a in v00.get("anchors", [])}
    s = static["V14"]
    checks = [("anchors_%s_in_definition" % "_".join(V14_SOURCE_ANCHORS),
               [anc.get(a, {}).get("verdict_mech") in ACCEPT_MECH for a in V14_SOURCE_ANCHORS], [True] * 4),
              ("lds_alias_line_present", len(s.get("lds_alias", [])), 1),
              ("link_option_uses_lds", len(s.get("target_link_options", [])), 1),
              ("no_c_definition_of_jiffies", len(s.get("active_c_definitions_of_jiffies", [])), 0),
              ("jiffies_64_defined_in_c", len(s.get("active_c_definitions_of_jiffies_64", [])), 1)]
    mism = [c for c in checks if c[1] != c[2]]
    return {"status": "VALID", "result": "OBSERVED_AS_PREDICTED" if not mism else "COUNTEREVIDENCE",
            "checks": [{"label": a, "observed": b, "predicted": c, "match": b == c} for a, b, c in checks],
            "elf_files_in_time_tree": s.get("elf_files_in_time_tree")}


def build_doc(run, v00, v01, static, a46, oldc, meta, frozen_results):
    errors = []
    doc = {}

    def section(name, fn):
        try:
            doc[name] = fn()
        except Exception as e:  # noqa: BLE001
            errors.append("%s: %s: %s" % (name, type(e).__name__, e))
            doc[name] = {"generator_error": True}
    run = run or {}
    ev = run.get("evaluation") or {}
    stg = run.get("stages") or {}
    done = lambda s: (stg.get(s) or {}).get("returncode") == 0 and not (stg.get(s) or {}).get("timed_out")
    # a file left behind by a stage that did not complete is not evidence
    v00 = v00 if done("v00_anchors.py") else None
    static = static if done("static_checks.py") else None
    a46 = a46 if done("a46_check.py") else None
    v00_valid = (ev.get("V00") or {}).get("status") == "VALID"
    section("run", lambda: {k: run.get(k) for k in ("status", "exit_code", "execution_complete", "verification_findings",
                                                    "infrastructure_errors", "dynamic_calls", "workdir", "meta_injection",
                                                    "stage_outputs_present", "stage_outputs_usable")})
    section("identity", lambda: {"gate": (run.get("identity") or {}).get("gate"), "error": (run.get("identity") or {}).get("error"),
                                 "pins": (run.get("identity") or {}).get("pins"),
                                 "execution": (run.get("identity") or {}).get("execution"),
                                 "originals": (run.get("identity") or {}).get("originals"),
                                 "technical_inputs_unchanged": (run.get("identity") or {}).get("technical_inputs_unchanged"),
                                 "core_review_fields": (run.get("identity") or {}).get("core_review_fields")})
    section("h00", lambda: {"dynamic_safety_ok": (run.get("h00") or {}).get("dynamic_safety_ok"),
                            "transport_ok": (run.get("h00") or {}).get("transport_ok"), "error": (run.get("h00") or {}).get("error"),
                            "probes": [{k: p.get(k) for k in ("probe", "as_expected", "gates_dynamic", "terminal_state", "kill",
                                                              "reaped_confirmed", "collect_timed_out", "output_lost", "error")}
                                       for p in (run.get("h00") or {}).get("probes", [])]})
    section("frozen_verifier", lambda: {"commit_short12": (run.get("frozen_manifest") or {}).get("commit_short12"),
                                        "names_match_expected": (run.get("frozen_manifest") or {}).get("names_match_expected"),
                                        "files": len((run.get("frozen_manifest") or {}).get("files", []))})
    section("stages", lambda: {k: {kk: v.get(kk) for kk in ("source", "returncode", "timed_out", "terminal_state", "status", "reason")}
                               for k, v in (run.get("stages") or {}).items()})

    def cases():
        v14s = v14_source(v00 if v00_valid else None, static)
        v14m = ev.get("V14_model") or {"status": "INCOMPLETE_EVIDENCE", "result": None}
        out = []
        for cid in ORDER:
            if cid == "V14":
                e = {"status": "VALID" if v14s.get("status") == "VALID" and v14m.get("status") == "VALID" else
                     (v14m.get("status") if v14m.get("status") != "VALID" else v14s.get("status")),
                     "result": None, "layers_detail": {"source": compact(v14s), "model": compact(v14m)}}
                if e["status"] == "VALID":
                    e["result"] = "OBSERVED_AS_PREDICTED" if (v14s["result"] == "OBSERVED_AS_PREDICTED" and
                                                             v14m["result"] == "OBSERVED_AS_PREDICTED") else "COUNTEREVIDENCE"
            else:
                e = compact(ev.get(cid) or {"status": "INCOMPLETE_EVIDENCE", "result": None, "evidence_reasons": ["no evaluation record"]})
            layers = {}
            for lname, src in LAYER_TEMPLATES[cid].items():
                if src is None:
                    layers[lname] = "NOT_RUN"
                elif src == "V14_source":
                    layers[lname] = "EXECUTED" if v14s.get("status") == "VALID" else v14s.get("status")
                elif src == "V14_model":
                    layers[lname] = "EXECUTED" if v14m.get("status") == "VALID" else v14m.get("status")
                elif src == "V08_function":
                    fsub = (((ev.get("V08") or {}).get("subresults") or {}).get("function_level_combinations") or {})
                    layers[lname] = "EXECUTED" if fsub.get("status") == "VALID" else (ev.get("V08") or {}).get("status", "INCOMPLETE_EVIDENCE")
                else:
                    layers[lname] = "EXECUTED" if (ev.get(src) or {}).get("status") == "VALID" else (ev.get(src) or {}).get("status", "INCOMPLETE_EVIDENCE")
            if cid == "V14" and isinstance(static, dict):
                elf = (static.get("V14") or {}).get("elf_files_in_time_tree")
                layers["existing_elf"] = "NOT_RUN"
                e["existing_elf_reason"] = ("no ELF file in the pinned time tree and none provided" if elf == [] else
                                            "ELF files present but not examined in this recheck")
            rec = {"case_id": cid, "evidence_status": e.get("status"), "result": e.get("result"), "layers": layers}
            rec.update({k: v for k, v in e.items() if k not in ("status", "result")})
            out.append(rec)
        a = compact(ev.get("V00_A46_correction") or {"status": "INCOMPLETE_EVIDENCE", "result": None})
        out.append({"case_id": "V00_A46_correction", "evidence_status": a.get("status"), "result": a.get("result"),
                    **{k: v for k, v in a.items() if k not in ("status", "result")},
                    "anchors": [[x.get("id"), x.get("verdict_mech"), x.get("contiguous_hits")] for x in (a46 or {}).get("anchors", [])]})
        return out
    section("cases", cases)

    def ca():
        res = {}
        by = {c["case_id"]: c for c in doc.get("cases", []) if isinstance(c, dict)}
        for ca_id, need in CA_RULES.items():
            obs = {k: [by.get(k, {}).get("evidence_status"), by.get(k, {}).get("result")] for k in need}
            missing = [k for k, v in obs.items() if v[0] != "VALID"]
            contra = [k for k, v in obs.items() if v[0] == "VALID" and v[1] != need[k]]
            ruling = "UNDETERMINED_MISSING_EVIDENCE" if missing else ("CONTRADICTED_IN_SCOPE" if contra else "SUPPORTED_IN_SCOPE")
            res[ca_id] = {"ruling": ruling, "criteria": {k: "evidence VALID and result == %s" % v for k, v in need.items()},
                          "observed": obs, "missing_or_invalid": missing, "contradicting": contra}
        v08 = by.get("V08", {})
        rb = v08.get("result_basis") or {}
        only_static_missing = (v08.get("evidence_status") == "INCOMPLETE_EVIDENCE"
                               and rb.get("limited_model_reachability") == "UNDETERMINED_STATIC_INPUT_MISSING")
        if v08.get("evidence_status") != "VALID" and not only_static_missing:
            r = "UNDETERMINED_MISSING_EVIDENCE"
        else:
            fl = "FUNCTION_LEVEL_SUPPORTED" if rb.get("function_level") == "OBSERVED_AS_PREDICTED" else "FUNCTION_LEVEL_CONTRADICTED"
            r = "%s; LIMITED_MODEL_REACHABILITY=%s; GLOBAL_REACHABILITY=%s" % (fl, rb.get("limited_model_reachability"),
                                                                               rb.get("global_reachability"))
        res["CA-02"] = {"ruling": r, "criteria": {"V08.function_level": "blocked current + empty queue returns current",
                                                  "V08.limited_model_reachability": "idle requeue sequence observed and static idle criteria all true",
                                                  "V08.global_reachability": "not decidable by this host check"},
                        "observed": {"V08": [v08.get("evidence_status"), v08.get("result")], "result_basis": rb}}
        v14 = by.get("V14", {})
        ld = v14.get("layers") or {}
        if v14.get("evidence_status") != "VALID":
            r = "UNDETERMINED_MISSING_EVIDENCE"
        elif v14.get("result") == "OBSERVED_AS_PREDICTED":
            r = "SUPPORTED_AT_%s" % "_AND_".join(sorted(k.upper() for k, s in ld.items() if s == "EXECUTED"))
        else:
            r = "CONTRADICTED_IN_SCOPE"
        res["CA-05"] = {"ruling": r, "criteria": {"V14": "source layer and host link model both as predicted"},
                        "observed": {"V14": [v14.get("evidence_status"), v14.get("result")], "layers": ld}}
        anc = {a.get("id"): a for a in (v00 or {}).get("anchors", [])} if isinstance(v00, dict) and v00_valid else {}
        if not anc:
            r, obs = "UNDETERMINED_MISSING_EVIDENCE", {}
        else:
            obs = {a: [anc.get(a, {}).get("verdict_mech"), (anc.get(a, {}).get("semantic") or {}).get("verdict")] for a in CA06_ANCHORS}
            a35 = " ".join(anc.get("A35", {}).get("pp_stack_at_quote") or [])
            obs["A35_under_CONFIG_BUG"] = "CONFIG_BUG" in a35
            ok = all(v[0] in ACCEPT_MECH and str(v[1]).startswith("SUPPORTS") for k, v in obs.items() if k in CA06_ANCHORS) \
                and obs["A35_under_CONFIG_BUG"]
            r = "SUPPORTED_AT_SOURCE_LEVEL" if ok else "CONTRADICTED_OR_UNSUPPORTED_AT_SOURCE_LEVEL"
        res["CA-06"] = {"ruling": r, "criteria": {"A01/A35/A36/A37": "mechanical match in definition and semantic review SUPPORTS*",
                                                  "A35": "definition inside #ifdef CONFIG_BUG"},
                        "observed": obs, "semantic_source": "v00_semantic_review.yaml reused unchanged from a1e7c2277705"}
        return dict(sorted(res.items()))
    section("ca_rulings", ca)

    def counts():
        cs = [c for c in doc.get("cases", []) if isinstance(c, dict) and c.get("case_id") in ORDER]
        by_status, by_result, layers_nr = {}, {}, []
        for c in cs:
            by_status[c["evidence_status"]] = by_status.get(c["evidence_status"], 0) + 1
            if c.get("result"):
                by_result[c["result"]] = by_result.get(c["result"], 0) + 1
            layers_nr += ["%s.%s" % (c["case_id"], k) for k, v in (c.get("layers") or {}).items() if v != "EXECUTED"]
        return {"top_level_cases": len(cs), "evidence_status": by_status, "results_for_valid_evidence": by_result,
                "layers_not_executed": layers_nr, "layers_not_executed_reason_template": NOT_RUN_REASON_TEMPLATE}
    section("counts", counts)

    def diff():
        if not isinstance(frozen_results, dict):
            return {"available": False}
        old = {c["case_id"]: c.get("result") for c in frozen_results.get("cases", [])}
        oca = {c["issue_id"]: c.get("ruling") for c in frozen_results.get("ca_rulings", [])}
        new = {c["case_id"]: c.get("result") for c in doc.get("cases", []) if isinstance(c, dict)}
        nca = {k: v.get("ruling") for k, v in (doc.get("ca_rulings") or {}).items() if isinstance(v, dict)}
        return {"source": "results.yaml @ %s" % H.PINS["core_results"],
                "case_results": {k: {"frozen": old.get(k), "recheck": new.get(k), "same": old.get(k) == new.get(k)} for k in ORDER},
                "ca_rulings": {k: {"frozen": oca.get(k), "recheck": nca.get(k), "same": oca.get(k) == nca.get(k)}
                               for k in sorted(set(oca) | set(nca))},
                "frozen_counts": frozen_results.get("counts")}
    section("diff_vs_frozen_core", diff)

    def r_items():
        o = oldc or {}
        a1 = (o.get("A1_R01_empty_observation") or {}).get("json") or {}
        a2 = {k: (v.get("json") or {}) for k, v in (o.get("A2_R02_gate_not_enforced") or {}).items()}
        a3 = (o.get("A3_R04_old_identity_and_readback_probe") or {}).get("json") or {}
        a5 = o.get("A5_R03_old_make_results") or {}
        return {
            "R01": {"old_v05_empty_return": (a1.get("v05_empty_return") or {}).get("verdict"),
                    "old_dispatch_on_empty_cases": a1.get("dispatch_evaluate_on_empty_cases"),
                    "handled_by_template": "evaluate2.validate_case contracts; results only for VALID evidence"},
            "R02": {"old_dynamic_entry_calls": {k: v.get("dynamic_entry_calls") for k, v in a2.items()},
                    "handled_by_template": "run_recheck identity gate before any dynamic entry; H00 gates fixtures"},
            "R03": {"old_generator": {k: {"returncode": v.get("returncode"), "generated": v.get("generated"),
                                          "V01": (v.get("case_results") or {}).get("V01")} for k, v in a5.items()},
                    "handled_by_template": "make_results2 computes every result/ruling from data"},
            "R04": {"old_identity_remote_heads_match": a3.get("old_identity_remote_heads_match"),
                    "old_readback_probe_as_expected": (a3.get("old_probe_readback") or {}).get("as_expected"),
                    "old_readback_cli": {k: v.get("returncode") for k, v in (o.get("A3_R04_old_readback_cli") or {}).items()},
                    "old_run_pg_group_gone": (o.get("A6_old_run_pg_group_gone") or {}).get("json"),
                    "handled_by_template": "identity2 separates frozen objects from the moving head; readback2 object mode and fixed CLI"}}
    section("r_items", r_items)
    section("m_items", lambda: {m["id"]: {"requirement": m.get("requirement"), "met": m.get("met"),
                                          "old_side": m.get("old_summary"), "new_side": m.get("new_summary")}
                                for m in (meta or {}).get("tests", [])} if meta else {"available": False})
    doc["generator_errors"] = errors
    return doc


def frozen_old_results():
    try:
        return yaml.safe_load(H.blob(H.PINS["core_results"], H.RESULTS_ROOT + "core/results.yaml"))
    except Exception:  # noqa: BLE001
        return None


def header(doc):
    run = doc.get("run") or {}
    return {
        "task_id": H.PACKET, "packet_id": H.PACKET, "followup_id": "CORE-CHECK-01-RECHECK-01", "phase": "core_recheck_01",
        "record_type": "verifier_recheck_results",
        "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
        "execution_model_selection": "unknown_or_not_attestable",
        "execution_model_selection_source": "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）",
        "execution_surface": "claude.ai/code 托管云端会话容器；普通用户态进程",
        "date": "2026-09-27",
        "base_snapshot": "kernel=time a039d9803ade；workspace=master de3bb1df906a；taskbook 57a7c3e0eebf；主线回执头 ec62453e76d2；冻结 core a1e7c2277705（均为短 SHA）",
        "status": "generated_from_run_status_%s" % run.get("status"),
        "acceptance_ceiling": "PASS_PENDING_LOCAL",
        "results_are_generated": True,
        "open_questions_template": ["idle 以非 RUNNING 状态被切出是否可能（CA-02 全局可达性）",
                                    "真实 ELF、时基、IRQ/SMP 与上下文切换仍未执行"],
    }


def render(doc):
    doc = json.loads(json.dumps(doc, ensure_ascii=False))
    text = yaml.safe_dump(doc, sort_keys=False, allow_unicode=True, width=1000)
    if re.search(r"[0-9a-fA-F]{40}", text):
        raise ValueError("40-hex in generated YAML")
    if yaml.safe_load(text) != doc:
        raise ValueError("YAML round trip mismatch")
    return text


def generate(obs_dir):
    b = {n: _load(obs_dir, n + ".json") for n in ("run", "v00", "v01", "static", "a46", "old_counterexamples", "meta_tests")}
    body = build_doc(b["run"], b["v00"], b["v01"], b["static"], b["a46"], b["old_counterexamples"], b["meta_tests"],
                     frozen_old_results())
    doc = header(body)
    doc.update(body)
    return doc


if __name__ == "__main__":
    d = generate(sys.argv[1])
    t = render(d)
    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write(t)
    print("wrote %s (%d bytes); generator_errors=%d" % (sys.argv[2], len(t.encode()), len(d.get("generator_errors", []))))
