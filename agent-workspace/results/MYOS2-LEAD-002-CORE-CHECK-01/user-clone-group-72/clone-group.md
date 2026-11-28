---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-CLONE-GROUP-72
record_type: bounded_clone_group_signal_ownership_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported; backend not attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-11
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
execution_parent_short12: 5f3b00d9436f
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00: NOT_APPLICABLE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第72批：进程创建入口、信号对象所有权与线程组可见性

所有结论仅按`time@a039d9803ade`固定源码的实际活动语句成立。一个结构字段的存在或被复制、clone flags通过局部门、PID数字已写入、task挂到链表、调用`wake_up_new_task`与用户程序实际能创建并运行多线程，是六层不同证据。接口返回0、原子计数发生改变、缓存释放函数被调用、线程组结构完整也不能混为一事。没有编译、预处理、仓库代码运行、CPU/QEMU/实机试验。

本批承接103号固定任务，71正式审查的R71-S01/S02/S03、70的R70-S01/S02/S03和PID11§3、回滚44§2–4、FS45§2–3；不重做71，不自关EC71、UV-14或历史问题。D00不适用。

## G01/A — 真正的调用者和clone标志门

`sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)`确有活动定义，构造的`kclone_args_s`初始化列表只显式写`exit_signal=SIGCHLD`，随后`return kernel_clone(&args)`（A01/A02）。在常规C结构聚合初始化规则下，未写的flags与kthread等成员初始为零；不是从已定义`CLONE_THREAD`宏存在就给用户fork暗中附上线程标志。源码里紧邻fork的一段计数循环存在，但本任务不将其执行成本或行为作为fork/clone机制完整鉴定。

`kernel_thread(fn,arg,name,flags)`是另一真实生产者：`args.flags=((lower_32_bits(flags)|CLONE_VM|CLONE_UNTRACED)&~CSIGNAL)`，而`args.exit_signal=(lower_32_bits(flags)&CSIGNAL)`；同时赋`fn/fn_arg/name/kthread=1`，并把fn和arg的整数表示放进stack/stack_size（A08–A12）。这一路**不自动添加**CLONE_THREAD或CLONE_SIGHAND；其内部任务首帧仍服从67/68先前限制。传入任意`kclone_args_s`的内部调用者及真实外部调用者不能等同于fork/kernel_thread两个已读生产者。

x86-64当前所读`syscall_64.c::x64_sys_call`以`__SYSCALL`宏从`asm/syscalls_64.h`展开分派（A30/A31），同源定义的`sys_call_table`也消费这一头文件（A32）。本轮直接读该表全部80行：有活动fork映射、vfork映射为注释，所读列表没有clone映射；这是**该固定表**的有限观察，而不是经预处理/链接或全仓穷举后断言任何用户态clone路径不存在。宏条目文本为背景上下文，不把宏调用伪称`__SYSCALL`宏定义的源码引文。

固定`kernel_clone_args`结构、别名和`CSIGNAL/CLONE_VM/CLONE_SIGHAND/CLONE_PIDFD/CLONE_VFORK/CLONE_PARENT/CLONE_THREAD/CLONE_NEWUSER/CLONE_NEWPID`各独立宏均已直接读（A03–A07/A33–A41）。clone参数经过`kernel_clone`的PIDFD与PARENT_SETTID同址拒绝门，再到`copy_process`。其真实早退条件按最早门分别是：NEWNS+FS、NEWUSER+FS；THREAD不带SIGHAND；SIGHAND不带VM；CLONE_PARENT加当前signal.UNKILLABLE；THREAD与NEWUSER/NEWPID或子PID namespace不一致；PIDFD与DETACHED/THREAD组合（A14/A20–A28）。满足THREAD→SIGHAND→VM是必要条件，不是能构造正确线程、共享signal、拥有有效nsproxy或创建完成的充分条件。

`kernel_clone`在`copy_process`返回ERR_PTR后直接取PTR_ERR，成功后取得PID对象和nr，发出`wake_up_new_task`，归还临时PID引用后返回nr（A15–A19）。VFORK分支有完成对象初始化，但其等待工作在所读body中为注释（A16/A18）；不因此说父任务确实等待到了子任务。一个PID数值可返回并不证明子线程实际运行或完成线程组发布。

## G02/B — signal与sighand逐对象持有账

`dup_task_struct(current,node)`的有效路径在分配新task后执行`*tsk=*orig`（B01/B02），因此最初的`p->signal`、`p->sighand`及parent等可能只是**继承的指针数值**。这个赋值本身不增加signal_s.sigcnt或sighand_s.count，不证明任何真实独立持有。静态`init_task`明确持有`init_signals`和`init_sighand`，静态初值中分别见`nr_threads=1`与sighand.count=1（B30–B32）；后续派生task要合读调用者current/orig同一对象、没有中途改写和对象仍有效。

| 条件 | 关键活动写入 | 子任务成员来源与限制 |
|---|---|---|
| CLONE_SIGHAND | `refcount_inc(&current->sighand->count)`后return0（B06） | 未显式另赋p.sighand；依赖复制得到旧同址指针，计数合法且无并发写者 |
| 非CLONE_SIGHAND | `kmem_cache_alloc`新sighand、count=1、旧action拷贝（B07–B10） | 安装`tsk->sighand=sig`的语句为注释；返回0不能当“新对象已发布” |
| CLONE_THREAD下copy_signal | 直接return0（B13） | p.signal沿结构复制继承旧指针；此函数没有新的sigcnt/nr_threads增加 |
| 非CLONE_THREAD下copy_signal | `kmem_cache_zalloc`先写`tsk->signal=sig`，成功后nr_threads=1、sigcnt=1、curr_target=tsk、shared_pending初始化、rlim复制（B14–B19） | 赋值先于NULL检查；分配失败时p.signal可能被写NULL，区别于sighand分配成功却不发布 |

这里的`signal_s`包含`sigcnt/nr_threads/curr_target/shared_pending/pids/rlim`，`sighand_s`包含另外的`count/action/siglock`；`task.pending`又是独立的待处理信号对象（B20–B29）。所读`init_sigpending`的活动体只调用列表初始化，清信号位集合的语句为注释（B26）；非线程新signal来自零分配，task.pending则来自任务结构整体赋值后再调用`init_sigpending(&p->pending)`（B33）。不能将这两次调用合成对同一队列的初始化，或从helper名称推出两套信号位都被重新清零。

**旧根因已查证，不重复登记：** `CORE-USER-VFS-BASELINE-05::UV-14`原始deferred明确为`needs_evidence`，描述非共享`copy_sighand`分配了新对象但没有安装，稍后失败通过`__cleanup_sighand(p->sighand)`减到继承的静态sighand引用，可能把静态对象传给cache释放请求；也包括未发布新sighand的后果。本72只补struct复制的真实来源、共享计数分支与标签链，不报告新UV号，不声称已复现静态对象free。

`copy_signal`若非THREAD且分配成功，它的`signal_s`是另一个**确实写到tsk.signal**的新对象；晚期错误链的`free_signal_struct(p->signal)`是注释，因而这一对象的本地清理请求不见活动实现（C38）。CG72-F02仅把这种新的signal_s所有权缺口列为条件候选，与旧UV-14针对sighand_s的根因对象不同；实际错误可达性、其它释放者与物理泄漏均未证。

## G03/C — 组身份分类、PID挂接和失败清理

实际`copy_process`先在有界成功前置之后以`p->pid=pid_nr(pid_struct)`得到新PID，若CLONE_THREAD则写`p->tgid=current->tgid`，否则写`p->tgid=p->pid`（C01/C02）。`p->group_leader=p`和`p->group_leader=current->group_leader`都是注释，且在本task结构所读定义中`group_leader`成员本身也被注释（C03/C13）。因此tgid数值一致不等于拥有一个有效group_leader对象指针或线程列表。

父关系也不同：CLONE_PARENT或THREAD分支给`p->real_parent=current->real_parent`，其余给current；分支中`p->exit_signal=-1`和`p->exit_signal=args->exit_signal`等赋值均为注释（C04–C06），没有证据可以无条件把新的exit_signal等同于入参。直接定义的`thread_group_leader(p)`仅返回`p->exit_signal >= 0`（C09），**不是**比较p.pid和p.tgid，也不直接检查CLONE_THREAD。若在合法假设的THREAD调用中继承到非负exit_signal并且此前callee没有改写，后续便可能把它送入leader发布分支；这是CG72-F01的条件问题。当前实际用户clone入口未建立，且sched_fork/外部写者、有效PID/parent均未全量验证，不能宣称已实际触发。

PID链按若干相互独立步骤操作：`init_task_pid_links`初始化task的不同PIDTYPE hlist节点；`init_task_pid`在PIDTYPE_PID写task.thread_pid，其他类型写`task.signal->pids[type]`（C07/C08）；`attach_pid`才从相应来源取pid对象，把task.pid_links节点加入其pid.tasks链（C24/C25）。`copy_process`只对`p->pid != 0`进入相关发布段（C15）；若`thread_group_leader(p)`真，设置TGID/PGID/SID类别来源、把task加入real_parent.children及全局all_task，调用三个类别的attach_pid（C16–C19）。如果leader判据为假，局部的`signal.nr_threads++`、live/sigcnt增量、thread_group/thread_node链表追加**全部是注释**（C20/C21）。无论哪类，只在所见非零pid分支末才会`attach_pid(PIDTYPE_PID)`并做**全局**`nr_threads++`（C22）。全局nr_threads、signal_s.nr_threads、复制得到的signal指针、tgid和PID hlist可查性不可互相替代；实际成员链与任何外部竞争访问者都须另证。

正常控制`return p`发生在全部这些有界源码请求之后，阻断顺序落入失败标签（C23）。`kernel_clone`随后对IS_ERR进行检查、取得PID对象/数字、可条件初始化vfork完成对象，发出wake-up请求，归还PID引用并返回数字（C26–C31）。这不是“已经按CPU运行”、也不是完整IDR/namespace/任务锁与signal lifetime安全的证明，PID11§3的区分继续有效。

**失败与清理必须按标签入口**：copy_sighand失败直接去cleanup_fs，跳过显式sighand清理；copy_signal失败去cleanup_sighand；copy_mm失败进入cleanup_signal；后续namespaces/thread/alloc_pid错误分别从其实际标签向下，若中途调用正常返回才可能继续（B04/B05、C32–C40）。`bad_fork_cleanup_signal`中的`free_signal_struct`为注释，紧接着的`__cleanup_sighand(p->sighand)`为活动调用，随后才有`exit_fs`、`exit_files`、task stack与`ERR_PTR(retval)`（C37–C40）。尤其旧UV-14若在错误减引用后令对象失效，则不能越过该最早停止点假定FS/files或fork_out完成；copy_thread失败不从cleanup_thread开始，alloc_pid错误也不向上倒行经过free_pid（R44-S01/S02不变）。

这条静态链最多证明在合法任务/文件/组对象和所有前置callee正常返回条件下，哪些函数**被请求调用**；不证明释放对象为slab、最后持有者、回收线程、锁正确、PID被IDR可查，更不证明内核已实际执行成功或失败。

## 附件与整体上限

六件包括MANIFEST、本报告、机器可读facts、条件分型DOT、deferred及逐项evidence。facts记录`source_ledger`、`caller_flag_matrix`、`clone_admission_matrix`、`signal_ownership_matrix`、`group_publication_matrix`、`failure_ownership_matrix`、22个有限布尔/引用模型与原qualified_id；新deferred为CG72-F01/F02两项条件候选、CG72-U01–U08八项缺口，零实测缺陷。新直接短锚A41+B33+C40共114段，均二次固定time回源，非空、连续1–5物理行，且每段单一定义；新六件P6原始全文计数必须独立计算并与MANIFEST三数一致。

完整本地P9、预处理、编译、链接、仓库脚本/代码/旧夹具、Graphviz、QEMU、实机和动态线程组/信号递送均NOT_RUN。A/B/C仅`completed_in_scope`，总体`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。本批D00不适用。只有原PR17新`user-clone-group-72/`六件与本人唯一正式评论可写；旧文件、旧评论、PR元数据、内核冻结。完成GitHub提交和逐项远端回读后STOP_WRITING，交MYOS2-A-C05独立验收；执行者不自签72通过、不启动73。
