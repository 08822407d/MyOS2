---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SEMAPHORE-TIMEOUT-51
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6 (Owner-reported, backend unverified)"
date: 2026-11-06
base_snapshot: time（分支名）@a039d9803ade（短号）
read_channel: connector
record_type: bounded_semaphore_timeout_analysis
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第51批：count与超时/授予链的静态取证
## S01 对象、初始化、快路径
`sema_t`实为struct semaphore，包含spinlock_t lock、uint count、普通List_s wait_list；局部struct semaphore_waiter含List_s list、task_s* task、bool up。静态/动态初始化从__SEMAPHORE_INITIALIZER/DEFINE_SEMAPHORE/sema_init衔接（SM51-A001..A006）。此wait_list是普通List_s头，而非swait的List_hdr_s计数表头。
`down_timeout`首先`int result=0`、锁住sem->lock，若count>0直接count--并解锁返回0（不论timeout=-1/0/1/一般正数/MAX，均不会进入信号或时间门）；count=0才调用__down_timeout，后者传TASK_UNINTERRUPTIBLE、timeout到__down_common->___down_common，再将int结果逐层返回（A007..A012）。普通down是void、慢路MAX；down_interruptible传TASK_INTERRUPTIBLE与MAX，只做差异对照，不当作down_timeout的信号语义（A025..A031）。

## S02 慢路径、信号与超时
慢路在已有锁下创建栈上waiter，list_add_tail到sem.wait_list，写task=current及up=false。每轮先检查signal_pending_state，为真去interrupted摘除自己的waiter，返回-EINTR；若为假再测timeout<=0，为真去timed_out摘除并返回-ETIME。无资源且timeout=0或负值时，入队及两门检查可在schedule_timeout、state设置之前完成错误返回；与count>0快路径严格区别（A013..A019）。EINTR和ETIME来自两个实际不同的单宏，非通用errno猜测。
正timeout则设task state、spin_unlock_irq、timeout=schedule_timeout(timeout)，只有被调函数正常返回才spin_lock_irq重锁，检查waiter.up为真立即return0，即使timeout仍正；为假回到下一轮两个门。成功路径自身不list_del；授权方__up事先摘除；错误出口自己list_del。state为long传入uint signal helper，TASK_UNINTERRUPTIBLE=0x2在所读首掩码门下返回0（不含TASK_INTERRUPTIBLE=0x1或TASK_WAKEKILL=0x100），故公开down_timeout不因该门返回-EINTR；generic TASK_INTERRUPTIBLE/KILLABLE可在合适信号条件下先于超时返回-EINTR，但不是本入口保证。
第50批已核schedule_timeout：有限非负timeout正常结束时保留原timeout，函数体内schedule()在该分支是注释；MAX有一次活动schedule，正常返回MAX。因此在无授予、有效对象、所有callee正常返回的有限正等待模型中，timeout不能自然减至0，下一轮重复；但若up曾授予使waiter.up=true，正常恢复并重锁后仍可return0。不能将“有限超时不倒计时”升级成“任何情况下都不能成功”或已实机无限循环。若action不正常返回，后续重锁/flag检查/返回均未证明发生。

## S03 up授予与普通list
`up`持同一sem锁，空队列直接count++，非空调用__up：选择list_first_entry队首waiter、list_del、写waiter.up=true、调用wake_up_process，再外层解锁。非空分支不直接增加count；资源在本地等待者标志中授予。list_add_tail是list_add_to_prev别名，list_first_entry使用head->next，list_del摘链并毒化节点，list_empty检查head->next==head（A019..A024、A032..A037）。只有有效普通双链头和同步前提才能推局部队首优先，不能签全局FIFO、公平或内核链表正确。
跨上下文有条件次序：waiter入队并解锁→up获锁、摘队、置up标志、提出wake请求→如waiter的schedule_timeout正常返回并重锁观察up为真→down_timeout返回0。flag写入≠唤醒成功≠任务运行≠返回0。先超时摘除再up时，后者若看到空队列则增加count，不能回溯修改已返回的-ETIME。两个waiter A先B后入队的有限模型中一次up挑A而B仍排队，真实任务调度与公平仍不在范围。

## S03 已知CA-03和直接调用者定位边界
共26个有界源条件模型，含count×timeout、signal和timeout优先级、两waiter、先后up、action不返回等；详见facts，非宿主夹具。
旧core/results.yaml::CA-03/V09已有有限超时不递减的宿主夹具观察，本批仅固定time独立追踪semaphore直接消费者，保留旧ID和有界裁定，不另立同根SM51-F。通过GitHub默认分支索引和REST code-search检索down_timeout，未得到可在固定time进一步回源的外部调用点；这不是完整固定time全仓索引，不能断言无其它调用者或实际运行可达。

## 局限
第49批R49-01已由C04文档闭合且组合PASS_PENDING_LOCAL，第50批由C04 PASS_PENDING_LOCAL，均不重做。普通DS48/CW49/ST50/IP47/UV历史状态未改。本批不执行内核C/ASM/脚本/编译/预处理、QEMU/硬件、不审完整timer wheel、调度器、锁/IRQ、信号全链、对象生存期或真实SMP。结论是有前提的源码事件关系，不是实机唤醒、死锁或正确性认证。
