"""Case evaluation: compare fixture observations with the 07/09 predictions.

Each check is (label, observed, predicted). A case is OBSERVED_AS_PREDICTED when every check
matches, COUNTEREVIDENCE when any differs. Each evaluator also re-runs its comparison against a
deliberately wrong prediction ("negcontrol"); that comparison must report a mismatch, otherwise
the comparator is vacuous and the case is marked COMPARATOR_INVALID.
"""


def ev_find(events, **kw):
    return [e for e in events if all(e.get(k) == v for k, v in kw.items())]


def one(events, **kw):
    r = ev_find(events, **kw)
    return r[0] if r else {}


def judge(checks, wrong):
    mism = [c for c in checks if c[1] != c[2]]
    wrong_mism = [c for c in wrong if c[1] != c[2]]
    if not wrong_mism:
        v = "COMPARATOR_INVALID"
    else:
        v = "OBSERVED_AS_PREDICTED" if not mism else "COUNTEREVIDENCE"
    return {"verdict": v, "checks": [{"label": a, "observed": b, "predicted": c, "match": b == c} for a, b, c in checks],
            "negcontrol_wrong_prediction_detected": bool(wrong_mism),
            "negcontrol": [{"label": a, "observed": b, "wrong_prediction": c} for a, b, c in wrong]}


def events_of(rec, case):
    c = rec["cases"].get(case, {})
    return (c.get("run") or {}).get("events", []), c


def v02(w):
    e, c = events_of(w, "v02_single")
    p, k, f = one(e, step="after_prepare"), one(e, step="after_wake"), one(e, step="after_finish")
    checks = [("prepare.count", p.get("count"), 1), ("prepare.walk_len", p.get("walk_len"), 1),
              ("wake.links_empty(anchor_self)", k.get("anchor_self"), True), ("wake.count", k.get("count"), 1),
              ("wake.waiter_self_linked", k.get("a_self"), True),
              ("finish.count", f.get("count"), 1), ("finish.header_is_empty", f.get("header_is_empty"), False),
              ("exit_code", c.get("run", {}).get("returncode"), 0)]
    return judge(checks, [("finish.count", f.get("count"), 0)])


def v03(w):
    out = {}
    e, c = events_of(w, "v03_second_wake_direct")
    inv = one(e, ev="ttwu_invalid_task")
    out["second_wake_direct"] = judge(
        [("invalid_task_passed_to_try_to_wake_up", bool(inv), True), ("at_call", inv.get("call"), 2),
         ("task_field_is_queue_lock_word", inv.get("equals_watched_lock_word"), True),
         ("layout.anchor_container_task_aliases_lock", one(e, ev="layout").get("anchor_container_task_field_aliases_lock"), True),
         ("exit_code(stopped_by_invariant_guard)", c.get("run", {}).get("returncode"), 42)],
        [("invalid_task_passed_to_try_to_wake_up", bool(inv), False)])
    e, c = events_of(w, "v03_second_wake_via_complete")
    inv = one(e, ev="ttwu_invalid_task")
    out["second_wake_via_complete"] = judge(
        [("invalid_task_passed_to_try_to_wake_up", bool(inv), True),
         ("task_field_is_queue_lock_word", inv.get("equals_watched_lock_word"), True),
         ("task_ptr_nonzero_while_lock_held", (inv.get("task_ptr_value") or 0) != 0, True),
         ("exit_code", c.get("run", {}).get("returncode"), 42)],
        [("task_ptr_nonzero_while_lock_held", (inv.get("task_ptr_value") or 0) != 0, False)])
    e, c = events_of(w, "v03_all_two_waiters")
    t = ev_find(e, ev="ttwu")
    inv = one(e, ev="ttwu_invalid_task")
    out["all_locked_two_waiters"] = judge(
        [("valid_wakeups_in_order", [x.get("task") for x in t], [1, 2]),
         ("third_iteration_invalid", inv.get("call"), 3),
         ("exit_code", c.get("run", {}).get("returncode"), 42)],
        [("valid_wakeups_in_order", [x.get("task") for x in t], [1, 2, 1])])
    vs = [x["verdict"] for x in out.values()]
    return {"verdict": "OBSERVED_AS_PREDICTED" if all(v == "OBSERVED_AS_PREDICTED" for v in vs) else
            ("COMPARATOR_INVALID" if "COMPARATOR_INVALID" in vs else "COUNTEREVIDENCE"), "subcases": out}


def v09(w):
    e, _ = events_of(w, "v09_schedule_timeout_values")
    st = {x["in"]: x for x in ev_find(e, ev="st")}
    big = 9223372036854775807
    checks = []
    for t in (0, 1, 5):
        checks += [("in=%d.ret" % t, st.get(t, {}).get("ret"), t), ("in=%d.schedule_calls" % t, st.get(t, {}).get("sched_calls"), 0),
                   ("in=%d.state_after(UNINTERRUPTIBLE=2)" % t, st.get(t, {}).get("state_after"), 2)]
    checks += [("in=-1.ret", st.get(-1, {}).get("ret"), 0), ("in=-1.state_after(RUNNING)", st.get(-1, {}).get("state_after"), 0),
               ("in=MAX.schedule_calls", st.get(big, {}).get("sched_calls"), 1), ("in=MAX.ret", st.get(big, {}).get("ret"), big)]
    e2, _ = events_of(w, "v09_uninterruptible_wrapper")
    u = one(e2, ev="stu")
    checks += [("wrapper(5).ret", u.get("ret"), 5), ("wrapper(5).state_after", u.get("state_after"), 2)]
    e3, c3 = events_of(w, "v09_msleep_bounded")
    cap = one(e3, ev="step_cap")
    checks += [("msleep(5).stopped_at_step_cap", cap.get("cap"), 100), ("msleep.first_timeouts", cap.get("first_timeouts"), [5, 5, 5]),
               ("msleep.last_timeout", cap.get("last_timeout"), 5), ("msleep.schedule_calls", cap.get("sched_calls"), 0),
               ("msleep.never_returned", bool(one(e3, ev="msleep_returned")), False)]
    return judge(checks, [("in=5.ret", st.get(5, {}).get("ret"), 4)])


def v10(w):
    e, _ = events_of(w, "v10_done_preset_fast_path")
    a, b = one(e, ev="wfc_common"), one(e, ev="wait_for_completion")
    e2, _ = events_of(w, "v10_infinite_notify_during_schedule")
    r = one(e2, ev="returned")
    e3, _ = events_of(w, "v10_finite_timeout_no_notifier")
    cap = one(e3, ev="step_cap")
    checks = [("preset.schedule_calls", a.get("sched_calls"), 0), ("preset.done_consumed", a.get("done_after"), 0),
              ("preset.ret_is_MAX", a.get("ret_is_max"), True), ("preset.wait_for_completion.schedule_calls", b.get("sched_calls"), 0),
              ("infinite.schedule_calls", r.get("sched_calls"), 1), ("infinite.returned", bool(r), True),
              ("infinite.done_consumed", r.get("done_after"), 0), ("infinite.state_after(RUNNING)", r.get("state_after"), 0),
              ("finite(no notifier).bounded_non_progress", cap.get("cap"), 100), ("finite.schedule_calls", cap.get("sched_calls"), 0)]
    extra = {"infinite.header_count_after_return": r.get("count"), "infinite.walk_len_after_return": r.get("walk_len")}
    j = judge(checks, [("preset.schedule_calls", a.get("sched_calls"), 1)])
    j["observations_beyond_prediction"] = extra
    return j


def v11(w):
    e, c = events_of(w, "v11_wait_then_notify_then_reuse")
    s, ns, ne, wr = one(e, ev="schedule"), one(e, ev="notifier_start"), one(e, ev="notifier_end"), one(e, ev="waiter_returned")
    inv = one(e, ev="ttwu_invalid_task")
    checks = [("waiter_state_at_schedule(UNINTERRUPTIBLE)", s.get("current_state"), 2),
              ("queued_before_notify.count", ns.get("count"), 1), ("queued_before_notify.walk_len", ns.get("walk_len"), 1),
              ("after_notify.waiter_state(RUNNING)", ne.get("waiter_state"), 0), ("after_notify.done", ne.get("done"), 1),
              ("after_notify.count(stays 1)", ne.get("count"), 1), ("after_notify.walk_len", ne.get("walk_len"), 0),
              ("after_return.count(stays 1)", wr.get("count"), 1), ("after_return.done", wr.get("done"), 0),
              ("reuse.complete_hits_anchor_container", inv.get("equals_watched_lock_word"), True),
              ("exit_code", c.get("run", {}).get("returncode"), 42)]
    return judge(checks, [("after_return.count(stays 1)", wr.get("count"), 0)])


def v04(s):
    e, _ = events_of(s, "v04_noncurrent_wake")
    a, r = one(e, ev="q", step="after"), one(e, ev="ret")
    return judge([("ret", r.get("ret"), 0), ("state_after(RUNNING)", a.get("task_state"), 0),
                  ("queued_once", a.get("task_occ"), 1), ("rq0.count", a.get("count"), 1), ("preempt_balanced", a.get("preempt"), 0)],
                 [("ret", r.get("ret"), 1)])


def v05(s):
    e, _ = events_of(s, "v05_state_not_in_mask")
    checks = []
    for m in ev_find(e, ev="mask_case"):
        q = one(e, ev="q", step=m["label"]) if m["label"] != "current_uninterruptible_mask_interruptible" else one(e, ev="q", step="current_path")
        queued_expected = 0 if m["label"].startswith("current_") else 1
        checks += [("%s.state_in_mask" % m["label"], m.get("state_in_mask"), False),
                   ("%s.state_after(RUNNING)" % m["label"], q.get("task_state"), 0),
                   ("%s.queued" % m["label"], q.get("task_occ"), queued_expected), ("%s.ret" % m["label"], m.get("ret"), 0)]
    return judge(checks, [(checks[1][0], checks[1][1], 4)] if checks else [("empty", 0, 1)])


def v06(s):
    e, _ = events_of(s, "v06_double_wake")
    qs = {x["step"]: x for x in ev_find(e, ev="q")}
    r = one(e, ev="rets")
    checks = [("%s.occurrences" % k, qs.get(k, {}).get("task_occ"), 1) for k in ("after_first", "after_second", "after_third")]
    checks += [("%s.count" % k, qs.get(k, {}).get("count"), 1) for k in ("after_first", "after_second", "after_third")]
    checks += [("returns", [r.get("r1"), r.get("r2"), r.get("r3")], [0, 0, 0])]
    j = judge(checks, [("after_second.occurrences", qs.get("after_second", {}).get("task_occ"), 2)])
    if j["verdict"] == "OBSERVED_AS_PREDICTED":
        j["verdict"] = "NO_FAILURE_IN_SCOPE"
    return j


def v07(s):
    e, _ = events_of(s, "v07_cpu_metadata")
    q = {x["step"]: x for x in ev_find(e, ev="q")}
    checks = [("set_task_cpu(A,1).queued_on_rq1", q.get("set_task_cpu_A_to_1_rq1", {}).get("task_occ"), 1),
              ("set_task_cpu(A,1).task_cpu_meta_unchanged(3)", q.get("set_task_cpu_A_to_1_rq1", {}).get("task_cpu_meta"), 3),
              ("wake(B meta=2).queued_on_rq0", q.get("wake_B_meta2_rq0", {}).get("task_occ"), 1),
              ("wake(B meta=2).not_on_rq2", q.get("wake_B_meta2_rq2", {}).get("task_occ"), 0),
              ("new_task(C meta=3).queued_on_rq0", q.get("new_task_C_meta3_rq0", {}).get("task_occ"), 1),
              ("new_task(C meta=3).task_cpu_meta_unchanged(3)", q.get("new_task_C_meta3_rq0", {}).get("task_cpu_meta"), 3)]
    return judge(checks, [(checks[1][0], checks[1][1], 1)])


def v08(s):
    e, _ = events_of(s, "v08_pick_combinations")
    po = {x["name"]: x for x in ev_find(e, ev="pick_out")}
    combos = judge([
        ("a_running_empty.returns_current", po.get("a_running_empty", {}).get("returned_is_current"), True),
        ("b_blocked_empty.returns_current", po.get("b_blocked_empty", {}).get("returned_is_current"), True),
        ("c_blocked_one_runnable.returns_other", po.get("c_blocked_one_runnable", {}).get("returned"), 3),
        ("c.blocked_current_not_requeued(count 0)", po.get("c_blocked_one_runnable", {}).get("count_after"), 0),
        ("d_idle.requeued_at_tail", po.get("d_idle_one_runnable", {}).get("order_after"), [0]),
        ("e_blocked_only_idle.returns_idle", po.get("e_blocked_queue_only_idle", {}).get("returned"), 0)],
        [("b_blocked_empty.returns_current", po.get("b_blocked_empty", {}).get("returned_is_current"), False)])
    e2, _ = events_of(s, "v08_sequence_idle_requeue")
    so = {x["name"]: x for x in ev_find(e2, ev="pick_out")}
    seq = judge([("s1.idle_in_list_after_switch_out", so.get("s1_idle_switches_out", {}).get("idle_in_list"), True),
                 ("s2.idle_still_in_list", so.get("s2_first_blocks", {}).get("idle_in_list"), True),
                 ("s3.blocked_last_task_yields_idle", so.get("s3_second_blocks", {}).get("returned"), 0)],
                [("s3.blocked_last_task_yields_idle", so.get("s3_second_blocks", {}).get("returned"), 2)])
    e3, _ = events_of(s, "v08_sequence_idle_switched_out_blocked")
    to = {x["name"]: x for x in ev_find(e3, ev="pick_out")}
    brk = judge([("t1.idle_not_requeued", to.get("t1_blocked_idle_switches_out", {}).get("idle_in_list"), False),
                 ("t2.blocked_current_returned_with_empty_queue", to.get("t2_only_task_blocks", {}).get("returned_is_current"), True)],
                [("t2.blocked_current_returned_with_empty_queue", to.get("t2_only_task_blocks", {}).get("returned_is_current"), False)])
    e4, _ = events_of(s, "v08_vruntime_requeue_order")
    f = one(e4, ev="pick_out", name="f_running_requeue_by_vruntime")
    return {"function_level_combinations": combos, "sequence_with_idle_requeue": seq,
            "sequence_when_idle_switched_out_blocked": brk,
            "extra_observation_requeue_order": {"order_before_ids": [3, 4, 5], "vruntimes": {"2": 25, "3": 10, "4": 20, "5": 30},
                                                "order_after": f.get("order_after"), "vruntime_sorted_would_be": [4, 2, 5]}}


def v12(p):
    e, _ = events_of(p, "v12_add_test_negative")
    vec = {(x["init"], x["i"]): x for x in ev_find(e, ev="vec")}
    checks = []
    for k in ((-1, 1), (1, 2), (2, -1)):
        x = vec.get(k, {})
        checks += [("(%d,%d).after_is_subtraction" % k, x.get("after"), k[0] - k[1]),
                   ("(%d,%d).ret_is_sign_of_subtraction" % k, x.get("ret"), (k[0] - k[1]) < 0)]
    j = judge(checks, [("(-1,1).after_is_addition", vec.get((-1, 1), {}).get("after"), 0)])
    j["contract_mismatch_vectors"] = [list(k) for k, x in vec.items() if not (x.get("after_matches_contract") and x.get("ret_matches_contract"))]
    return j


def v13(p):
    e, _ = events_of(p, "v13_trylock")
    l = {x["step"]: x for x in ev_find(e, ev="lock")}
    checks = [("init.val", l.get("after_init", {}).get("val"), 0),
              ("first_trylock.ret(success)", l.get("after_first_trylock", {}).get("ret"), 1),
              ("first_trylock.val_unchanged", l.get("after_first_trylock", {}).get("val"), 0),
              ("first_trylock.is_locked_after", l.get("after_first_trylock", {}).get("is_locked"), 0),
              ("second_trylock_without_unlock.ret(success again)", l.get("after_second_trylock_without_unlock", {}).get("ret"), 1),
              ("control.trylock_while_held_by_arch_spin_lock.ret", l.get("trylock_while_held_by_arch_spin_lock", {}).get("ret"), 0)]
    return judge(checks, [("second_trylock_without_unlock.ret(success again)",
                           l.get("after_second_trylock_without_unlock", {}).get("ret"), 0)])


def v14(jf, jc):
    e, _ = events_of(jf, "v14")
    a = one(e, ev="one_handler_call")
    e2, _ = events_of(jc, "v14_control")
    b = one(e2, ev="one_handler_call")
    return judge([("alias.same_address", a.get("same_address"), True), ("alias.jiffies_64_delta_per_handler", a.get("jiffies_64_delta"), 2),
                  ("control.same_address", b.get("same_address"), False), ("control.jiffies_64_delta_per_handler", b.get("jiffies_64_delta"), 1)],
                 [("alias.jiffies_64_delta_per_handler", a.get("jiffies_64_delta"), 1)])
