# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: generate scheduler-order-02/results.yaml from observations/ only (runs.json, controls.json,
#   guard_start.json, frozen_0d62_manifest.json). Every per-scenario value is computed here by
#   evaluate_order.judge from the two saved runs; fixed text is limited to keys ending in _template.
#   Same observations -> same bytes.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 make_results4.py <observations dir> <out.yaml>"""
import json
import os
import sys

import yaml

import common4 as C
import evaluate_order as V

FIDELITY_TEMPLATE = [
    "host user-space process; pick_next_task_myos and the list primitives are verbatim from time a039d9803ade",
    "task_struct/runqueue reduced to the members the function touches (type choice of core/fixtures/fx_sched.c @ 0d62)",
    "current, need_resched() and jiffies are explicit fixture inputs, not a clock, an interrupt or preemption",
    "W07/W08: making the returned task current is a sequence model, not a context switch",
    "single CPU, no concurrent modification, time_slice=100, last_jiffies initial 100 set once per scenario",
    "observers only before/after each call; no probe inside the original function; queue walk by address, max 16 links",
]


def load(d, n):
    with open(os.path.join(d, n), encoding="utf-8") as f:
        return json.load(f)


def run_brief(r):
    return {"rep": r.get("rep"), "returncode": r.get("returncode"), "timed_out": r.get("timed_out"), "terminal_state": r.get("terminal_state"),
            "reaped_confirmed": r.get("reaped_confirmed"), "stdout_bytes": len((r.get("stdout") or "").encode()),
            "stdout_sha256_0": C.sha_segments((r.get("stdout") or "").encode())[0], "stderr_bytes": len((r.get("stderr") or "").encode())}


def generate(d):
    runs, ctl, gs, fz = (load(d, n) for n in ("runs.json", "controls.json", "guard_start.json", "frozen_0d62_manifest.json"))
    doc = {"task_id": C.PACKET, "packet_id": C.PACKET, "followup_id": C.FOLLOWUP, "phase": "scheduler_order_witness",
           "record_type": "scheduler_order_results",
           "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
           "execution_model_selection": "unknown_or_not_attestable",
           "execution_surface": "claude.ai/code 托管云端会话容器；普通用户态宿主进程（不是 MyOS2 运行）",
           "date": "2026-10-02",
           "base_snapshot": "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 6706013a079a（15 号任务书与 RECHECK-02 收口回执，开工时固定）；冻结执行头 0d62c4d19711（均为短 SHA）",
           "status": "generated_from_observations", "acceptance_ceiling": "PASS_PENDING_LOCAL", "results_are_generated": True,
           "kernel_correctness_verdict_template": "not issued: these are bounded witnesses of one function's boundary behaviour"}
    ext = runs.get("extraction") or []
    pick = [x for x in ext if x.get("name") == "pick_next_task_myos"]
    doc["input_identity"] = {
        "guard_start_gate": gs.get("gate"), "review_fields": gs.get("review_fields"), "contract_fields": gs.get("contract_fields"),
        "lead_changes_since_previous_pin": gs.get("lead_changes_since_previous_pin"), "pins": gs.get("pins"),
        "frozen_0d62_reused": [[f["path"], f["bytes"], f["sha256_segments"][0], f["role"]] for f in fz.get("files", [])],
        "template": runs.get("template"), "expanded_source": runs.get("expanded_source"),
        "pick_next_task_myos": pick[0] if pick else None,
        "orig_slices": [[x["path"], x["kind"], x["name"], x["lines"], x["bytes"], x["sha256_segments"][0]] for x in ext],
        "compile": {k: (runs.get("compile") or {}).get(k) for k in ("returncode", "timed_out", "terminal_state", "binary_exists")},
        "compile_stderr_bytes": len(((runs.get("compile") or {}).get("stderr") or "").encode()),
        "cflags": runs.get("cflags"), "limits": runs.get("limits"), "tools": runs.get("tools"), "build_status": runs.get("status"),
    }
    doc["fidelity_template"] = FIDELITY_TEMPLATE
    scen, summary_p = {}, {}
    for name in V.SCENARIOS:
        rec = (runs.get("scenarios") or {}).get(name)
        if rec is None:
            scen[name] = {"completed": False, "not_executed_reason": "no run record (build status %s)" % runs.get("status")}
            continue
        j = V.judge(name, rec)
        s = {"completed": j["evidence_status"] == "VALID", "evidence_status": j["evidence_status"], "evidence_reasons": j["evidence_reasons"],
             "declared_inputs": {k: v for k, v in V.SCENARIOS[name]["in"].items()},
             "runs": [run_brief(r) for r in rec.get("runs", [])], "stdout_identical": rec.get("stdout_identical")}
        if j["evidence_status"] == "VALID":
            s["input_fidelity"] = j["input_fidelity"]
            s["steps"] = [{"step": x["step"], "current_before": x["current_before"], "queue_before": x["queue_before"],
                           "returned": x["selection"]["returned"], "rq_curr": x["selection"]["rq_curr"], "queue_after": x["queue_after"],
                           "used": x["accounting"]["used_external"], "switch_oracle": x["accounting"]["switch_oracle"],
                           "deltas_recorded": x["accounting"]["deltas_recorded"], "deltas_oracle": x["accounting"]["deltas_oracle"],
                           "last_jiffies_after": x["accounting"]["last_jiffies_after"],
                           "structure": x["structure"], "structure_ok": x["structure_ok"], "accounting_ok": x["accounting_ok"],
                           "selection_matches_oracle": x["selection"]["match"], "membership_matches_oracle": x["membership"]["match"],
                           "selected_key_is_queue_minimum": x["selection"]["selected_key_is_queue_minimum"],
                           "queue_minimum_before": x["selection"]["queue_minimum_before"],
                           "P": x["P"], "P_conformant_order_oracle": x["P_conformant_order_oracle"]} for x in j["steps"]]
            s["total_time_charged"] = j["total_time_charged"]
            s["checks"] = {"structure": j["all_structure_ok"], "accounting": j["all_accounting_ok"], "selection": j["all_selection_ok"],
                           "membership": j["all_membership_ok"], "input_fidelity": all(j["input_fidelity"].values())}
            s["P_by_step"] = j["P_by_step"]
            s["candidate_P"] = "VIOLATED" if j["P_violated_at_any_step"] else "SATISFIED"
            s["lead_derivation"] = j["lead_derivation"]
            s["lead_derivation_all_agree"] = j["lead_derivation_all_agree"]
            summary_p[name] = s["candidate_P"]
        scen[name] = s
    doc["scenarios"] = scen
    w2 = scen.get("W02", {}).get("steps", [{}])[0] if scen.get("W02", {}).get("completed") else {}
    post_removal_head = (w2.get("queue_before") or [None, None])[1] if w2 else None
    doc["cursor_fix_alone_basis"] = {
        "W02_current_key_at_insertion": (w2.get("current_before") or {}).get("vruntime") if w2 else None,
        "W02_head_after_removal": post_removal_head,
        "W02_insert_position_recorded": [t for t, _ in (w2.get("queue_after") or [])].index(2) if w2 and 2 in [t for t, _ in w2.get("queue_after", [])] else None,
        "W02_charged_after": (w2.get("deltas_recorded") or {}).get("2") if w2 else None,
        "reading_template": ("the current task's key before charging (15) is not greater than the head after removal (C, 20), so the "
                             "insertion loop is not entered and the task is placed first; the 10 jiffies are added afterwards. A change "
                             "confined to the loop (moving the compared entity with the cursor) cannot alter this case; it only "
                             "concerns W01, where the loop runs past D and idle to the anchor. Source reading of myos_rt.c lines "
                             "37-50 plus the recorded boundary values; no modified function was run."),
        "loop_condition_order_note_template": ("INFERRED from the source only, not executed: the loop tests the vruntime comparison "
                                               "before 'tmp_list != anchor'. Today the compared entity stays at the first node, so W01 "
                                               "reaches the anchor without an invalid read. A cursor change that re-derives the compared "
                                               "entity from tmp_list each step, without reordering that condition, would evaluate the "
                                               "anchor's container once the walk reaches the anchor."),
    }
    allv = {n: s for n, s in scen.items() if s.get("completed")}
    doc["controls"] = {"input": ctl.get("input"), "results": {k: {"transformation": v["transformation"],
                                                                 "evidence_status": v["result"]["evidence_status"],
                                                                 "P_nonidle": (v["result"].get("P_final") or {}).get("P_nonidle"),
                                                                 "lead_P_nonidle": (v["result"].get("lead_derivation") or {}).get("P_nonidle"),
                                                                 "met": v["met"]} for k, v in ctl.get("controls", {}).items()},
                       "all_met": ctl.get("all_met")}
    doc["summary"] = {
        "scenarios_completed": sorted(allv), "scenarios_not_completed": sorted(n for n in V.SCENARIOS if n not in allv),
        "candidate_P": summary_p,
        "P_violations_by_step": {n: {k: v for k, v in s["P_by_step"].items() if "VIOLATED" in v} for n, s in allv.items()
                                 if any("VIOLATED" in v for v in s["P_by_step"].values())},
        "all_structure_ok": all(s["checks"]["structure"] for s in allv.values()),
        "all_accounting_matches_oracle": all(s["checks"]["accounting"] for s in allv.values()),
        "all_selection_matches_oracle": all(s["checks"]["selection"] for s in allv.values()),
        "lead_derivations_all_agree": all(s["lead_derivation_all_agree"] for s in allv.values()),
        "lead_derivations_differing": {n: {k: v for k, v in s["lead_derivation"].items() if v[-1] != "AGREES"} for n, s in allv.items()
                                       if not s["lead_derivation_all_agree"]},
        "selected_task_not_queue_minimum": [[n, x["step"], x["returned"], x["queue_minimum_before"]] for n, s in allv.items()
                                            for x in s["steps"] if x["selected_key_is_queue_minimum"] is False],
        "controls_all_met": ctl.get("all_met"),
    }
    doc["open_template"] = ["global reachability of these states in a running MyOS2 (CA-02) is not established",
                            "real IRQ / SMP / context switch / clock not executed",
                            "P is the candidate constraint of the 15 contract, not a policy chosen by the Owner",
                            "no kernel change or candidate patch was produced"]
    return doc


def render(doc):
    text = yaml.safe_dump(json.loads(json.dumps(doc, ensure_ascii=False)), sort_keys=False, allow_unicode=True, width=1000)
    if C.HEX40.search(text):
        raise ValueError("40-hex in generated YAML")
    return text


if __name__ == "__main__":
    t = render(generate(sys.argv[1]))
    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write(t)
    print("wrote %s (%d bytes)" % (sys.argv[2], len(t.encode())))
