---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-WAIT-WAKE-CONSISTENCY-52-REVIEW-001
record_type: independent_combined_static_review_with_documentation_return
evidence_class: 固定源码回核、原始交付及明确方法边界
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近确认）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-07
base_snapshot: agent/MYOS2-LEAD-002
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract80_short12: c7f6cad5316f
reviewed_base_short12: 1e128a33a014
reviewed_head_short12: ae6ce5353844
reviewed_delivery_comment: 6701215032
lead_before_publication_short12: c7f6cad5316f
acceptance_verdict: RETURN
return_kind: DOCUMENTATION_ONLY
returned_scope: [R52-01]
mandatory_supplements: [R52-S01, R52-S02]
subject_A: CORE_SOURCE_SUPPORTED_WITH_LIMITS
subject_B: CORE_SOURCE_SUPPORTED_WITH_LIMITS
subject_C: NAMED_CALLS_AND_CONTRACTS_SUPPORTED_SCAN_NOT_INDEPENDENTLY_REPLAYED
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original67_automatic_quote_scan: NOT_RUN
original205_C_scan_replay: NOT_RUN
original_JSON_DOT_mechanical_replay: NOT_RUN
reviewer_independent_models: 15
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段单函数短引文, method: 固定源码文本人工核对}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
next_followup: CORE-USER-COMPLETION-OWNERSHIP-53
next_task_ref: 81-user-completion-ownership-contract.md
phase: 2
status: CORE_ANALYSIS_RETAINED_DOCUMENTATION_RETURN_OPEN
owner_decisions_requested_now: []
open_questions: []
---

# 第52批三个专题核心结论保留；仅交付记录定点退回

## 1. 裁定与现场

依据80号、79号合包安排、R51/R50/R49C限定、协议P1–P13及28/63号，完整读取六件和交付评论，并独立回核关键源码。
裁定RETURN，仅R52-01文档问题；不是A/B/C技术分析重做，不要求重新扫描205个C文件，不要求修内核。核心结论依R52-S01/S02保留，原六件整体暂不签PASS_PENDING_LOCAL。
执行提交ae6ce5353844、评论6701215032真实存在，执行者已声明STOP_WRITING。compare 1e128a33a014→ae6ce5353844为ahead1/behind0、一提交、六件A新增1826行：MANIFEST78、正文41、facts792、evidence687、DOT107、deferred121；旧件零净改删。
PR16/17均Open/unmerged，PR17 Draft；实际主线c7f6cad5316f、time a039d9803ade、master e8e60cd19540。开放列表第三项为空。PR16本轮读取为mergeable=true；不据此发起合并或解释此前标志变化。
沿用C04主线唯一写权和C04c执行身份，不导入其它项目维护。另一聊天的完整内部执行轨迹及真实后台模型不由提交记录认证。

## 2. 三专题独立判断

### A：队列计数与摘链

已读固定swait、普通链表类型/辅助体及宏，支持：counted入队加表头count，而swake使用普通摘链；finish见节点已自指时不会补减。
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
	curr = list_headr_first_container(&q->task_list_hdr, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
```
在同一有效队列、初始计数一致、相关调用正常返回且无其它写者时，一待项唤醒后可形成count=1而链空；再次按count取首会失去真实waiter容器前提。止于该边界，不继续声称实际CPU故障。
未唤醒待项的counted删除和已被wake摘除待项的finish是不同路径；普通semaphore的List_s没有这个表头count字段，不能混用。
沿用CA-04的既有有界身份；旧宿主观察不是本轮内核运行或新发现。

### B：登记和取消

独立核timer.c登记/锁基/摘除、timer.h、hash_list及必要选项。MOD_TIMER_NOTPENDING=0x04，MOD_TIMER_PENDING_ONLY=0x01；本调用排除早期pending优化及PENDING_ONLY早退门。
idx保持UINT_MAX时，正常登记路径走internal_add_timer，再调用enqueue_timer；不能把调用完成解释为timer已经到期或回调已执行。
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (idx != UINT_MAX && clk == base->clk)
		enqueue_timer(base, timer, idx, bucket_expiry);
	else
		internal_add_timer(base, timer);
```
取消的running_timer门、负返回重试、shutdown=false及int结果被调用者丢弃得到支持。有限timeout的返回问题仍属CA-03，未重复编号；完整轮盘/真实时基/锁运行不在裁定内。

### C：调用身份与局部衔接

本轮直接回读devtmpfs、kthread、init、keyboard、IDE、NVMe、XHCI所列调用范围，确认命名对象和直接调用点；completion的公开void、内部MAX action与semaphore的授予/int错误协议不能互换。
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
				req->err = handle(req->name, req->mode,
						req->uid, req->gid, req->dev);
				complete(&req->done);
				kfree(req);
				req = next;
```
submitter在wait后读req->err，而worker在complete后发出kfree请求；它们的相对顺序和对象来源需后继审查，不能由通知本身签安全生命周期或实际UAF。
205/205个.c、632个.h以及读取失败0是执行者记录的扫描覆盖声明。本轮未重放该扫描，也未得到逐205文件的独立复核；C的接收范围是已核命名调用点和有界接口矩阵，不给“全仓没有外部调用者”背书。函数指针、宏、头文件、构建选择和真实可达性仍未全核。

## 3. R52-01：正式交付记录补齐（唯一退回项）

### 确认的问题

1. MANIFEST有model_per_owner、执行身份和金丝雀原文，但缺少协议P13要求的produced_by字段，也没有P3/P9-7规定的branch_canary_quotes字段。不是没有金丝雀文字、不是证明读错分支；其它正确字段不能替代两个必备字段。合包规则79未取消这些要求。
2. 评论主动披露C14/C16的definition_range过宽。独立回源确认kthread定义164–215、kernel_init定义270–309；引文203–205及275–277确在各自函数内。这不同于49的extern/错函数锚点，不判伪造，但需把已披露修正与文件消费关系正式保存。
3. deferred把UV-01的既有deferred_owner_not_ready缩成deferred_not_ready，与“不改变旧状态”的申报不一致。只视为待纠正的状态拼写，不批准任何历史状态迁移。

### 关闭条件

在53规定的新六件中追加专属补充，不改52原件/旧评论：
- 53 MANIFEST自身具备全部要求；另设prior_batch52_manifest_supplement，明确只补52的produced_by、branch_canary_quotes和适用边界，supersedes_fields指向52 MANIFEST。署名只能按实际可知信息填写；无法读取界面时用unknown并解释，不拿Owner选模冒充后台认证。两条金丝雀须从固定源重核，保留真实tab/反斜杠。
- 正文/evidence的52勘误区登记C14/C16原范围、正确范围、原引文仍在定义体内及消费者；旧范围与原评论保留历史。不要求重审无关65条，也不将局部勘误追认为67/67全重验。
- 明确UV-01有效沿用值为deferred_owner_not_ready，52缩写不产生状态变更。CA-03/04及其它DS/CW/ST/SM/WW条目原状态不自行改变。
- 给MANIFEST、正文、facts、evidence、deferred和交付评论建立必要的修正映射；新核/继承/未重核分别登记。只申请R52-01_DOCUMENTATION_CLOSURE_REQUESTED，关闭权归C04。

一项文档补齐随53的新取证合并交付，不另建纠正批次、分支、PR或第七件，不要求重做三个专题。

## 4. R52-S01：A/B技术消费限定

prepare即使不再次入队，也会先重写wait->task；模型必须保持同一合法waiter/current前提，不能把“没有第二次append”解释为完全无写入。
counted添加实际先改链再count++；删除先调用下层再改count。前序helper失效或不返回时，不跨过它认定最终不变量。
错误队列参数应区分合法但不含此节点与无效头/锁：前者的contains可返回false，finish仍先请求TASK_RUNNING；不宣称所有错误参数都发生同一后果。
计数回绕、损坏链、任意并发不在少量正常对象模型中。swake_all仍是独立调用者，不是swake内部主动调用all。

detach_timer(clear_pending=false)摘链但不清pprev，故局部过渡态的timer_pending不能无条件等同“当前已在某桶链内”；正常重新入队会重写链字段。true分支的清pprev也仅在前序有效删除正常完成后讨论。
__mod_timer的ret=0可表示原先未pending，也可来自function为NULL的早退，不等于成功/失败的通用布尔值。
取消非负返回至多支持所读锁内running/pending门的局部状态；不证明其它CPU永不重登记、回调一定执行过或未来不会执行。shutdown=true是generic模型，本调用固定false。
基础设施失效、锁基不返回、回调对象无效分别保留停止点；不从源码注释引入进展保证。

## 5. R52-S02：C、图与证据来源

C14/C16只纠正元数据包围范围，原短引文未越真实函数体。C02的long类型需合读真实wrapper签名或已闭合C49-A02，不由return片段单独推出类型。
C中的设备局部done、启动静态done和devtmpfs req.done不是同一对象；一个通知者或同名字段不证明其它请求的结果、完成次数或释放责任。
“worker complete后free、consumer wait后read”没有给出free与read的唯一跨上下文全序；正常调用/对象存活是模型前提，不是已证明结果。

原DOT是摘要关系图，不是全CFG：a0→a1→a2不得解释成count先于插链；a3→a11只能作调用者关系提示，真实方向是all的循环调用swake。缺少的空队列返回、已自指finish跳过和function=NULL早退必须合读源码/正文；不能当这些路径不存在。
b0的有限范围包含0，原边“finite positive”仅是子集。c6→b0的dotted STOP是禁止把公开MAX套有限路径，不是可执行调用边。C类别选择边不是实际单线程分支；跨上下文虚线不是同步返回。
作者47节点/54边、36模型、67引文计数保留为作者检查记录。本轮做图语义和全部证据账本人工阅读，不宣称重放原图机械检查/JSON解析/全量逐字扫描。

## 6. 方法、效率与后继

本轮完整读52六件及评论；80/79、R51/R50/R49C、00/28沿用本对话已读固定正文，另全文重读63及协议v2。不是重新接收或全面刷新Mnemosyne。
新增直接回源：timer.c115–340，timer.h全文，hash_list.h85–210，timer_const相关范围，double_list_types全文及macro300–尾；devtmpfs1–180/340–尾，kthread135–320，init75–135/268–320，keyboard100–190，IDE130–295，NVMe120–235，XHCI290–400；配置全文与panic1–100。
A的swait/double_list已有同pin原始源码、B取消体及C的completion/semaphore/limits采用本对话先前原始读取交叉核对，不把执行者账本当独立源码，也不冒称所有源码本轮重新下载。
实际运行15个自建纯Python模型：8个计数/队列序列、1个选项位门、4个取消条件、1个重试序列、1个通知后读/释放偏序。不是作者36例重放，不是仓库源码执行或真实并发。
全67引文自动扫描、205C扫描重放、JSON/DOT机械重放、Graphviz、全树符号/编译选择、完整本地P9、内核/仓库脚本/旧宿主夹具/预处理编译/QEMU/硬件均NOT_RUN。可选raw下载因临时环境DNS失败，无文件取得，不计GitHub写入失败。
角色独立复核不等于人类或经认证的异构模型审查。合包减少交接，不降低必备字段与证据标准，也不为凑工作量重扫已完成的大目录。
81号CORE-USER-COMPLETION-OWNERSHIP-53沿用C04c：先补本项记录，再一次完成devtmpfs请求所有权、kthread创建握手、静态/一次性completion复用与释放边界三个相关专题；不扩整个驱动、调度器或分配器。
固定成果：https://github.com/08822407d/MyOS2/tree/ae6ce5353844/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-wait-wake-consistency-52
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6701215032
