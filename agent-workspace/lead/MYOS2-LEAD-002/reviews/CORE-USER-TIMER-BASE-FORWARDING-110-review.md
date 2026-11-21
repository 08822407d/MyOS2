---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-BASE-FORWARDING-110"
record_type: "independent_batch_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "c06abde41e91"
execution_pin_short12: "d7ebfb1d5e40"
status: "RETURN"
R108_01: "CLOSED_DOCUMENTATION_ONLY"
R108_02: "CLOSED_DOCUMENTATION_ONLY"
batch108_composite: "PASS_PENDING_LOCAL"
R109_01: "OPEN"
R109_02: "CLOSED_DOCUMENTATION_ONLY"
batch109: "RETURN"
R110_01: "OPEN"
R110_02: "OPEN"
batch110: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/156-user-timer-base-forwarding-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMER-WHEEL-PLACEMENT-109-D00-108-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md","mykernel/arch/x86_64/include/asm/percpu.h","mykernel/lib/list/hash_list.h"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":7,"quotes_reconfirmed":7,"downgraded_to_inferred":0,"scope":"本件主线独立源码短引文"}
---

# 第110批独立验收　基准选择、推进与同桶修改

**整包 RETURN，限定文档纠正。** 三个技术专题在下述前提内可采用。R110-01处理事实表行号系统性失效，R110-02处理回调门顺序、旧桶号的前提以及图的调用主体。R108-02、R109-02 文档闭合；R109-01 的六处定位随 R110-01 一次纠正。[历史三项独立裁定](CORE-USER-TIMER-BASE-FORWARDING-110-D00-108-109-review.md) · [110固定成果](https://github.com/08822407d/MyOS2/tree/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110) · [作者正式评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7011963153)

## 交付核验

唯一提交 d7ebfb1d5e40，唯一父3b2efdee5d85，compare ahead=1、behind=0。仅新增以下六件，共3377行，旧修改、删除各0。固定提交与活动执行分支逐件全文、blob一致。第109批原六件也与原固定版本一致。[单步差异](https://github.com/08822407d/MyOS2/compare/3b2efdee5d85...d7ebfb1d5e40)

| 文件 | 新增行 | Git blob 短值 |
|---|---:|---|
| MANIFEST.md | 87 | 13011e2d8986 |
| deferred-findings.yaml | 286 | b077f3112212 |
| evidence.md | 864 | 4bc15999d2be |
| facts-and-boundaries.yaml | 1857 | 3c2401b722b7 |
| timer-base-forwarding-paths.dot | 218 | 9adecaa2876b |
| timer-base-forwarding.md | 65 | c147c8c95434 |

写前 lead=c06abde41e91、exec=d7ebfb1d5e40、time=a039d9803ade、direct master=e8e60cd19540。开放PR仅16/17，base均master，均未合并，16 Ready，17 Draft。作者评论按ID两次及无since全分页回读，正文一致。验收时页长50/50/50/50/4/0，共204条，作者正式标记唯一。direct master 公约§1(2)整句、两分支两对金丝雀已独立回源。

从12个固定源码文件逐行核对74个不同的1–5行窗口，74/74逐字命中。六件原始验证标签74、40位哈希串0。另核26个普通定位，其中P11是两个分开的清单位置。五张事实表9/10/10/10/8，共47行；12个模型、8项候选、8项缺口。DOT文本中92节点、121条边，所有端点均已声明。作者的引文账成立，facts中的行号账另见 R110-01。

## A　基准选择

flags 为0x00000005与0x00100005时，掩低18位均得5，符号索引分别为 BASE_GLOBAL、BASE_LOCAL。显式 cpu 参数若为7，则 get_timer_cpu_base 使用7；get_timer_this_cpu_base 取当前 CPU 版本。返回候选地址的两个选择函数内部使用 per_cpu_ptr、this_cpu_ptr 宏。timer_bases[NR_BASES] 是所读头文件中的 extern 声明，实际对象地址及链接配置延期。lock_timer_base 读取完整 flags，仅在迁移位未置时取得候选、请求锁，锁后完整标志相等才返回，不等则解锁并重试。[基准选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L114-L146) · [快照、请求锁与再次比较](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L197-L209) · [两个宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/percpu.h#L244-L251)

## B　推进 base->clk

固定 CMake 的 C 选项包含 CONFIG_BUG，所读 C 宏 WARN_ON_ONCE(condition) 展开为条件本身。在合法对象、小距离、符合 long/ulong 判序的输入域，设置旧 clk=1000，独立代入得到下表。初始门先比较 basej 与旧clk；next 与 basej 的比较为严格晚于。前两项已返回时不会读到后面的判断。

| basej | next_expiry | 经过的分支 | 写入 clk | 出口值 |
|---:|---:|---|---|---:|
|999|1030|basej早于旧clk|无|1000|
|1000|1010|basej等于旧clk|无|1000|
|1010|999|next早于旧clk，条件退回|无|1000|
|1010|1000|取next|写1000，同值|1000|
|1010|1005|取next|写1005|1005|
|1010|1010|next等于basej，取next|写1010|1010|
|1010|1020|next严格晚于basej，取basej|写1010|1010|

给定 basej>旧clk 且 next≥旧clk 时，可写为 min(basej,next)。forward 的 READ_ONCE(jiffies) 与 schedule_timeout 形成 expires 的读取彼此独立。[源码分支和采样](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L150-L175) · [比较宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_macro.h#L27-L60) · [配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L65-L66) · [WARN_ON_ONCE](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/asm-generic/bug.h#L113-L113)

## C　同桶原地更新与初算结果复用

mod_timer、mod_timer_pending、timer_reduce 各传 0、PENDING_ONLY、REDUCE。给定有效、稳定 pending 的对象、非空回调、适用的有符号差值域、可用锁和**正确的既存 flags 桶号**，下表从固定分支复算。桶样例的初算 clk 为1000，设旧桶也按同一基准登记。源代码从 flags 取旧 idx，用本次的期限和基准计算新 idx。不能只由“旧 expires=1064，当前 clk=1000”推得既存桶号。

| 入口与旧→新期限 | 核收的写入、入桶与返回 |
|---|---|
| 普通修改，1064→1064 | diff=0，锁前返回1。 |
| 普通修改，1064→1065 | 旧桶70，新期限也选70，桶期限1072；同桶分支写 timer->expires 后返回1，保留原链。此前 forward 可能写过 base->clk。 |
| REDUCE，1064→1065；1065→1064 | 延长请求在锁前返回1，保留旧期限；缩短请求在所设同桶条件下写1064，解锁返回1。 |
| 普通修改，1063→1064 | 给定旧桶69，新桶70，先摘链。若初算 clk 与最终 clk 均为1000且 idx 有效，直接向70号桶登记，桶期限1072；不同则按最终基准重算。没有基准指针相等的复用测试。 |
| PENDING_ONLY，原未登记 | 有效回调时可先推进基准；detach_if_pending 返回0，跳过写 expires 和新登记，公共解锁出口返回0。 |

同桶只写期限是对该 timer 的这个分支而言。异桶时单节点可使旧基准置 next_expiry_recalc，随后的新桶登记是否清此标志又有独立条件。pending 的判定取自 entry.pprev；真实链遍历、并发、异步回调和地址有效性均不在模型内。[锁内选桶和同桶出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L246-L278) · [摘链、重选、复用与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L292-L337) · [旧下标解码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L81-L85)

## R110-01 OPEN　修复事实表的223个源位置

实际异常全部在固定 facts 的 `source_paths` / `source_locations`，首例位于42行，末例1839行。每处写成 `#LNaN-L<原起点>`。右端恰是原引文的**起点**。源文件、证据编号和 evidence.md 的引文本身均可恢复。不能把带 NaN 的字符串当作可回源的行窗，也不能把右端简单复制两次。MANIFEST 的P9第4行只对 evidence 原窗成立。[首个坏位置](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml#L42) · [末个坏位置](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/facts-and-boundaries.yaml#L1839) · [自检声明](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/MANIFEST.md#L70-L71)

| 消费区 | 涉及记录 | 异常出现次数 |
|---|---:|---:|
| D00（28条） | 13 | 23 |
| 基准表 A / 推进表 B | 8 / 10 | 26 / 38 |
| pending表 C / 桶复用表 D | 10 / 10 | 31 / 35 |
| 交叉检查表 X / 纸面模型 | 8 / 12 | 33 / 37 |
| 合计 | **71** | **223** |

按冻结消费者的“记录ID＋字段内序号”取同位证据ID，可唯一恢复。223处涉及72个证据ID、71种错误字符串。E017和E051共享 timer.c 原起点296，正确窗口分别为296–296、296–298。以下是C07从固定 time 逐一确认的对照，数字为 TB110-E 后缀与包含首尾的行号。111可以沿用本字典，给出按消费者逐项应用的覆盖结果；旧文件冻结。[原证据全文](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/evidence.md)

| 固定 time 源路径 | 证据编号及正确窗口 |
|---|---|
| mykernel/scripts/options_flags.cmake | 001=65，002=66 |
| mykernel/include/linux/kernel/asm-generic/param.h | 003=9 |
| mykernel/time/timer/timer_const.h | 004=183，005=184，006=185–187，007=189，035=204，036=205，037=206，066=105，067=97，068=110，069=111 |
| mykernel/time/timer/timer.h | 008=117–118，009=125，010=131–132，012=145，027=155–156，028=162–164，029=165–166，030=167，031=174，038=63，061=84，062=90–91，063=109–111 |
| mykernel/time/timer/timer.c | 013=197，014=199–200，015=201–204，016=205–208，017=296，018=306–308，019=310–312，020=313–315，033=255，034=289，039=227，040=233–236，041=237–238，042=246，043=252–255，044=257–261，045=263–264，046=271–274，047=275–277，049=286–289，050=292–294，051=296–298，052=321，053=328–331，054=337，055=358，056=391，057=417，058=159–162，059=164–167，060=169–170，064=51–55，065=56–59，070=796–799 |
| mykernel/time/timer/timer_types.h | 021=75–78，022=60–63 |
| mykernel/time/systick/systick_macro.h | 023=39，024=27–31，025=48–52，026=60 |
| mykernel/include/linux/kernel/asm-generic/bug.h | 032=113 |
| mykernel/time/syscall.c | 071=19–22 |
| mykernel/arch/x86_64/include/asm/uaccess.h | 072=191–194 |
| mykernel/include/uapi/asm-generic/errno-base.h | 073=5 |
| mykernel/time/timekeeping/timekeeping.c | 074=199–200 |

## R110-02 OPEN　校正新正文、事实表及图的解释

1. 110正文第53行把“取锁并 forward；再次检查function”写反。pending 路径是取锁、检查 function、再 forward；随后重查 pending、REDUCE 和期限。normal 路径同样先过 function 门。空回调走公共解锁，早于这次推进。新表 B10 应同时定位 E042/E043 与 E048/E049。C10 的 pending 异桶路径应引 E043/E045；E049属于另一个分支。[正文原句](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding.md#L53) · [源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L246-L264)
2. 在 D01/D02/D05/D06、X03/X04、M10/M11 对照中明确旧桶由 flags 解码，需给定既存下标或声明旧登记使用相同基准。C04 的锁后 REDUCE 判断须先通过锁前门；若以相同稳定输入便已锁前返回，则须用条件描述其锁后作用。C05 的同桶比较需 E046；C09/X06 的未登记返回联 E058/E050，D08 的数值复用联 E053，D09 关于再清标志联 TW109-E037/E038；同桶保链不等于整个调用没有 base 字段写入。[快路径与下标比较](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L227-L277) · [复用条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L321-L331)
3. DOT 第109与114行把 per_cpu_ptr、this_cpu_ptr 标成直接函数调用，实际两者都是宏。返回候选的是各选择函数。第182行从赋值阶段 changeBase 画出 DIRECT_CALL，实际调用者仍是 __mod_timer，第三处调用在315行。109X 的“Only…255/289”仅能指两个原分支；同文件 add_timer_on 第532行也调用推进。第194行三个包装的 options 流向 __mod_timer 形参；第187行初始化 UINT_MAX 须取初值证据 TW109-E007，E053只证明最终测试。[图中基准选择](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding-paths.dot#L106-L115) · [图中迁移和 options](https://github.com/08822407d/MyOS2/blob/d7ebfb1d5e40/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-base-forwarding-110/timer-base-forwarding-paths.dot#L179-L194) · [显式CPU入口的调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L523-L535)

## 主线独立源码短引文

### RV110-Q1

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_ptr]
```c
	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})
```

固定x86头中的显式CPU指针宏。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/percpu.h#L244-L248)

### RV110-Q2

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::this_cpu_ptr]
```c
	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
```

当前CPU访问由宏转交架构助手。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/percpu.h#L250-L250)

### RV110-Q3

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```

先检查函数指针，通过后才推进。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L252-L255)

### RV110-Q4

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
			WRITE_ONCE(timer->flags,
				(timer->flags & ~TIMER_BASEMASK) | base->cpu);
			forward_timer_base(base);
```

迁移门内的第三次调用表达式。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L313-L315)

### RV110-Q5

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		clk = base->clk;
		idx = calc_wheel_index(expires, clk, &bucket_expiry);
```

这次预计算针对新期限和现场游标。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L263-L264)

### RV110-Q6

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (idx == timer_get_idx(timer)) {
			if (!(options & MOD_TIMER_REDUCE))
				timer->expires = expires;
			else if (time_after(timer->expires, expires))
```

旧桶号来自 timer_get_idx。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L271-L274)

### RV110-Q7

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (idx != UINT_MAX && clk == base->clk)
		enqueue_timer(base, timer, idx, bucket_expiry);
	else
		internal_add_timer(base, timer);
```

直接入桶与重算由有效下标和游标的数值比较选择。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L328-L331)

完整本地P9、预处理、编译链接、仓库脚本与程序、Graphviz、内核、QEMU和实机均 NOT_RUN。下一批以157号合同在原执行分支追加勘误，并进入定时器初始化和登记入口。
