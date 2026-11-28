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
record_type: "independent_static_delivery_review"
followup_id: "CORE-USER-RTC-WALL-CLOCK-104"
status: "RETURN"
R104_01: "OPEN"
R104_02: "OPEN"
R103_01: "OPEN_CONSUMER_MAPPING_ONLY"
R103_02: "CLOSED_DOCUMENTATION_ONLY"
batch102_composite: "PASS_PENDING_LOCAL"
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/rtc-wall-clock.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/rtc-wall-clock-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/148-user-rtc-wall-clock-contract.md","固定time的26份引文源及直接定义，读取边界详检查点"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","time_panic":"\tthis_cpu = smp_processor_id();","master_options":"\t-m64 -mcmodel=large -fPIE \\","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":7,"quotes_reconfirmed":7,"downgraded_to_inferred":0,"scope":"本主线文件独立源码短锚"}
---

# 第104批独立审核

第104批 **RETURN，限追加文档纠正**。六件已完整入库。RTC采样、BCD解码和年月日进入启动墙钟的主链可以在下述条件内消费。R104-01处理引文与引用行错位，R104-02处理减法方向和样例输入。四个历史项另见[本轮D00裁定](CORE-USER-RTC-WALL-CLOCK-104-D00-102-103-review.md)。

## 交付与结构

[148号合同](https://github.com/08822407d/MyOS2/blob/40725302f9ee/agent-workspace/lead/MYOS2-LEAD-002/148-user-rtc-wall-clock-contract.md)、[第104批固定目录](https://github.com/08822407d/MyOS2/tree/51f1f5b5dc08/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rtc-wall-clock-104)、[单步比较](https://github.com/08822407d/MyOS2/compare/4d3183678a64...51f1f5b5dc08)、[作者正式评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6998446053)。唯一父4d3183678a64，ahead 1 / behind 0，严格六件4751行，旧件修改/删除0/0。固定头与活动头独立读取全文及blob，六件相同。

| 文件 | 新增行 | blob短SHA |
|---|---:|---|
| MANIFEST.md | 93 | d6244f0f4cfb |
| deferred-findings.yaml | 272 | 636dd55f0bf2 |
| evidence.md | 793 | a2864eb8efe7 |
| facts-and-boundaries.yaml | 3350 | 5513a3e19049 |
| rtc-wall-clock-paths.dot | 157 | 9d8fc1d64829 |
| rtc-wall-clock.md | 86 | 6b196c84dfd0 |

PR17顶层无since四页50+50+50+41=191条，作者标记仅6998446053一处；按ID回读正文与分页正文相同。审查时lead=40725302f9ee、exec=51f1f5b5dc08、time=a039d9803ade、master=e8e60cd19540。开放PR只有16及17，分别Ready和Draft，均未合并。源引文104条，全部在evidence；26份固定time原文与索引逐条验证连续1–5行、辅助位置、字面相同，六件40hex计数0。两种快照各自的金丝雀与公约整句相符。作者MANIFEST叙述公约取自lead，148要求direct master；本轮另取direct master全文并比到lead同blob，整句可用，此次补记不能代替作者当时的渠道账。下次按真实渠道填写。

七表56行、14样例、5项条件候选和7项缺口。DOT有73个显式节点、79条有向边，端点均存在。结构计数成立。未运行完整本地P9、编译、Graphviz、QEMU或实机。

## 可以消费的源码结论

- 固定源码依次请求秒、分、时、日、月、年，六字段分别写入rtc_time。每次索引选择与返回字节独立。采样函数所读范围里，锁调用、控制字节/世纪读取、更新期重试处于注释。字段时间一致性及设备当前模式仍待证。未填的星期、年内日、夏令时字段没有进入这里的六参数转换。
- 常量BCD宏与运行helper的公式对一个8位输入一致。0x59得59，0xFA按算式得160，其半字节不满足十进制数码条件。实际由哪条编译路径完成须有构建证据。解码年69/70经转换分别映射到2069/1970；月份先减1，调用历法转换时加1。
- rtc_valid_tm另有年月日、月份天数和时分秒检查。所读持久墙钟直达路径未调用它。Feb 29对2000、2001的校验区别取决于该校验器真的收到相应的完整字段。
- mktime64的一月路径先以uint作mon-=2，再把结果转int用于比较。给定所需表示和午夜输入，1970-01-01对应0秒，2000-02-29与03-01分别是11016和11017天，即951782400和951868800秒。实际读到的日期、日内时刻及RTC精度仍开放。历法式中括号内无符号日项先算再转time64_t，year*365自身仍以无符号类型求积，再与左项相加。
- read_persistent_clock64写出历法秒及零纳秒，timekeeping_init用wall_time计算偏移并调用tk_set_xtime。偏移本地变量的值、传给tk_set_wall_to_mono及该函数内真正写回字段分别立证。TK103-U02中persistent来源得到了界定，另一输入local_clock留给105。

## R104-01 OPEN　定义归属与引用行

1. **E047跨两个独立宏。** 标签为lock_cmos_prefix，引文第二行定义lock_cmos_suffix。保留真实命中数104，分别给两个宏自己的标签；直接调用点被注释由E032支持。active_sampling_steps_matrix-05当前的E033只是返回val，补正确指向。
2. **E060是宏实例化位置。** BUILDIO(b,b,char) 出现在宏定义结束后；E056–059属于定义。保留实例化作为源级展开处、行定位和交叉引用，不能用“在BUILDIO宏体内”的P2身份计入。若确有独立的声明生成单元标识，说明边界，否则以普通调用点记录。核 A采样矩阵02/03、U01以及图中的展开方向。
3. **缺少真正调用处。** E063只声明局部tm，E064是随后的两次赋值；两者均不含 mc146818_get_time(&tm)。F05、字段来源08、采样步骤08及图 persistent→mc_get 要证明丢弃返回值，须引用该表达式并联E062；F03限定未调用校验时亦应引用完整直达链。调用在固定源第45行已定位。
4. **纠正偏移行索引。** wall_clock_timekeeper_fields_matrix-04的调用在E092，E093为减法；05的调用应是E093并联E100，E094为选源/建立缓存；06应使用E094并联真实setup写shift（可复用TK103-E067等）；07用E095/E096和E098核目标类型；08应合读E095的调用及raw_sec清零、E097的注释赋值。检查M12、U06及正文实际消费者。未用字段行07应使用E036/E037/E046/E061/E085，E031只有局部变量val的声明。
5. 查核D00及其他清单时采用确定的仓库路径和文件内ID。若用类型断言，提供固定定义的签名位置/字段及配对体内证据，不把签名当作函数体内语句计数。E023仅支持缓存字段，默认mask/shift使用历史固定证据见D00回执。只修有差异的项，旧原始文件和自检不改。

## R104-02 OPEN　方向、字节含义和样例输入

- **图上减法方向反了。** 边 timekeeping_init→timespec_sub 写“wall minus boot offset”。真实源码和图中lhs/rhs恰为 **boot_offset − wall_time**。次批保留正确两条实参边，将调用标签与E093/E100对齐。
- simple_get→tm_others 标为 DATA_FLOW，边自身却说没有写入。应列局部无写注记或另分标注；mc_get→control 若表示控制字节读的注释位置，应归 simple_mc146818_gettime（E054）。若表达上层调用链的有限否定，显式写出经过该函数。不得把注释请求画成活动读。
- 字段表首六行的“CMOS_READ(...)=index”应分成请求的寄存器编号和所返数据。例如 RTC_SECONDS 为索引0，所采到的秒字节另取决于设备返回。保留既有顺序、宏值和BCD处理。
- M07/M09/M10/M11的现有输入只列公历日期。给出确切历法秒值时补明确 hour=min=sec=0。若断言rtc_valid_tm接受M07，还须给全套rtc_time输入，以2000年2月29日为例是tm_year=100、tm_mon=1、tm_mday=29，00:00:00。2001年2月29日的月份长度即可使校验拒绝，但未走校验的换算与3月1日相合应另列同一时分秒。M07和M10可交叉复用。任何笼统的C方言及转换说法限于声明的类型/编译实施前提。
- 原图、原样例与候选保留原ID。在105新六件追加一个有依据的差量表即可；正常午夜样例数值不需改变。纠正是证据和表示问题，源码修复及设备运行不在本裁定内。

## 主线独立定位

### RV104-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

固定 time 的单条配置命令节选。实际构建另行取证。 辅助位置 58–61。

### RV104-Q2

[VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::lock_cmos_prefix]
```c
	#define lock_cmos_prefix(reg)	do {} while (0)
```

第一个独立宏。 辅助位置 16–16。

### RV104-Q3

[VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::lock_cmos_suffix]
```c
	#define lock_cmos_suffix(reg)	do {} while (0)
```

第二个独立宏。 辅助位置 17–17。

### RV104-Q4

[VERIFIED mykernel/arch/x86_64/kernel/rtc.c::read_persistent_clock64]
```c
		mc146818_get_time(&tm);

		ts->tv_sec = rtc_tm_to_time64(&tm);
		ts->tv_nsec = 0;
```

完整调用表达式和后续两字段写入。 辅助位置 45–48。

### RV104-Q5

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	wall_to_mono = timespec64_sub(boot_offset, wall_time);
```

第一个参数boot_offset，第二个wall_time。 辅助位置 251–251。

### RV104-Q6

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_sub]
```c
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
			return ts_delta;
```

先算lhs减rhs，再归一。 辅助位置 80–83。

### RV104-Q7

[VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
			)*24 + hour /* now have hours - midnight tomorrow handled here */
		)*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
```

日内时、分、秒分别进入结果。 辅助位置 256–258。

后继按149号合同落实D00，原批作者已STOP_WRITING。
