# ---- provenance (metadata comment) --------------------------------------------------------------
# packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-CHECK-01-RECHECK-01
# executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
# source: revised from core/fixtures/evaluate.py @ a1e7c2277705 (frozen original unchanged)
# change (R01): evidence first, behaviour second. Every case has a fixed contract (exit code class,
#   exact event sequence, field types); the run record must be complete (RAN, no timeout, no collection
#   timeout, confirmed reap, empty stderr, identical repeat). Only VALID evidence is compared with the
#   unchanged 07/09 predictions. Empty, partial, duplicated, conflicting, unparsable or failed runs give
#   NOT_RUN / BLOCKED / ERROR / INCOMPLETE_EVIDENCE / INVALID_EVIDENCE and never a behaviour verdict.
# --------------------------------------------------------------------------------------------------
"""Evidence contracts and case evaluators for the recheck."""
import json

STATUS_ORDER = ["ERROR", "BLOCKED", "INVALID_EVIDENCE", "INCOMPLETE_EVIDENCE", "NOT_RUN", "VALID"]
V05_LABELS = ["stopped_via_wake_up_process", "uninterruptible_mask_interruptible", "task_new_via_wake_up_process",
              "running_not_queued_via_wake_up_process", "current_uninterruptible_mask_interruptible"]
MAX = 9223372036854775807

# ---------------------------------------------------------------- field types
I, B, S, L, BN = "int", "bool", "str", "list", "bool|null"
FIELDS = {
    "layout": {"sizeof_lock": I, "offsetof_hdr_task_list_hdr": I, "offsetof_waiter_task_list": I,
               "anchor_container_task_field_aliases_lock": B},
    "snap": {"step": S, "count": I, "walk_len": I, "count_equals_nodes": B, "anchor_self": B, "header_is_empty": B,
             "a_self": BN, "a_occ": I, "b_self": BN, "b_occ": I, "lock_word": I, "preempt": I},
    "ttwu": {"call": I, "task": I, "state_before": I, "mask": I},
    "ttwu_invalid_task": {"call": I, "task_ptr_value": I, "watched_lock_word": I, "equals_watched_lock_word": B},
    "end": {"ttwu_calls": I},
    "second_wake_begin": {"lock_held": B},
    "second_complete_begin": {"lock_held_inside": B},
    "st": {"in": I, "ret": I, "sched_calls": I, "mod_timer_calls": I, "del_calls": I, "state_after": I},
    "schedule": {"call": I, "current": I, "current_state": I},
    "stu": {"in": I, "ret": I, "sched_calls": I, "state_after": I},
    "msleep_begin": {"msecs": I, "cap": I},
    "step_cap": {"where": S, "cap": I},
    "wfc_common": {"ret_is_max": B, "done_after": I, "sched_calls": I, "count": I, "state_after": I},
    "wait_for_completion": {"done_after": I, "sched_calls": I, "count": I},
    "notifier_start": {"waiter_state": I, "done": I, "count": I, "walk_len": I},
    "notifier_end": {"waiter_state": I, "done": I, "count": I, "walk_len": I},
    "returned": {"ret_is_max": B, "done_after": I, "sched_calls": I, "count": I, "walk_len": I, "state_after": I},
    "begin": {"timeout": I, "cap": I},
    "waiter_returned": {"done": I, "count": I, "walk_len": I, "anchor_self": B, "header_is_empty": B, "state": I,
                        "sched_calls": I, "ttwu_calls": I},
    "reuse_complete_begin": {"note": S},
    "q": {"step": S, "cpu": I, "count": I, "walk_len": I, "order": L, "task": I, "task_state": I, "task_occ": I,
          "task_cpu_meta": I, "preempt": I},
    "ret": {"fn": S, "ret": I},
    "mask_case": {"label": S, "state_in": I, "mask": I, "state_in_mask": B, "ret": I},
    "rets": {"r1": I, "r2": I, "r3": I},
    "pick_in": {"name": S, "current": I, "current_state": I, "current_is_idle": B, "need_resched": I,
                "order_before": L, "count_before": I},
    "pick_out": {"name": S, "returned": I, "returned_is_current": B, "rq_curr": I, "order_after": L,
                 "count_after": I, "walk_len_after": I, "idle_in_list": B},
    "vec": {"init": I, "i": I, "after": I, "ret": B, "contract_after_add": I, "contract_ret": B, "sub_after": I,
            "sub_ret": B, "after_matches_contract": B, "ret_matches_contract": B, "after_matches_sub": B},
    "lock": {"step": S, "ret": I, "val": I, "head": I, "tail": I, "is_locked": I},
    "one_handler_call": {"same_address": B, "jiffies_64_delta": I, "jiffies_delta": I, "sizeof_jiffies": I,
                         "sizeof_jiffies_64": I, "tty_calls": I},
}
EXTRA_FIELDS = {("step_cap", "__mod_timer"): {"first_timeouts": L, "last_timeout": I, "sched_calls": I, "current_state": I}}


def key_of(e):
    """Event identity used for sequence/duplicate checks."""
    ev = e.get("ev")
    if ev in ("snap", "q", "lock"):
        return (ev, e.get("step"))
    if ev in ("ttwu", "schedule"):
        return (ev, e.get("call"))
    if ev == "st":
        return (ev, e.get("in"))
    if ev == "mask_case":
        return (ev, e.get("label"))
    if ev in ("pick_in", "pick_out"):
        return (ev, e.get("name"))
    if ev == "vec":
        return (ev, (e.get("init"), e.get("i")))
    return (ev, None)


def _q(step):
    return ("q", step)


def _pick(*names):
    return [x for n in names for x in (("pick_in", n), ("pick_out", n))]


V02_PREFIX = [("layout", None), ("snap", "init"), ("snap", "after_prepare"), ("ttwu", 1), ("snap", "after_wake"),
              ("snap", "after_finish")]
CONTRACTS = {
    ("fx_wait", "v02_single"): (0, V02_PREFIX + [("end", None)]),
    ("fx_wait", "v03_second_wake_direct"): (42, V02_PREFIX + [("second_wake_begin", None), ("ttwu_invalid_task", None)]),
    ("fx_wait", "v03_second_wake_via_complete"): (42, [("ttwu", 1), ("snap", "after_first_complete"),
                                                      ("snap", "after_waiter_finish"), ("second_complete_begin", None),
                                                      ("ttwu_invalid_task", None)]),
    ("fx_wait", "v03_all_two_waiters"): (42, [("snap", "after_two_prepare"), ("ttwu", 1), ("ttwu", 2),
                                             ("ttwu_invalid_task", None)]),
    ("fx_wait", "v09_schedule_timeout_values"): (0, [("st", 0), ("st", 1), ("st", 5), ("st", -1), ("schedule", 1),
                                                    ("st", MAX)]),
    ("fx_wait", "v09_uninterruptible_wrapper"): (0, [("stu", None)]),
    ("fx_wait", "v09_msleep_bounded"): (43, [("msleep_begin", None), ("step_cap", None)]),
    ("fx_wait", "v10_done_preset_fast_path"): (0, [("wfc_common", None), ("wait_for_completion", None)]),
    ("fx_wait", "v10_infinite_notify_during_schedule"): (0, [("schedule", 1), ("notifier_start", None), ("ttwu", 1),
                                                            ("notifier_end", None), ("returned", None)]),
    ("fx_wait", "v10_finite_timeout_no_notifier"): (43, [("begin", None), ("step_cap", None)]),
    ("fx_wait", "v11_wait_then_notify_then_reuse"): (42, [("schedule", 1), ("notifier_start", None), ("ttwu", 1),
                                                         ("notifier_end", None), ("waiter_returned", None),
                                                         ("reuse_complete_begin", None), ("ttwu_invalid_task", None)]),
    ("fx_sched", "v04_noncurrent_wake"): (0, [_q("before"), ("ret", None), _q("after")]),
    ("fx_sched", "v05_state_not_in_mask"): (0, [x for lab in V05_LABELS for x in
                                               (("mask_case", lab), _q("current_path" if lab.startswith("current_") else lab))]),
    ("fx_sched", "v06_double_wake"): (0, [_q("after_first"), _q("after_second"), _q("after_third"), ("rets", None)]),
    ("fx_sched", "v07_cpu_metadata"): (0, [_q("set_task_cpu_A_to_1_rq1"), ("ret", None), _q("wake_B_meta2_rq0"),
                                          _q("wake_B_meta2_rq2"), _q("new_task_C_meta3_rq0"), _q("new_task_C_meta3_rq3")]),
    ("fx_sched", "v08_pick_combinations"): (0, _pick("a_running_empty", "b_blocked_empty", "c_blocked_one_runnable",
                                                     "d_idle_one_runnable", "e_blocked_queue_only_idle")),
    ("fx_sched", "v08_sequence_idle_requeue"): (0, _pick("s1_idle_switches_out", "s2_first_blocks", "s3_second_blocks")),
    ("fx_sched", "v08_sequence_idle_switched_out_blocked"): (0, _pick("t1_blocked_idle_switches_out", "t2_only_task_blocks")),
    ("fx_sched", "v08_vruntime_requeue_order"): (0, _pick("f_running_requeue_by_vruntime")),
    ("fx_prims", "v12_add_test_negative"): (0, [("vec", v) for v in ((-1, 1), (1, 2), (2, -1), (0, 0), (5, 3))]),
    ("fx_prims", "v13_trylock"): (0, [("lock", s) for s in ("after_init", "after_first_trylock",
                                                          "after_second_trylock_without_unlock", "after_arch_spin_lock",
                                                          "trylock_while_held_by_arch_spin_lock", "after_arch_spin_unlock")]),
    ("fx_jiffies", "v14"): (0, [("one_handler_call", None)]),
    ("fx_jiffies_control", "v14_control"): (0, [("one_handler_call", None)]),
}
STOP_EVENT = {42: "ttwu_invalid_task", 43: "step_cap"}


def _type_ok(v, t):
    if t == I:
        return isinstance(v, int) and not isinstance(v, bool)
    if t == B:
        return isinstance(v, bool)
    if t == BN:
        return v is None or isinstance(v, bool)
    if t == S:
        return isinstance(v, str)
    if t == L:
        return isinstance(v, list)
    return False


def _norm_key(k):
    ev, x = k
    return (ev, tuple(x) if isinstance(x, list) else x)


def parse_stdout(stdout):
    """Every non-empty stdout line must be a JSON object; others are reported, not dropped."""
    events, bad = [], []
    for n, ln in enumerate((stdout or "").splitlines(), 1):
        s = ln.strip()
        if not s:
            continue
        try:
            obj = json.loads(s)
        except ValueError:
            bad.append({"line": n, "reason": "not JSON"})
            continue
        if not isinstance(obj, dict):
            bad.append({"line": n, "reason": "not an object"})
            continue
        events.append(obj)
    return events, bad


def validate_case(fx_rec, fixture, case):
    """Return {"status", "reasons", "index"}; index maps event key -> event only when VALID."""
    out = {"fixture": fixture, "case": case, "status": None, "reasons": [], "index": None}

    def fail(status, reason):
        out["reasons"].append(reason)
        if out["status"] is None or STATUS_ORDER.index(status) < STATUS_ORDER.index(out["status"]):
            out["status"] = status
    if (fixture, case) not in CONTRACTS:
        fail("ERROR", "no contract for case")
        return out
    exp_rc, exp_seq = CONTRACTS[(fixture, case)]
    if not isinstance(fx_rec, dict):
        fail("INCOMPLETE_EVIDENCE", "fixture record missing")
        return out
    fst = fx_rec.get("status")
    if fst is None:
        fail("INCOMPLETE_EVIDENCE", "fixture status missing")
    elif fst == "NOT_RUN":
        fail("NOT_RUN", "fixture not run: %s" % fx_rec.get("reason", ""))
        return out
    elif str(fst).startswith("BLOCKED"):
        fail("BLOCKED", "fixture %s" % fst)
        return out
    elif fst != "BUILT":
        fail("ERROR", "fixture status %s" % fst)
        return out
    c = (fx_rec.get("cases") or {}).get(case)
    if not isinstance(c, dict):
        fail("INCOMPLETE_EVIDENCE", "case record missing")
        return out
    cst = c.get("status")
    if cst == "ERROR":
        fail("ERROR", "case error: %s" % c.get("reason"))
        return out
    if cst == "BLOCKED":
        fail("BLOCKED", "case blocked: %s" % c.get("reason"))
        return out
    if cst != "RAN":
        fail("INCOMPLETE_EVIDENCE", "case status %r" % cst)
        return out
    run = c.get("run")
    if not isinstance(run, dict):
        fail("INCOMPLETE_EVIDENCE", "run record missing")
        return out
    if run.get("timed_out") is not False:
        fail("INVALID_EVIDENCE", "run timed_out=%r" % run.get("timed_out"))
    if run.get("collect_timed_out") is True:
        fail("INVALID_EVIDENCE", "output collection timed out")
    if run.get("terminal_state", "exited") != "exited":
        fail("INVALID_EVIDENCE", "terminal_state=%r" % run.get("terminal_state"))
    if run.get("reaped_confirmed", True) is not True:
        fail("INVALID_EVIDENCE", "reap not confirmed")
    if run.get("stderr"):
        fail("INVALID_EVIDENCE", "stderr not empty")
    if c.get("repeat_identical") is not True:
        fail("INVALID_EVIDENCE", "repeat run missing or not identical")
    if c.get("repeat_returncode") != run.get("returncode"):
        fail("INVALID_EVIDENCE", "repeat exit code differs")
    events, bad = parse_stdout(run.get("stdout"))
    for b in bad:
        fail("INVALID_EVIDENCE", "stdout line %d %s" % (b["line"], b["reason"]))
    obs = []
    for e in events:
        if e.get("case") != case or not isinstance(e.get("ev"), str):
            fail("INVALID_EVIDENCE", "event without matching case/ev: %s" % json.dumps(e, ensure_ascii=False)[:120])
            continue
        spec = dict(FIELDS.get(e["ev"], {}))
        if e["ev"] not in FIELDS:
            fail("INVALID_EVIDENCE", "unknown event type %s" % e["ev"])
        spec.update(EXTRA_FIELDS.get((e["ev"], e.get("where")), {}))
        for f, t in spec.items():
            if f not in e:
                fail("INCOMPLETE_EVIDENCE", "%s missing field %s" % (e["ev"], f))
            elif not _type_ok(e[f], t):
                fail("INVALID_EVIDENCE", "%s field %s has type %s, expected %s" % (e["ev"], f, type(e[f]).__name__, t))
        obs.append(e)
    okeys = [_norm_key(key_of(e)) for e in obs]
    ekeys = [_norm_key(k) for k in exp_seq]
    dups = sorted({repr(k) for k in okeys if okeys.count(k) > 1})
    if dups:
        fail("INVALID_EVIDENCE", "duplicate or conflicting events: %s" % ", ".join(dups))
    missing = [repr(k) for k in ekeys if k not in okeys]
    extra = [repr(k) for k in okeys if k not in ekeys]
    if missing:
        fail("INCOMPLETE_EVIDENCE", "missing events: %s" % ", ".join(missing))
    if extra:
        fail("INVALID_EVIDENCE", "unexpected events: %s" % ", ".join(extra))
    if not missing and not extra and not dups and okeys != ekeys:
        fail("INVALID_EVIDENCE", "event order differs from case contract")
    rc = run.get("returncode")
    if rc != exp_rc:
        fail("INVALID_EVIDENCE", "exit code %r inconsistent with case contract %d" % (rc, exp_rc))
    stop = STOP_EVENT.get(exp_rc)
    if stop and (not obs or obs[-1].get("ev") != stop):
        fail("INVALID_EVIDENCE" if obs else "INCOMPLETE_EVIDENCE", "controlled stop %d without final %s event" % (exp_rc, stop))
    if exp_rc == 0 and any(e.get("ev") in STOP_EVENT.values() for e in obs):
        fail("INVALID_EVIDENCE", "stop event present in a case whose contract ends normally")
    if out["status"] is None:
        out["status"] = "VALID"
        out["index"] = {_norm_key(key_of(e)): e for e in obs}
    return out


# ---------------------------------------------------------------- judging
def judge(checks, wrong):
    mism = [c for c in checks if c[1] != c[2]]
    wrong_mism = [c for c in wrong if c[1] != c[2]]
    if not checks:
        return {"status": "ERROR", "result": None, "reason": "no checks evaluated", "checks": []}
    if not wrong_mism:
        return {"status": "ERROR", "result": None, "reason": "COMPARATOR_INVALID: wrong prediction not detected",
                "checks": [{"label": a, "observed": b, "predicted": c, "match": b == c} for a, b, c in checks]}
    return {"status": "VALID", "result": "OBSERVED_AS_PREDICTED" if not mism else "COUNTEREVIDENCE",
            "checks": [{"label": a, "observed": b, "predicted": c, "match": b == c} for a, b, c in checks],
            "negcontrol_wrong_prediction_detected": True,
            "negcontrol": [{"label": a, "observed": b, "wrong_prediction": c} for a, b, c in wrong]}


def gather(fx_records, needs):
    """needs: list of (fixture, case). Returns (status, per-case validation map, reasons)."""
    per = {}
    for fixture, case in needs:
        per[case] = validate_case((fx_records or {}).get(fixture), fixture, case)
    worst = min((v["status"] for v in per.values()), key=STATUS_ORDER.index) if per else "INCOMPLETE_EVIDENCE"
    reasons = ["%s: %s" % (k, "; ".join(v["reasons"])) for k, v in per.items() if v["reasons"]]
    return worst, per, reasons


def _wrap(case_id, fx_records, needs, body):
    st, per, reasons = gather(fx_records, needs)
    res = {"case_id": case_id, "constituents": {k: v["status"] for k, v in per.items()}}
    if st != "VALID":
        res.update({"status": st, "result": None, "evidence_reasons": reasons})
        return res
    idx = {k: v["index"] for k, v in per.items()}
    try:
        j = body(idx)
    except Exception as e:  # noqa: BLE001 - evaluator bug is an infrastructure error, not a finding
        res.update({"status": "ERROR", "result": None, "evidence_reasons": ["evaluator exception %s: %s" % (type(e).__name__, e)]})
        return res
    res.update(j)
    return res


def g(ix, case, key):
    return ix[case].get(_norm_key(key))


def v02(fx):
    def body(ix):
        p, k, f = (g(ix, "v02_single", ("snap", s)) for s in ("after_prepare", "after_wake", "after_finish"))
        return judge([("prepare.count", p["count"], 1), ("prepare.walk_len", p["walk_len"], 1),
                      ("wake.links_empty(anchor_self)", k["anchor_self"], True), ("wake.count", k["count"], 1),
                      ("wake.waiter_self_linked", k["a_self"], True), ("finish.count", f["count"], 1),
                      ("finish.header_is_empty", f["header_is_empty"], False)],
                     [("finish.count", f["count"], 0)])
    return _wrap("V02", fx, [("fx_wait", "v02_single")], body)


def v03(fx):
    cases = ["v03_second_wake_direct", "v03_second_wake_via_complete", "v03_all_two_waiters"]

    def body(ix):
        a = g(ix, cases[0], ("ttwu_invalid_task", None))
        lay = g(ix, cases[0], ("layout", None))
        b = g(ix, cases[1], ("ttwu_invalid_task", None))
        t1, t2 = g(ix, cases[2], ("ttwu", 1)), g(ix, cases[2], ("ttwu", 2))
        c = g(ix, cases[2], ("ttwu_invalid_task", None))
        return judge([("direct.invalid_at_call", a["call"], 2), ("direct.task_field_is_lock_word", a["equals_watched_lock_word"], True),
                      ("layout.anchor_container_task_aliases_lock", lay["anchor_container_task_field_aliases_lock"], True),
                      ("via_complete.task_field_is_lock_word", b["equals_watched_lock_word"], True),
                      ("via_complete.task_ptr_nonzero_while_lock_held", b["task_ptr_value"] != 0, True),
                      ("all_two.valid_wakeups_in_order", [t1["task"], t2["task"]], [1, 2]),
                      ("all_two.third_iteration_invalid", c["call"], 3)],
                     [("all_two.valid_wakeups_in_order", [t1["task"], t2["task"]], [1, 2, 1])])
    return _wrap("V03", fx, [("fx_wait", c) for c in cases], body)


def v04(fx):
    def body(ix):
        a, r = g(ix, "v04_noncurrent_wake", _q("after")), g(ix, "v04_noncurrent_wake", ("ret", None))
        return judge([("ret", r["ret"], 0), ("state_after(RUNNING)", a["task_state"], 0), ("queued_once", a["task_occ"], 1),
                      ("rq0.count", a["count"], 1), ("preempt_balanced", a["preempt"], 0)], [("ret", r["ret"], 1)])
    return _wrap("V04", fx, [("fx_sched", "v04_noncurrent_wake")], body)


def v05(fx):
    """Accepts the old call shape too: v05({"cases": {...}}) is treated as the fx_sched record."""
    if isinstance(fx, dict) and "cases" in fx and "fx_sched" not in fx:
        fx = {"fx_sched": dict(fx, status=fx.get("status", "BUILT"))}

    def body(ix):
        checks = []
        for lab in V05_LABELS:
            m = g(ix, "v05_state_not_in_mask", ("mask_case", lab))
            q = g(ix, "v05_state_not_in_mask", _q("current_path" if lab.startswith("current_") else lab))
            checks += [("%s.state_in_mask" % lab, m["state_in_mask"], False), ("%s.state_after(RUNNING)" % lab, q["task_state"], 0),
                       ("%s.queued" % lab, q["task_occ"], 0 if lab.startswith("current_") else 1), ("%s.ret" % lab, m["ret"], 0)]
        return judge(checks, [(checks[1][0], checks[1][1], 4)])
    return _wrap("V05", fx, [("fx_sched", "v05_state_not_in_mask")], body)


def v06(fx):
    def body(ix):
        qs = {s: g(ix, "v06_double_wake", _q(s)) for s in ("after_first", "after_second", "after_third")}
        r = g(ix, "v06_double_wake", ("rets", None))
        checks = [("%s.occurrences" % k, v["task_occ"], 1) for k, v in qs.items()] + \
                 [("%s.count" % k, v["count"], 1) for k, v in qs.items()] + [("returns", [r["r1"], r["r2"], r["r3"]], [0, 0, 0])]
        j = judge(checks, [("after_second.occurrences", qs["after_second"]["task_occ"], 2)])
        if j.get("result") == "OBSERVED_AS_PREDICTED":
            j["result"] = "NO_FAILURE_IN_SCOPE"
        return j
    return _wrap("V06", fx, [("fx_sched", "v06_double_wake")], body)


def v07(fx):
    def body(ix):
        q = lambda s: g(ix, "v07_cpu_metadata", _q(s))
        checks = [("set_task_cpu(A,1).queued_on_rq1", q("set_task_cpu_A_to_1_rq1")["task_occ"], 1),
                  ("set_task_cpu(A,1).task_cpu_meta_unchanged(3)", q("set_task_cpu_A_to_1_rq1")["task_cpu_meta"], 3),
                  ("wake(B meta=2).queued_on_rq0", q("wake_B_meta2_rq0")["task_occ"], 1),
                  ("wake(B meta=2).not_on_rq2", q("wake_B_meta2_rq2")["task_occ"], 0),
                  ("new_task(C meta=3).queued_on_rq0", q("new_task_C_meta3_rq0")["task_occ"], 1),
                  ("new_task(C meta=3).task_cpu_meta_unchanged(3)", q("new_task_C_meta3_rq0")["task_cpu_meta"], 3)]
        return judge(checks, [(checks[1][0], checks[1][1], 1)])
    return _wrap("V07", fx, [("fx_sched", "v07_cpu_metadata")], body)


def v08(fx, static=None):
    names = ["v08_pick_combinations", "v08_sequence_idle_requeue", "v08_sequence_idle_switched_out_blocked",
             "v08_vruntime_requeue_order"]

    def body(ix):
        po = lambda c, n: g(ix, c, ("pick_out", n))
        combos = judge([("a_running_empty.returns_current", po(names[0], "a_running_empty")["returned_is_current"], True),
                        ("b_blocked_empty.returns_current", po(names[0], "b_blocked_empty")["returned_is_current"], True),
                        ("c_blocked_one_runnable.returns_other", po(names[0], "c_blocked_one_runnable")["returned"], 3),
                        ("c.blocked_current_not_requeued(count 0)", po(names[0], "c_blocked_one_runnable")["count_after"], 0),
                        ("d_idle.requeued_at_tail", po(names[0], "d_idle_one_runnable")["order_after"], [0]),
                        ("e_blocked_only_idle.returns_idle", po(names[0], "e_blocked_queue_only_idle")["returned"], 0)],
                       [("b_blocked_empty.returns_current", po(names[0], "b_blocked_empty")["returned_is_current"], False)])
        seq = judge([("s1.idle_in_list_after_switch_out", po(names[1], "s1_idle_switches_out")["idle_in_list"], True),
                     ("s2.idle_still_in_list", po(names[1], "s2_first_blocks")["idle_in_list"], True),
                     ("s3.blocked_last_task_yields_idle", po(names[1], "s3_second_blocks")["returned"], 0)],
                    [("s3.blocked_last_task_yields_idle", po(names[1], "s3_second_blocks")["returned"], 2)])
        brk = judge([("t1.idle_not_requeued", po(names[2], "t1_blocked_idle_switches_out")["idle_in_list"], False),
                     ("t2.blocked_current_returned_with_empty_queue", po(names[2], "t2_only_task_blocks")["returned_is_current"], True)],
                    [("t2.blocked_current_returned_with_empty_queue", po(names[2], "t2_only_task_blocks")["returned_is_current"], False)])
        f = po(names[3], "f_running_requeue_by_vruntime")
        # static criteria for the limited-model reachability argument (from static_checks.py output)
        st = (static or {}).get("V08") if isinstance(static, dict) else None
        crit = None
        if isinstance(st, dict) and all(k in st for k in ("active_idle_state_writers", "rest_init_state_changes",
                                                          "active_rq_idle_assignments", "init_task_state_initializer")):
            crit = {"no_active_idle_state_writer": len(st["active_idle_state_writers"]) == 0,
                    "no_explicit_state_change_in_rest_init": len(st["rest_init_state_changes"]) == 0,
                    "single_rq_idle_assignment": len(st["active_rq_idle_assignments"]) == 1,
                    "init_task_starts_running": any("TASK_RUNNING" in h.get("text", "") for h in st["init_task_state_initializer"])}
        subs = {"function_level_combinations": combos, "sequence_with_idle_requeue": seq,
                "sequence_when_idle_switched_out_blocked": brk}
        if any(s["status"] != "VALID" for s in subs.values()):
            return {"status": "ERROR", "result": None, "subresults": subs, "evidence_reasons": ["sub-comparison invalid"]}
        fl = combos["result"]
        if crit is None:
            reach = "UNDETERMINED_STATIC_INPUT_MISSING"
        elif seq["result"] == "OBSERVED_AS_PREDICTED" and brk["result"] == "OBSERVED_AS_PREDICTED" and all(crit.values()):
            reach = "COUNTEREVIDENCE_IN_LIMITED_MODEL"
        else:
            reach = "NO_COUNTEREVIDENCE_FOUND"
        top = "COUNTEREVIDENCE" if (fl == "COUNTEREVIDENCE" or reach == "COUNTEREVIDENCE_IN_LIMITED_MODEL") else fl
        return {"status": "VALID", "result": top,
                "result_basis": {"function_level": fl, "limited_model_reachability": reach,
                                 "global_reachability": "NOT_ESTABLISHED_BY_THIS_CHECK", "static_criteria": crit},
                "subresults": subs,
                "extra_observation_requeue_order": {"order_before": g(ix, names[3], ("pick_in", "f_running_requeue_by_vruntime"))["order_before"],
                                                    "order_after": f["order_after"], "vruntime_sorted_would_be": [4, 2, 5]}}
    return _wrap("V08", fx, [("fx_sched", n) for n in names], body)


def v09(fx, static=None):
    cases = ["v09_schedule_timeout_values", "v09_uninterruptible_wrapper", "v09_msleep_bounded"]

    def body(ix):
        st = {t: g(ix, cases[0], ("st", t)) for t in (0, 1, 5, -1, MAX)}
        checks = []
        for t in (0, 1, 5):
            checks += [("in=%d.ret" % t, st[t]["ret"], t), ("in=%d.schedule_calls" % t, st[t]["sched_calls"], 0),
                       ("in=%d.state_after(UNINTERRUPTIBLE=2)" % t, st[t]["state_after"], 2)]
        checks += [("in=-1.ret", st[-1]["ret"], 0), ("in=-1.state_after(RUNNING)", st[-1]["state_after"], 0),
                   ("in=MAX.schedule_calls", st[MAX]["sched_calls"], 1), ("in=MAX.ret", st[MAX]["ret"], MAX)]
        u = g(ix, cases[1], ("stu", None))
        checks += [("wrapper(5).ret", u["ret"], 5), ("wrapper(5).state_after", u["state_after"], 2)]
        cap = g(ix, cases[2], ("step_cap", None))
        checks += [("msleep(5).stopped_at_step_cap", cap["cap"], 100), ("msleep.first_timeouts", cap["first_timeouts"], [5, 5, 5]),
                   ("msleep.last_timeout", cap["last_timeout"], 5), ("msleep.schedule_calls", cap["sched_calls"], 0)]
        return judge(checks, [("in=5.ret", st[5]["ret"], 4)])
    return _wrap("V09", fx, [("fx_wait", c) for c in cases], body)


def v10(fx):
    cases = ["v10_done_preset_fast_path", "v10_infinite_notify_during_schedule", "v10_finite_timeout_no_notifier"]

    def body(ix):
        a, b = g(ix, cases[0], ("wfc_common", None)), g(ix, cases[0], ("wait_for_completion", None))
        r = g(ix, cases[1], ("returned", None))
        cap = g(ix, cases[2], ("step_cap", None))
        j = judge([("preset.schedule_calls", a["sched_calls"], 0), ("preset.done_consumed", a["done_after"], 0),
                   ("preset.ret_is_MAX", a["ret_is_max"], True), ("preset.wait_for_completion.schedule_calls", b["sched_calls"], 0),
                   ("infinite.schedule_calls", r["sched_calls"], 1), ("infinite.done_consumed", r["done_after"], 0),
                   ("infinite.state_after(RUNNING)", r["state_after"], 0),
                   ("finite(no notifier).bounded_non_progress", cap["cap"], 100), ("finite.schedule_calls", cap["sched_calls"], 0)],
                  [("preset.schedule_calls", a["sched_calls"], 1)])
        j["observations_beyond_prediction"] = {"infinite.header_count_after_return": r["count"], "infinite.walk_len_after_return": r["walk_len"]}
        return j
    return _wrap("V10", fx, [("fx_wait", c) for c in cases], body)


def v11(fx):
    c = "v11_wait_then_notify_then_reuse"

    def body(ix):
        s, ns, ne = g(ix, c, ("schedule", 1)), g(ix, c, ("notifier_start", None)), g(ix, c, ("notifier_end", None))
        wr, inv = g(ix, c, ("waiter_returned", None)), g(ix, c, ("ttwu_invalid_task", None))
        j = judge([("waiter_state_at_schedule(UNINTERRUPTIBLE)", s["current_state"], 2),
                   ("queued_before_notify.count", ns["count"], 1), ("queued_before_notify.walk_len", ns["walk_len"], 1),
                   ("after_notify.waiter_state(RUNNING)", ne["waiter_state"], 0), ("after_notify.done", ne["done"], 1),
                   ("after_notify.count(stays 1)", ne["count"], 1), ("after_notify.walk_len", ne["walk_len"], 0),
                   ("after_return.count(stays 1)", wr["count"], 1), ("after_return.done", wr["done"], 0),
                   ("reuse.complete_hits_anchor_container", inv["equals_watched_lock_word"], True)],
                  [("after_return.count(stays 1)", wr["count"], 0)])
        j["layers"] = {"local_sequence_with_scripted_interleaving": "EXECUTED", "real_context_switch": "NOT_RUN"}
        return j
    return _wrap("V11", fx, [("fx_wait", c)], body)


def v12(fx):
    def body(ix):
        checks = []
        for k in ((-1, 1), (1, 2), (2, -1)):
            x = g(ix, "v12_add_test_negative", ("vec", k))
            checks += [("(%d,%d).after_is_subtraction" % k, x["after"], k[0] - k[1]),
                       ("(%d,%d).ret_is_sign_of_subtraction" % k, x["ret"], (k[0] - k[1]) < 0)]
        j = judge(checks, [("(-1,1).after_is_addition", g(ix, "v12_add_test_negative", ("vec", (-1, 1)))["after"], 0)])
        j["contract_mismatch_vectors"] = [list(k[1]) for k, x in ix["v12_add_test_negative"].items()
                                          if not (x["after_matches_contract"] and x["ret_matches_contract"])]
        return j
    return _wrap("V12", fx, [("fx_prims", "v12_add_test_negative")], body)


def v13(fx):
    def body(ix):
        l = lambda s: g(ix, "v13_trylock", ("lock", s))
        return judge([("init.val", l("after_init")["val"], 0), ("first_trylock.ret(success)", l("after_first_trylock")["ret"], 1),
                      ("first_trylock.val_unchanged", l("after_first_trylock")["val"], 0),
                      ("first_trylock.is_locked_after", l("after_first_trylock")["is_locked"], 0),
                      ("second_trylock_without_unlock.ret(success again)", l("after_second_trylock_without_unlock")["ret"], 1),
                      ("control.trylock_while_held_by_arch_spin_lock.ret", l("trylock_while_held_by_arch_spin_lock")["ret"], 0)],
                     [("second_trylock_without_unlock.ret(success again)", l("after_second_trylock_without_unlock")["ret"], 0)])
    return _wrap("V13", fx, [("fx_prims", "v13_trylock")], body)


def v14_model(fx):
    def body(ix):
        a, b = g(ix, "v14", ("one_handler_call", None)), g(ix, "v14_control", ("one_handler_call", None))
        return judge([("alias.same_address", a["same_address"], True), ("alias.jiffies_64_delta_per_handler", a["jiffies_64_delta"], 2),
                      ("control.same_address", b["same_address"], False), ("control.jiffies_64_delta_per_handler", b["jiffies_64_delta"], 1)],
                     [("alias.jiffies_64_delta_per_handler", a["jiffies_64_delta"], 1)])
    return _wrap("V14_model", fx, [("fx_jiffies", "v14"), ("fx_jiffies_control", "v14_control")], body)


DYNAMIC = [("V02", v02), ("V03", v03), ("V04", v04), ("V05", v05), ("V06", v06), ("V07", v07), ("V09", v09),
           ("V10", v10), ("V11", v11), ("V12", v12), ("V13", v13)]


def evaluate_all(fx, static=None):
    out = {}
    for vid, fn in DYNAMIC:
        out[vid] = fn(fx)
    out["V08"] = v08(fx, static)
    out["V14_model"] = v14_model(fx)
    return out


# ---------------------------------------------------------------- read-only stages
ACCEPT_MECH = {"MATCH_IN_DEFINITION", "MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE"}


def v00(v00j, stage_rec=None):
    res = {"case_id": "V00"}
    if stage_rec is not None and (stage_rec.get("returncode") != 0 or stage_rec.get("timed_out")):
        return dict(res, status="ERROR", result=None, evidence_reasons=["stage exit %r timed_out %r" % (stage_rec.get("returncode"), stage_rec.get("timed_out"))])
    if not isinstance(v00j, dict) or not isinstance(v00j.get("anchors"), list):
        return dict(res, status="INCOMPLETE_EVIDENCE", result=None, evidence_reasons=["v00 output missing or malformed"])
    anc = v00j["anchors"]
    need = ("id", "verdict_mech", "contiguous_hits", "length_1_to_5", "path_exists_in_time")
    bad = [a.get("id") for a in anc if not all(k in a for k in need)]
    if bad or len(anc) != v00j.get("tag_count_regex"):
        return dict(res, status="INCOMPLETE_EVIDENCE", result=None,
                    evidence_reasons=["anchor records incomplete: %s; count %d vs tags %r" % (bad, len(anc), v00j.get("tag_count_regex"))])
    failing = [{"id": a["id"], "verdict_mech": a["verdict_mech"]} for a in anc if a["verdict_mech"] not in ACCEPT_MECH]
    checks = [("tag_count", v00j.get("tag_count_regex"), 47), ("raw_tag_count", v00j.get("tag_count_raw_substring"), 47),
              ("ids_unique", v00j.get("ids_unique"), True), ("ids_missing", v00j.get("ids_missing_from_A01_A47"), []),
              ("ids_duplicated", v00j.get("ids_duplicated"), []), ("anchors_failing_boundary_or_match", failing, []),
              ("canaries", [c.get("verdict") for c in v00j.get("canaries", [])], ["DISCRIMINATES", "DISCRIMINATES"])]
    j = judge(checks, [("tag_count", v00j.get("tag_count_regex"), 46)])
    j["failing_anchors"] = failing
    j["mechanical_verdict_counts"] = v00j.get("mechanical_verdict_counts")
    j["semantic_verdict_counts"] = v00j.get("semantic_verdict_counts")
    j["semantic_source"] = "v00_semantic_review.yaml reused unchanged from a1e7c2277705 (executor reading judgement, not recomputed)"
    return dict(res, **j)


def v01(v01j, stage_rec=None):
    res = {"case_id": "V01"}
    if stage_rec is not None and (stage_rec.get("returncode") != 0 or stage_rec.get("timed_out")):
        return dict(res, status="ERROR", result=None, evidence_reasons=["stage exit %r" % stage_rec.get("returncode")])
    need = ("parse", "issue_ids_expected_CA01_CA07", "anchor_refs", "case_refs", "contract09_anchor_refs_dangling",
            "referenced_paths", "report_inputs_read", "manifest_self_check", "startup_selfcheck_quote_in_master_conventions",
            "completion_flags", "hex40_hits_in_scope_files")
    if not isinstance(v01j, dict) or any(k not in v01j for k in need):
        return dict(res, status="INCOMPLETE_EVIDENCE", result=None,
                    evidence_reasons=["v01 output missing keys: %s" % [k for k in need if not isinstance(v01j, dict) or k not in v01j]])
    ms = v01j["manifest_self_check"]
    cf = v01j["completion_flags"]
    checks = [("all_six_parse", all(v.get("ok") for v in v01j["parse"].values()) and len(v01j["parse"]) == 6, True),
              ("issue_ids_CA01_CA07", v01j["issue_ids_expected_CA01_CA07"], True),
              ("anchor_refs_dangling", v01j["anchor_refs"].get("dangling"), []),
              ("anchor_dups_within_issue", v01j["anchor_refs"].get("duplicates_within_an_issue"), {}),
              ("case_refs_dangling", v01j["case_refs"].get("dangling"), []),
              ("contract09_anchor_refs_dangling", v01j["contract09_anchor_refs_dangling"], []),
              ("referenced_paths_exist", all(x.get("exists_taskbook") for x in v01j["referenced_paths"].values()), True),
              ("report_inputs_read_exist", all(x.get("exists") for x in v01j["report_inputs_read"]), True),
              ("self_check_automated_equals_declared", ms.get("automated_equals_declared"), True),
              ("self_check_arithmetic", ms.get("arithmetic_ok"), True),
              ("startup_selfcheck_quote", v01j["startup_selfcheck_quote_in_master_conventions"], True),
              ("no_whole_completion_claims", [cf.get(k) for k in ("whole_002R_complete", "whole_003R_complete",
                                                                  "whole_007R_complete", "whole_wave2_complete")], [False] * 4),
              ("hex40_hits", sum(v01j["hex40_hits_in_scope_files"].values()), 0)]
    j = judge(checks, [("hex40_hits", sum(v01j["hex40_hits_in_scope_files"].values()), 1)])
    if j.get("result") == "OBSERVED_AS_PREDICTED":
        j["result"] = "NO_FAILURE_IN_SCOPE"
    return dict(res, **j)


def a46r(a46j, stage_rec=None):
    res = {"case_id": "V00_A46_correction"}
    if stage_rec is not None and (stage_rec.get("returncode") != 0 or stage_rec.get("timed_out")):
        return dict(res, status="ERROR", result=None, evidence_reasons=["stage exit %r" % stage_rec.get("returncode")])
    if not isinstance(a46j, dict) or not isinstance(a46j.get("anchors"), list) or len(a46j["anchors"]) != 2:
        return dict(res, status="INCOMPLETE_EVIDENCE", result=None, evidence_reasons=["a46 output missing or not two anchors"])
    by = {a.get("id"): a for a in a46j["anchors"]}
    checks = [("%s.verdict_mech" % k, (by.get(k) or {}).get("verdict_mech"), "MATCH_IN_DEFINITION") for k in ("A46-C", "A46-ASM")]
    checks += [("old_A46_still_fails_in_frozen_v00", a46j.get("old_A46_verdict_in_frozen_v00"), "OUTSIDE_OR_PARTIAL_DEFINITION")]
    return dict(res, **judge(checks, [("A46-C.verdict_mech", (by.get("A46-C") or {}).get("verdict_mech"), "QUOTE_NOT_FOUND")]))
