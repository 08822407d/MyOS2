# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: the three light consumer controls of the 15 contract section 5, on in-memory copies of the
#   real W03 record (runs.json is read, never written):
#     C1 complete W03 sample            -> VALID, judged normally;
#     C2 one required snapshot removed  -> INCOMPLETE_EVIDENCE, no ordering verdict ("no failure" refused);
#     C3 complete but different         -> VALID and P violated; a counter-witness to the lead's
#                                          derivation, not COMPARATOR_INVALID or ERROR.
#   C2/C3 inputs are HARNESS_META_TEST transformations, not MyOS2 output.
# --------------------------------------------------------------------------------------------------
"""Usage: python3 controls4.py <runs.json> <out.json>"""
import copy
import json
import sys

import common4 as C
import evaluate_order as V

META = "HARNESS_META_TEST"


def brief(r):
    return {"evidence_status": r["evidence_status"], "evidence_reasons": r.get("evidence_reasons"),
            "P_final": r.get("P_final"), "lead_derivation": r.get("lead_derivation"),
            "lead_derivation_all_agree": r.get("lead_derivation_all_agree")}


def main(runs_path, out):
    runs = json.load(open(runs_path, encoding="utf-8"))
    base = runs["scenarios"]["W03"]
    res = {"check": "SCHED_ORDER_02_CONTROLS", "input": "runs.json scenarios.W03 (real record; copies only)", "controls": {}}
    c1 = V.judge("W03", copy.deepcopy(base))
    res["controls"]["C1_complete_W03"] = {"transformation": None, "result": brief(c1),
                                          "met": c1["evidence_status"] == "VALID" and c1["P_final"]["P_nonidle"] == "HOLDS"}
    c2 = copy.deepcopy(base)
    for r in c2["runs"]:
        lines = r["stdout"].splitlines(keepends=True)
        r["stdout"] = "".join(x for x in lines if not ('"ev":"snap"' in x and '"where":"after"' in x))
    j2 = V.judge("W03", c2)
    res["controls"]["C2_after_snapshot_removed"] = {
        "transformation": META + ": the snap/after/1 line removed from both runs' stdout", "result": brief(j2),
        "met": j2["evidence_status"] == "INCOMPLETE_EVIDENCE" and "P_final" not in j2}
    c3 = copy.deepcopy(base)
    for r in c3["runs"]:
        out_lines = []
        for x in r["stdout"].splitlines(keepends=True):
            if '"ev":"snap"' in x and '"where":"after"' in x:
                e = json.loads(x)
                e["queue"] = [[4, 20], [2, 15], [5, 30], [0, 0]]   # A placed after C; members, count and keys unchanged
                x = json.dumps(e, separators=(",", ":")) + "\n"
            out_lines.append(x)
        r["stdout"] = "".join(out_lines)
    j3 = V.judge("W03", c3)
    res["controls"]["C3_complete_but_different"] = {
        "transformation": META + ": in both runs the snap/after/1 queue reordered to C20,A15,D30,I (same members, count, keys)",
        "result": brief(j3),
        "met": j3["evidence_status"] == "VALID" and j3["P_final"]["P_nonidle"] == "VIOLATED"
               and j3["lead_derivation"]["P_nonidle"][-1] == "DIFFERS"}
    res["all_met"] = all(v["met"] for v in res["controls"].values())
    C.emit(res, out)
    print(C.dump({k: v["met"] for k, v in res["controls"].items()}))
    return 0 if res["all_met"] else 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:3]))
