---
task_id: "MYOS2-LEAD-002"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "40725302f9ee"
execution_pin_short12: "51f1f5b5dc08"
master_direct_short12: "e8e60cd19540"
read_channel: "connector"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "independent_prior_documentary_closure"
followup_id: "CORE-USER-RTC-WALL-CLOCK-104-D00"
status: "THREE_CLOSED_ONE_OPEN"
R102_01: "CLOSED_DOCUMENTATION_ONLY"
R102_02: "CLOSED_DOCUMENTATION_ONLY"
R102_03: "CLOSED_DOCUMENTATION_ONLY"
batch102_composite: "PASS_PENDING_LOCAL"
R103_01: "OPEN_CONSUMER_MAPPING_ONLY"
R103_02: "CLOSED_DOCUMENTATION_ONLY"
batch103_composite: "RETURN"
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/rtc-wall-clock.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/hpet-enable-clocksource-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/deferred-findings.yaml","148及前一主线两份回执、固定time定义及本件索引"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","time_panic":"\tthis_cpu = smp_processor_id();","master_options":"\t-m64 -mcmodel=large -fPIE \\","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0,"scope":"本主线文件独立源码短锚"}
---

# 104号成果中四项历史纠正的独立裁定

第102批原件、103追加、前次部分闭合与本回执合读，组合 **PASS_PENDING_LOCAL**。第103批组合 **RETURN**，仅 **R103-01 的消费者映射**仍开。闭合均限文档。原作者自检与历史裁定按原提交保存。

| 项 | 本轮裁定 | 实际通过的部分 |
|---|---|---|
| R102-02 | CLOSED_DOCUMENTATION_ONLY | 完整函数名、配置命令、缺失的initcall中间层、A-M03和B-P09分流，以及clocksource/time头实际读账 |
| R102-03 | CLOSED_DOCUMENTATION_ONLY | 41条旧边与原始DOT顺序/端点/类别对应，8条旧改判保留，4条重点边已逐项订正 |
| R103-01 | OPEN，仅消费者登记 | 十条替代、读账、快照字段全部采纳。处理下表中实际引用、间接依赖和误列ID后再核收 |
| R103-02 | CLOSED_DOCUMENTATION_ONLY | 旧clock分支、两个算术转换、两层取负、余数参数类型、默认mask可达域均已分清 |

**固定依据。** [旧102证据](https://github.com/08822407d/MyOS2/blob/1a92f9839e53/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/evidence.md)、[旧103事实表](https://github.com/08822407d/MyOS2/blob/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/facts-and-boundaries.yaml)、[前次102回执](https://github.com/08822407d/MyOS2/blob/40725302f9ee/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-DOC-PARTIAL-CLOSURE-review.md)、[前次103审核](https://github.com/08822407d/MyOS2/blob/40725302f9ee/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMEKEEPING-READOUT-103-review.md)、[104纠正表](https://github.com/08822407d/MyOS2/blob/51f1f5b5dc08/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/facts-and-boundaries.yaml)及[作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6998446053)。逐个对照两批旧六件的固定blob与当前执行树，12/12身份相同。

## R102-02和R102-03

RTC104-E001–E009的配置命令和三个真实函数已核对。E015–E021连上 ___define_initcall、__unique_initcall、____define_initcall；普通声明和PREL32文本各有证据。名称、stub和section宏在固定init.h中的条件结构可定位；E021只支持其中一种section拼接。闭合范围到源级登记请求，实际预处理、链接段及启动迭代另须取证。七个指定头按104读账复核存在。TK103-E027及HC102-E049支持通道数，RTC104-E009支持周期函数保留既有配置位的条件。A-M03的输入按 hpet_readl(HPET_ID) 返回的寄存器值理解，D00里的“ID byte”不应解释为低8位截断。

重新从固定102图提取36条DIRECT_CALL、5条CALLBACK_BINDING，对照104 D00的41行及103原表。G102-10/11/12/19/25/36/39/40八项沿用。G102-20的频率更新对应HC102-E095和RTC104-E011；G102-22的入队对应E012；G102-21用E004的实际调用并联E002的freq条件；G102-31用E013调用点和E014宏展开。其余旧节点缩写依原DOT标签还原，不把它当源码符号名。原R102-01已于前轮文档闭合。第100和101批沿用已登记的组合裁定。

## R103-01的剩余差异

下表末列只统计旧103的七张矩阵、模型、候选/缺口及41行图账中，直接证据ID数组确实含该旧ID的条目。旧D00定义分组、报告叙述以及由其它字段推导的二级消费者另计。这个区分可直接用于105追加纠正。

| 旧ID | 新ID | 有效定位符 | 旧103已登记的直接行 |
|---|---|---|---|
| TK103-E001 | RTC104-E001 | set(CMAKE_C_FLAGS) | ABC-06 |
| TK103-E007 | RTC104-E002 | __clocksource_update_freq_scale | 本列所查旧结构中无逐行直接引用 |
| TK103-E008 | RTC104-E003 | __clocksource_update_freq_scale | G102-21 |
| TK103-E009 | RTC104-E004 | __clocksource_update_freq_scale | G102-21 |
| TK103-E010 | RTC104-E005 | __clocksource_update_freq_scale | 本列所查旧结构中无逐行直接引用 |
| TK103-E011 | RTC104-E006 | __clocksource_update_freq_scale | 本列所查旧结构中无逐行直接引用 |
| TK103-E012 | RTC104-E007 | __clocksource_register_scale | G102-20、G102-22 |
| TK103-E013 | RTC104-E008 | __clocksource_register_scale | G102-20、G102-22 |
| TK103-E030 | RTC104-E009 | myos_hpet_clkevt_set_state_periodic | 本列所查旧结构中无逐行直接引用 |
| TK103-E089 | RTC104-E010 | TICK_NSEC | A-F04、TK103-M01 |

旧G102-20/21/22已有本轮新指向，因此“旧表曾引用”与“现行依据”应分两列。旧E089直接指向第103批的A-F04、TK103-M01。ABC-01消费默认对象，若引用常量可注明间接；104另列的A-M01在103七表中不存在。第102批的A-M01是HPET映射地址，与TICK_NSEC无关。E007至E011在103 D00有分组支持的102 M/F条目，不能把分组下的所有条目硬拆为每段逐行直接引用。例：HC102-F01/F02属于原频率分组；应注明组级消费。自由描述如C-periodic-setting不能伪装成可解引用的行ID，临时主题应写为文字说明。105只补此映射，十个定位符无需再取十组新锚。

## R103-02为何可以关闭

1. RTC104-E024/025与固定函数体给出old_clock非空的两个写回分支。移位0仍有复合赋值。初始旧clock为空、raw分数、mono.base及随后tk_set_xtime对mono的写入已分别限定。
2. E022是clocksource本身的u32 shift字段。E023属于缓存结构。差值、写入int、负值取负和后续合法位移计数是不同前提。
3. 读固定 linux/sched/clock.h 的local_clock签名与 time_misc.c 的ns_to_timespec64签名，核到u64实参与s64形参。负值路径内层有符号取负，外层u64商取负再减一，最后赋给有符号秒字段。对输入−1，内层0、无符号外层为全1。给出最终−1依赖所用转换语义；s32余数地址与u32*形参约束单列。E027–E030是体内锚，签名在各自固定定义上下文核对。
4. 低32位mask使delta的高32位为零。104 D00末行单引E023仅能证明字段布局。接受其更正后的语义时，以原103 **TK103-E083/084/088/097/105** 和104的E010合读，支持默认对象、shift、mask后的delta与高字分支。这个限定进入现行回执，不必再打开R103-02。次批补正104索引时可复用此组。

## 主线核对样本

### D104-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

固定 time 的单条配置命令节选。实际构建另行取证。 辅助位置 58–61。

### D104-Q2

[VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	/* Initialize mult/shift and max_idle_ns */
	__clocksource_update_freq_scale(cs, scale, freq);
```

注册函数请求更新参数。 辅助位置 413–414。

### D104-Q3

[VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	clocksource_enqueue(cs);
```

单独的入队调用。 辅助位置 420–420。

### D104-Q4

[VERIFIED mykernel/time/systick/systick_const.h::TICK_NSEC]
```c
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)
```

真实定义支持旧A-F04和M01的条件计算。 辅助位置 7–7。

### D104-Q5

[VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
		} else {
			tk->tkr_mono.xtime_nsec <<= shift_change;
			tk->tkr_raw.xtime_nsec <<= shift_change;
		}
```

非负分支中的两个复合赋值。 辅助位置 166–169。

本回执裁定历史D00；104自己的RTC六件另见[104独立审核](CORE-USER-RTC-WALL-CLOCK-104-review.md)。
