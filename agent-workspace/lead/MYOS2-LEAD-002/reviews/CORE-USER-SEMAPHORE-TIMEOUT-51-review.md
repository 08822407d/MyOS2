---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-SEMAPHORE-TIMEOUT-51-REVIEW-001
record_type: independent_bounded_static_acceptance
evidence_class: 固定源码人工回核及独立条件模型
produced_by: ChatGPT主线独立复核
model_per_owner: gpt pro（Owner本轮确认）
backend_identity: unknown_or_not_attestable
conversation_display_name: MYOS2-A-C04 内核分析主线续建
date: 2026-11-07
base_snapshot: agent/MYOS2-LEAD-002
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract78_short12: 72ef20aeff03
reviewed_base_short12: b79feb4097e2
reviewed_head_short12: 1e128a33a014
reviewed_delivery_comment: 6688576624
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R51-S01, R51-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original48_automatic_quote_scan: NOT_RUN
original_JSON_DOT_mechanical_replay: NOT_RUN
reviewer_independent_gate_models: 14
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段单函数短引文, method: 固定源码文本人工核对}
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
next_followup: CORE-USER-WAIT-WAKE-CONSISTENCY-52
next_task_ref: 80-user-wait-wake-consistency-contract.md
phase: 2
inputs_read: [51六件, 评论6688576624, 78号及R49C/R50限定, 28号, 本轮及此前同pin原始源码]
status: ACCEPTED_WITH_SUPPLEMENTS
open_questions: []
---

# 第51批限定接收；下一批采用有界组合工作包

## 1. 裁定与真实交付

依据78号及R49C-S01、R50-S01/S02，完整读取51六件及正式评论，独立回核核心源码后，裁定PASS_PENDING_LOCAL，无另行返工项。原六件及旧评论冻结；后续合读R51-S01/S02。
实际PR17头1e128a33a014、评论6688576624；compare b79feb4097e2→1e128a33a014为ahead1/behind0、一提交，恰六件A新增1023行：MANIFEST61、正文33、facts283、DOT81、deferred96、evidence469。无旧件净改删。
PR16头72ef20aeff03，PR16/17均open/unmerged，PR17 draft。执行者声明STOP_WRITING。声明不证明另一聊天内部轨迹或后台模型。
49文档闭合及50限定接收不变；CA-03/V09与DS48、CW49、ST50、SM51-U等原身份/状态不自动改变。本轮不签运行正确或端到端资源生命周期通过。

## 2. 独立确认的关键条件

count快路先于慢路时间和信号门；有效对象、实际锁及正常返回前提下，已有资源即递减并返回初始化的0，即使timeout非正。
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_timeout]
```c
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_timeout(sem, timeout);
```
慢路先入队，循环先查信号再查timeout<=0；公开TASK_UNINTERRUPTIBLE不满足所读signal_pending_state首掩码门。错误出口各摘本waiter，返回独立宏EINTR=4/ETIME=62的负值。
既有schedule_timeout有限正值不递减，不等于永远无法获得资源。重锁后先检查已授予标志，成功在下轮信号/时间检查之前返回：
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
				spin_lock_irq(&sem->lock);
				if (waiter.up)
					return 0;
```
up非空分支先选队首，摘链、写标志、再请求唤醒；该分支不直接增加sem.count：
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__up]
```c
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
```
普通List_s头和swait的List_hdr_s计数头不可混同；有限序列只支持局部队首选择，不能据此认证全局公平、真实唤醒或栈上waiter寿命。

## 3. R51-S01：输入、授予与状态边界

- sem.count为uint。sema_init的int val转入该字段和up的count++均须按实际类型解释；无饱和检查不能推广为所有输入都单调增加。现有0/1模型不证明初始化负值或溢出安全，本轮不扩成新缺陷。
- 宏文本展示初始化表达式，不证明全部嵌套宏选择、构建链接或锁运行正确。time固定源码与目标构建实例分开。
- down_timeout快路绕过时间/信号门，但不能由timeout本身推断必走快路。慢路的-EINTR分支是generic/其它state模型，不是公开TASK_UNINTERRUPTIBLE的已有出口。
- waiter.up成功检查位于action正常返回及重锁之后、下轮信号/时间门之前。已授予的局部flag、wake调用完成、等待者获得CPU与down_timeout返回是不同事件；某一步失效不能直接跨到后一步。
- 锁有效并覆盖队列访问时，超时摘队与up选择同一waiter不能无约束同时成立。先失败摘除后出现up，不回溯改变已返回的错误；两个waiter模型还需有效链、无其它修改及正常调用前提。
- timeout不递减的根因继续关联CA-03/V09，不能新增一个同义缺陷来增加数量。默认分支索引没有找到外部调用者不证明固定time全树没有，更不证明用户可达或不可达。

## 4. R51-S02：证据和图示消费

48条标签及全文已通读；本轮回源semaphore.c/h/types/macro全文、sema_t单typedef、EINTR/ETIME与list空门。其余列表/信号/状态/timeout锚点合读本对话此前同一固定pin的原始源码，不把执行者证据账本本身当独立源。
部分evidence_metadata的consumer_positions为空或过泛，例如A010标up，却实际证明__down_timeout。该字段不单独证明语义关联：入口/慢路用A007–A019，释放侧用A020–A024，普通链表用A032–A037；正文的真实调用关系与实际定义优先。无需改写原件。
图signal的true generic边从公开down_timeout固定state进入时不可达；flag→grant虚线只表示同一waiter在正常action返回且重锁后读到flag，不是同步调用或单凭flag写入保证恢复。
图公共rslow/ounlock须匹配当前down_timeout的调用栈；成功路径的队列节点由授予侧先摘，不能在成功return后补出第二次list_del。
作者申报DOT36节点40边及26模型保留为作者自查；本轮做语义审阅，不冒称重放其机械图计数/哈希、JSON解析或26模型全集。

## 5. 实际核验、未执行项与后继

强制门、函数归属、实际参数、错误码、返回和正常前提人工核对；所需其它短引文通过已读同pin源码复核。无与49类似的函数体/声明区错配。完整本地P9、全量48条自动字节扫描、全树符号/构建认证仍NOT_RUN。
本轮只运行14个独立纯Python门序模型：count0/1×timeout五类10例，generic信号优先、公开信号屏蔽及有限/MAX授予4例。模型假定合法对象与先行调用正常返回，不运行仓库C/ASM/脚本或旧宿主夹具，不证明内核效果。
未编译/预处理/QEMU/硬件/真实并发，未安装工具或调用付费模型。可选公开raw读取因临时环境DNS失败未取到内容，正式证据全部来自连接器，不计GitHub写入失败。
本轮同时回应Owner的Pro额度/工作量问题，见79号。80号将几个相关可独立取证边界合为一个52工作包；仍由C04c执行、C04独立验收，不改变阶段2及安全边界。
固定成果：https://github.com/08822407d/MyOS2/tree/1e128a33a014/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-semaphore-timeout-51
交付评论：https://github.com/08822407d/MyOS2/pull/17#issuecomment-6688576624
