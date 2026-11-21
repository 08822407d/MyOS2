"""Generate core/results.yaml from $CORE_WORK/{core_run,h00,v00,v01,static}.json. Values that come
from runs are read from those files; prose fields state scope and limits. Refuses 40-hex output
and verifies the YAML round-trips. Usage: python3 make_results.py <out.yaml>
"""
import json
import os
import re
import sys

import yaml

W = os.environ["CORE_WORK"]
J = {k: json.load(open(os.path.join(W, "%s.json" % k), encoding="utf-8"))
     for k in ("core_run", "h00", "v00", "v01", "static")}
run, h00, v00, v01, st = J["core_run"], J["h00"], J["v00"], J["v01"], J["static"]
ev, idt, fx = run["evaluation"], run["identity"], run["fixtures"]
EVD = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/evidence.md"
FXD = "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/fixtures/"
TIME = "time@%s" % idt["pins"]["time"]


def checks_obs(j):
    return {c["label"]: c["observed"] for c in j.get("checks", [])}


def events(fxname, case):
    return fx[fxname]["cases"][case]["run"]["events"]


def run_meta(fxname, cases):
    out = []
    for c in cases:
        r = fx[fxname]["cases"][c]
        out.append({"case": c, "exit": r["run"]["returncode"], "timed_out": r["run"]["timed_out"],
                    "repeat_identical": r["repeat_identical"]})
    return out


anc = v00["anchors"]
exceptions = [{"id": a["id"], "mechanical": a["verdict_mech"], "semantic": a["semantic"]["verdict"],
               "note": a["semantic"].get("note", "")} for a in anc
              if a["verdict_mech"] != "MATCH_IN_DEFINITION" or a["semantic"]["verdict"] not in ("SUPPORTS",)]
multi_hit = [{"id": a["id"], "hits": a["contiguous_hits"]} for a in anc if len(a.get("contiguous_hits", [])) > 1]

cases = []
cases.append({
    "case_id": "V00", "execution_status": "EXECUTED", "result": "COUNTEREVIDENCE",
    "result_scope": "47 个自报锚点中 1 个（A46）不满足“引文全在所标符号定义体内”的 P2 规则；其余逐字与边界成立",
    "evidence_types": ["SOURCE_MATCH"],
    "source_refs": [TIME + ":<A01-A47 所列路径>", "taskbook@%s:%s" % (idt["pins"]["taskbook"], v00["report"]["path"])],
    "preconditions": ["引文取自固定 blob 的 07 报告；源码取自固定 time 提交；比较为按 \\n 切行的整行逐字节连续匹配，制表符与反斜杠不归一化"],
    "observations": {
        "tag_count_regex": v00["tag_count_regex"], "tag_count_raw_substring": v00["tag_count_raw_substring"],
        "ids_unique": v00["ids_unique"], "ids_missing": v00["ids_missing_from_A01_A47"],
        "ids_duplicated": v00["ids_duplicated"],
        "all_quotes_1_to_5_lines": all(a["length_1_to_5"] for a in anc),
        "all_quotes_contiguous_hit_in_time": all(a.get("contiguous_hits") for a in anc),
        "quotes_with_real_tab": sum(1 for a in anc if a["quote_has_tab"]),
        "quotes_with_backslash": sum(1 for a in anc if a["quote_has_backslash"]),
        "mechanical_verdict_counts": v00["mechanical_verdict_counts"],
        "semantic_verdict_counts": v00["semantic_verdict_counts"],
        "exceptions_and_notes": exceptions,
        "quotes_hitting_more_than_one_place": multi_hit,
        "canaries": [{"key": c["key"], "verdict": c["verdict"]} for c in v00["canaries"]],
        "write_boundary": v00["protection"],
    },
    "evidence_refs": [EVD + " §3 V00（47 条逐项表）", FXD + "v00_anchors.py", FXD + "locate.py", FXD + "v00_semantic_review.yaml"],
    "limitations": ["定义体边界由词法定位器给出（C/CMake/链接脚本/shell/汇编各自规则，汇编与 shell 为启发式），不是编译器或预处理器结果",
                    "语义支持栏是执行者阅读判断（v00_semantic_review.yaml），不是机械结论",
                    "SOURCE_MATCH 只证明引文在所读版本中存在于所标定义体，不证明行为正确"]})
cases.append({
    "case_id": "V01", "execution_status": "EXECUTED", "result": "NO_FAILURE_IN_SCOPE",
    "evidence_types": ["SOURCE_MATCH"],
    "source_refs": ["taskbook@%s:agent-workspace/lead/MYOS2-LEAD-002/{07 报告,07 YAML,09,10,11,MANIFEST}" % idt["pins"]["taskbook"],
                    "master@%s:agent-workspace/conventions.md" % idt["pins"]["master"]],
    "preconditions": ["PyYAML %s 解析；旧批自检分母只用 MANIFEST.scope_files 六文件" % run["tools"]["pyyaml"]],
    "observations": {
        "yaml_parse_ok": {k: v["ok"] for k, v in v01["parse"].items()},
        "issue_ids_are_CA01_to_CA07": v01["issue_ids_expected_CA01_CA07"],
        "anchor_refs": v01["anchor_refs"], "case_refs": v01["case_refs"],
        "contract09_anchor_refs_dangling": v01["contract09_anchor_refs_dangling"],
        "referenced_paths_exist": all(x["exists_taskbook"] for x in v01["referenced_paths"].values()),
        "report_inputs_read_all_exist": all(x["exists"] for x in v01["report_inputs_read"]),
        "manifest_self_check": v01["manifest_self_check"],
        "startup_selfcheck_quote_in_master_conventions": v01["startup_selfcheck_quote_in_master_conventions"],
        "completion_flags": v01["completion_flags"],
        "hex40_hits_in_scope_files": v01["hex40_hits_in_scope_files"],
        "p9_wording_hits_in_scope_files": v01["p9_wording_hits_in_scope_files"]},
    "evidence_refs": [EVD + " §3 V01", FXD + "v01_structure.py"],
    "limitations": ["结构与引用完整，不代表内容全量完成；旧批 MANIFEST 声明的 47 与机械计数一致，只说明计数口径一致"]})


def host(case_id, result, ev_key, cases_run, fxname, pre, obs, limits, types=("HOST_ORIGINAL_SLICE",), refs=()):
    j = ev[ev_key]
    return {"case_id": case_id, "execution_status": "EXECUTED", "result": result, "evidence_types": list(types),
            "source_refs": list(refs), "preconditions": pre, "observations": obs,
            "comparator_negative_control_detected": j.get("negcontrol_wrong_prediction_detected") if isinstance(j, dict) else None,
            "runs": run_meta(fxname, cases_run),
            "evidence_refs": [EVD + " §4 " + case_id, FXD + fxname + ".c", FXD + "evaluate.py"],
            "limitations": limits}


SW = [TIME + ":mykernel/kactive/swait/swait.c", TIME + ":mykernel/lib/list/double_list.h",
      TIME + ":mykernel/lib/list/double_list_macro.h", TIME + ":mykernel/kactive/completion/completion.c",
      TIME + ":mykernel/kactive/completion/completion.h"]
cases.append(host("V02", ev["V02"]["verdict"], "V02", ["v02_single"], "fx_wait",
                  ["单个合法 waiter；无并发；try_to_wake_up 由记录型替身代替（记录调用、校验指针、只置 TASK_RUNNING、返回 0）"],
                  checks_obs(ev["V02"]),
                  ["不证明真实唤醒或抢占；替身不入队"], refs=SW))
v3 = ev["V03"]["subcases"]
cases.append(host("V03", ev["V03"]["verdict"], "V03", ["v03_second_wake_direct", "v03_second_wake_via_complete", "v03_all_two_waiters"], "fx_wait",
                  ["承接 V02 状态；swake_up_all_locked 两个 waiter；指针合法性守卫在把非 waiter 指针交给真实 try_to_wake_up 之前停止进程（退出码 42）"],
                  {"earliest_deviation": "第一次唤醒之后：count=1 而链上 0 个节点（V02 after_wake）",
                   "second_wake_direct": checks_obs(v3["second_wake_direct"]),
                   "second_wake_via_complete": checks_obs(v3["second_wake_via_complete"]),
                   "all_locked_two_waiters": checks_obs(v3["all_locked_two_waiters"]),
                   "layout": {k: v for k, v in [x for x in events("fx_wait", "v03_second_wake_direct") if x.get("ev") == "layout"][0].items()
                              if k not in ("case", "ev")},
                   "consequence": "anchor 反推出的“waiter”其 task 字段与队列头 lock 字重叠：锁空闲时为 0，complete() 持锁时为非零票据值；真实 try_to_wake_up 会把它当作 task 指针写入（本夹具在此之前停止，未执行该写入）",
                   "active_callers_outside_swait_c": "所读快照中 swake_up_all_locked 与 finish_swait 在 swait.c 外未见调用；completion 路径使用 __prepare_to_swait/swake_up_locked/__finish_swait"},
                  ["未让真实 try_to_wake_up 解引用该值；不声称已观察到内核崩溃或挂死", "不证明并发行为"], refs=SW))
cases.append(host("V04", ev["V04"]["verdict"], "V04", ["v04_noncurrent_wake"], "fx_sched",
                  ["目标任务 TASK_UNINTERRUPTIBLE、不在自有队列、非 current；task_struct/runqueue 为成员子集（被测函数所用成员，子结构原样复制）"],
                  checks_obs(ev["V04"]), ["单 CPU 数组模型；不证明 SMP 或真实调度"],
                  refs=[TIME + ":mykernel/sched/scheduler/scheduler_core.c", TIME + ":mykernel/sched/scheduler/scheduler.h"]))
cases.append(host("V05", ev["V05"]["verdict"], "V05", ["v05_state_not_in_mask"], "fx_sched",
                  ["四种非 current 状态与一条 current 路径，状态均不在传入 mask 内"],
                  {"checks": checks_obs(ev["V05"]),
                   "documented_contract_lines": st["V05"]["ttwu_doc_contract"],
                   "sched_fork_TASK_NEW_contract_lines": st["V05"]["sched_fork_contract"],
                   "active_ttwu_state_match_calls": len(st["V05"]["ttwu_state_match_active_calls"]),
                   "design_conflict": "函数注释合同“If (@state & @p->state) @p->state = TASK_RUNNING”与 sched_fork 注释“TASK_NEW 不会被外部事件唤醒并插入运行队列”在所读实现中都不成立；本项不判断哪一方应改，交主线"},
                  ["只测串行调用；不含信号或真实 IRQ 上下文"], types=("HOST_ORIGINAL_SLICE", "SOURCE_MATCH"),
                  refs=[TIME + ":mykernel/sched/scheduler/scheduler_core.c"]))
cases.append(host("V06", ev["V06"]["verdict"], "V06", ["v06_double_wake"], "fx_sched",
                  ["同一未运行目标连续唤醒三次（第二次前目标重新置为 UNINTERRUPTIBLE 但仍在队列中）"],
                  checks_obs(ev["V06"]), ["串行夹具无失败；并发唤醒未测（NOT_RUN）", "wake_up_new_task 的 add_to_head 前没有 contains 检查（静态，见 V00 A09 注），本项未测对同一任务重复调用它"],
                  refs=[TIME + ":mykernel/sched/scheduler/scheduler_core.c"]))
cases.append(host("V07", ev["V07"]["verdict"], "V07", ["v07_cpu_metadata"], "fx_sched",
                  ["task_thread_info(p)->cpu 预置为非 0 值，观察 set_task_cpu/唤醒/新任务路径"],
                  {"checks": checks_obs(ev["V07"]),
                   "static_active___set_task_cpu_calls": len(st["V07"]["active___set_task_cpu"]),
                   "static_active_thread_info_cpu_writes": len(st["V07"]["active_thread_info_cpu_writes"]),
                   "wake_up_new_task_rq": st["V07"]["wake_up_new_task_rq_line"],
                   "ttwu_select": st["V07"]["ttwu_select_line"]},
                  ["静态检索为正则、限 mykernel/ 下 .c/.h/.S/.lds；未见不等于全仓不存在", "未启动 AP，不证明迁移行为"],
                  types=("HOST_ORIGINAL_SLICE", "SOURCE_MATCH"), refs=[TIME + ":mykernel/sched/scheduler/scheduler_core.c"]))
v8 = ev["V08"]
cases.append({
    "case_id": "V08", "execution_status": "EXECUTED", "result": "COUNTEREVIDENCE",
    "result_scope": "函数层面与预测一致（阻塞 current + 空队列时返回 current）；但对该组合的可达性给出有条件反证：只要 idle 从不在非 RUNNING 状态被切出，非 idle 的 current 运行时 idle 必在队列中，count 不可能为 0",
    "evidence_types": ["HOST_ORIGINAL_SLICE", "STATIC_COUNTEREXAMPLE", "SOURCE_MATCH"],
    "source_refs": [TIME + ":mykernel/sched/scheduler/myos_rt.c", TIME + ":mykernel/sched/scheduler/scheduler_core.c", TIME + ":mykernel/init/init_task.c"],
    "preconditions": ["pick_next_task_myos 为原文；“切换”由夹具把返回任务设为 current 来模拟（非真实 context_switch）"],
    "observations": {
        "function_level": checks_obs(v8["function_level_combinations"]),
        "sequence_with_idle_requeue": checks_obs(v8["sequence_with_idle_requeue"]),
        "sequence_when_idle_switched_out_blocked": checks_obs(v8["sequence_when_idle_switched_out_blocked"]),
        "invariant_basis": {
            "idle_requeued_when_switched_out_running": "myos_rt.c pick_next_task_myos：curr_task == rq->idle 且 TASK_RUNNING 时 list_header_add_to_tail",
            "active_running_lhdr_sites": [(h["path"].split("/")[-1], h["line"]) for h in st["V08"]["active_running_lhdr_sites"]],
            "active_rq_idle_assignments": [(h["path"].split("/")[-1], h["line"]) for h in st["V08"]["active_rq_idle_assignments"]],
            "init_task_state": [h["text"] for h in st["V08"]["init_task_state_initializer"]],
            "active_idle_state_writers": len(st["V08"]["active_idle_state_writers"]),
            "rest_init_explicit_state_changes": len(st["V08"]["rest_init_state_changes"]),
            "schedule_idle_expects_running": [h["line"] for h in st["V08"]["schedule_idle_expects_running"]]},
        "not_established": "未证明 idle 上下文中调用的 kernel_thread/copy_process 等路径绝不会让 idle 进入非 RUNNING 并调用 schedule；这些函数本轮未读",
        "extra_observation_not_in_07": {
            "requeue_order": v8["extra_observation_requeue_order"],
            "static": "myos_rt.c 第37-40行：插入循环只比较初始首项 tmp_rt 的 vruntime（循环内不更新 tmp_rt），且在队列取空后先读 container_of(anchor) 的 vruntime 再判 anchor；后者为静态阅读，未执行"}},
    "comparator_negative_control_detected": all(v8[k]["negcontrol_wrong_prediction_detected"] for k in
                                                ("function_level_combinations", "sequence_with_idle_requeue", "sequence_when_idle_switched_out_blocked")),
    "runs": run_meta("fx_sched", ["v08_pick_combinations", "v08_sequence_idle_requeue", "v08_sequence_idle_switched_out_blocked", "v08_vruntime_requeue_order"]),
    "evidence_refs": [EVD + " §4 V08", FXD + "fx_sched.c", FXD + "static_checks.py"],
    "limitations": ["可达性论证限于所列活动修改点与 idle 状态写入点（正则检索）", "不证明真实上下文切换、IRQ 或 SMP"]})
TM = [TIME + ":mykernel/time/timer/timer.c"]
cases.append(host("V09", ev["V09"]["verdict"], "V09", ["v09_schedule_timeout_values", "v09_uninterruptible_wrapper", "v09_msleep_bounded"], "fx_wait",
                  ["定时器函数为记录型替身（正常返回）；jiffies 固定不走；msecs_to_jiffies 以恒等替身（只影响 0/非 0）；msleep 以 __mod_timer 第 100 次调用为步数上限"],
                  {"checks": checks_obs(ev["V09"]),
                   "independent_conditions_static": {
                       "finite_path_lines": st["V09"]["finite_path_lines"],
                       "process_timeout_active_references": [(h["path"].split("/")[-1], h["line"]) for h in st["V09"]["active_process_timeout_references"]],
                       "timer_expiry_dispatch_active_hits": len(st["V09"]["active_timer_expiry_dispatch_candidates"]),
                       "msleep_active_references_outside_timer_c": [h["text"] for h in st["V09"]["active_msleep_references_outside_timer_c"]],
                       "schedule_timeout_family_active_references_outside_timer_c": [(h["path"].split("/")[-1], h["line"]) for h in st["V09"]["active_schedule_timeout_family_references_outside_timer_c"]]}},
                  ["替身定时器不代表真实到期分发；步数上限只见证 100 步内不前进，不推断具体挂死形态",
                   "所读快照中 msleep 只有声明、未见活动调用者；semaphore ___down_common 以 schedule_timeout 循环（down_timeout 路径的活动调用者本轮未查）"],
                  refs=TM))
cases.append(host("V10", ev["V10"]["verdict"], "V10", ["v10_done_preset_fast_path", "v10_infinite_notify_during_schedule", "v10_finite_timeout_no_notifier"], "fx_wait",
                  ["done 预置为 1；或 MAX_SCHEDULE_TIMEOUT 且通知方在 schedule 替身内执行一次 complete（脚本化交错）；或经 wait_for_common 直接给有限 timeout"],
                  {"checks": checks_obs(ev["V10"]), "beyond_prediction": ev["V10"].get("observations_beyond_prediction"),
                   "public_finite_completion_api": "所读 completion.c 只有 complete 与 wait_for_completion；有限 timeout 分支只经 wait_for_common 直接调用得到"},
                  ["通知方交错是夹具脚本，不是真实调度", "不支持“所有 completion 都不工作”的扩大结论"], refs=SW + TM))
cases.append({
    **host("V11", ev["V11"]["verdict"], "V11", ["v11_wait_then_notify_then_reuse"], "fx_wait",
           ["waiter 先经 wait_for_completion 入队并置 UNINTERRUPTIBLE；通知方在 schedule 替身内 complete；随后对同一 completion 再 complete 一次"],
           {"checks": checks_obs(ev["V11"])},
           ["真实上下文切换层 NOT_RUN：交错由 schedule 替身脚本给出", "不测试取消协议；“等待函数返回”不代表异步生产者已停止"], refs=SW),
    "execution_status": "EXECUTED_LOCAL_SEQUENCE_DYNAMIC_LAYER_NOT_RUN"})
cases.append(host("V12", ev["V12"]["verdict"], "V12", ["v12_add_test_negative"], "fx_prims",
                  ["x86-64 宿主，gcc -O0 编译原内联 asm（含原 LOCK_PREFIX）；合同列按函数自身注释（先加再判负）用普通 C 计算"],
                  {"checks": checks_obs(ev["V12"]), "contract_mismatch_vectors": ev["V12"].get("contract_mismatch_vectors"),
                   "all_vectors": [{k: v for k, v in x.items() if k not in ("case", "ev")} for x in events("fx_prims", "v12_add_test_negative")]},
                  ["单线程；不证明多核原子性"], refs=[TIME + ":mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h"]))
cases.append(host("V13", ev["V13"]["verdict"], "V13", ["v13_trylock"], "fx_prims",
                  ["arch_spin_init/arch_atomic64_read/arch_spin_is_locked/arch_spin_lock/unlock 均为原文；初始化前锁字预填 0xa5 字节以排除“恰好为 0”"],
                  {"checks": checks_obs(ev["V13"]),
                   "lock_steps": [{k: v for k, v in x.items() if k not in ("case", "ev")} for x in events("fx_prims", "v13_trylock")]},
                  ["串行片段；不替代多核压力或完整 spinlock 正确性证明；未测 arch_spin_lock 的并发正确性"],
                  refs=[TIME + ":mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h"]))
cases.append({
    "case_id": "V14", "execution_status": "EXECUTED_PARTIAL", "result": ev["V14"]["verdict"],
    "result_scope": "源码与构建引用层 + 宿主链接模型层；EXISTING_ELF_EVIDENCE 未执行（无 ELF）",
    "evidence_types": ["SOURCE_MATCH", "MODEL_ONLY"],
    "source_refs": [TIME + ":mykernel/arch/x86_64/kernel/hpet.c", TIME + ":mykernel/time/timekeeping/timekeeping.c",
                    TIME + ":mykernel/arch/x86_64/kernel.lds", TIME + ":mykernel/scripts/target_kernel.cmake"],
    "preconditions": ["HPET_handler 与 do_timer 原文；别名由含 kernel.lds 原行的隐式链接脚本提供；对照组让 jiffies 独立存储"],
    "observations": {"model_checks": checks_obs(ev["V14"]), "alias_line": fx["fx_jiffies"]["alias_ld"],
                     "static": {"c_definitions_of_jiffies": len(st["V14"]["active_c_definitions_of_jiffies"]),
                                "c_definitions_of_jiffies_64": [(h["path"].split("/")[-1], h["line"]) for h in st["V14"]["active_c_definitions_of_jiffies_64"]],
                                "link_option": st["V14"]["target_link_options"], "arch_default": st["V14"]["arch_default"],
                                "hpet_registration_chain": [(h["path"].split("/")[-1], h["line"], h["text"]) for h in st["V14"]["hpet_registration"]],
                                "late_time_init_callers": [(h["path"].split("/")[-1], h["line"]) for h in st["V14"]["late_time_init_callers"]],
                                "elf_files_in_time_tree": st["V14"]["elf_files_in_time_tree"]},
                     "existing_elf_evidence": "NOT_RUN：time 树中无 ELF，Owner 未提供可信产物；不构建内核"},
    "comparator_negative_control_detected": ev["V14"]["negcontrol_wrong_prediction_detected"],
    "runs": run_meta("fx_jiffies", ["v14"]) + run_meta("fx_jiffies_control", ["v14_control"]),
    "evidence_refs": [EVD + " §4 V14", FXD + "fx_jiffies.c"],
    "limitations": ["MODEL_ONLY：宿主可执行文件，不是 MyOS2 ELF；不证明实际中断频率、墙钟倍率或 timeout 真实倍率",
                    "真实时基保持 pending_local"]})

ca = [
    {"issue_id": "CA-01", "ruling": "SUPPORTED_IN_SCOPE", "cases": ["V04", "V05", "V06", "V07"],
     "basis": "原函数片段：非 current 唤醒经 set_task_cpu 进入自有 running_lhdr 并置 RUNNING，却返回 0；状态掩码未筛选（含 TASK_NEW）；串行重复唤醒不重复入队；任务 CPU 元数据不随 set_task_cpu 更新，唤醒与新任务都放 CPU0",
     "not_established": ["完整唤醒正确性", "SMP 放置"]},
    {"issue_id": "CA-02", "ruling": "FUNCTION_LEVEL_SUPPORTED_REACHABILITY_COUNTEREVIDENCE", "cases": ["V08"],
     "basis": "阻塞 current + 空队列时原函数返回 current；但 idle 以 RUNNING 切出时被重新挂回队列，使非 idle current 下 count=0 不可达，除非 idle 以非 RUNNING 状态切出（夹具序列显示此前提一旦出现即到达该组合）",
     "not_established": ["idle 上下文绝不睡眠（kernel_thread/copy_process 未读）"]},
    {"issue_id": "CA-03", "ruling": "SUPPORTED_IN_SCOPE", "cases": ["V09", "V10", "V11"],
     "basis": "有限 timeout 原样返回、不调度、状态保持 UNINTERRUPTIBLE；msleep 在 100 步内不前进；MAX 路径调度一次；done 预置走快路径",
     "not_established": ["真实到期分发", "有限 timeout 的活动调用者影响面（msleep 未见活动调用者；semaphore 路径未追到调用者）"]},
    {"issue_id": "CA-04", "ruling": "SUPPORTED_IN_SCOPE", "cases": ["V02", "V03", "V11"],
     "basis": "摘链后 count 保持 1；再次唤醒或全唤醒第三轮把 anchor 当作 waiter，其 task 字段即队列锁字；completion 复用时同样触发",
     "not_established": ["真实内核中的崩溃/挂死形态"]},
    {"issue_id": "CA-05", "ruling": "SUPPORTED_AT_SOURCE_AND_MODEL_LEVEL", "cases": ["V14"],
     "basis": "源码与构建引用成立；宿主别名模型一次调用 +2（对照 +1）",
     "not_established": ["ELF 符号核对", "实际时基/倍率"]},
    {"issue_id": "CA-06", "ruling": "SUPPORTED_AT_SOURCE_LEVEL", "cases": ["V00", "V01"],
     "basis": "A35 位于活动的 #ifdef CONFIG_BUG 分支且 CONFIG_BUG 在 CMAKE_C_FLAGS 中定义；A36/A37 为脚本顶层行；未运行脚本或虚拟机",
     "not_established": ["任何运行时观测通道的可靠性"]},
    {"issue_id": "CA-07", "ruling": "SUPPORTED_BY_HOST_ORIGINAL_SLICE", "cases": ["V12", "V13"],
     "basis": "原 asm 实测为减法语义；trylock 成功不改锁字、可重复成功，而真实持锁时返回失败",
     "not_established": ["多核原子性与完整自旋锁正确性"]},
]

doc = {
    "task_id": "MYOS2-LEAD-002-CORE-CHECK-01", "track_id": "MYOS2-LEAD-002", "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
    "phase": "core", "record_type": "cloud_core_verification_results",
    "produced_by": "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection",
    "execution_model_selection": "unknown_or_not_attestable",
    "execution_model_selection_source": "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述，主线回执已将其记为执行者解释）",
    "execution_surface": "claude.ai/code 托管云端会话容器；%s；普通用户态进程" % run["tools"]["cpu_arch"],
    "date": "2026-09-27",
    "base_snapshot": "kernel=time（短 SHA %s）；workspace=master（短 SHA %s）；taskbook=agent/MYOS2-LEAD-002（技术输入短 SHA %s，回执所在头短 SHA %s）；短 SHA 均自本会话 git rev-parse --short=12 输出" % (
        idt["pins"]["time"], idt["pins"]["master"], idt["pins"]["taskbook"], idt["pins"]["review"]),
    "inputs_read": ["agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/11-core-verification-cloud.md",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/07-core-audit-map.yaml",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/MANIFEST.md",
                    "agent/MYOS2-LEAD-002:agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-09-27-pilot-reviewed-core-ready.md",
                    "master:agent-workspace/conventions.md", "master:agent-workspace/tasks/00-gpt-task-protocol-v2.md",
                    "time:夹具 extraction 清单与 V00 所列源码（见 evidence.md）"],
    "status": "frozen_before_remote_readback",
    "readback_note": "本文件在远端回读前提交并冻结；回读结论只写在 evidence.md 与 MANIFEST.md。",
    "acceptance_ceiling": "PASS_PENDING_LOCAL",
    "kernel_modified": False, "repo_scripts_run": False, "full_kernel_build": False, "qemu_run": False, "tools_installed": False,
    "open_questions": ["idle 上下文是否可能以非 RUNNING 状态被切出（决定 CA-02 组合是否可达）",
                       "有限 timeout 在活动调用链中的实际使用者（semaphore down_timeout 调用者未查）",
                       "真实 ELF、时基、IRQ/SMP 仍待本地"],
    "admission": {"review_path": idt["review"]["path"], "review_bytes": idt["review"]["bytes"],
                  "review_sha256_segments": idt["review"]["sha256_segments"], "fields": idt["review"]["fields"],
                  "gate": idt["gate"]},
    "source_refs": {"read_time_utc": idt["read_time_utc"], "pins": idt["pins"], "remote_heads_match": idt["remote_heads_match"],
                    "taskbook_pin_is_ancestor_of_review_pin": idt["taskbook_pin_is_ancestor_of_review_pin"],
                    "six_technical_inputs_identical_taskbook_vs_review": {k: v["identical"] for k, v in idt["six_technical_inputs_taskbook_vs_review"].items()},
                    "taskbook_to_review_changes": idt["taskbook_to_review_changes"],
                    "sha256_segments_rule": "4 段各 16 个小写十六进制字符，按顺序无分隔拼接即完整 SHA-256"},
    "environment": run["tools"],
    "hardening_H00": {p["probe"]: (p.get("as_expected") if "as_expected" in p else p.get("weakness_reproduced")) for p in h00["probes"]},
    "cases": cases,
    "ca_rulings": ca,
    "omitted_paths_question_09_s5_4": {
        "active_enqueue": "running_lhdr 的活动修改点只有 set_task_cpu、wake_up_new_task、pick_next_task_myos（及初始化）；未见遗漏的活动入队",
        "count_maintenance": "swait 中 swake_up_locked 与 finish_swait 均用不带 count 的 list_del_init（后者在 swait.c 外未见调用）；pick 的 list_add_to_prev 后手工 count++ 配对",
        "expiry_path": "按 call_timer_fn/expire_timers/__run_timers/run_timer_softirq/run_timer_base/run_local_timers/->function( 检索无活动命中（仅注释 2 处）；到期分发链仍未闭合"},
    "counts": {"cases_total": 15,
               "executed": sum(1 for c in cases if c["execution_status"].startswith("EXECUTED")),
               "not_run": sum(1 for c in cases if c["execution_status"] == "NOT_RUN"),
               "results": {r: sum(1 for c in cases if c["result"] == r) for r in sorted({c["result"] for c in cases})}},
    "evidence_refs": [EVD, "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/MANIFEST.md", FXD],
    "limitations": ["所有执行都是本次云端会话里的普通用户态宿主进程，不是 MyOS2 在 CPU 上运行",
                    "HOST_ORIGINAL_SLICE 保留原函数文本，但类型为成员子集、依赖为替身；替身逐项列在 evidence.md",
                    "未修改内核、未运行原仓库脚本、未构建内核、未启动 QEMU、未安装工具",
                    "结果不是任何范围的 MyOS2 PASS；验收上限仍为 PASS_PENDING_LOCAL"],
}
doc = json.loads(json.dumps(doc, ensure_ascii=False))  # tuples -> lists for the safe dumper
text = yaml.safe_dump(doc, sort_keys=False, allow_unicode=True, width=1000)
if re.search(r"[0-9a-fA-F]{40}", text):
    sys.exit("REFUSED: 40-hex in results.yaml")
assert yaml.safe_load(text) == doc
open(sys.argv[1], "w", encoding="utf-8").write(text)
print("wrote %s (%d bytes, %d lines)" % (sys.argv[1], len(text.encode()), text.count("\n")))
print(json.dumps(doc["counts"], ensure_ascii=False))
