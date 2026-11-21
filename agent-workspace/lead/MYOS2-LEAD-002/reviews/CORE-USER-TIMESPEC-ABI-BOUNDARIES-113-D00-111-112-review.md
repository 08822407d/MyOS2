---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMESPEC-ABI-BOUNDARIES-113"
record_type: "independent_historical_corrections_review"
status: "TWO_CLOSED_ONE_OPEN"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-20"
date_timezone: "Asia/Singapore"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "c58d1ad8f222"
execution_pin_short12: "41697d76ae5e"
R110_01: "CLOSED_DOCUMENTATION_ONLY"
R110_02: "CLOSED_DOCUMENTATION_ONLY"
batch110: "PASS_PENDING_LOCAL"
R111_01: "CLOSED_DOCUMENTATION_ONLY"
R111_02: "CLOSED_DOCUMENTATION_ONLY"
batch111_composite: "PASS_PENDING_LOCAL"
R112_01: "CLOSED_DOCUMENTATION_ONLY"
R112_02: "OPEN"
batch112: "RETURN"
R113_01: "OPEN"
batch113: "RETURN"
full_local_P9: "NOT_RUN"
runtime: "NOT_RUN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/161-user-timespec-abi-boundaries-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-D00-110-111-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-20-ktime112-reviewed-timespec113.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot","mykernel/time/timer/timer.c","mykernel/time/timer/timer.h","mykernel/time/timer/timer_const.h","mykernel/time/time_type_declaration.h","mykernel/time/misc/time_misc_const.h","mykernel/time/misc/time_misc.c","mykernel/time/misc/time_misc_types.h","mykernel/include/linux/kernel/math64.h","mykernel/time/ktime/ktime.h","mykernel/time/misc/time_misc.h"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
---

# 第113批 D00 独立裁定

| 原事项 | C07裁定 | 剩余范围 |
|---|---|---|
| R111-02 | CLOSED_DOCUMENTATION_ONLY | 七个原键、三个返回类型映射、M09两次写位全部核实。第111批组合 PASS_PENDING_LOCAL。 |
| R112-01 | CLOSED_DOCUMENTATION_ONLY | 十个宏已分别取证；time64_t 与112清单三字段均有可解引用的追加记录。 |
| R112-02 | OPEN，限模型注记与修订图 | 六例两列的整数、两次类型转换、余数形参和八条旧边的主要动作均已核收。五个非零样例的局部前提与全表说明冲突；修订图的零分支终点、负门依据须对齐源码。112保持 RETURN。 |

按照[161固定合同](https://github.com/08822407d/MyOS2/blob/c58d1ad8f222/agent-workspace/lead/MYOS2-LEAD-002/161-user-timespec-abi-boundaries-contract.md)及[112独立验收](https://github.com/08822407d/MyOS2/blob/c58d1ad8f222/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-review.md)核收[113追加事实](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml)，以[上一轮D00裁定](https://github.com/08822407d/MyOS2/blob/c58d1ad8f222/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-D00-110-111-review.md)为历史决定。文档闭合仅评定冻结原件、追加件与本回执的组合。

## R111-02　按冻结111实际键反查

对113的七条 `matrix_records`，逐条进入111的 `seven_bounded_matrices[矩阵]`，按记录 `id` 精确取行；原 `source_ids` 的内容与顺序7/7一致，另核112相应旧行的 `new_ids`。结果如下。[111原表](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/facts-and-boundaries.yaml) · [112错误键所在原表](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1518) · [113修订表](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L19)

| 矩阵 | 真实行键 | 冻结111的引文序号（前缀 AT111-E） |
|---|---|---|
| current_cpu_wrappers | B08 | 019、020、021、023、024、026 |
| explicit_cpu_add_timer_on | C07 / C08 / C10 | 045/046/047；039/040/041/045/046；039/032/034/033/051 |
| cross_path_boundaries | X06 / X07 / X08 | 037/038/039/044；037/038/041；039/040/045/046 |

另从111 `paper_models` 原取 AT111-M08、M10、M11；P02、P03、P04分别回到 timer.c 第441、457、474行，P05回到495行，都是 void 函数。113的 P17覆盖前三个定义，P18覆盖后一个。M09原表包含E044，原表未含E052；误归发生在112附记。E052第795–797行为 schedule_timeout 的栈定时器登记。移出后，E044/TS113-E018支持CPU位，E053/TS113-E017支持桶号位，E031支持 internal_add_timer 的计算与传递，TS113-E019支持 enqueue_timer 调用 timer_set_idx。[冻结111引文](https://github.com/08822407d/MyOS2/blob/ab6c14299a76/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timer-arm-target-111/evidence.md) · [113模型修订](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L149) · [显式CPU登记](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L495-L539) · [计算与入队调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L146-L152)

独立复算沿原前提：旧CPU5，目标7、初始 flags=5，PINNED后为0x00100005；基准不同且回调有效，中间CPU写完为0x00100007。推进后 clk=1000，expires=1064，delta=64 落在 [63,504)，选层1。令 t=(1064>>3)+1=134，则桶期为1072，idx=64+(134&63)=70。入桶写高位得到 (0x00100007 & ~0xFFC00000) | (70<<22) = 0x11900007。运行时到达仍待证。结合前次 R111-01 已闭合，111组合 PASS_PENDING_LOCAL。[两步的桶号编码与计算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.h#L88-L111)

## R112-01　十个定义和112的清单附记

冻结112的53个标签仍按53处字面命中、50处单一定义合格解释。复核每个替代标签的宏名、固定文件、物理行号及用途，4+2+4均可直接定位。

| 旧复合标签 | 113新标签 | 实际单宏与固定位置 |
|---|---|---|
| KD112-E026 | TS113-E002–E005 | LVL_BITS、LVL_SIZE、LVL_MASK、LVL_OFFS，timer_const.h 108–111，每宏一行。 |
| KD112-E027 | TS113-E006/E007 | TIMER_ARRAYSHIFT、TIMER_ARRAYMASK，timer_const.h 195、196。 |
| KD112-E034 | TS113-E008–E011 | KTIME_MAX、KTIME_MIN、KTIME_SEC_MAX、KTIME_SEC_MIN，time_misc_const.h 44–47，每宏一行。 |

阈值门见 TS113-E014/E015，移位、取整和桶号写回见 E012/E013/E016/E017。P23重新定位到 `typedef __s64 time64_t;` 第10行。旧清单缺的 inputs_read、open_questions、branch_canary_quotes 已作为112专属对象写在113 facts第371行起。49条MyOS2路径各不重复且在对应树有对象。按冻结112的接收和规则加载记录、111六件、110原表及历史证据源检查路径类别，两组 time/master 金丝雀逐字核实。open_questions为空限定于Owner待决定事项；技术未证项仍按112 deferred 保存。认可这份有来源的文档补记；文件存在本身不能证明历史操作者每次打开过文件。[新引文索引](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/evidence.md#L5) · [三字段附记](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L371) · [112原清单](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/MANIFEST.md) · [时间字段别名](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/time_type_declaration.h#L9-L15)

## R112-02　已核收和待改正的部分

取冻结112的 C01–C06 原始标量，用固定 ns_to_timespec64 的分支从头算。两个前向函数的门独立检查，六例均在其可表示的小数值域。下表两个返回列分别计算。[112原表](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/facts-and-boundaries.yaml#L1993) · [113转换步骤及模型](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L443) · [零、正、负路径](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L50-L69) · [按值接收与两个标量](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/ktime/ktime.h#L51-L65) · [第二个返回路径](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L130-L141)

| 模型 | 输入 ns | q / rem | (秒, 纳秒) | 经 ktime | 经 to_ns |
|---|---:|---|---|---:|---:|
| C01 | 0 | 无调用 | (0,0) | 0 | 0 |
| C02 | 1 | 0/1 | (0,1) | 1 | 1 |
| C03 | -1 | 0/0 | (-1,999999999) | -1 | -1 |
| C04 | 1000000000 | 1/0 | (1,0) | 1000000000 | 1000000000 |
| C05 | -1000000000 | 0/999999999 | (-1,0) | -1000000000 | -1000000000 |
| C06 | -1000000001 | 1/0 | (-2,999999999) | -1000000001 | -1000000001 |

负输入限定有符号内层 `-nsec - 1` 可表示。辅助函数返回 u64 商 q；外层 `-q-1` 在模2^64下算，q=0与1分别给18446744073709551615与18446744073709551614。存进有符号秒字段须声明目标转换约定。非零分支另有调用处 s32* 对形参 u32* 的类型问题，运行、编译诊断均未取证。113总表已准确分开这两件事。按冻结112图82、89、102、103、107、112、113、115核对113修订登记，显式CPU调用、pprev先写后读、两个无调用注记、safe-add直接调用者、两臂真实除法调用者、商返回与写余数/字段的分工均有固定出处。修订登记的旧边按端点与语义定位；固定原图为原始标签全文的依据。[八条历史边的对应表](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L668) · [112原图](https://github.com/08822407d/MyOS2/blob/f909700f17ab/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-ktime-deadline-arithmetic-112/ktime-deadline-arithmetic-paths.dot#L82) · [返回与余数参数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/math64.h#L41-L45) · [有符号秒字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_types.h#L7-L10)

**保留 OPEN 的具体依据**　113 facts 五个非零行的 caveat（548、577、606、635、664）均写 “negative q additionally requires signed target conversion”。q是无符号商，方向取决于输入 nsec。C02/C04写余数指针前提即可；C03/C05/C06另写负输入臂的无符号表达式与有符号目标转换。其余数值和字段已核收。113图第114行把 `nsec<0` 条件挂到只含64–65行赋值的 E051；实际条件在58行，完整体P13覆盖。第122行把零分支的 RETURN_INITIALIZED_PAIR 连到代表字段写入的 fields 节点；在52、55、58、68行可见，零输入跳过两臂，返回已初始化的结构。以返回结果为单独终点即可。后继只需追加五条局部条件与两条边。[五处注记起点](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L548) · [113图的两个端点](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries-paths.dot#L113)

## 主线独立短引文

[VERIFIED mykernel/time/timer/timer.c::add_timer_on]
```c
		WRITE_ONCE(timer->flags,
			   (timer->flags & ~TIMER_BASEMASK) | cpu);
```

CPU写回的发生点。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer.c#L529-L530)

[VERIFIED mykernel/time/timer/timer_const.h::TIMER_ARRAYSHIFT]
```c
	#define TIMER_ARRAYSHIFT		22
```

桶号位移独立宏。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/timer_const.h#L195-L195)

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

外层无符号商的取负和减一由调用者用于写入结构字段。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L64-L65)
