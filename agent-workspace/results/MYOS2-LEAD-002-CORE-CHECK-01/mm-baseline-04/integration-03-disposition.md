---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
base_snapshot: "kernel=time a039d9803ade；lead=agent/MYOS2-LEAD-002 c2176ad4da02；execution base ee9e6a738224（integration-03 原件）"
inputs_read: ["c2176ad4da02: 17-memory-baseline-contract.md、reviews/CORE-SCHED-INTEGRATION-03-review.md", "ee9e6a738224: scheduler-integration-03/facts-and-regressions.yaml（candidate_changes、owner_decisions、future_regressions、gaps）、integration.md §2 C3、MANIFEST.md、records/d01_index.json", "time a039d9803ade: sched/scheduler/myos_rt.c（pick_next_task_myos）、lock_IPC/signal/signal.c（signal_wake_up_state）"]
status: final
open_questions: ["A/B/键下限均未采用；NR-1…NR-6 仍 NOT_RUN"]
record_type: spec_errata_disposition
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-MM-BASELINE-04
part: M00
executor: "Claude Code cloud session (claude.ai/code)"
model: unknown_or_not_attestable
date: 2026-10-02
responds_to: "CORE-SCHED-INTEGRATION-03-REVIEW-001 的 IR-01、IR-02（主线对象 c2176ad4da02）"
target_package: "scheduler-integration-03（被审头 ee9e6a738224，结果冻结 b7fa83583e35）"
supersedes_scope:
  - "facts-and-regressions.yaml → future_regressions/NR-6 的 input 中 “P6 @200（N2 计 10，选 N1）” 对 idle 键 0 变体的适用性，以及 expected/all 中“N2=60、N1=40、合计 100”对所有分支的统一适用"
  - "facts-and-regressions.yaml → candidate_changes/B/must_keep 的 “A 的全部保持项” 与 “唤醒返回值语义不变（恒 0；调用者无依赖变化）” 两条"
  - "integration.md §2 C3 末句 “结论是：现有调用者不依赖返回值……” 的总括"
  - "MANIFEST.md D01 行 “需定位的定义全部找到”"
  - "OD-1/OD-2 的 safe_default_if_unanswered 被读作授权的可能（本文明确不是）"
not_superseded: "其余源码事实、引文、链路分析、W/V 引用、DV 推导、缺口记录全部保持原样；原件一字未改。"
kernel_change: none
runtime: NOT_RUN
---

# IR-01 / IR-02 处置：只改规格文字与未来预期，不改内核

**结论先行。**

- **IR-01：** 按 NR-6 的原输入和原 `pick_next_task_myos` 逐步手推，idle 键为 0 时原函数是 N2=50、N1=40、idle 区间 10；idle 取真实最大键时原函数是 60/40/0。按规格正确实现的 A 或 B，两种 idle 键下都应是 60/40/0。三组数都满足“普通任务计费之和 + idle 区间 = 200 − 100”。主线的推导与我的独立复核一致，没有找到反证。
- **IR-02：** B 的保持项改为显式清单，共 10 条（B-K1…B-K10），并明确排除“保留普通唤醒头插”。键下限按“设”与“不设”两个分支分别写出，不替 Owner 选择。
- **其他：** A、B、键下限本轮都没有采用。Owner 不答复，不构成实施 A 的授权。本文只修改解释和未来预期，没有改动内核，也没有改动原包的任何文件。

## 1. NR-6 的原输入（不改）

来源：`scheduler-integration-03/facts-and-regressions.yaml`，`future_regressions` 中 `id: NR-6` 的 `input` 字段。

- **起点：** idle 为 current，队列为空，jiffies = last_jiffies = 100，每次 pick 前 need_resched = 1。
- **事件顺序：** 先 `wake_up_new_task(N1)`，再 `wake_up_new_task(N2)`，两者键都为 0，队列为 N2,N1。此后依次是 P1 @100、P2 @130；N1 写入 TASK_UNINTERRUPTIBLE 后是 P3 @150；@160 执行 `wake_up_process(N1)`；然后是 P4 @170、P5 @190、P6 @200。
- **两个变体：** idle 键分别取 2^64−1（真实值，DV-01）和 0。

## 2. 原函数的四条规则（逐字引文，`mm_facts_check.py` 机械核对）

规则一：current 若仍是 RUNNING 才回插，idle 一律尾插。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task->__state == TASK_RUNNING)
		{
			if (curr_task == rq->idle)
			{
```

规则二：非 idle 回插时，比较对象固定为摘除后的首节点 `tmp_rt`，游标只是一路前进到 anchor。所以只有两种结果：“不大于首节点键就插在首节点前”，或“大于首节点键就插到尾部”。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

规则三：计费在定位之后进行，idle 不计费。

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

规则四：选取总是摘下的那个队首。这一点已在 integration-03 的 SF-C4 中引用，这里不重复。

## 3. 值表

表中“队列”指 pick 返回后的状态，从左到右为队首到队尾。“计费”是本步记到 current 上的量。三张表的每一步都经 `scripts/mm_facts_check.py` 的 M00 部分做了算术核对：区间、计费和守恒式（`records/facts_check.json` → `m00`）。核对时没有执行原函数，也没有用重写的算法冒充原函数。

### 3.1 原函数，idle 键 = 0

| 步 | 时刻 | current | 本步计费 | 选中 | 队列 | 依据 |
|---|---|---|---|---|---|---|
| P1 | 100 | I | 0 | N2 | N1,I | 摘 N2；idle 尾插，不计费 |
| P2 | 130 | N2 | 30（N2=30） | N1 | N2,I | 摘 N1 后剩 [I0]；0 > 0 为假，插在 I 前；随后计费 |
| P3 | 150 | N1 | 20（N1=20） | N2 | I | N1 非 RUNNING，不回插但计费；@160 唤醒头插后为 [N1,I] |
| P4 | 170 | N2 | 20（N2=50） | N1 | I,N2 | 摘 N1 后剩 [I0]；30 > 0 为真，游标走到 anchor，N2 落到 I0 之后 |
| P5 | 190 | N1 | 20（N1=40） | **I** | N1,N2 | 摘队首 I；N1 键 20 与首节点 N2(50) 比较为假，插在 N2 前 |
| P6 | 200 | I | 0 | N1 | N2,I | current 是 idle：摘 N1，idle 尾插，不计费 |

合计 N2=50，N1=40，普通任务共 90。190–200 这一段由 idle 运行，idle 区间为 10，90 + 10 = 100。

### 3.2 原函数，idle 键 = 2^64−1

| 步 | 时刻 | current | 本步计费 | 选中 | 队列 |
|---|---|---|---|---|---|
| P1 | 100 | I | 0 | N2 | N1,I |
| P2 | 130 | N2 | 30（N2=30） | N1 | N2,I |
| P3 | 150 | N1 | 20（N1=20） | N2 | I（@160 后 N1,I） |
| P4 | 170 | N2 | 20（N2=50） | N1 | N2,I |
| P5 | 190 | N1 | 20（N1=40） | N2 | N1,I |
| P6 | 200 | N2 | 10（N2=60） | N1 | N2,I |

合计 N2=60，N1=40，idle 区间 0。这里任何比较都不会大于 2^64−1，所以 N2 不会落到 idle 之后。

### 3.3 候选 A 或 B 的规格预期（两种 idle 键，未实现、未运行）

A-R1 要求先计费再定位，A-R4 要求以身份判断 idle。在这两条之下，P2、P4、P5、P6 的插回位置都在 idle 之前。@160 的唤醒在 A 下是头插，在 B 下是有序插入，两者此时都得到 [N1,I]。选取结果与 3.2 相同，合计同为 60/40/0。B 的键下限在本输入中不起作用：两次唤醒和首次入队时，队首要么没有非 idle 任务，要么键值相等。这只是对未实现规格的预期，不能记为实测。

### 3.4 守恒式与更正后的 expected 文字

原 `expected/all` 把“N2=60、N1=40、合计 100”套在所有分支上，这是错的。更正如下：

- **一般守恒：** 按每段实际的 current 分账。普通任务计费之和 + idle 运行区间之和 = 最后一次 pick 时刻 − 起点（本例为 100）。idle 从不计费；不能把整段墙钟时间都记到普通任务头上。
- **原函数、idle 键 0：** N2=50、N1=40、idle 区间 10。P5 选中 idle；P6 的 current 是 idle，不向 N2 计费。
- **原函数、idle 键为最大值：** N2=60、N1=40、idle 区间 0。
- **A 或 B（两种 idle 键，未实现）：** N2=60、N1=40、idle 区间 0。idle 键 0 的变体仍是 A-R4 身份判定的回归点：原函数会在 P4 把 N2 排到 I0 之后。
- **结构不变量保持原文：** 无重复节点，count 等于节点数，idle 至多出现一次。本例全程都没有出现“摘除后队列为空”，所以 NR-6 不触发 DV-11。

### 3.5 反证检查

我没有直接采用主线给出的数字，而是从源码规则重推了一遍，并逐一检查了下面几个可能的分歧点，结论都与主线一致：

- **P3 是否会因 BUG_ON 或 count 条件跳过切换：** 不会。count 为 2，条件为真，BUG_ON 的条件不成立。
- **P4 的游标是否会在 I0 前停下：** 不会。比较对象固定为首节点 I0，30 > 0 恒为真，游标只能走到 anchor。
- **P5 的比较对象：** 是 N2(50)，比较发生在计费之前，此时 N1 键为 20。
- **P6 是否仍满足切换条件：** 满足，因为 current 就是 idle。

没有发现需要改动主线数字的情况。未来如果要执行，仍然缺少观测数据。没有观测，不等于没有失败。

## 4. IR-02：候选 B 的显式保持项

原 `must_keep` 的第一条是“A 的全部保持项”，它把 A 的“wake_up_new_task 与 set_task_cpu 的头插”也一并继承下来，与 B-R2 的“普通唤醒改为有序插入”矛盾。现改为下面的逐项清单，B 不再整体继承 A：

| 编号 | B 保持的约定 | 来源 |
|---|---|---|
| B-K1 | 选取仍是“摘队首”；在 B 的不变量下，队首等于队列中（不含 current）非 idle 任务的最小键；队中只剩 idle 时队首为 idle。idle 按身份排在尾部，与其键值无关（idle 键 0 时它的键最小，但仍在尾部） | B-R5、B-K2 |
| B-K2 | idle 由 pick 尾插、不计费；排序时按 rq->idle 身份排除 idle，不依赖它的键 | B-R4、A-R4 |
| B-K3 | 切换条件不变（need_resched、idle、时间片、非 RUNNING，且 count>0） | SF-C4-04 |
| B-K4 | last_jiffies 只在切换时更新 | A 保持项 |
| B-K5 | 阻塞的 current 不插回，但仍计费 | W05 |
| B-K6 | 同一时刻不重复计费 | W07 第 2 步 |
| B-K7 | 不切换就不计费 | W08 |
| B-K8 | 用 contains 防止重复入队（或换成 DV-22 的 O(1) 自环判定） | V06、DV-22 |
| B-K9 | 唤醒仍写 TASK_RUNNING，并放进 CPU0 队列 | V04/V07 现状、DV-16 |
| B-K10 | try_to_wake_up 的返回值保持现值（恒 0）。存在显式读取者：signal_wake_up_state 依据 `!wake_up_state(...)` 决定是否调用 kick_process（见下方引文）。B 不改变这个返回值，所以该读取者的行为不受 B 影响；不能写成“调用者都不读返回值” | 回源，见下 |

[VERIFIED mykernel/lock_IPC/signal/signal.c::signal_wake_up_state]
```c
	if (!wake_up_state(t, state | TASK_INTERRUPTIBLE))
		kick_process(t);
```

**明确排除的项：**

- **set_task_cpu 中普通唤醒的头插：** B-R2 把它改为有序插入，B 不保留。
- **wake_up_new_task 的头插：** 不作为一项“政策”保留。B-R3 允许这行代码的位置不改，理由是在 B 的不变量成立、并补上“节点未链接”的前置保护之后，键 0（或钳制后等于队首键的键）头插的结果恰好等于有序插入。这是结果上的等价，B 并不承诺“新任务凭头插优先运行”。

**键下限的两个分支（OD-2 未决，本文不选）：**

- **不设下限：** 新任务键为 0，长睡任务键低，它们按键直接排在最前，并与最低键任务交替运行，直到追上（DV-09）。键高于队首的被唤醒任务按键排序，失去头插带来的优先。
- **设下限 max(自身键, 当时队首非 idle 任务的键)，队首没有非 idle 任务时保持自身键：** 被唤醒任务和新任务的入队键被抬到不低于队首。按 A-R6 的并列规则，它们插在队首之前，因此仍会先运行一次。但它们的键值从此改变，随后与其他任务的先后顺序和追赶时长也随之改变。新任务和长睡任务这类低键任务同样受影响，不能概括为“只影响高键唤醒”。

## 5. 同文登记的消费限界

| 编号 | 登记内容 | 依据 |
|---|---|---|
| R1 | Owner 不答复不等于授权实施 A。OD-1/OD-2 的 `safe_default_if_unanswered` 只描述将来获得授权时先准备什么，不是实施许可。当前 A、B、键下限都未采用（`candidate_A_adopted: false`、`candidate_B_adopted: false`） | 17 号任务书头字段，审查回执 §2 |
| R2 | 151/151 的引文核对与词法分类不等于编译或运行证明，也不是逐条语义认证 | 审查回执 §5 |
| R3 | 有 10 个名字未定位：`__set_task_cpu`、`myos_wake_up_new_task`、`fair_sched_class`、`idle_sched_class`、`rt_sched_class`、`dl_sched_class`、`stop_sched_class`、`HAVE_ARCH_BUG_ON`、`ttwu_queue`、`ttwu_state_match`。这是 scan_index.py 在其检索范围内的记录，不能证明全局不存在。原 MANIFEST 的“需定位的定义全部找到”不准确，以本行为准 | ee9e6a738224 `records/d01_index.json` → `definitions[].status` |
| R4 | gap 记录共 12 条：11 条 open（GAP-01…GAP-11），1 条 narrowed_or_closed（GAP-NARROWED）。这是 12 条记录，不是 12 个开放风险 | 同包 YAML `gaps` |
| R5 | 唤醒返回值：d01 中 12 行 `wake_up_process` 调用都丢弃返回值；`signal_wake_up_state` 读取 `wake_up_state` 的返回值，因为返回值恒为 0，它总会调用 `kick_process`。integration.md §2 C3 的“现有调用者不依赖返回值”应改为这两句 | 上方引文 |
| R6 | DV-19 的两个分支要分开写。“立即返回”要求 `__mod_timer` 与 `del_timer_sync`（宏别名到 `timer_delete_sync`）等下层调用都能返回；`__timer_delete_sync` 只在 `ret < 0` 时进入 `cpu_relax` 忙等循环。这是不同条件下的两种结果，不是同一次调用的必然时序 | 审查回执 F03–F05 |
| R7 | DV-11 说的是源码表达式对非法对象的解释路径，不能推出真实机器一定发出这次加载；优化后的代码与实际触发都未验证 | 审查回执 §1 |

## 6. 本处置没有做的事

- 没有修改内核、ee9e6a738224 中的任何文件或 integration-03 的原件。
- 没有新增调度实验，也没有重跑 W、V、M、N。
- 没有编译或运行 C/ASM、内核或 QEMU。
- A 或 B 的预期仍是未执行规格。NR-1…NR-6 的状态仍为 NOT_RUN。
