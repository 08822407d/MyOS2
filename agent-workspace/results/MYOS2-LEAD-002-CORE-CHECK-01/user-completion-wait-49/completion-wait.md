---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-COMPLETION-WAIT-49
track_id: MYOS2-LEAD-002
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6（自报；实际后台未独立认证）"
model_per_owner: GPT-6
effort_per_owner: Extra
date: 2026-11-06
base_snapshot: "time（分支名）@a039d9803ade"
record_type: bounded_static_completion_wait_report
read_channel: GitHub_connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第49批：completion 计数、等待和直接 swait 语义

## 已核核心结论
固定 `time@a039d9803ade` 中，`completion_s.done` 是完成计数，不是 errno；`complete()` 修改计数并发起 swait 唤醒请求，但不认证实际任务运行。公开 `wait_for_completion()` 为 `void`，其内部 `long` 返回值不传给调用者。所有结论限于已读函数文本与明示的条件；源码证据见 `evidence.md` E01–E36。

## W01：对象、初始化、生产者
`completion_s` 通过 typedef 指向 `struct completion`，成员为 `uint done` 和 `swqueue_hdr_s wait`（E01–E02）。静态 `COMPLETION_INITIALIZER(work)` 包含 `0` 与 `__SWAIT_QUEUE_HEAD_INITIALIZER((work).wait)`，其 lock 与 list-header 展开表达式见 E03/E06；`DECLARE_COMPLETION` 为声明宏，`DECLARE_COMPLETION_ONSTACK` 在此源码中仅别名（E04–E05）。动态 `init_completion` 写 `x->done=0` 并调用 `init_swait_queue_head`，宏进一步调用 `__init_swait_queue_head` 初始化锁和链表头（E07–E08、E33）。这些是代码及宏文本，不是完成预处理或自动变量生存期认证。

`complete()` 在 `spin_lock_irqsave` 后判断 `x->done != UINT_MAX`，非饱和则 ++；**无论是否饱和都调用** `swake_up_locked(&x->wait,0)`，最后解锁（E09–E10）。`UINT_MAX` 实际定义 `~0U`（E32）。

| 完成前 done | complete 后 done（无其他写者） | wake 请求 |
|---|---|---|
| 0 | 1 | 是 |
| 1 | 2 | 是 |
| 一般正值 k，1<k<U−1 | k+1 | 是 |
| U−1 | U | 是 |
| U | U，不递增 | 是 |

其中 U=UINT_MAX。饱和计数不是本批已审过的 complete_all 调用，也不等于 worker 错误码。请求不等同 try_to_wake_up 成功、任务运行或用户可观察成功。

## W02：公开入口、long helper、控制流和消费
入口 `void wait_for_completion(x)` 将 `MAX_SCHEDULE_TIMEOUT`（宏为 `LONG_MAX`）与 `TASK_UNINTERRUPTIBLE` 送入 `wait_for_common`（E11、E29）。后者返回 `long`：在 wait.lock 下调用 `do_wait_for_common(x,schedule_timeout,timeout,state)`，随后解锁并把 long 向内层调用者返回（E12）；最外层 void 不保存这个值。

**初始 done 非零（快路）**：`if (!x->done)` 为假，不创建 swait 待项、不执行 `action`；若 done!=UINT_MAX，则 done--；返回 `timeout ?: 1`，其语义为 timeout 非零取 timeout，零则取1（E13、E17）。在公开入口的初始正 timeout 下，内部 long 为 LONG_MAX；对调用公开接口的 C 调用者不可见。

**初始 done=0（慢路）**：声明局部 `wait`，每轮先检查 `signal_pending_state(state,current)`；若为真则将 timeout 设为 `-ERESTARTSYS`，跳出 do-loop；否则 `__prepare_to_swait`、`__set_current_state(state)`、释放 wait.lock、`action(timeout)`、重新取得 wait.lock（E13–E15）。正常返回 action 后测试 `!x->done && timeout`，真则重复，否则执行 `__finish_swait`；若 done 仍为0直接 return timeout，否则除 UINT_MAX 外消费一次，然后 return `timeout ?: 1`（E16–E17）。注意 `action` 不正常返回时不能声称其后的加锁、摘队、消费或函数返回。

**公开 state 使 generic 信号分支不可达**：固定 `TASK_INTERRUPTIBLE=0x1`、`TASK_UNINTERRUPTIBLE=0x2`、`TASK_WAKEKILL=0x100`（E26–E28）；`signal_pending_state` 若没有第1和第3位会直接返回0（E30）。因此这个公开入口不会因所读 helper 的该信号门赋值 `-ERESTARTSYS`；generic helper 传入别的状态时则可能触发该分支（E14、E31）。不能从内部 long 签名凭空推断公开不可中断入口可返回负 errno。

**返回值及计数彼此独立**：done 仍0、action 返回0时，`if (!done) return timeout` 得0，未消费计数；若 done 已变正且 timeout=0，仍经完成分支减计数（除饱和）并以 `timeout ?: 1` 得内部 long=1；公开 void 仍将这个值丢弃。假设 action 返回负非零且 done 仍0，循环条件中的 timeout 是真值，不会直接将该负值退出返回。`schedule_timeout` 的真实返回值域未经本批核查。

## W03：直接 swait，锁前提与有限事件序
`DECLARE_SWAITQUEUE` 定义栈上 `swqueue_s wait`，宏文本含 `task=current` 和 list 节点初始化；结构保存头锁/表头、待项的任务指针/节点（E18–E21）。`__prepare_to_swait` 重置 `wait->task=current` 并仅在节点当前为空时加到队尾；`__finish_swait` 设 `TASK_RUNNING` 且节点仍在队列时摘链（E22–E23）。在本次 wait 路径，wrapper 锁覆盖 prepare/finish；不能将它们看成自身获得锁的函数。

`swake_up_locked` 对空队列直接 return；非空选链表首项，调用 `try_to_wake_up(task,TASK_NORMAL,wake_flags)`，之后 `list_del_init`，没有检查该调用的返回值（E24–E25）。这只认证尝试唤醒并摘队，不认证它真的让目标进程运行，也没有从注释升格出公平性、全局FIFO、固定跨核全序或无丢唤醒结论。

## W03 有界时序矩阵
下表各行是附带前提的静态模型，不是运行实验。详细前提与 M01–M15见 `facts-and-boundaries.yaml`。

| 序列 | 能在本范围内推出的事 | 禁止延伸 |
|---|---|---|
| 先 complete(0→1) 再 wait | 后者快路消费1→0，不调用 action | 不证实际调度 |
| done>1 顺序消费 | 各次快路递减，直到0 | 不证明公平或事件身份 |
| done=U−1/U 时 complete | 分别增到U/保持U，仍调用 swake | 不证 complete_all |
| wait 从0进入慢路，producer 期间 complete | 同锁下 producer 提供 done 和尝试唤醒；仅在 action 正常返回、waiter 重取锁时可观察/消费 | 不证实际成功唤醒、无丢唤醒 |
| 无 producer，action 正常返回0 | finish 后若done0则return0且不消费 | 不证这种真实返回可发生 |
| 无 producer，action 返回正非零 | 循环 `!done && timeout` 为真而再次等待 | 不证最终终止 |
| 无 producer，action 返回负非零 | 负数是真值，不能立刻跳出 | 不证 schedule_timeout 会产生负值 |
| generic signal 分支条件成立，done保持0 | break 后finish、return -ERESTARTSYS，不消费 | 公开 state 下该门不会真 |
| action不正常返回 | 后续 re-lock、finish、consume 未发生 | 不证现实死锁 |
| done>0 且 action 返回0 | 消费后内部 long=1，公开 void 丢弃 | 不证 C caller 得到1 |

图 `completion-wait-paths.dot` 区分实线调用/控制/值传播、虚线有限跨上下文候选与 STOP 边界。producer 和 waiter 在不同执行上下文的操作不是单一调用栈；必须满足对同锁的拥有关系及每步正常返回才可讨论后续事件。

## 对48的收窄说明与未证事项
固定 `devtmpfs.c` 具有静态 `setup_done`，creator 等待、worker 后段通知（E34–E36）。原 `CORE-USER-DEVTMPFS-SETUP-48::DS48-F01` 在 worker 错误早退且无其他有效生产者的限定前提下，得不到来自该 worker 的 done 增量；本批证明公开 wait 不能凭内部 long 解释成 worker 状态或已解除等待。仍不能认证实机死锁、全部通知者缺席、对象有效/重用、系统启动后果；原DS48-F01/F02/F03和IP47/UV状态不变。

本批不审完整 `schedule_timeout`、`try_to_wake_up`、架构锁/中断内存序、线程/挂载/请求生存期。DEBUG 宏条件与链接实例未预处理，Graphviz、完整P9、C/ASM/脚本、编译、QEMU、硬件、运行与故障注入均 NOT_RUN。由 C04 按 R48-S01/S02 独立验收，最高 PASS_PENDING_LOCAL。

