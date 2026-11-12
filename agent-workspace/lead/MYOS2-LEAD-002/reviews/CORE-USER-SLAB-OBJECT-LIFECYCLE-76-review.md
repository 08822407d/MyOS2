---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SLAB-OBJECT-LIFECYCLE-76-REVIEW-001
record_type: independent_bounded_static_acceptance_before_mainline_handoff
evidence_class: 六件全文、远端净差异及固定源码可读核对；非运行证明
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；未直接读取客户端或认证后台）
model_per_owner: gpt pro（沿用Owner此前指定）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract108_short12: bfb6578ca8c2
reviewed_base_short12: 0681d7186406
reviewed_head_short12: 59733f24b6d3
reviewed_delivery_comment: 6818039586
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
subjects: {A: ACCEPTED_WITH_SUPPLEMENTS, B: ACCEPTED_WITH_SUPPLEMENTS, C: ACCEPTED_WITH_SUPPLEMENTS}
D00: NOT_APPLICABLE
mandatory_supplements: [R76-S01, R76-S02, R76-S03, R75-S01, R75-S02, R75-S03]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original107_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
original_DOT_hash_parser_Graphviz: NOT_RUN
original40_model_reexecution: NOT_RUN
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四条直接短锚，可读核对而非机械扫描}
next_action: MAINLINE_C05_TO_C06_READ_ONLY_HANDOFF
batch77: NOT_DESIGNED_NOT_PUBLISHED_NOT_STARTED_ON_HOLD
status: BOUNDED_STATIC_ACCEPTANCE_WITH_SUPPLEMENTS
phase: 2
owner_decisions_requested_now: []
inputs_read: [76固定六件与正式评论, 108合同与75回执, 本文列明的源码和规则]
open_questions: []
---

# 第76批独立验收：限定接收，无新增返工

第76批按R76-S01/S02/S03组合消费为PASS_PENDING_LOCAL。三个技术专题保留，无D00和新增退回项。两处图示纠正及语义限定由本回执直接提供，不要求09另交76勘误包，也不修改旧六件或旧评论。当前Owner明确要求验收后交接主线，故本轮不按28号通常节奏设计77；第77批未设计、未发布、未启动。

## 1. 固定交付和实际审查范围

主线自行从PR17取得唯一正式交付6818039586，执行者C05c/CHAT-EXEC-09，提交59733f24b6d3与直接执行ref和PR17一致。compare 0681d7186406→59733f24b6d3为ahead1/behind0、一提交、六件added共2696行，旧件零净修改删除。PR16/17开放未合并，17仍Draft；正式评论中的STOP_WRITING是执行方声明，不是对另一聊天物理后台状态的监测。

完整读取MANIFEST125、正文84、facts1096、evidence982、DOT147、deferred262行，长响应缺段已补齐。可读计数A47+B33+C27=107，与清单107/107/0一致；其余五件所读全文未见额外保留标签前缀或完整40位提交串。此为可读审查，不冒称自动全文扫描、完整JSON解析或重跑作者程序。

代表逐字/定义核对为76/107：A01–A11、A14–A23、A27–A42；B01–B08、B17–B30；C01–C15、C25/C26，覆盖尺寸/字段/槽、抽取迁移、归还门及链表强制结论。其余31段已读交付原文，未逐段在本轮重新认证。使用本轮直接回源与本有效C05此前实际读到的同一固定Git对象，不借执行者的已读声明替代证据。完整本地P9、107全量机械定义检查仍NOT_RUN。

结构化branch_canary_quotes、read_channel和startup_selfcheck_quote存在；与本有效C05已读公约及time/master原行相符。直接time仍a039d9803ade、master仍e8e60cd19540。原40模型及60节点81边属于执行方资料与申报，主线核了关键条件和图语义，未自动重算图、运行Graphviz或重放模型。

## 2. 保留的技术增量

A区分cache.inuse字节布局、slab.inuse对象计数、oo的16位字段及slab.objects的15位字段；实际尺寸生产者、两次对齐、槽步长及ctor/freepointer次序可读。首槽未收到该循环ctor、后继槽先ctor后可能被freepointer覆盖属于旧MC-08的深化，不是新根因或所有消费者必坏。

B区分slab上线与object交付；抽取先读取freelist并调用get_freepointer，之后递增inuse，调用者的NULL判断在后。满值后partial→full；新slab返回后仍须上线并重新取object。列表count、nr_slabs、total_objects分别有生产者，不能互相替代。

C区分归还前满值迁移、归还后的inuse与partial计数、锁外OR条件和再次加锁下线。尺寸96/objects42/min_partial4，以及尺寸4096/objects4/min_partial6的跨不同满slab单线程归还模型，支持旧MC-07的条件触发门而不需要多CPU并发。只证明源级请求到free_slab，未证明物理页已回收、复用或真实UAF。

## 3. R76-S01：生产状态、数值模型和最早停止

size96模型只有在其明确的pointer-width、align16、正size、可表示运算及所选get_order/ilog2语义下，才给出order0、42对象和阈值4；size4096模型同样有前提。它们不是已编译cache布局。n=1/n=2槽例是对局部循环的假设算例，不因默认slub_min_objects=4而自动取得合法生产路径证明；必须与完整生产不变量分开。

oo.obj_nr可容纳的数不保证可写入15位slab.objects；简单非零返回门不等于证明所有截断、阶上限和容量均正确。零size、异常align或运算条件不成立时停止该模型，不任意指定实际最早机器异常，也不把所有数值回绕/不变量破坏一概称作C语言未定义行为。

offset=0仅在已声明初始化来源及写入范围内使用。set_freepointer覆盖的是其实际写入范围，不证明整个ctor状态被毁；无ctor不等于槽内容全零，post-hook是否清零仍按75的标志/长度限制。safe迭代宏和指针检验名称都不证明容器、存储来源或生命周期安全。

## 4. R76-S02：归还不是事务，阈值是判断时点

[VERIFIED mykernel/mm/kmalloc/slub.c::free_single_to_partial]
```c
	slab->inuse--;
	set_freepointer(s, object, slab->freelist);
	slab->freelist = object;
```

若set_freepointer不能合法完成或不返回，不能声称最后的freelist头赋值已发生；此前的inuse减量却位于调用之前。分析停止不是整段自动回滚。抽取路径相反：get_freepointer返回前不能认定后面的inuse++已经发生。

[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```

contains命中还须list_del_init内的验证与链接更新合法返回，才可使用后续减量/返回值。add的count++也在实际链接callee之后；下层验证可能不返回，不能将函数名或外层调用当成已完成迁移。offline不检查删除结果后的统计请求仍按SL76-U08保留，未证明非成员状态来自合法前序。

MC-07四满slab模型的partial.count=4是OR判断时点，不是整个free调用结束后的必然状态。若offline成功摘下本slab，则链表count又减为3；六满slab模型同理6→5。该变化不取消判断时目标inuse仍正的事实。原历史“顺序释放不会触发”不得解释成任意单线程顺序免疫；逐个slab清空与跨slab各归还一个是不同序列。没有把锁间窗口、原子接口调用或屏障名字提升为真实并发/互斥证明。

## 5. R76-S03：两处图示的明确替代消费

原DOT A_ORDER_OBJECTS→A_OO标CALL，但真实调用方向相反：oo_make调用order_objects取得obj_nr；order_objects的返回数据才流回oo_make。

[VERIFIED mykernel/mm/kmalloc/slub.c::oo_make]
```c
		.order		= order,
		.obj_nr		= order_objects(order, size),
```

后继把该CALL边按A_OO→A_ORDER_OBJECTS解释；若保留原箭头，只可标返回数据，不能作为真实调用。图中C_ENTRY标签写成kmem_cache_free(s,virt_to_slab(x),x)，不是源码存在的三参数kmem_cache_free调用。真实入口为kmem_cache_free(s,x)，其主体调用：

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]
```c
	slab_free(s, virt_to_slab(x), x);
```

C_ENTRY应按两参数外层入口理解，C_ENTRY→C_FREE表示实参转换正常结束后的slab_free调用；R75-S02继续约束最早停止点。两项是图标签/边的局部纠正，证据A11/C15本身真实，正文并未因此取得错误的内核API。原DOT不修改，本回执明确替代，returned_scope仍为空。

其余CALLER_SEQUENCE边均需对应调用者、分支和callee返回；ANALYSIS_STOP不是内核goto。free_slab的页头while、folio有效性、order/移位条件及后续callee均可能限制到达__free_pages，不能将末端接口存在写成真实回收闭环。

## 6. 来源、历史与交接

本轮新回源：double_list.h130–310及620–末尾，double_list_macro.h250–末尾，double_list_types.h全文，align.h全文，log2.h1–165，minmax.h1–135。slub.c全文、slub_types.h、slab_common.c、slub_const.h所需宏、getorder.h、const.h、page_const_arch.h和slub.h使用本有效C05此前直接读到且pin/blob一致的原文；不伪称本轮另全读所有源码。

108及75回执/74闭合、04的MC-07/08/09与05限定沿已取得固定证据消费；63/79/24/28本轮完整刷新，00/18/公约/协议v2复用已读原文。没有以Mnemosyne指导导入任何外项目维护。

SL76-U01–U09保持needs_evidence；MC-07/08/09、DR-04、RF-02/05、AE75和MR-02保持原qualified_id及限制。73、74文档闭合及75接收不重开，58/63误判撤回继续有效。该接收是同族模型独立会话审阅，不等于异族或本地最终验收。

本轮不出77合同。后继唯一动作是109号C05→C06只读接收；C06不得重复验收76，不得在写权CONFIRMED和Owner切换Pro之前设计77。没有内核修改、构建运行、原件覆盖、合并或阶段3授权。
