# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-02 (consumer-fix-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: generate consumer-fix-02/results.yaml from this round's observations only (old_side_s01_s03,
#   consumer_tests, guard_start, frozen_b843_manifest_start, n06_revalidation.yaml). Every value below is
#   read from those files; fixed text is limited to keys ending in _template. Deterministic: the same
#   observations give the same bytes. Source-run facts and consumer facts are kept in separate keys.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 make_summary3.py <consumer-fix-02/observations> <out.yaml>"""
import json
import os
import sys

import yaml

import common3 as C


def load(d, n):
    p = os.path.join(d, n)
    with open(p, encoding="utf-8") as f:
        return yaml.safe_load(f) if n.endswith(".yaml") else json.load(f)


def st(x):
    r = (x or {}).get("returned") or {}
    return [r.get("status"), r.get("result")]


def generate(d):
    old, ct, gd, fm = (load(d, n) for n in ("old_side_s01_s03.json", "consumer_tests.json", "guard_start.json",
                                            "frozen_b843_manifest_start.json"))
    n06doc = load(d, "n06_revalidation.yaml")
    T = {t["id"]: t for t in ct["tests"]}
    s1, s2, s3 = old["S01"], old["S02"], old["S03"]
    doc = {
        "task_id": C.PACKET, "packet_id": C.PACKET, "followup_id": C.FOLLOWUP, "phase": "evidence_consumer_recheck",
        "record_type": "consumer_fix_results",
        "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
        "execution_model_selection": "unknown_or_not_attestable",
        "execution_surface": "claude.ai/code 托管云端会话容器；普通用户态 Python 数据层实验（每例独立进程，30 s 上限）",
        "date": "2026-10-01",
        "base_snapshot": "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 b0aa54db1b70（13 号任务书与 recheck-01 回执，开工时固定）；被审输入 b843d475367a；recheck-01 results 冻结于 efb9846b88ec（均为短 SHA）",
        "status": "generated_from_observations",
        "acceptance_ceiling": "PASS_PENDING_LOCAL",
        "results_are_generated": True,
        "scope_template": "data layer only: no C compile/run, no H00, no tree scan, no kernel change; recorded observations re-judged",
    }
    doc["input_identity"] = {
        "pins": gd.get("pins"), "guard_start_gate": gd.get("gate"), "review_fields": gd.get("review_fields"),
        "lead_changes_since_previous_pin": gd.get("lead_changes_since_previous_pin"), "efb_to_b843": gd.get("efb_to_b843"),
        "b843_files": gd.get("b843_files"),
        "frozen_b843_extraction": {"commit_short12": fm.get("commit_short12"), "fixture_names_match_expected": fm.get("fixture_names_match_expected"),
                                   "observation_names_match_expected": fm.get("observation_names_match_expected"),
                                   "all_unchanged_at_head": fm.get("all_unchanged_at_head"),
                                   "reused_modules": [[f["path"], f["bytes"], f["sha256_segments"][0]] for f in fm.get("files", [])
                                                      if f.get("used_by_consumer_fix_02")]},
    }
    doc["old_side"] = {
        "programs": old.get("frozen_programs"), "workdir": old.get("workdir"), "input_label": old.get("input_label"),
        "S01": {"case": s1["case"], "deleted_run_keys": s1["deleted_run_keys"], "lead_prediction": s1["lead_prediction"],
                "old_returned": st(s1["deleted_four_keys"]), "old_control": st(s1["control_unmodified"]), "reproduced": s1["reproduced"]},
        "S02": {"lead_prediction": s2["lead_prediction"], "old_returned": {k: st(v) for k, v in s2["variants"].items()},
                "old_control": st(s2["control_unmodified"]), "reproduced": s2["reproduced"]},
        "S03": {"lead_prediction": s3["lead_prediction"],
                "control_complete_copy": {"returncode": s3["control_complete_copy"]["returncode"],
                                          "report_identical_to_efb_results_yaml": s3["control_complete_copy"]["report_identical_to_efb_results_yaml"]},
                "static_omitted": {"returncode": s3["static_omitted"]["returncode"], "report_written": s3["static_omitted"]["report_written"],
                                   "report": s3["static_omitted"]["report"]},
                "static_truncated_json": {"returncode": s3["static_truncated_json"]["returncode"],
                                          "report_written": s3["static_truncated_json"]["report_written"],
                                          "stderr_last": s3["static_truncated_json"]["stderr_tail"][-1:]},
                "reproduced": s3["reproduced"]},
        "S04_found_this_round": ct.get("S04_valid_observation_equal_to_negcontrol_value"),
        "all_lead_derivations_reproduced": old.get("all_three_reproduced"),
        "counterevidence_to_lead": [] if old.get("all_three_reproduced") else ["see S01-S03 reproduced flags"],
    }
    tests = {}
    for tid in ("N01", "N02", "N03", "N04", "N05"):
        t = T.get(tid, {})
        rec = {"requirement": t.get("requirement"), "input_label": t.get("input_label"), "met": t.get("met")}
        if t.get("harness_error"):
            rec["harness_error"] = t["harness_error"]
        v = t.get("variants") or {}
        if tid in ("N01", "N02"):
            rec["variants"] = {k: {"old": x.get("old"), "new": x.get("new"), "new_categories": x.get("new_categories"), "met": x.get("met")}
                               for k, x in v.items()}
        elif tid in ("N03", "N04"):
            rec["checks"] = t.get("checks")
            rec["variants"] = {}
            for k, x in v.items():
                o, n = x.get("old") or {}, x.get("new") or {}
                nr = n.get("report") or {}
                rec["variants"][k] = {
                    "old_cli": {"returncode": o.get("returncode"), "report_written": o.get("report_written"),
                                "stderr_last": (o.get("stderr_tail") or [])[-1:],
                                "V08": ((o.get("report") or {}).get("V08") or {}).get("evidence_status"),
                                "V14": ((o.get("report") or {}).get("V14") or {}).get("evidence_status")} if o else None,
                    "new_cli": {"returncode": n.get("returncode"), "report_written": n.get("report_written"), "status": nr.get("status"),
                                "input_problems": nr.get("input_problems"), "not_valid": sorted(k2 for k2, s in (nr.get("cases") or {}).items() if s[0] != "VALID"),
                                "cached_differs": nr.get("cached_differs")}}
        else:
            rec["variants"] = {}
            for k, x in v.items():
                if k == "through_cli":
                    rec["through_cli"] = {kk: {"old_cli_returncode": (xx.get("old_cli") or {}).get("returncode"),
                                               "new_cli": {"returncode": (xx.get("new_cli") or {}).get("returncode"),
                                                           "status": (xx.get("new_cli") or {}).get("status"),
                                                           "not_valid": {c: s for c, s in ((xx.get("new_cli") or {}).get("cases") or {}).items() if s[0] != "VALID"}},
                                               "met": xx.get("met")} for kk, xx in x.items()}
                else:
                    rec["variants"][k] = {"old": x.get("old", x.get("old_all")), "new": x.get("new", x.get("new_all")), "met": x.get("met")}
        tests[tid] = rec
    doc["new_tests"] = tests
    n6 = T.get("N06", {})
    cv = n06doc.get("consumer_validation") or {}
    se = n06doc.get("source_execution") or {}
    doc["recorded_observation_revalidation"] = {
        "kind": n6.get("kind"), "met": n6.get("met"), "checks": n6.get("checks"),
        "source_execution_facts": {"run_status": (se.get("run") or {}).get("status"), "run_exit_code": (se.get("run") or {}).get("exit_code"),
                                   "identity_read_time_utc": se.get("identity_read_time_utc"), "execution_head_short12": se.get("execution_head_short12"),
                                   "note_template": "historical facts of the source run; nothing re-executed"},
        "consumer_facts": {"status": cv.get("status"), "date_utc": cv.get("consumer_date_utc"), "programs": cv.get("consumer_programs"),
                           "inputs": [[i["file"], i["state"], i["bytes"]] for i in cv.get("inputs", [])],
                           "dynamic_raw_observations_available": cv.get("dynamic_raw_observations_available"),
                           "cached_evaluation_differs": (cv.get("cached_evaluation_vs_consumer") or {}).get("differs")},
        "cases": {c["case_id"]: [c.get("evidence_status"), c.get("result")] for c in n06doc.get("cases", [])},
        "ca_rulings": {k: v.get("ruling") for k, v in (n06doc.get("ca_rulings") or {}).items()},
        "counts": n06doc.get("counts"),
        "diff_vs_recheck01_frozen_results": n06doc.get("diff_vs_recheck01_frozen_results"),
        "report_file": n6.get("report_file"),
        "open_template": ["CA-02 global reachability", "V08 requeue-order candidate (14 号 CORE-SCHED-ORDER-02 is not executed here)",
                          "real ELF / IRQ / SMP / context switch"],
    }
    doc["summary"] = {"S01_S03_reproduced_on_b843": old.get("all_three_reproduced"),
                      "N_met": {k: (T.get(k) or {}).get("met") for k in ("N01", "N02", "N03", "N04", "N05", "N06")},
                      "all_N_met": ct.get("all_met"),
                      "S04_old_new": {k: [v.get("old"), v.get("new")] for k, v in (ct.get("S04_valid_observation_equal_to_negcontrol_value") or {}).items()},
                      "consumer_tests_workdir": ct.get("workdir")}
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
