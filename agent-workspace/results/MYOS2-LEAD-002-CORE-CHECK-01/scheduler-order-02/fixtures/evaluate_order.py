# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: new file
# purpose: judge the two recorded runs of each scenario. Order of work:
#   1. record completeness (both runs: exit 0, no timeout, exited, reaped, empty stderr, identical
#      stdout; every required event and field present with its type) -> VALID / INCOMPLETE_EVIDENCE /
#      INVALID_EVIDENCE / ERROR; nothing below is judged without VALID evidence;
#   2. an independent value-list oracle computed from the declared inputs (SCENARIOS), not from the
#      original function's pointer walk or charging order: switch decision, selected task (queue head),
#      membership after the call, per-task vruntime deltas, last_jiffies, total time charged;
#   3. the candidate constraint P on the RECORDED queue: requeued non-idle tasks in non-decreasing
#      updated vruntime (P_nonidle) and idle at the tail (idle_tail), reported separately;
#   4. comparison with the lead's derivation in the 15 contract (kept as stated, not adjusted).
#   A complete record that violates P is a VALID witness, never COMPARATOR_INVALID.
# --------------------------------------------------------------------------------------------------
"""Evaluator for scheduler-order-02 (only the fields these eight scenarios need)."""
import copy
import json

IDLE = 0
RUNNING, UNINTERRUPTIBLE = 0, 2   # values of TASK_RUNNING / TASK_UNINTERRUPTIBLE as recorded by the fixture
BASE_Q = [[3, 10], [4, 20], [5, 30], [0, 0]]
TIME_SLICE = 100
LAST0 = 100


def _sc(a_vr, a_state, jiffies, resched, cur_idle=False, idle_vr=0, steps=1):
    q = [[3, 10], [4, 20], [5, 30]] if cur_idle else copy.deepcopy(BASE_Q)
    return {"jiffies": jiffies, "need_resched": resched, "last_jiffies": LAST0, "time_slice": TIME_SLICE,
            "current": IDLE if cur_idle else 2, "a_vr": a_vr, "a_state": a_state, "idle_vr": idle_vr,
            "queue": q, "steps": steps}


# declared inputs (15 contract section 4) and the lead's derivation for each scenario, as written there
SCENARIOS = {
    "W01": {"in": _sc(25, RUNNING, 100, 1), "lead": {"selected": [3], "final_order": [4, 5, 0, 2], "final_order_qualifier": "possible",
                                                     "used": [0], "P_nonidle": "VIOLATED"}},
    "W02": {"in": _sc(15, RUNNING, 110, 1), "lead": {"selected": [3], "final_order": [2, 4, 5, 0], "a_final": 25, "P_nonidle": "VIOLATED"}},
    "W03": {"in": _sc(15, RUNNING, 100, 1), "lead": {"selected": [3], "final_order": [2, 4, 5, 0], "a_final": 15, "P_nonidle": "HOLDS"}},
    "W04": {"in": _sc(15, RUNNING, 105, 1), "lead": {"selected": [3], "a_final": 20, "P_nonidle": "HOLDS"}},
    "W05": {"in": _sc(15, UNINTERRUPTIBLE, 110, 1), "lead": {"selected": [3], "a_requeued": False, "a_final": 25}},
    "W06": {"in": _sc(15, RUNNING, 110, 1, cur_idle=True, idle_vr=7), "lead": {"selected": [3], "final_order": [4, 5, 0], "idle_final": 7}},
    "W07": {"in": _sc(15, RUNNING, 110, 1, steps=2), "lead": {"step1_a_delta": 10, "step2_switched_out_delta": 0, "total_charged": 10}},
    "W08": {"in": _sc(15, RUNNING, 105, 0, steps=2), "lead": {"step1_unchanged": True, "step2_a_delta": 5, "total_charged": 5}},
}
SNAP_FIELDS = {"where": str, "step": int, "jiffies": int, "last_jiffies": int, "used_external": int, "need_resched": int,
               "current": dict, "rq_curr": int, "rq_idle": int, "tasks": list, "queue": list, "count": int, "walk_len": int,
               "walk_complete": bool, "unknown_link": bool, "duplicate_nodes": bool, "idle_occurrences": int, "current_occurrences": int}
CUR_FIELDS = {"id": int, "state": int, "vruntime": int, "time_slice": int}


def _t(v, t):
    return type(v) is t


# ---------------------------------------------------------------- 1. record completeness
def parse(stdout):
    evs, bad = [], []
    for n, ln in enumerate((stdout or "").splitlines(), 1):
        if not ln.strip():
            continue
        try:
            o = json.loads(ln)
        except ValueError:
            bad.append(n)
            continue
        if not isinstance(o, dict):
            bad.append(n)
            continue
        evs.append(o)
    return evs, bad


def completeness(name, rec):
    """(status, reasons, events of run 1)."""
    st, why = "VALID", []
    order = ["ERROR", "INVALID_EVIDENCE", "INCOMPLETE_EVIDENCE", "VALID"]

    def fail(s, r):
        nonlocal st
        why.append(r)
        if order.index(s) < order.index(st):
            st = s
    runs = (rec or {}).get("runs")
    if not isinstance(runs, list) or len(runs) != 2:
        fail("INCOMPLETE_EVIDENCE", "two runs are required, found %s" % (len(runs) if isinstance(runs, list) else None))
        return st, why, []
    for r in runs:
        tag = "run%s" % r.get("rep")
        for k, t in (("returncode", int), ("timed_out", bool), ("terminal_state", str), ("reaped_confirmed", bool),
                     ("collect_timed_out", bool), ("output_lost", bool), ("stdout", str), ("stderr", str)):
            if k not in r:
                fail("INCOMPLETE_EVIDENCE", "%s.%s missing" % (tag, k))
            elif not _t(r[k], t):
                fail("INVALID_EVIDENCE", "%s.%s wrong type" % (tag, k))
        if r.get("timed_out") is True or r.get("collect_timed_out") is True or r.get("output_lost") is True:
            fail("INVALID_EVIDENCE", "%s timed out or lost output" % tag)
        if r.get("terminal_state") not in (None, "exited") or r.get("reaped_confirmed") is False:
            fail("INVALID_EVIDENCE", "%s terminal_state=%r reaped=%r" % (tag, r.get("terminal_state"), r.get("reaped_confirmed")))
        if _t(r.get("returncode"), int) and r["returncode"] != 0:
            fail("INVALID_EVIDENCE", "%s exit code %d (42 = BUG_ON/guard stop)" % (tag, r["returncode"]))
        if r.get("stderr"):
            fail("INVALID_EVIDENCE", "%s stderr not empty" % tag)
    if all("stdout" in r for r in runs) and runs[0]["stdout"] != runs[1]["stdout"]:
        fail("INVALID_EVIDENCE", "the two runs differ in stdout")
    evs, bad = parse(runs[0].get("stdout"))
    if bad:
        fail("INVALID_EVIDENCE", "unparsable stdout lines %s" % bad)
    steps = SCENARIOS[name]["in"]["steps"]
    if [e.get("ev") for e in evs if e.get("ev") == "BUG_ON"]:
        fail("INVALID_EVIDENCE", "BUG_ON stop recorded")
    want = ["scenario"]
    for k in range(1, steps + 1):
        if k > 1:
            want.append("sequence")
        want += ["snap:before:%d" % k, "call:%d" % k, "snap:after:%d" % k]
    want.append("end")
    got = []
    for e in evs:
        if e.get("case") != name:
            fail("INVALID_EVIDENCE", "event of another case")
        k = e.get("ev")
        got.append("snap:%s:%s" % (e.get("where"), e.get("step")) if k == "snap" else "call:%s" % e.get("step") if k == "call" else k)
    missing = [w for w in want if w not in got]
    extra = [g for g in got if g not in want]
    if missing:
        fail("INCOMPLETE_EVIDENCE", "missing events %s" % missing)
    if extra or (not missing and got != want):
        fail("INVALID_EVIDENCE", "unexpected or reordered events %s" % (extra or got))
    for e in evs:
        if e.get("ev") == "snap":
            for f, t in SNAP_FIELDS.items():
                if f not in e:
                    fail("INCOMPLETE_EVIDENCE", "snap %s/%s missing %s" % (e.get("where"), e.get("step"), f))
                elif not _t(e[f], t):
                    fail("INVALID_EVIDENCE", "snap %s/%s field %s wrong type" % (e.get("where"), e.get("step"), f))
            for f, t in CUR_FIELDS.items():
                if not _t((e.get("current") or {}).get(f), t):
                    fail("INCOMPLETE_EVIDENCE", "snap current.%s missing/wrong" % f)
        if e.get("ev") == "call" and not (_t(e.get("returned"), int) and _t(e.get("rq_curr"), int)):
            fail("INCOMPLETE_EVIDENCE", "call %s missing returned/rq_curr" % e.get("step"))
    end = [e for e in evs if e.get("ev") == "end"]
    if end and end[0].get("steps") != steps:
        fail("INVALID_EVIDENCE", "end.steps %r, expected %d" % (end[0].get("steps"), steps))
    return st, why, evs


# ---------------------------------------------------------------- 2. independent value-list oracle
def initial_state(inp):
    vr = {0: inp["idle_vr"], 2: inp["a_vr"], 3: 10, 4: 20, 5: 30}
    state = {0: RUNNING, 2: inp["a_state"], 3: RUNNING, 4: RUNNING, 5: RUNNING}
    return {"vr": vr, "state": state, "queue": [q[0] for q in inp["queue"]], "current": inp["current"],
            "jiffies": inp["jiffies"], "last": inp["last_jiffies"], "need_resched": inp["need_resched"], "time_slice": inp["time_slice"]}


def oracle_call(s):
    """Expected boundary effects of one call from the plain values in s (no pointer walk)."""
    used = s["jiffies"] - s["last"]
    cur = s["current"]
    switch = (bool(s["need_resched"]) or cur == IDLE or used >= s["time_slice"] or s["state"][cur] != RUNNING) and len(s["queue"]) > 0
    n = copy.deepcopy(s)
    out = {"used": used, "switch": switch, "returned": cur, "delta": {k: 0 for k in s["vr"]}}
    if switch:
        sel = s["queue"][0]
        rest = s["queue"][1:]
        requeued = s["state"][cur] == RUNNING
        if cur != IDLE:
            n["vr"][cur] += used
            out["delta"][cur] = used
        n["queue_members"] = sorted(rest + ([cur] if requeued else []))
        n["last"] = s["jiffies"]
        out.update(returned=sel, requeued=requeued, members_after=n["queue_members"],
                   p_order=sorted([t for t in n["queue_members"] if t != IDLE], key=lambda t: n["vr"][t])
                   + ([IDLE] if IDLE in n["queue_members"] else []))
    else:
        n["queue_members"] = sorted(s["queue"])
        out.update(members_after=n["queue_members"], requeued=None, p_order=None)
    out["vr_after"] = n["vr"]
    out["last_after"] = n["last"]
    return out, n


def p_check(queue):
    """P on a recorded queue [[id, vr], ...]: non-idle keys non-decreasing; idle (if queued) last."""
    keys = [vr for tid, vr in queue if tid != IDLE]
    pairs = [[a, b] for a, b in zip(keys, keys[1:]) if a > b]
    nonidle = "HOLDS" if not pairs else "VIOLATED"
    ids = [tid for tid, _ in queue]
    tail = "NOT_APPLICABLE" if IDLE not in ids else ("HOLDS" if ids[-1] == IDLE else "VIOLATED")
    return {"P_nonidle": nonidle, "descending_pairs": pairs, "idle_tail": tail}


def evaluate(name, rec):
    sc = SCENARIOS[name]
    st, why, evs = completeness(name, rec)
    res = {"scenario": name, "evidence_status": st, "evidence_reasons": why}
    if st != "VALID":
        res["note"] = "no structure/accounting/ordering verdict without VALID evidence"
        return res
    snaps = {(e["where"], e["step"]): e for e in evs if e.get("ev") == "snap"}
    calls = {e["step"]: e for e in evs if e.get("ev") == "call"}
    inp = sc["in"]
    s = initial_state(inp)
    b1 = snaps[("before", 1)]
    fid = {"jiffies": b1["jiffies"] == inp["jiffies"], "last_jiffies": b1["last_jiffies"] == inp["last_jiffies"],
           "need_resched": b1["need_resched"] == inp["need_resched"], "current": b1["current"]["id"] == inp["current"],
           "current_state": b1["current"]["state"] == s["state"][inp["current"]],
           "queue": b1["queue"] == [[t, s["vr"][t]] for t in s["queue"]],
           "time_slice": b1["current"]["time_slice"] == inp["time_slice"],
           "task_vruntimes": {t[0]: t[1] for t in b1["tasks"]} == s["vr"]}
    res["input_fidelity"] = fid
    steps, total = [], 0
    for k in range(1, inp["steps"] + 1):
        b, a, c = snaps[("before", k)], snaps[("after", k)], calls[k]
        if k > 1:  # sequence model: returned task becomes current, need_resched set to 1, clock unchanged
            s["current"] = prev["returned"]
            s["need_resched"] = 1
            s["queue"] = [t for t, _ in b["queue"]]   # order as recorded; the oracle only uses its head and members
        exp, nxt = oracle_call(s)
        rec_vr_b = {t[0]: t[1] for t in b["tasks"]}
        rec_vr_a = {t[0]: t[1] for t in a["tasks"]}
        delta = {t: rec_vr_a[t] - rec_vr_b[t] for t in rec_vr_a}
        qa = a["queue"]
        ids_a = [t for t, _ in qa]
        structure = {"count_equals_walk": a["count"] == a["walk_len"], "walk_complete": a["walk_complete"] and not a["unknown_link"],
                     "no_duplicate_nodes": not a["duplicate_nodes"],
                     "idle_occurrences_ok": a["idle_occurrences"] == (1 if IDLE in exp["members_after"] else 0),
                     "returned_not_queued": c["returned"] not in ids_a}
        accounting = {"before_state_matches_oracle": rec_vr_b == s["vr"] and b["last_jiffies"] == s["last"],
                      "used_external": b["used_external"], "used_oracle": exp["used"], "switch_oracle": exp["switch"],
                      "deltas_recorded": {str(t): d for t, d in delta.items() if d}, "deltas_oracle": {str(t): d for t, d in exp["delta"].items() if d},
                      "deltas_match": delta == exp["delta"], "last_jiffies_after": a["last_jiffies"], "last_jiffies_oracle": exp["last_after"],
                      "last_jiffies_match": a["last_jiffies"] == exp["last_after"]}
        qb_nonidle = [[t, v] for t, v in b["queue"] if t != IDLE]
        selection = {"returned": c["returned"], "rq_curr": c["rq_curr"], "oracle": exp["returned"],
                     "match": c["returned"] == exp["returned"] and c["rq_curr"] == exp["returned"],
                     # observation only (not part of P): is the selected head the smallest non-idle key queued before?
                     "selected_key_is_queue_minimum": (None if not exp["switch"] or c["returned"] == IDLE or not qb_nonidle
                                                       else rec_vr_b.get(c["returned"]) == min(v for _, v in qb_nonidle)),
                     "queue_minimum_before": min(qb_nonidle, key=lambda x: x[1]) if qb_nonidle else None}
        membership = {"recorded": sorted(ids_a), "oracle": exp["members_after"], "match": sorted(ids_a) == exp["members_after"]}
        steps.append({"step": k, "queue_before": b["queue"], "queue_after": qa, "current_before": b["current"],
                      "structure": structure, "structure_ok": all(structure.values()), "accounting": accounting,
                      "accounting_ok": accounting["deltas_match"] and accounting["last_jiffies_match"] and accounting["before_state_matches_oracle"],
                      "selection": selection, "membership": membership, "P": p_check(qa),
                      "P_conformant_order_oracle": exp["p_order"], "requeued_current": exp["requeued"]})
        total += sum(delta.values())
        prev = exp
        s = nxt
        s["queue"] = ids_a
    res["steps"] = steps
    res["total_time_charged"] = total
    res["all_structure_ok"] = all(x["structure_ok"] for x in steps)
    res["all_accounting_ok"] = all(x["accounting_ok"] for x in steps)
    res["all_selection_ok"] = all(x["selection"]["match"] for x in steps)
    res["all_membership_ok"] = all(x["membership"]["match"] for x in steps)
    res["P_final"] = steps[-1]["P"]
    res["P_by_step"] = {str(x["step"]): [x["P"]["P_nonidle"], x["P"]["idle_tail"]] for x in steps}
    res["P_violated_at_any_step"] = any(x["P"]["P_nonidle"] == "VIOLATED" or x["P"]["idle_tail"] == "VIOLATED" for x in steps)
    res["lead_derivation"] = compare_lead(name, sc["lead"], steps, total)
    return res


# ---------------------------------------------------------------- 4. lead derivation, as stated
def compare_lead(name, lead, steps, total):
    out = {}
    last = steps[-1]
    final_ids = [t for t, _ in last["queue_after"]]
    if "selected" in lead:
        out["selected"] = [[st["selection"]["returned"] for st in steps][:len(lead["selected"])], lead["selected"]]
    if "final_order" in lead:
        out["final_order"] = [final_ids, lead["final_order"]] + ([lead.get("final_order_qualifier")] if lead.get("final_order_qualifier") else [])
    if "used" in lead:
        out["used"] = [[st["accounting"]["used_external"] for st in steps], lead["used"]]
    if "P_nonidle" in lead:
        out["P_nonidle"] = [last["P"]["P_nonidle"], lead["P_nonidle"]]
    for key in ("a_final", "idle_final"):
        if key in lead:
            out[key] = [None, lead[key]]   # recorded value filled by finalize_lead from the last snapshot
    if "a_requeued" in lead:
        out["a_requeued"] = [2 in final_ids, lead["a_requeued"]]
    if "step1_a_delta" in lead:
        out["step1_a_delta"] = [steps[0]["accounting"]["deltas_recorded"].get("2", 0), lead["step1_a_delta"]]
    if "step2_switched_out_delta" in lead:
        cur2 = steps[1]["current_before"]["id"]
        out["step2_switched_out_delta"] = [steps[1]["accounting"]["deltas_recorded"].get(str(cur2), 0), lead["step2_switched_out_delta"]]
    if "step2_a_delta" in lead:
        out["step2_a_delta"] = [steps[1]["accounting"]["deltas_recorded"].get("2", 0), lead["step2_a_delta"]]
    if "step1_unchanged" in lead:
        s1 = steps[0]
        out["step1_unchanged"] = [s1["queue_before"] == s1["queue_after"] and not s1["accounting"]["deltas_recorded"]
                                  and s1["accounting"]["last_jiffies_after"] == LAST0, lead["step1_unchanged"]]
    if "total_charged" in lead:
        out["total_charged"] = [total, lead["total_charged"]]
    return out


def finalize_lead(res, evs_after_vr):
    """Fill a_final / idle_final from the recorded final task vruntimes."""
    ld = res.get("lead_derivation") or {}
    for key, tid in (("a_final", 2), ("idle_final", 0)):
        if key in ld:
            ld[key][0] = evs_after_vr.get(tid)
    for v in ld.values():
        v.append("AGREES" if v[0] == v[1] else "DIFFERS")
    res["lead_derivation_all_agree"] = all(v[-1] == "AGREES" for v in ld.values())
    return res


def judge(name, rec):
    res = evaluate(name, rec)
    if res["evidence_status"] != "VALID":
        return res
    evs, _ = parse(rec["runs"][0]["stdout"])
    last_after = [e for e in evs if e.get("ev") == "snap" and e.get("where") == "after"][-1]
    return finalize_lead(res, {t[0]: t[1] for t in last_after["tasks"]})
