---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMER-WHEEL-PLACEMENT-109"
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
lead_before_publication_short12: "66576d5b78eb"
execution_pin_short12: "3b2efdee5d85"
status: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R108_01: "CLOSED_DOCUMENTATION_ONLY"
R108_02: "OPEN"
R109_01: "OPEN"
R109_02: "OPEN"
batch108: "RETURN"
batch109: "RETURN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/155-user-timer-wheel-placement-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DURATION-TICK-CONVERSION-108-D00-106-107-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/evidence.md"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":6,"quotes_reconfirmed":6,"downgraded_to_inferred":0,"scope":"本文件固定源码短引文"}
---

# 第109批独立验收　正有限超时的时间轮登记

**RETURN，限定文档纠正。** C07核收 A/B/C 的普通有效对象路径、六组数值样本和条件写入顺序。第108批 D00 已单独裁定 R108-01 文档闭合、R108-02 OPEN。109新开 R109-01（表与模型的证据定位）、R109-02（新图的调用主体和边类型）。后继可以消费下表的限定结论。[D00独立回执](CORE-USER-TIMER-WHEEL-PLACEMENT-109-D00-108-review.md) · [155号原合同](https://github.com/08822407d/MyOS2/blob/66576d5b78eb/agent-workspace/lead/MYOS2-LEAD-002/155-user-timer-wheel-placement-contract.md) · [作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7009620914)

## 交付身份与证据覆盖

固定提交 3b2efdee5d85，唯一父 a8436da2c2b3。比较为 ahead 1、behind 0，六件全部新增，共3281行，旧文件修改、删除各0。固定提交与活动执行分支逐件比较了全文和 Git blob，也重核了108固定及活动原件。[109固定目录](https://github.com/08822407d/MyOS2/tree/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109) · [单提交差异](https://github.com/08822407d/MyOS2/compare/a8436da2c2b3...3b2efdee5d85)

| 文件 | 新增行 | Git blob 短值 |
|---|---:|---|
| MANIFEST.md | 94 | 6ed288d28ed2 |
| deferred-findings.yaml | 270 | 1e8738d66054 |
| evidence.md | 799 | b058db53421b |
| facts-and-boundaries.yaml | 1881 | d217cd788d0d |
| timer-wheel-placement-paths.dot | 162 | 230bae4fc462 |
| timer-wheel-placement.md | 75 | b9d4775a25d9 |

按ID回取评论7009620914并与PR17完整无since分页正文对比，分页50+50+50+50+2，末页之后为空。202条中作者正式标记恰好一次。写前四条 direct refs 为 lead 66576d5b78eb、exec 3b2efdee5d85、time a039d9803ade、master e8e60cd19540。仅 PR16/17 开放，均对 master、未合并，16 Ready、17 Draft。双分支两对金丝雀和公约原句已独立核对。

固定 time 回源逐行比较67个不同窗口，覆盖16份源文件，每条1–5行，67/67逐字命中。六件的原始验证词头67、40位哈希串0；另核24个普通定位。六张表8/9/9/8/8/8，共50行，12个模型含6组分桶样本，7项条件候选和7项证据缺口。DOT文本有72个显式节点、85条边，端点均已声明，其中1条条件边仅有空证据括号。数量无法替代对真实调用方、宏选择条件与返回体的逐项定位。[作者证据](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/evidence.md) · [原表与模型](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/facts-and-boundaries.yaml) · [原图](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement-paths.dot)

## A　期限、基准和轮级

正且有限的 timeout 在 schedule_timeout 形成 expire=timeout+jiffies。栈 timer 与 MOD_TIMER_NOTPENDING 进入 __mod_timer 后，假定对象有效、必要的锁/基准条件成立，回调非空，idx 保持 UINT_MAX。写 timer->expires 后走 internal_add_timer，后者消费该字段与当时的 base->clk。两次时钟读数的相等限定为样本输入。[入口与后续删除请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L793-L802) · [重算分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L321-L331)

固定 HZ=1000 选择9层，每层64槽，共576。由 LVL_START(n)=63×8^(n−1) 得八个层门 63、504、4032、32256、258048、2064384、16515072、132120576；逐级严格小于。截止值1056964608，最大相对值1040187392。前述普通范围之外，转 long 为负的分支直接用 clk 掩码并写 bucket_expiry=clk；最高层还分别处理超 cutoff 与未到 cutoff 两种情况。本文只签普通无回绕的有限计算。[层门与两种特殊处理](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L51-L83) · [几何宏及选择门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_const.h#L94-L128)

## B　桶到期、槽和全数组下标

先以 delta=expires−clk 选层，再以绝对 expires 求 q=floor(expires/粒度)+1。桶期限=q×粒度，槽=q mod 64，下标=64×层+槽。分别给 jiffies 与 clk 赋表中同值，独立代入得到下表，逐字段与六组交付样本相符。

| jiffies 与 clk | timeout | expires | delta | 层 | 粒度 | q | 桶到期 | 槽 | 下标 | 多出的节拍 |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1000|62|1062|62|0|1|1063|1063|39|39|1|
|1000|63|1063|63|1|8|133|1064|5|69|1|
|1000|64|1064|64|1|8|134|1072|6|70|8|
|1001|63|1064|63|1|8|134|1072|6|70|8|
|1008|63|1071|63|1|8|134|1072|6|70|1|
|1000|504|1504|504|2|64|24|1536|24|152|32|

expires 恰为1064且选第1层时仍向后一个完整粒度。相同的63节拍在1000、1001两个基准上分别落在69、70号桶。固定函数通过指针写出桶到期值，通过 return 传递下标。[完整计算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L98-L112)

## C　入链、保留位与条件更新

有效 idx 的范围是0–575。enqueue_timer 先把 timer->entry 接到 base->vectors[idx] 对应的桶头，再调用 timer_set_idx。列表别名的实体先写 n->next，旧首节点存在时才改其 pprev，随后写 h->first 和 n->pprev。以 u32 flags、idx=69 和旧值0x00100002代入，得到0x11500002，反取高10位得69，低22位保留。 [列表实体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/list/hash_list.h#L173-L181) · [索引存取](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L81-L92)

在可用有符号差判序的小距离内，桶期限1064与旧 next_expiry 为1065/1064/1060时，只有1065一例写 next_expiry=1064、timers_pending=true、next_expiry_recalc=false，再调用 trigger_dyntick_cpu。其余两例也已经入链和写索引。置 pending_map 处及所读 trigger 函数体中的远端唤醒语句均为注释。有限分支登记后可见 del_timer_sync 请求与栈清理。实际睡眠、回调触发、硬件投递、并发锁效果均无运行核验。[登记与更新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L119-L144) · [trigger定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L87-L112)

## R109-01 OPEN　使表、模型和引用能独立回到源

| 固定109消费者 | 需补的最小关系 |
|---|---|
| A08、X08，N06/N07 | 前两行声称登记后删除且 schedule 调用为注释，其现有源窗都未到798–802，应联 P21 和该处活动调用。N06 的条件假需要 E037+E038；N07 的无活动唤醒需要函数体 P06，单用调用点 E038 不够。 |
| G03、G07、X04、M01–M07 | HZ=1000→九层须用 P08 的 #if 与 E002/E031；层门7走 E019 末行的条件与选级体，门8走 E020。X04 只引 E035，缺来源 E006 和选层 E016/E017。M01–M07 的所需移位、粒度、槽掩码与层偏移可复用 E023–E032，按实际数值依赖择取；M02–M07 用 E035 解释两种输出。 |
| C03、N08、R108-01-F（109新增行） | timer_set_idx 是函数，实际宏为 ARRAYSHIFT/ARRAYMASK。N08 的1064→1072还须 B07/E035、粒度和选层；E037只给条件比较。毫秒反向签名为旧P11，微秒为P12，新行如实订正；108原事项的裁定见D00独立回执。 |

## R109-02 OPEN　时间轮新图的调用方与边类型

固定图87行把 `base_lock -> base_forward` 标成 DIRECT_CALL。lock_timer_base 负责返回基准，随后检查回调并调用 forward_timer_base 的主体是 **__mod_timer**。若保留两个阶段之间的先后关系，标时序；直接调用边从真实调用者发出。145行 `trigger -> triggerbody` 描述同一函数正文，应标作用范围或注释，真正的调用从 enqueue_timer 的严格更早分支发出。98行 hz→depth 联 P08 的 #if，才能支撑图中的 HZ>100。108遗留的短路空边仍记 R108-02，两个问题各有固定入口。[109新图](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement-paths.dot#L81-L100) · [条件调用及函数体标注](https://github.com/08822407d/MyOS2/blob/3b2efdee5d85/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-wheel-placement-109/timer-wheel-placement-paths.dot#L137-L155) · [__mod_timer 实际顺序](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L279-L290)

## C07独立短引文

### RV109-Q1

[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```

固定有限分支连续展示设定、登记、注释和删除请求，可补 A08/X08。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L796-L799)

### RV109-Q2

[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```

回调门后调用推进函数的主体为 __mod_timer。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L286-L289)

### RV109-Q3

[VERIFIED mykernel/time/timer/timer.h::calc_index]
```c
			expires = (expires >> LVL_SHIFT(lvl)) + 1;
			*bucket_expiry = expires << LVL_SHIFT(lvl);
			return LVL_OFFS(lvl) + (expires & LVL_MASK);
```

选定层级后在同一函数中得到两个不同的输出。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L109-L111)

### RV109-Q4

[VERIFIED mykernel/time/timer/timer.c::enqueue_timer]
```c
	hlist_add_head(&timer->entry, base->vectors + idx);
	// __set_bit(idx, base->pending_map);
	timer_set_idx(timer, idx);
```

入链和记录桶号位于条件比较之前。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L123-L125)

### RV109-Q5

[VERIFIED mykernel/time/timer/timer.c::enqueue_timer]
```c
		WRITE_ONCE(base->next_expiry, bucket_expiry);
		base->timers_pending = true;
		base->next_expiry_recalc = false;
		trigger_dyntick_cpu(base, timer);
```

更早的桶期限触发这四个语句。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L139-L142)

### RV109-Q6

[VERIFIED mykernel/time/timer/timer.h::timer_set_idx]
```c
			timer->flags = (timer->flags & ~TIMER_ARRAYMASK) |
								idx << TIMER_ARRAYSHIFT;
```

写标志的是函数体，以掩码保留原位。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L90-L91)

完整本地 P9、预处理、编译链接、Graphviz、仓库程序、内核、QEMU和硬件运行均 NOT_RUN。按28、79号将以上有限补正与三个新专题合入156号；作者冻结109六件，限110新增。
