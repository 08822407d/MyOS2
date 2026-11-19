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
record_type: "independent_static_delivery_review"
followup_id: "CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105"
status: "RETURN"
R105_01: "OPEN"
R105_02: "OPEN"
batch103_composite: "PASS_PENDING_LOCAL"
batch104_composite: "PASS_PENDING_LOCAL"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/149-user-sched-clock-boot-offset-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/sched-clock-boot-offset.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/sched-clock-boot-offset-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105/evidence.md"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0,"scope":"本主线文件独立短引文"}
---

# 第105批独立审核　启动调度时钟与 boot_offset

**RETURN，限两个文档事项。** 作者交付完整，固定/活动六件一致。允许后续消费下列有源码支持的结论。R105-01涉及直接引文和来源账，R105-02涉及图边和调用归属。历史三项已单独裁定关闭。[历史D00回执](CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105-D00-103-104-review.md)

## 交付边界

[149合同](https://github.com/08822407d/MyOS2/blob/87ad6b6c914b/agent-workspace/lead/MYOS2-LEAD-002/149-user-sched-clock-boot-offset-contract.md) · [作者评论6999449227](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6999449227) · [固定六件](https://github.com/08822407d/MyOS2/tree/6cf2affdba45/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-sched-clock-boot-offset-105) · [单步差异](https://github.com/08822407d/MyOS2/compare/51f1f5b5dc08...6cf2affdba45)

执行提交6cf2affdba45的唯一父为51f1f5b5dc08。GitHub compare 为 ahead 1、behind 0、严格六件纯新增5157行，旧文件修改/删除0/0。六件逐件以固定提交、活动分支两次取回，比对全文与Git blob。审查时四条direct refs为 lead=87ad6b6c914b、exec=6cf2affdba45、time=a039d9803ade、master=e8e60cd19540。开放PR只有16、17，均以master为base，16 Ready、17 Draft，均未合并。PR17四页50+50+50+43=193条顶层评论，作者标记一次，按ID回读全文与分页中的同一条相同。

| 新文件 | blob短SHA | 新增行 |
|---|---|---:|
| MANIFEST.md | c75cfdbb5dfd | 93 |
| deferred-findings.yaml | e84855cf2b26 | 354 |
| evidence.md | f592ed342b38 | 916 |
| facts-and-boundaries.yaml | 40611a3b2faa | 3491 |
| sched-clock-boot-offset-paths.dot | fc7ca049e1f9 | 211 |
| sched-clock-boot-offset.md | debdc143f275 | 92 |

对33份固定time引文源核对所有 SC105-E001…E114 的路径、标识、辅助起止行、连续1–5行及字面原文。六件中原始验证词头114、源引文114、40hex为0。依作者ID计114条，按“路径＋起止行”去重为113处，E007和E102均取同一行支持不同上下文。七张表分别13、15、12、12、10、8、9行，共79行，17个模型、8项条件候选、8项缺口，所引本包ID均存在。DOT有89个显式节点、117条有向边，无悬空端点。双分支金丝雀与direct master公约整句已独立核对。结构核验与下面的语义裁定分开记账。

## 可消费的结论

- **时序与选择。** start_kernel 先调用 setup_arch，返回后才请求 setup_per_cpu_areas 和稍后的 timekeeping_init。setup_arch 内先处理平台，再请求 tsc_early_init。固定配置命令列出 guest 选项；原生回调初值、平台候选的检测、非空回调复制及 VMware 取得有效查询结果后的替换分别有条件。原生CPU校频函数要求0x16能力，活动指令参数写的是0x15；这里只签源码对返回字的使用。原生TSC另一条路径计算晶振kHz与比例。clocksource_tsc.freq_khz 在 early 的特定回调分支写入，随后的零值回退可改变全局 tsc_khz。
- **每CPU换算。** 早期成功分支在启用 __use_tsc 前请求初始化换算数据。早期地址有效性继承86限定。当前CPU旧表示在同一计数点给出 ns_now，新 offset 按模减平衡新表示；相同计数、合法目标CPU及一致读取是等式前提。三个独立字段读与一个结构写的源文本不能证明跨核一致版本。模板复制、单位偏移和CPU0的GS更新由不同语句承担。
- **读回与消费。** 原生调度时钟可按 TSC 换算，或按 jiffies_64 减初值再乘每tick纳秒数。调度时钟开关与 clocksource 的注册/unstable 标记各有自己的写入点。取得的 u64 交给 s64 形参之前有转换前提。RTC提供独立的 wall_time，偏移公式为 boot_offset − wall_time；tk_set_xtime 读 wall_time，偏移字段的保存语句处在注释中。
- **数值。** 按声明的有限输入，M01/M03/M04/M05 对应42000、36000、3000000、37498 kHz。M02采用源码默认宏且候选为零时回退3000000。M06为两个候选均零的条件分支。M07/M08 模板对3、6周期分别给0、1ns，M09 在同一点得到 888+12=900，M10 将乘数减半取整为715827882。HZ=1000 时 M11/M12 得0、1000000ns；M13初值为4294667296。M14的u64模结果为18446744073708551616，M15在传参前跨过有符号表示上界。M16、M17在各自整数前提下为(1,0)及1秒局部差。算式均为给定数值的核算，CPU频率、真实耗时和系统选编继续待证。

## R105-01 OPEN　把声明、条件和使用点配齐

| 原105消费者 | 本次核准的部分 | 106需追加的差量 |
|---|---|---|
| startup_order_and_gates-03/-04/-10/-12 | E023/E024 是两个候选，E025–E029 是检测与复制。E014是timekeeping调用，E057/E058是晚期重试调用方。 | 加同一配置命令68–69行、hypervisors数组里VMware项与条件KVM项；-10联合E013/E014才能定相对次序；-12加determine_cpu_tsc_frequencies自身else内注释，不以调用者证明被调者。 |
| integer_conversion_and_continuity-09、frequency_source_and_units-14、SC105-F08 | E053 是比较语句；cpu_khz/tsc_khz 的全局声明和 abs 的类型分支不在此引文。 | 分别补两对象和 math.h 的 abs / __abs_choose_expr 真实定义。若该定义被选择，uint 先作减法，随后选相应 signed int 临时量。超范围转换、最小负值的取负、有符号乘10的可表示性和最终混合比较分别设门。 |
| boot_offset_consumer-02/-03/-05、ABC_join-06、SC105-M15/M16、F07/U06 | E100/E101/E103/E104能证实返回表达式、调用和局部量。单个体内返回式不显示签名，E103不含负分支。 | 配对local_clock:51、time_misc.c:50、math64.h:41–45与其实际调用。零输入行补负分支条件/末尾return，扩写只限有差的直接消费者。可复用前次主线已核的签名依据。 |
| 每CPU读链图、read_path_and_fallback-03、ABC_join-03 | E067是 __simple_cyc2ns_read 函数体，E068调用 __cyc2ns_read。 | 补 tsc.c:32 独立别名定义，给真正使用别名的边/行加引用。 |
| 读账与去重口径 | facts、PR17评论列33份引文源；MANIFEST.inputs_read写34份。 | 说明33个唯一源路径、114个标签、113个独立窗口和重复的确切ID；冻存旧自检114/114/0和原作者评论。 |

不要求重录全套114锚。固定源码支持的主张与未闭合的逐行证据映射分别登记。第104批原问题中的签名已由前次回执和本轮历史裁定配对，本项针对105新表与新候选。

## R105-02 OPEN　纠正边方向、操作归属及调用层次

| 原位置 | 固定源码给出的关系 | 追加方式 |
|---|---|---|
| jiffies_branch → jiffies64，DATA_FLOW | native_sched_clock 读取 jiffies_64。fallback 表达式消费计数器值。 | 将计数对象连到表达式，与初值、HZ输入保持同一方向；以返回值作为输出。 |
| hyper_detect → hyper_cb，说明“selected init callback copy” | detect_hypervisor_vendor 返回选中的 h；simple_init_hypervisor_platform 接收 h，调用 copy_array，随后经 x86_init.hyper.init_platform() 间接调用。 | 分列返回值、复制操作、字段和间接调用，注明非空槽与候选条件。 |
| pcpu_offsets → percpu_ptr 及 pcpu_first → percpu_copy 的合并说明；per_cpu_scale_provenance-12 | simple_pcpu_setup_first_chunk 复制模板并填 pcpu_unit_offsets。setup_per_cpu_areas 在返回后计算 delta，把 delta+unit_offset 写入 per_cpu_offset(cpu)，per_cpu_ptr 消费后者。 | 保留间接依赖，补发布语句的实际函数和中间字段。GS和CPU地址可用性继续按86限定。 |
| 正文首段 late_time_init→hpet_time_init→tsc_init | late_time_init 内先调用 hpet_time_init，再调用 tsc_init。105图已正确列为两条直接边。 | 正文用同一调用者的先后次序表达；复用既有两条正确边。 |

下一批只追加纠正，按原105节点、证据号和实际表行给出可回查的位置。第105批作者交付状态仍是 READY_FOR_MAINLINE_REVIEW / STOP_WRITING，主线本轮结论为 RETURN。

## 主线固定源码短证据

### RV105-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
```

同一固定配置命令传入的两个选项，实际构建仍待证。 [固定源码 68–69](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L68-L69)

### RV105-Q2

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cyc2ns_read]
```c
#define __cyc2ns_read		__simple_cyc2ns_read
```

这条独立宏连接使用名称与实现。 [固定源码 32–32](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L32-L32)

### RV105-Q3

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	return (jiffies_64 - INITIAL_JIFFIES) * (1000000000 / HZ);
```

jiffies_64 是回退表达式输入。 [固定源码 237–237](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/tsc.c#L237-L237)

### RV105-Q4

[VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	copy_array(&h->init, &x86_init.hyper, sizeof(h->init));
	copy_array(&h->runtime, &x86_platform.hyper, sizeof(h->runtime));

	x86_hyper_type = h->type;
	x86_init.hyper.init_platform();
```

复制与随后调用的归属。 [固定源码 103–107](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/cpu/hypervisor.c#L103-L107)

### RV105-Q5

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```

实际发布的每CPU偏移包含 delta。 [固定源码 75–79](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/smp/setup_percpu.c#L75-L79)


完整本地P9、预处理、编译、链接、Graphviz、内核、虚拟机和实机均 NOT_RUN。普通技术发现沿18号延期。
