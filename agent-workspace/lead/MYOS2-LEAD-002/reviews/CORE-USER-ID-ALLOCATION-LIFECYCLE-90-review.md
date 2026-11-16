---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ID-ALLOCATION-LIFECYCLE-90-REVIEW-001
record_type: independent_bounded_static_review_with_targeted_documentation_return
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收安排）
date: 2026-11-16
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract127_short12: 07ed0fed054c
executor_amendment128_short12: ffe6d8b314c8
reviewed_base_short12: 42897a06feb1
reviewed_head_short12: 034cd9c52dbf
reviewed_delivery_comment: 6896688417
executor: MYOS2-A-C06c / MYOS2-LEAD-002-CHAT-EXEC-12
status: RETURN
acceptance_verdict: RETURN
returned_items: [R90-01, R90-02]
return_scope: TARGETED_MANIFEST_AND_EVIDENCE_DOCUMENTATION_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_WITH_R90_S01_S02_S03
R89_01_and_02: CLOSED_BY_SEPARATE_SAME_ROUND_RECEIPT
batch89_composite: PASS_PENDING_LOCAL
mandatory_supplements: [R90-S01, R90-S02, R90-S03, R89-S01, R89-S02, R89-S03]
original_artifacts_modified: false
author_narrative_count: 59/59/0
original_canonical_self_check_fields: MISSING
reviewer_readable_raw_tag_count: 59
self_check: {verified_claims: 5, quotes_reconfirmed: 5, downgraded_to_inferred: 0, scope: 本件五条新短锚的可读固定源码核对，不是原59项机械认证}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-KTHREAD-NAME-STORAGE-91
next_contract: ../129-user-kthread-name-storage-contract.md
phase: 2
inputs_read: [90六件和正式评论全文, 127与128及89回执的同一C06固定对象, 本轮项目规则和固定源，详见同轮检查点]
owner_decisions_requested_now: []
open_questions: []
---

# 第90批独立审核：两个文档补正项，分析主干限定保留

89的两项纠正另件关闭，组合PASS_PENDING_LOCAL。90为RETURN，仅R90-01清单自检字段及R90-02跨宏证据。不是要求重做IDA/IDR三专题，更不是内核修复授权。局部次序、输入前提及导航问题由R90-S01/S02/S03规定有效读法，下一包合读。

## 1. 交付与审查范围

正式评论6896688417创建/更新时间为2026-11-15T15:14:59Z。初核PR17和直接exec均034cd9c52dbf；lead ffe6d8b314c8、master e8e60cd19540、time a039d9803ade。PR16/17 Open/unmerged，17仍Draft；PR缓存base_sha不替代master直接引用。

compare 42897a06feb1→034cd9c52dbf：ahead1/behind0、单提交，只新增六件3307行：MANIFEST112、正文209、facts1694、DOT114、deferred464、evidence714。旧件零净改删，作者遵守新12身份与STOP_WRITING；未自签89闭合或90通过。

六件本轮全部独立读取至末尾，截断区以重叠区段补齐。evidence有E001–E059，其他五件可读未见额外原始证据标签。十矩阵51行、30条件模型、八候选十缺口是不同计数域，不等于运行通过或十八个独立根因。固定idr.c/h/const/types、cpumask、runqueue、err、pid相关完整函数、worker调用方和配置面已独立回读；强制主体及多数旁证有真实源支持。没有将作者摘录当唯一原文。

P9-2/5/13有已知不合格项，故不能直接PASS_PENDING_LOCAL。P9-1/3/4/7/8按可读范围及本有效C06同pin金丝雀/公约核对；原六件正则、完整解析和全符号字节检查没有运行。6没有新增外部标准认证；9/10/12无代码件、补丁或降级交付，相应N/A；11保留NOT_RUN。没有认证全部语言语义、全局锁、运行可达性或二进制正确性。

## 2. R90-01：必需自检字段不能由构造器统计代替

协议v2 P6及127要求MANIFEST.self_check显式包含verified_claims、quotes_reconfirmed、downgraded_to_inferred三个整数。原MANIFEST只有evidence_anchor_tag_prefix_count_expected、evidence_anchor_literals_constructed_from_independently_fetched_fixed_source、evidence_anchor_constructor_mismatch_count等自定义键，没有上述规范键。

正文的59/59/0以及交付评论的计数，不能补足P6“闸门只认MANIFEST”的要求；构造时字符串能找到、交付前重新回源、真实符号定义内合格是三种不同检查。E013进一步说明数量吻合不等于59条全部满足P2。

91/D00只追加原90清单补充，不改原件：列明缺失键、旧59/59/0实际声明的含义、原六件逐件原始标签复核数及重新核对方法/范围；撤回把构造计数等同完整合格的读法。对原90的新核结果按本次实际证据记录，不倒造发布时已经拥有规范字段或已经运行机械检查。原E013撤回/替代的状态必须单列，不能把冻结文件说成已删除该标签。

91自身MANIFEST必须直接给出三个规范字段，按91六件全部原始前缀计数，包含D00、说明、字符串，不用expected替代actual。原90历史59、新追加替代锚、91全包三组分别计账；补记通过由主线裁定，不自行签90闭合。此项不要求第七件或重做90全部模型。

## 3. R90-02：E013跨三个独立容量宏

IDA90-E013标IDA_BITMAP_LONGS，但引用前一个历史注释以及IDA_CHUNK_SIZE、IDA_BITMAP_LONGS、IDA_BITMAP_BITS三个独立宏。字符存在不满足单一定义边界。应分别使用：

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_CHUNK_SIZE]
```c
	#define IDA_CHUNK_SIZE			MYOS_IDR_BUF_SIZE	/* 128 bytes per chunk */
```

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_LONGS]
```c
	#define IDA_BITMAP_LONGS		(IDA_CHUNK_SIZE / sizeof(long))
```

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_BITS]
```c
	#define IDA_BITMAP_BITS 		(IDA_BITMAP_LONGS * sizeof(long) * 8)
```

宏中保留的“128 bytes”是历史注释，不能推翻活动替换式；MYOS_IDR_BUF_SIZE的32768另有E012，不要拼成复合锚。

91/D00须追加三个定义内短锚及旧新消费者映射，至少覆盖facts.source_ledger、fact_to_evidence_ids.ida_units、单位/扫描/释放矩阵、M04/M07/M08/M12等实际依赖、IDA90-F04/U01/U02及正文和DOT容量节点。回查其他同义消费，不以列出的示例当全部清单。保留原59/59/0历史，撤回全部59条边界合格的总括，新包重新计数。不因宏拆开而另造源码问题或重做整数模型。

## 4. R90-S01：映射副作用、模型和图示次序

facts.C-LRR03的“返回旧A后写B”顺序颠倒。真实次序是先取旧值、写入新值、最后返回旧值；M24及DOT相应节点已表达正确，不把一处文字误序扩成全包失效。

[VERIFIED mykernel/lib/idr/idr.c::idr_replace]
```c
	entry = idr->idr_rt[id];
	idr->idr_rt[id] = ptr;

	return entry;
```

idr_remove同样先写NULL再返回旧值。C-CON03把slot7/8均空与“若7忙可扫8”写在同一行，后半只能作为另一个输入变体：slot7已占且slot8空、调用方和编号均有效。不能在原全空输入下声称越到8。PID显式tid还有1<=tid<pid_max与child_reaper等上层门；局部反例不能跳过这些门。

for_each没有活动回调，next_ul没有活动搜索/输出推进，这些结论成立于有效入参；二者仍读取idr_base，next_ul还读取nextid，不能因算法注释而声称NULL对象安全。get_next负输入的转换及WARN结果保留目标类型前提，不证明真实平台诊断或全局行为。

DOT的黑色容量/存储箭头按构造依赖提示消费，不当作调用边或已满足统一数据边方向的证明；返回ID箭头是返回信息。iter→nextul注明独立API，只是比较关系，不可作为可执行路径遍历。scan→result的“tested bit0”应读作被测试位的值为0，不是只测位号0。局部源序与跨线程可观察顺序仍不同。

## 5. R90-S02：数值单位、接口承诺与条件范围

IDA循环实际把min当word下标，内层上界为sizeof(long)：

[VERIFIED mykernel/lib/idr/idr.c::ida_alloc_range]
```c
	for (uint slot_idx = min; slot_idx < IDA_BITMAP_LONGS; slot_idx++)
	{
		ulong slot = ida->bitmap[slot_idx];
		if (slot != ~1ULL)
		for (uint bit_idx = 0; bit_idx < sizeof(long); bit_idx++)
```

LP64例采用long=8字节及相应ulong/ULL宽度；4096word、低8位、32768候选整数与262144个物理位须分开。M06/M07等从word0开始的例子补明min=0；M08的32760是该具体首空位结果，32767只是给定模型的扫描编号上端，不是同次返回两个值。无效指针、索引或转换前停止正常推导；不由条件模型签实际故障。

IDA90-F01中min=5返回40本身不等于违反“返回值至少为5”，也不能仅凭word5空证明ID5可用。有效主张是坐标错用导致跳过候选搜索；更强反例为有效全空位图、min=4096且仍有合法请求编号时，循环直接返回-1。若声明具体ID5空，还须相应word0位5前提。与F04都涉及单位但检验的接口不变量不同，不保证八候选彼此独立。

F02/F05的上界不消费、F03特殊word跳过、F06base未作用、F07/F08迭代空实现按完整活动正文成立，返回负值不擅改为注释中的errno。IDR游标移动不等于排他占号，NULL仍为空槽；失败重试受id>start和实际callee返回控制。大无符号到int的未证转换不外推为可移植固定结果。

## 6. R90-S03：历史、索引及证据资格

WQP89-F01/F02/F03/F05和PID11-F01/F02沿用原身份，新增只作证据追加。deferred.prior_qualified_id_links里IDA90-A-SCAN01等矩阵引用，实际facts的行ID为A-SCAN01；同样去掉装饰性的IDA90-前缀定位B-PUB01、C-CON03等行。E与M编号仍保留IDA90前缀，不能一概删前缀。这是导航修正，不补造缺失的事实键。

旧11回执的历史RETURN不能越过后续处置重新激活；90-U08中的旧状态只作历史来源。R88-01/R86-01等闭合不变。read_channel的GitHub_connector准确表达实际通道，下一包按协议规范值connector填写，不因枚举文字另造技术退回。

接收/九规则/110/124/指导刷新记录是12自己的申报与仓库可读记录；主线确认没有继承11署名，但不认证另一聊天的后台步骤或模型。30模型均为纯文本条件推导；主线未运行作者构造器、原六件正则/JSON/DOT解析、仓库程序或内核。NOT_RUN并非发现源码候选必须停工的理由。

91选名称格式化计数与截断、kvasprintf两遍分配、线程名称存储读取三个关联增量；已对照53及68避免重复线程创建/完成交接。该设计是预读后的任务范围，不是91已执行。91先D00处理R90-01/02，再G01–G04；仍由12次一档执行，交本C06下一轮Pro合并验收，不为文档小项增加单独Pro回合。
