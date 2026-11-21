---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-REALTIME-READOUT-ABI-107"
record_type: "independent_prior_batch_documentation_disposition"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "120f1597cfdd"
execution_pin_short12: "988632b3757e"
status: "PARTIAL_DOCUMENTATION_CLOSURE"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "CLOSED_DOCUMENTATION_ONLY"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
R106_01: "CLOSED_DOCUMENTATION_ONLY"
R106_02: "OPEN_TWO_DIRECT_REFERENCES"
R107_01: "OPEN"
R107_02: "OPEN"
batch105_composite: "PASS_PENDING_LOCAL"
batch106_composite: "RETURN"
batch107: "RETURN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/153-user-realtime-readout-abi-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOOT-DELAY-ACCOUNTING-106-D00-105-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOOT-DELAY-ACCOUNTING-106-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0,"scope":"本件独立源码短锚"}
---

# 第107批 D00 独立裁定

| 历史问题 | C07 裁定 | 对原批次的影响 |
|---|---|---|
| R105-01 | CLOSED_DOCUMENTATION_ONLY | 两个余项均已补齐。连同已闭合的 R105-02，第105批原件与后续差量合读为 PASS_PENDING_LOCAL。 |
| R106-01 | CLOSED_DOCUMENTATION_ONLY | 25 个旧证据窗口均有可追踪去向。第106批证据须与本次差量合读。 |
| R106-02 | OPEN，限下表两处直接出处 | 小整数算例、一般乘法公式和大部分图表关系可消费。第106批组合状态继续 RETURN。 |

裁定对象为固定第107批 facts 的三个 D00 数组，分别有 2、25、19 行。第105、106批固定目录各六个 blob 与第107批执行树中的对应目录逐件相同。第100至104批既有文档闭合和组合 PASS_PENDING_LOCAL 继续生效。所有结论限固定源码和条件化整数推导。[107差量表](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/facts-and-boundaries.yaml) · [106原表](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/facts-and-boundaries.yaml) · [105原表](https://github.com/08822407d/MyOS2/blob/6cf2affdba45/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml)

## R105-01 两处均核收

1. RT107-E001 和 E002 分别逐字命中 tsc.c 的 cpu_khz 与 tsc_khz 两条 uint 定义。105 的 integer_conversion_and_continuity-09、frequency_source_and_units-14 和 SC105-F08 均列入差量。将 SC105-E053 与 BD106-E008/E009、RT107-E040 合读后，可区分无符号减法、临时有符号值及后续算术所需的表示条件。[固定对象与类型](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L36-L38)
2. RT107-E003/E004/E005/E007 给出结构初值、正数门、负数门及共同返回。输入零时两门均假，boot_offset_consumer-05 与 RT107-M09 的结果均为 (0,0)。P07、P19 保留参数类型的位置，E009 给出除法体内表达式。对零输入的结论无需进入该除法。[固定函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L50-L69)

## R106-01 的逐项去向

已将全部25项的旧 ID、path::symbol 和起止行同固定106证据索引逐项比较，并检查新锚及位置记录。下表编号左栏为 BD106-E，右栏 E/P 为 RT107。旧79个标签、18份源码、79个原窗口保留历史身份。[106证据索引](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/evidence.md) · [107证据索引](https://github.com/08822407d/MyOS2/blob/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md)

| 旧证据 | 可消费的新证据 | 核收口径 |
|---|---|---|
| 011、012、013、016、020 | E008、E003–007、E009、E010、E011 | 函数体的实际返回、分支、商余、读转换参数及本地大小。类型另见 P07/P18/P19。 |
| 030、032、037 | E012、E013、E014 | 计算 LPJ、写全局、声明并查询局部值。实际给 VMware 局部量赋值另用 BD106-E038。 |
| 051–055、058、059、064、074 | E015–019、E020、E021/E033/E034、E022、E023 | 逐个对应四个包装、读 LPJ、函数指针调用、循环、两个活动时钟字段和配置体。 |
| 007、027、028、029、042 | E001/E002、P09、E026/P11、E027/P12、P10/E029/E030 | 独立对象与声明各自定位。四个 extern 在 P10 分列，运行分支另查宏体。 |
| 026、040、041 | E024/P13、E025/P13、P14/E013 | 活动初值、函数指针初值和注释候选可区分。P16 的 205 行是活动 channel 初始化，206–207 行才是旧字段注释，使用 E022 时读 208–209 行。见 D107-Q4。 |

## R106-02 已采纳与余项

- 01–05、14、15、17、19 的核心纠正成立。先按 C 操作数类型计算两个表达式，再取各自低32位 a、b。在注明的汇编输出绑定和位宽前提下，高字为 floor(a×b/2^32)，传给延时函数时再加1。六个旧输入分别得到 1、1、43、2501、3001、10372。LPJ=2^31 且 HZ=1000 时，第二个低字为零，传参为1。整型常量的除法门、字面1的路径、模型自增与循环末尾减一均已定位。旧结构化模型为九项，七指事实矩阵。
- 06–07、09–13、18 的主要关系可消费。保留旧表已有 SC105-E013、BD106-E034，并接 SC105-E016 与 RT107-E013 说明早期写入的位置。SC105-E014/E015 支持同一启动函数内的先后。虚拟化平台对象、条件绑定、局部赋值与间接读取使用各自证据。HPET 字段由 E022 提供，E037 提供计算，E035/E036 给出两写一延时和重启次序。第一个比较器值的形成与写出合读 BD106-E065/E066，通道0见 D107-Q4。右移计数、窄化与设备是否接收操作各有前提。[106原图](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting-paths.dot)

| 107差量行 | 尚缺的直接出处 | 108最小追加 |
|---|---|---|
| R106-02-08，determine → early_true | 所列 SC105-E053/E054 是调整频率与返回 false 的门。BD106-E034 是调用者检查。正向返回在被调者末尾，旧行的引用未抵达。 | 将 D107-Q1 与原行三个 ID 配对，区分被调者成功返回和调用者继续启用。 |
| R106-02-16，头文件依赖 | P09 确认两个 include。BD106-E063 到 options_flags.cmake 第82行为止，架构 include 根在第83行。原行只写“time flags”，未给该根与目标消费位置。 | 将 D107-Q2/Q3 与 P09 配对，按两条 include 列目标文件和对应搜索根。实际编译选择仍待本地。 |

108可直接引用本回执四个独立 ID，作者仅需把余项绑定到冻结的旧表和图边。其余已采纳内容无需重做。

## C07 固定源码短证据

### D107-Q1

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	return true;
```

通过零值门且执行至函数末尾时返回真。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L412-L412)

### D107-Q2

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]
```cmake
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
```

同一目录清单中的通用头根与架构头根。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L80-L83)

### D107-Q3

[VERIFIED mykernel/scripts/target_kernel.cmake::target_include_directories(kernel PRIVATE)]
```cmake
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
```

kernel 目标消费该目录变量。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/target_kernel.cmake#L2-L4)

### D107-Q4

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	uint channel = 0;
```

活动的比较器通道局部值。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/hpet.c#L205-L205)

完整本地 P9、编译、运行与硬件均为 NOT_RUN。
