---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-KTHREAD-ENTRY-68
record_type: bounded_kthread_entry_analysis_with_R67_P6_disposition
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported; backend not attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
execution_base_short12: 157bf5604754
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00: documentation_disposition_submitted_pending_C05
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第68批：kernel_thread实参、kthread包装与真正启动入口

本件只以固定 `time@a039d9803ade` 源码、明确的源级条件和本轮纯文本/布尔模型为证据，不认证链接后地址、ABI寄存器值、CPU时序、真实内核线程调度与用户程序运行。历史FR67/SB66/09条件问题在限定范围内消费，不得以本包的静态无正常返回路径代替实际内核行为证明。

## D00 — R67-01仅修正P6计数与原声明

原67六件在`157bf5604754`固定对象上重新逐件全文读取，并由本轮自写的文本表达式按协议规定的左方括号紧接大写VERIFIED的**原始前缀**逐字统计，不过滤后续字符。结果为：原`evidence.md`真正带文件和符号的源码标签82个（D1、A33、B25、C23），原`facts-and-boundaries.yaml::P9_text_subchecks[5]`解释语句额外命中一次，其余四件和MANIFEST均零。原六文件实际原始总匹配数为**83**。这是当前对话对直接GitHub文本的精确自建统计，不冒称运行了旧P9工具、编译器或作者检查器。

此额外一次是说明文本而不是第83条源码锚；原MANIFEST的`verified_claims=82`、`quotes_reconfirmed=82`、`downgraded_to_inferred=0`中82可描述旧源码标签数，但**不等于原六件全文计数**。按P6将n理解为原始总数83时，旧m+k=82，不满足恒等关系。不能把旧源标签改成83、将解释句当作真实引文、或将其降级凑数。原67的MANIFEST自检/P9第5项、facts中的说明字段以及正式评论#6757793690的“82/82即全文匹配”口径均须按此替代；影响索引详见新facts.prior_return_disposition。其余82条旧源码的具体语义质量并未在本D00逐条重新复核，不将旧P6错误扩展为所有技术主干均失效。原67六件、旧正式评论、PR元数据和内核一字节不改。

本包新文件避免在所有说明字段写出会匹配保留前缀的字面序列，仅真实源码标签采用该形式；MANIFEST的三个新计数必须由**新六件全文**再次核定。D00仅`documentation_disposition_submitted_pending_C05`，不自关R67-01。第66批R66-01已由主线独立CLOSED_DOCUMENTATION_ONLY，组合PASS_PENDING_LOCAL_WITH_R66C-S01；R67-S01/S02、R66-S01/S02、R65C-S01继续生效，65/64及63/58已闭合/撤回项不重开。

## G01/A — kernel_thread到copy_thread首帧

固定fork.c::kernel_thread接收`fn(arg)`、`name`、`flags`，使用`kclone_args_s args`：
- `flags=((lower_32_bits(flags)|CLONE_VM|CLONE_UNTRACED)&~CSIGNAL)`，`exit_signal=lower_32_bits(flags)&CSIGNAL`（A01/A02）；
- `args.fn=fn`，`args.fn_arg=arg`，`args.name=name`，`args.kthread=1`（A03），并额外把函数指针和参数转整数存到`args.stack`和`args.stack_size`（A04）；
- 函数末尾`return kernel_clone(&args)`（A05）。参数类型来自真实`kernel_clone_args`结构与`kclone_args_s`别名（A06–A09/A29），`PF_KTHREAD`从本仓库宏直接取证（A28）。

**两个stack字段不能偷换：** `kernel_thread`填入`args.stack=(unsigned long)fn`与`args.stack_size=(unsigned long)arg`并不说明它们变成真正的用户RSP或内核任务栈地址。`copy_process`在新task复制成功后先清PF_KTHREAD、再依args.kthread置位，必要时调用set_kthread_struct并检查失败，然后调用copy_thread（A17–A20）。`copy_thread`的确把args.stack读到局部sp（A22），但若PF_KTHREAD为真，它先清零childregs并以`kthread_frame_init(frame,args.fn,args.fn_arg)`设置bx/r12后**提前return 0**（A25/A26）；普通用户分支的`if (sp) childregs->sp=sp`在该早退之后（A27），不能倒用它解释PF_KTHREAD的栈。首帧仍受上一批确认的线程栈分配、地址/对齐与ret_addr合法性前提限制。

`kernel_clone`在已读函数内有PIDFD参数冲突的有限EINVAL门，`copy_process`失败经IS_ERR/PTR_ERR返回；成功源路径取pid/nr、经`wake_up_new_task(p)`再返回nr（A10–A16）。返回正PID或发出wakeup调用，**不证明新任务已实际被调度、执行到了首帧fn、完成创建通知或执行了业务回调**。在所读`kernel_thread`函数体内没有直接if(!fn)门，然而这不是整个仓库任何调用者都可传NULL的证明；本包实际读取的具名启动调用者另以非空符号为fn（见C）。

## G02/B — 通用kthread包装层和业务threadfn的不同身份

真实请求`kthd_create_info_s`持有`threadfn/data/full_name/result/done/list`，而任务的`kthread_s`持有`flags/threadfn/data/exited`等独立成员（B01–B06）。`__kthread_create_on_node`分配请求，写入外层提交的**业务函数**`threadfn`和data，建立栈上done、格式化名称，发布请求队列并调用`wake_up_process(kthreadd_task)`，再通过活动`wait_for_completion(&done)`读取`create->result`并清理请求（B26–B32）。若分配或名称失败则提前返回错误；请求排队和通知协议并未进行并发或内存生命周期完整验证。

`kthreadd`在自身循环里取出create请求，调用`create_kthread(create)`（B35–B38）。该函数真正调用的是`kernel_thread(kthread,create,create->full_name,CLONE_FS|CLONE_FILES|SIGCHLD)`，即**首帧fn为包装器kthread、fn_arg为create指针**（B23/C25），不是外层业务threadfn。调用失败时只按可见错误门写create->result=ERR_PTR(pid)、complete(done)（B24/B25）；`copy_process`对PF_KTHREAD的`set_kthread_struct`还将kthread包装体分配并绑定到task.worker_private（A19/B09–B11）。这些均为不同对象，不可只用一个“fn”名把包装器和业务层合并。

新任务首帧经67受限的ret_from_fork第一次调用把包装器`kthread(_create)`启动后：
1. 包装器立即把`create->threadfn/create->data`保存到局部变量，读取`to_kthread(current)`，再`xchg(&create->done,NULL)`（B14–B16）。若done为空，局部释放create与name，走`kthread_exit(-EINTR)`，正常不继续业务函数（B17）。
2. 若done非空，保存`self->full_name/threadfn/data`，把当前任务状态置为TASK_UNINTERRUPTIBLE，`create->result=current`，然后禁止抢占、`complete(done)`、调用`schedule_preempt_disabled()`并恢复抢占（B18–B20）。**完成通知只说明包装器到达创建交接点，不等于业务threadfn已经启动。** 直到调度接口正常恢复，才可能进入后继STOP门。
3. 先设ret=-EINTR；仅当SHOULD_STOP位没有设置才执行`ret=threadfn(data)`，这是**包装器内部的业务调用**，并非首帧那次`kthread(_create)`调用（B21）。若业务函数不返回，就停在它；若正常返回、或STOP位为真跳过它，后继是`kthread_exit(ret)`（B22）。`self->threadfn/data`写入并不是此处间接调用的表达式，实际调用使用前面复制的局部threadfn/data。

终止最近一层：`kthread_exit`把result写到包装体后调用`do_exit(0)`（B12）；所读do_exit体最后调用`do_task_dead()`，后者设置TASK_DEAD、尝试`__schedule`、其后BUG及fallback `for(;;)`（C28–C33）。因此**在所读有效对象和正常终止语义条件下，这条链没有源码级正常return到ret_from_fork**，但不将声明`__noreturn`单独当作真实CPU/资源清理完成的证明，也不推导所有清理callee都正确。包装层的业务threadfn可以返回，不能据此推出外层包装器也会正常返回。

## G03/C — 真正直接启动者与FR67-F01的适用范围

`rest_init`明确给`kernel_thread`传入两个非空函数符号：`kernel_init`和`kthreadd`（C02/C04）。它查pid对应task、配置启动属性并设置`kthreadd_task`，通知`kthreadd_done`后进入schedule_preempt_disabled/cpu_startup_entry（C01/C03–C06）。在本`rest_init`函数体内，没有看到对两个kernel_thread返回负pid的本地判断再用这些task指针；只有创建失败且查找返回无效对象等前提才构成KT68-F01的条件风险，不宣称真实崩溃或所有调用者都不检查。

`kernel_init`先等待`kthreadd_done`，依次做所读初始化、设备队列和root-disk切换，再`kjmp_to_doexecve()`，其后为活动`while(true)`，文本`return 1`在循环之后（C07–C13）。`kjmp_to_doexecve`按其直接body清PF_KTHREAD、设置mm并调用kernel_execve，随后内联汇编源码请求更换RSP并跳到`syscall_return_via_sysret`标签（C14–C17）。若汇编跳出成立，就不按普通C返回；即使`kjmp`以某种方式返回，`kernel_init`本地while仍不产生正常return。**这不证明exec/CPU用户态已经成功**，也不扩审ELF、根目录或内存映射。

`kthreadd`是具有`for(;;)`的创建守护循环，空队列时schedule，非空时逐一从队列取件并create_kthread（C18–C23）。源码尾确有`return 0`，但在所读无break的无限循环之后，不是可据其文字位置认定的正常可达出口（C24）。正常执行循环并不返回到67的汇编第二调用点。

通用创建路径中的首帧fn又是包装器`kthread`；done缺失、STOP跳过、业务函数正常返回三条本地路径都经过上述`kthread_exit→do_exit→do_task_dead`终止链；业务不返回也无法向外正常返回。结合所读三种具名入口，**FR67-F01 的第二个汇编`call *rbx`所需的“第一层fn正常返回”在这些选定入口下都没有源级正常返回证据**。这收窄了旧FR67-F01的适用场景，不关闭其`needs_evidence`：更广的kernel_thread调用者、异常控制、ABI/活跃RBX/R12保全、运行时终止与真实任务的能力没有全量审查。不能把`task.flags`、保存帧bx/r12、当前活跃RBX/R12和pt_regs内同名字段混作一个身份，也不能凭函数地址同值认证实际两次业务回调。

另一项边界是`FR67-F03`假设首帧fn为NULL：当前读到的rest_init两个入口及通用create_kthread传入的**都是非空符号**，所以不能拿这三个调用者当NULL反例；但既未完整审所有外部调用者，也未证明全局禁NULL。业务threadfn能否为NULL是另一层条件问题KT68-F02，不把它误编号成“首帧NULL”。新候选仅为KT68-F01/F02的条件记录，另有8个KT68-Uxx证据缺口，详见deferred。

## 交付上限和不可穿越门

新facts记录本次真实旧67计数方法、12个固定源码台账、entry_argument_matrix、wrapper_callback_matrix、return_condition_matrix、16个纯布尔/整数模型及原FR67/SB66/09限定。关系图分真实CALL、函数指针数据、条件返回、局部控制、尾跳/CPU终点与ANALYSIS_STOP；图中STOP意为证据停止，不是实机停机。新引文共104条，只有新证据文件使用保留的源码标签前缀；新MANIFEST三数必须与新六件实际全文计数相符。
完整本地P9、源码预处理/编译/链接、偏移重生成、旧夹具、仓库C/ASM/脚本、Graphviz、QEMU与真实CPU/任务/用户程序均NOT_RUN。A/B/C=`completed_in_scope`仅代表本任务有界静态分析；D00=`documentation_disposition_submitted_pending_C05`，整体`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`。只允许PR17新增规定六件和本人正式评论，旧文件、旧评论和PR元数据不动；不自关R67-01、不自签68通过、不开始69，不合并、不修内核。
