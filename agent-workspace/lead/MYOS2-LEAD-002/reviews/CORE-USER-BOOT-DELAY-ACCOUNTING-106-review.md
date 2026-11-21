---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-BOOT-DELAY-ACCOUNTING-106"
record_type: "independent_static_delivery_review"
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
status: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "OPEN_TWO_EVIDENCE_ITEMS"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
R106_01: "OPEN"
R106_02: "OPEN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/152-user-boot-delay-accounting-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/boot-delay-accounting-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SCHED-CLOCK-BOOT-OFFSET-105-review.md"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":6,"quotes_reconfirmed":6,"downgraded_to_inferred":0,"scope":"本件独立源码短锚"}
---

# 第106批独立审核　启动延时与 HPET 一微秒请求

**主线裁定 RETURN，限两项文档纠正。** 六件完整入库，HPET 比较器写入次序和本批列举的小整数结果可在下列前提下消费。R106-01处理证据单元，R106-02处理换算适用域、表与图的直接依据。历史R105-02已文档闭合，R105-01余两处，见[独立D00回执](CORE-USER-BOOT-DELAY-ACCOUNTING-106-D00-105-review.md)。

## 交付核验

执行提交11971296e4f3，唯一父6cf2affdba45。compare为ahead 1、behind 0，六件全部新增2487行，旧件修改/删除0/0。以固定提交与活动分支分别读取六件，全文与blob逐一相同。[152修订合同](https://github.com/08822407d/MyOS2/blob/b8745d51ba7e/agent-workspace/lead/MYOS2-LEAD-002/152-user-boot-delay-accounting-contract.md) · [固定六件](https://github.com/08822407d/MyOS2/tree/11971296e4f3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-boot-delay-accounting-106) · [单步差异](https://github.com/08822407d/MyOS2/compare/6cf2affdba45...11971296e4f3) · [作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7002447381)

| 文件 | 新增行 | blob短SHA |
|---|---:|---|
| MANIFEST.md | 94 | 2a276a9300af |
| boot-delay-accounting-paths.dot | 178 | 36b4ed5c1a2b |
| boot-delay-accounting.md | 86 | 7b9de1723eb2 |
| deferred-findings.yaml | 221 | 6a0d6d01775b |
| evidence.md | 960 | 8b16bb961423 |
| facts-and-boundaries.yaml | 948 | 9af7cc1260c6 |

四条直接引用为lead b8745d51ba7e、exec 11971296e4f3、time a039d9803ade、master e8e60cd19540。开放PR只有16、17，分别Open/Ready和Open/Draft，均以master为base、均未合并。无since分页50＋50＋50＋46＝196条PR17顶层评论，作者标记只在7002447381出现一次。按ID回读与分页正文完整相同。

逐条从固定time重取79个证据窗，18个不同路径、79个不同窗口，行数均1–5、逐字一致。全六件词头79、40hex为零。七表分别9、9、12、9、7、9、8行，共63行；结构化模型M01–M09共9项，候选/缺口各6。DOT为76个显式节点、96条边，无悬空端点。表、模型、候选、缺口及带证据标签的图边所用本批/上批编号可找到。两对金丝雀和direct master公约整句均已回源。字面命中数、作者79/79/0与定义资格分开记。

## 可以消费的源码及条件数值

- 全局 loops_per_jiffy 活动初值10371364。get_loops_per_jiffy 在u64域以 tsc_khz 乘 KHZ、除以 HZ，调度时钟启用函数写回全局。固定源码两个宏值均1000。VMware局部变量、校频回调替换和全局写入是三个可区分的位置。能否在HPET调用前采用早期值取决于校频和写入顺序。
- udelay 的常量1支路以0x10c7得到4295；值为1的非常量支路经 __udelay 包装也得到4295。__const_udelay 取全局lpj、先乘4，再使用与累加器/高字输出匹配的汇编约束，随后把自增值传给 __delay。delay_fn 的活动初始目标是 delay_loop。零输入和非零输入的跳转最终都可到标签3的末尾减一。固定文本只列输入约束，实编寄存器和经过时间未测。
- 按作者明确的32位无符号乘法、寄存器结果解释及 HZ=1000，在 lpj 分别为0、1、42000、2500000、3000000、10371364时，以整数独立复算，传给 __delay 的值分别为 **1、1、43、2501、3001、10372**。该数是函数入参。源码先对HPET通道0写cmp，调用udelay(1)，再对同一比较器写(uint)delta，随后请求启动计数器。LEGSUP等前置门、实际MMIO效果、两次写的物理间隔各自另证。
- 证据链可支持“早期写入可能影响本次读值”。late_time_init 在 hpet_time_init 返回后才调用 tsc_init；可达的后续写入对先前这次读值没有追溯影响。现有源码材料不能给出某台机器的精确微秒数。

## R106-01 OPEN　按真实定义分别记录证据

P2要求函数锚的1–5行在函数体内。签名应保留为普通定位及类型说明，并与体内语句配对。101审核已处理过同一口径。[既有裁定](https://github.com/08822407d/MyOS2/blob/b8745d51ba7e/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-IRQ-REGISTRATION-DISPATCH-101-review.md)

| 固定106的BD106-E编号 | 独立发现 | 最小追加方式 |
|---|---|---|
| 011、012、013、016、020、030、032、037、051–055、058、059、064、074 | 17个窗口从签名、返回类型或函数名开始；有的同时含可用体内表达式 | 以同一函数内真实语句替代P2窗口，类型签名另列固定位置。已有合格体内ID可直接复用。闭括号和体内注释本身无须纠正。 |
| 007、027、028、029、042 | 两个频率对象合并；extern声明窗口夹两个include；KHZ窗口混入另一个宏；HZ窗口含独立undef；通配名 __bad_* 包含四个独立声明 | 单个对象/宏各用实际标识。四个extern按需分列声明位置；宏体证明实际引用。027拆为声明与头文件依赖，028分开默认DUMMY常量和KHZ。 |
| 026、040、041 | 活动对象前的注释旧定义与活动定义混记；041整个窗口是被注释的use_tsc_delay函数文本 | 初值及函数指针分别取各自单行。旧文本按注释位置引用，活动路径中未切换可引用tsc_enable_sched_clock体内被注释的调用。 |

仅在107新增差量索引：原ID、替代/普通位置、受影响的实际表行/图边/模型或候选、修正后可支持的结论。旧引文、数字及作者评论全部冻结。R105-01的两个对象可与本项共用一份补证。

## R106-02 OPEN　补全整数适用域与跨文件关系

1. **高32位公式。** multiplication_and_width-05 只列“假定32位无符号MUL”，给出的式子却把整个 lpj×250 带入乘法。以 B＝2^32 为模，先按C类型完成两个表达式，再分别取低32位为a、b。高字是 floor(a×b/B)，在结果按预期进入 xloops 时传给 __delay 的值为高字＋1。对 udelay(1) 及本批六个 lpj 样例两输入均容于u32，原六个数全保留。作为纸面区分，若假定64位unsigned long、HZ=1000、lpj=2^31，后一乘数为125×B，取低字得0，故高字为0。此例不涉及仓库程序或实际时钟。正文的特例与表中一般公式需分开。模型M05–M08的“＋1”补E057，M09的jz补E060。
2. **宏判断与样例。** macro_const_and_wrappers-12说负数或其他整数类型令 n/20000>=1 和 n>=20000 不等价。对同一普通C整数值、除数20000及通常算术转换，两谓词等价；负整数的两个结果皆假。随后与ul常量相乘的结果可能不同，应另列。区分特定调用 udelay(1) 和通用 udelay(n)：字面1不会达到超阈值bad分支。若讨论非常量包装，用独立变量样例。原正文末“七组纸面模型”未说明分组；本批可复查模型为九项。   
3. **直接来源与图边。** start→late 目前E024位于late的函数体，应用SC105-E015证明调用，配E014证明与timekeeping的先后；cross_topic_timeline-01还须SC105-E016、BD106-E032，-03改用SC105-E014/E015。determine→early_true 用原SC105-E053/E054及BD106-E034补返回门；VMware回调的初值/改写与determine中的实际间接调用分别标明。vmsetup→lpj_local的赋值用E038，E037只给局部声明和查询。localclk→clockvalue如果指返回值传递，应表达native结果进入local_clock并交给调用者；E011表明local_clock返回native_sched_clock()。
4. **HPET字段与头文件。** hpet_comparator_two_writes-05和mult→delta援引E064，此窗只有通道和两行注释。活动读取clksrc->mult/shift 在208–209行，配E065的算式。-06用E065/E066；-07和udmacro→write2联合E067–E069，清楚表示在同一调用者中的先后。区分“有条件到达”与“直接调用”。loop_and_dispatch-04及delayloop→loop_asm应给E060–E062；不要以E059单证末尾减一。mdelay旁注0x418937与活动0x418938分别定位。头链 linux/kernel/delay.h → asm/delay.h → asm-generic/delay.h 的两条include及实际include搜索根各独立定位。   
5. **算式前提。** HPET delta模型要给活动mult/shift、64位右移合法计数和窄化假设。原报告已经把设备效果保留待证；这些前提只限制源码算式的解释。修正优先面向实际直接消费者和新图中相关边，不重做整棵时钟树。

## 主线独立短证据

### RV106-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

固定配置声明。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L61)

### RV106-Q2

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	xloops *= 4;
	asm(	"mull	%%edx	\n\t"
		:	"=d"(xloops), "=&a"(d0)
		:	"1"(xloops), "0"(lpj * (HZ / 4)));
```

C乘4和内联汇编的输入输出约束。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/lib/delay.c#L194-L197)

### RV106-Q3

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	__delay(++xloops);
```

高字结果之后自增并传参。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/lib/delay.c#L199-L199)

### RV106-Q4

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	u32 mult = clksrc->mult;
	u32 shift = clksrc->shift;
```

活动的乘数和位移读取。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/hpet.c#L208-L209)

### RV106-Q5

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
	udelay(1);
```

首次CMP写与一微秒请求。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/hpet.c#L222-L224)

### RV106-Q6

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
	hpet_start_counter();
```

第二次CMP写与计数器启动。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/kernel/hpet.c#L232-L233)


完整本地P9、预处理/编译/链接、Graphviz、仓库模型程序、QEMU、硬件与并发均NOT_RUN。
