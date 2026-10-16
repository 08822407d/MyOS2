---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_type: scoped_scheduler_semantic_followup
conversation_display_name: "MYOS2-A-C02 内核分析主线"
produced_by: "newest gpt6"
model_per_owner: gpt6
effort_per_owner: pro
date: 2026-10-01
base_snapshot: "time（分支名）；写入 agent/MYOS2-LEAD-002"
source_short12: a039d9803ade
source_identity_check: "连接器比较 a039d9803ade 与 time 为 identical；没有执行本地命令。"
read_channel: connector
evidence_class: "实际源码引文与未执行的局部状态推导；不证明全局运行可达性"
inputs_read:
  - "time:mykernel/scripts/options_flags.cmake 全文"
  - "time:mykernel/sched/scheduler/myos_rt.c 全文"
  - "time:mykernel/lib/list/double_list.h 的 list_add_to_next/list_add_to_prev 定义及相邻注释"
  - "time:mykernel/debug/panic.c 开头至第160行；仅用于分支引文，不申报全文"
  - "PR17 b843d475367a:core/fixtures/fx_sched.c 的 pick 辅助函数与相邻内容"
  - "PR17 recheck-01/evidence.md 的 V08、rerun_vs_batch1.json；最新回收复核所列限制"
status: SOURCE_FOLLOWUP_COMPLETE_DYNAMIC_WITNESS_PENDING
linked_case: V08
candidate_id: CORE-SCHED-ORDER-02
candidate_is_new_source_inference: true
changes_old_CA_or_DR008_ids: false
kernel_modified: false
execution_authorized_by_this_file: false
kernel_acceptance_verdict: NOT_ISSUED
acceptance_ceiling: PASS_PENDING_LOCAL
local_validation: "待执行面；本文件中的新见证未运行，不属于13号数据层任务的执行范围。"
startup_selfcheck_quote: '2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。'
write_authority_note: "本主线的 lead 目录写权来自 MYOS2-LEAD-002 工作令与 Owner 继续指令；没有改公共公约。"
branch_canary_quotes:
  time:
    options_flags_cmake: "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
    panic_c_panic: "\tthis_cpu = smp_processor_id();"
branch_canary_comparison: "本轮实际读取 time 引文；time/master 的机械区分沿用已交 recheck V00，不申报主线重跑。"
self_check:
  scope: this_file_only
  verified_claims: 6
  quotes_reconfirmed: 6
  downgraded_to_inferred: 0
  method: "模型逐条对照本轮读取原文并计数；待执行面机械核对，不并入旧47条分母。"
open_questions:
  - "非 idle 队列是否要求按更新后的 vruntime 排序，是这里明确提出的候选不变量，不冒称 Owner 已裁决。"
  - "新状态序列的动态见证、正常内核路径可达性及完整调度正确性未完成。"
---

# V08 后续：修比较游标，还不一定修好回插顺序

**已把执行者报告的回插顺序问题继续核到源码：除了比较对象不随游标更新，计费发生在回插之后也是一个独立的候选失序原因。** 这一步不需要等待验证器修订，因此本轮一并完成。这里只给出源码和有限状态见证，不改内核、不调用测试，也不占用新的研究任务号。

本文件不加入 13 号数据层任务的必做项。后者只修结果读取；此处的内核候选用于后续有明确授权的局部实验和学习设计，不能借机让执行者修内核。

## 1. 源码事实

K01｜配置面已读取，以下是第二个 C 编译参数定义中的一行；不据 CONFIG_NR_CPUS 推定 SMP 已运行：

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_BUG \
```

K02｜使用本轮与上轮记录的时钟差作为 used_jiffies：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
	ulong used_jiffies = jiffies - myos_rq->last_jiffies;
```

K03｜比较对象 tmp_rt 在遍历开始前取自首节点：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
				List_s * tmp_list = myos_rq->running_lhdr.anchor.next;
				sched_rt_entity_s *tmp_rt = container_of(tmp_list, sched_rt_entity_s, run_list);
				while ((curr_task->se.vruntime > container_of(tmp_rt, task_s, rt)->se.vruntime) &&
						tmp_list != &myos_rq->running_lhdr.anchor)
```

K04｜循环体推进 tmp_list，随后插入当前任务；本轮完整函数内未见对 tmp_rt 的另一次赋值：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
					tmp_list = tmp_list->next;
				}
				list_add_to_prev(&rt->run_list, tmp_list);
				myos_rq->running_lhdr.count++;
```

K05｜计费更新在上述回插之后：

[VERIFIED mykernel/sched/scheduler/myos_rt.c::pick_next_task_myos]
```c
		if (curr_task != rq->idle)
			curr_task->se.vruntime += used_jiffies;
```

K06｜回插接口把新节点放在指定节点前面：

[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
			__list_add_between(new, head->prev, head);
```

以上只证明所读函数的语句、相对顺序和接口操作，不证明该函数每个状态组合都在实际内核运行中可达。

## 2. 两个不同的局部见证

[INFERRED] 为讨论明确一个候选不变量：回插后的非 idle 任务按更新后的 vruntime 非递减排列；idle 可以按原设计单列在尾部。这里不把 Linux 的完整公平调度方案强加给 MyOS2，也不声称这个不变量已获 Owner 裁决。

**见证 A 是已交 V08 的游标问题。** 原夹具在本次运行中给出 [3,4,5] → [4,5,2]，相应 vruntime 排序应为 [4,2,5]。K03/K04 中 tmp_list 与 tmp_rt 没有同步前进，与这一局部输出一致。该运行证据的范围和来源仍属于 PR17，不是本轮新实跑。

**见证 B 是本轮新推导，独立于游标是否更新。** 前提：合法、互不重复的任务节点；无并发修改；current 为非 idle、TASK_RUNNING、vruntime=15；used_jiffies=10，触发选择下一任务。队列为 B(10)、C(20)、D(30)、idle，idle 节点明确保留，避免把已知 idle 不变量删掉才制造反例。

| 步骤 | 按当前函数推导的状态 |
|---|---|
| 取下一任务 | 取走 B；队列剩 C(20)、D(30)、idle。 |
| 搜索当前任务回插点 | 当前键仍是 15，首次比较 15 > 20 为假；无须进入遍历循环，直接插在 C 前。 |
| 记入本轮运行时间 | K05 把 current 的 vruntime 更新为 25。 |
| 返回前的非 idle 队列 | current(25)、C(20)、D(30)，对候选不变量失序。 |

这组数值没有整数溢出，且没有把空 anchor 当作任务；因此该见证不依赖空队列、非法容器读取或不更新 tmp_rt 的循环路径。仅在循环中更新 tmp_rt，仍不能改变这次首次比较为假的结果。**若目标是不变量所说的“按更新后的键回插”，计费与定位必须作为同一组操作考虑。** 这是设计推论，不是已提交/已执行的修复方案。

## 3. 为什么现有 V08 实跑还没有覆盖见证 B

[VRF] 已读 `core/fixtures/fx_sched.c::pick` 在调用原 pick_next_task_myos 前设置：

```c
	rq->myos.last_jiffies = jiffies;
```

[INFERRED] 在该宿主夹具没有外部时钟推进的调用中，K02 因而得到 0；它能观察游标失序，却没有覆盖正的 used_jiffies 改变排序键的情况。不能把重新复判这批相同日志说成已经补测了计费时点。

后续最小动态规格已经可以确定：对比 used_jiffies=0/10；保留 idle 和合法节点；逐步记录选择结果、回插前后节点身份/vruntime、count 与实际链长度。对计费改动的任何方案还要检查 blocked current 是否仍按原规则记账、idle 是否不记账，以及同一个时间片是否被重复累计。真正执行、修改原函数或验证新实现须另行授权，不纳入当前纯数据重核。

## 4. 对当前主线的作用

这份补充把“下一步查调度排序”缩成了两个可区分的具体问题：比较对象与游标同步、排序键更新时点。它不是新的全量学习路线，也不替代 002R/003R 的完成度和依赖材料。原学习目标仍是理解并改进自己内核的机制，已有研究与工具工作为此服务。

原件、内核和旧测试均未改。新见证 B 继续标未执行；CA-02 全局可达性、真实上下文切换和 SMP 也没有被本文件解决。源码入口为 [myos_rt.c](https://github.com/08822407d/MyOS2/blob/time/mykernel/sched/scheduler/myos_rt.c)，夹具入口为 [冻结 fx_sched.c](https://github.com/08822407d/MyOS2/blob/b843d475367a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/core/fixtures/fx_sched.c)。
