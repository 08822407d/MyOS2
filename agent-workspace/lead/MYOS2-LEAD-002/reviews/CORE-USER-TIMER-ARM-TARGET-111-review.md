---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-ARM-TARGET-111"
record_type: "independent_batch_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-20"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "2735aa3116c3"
execution_pin_short12: "ab6c14299a76"
master_direct_short12: "e8e60cd19540"
status: "RETURN"
R109_01: "CLOSED_DOCUMENTATION_ONLY"
R109_02: "CLOSED_DOCUMENTATION_ONLY"
batch109_composite: "PASS_PENDING_LOCAL"
R110_01: "OPEN"
R110_02: "OPEN"
batch110: "RETURN"
R111_01: "OPEN"
R111_02: "OPEN"
batch111: "RETURN"
full_local_P9: "NOT_RUN"
runtime: "NOT_RUN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/157-user-timer-arm-target-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-BASE-FORWARDING-110-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-BASE-FORWARDING-110-D00-108-109-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/timer-arm-target.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/timer-arm-target-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md","mykernel/scripts/options_flags.cmake","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/time/timer/timer_const.h","mykernel/time/timer/timer_macro.h","mykernel/time/timer/timer.c","mykernel/time/timer/timer_types.h","mykernel/lib/list/hash_list.h","mykernel/time/timer/timer.h","mykernel/arch/x86_64/include/asm/percpu.h","mykernel/lock_IPC/spinlock/spinlock_smp_macro.h"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":8,"quotes_reconfirmed":8,"downgraded_to_inferred":0,"scope":"本件C07定义内源码短引文"}
---

# 第111批独立验收　定时器初始化与登记目标

**主线裁定 RETURN，退回范围是引文资格和文档证据。** A/B/C 的下列源码结论可按所列前提使用。R111-01处理一条跨越函数体边界的必需引文，R111-02处理当前与显式CPU的取证、字段写入阶段及一个宏的图边。D00另有独立裁定。[D00裁定](CORE-USER-TIMER-ARM-TARGET-111-D00-109-110-review.md) · [157号合同](https://github.com/08822407d/MyOS2/blob/2735aa3116c3/agent-workspace/lead/MYOS2-LEAD-002/157-user-timer-arm-target-contract.md) · [执行者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7013194010)

## 发布身份与独立回读

实际执行头 ab6c14299a76，唯一父 d7ebfb1d5e40，比较为 ahead 1、behind 0。六新增共8580行，旧文件净修改0、删除0。分别按固定提交和活动执行分支读取六件全文及Git blob，逐件相同。第110批固定与活动六件也逐件相同。PR17顶层评论无 since，50/50/50/50/6/0 共206条，作者唯一标记在7013194010。两次按ID取得的正文与全分页正文一致。[单步差异](https://github.com/08822407d/MyOS2/compare/d7ebfb1d5e40...ab6c14299a76) · [111固定目录](https://github.com/08822407d/MyOS2/tree/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111)

| 文件 | 新增行 | 固定 blob 短值 |
|---|---:|---|
| MANIFEST.md | 87 | bf2276815a99 |
| deferred-findings.yaml | 281 | 7d027806f0bc |
| evidence.md | 617 | eda0154a93a7 |
| facts-and-boundaries.yaml | 7378 | d48c2f14f92b |
| timer-arm-target-paths.dot | 157 | 79d901e2930d |
| timer-arm-target.md | 60 | ee4775513312 |

审核时直接引用 lead=2735aa3116c3、exec=ab6c14299a76、time=a039d9803ade、master=e8e60cd19540。仅开放PR16、17，两者base均master，均未合并。16 Ready，17原题Draft。已从主线合同、公约及两分支真实源码核对读写边界、配置与panic金丝雀。

新54条引文在9份源码的54个不同窗口逐字命中，53条整段在相应定义内。22个普通定位与旧批26个位置另记。六件原始验证标签54、40位十六进制串0。固定 facts 四表8/8/10/8行，共34行；12个纸面模型。新表与模型、D00 的有编号源码窗口已对回固定证据的真实路径与整数首尾。DOT文本63节点、89边，端点全部已声明。另有8项条件候选、8项未证缺口。完整本地 P9、预处理、构建、运行、并发、实机均 NOT_RUN。

## A　初始化设置的字段

两个 setup 宏都传入对象、回调和 flags。simple_init_timer_key 依次设 entry.pprev=NULL、function=func，必要时以 TIMER_INIT_FLAGS 筛局部 flags，然后与给定 CPU 号按位或并写 timer->flags。完整初始化体未见 expires 或 entry.next 的写入。对普通有效对象取 CPU5，输入0、PINNED、PINNED|IRQSAFE、0x00340000，写回分别为 0x00000005、0x00100005、0x00300005、0x00300005。pprev 为NULL使 hlist_unhashed_lockless 的结果为真，timer_pending 再取反得假；该结论限此处读取的对象状态。[两个宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_macro.h#L19-L22) · [筛选与赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L713-L722) · [位定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_const.h#L183-L196) · [pending外层](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L60-L64)

**R111-01 OPEN。** AT111-E017 引用了 hash_list.h 的109–110行。109为函数声明头和开花括号，110才是函数体语句。冻结账保留“54次文本命中”，定义体资格计53。以 RV111-Q2 的第110行补强必需锚；签名计普通定位。让 A07、B05、M08、两条读 pprev 的图边及仍引用 E017 的其他消费者使用该追加锚。MANIFEST、证据账、报告、作者摘要均以追加说明订正，原六件和旧评论保全。[作者原锚](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md#L177) · [函数定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/hash_list.h#L107-L111)

## B　当前CPU的三个登记入口

| 入口 | 已 pending | 通过入口门后对 PINNED 的处理 | 传给下层 |
|---|---|---|---|
| add_timer | return | 保留 | timer->expires、MOD_TIMER_NOTPENDING |
| add_timer_local | return | 置位 | 同上 |
| add_timer_global | return | 清位 | 同上 |

三个入口均返回void。它们调用 __mod_timer 并丢弃其 int 返回值。在既有有效期限、回调有效且锁定路径可达的有界模型中，NOTPENDING 绕过 pending 快路径；旧候选由 flags 快照和低位CPU定位，目标候选由 get_timer_this_cpu_base 在当前CPU选 LOCAL/GLOBAL。已登记对象早退时三个入口都尚未改位。模型M05/M06/M07末列 AT111-E032 对应显式 CPU 的选择函数；当前CPU应接 E049→E033→TB110-E011，旧候选另接 E035/036→E034。对原 B08 的 void 结论直接使用 P02/P03/P04。[三个登记入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L441-L480) · [锁定旧候选](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L197-L209) · [选择当前候选](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L292-L298) · [两个选择函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L114-L146)

## C　显式CPU的 add_timer_on

| 入口前提 | 可沿源码确认的动作 |
|---|---|
| 已pending | 在置PINNED和取候选之前返回。 |
| 未pending、回调为NULL | 已置PINNED、用形参cpu选目标、按当时的flags请求锁旧候选，再到公共解锁。 |
| 回调有效、基准指针相等 | 跳过迁移分支，随后请求推进、按既有expires入桶、公共解锁。 |
| 回调有效、基准指针不同 | 设置MIGRATING，放旧锁、换base、取新锁，以显式cpu改写低位。其后推进并请求登记。 |

旧候选读到的是**置PINNED之后**的 flags，低位仍含原CPU。add_timer_on 的CPU改写操作数是形参 cpu；__mod_timer 在相应分支用 base->cpu。两个函数的源码操作数可以分别定位。add_timer_on 自身不写 expires，下层 internal_add_timer 消费 timer->expires 和推进后的 base->clk。[显式入口全体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L495-L539) · [下层登记](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L146-L152) · [另一路CPU操作数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L306-L315)

**R111-02 OPEN。** 第112批只需追加以下消费者修订。

1. 给 M05–M07 增补上节的当前CPU路径；B08、C07及提到返回类型的模型连到 P02–P05 对应签名。C08 的“本体未写”连 P11 全函数，实际消费连 E031。C10、X08 的相等分支连 E042、E045、E046，退出连 E047。X06 的异基准分支补 E042–E046；X07 公共解锁补 E047。对 P10、P15、P16 称“完整体”时让范围包含闭括号，分别到 timer.c:480、timer.h:126、140。
2. X06及M09的 **0x00100007** 是 CPU7 写回时的 flags。在固定 time、初始flags=5、目标CPU7、指针不同、迁移完成，且进入 internal_add_timer 时 clk=1000、既存 expires=1064 的具体模型，delta=64，层1，(1064>>3)+1=134，有效到期1072，idx=64+(134&63)=70。enqueue_timer 还调用 timer_set_idx，得到 (0x00100007 & ~0xFFC00000) | (70<<22) = **0x11900007**。注明各语句的取样点即可纠正作者评论“最终”二字。实际CPU、对象地址、运行结果都不在此算例内。[几何公式](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L105-L111) · [桶登记及下标写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L119-L125) · [写位公式](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L87-L92)
3. 新图第145行 on→onUnlock 把 spin_unlock_irqrestore 标作 DIRECT_CALL。固定头文件将该名字定义为宏并传 &flags 给 raw_spin_unlock_irqrestore。将这一条改成有源码定位的宏使用/展开边，同时保留 add_timer_on 中真实的公共解锁使用位置。两个 selector 的宏/返回边已修正。[作者图原边](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/timer-arm-target-paths.dot#L145) · [实际宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/spinlock/spinlock_smp_macro.h#L169-L170)

## 主线独立源码短引文

### RV111-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_BUG \
```

所读快照 C 配置中的 CONFIG_BUG。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L66)

### RV111-Q2

[VERIFIED mykernel/lib/list/hash_list.h::hlist_unhashed_lockless]
```c
			return !READ_ONCE(h->pprev);
```

单独复核的体内语句。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/hash_list.h#L110)

### RV111-Q3

[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->entry.pprev = NULL;
	timer->function = func;
	if (WARN_ON_ONCE(flags & ~TIMER_INIT_FLAGS))
		flags &= TIMER_INIT_FLAGS;
	timer->flags = flags | smp_processor_id();
```

函数体内的三项写入及局部标志筛选。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L716-L720)

### RV111-Q4

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	new_base = get_timer_this_cpu_base(timer->flags);
```

当前CPU候选的真实调用位置。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L296)

### RV111-Q5

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
		WRITE_ONCE(timer->flags,
			   (timer->flags & ~TIMER_BASEMASK) | cpu);
```

异基准分支使用显式 cpu 改写 flags。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L529-L530)

### RV111-Q6

[VERIFIED mykernel/time/timer/timer.c::enqueue_timer]
```c
	hlist_add_head(&timer->entry, base->vectors + idx);
	// __set_bit(idx, base->pending_map);
	timer_set_idx(timer, idx);
```

登记请求后还有桶号写入。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L123-L125)

### RV111-Q7

[VERIFIED mykernel/time/timer/timer.h::timer_set_idx]
```c
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
```

以桶号覆盖 ARRAYMASK 对应位。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L90-L91)

### RV111-Q8

[VERIFIED mykernel/lock_IPC/spinlock/spinlock_smp_macro.h::spin_unlock_irqrestore]
```c
	#define spin_unlock_irqrestore(lock, flags)	\
				raw_spin_unlock_irqrestore(lock, &flags)
```

公共解锁使用的名字是宏。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lock_IPC/spinlock/spinlock_smp_macro.h#L169-L170)
