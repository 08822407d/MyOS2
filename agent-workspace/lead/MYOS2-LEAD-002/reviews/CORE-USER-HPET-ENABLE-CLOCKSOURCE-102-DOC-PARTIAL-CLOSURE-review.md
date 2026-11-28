---
task_id: "MYOS2-LEAD-002"
track_id: "MYOS2-LEAD-002"
produced_by: "GPT-6 Astra Pro（当前会话标识；不认证后台路由）"
model_per_owner: "Pro（主线）"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
lead_before_publication_short12: "7564e79a9b02"
exec_pin_short12: "4d3183678a64"
master_direct_short12: "e8e60cd19540"
phase: 2
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
record_type: "independent_partial_documentary_closure"
followup_id: "CORE-USER-HPET-ENABLE-CLOCKSOURCE-102"
status: "RETURN_PARTIAL_DOCUMENTARY_CLOSURE"
R102_01: "CLOSED_DOCUMENTATION_ONLY"
R102_02: "OPEN"
R102_03: "OPEN"
inputs_read: ["固定102六件原始文本与旧审核","103固定六件 D00、原DOT41边、所指旧新源码锚","固定time主线取证、公约和双金丝雀"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0,"scope":"本主线文件新增的短锚；不是作者包的自检或本地P9"}
---

# 第102批三项追加纠正的分别裁定

第102批维持 **RETURN**。本次只关闭 R102-01（文档口径），不签整批通过。R102-02 与 R102-03 已有可复用的纠正，本次明确尚缺哪一步。第100、101批文档闭合的效力不变。

| 项 | 本轮独立结论 | 后继只需处理 |
|---|---|---|
| R102-01 | CLOSED_DOCUMENTATION_ONLY | 无；不再重计为新源码证据 |
| R102-02 | OPEN，部分纠正可消费 | 正确符号、initcall 中间层、实际读账与修正后的依赖映射 |
| R102-03 | OPEN，41条清单与8条改判成立 | 至少 G102-20 的逐边证据；校对保留边表中的引用与实际调用者 |

## 输入与复核方式

[原102审核](https://github.com/08822407d/MyOS2/blob/7564e79a9b02/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-review.md)；[原102六件](https://github.com/08822407d/MyOS2/tree/1a92f9839e53/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102)与[原作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6978933773)；[103 D00与映射](https://github.com/08822407d/MyOS2/blob/4d3183678a64/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timekeeping-readout-103/facts-and-boundaries.yaml)、[103作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-6983342388)。两套固定结果和源码分开引用：102结果=1a92f9839e53，103追加=4d3183678a64，源码=time@a039d9803ade。旧102六个 blob 在103活动头逐件相同。

## R102-01：原始词头与真正源码锚

对旧六件原始文本逐件数协议规定的标签词头（不要求词头后有空格）。MANIFEST 的 P9-5 有1次说明示例，evidence 有130次，其余四件为0。故131次原始出现、130条实际源码引文；不能把第131次登记成源码。103 D00正确撤回旧总括，未改旧件；103六件独立为137次，均位于 evidence。此次闭合只处理计数，不为旧102或新103签署所有命题合格。

## R102-02：可保留的实质及未闭合处

- clocks_calc_mult_shift 函数体、精度/结果写回、max_adjustment、HPET_MASK 与必要底层位宏、hpet_base 对象/通道、list 插入与实参数据、raw/mono 的 clock/mask/cycle_last 和 shift、readl/writel 条件包装，均补了真实源码片段。接受与其**具体片段**相符的静态命题；硬件行为、实际启用的预处理分支、ELF和选中时钟未证。
- TK103-E007–011 标签写 update_freq_scale，源码函数全名为 __clocksource_update_freq_scale；E012–013 的 register_scale 应为 __clocksource_register_scale；E030 的 periodic 应为 myos_hpet_clkevt_set_state_periodic；E001 的人为拼写不等于单条 set(CMAKE_C_FLAGS …) 命令。片段逐字存在与定位符有效是两件事。104可一次修正并同时供 R103-01 消费，不要求复制两套。
- E050/051/052 从 fs_initcall、__define_initcall 跳到 ____define_initcall。中间有 ___define_initcall、__unique_initcall；目标生成器本身又有配置分支。补中间调用、直接所用名称/section机制至所声称边界；若只保留源码登记请求，未证预处理、最终段、迭代与运行均明确截断。宏展开不画成开机已经调用。
- D00.R102_S02 的 A-M03 括号已改正，但 new_id=TK103-E030 是周期配置读改写。通道数应指 TK103-E027（或[HC102-E049](https://github.com/08822407d/MyOS2/blob/1a92f9839e53/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/evidence.md)）；E030 在恢复真实符号后留给 B-P09。保留原A-M03/M08等结果和“未显式新增32BIT位不等于结果位清零”的修订。不将已成立的 S01/S03 再退回。
- read_ledger.source_paths 的30项恰为有引文的文件；full_primary 还列 timekeeping_api.h。原审核明确要求 clocksource.h / clocksource_const.h / clocksource_api.h、time_const.h / time_type_declaration.h 等真实读账。没有列入引文清单不证明未读，同样不能证明已经读到。104补固定路径、完整或窗口范围、用途和身份；缺读则现在补读，不追认旧作者行为。

## R102-03：边清单覆盖通过，支持关系仍有错配

从102 DOT按原顺序提取36条 DIRECT_CALL、5条 CALLBACK_BINDING，与103 facts 的 G102-01…41逐行核对原端点和类型，41/41相同。接受 G102-10/11/12/19/25/36/39/40 八项重标：摘要节点不冒充调用者，注册宏展开与函数调用区分，回调字段绑定、描述符写入、间接调用和真实设备投递分开。原图冻结；其余33行的“保留”不等于每个引用都已指向证明。

**G102-20 实例**：register_scale → update_scale 的函数调用确实存在；表给 TK103-E012/E013 却分别支持入队和注册尾部。改指固定102的 HC102-E095，真实函数名写全。G102-22 入队仍可复用相应入队证据；不要把这两行合并。G102-31 的 select 要用调用处＋宏定义说明展开后才调用真正函数；启动代码中注释的 if 也不能当作活动保护条件。后继以“旧边ID—真实符号—边类别—最小支持ID—首个未证门”作差量表，接受的八项不必重做成第四十一条新发现。

## 独立定位样本

### CL102-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

单条 set 命令的连续片段；不能证明实际编译或链接。 固定 time 短 SHA a039d9803ade；辅助位置 58–61。

### CL102-Q2

[VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	/* Initialize mult/shift and max_idle_ns */
	__clocksource_update_freq_scale(cs, scale, freq);
```

更新频率的真正调用，支持 G102-20。 固定 time 短 SHA a039d9803ade；辅助位置 413–414。

### CL102-Q3

[VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	clocksource_enqueue(cs);
```

入队是另一个动作，支持 G102-22。 固定 time 短 SHA a039d9803ade；辅助位置 420–420。

### CL102-Q4

[VERIFIED mykernel/include/linux/init/init.h::___define_initcall]
```c
			#define ___define_initcall(fn, id, __sec)			\
						__unique_initcall(fn, id, __sec, __initcall_id(fn))
```

中间宏连接到 __unique_initcall；不能直接跳过。 固定 time 短 SHA a039d9803ade；辅助位置 275–276。


## 交接

第104批只在新六件内追加。原102、原103的编号、历史作者自检、批次裁定及旧评论不重写。102只有在后续 C07 对剩余两项独立核收后才可另行改变组合状态。
