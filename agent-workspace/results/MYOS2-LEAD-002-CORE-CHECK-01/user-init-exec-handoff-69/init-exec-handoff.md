---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INIT-EXEC-HANDOFF-69
record_type: bounded_initial_exec_handoff_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-08
produced_by: GPT-6 (model self-reported; not backend attested)
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
initial_exec_head_short12: 4d45fa939fa1
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
D00: NOT_APPLICABLE
runtime_evidence: NOT_RUN
---

# 第69批：首次exec桥接、返回门与寄存器帧消费

本包的判断仅成立于固定`time@a039d9803ade`源码和明确的前置对象、ABI与控制流假设。不把`kernel_execve`函数存在、返回非负值、ELF帧字段写入、ASM执行源文本的跳转、CPU接纳SYSRET状态以及真实用户进程开始执行合并为同一事实；后四层均没有编译或运行认证。固定主线的R67C-S01已闭合R67-01；67组合包与68均`PASS_PENDING_LOCAL`（68必须合读R68-S01/S02/S03）。本批无D00，只完成A/B/C并待C05独立验收。

## G01/A：桥接前的task/mm/pt_regs身份

`kernel_init`所读直接body在`myos_switch_to_root_disk()`之后调用`kjmp_to_doexecve()`，若该调用返回源码随后`while(true)`；没有认定此前文件系统步骤必定成功（A10/A11）。固定`arch_task.c::kjmp_to_doexecve`中`curr=current`、`curr_ptregs=task_pt_regs(curr)`发生在**调用exec之前**，随后清`curr->flags`中的PF_KTHREAD位（A02/A03/A17）。`current`是`get_current()`宏，所读函数取perCPU`pcpu_hot.current_task`（A13/A14）；`task_pt_regs`以task栈来源和THREAD_SIZE作地址计算（A15），`current_pt_regs()`另将其用于当前任务（A16）。因此后续ELF使用的`current_pt_regs()`与桥接前保存的`curr_ptregs`若指向同一帧，需要**当前任务身份、内核栈对象/位置和它们的有效性在中途保持**；源码中的相似计算式不提供运行时对象身份或CPU调度证明。旧SB66的地址转换、大小与生命周期边界均不自动闭合。

`initd_name`为`/boot/init`，argv/envp分别是有限的NULL结尾内核指针数组（A04/A05）。源码先`curr->mm=curr->active_mm`，再调用`kernel_execve(initd_name,argv,envp)`（A06/A18）；task结构里确有独立`mm`、`active_mm`指针字段（A12）。这是局部指针**赋值**，不证明mm引用所有权、CR3、每CPU/GS或地址空间激活已完成，也不能将执行前任务的PF_KTHREAD标志与内核栈硬件状态直接等同。exec开始时再次检查PF_KTHREAD的函数（B02）在当前task标志没有被其他路径重置的假设下不会因该位触发返回，但不能据此说全路径永不失败。

这次调用的整型返回值在桥接函数中**没有接收或判断**，后面先执行`hwtss_cpu0=this_cpu_ptr(&(cpu_tss_rw.x86_tss))`（A07）——这是全局指针赋值，并不是写`TSS.sp0`或发出TR加载。最后的`asm volatile`源码包含 `movq %0,%%rsp`、`jmp syscall_return_via_sysret`；输入约束是`"m"(curr_ptregs)`，输出与clobber列表均为空（A08/A09）。其预期从内存操作数读出先前保存的寄存器帧指针再切换RSP，但该文字未自行声明C函数`__noreturn`、RSP修改的编译器可见clobber，也不保证实际编译布局、目标指令或RSP安全。ASM之前任一callee若不返回或对象非法，本地后继不能宣称已达；ASM的源级分支意图亦不等于实际CPU进入用户态。

## G02/B：exec的每道失败门、格式循环和返回值如何被桥接消费

`kernel_execve`进入时有`WARN_ON_ONCE(current->flags & PF_KTHREAD)`真则`-EINVAL`返回；随后按序走`getname_kernel`、`alloc_bprm`、argv/envp计数、栈限制、文件名与环境/参数字符串复制。每道所读负值门都有相应返回/清理出口（B01–B13）。`alloc_bprm`内部先`do_open_execat`并处理文件指针错误，再申请`bprm`，设置filename/interp，并仅在`bprm_mm_init`返回0时给出bprm（B29–B33）。它们是函数之间的**条件返回接口**，文件和页引用的全部实际生存期并未逐条审核。argv为空会被这条`kernel_execve`门视为EINVAL；本次桥接argv数组源文非空，但这不能抵销任意实际复制/分配失败。

调用`bprm_execve`的正/负值路径（B14–B20）先设置`curr->in_execve=1`，再把`search_binary_handler(bprm)`的返回值存入retval：负值走out，非负值清`curr->fs->in_exec`及`curr->in_execve`后返回；失败out也清相应标志再返回。同一`kernel_execve`随后在`out_free`执行`free_bprm`、`out_ret`执行`putname`，最后返回retval（B12/B13）。这并不是free_bprm的对象身份/所有权认证；其中非空bprm.file的fput与do_close_execat调用属于需要独立消费者审查的接口（B34–B36），不在本包直接签一个重复bug。

`search_binary_handler`先`prepare_binprm`并在负值时返回；随后将retval初始化为`-ENOENT`，**对formats链表中的每一个fmt依次写`retval=fmt->load_binary(bprm)`**，原本根据`point_of_no_return`或`retval!=-ENOEXEC`决定提前返回的整个代码块处于注释内，循环结束才返回最后retval（B21–B27/B39）。所以即使某个处理器`load_binary`返回0（代表它自身的一个成功状态），只要有后继处理器运行，最终retval仍取决于**后一个处理器**；这不是全体格式器状态的一致性保证。新增IE69-F01为条件风险：必须真有多格式处理器、早先处理器修改状态且后者继续执行，才讨论结果覆盖或错误状态消费；本次未证明装载列表实际有两个，也未执行格式处理器。

桥接的结果消费严格如下：

| exec路径结束状态 | `kernel_execve`可能返回 | 桥接源局部后继 |
|---|---|---|
| PF_KTHREAD门/名字/分配/参数复制失败 | 负值，或前置调用不正常返回 | 若正常返回负值，桥接**不测试负值**，继续写指针并请求汇编切换 |
| prepare_binprm或最终fmt处理失败 | 负值，清理后若正常返回 | 同上；不能从返回值推出帧已被写好 |
| 最后一个格式处理器返回非负 | 非负，清理后若正常返回 | 同上；非负也不能证明预期ELF处理器就是最后一个，或帧/CPU状态完整 |
| 清理或前置callee不返回/对象无效 | 无正常返回证明 | **止于最早失败点**，不可越过它声称发生汇编跳转 |

`kernel_execve`的返回与ELF处理器的实际字段写入是不同证据层；load_elf_binary本身在已读成功尾部把retval设置为0（C10），但不能把这一行延伸为所有处理器均返回0。也不能仅凭部分header读取的非负字节数断言内容合法或整个ELF装载成功。普通系统调用的`do_execve`存在另外入口，本包只区分接口，不重做09系统调用、25栈表及28解释器历史报告。

## G03/C：ELF写入的保存帧与SYSRET共享标签实际软件消费

固定`binfmt_elf.c`的load_elf_binary从bprm->buf取得ELF头并检魔数/类型；根据有无解释器，所读后段分别取`load_elf_interp`调整后的`elf_entry`或直接取`e_entry`并有地址失败门（C01–C06）。`create_elf_tables`负值失败门之后才设置`mm->start_stack=bprm->p`（C07/C08）。末尾`regs=current_pt_regs()`，随后`finalize_exec(bprm)`、`START_THREAD(elf_ex,regs,elf_entry,bprm->p)`，再写retval=0返回（C09/C10）。真正`START_THREAD`宏位于`linux/kernel/elf.h`，所读`#ifndef`分支把调用转到`start_thread(regs,elf_entry,bprm->p)`（C11）。已读取它包含的`asm/elf.h`核直接定义位置，但未运行预处理，故仅按**所读宏分支成立**推理。

`start_thread`将`__USER_CS`、`__USER_DS`及_ds=0送到`start_thread_common`（C17）；后者有regs与current_pt_regs不相等时WARN的源码检查及一组FS/GS/ES/DS载入调用（C12/C13）。如果这些callee正常返回且目标regs有效，它才依次写`regs->ip=new_ip`、`regs->sp=new_sp`、`regs->cs=_cs`、`regs->ss=_ss`，最后`regs->flags=X86_EFLAGS_IF|X86_EFLAGS_FIXED`（C14–C16）。源宏为flags bit9与bit1；写这些值不等于实际CPU按这些值形成CS/SS。用户段选择子在所读宏有条件布局分支，是否取到该分支也未经预处理（C37–C40）。

**本函数未直接写regs->cx、regs->r11、regs->ax、regs->orig_ax、regs->irq_nr**。这些是独立的保存字段，保留其原来的生产/变更前提；不能只因`start_thread`没写它们就断言它们任何时刻为零。曾有PF_KTHREAD新fork时清零childregs的源级路径，但中间exec与返回工作可能改变相关对象；并且`kernel_execve`的C返回值不自动变成`regs->ax`。保存结构`pt_regs_s`的原顺序见C18–C22；仓库现存各独立偏移宏见C23–C27及C44–C48，**没有**重新生成offsetof/sizeof或用编译产物确认。

SYSRET共享标签以`cli`开始，先两次交换live R11/RCX与保存的EFLAGS/RIP，继而与保存的R11/RCX，再重复与EFLAGS/RIP交换（C32–C34）。纯数值/内存槽符号模型的六次交换，若初始保存flags=F、ip=I、原存R11=U、原存RCX=V，进入时live R11=X、live RCX=Y：

| 源级完成的交换 | 活跃R11/RCX | 被改的保存槽 |
|---|---|---|
| 第1和第2次 | F / I | EFLAGS=X，RIP=Y |
| 第3和第4次 | U / V | R11=F，RCX=I |
| 第5和第6次 | X / Y | EFLAGS=U，RIP=V |

接下来`POP_REGS`实际弹15个8字节通用寄存器槽（C28–C31），其中活动R11从保存R11槽取到**F**、活动RCX从保存RCX槽取到**I**、活动RAX从保存ax槽取值，而不是从C函数当前retval直接读。其后RSP由内核pt_regs起点加15×8=120字节（已到IRQ_NR），再`addq $0x28`跳过5个8字节槽到偏移160（保存的RSP槽），`popq %rsp`取出保存的用户SP，随后`swapgs`、`sti`、`sysretq`（C35/C36）。**这是以真实源码顺序、已有偏移常量和每槽8字节为条件的软件推演**，不代表真实机器上的SYSRET执行、安全性或ISA验证。保存的CS/SS位于帧尾，所读软件序列中并未把这两个槽直接装入段寄存器；其CPU生效条件需原始硬件规范与运行证据，不凭注释代言。

**路径区别**：普通`ret_from_fork`在相应C body有`syscall_exit_to_user_mode(regs)`活动调用，后者又直接调用用户退出工作（C42/C43）；本桥接在调用`kernel_execve`后直接请求跳至共享汇编标签，当前桥接局部没有调用该C工作。因而就**本已读局部控制段**看，两条软件调用顺序不等价；至于exec内是否另有同等信号/重启效果、是否真的漏处理用户信号，以及后继硬件用户态结果，本包不作全局断言，继续遵循09限定回执。

## 发现、模型及验收边界

新`facts-and-boundaries.yaml`包含18项固定源码/宏台账、A桥接状态矩阵、B错误/返回矩阵、C寄存器消费矩阵、16个纯JS数值/布尔模型及旧qualified_id消费链。新`deferred-findings.yaml`有IE69-F01/F02/F03三项**条件候选**和IE69-U01–U08八项证据缺口；无实测bug。DOT按真实调用、宏展开、间接处理器调用、条件返回、指针数据/局部控制和ANALYSIS_STOP分型，既不把连续函数看作彼此直接调用，也不将证据STOP当CPU停机。

A/B/C=`completed_in_scope`仅代表固定源码范围内的结构/条件分析；D00不适用。完整本地P9、偏移生成、预处理、编译、链接、仓库C/ASM/脚本/夹具、Graphviz、QEMU与实机、CPU/GS/SYSRET/首次用户指令均NOT_RUN。旧成果、评论、PR元数据和内核未修改；本次仅原PR17新69目录六件及本人一条正式评论可写。完成后STOP_WRITING，交MYOS2-A-C05独立验收，不自签69通过或开始70。
