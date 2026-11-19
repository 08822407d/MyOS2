---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "87ad6b6c914b"
execution_pin_short12: "6cf2affdba45"
master_direct_short12: "e8e60cd19540"
read_channel: "connector"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "independent_historical_documentation_disposition"
followup_id: "CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105"
status: "CLOSED_DOCUMENTATION_ONLY"
R103_01: "CLOSED_DOCUMENTATION_ONLY"
R104_01: "CLOSED_DOCUMENTATION_ONLY"
R104_02: "CLOSED_DOCUMENTATION_ONLY"
batch103_composite: "PASS_PENDING_LOCAL"
batch104_composite: "PASS_PENDING_LOCAL"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RTC-WALL-CLOCK-104-D00-102-103-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RTC-WALL-CLOCK-104-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/sched-clock-boot-offset-paths.dot"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0,"scope":"本主线文件独立短引文"}
---

# 第105批 D00 独立裁定

| 历史事项 | 本轮裁定 | 合并阅读后的批次状态 |
|---|---|---|
| R103-01，十条替代引文的消费者 | CLOSED_DOCUMENTATION_ONLY | 103 两项文档退回均已关闭，PASS_PENDING_LOCAL |
| R104-01，定义单元、调用点和引用行 | CLOSED_DOCUMENTATION_ONLY | 与 R104-02 一并关闭，104 为 PASS_PENDING_LOCAL |
| R104-02，偏移图、寄存器字段和日历输入 | CLOSED_DOCUMENTATION_ONLY | 同上 |

以 [103固定六件](https://github.com/08822407d/MyOS2/tree/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103)、[104固定六件](https://github.com/08822407d/MyOS2/tree/51f1f5b5dc08/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104)、[上次主线裁定](https://github.com/08822407d/MyOS2/blob/87ad6b6c914b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-RTC-WALL-CLOCK-104-D00-102-103-review.md)和[105追加账](https://github.com/08822407d/MyOS2/blob/6cf2affdba45/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml)共同构成有效文档。两批旧目录的十二个 blob 与105执行树逐件相同。102及之前已签的范围沿用。本文的关闭针对列名的文档事项；条件性发现、运行和完整本地P9仍按原状态登记。

## R103-01　从固定原件重算

在旧103七表、有限模型、候选/缺口以及 D00.R102_03 的41行图账中，逐条匹配直接证据数组。下列缩写 E 均指 TK103-E，替代号指 RTC104-E。

| 旧 E | 替代 E | 旧直接消费者 |
|---|---|---|
| 001 | 001 | ABC-06 |
| 007 | 002 | 0 |
| 008 | 003 | G102-21 |
| 009 | 004 | G102-21 |
| 010 | 005 | 0 |
| 011 | 006 | 0 |
| 012 | 007 | G102-20、G102-22 |
| 013 | 008 | G102-20、G102-22 |
| 030 | 009 | 0 |
| 089 | 010 | A-F04、TK103-M01 |

六个旧证据号有直接引用，四个为零，共九个“证据号×消费者”配对。作者正式评论写“其余五条”，以本表及作者结构化十行为准。G102-20 现用 HC102-E095 与 RTC104-E011，G102-21 用 RTC104-E002/E004 并保留 freq 条件，G102-22 用 RTC104-E012。HC102-M/F成组消费在另一层。E089 的 ABC-01 是间接常量依赖，A-M01 属于不同批号；旧体内定位和下游映射至此可联合使用。[103原表](https://github.com/08822407d/MyOS2/blob/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/facts-and-boundaries.yaml) · [作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6999449227)

## R104-01　已核的定义、实参和字段

| 旧问题 | 核收依据 |
|---|---|
| RTC104-E047、E060 | SC105-E002/E003 各含一个锁宏。E012 直接显示两处注释。E005/E006 位于 BUILDIO 的定义，io_arch.h 第239行是字节实例化位置。旧104原始词头计数104继续留档。 |
| getter 返回状态、字段来源和采样映射 | SC105-E004 显示单独的调用表达式及随后赋值，旧 RTC104-E038/E062 连接采样与返回，rtc-mc146818.c 第39行给出 int 签名。未填的 wday/yday/isdst 与六个转换实参以 RTC104-E036/E037/E046/E061/E085 确定。 |
| wall_clock_timekeeper_fields_matrix-04 至 -08 | 依次为 RTC104-E092 的两输入，E093/E100 的差值，E094 与 TK103-E067 的选源/缓存 shift，E095/E096/E098 的调用/目标，E095/E097 的调用与注释赋值。采用105结构化逐行账。 |
| RTC104-M12 / U06、默认源边界 | SC105-E100/E101/E103 配对下列两个固定签名，余数形参另查 math64.h 第42行。上轮 D00 已核过同一类型接口。TK103-E083/E084/E088/E097/E105 与 RTC104-E010 支持特定默认对象的 mask、shift 和运算前提。 |
| 公约 | 本轮独立读取 direct master 公约 §1 第2条。105 MANIFEST 如实登记该渠道，原104读账冻结。 |

固定接口中 local_clock 返回 u64，ns_to_timespec64 接受 s64，div_u64_rem 的余数指针形参为 u32*。调用处以 s32 rem 取址，这一类型前提继续有效。上述签名是独立的位置证据，体内调用另由短引文支持。[返回类型](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/sched/clock.h#L51-L54) · [形参和局部量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L50-L57) · [余数接口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/math64.h#L41-L45) · [getter签名](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/rtc/rtc-mc146818.c#L39-L45)

## R104-02　图、索引和日历

105图的两个实参以 boot_offset 为左、wall_time 为右。simple_get 到未写字段及 RTC_CONTROL 的两条边均为注记。RTC104-E039–E044 给出请求编号 0、2、4、7、8、9，SC105-E012 的端口返回是各次独立的未知数据。实际设备字节和一致性继续待证。[105关系图](https://github.com/08822407d/MyOS2/blob/6cf2affdba45/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/sched-clock-boot-offset-paths.dot)

对 RTC104-M07/M09/M10/M11，纠正表列明午夜和完整 tm 年/月偏移。按给出的历法与整数前提，1970-01-01 为0秒，2000-02-29 为951782400秒，次日为951868800秒，差86400秒。M08 的2001-02-29由独立校验器拒绝，绕过校验的换算另保留条件。1月无符号月份减2再转 int 的实现前提已明列。实际启动路径是否校验、读数是否有效和历法算例分别消费。

## 固定源码短证据

### D105-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

固定配置命令。 [固定源码 58–61](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L61)

### D105-Q2

[VERIFIED mykernel/arch/x86_64/kernel/rtc.c::read_persistent_clock64]
```c
		mc146818_get_time(&tm);

		ts->tv_sec = rtc_tm_to_time64(&tm);
		ts->tv_nsec = 0;
```

getter 的返回值在此表达式被丢弃。 [固定源码 45–48](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/rtc.c#L45-L48)

### D105-Q3

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	wall_to_mono = timespec64_sub(boot_offset, wall_time);
```

两个实参位置。 [固定源码 251–251](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timekeeping/timekeeping.c#L251-L251)

### D105-Q4

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_sub]
```c
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
			return ts_delta;
```

分量采用左减右。 [固定源码 80–83](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L80-L83)


第105批自身的证据限定及退回项见[105独立审核](CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105-review.md)。
