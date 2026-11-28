---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88
record_type: bounded_work_item_submission_notification_and_callback_analysis
produced_by: GPT-6 (visible family; backend not independently attested)
model_per_owner: GPT-6 + Extra
date: 2026-11-15
base_snapshot: time
read_channel: connector
status: AUTHOR_STATIC_READY_FOR_MAINLINE_REVIEW_AFTER_PUBLICATION
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [WQ88-U01, WQ88-U02, WQ88-U03, WQ88-U04, WQ88-U05, WQ88-U06, WQ88-U07, WQ88-U08, WQ88-U09]
---

# 第88批｜工作项提交、pool通知和worker回调前后

固定证据只来自MyOS2 `time@a039d9803ade`；上游125号合同`cf2377d7c580`；本批从执行分支已有`bc611665b38f`接续，仍是原MYOS2-A-C06b和CHAT-EXEC-11。不修改86、87成果，不引入D00。第86批R86-01已由C06关闭、组合PASS_PENDING_LOCAL；第87批限定PASS_PENDING_LOCAL，需合读R87-S01/S02/S03、R86-S01/S02/S03。两个PASS均不认证编译/真实启动。以下只做本固定源码中的有限静态分析；先由有效对象、初始化、CPU和指针范围作为前提，不把想象的可达性当真实机器状态。

本批 `evidence.md` 有103项从固定time重新核取的真实单定义内1–5行短锚；`facts-and-boundaries.yaml` 保留10组62行结构化矩阵、26个带中间状态的有限条件模型及新读／同会话复用／未读账；`deferred-findings.yaml` 区分WQ88-F01..F07和WQ88-U01..U09。不复算87的perCPU分配器：WQ对象的cpu_pwq槽、PWQ对象/池和CPU实例地址合法性，仍受PA87-F05/U06等旧条件限制。

## G01/A｜提交、工作状态和返回值

真实 `queue_work(wq,work)` 只是把CPU实参设为 `WORK_CPU_UNBOUND`并调用`queue_work_on`。该值的源码定义是`NR_CPUS`，属于CPU选择哨兵；`WQ_UNBOUND`则是工作队列flags中的另一个标志。内层`__simple_queue_work`遇到前者把cpu改为`smp_processor_id()`，原有针对WQ_UNBOUND的其它CPU路由分支在所读代码中被注释。这不说明实际CPU值有效，更不意味着已经审计unbound工作队列。

`queue_work_on`的执行性语句依次初始化`ret=false`、`local_irq_save`，同步调用返回类型为void的`__simple_queue_work`，**如果该调用正常返回**则执行`ret=true`、恢复IRQ并返回ret。把工作项pending位作test-and-set的包围条件已注释，当前固定函数源码中没有正常返回false的独立分支；这里的true仅证实此源码走到了赋值/返回，而不是链表已插入、worker已被通知或回调已执行。

内层首先以`pwq=*per_cpu_ptr(wq->cpu_pwq,cpu)`和`pool=pwq->pool`取得对象，再请求`spin_lock(&pool->lock)`。即使这些都有效，当`work->entry`已经是非空链节点，`WARN_ON(!list_empty(...))`也会使源码跳到`out`、请求解锁而不调用`list_header_add_to_tail`或`kick_pool`。此仓库所读`WARN_ON`宏是直接求值`(condition)`，不能仅凭WARN这个名称认证已经打印了诊断。举例WQ88-M02/M03：entry已有链、pending=0或1，假定先行操作及解锁正常返回，则本次无新增入链、无kick，但外层返回仍为true。这里是条件性返回语义失配，不是复现的重复提交。

工作项定义有`atomic_long_t data`、`List_s entry`和`work_func_t func`三个独立成员；pending/PWQ/linked标志常量存在，但这不构成对应对象的初始化或实际写入。原合同所述一次默认分支INIT_WORK检索仅命中注释，不能用于全树否定结论。本包未找到能够在**所读固定入口**中证明当前work对象的data、entry、func均已正确建立的完整生产者，因此每个后续可达样例均要显式假定合法节点、可调用函数和正确PWQ数据。data的pending位与entry链态没有由当前提交函数建立的双向不变量；两者不能混同。

## G02/B｜pool工作链表、kick决策与唤醒请求

在有效WQ/PWQ/CPU实例前提下，内层持pool锁，先检查entry是否已链接，再请求`list_header_add_to_tail(&pool->worklist,&work->entry)`，**之后**调用`kick_pool(pool)`，最后走到解锁。链表包装的真实定义先调用`list_add_to_prev`，成功返回后才增加表头count；其下级`__list_add_valid`有等待循环。因此“已到达链表调用点”“节点/anchor真正更新”“count加一”都是不同前缀，需以该链的内存和前序callee正常返回作为门；有非法链对象时不得继续声称通知已经发生。

`kick_pool`先取得`first_idle_worker(pool)`。该辅助函数在`idle_list`按count为空时返回NULL，否则调用首元素转换宏。之后`kick_pool`判定`need_more_worker(pool) = (worklist非空 && nr_running==0)`。工作表count0、运行数非0，或者没有空闲worker，都会使`kick_pool`返回false，尽管之前的合法入链可能已经完成。源中`may_start_working`又只依`idle_list.count!=0`，`keep_working`只依工作表非空且`nr_running<=1`，它们不是“某线程已运行”证书。

当队列非空、nr_running为0、idle worker存在、且**不是POOL_BH**时，`kick_pool`取`worker->task`，请求`wake_up_process(p)`，不检查这个callee的整数返回，随后返回true。该callee在所读调度源里又转调`try_to_wake_up`；本包没有重做调度器或证明真实任务获得CPU。WQ88-M11把它的返回0列为条件输入，`kick_pool=true`并不推出实际回调开始。另一分支更必须独立：同样需要worker且POOL_BH为真时，实际代码进入`while(1);`，其后虽然书写`return true`却不能从这条路径证明正常到达。若这组字段值真实出现，入链后可能在kick里无法正常返回；没有实机资料，不说已发生死循环或全局死锁。

`init_worker_pool`写出worklist与idle_list初始化调用；`worker_enter_idle`设WORKER_IDLE并将节点加入idle_list，`worker_leave_idle`在WARN门后清标志并删节点。真实结果仍取决于初始化者及链操作返回。worker的`WORKER_NOT_RUNNING`为PREP、CPU_INTENSIVE、UNBOUND、REBOUND的组合掩码：进入NOT_RUNNING时`worker_set_flags`可以减`nr_running`，只有清除最后一个NOT_RUNNING位时`worker_clr_flags`才增计数。空表、计数、worker任务及标志的真实状态没有经执行证明，也不能由代码注释替代锁/IRQ或全系统并发审计。

## G03/C｜worklist→scheduled、PWQ解码、实际回调前后

`simple_worker_thread`在`woke_up`请求锁pool并离开idle，先根据need_more_worker决定是否进入`sleep`，必要时走manage_workers/recheck。然后调用`WARN_ON_ONCE(!list_header_is_empty(&worker->scheduled))`，清除PREP/REBOUND，再在`keep_working(pool)`条件下取工作表首元素，调用`simple_assign_work`。这只能证明**源码层面的条件接续关系**；worker任务真正创建、调度、进入此函数、标志/计数符合预期，仍属于待证门。

`simple_assign_work`在声明合法work属于pool->worklist时先调用`list_header_delete_node`，再`list_header_add_to_tail(worker->scheduled,entry)`，若二者都正常完成则worklist计数减1、scheduled计数增1，最后字面返回true。**源未检查删除函数返回指针**，所以其true也不能无条件当成“确实从前一条链删除了工作项”。`process_scheduled_works`使用`while (work = list_headr_first_container(&worker->scheduled,...))`逐项调用`process_one_work`。实际上，`list_headr_first_container`展开为`list_first_entry(anchor,...)`，又经`container_of`用anchor.next计算容器指针；这里没有判空。即使合法空链表有count0且anchor.next=anchor，取首元素时仍会计算一个以表头哨兵为基础的容器地址，而不是有保障的NULL。故在原列表为空、或唯一工作处理完而链表变空的后续迭代，`while(work)`**不能据此认证安全结束**；在非对象地址被解引用之前必须停止证明。具体“可能形成非空无效指针”是带地址和C语义前提的风险，不是实测崩溃或已经无限循环。

`process_one_work`首先`pwq=get_work_pwq(work)`。该宏链以`atomic_long_read(work->data)`取值，若设置`WORK_STRUCT_PWQ`则用`WORK_STRUCT_PWQ_MASK`屏蔽低位并转为PWQ指针；否则返回NULL。接下来源码先设置`worker->current_work`、`current_func=work->func`和`current_pwq=pwq`，然后执行`strscpy(worker->desc,pwq->wq->name,...)`，尚无针对pwq==NULL的本地保护。因此不能将“工作项在list上”自动当成“data已记录合法PWQ”。WQ88-M18/M19分别选择无PWQ标志和有标志但掩码后地址为零的隔离模型，正常执行证明在首次需要真实PWQ对象的字段访问处停止；绝不能继续声称真实callback已被调用。87中的cpu_pwq地址/初始化缺口仍为另一独立前提。

在正确PWQ、有效work->func和合法列表等条件下，函数先复制描述字符串、从scheduled删除entry、视WQ_CPU_INTENSIVE更新worker flags、请求kick_pool，再请求`spin_unlock_irq(&pool->lock)`，之后才执行`worker->current_func(work)`。pending清除和若干统计、trace均为注释，不能因Linux传统行为补出本源码没有执行的步骤。如果回调正常返回，后面重新请求`spin_lock_irq`、清除CPU_INTENSIVE位并把`current_work/current_func/current_pwq`三个字段置NULL。若回调不返回，源并不会到达这些后续清理；若回调释放、改写或重新入队工作项，则必须另加对象生命周期和新调用的返回门，不能默认为某种“恰好执行一次”保证。

即使worker完成一次回调，上一段的scheduled首元素空表问题也可能阻止`process_scheduled_works`被证明正常返回。另一条独立路径是`simple_worker_thread`进入`sleep`，调用worker_enter_idle、`__set_current_state(TASK_IDLE)`、解锁并`schedule()`，之后的实际休眠与唤醒、再次获得CPU与调度时序都没有在本批认证。

## 条件结果分层与证据不足

本包区分四层“成功”：

| 层级 | 最小源码事件 | 不自动意味着 |
|---|---|---|
| API返回true | queue_work_on内部调用正常返回后执行ret=true | 工作项真正新增入链 |
| 加入pool链 | 合法list_add完成，worklist节点/计数发布 | kick找到空闲worker |
| kick通知 | 普通池调用wake_up_process且自身返回 | task实际获得CPU |
| worker回调 | 合法scheduled、PWQ、func、锁及所有先行callee通过，`current_func(work)`真正被调用 | 回调一定返回、对象依然存活或后续清理一定完成 |

新WQ88-F01..F07是条件性候选，不是七个已经复现的系统缺陷；WQ88-U01..U09是九类证据缺口，不应相加为16个bug。前序PA87-F05/U06与PWQ初始化、R87-S01/S02/S03、R86-S01/S02/S03的ABI/链接/CPU条件继续适用；所有旧qualified_id保持原身份，不能借这批文字关闭。结构化矩阵和模型见后附索引与facts，图中的数据关联、直接C调用、同函数源序与未证假设分别表示，不是完整CFG。

执行许可没有覆盖真实编译、预处理、链接、仓库脚本/Graphviz、QEMU、硬件、多核启动/真实并发、工作项实际执行或补丁。因此本报告及有限模型是**源码层静态分析**，完整本地P9保持NOT_RUN。正式提交号、评论号及全部远端回读需要在发布之后由唯一PR17正式评论后置登记。完成发布后只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING；经原MYOS2-A-C06使用Pro独立验收本88并同轮安排后继，执行者不自行签收也不启动89。


## 十组可读矩阵（逐行条件结果；完整前提见facts）

### submission_and_return_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-A-S01 | caller selects current-CPU sentinel routing, not WQ_UNBOUND queue flag | CPU index and actual WQ state must be valid | WQ88-E001, WQ88-E014, WQ88-E015 |
| WQ88-A-S02 | normal path invokes internal void helper without active pending test | IRQ helper return and valid objects | WQ88-E002, WQ88-E003, WQ88-E018 |
| WQ88-A-S03 | true is source-level completion of enqueue attempt, not an insertion/wake/callback receipt | callee may skip insertion or never return | WQ88-E004, WQ88-E008, WQ88-E009 |
| WQ88-A-S04 | no ordinary defined false-return branch established in this function | do not treat aborted/trap/nonreturn as returned false | WQ88-E002, WQ88-E003, WQ88-E004 |
| WQ88-A-S05 | linked-entry branch skips list_add and kick; outer can return true if helper returns | node validity and WARN macro expansion required | WQ88-E008, WQ88-E009, WQ88-E016, WQ88-E004, WQ88-E102, WQ88-E103 |
| WQ88-A-S06 | two distinct statements; list mutation conditional on helper returns | list helper may block/invalid; kick may not return | WQ88-E008, WQ88-E009, WQ88-E046 |

### work_state_and_data_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-A-D01 | three separate producers/consumers, not implied by workqueue name | this scoped read does not obtain real work initializer | WQ88-E010, WQ88-E011 |
| WQ88-A-D02 | pending value 0 or 1 does not select different active source branch | other writers/unseen call site not exhaustive | WQ88-E012, WQ88-E013, WQ88-E003 |
| WQ88-A-D03 | flag present necessary for non-null decoded PWQ, not sufficient for valid address | atomic long read value, pointer provenance and lifetime missing | WQ88-E021, WQ88-E022, WQ88-E023 |
| WQ88-A-D04 | linkedness test is independent from work->data pending bit | source has no invariant proving data and list always match | WQ88-E008, WQ88-E010, WQ88-E016, WQ88-E102, WQ88-E103 |
| WQ88-A-D05 | queue_work_on does not itself invoke func or validate nonnull | valid function pointer producer and later thread reachability not proven | WQ88-E010, WQ88-E011, WQ88-E069, WQ88-E074 |
| WQ88-A-D06 | later process_one_work cannot be certified when it dereferences pwq | no actual queued work data observed | WQ88-E022, WQ88-E023, WQ88-E070 |

### cpu_selection_and_preconditions_matrix（5行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-A-C01 | actual perCPU slot CPU index derived from current CPU, not arbitrary workqueue flag | smp_processor_id and real CPU topology validity not measured | WQ88-E006, WQ88-E007, WQ88-E014 |
| WQ88-A-C02 | input CPU is used for wq->cpu_pwq lookup | 0<=cpu<N and 87 pointer decoding/lifetime prerequisite | WQ88-E005, WQ88-E006, WQ88-E007 |
| WQ88-A-C03 | this scoped internal helper still selects current CPU for sentinel | full unbound queue setup remains outside scope | WQ88-E006, WQ88-E015, WQ88-E020 |
| WQ88-A-C04 | first derefs can fail before any queue test or returned true | PA87-F05/U06 and initialization gates remain unresolved | WQ88-E007, WQ88-E008 |
| WQ88-A-C05 | normal path cannot be extrapolated beyond first invalid access | no real occurrence asserted | WQ88-E007, WQ88-E008, WQ88-E010 |

### pool_queue_publication_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-B-Q01 | pool object selected before acquiring lock | 87 address/initialization and CPU identity unproved | WQ88-E007, WQ88-E008, WQ88-E036 |
| WQ88-B-Q02 | duplicate linked work skips insertion and kick; source may unlock | WARN macro just evaluates, node must be live | WQ88-E008, WQ88-E016, WQ88-E048, WQ88-E049 |
| WQ88-B-Q03 | pool worklist count advances N→N+1 when underlying add completes | __list_add_valid/memory/synchronization not globally verified | WQ88-E009, WQ88-E046, WQ88-E043, WQ88-E097, WQ88-E098, WQ88-E099, WQ88-E100 |
| WQ88-B-Q04 | worker notification request can be separate and conditionally skipped | kick_pool can return false, nonreturn or wake callee failure | WQ88-E009, WQ88-E024, WQ88-E025, WQ88-E026, WQ88-E027 |
| WQ88-B-Q05 | source requests two empty headers, distinct anchors/count fields | real init path and perCPU backing integrity not attested | WQ88-E042, WQ88-E043, WQ88-E037, WQ88-E038 |
| WQ88-B-Q06 | POOL_BH branch is specifically conditional, not universal worker behavior | actual initialized flag values require source producer/return | WQ88-E039, WQ88-E026, WQ88-E068 |

### idle_worker_and_kick_matrix（7行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-B-K01 | need_more_worker false, kick returns false if callee returns | first_idle_worker called first, must return normally | WQ88-E024, WQ88-E025, WQ88-E028, WQ88-E045 |
| WQ88-B-K02 | kick returns false even when idle worker pointer nonnull | nr_running producers and actual current execution unverified | WQ88-E025, WQ88-E028, WQ88-E037 |
| WQ88-B-K03 | kick returns false, no wake_up_process | count/anchor consistent, no manage_workers implied | WQ88-E024, WQ88-E025, WQ88-E031, WQ88-E038 |
| WQ88-B-K04 | return true means callee returned after wake request, not task ran or function callback | task pointer and nested wake implementation/CPU state unknown | WQ88-E024, WQ88-E025, WQ88-E027, WQ88-E031, WQ88-E052 |
| WQ88-B-K05 | normal return from kick_pool not established under branch | no actual BH run or trap measured | WQ88-E026, WQ88-E039 |
| WQ88-B-K06 | list count & flag change under accepted helper path; no automatic notification implied | worker task/idle list data and lock correctness unproved | WQ88-E032, WQ88-E033, WQ88-E034, WQ88-E035, WQ88-E046, WQ88-E047 |
| WQ88-B-K07 | count alone is not proof of the list links/object safety | source doesn't establish global invariant across all writers | WQ88-E045, WQ88-E031, WQ88-E082 |

### locking_and_nonreturn_prefix_matrix（7行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-B-L01 | save request before entry and restore request only if callee returns | arch IRQ helpers/actual IRQ state not proven | WQ88-E002, WQ88-E003, WQ88-E004, WQ88-E018, WQ88-E019 |
| WQ88-B-L02 | source lock request/return bracket only, not global concurrency certification | raw spin implementation/real interleavings beyond scope | WQ88-E008, WQ88-E009, WQ88-E048, WQ88-E049 |
| WQ88-B-L03 | count increment and kick cannot be presumed reached | underlying helper may stall or accesses illegal pointers | WQ88-E046, WQ88-E047, WQ88-E097, WQ88-E098, WQ88-E099, WQ88-E100 |
| WQ88-B-L04 | caller can remain before out unlock and outer true return | not a measured deadlock or actual worker execution | WQ88-E026, WQ88-E009, WQ88-E004 |
| WQ88-B-L05 | kick_pool true independent of returned integer, provided callee returns | scheduler wake behavior not reaudited; no started-worker proof | WQ88-E027, WQ88-E052 |
| WQ88-B-L06 | counter not solely determined by worklist.count or creation success | actual flags/CPU context and valid counter are separate gates | WQ88-E040, WQ88-E041, WQ88-E078, WQ88-E079, WQ88-E080, WQ88-E081 |
| WQ88-B-L07 | new worker source creation not a proof scheduled/task running | kthread_create_on_node, start scheduler and hardware not audited | WQ88-E094, WQ88-E095, WQ88-E096, WQ88-E032 |

### scheduled_transfer_and_empty_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-C-T01 | only if need_more and may_start/manage gates return can clear PREP and loop | no real kthread or scheduler execution proof | WQ88-E057, WQ88-E058, WQ88-E059, WQ88-E060, WQ88-E061, WQ88-E062 |
| WQ88-C-T02 | selected work pointer is source candidate, not proven initialized work | list count/anchor and memory validity required | WQ88-E030, WQ88-E062, WQ88-E063, WQ88-E082, WQ88-E083 |
| WQ88-C-T03 | if delete succeeds and add succeeds: counts worklist N→N-1, scheduled M→M+1 | delete return is not checked and linkedness/lifetime unproven | WQ88-E053, WQ88-E054, WQ88-E046, WQ88-E047, WQ88-E086 |
| WQ88-C-T04 | true indicates source path return, not downstream callback success | list helper may nonreturn/invalid before value | WQ88-E053, WQ88-E054 |
| WQ88-C-T05 | macro computes container from anchor, not nullable first-element API; loop is not proven to terminate on empty | container_of numeric/address may produce invalid pointer; stop before process_one_work | WQ88-E055, WQ88-E082, WQ88-E083, WQ88-E084, WQ88-E085 |
| WQ88-C-T06 | after removal on one item, next iteration meets empty-list hazard; cannot conclude function returns normally | subroutine has get_work_pwq and callback gates before removal | WQ88-E055, WQ88-E056, WQ88-E070, WQ88-E086 |

### pwq_decode_and_callback_matrix（7行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-C-P01 | process_one_work assigns current_pwq=NULL then pwq->wq->name expression cannot be certified | actual data producer not read, CPU/pool conditions still required | WQ88-E022, WQ88-E023, WQ88-E066, WQ88-E069, WQ88-E070 |
| WQ88-C-P02 | non-NULL valid PWQ requires object lifetime and alignment, not mere flag | work->data and PWQ construction not shown in submit path | WQ88-E021, WQ88-E022, WQ88-E090, WQ88-E093 |
| WQ88-C-P03 | three fields get assigned before scheduled deletion and kick | work->func valid function pointer and PWQ->wq readable | WQ88-E069, WQ88-E070, WQ88-E087, WQ88-E089 |
| WQ88-C-P04 | normal progress proof stops at first invalid PWQ/wq/name reference | allocation+CPU pointer assumptions inherited from 87 | WQ88-E070, WQ88-E089 |
| WQ88-C-P05 | scheduled count decremented if contains true; work->entry self-linked after delete | membership return ignored, list header storage validity unproven | WQ88-E070, WQ88-E086, WQ88-E047 |
| WQ88-C-P06 | callback request occurs with pool lock unlock source prior to call | callee may not return; lock state in hardware not observed | WQ88-E072, WQ88-E073, WQ88-E074, WQ88-E050, WQ88-E051 |
| WQ88-C-P07 | C source doesn't establish self-requeue eligibility or single-run guarantee | other writers/all initialized work consumers not exhaustively read | WQ88-E003, WQ88-E012, WQ88-E013, WQ88-E073 |

### completion_and_requeue_boundary_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-C-F01 | conditional source reaches current_work/func/pwq=NULL | callback could mutate worker/work, memory and locks need independent proof | WQ88-E074, WQ88-E075, WQ88-E076, WQ88-E077 |
| WQ88-C-F02 | post-callback worker state cannot be claimed updated | not observed; conditional scenario only | WQ88-E074, WQ88-E075, WQ88-E077 |
| WQ88-C-F03 | a callback may attempt reuse, but no outcome or global safety guarantee | work lifetime/list membership/irq/worker availability unknown | WQ88-E003, WQ88-E008, WQ88-E074, WQ88-E075 |
| WQ88-C-F04 | clearing one of several NOT_RUNNING bits need not increment if another remains | valid flags, pool->nr_running, CPU conditions and helper return | WQ88-E040, WQ88-E041, WQ88-E071, WQ88-E076, WQ88-E078, WQ88-E079, WQ88-E080, WQ88-E081 |
| WQ88-C-F05 | sleep call requested, not proof worker slept or woke again | scheduler and wakeup detailed semantics out of scope | WQ88-E059, WQ88-E064, WQ88-E065, WQ88-E032, WQ88-E035 |
| WQ88-C-F06 | even after one valid callback, next empty iteration may stop normal proof | do not continue through invalid object or claim actual infinite loop | WQ88-E055, WQ88-E056, WQ88-E082, WQ88-E083, WQ88-E084 |

### ABC_join_matrix（6行）

| 行ID | 条件源结果 | 不能跨越的首个门 | 锚ID |
|---|---|---|---|
| WQ88-ABC-J01 | inner linkedness false and list insertion are gates distinct from bool true | 87 per_cpu_ptr decoded slot and producer state must hold | WQ88-E001, WQ88-E003, WQ88-E007, WQ88-E008, WQ88-E009, WQ88-E046 |
| WQ88-ABC-J02 | a queue entry can exist without proven task wake or CPU execution | idle worker/POOL_BH/task pointer and nested callee | WQ88-E024, WQ88-E025, WQ88-E026, WQ88-E027, WQ88-E028, WQ88-E031, WQ88-E052 |
| WQ88-ABC-J03 | two list headers count operations need valid object/containment | worker thread start and count/anchor consistency unproven | WQ88-E062, WQ88-E063, WQ88-E053, WQ88-E054, WQ88-E086 |
| WQ88-ABC-J04 | actual callback invocation is later than successful queue/transfer gates | no proven work data producer or target lvalue valid | WQ88-E022, WQ88-E023, WQ88-E066, WQ88-E069, WQ88-E070, WQ88-E073, WQ88-E074 |
| WQ88-ABC-J05 | per-work callback completion ≠ queue_work true ≠ worker wake signal | lifetime/requeue/lock scheduler states unknown | WQ88-E075, WQ88-E076, WQ88-E077, WQ88-E065 |
| WQ88-ABC-J06 | no new WQ88 proof closes PA87-F05/U06 or earlier entries, no runtime acceptance | Pro mainline independent review still pending | WQ88-E007, WQ88-E022, WQ88-E023, WQ88-E070 |

## 有界模型导航（26个；逐步中间值与真实输入见facts）

| 条件模型 | 场景 | 本模型能得出的结果 | 不能认证 |
|---|---|---|---|
| WQ88-M01 | N/A | {"return":true,"inserted":true,"woken":false,"callback":false} | all objects and CPU conditions unobserved |
| WQ88-M02 | N/A | {"return":true,"inserted":false,"kick_called":false} | not an actual duplicate event |
| WQ88-M03 | N/A | {"return":true,"inserted":false,"kick_called":false} | bit value producer unknown |
| WQ88-M04 | N/A | {"return":true,"inserted":true,"pending_still_unknown":true} | no state writer certified |
| WQ88-M05 | N/A | {"selected_cpu":1,"return_if_callee_returns":true} | runtime CPU count and slot validity not proven |
| WQ88-M06 | N/A | {"selected_cpu":1,"queue_flag_not_cpu_sentinel":true} | real CPU identity and unbound routing unknown |
| WQ88-M07 | N/A | {"normal_outer_return":"NOT_PROVEN"} | stop before invalid object access, do not claim false return |
| WQ88-M08 | N/A | {"wake_up_process_calls":0,"kick_return":false} | no worker callback proof |
| WQ88-M09 | N/A | {"kick_return":false,"wake_up_process_calls":0} | existing running worker may still consume queue |
| WQ88-M10 | N/A | {"kick_return":false,"wake_up_process_calls":0} | worker creation/reachability unknown |
| WQ88-M11 | N/A | {"kick_return":true,"callback_executed":"NOT_PROVEN"} | wakeup result need not imply task ran |
| WQ88-M12 | N/A | {"kick_normal_return":"NOT_ESTABLISHED","outer_true_return":"NOT_ESTABLISHED"} | no actual BH hang measured |
| WQ88-M13 | N/A | {"inserted":true,"kick_return":false,"outer_return":true} | no physical worker state observed |
| WQ88-M14 | N/A | {"idle_count_after":0,"flags_after":"WORKER_PREP"} | not live worker scheduling |
| WQ88-M15 | N/A | {"pool_count":0,"scheduled_count":1,"callback_called":false} | process_scheduled_works and PWQ still gated |
| WQ88-M16 | N/A | {"returns_NULL":"NOT_GUARANTEED","normal_termination":"NOT_PROVEN"} | stop before deref a nonexistent work |
| WQ88-M17 | N/A | {"scheduled_count_after":0,"loop_normal_return":"NOT_PROVEN"} | conditional next empty iteration can stop proof |
| WQ88-M18 | N/A | {"callback_reached":"NOT_PROVEN"} | no actual work data producer |
| WQ88-M19 | N/A | {"PWQ_valid":false,"callback_reached":"NOT_PROVEN"} | model does not assume this data occurred |
| WQ88-M20 | N/A | {"masked_pointer":"0x123400","callback_called_conditionally":true} | address is a hypothetical input, not kernel link value |
| WQ88-M21 | N/A | {"current_work":null,"current_func":null,"current_pwq":null} | callback may mutate objects; lifetime conditional |
| WQ88-M22 | N/A | {"post_clear":"NOT_REACHED","worker_loop_continue":"NOT_ESTABLISHED"} | not a measured callback hang |
| WQ88-M23 | N/A | {"flags_after":0,"nr_running_after":1} | actual worker counter not measured |
| WQ88-M24 | N/A | {"flags_after":"WORKER_UNBOUND","nr_running_after":0} | no real CPU status observed |
| WQ88-M25 | N/A | {"schedule_requested":true,"wakeup_and_return":"UNKNOWN"} | scheduler and task-state semantics not reaudited |
| WQ88-M26 | N/A | {"requeue_can_be_attempted":true,"exactly_once":"NOT_CERTIFIED"} | does not certify arbitrary callback/concurrent correctness |

模型是独立有限条件计算，不是运行MyOS2仓库程序或真实CPU。各行若出现非对象地址、未返回callee或无生产者字段，只记录停止正常证明的节点，不继续推断实际内存/调度结果。此正文可读矩阵、facts结构化矩阵、问题台账和图的ID与支持关系须结合具体条件阅读。
