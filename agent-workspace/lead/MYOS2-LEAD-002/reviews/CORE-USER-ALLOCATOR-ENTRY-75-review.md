---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ALLOCATOR-ENTRY-75-REVIEW-001
record_type: independent_bounded_static_acceptance
evidence_class: 六件全文、净差异、强制锚与代表定义核对；非运行证明
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；未直接读取客户端或认证后台）
model_per_owner: gpt pro（Owner此前指定）
date: 2026-11-12
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract107_short12: 1ba6acf132a0
reviewed_base_short12: 6f792023d592
reviewed_head_short12: 0681d7186406
reviewed_delivery_comment: 6813648992
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
subjects: {D00: ACCEPTED_DOCUMENTATION_ONLY, A: ACCEPTED_WITH_LIMITS, B: ACCEPTED_WITH_LIMITS, C: ACCEPTED_WITH_LIMITS}
prior_R74_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
mandatory_supplements: [R74C-S01, R75-S01, R75-S02, R75-S03]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original87_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
original_DOT_hash_parser_Graphviz: NOT_RUN
original39_model_reexecution: NOT_RUN
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三条直接短引文，人工核对}
next_followup: CORE-USER-SLAB-OBJECT-LIFECYCLE-76
next_task_ref: ../108-user-slab-object-lifecycle-contract.md
executor: MYOS2-A-C05c / MYOS2-LEAD-002-CHAT-EXEC-09
executor_rotation: false
status: BOUNDED_STATIC_ACCEPTANCE_WITH_SUPPLEMENTS
phase: 2
owner_decisions_requested_now: []
inputs_read: [第75批固定六件及评论, 107合同和74回执, 本文列明的固定源码与历史记录]
open_questions: []
---

# 第75批独立验收：PASS_PENDING_LOCAL，无新增返工

D00纠正另行闭合R74-01；A/B/C按R75-S01/S02/S03限定接收。下述补充由主线直接提供，不要求执行者再交75勘误包。它们不是内核修复、运行通过或全SLUB正确性认证。

## 1. 交付身份与可读核查

Owner本次只转达STOP_WRITING，主线自行从PR17取得唯一正式评论6813648992及真实提交0681d7186406，没有要求Owner补搬材料。直接执行ref和PR17头一致；PR16仍为1ba6acf132a0。两PR开放未合并，17为Draft。STOP_WRITING作为执行者正式交付声明消费，不声称监控了另一聊天的后台进程。

compare 6f792023d592→0681d7186406：ahead1/behind0、一提交、严格六件added共3127行，旧文件零净改删。全文读到末尾：MANIFEST126、正文94、facts1748、DOT149、deferred215、evidence795行；长件分段补齐。D5+A31+B26+C25=87个标签的可读计数与清单87/87/0相符，其他五件所读文本未见额外保留前缀或完整40位串；此为人工可读核对，非原始六件机械扫描或JSON解析。

定义和逐字代表核对共54/87，覆盖本合同强制锚：D01–D05；A01/02/03/04/05/06/08/09/13/15/16/17/19/21/23/26/27；B01/02/03/04/05/06/07/08/09/10/11/13/15/17/18；C01/02/03/04/05/06/07/09/10/11/12/13/14/15/16/17/24。固定源本轮新读与同一有效C05已直接取得的相同Git对象复用分列于§5，不用执行者的已读声明替代来源。其余33段已读交付文本，未在本轮逐段重新作定义身份认证；原87自动重验仍NOT_RUN。

结构化金丝雀、read_channel和公约自检原句存在。公约§1(2)本轮直接比对；time/master双金丝雀沿上一轮已回读的相同固定对象，直接refs仍分别为a039d9803ade、e8e60cd19540。代码/补丁相关P9项N/A；完整本地闸门、最终预处理和运行项保留待本地，不签PASS。

## 2. 可保留的技术主干

A：普通kmalloc按KMALLOC_MAX_CACHE_SIZE分流；固定宏模型为PAGE_SIZE=4096。零请求在kmalloc_slab早退，1–192由size_to_index分档，193–4096用fls(size-1)。8192表槽与创建循环存在，不证明普通kmalloc会选中该槽。直接超界__kmalloc的循环是旧MC-09的一个接口断点，不扩展为所有大尺寸kmalloc均不返回。task的sizeof保持符号值，栈32768投影进入大块入口。

B：kzalloc添加ZERO；有效小块在slab_alloc取到对象后经hook按object_size请求memset，大块本体在page非空且ZERO时按原size请求memset。两者的请求长度、取页容量和实际内存内容不能合并；未添加ZERO不等于实测字节非零。cache/页/对象有效和所有先行callee正常返回均是前提。

C：kfree对NULL有早退，非空经folio分类分向cache或large释放；标志和名称不构成分配来源或最后持有证明。free_large_kmalloc的order0分支没有源码return，只有告警之后的条件性继续。直接cache free、正常退出和task的重复put不借用彼此的保护条件。旧MC-07及TSL74-U04物理释放部分未关闭。

## 3. R75-S01：数字模型、类型与配置边界

D00五个独立宏支持所选C分支的32768模型，不能取代最终宏选择。get_order不仅有原A21所引的非恒量路径，还存在编译器常量判断分支：

[VERIFIED mykernel/include/linux/kernel/asm-generic/getorder.h::get_order]
```c
			if (__builtin_constant_p(size)) {
				if (!size)
					return BITS_PER_LONG - PAGE_SHIFT;
```

原28个尺寸例及39例集合可以作为其声明的有限整数模型消费，不称为实际执行了BSR、ilog2或某个内联分支。当前普通large调用前提为size>4096，所以不能把这些非零样本推广为get_order(0)或fls(0)的硬件认证；bitops注释中的厂商说法也未作外部规范认证。

小块限定区间内size到uint/unsigned int的值可表示；任意极大size、移位上界、分配阶限制和完整ABI不在所验范围。cache slot非NULL、cache创建成功和slab_state=UP是不同断言。65→96及193→256是选定表与合法cache初始化下的长度模型，不保证任意运行时cache元数据从未变化。

## 4. R75-S02/S03：最早停止点、循环及图

### R75-S02：直接cache释放的参数求值顺序

[VERIFIED mykernel/mm/kmalloc/slub.c::kmem_cache_free]
```c
	slab_free(s, virt_to_slab(x), x);
```

virt_to_slab(x)作为实参表达式须在进入slab_free主体之前完成；s的值传递不等于已经求值s->node。原正文及free_provenance/earliest_stop矩阵中“坏cache先在s->node停止”只适用于x的转换已经合法返回的子情形。若x首先不能合法转换，则不能认证已进入slab_free，更不能继续归还计数或释放页。不为其他实参之间虚构统一的从左到右求值顺序。

即便转换得到NULL slab，也不能借kfree(NULL)早退解释直接kmem_cache_free。folio_slab只是经folio_to_slab的cast；分类flag、cache指针、对象起点、存储来源和最后持有仍分别需要证据。task的后续stack=NULL只属于相应调用者在callee正常返回后的写入，不是任意kfree都会清空其调用者字段。

### R75-S03：partial未命中不等于整个申请失败

[VERIFIED mykernel/mm/kmalloc/slub.c::slab_alloc]
```c
	while ((object = get_from_partial_list(s, n)) == NULL) {
		slab_s *slab = alloc_init_new_slab(s, gfpflags);
		if (unlikely(!slab))
			return NULL;
```

get_from_partial_list返回NULL会进入新slab创建，不直接使slab_alloc返回NULL。只有实际下层失败经相应门传播，才可说明hook未到达；新slab非NULL后还需上线并再次从partial取对象，不等于直接把slab头交给用户。正文“任何一层返回NULL”按facts.zeroing_failure_matrix中alloc_slab→alloc_init_new_slab→slab_alloc的具体失败链收窄，不包括partial普通未命中。该补充不要求新返工。

图A_PARTIAL→A_INITNEW为slab_alloc调用者控制下的条件顺序，不是get_from_partial_list内部直接调用；共享A_ALLOCPAGES节点不能把cache阶与large阶参数混用。C_NULL/C_KFREE→C_NULLSTORE仅适用于release_task_stack调用者；C_MC07是历史条件提示，不是执行函数。65节点/77边及39模型是作者申报的资料统计，本轮未自动重算原图或运行原模型。ANALYSIS_STOP不当CPU控制流。

历史MC-07/08/09状态保持static_concern；RF-02是旧断言受反证的记录，不等于整个ctor机制安全。MR-02须按05主线回执完整语义CLOSED_BY_EXPLICIT_NARROWING_NOT_GLOBAL_PROOF理解，75中的短状态名不构成全局证明。八项AE75-Uxx仍needs_evidence，无新独立根因，不因本次接收自动关闭物理/并发缺口。

## 5. 来源与后继

本轮新读固定time：uapi/linux/const.h全文；getorder.h全文；bitops_arch.h175–260；slub_types.h全文；slub.c1–末尾；buddy.h1–165；slub_macro.h全文；page-flags.h235–285；buddy_const.h1–50及215–325。同有效C05此前直接读到的相同固定对象继续用于page_const_arch.h、slub.h、slab_common.c、slub_const.h、fork.c相关调用、CONFIG_BUG与双金丝雀，不伪称本轮又全读这些文件。

历史新增定点读取：04 facts1940–2075及2820–3060的MC-07/08/09与A-KM证据，1700段可见部分只作定位不追认旧绝对句；05 deferred800–末尾和05主线回执§1–3。00工作令本轮分段刷新至末尾、18全文及公约§1另读；107及74回执、协议v2、28/63/79沿本有效上下文完整固定文本复用，不导入跨项目任务或重放启动仪式。

下一76检查SLAB槽/对象初始化、partial/full与freelist状态转换、归还及整块下线门。必须以生产者和状态组合增量深化MC-07/08，不以重复找到OR或首槽ctor事实充当新根因；不重做全部04、75或buddy。只新增本主线文档并维护PR导航，执行成果仍由09在Owner转发后写原PR17新目录。

同族局限：这是C05对C05c产出的独立会话审阅，仍不构成异族独立复核。原87全量机械扫描、完整P9、JSON解析、原图/模型重放、所有内核构建运行均NOT_RUN；本轮没有执行本地有限模型或仓库脚本。
