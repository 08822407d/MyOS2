---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-COMPLETION-OWNERSHIP-53-REVIEW-001
record_type: independent_combined_static_review_with_documentation_return
evidence_class: 固定源码、逐对象消费与文档锚点分开裁定
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近确认）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-07
base_snapshot: agent/MYOS2-LEAD-002
read_channel: GitHub_connector; Python_literal_snippets_and_partial_orders
source_pin_short12: a039d9803ade
contract81_short12: f044f78846d7
reviewed_base_short12: ae6ce5353844
reviewed_head_short12: caf179f242ec
reviewed_delivery_comment: 6703139904
lead_before_publication_short12: f044f78846d7
acceptance_verdict: RETURN
return_kind: DOCUMENTATION_ONLY
returned_scope: [R53-01]
mandatory_supplements: [R53-S01, R53-S02]
R52_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
core_A_B_C: RETAINED_WITH_LIMITS
original_artifacts_modified: false
original79_full_automatic_quote_scan: NOT_RUN
original_JSON_DOT_mechanical_replay: NOT_RUN
reviewer_partial_order_models: 4
reviewer_literal_snippet_checks: 3
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段正确替代引文}
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
phase: 2
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
next_followup: CORE-USER-REQUEST-HANDOFF-54
next_task_ref: 82-user-request-handoff-contract.md
status: CORE_RETAINED_DOCUMENTATION_RETURN_OPEN
inputs_read: [81号与52回执及63/79既有固定正文, 53六件, 评论6703139904, 本回执方法节所列原始源码]
owner_decisions_requested_now: []
open_questions: []
---

# 第53批核心分析保留；三个锚点问题合为一个文档纠正项

## 1. 身份、差异与独立决定

完整读取53六件和正式评论，按81号及R52/R51/R50/R49C限定独立回源。裁定RETURN，仅R53-01文档纠正；A/B/C核心分析依本回执限定保留，不要求重做三个专题。
R52-01已经由另一份回执独立关闭；不能因为53新证据退回而重新打开52，也不能用52闭合替53签通过。
compare ae6ce5353844→caf179f242ec为ahead1/behind0、一个提交，仅六件A新增1618行：MANIFEST75、正文40、facts436、evidence819、DOT89、deferred159。旧件零净改删，执行者声明STOP_WRITING。
PR16/17均Open/unmerged，PR17 Draft；主线f044f78846d7，执行caf179f242ec，direct time=a039d9803ade、master=e8e60cd19540。不改执行成果、旧评论或内核。

## 2. R53-01：连续原文与定义归属（唯一退回项）

### 已独立确认的三处

- CO53-A03：原引文删掉devtmpfs_create_node中const char *tmp = NULL;与kzalloc之间的源码空行。原四行不是连续原文；quote_range=109–112也未覆盖实际113行的kzalloc。正确连续范围可为109–113，共五行。
- CO53-B03：原引文删掉DECLARE_COMPLETION_ONSTACK(done);与task_s *task;之间的源码空行。原四行不是连续原文；正确连续范围可为242–246，共五行，而非242–245。
- CO53-C03：摘录17–21的前两行是struct completion定义之前的文档注释；真实结构定义为19–22。字段本身存在且支持核心命题，但结构体外的注释不能被算作该定义内证据。

这些不是核心函数不存在，也不作作者动机判断；但P2要求逐字、连续、定义内，P6的79项全部合格不能原样采信。原文件、元数据和79/79声明保留历史，不能把去空行匹配当逐字通过。

### 正确替代示例（须由后继独立确认）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_create_node]
```c
int devtmpfs_create_node(device_s *dev)
{
	const char *tmp = NULL;

	req_s *req = kzalloc(sizeof(req_s), GFP_KERNEL);
```

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	DECLARE_COMPLETION_ONSTACK(done);

	task_s *task;
	kthd_create_info_s *create =
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);
```

[VERIFIED mykernel/kactive/completion/completion_types.h::struct completion]
```c
	struct completion {
		uint			done;
		swqueue_hdr_s	wait;
	};
```

### 关闭条件

在54新六件的专属勘误区追加上述三锚点的旧值、替代值、真实定义范围和消费者。保留空行/制表符/反斜杠；超五行应重新选择范围或拆证，不能删中间行。
至少映射A.heap_origin/B.origin/C.count_distinction及正文、facts对象表、DOT相应对象节点、MANIFEST原P6和交付评论。新核三项与54新证分别统计，其余76条不追认全量重验。
补充原79/79是历史作者自查、至少这三项未满足P2，不能继续当全部通过。不要改写原53或旧评论；只申请R53-01_DOCUMENTATION_CLOSURE_REQUESTED，关闭权归C04。
同包消费R53-S02的图分支和规范状态说明即可，不要求重画53全图、不增加第七件、不另派纯勘误批。

## 3. A/B/C核心分析及R53-S01

A：固定devtmpfs源码支持create分配req后未判空即写成员、name空时本函数局部无req释放、delete用自动对象而name/uid/gid未赋值、submit发布同一对象及worker写err→complete→kfree的顺序。create与delete不能合并为同一种存储来源。
CO53-F01/F02/F03保留为有条件源码候选，沿WW52-U04等身份补证，不升格为已观测UAF、非法free或泄漏。F02必须停在最早不确定字段求值，且handle_remove无活动return的值被上层消费；不能跨过这些边界认证实际执行到kfree(stack)。F03中的分配失败与name早退是不同子条件，不能算一次运行同时发生。
F01原模型选取wait能正常返回后的结果读；实际未证范围还包括worker在提交者进入wait前就释放含done的req，或在消费者仍使用内嵌done时释放。任何这类条件路径都应停于最早失去对象寿命前提之处，不把req.err当唯一潜在首次访问点。completion计数/锁本身不是req的所有权引用。

B：成功kthread先读取threadfn/data和full_name、交换done并写create.result，之后complete；所读该函数在通知后不再解引用create，但还会调用schedule_preempt_disabled等。创建方wait正常返回后先读result再kfree(create)。因此“生产函数未完全返回”本身不足以推出create释放错误；逐对象最后访问才是此局部结论的依据。
失败producer的pid<0、done有/无及名字释放须分开；所选creator的killable取消逻辑是注释，不凭注释认证NULL-done可达。to_kthread得到有效self、kernel_thread及交换宏正常工作的前提仍需保留；本轮没有重新认证完整fork或任务退出。
full_name是从kvasprintf取得的另一个指针，成功赋给self不是深拷贝；本轮未展开kvasprintf内部或证明free_kthread_struct实际可达。该函数中存在后续释放调用，不等于已证明整个name寿命安全。
xchg路由和汇编宏正文已回读；只在所选宽度/实现确实完成交换的条件下用“取得旧done、置NULL”推演，不以宏名称或注释认证实际机器原子性和内存序。

C：getcode_done为文件作用域对象、具有静态存储期，不等于声明中有static关键字或内部链接。keyboard缓冲count、completion.done、swait表头count三者独立；所读init_keyboard未见显式重置completion，只是该函数范围结论。先积累done的快路与真正入队后重用必须分开，CA-04不自动扩成每次键盘读取必败。
IDE的node、Params和调用栈done来源得到支持；正常wait返回后释放Params/node，结束函数complete之后只操作全局req_in_using与锁，所读该函数不再解引用node。尚不能据此排除别的IRQ/daemon持有旧指针。node为NULL时end_request只打印而未早退，后续解引用仍有有效性前提；调用方后置if(node)也不能保护此前node->DevSpecParams访问。

## 4. R53-S02：图、状态、数值及证据消费

原DOT是摘要图，不是完整CFG。Bcomplete→Bcontinue(schedule)只属于成功新kthread分支；create_kthread的失败通知之后没有沿此边调用schedule。其返回必须与失败producer匹配。Bsuccess→Btransfer/Bresult也须保留done非NULL门；分配/命名早退、NULL-done退出不能从缺边推为不存在。
Adelete→Auninit是后续worker实参求值的停止点提示，不是delete入口立即读取那些成员。Acreate→Asubmit只在分配、name及先行操作满足条件时成立。
complete进入、计数更新/锁释放、complete函数返回、等待者返回是不同事件；跨上下文虚线不提供完整全序。完成方释放completion锁之后，等待者可能运行，而完成函数的调用返回仍未发生。对原三偏序中complete事件应说明抽象阶段，不能把各线程的整个函数调用一律当不可拆原子事件。
原A模型4个线性序中的3/1、B的4、C的6只是给定边约束的计数，不是现实概率、频率或已经发生的交错。
CA-03/CA-04 carried字段的SUPPORTED_IN_SCOPE_HISTORICAL只作来源说明，规范状态仍SUPPORTED_IN_SCOPE；UV-01仍deferred_owner_not_ready。后继分列状态和来源，不修改历史原件或凭本轮关闭缺陷。
CO53-B23片段本身只展示初始化返回变量和按宽度分派；交换指令语义还需完整__xchg_op各分支，不能只凭这个短片段及B24返回__ret证明已经交换。

## 5. 方法和未执行项

完整读取53六件及评论。81/52回执、63/79、00/28和协议P1–P13使用本对话已有固定全文，不宣称新对话接收或全面刷新Mnemosyne。
本轮新取：devtmpfs90–153及190–377；kthread1–363；slub130–203；cmpxchg_api1–83与atomic_macro510–526；keyboard1–180；ide267–406；completion_types/macro全文；kernel_init268–312。其余req/IDE类型、此前IDE直接函数、completion/swait和双金丝雀采用同pin已有原始源码交叉核对，不将执行者摘录替代独立源。
定点重新读取原evidence A03/B03，核实删空行；以连接器原文片段运行三次字面/范围检查。不是全79项自动扫描，不声称完成全树符号/预处理/链接门。
独立重建原三偏序得到4、4、6个序列，并增加“complete锁释放不等于函数返回”的细化模型，共4个模型。细化模型49个抽象线性序保留free在wait入口、done访问、解锁或err读取之前的不同停止边界；不把失效后的符号事件当定义良好的C执行。
原JSON/DOT机械重放、图哈希、Graphviz、完整本地P9、仓库C/ASM/脚本、旧宿主夹具、编译/QEMU/硬件/真实并发均NOT_RUN。角色独立复核不构成人类或已认证异构审查。
后继82号继续三个专题组合：设备注册的结果消费、IDE活动请求交接、NVMe/XHCI局部差异。为划界预读core.c1–310，不预签新批结论，不扩完整驱动/分配器/调度器。
固定成果：https://github.com/08822407d/MyOS2/tree/caf179f242ec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-completion-ownership-53
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6703139904
