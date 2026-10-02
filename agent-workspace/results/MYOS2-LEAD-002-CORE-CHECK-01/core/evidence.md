---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
phase: core
record_type: cloud_core_verification_evidence
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_model_selection_source: "执行者所在平台的规则不允许在推送到仓库的产物中写模型标识（执行者自述）"
execution_surface: "claude.ai/code 托管云端会话容器；x86_64；普通用户态进程"
date: "2026-09-27"
base_snapshot: "kernel=time（短 SHA a039d9803ade）；workspace=master（短 SHA de3bb1df906a）；taskbook 技术输入短 SHA 57a7c3e0eebf；回执头短 SHA f79b3a281616"
inputs_read: ["见 results.yaml inputs_read"]
status: final_for_core
results_commit_short12: 7e2fa84a9823
redaction: "未发现需脱敏内容；push 输出经 40 位十六进制替换过滤（实际无命中）。每次 Bash 调用后平台附加的 Shell cwd was reset 提示不是命令输出，已省略。"
transcription: "除 §1.1 标注为手工转录的两段外，本文件的数值、事件行与表格均由 fixtures/build_evidence.py 从正式运行输出生成。"
open_questions: []
---

# CORE-CHECK-01 core 证据：准入、加固、47 引文、夹具与回读

**十五项全部执行（V11 的真实上下文切换层与 V14 的 ELF 层未执行）；两项给出反证：V00 的 A46 定义体边界、V08 空队列组合的可达性。** 所有运行都在本次云端会话的普通用户态进程中，不是 MyOS2 在 CPU 上运行。

正式运行目录 `CORE_WORK=/tmp/claude-0/-home-user-MyOS2/e3ce16b3-588b-596b-86aa-5e369173435c/scratchpad/core-work/canonical`（本会话 scratchpad 下新建，执行者未删除；容器回收后不保留）。夹具与脚本在 `agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/fixtures/`，均为文本；构建时从固定 time 提交逐字复制原函数，不入库任何二进制或整份源码。

## 1. 执行身份与准入

### 1.1 回执读取（手工转录）

```text
$ curl -sS -o $S/review.md -w 'http=%{http_code} bytes=%{size_download}\n' https://raw.githubusercontent.com/08822407d/MyOS2/agent/MYOS2-LEAD-002/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md; echo curl_exit=$?
$ git show origin/agent/MYOS2-LEAD-002:<同路径> > $S/review.git.md; cmp $S/review.md $S/review.git.md && echo IDENTICAL
http=200 bytes=13429
curl_exit=0
IDENTICAL
```

会话开始时 `git fetch` 使 origin/agent/MYOS2-LEAD-002 由 `57a7c3e0eebf` 前进到 `f79b3a281616`；`git diff --stat` 只显示新增回执与检查点两个文件。11 号文件与 09 号文件全文读取（`git show 57a7c3e0eebf:<path>`）。

### 1.2 准入绑定与输入身份（run_all.py identity，机械）

```json
{
 "read_time_utc": "2026-09-27T05:29:30Z",
 "pins": {
  "time": "a039d9803ade",
  "master": "de3bb1df906a",
  "taskbook": "57a7c3e0eebf",
  "review": "f79b3a281616",
  "pilot_head": "0851af4fc08b"
 },
 "remote_heads_match": {
  "master==pin.master": true,
  "time==pin.time": true,
  "agent/MYOS2-LEAD-002==pin.review": true,
  "claude/dazzling-cori-q0dnyt==pin.pilot_head": true
 },
 "taskbook_pin_is_ancestor_of_review_pin": true,
 "taskbook_to_review_changes": [
  "A\tagent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-09-27-pilot-reviewed-core-ready.md",
  "A\tagent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md"
 ],
 "review": {
  "path": "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-CHECK-01-pilot-review.md",
  "bytes": 13429,
  "sha256_segments": [
   "b1996da334a30973",
   "2491a28e77d77d3c",
   "ca701044011d72a1",
   "09119196bdd831e2"
  ],
  "fields": {
   "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
   "disposition": "ALLOW_CORE",
   "reviewed_pr": 17,
   "reviewed_branch": "claude/dazzling-cori-q0dnyt",
   "reviewed_commit_short12": "0851af4fc08b",
   "pilot_result_commit_short12": "10ecb7dd0bdb"
  },
  "reviewed_input_refs": {
   "workspace": {
    "branch": "master",
    "short12": "de3bb1df906a"
   },
   "kernel": {
    "branch": "time",
    "short12": "a039d9803ade"
   },
   "taskbook": {
    "branch": "agent/MYOS2-LEAD-002",
    "short12": "57a7c3e0eebf"
   }
  }
 },
 "gate": {
  "disposition_is_ALLOW_CORE": true,
  "packet_matches": true,
  "pr_matches_17": true,
  "branch_matches": true,
  "reviewed_head_matches_pin": true,
  "reviewed_inputs_match_pins": true,
  "ALLOW_CORE_bound_to_this_execution": true
 }
}
```

六份原技术输入在 `57a7c3e0eebf` 与 `f79b3a281616` 两处逐字节相同：

| 文件 | 相同 | 字节 | SHA-256 第 1 段 |
|---|---|---|---|
| 07-scheduler-wakeup-timer-audit.md | True | 22708 | a44f91940cf33240 |
| 07-core-audit-map.yaml | True | 9612 | 0ce03c08a9593cb7 |
| MANIFEST.md | True | 7093 | 1d591d6b4b2ee7dd |
| 09-local-verification-contract.md | True | 14406 | 3d362d09fd8af8dd |
| 10-cloud-pilot-and-github-handoff.md | True | 16036 | 4160a37fcfd8936d |
| 11-core-verification-cloud.md | True | 14462 | 41d03558e03abcd8 |

工具：`{"cpu_arch": "x86_64", "kernel_release": "6.18.44-fc-v37", "python3": "3.11.15", "gcc": "gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0", "clang": "Ubuntu clang version 18.1.3 (1ubuntu1)", "git": "git version 2.43.0", "nm": "GNU nm (GNU Binutils for Ubuntu) 2.42", "pyyaml": "6.0.1"}`

## 2. 驱动加固（回执 §3，正式检查前完成）

`h00_hardening.py`；运行器 `harness.run_pg` 让每个命令独立成会话/进程组，超时只对该组发 SIGKILL，收尾读取另设 5 秒上限。

| 探针 | 结果 | 关键值 |
|---|---|---|
| group_kill | 符合预期 | elapsed=0.302s, rc=-9, group_killed=True, pgid≠own=True, 孙进程状态采样末值=ABSENT, 旁观进程存活=True |
| pilot_runner_contrast | 符合预期 | pilot 式运行器：{"timed_out": true, "elapsed_s": 3.018, "child_alive_0p2s_after_parent_kill": true}；复现缺陷=True |
| compile_failure | 符合预期 | status=BLOCKED_COMPILE, compile rc=1, 旧二进制残留=False, 案例={'c1': 'BLOCKED', 'c2': 'BLOCKED'} |
| case_exception_isolated | 符合预期 | 注入异常案例=ERROR；后续案例=RAN |
| readback_criteria | 符合预期 | 不存在路径：[('404', False), ('404', False)]；冻结 pilot 文件：[('200', True, True, True, True), ('200', True, True, True, True)] |

结论：pilot 驱动在父进程被杀后仍等待孤儿子进程约 3 秒且子进程存活，已在本轮改为进程组隔离；编译失败不运行旧产物；单案例异常不吞批次；回读判据显式包含 curl 退出码、超时、HTTP 状态、远端分支头、目标 blob 字节与解析字段。

## 3. V00 与 V01

### 3.1 V00：47 条引文逐项明细

标签计数：正则 47、原始子串 47；ID 唯一=True，缺失=[]，重复=[]。比较为整行逐字节连续匹配（制表符、反斜杠不归一化）。“机械”列来自 v00_anchors.py；“语义”列为执行者阅读判断（v00_semantic_review.yaml）。

| ID | 路径::符号 | 行数 | 命中行 | 定义体范围 | 注释行 | 机械 | 语义 | 说明 |
|---|---|---|---|---|---|---|---|---|
| A01 | scripts/options_flags.cmake::CMAKE_C_FLAGS | 3 | 64 | 58-70 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 三行位于第二个 set(CMAKE_C_FLAGS ...)（第58-70行）；文件共有两个 set(CMAKE_C_FLAGS) 命令，第一个不含这些 -D。 |
| A02 | sched/scheduler/scheduler_core.c::set_task_cpu | 4 | 177 | 123-181 | - | MATCH_IN_DEFINITION | SUPPORTS | set_task_cpu 活动尾部：取 per_cpu rq、写 TASK_RUNNING、contains 检查后 add_to_head；其前全部为注释。 |
| A03 | sched/scheduler/scheduler_core.c::try_to_wake_up | 2 | 330 | 328-483 | - | MATCH_IN_DEFINITION | SUPPORTS | success 仅在此声明处初始化为 0。 |
| A04 | sched/scheduler/scheduler_core.c::try_to_wake_up | 1 | 462 | 328-483 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 调用活动；传入的是局部 cpu（初值 0），原 p->wake_cpu 版本被注释。 |
| A05 | sched/scheduler/scheduler_core.c::try_to_wake_up | 1 | 471 | 328-483 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 外层 if (task_cpu(p) != cpu) 被注释，故 set_task_cpu 在非 current 路径无条件执行。 |
| A06 | sched/scheduler/scheduler_core.c::try_to_wake_up | 3 | 480 | 328-483 | - | MATCH_IN_DEFINITION | SUPPORTS | 定义体内对 success 的活动引用只有声明与本 return；ttwu_state_match(...&success) 全为注释。 |
| A07 | sched/scheduler/scheduler.h::select_task_rq | 1 | 79 | 56-80 | - | MATCH_IN_DEFINITION | SUPPORTS | select_task_rq 除 return cpu 外全部为注释。 |
| A08 | sched/scheduler/scheduler_core.c::wake_up_process | 1 | 558 | 556-559 | - | MATCH_IN_DEFINITION | SUPPORTS | wake_up_process 直接返回 try_to_wake_up(p, TASK_NORMAL, 0)。 |
| A09 | sched/scheduler/scheduler_core.c::wake_up_new_task | 1 | 721 | 684-722 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | rq 取 per_cpu(runqueues, 0)；与 set_task_cpu 不同，此处 add_to_head 前没有 list_header_contains 检查。 |
| A10 | sched/scheduler/myos_rt.c::pick_next_task_myos | 2 | 24 | 5-58 | - | MATCH_IN_DEFINITION | SUPPORTS | pick_next_task_myos 在条件分支内 remove_head 取下一任务。 |
| A11 | sched/scheduler/myos_rt.c::myos_rt_sched_class | 2 | 66 | 65-68 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 名称由 DEFINE_SCHED_CLASS(myos_rt) 宏拼接生成；所读快照中 DEFINE_SCHED_CLASS 只在 myos_rt.c 使用一次，kernel.lds 以 __sched_class_highest/lowest 包住 .data.sched_class。 |
| A12 | sched/scheduler/scheduler.h::__pick_next_task | 3 | 264 | 220-277 | - | MATCH_IN_DEFINITION | SUPPORTS | for_each_class 循环内活动调用 class->pick_next_task；注释掉的是 pick_task 分支。 |
| A13 | sched/scheduler/scheduler.h::__schedule_loop | 5 | 282 | 279-287 | - | MATCH_IN_DEFINITION | SUPPORTS | schedule() 调用 __schedule_loop(SM_NONE)（同文件第1211行），循环内调用 __schedule。 |
| A14 | arch/x86_64/myos/interrupt.c::excep_hwint_context | 2 | 128 | 119-130 | - | MATCH_IN_DEFINITION | SUPPORTS | excep_hwint_context 在异常/硬中断处理后 !in_atomic() 即调用 schedule()，不看 need_resched。 |
| A33 | sched/scheduler/myos_rt.c::pick_next_task_myos | 5 | 14 | 5-58 | - | MATCH_IN_DEFINITION | SUPPORTS | 取下一任务的整个分支以 running_lhdr.count > 0 为合取条件。 |
| A34 | sched/scheduler/myos_rt.c::pick_next_task_myos | 1 | 9 | 5-58 | - | MATCH_IN_DEFINITION | SUPPORTS | retval 默认 current；分支不执行时 rq->curr = retval 并返回 current。 |
| A44 | sched/scheduler/scheduler_core.c::__schedule | 1 | 1116 | 1017-1170 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | pick_next_task 在同文件第976行 #define 为 __pick_next_task。 |
| A45 | sched/scheduler/scheduler_core.c::__schedule | 1 | 1162 | 1017-1170 | - | MATCH_IN_DEFINITION | SUPPORTS | prev != next 分支调用 context_switch。 |
| A15 | time/timer/timer.c::schedule_timeout | 4 | 796 | 759-806 | 798 | MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE | SUPPORTS_WITH_NOTE | 引文第3行 '// schedule();' 是注释行，属于有意引用；其余三行为活动语句。机械结果 MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE 与此一致。 |
| A16 | time/timer/timer.c::schedule_timeout | 1 | 805 | 759-806 | - | MATCH_IN_DEFINITION | SUPPORTS | 定义体内没有以 jiffies/expire 计算剩余量的活动语句；out 标签后只有此返回式。 |
| A17 | time/timer/timer.c::msleep | 2 | 856 | 852-858 | - | MATCH_IN_DEFINITION | SUPPORTS | msleep 循环以 schedule_timeout_uninterruptible 的返回值作为下一轮 timeout。 |
| A18 | time/timer/timer.c::schedule_timeout_uninterruptible | 2 | 828 | 826-830 | - | MATCH_IN_DEFINITION | SUPPORTS | 先写 TASK_UNINTERRUPTIBLE 再调用 schedule_timeout。 |
| A19 | time/timer/timer.c::schedule_timeout | 2 | 774 | 759-806 | - | MATCH_IN_DEFINITION | SUPPORTS | case MAX_SCHEDULE_TIMEOUT 分支 schedule(); goto out。 |
| A20 | kactive/completion/completion.c::wait_for_completion | 1 | 54 | 53-55 | - | MATCH_IN_DEFINITION | SUPPORTS | wait_for_completion 以 MAX_SCHEDULE_TIMEOUT 调 wait_for_common；后者把 schedule_timeout 作为 action（completion.h）。 |
| A40 | init/main.c::rest_init | 1 | 120 | 82-129 | - | MATCH_IN_DEFINITION | SUPPORTS | rest_init 在两次 kernel_thread 之后 complete(&kthreadd_done)。 |
| A41 | init/main.c::kernel_init | 1 | 277 | 270-309 | - | MATCH_IN_DEFINITION | SUPPORTS | kernel_init 首条语句等待 kthreadd_done。 |
| A21 | kactive/swait/swait.c::__prepare_to_swait | 3 | 47 | 46-50 | - | MATCH_IN_DEFINITION | SUPPORTS | __prepare_to_swait：节点自指时 list_header_add_to_tail。 |
| A22 | lib/list/double_list.h::list_header_add_to_tail | 2 | 665 | 662-667 | - | MATCH_IN_DEFINITION | SUPPORTS | list_header_add_to_tail 增加 count。 |
| A23 | kactive/swait/swait.c::swake_up_locked | 5 | 26 | 23-31 | - | MATCH_IN_DEFINITION | SUPPORTS | swake_up_locked 以 count 判空，取 anchor.next 容器，唤醒后 list_del_init（不带头计数）。 |
| A24 | lib/list/double_list.h::list_del_init | 2 | 281 | 278-283 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | list_del_init 只改链接并自指，不改 count；其 __list_del_entry_valid 在链接不一致时是 while(...); 自旋而不是报错返回。 |
| A25 | lib/list/double_list.h::list_header_is_empty | 1 | 631 | 628-632 | - | MATCH_IN_DEFINITION | SUPPORTS | list_header_is_empty 只读 count。 |
| A26 | kactive/swait/swait.c::__finish_swait | 3 | 53 | 52-56 | - | MATCH_IN_DEFINITION | SUPPORTS | __finish_swait 仅在节点非自指时调用带 count 的 list_header_delete_node。 |
| A27 | lib/list/double_list_macro.h::list_headr_first_container | 2 | 334 | 334-335 | - | MATCH_IN_DEFINITION | SUPPORTS | 宏展开为 list_first_entry(&anchor, ...) -> container_of(anchor.next, ...)，不检查 count 或 anchor。 |
| A42 | kactive/completion/completion.c::complete | 3 | 36 | 32-40 | - | MATCH_IN_DEFINITION | SUPPORTS | complete 在锁内 done++ 后调用 swake_up_locked。 |
| A28 | arch/x86_64/kernel/hpet.c::HPET_handler | 2 | 527 | 525-537 | - | MATCH_IN_DEFINITION | SUPPORTS | HPET_handler 前两条活动语句 jiffies++; do_timer(1);。 |
| A29 | time/timekeeping/timekeeping.c::do_timer | 1 | 272 | 270-273 | - | MATCH_IN_DEFINITION | SUPPORTS | do_timer 唯一语句 jiffies_64 += ticks。 |
| A30 | arch/x86_64/kernel.lds::jiffies | 1 | 17 | 17-17 | - | MATCH_IN_DEFINITION | SUPPORTS | kernel.lds 第17行顶层赋值（SECTIONS 之外）。 |
| A31 | scripts/target_kernel.cmake::target_link_options | 3 | 6 | 6-8 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | kernel 目标以 -T arch/${ARCH}/kernel.lds 链接；ARCH 缺省在 options_flags.cmake 设为 x86_64。未运行 CMake。 |
| A46 | CMakeLists.txt::KERNEL_C_SRCS | 2 | 30 | 30-30 | - | OUTSIDE_OR_PARTIAL_DEFINITION | PARTIAL_BOUNDARY | 语义支持“根 CMake 递归收集源码”；但第二行 file(GLOB_RECURSE KERNEL_ASM_SRCS ...) 是另一个变量的定义，落在所标符号 KERNEL_C_SRCS 的定义（第30行单行命令）之外，按 P2 定义体规则为部分失败。 |
| A32 | arch/x86_64/myos/LVT_timer.c::LVT_timer_handler | 1 | 33 | 31-41 | - | MATCH_IN_DEFINITION | SUPPORTS | LVT_timer_handler 递增 lvt_count，不触及 jiffies。 |
| A43 | time/timer/timer.c::process_timeout | 2 | 43 | 41-45 | - | MATCH_IN_DEFINITION | SUPPORTS | process_timeout 取容器后 wake_up_process(timeout->task)。 |
| A35 | include/linux/kernel/asm-generic/bug.h::WARN_ON | 1 | 130 | 130-130 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 定义位于 #ifdef CONFIG_BUG 内；CONFIG_BUG 由 A01 的 -DCONFIG_BUG 定义；所读快照中 WARN_ON 只有这一处活动 #define（其余为注释）。 |
| A36 | .vscode-kdbg/run-qemu-gdb-myos2.sh::(top-level) | 1 | 73 | top-level | - | MATCH_IN_DEFINITION | SUPPORTS | 顶层 QEMU 命令续行；同一命令另有 -smp 1 与 -S；该 drive 行无 readonly/snapshot。 |
| A37 | .vscode-kdbg/run-qemu-gdb-myos2.sh::(top-level) | 1 | 88 | top-level | - | MATCH_IN_DEFINITION | SUPPORTS | 顶层 for 循环中 nc -z 成功即 log_ready；另一分支用 /dev/tcp 探测，含义相同（端口可连）。 |
| A38 | arch/x86_64/lock_IPC/atomic/atomic_arch.h::arch_atomic_add_test_negative | 3 | 170,274 | 270-283 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | 同样三行也出现在 arch_atomic_sub_test_zero（第170行）；单纯 grep -F 会命中两处，定义体边界定位到第274行 arch_atomic_add_test_negative。 |
| A39 | arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h::arch_spin_trylock | 5 | 52 | 49-57 | - | MATCH_IN_DEFINITION | SUPPORTS | arch_spin_trylock 只读 val 并比较 head/tail，无写入。 |
| A47 | arch/x86_64/kernel/myos_APboot.S::AP_DEBUG | 1 | 124 | 111-130 | - | MATCH_IN_DEFINITION | SUPPORTS_WITH_NOTE | SYM_CODE_START(AP_DEBUG) 后第124行 jmp .；定义体终点按“下一个 SYM_*_START/列0标签/EOF”启发式确定。 |

机械汇总 `{"MATCH_IN_DEFINITION": 45, "MATCH_IN_DEFINITION_INCLUDES_COMMENT_LINE": 1, "OUTSIDE_OR_PARTIAL_DEFINITION": 1}`；语义汇总 `{"SUPPORTS_WITH_NOTE": 12, "SUPPORTS": 34, "PARTIAL_BOUNDARY": 1}`。

预处理条件（C 族，命中处）：

- A07：`#ifndef _LINUX_SCHEDULER_H_ / #if defined(SCHEDULER_DEFINATION) || !(DEBUG)`
- A12：`#ifndef _LINUX_SCHEDULER_H_ / #if defined(SCHEDULER_DEFINATION) || !(DEBUG)`
- A13：`#ifndef _LINUX_SCHEDULER_H_ / #if defined(SCHEDULER_DEFINATION) || !(DEBUG)`
- A22：`#ifndef _LINUX_DOUBLE_LIST_H_ / #if defined(LIST_DEFINATION) || !(DEBUG)`
- A24：`#ifndef _LINUX_DOUBLE_LIST_H_ / #if defined(LIST_DEFINATION) || !(DEBUG)`
- A25：`#ifndef _LINUX_DOUBLE_LIST_H_ / #if defined(LIST_DEFINATION) || !(DEBUG)`
- A27：`#ifndef _LINUX_DOUBLE_LIST_MACROS_H_`
- A35：`#ifndef _ASM_GENERIC_BUG_H / #ifndef __ASSEMBLY__ / #ifdef CONFIG_BUG`
- A38：`#ifndef _ASM_X86_ATOMIC_H_ / #if defined(ARCH_ATOMIC_DEFINATION) || !(DEBUG)`
- A39：`#ifndef _ASM_X86_SPINLOCK_H_ / #if defined(ARCH_SPINLOCK_SMP_DEFINATION) || !(DEBUG)`

金丝雀与写入边界：

```json
{
 "canaries": [
  {
   "key": "options_flags_cmake",
   "path": "mykernel/scripts/options_flags.cmake",
   "quote_repr": "'\\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\\\'",
   "time_whole_line": 1,
   "master_substring": 0,
   "verdict": "DISCRIMINATES"
  },
  {
   "key": "panic_c_panic",
   "path": "mykernel/debug/panic.c",
   "quote_repr": "'\\tthis_cpu = smp_processor_id();'",
   "time_whole_line": 1,
   "master_substring": 0,
   "verdict": "DISCRIMINATES"
  }
 ],
 "protection": {
  "work_branch": "claude/dazzling-cori-q0dnyt",
  "head_short12": "0851af4fc08b",
  "head_descends_from_pilot_head": true,
  "pilot_files_changed_since_pilot_head": [],
  "worktree_changes_outside_core_dir": []
 }
}
```

### 3.2 V01：结构与引用完整性

```json
{
 "parse": {
  "report": {
   "ok": true,
   "top_keys": 21
  },
  "map": {
   "ok": true,
   "top_keys": 23
  },
  "contract09": {
   "ok": true,
   "top_keys": 22
  },
  "pilot10": {
   "ok": true,
   "top_keys": 21
  },
  "core11": {
   "ok": true,
   "top_keys": 21
  },
  "manifest": {
   "ok": true,
   "top_keys": 30
  }
 },
 "issue_ids": [
  "CA-01",
  "CA-02",
  "CA-03",
  "CA-04",
  "CA-05",
  "CA-06",
  "CA-07"
 ],
 "issue_ids_expected_CA01_CA07": true,
 "anchor_refs": {
  "in_issues": 51,
  "in_relations": 32,
  "dangling": [],
  "report_anchors_not_referenced_by_any_issue": [],
  "duplicates_within_an_issue": {}
 },
 "case_refs": {
  "dangling": [],
  "cases_not_referenced_by_any_issue": []
 },
 "contract09_anchor_refs_dangling": [],
 "referenced_paths": {
  "map.report": {
   "path": "agent-workspace/lead/MYOS2-LEAD-002/07-scheduler-wakeup-timer-audit.md",
   "exists_taskbook": true
  },
  "map.validation_contract": {
   "path": "agent-workspace/lead/MYOS2-LEAD-002/09-local-verification-contract.md",
   "exists_taskbook": true
  },
  "map.CA-01.old_claim_ref": {
   "path": "agent-workspace/WAVE-1-REVIEW.md",
   "exists_taskbook": true
  }
 },
 "manifest_self_check": {
  "scope_files": [
   "07-scheduler-wakeup-timer-audit.md",
   "07-core-audit-map.yaml",
   "08-conversation-archive-disposition.md",
   "09-local-verification-contract.md",
   "MANIFEST.md",
   "checkpoints/2026-09-24-core-audit-and-archive.md"
  ],
  "tag_counts_per_scope_file": {
   "07-scheduler-wakeup-timer-audit.md": 47,
   "07-core-audit-map.yaml": 0,
   "08-conversation-archive-disposition.md": 0,
   "09-local-verification-contract.md": 0,
   "MANIFEST.md": 0,
   "checkpoints/2026-09-24-core-audit-and-archive.md": 0
  },
  "automated_total": 47,
  "declared_verified_claims": 47,
  "declared_reconfirmed_plus_downgraded": 47,
  "map_expected_quote_count": 47,
  "automated_equals_declared": true,
  "arithmetic_ok": true
 },
 "startup_selfcheck_quote_in_master_conventions": true,
 "completion_flags": {
  "whole_002R_complete": false,
  "whole_003R_complete": false,
  "whole_007R_complete": false,
  "whole_wave2_complete": false,
  "acceptance_verdict": "NOT_ISSUED",
  "kernel_modified": false,
  "map.replacement_for_002R_003R_007R": false
 },
 "hex40_hits_in_scope_files": {
  "07-scheduler-wakeup-timer-audit.md": 0,
  "07-core-audit-map.yaml": 0,
  "08-conversation-archive-disposition.md": 0,
  "09-local-verification-contract.md": 0,
  "MANIFEST.md": 0,
  "2026-09-24-core-audit-and-archive.md": 0
 },
 "p9_wording_hits_in_scope_files": {}
}
```

07 报告 inputs_read 中 33 个路径全部存在：True。

## 4. 宿主夹具（HOST_ORIGINAL_SLICE / MODEL_ONLY）

### 4.1 构建方式与替换依赖

`build.py` 展开模板中的 `//@@ORIG <pin> <path> <kind> <name>`：用与 V00 相同的定位器在固定 blob 中找到唯一定义，逐字复制整段并用 `#line` 指回原文件行号；定位不唯一即报错。编译命令（每个夹具相同）：

```text
gcc -std=gnu11 -O0 -g -fno-strict-aliasing -Wall -Wno-unused-label -Wno-unused-variable -Wno-unused-function -I <fixtures> -o <exe> <fixture>.expanded.c [alias.ld]
```

编译上限 60 秒，每次运行上限 5 秒，每个案例独立进程运行两次并比较事件与退出码；步数上限 100（`__mod_timer` 或 schedule 替身计数）；非 waiter 指针在交给真实函数前由守卫停下（退出码 42），步数到上限退出码 43。

| 夹具 | 原文片段数 | 手写替换（全部） |
|---|---|---|
| fx_wait | 85 | fx_common.h 的整数 typedef/PREFIX_* /READ_ONCE/WRITE_ONCE/likely；task_s 仅 __state+id；current；try_to_wake_up 与 wake_up_process（记录、校验指针、只置 RUNNING、返回 0）；local_irq_*、preempt_* 钩子；schedule（计数，可脚本化调用一次通知方）；timer_list_s 布局；simple_init_timer_key/__mod_timer/timer_delete_sync（仅记录，__mod_timer 施加步数上限）；jiffies 固定为 1000；msecs_to_jiffies 恒等；__sched 为空；signal_pending/__fatal_signal_pending 返回 0；typedef 行 |
| fx_sched | 53 | 同 fx_common.h；task_struct/runqueue 为成员子集（thread_info、__state、on_cpu、se、rt、id / curr、idle、myos），子结构原样；per_cpu(runqueues,cpu) 改为数组下标；current；preempt_*；smp_rmb 为编译器屏障；need_resched() 为夹具标志；jiffies 为变量；BUG_ON 记录并停止；typedef 行 |
| fx_prims | 13 | 同 fx_common.h；typedef 行；__READ_ONCE 为普通 volatile 读（原文用 __unqual_scalar_typeof） |
| fx_jiffies | 2 | jiffies/jiffies_64 声明（jiffies_64 初值 0 而非 INITIAL_JIFFIES）；pt_regs_s；tty 与颜色桩；DEBUG_show_jiffies=false；别名由隐式链接脚本 alias.ld 提供，其唯一一行取自 kernel.lds |
| fx_jiffies_control | 2 | 同 fx_jiffies，但以 -DCONTROL_SEPARATE 给 jiffies 独立存储且不加 alias.ld |

原文片段清单（路径、种类、名称、行范围、字节、SHA-256 第 1 段；按文件去重列出）：

| 路径 | 种类 | 名称 | 行 | 字节 | SHA-256[0] | 用于 |
|---|---|---|---|---|---|---|
| arch/x86_64/include/asm/alternative.h | macro | LOCK_PREFIX | 48-48 | 51 | 9a5b7a0654e64f55 | fx_prims,fx_wait |
| arch/x86_64/include/asm/alternative.h | macro | LOCK_PREFIX_HERE | 41-46 | 181 | b4a3e09c335307e4 | fx_prims,fx_wait |
| arch/x86_64/kernel/hpet.c | func | HPET_handler | 525-537 | 295 | 370ba3406993097a | fx_jiffies,fx_jiffies_control |
| arch/x86_64/lock_IPC/atomic/atomic64_arch.h | func | arch_atomic64_read | 89-93 | 113 | 7c2dcb22c0d604c4 | fx_prims |
| arch/x86_64/lock_IPC/atomic/atomic_arch.h | func | arch_atomic_add_test_negative | 270-283 | 343 | ee240b32ae955309 | fx_prims |
| arch/x86_64/lock_IPC/atomic/atomic_types_arch.h | struct | atomic | 7-9 | 36 | 79de26326600be29 | fx_prims |
| arch/x86_64/lock_IPC/atomic/atomic_types_arch.h | struct | atomic64 | 11-13 | 38 | 88746cc46d19b8f9 | fx_prims,fx_wait |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h | func | arch_spin_init | 36-40 | 130 | cf7bd89531c709bd | fx_prims,fx_wait |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h | func | arch_spin_is_locked | 42-47 | 171 | b925f2a34c84ac52 | fx_prims |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h | func | arch_spin_lock | 59-80 | 652 | f10132ff49db386e | fx_prims,fx_wait |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h | func | arch_spin_trylock | 49-57 | 208 | 943b540e732c1bd4 | fx_prims |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h | func | arch_spin_unlock | 82-92 | 236 | 398186ca12164db8 | fx_prims,fx_wait |
| arch/x86_64/lock_IPC/spinlock/spinlock_smp_macro_arch.h | macro | __ARCH_SPIN_LOCK_UNLOCKED | 8-11 | 87 | c32fded0a8cd7c82 | fx_prims,fx_wait |
| arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h | struct | tspinlock | 8-17 | 106 | 6b43dbd41422b7c4 | fx_prims,fx_wait |
| arch/x86_64/sched/context/thread_info_types_arch.h | struct | thread_info | 19-24 | 189 | 848398c2f9f642ad | fx_sched |
| include/linux/lib/errno.h | macro | ERESTARTSYS | 13-13 | 27 | 8176f6fe1a157cda | fx_wait |
| include/uapi/linux/stddef.h | macro | container_of | 9-13 | 263 | ae12efae715f1169 | fx_sched,fx_wait |
| kactive/completion/completion.c | func | complete | 32-40 | 207 | f7470dc2d5c81485 | fx_wait |
| kactive/completion/completion.c | func | wait_for_completion | 53-55 | 110 | 81402eb4e23497df | fx_wait |
| kactive/completion/completion.h | func | do_wait_for_common | 42-68 | 645 | 12e691ec39b573ba | fx_wait |
| kactive/completion/completion.h | func | wait_for_common | 70-80 | 274 | ab79405fd3d9702b | fx_wait |
| kactive/completion/completion_types.h | struct | completion | 19-22 | 61 | e948137e76e750c8 | fx_wait |
| kactive/swait/swait.c | func | __finish_swait | 52-56 | 203 | 488a6ff2f334d692 | fx_wait |
| kactive/swait/swait.c | func | __init_swait_queue_head | 11-15 | 117 | d70464a1aa57ba6a | fx_wait |
| kactive/swait/swait.c | func | __prepare_to_swait | 46-50 | 193 | 392489bb6f2a01be | fx_wait |
| kactive/swait/swait.c | func | swake_up_all_locked | 40-43 | 121 | 85e418f5556391be | fx_wait |
| kactive/swait/swait.c | func | swake_up_locked | 23-31 | 301 | 8643dea35fe86cb7 | fx_wait |
| kactive/swait/swait_macro.h | macro | DECLARE_SWAITQUEUE | 10-11 | 87 | fee5419c7404dc6e | fx_wait |
| kactive/swait/swait_macro.h | macro | __SWAITQUEUE_INITIALIZER | 5-8 | 131 | 836895663cf6c39c | fx_wait |
| kactive/swait/swait_macro.h | macro | init_swait_queue_head | 21-24 | 105 | 4b62cf034822ac26 | fx_wait |
| kactive/swait/swait_types.h | struct | swait_queue | 12-15 | 63 | 4e68787a6a9e3597 | fx_wait |
| kactive/swait/swait_types.h | struct | swait_queue_head | 7-10 | 77 | aa0d280c38f57622 | fx_wait |
| lib/list/double_list.h | func | INIT_LIST_HEADER_S | 153-158 | 136 | 53598975b6d1421a | fx_sched,fx_wait |
| lib/list/double_list.h | func | INIT_LIST_S | 147-152 | 129 | 6503fe0d66ff1558 | fx_sched,fx_wait |
| lib/list/double_list.h | func | __list_add_between | 187-197 | 248 | 9147161d2d4ba947 | fx_sched,fx_wait |
| lib/list/double_list.h | func | __list_add_valid | 160-168 | 247 | 3ed8f4cedd0f9fda | fx_sched,fx_wait |
| lib/list/double_list.h | func | __list_del | 233-238 | 133 | 4b2d41eef303900b | fx_sched,fx_wait |
| lib/list/double_list.h | func | __list_del_entry | 253-260 | 162 | 1e6b8fa214378a31 | fx_sched,fx_wait |
| lib/list/double_list.h | func | __list_del_entry_valid | 169-179 | 306 | 13f263662c49e0ff | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_add_to_next | 206-210 | 127 | 1388a0b7eda6e30b | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_add_to_prev | 219-223 | 127 | 69a63f65475e8438 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_del_init | 278-283 | 120 | 69ca4826f39fa5d2 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_add_to_head | 644-649 | 157 | bdf1ccbe85075372 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_add_to_tail | 662-667 | 157 | e7451841f25dda30 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_contains | 633-642 | 212 | d2c885447e83747d | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_delete_node | 681-691 | 224 | 239d2d2bf75873c5 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_is_empty | 628-632 | 125 | 6d2bbba923422901 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_header_remove_head | 650-660 | 245 | c36e35500b6e3970 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_is_empty_entry | 428-432 | 118 | a917ea3ed5156f20 | fx_sched,fx_wait |
| lib/list/double_list.h | func | list_is_head_anchor | 419-423 | 121 | 2115a38e9262a008 | fx_sched,fx_wait |
| lib/list/double_list.h | macro | INIT_LIST_HEAD | 125-125 | 48 | 0c5f3ae9c2784896 | fx_sched,fx_wait |
| lib/list/double_list.h | macro | list_is_head | 132-132 | 42 | 7eadc5411f450e4d | fx_sched,fx_wait |
| lib/list/double_list_const.h | macro | LIST_POISON1 | 5-5 | 39 | 4208b26ec9006c27 | fx_sched,fx_wait |
| lib/list/double_list_const.h | macro | LIST_POISON2 | 6-6 | 39 | 7e513a95411558d5 | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | LIST_HEADER_INIT | 9-12 | 106 | fd948458e96905d1 | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | LIST_INIT | 5-8 | 65 | 3a4d4e1a6894cb52 | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_container | 25-26 | 80 | 98d6a6e49e50f5a3 | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_entry | 27-27 | 34 | 18fedd137eb90cba | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_first_entry | 37-38 | 88 | 9158858037a4a4dc | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_for_each | 88-91 | 126 | 0d8764ba976cf21c | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_header_foreach | 340-341 | 83 | 63e3a3c429a91383 | fx_sched,fx_wait |
| lib/list/double_list_macro.h | macro | list_headr_first_container | 334-335 | 109 | 68549932735e1f43 | fx_sched,fx_wait |
| lib/list/double_list_types.h | struct | list_hdr | 13-16 | 56 | d248e3e72255d766 | fx_sched,fx_wait |
| lib/list/double_list_types.h | struct | list_head | 8-11 | 57 | ebf1c027f3463abc | fx_sched,fx_wait |
| lock_IPC/signal/signal.h | func | signal_pending_state | 498-507 | 261 | 3b31a35d29a5fcf9 | fx_wait |
| lock_IPC/spinlock/spinlock_smp.h | func | raw_spin_lock_irq | 72-78 | 149 | 748649dcc846086c | fx_wait |
| lock_IPC/spinlock/spinlock_smp.h | func | raw_spin_lock_irqsave | 63-69 | 170 | 3da2f322937ed1d4 | fx_wait |
| lock_IPC/spinlock/spinlock_smp.h | func | raw_spin_unlock_irq | 115-121 | 151 | 1ad2b9eadc0f003d | fx_wait |
| lock_IPC/spinlock/spinlock_smp.h | func | raw_spin_unlock_irqrestore | 106-112 | 179 | bc9dbf93f5b0595e | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | raw_spin_lock_init | 67-67 | 54 | 614867e1d4fcba55 | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | spin_lock_init | 124-125 | 60 | 249f4dea7e51c0ba | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | spin_lock_irq | 148-149 | 58 | b14865ea84fc2a5b | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | spin_lock_irqsave | 151-152 | 81 | 7263a35fae6325bb | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | spin_unlock_irq | 166-167 | 62 | 91ea3269eae535d9 | fx_wait |
| lock_IPC/spinlock/spinlock_smp_macro.h | macro | spin_unlock_irqrestore | 169-170 | 91 | f7eabb2b7d0f933e | fx_wait |
| sched/misc/sched_misc_api.h | macro | task_thread_info | 35-35 | 54 | 58d81a63e9d7f50a | fx_sched |
| sched/runqueue/runqueue_macro.h | macro | __set_current_state | 64-67 | 134 | f4b4241155c77bb1 | fx_wait |
| sched/runqueue/runqueue_types.h | struct | myos_rq | 193-197 | 131 | 6470522644a0034b | fx_sched |
| sched/scheduler/myos_rt.c | func | pick_next_task_myos | 5-58 | 1598 | 2ff36b570d0422f3 | fx_sched |
| sched/scheduler/scheduler.h | func | select_task_rq | 56-80 | 808 | c743e172d84c2787 | fx_sched |
| sched/scheduler/scheduler.h | func | task_cpu | 47-51 | 110 | 110ac54418f24183 | fx_sched |
| sched/scheduler/scheduler_const.h | macro | WF_TTWU | 53-53 | 73 | 9ea60426d3212d2a | fx_sched |
| sched/scheduler/scheduler_core.c | func | set_task_cpu | 123-181 | 1906 | 04f3270a1000e7b9 | fx_sched |
| sched/scheduler/scheduler_core.c | func | try_to_wake_up | 328-483 | 5127 | d4ed78c52d17f1cd | fx_sched |
| sched/scheduler/scheduler_core.c | func | wake_up_new_task | 684-722 | 1158 | f89e51f6b1ede267 | fx_sched |
| sched/scheduler/scheduler_core.c | func | wake_up_process | 556-559 | 77 | bdfcdb5beb19c0c7 | fx_sched |
| sched/scheduler/scheduler_types.h | struct | sched_entity | 8-40 | 811 | 2ae073a5d065956e | fx_sched |
| sched/scheduler/scheduler_types.h | struct | sched_rt_entity | 42-58 | 454 | b5dd1d6f5523a96d | fx_sched |
| sched/task/task_const.h | macro | MAX_SCHEDULE_TIMEOUT | 86-86 | 38 | 29697bf0946d3b15 | fx_wait |
| sched/task/task_const.h | macro | TASK_INTERRUPTIBLE | 27-27 | 39 | 9b3420f0602f3495 | fx_sched,fx_wait |
| sched/task/task_const.h | macro | TASK_NEW | 41-41 | 31 | 6efa559c5c3cacc3 | fx_sched |
| sched/task/task_const.h | macro | TASK_NORMAL | 73-75 | 85 | 1aac90836fb20cb7 | fx_sched,fx_wait |
| sched/task/task_const.h | macro | TASK_RUNNING | 26-26 | 34 | 1eb741bd7c9b5641 | fx_sched,fx_wait |
| sched/task/task_const.h | macro | TASK_UNINTERRUPTIBLE | 28-28 | 40 | 160d93bd1d542e1b | fx_sched,fx_wait |
| sched/task/task_const.h | macro | TASK_WAKEKILL | 38-38 | 35 | 0411eee40b11ff31 | fx_wait |
| sched/task/task_const.h | macro | TASK_WAKING | 39-39 | 34 | 7e259640465d7154 | fx_sched |
| sched/task/task_const.h | macro | __TASK_STOPPED | 29-29 | 36 | 423917040ac7f172 | fx_sched |
| time/timekeeping/timekeeping.c | func | do_timer | 270-273 | 52 | 919ca040e3f7b43d | fx_jiffies,fx_jiffies_control |
| time/timer/timer.c | func | msleep | 852-858 | 142 | 249f96a1b94a9e8e | fx_wait |
| time/timer/timer.c | func | process_timeout | 41-45 | 137 | 1d7d2dffb0f2e7f9 | fx_wait |
| time/timer/timer.c | func | schedule_timeout | 759-806 | 1305 | 89e94155be574cd1 | fx_wait |
| time/timer/timer.c | func | schedule_timeout_uninterruptible | 826-830 | 142 | eaae1a722e66687d | fx_wait |
| time/timer/timer.c | struct | process_timer | 32-35 | 84 | 16e79c329ea39368 | fx_wait |
| time/timer/timer_const.h | macro | MOD_TIMER_NOTPENDING | 206-206 | 34 | 484d475fcba5a779 | fx_wait |
| time/timer/timer_macro.h | func | destroy_timer_on_stack | 27-27 | 67 | 0433e8a9d62e97a9 | fx_wait |
| time/timer/timer_macro.h | macro | del_timer_sync | 25-25 | 41 | 7ce800c6eb1a3e4d | fx_wait |
| time/timer/timer_macro.h | macro | from_timer | 6-7 | 123 | 6c3505bcd7ac135d | fx_wait |
| time/timer/timer_macro.h | macro | timer_setup_on_stack | 21-22 | 111 | 1b41531e7a600d95 | fx_wait |

jiffies 别名行：`jiffies = jiffies_64;`（time:mykernel/arch/x86_64/kernel.lds 第 17 行）。

### 4.2 各案例实际输出与判定

判定由 `evaluate.py` 按 07/09 的预测逐项比较；每项另用一条故意错误的预测做负控，必须被判为不符，否则记 COMPARATOR_INVALID（本轮无此情况）。

#### V02

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| prepare.count | `1` | `1` | True |
| prepare.walk_len | `1` | `1` | True |
| wake.links_empty(anchor_self) | `true` | `true` | True |
| wake.count | `1` | `1` | True |
| wake.waiter_self_linked | `true` | `true` | True |
| finish.count | `1` | `1` | True |
| finish.header_is_empty | `false` | `false` | True |
| exit_code | `0` | `0` | True |

命令 `<CORE_WORK>/fx_wait/fx_wait v02_single`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v02_single","ev":"layout","sizeof_lock":8,"offsetof_hdr_task_list_hdr":8,"offsetof_waiter_task_list":8,"anchor_container_task_field_aliases_lock":true}
{"case":"v02_single","ev":"snap","step":"init","count":0,"walk_len":0,"count_equals_nodes":true,"anchor_self":true,"header_is_empty":true,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v02_single","ev":"snap","step":"after_prepare","count":1,"walk_len":1,"count_equals_nodes":true,"anchor_self":false,"header_is_empty":false,"a_self":false,"a_occ":1,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v02_single","ev":"ttwu","call":1,"task":1,"state_before":2,"mask":3}
{"case":"v02_single","ev":"snap","step":"after_wake","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v02_single","ev":"snap","step":"after_finish","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v02_single","ev":"end","ttwu_calls":1}
```

#### V03

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：None

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|

子案例 second_wake_direct：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| invalid_task_passed_to_try_to_wake_up | `true` | `true` | True |
| at_call | `2` | `2` | True |
| task_field_is_queue_lock_word | `true` | `true` | True |
| layout.anchor_container_task_aliases_lock | `true` | `true` | True |
| exit_code(stopped_by_invariant_guard) | `42` | `42` | True |

子案例 second_wake_via_complete：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| invalid_task_passed_to_try_to_wake_up | `true` | `true` | True |
| task_field_is_queue_lock_word | `true` | `true` | True |
| task_ptr_nonzero_while_lock_held | `true` | `true` | True |
| exit_code | `42` | `42` | True |

子案例 all_locked_two_waiters：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| valid_wakeups_in_order | `[1, 2]` | `[1, 2]` | True |
| third_iteration_invalid | `3` | `3` | True |
| exit_code | `42` | `42` | True |

命令 `<CORE_WORK>/fx_wait/fx_wait v03_second_wake_direct`：退出码 42，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v03_second_wake_direct","ev":"layout","sizeof_lock":8,"offsetof_hdr_task_list_hdr":8,"offsetof_waiter_task_list":8,"anchor_container_task_field_aliases_lock":true}
{"case":"v03_second_wake_direct","ev":"snap","step":"init","count":0,"walk_len":0,"count_equals_nodes":true,"anchor_self":true,"header_is_empty":true,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v03_second_wake_direct","ev":"snap","step":"after_prepare","count":1,"walk_len":1,"count_equals_nodes":true,"anchor_self":false,"header_is_empty":false,"a_self":false,"a_occ":1,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v03_second_wake_direct","ev":"ttwu","call":1,"task":1,"state_before":2,"mask":3}
{"case":"v03_second_wake_direct","ev":"snap","step":"after_wake","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v03_second_wake_direct","ev":"snap","step":"after_finish","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":0,"preempt":0}
{"case":"v03_second_wake_direct","ev":"second_wake_begin","lock_held":false}
{"case":"v03_second_wake_direct","ev":"ttwu_invalid_task","call":2,"task_ptr_value":0,"watched_lock_word":0,"equals_watched_lock_word":true}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v03_second_wake_via_complete`：退出码 42，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v03_second_wake_via_complete","ev":"ttwu","call":1,"task":1,"state_before":0,"mask":3}
{"case":"v03_second_wake_via_complete","ev":"snap","step":"after_first_complete","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":4294967297,"preempt":0}
{"case":"v03_second_wake_via_complete","ev":"snap","step":"after_waiter_finish","count":1,"walk_len":0,"count_equals_nodes":false,"anchor_self":true,"header_is_empty":false,"a_self":true,"a_occ":0,"b_self":null,"b_occ":-1,"lock_word":4294967297,"preempt":0}
{"case":"v03_second_wake_via_complete","ev":"second_complete_begin","lock_held_inside":true}
{"case":"v03_second_wake_via_complete","ev":"ttwu_invalid_task","call":2,"task_ptr_value":4294967298,"watched_lock_word":4294967298,"equals_watched_lock_word":true}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v03_all_two_waiters`：退出码 42，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v03_all_two_waiters","ev":"snap","step":"after_two_prepare","count":2,"walk_len":2,"count_equals_nodes":true,"anchor_self":false,"header_is_empty":false,"a_self":false,"a_occ":1,"b_self":false,"b_occ":1,"lock_word":0,"preempt":0}
{"case":"v03_all_two_waiters","ev":"ttwu","call":1,"task":1,"state_before":0,"mask":3}
{"case":"v03_all_two_waiters","ev":"ttwu","call":2,"task":2,"state_before":0,"mask":3}
{"case":"v03_all_two_waiters","ev":"ttwu_invalid_task","call":3,"task_ptr_value":0,"watched_lock_word":0,"equals_watched_lock_word":true}
```

#### V04

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| ret | `0` | `0` | True |
| state_after(RUNNING) | `0` | `0` | True |
| queued_once | `1` | `1` | True |
| rq0.count | `1` | `1` | True |
| preempt_balanced | `0` | `0` | True |

命令 `<CORE_WORK>/fx_sched/fx_sched v04_noncurrent_wake`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v04_noncurrent_wake","ev":"q","step":"before","cpu":0,"count":0,"walk_len":0,"order":[],"task":2,"task_state":2,"task_occ":0,"task_cpu_meta":0,"preempt":0}
{"case":"v04_noncurrent_wake","ev":"ret","fn":"wake_up_process","ret":0}
{"case":"v04_noncurrent_wake","ev":"q","step":"after","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
```

#### V05

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| stopped_via_wake_up_process.state_in_mask | `false` | `false` | True |
| stopped_via_wake_up_process.state_after(RUNNING) | `0` | `0` | True |
| stopped_via_wake_up_process.queued | `1` | `1` | True |
| stopped_via_wake_up_process.ret | `0` | `0` | True |
| uninterruptible_mask_interruptible.state_in_mask | `false` | `false` | True |
| uninterruptible_mask_interruptible.state_after(RUNNING) | `0` | `0` | True |
| uninterruptible_mask_interruptible.queued | `1` | `1` | True |
| uninterruptible_mask_interruptible.ret | `0` | `0` | True |
| task_new_via_wake_up_process.state_in_mask | `false` | `false` | True |
| task_new_via_wake_up_process.state_after(RUNNING) | `0` | `0` | True |
| task_new_via_wake_up_process.queued | `1` | `1` | True |
| task_new_via_wake_up_process.ret | `0` | `0` | True |
| running_not_queued_via_wake_up_process.state_in_mask | `false` | `false` | True |
| running_not_queued_via_wake_up_process.state_after(RUNNING) | `0` | `0` | True |
| running_not_queued_via_wake_up_process.queued | `1` | `1` | True |
| running_not_queued_via_wake_up_process.ret | `0` | `0` | True |
| current_uninterruptible_mask_interruptible.state_in_mask | `false` | `false` | True |
| current_uninterruptible_mask_interruptible.state_after(RUNNING) | `0` | `0` | True |
| current_uninterruptible_mask_interruptible.queued | `0` | `0` | True |
| current_uninterruptible_mask_interruptible.ret | `0` | `0` | True |

命令 `<CORE_WORK>/fx_sched/fx_sched v05_state_not_in_mask`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v05_state_not_in_mask","ev":"mask_case","label":"stopped_via_wake_up_process","state_in":4,"mask":3,"state_in_mask":false,"ret":0}
{"case":"v05_state_not_in_mask","ev":"q","step":"stopped_via_wake_up_process","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v05_state_not_in_mask","ev":"mask_case","label":"uninterruptible_mask_interruptible","state_in":2,"mask":1,"state_in_mask":false,"ret":0}
{"case":"v05_state_not_in_mask","ev":"q","step":"uninterruptible_mask_interruptible","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v05_state_not_in_mask","ev":"mask_case","label":"task_new_via_wake_up_process","state_in":2048,"mask":3,"state_in_mask":false,"ret":0}
{"case":"v05_state_not_in_mask","ev":"q","step":"task_new_via_wake_up_process","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v05_state_not_in_mask","ev":"mask_case","label":"running_not_queued_via_wake_up_process","state_in":0,"mask":3,"state_in_mask":false,"ret":0}
{"case":"v05_state_not_in_mask","ev":"q","step":"running_not_queued_via_wake_up_process","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v05_state_not_in_mask","ev":"mask_case","label":"current_uninterruptible_mask_interruptible","state_in":2,"mask":1,"state_in_mask":false,"ret":0}
{"case":"v05_state_not_in_mask","ev":"q","step":"current_path","cpu":0,"count":0,"walk_len":0,"order":[],"task":2,"task_state":0,"task_occ":0,"task_cpu_meta":0,"preempt":0}
```

#### V06

判定：**NO_FAILURE_IN_SCOPE**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| after_first.occurrences | `1` | `1` | True |
| after_second.occurrences | `1` | `1` | True |
| after_third.occurrences | `1` | `1` | True |
| after_first.count | `1` | `1` | True |
| after_second.count | `1` | `1` | True |
| after_third.count | `1` | `1` | True |
| returns | `[0, 0, 0]` | `[0, 0, 0]` | True |

命令 `<CORE_WORK>/fx_sched/fx_sched v06_double_wake`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v06_double_wake","ev":"q","step":"after_first","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v06_double_wake","ev":"q","step":"after_second","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v06_double_wake","ev":"q","step":"after_third","cpu":0,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":0,"preempt":0}
{"case":"v06_double_wake","ev":"rets","r1":0,"r2":0,"r3":0}
```

#### V07

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| set_task_cpu(A,1).queued_on_rq1 | `1` | `1` | True |
| set_task_cpu(A,1).task_cpu_meta_unchanged(3) | `3` | `3` | True |
| wake(B meta=2).queued_on_rq0 | `1` | `1` | True |
| wake(B meta=2).not_on_rq2 | `0` | `0` | True |
| new_task(C meta=3).queued_on_rq0 | `1` | `1` | True |
| new_task(C meta=3).task_cpu_meta_unchanged(3) | `3` | `3` | True |

命令 `<CORE_WORK>/fx_sched/fx_sched v07_cpu_metadata`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v07_cpu_metadata","ev":"q","step":"set_task_cpu_A_to_1_rq1","cpu":1,"count":1,"walk_len":1,"order":[2],"task":2,"task_state":0,"task_occ":1,"task_cpu_meta":3,"preempt":0}
{"case":"v07_cpu_metadata","ev":"ret","fn":"wake_up_process","ret":0}
{"case":"v07_cpu_metadata","ev":"q","step":"wake_B_meta2_rq0","cpu":0,"count":1,"walk_len":1,"order":[3],"task":3,"task_state":0,"task_occ":1,"task_cpu_meta":2,"preempt":0}
{"case":"v07_cpu_metadata","ev":"q","step":"wake_B_meta2_rq2","cpu":2,"count":0,"walk_len":0,"order":[],"task":3,"task_state":0,"task_occ":0,"task_cpu_meta":2,"preempt":0}
{"case":"v07_cpu_metadata","ev":"q","step":"new_task_C_meta3_rq0","cpu":0,"count":2,"walk_len":2,"order":[4,3],"task":4,"task_state":0,"task_occ":1,"task_cpu_meta":3,"preempt":0}
{"case":"v07_cpu_metadata","ev":"q","step":"new_task_C_meta3_rq3","cpu":3,"count":0,"walk_len":0,"order":[],"task":4,"task_state":0,"task_occ":0,"task_cpu_meta":3,"preempt":0}
```

#### V08

子项 function_level_combinations：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| a_running_empty.returns_current | `true` | `true` | True |
| b_blocked_empty.returns_current | `true` | `true` | True |
| c_blocked_one_runnable.returns_other | `3` | `3` | True |
| c.blocked_current_not_requeued(count 0) | `0` | `0` | True |
| d_idle.requeued_at_tail | `[0]` | `[0]` | True |
| e_blocked_only_idle.returns_idle | `0` | `0` | True |

子项 sequence_with_idle_requeue：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| s1.idle_in_list_after_switch_out | `true` | `true` | True |
| s2.idle_still_in_list | `true` | `true` | True |
| s3.blocked_last_task_yields_idle | `0` | `0` | True |

子项 sequence_when_idle_switched_out_blocked：**OBSERVED_AS_PREDICTED**（负控检出：True）

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| t1.idle_not_requeued | `false` | `false` | True |
| t2.blocked_current_returned_with_empty_queue | `true` | `true` | True |

附加观察 extra_observation_requeue_order：`{"order_before_ids": [3, 4, 5], "vruntimes": {"2": 25, "3": 10, "4": 20, "5": 30}, "order_after": [4, 5, 2], "vruntime_sorted_would_be": [4, 2, 5]}`


命令 `<CORE_WORK>/fx_sched/fx_sched v08_pick_combinations`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v08_pick_combinations","ev":"pick_in","name":"a_running_empty","current":2,"current_state":0,"current_is_idle":false,"need_resched":1,"order_before":[],"count_before":0}
{"case":"v08_pick_combinations","ev":"pick_out","name":"a_running_empty","returned":2,"returned_is_current":true,"rq_curr":2,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
{"case":"v08_pick_combinations","ev":"pick_in","name":"b_blocked_empty","current":2,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[],"count_before":0}
{"case":"v08_pick_combinations","ev":"pick_out","name":"b_blocked_empty","returned":2,"returned_is_current":true,"rq_curr":2,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
{"case":"v08_pick_combinations","ev":"pick_in","name":"c_blocked_one_runnable","current":2,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[3],"count_before":1}
{"case":"v08_pick_combinations","ev":"pick_out","name":"c_blocked_one_runnable","returned":3,"returned_is_current":false,"rq_curr":3,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
{"case":"v08_pick_combinations","ev":"pick_in","name":"d_idle_one_runnable","current":0,"current_state":0,"current_is_idle":true,"need_resched":0,"order_before":[3],"count_before":1}
{"case":"v08_pick_combinations","ev":"pick_out","name":"d_idle_one_runnable","returned":3,"returned_is_current":false,"rq_curr":3,"order_after":[0],"count_after":1,"walk_len_after":1,"idle_in_list":true}
{"case":"v08_pick_combinations","ev":"pick_in","name":"e_blocked_queue_only_idle","current":2,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[0],"count_before":1}
{"case":"v08_pick_combinations","ev":"pick_out","name":"e_blocked_queue_only_idle","returned":0,"returned_is_current":false,"rq_curr":0,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
```

命令 `<CORE_WORK>/fx_sched/fx_sched v08_sequence_idle_requeue`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v08_sequence_idle_requeue","ev":"pick_in","name":"s1_idle_switches_out","current":0,"current_state":0,"current_is_idle":true,"need_resched":0,"order_before":[3,2],"count_before":2}
{"case":"v08_sequence_idle_requeue","ev":"pick_out","name":"s1_idle_switches_out","returned":3,"returned_is_current":false,"rq_curr":3,"order_after":[2,0],"count_after":2,"walk_len_after":2,"idle_in_list":true}
{"case":"v08_sequence_idle_requeue","ev":"pick_in","name":"s2_first_blocks","current":3,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[2,0],"count_before":2}
{"case":"v08_sequence_idle_requeue","ev":"pick_out","name":"s2_first_blocks","returned":2,"returned_is_current":false,"rq_curr":2,"order_after":[0],"count_after":1,"walk_len_after":1,"idle_in_list":true}
{"case":"v08_sequence_idle_requeue","ev":"pick_in","name":"s3_second_blocks","current":2,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[0],"count_before":1}
{"case":"v08_sequence_idle_requeue","ev":"pick_out","name":"s3_second_blocks","returned":0,"returned_is_current":false,"rq_curr":0,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
```

命令 `<CORE_WORK>/fx_sched/fx_sched v08_sequence_idle_switched_out_blocked`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v08_sequence_idle_switched_out_blocked","ev":"pick_in","name":"t1_blocked_idle_switches_out","current":0,"current_state":2,"current_is_idle":true,"need_resched":0,"order_before":[2],"count_before":1}
{"case":"v08_sequence_idle_switched_out_blocked","ev":"pick_out","name":"t1_blocked_idle_switches_out","returned":2,"returned_is_current":false,"rq_curr":2,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
{"case":"v08_sequence_idle_switched_out_blocked","ev":"pick_in","name":"t2_only_task_blocks","current":2,"current_state":2,"current_is_idle":false,"need_resched":0,"order_before":[],"count_before":0}
{"case":"v08_sequence_idle_switched_out_blocked","ev":"pick_out","name":"t2_only_task_blocks","returned":2,"returned_is_current":true,"rq_curr":2,"order_after":[],"count_after":0,"walk_len_after":0,"idle_in_list":false}
```

命令 `<CORE_WORK>/fx_sched/fx_sched v08_vruntime_requeue_order`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v08_vruntime_requeue_order","ev":"pick_in","name":"f_running_requeue_by_vruntime","current":2,"current_state":0,"current_is_idle":false,"need_resched":1,"order_before":[3,4,5],"count_before":3}
{"case":"v08_vruntime_requeue_order","ev":"pick_out","name":"f_running_requeue_by_vruntime","returned":3,"returned_is_current":false,"rq_curr":3,"order_after":[4,5,2],"count_after":3,"walk_len_after":3,"idle_in_list":false}
```

#### V09

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| in=0.ret | `0` | `0` | True |
| in=0.schedule_calls | `0` | `0` | True |
| in=0.state_after(UNINTERRUPTIBLE=2) | `2` | `2` | True |
| in=1.ret | `1` | `1` | True |
| in=1.schedule_calls | `0` | `0` | True |
| in=1.state_after(UNINTERRUPTIBLE=2) | `2` | `2` | True |
| in=5.ret | `5` | `5` | True |
| in=5.schedule_calls | `0` | `0` | True |
| in=5.state_after(UNINTERRUPTIBLE=2) | `2` | `2` | True |
| in=-1.ret | `0` | `0` | True |
| in=-1.state_after(RUNNING) | `0` | `0` | True |
| in=MAX.schedule_calls | `1` | `1` | True |
| in=MAX.ret | `9223372036854775807` | `9223372036854775807` | True |
| wrapper(5).ret | `5` | `5` | True |
| wrapper(5).state_after | `2` | `2` | True |
| msleep(5).stopped_at_step_cap | `100` | `100` | True |
| msleep.first_timeouts | `[5, 5, 5]` | `[5, 5, 5]` | True |
| msleep.last_timeout | `5` | `5` | True |
| msleep.schedule_calls | `0` | `0` | True |
| msleep.never_returned | `false` | `false` | True |

命令 `<CORE_WORK>/fx_wait/fx_wait v09_schedule_timeout_values`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v09_schedule_timeout_values","ev":"st","in":0,"ret":0,"sched_calls":0,"mod_timer_calls":1,"del_calls":1,"state_after":2}
{"case":"v09_schedule_timeout_values","ev":"st","in":1,"ret":1,"sched_calls":0,"mod_timer_calls":1,"del_calls":1,"state_after":2}
{"case":"v09_schedule_timeout_values","ev":"st","in":5,"ret":5,"sched_calls":0,"mod_timer_calls":1,"del_calls":1,"state_after":2}
{"case":"v09_schedule_timeout_values","ev":"st","in":-1,"ret":0,"sched_calls":0,"mod_timer_calls":0,"del_calls":0,"state_after":0}
{"case":"v09_schedule_timeout_values","ev":"schedule","call":1,"current":1,"current_state":2}
{"case":"v09_schedule_timeout_values","ev":"st","in":9223372036854775807,"ret":9223372036854775807,"sched_calls":1,"mod_timer_calls":0,"del_calls":0,"state_after":2}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v09_uninterruptible_wrapper`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v09_uninterruptible_wrapper","ev":"stu","in":5,"ret":5,"sched_calls":0,"state_after":2}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v09_msleep_bounded`：退出码 43，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v09_msleep_bounded","ev":"msleep_begin","msecs":5,"cap":100}
{"case":"v09_msleep_bounded","ev":"step_cap","where":"__mod_timer","cap":100,"first_timeouts":[5,5,5],"last_timeout":5,"sched_calls":0,"current_state":2}
```

#### V10

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| preset.schedule_calls | `0` | `0` | True |
| preset.done_consumed | `0` | `0` | True |
| preset.ret_is_MAX | `true` | `true` | True |
| preset.wait_for_completion.schedule_calls | `0` | `0` | True |
| infinite.schedule_calls | `1` | `1` | True |
| infinite.returned | `true` | `true` | True |
| infinite.done_consumed | `0` | `0` | True |
| infinite.state_after(RUNNING) | `0` | `0` | True |
| finite(no notifier).bounded_non_progress | `100` | `100` | True |
| finite.schedule_calls | `0` | `0` | True |

命令 `<CORE_WORK>/fx_wait/fx_wait v10_done_preset_fast_path`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v10_done_preset_fast_path","ev":"wfc_common","ret_is_max":true,"done_after":0,"sched_calls":0,"count":0,"state_after":0}
{"case":"v10_done_preset_fast_path","ev":"wait_for_completion","done_after":0,"sched_calls":0,"count":0}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v10_infinite_notify_during_schedule`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v10_infinite_notify_during_schedule","ev":"schedule","call":1,"current":1,"current_state":2}
{"case":"v10_infinite_notify_during_schedule","ev":"notifier_start","waiter_state":2,"done":0,"count":1,"walk_len":1}
{"case":"v10_infinite_notify_during_schedule","ev":"ttwu","call":1,"task":1,"state_before":2,"mask":3}
{"case":"v10_infinite_notify_during_schedule","ev":"notifier_end","waiter_state":0,"done":1,"count":1,"walk_len":0}
{"case":"v10_infinite_notify_during_schedule","ev":"returned","ret_is_max":true,"done_after":0,"sched_calls":1,"count":1,"walk_len":0,"state_after":0}
```

命令 `<CORE_WORK>/fx_wait/fx_wait v10_finite_timeout_no_notifier`：退出码 43，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v10_finite_timeout_no_notifier","ev":"begin","timeout":5,"cap":100}
{"case":"v10_finite_timeout_no_notifier","ev":"step_cap","where":"__mod_timer","cap":100,"first_timeouts":[5,5,5],"last_timeout":5,"sched_calls":0,"current_state":2}
```

#### V11

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| waiter_state_at_schedule(UNINTERRUPTIBLE) | `2` | `2` | True |
| queued_before_notify.count | `1` | `1` | True |
| queued_before_notify.walk_len | `1` | `1` | True |
| after_notify.waiter_state(RUNNING) | `0` | `0` | True |
| after_notify.done | `1` | `1` | True |
| after_notify.count(stays 1) | `1` | `1` | True |
| after_notify.walk_len | `0` | `0` | True |
| after_return.count(stays 1) | `1` | `1` | True |
| after_return.done | `0` | `0` | True |
| reuse.complete_hits_anchor_container | `true` | `true` | True |
| exit_code | `42` | `42` | True |

命令 `<CORE_WORK>/fx_wait/fx_wait v11_wait_then_notify_then_reuse`：退出码 42，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v11_wait_then_notify_then_reuse","ev":"schedule","call":1,"current":1,"current_state":2}
{"case":"v11_wait_then_notify_then_reuse","ev":"notifier_start","waiter_state":2,"done":0,"count":1,"walk_len":1}
{"case":"v11_wait_then_notify_then_reuse","ev":"ttwu","call":1,"task":1,"state_before":2,"mask":3}
{"case":"v11_wait_then_notify_then_reuse","ev":"notifier_end","waiter_state":0,"done":1,"count":1,"walk_len":0}
{"case":"v11_wait_then_notify_then_reuse","ev":"waiter_returned","done":0,"count":1,"walk_len":0,"anchor_self":true,"header_is_empty":false,"state":0,"sched_calls":1,"ttwu_calls":1}
{"case":"v11_wait_then_notify_then_reuse","ev":"reuse_complete_begin","note":"waiter stack object no longer exists"}
{"case":"v11_wait_then_notify_then_reuse","ev":"ttwu_invalid_task","call":2,"task_ptr_value":12884901892,"watched_lock_word":12884901892,"equals_watched_lock_word":true}
```

#### V12

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| (-1,1).after_is_subtraction | `-2` | `-2` | True |
| (-1,1).ret_is_sign_of_subtraction | `true` | `true` | True |
| (1,2).after_is_subtraction | `-1` | `-1` | True |
| (1,2).ret_is_sign_of_subtraction | `true` | `true` | True |
| (2,-1).after_is_subtraction | `3` | `3` | True |
| (2,-1).ret_is_sign_of_subtraction | `false` | `false` | True |

命令 `<CORE_WORK>/fx_prims/fx_prims v12_add_test_negative`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v12_add_test_negative","ev":"vec","init":-1,"i":1,"after":-2,"ret":true,"contract_after_add":0,"contract_ret":false,"sub_after":-2,"sub_ret":true,"after_matches_contract":false,"ret_matches_contract":false,"after_matches_sub":true}
{"case":"v12_add_test_negative","ev":"vec","init":1,"i":2,"after":-1,"ret":true,"contract_after_add":3,"contract_ret":false,"sub_after":-1,"sub_ret":true,"after_matches_contract":false,"ret_matches_contract":false,"after_matches_sub":true}
{"case":"v12_add_test_negative","ev":"vec","init":2,"i":-1,"after":3,"ret":false,"contract_after_add":1,"contract_ret":false,"sub_after":3,"sub_ret":false,"after_matches_contract":false,"ret_matches_contract":true,"after_matches_sub":true}
{"case":"v12_add_test_negative","ev":"vec","init":0,"i":0,"after":0,"ret":false,"contract_after_add":0,"contract_ret":false,"sub_after":0,"sub_ret":false,"after_matches_contract":true,"ret_matches_contract":true,"after_matches_sub":true}
{"case":"v12_add_test_negative","ev":"vec","init":5,"i":3,"after":2,"ret":false,"contract_after_add":8,"contract_ret":false,"sub_after":2,"sub_ret":false,"after_matches_contract":false,"ret_matches_contract":true,"after_matches_sub":true}
```

#### V13

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| init.val | `0` | `0` | True |
| first_trylock.ret(success) | `1` | `1` | True |
| first_trylock.val_unchanged | `0` | `0` | True |
| first_trylock.is_locked_after | `0` | `0` | True |
| second_trylock_without_unlock.ret(success again) | `1` | `1` | True |
| control.trylock_while_held_by_arch_spin_lock.ret | `0` | `0` | True |

命令 `<CORE_WORK>/fx_prims/fx_prims v13_trylock`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v13_trylock","ev":"lock","step":"after_init","ret":-1,"val":0,"head":0,"tail":0,"is_locked":0}
{"case":"v13_trylock","ev":"lock","step":"after_first_trylock","ret":1,"val":0,"head":0,"tail":0,"is_locked":0}
{"case":"v13_trylock","ev":"lock","step":"after_second_trylock_without_unlock","ret":1,"val":0,"head":0,"tail":0,"is_locked":0}
{"case":"v13_trylock","ev":"lock","step":"after_arch_spin_lock","ret":-1,"val":1,"head":1,"tail":0,"is_locked":1}
{"case":"v13_trylock","ev":"lock","step":"trylock_while_held_by_arch_spin_lock","ret":0,"val":1,"head":1,"tail":0,"is_locked":1}
{"case":"v13_trylock","ev":"lock","step":"after_arch_spin_unlock","ret":-1,"val":4294967297,"head":1,"tail":1,"is_locked":0}
```

#### V14

判定：**OBSERVED_AS_PREDICTED**；负控检出错误预测：True

| 检查项 | 实测 | 预测 | 一致 |
|---|---|---|---|
| alias.same_address | `true` | `true` | True |
| alias.jiffies_64_delta_per_handler | `2` | `2` | True |
| control.same_address | `false` | `false` | True |
| control.jiffies_64_delta_per_handler | `1` | `1` | True |

命令 `<CORE_WORK>/fx_jiffies/fx_jiffies v14`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v14","ev":"one_handler_call","same_address":true,"jiffies_64_delta":2,"jiffies_delta":2,"sizeof_jiffies":8,"sizeof_jiffies_64":8,"tty_calls":0}
```

命令 `<CORE_WORK>/fx_jiffies_control/fx_jiffies_control v14_control`：退出码 0，超时 False，重复一致 True，stderr 0 字节。stdout：

```json
{"case":"v14_control","ev":"one_handler_call","same_address":false,"jiffies_64_delta":1,"jiffies_delta":1,"sizeof_jiffies":8,"sizeof_jiffies_64":8,"tty_calls":0}
```

## 5. 静态核查（static_checks.py）

在固定 time 提交的 mykernel/ 下 843 个 .c/.h/.S/.lds 文件中检索，“活动”指去除 C 注释后该行仍有文本。检索为正则，未见不等于全仓不存在。

```json
{
 "V05": {
  "ttwu_doc_contract": [
   {
    "line": 297,
    "text": "* Conceptually does:"
   },
   {
    "line": 299,
    "text": "*   If (@state & @p->state) @p->state = TASK_RUNNING."
   },
   {
    "line": 325,
    "text": "* Return: %true if @p->state changes (an actual wakeup was done),"
   }
  ],
  "sched_fork_contract": [
   {
    "line": 623,
    "text": "* We mark the process as NEW here. This guarantees that"
   },
   {
    "line": 624,
    "text": "* nobody will actually run it, and a signal or other external"
   },
   {
    "line": 625,
    "text": "* event cannot wake it up and insert it on the runqueue either."
   }
  ],
  "ttwu_state_match_active_calls": []
 },
 "V07": {
  "active___set_task_cpu": [],
  "active_thread_info_cpu_writes": [],
  "wake_up_new_task_rq_line": [
   {
    "line": 688,
    "text": "rq_s *rq = &(per_cpu(runqueues, 0));"
   }
  ],
  "ttwu_select_line": [
   {
    "line": 462,
    "text": "cpu = select_task_rq(p, cpu, wake_flags | WF_TTWU);"
   }
  ]
 },
 "V08": {
  "active_running_lhdr_sites": [
   {
    "path": "mykernel/sched/runqueue/runqueue_types.h",
    "line": 195,
    "text": "List_hdr_s\t\trunning_lhdr;"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 18,
    "text": "myos_rq->running_lhdr.count > 0)"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 21,
    "text": "myos_rq->running_lhdr.count <= 0);"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 24,
    "text": "List_s * next_lp = list_header_remove_head(&myos_rq->running_lhdr);"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 33,
    "text": "list_header_add_to_tail(&myos_rq->running_lhdr, &rt->run_list);"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 37,
    "text": "List_s * tmp_list = myos_rq->running_lhdr.anchor.next;"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 40,
    "text": "tmp_list != &myos_rq->running_lhdr.anchor)"
   },
   {
    "path": "mykernel/sched/scheduler/myos_rt.c",
    "line": 45,
    "text": "myos_rq->running_lhdr.count++;"
   },
   {
    "path": "mykernel/sched/scheduler/scheduler_core.c",
    "line": 179,
    "text": "if (!list_header_contains(&target_rq->myos.running_lhdr, &p->rt.run_list))"
   },
   {
    "path": "mykernel/sched/scheduler/scheduler_core.c",
    "line": 180,
    "text": "list_header_add_to_head(&target_rq->myos.running_lhdr, &p->rt.run_list);"
   },
   {
    "path": "mykernel/sched/scheduler/scheduler_core.c",
    "line": 721,
    "text": "list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);"
   },
   {
    "path": "mykernel/sched/scheduler/scheduler_core.c",
    "line": 1458,
    "text": "INIT_LIST_HEADER_S(&rq->myos.running_lhdr);"
   }
  ],
  "active_rq_idle_assignments": [
   {
    "path": "mykernel/sched/scheduler/scheduler_core.c",
    "line": 1436,
    "text": "rq->idle = idle;"
   }
  ],
  "init_task_state_initializer": [
   {
    "path": "mykernel/init/init_task.c",
    "line": 76,
    "text": ".__state\t\t\t= TASK_RUNNING,"
   }
  ],
  "schedule_idle_expects_running": [
   {
    "line": 1236,
    "text": "WARN_ON_ONCE(current->__state);"
   }
  ],
  "rest_init_state_changes": [],
  "active_idle_state_writers": []
 },
 "V09": {
  "finite_path_lines": [
   {
    "line": 797,
    "text": "__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);"
   },
   {
    "line": 798,
    "text": "// schedule();"
   },
   {
    "line": 799,
    "text": "del_timer_sync(&timer.timer);"
   },
   {
    "line": 805,
    "text": "return timeout < 0 ? 0 : timeout;"
   }
  ],
  "active_process_timeout_references": [
   {
    "path": "mykernel/time/timer/timer.c",
    "line": 42,
    "text": "process_timeout(timer_list_s *t) {"
   },
   {
    "path": "mykernel/time/timer/timer.c",
    "line": 796,
    "text": "timer_setup_on_stack(&timer.timer, process_timeout, 0);"
   }
  ],
  "active_msleep_references_outside_timer_c": [
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 30,
    "text": "void msleep(uint msecs);"
   }
  ],
  "active_timer_expiry_dispatch_candidates": [],
  "active_schedule_timeout_family_references_outside_timer_c": [
   {
    "path": "mykernel/kactive/completion/completion.h",
    "line": 76,
    "text": "timeout = do_wait_for_common(x, schedule_timeout, timeout, state);"
   },
   {
    "path": "mykernel/lock_IPC/semaphore/semaphore.h",
    "line": 57,
    "text": "timeout = schedule_timeout(timeout);"
   },
   {
    "path": "mykernel/sched/scheduler/scheduler_api.h",
    "line": 35,
    "text": "extern long schedule_timeout(long timeout);"
   },
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 24,
    "text": "long schedule_timeout(long timeout);"
   },
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 25,
    "text": "long schedule_timeout_interruptible(long timeout);"
   },
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 26,
    "text": "long schedule_timeout_killable(long timeout);"
   },
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 27,
    "text": "long schedule_timeout_uninterruptible(long timeout);"
   },
   {
    "path": "mykernel/time/timer/timer_api.h",
    "line": 28,
    "text": "long schedule_timeout_idle(long timeout);"
   }
  ]
 },
 "V14": {
  "lds_alias": [
   {
    "line": 17,
    "text": "jiffies = jiffies_64;"
   }
  ],
  "target_link_options": [
   {
    "line": 7,
    "text": "-T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds"
   }
  ],
  "arch_default": [
   {
    "line": 8,
    "text": "set(ARCH x86_64)"
   }
  ],
  "active_c_definitions_of_jiffies": [],
  "active_c_definitions_of_jiffies_64": [
   {
    "path": "mykernel/time/timer/timer.c",
    "line": 38,
    "text": "__visible u64 jiffies_64 __cacheline_aligned_in_smp = INITIAL_JIFFIES;"
   }
  ],
  "late_time_init_callers": [
   {
    "path": "mykernel/arch/x86_64/kernel/time.c",
    "line": 42,
    "text": "void late_time_init(void)"
   },
   {
    "path": "mykernel/include/linux/init/init.h",
    "line": 169,
    "text": "extern void late_time_init(void);"
   },
   {
    "path": "mykernel/init/main.c",
    "line": 177,
    "text": "late_time_init();"
   }
  ],
  "active_extern_jiffies_declarations": [
   {
    "path": "mykernel/lock_IPC/semaphore/semaphore_api.h",
    "line": 12,
    "text": "extern int __must_check down_timeout(sema_t *sem, long jiffies);"
   },
   {
    "path": "mykernel/time/misc/time_misc_api.h",
    "line": 12,
    "text": "extern void jiffies_to_timespec64(const ulong jiffies, timespec64_s *value);"
   },
   {
    "path": "mykernel/time/systick/systick_api.h",
    "line": 25,
    "text": "extern ulong volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;"
   }
  ],
  "hpet_registration": [
   {
    "path": "mykernel/arch/x86_64/include/asm/time.h",
    "line": 8,
    "text": "extern void hpet_time_init(void);"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/hpet.c",
    "line": 539,
    "text": "void myos_HPET_init()"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/hpet.c",
    "line": 553,
    "text": "0, &HPET_int_controller, &HPET_handler);"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/time.c",
    "line": 31,
    "text": "void __init hpet_time_init(void)"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/time.c",
    "line": 37,
    "text": "extern void myos_HPET_init(void);"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/time.c",
    "line": 38,
    "text": "myos_HPET_init();"
   },
   {
    "path": "mykernel/arch/x86_64/kernel/time.c",
    "line": 51,
    "text": "hpet_time_init(); /* == x86_init.timers.timer_init() */"
   }
  ],
  "elf_files_in_time_tree": []
 }
}
```

## 6. 开发迭代中的失败与修正（正式运行之前）

- V00 初版把引文中的空行误计为“注释行”（A06）；改为只统计原文有字而去注释后为空的行。A15 的 `// schedule();` 仍如实计为注释行（引文本意如此）。
- V01 初版悬空引用表达式写法冗余，改为直接集合差；结果不变（无悬空）。
- fx_sched 初版观察函数的循环变量名为 p，而原版 container_of 宏内部也声明局部变量 p，导致自引用取到垃圾值并在 v06/v07 段错误；改名后正常。已检索内核：所读快照中未见以 p 为实参调用 container_of/list_entry/list_container。
- fx_jiffies 初版直接比较 &jiffies 与 &jiffies_64，被编译器按“两个不同声明地址必不同”折叠为 false；改为经 volatile 整数在运行期比较，nm 亦显示两符号同址。
- fx_wait 中 __always_inline 与 glibc 定义重名产生警告，改为先 #undef。
- 以上均为夹具/检查脚本自身问题；被测原函数文本未改动。上表与 §4 的结果全部来自修正后的一次正式运行（run_all.py，退出码 0，stderr 0 字节）。

## 7. 提交、推送与远端回读

正式运行命令：

```text
$ cd <repo>/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/fixtures
$ export CORE_WORK=<scratch>/core-work/canonical && rm -rf "$CORE_WORK" && mkdir -p "$CORE_WORK"
$ python3 run_all.py > $CORE_WORK/run_all.stdout 2> $CORE_WORK/run_all.stderr; echo "run_all exit=$?"
run_all exit=0
$ python3 make_results.py ../results.yaml
wrote ../results.yaml (44237 bytes, 1150 lines)
```

run_all.py 标准输出：

```json
{
  "gate": {
    "disposition_is_ALLOW_CORE": true,
    "packet_matches": true,
    "pr_matches_17": true,
    "branch_matches": true,
    "reviewed_head_matches_pin": true,
    "reviewed_inputs_match_pins": true,
    "ALLOW_CORE_bound_to_this_execution": true
  },
  "remote": {
    "master==pin.master": true,
    "time==pin.time": true,
    "agent/MYOS2-LEAD-002==pin.review": true,
    "claude/dazzling-cori-q0dnyt==pin.pilot_head": true
  },
  "stages": {
    "h00_hardening.py": [
      0,
      false
    ],
    "v00_anchors.py": [
      0,
      false
    ],
    "v01_structure.py": [
      0,
      false
    ],
    "static_checks.py": [
      0,
      false
    ]
  },
  "fixtures": {
    "fx_wait": "BUILT",
    "fx_sched": "BUILT",
    "fx_prims": "BUILT",
    "fx_jiffies": "BUILT",
    "fx_jiffies_control": "BUILT"
  },
  "evaluation": {
    "V02": "OBSERVED_AS_PREDICTED",
    "V03": "OBSERVED_AS_PREDICTED",
    "V09": "OBSERVED_AS_PREDICTED",
    "V10": "OBSERVED_AS_PREDICTED",
    "V11": "OBSERVED_AS_PREDICTED",
    "V04": "OBSERVED_AS_PREDICTED",
    "V05": "OBSERVED_AS_PREDICTED",
    "V06": "NO_FAILURE_IN_SCOPE",
    "V07": "OBSERVED_AS_PREDICTED",
    "V08": {
      "function_level_combinations": "OBSERVED_AS_PREDICTED",
      "sequence_with_idle_requeue": "OBSERVED_AS_PREDICTED",
      "sequence_when_idle_switched_out_blocked": "OBSERVED_AS_PREDICTED",
      "extra_observation_requeue_order": null
    },
    "V12": "OBSERVED_AS_PREDICTED",
    "V13": "OBSERVED_AS_PREDICTED",
    "V14": "OBSERVED_AS_PREDICTED"
  }
}
```

第一批提交只含 results.yaml 与 fixtures/，提交 `7e2fa84a9823` 后推送（输出经 40 位十六进制过滤，实际无替换）：

```text
To https://github.com/08822407d/MyOS2
   0851af4..7e2fa84  claude/dazzling-cori-q0dnyt -> claude/dazzling-cori-q0dnyt
branch 'claude/dazzling-cori-q0dnyt' set up to track 'origin/claude/dazzling-cori-q0dnyt'.
```

随后 `readback.py` 从 GitHub 两个通道读取 results.yaml，并与提交 `7e2fa84a9823` 中的 blob 比较（readback exit=0）：

```json
{
 "branch": "claude/dazzling-cori-q0dnyt",
 "commit_short12": "7e2fa84a9823",
 "path": "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/results.yaml",
 "remote_head_equals_commit": true,
 "expected_blob_available": true,
 "expected_bytes": 44237,
 "expected_sha256_segments": [
  "f842678f6841fe9c",
  "81bc68354f117f14",
  "6e41bb39bee666b0",
  "5aab87cd1149c740"
 ],
 "channels": [
  {
   "channel": "raw.githubusercontent.com",
   "url": "https://raw.githubusercontent.com/08822407d/MyOS2/claude/dazzling-cori-q0dnyt/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/results.yaml",
   "curl_exit": 0,
   "timed_out": false,
   "http_code": "200",
   "curl_stderr": "",
   "bytes": 44237,
   "sha256_segments": [
    "f842678f6841fe9c",
    "81bc68354f117f14",
    "6e41bb39bee666b0",
    "5aab87cd1149c740"
   ],
   "transport_ok": true,
   "byte_identical": true,
   "field": {
    "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01"
   },
   "field_ok": true,
   "ok": true
  },
  {
   "channel": "api.github.com_contents_raw",
   "url": "https://api.github.com/repos/08822407d/MyOS2/contents/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/results.yaml?ref=claude/dazzling-cori-q0dnyt",
   "curl_exit": 0,
   "timed_out": false,
   "http_code": "200",
   "curl_stderr": "",
   "bytes": 44237,
   "sha256_segments": [
    "f842678f6841fe9c",
    "81bc68354f117f14",
    "6e41bb39bee666b0",
    "5aab87cd1149c740"
   ],
   "transport_ok": true,
   "byte_identical": true,
   "field": {
    "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01"
   },
   "field_ok": true,
   "ok": true
  }
 ],
 "ok": true
}
```

results.yaml 自此冻结；本文件与 MANIFEST.md 在第二批提交中新增，不回填 results.yaml。第二批提交前的范围检查见下。

### 7.1 第二批提交前的范围检查（final_check.py）

说明：results.yaml 第 1150 行是否定表述（原文“结果不是任何范围的 MyOS2 …；验收上限仍为 PASS_PENDING_LOCAL”），不是通过声明；检查脚本只列出行号供读者判断。此脚本的早期版本曾因同一原因把该行计为失败并在嵌入后自我命中，已改为只报告行号（开发期修正，正式交付前完成）。下方为最终暂存状态的输出，嵌入本节后重跑结果逐字节相同。
```json
{
  "check": "FINAL_SCOPE_CORE",
  "index_vs_master_count": 26,
  "outside_pilot_core": [],
  "non_added": [],
  "worktree_outside": [],
  "pilot_changed_since_pilot_head": [],
  "results_yaml_changed_since_results_commit": false,
  "core_files_staged": 23,
  "binary_like_core_files": [],
  "hex40_hits": {},
  "secret_pattern_hits": [],
  "docs": {
    "MANIFEST.md": {
      "yaml_parses": true,
      "first_key": "task_id",
      "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
      "verified_tag_count": 0,
      "other_pass_token_lines": []
    },
    "evidence.md": {
      "yaml_parses": true,
      "first_key": "task_id",
      "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
      "verified_tag_count": 0,
      "other_pass_token_lines": []
    },
    "results.yaml": {
      "yaml_parses": true,
      "first_key": "task_id",
      "packet_id": "MYOS2-LEAD-002-CORE-CHECK-01",
      "verified_tag_count": 0,
      "other_pass_token_lines": [
        1150
      ]
    }
  },
  "manifest_self_check_consistent": true,
  "startup_selfcheck_quote_in_conventions": true,
  "all_as_expected": true
}
```

