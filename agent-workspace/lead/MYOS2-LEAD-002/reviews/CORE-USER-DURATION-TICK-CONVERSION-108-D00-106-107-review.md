---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DURATION-TICK-CONVERSION-108"
record_type: "independent_historical_documentation_disposition"
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
status: "CLOSED_DOCUMENTATION_ONLY"
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
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/154-user-duration-tick-conversion-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-D00-105-106-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/duration-tick-conversion.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi-paths.dot"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0,"scope":"本件固定源码引文"}
---

# 第108批 D00　106、107 的三个开放事项文档闭合

| 事项 | C07 裁定 | 与冻结原件合读的结论 |
|---|---|---|
| R106-02 | CLOSED_DOCUMENTATION_ONLY | 被调者真返回与头文件搜索根已补齐。连同 R106-01，第106批组合为 PASS_PENDING_LOCAL。 |
| R107-01 | CLOSED_DOCUMENTATION_ONLY | CMake 配置、三次同名宏、清单实例及实际包含位置均可独立定位。 |
| R107-02 | CLOSED_DOCUMENTATION_ONLY | 归一化整数域、三条周期转换路径、五个表行及旧图的指定边已在差量中说明。第107批组合为 PASS_PENDING_LOCAL。 |

第105批及更早既有主线裁定沿用。106、107 各自六件在原固定提交与108执行树中逐一具有相同 blob。作者提交时的 OPEN 属实，本回执记录随后作出的 C07 独立裁定。本次采用固定源码、作者差量与主线回执合读，完整本地 P9 和运行仍为 NOT_RUN。[108差量](https://github.com/08822407d/MyOS2/blob/a8436da2c2b3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-duration-tick-conversion-108/facts-and-boundaries.yaml) · [107审核](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-review.md) · [106余项裁定](https://github.com/08822407d/MyOS2/blob/b089a6e48c7b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-REALTIME-READOUT-ABI-107-D00-105-106-review.md)

## R106-02　两条差量

- **-08**　SC105-E053/E054 显示频率处理和零频率早退，BD106-E034 是 tsc_early_init 的调用方条件。DT108-E056 位于被调者末尾。正向返回到达后，调用者才继续请求启用。对应107差量 R106-02-08 及106的 determine → early_true 边。仍需运行证据才能判断某次校频走哪一支。[固定被调者](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L395-L413) · [106原图](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting-paths.dot)
- **-16**　P01 的 asm/delay.h 对应架构 include 根，P02 的 asm-generic/delay.h 对应 include/linux/kernel 根。E007 明示两个候选目录，E008 明示 kernel 目标消费 INCLUDE_DIRS。这支持固定 ARCH=x86_64 时的源文件解析关系。实际预处理仍待证。[根目录列表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L78-L85) · [目标消费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/target_kernel.cmake#L2-L5)

## R107-01　配置与宏展开

四行分别审查 config、extern、table、switch。DT108-E001 属于设置 CMAKE_C_FLAGS 的同一真实 CMake 命令，P08 定位条件包含。DT108-E002/E003/E004 分别是名为 __SYSCALL 的三个定义，P03/P04 提供两个系统调用映射实例。P05–P07 提供三次包含，前两次随后 undef。E005 给出数组初始化，E006 给出实际 switch。t1-03至08 及 t2-08 可据此解引用，构建、链接及使用哪份已生成二进制仍待证。[入口源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L10-L32) · [107证据](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md)

## R107-02　十三行的核收结果

| 108差量行 | 冻结的107消费者 | 独立复核 |
|---|---|---|
| 01–03 | t4-07、t7-05、RT107-M04 | 对无回绕纳秒和 S、除数 B=10^9，商须容于 u32，最终秒须可表示。E011/E012/E013/E057 连接计数器、返回、余数和最终两字段。S=B 时商1余0；S=B×2^32 的数学商超出该计数器，普通样例仍为 (13,0)。 |
| 04、13 | t4-05、to_ns → sample_ns | E018–020 给有效分支条件。超过 max_cycles 后有高位条件的返回与安全换算调用，其余才是普通乘加右移。 |
| 05–09 | t2-06、t2-08、t5-09、t6-06、t6-11 | 依次由 E014 的 tz 注释、E002–006 和 P03/P04 的公共清单、E015 的输出赋值、E009 的实时回调返回、E010/E016 的零常量和宏体补足。 |
| 10–12 | tvnonnull → ktimeread、do_gt → putsec/putusec、ktimeread 及 iterdiv 到最终字段 | 108事实表明确区分条件、真正调用者、宏展开、逻辑或的短路与最后商余。可由 RT107-E057、DT108-E016/E017、E011/E012/E057 独立复查。 |

**旧文件的定位以上表的消费者类型为准。** 原表行和 M04 在107 facts；RT107-E/P 在107 evidence；旧图边在107 DOT；原配置读取声明另见107 MANIFEST。108事实表将若干混合消费者统一指到旧 facts，按这四个固定入口可还原。C07 已据实际原件定位并核收旧争点。108 自身图上的空标签及结构化路径索引列在[108审核](CORE-USER-DURATION-TICK-CONVERSION-108-review.md)，由下一批在新目录补正。[107原表](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/facts-and-boundaries.yaml) · [107证据](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md) · [107原图](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi-paths.dot) · [107清单](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/MANIFEST.md)

## 独立短引文

### D108-Q1

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	return true;
```

执行到被调函数末尾时返回真。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L412-L412)

### D108-Q2

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]
```cmake
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
```

普通内核头目录和架构头目录来自同一项设置。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L80-L83)

### D108-Q3

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

第三次定义使用相同宏名，生成 switch 的 case。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26-L26)

### D108-Q4

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_add_ns]
```c
			a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
			a->tv_nsec = ns;
```

秒累加有界商，纳秒写回余数。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L154-L155)
