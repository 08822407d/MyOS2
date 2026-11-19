---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-BOOT-DELAY-ACCOUNTING-106"
record_type: "independent_historical_documentation_disposition"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "b8745d51ba7e"
execution_pin_short12: "11971296e4f3"
master_direct_short12: "e8e60cd19540"
read_channel: "GitHub connector"
status: "PARTIAL_DOCUMENTATION_CLOSURE"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "OPEN_TWO_EVIDENCE_ITEMS"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
batch105_composite: "RETURN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/152-user-boot-delay-accounting-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0,"scope":"本件独立源码短锚"}
---

# 第106批 D00 独立回执　两项分别裁定

| 原退回 | C07裁定 | 实际余项 |
|---|---|---|
| R105-01 | OPEN，限两个证据缺口 | 两个频率对象各自的定义锚；零纳秒输入的条件和最终返回 |
| R105-02 | CLOSED_DOCUMENTATION_ONLY | 四处关系已由旧图差量、106图及固定源码相互印证 |

第105批继续 RETURN。已核准的改动可直接复用。完整本地P9、实际选编、CPU与设备执行仍待证。[上次105审核](https://github.com/08822407d/MyOS2/blob/38860d011282/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105-review.md) · [105原表](https://github.com/08822407d/MyOS2/blob/6cf2affdba45/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml) · [106差量表](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/facts-and-boundaries.yaml)

## R105-01 已核收范围

- 配置命令中的GUEST和KVM选项、候选数组中的VMware及条件KVM项，见BD106-E001/E002。detector选出并返回h，外层接收，复制非空槽后间接调用，见E003–E006。按源码候选消费，实际运行平台仍待证。
- 启动顺序以SC105-E013/E014联读。晚期被调者的else中的重新测量语句已注释，BD106-E010与调用方E035/E036明确区分。结构化差量表中“old_evidence”包含用于修正的既有编号，原105行原本列过哪些编号仍查原表。
- abs 与 __abs_choose_expr 的实际定义已定位。若此宏选中，uint减法、进入signed int临时量、取负、乘10及与uint比较分别有表示前提。宏别名 __cyc2ns_read 与实际读实现、调用点已接通。三组时钟签名、局部有符号余数与 u32* 形参的配对，作为固定位置和类型说明采纳；函数体短锚的资格在106审核单列。
- 105原索引复算为114个标签、33份不同源码、113个不同的路径及起止行。SC105-E007与E102共用同一窗口，旧MANIFEST写34份。106追加账已解释这三个数；旧六件在当前执行树中的blob与固定105一致。

## R105-01 仅余两处

1. **两个对象须分别可定位。** BD106-E007的字面成立，但定位符 `cpu_khz/tsc_khz` 是两条独立定义拼成的名称。按固定tsc.c第36、38行各列一个对象。映射回105的 integer_conversion_and_continuity-09、frequency_source_and_units-14 与 SC105-F08，联合旧SC105-E053和106的abs宏。其余定义和数学定性无需重做。[第一个对象](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L36) · [第二个对象](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L38)
2. **零输入的返回。** 105的 boot_offset_consumer-05 断言零输入返回初始结构。BD106-E014只给负分支内两次赋值，未含 `else if (nsec < 0)` 及函数末尾 `return ts`。配对已有初始化与正值门，追加负值门和最终返回即可。无需重做负纳秒算例。[固定函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L50-L69)

## R105-02 核收明细

| 原105位置 | 核准关系 | 直接依据 |
|---|---|---|
| jiffies_branch → jiffies64 | jiffies_64、初值、HZ进入回退表达式，结果进入返回值 | BD106-E017，原SC105-E093/E095/E097/E098 |
| hyper_detect → hyper_cb | detector返回h；simple_init_hypervisor_platform接收，调用copy_array并经字段调用 | BD106-E003–E006，原SC105-E030给出VMware初始绑定 |
| pcpu_first的合并说明、pcpu_offsets → percpu_ptr | helper复制模板、生成单位偏移；setup_per_cpu_areas计算delta＋unit offset并写per_cpu_offset(cpu)，宏消费该偏移 | BD106-E018/E019/E021–E023，SC105-E080；[宏定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/percpu.h#L244-L248) |
| 正文late调用链 | late_time_init先直接调用hpet_time_init，返回后直接调用tsc_init，两条边的调用者相同 | BD106-E024/E025 |

以上只关闭指定旧图问题。106图自身的证据标签和宏分支见[本批独立审核](CORE-USER-BOOT-DELAY-ACCOUNTING-106-review.md)。[106固定图](https://github.com/08822407d/MyOS2/blob/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting-paths.dot)

## 主线独立短证据

### D106-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
```

同一条配置命令的选项。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L68-L69)

### D106-Q2

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cpu_khz]
```c
uint __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */
```

第一个对象的独立定义。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L36-L36)

### D106-Q3

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_khz]
```c
uint __read_mostly tsc_khz;
```

第二个对象的独立定义。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L38-L38)

### D106-Q4

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_nsec = rem;
	} else if (nsec < 0) {
```

负值条件在赋值之前。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L57-L58)

### D106-Q5

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	return ts;
```

两个条件都跳过时仍返回已初始化的结构。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L68-L68)


这些是C07本次的定位。107可引用本回执的固定ID及文件路径，用差量说明补齐作者侧原表。
