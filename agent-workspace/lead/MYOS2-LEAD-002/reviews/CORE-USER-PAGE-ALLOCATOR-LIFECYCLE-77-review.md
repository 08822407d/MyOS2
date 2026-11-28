---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-REVIEW-001
record_type: independent_bounded_static_review_with_documentation_return
evidence_class: 六件全文、远端净差异、定义与语义可读核对；非运行证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner在本C06明确告知）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract111_short12: 89f84109b470
reviewed_base_short12: 59733f24b6d3
reviewed_head_short12: 5a8c5dc1cd38
reviewed_delivery_comment: 6821135838
acceptance_verdict: RETURN
returned_scope: [R77-01]
returned_scope_kind: documentation_selfcheck_only
accepted_partial_scope: A/B/C技术主干按本回执限定保留；不是整包通过
mandatory_supplements: [R77-S01, R77-S02, R77-S03, R75-S01, R75-S02, R75-S03, R76-S01, R76-S02, R76-S03]
original_artifacts_modified: false
original_definition_anchors_readable: 121
original_raw_reserved_prefixes_readable: 122
original_manifest_declared_prefixes: 121
original_nonsource_prefixes_readable: 1
original_full_automatic_prefix_or_definition_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
original_DOT_parser_Graphviz: NOT_RUN
original36_model_reexecution: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
self_check: {verified_claims: 5, quotes_reconfirmed: 5, downgraded_to_inferred: 0, scope: 本回执五条直接短锚，可读回源，非原包自动检查}
next_followup: CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78
next_task_ref: ../112-user-page-bootstrap-handoff-contract.md
D00_next: R77-01追加文档纠正，与三专题同包
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第77批：技术主干保留；清单计数一项文档退回

独立验收已经完成，整包裁定RETURN，仅R77-01待追加纠正；不是内核退回，不要求重做页分配分析。A/B/C按R77-S01/S02/S03继续限定使用。按28/79及110号，同轮发布第78批：先处理短小D00，再连续进行三个初始化接合专题，下一次由本C06独立核收并决定能否闭合R77-01。

本轮较早进度消息曾说可限定接收、无须另交返工包；最终清单核对发现下述硬性计数不符后，该初步判断已撤回并显式告知Owner。最终以本RETURN回执为准，不用进度消息代替裁定。

## 1. 正式交付及实际核查范围

主线自行读取PR17唯一正式交付6821135838，固定执行头5a8c5dc1cd38与直接ref、PR17一致。独立compare 59733f24b6d3→5a8c5dc1cd38为ahead1/behind0、一提交，严格六件added，共3774行，旧件零净修改删除。PR16/17开放未合并，17保持Draft。STOP_WRITING来自Owner与执行者交付声明，不冒称监控另一聊天后台。

六件完整读取到末尾：MANIFEST127行、正文70、facts1886、DOT156、deferred318、evidence1217；长响应分段并补齐。121条定义标签按A40+B38+C43逐项可读计数；另有清单说明中的一次原始保留前缀，不能混成第122条源码证据。完整原包机械前缀扫描、JSON解析、原36模型重放与Graphviz均未执行。

定义与短引文对照覆盖本合同强制结论及所读121条证据文本，使用本轮新回源和本有效C06此前直接取得的同一固定Git对象；没有借执行者的121/121自报代替源文。完整本地P9、最终预处理选择、ABI和物理效果不在本次证明内。source ledger的22份、图71节点79边、36模型是交付统计；本轮已读其内容，但不冒称自动重算或执行原检查器。

结构化startup_selfcheck_quote、branch_canary_quotes和read_channel已见；与本有效C06已直接读到的公约原句、固定time/master双金丝雀相符。直接time仍a039d9803ade、master仍e8e60cd19540；PR缓存base_sha不作master现状。

## 2. R77-01：原始前缀总数与清单申报不符

定位：固定5a8c5dc1cd38的user-page-allocator-lifecycle-77/MANIFEST.md，标题“P9-1至P9-13实账”下第5项。该句在说明要统计的前缀时，自己又写入了一次连续字符“U+005B左方括号紧接VERIFIED”。这里用字符名称描述，避免本回执再次引入非证据保留前缀。

全文可读账为：evidence.md的121条定义标签＋MANIFEST说明文字1次＝122次原始前缀；其余四件未见该前缀。清单却声明六件原始前缀总数121。协议v2 P6/P9-5及111号§6明确要求六件原始全文计数，不是只数成功解析出的源码标签，因此不能把说明文字中的命中悄悄过滤后判通过。这不是空白行、引文排版或历史58/63指控，不能以已撤回的旧误判为由忽略当前实在存在的额外字符。

本轮没有运行原六件机械扫描；121个有编号定义标签与这1个说明文字命中已直接可读确认，足以否定121的全包总括。121条真实定义短锚仍分别评价，额外命中不等于发现了虚构内核代码，也不能将历史self_check改写成原来就正确。

下一78/D00须完整回读原六件，记录逐文件原始前缀数、真实定义标签数、额外命中位置及实际方法；纠正原清单P9-5和正式评论中相关总括的消费方式。原件和旧评论冻结，仅在78六件内追加prior_return_disposition并在新交付评论明确替代。不得简单把verified_claims改成122而伪造第122条源码锚，也不要求重新做121条技术分析。D00交付只报待C06闭合，执行者不得自关R77-01。

## 3. R77-S01：页层与对象层hook的标志必须分开

原facts的PA77-M20中post_alloc_hook_receives_original_flags=true不能按buddy.c的同名页层hook消费。页层收到的是prepare_alloc_flags的结果，包括过滤后的位、cache.allocflags以及后续NOWARN/NORETRY等处理；不是未经处理的原始slab请求。

[VERIFIED mykernel/mm/kmalloc/slub.c::alloc_slab]
```c
*alloc_slab(kmem_cache_s *s, gfp_t flags) {
	gfp_t alloc_gfp = prepare_alloc_flags(s, flags);
	folio_s *folio = (folio_s *)alloc_pages(alloc_gfp, oo_order(s->oo));
```

替代读法：page_post_alloc_hook_argument=filtered_alloc_gfp；slab_post_alloc_hook_argument=original_gfpflags。在M20声明的原ZERO请求及所选allocflags生产者条件下，页层ZERO位为false；对象层另在成功取得对象之后接收原参数。不能把不同层的同名片段当同一callee，也不要求两个数值在所有输入上必然不同。

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	slab_post_alloc_hook(s, gfpflags, &object);
```

其余ZERO区间模型作为“请求范围”保留：order1并集[0,4097)，order3并集[0,4103)，不等于残余字节实测非零。large局部memset按原size请求，与页层和对象层分别记账；未知后续cache writer不能被排除。该局部字段更正由本回执直接提供，不另开源码返工。

## 4. R77-S02：图的失败出口和分裂/释放顺序不能合并

原DOT把order超界与__rmqueue_smallest未命中都接到A_FAIL，并让A_FAIL继续A_PREF。必须拆开解释：前者是整个__myos_alloc_pages的直接NULL返回，后者才是调用者尝试下一合法偏好索引；越过三元素数组后的结果仍在首次非法求值前停止，不认证安全NULL退出。

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_alloc_pages]
```c
	if (unlikely(order > MAX_ORDER)) {
		return NULL;
	}
```

共享C_BUDDYFLAG→C_ADD的顺序只适用于释放尾部，不可套到expand。expand对每个分裂余块先请求入链，再写阶与Buddy标志；释放尾部则相反。原图保留，后继按下列上下文分开消费，不拼出不存在的统一执行顺序。

[VERIFIED mykernel/mm/page_alloc/buddy.c::expand]
```c
		add_to_free_list(p, zone, high);
		set_buddy_order(p, high);
```

[VERIFIED mykernel/mm/page_alloc/buddy.c::__myos_free_one_page]
```c
	set_buddy_order(page, order);
	add_to_free_list(page, zone, order);
```

数据节点或锁区节点的CALL边只能表示相应真实调用者的操作，不能发明一个以free_area或锁为函数名的callee。所有后续边依先行调用正常返回；部分更新不是事务回滚。free_pages_prepare返回类型为bool而没有活动return，调用者却不使用返回值：不能仅据该签名增加返回false、拒绝释放或自动ANALYSIS_STOP门。myos_page_zone未找到区而其返回值随后被使用，是另一项独立的前提缺口。

## 5. R77-S03：补充多级模型、元数据位置与新候选限界

原36模型明确给出了单级合并，未给一个完整多级计数实例。主线直接补充如下源级条件例，不倒填为执行者原36例中的已运行项：有效同区描述符数组内，归还尚未入链的PFN16/order0；原伙伴头PFN17/order0、PFN18/order1、PFN20/order2分别在相应空闲链，且各次类型/阶/zone检查与链接操作合法返回。依实际循环依次合成[16,18)、[16,20)、[16,24)，order变为3；下一候选PFN24地址合法但不满足伙伴条件，从而停止合并。

若初始area0/1/2各count=1、area3=0，在没有其它写者且这些旧伙伴确未被摘链的声明模型中，旧三阶count仍各1，最终area3因挂入PFN16增加为1。该例只补MC-01的多级源级成员/计数前提；不证明正常启动能产生所设类型值、不证明真实重叠分配或物理UAF。所有差值在本例为零或正数，不能绕过PA77-F01的较低伙伴地址问题后继续同一推演。

folio的_flags_1属于第二个union、与__page_1叠放；“head元数据”应理解为逻辑folio的字段，不能写成全部位于第一页的flags。赋值(flags_1 & ~0xffUL) | order并不额外屏蔽任意过大的order输入；所选order≤10模型与任意配置上限分别评价，最终编译布局未证。

PA77-F01保持needs_evidence：较低PFN的无符号差值与随后指针表达式必须区分；无符号算术回绕本身不是所有语言错误的统称，后置伙伴检查也不能为先前地址形成补合法性。64位ulong、描述符数组边界、编译实现及运行可达性仍是条件，不裁定机器必崩溃。PA77-F02的0→1→1→2只在同页被重新选中、无外部引用修改、原语实现满足所用算术语义且全链正常返回时成立；不能凭直接free入口名认定最后持有者或实测引用漂移。

八项PA77-U01–U08继续needs_evidence；MC-01–05、MC-07、RF-01、LB-01/LB-08、GAP-M08、AE75-U07、SL76-U09沿原qualified_id及限定。MR-02仍为CLOSED_BY_EXPLICIT_NARROWING_NOT_GLOBAL_PROOF；75/76与73/74历史闭合不重开。

## 6. 来源与后继

本轮新读slub.c145–300、355–435，slab_common.c245–310；buddy_types.h1–280；page-flags.h120–165及365–520；page-flags_const.h80–末尾；mmzone_types.h1–210；buddy_const.h全文；highmem_macro.h、asm-generic/page.h、double_list_types.h、uapi/linux/const.h全文；mm_init.c206–末尾；double_list.h620–末尾；page_const_arch.h1–65；page-flags_macro.h230–末尾。

本有效C06上一轮已直接取得且pin/blob一致的buddy.c、buddy.h、mempolicy.c、mmzone_const.h全文、mm_init.c1–205、slub.c1–145及双金丝雀用于本次可读核对，不冒称本轮重新全读所有源码，也不继承执行者的读取资格。78设计预读另在检查点列明，不作为77新执行成果。

本回执五条短锚均与已取得的固定定义逐字复核；原六件121标签的读取、定义比对和计数是可读核查，不是机械扫描。没有运行预处理器、内核、原检查程序、原模型或完整本地P9。这是同族模型的独立会话验收，不是异族独立复核。

下一112号含R77-01/D00和三个安全初始化专题。C06不代写执行结果；09只在Owner转发新合同且G00通过后取得78限定写权。78尚未启动、交付或验收；110号后续交接必读约定持续有效。
