---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-NEW-TASK-RUNQUEUE-ENTRY-136
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: executor_static_report
date: 2026-11-23
produced_by: GPT-6（后端未认证）
model_per_owner: GPT-6＋Extra（Owner指定）
base_snapshot: time（分支名）
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第136批：新子任务调度初始化、CPU0就绪队列和一次给定选择

这是阶段2的源码静态推演，不是实际运行结果。第135批三项错误仅在此新增文件中补正，旧原件、旧RETURN和数值证据不变；作者没有独立验收权限。

## D00 · R135-01：观察者对象与命名空间不是同一概念

旧十例的20字段各组原值、全部200键及十个返回均原样复制到 `facts.D00.R135_01.queries[*].old_preserved_twenty_fields`。新字段 `target_is_observer` 表示目标任务是否恰为当次 current，不是“缺省ns是否取自current”。五个缺省请求依次是 **true/false/true/false/true**，但它们的 ns 提供者都是当前 P 的 thread_pid 对象，顺着 T.level=1 得 N1。五个显式ns请求在 pid.c:473 的判断为假，跳过474，执行475；旧图的 `SOURCE_STATEMENT_ORDER_PID_C_474_BEFORE_475` 被改成“`IF_NS_NONNULL_SKIP_474`→`SEQUENCE_TO_475`”，不虚构474已执行，也不猜C实参求值顺序。

| 旧查询ID | current | target | 相等 | ns类型 | 原返回 |
|---|---|---|---|---|---:|
| C1_GETPID | P | P | true | DEFAULT_CURRENT | 20 |
| C1_GETPPID | P | RealParent | false | DEFAULT_CURRENT | 7 |
| C1_PIDVNR | P | P | true | DEFAULT_CURRENT | 22 |
| C2_N0_TGID | P | P | true | EXPLICIT_ARGUMENT | 5100 |
| C2_N1_TGID | P | P | true | EXPLICIT_ARGUMENT | 20 |
| C3_GETPPID_PARENT0 | P | RealParent0 | false | DEFAULT_CURRENT | 0 |
| C3_N0_GP0 | P | RealParent0 | false | EXPLICIT_ARGUMENT | 4100 |
| C3_GETPID | P | P | true | DEFAULT_CURRENT | 20 |
| C4_M1_G | P | P | true | EXPLICIT_ARGUMENT | 0 |
| C4_N1_G | P | P | true | EXPLICIT_ARGUMENT | 20 |

旧135 facts/DOT 的具体源ID、edge kind和新136相应图ID逐条存在机件 Q 数组；并保留旧134 EVALUATION_ORDER 参考。返回顺序20/7/22/5100/20/0/4100/20/0/20，十个查询没有以修正布尔值为名重算。证据 E136-001/002。

## D00 · R135-02：分层写槽及 IS_ERR 的变量身份

`copy_process` 中 K.pid!=0 的外门使 PIDTYPE_PID 槽 `K.thread_pid=KPID` 可写；这一步不需 K 是线程组长。随后另看 `thread_group_leader(p)`，为真才写 PIDTYPE_TGID 槽 `K.signal.pids[PIDTYPE_TGID]=KPID`。C1/C2 的两门均由合同给定为真，不能根据 args.exit_signal=SIGCHLD 推出 K.exit_signal 的实际值。C3 在 dup_task_struct 返回NULL，不到任何槽。旧 `A.copy_process.slots.require_group_leader=true` 被拆为各自 gate。

`kernel_clone` 用 `IS_ERR(p)` 判断 `copy_process` 返回的原始指针 p；成功时 p 才等于 K，失败时 p 为 ERR_PTR(-ENOMEM)，经 PTR_ERR(p) 返回 -ENOMEM。原图节点 `A135_iserr` 的 `IS_ERR(K)` 不成立，已改标签与两支。证据 E136-003至E136-007。

## D00 · R135-03：父方源码结果不支配子方实际执行

C1/C2 的父方 `kernel_clone` 成功源码返回分别是5200/30，`success_conditional_on_libc=false`。是否通过用户库/ABI成为 `fork()` 观察值仍 UNKNOWN。旧 C135_C1_19/C135_C2_19 指向未来子方 getpid/getppid 的四条边与指向C2显式N0 helper的一条边，都撤掉了暗示实际时序的含义；旧5边标识、原facts/DOT定位写在机件中。子方须先具有已保存的 ax=0、被入队、在未来确实被选择并执行、回到用户态及到达查询的独立前提。父子谁先回到用户态 UNKNOWN。C2的显式 `__task_pid_nr_ns(K,TGID,N0)` 结果5200仅作单独给定对象求值。

C1原26个字段和C2原27个字段、两个19步源码事件原结论继续冻结；C3原六步及11组未达保留于机件。作者不签R135关闭，交主线复核。证据 E136-006、E136-008至E136-010。

## A · sched_fork 初始化

普通成功路径 `kernel_clone → copy_process → sched_fork → __sched_fork`。活动写点：`se.vruntime=0`；`INIT_LIST_S(&rt.run_list)` 令节点next、prev都指向自己；`__state=TASK_NEW(0x800)`；`on_cpu=0`。注释掉的 `on_rq`、`rt.time_slice` 等不是已发生写点，不推定内存全局默认值。K、J 都按合同完成这段准备。copy_process成功后 kernel_clone 调用 wake_up_new_task，未因此证明新子任务执行。证据 E136-011至E136-017。

## B · CPU0队首插入的双向链与计数

`wake_up_new_task` 固定取 `&per_cpu(runqueues,0)`，写 `__state=TASK_RUNNING(0)`，向 `List_hdr_s running_lhdr` 头加入独立的 `List_s child.rt.run_list` 节点。源码调用链是 `list_header_add_to_head → list_add_to_next → __list_add_between`，四个真实写入依次为：`next->prev=new`、`new->next=next`、`new->prev=H`、`H.next=new`，然后 `count++`。两种顺序前后互反关系、四写的具体节点在 facts.B。

| 独立场景 | 第一步状态 | 第二步状态 |
|---|---|---|
| K后J | H-K-Q-R-H, count=3 | H-J-K-Q-R-H, count=4 |
| J后K | H-J-Q-R-H, count=3 | H-K-J-Q-R-H, count=4 |

反向分别是 H-R-Q-K-H、H-R-Q-K-J-H、H-R-Q-J-H、H-R-Q-J-K-H；尾节点始终R。该片段中未实际调用CPU迁移、队列锁、时钟更新、通用activate_task或check_preempt_curr（相应行被注释），亦未改rq.curr或need_resched；`on_cpu` 保持sched_fork写的0。**入队可运行与获得CPU执行不同**。证据 E136-018至E136-026。

## C · 103与110两次独立picker

每次均从新鲜的 H-Q-R-H,count2 开始，只插入K：H-K-Q-R-H,count3，rq.curr=P。P当前TASK_RUNNING且非idle，need_resched=false，旧vruntime3、time_slice10、last_jiffies100；K/Q/R vruntime=0/20/40。

| 条件及后果 | jiffies=103 | jiffies=110 |
|---|---|---|
| used_jiffies | 3 | 10 |
| used>=time_slice | false | true（含等号） |
| 后续state检查 | false值被求出 | 前一OR为真，**不求值** |
| && count>0 | 前一OR为假，**不求值** | 求值=true |
| 摘首/回插 | 都不进行 | 摘K, count3→2；把P按旧vruntime3插在Q20之前，count2→3 |
| P的vruntime | 3未变化 | 回插**以后**计费3+10=13 |
| last_jiffies | 100 | 110 |
| 末队列 | H-K-Q-R-H,count3 | H-P-Q-R-H,count3 |
| return与rq.curr | P、P | K、K |

103时K虽然可运行，仍未被当前picker取走；110时picker取走K，设置rq.curr=K，K节点摘后自连，但**实际切换/首条指令/返回用户态没有证据**。对源码表达式采用C短路求值，不把未求值的门伪造为true/false现场读取。证据 E136-027至E136-035。

## 完成边界

全部结果为固定time版本与合同指定输入的静态模型；涉及用户库/ABI、真实父子先后、真实调度、并发锁和被注释功能的实现均未获运行证明。完整本地P9、预处理/编译/链接、内核程序或脚本、Graphviz、QEMU、实机、真实PID/时间/调度均 **NOT_RUN**。六件一旦远端通过G04，此执行身份只交回C09以Pro独立验收和同轮发布下一包，并停止写入。
