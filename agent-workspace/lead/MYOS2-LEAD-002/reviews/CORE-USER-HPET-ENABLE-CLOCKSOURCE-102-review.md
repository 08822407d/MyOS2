---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-HPET-ENABLE-CLOCKSOURCE-102-REVIEW-001
record_type: independent_static_review_with_targeted_document_return
evidence_class: 六件全文、130条引文回源、计数与强制定义核查、条件语义和图边核收
conversation_display_name: MYOS2-A-C07 内核分析主线续建
produced_by: GPT-6 Astra Pro（当前会话模型标识；后台不可独立认证）
model_per_owner: Pro（沿用Owner主线选择）
date: 2026-11-18
base_snapshot: time（分支名）
read_channel: connector
source_pin_short12: a039d9803ade
contract146_short12: 75338cd7d005
reviewed_base_short12: 14e8f1580f5a
reviewed_head_short12: 1a92f9839e53
reviewed_delivery_comment: 6978933773
executor: MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15
status: REVIEW_COMPLETE_WITH_TARGETED_RETURN
acceptance_verdict: RETURN
returned_items: [R102-01, R102-02, R102-03]
return_scope: RAW_TEXT_ACCOUNTING_MANDATORY_DEFINITION_SUPPORT_AND_GRAPH_RELATIONS_ONLY
mandatory_supplements: [R102-S01, R102-S02, R102-S03, R101-S01, R101-S02, R101-S03]
R101_01: CLOSED_BY_SEPARATE_101_DOC_CLOSURE
batch101_composite: PASS_PENDING_LOCAL
R100_01: CLOSED_DOCUMENTATION_ONLY
batch100_composite: PASS_PENDING_LOCAL
original_author_self_check: 130/130/0
reviewer_raw_label_count: 131
reviewer_source_quote_count: 130
reviewer_literal_quote_matches: 130
reviewer_mandatory_definition_coverage: FAIL_SEE_R102_02
self_check: {verified_claims: 8, quotes_reconfirmed: 8, downgraded_to_inferred: 0, scope: 本件R102-Q1至Q8，与作者130条分账}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-TIMEKEEPING-READOUT-103
next_contract: ../147-user-timekeeping-readout-contract.md
next_executor: MYOS2-A-C07a / MYOS2-LEAD-002-CHAT-EXEC-15
executor_rotation: NONE_REUSE_CURRENT_EXEC15
phase: 2
inputs_read: [146、102六件及正式评论全文；130源锚逐条及相关定义；原101 D00消费者；直接辅助定义与103有界划界预读]
owner_decisions_requested_now: []
open_questions: []
---

# 第102批已完整交付；三项文档补正并入103

C07裁定RETURN，仅限R102-01/02/03。主要分析按本件限定保留，不重做102全部矩阵和模型，不修改旧成果，不安排内核修复。第101批D00已独立核收，R101-01由[单独闭合件](CORE-USER-IRQ-REGISTRATION-DISPATCH-101-DOC-CLOSURE-review.md)关闭，组合PASS_PENDING_LOCAL。

READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING和作者STOP_WRITING是正确的执行交付终态。作者已将六件完整写入GitHub；本次退回与缺文件、未发布或作者应自签通过无关。

## 1. 远端交付和读取账

[VRF] 1a92f9839e53唯一父14e8f1580f5a，compare为ahead1/behind0/total_commits1；严格六件added、5518行，旧文件净修改0、删除0。短SHA均由实际连接器输出复制。

目录：agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-hpet-enable-clocksource-102/。

| 文件 | 新增行数 | Git blob短SHA |
|---|---:|---|
| MANIFEST.md | 118 | 3a008fc9b176 |
| hpet-enable-clocksource.md | 101 | 861e8ac703ba |
| facts-and-boundaries.yaml | 3759 | 4dafb07822e6 |
| hpet-enable-clocksource-paths.dot | 158 | 7920cf1bd5ff |
| deferred-findings.yaml | 358 | 3376b31549ac |
| evidence.md | 1024 | 8722dd9111d3 |

六件固定与活动全文及blob一致。本C07已读六件全部正文，包括130锚、D00二十条映射、七组54行矩阵、18有限模型、8候选、8缺口和DOT；长件按结构分段读完。facts/deferred是JSON兼容YAML，纯JSON解析成功。DOT纯文本66唯一节点85边，未定义端点0；图的语义错误仍见R102-03。没有运行Graphviz。

130条均在实际固定time源文本中连续逐字命中，1–5行，辅助行号与索引一致；本次未发现101那类函数头越界锚。引文真实和定义单元合法不代表每个消费命题都有足够支持，更不代表强制定义齐全。

取回作者所列27份源文本并逐锚/直接定义阅读；不把27文件下载等同27份所有行的语义审计。完整主干、辅助窗口与同C07已读且同blob的复用账见本轮检查点。主线新增读取不补写作者原始读账；作者完整读取合同指定header的情况不能由缺少ledger单独证明未读，但必须追加准确的实际范围，不能倒填。

[VRF] 作者6978933773全文与元数据已读，created/updated相同。PR17顶层issue comments查询未传since、per_page50、page1–4，实际50/50/50/37共187条，精确正式marker仅该评论一处：
MYOS2-LEAD-002-CHAT-EXEC-15 / CORE-USER-HPET-ENABLE-CLOCKSOURCE-102 / FORMAL_DELIVERY。
这是全量顶层评论marker核对，不声称187条均完成历史语义审计。

本轮观察四direct refs：lead75338cd7d005、exec1a92f9839e53、timea039d9803ade、mastere8e60cd19540。PR16 Open/Ready，PR17原title/base/Open/Draft，均未合并；开放PR首100条实际只有16/17。发布前后另刷新，不以缓存base_sha替代master直接头。

## 2. R102-01：原始文本计数多一处

合同146§8与协议P6/P9-5要求全六件原始文本计数，包括说明、模板和字符串。实际：

| 文件/口径 | 次数 |
|---|---:|
| evidence.md的130条源码锚标签 | 130 |
| MANIFEST.md P9-5的标签示例文字 | 1 |
| 另四件 | 0 |
| 原始六件总计 | 131 |

MANIFEST却声明130/130/0及“均在evidence.md”，故P9-5失败。额外的一次是说明文字，不是第131条源码引文，不能给它编造source_ref或机械改成131/131/0。

103 D00追加真实原始131、实际源码引文130的口径差别，撤回“原始六件130/130/0通过”的总括。旧六件、原作者130/130/0、评论和字面命中记录全部冻结。103自己的self_check按最终严格六件原始文本重新全计，禁止在例子/说明中无意产生无锚标签。无需重录102全部130条。

## 3. R102-02：强制定义与直接消费支持未齐

判据已经在146§4–7，尤其§7第4/5项；不是主线追加新的研究范围。130超过120软上限已由作者解释为独立宏补齐，本次不因数量超过软限退回。

### 3.1 必须补齐的具体链

| 102现状 | 追加纠正必须提供的支持及消费者 |
|---|---|
| E092–E094只有freq分支、sec计算片段和clocks_calc_mult_shift调用，无被调用函数定义锚 | clocks_calc_mult_shift真实体内算式、精度/移位选择和结果发布；clocksource_max_adjustment及update中的调整门；sec归一到1及必要常量/类型。连接B-P04、C-R03、M01–M07、F01/F02及真实其它消费者 |
| E038使用HPET_MASK，E088只到CLOCKSOURCE_MASK→GENMASK_ULL | 独立HPET_MASK、GENMASK_ULL和实际展开所需末端定义，不以同名符号或外部Linux习惯补出0xffffffff；每个独立宏单独取锚 |
| E034是hpet_base结构字段，没有同名静态全局对象的定义锚 | 单独登记hpet_base全局对象，区分结构定义与对象定义；补所消费的hpet_channel.boot_cfg等缺少字段，保留地址/频率原证据 |
| E098只请求list_add，E119只到list_for_each_entry；已推导插入/排序和计数 | 必要的list_add直接实现及实际下探写链/迭代定义，或严格限定为请求；146强制的必要list原语不能全部留opaque。对象初态、有效链和重复登记合法性另设门 |
| E104只有clocksource_select调用；E095/E096未覆盖register尾部 | clocksource_select实际宏；register尾部返回0和注释选择语句。C-R05不能借另一函数__clocksource_select的尾部代证register |
| E109/E110覆盖mono部分和mult，未覆盖raw的clock/mask/cycle_last及两个shift | 补tk_setup_internals实际相关赋值，连接C消费矩阵与正文；.enable绑定、直接调用、注册列表与timekeeper绑定保持分层 |
| E040/E041只有外层hpet读写wrapper，E123是COUNTER偏移宏 | 若保留“32位低部访问”命题，应取得沿include定位的readl/writel及其直接raw包装定义、必要类型和选择条件；否则只说外层uint结果与请求，不能用偏移宏证访问宽度/volatile/屏障 |
| 其它实际用作数值前提的独立位/偏移宏与初始化宏未单列 | 定点补HPET_ID、Tn_INT_ROUTE_CNF_SHIFT及实际消费的HPET_TN_LEVEL/FSB/PERIODIC_CAP、CLOCK_SOURCE_IS_CONTINUOUS/VALID_FOR_HRES等位宏；fs_initcall到登记机制、LIST_HEAD_INIT/DECLARE_LIST_HDR_S等按确实宣称的定义层支持。不扩展全initcall运行证明 |

以上是最小消费链核对表，不要求重新制作130条原证据。可以复用已经合格、内容相同且source_ref明确的102锚；欠缺的定义必须本人重新取得。真实声明选择、编译结果、MMIO物理行为仍是不同门。readl/writel的通用定义已由主线定位于mykernel/include/linux/kernel/asm-generic/io.h；这不自动证明实际预处理选中了该版本。

必须补146指定读取账：clocksource.h/const/api、time_const.h、time_type_declaration.h及实际新增依赖，按真实完整或窗口范围记。不能将主线本轮补读表写成作者上轮已完成。

### 3.2 数值方向可以保留，但不能据此免掉强制锚

主线完整读clocks_calc_mult_shift/update/max_adjustment；按七组给定period、频率和作者shift独立代入闭式整数关系，未调用、翻译重演或运行仓库函数/作者模型。先核整数类型/位移可表示和除数非零，再比较相邻更高shift的界限、11%调整量及delta。七组均与作者相符：

| period(fs) | 完整Hz | u32登记Hz | mult / shift | periodic delta | 名义1ms计数 |
|---|---:|---:|---|---:|---:|
| 100000 | 10000000000 | 1410065408 | 3045934800 / 32 | 5540 | 10000000 |
| 200000 | 5000000000 | 705032704 | 3045934800 / 31 | 11081 | 5000000 |
| 232830 | 4294979169 | 11873 | 2759875347 / 15 | 658005558 | 4294979 |
| 232831 | 4294960722 | 4294960722 | 1000001531 / 32 | 1818 | 4294960 |
| 1000000 | 1000000000 | 1000000000 | 2147483648 / 31 | 7812 | 1000000 |
| 10000000 | 100000000 | 100000000 | 2684354560 / 28 | 78125 | 100000 |
| 100000000 | 10000000 | 10000000 | 3355443200 / 25 | 781250 | 10000 |

对这些指定输入，sec=max(1,floor(0xffffffff/freq32))；mask不大于UINT_MAX，600截断条件不成立。以给定shift核M=floor(((10^9×2^shift)+floor(freq32/2))/freq32)，检查更高shift或32上限和maxadj；再算floor(10^6×M/2^(shift+7))。这些是本主线有界算术交叉检查，不是作者缺失定义在原交付时已合格的证据，也不推广到任意period或设备值。

原B-P04/F02所指出的cycles→ns参数和periodic预期用途之间的量纲问题可以有条件消费；实际硬件周期、设备接受、时钟准确度和IRQ到达仍未证。不能把上述数字称为实测或完整C实现运行结果。

## 4. R102-03：DOT的调用者与回调关系标错

146§6明确要求对象/摘要节点不能充当DIRECT_CALL的调用函数。85条边中至少以下实际关系需要追加纠正表，旧图不编辑：

| 旧边 | 问题 | 应补的实际关系 |
|---|---|---|
| channels_decode→kcalloc，DIRECT_CALL | channels_decode是摘要节点 | 实际调用者为hpet_enable；摘要表达数据/条件，不冒充函数 |
| cfg_sanitize→hpet_readl / hpet_writel，DIRECT_CALL | cfg_sanitize是摘要节点 | 实际hpet_enable中的读写请求，条件/循环与对象数据另画 |
| legacy→periodic，DIRECT_CALL | legacy是分支摘要 | hpet_enable在LEGSUP条件下调用periodic |
| timekeeping_get_ns→tk_clock_read，DIRECT_CALL | tk_clock_read位于另一个调用的实参中，但这里确有直接表达式 | 此边可保留；应另列timekeeping_get_ns→timekeeping_cycles_to_ns，并把其参数求值和具体clock读取分开 |
| register_irq→irq_handler，CALLBACK_BINDING | register_irq没有绑定hwint_irq_handler | register_irq写描述符handler字段；该字段来自myos_HPET_init传入HPET_handler；hwint_irq_handler依据regs向量读取描述符并间接调用 |
| irq_handler→hpet_handler、tk_clock_read→jiffies_clock，CALLBACK_BINDING | 将间接调用/对象选择与字段绑定混用 | 字段赋值/对象初始化属于绑定；函数通过read/handler调用属于有前提的间接调用；可增CALLBACK_INVOKE类型并定义，不能说已发生真实IRQ |

R102-Q7/Q8给出实际绑定与分发位置。103 D00须对原DOT每条DIRECT_CALL/CALLBACK_BINDING做定点对账，登记source_ref、原from/to/type、处置、真实调用函数或字段和证据ID；上表不是要求重画整张102图或重审全部IRQ入口。

本文不把所有图边一概判错。例如late_time_init确实有函数体；hz宏至register的展开可以明确标注MACRO_EXPANSION。补正必须依据真实定义，不能凭节点名称批量替换边类型。

## 5. 保留主干与必须合读的三个限定

### R102-S01：资源状态、访问和返回只在明确条件下成立

A的早退、out_nohpet、无legacy仍已登记/分配、legacy正常返回等出口区分成立；所有cleanup、分配器、显示和MMIO调用均须合法且正常返回，清指针不等于真实解除映射。首次和重复前态分开，不认证真实重复启动/泄漏/双释放。

hpet_enable所读体中清CFG_LEGACY，periodic调用的hpet_start_counter只新增ENABLE；hpet_enable_legacy_int虽然有定义，当前所读直接路径未调用。ID_LEGSUP是能力测试，channel.mode=LEGACY是软件状态，均不能替代实际CFG_LEGACY置位或IRQ路由证明。103定点追加源码支持，不漫游完整硬件规范。

TSC的无符号差值回绕不是自动C未定义行为；有限进展条件、读取能否返回和真实经过时间分别判定。200000不能单独保证墙钟上限。

### R102-S02：算式括号和位状态

facts mapping_channel_ownership_matrix的A-M03 source_trace把原表达式括号写丢，显示为右移后紧接加1，不能当作原C表达式消费。E049及模型M08支持的完整关系是((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1；结果1..32可保留，追加纠正source_trace并映射消费者，旧行不改。

B-P09的“活动TN_32BIT位未设置”须限定为此函数的OR未显式新增该位；cfg来自初读，若入值为1可能保留，不能推出最终为0。源注释中的另一条OR不当作执行语句。32位结果类型、访问请求宽度、硬件完整64位计数和单调性继续分层，按R102-02补直接定义。

### R102-S03：旧根身份和选择/消费边界

HC102-F06是IR101-F08/U05关于hpet_enable返回未消费的后续出口分析，不算新的独立根因。保留8个登记条目的历史数量，但追加标为旧根跟进；其余7个也只是本包局部候选，不能未经跨历史查重就宣称7个全新已证缺陷。CA-05/V14、FM96-F03/U08和65既有限定继续使用原身份。

clocksource列表、curr_clocksource与timekeeper缓存对象三者分开；函数中注释的select/notify/赋值不代表全树不存在其它路径。weak默认函数的文本与真实ELF选择分开。timekeeping初始化早于HPET登记的源码顺序不保证真实运行到达，也不证明之后从未重绑定。

## 6. P9可读子集和范围

| 项 | 结果 |
|---|---|
| 1 | 六件40hex命中0 |
| 2 | 130条字面、定义单元和行范围成立；部分消费支持欠缺，R102-02 |
| 3 | 锚点路径和D00真实旧件可读；作者实际读取过程不由主线替其认证，补准确读账 |
| 4 | 130条辅助行号/索引匹配 |
| 5 | FAIL：原始131与作者130/130/0不符 |
| 6 | 无未取版本手册的硬件认证；外部结论未做 |
| 7 | time/master options_flags与panic独立回源，两组不同 |
| 8 | master公约§1第2条整句、read_channel和必备字段核对 |
| 9/10 | 无源码改动/补丁，N/A |
| 11 | 无编译、运行、硬件或实际并发认证 |
| 12 | 非降级交付，N/A |
| 13 | FAIL：clocks_calc_mult_shift等强制定义及必要直接链未齐 |

完整本地P9仍NOT_RUN；纯字符串、JSON、DOT端点检查和有界整数等式不是运行仓库验证器。主线同属GPT对话复核，不能冒称异族独立评审或后台模型认证。

## 7. 主线独立限定短锚

本节8条和作者130条分账，原始全文计数8。Q1在完整配置读取之后记录，所有有限否定均限所读快照和指定函数，不扩展为全树证明。

### R102-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

配置前置，只取一条set命令内部。 固定time短SHA a039d9803ade；辅助行号58–61，不代替符号定位。

### R102-Q2

[VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	tmp = ((u64)maxsec * from) >> 32;
	while (tmp) {
		tmp >>=1;
		sftacc--;
	}
```

计算可用精度的循环，完整函数已读。 固定time短SHA a039d9803ade；辅助行号57–61，不代替符号定位。

### R102-Q3

[VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	for (sft = 32; sft > 0; sft--) {
		tmp = (u64) to << sft;
		tmp += from / 2;
		do_div(tmp, from);
		if ((tmp >> sftacc) == 0)
```

定标算式与候选移位检查，不能只用调用点代替。 固定time短SHA a039d9803ade；辅助行号67–71，不代替符号定位。

### R102-Q4

[VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	*mult = tmp;
	*shift = sft;
```

结果确实写入调用者给定mult/shift。 固定time短SHA a039d9803ade；辅助行号74–75，不代替符号定位。

### R102-Q5

[VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_max_adjustment]
```c
	ret = (u64)cs->mult * 11;
	do_div(ret,100);
	return (u32)ret;
```

11%整数调整量的真实算式；不是外部硬件规律。 固定time短SHA a039d9803ade；辅助行号133–135，不代替符号定位。

### R102-Q6

[VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	// clocksource_select();
	// clocksource_select_watchdog(false);
	// __clocksource_suspend_select(cs);
	// mutex_unlock(&clocksource_mutex);
	return 0;
```

此函数尾部select仍为注释，正常走到末尾返回0。 固定time短SHA a039d9803ade；辅助行号424–428，不代替符号定位。

### R102-Q7

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::register_irq]
```c
	p->parameter = parameter;
	p->flags = 0;
	p->handler = handler;
```

字段绑定的是传入handler，不能画成注册hwint_irq_handler。 固定time短SHA a039d9803ade；辅助行号185–187，不代替符号定位。

### R102-Q8

[VERIFIED mykernel/arch/x86_64/myos/interrupt.c::hwint_irq_handler]
```c
	if(irq_d->handler != NULL)
		irq_d->handler(irq_d->parameter, regs);
	if(irq_d->controller != NULL && irq_d->controller->ack != NULL)
		irq_d->controller->ack(irq_nr);
```

分发由描述符间接调用handler，随后按当前controller字段请求ack。 固定time短SHA a039d9803ade；辅助行号166–169，不代替符号定位。

## 8. 后继和写权

第103批按147：D00追加上述三项纠正与S01/S02/S03，再做timekeeper初始化/换算/读取接口三个关联专题。继续现C07a/CHAT-EXEC-15，次一档；无需新对话、无需召回旧C06e。C07主线仍保留唯一lead/PR16及导航写权，执行者只写新103严格六件和本人正式评论。

R102-01/02/03在103交回后由C07独立核收，当前均OPEN、102仍RETURN。Owner转发固定正式入口并fresh G00通过后103才启动。103作者不自签102关闭、不启动104、不改旧成果、旧评论、源码、PR元数据或主线文件。
