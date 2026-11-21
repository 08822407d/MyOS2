---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-INTEGRATION-03
phase: scoped_source_integration
record_type: scheduler_integration_evidence
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_surface: "claude.ai/code 托管云端会话容器；只读 git 对象查询与 Python 文本处理（git 2.43.0、Python 3.11.15，均为容器既有工具）；与 pilot/core/recheck/scheduler-order-02 同一会话、同一执行分支"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 40bc4faa4202（16 号任务书与 CORE-SCHED-ORDER-02 审查回执，开工时固定）；被审执行头 f36b89b8a53a；见证结果 5078686e8267。均为短 SHA"
work_branch: claude/dazzling-cori-q0dnyt
execution_pr: 17
results_commit_short12: b7fa83583e35
acceptance_ceiling: PASS_PENDING_LOCAL
read_channel: mixed
read_channel_detail: "16 号任务书经 raw URL 读取并与 git 对象逐字节比较；审查回执、检查点、源码与旧记录均按固定对象 git show / git grep；PR 状态经 GitHub MCP；结果回读经 raw.githubusercontent.com 与 api.github.com"
self_check:
  scope: this_file_only
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
  note: "本文件不新增源码断言标签；源码引文的机械核对结果见 §3。"
---

# 证据：实际命令、检索范围、引文核对与未执行声明

本文件记录本次实际做了什么、怎么做的、得到什么输出。分析结论见 integration.md，结构化事实与回归规格见 facts-and-regressions.yaml。

## 1. 开工核对

- **任务书。** 16 号任务书经 raw URL 读取：HTTP 200，13993 字节，与主线对象 40bc4faa4202 中的同名文件逐字节相同（`cmp`）。
- **其余输入。** 审查回执 `reviews/CORE-SCHED-ORDER-02-review.md`（13696 字节）与检查点（5297 字节）按同一对象 `git show` 读取。执行头 f36b89b8a53a 的 `scheduler-order-02/MANIFEST.md` 也按对象读取（14153 字节）。
- **主线变化。** 主线自 6706013a079a 到 40bc4faa4202 只新增三件：16 号任务书、审查回执、检查点；远端主线头与 40bc4faa4202 相同。
- **PR17。** 开工时为 open、Ready（非 Draft）、未合并，头为 f36b89b8a53a。按 16 号 §5 在任何写入前转回 Draft 并复用；同时 open 的只有主线 PR16（写区为 lead/，与本 PR 不相交）。

`check_scope.py start` 的门（records/scope_start.json）：

```json
{
 "contract_binding": true,
 "review_binding": true,
 "lead_pin_advanced_by_additions_only": true,
 "remote_lead_appends_only_after_pin": true,
 "on_execution_branch": true,
 "head_contains_f36": true,
 "f36_files_unchanged_only_additions_under_prefix": true,
 "worktree_clean_outside_prefix": true,
 "remote_on_same_line": true,
 "kernel_source_equals_pin": true,
 "order02_manifest_present": true,
 "ok": true
}
```

回执字段（record_id、packet、followup、PR17、分支、f36 被审头、5078 冻结结果、PASS_PENDING_LOCAL、15 号合同无剩余退回项）与 16 号任务书字段（allowed_write_prefix、kernel/patch/host fixture 授权均为 false 等）逐项核对，全部一致（同一文件 `contract_fields`、`review_fields`）。

## 2. D01 定点检索

命令形式（全部对提交对象执行，不检出、不编译）：

```text
git grep -n -I --no-color -w -F -e <词> a039d9803ade -- mykernel      # mode = word
git grep -n -I --no-color -E -e <正则> a039d9803ade -- mykernel       # mode = regex
git show a039d9803ade:<path>                                           # 读取命中文件以定位符号、注释与条件
```

完整 argv、每条命中的路径/行/逐字文本/所在符号/注释判定/预处理条件/分类，以及别名与定义定位都在 records/d01_index.json，由 `scripts/scan_index.py` 重建。汇总：44 个查询，错误退出 无，495 行命中，逐行分类 398 行，通用 list 原语在 `double_list.h` 与 `sched/` 之外的 97 行只计数，读取 152 个文件，需定位的 46 个定义未能定位：__set_task_cpu、myos_wake_up_new_task、fair_sched_class、idle_sched_class、rt_sched_class、dl_sched_class、stop_sched_class、HAVE_ARCH_BUG_ON、ttwu_queue、ttwu_state_match。

| 查询 | 模式 | 方式 | 退出码 | 行 | 文件 | 逐行分类 | 只计数 |
|---|---|---|---|---|---|---|---|
| F01 | `running_lhdr` | word | 0 | 12 | 3 | 声明/注释 1、活动 11 | 0 |
| F02 | `last_jiffies` | word | 0 | 3 | 2 | 声明/注释 1、活动 2 | 0 |
| F03 | `vruntime` | word | 0 | 5 | 4 | 声明/注释 1、活动 4 | 0 |
| F04 | `run_list` | word | 0 | 11 | 4 | 声明/注释 2、活动 9 | 0 |
| F05 | `(\.\|->)myos\b` | regex | 0 | 5 | 2 | 活动 5 | 0 |
| F06 | `myos_rq` | word | 0 | 13 | 3 | 声明/注释 3、活动 10 | 0 |
| F07 | `myos_rt_sched_class` | word | 0 | 2 | 2 | 声明/注释 1、活动 1 | 0 |
| F08 | `time_slice` | word | 0 | 4 | 4 | 声明/注释 2、活动 2 | 0 |
| F09 | `jiffies_64` | word | 0 | 25 | 6 | 声明/注释 21、活动 4 | 0 |
| F10 | `\b(sum_exec_runtime\|exec_start\|prev_sum_exec_runtime)\b` | regex | 0 | 10 | 4 | 声明/注释 10 | 0 |
| F11 | `\b(nr_switches\|nvcsw\|nivcsw)\b` | regex | 0 | 7 | 4 | 声明/注释 5、活动 2 | 0 |
| F12 | `on_rq` | word | 0 | 42 | 6 | 声明/注释 41、活动 1 | 0 |
| E01 | `set_task_cpu` | word | 0 | 8 | 2 | 声明/注释 6、活动 2 | 0 |
| E02 | `try_to_wake_up` | word | 0 | 19 | 7 | 声明/注释 15、活动 4 | 0 |
| E03 | `wake_up_process` | word | 0 | 20 | 15 | 声明/注释 7、活动 13 | 0 |
| E04 | `wake_up_new_task` | word | 0 | 4 | 3 | 声明/注释 2、活动 2 | 0 |
| E05 | `__sched_fork` | word | 0 | 4 | 1 | 声明/注释 2、活动 2 | 0 |
| E06 | `sched_fork` | word | 0 | 4 | 3 | 声明/注释 2、活动 2 | 0 |
| E07 | `init_idle` | word | 0 | 7 | 3 | 声明/注释 5、活动 2 | 0 |
| E08 | `sched_init` | word | 0 | 4 | 4 | 声明/注释 2、活动 2 | 0 |
| E09 | `__schedule` | word | 0 | 25 | 5 | 条件未解 1、声明/注释 18、活动 6 | 0 |
| E10 | `pick_next_task_myos` | word | 0 | 2 | 1 | 活动 2 | 0 |
| E11 | `pick_next_task` | word | 0 | 9 | 4 | 条件未解 1、声明/注释 5、活动 3 | 0 |
| E12 | `__pick_next_task` | word | 0 | 3 | 2 | 条件未解 1、声明/注释 1、活动 1 | 0 |
| E13 | `for_each_class` | word | 0 | 2 | 2 | 条件未解 1、活动 1 | 0 |
| E14 | `DEFINE_SCHED_CLASS` | word | 0 | 2 | 2 | 活动 2 | 0 |
| E15 | `sched_class\s*=[^=]` | regex | 0 | 4 | 1 | 声明/注释 3、活动 1 | 0 |
| E16 | `wake_up_state` | word | 0 | 8 | 5 | 声明/注释 6、活动 2 | 0 |
| E17 | `resched_curr` | word | 0 | 5 | 3 | 声明/注释 3、活动 2 | 0 |
| E18 | `schedule` | word | 0 | 59 | 25 | 声明/注释 48、活动 11 | 0 |
| E19 | `dup_task_struct` | word | 0 | 3 | 1 | 声明/注释 1、活动 2 | 0 |
| E20 | `kernel_clone` | word | 0 | 4 | 3 | 声明/注释 1、活动 3 | 0 |
| E21 | `kernel_thread` | word | 0 | 9 | 5 | 声明/注释 5、活动 4 | 0 |
| L01 | `list_header_add_to_head` | word | 0 | 15 | 8 | 条件未解 1、声明/注释 1、活动 2 | 11 |
| L02 | `list_header_add_to_tail` | word | 0 | 31 | 21 | 条件未解 1、声明/注释 1、活动 8 | 21 |
| L03 | `list_header_remove_head` | word | 0 | 6 | 5 | 条件未解 1、声明/注释 1、活动 2 | 2 |
| L04 | `list_header_remove_tail` | word | 0 | 7 | 6 | 条件未解 1、声明/注释 1、活动 1 | 4 |
| L05 | `list_header_delete_node` | word | 0 | 18 | 11 | 条件未解 1、声明/注释 1、活动 1 | 15 |
| L06 | `list_header_contains` | word | 0 | 13 | 5 | 条件未解 2、声明/注释 1、活动 1 | 9 |
| L07 | `list_header_is_empty` | word | 0 | 11 | 6 | 条件未解 1、声明/注释 1、活动 2 | 7 |
| L08 | `INIT_LIST_HEADER_S` | word | 0 | 27 | 17 | 条件未解 1、声明/注释 1、活动 3 | 22 |
| L09 | `list_add_to_prev` | word | 0 | 9 | 4 | 条件未解 3、声明/注释 2、活动 2 | 2 |
| L10 | `list_add_to_next` | word | 0 | 11 | 4 | 条件未解 3、声明/注释 5、活动 1 | 2 |
| L11 | `list_header_foreach` | word | 0 | 3 | 3 | 条件未解 1 | 2 |

分类规则（脚本内 `CLASS_RULES`）：

- declaration_or_comment: every match on the line is inside a comment, or the code part is a declaration (extern, prototype ending in ';', struct/union member) or a commented-out line
- visible_active_text: a match in code that is not a declaration and has no enclosing unresolved preprocessor condition (include guards ignored; conditions on CMAKE_DEFINES count as resolved)
- conditional_unresolved: as visible_active_text, but at least one enclosing #if/#ifdef/#ifndef/#else depends on a macro that is not in CMAKE_DEFINES (e.g. DEBUG, file-local defines)
- unlocated: a needed definition with no definition site found by this script (see definitions[])

已记录的别名：局部指针别名 1 条、单标识符宏别名 3 条（见 records/d01_index.json `aliases`），条件与链接别名的人工解析见 facts-and-regressions.yaml `aliases_and_conditions`。**直接文本检索的边界**：direct text search only: an alias, macro expansion, function pointer or computed access that does not spell the searched token is not covered; absence of a hit is not absence of a writer or caller

## 3. 引文机械核对

`scripts/verify_anchors.py` 在 time 对象上逐行比对（records/anchors_check.json）：

- facts-and-regressions.yaml `source_facts`：151 / 151 条引文与所写行号逐字一致，且位于所写符号内（或为该行定义的宏、文件作用域命名）；每条 1–5 行。
- integration.md：10 / 10 个 VERIFIED 标签核对通过。
- 主线审查回执的 G01–G07 七条锚点单独机械复核：7 / 7 通过。只作本轮定点计数，不并入旧的 47 条分母，也不重跑整批核验。

| 锚点 | 路径 | 符号 | 行数 | 实际位置 | 结果 |
|---|---|---|---|---|---|
| G01 | mykernel/scripts/options_flags.cmake | CMAKE_C_FLAGS | 1 | 第 66 行起 | 通过 |
| G02 | mykernel/sched/scheduler/scheduler_core.c | set_task_cpu | 4 | 第 177 行起 | 通过 |
| G03 | mykernel/sched/scheduler/scheduler_core.c | wake_up_new_task | 1 | 第 721 行起 | 通过 |
| G04 | mykernel/sched/scheduler/scheduler_core.c | __sched_fork | 1 | 第 582 行起 | 通过 |
| G05 | mykernel/sched/scheduler/scheduler_core.c | init_idle | 2 | 第 1457 行起 | 通过 |
| G06 | mykernel/lib/list/double_list.h | list_header_add_to_head | 2 | 第 647 行起 | 通过 |
| G07 | mykernel/sched/scheduler/myos_rt.c | pick_next_task_myos | 2 | 第 49 行起 | 通过 |

## 4. 并行只读分析与对抗核验

D02–D03 的源码阅读，按 初始化/idle、新任务首次入队、睡眠与唤醒、回插与分派、计费、链表原语与结构 六个读取面并行进行。每个读取面由一个只读分析代理完成，再由一个独立核验代理逐条复核其引文、符号、分类与推导；最后由一个完整性审查代理对照 16 号 D01–D03 找缺项。

所有代理都只能用 `git show` / `git grep` 读固定对象，不能写仓库，不能编译或运行。它们的原始输出是中间材料，没有作为交付文件；被采用的内容全部重新写成 facts-and-regressions.yaml 中的条目，并由 §3 的脚本机械核对。

- 分析代理提交事实 484 条、推导 86 条、缺口 52 条。
- 核验结论：事实 484 条中 CONFIRMED 462、OVERREACH 18、WRONG_SYMBOL 3、WRONG_LOCATION 1。
- 推导复核：推导 86 条中 SOUND 49、NEEDS_PREMISE 34、UNSOUND 3。
- 核验代理补报的遗漏事实共 154 条，完整性审查列出缺项 31 条。

以下是因核验或审查而改动或保留的要点（反证优先保留）：

- DV-11（现有代码在“摘除后队列为空”时已经经 anchor 求出伪任务指针并读取）由回插/分派读取面提出，计费与结构两个读取面独立得到同一结论，核验后采纳；措辞按核验意见改为“伪 task_s 指针读取”，不称“越界”（布局偏移未读）。它同时更正了 SCHED-ORDER-02 与本报告初稿中“只改游标才会读 anchor 容器”的说法。
- 更正：pick 读的 need_resched() 是 current 的 TIF 标志，do_idle 的 set_preempt_need_resched 只改 preempt 计数位，不使 need_resched() 为真（初稿曾写 do_idle 置 need_resched）。
- DV-19 由“有限超时分支不调度”改为条件推导。核验代理称 del_timer_sync 在树内没有定义；主线复查发现它是 timer_delete_sync 的宏别名（timer_macro.h:25），后者经 __timer_delete_sync 以 cpu_relax 忙等，timer.c 内不调度——核验的这一点被反驳，但“timer.c 之外的被调函数未读”的限制保留。
- B-R3“首次入队可保持头插”加上前提：只在 B-R1/B-R2 已使队列有序之后成立（计费读取面的“统一顺序只需改 set_task_cpu”被核验判为越界，未采纳）。
- “set_task_cpu 不更新任务 CPU 元数据”改为“活动文本中没有写 task_thread_info(p)->cpu”，避免借用上游 __set_task_cpu 的语义（新任务读取面核验）。
- 结构不变量 I4 的例外窗口延长到真实上下文切换之前；“永久自旋”标为源码语义，优化编译后的行为列为 GAP-11（结构读取面核验）。
- 按完整性审查新增：C1 分三步；C3 离队四类入口与 do_task_dead；C2×C3 接合处一行（含“已被选中正在运行”的第三种结果）；两种切入方式；三条切换/计费路径；DV-23（写状态后、检查条件前被中断切出）；DV-24（sched_class 的来历）；统一别名表；缺口状态（开放/收窄）。
- “未能定位”清单由 scan_index.py 机械记录：__set_task_cpu、myos_wake_up_new_task、上游各调度类对象、HAVE_ARCH_BUG_ON、ttwu_queue、ttwu_state_match 只有注释或 extern 命中。完整性审查把 del_timer_sync 列为未定位，经复查为已定位的宏别名，此点未采纳。
- 所有“唯一/只有”类说法统一加上“直接文本命中（别名、函数指针、汇编、宏生成名不排除）”的限定。
- cpu_rq 在读取面之间的说法矛盾，以 runqueue_macro.h:88 的定义解决（SF-C1-26）。
- 明确 W01–W08 只执行了 pick_next_task_myos；首次入队与唤醒只引用冻结的 V04–V07，且限于其已审结论。
- 保留的反证与未采纳项：W 夹具 idle 键为 0/7，源码为 2^64-1（不改变 W01/W02/W07 的结论，交 NR-4 复核）；新任务读取面关于 used_jiffies 起点的说法被核验反驳，未采纳；结构读取面 L6-D04 后半与 L6-D08 的行区间错误被核验指出，未采纳。

D04/D05 另由一个独立的只读审查代理复核：逐项手算 NR-1…NR-6 的预期，检查候选 A/B 的要求文本、Owner 取舍与合同符合性。它报告 17 处问题，全部改正，主要有：（1）NR-6 补上 need_resched、idle 键与完整的 6 次 pick 及逐步计费；（2）W04 在原函数中不是并列见证；（3）W01 不支撑 A-R3/A-R4，A-R4 改由 NR-3 子例 2 与 NR-6 的 idle 键 0 变体检验；（4）候选 A 会改变 W07 的记录；（5）B 只让高键唤醒失去插队，新任务仍在队首；B 选中的是不含 current 的最小键；（6）OD-2 的不答处理改为“不准备 B”，并补上影响；（7）DV-08 的例子顺序修正；（8）NR-3 的读访问检测改为按 offsetof 放守护页，无法检测时记 INCOMPLETE_EVIDENCE。

## 5. 未解宏、别名与条件

- [local_alias] myos_rt.c:7 myos_rq = &rq->myos（pick_next_task_myos 内 running_lhdr/last_jiffies 均经此别名访问）
- [local_alias] myos_rt.c:10 rt = &curr_task->rt（idle 尾插与非 idle 插回都经 &rt->run_list）
- [local_alias] scheduler_core.c:177 target_rq = &(per_cpu(runqueues, new_cpu))；688 rq = &(per_cpu(runqueues, 0))
- [macro_alias] scheduler_core.c:976 #define pick_next_task __pick_next_task
- [macro_alias] double_list.h:127-128 list_add -> list_add_to_next；list_add_tail -> list_add_to_prev
- [macro_alias] runqueue_macro.h:69,85 set_current_state 与 set_special_state 都展开为 __set_current_state
- [linker_alias] kernel.lds:17 jiffies = jiffies_64（同一地址）
- [fixed_cursor] myos_rt.c:38 tmp_rt 只在循环前由 tmp_list 算一次，此后不随游标更新（W01 的来源）
- [macro_alias] current → get_current() → 每 CPU pcpu_hot.current_task；另有独立的每 CPU 变量 current_task（percpu_area.c:14），二者不是同一对象
- [macro_alias] runqueue_macro.h:88 cpu_rq(cpu) = &per_cpu(runqueues, (cpu))：init_idle、wake_up_new_task、set_task_cpu 用的是同一族对象
- [macro_alias] task_macro.h:14 need_resched() = test_tsk_need_resched(current)（TIF 标志，不是 preempt 位）
- [call_alias] wake_up_process、wake_up_state、swake_up_locked 都直接进入 try_to_wake_up
- [macro_alias] timer_macro.h:25 del_timer_sync → timer_delete_sync → __timer_delete_sync
- [macro_alias] double_list.h:133 list_empty → list_is_empty_entry（O(1) 自环判定）
- [condition] scheduler.h:45 #if defined(SCHEDULER_DEFINATION) || !(DEBUG)：DEBUG 取决于 CMAKE_BUILD_TYPE（options_flags.cmake 16-23 行）；scheduler_core.c 第 1 行定义 SCHEDULER_DEFINATION，故该文件内两种构建类型都编译这些定义；其他包含者在 DEBUG 构建中只得到 extern 声明。只解析到这一层。
- [condition] double_list.h:138 #if defined(LIST_DEFINATION) || !(DEBUG)：double_list.c 第 1 行定义 LIST_DEFINATION；非 DEBUG 构建中每个包含者得到同一 static inline 正文。注意 PREFIX_STATIC_INLINE 用 #ifndef DEBUG（看是否定义），这里用 !(DEBUG)（看取值）；若 DEBUG 被定义为 0，两者不一致、本结论不适用。cmake 只给 -DDEBUG 或 -DRELEASE，不会出现这种情形。
- [condition] INITIAL_JIFFIES 在 systick_const.h:69 与 systick_macro.h:227 各定义一次（正文相同）；哪一个生效未追踪。
- [unresolved] in_atomic() 在 excep_hwint_context 中的取值（preempt_count 的实际演变）未追踪：中断后 schedule() 是否真正进入，取决于运行期计数。

缺口（不以“无命中”替代“不存在”）：

- GAP-01：每 CPU 模板中 runqueues 的实际字节与装载未核（只读了模板复制与单元偏移的源码，SF-C1-21/22/23），last_jiffies 初值与其他 CPU 的 running_lhdr 内容只是推导。影响：DV-02 表明 last_jiffies 初值不进入任何非 idle 计费；其他 CPU 不在本规格范围（DV-16）
- GAP-02：in_atomic()/preempt_count 的实际演变未追踪（只知启动初值为 INIT_PREEMPT_COUNT，非 0，SF-C5-27/28）。影响：中断后是否真正进入 schedule()、try_to_wake_up 结尾的 preempt_enable 是否会进入 __schedule 都未定；DV-04 的第一次切换时点因此未定
- GAP-03：switch_to 宏到 __switch_to 的展开链与汇编未逐段引用。影响：只引用了 current_task 的写入行
- GAP-04：链接段 .data.sched_class 的实际内容未经链接器验证。影响：DV-12 以文本命中为前提
- GAP-05：其他 CPU 的 idle/runqueue 初始化与 SMP 启动未读。影响：所有结论限于 CPU0；唤醒与新任务在文本上都进 CPU0（V07）
- GAP-06：经回调/函数指针的间接唤醒者未枚举。影响：直接调用者只列 d01 文本命中
- GAP-07：idle 能否以非 RUNNING 被切出（V08 可达性）。影响：决定 count == 0 分支是否可达
- GAP-08：是否存在在 copy_process 与 wake_up_new_task 之间唤醒新任务的调用者。影响：决定 DV-13 的挂死/破环风险是否可达
- GAP-10：除信号量 down_timeout 外，使用有限超时的 schedule_timeout 活动调用者（含 schedule_timeout_interruptible 等包装的调用者）未枚举；__mod_timer、timer_delete_sync 在 timer.c 之外的被调函数未读。影响：决定 DV-19 的失唤醒/忙等风险是否可达
- GAP-11：源码中的无副作用空循环（__list_add_valid、__list_del_entry_valid、pick 的 while (next_lp == NULL);）在实际编译产物中是否保留为永久自旋。影响：DV-05/DV-13/DV-21 中“自旋挂死”只是源码语义
- GAP-09：HPET 实际中断频率与 jiffies 双增的运行确认。影响：DV-10 只是源码推导
- GAP-NARROWED：已由文本关闭或收窄的读取面缺口（不再列为开放）。影响：select_task_rq 原样返回 cpu（SF-C3-17）；cpu_rq 即 &per_cpu(runqueues, cpu)（SF-C1-26）；DEBUG 只在 Debug 构建定义、为二选一（options_flags.cmake 16-23 行）；类段在直接文本中只有 myos_rt（DV-12，仍为推导）；jiffies 与 jiffies_64 同宽（SF-ACC-06）；del_timer_sync 是 timer_delete_sync 的宏别名（SF-C3-31）

## 6. 未执行声明

本任务没有做以下事情：

- 编译、汇编或运行任何 C/ASM。
- 重跑 H00、W01–W08、V/M/N 或任何验证器。
- 运行原仓库脚本、cmake、构建、QEMU、内核或硬件探针。
- 安装工具、提权或输出凭据。
- 修改内核或任何 f36b89b8a53a 中的已有文件。
- 生成补丁、diff 或 C 实现。
- 推送到主线、master 或 time。

facts-and-regressions.yaml 中的 NR-1…NR-6 全部为 NOT_RUN 的未来规格；W/V 记录只引用其已交与已审范围。

## 7. 提交、推送与回读

- **结果批 `b7fa83583e35`。** 包含 scripts/、records/d01_index.json、records/scope_start.json、records/anchors_check.json、facts-and-regressions.yaml、integration.md。先提交固定，再推送。
- **回读方式。** 推送后用冻结的回读助手（core/recheck-01/fixtures/readback2.py 与 harness2.py，从 f36b89b8a53a 抽取到隔离目录运行，不改原件），按 object（固定提交）与 branch（分支名，要求头相同或后代且同 blob）两种模式，经 raw 与 API 两个通道读取两件主件。
- **回读记录。** 存于 records/after_results/。该助手会把下载体写在 `--out` 旁的临时目录；这些目录已移出仓库，放到会话草稿区，只保留 JSON 记录（其中已有字节数、分段摘要与逐字节一致的判定）。

| 文件 | 模式 | 通道 | HTTP | 字节 | 与提交逐字节一致 | 该模式整体 |
|---|---|---|---|---|---|---|
| facts-and-regressions.yaml | branch | raw.githubusercontent.com | 200 | 130146 | True | True |
| facts-and-regressions.yaml | branch | api.github.com_contents_raw | 200 | 130146 | True | True |
| facts-and-regressions.yaml | object | raw.githubusercontent.com | 200 | 130146 | True | True |
| facts-and-regressions.yaml | object | api.github.com_contents_raw | 200 | 130146 | True | True |
| integration.md | branch | raw.githubusercontent.com | 200 | 32433 | True | True |
| integration.md | branch | api.github.com_contents_raw | 200 | 32433 | True | True |
| integration.md | object | raw.githubusercontent.com | 200 | 32433 | True | True |
| integration.md | object | api.github.com_contents_raw | 200 | 32433 | True | True |

文档批（MANIFEST.md、evidence.md、回读记录）在结果批之后追加，不改结果批任何文件，以避免自引用。

提交结果批前 `check_scope.py stage` 的输出（暂存状态）：

```json
{
 "check": "SCOPE_INTEGRATION_03",
 "mode": "stage",
 "pins": {
  "time": "a039d9803ade",
  "master": "de3bb1df906a",
  "lead_prev": "6706013a079a",
  "lead": "40bc4faa4202",
  "reviewed": "f36b89b8a53a",
  "witness_results": "5078686e8267"
 },
 "contract_fields": {
  "packet_id": [
   "MYOS2-LEAD-002-CORE-CHECK-01",
   true
  ],
  "followup_id": [
   "CORE-SCHED-INTEGRATION-03",
   true
  ],
  "required_review_record": [
   "CORE-SCHED-ORDER-02-REVIEW-001",
   true
  ],
  "reviewed_execution_short12": [
   "f36b89b8a53a",
   true
  ],
  "witness_results_frozen_short12": [
   "5078686e8267",
   true
  ],
  "kernel_short12": [
   "a039d9803ade",
   true
  ],
  "execution_branch": [
   "claude/dazzling-cori-q0dnyt",
   true
  ],
  "initial_execution_pr": [
   17,
   true
  ],
  "allowed_write_prefix": [
   "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/",
   true
  ],
  "acceptance_ceiling": [
   "PASS_PENDING_LOCAL",
   true
  ],
  "kernel_change_authorized": [
   false,
   true
  ],
  "kernel_patch_production_authorized": [
   false,
   true
  ],
  "host_fixture_execution_authorized": [
   false,
   true
  ]
 },
 "review_fields": {
  "record_id": [
   "CORE-SCHED-ORDER-02-REVIEW-001",
   true
  ],
  "packet_id": [
   "MYOS2-LEAD-002-CORE-CHECK-01",
   true
  ],
  "followup_id": [
   "CORE-SCHED-ORDER-02",
   true
  ],
  "reviewed_pr": [
   17,
   true
  ],
  "reviewed_branch": [
   "claude/dazzling-cori-q0dnyt",
   true
  ],
  "reviewed_commit_short12": [
   "f36b89b8a53a",
   true
  ],
  "results_frozen_short12": [
   "5078686e8267",
   true
  ],
  "acceptance_verdict": [
   "PASS_PENDING_LOCAL",
   true
  ],
  "remaining_return_items_in_contract15": [
   [],
   true
  ],
  "next_followup": [
   "CORE-SCHED-INTEGRATION-03",
   true
  ]
 },
 "lead_changes_prev_to_pin": [
  "A\t16-scheduler-integration-contract.md",
  "A\tcheckpoints/2026-10-02-scheduler-witness-reviewed-integration-ready.md",
  "A\treviews/CORE-SCHED-ORDER-02-review.md"
 ],
 "remote_lead_short12": "40bc4faa4202",
 "remote_lead_changes_after_pin": [],
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "f36b89b8a53a",
 "index_vs_f36": [
  "A\t<prefix>facts-and-regressions.yaml",
  "A\t<prefix>integration.md",
  "A\t<prefix>records/anchors_check.json",
  "A\t<prefix>records/d01_index.json",
  "A\t<prefix>records/scope_start.json",
  "A\t<prefix>scripts/check_scope.py",
  "A\t<prefix>scripts/scan_index.py",
  "A\t<prefix>scripts/verify_anchors.py"
 ],
 "worktree_outside_prefix": [],
 "remote_branch_short12": "f36b89b8a53a",
 "remote_branch_relation": "equal",
 "remote_time_equals_pin": true,
 "remote_master_equals_pin": true,
 "order02_manifest_at_f36": {
  "bytes": 14153,
  "followup_id": "CORE-SCHED-ORDER-02"
 },
 "hygiene": {
  "staged_files": [
   "facts-and-regressions.yaml",
   "integration.md",
   "records/anchors_check.json",
   "records/d01_index.json",
   "records/scope_start.json",
   "scripts/check_scope.py",
   "scripts/scan_index.py",
   "scripts/verify_anchors.py"
  ],
  "untracked_in_prefix": [],
  "unstaged_in_prefix": [],
  "binary_like": [],
  "hex40_hits": {},
  "secret_pattern_hits": [],
  "json_parse_failures": [],
  "yaml_parse_failures": [],
  "python_failures": []
 },
 "gate": {
  "contract_binding": true,
  "review_binding": true,
  "lead_pin_advanced_by_additions_only": true,
  "remote_lead_appends_only_after_pin": true,
  "on_execution_branch": true,
  "head_contains_f36": true,
  "f36_files_unchanged_only_additions_under_prefix": true,
  "worktree_clean_outside_prefix": true,
  "remote_on_same_line": true,
  "kernel_source_equals_pin": true,
  "order02_manifest_present": true,
  "hygiene": true,
  "ok": true
 }
}
```

提交文档批前 `check_scope.py docs b7fa83583e35` 的输出：

```json
{
 "check": "SCOPE_INTEGRATION_03",
 "mode": "docs",
 "pins": {
  "time": "a039d9803ade",
  "master": "de3bb1df906a",
  "lead_prev": "6706013a079a",
  "lead": "40bc4faa4202",
  "reviewed": "f36b89b8a53a",
  "witness_results": "5078686e8267"
 },
 "contract_fields": {
  "packet_id": [
   "MYOS2-LEAD-002-CORE-CHECK-01",
   true
  ],
  "followup_id": [
   "CORE-SCHED-INTEGRATION-03",
   true
  ],
  "required_review_record": [
   "CORE-SCHED-ORDER-02-REVIEW-001",
   true
  ],
  "reviewed_execution_short12": [
   "f36b89b8a53a",
   true
  ],
  "witness_results_frozen_short12": [
   "5078686e8267",
   true
  ],
  "kernel_short12": [
   "a039d9803ade",
   true
  ],
  "execution_branch": [
   "claude/dazzling-cori-q0dnyt",
   true
  ],
  "initial_execution_pr": [
   17,
   true
  ],
  "allowed_write_prefix": [
   "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/scheduler-integration-03/",
   true
  ],
  "acceptance_ceiling": [
   "PASS_PENDING_LOCAL",
   true
  ],
  "kernel_change_authorized": [
   false,
   true
  ],
  "kernel_patch_production_authorized": [
   false,
   true
  ],
  "host_fixture_execution_authorized": [
   false,
   true
  ]
 },
 "review_fields": {
  "record_id": [
   "CORE-SCHED-ORDER-02-REVIEW-001",
   true
  ],
  "packet_id": [
   "MYOS2-LEAD-002-CORE-CHECK-01",
   true
  ],
  "followup_id": [
   "CORE-SCHED-ORDER-02",
   true
  ],
  "reviewed_pr": [
   17,
   true
  ],
  "reviewed_branch": [
   "claude/dazzling-cori-q0dnyt",
   true
  ],
  "reviewed_commit_short12": [
   "f36b89b8a53a",
   true
  ],
  "results_frozen_short12": [
   "5078686e8267",
   true
  ],
  "acceptance_verdict": [
   "PASS_PENDING_LOCAL",
   true
  ],
  "remaining_return_items_in_contract15": [
   [],
   true
  ],
  "next_followup": [
   "CORE-SCHED-INTEGRATION-03",
   true
  ]
 },
 "lead_changes_prev_to_pin": [
  "A\t16-scheduler-integration-contract.md",
  "A\tcheckpoints/2026-10-02-scheduler-witness-reviewed-integration-ready.md",
  "A\treviews/CORE-SCHED-ORDER-02-review.md"
 ],
 "remote_lead_short12": "40bc4faa4202",
 "remote_lead_changes_after_pin": [],
 "branch": "claude/dazzling-cori-q0dnyt",
 "head_short12": "b7fa83583e35",
 "index_vs_f36": [
  "A\t<prefix>MANIFEST.md",
  "A\t<prefix>evidence.md",
  "A\t<prefix>facts-and-regressions.yaml",
  "A\t<prefix>integration.md",
  "A\t<prefix>records/after_results/readback_facts-and-regressions.branch.json",
  "A\t<prefix>records/after_results/readback_facts-and-regressions.object.json",
  "A\t<prefix>records/after_results/readback_integration.branch.json",
  "A\t<prefix>records/after_results/readback_integration.object.json",
  "A\t<prefix>records/anchors_check.json",
  "A\t<prefix>records/d01_index.json",
  "A\t<prefix>records/scope_start.json",
  "A\t<prefix>scripts/check_scope.py",
  "A\t<prefix>scripts/scan_index.py",
  "A\t<prefix>scripts/verify_anchors.py"
 ],
 "worktree_outside_prefix": [],
 "remote_branch_short12": "b7fa83583e35",
 "remote_branch_relation": "equal",
 "remote_time_equals_pin": true,
 "remote_master_equals_pin": true,
 "order02_manifest_at_f36": {
  "bytes": 14153,
  "followup_id": "CORE-SCHED-ORDER-02"
 },
 "hygiene": {
  "staged_files": [
   "MANIFEST.md",
   "evidence.md",
   "facts-and-regressions.yaml",
   "integration.md",
   "records/after_results/readback_facts-and-regressions.branch.json",
   "records/after_results/readback_facts-and-regressions.object.json",
   "records/after_results/readback_integration.branch.json",
   "records/after_results/readback_integration.object.json",
   "records/anchors_check.json",
   "records/d01_index.json",
   "records/scope_start.json",
   "scripts/check_scope.py",
   "scripts/scan_index.py",
   "scripts/verify_anchors.py"
  ],
  "untracked_in_prefix": [],
  "unstaged_in_prefix": [],
  "binary_like": [],
  "hex40_hits": {},
  "secret_pattern_hits": [],
  "json_parse_failures": [],
  "yaml_parse_failures": [],
  "python_failures": []
 },
 "results_commit_short12": "b7fa83583e35",
 "non_added_since_results_commit": [],
 "verified_tags_by_file": {
  "MANIFEST.md": 0,
  "evidence.md": 0,
  "integration.md": 10
 },
 "manifest_self_check": {
  "verified_claims": 10,
  "quotes_reconfirmed": 10,
  "downgraded_to_inferred": 0,
  "by_file": {
   "integration.md": 10,
   "evidence.md": 0
  },
  "method": "VERIFIED 标签全部由 scripts/verify_anchors.py 在 time 对象上逐行核对；MANIFEST 与 evidence.md 本身不新增源码标签"
 },
 "manifest_file_list_matches_staged": true,
 "gate": {
  "contract_binding": true,
  "review_binding": true,
  "lead_pin_advanced_by_additions_only": true,
  "remote_lead_appends_only_after_pin": true,
  "on_execution_branch": true,
  "head_contains_f36": true,
  "f36_files_unchanged_only_additions_under_prefix": true,
  "worktree_clean_outside_prefix": true,
  "remote_on_same_line": true,
  "kernel_source_equals_pin": true,
  "order02_manifest_present": true,
  "hygiene": true,
  "docs_consistency": true,
  "ok": true
 }
}
```
