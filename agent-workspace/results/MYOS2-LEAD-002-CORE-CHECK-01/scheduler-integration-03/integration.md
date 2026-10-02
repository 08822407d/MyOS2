---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-SCHED-INTEGRATION-03
phase: scoped_source_integration
record_type: scheduler_integration_report
produced_by: "Claude Code 云端会话执行者（claude.ai/code）；模型名不写入仓库产物，见 execution_model_selection"
execution_model_selection: unknown_or_not_attestable
execution_surface: "claude.ai/code 托管云端会话容器；只读 git 对象查询与 Python 文本处理；与 pilot/core/recheck/scheduler-order-02 同一会话、同一执行分支"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a；主线 40bc4faa4202（16 号任务书与 CORE-SCHED-ORDER-02 审查回执，开工时固定）；被审执行头 f36b89b8a53a；见证结果 5078686e8267。均为短 SHA"
status: static_report_and_candidate_spec
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_change_made: false
patch_or_diff_produced: false
c_or_asm_compiled_or_run: false
read_channel: mixed
read_channel_detail: "16 号任务书经 raw URL 读取并与 git 对象逐字节比较；审查回执、检查点按主线对象 git show；源码只按 time 对象 git show / git grep；W/V 记录按 f36b89b8a53a 对象读取"
self_check:
  scope: this_file_only
  verified_claims: 10
  quotes_reconfirmed: 10
  downgraded_to_inferred: 0
  method: "本文件每个 VERIFIED 标签后的代码块由 scripts/verify_anchors.py 在 time 对象上逐行核对（records/anchors_check.json）"
open_questions:
  - "队列目标契约（OD-1）与键下限（OD-2）待 Owner 决定；本报告给出建议，不代为采用。"
  - "真实运行层（中断时机、in_atomic、SMP、上下文切换、CA-02 全局可达性）未执行。"
---

# 调度器接入关系：只修 pick 能修好什么、修不好什么

**结论先行。** 在 mykernel/ 的直接文本命中里，运行队列 `running_lhdr` 只有四个写入者（别名、函数指针未排除）：`init_idle` 清空、`wake_up_new_task` 无检查头插、`set_task_cpu` 检查后头插、`pick_next_task_myos` 摘队首并插回。选择永远是“摘队首”，从不读键。

所以有两点要分开说。第一，W01/W02 两处回插失序可以只改 `pick_next_task_myos` 修掉，而且不改任何入口政策，这就是候选 A。第二，“全队始终按 vruntime 升序、选中最小键”需要连同普通唤醒与首次入队一起改，这就是候选 B。B 会让键高于队首的被唤醒任务不再插队；新任务键为 0，仍在队首。

建议先实施 A，把 B 作为后续目标契约。B 是否采用、是否给新任务与唤醒任务设键下限，是本轮仅有的两项 Owner 取舍。

本报告只做静态文本查询与推导，没有编译或运行任何 C/ASM，没有改内核，也没有给出补丁。已有 W01–W08 与 V04–V08 只引用，没有重跑。

## 0. 证据层次

| 层 | 含义 | 位置 |
|---|---|---|
| 源码事实 | time a039d9803ade 中的活动文本（或明确标为注释/声明/条件未解），逐字引文 | facts-and-regressions.yaml `source_facts`（151 条），本文 VERIFIED 标签 |
| 直接调用边 | `git grep` 的直接文本命中，不是完整调用图 | records/d01_index.json，yaml `call_edges` |
| 推导 | 标 INFERRED，写出前提与假设 | yaml `derivations`（DV-01…DV-24）、`high_key_wake_state_derivation` |
| 已有见证 | W01–W08（5078686e8267）、V04–V08（core，已审范围） | yaml `witness_refs` |
| 政策提案 | 候选 A/B、OD-1/OD-2；未实施、未授权 | yaml `candidate_changes`、`owner_decisions` |
| 未来回归 | NR-1…NR-6，一律 NOT_RUN | yaml `future_regressions` |

D01 的定点检索共 44 个查询，495 行命中。其中 398 行按“声明或注释 / 可见活动文本 / 条件依赖未解”分类（条件依赖未解 20、声明或注释 235、可见活动文本 143），另有 97 行是 `double_list.h` 与 `sched/` 之外的通用 list 原语使用者，只计数、不逐行分类：这些行都不含 `running_lhdr`（F01 命中全在 `sched/`），但经别名或指针传入运行队列的可能没有排除。需定位的宏和定义中，以下名字在活动文本里找不到定义（只有注释或 extern 命中），归入“未能定位”：__set_task_cpu、myos_wake_up_new_task、fair_sched_class、idle_sched_class、rt_sched_class、dl_sched_class、stop_sched_class、HAVE_ARCH_BUG_ON、ttwu_queue、ttwu_state_match；其余都找到了定义处。

“直接字段检索没有其他命中”在本报告中从不等于“没有其他写入者或调用者”：别名、函数指针、汇编、宏生成的名字都没有被排除。

## 1. 五条生命周期链

五条链在 yaml `chains` 中逐行给出入口与直接调用、state 与键的输入输出、队列与 count 操作、idle 处理、调用者可见返回值、证据与覆盖限制。C1 分三步，C3 分离队与唤醒两步，另有一行“C2×C3 接合处”。每行的覆盖限制都写明：只覆盖 CPU0；CPU≥1 的队列在直接文本中没有初始化，也没有入队者。

### C1 初始化与 idle

`start_kernel` 调 `sched_init`，后者以当时的 `current`（引导任务 `init_task`）调用 `init_idle`。`init_idle` 的 MyOS2 部分只做两件事：把 `rq->curr` 设为 idle，并把队列清空。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::init_idle]
```c
	/* MyOS2 initiations */
	rq->curr = idle;
	INIT_LIST_HEADER_S(&rq->myos.running_lhdr);
```
（SF-C1-11，第 1456–1458 行）

它不调用 `__sched_fork`（该行被注释，SF-C1-08），所以 idle 的键来自静态初值：

[VERIFIED mykernel/init/init_task.c::init_task]
```c
/* MyOS2 initiate members */
	.se					= {
		.vruntime			= -1,
	},
```
（SF-C1-03，第 224–227 行）

`vruntime` 是 `u64`（SF-C1-04），`-1` 即 2^64-1。pick 从不给 idle 计费，所以引导 idle 的键始终是最大值（DV-01）。W 夹具里 idle 取 0 或 7；W01/W02/W07 的失序结论不依赖这个值，但修复后的边界需要按真实键复核（NR-4）。

idle 不在 `init_idle` 中入队。它第一次进入队列，是 `pick_next_task_myos` 以 RUNNING 状态把它切出时的尾插（DV-03）。之后 idle 循环 `do_idle` 的等待段被注释，idle 每轮都进入 `__schedule`（SF-C1-25）。`rest_init` 先用两次 `kernel_thread` 建出 kernel_init 与 kthreadd，再调用 `schedule_preempt_disabled()`（SF-C1-15/16/17）。第一次切换必定发生在 idle 为 current 时（DV-02）。因此 `last_jiffies` 虽然没有显式初始化（启动值很可能为 0，DV-17），它的初值不会计入任何非 idle 任务。

还有两点需要写进规格前提。第一，`init_idle` 写的 `on_rq` 不能当作“是否在队列中”的判据：它只在这里被写一次，活动文本中没有读取，而且 idle 此时并不在队列里（DV-15）。第二，`sched_init` 的每 CPU 循环只写 `rq->cpu` 与 `rq->online`（SF-C1-24），只有引导 CPU 的队列经过 `init_idle` 初始化。现在的唤醒与新任务都固定进 CPU0，所以这不会被触发；但两种候选都必须保留“只用 CPU0 队列”这个前提（DV-16）。

### C2 新任务首次入队

调用链为 `kernel_clone → copy_process → dup_task_struct(current) → sched_fork → __sched_fork`，成功后再 `wake_up_new_task`。`dup_task_struct` 整体复制父任务（SF-C2-05），所以 `sched_class`、`rt.time_slice` 都是继承的。`sched_fork` 中按优先级设置类的代码被注释（SF-C2-09），`__sched_fork` 里重置 `time_slice` 的行也被注释（SF-C2-12）。`__sched_fork` 把键清零：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__sched_fork]
```c
	p->se.vruntime			= 0;
```
（SF-C2-11，第 582 行）

继承来源按调用者不同：`rest_init` 的 `kernel_thread` 以引导 idle 为父，`kthreadd` 中的 `create_kthread` 以 kthreadd 为父，fork 系统调用体以发起它的用户任务为父，各自继承其父的类、`time_slice` 与 `thread_info`。

`wake_up_new_task` 固定取 CPU0 的队列，不检查节点是否已在队列，直接头插：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::wake_up_new_task]
```c
	list_header_add_to_head(&rq->myos.running_lhdr, &p->rt.run_list);
}
```
（SF-C2-16，第 721–722 行）

键为 0 的任务头插进一个原本有序的队列，顺序仍然有序（DV-06）。所以新任务的头插不是失序来源，只是在多个零键任务之间后入者在前。零键只属于这一次首次入队，不能外推到之后的唤醒。

### C3 睡眠后重新唤醒入队

**离队。** `__schedule` 中按 `prev_state` 出队的上游代码整段被注释（SF-C3-03），上游让抢占不出队的条件也是注释（SF-C3-36）。离队完全依赖 pick 不把非 RUNNING 的 current 插回（SF-C3-04）。阻塞者在这次切换中仍被计费，这正是 W05 记录的行为。四类入口共用这一道门：

- 自愿：任务写非 RUNNING 后调用 `schedule()`。
- 中断或异常之后的 `schedule()`。
- `preempt_schedule*` 的 `__schedule(SM_PREEMPT)`。
- `do_task_dead` 写 `TASK_DEAD` 后直接 `__schedule`。若此时 count == 0，它会返回并执行 `BUG()`（SF-C3-37）。

由此有第三种离队时点（DV-23，条件性风险）：任务写了非 RUNNING、还没检查自己的等待条件（如 kthreadd 第 342–343 行之间），就被中断后的 `schedule()` 切出且不插回。若唤醒者早在它写状态之前已经完成，它要等下一次唤醒才能回到队列。这取决于中断上下文中 `in_atomic()` 的取值，未解（GAP-02）。

**唤醒。** `wake_up_process`、`wake_up_state` 和 `swake_up_locked` 都进入 `try_to_wake_up`。`p == current` 时只写 `TASK_RUNNING`，不入队（SF-C3-13）。否则依次写 `TASK_WAKING`、`select_task_rq`（返回传入的 0）、`set_task_cpu(p, 0)`。状态掩码检查被注释（SF-C3-14），所以 `TASK_NEW` 也会被唤醒入队（V05）。`set_task_cpu` 的活动文本是：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::set_task_cpu]
```c
	rq_s *target_rq = &(per_cpu(runqueues, new_cpu));
	p->__state = TASK_RUNNING;
	if (!list_header_contains(&target_rq->myos.running_lhdr, &p->rt.run_list))
		list_header_add_to_head(&target_rq->myos.running_lhdr, &p->rt.run_list);
```
（SF-C3-22，第 177–180 行）

上游同名函数只负责 CPU 迁移记账，这部分在本函数里以注释保留（SF-C3-20/21）。MyOS2 把它改成“写 RUNNING + 条件头插入队”，活动文本中也没有写 `task_thread_info(p)->cpu`（SF-C3-28，V07 宿主观察到元数据不变）。入队是 MyOS2 的副作用，不是同名上游职责。`set_task_cpu` 的活动直接调用只有 `try_to_wake_up:471`；`__set_task_cpu` 在 mykernel/ 中只有注释命中。MyOS2 的 `enqueue_task` 函数体整段被注释，`activate_task` 只调用它（SF-C3-29/30），所以入队只发生在显式的 `list_header_*` 调用里。唤醒路径不写 `se.vruntime`，任务带着睡前累计的键回到队首。

**调用者依赖。** `try_to_wake_up` 返回 `success`，而活动文本中它从未被置为非 0：

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::try_to_wake_up]
```c
	preempt_enable();

	return success;
```
（SF-C3-19，第 480–482 行）

d01 中 12 行 `wake_up_process` 调用全部丢弃返回值。在直接文本命中的调用者中，唯一使用返回值的 `signal_wake_up_state` 因此总会调用 `kick_process`，而后者的活动文本只把 `task_cpu(p)` 读进局部变量（SF-C3-24/25）。结论是：现有调用者不依赖返回值，依赖的是“被唤醒任务尽快运行”这一时延效果。

### C4 运行后回插

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```
（SF-C4-08，第 37–40 行）

比较对象 `tmp_rt` 在循环前取一次，之后只有 `tmp_list` 前进，这是 W01 的来源。循环条件先比较键、后判断 anchor。计费发生在插回之后，这是 W02 与 W07 第 1 步的来源：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```
（SF-C4-10，第 49–50 行）

由此，现有回插只会落在队首或队尾，从不落在中间（DV-18）。

**现有代码已经会经 anchor 求出伪任务指针并读取。** 进入切换块时若队列里只剩 idle、current 是 RUNNING 的非 idle 任务（单个普通任务时间片到期就是这种状态），摘下 idle 后队列为空。`tmp_list` 就是 anchor，第 39 行先求 anchor 的“所属任务”的键，再判断 `tmp_list != anchor`。读到的值不影响结果，但这是一次类型混淆的读取：被读的位置不属于任何任务对象（DV-11，文本推导，未执行；结构布局偏移未读，所以不断言读到哪里）。这条更正了此前“只有改了游标才会读 anchor 容器”的说法。A-R3 的“先判 anchor”会同时消除它。

回插只在 current 仍为 RUNNING 时发生（idle 也一样）。不切换有两条路径：四个触发条件都不成立（W08 第 1 步），或 count == 0（V08）；两者都返回 current，不计费，也不动 `last_jiffies`。

`BUG_ON` 位于已要求 `count > 0` 的块内，而它的条件要求 `count <= 0`，因此不能触发；紧随的 `while (next_lp == NULL);` 也发现不了计数与链表失配（DV-05）。它们都不是保护，后续规格不能依赖。

### C5 下一任务选取与分派

每次异常或外部中断处理之后，只要不在原子上下文，就会调用 `schedule()`。异常桩与中断桩都经 `sa_entintr_retp`，且在调用前已开中断（SF-C5-24/25）；启动时 preempt 计数为非 0 的 `INIT_PREEMPT_COUNT`，之后何时降到 0 没有追踪（GAP-02）：

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::excep_hwint_context]
```c
	if (!in_atomic())
		schedule();
```
（SF-C5-14，第 128–129 行）

HPET 中断本身只推进计数器、不置 `need_resched`：

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::HPET_handler]
```c
	jiffies++;
	do_timer(1);
```
（SF-C5-15，第 527–528 行）

所以时间片到期靠“中断后 schedule() → pick 中 `used_jiffies >= time_slice`”来判定。pick 读的 `need_resched()` 是 current 的 TIF 标志（SF-C5-22），直接文本中只有 `resched_curr` 设置它，而它唯一的活动直接调用者是 `sched_yield` 的 `yield_task_fair_myos`（SF-C5-12/13）。`do_idle` 的 `set_preempt_need_resched` 只改 preempt 计数位（SF-C5-23），不会让 `need_resched()` 为真。唤醒不调用 `resched_curr`，所以被唤醒者要等下一次 pick 进入切换分支才会被选中，不是立即抢占。

`__schedule` 在 pick 之后无条件清除 `need_resched`（SF-C5-30），所以 count == 0 时，`sched_yield` 的请求会被消耗掉而不发生切换。另有宏生成入口：`try_to_wake_up` 结尾的 `preempt_enable()` 在计数条件满足时可经 `preempt_schedule` 进入 `__schedule`（未追踪）。切入分两种：切回曾被切出的任务时，回到它当初的 `__schedule` 尾部并开中断（SF-C5-21）；新任务第一次被切入时经 `ret_from_fork_asm` → `ret_from_fork`，不回到 `__schedule` 尾部（SF-C5-26），这条路径的开中断与计数恢复没有追踪。

`__schedule` 先关本地中断（SF-C5-20），再经宏别名 `pick_next_task → __pick_next_task` 遍历链接段 `.data.sched_class` 中的类（SF-C5-02…SF-C5-10）。文本上放入该段的只有 `DEFINE_SCHED_CLASS(myos_rt)`，其 `pick_next_task` 钩子即 `pick_next_task_myos`（SF-C5-11）。这个函数至少返回 current，所以 `BUG()` 在这一前提下不可达；钩子的非空检查是注释，调用是无条件的（SF-C5-29，DV-12）。

任务的 `sched_class` 字段在直接文本中只在 `init_idle` 被写为 myos_rt。`init_task` 的静态初始化器里没有它，其余任务靠结构复制继承；前提是第一次 fork 发生在 `sched_init` 之后（DV-24）。分派本身不读这个字段，直接文本中读它的只有 `sched_yield`。选中的任务就是摘下的队首，与键无关。

## 2. 计费责任与队列不变量（D03）

| 对象 | 写入者（直接文本） | 读取者 | 备注 |
|---|---|---|---|
| `myos.last_jiffies` | 仅 pick 切换块：`= jiffies`（SF-C4-11） | pick 开头算 `used_jiffies`（SF-C4-03） | 无显式初始化（GAP-01）；DV-02 保证不影响非 idle 计费 |
| `se.vruntime` | `init_task` 静态 −1；`dup_task_struct` 复制；`__sched_fork` 置 0；pick 累加 | pick 插回比较（只比首节点） | 唤醒路径不写；d01 F03 无其他直接写入命中 |
| `rt.time_slice` | `init_task` 静态 `RR_TIMESLICE` = 100；fork 复制继承 | pick 切换条件 | 无递减、无重填 |
| `jiffies` | `HPET_handler` 的 `jiffies++` 与 `do_timer(1)`（同址） | pick | 每次 HPET 中断 +2（DV-10，未执行） |
| 其他时间记账 | 无：`sum_exec_runtime`/`exec_start` 等 10 行命中全为注释或声明 | — | `nr_switches`、`nivcsw` 只计次数；`rq->clock` 在 TSC 不可用时由 `jiffies_64` 派生（d01 F09），不是按任务计费 |

切换与计费有三条路径：进入切换块（非 idle 的 current 计费，与 state 无关；基线重置，W05/W06/W07）；count > 0 但触发条件都不成立（不计费、不动基线，W08 第 1 步）；count == 0（同上，可能返回非 RUNNING 的 current，V08）。

**计费守恒。** 相邻两次切换之间的非 idle 时间恰好计入一次，idle 时间按设计丢弃；不切换的调用不计费、不移动基线（DV-20）。因此 A-R1 的“先计费再定位”只改变插入位置，不改变任何计费量。

**结构不变量。** 队列的结构安全要靠全部写入者共同维持六条约定（DV-21）：

- I0：写入前环已初始化。
- I1：环完整。
- I2：每个任务节点至多一次。
- I3：count 等于节点数。
- I4：运行中的任务不在环中（例外：pick 插回后、真实上下文切换前）。
- I5：idle 至多一次。

原语本身不校验这些约定。`__list_add_valid` 只在新节点紧邻插入点时以永久自旋“报错”（这是源码语义；优化编译是否保留这种空循环未核，GAP-11）；节点若已链在同一环的其他位置，插入会静默破坏环。排序契约（头插优先或按键升序）是在这些结构约定之上的另一层，不能互相替代。

必须保留的正常行为与产生它们的源码：

- W05：阻塞者不插回但计费，因为插回只看 RUNNING，计费只看是否 idle。
- W06：idle 尾插不计费。
- W07 第 2 步：同一 jiffies 不重复计费，因为 `last_jiffies` 在上一次切换时已重置。
- W08：不切换时整块跳过，累计留到下次切换计入。
- W03：零增量正常。
- W04：结果 A20 在 C20 前、满足 P。原函数中这不是并列比较（A 先按旧键 15 定位），A 下才成为第一个真正的并列输入，按 A-R6 结果不变。

**高键睡眠任务头插的源码状态推导（INFERRED，未新增运行）。** 共同前提：CPU0 队列为 B10、C20、I（键 2^64-1）；X 键 35（已含它阻塞前那次计费），节点自环、不在队列、不是 current；唤醒目标 cpu = 0，select_task_rq 原样返回；插入经 `__list_add_valid` 校验；单 CPU、无并发。`wake_up_process(X)` 之后队列是 X35、B10、C20、I，count 从 3 变为 4，返回 0。

| 结构安全 | 记账守恒 | 头插优先 | 按更新键排序 |
|---|---|---|---|
| 保持（I0–I5 逐项成立）：环已初始化；`__list_add_valid` 通过；X 只链入一次；count 与节点数同增 1；current 不受影响；idle 仍只出现一次 | 保持：唤醒路径不写键与 `last_jiffies` | 成立，但不是立即抢占：要到下一次 pick 进入切换分支；期间若有别的唤醒或新任务头插，它们会排到 X 前面 | 被破坏：队首 35 大于次节点 10；现有代码中逆序还有 pick 回插这另一个来源 |

这是两种队列契约的分歧点，不是结构错误。头插造成的逆序会在后续切换中按后进先出被消费（DV-08），但在被消费之前，即使回插循环修好，回插位置也可能落在更低键之前。

## 3. 两种候选改进（D04，规格，不是实现）

| | 候选 A：兼容现有入口语义的局部修复 | 候选 B：统一队列顺序契约 |
|---|---|---|
| 契约 | 选择 = 摘队首；唤醒/新任务头插不变；只保证回插本身不制造逆序 | 非 idle 节点按更新后键非降序，idle 在尾，选择 = 队列中（不含 current）的最小键 |
| 修改要求 | A-R1 先计费再定位；A-R2 比较对象随游标；A-R3 先判 anchor 再求任务；A-R4 以 `rq->idle` 身份把插回限制在 idle 之前；A-R5 链接与 count+1 作为一个操作；A-R6 插在第一个键不小于自己的节点之前 | B-R1 = A 全部；B-R2 普通唤醒改为与回插同一规则的有序插入，保留 contains（或改用 O(1) 自环判定，DV-22），current 快速路径可保留；B-R3 在 B-R1、B-R2 使队列有序之后，首次入队可保持头插（键 0 头插等于有序插入，DV-06），只补“未链”保护；B-R4 idle 规则不变；B-R5 选择仍摘队首，由不变量保证它是队列（不含 current）中的最小键；B-R6 键下限（OD-2） |
| 精确受影响符号 | `myos_rt.c::pick_next_task_myos`（可选：`double_list.h` 新增“插到指定节点前并计数”原语） | 以上，加 `scheduler_core.c::set_task_cpu`（改插入位置）、`wake_up_new_task`（只补前置保护）与一个共用的有序插入 helper（位置待定） |
| 可观察变化 | 只改变 W01/W02/W07 这类输入的插回位置与随后的选择：W01、W02 变为 C20,A25,D30,I；W07 第 2 步改选 C20 | 键高于队首非 idle 任务的被唤醒任务不再在下一次切换运行（新任务仍在队首）；守护线程（NVMe/ATA/XHCI rq_deamon）、workqueue worker、kthreadd、softirqd、信号量与定时器等待者的等待时间改由键决定 |
| 能承诺 | 消除两类已见证失序；消除现有的伪任务指针读取（DV-11）；插回前有序且 idle 在尾，则插回后仍如此；anchor 与 idle 边界安全；W03–W06、W08 的记录不变，W07 保持“不重复计费” | 全队有序；选中的是队列中（不含 current）的最小键，因为 pick 先摘队首再插回 current（在单 CPU、写入者仍为这四个的前提下） |
| 不能承诺 | 全局升序；选中最小键（唤醒后仍有 W07 第 2 步型选择） | 唤醒时延；不设下限时的追赶突发（DV-09） |
| 风险 | 修复后低键任务的追赶交替变得可见（DV-09）；若 A-R4 只靠 idle 键哨兵，将来一旦 idle 键被改写，普通任务可能被插到 idle 之后 | 时延语义改变；有序插入 O(n) 与 contains 的 O(n) 叠加；需同源复跑 V04–V07 |
| 剩余验证 | W01–W08 重跑（两次完整输出）+ NR-3、NR-4、NR-6 | A 的全部 + NR-1、NR-2、NR-5、NR-6 的 B 列 + V04–V07 同源复跑 |
| 先后 | 先做；需 Owner 的内核修改授权 | A 之后；需先答 OD-1、OD-2 |

两种方案都不引入 CFS/EEVDF、SMP 负载均衡或新的调度类，都保持唤醒返回值（恒 0）与 CPU0 放置不变。两种方案都没有给出 C 代码或 diff。

候选 A 之外另列两项可选加固，各自单独评审，不属于 A 的最小要求：

- **A-O1：** 在 `init_idle` 显式写 `last_jiffies = jiffies`。按 DV-02 它不改变任何现有计费输出，只是去掉对初值的依赖。
- **A-O2：** 首次入队前确认节点未链接，或让普通唤醒不处理 `TASK_NEW`，针对 DV-13。后一种做法会改变 V05 记录的现状。

## 4. 需要 Owner 决定的事项（共两项）

**OD-1 队列目标契约。** 问题是保留“唤醒/新任务头插优先”（以 A 为终态），还是采用“全队按更新后 vruntime 非降序、选最小键”（B）。

- **建议：** 先实施 A，把 B 定为后续目标。
- **理由：** 活动文本中对运行队列做排序比较的只有回插循环，它比较的是 `se.vruntime`（SF-C4-08）。B 给出一条可机械检查的不变量，便于学习与回归。头插优先没有任何键或标志表达，难以测试和解释。这是基于可验证性的建议，不是对作者意图的推断。
- **影响：** 选 B 要改 `set_task_cpu` 的插入位置，并给 `wake_up_new_task` 补前置保护；键高于队首的被唤醒任务（守护线程、等待者）不再插队，新任务仍在队首。只选 A，则唤醒后仍会选中非最小键，回归须把选择定义为“取队首”。
- **不答时：** 只准备 A，不动两个入队入口。

**OD-2 新任务与唤醒任务的键下限。** 采用 B 时必须回答；只做 A 时这一项只影响修复后的追赶行为，不需要实现。

- **建议：** 入队键取 max(自身键, 当时队首非 idle 任务的键)，队空时保持自身键，不引入 `min_vruntime` 体系。
- **理由：** 否则零键新任务与长睡任务会长时间与最低键任务交替，压后其余任务。
- **影响：** 新任务的首次位置不变，仍是队首；改变的是新任务与长睡任务的键值，以及之后的追赶时长。由 NR-1 子例 2、NR-2、NR-6 覆盖。
- **不答时：** B 不实施；即使 OD-1 选了 B，也在本项有答复之前不准备 B。

扫描工具、文件拆分、执行顺序等内部安排不提交 Owner。

## 5. 接到已有回归（D05）

| 证据 | 支持或要求保持 | 说明 |
|---|---|---|
| W01 | A-R2 | 比较对象停在首节点，A25 落到 idle 之后（W01 不读 anchor 容器；A-R3 只由 DV-11 与 NR-3 子例 0 支撑，A-R4 由 NR-3 子例 2 与 NR-6 的 idle 键 0 变体检验） |
| W02 | A-R1 | 按旧键 15 定位、后计费，循环未进入；只改游标修不掉 |
| W07 第 1 步 | A-R1 | 同 W02 |
| W07 第 2 步 | 保持“不重复计费”；B-R5 的失败样例 | 选中 A25 而 C20 在队 |
| W03、W04、W05、W06、W08 | 保持 | 零增量、W04 结果（原函数中并非并列比较，A 下成为首个并列输入、结果不变）、阻塞计费、idle 不计费、不切换不计费 |
| V04、V06、V07 | 保持（已审范围内） | 非 current 唤醒入 CPU0、返回 0；串行重复唤醒只一个节点；CPU 元数据不更新 |
| V05 | 现状记录，与 DV-13 相关 | `TASK_NEW` 也被唤醒入队 |
| V08 | 开放（GAP-07） | count == 0 分支的可达性依赖 idle 以非 RUNNING 切出 |

新增回归只补接合处、anchor 边界与政策差异，共六项，全部 NOT_RUN：

- NR-1：睡眠 → 唤醒 → 选取，A/B 预期分列；含两次唤醒之间的后进先出。
- NR-2：新任务零键并列与继承的 `time_slice`。
- NR-3：anchor 边界与 idle 身份判定，含现有代码已会触发的“队列只剩 idle、普通任务时间片到期”情形。读访问按 offsetof 放到守护页上检测，探针不进被测函数；无法检测时记 INCOMPLETE_EVIDENCE。
- NR-4：真实 idle 键 2^64-1。
- NR-5：current 快速路径与重复唤醒。
- NR-6：多步计费守恒与结构不变量。

每项都要求两个独立进程各完整运行一次，并保存完整输出。判定规则：

- 缺必需快照判 INCOMPLETE_EVIDENCE，不给排序结论。
- 记录完整但违反候选策略判 VALID + VIOLATED，不判 COMPARATOR_INVALID。
- “按队首选择符合代码”与“选到策略目标”分栏报告。

具体输入、观测与各列预期见 yaml `future_regressions`。本任务没有做任何动态测试。

## 6. 停止点

- **可以准备实施：** 候选 A。只改 `pick_next_task_myos`，规格与回归计划已齐，实施仍需 Owner 的内核修改授权。
- **需要先作政策决定：** 候选 B（OD-1）、键下限（OD-2）。
- **因真实运行层未知而不能承诺：**
  - CA-02 全局可达性；
  - HPET 实际频率、`in_atomic` 与中断时机；
  - SMP 与真实上下文切换；
  - idle 以非 RUNNING 切出（V08）；
  - `TASK_NEW` 唤醒风险是否可达（DV-13）；
  - 中断窗口离队与有限超时等待两类风险是否可达（DV-23、DV-19）；
  - jiffies 每次中断加 2（DV-10）。

这不是第二波全量完成，也不是新一轮研究清单。

## 7. 反证、修正与未解项

- **对“只修 pick 不够”的细化。** 新任务的零键头插在有序队列中不制造逆序（DV-06）；真正破坏升序的是高键任务的普通唤醒（DV-07）。而且这类逆序会在后续切换中被消费，不是永久状态（DV-08）。
- **对“只改游标才会读 anchor”的更正。** 现有代码在“单个普通任务 + idle、时间片到期”时已经读取 anchor 的所属任务（DV-11，读取面 L4 发现、经核验后采纳）。修复要求 A-R3 因此是消除现有问题，不只是防止修复引入新问题。
- **对“抢占靠 need_resched”的修正。** HPET 不置 `need_resched`，时间片到期由中断后的 `schedule()` 加 pick 的 `used_jiffies` 判定（SF-C5-14/15）。
- **W 夹具与源码的差异。** W 中 idle 键为 0 或 7，源码为 2^64-1；不改变 W01/W02/W07 的结论，需由 NR-4 复核。
- **`__list_add_valid` 不是去重守卫。** 它只拦紧邻插入点的重复节点；其他重复插入会静默破坏环（DV-21）。每次唤醒的 `list_header_contains` 都要走完整个环；节点自环可作 O(1) 的未链判定，现成原语 `list_is_empty_entry` 就是这种判定，现有调度写入者都没用（DV-22）。
- **新发现、待确认的结构风险（C2×C3 接合处，DV-13）。** `TASK_NEW` 任务若在 `wake_up_new_task` 之前被普通唤醒，结果分三种：仍在队首时自旋挂死；在队中但不在队首时环被破坏、count 多计；已被选中正在运行时，运行中的任务被链入环。是否存在这样的唤醒者没有确立（GAP-08）。
- **新发现、待确认的离队时点风险（DV-23）。** 写了非 RUNNING、还没检查等待条件就被中断后的 `schedule()` 切出的任务不会回队，可能错过早已发生的唤醒。上游让抢占不出队的条件在 MyOS2 中是注释；这取决于中断上下文中 `in_atomic()` 的取值（GAP-02）。
- **新发现、待确认的失唤醒与忙等风险（DV-19）。**
  - **源码事实：** `schedule_timeout` 的有限超时分支在可见行内没有活动的 `schedule()`（第 798 行被注释）；它调用的 `__mod_timer` 与 `del_timer_sync`（宏别名到以 `cpu_relax` 忙等的 `__timer_delete_sync`）在 `timer.c` 中也不调度（SF-C3-26/31/32）。
  - **推导，前提是 `timer.c` 之外的被调函数都不调度：** 先写非 RUNNING 状态再调用它的任务会立即返回，返回值不减，并以非 RUNNING 状态继续运行。下一次切换时它会被切出且不回队，为它挂的定时器也已删除。
  - **已知受影响者：** 信号量 `down_timeout` 的等待循环会反复写等待状态并调用它，超时永不减少，被切出后要靠 `up()` 唤醒回队。
  - **未枚举：** 其他调用者（GAP-10）。
- **计费量纲。** 每次 HPET 中断 jiffies 加 2（DV-10）。这是源码推导，不影响排序关系。
- **sched_class 的来历。** 直接文本中只有 `init_idle` 写它，`init_task` 静态初始化器里没有它；分派不读它（DV-24）。
- **两个容易误用的前提。** `on_rq` 不是队列成员判据（DV-15）；其他 CPU 的队列没有初始化（DV-16）。
- **kthread 的注释与代码不符。** `kthread_run` 的注释写着 `myos_wake_up_new_task`，活动宏体调用的却是普通唤醒 `wake_up_process`（SF-C2-19/20），规格以活动文本为准。
- **未解项。** GAP-01 至 GAP-11 见 yaml `gaps`。每一项都写明了它影响哪个结论，不用“无命中”替代“不存在”。
