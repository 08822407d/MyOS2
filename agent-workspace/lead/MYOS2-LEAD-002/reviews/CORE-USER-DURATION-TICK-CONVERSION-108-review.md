---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DURATION-TICK-CONVERSION-108"
record_type: "independent_duration_tick_conversion_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "b089a6e48c7b"
execution_pin_short12: "a8436da2c2b3"
status: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "CLOSED_DOCUMENTATION_ONLY"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
R106_01: "CLOSED_DOCUMENTATION_ONLY"
R106_02: "CLOSED_DOCUMENTATION_ONLY"
R107_01: "CLOSED_DOCUMENTATION_ONLY"
R107_02: "CLOSED_DOCUMENTATION_ONLY"
batch106_composite: "PASS_PENDING_LOCAL"
batch107_composite: "PASS_PENDING_LOCAL"
batch108: "RETURN"
R108_01: "OPEN"
R108_02: "OPEN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/154-user-duration-tick-conversion-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-D00-105-106-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/facts-and-boundaries.yaml"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":6,"quotes_reconfirmed":6,"downgraded_to_inferred":0,"scope":"本件固定源码引文"}
---

# 第108批独立审核　时长与内核节拍的双向换算

**主线裁定 RETURN，限 R108-01 和 R108-02 两项文档补正。** 固定 HZ=1000 下，普通小输入的比例、12个纸面模型和正文的五个 timespec 往返样例均通过独立复算。可消费这些数值和下列适用条件。三个历史问题已文档闭合，详见[D00裁定](CORE-USER-DURATION-TICK-CONVERSION-108-D00-106-107-review.md)。

## 交付核验

执行提交 a8436da2c2b3，唯一父 988632b3757e。compare 为 ahead 1、behind 0，严格六新增2,729行，旧修改/删除0/0。逐件读取固定提交与活动执行分支，全文和 blob 均一致。[固定六件](https://github.com/08822407d/MyOS2/tree/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108) · [单步差异](https://github.com/08822407d/MyOS2/compare/988632b3757e...a8436da2c2b3) · [作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7006324048)

| 文件 | 新增行 | blob 短值 |
|---|---:|---|
| MANIFEST.md | 91 | 0f31fe4611fb |
| deferred-findings.yaml | 246 | 1d9d37f784e5 |
| duration-tick-conversion-paths.dot | 184 | 03b76ecd1a32 |
| duration-tick-conversion.md | 59 | 7d10376e810d |
| evidence.md | 630 | 6daca3bb085c |
| facts-and-boundaries.yaml | 1519 | 05085da6c222 |

发布前独立回读 lead b089a6e48c7b、exec a8436da2c2b3、time a039d9803ade、master e8e60cd19540。开放 PR 仅16、17，分别 Open/Ready 和 Open/Draft，均 base master、未合并。PR17 无 since 读取四页、每页50条，第五页为空，共200条。正式作者标记仅见7006324048一次，按 ID 与分页取得的正文相同。

57个新证据窗分别从固定 time 回源，覆盖18个路径、57个不同窗口，全部逐字一致且为1–5行。六件原始验证词头共57，40hex 为零。另有25个普通位置。七表为8/8/8/8/8/9/8行，共57行，12个模型、6项候选、6项缺口。DOT 显式节点80、边99，所有端点均已声明。time/master 的两对金丝雀和 direct master 公约整句均已回源。完整本地 P9、编译、ELF、Graphviz、内核、硬件和实测等待均 NOT_RUN。

## 数值与成立条件

1. 固定 HZ=1000、NSEC_PER_SEC=10^9。整数表达式 (1000000000+500)/1000 的商是1000000、余数500；未加偏置时的每节拍纳秒比值恰好是整数。SHIFT_HZ=10，秒移位初值21。用源式计算出探测值 2097152000（0x7d000000），与 0x80000000 按位与为0，故秒移位改为22，纳秒移位为51。SEC_CONVERSION=4194304000，NSEC_CONVERSION=2251799814。[选支与比例](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_const.h#L156-L170)
2. 在 long/ulong 64位、uint 32位的模型下，MAX_JIFFY_OFFSET=4611686018427387902。按 SH_DIV 的商、余数分项计算得到 MAX_SEC_IN_JIFFIES=2199023255。大 j 进入 jiffies_to_usecs 时，unsigned long 乘积按64位折回为18446744073709549616，返回 uint 后为4294965296 **微秒**。该返回用作 usecs_to_jiffies 的比较值。[秒钳制宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_const.h#L175-L179) · [微秒返回类型及计算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L186-L199)
3. 通过公开入口的门、假定相关指针及整数运算有效时，0/1毫秒得到0/1个节拍；999/1000/1001微秒得到1/1/2个节拍，换回1000/1000/2000微秒。msecs_to_jiffies 的往返等值需满足 (int)m 非负并走普通转换体，同时反向结果可表示。微秒输入的门按前述 uint 比较值判断。
4. 正规 timespec 的小样例未触发秒钳制。设 s 为秒、n 为纳秒，源码的纸面计算为 `((s*4194304000) + (((n+999999)*2251799814) >> 29)) >> 22`。反向以节拍乘1000000，再按10^9取商余。0秒下 n=999999/1000000/1000001 分别回到1000000/1000000/2000000纳秒；(1秒,0) 与 (1秒,1纳秒) 分别得到1000与1001节拍。运行时长和中断次数仍待实测。[转换体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L142-L154) · [反向写回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L208-L218)

## R108-01 OPEN　定点选支、单位与定位账

| 108原位置 | 独立发现 | 下一批最小差量 |
|---|---|---|
| facts A04、S01、DT108-M01；图 hertz→shiftHz、secshiftgate→secshift | P25 是 SHIFT_HZ 的门。决定 SEC_JIFFIE_SC 是否从21改为22的门在 P15，S01 却引 P25。M01 给 E026 配28–30行，实际短锚仅29行；M01 的 51 还需 E029。 | 逐一对齐 P25+E026 与 P15+E027/E028，补 E029 及实际比例依赖。保留 E026 原窗，列探测中间值和结果。 |
| 正文 A、facts A02、U04、M07、U05–07 | 加偏置后的分子除1000有余500；U04 把微秒写作毫秒；M07 的公开入口条件应写明有符号转换门；U05–07 的反向输出只有前向引证。 | 明确原比值与带偏置整数除法，订正单位；对 U04/R08/DT108-M12 的返回宽度联 P12；M07 联 E037/E039；U05–07 联 E046。数值照上文保留。 |
| MANIFEST inputs_read、P9第3行、正文开头、evidence 索引 | 清单部分文字仍称24个普通位置或截至 P24。实际 P01–P25 共25个。 | 在新增差量行列出三处旧位置和真实总数。原57/57/0和25项定位保全。 |

## R108-02 OPEN　修正108图边及混合消费者的文件路径

| 固定108消费者 | 补正口径 |
|---|---|
| DOT 第97行 putsec → putus | 原标签止于 `CONDITIONAL_STEP: C  []`。第一宏返回0才会求第二宏，补 DT108-E017/E016/E010。108事实表 R107-02-11 的短路解释已采纳。 |
| archflag → declmacro；switchmacro → switchcall | wrapper 配置门控制 syscalls.h 的架构包装头包含，syscall_64.c 中第一条 __SYSCALL 定义有独立出处。将导入边接到真实头；switch 分支合 E004、E006 和清单实例，不以 E006 单证生成内容。 |
| delta 节点；addns → iterdiv；iterdiv → q32 | delta 取 (cycles−cycle_last) 与 tkr->mask 的按位与。辅助函数执行商余计算，边应标调用 / 商与余数来源。u32 计数和返回分别见 E011/E012，字段消费见 E057。 |
| 108 facts 中 R107-01 的混合消费者及 R107-02-10至13 | 按原证据、清单、表、模型和 DOT 边分别指向107实际文件。尤其四个以 DOT 开头的消费者，old_fixed_file 写的是 facts。保留已核收的纠正语义，下一批只补可导航的固定映射。 |

对108的退回限上述文档问题。旧图、旧表、作者评论及所有原始引文保全。其余运行前提维持延期。[108事实与模型](https://github.com/08822407d/MyOS2/blob/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/facts-and-boundaries.yaml) · [108图](https://github.com/08822407d/MyOS2/blob/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion-paths.dot) · [证据索引](https://github.com/08822407d/MyOS2/blob/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/evidence.md)

## 独立短引文

### RV108-Q1

[VERIFIED mykernel/time/systick/systick_const.h::TICK_NSEC]
```c
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)
```

分子中先加 HZ/2，再做整数除法。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_const.h#L7-L7)

### RV108-Q2

[VERIFIED mykernel/time/systick/systick_const.h::SEC_CONVERSION]
```c
	#define SEC_CONVERSION		(								\
				(ulong)((((u64)NSEC_PER_SEC << SEC_JIFFIE_SC) + \
					TICK_NSEC -1) / (u64)TICK_NSEC)				\
			)
```

秒比例的向上整除宏。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_const.h#L162-L165)

### RV108-Q3

[VERIFIED mykernel/time/systick/systick_const.h::NSEC_CONVERSION]
```c
	#define NSEC_CONVERSION		(						\
				(ulong)((((u64)1 << NSEC_JIFFIE_SC) +	\
					TICK_NSEC -1) / (u64)TICK_NSEC)		\
			)
```

纳秒比例有自己的移位与分子。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/systick/systick_const.h#L167-L170)

### RV108-Q4

[VERIFIED mykernel/time/misc/time_misc.c::timespec64_to_jiffies]
```c
	return ((sec * SEC_CONVERSION) +
		(((u64)nsec * NSEC_CONVERSION) >>
		 (NSEC_JIFFIE_SC - SEC_JIFFIE_SC))) >> SEC_JIFFIE_SC;
```

先右移纳秒乘积，再加秒贡献、做最后移位。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L151-L153)

### RV108-Q5

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
		ktime_get_real_ts64(&ts);
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
```

短路条件与两个宏调用位于同一函数体。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L19-L22)

### RV108-Q6

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	/* Calculate the delta since the last update_wall_time() */
	u64 mask = tkr->mask, delta = (cycles - tkr->cycle_last) & mask;
```

周期差使用 tkr->mask。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timekeeping/timekeeping.c#L199-L200)
