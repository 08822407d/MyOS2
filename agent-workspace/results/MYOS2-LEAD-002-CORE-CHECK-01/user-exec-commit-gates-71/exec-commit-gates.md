---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-COMMIT-GATES-71
record_type: bounded_exec_admission_state_commit_and_failed_exit_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported; actual backend not attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-11
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
execution_parent_short12: 1058913beefe
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00: NOT_APPLICABLE
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第71批：exec线程准入、状态提交与失败返回链

本件只使用固定`time@a039d9803ade`下直接读取的源码、项目现有的有界主线回执和独立纯文字/布尔模型。明确分开“写入一个标志位”“某个C函数正常返回”“线程组已独占”“MM或文件表真正切换”“返回码已被消费”“致命信号已执行”“用户代码实际运行”。最后六者不能因代码中的注释、函数名称或一段调用被默认认证。内核源码/脚本/旧夹具、预处理、编译、链接、QEMU及实机均NOT_RUN。

## 上游消费与G00范围

固定102号、R69C-S01闭合与第70批独立验收R70-S01/S02/S03和检查点均已读取。第69批R69-01=CLOSED_DOCUMENTATION_ONLY，69组合材料及70均仅PASS_PENDING_LOCAL；本批没有D00返工，不改任何旧件。R70-S01中的真正`mm_alloc`生产者、R70-S02的`old==new`时get/put相抵以及注释中的`__mmput`清理、R70-S03的CALLER_SEQUENCE图语义保持；并未把源码返回0等同完整引用回滚。历史R43-S01/S02、R15-S01/S02和R45-S01/S02已定点阅读，09文档闭合回执在信号返回消费中仅按原限定使用。

## G01/A — de_thread和真正的提交准入

`load_elf_binary`在所读路径调用`begin_new_exec(bprm)`，返回非零才goto`out_free_dentry`（A18/C28）。`begin_new_exec`活动起始动作先写`bprm->point_of_no_return=true`（A13/C01/C02），**在调用`de_thread`之前**。这只是bprm状态；没有源码级配套的“设置该位就必定发出致命信号或不可返回”操作。

完整读取`exec.c::de_thread`确认：`thread_group_empty`判断、设置`group_exec_task`、`zap_other_threads`、notify_count等待、schedule、leader退出与PID/TGID转移等均是注释文本（A02–A08）。活动正常路径经过`no_thread_group:`并执行`return 0`（A09）。源码还保留`killed:`标签和`return -EAGAIN`（A10/A11），但这个函数内现存将执行流转到killed的goto均被注释；因此不能把这个标签当作实际可达的致命信号/等待失败结果，更不能把包含它的假设笛卡尔案例冒充可达任务。

固定`errno-base.h::ENOERR`的定义就是`0`（A12），所以`begin_new_exec`里的`retval=de_thread(me); if(retval != -ENOERR) goto out`（A14）对上述正常返回0的比较为false，继续后继调用。**可以确认的是局部准入返回门通过，不是其他线程已经退出、leader/PID关系已经正确转换或调度同步完成。** 如果实际任务本来单线程、对象有效且上游正常，这个缺失动作未必导致这一次执行出错；倘若仍有其它组员且后续exec跨线程共享状态，则是`EC71-F01`的条件候选。真实线程组规模与任何外部隔离协议未全量核，不能宣布在运行中确实发生冲突。

随后三个可见门依次为`unshare_files`非零返回则out（A15）、`set_mm_exe_file`非零则out（A16）、`exec_mmap`非零则out，正常成功后才写`bprm->mm=NULL`（A17）。`set_mm_exe_file`在所选合法正常体的末尾返回0，`exec_mmap`所读正常old_mm两分支也都返回0（B42–B45）；不能为了补一个后提交失败例，凭空假定它们在有效对象与正常callee条件下会生产一个非零结果。它们仍有引用、锁、BUG_ON、MM激活及不可返回前提，遇到未闭合callee先停止。真实有条件的**更晚失败门**是have_execfd分支`get_unused_fd_flags(0)`的负值跳转（A19），下文C再分层。

## G02/B — 文件表、任务私有字段与共享fs的提交账本

`fork.c::unshare_fd`仅在带CLONE_FILES、当前files指针非NULL且`atomic_read(fd->refcount)>1`时，才调用`dup_fd(fd,max_fds,&error)`，结果NULL则返回error，否则函数在末尾返回0（B01–B03）。调用者`unshare_files`把本地`copy`初始化为NULL，调用`unshare_fd(CLONE_FILES,NR_OPEN_MAX,&copy)`，若error非零**或copy仍为NULL**就直接返回该error；仅在真正拿到非NULL copy后，它才保存old、锁内写`task->files=copy`、解锁、`put_files_struct(old)`并返回0（B04–B07）。

必须分别解释三种路径：

| 对象与来源前提 | 新表对象 | task.files变化 | 正常返回 |
|---|---|---|---|
| fd有效且引用计数不超过1 | 没有尝试dup | 没有本地替换 | 0（无需复制的语义，只在引用协议有效时） |
| fd有效、计数大于1且dup成功 | 新建files_struct，初值refcount=1 | 替换为新表后put旧表 | 0（复制成功的语义） |
| fd有效、计数大于1且dup上层分配失败 | NULL | 本地不替换 | error（例如-ENOMEM） |

未持有有效files对象、refcount为0/负值、并发更改或`dup_fd`内部每槽分配NULL时，不应以第三行的普通错误返回概括所有失败。`dup_fd`确实建立新表、设置refcount=1，按max_fds遍历源fd_array，为非空旧槽单独分配新file_s并按值复制（B08–B15）。这里内层`kzalloc`后缺少检查就解引用的旧问题已由43主线限制，**不是**本71新报一个重复根因；复制对象与底层file引用、f_path持有关系继续服从R43-S01/S02。旧表`put_files_struct`是表级计数归零后才遍历filp_close及提交缓存释放请求（B16–B18），不证明完整文件生命周期或物理回收。

`task_struct`包含独立`fs`与`files`两个指针，`fs_struct`的`users`为普通整数并包含另一个`in_exec`字段，files_struct则有自己的原子refcount（B19/B23/B37）。在`copy_fs(CLONE_FS)`中，活动检查`fs->in_exec`若非零则返回-EAGAIN，否则`fs->users++`（B20/B21）；`copy_fs_struct`新对象把`in_exec`初始化为0（B22）。它们与task私有的`in_execve`**绝不是同一个存储位置**。

`bprm_execve`真正活动的`curr->in_execve=1`在处理器分派前（B24/C07）；处理器成功和错误出口都显式`curr->fs->in_exec=0`和`curr->in_execve=0`（B25/B26、C09/C13）。在这条已读`bprm_execve`源体内没有找到相应的`curr->fs->in_exec=1`操作；这个**有限否定**不能推断全仓任何写者都不存在，或假定同步模型一定有缺陷。与copy_fs的非零拒绝门合读，这构成`EC71-F03`的条件生产者/并发缺口；仍需实际共享fs关系、并行CLONE_FS调用和更广的写者审查。

提交过MM门之后，`begin_new_exec`活动地清`PF_RANDOMIZE/PF_FORKNOEXEC/PF_NOFREEZE/PF_NO_SETAFFINITY`，把`sas_ss_sp`、`sas_ss_size`设0，并经`__set_task_comm`在task_lock内设置comm（B27、B29–B31、B38/B39）。`do_close_on_exec(me->files)`与`flush_signal_handlers(me,0)`在本源体是注释（B28/B32），不能作为已完成的fd-CLOEXEC或信号处理重置动作，也不能因此说全仓绝无其它等效机制。之后`setup_new_exec`调用`arch_pick_mmap_layout`，正常返回后写mm.task_size（B33/B34）；`finalize_exec`可见的RLIMIT操作被注释（B35）。`exec_mmap`直接更新task.active_mm和task.mm，并调用activate_mm（B40/B41），旧MM归还分支均带对象/计数前提（B42/B43），而`bprm.mm=NULL`是在`begin_new_exec`等到`exec_mmap`正常返回后执行；不能把这一步误归属给exec_mmap，也不能把指针变更当实际CPU CR3已经成功切换。

## G03/C — 错误门、已发生副作用与两类调用者

`linux_binprm`的`point_of_no_return`只是结构体位字段（C01）。`begin_new_exec`把它置true之后，`have_execfd`路径仍可能因为`get_unused_fd_flags(0)<0`跳到`out_unlock`，其中解锁语句也为注释，然后`out:`直接`return retval`（A19–A22、C03–C05）。进入这个门需要：此前de_thread、unshare_files、set_mm_exe_file、exec_mmap及对task flags/comm的操作均按有效对象与正常返回前提完成，而且实际存在have_execfd与fd分配失败。此时bprm.mm已经置NULL、task.mm/active_mm可能已转交、task flags/备用信号栈/comm可能已改变。所读局部没有把这些状态原样全部恢复；但这不能证明真正分配器一定失败，或CPU真的返回了不一致的用户态。

更直接的后续失败来源是`load_elf_binary`在`begin_new_exec`成功后调用`setup_new_exec`，再检查`setup_arg_pages`负返回，及后续`total_mapping_size`无效、`elf_load` BAD_ADDR的错误门（C28–C32）。这些分支走ELF的`out_free_dentry`及资源清理；其后是否成为`bprm_execve`最终负返回，还受`search_binary_handler`格式列表是否继续枚举、实际最终返回值和对象有效性限制（IE69-F01）。**一次局部错误跳转不等于完成了所有资源回滚，也不等于原程序自动安全恢复。**

`bprm_execve`先设置task.in_execve（C07），接`search_binary_handler`返回负值时goto`out`（C08）。失败out附近的“如果point_of_no_return则force_fatal_sig(SIGSEGV)”完整条件语句在本快照中都是注释（C11/C12）；活动出口仅清fs.in_exec、task.in_execve并返回retval（C13）。非负分支也清这两个字段再返回retval（C09/C10）。故`point_of_no_return=true`并不能证明本执行实现已强制终止不完整exec；`EC71-F02`只记录在后续有真实负返回且上下游正常继续时的**条件风险**，不是实测严重故障或宣称用户一定恢复运行。

普通调用链有明确的`MYOS_SYSCALL_DEFINE3(execve)→do_execve→do_execveat_common`源级函数转发（C24/C25）。其中`do_execveat_common`先处理filename错误及PF_NPROC_EXCEEDED，然后在bprm_execve返回后进入free_bprm/putname，最后`return retval`（C14–C19）。`kernel_execve`独立有PF_KTHREAD与bprm获取错误门，也在bprm_execve正常返回后释放bprm和filename，再返回retval（C20–C23）。**这两条有显式C返回码消费，不代表下游硬件SYSRET或信号分流已通过认证。**

初始桥`kjmp_to_doexecve`却只是调用`kernel_execve(initd_name,argv,envp)`而没有读取整型结果，后续本地源文本请求跳到共享SYSRET标签（C26/C27）。这属于既有`IE69-F02`的根因，本批只增加调用者对照，绝不换一个新编号重复报告。任何前置无效指针、CPU寄存器/栈条件、返回工作和信号处理仍须在最早未闭合处停下；09的限定资料未授权改写真实信号/重启行为。

## 有限模型、延期项和交付上限

事实件包含`source_ledger`（11份真实固定文件）、`admission_gate_matrix`、`task_state_matrix`、`post_commit_failure_matrix`、`caller_return_matrix`、25个纯布尔/引用整数模型、旧qualified ID挂靠。模型分别将de_thread真实返回0与不可达的任意非零输入分开，将files引用计数0列为无合法持有者前提，将post-MM未完成却假设execfd提交的组合标为不可达。所有数值只在正计数/合法对象/前置callee正常返回条件下解释，不能把非法中间态继续算出确定errno。

新延期件列`EC71-F01/F02/F03`三个条件候选与`EC71-U01–U08`八项缺口；零实测内核bug，不闭合EO70/IE69/FT43/MC15/FS45/09旧问题。DOT分明确的真实CALL、间接回调、DATA、调用者控制顺序、源码注释及ANALYSIS_STOP；Graphviz NOT_RUN。新证据为A22+B45+C36共103条非空、连续1–5物理行的真正单定义源码锚，均已二次按固定time回源；其标签数和全六件原始文本计数要在G04独立核对。

执行状态仅`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`，A/B/C=`completed_in_scope`（源码限定），D00不适用。只可在原PR17结果目录`user-exec-commit-gates-71/`新增六件及本人唯一正式评论；不改内核、旧件/旧评论/PR元数据，不编译/运行、不自签71通过、不启动72。正式评论并远端回读后STOP_WRITING，交MYOS2-A-C05独立审查。
