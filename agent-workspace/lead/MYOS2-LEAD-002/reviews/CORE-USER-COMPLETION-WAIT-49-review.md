---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-COMPLETION-WAIT-49-REVIEW-001
record_type: independent_bounded_static_review_with_documentation_return
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: ChatGPT本主线独立复核
model_per_owner: gpt pro（沿用Owner报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-06
read_channel: GitHub连接器；临时环境仅纯文本及自建有限模型
base_snapshot: agent/MYOS2-LEAD-002
source_pin_short12: a039d9803ade
contract75_short12: a1beca5a67a1
reviewed_pr: 17
reviewed_base_short12: d23aa99d2c3a
reviewed_head_short12: ea079bca1985
reviewed_delivery_comment: 6681100222
lead_before_publication_short12: a1beca5a67a1
acceptance_verdict: RETURN
returned_scope: [R49-01]
return_kind: DOCUMENTATION_ONLY
core_analysis_disposition: RETAIN_WITH_R49_S01_S02_NOT_WHOLE_PACKAGE_PASS
mandatory_supplements: [R49-S01, R49-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
original36_automatic_quote_scan: NOT_RUN
original_facts_deferred_JSON_parse: NOT_RUN
reviewer_DOT_identity: full_connector_text_reconstruction_matches_complete_remote_Git_blob
reviewer_DOT_blob_short12: 31f22d5d943b
reviewer_DOT_bytes: 4277
reviewer_graph_structure: {explicit_nodes: 38, unique_nodes: 38, edges: 45, undefined_endpoints: 0}
reviewer_Graphviz: NOT_RUN
reviewer_independent_finite_models: 32
self_check: {verified_claims: 4, quotes_reconfirmed: 4, downgraded_to_inferred: 0, scope: 本回执四段单函数体引文, method: 固定源码连接器文本人工核对}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
executor_display_name: MYOS2-A-C04c 代码分析执行
executor_rotation: false
next_followup: CORE-USER-SCHEDULE-TIMEOUT-50
next_task_ref: 77-user-schedule-timeout-contract.md
phase: 2
kernel_change_authorized: false
build_or_runtime_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
open_questions: []
---

# 第49批已真实交付；仅证据归属与支持关系定点退回

## 1. 裁定与交付确认

依据75号、R48-S01/S02及既有46C/47限定，独立读取49六件、交付评论和固定源码后，裁定RETURN，仅R49-01文档问题。
核心完成计数、等待分支和公开信号状态结论可按R49-S01/S02保留，但原六件整体不能签为PASS_PENDING_LOCAL。
不要求重做整批分析，不要求修内核；第50批首项追加勘误，并与独立新取证合并交回。
DS48-F01/F02/F03、IP47-F01、UV-01及CW49-U01..U04保持原状态；本轮不因没有新缺陷而强行立项，也不作运行正确性裁定。

实际PR17 HEAD ea079bca1985，评论6681100222声明READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING和STOP_WRITING。
compare d23aa99d2c3a→ea079bca1985为ahead1/behind0、恰一提交，仅user-completion-wait-49/六件A新增1126行：MANIFEST152、正文74、facts383、DOT89、deferred79、evidence349。
旧件零净改删，无写区外路径。PR16/17 Open/unmerged，PR17 Draft；主线HEAD a1beca5a67a1。direct refs为time a039d9803ade、master e8e60cd19540，开放列表第三项为空。

Owner转来的最终回复同时保留接收阶段待办和后来完成声明，容易误读，但其中提交/评论标识实际已经存在。
远端MANIFEST分别记录接收、项目规则独立读取、Mnemosyne指导刷新，正式评论记录最后提交与回读；没有发现“只接收未交付”的证据。
该记录只确认已提交的申报及可核仓库状态，不认证另一聊天的内部执行轨迹或后台模型。
本轮以远端固定交付为正式证据；用户粘贴是导航和沟通反馈，不作为缺少仓库文件的降级交付，不另重录成长附件。

## 2. R49-01：两处强制锚点错配及跨件修正

状态OPEN，关闭权仅C04。判据是75号W02/W04的真实函数定义、单定义短引文与支持强度，以及00工作令§5的可读闸门。

### 2.1 已确认的具体问题

- E12标签为completion.h::wait_for_common，标题是“wait wrapper 锁和 long 返回”，实际摘录为该文件59–63行，属于do_wait_for_common中的重锁、循环和未完成返回；不是wait_for_common函数体，也不证明wrapper的schedule_timeout实参和返回路径。
- E13标签为do_wait_for_common，标题是“done 初始门”，实际摘录为16–20行的extern声明区，还跨到另一函数wait_for_common的声明。它没有函数体、没有if(!x->done)，不能被计为真实定义体证据。
- 两者被正文W02、facts中CW49-T05/T06/T07及wait_matrix的entry/gate/public-return等位置重复消费；MANIFEST与交付评论又申报36/36。文字在文件中出现不等于符号归属、证据类别和命题支持全部合格。
- E01的前置struct completion声明仅作上下文，typedef断言宜归一为单独typedef行；这不另立第二个退回项。

这不是否认实际函数存在或核心结论必然错误，也没有证据认定作者未作文本检查；退回不作动机判断。

### 2.2 正确源码的定点对照

以下均为固定time a039d9803ade，不替执行者完成追加勘误或自行关闭R49-01。

wait_for_common的真实long返回类型：
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
		long
		wait_for_common(completion_s*x, long timeout, int state) {
```

同一函数的锁、真实callee实参及返回：
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
			spin_lock_irq(&x->wait.lock);
			timeout = do_wait_for_common(x, schedule_timeout, timeout, state);
			spin_unlock_irq(&x->wait.lock);

			return timeout;
```

do_wait_for_common的实际初始门：
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
			if (!x->done) {
				DECLARE_SWAITQUEUE(wait);
```

公开state判断所需的实际helper门：
[VERIFIED mykernel/lock_IPC/signal/signal.h::signal_pending_state]
```c
		signal_pending_state(uint state, task_s *p) {
			if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
				return 0;
```

### 2.3 关闭条件

在第50批规定新目录追加专属勘误，不改49六件或旧评论：
1. 逐项处置E12/E13：旧摘录保留历史，注明实际归属或声明上下文；新增真实wrapper返回类型、锁/callee/返回和do_wait初始done门的单定义1–5行引文。
2. 函数定义核对必须识别函数体范围，不能用首次命中的extern或另一个函数内的同名调用替代；标题声称的关键语句必须落在引文内。
3. 给正文W02、facts CW49-T05/T06/T07、相关wait_matrix、图wrapper/lock/gate/fast/make/wunlock/rvoid及原自查声明提供明确旧位置→替代证据→消费限制矩阵；不要求重画整图或重审无关34项。
4. 同步将E01的前向声明与typedef分开说明；新核引文单列实际计数。原36标签及36/36仅保留历史申报，局部更正不追认其余标签已经全部重新通过。
5. 执行者只能申请R49-01_DOCUMENTATION_CLOSURE_REQUESTED，不能自行将原RETURN改成CLOSED/PASS。第50批新分析另申请独立验收。

## 3. R49-S01：核心结论与有界消费

complete的done增加受UINT_MAX门控制，swake_up_locked调用在门之后；动态/静态初始化、锁表达式、计数更新、唤醒请求、任务上CPU和函数返回分别记录。
wrapper在所读函数体中持锁调用do_wait、解锁再返回long；公开wait_for_completion返回void。E12/E13错配不使这段实际源码消失，但必须补正确证据才能关闭文档退回。

初始done非零时走快路，不创建局部wait、不调用action或__finish_swait；非饱和计数减一，return timeout ?:1。
初始done为零时进入do循环；generic helper即使传timeout=0，也不能由入口参数直接推断零次action，仍须依信号门和首轮控制。
负非零timeout在循环条件中为真；generic快路的负timeout也可能原值返回且消费计数。这些helper参数模型不证明公开入口或schedule_timeout实际生产负值。
TASK_UNINTERRUPTIBLE=0x2与0x1|0x100无交集，所选signal_pending_state先返回0；只排除这个信号门，不证明没有其它唤醒、任务状态变化或所有等待均成功。

M06的完成事件与waiter重锁观察之间不能省略其它消费者：只有重锁时实际仍见done>0，才能走完成消费分支。某producer曾增加一次不保证指定waiter取得这一份计数。
signal分支先于prepare；首次即break时wait可能尚未入队，finish仍按实际节点状态处理。不能把完成、唤醒或action正常返回解释成同一个事件。

__prepare_to_swait、__finish_swait和swake_up_locked的调用/入队/摘队源语句得到支持，但未完整审计其容器实现、头计数不变量、锁/内存序与对象寿命；不能签完整swait正确性。
后一步不正常返回时，只停在最早证据边界；不担保含未定义行为的机器代码必然保留所有此前副作用。
原49没有新增缺陷可保留；旧DS48等待缺口仅获得局部语义信息，不因此关闭。

## 4. R49-S02：图、来源与交付显示

原DOT完整4277字节经连接器文本重建，Git blob匹配远端对象；38唯一显式节点、45边、无未定义端点。结构检查不替代语义或证据支持。
图signal→sigbreak是真实generic helper分支，但从图中公开entry使用固定state进入时不可达；必须区分generic条件模型与公开调用，而非画成公开接口可报告信号错误。
wake→relock虚线只能表示有条件的跨上下文情形，仍须action正常返回、同一对象和成功取得锁；不是一次wake调用直接使waiter返回，也不保证该waiter一定消费计数。
trywake→unlink只在try_to_wake_up正常返回后成立；返回值不检查不等于成功或失败已知。SOURCE门、分析前提与未知实现保持分层。
对wrapper/初始gate的事实消费必须使用R49-01纠正后的支持关系，图计数正确不能抵销两个强制定义锚点不合格。

后续交付回复需先给最终当前状态，不再把初始“待加载”快照与最终“已交付”无区分并列。接收/加载明细保存在MANIFEST供追溯，回复开头给一行可直接转发的批次、短提交、评论号与待验收状态。
这是本轨道交付可读性改进，不据此认定49未执行接收或规则加载，也不要求另写接收批次。

## 5. 可读闸门、方法与未执行项

已完整读取75号、49六件和评论6681100222；长facts/evidence按范围补至尾。重读00工作令相关范围，沿用28/63及已有C04接管、R48/46C/47固定材料；没有全面刷新Mnemosyne或导入其维护任务。
本轮直接回源completion.h全文、swait.c/swait_macro.h/swait_types.h全文、signal.h460–515、task_const.h1–100、limits.h全文。
completion.c、completion_types.h、completion_macro.h、completion_s typedef及devtmpfs调用点使用本对话此前同一固定pin已完整/定点取得的原始源码，区别于执行者自述；不冒称本轮全部重新下载。
为50规划另外定位time真实timer/timer.c并读取1–240及750至文件尾。默认索引的time/timer.c在固定time不存在，沿固定树纠正；不以默认分支替换源码，不预签50结论。

P9可读子集：六件及目标路径、1126行纯新增、原件保留和NOT_RUN边界已核；36个标签的原始条目数相符，但引文定义归属/支持关系及强制wrapper/gate锚点FAIL，因此正式RETURN。
其它源码与模型按上述有界方法人工核对；全量原36引文自动扫描、原facts/deferred机械解析、全树符号/链接/配置、完整本地P9均未执行，不宣称其余34项全部重新通过。
本轮实际纯Python32模型：计数生产5、非零done快路9、慢路7、信号/边界/事件关系11；显式uint32模型，不认证编译ABI，不是作者原15条全集重放。
Graphviz、C/ASM、仓库脚本、预处理/编译、QEMU/硬件、真实调度/并发和额外付费API均NOT_RUN。AI角色独立不等于人类或已认证异构模型审查。

## 6. 后继与固定入口

77号CORE-USER-SCHEDULE-TIMEOUT-50继续交现有C04c，T00先补49文档，T01–T04独立核实际timeout参数、活动调度调用与返回消费；不扩大为完整timer wheel/调度器/锁或运行验证。
固定任务：https://github.com/08822407d/MyOS2/blob/a1beca5a67a1/agent-workspace/lead/MYOS2-LEAD-002/75-user-completion-wait-contract.md
固定成果：https://github.com/08822407d/MyOS2/tree/ea079bca1985/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-completion-wait-49
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6681100222
C04只新增主线回执/任务/检查点并维护既有PR导航；原PR17成果不改，R49-01由后继独立闭合，50不得自行开始51。
