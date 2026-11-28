---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-KTIME-DEADLINE-ARITHMETIC-112"
record_type: "independent_D00_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
date: "2026-11-20"
date_timezone: "Asia/Singapore"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "92d7b5af084f"
execution_pin_short12: "f909700f17ab"
status: "THREE_CLOSED_ONE_OPEN"
R109_01: "CLOSED_DOCUMENTATION_ONLY"
R109_02: "CLOSED_DOCUMENTATION_ONLY"
batch109_composite: "PASS_PENDING_LOCAL"
R110_01: "CLOSED_DOCUMENTATION_ONLY"
R110_02: "CLOSED_DOCUMENTATION_ONLY"
batch110_composite: "PASS_PENDING_LOCAL"
R111_01: "CLOSED_DOCUMENTATION_ONLY"
R111_02: "OPEN"
batch111: "RETURN"
R112_01: "OPEN"
R112_02: "OPEN"
batch112: "RETURN"
full_local_P9: "NOT_RUN"
runtime: "NOT_RUN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/160-user-ktime-deadline-arithmetic-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-D00-109-110-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-20-timer-arm111-reviewed-ktime112.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/timer-arm-target-paths.dot","mykernel/time/timer/timer.c","mykernel/time/timer/timer_const.h","mykernel/time/timer/timer.h","mykernel/lib/list/hash_list.h","mykernel/lock_IPC/spinlock/spinlock_smp_macro.h"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0,"scope":"本件C07独立源码短引文"}
---

# 第112批 D00 独立核收

| 事项 | 主线决定 | 裁定范围 |
|---|---|---|
| R110-01 | CLOSED_DOCUMENTATION_ONLY | 71条原记录的字段、223项同位ID、路径和整数行窗全部可还原。 |
| R110-02 | CLOSED_DOCUMENTATION_ONLY | 三个包装实参和哨兵初值、最终复用测试均已在修订表及图中独立定位。110组合 PASS_PENDING_LOCAL。 |
| R111-01 | CLOSED_DOCUMENTATION_ONLY | 替代引文仅取第110行；对全部直接消费者生效，含旧缺口 AT111-U04。 |
| R111-02 | OPEN，限映射表余项 | 三条CPU路径、等/异基准、两次写位与解锁宏均已核收。七个旧行键、三个模型的返回类型来源以及M09的旧引文还需订正。111保持 RETURN。 |

本件按[160号合同](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/160-user-ktime-deadline-arithmetic-contract.md)与[111独立裁定](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-review.md)审查[112追加事实表](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml)、[修订图](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot)及[执行者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7029831144)。下述关闭限文档与源级证据；实际硬件与本地P9待证。

## R110-01　从原字段解引用

逐条枚举110原表的87条记录、375个位置。对含无效行号的位置，取原字段及字段内0起序号，核对原 source_ids、111同位索引和112字段覆盖，再用111字典的路径与首尾行切取固定 time 原文，与110引文比对。71条受影响记录、223项均匹配，无遗漏或重复。72个不同证据ID对应71种旧错误字面；74条字典引文全部逐字命中。[110原始事实](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml) · [110原始引文](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md) · [111冻结索引](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml) · [112的71行追加规则](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L133)

| 原区段 | 实际字段 | 记录数 | 位置数 |
|---|---|---:|---:|
| D00 | source_locations | 13 | 23 |
| base_selection | source_paths | 8 | 26 |
| forwarding_branch | source_paths | 10 | 38 |
| pending_and_options | source_paths | 10 | 31 |
| bucket_reuse_and_recalc | source_paths | 10 | 35 |
| cross_checks | source_paths | 8 | 33 |
| models | source_paths | 12 | 37 |
| 合计 | 两种字段 | 71 | 223 |

D00原有23个字段键可直接使用，其余200处借112覆盖恢复为 source_paths。首项 D00/108A/source_locations[0] → TB110-E071 → mykernel/time/syscall.c 19–22；末项 models/TB110-M12/source_paths[3] → TB110-E041 → timer.c 237–238。timer.c 的 E017 与 E051 同在296行起，分别止于296和298。三个样例均从原字段重新取得。

## R110-02　O11、O12和原消费者

mod_timer_pending、mod_timer、timer_reduce 分别向 __mod_timer 传 MOD_TIMER_PENDING_ONLY、0、MOD_TIMER_REDUCE。新图 modp/mod/reduce → modcore 三条有实参的直接调用边各有自己的函数体出处。新表及图另将 __mod_timer 初值 idx=UINT_MAX（217行）与最终 idx!=UINT_MAX 且 clk==base->clk（328–331行）分开；原 TB110-E027 只支持 __forward_timer_base 在 basej 门早退。O01记清110报告第53行与110 facts/B10。结合先前已核收的 O01–O10、R109-01/02，本次将第110批原件、差量和主线回执组合裁为 PASS_PENDING_LOCAL。[差量](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1413) · [三个包装与两处判定](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot#L67-L74) · [先前D00回执](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-D00-109-110-review.md)

## R111-01　替代范围

AT111-E017 两行原文的第一行为函数签名。KD112-E002 只取 hlist_unhashed_lockless 第110行，KD112-E003 独立支持 timer_pending 外层取反。原111的54段逐字相同，53段整段处于所标定义。112同时明列通用替代及 A07、B05、AT111-M08、pprev→unhashed 和 pending→initPending。C07另按原ID查到冻结 deferred 的 AT111-U04，依同一替代规则将其内层依据指向 KD112-E002，外层依 KD112-E003，并发观察仍待证。此次关闭旧引文资格项。[112替代表](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1476) · [111原缺口](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/deferred-findings.yaml#L211) · [真实函数体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/hash_list.h#L107-L111)

## R111-02　可消费的结论及剩余字段

- __mod_timer:296 → get_timer_this_cpu_base:131–132、139，决定当前CPU候选。锁旧候选使用锁内 flags 快照、低位CPU；显式路径为 add_timer_on:508 → get_timer_cpu_base。新图第82行的动作存在，E014为被调体，调用点可取 AT111-E039。空回调发生在置PINNED及取旧基准之后。相等指针只跳过迁移，随后照常请求推进、登记并公共解锁。解锁的调用点和宏展开分别由 KD112-E019/E020 支持。原 P10/P15/P16 的完整体已补到闭括号480、126和140。[CPU路径与分支](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1495) · [显式调用点](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L502-L521) · [解锁宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/spinlock/spinlock_smp_macro.h#L169-L170)
- 以原CPU5、目标7、指针不同、回调有效、推进后clk=1000、expires=1064为前提，CPU重写后为0x00100007。delta=64，经63和504两门选层1。134=(1064>>3)+1，有效桶期为1072，idx=64+(134&63)=70。入桶再写高位得0x11900007。实际到达、指针和锁状态待证。[入桶模型](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1649) · [选层](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L51-L57) · [宏与桶几何的独立旧锚](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/evidence.md) · [下标回写](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L90-L91)
- 112 `D00.R111_02.consumers` 将冻结矩阵的 B08、C07、C08、C10、X06、X07、X08 写作 AT111-B08 等七个带前缀的键。按112所给字面在111矩阵取 `id` 均无命中。原件应分别取 current_cpu_wrappers/B08、explicit_cpu_add_timer_on/C07、C08、C10 与 cross_path_boundaries/X06、X07、X08。模型ID本身带 AT111- 前缀。[112映射](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1518) · [111原矩阵](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml)
- 旧 AT111-M08 说三个包装返回 void，M10、M11 说显式CPU入口返回 void。上轮要求这些模型各自连回签名；112表补了 B08/C07，未给这三项模型的签名映射。M08补 AT111-P02–P04，M10/M11补 P05。另将 M09 所列 AT111-E052（实际为 schedule_timeout 登记）移出该样例的写位依据。CPU重写用 AT111-E044/KD112-E016，桶号写回用 AT111-E053/KD112-E023，连接 internal_add_timer 与 enqueue_timer 时分别取 KD112-E021/E022。[112的 M09](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1617) · [111引文原账](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md) · [111主线验收条款](https://github.com/08822407d/MyOS2/blob/92d7b5af084f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-ARM-TARGET-111-review.md)

113只需在一张小型映射表补上述精确原键、模型签名与M09引文。R110-01/02、R111-01和两个已核 flags 数值可直接引用。

## 主线逐字取证

### D112-Q1

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	uint idx = UINT_MAX;
```

哨兵的实际初值。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L217-L217)

### D112-Q2

[VERIFIED mykernel/lib/list/hash_list.h::hlist_unhashed_lockless]
```c
			return !READ_ONCE(h->pprev);
```

替代窗口整行位于函数体内。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/hash_list.h#L110-L110)

### D112-Q3

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
	new_base = get_timer_cpu_base(timer->flags, cpu);
```

实际调用者传入显式CPU。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L508-L508)

### D112-Q4

[VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp_macro.h::spin_unlock_irqrestore]
```c
	#define spin_unlock_irqrestore(lock, flags)	\
				raw_spin_unlock_irqrestore(lock, &flags)
```

宏向底层传递 flags 的地址。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/spinlock/spinlock_smp_macro.h#L169-L170)
