---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-RETURN-SIGNAL-09
record_type: user_return_signal_source_guide
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-29"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 8983a9235c8a；主线 agent/MYOS2-LEAD-002 14ecef9fc596；master e8e60cd19540（均为短 SHA）"
inputs_read: "见 evidence.md §2"
status: final_for_user_return_signal_09
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
---

# 系统调用完成以后怎样回到用户程序：返回工作、信号与重启（CORE-USER-RETURN-SIGNAL-09）

本文接在 07 的 `user-boundaries-07/user-entry-console.md`（返回帧）与 08 的 `user-syscall-stdio-08/user-syscall-stdio.md`（分派）之后，回答一个问题：处理函数算出返回值以后，到用户程序真正拿到它之前，内核还会对寄存器帧做什么。所有结论来自 `time a039d9803ade` 的源码文本，**没有构建、预处理，也没有运行内核、用户程序或 QEMU**（只运行了只读 git 检索与文本核对脚本）；运行层与硬件语义一律 NOT_RUN。

机器可读的事实在 `facts-and-boundaries.yaml`，调用与数据边的图在 `return-paths.dot`（与事实同一组 ID）；普通问题在 `deferred-findings.yaml`（RS-01…RS-14，另有对 07 UB-09 的两条证据追加与三条文档勘误 ER-01…ER-03），当前都不需要 Owner 决策。08 两项退回的处置在 `stdio-08-disposition.md`。

**编号约定。** 不带前缀的 RS、ER、AD、QD、SD、RT、FF、RR、FN、ED、NVR 指本批；UV、GAP-Uxx、A-Cx-xx 指 05，UB、AB/QB/SB、FP、NVB 指 07，US、AS/QS/SS、FL、VT-SYSTAB 指 08。引文行写成“文件:行（QD-Lxx）”的，表示该行没有单独立锚点（本批锚点上限 75），由整行精确匹配检索 QD-Lxx 复核。

## 0. 结论

1. **返回工作（D01）。** 活动链只有一条：`do_syscall_64` → `syscall_exit_to_user_mode` → `__syscall_exit_to_user_mode_work` → `exit_to_user_mode_loop(regs, 0)` → `arch_do_signal_or_restart(regs)`。原来的循环与全部标志判断都在注释里，`ti_work` 不控制任何东西；所以每次经这条尾段返回，都无条件进入信号与重启处理，没有调度门，也没有信号门。之后 `do_syscall_64` 计算的 SYSRET 判断不被使用，一律按位置落入 `syscall_return_via_sysret`（07 已记，UB-09）。`ret_from_fork` 进入同一尾段（07 FP-2）。
2. **信号设置与选取（D02）。** `rt_sigaction` 与 `rt_sigprocmask` 把处置与掩码写进 `sighand->action[]` 与 `current->blocked`；`copy_from_user` 的活动实现不做 `access_ok`（RS-11）。`sa_flags` 存入时与 `UAPI_SA_FLAGS` 相与，按头文件包含次序推断 `SA_RESTORER` 在这一步被清掉（RS-01，推断，需构建侧核实）。`get_signal` 每次都持 siglock 扫描 `pending & ~blocked`（TIF_SIGPENDING 机制两端停用，RS-04）；只有用户处理函数分支返回真；默认的终止、停止、转储动作只置 `PF_SIGNALED`，任务既不退出也不停止（RS-03）。fork 出的任务共用 `init_sighand`（RS-12），并继承父任务的私有 pending 位图（RS-13）。
3. **建帧（D02）。** `x64_setup_rt_frame` 要求 `SA_RESTORER`，否则在写任何帧字段之前返回 -EFAULT；`handle_signal` 不处理建帧失败（`signal_setup_done` 被注释），已出队的信号就此丢失；若 (int)orig_ax ≠ -1 且 ax 恰为四个重启码之一，重启改写在建帧之前已经做了，不回滚（RS-02）。建帧成功时，帧地址从用户 `sp` 减去 128 字节红区、按需换到备用栈、减去帧大小、16 字节对齐后再减 8；寄存器、掩码等写入用户帧前没有 `access_ok`，`Efault` 标号不可达（RS-08；写入出错时没有缺页修复是全内核的情况，不只在信号帧），`uc_flags` 未初始化（RS-09），不保存 FPU 状态（RS-10）；然后 `ip` 改为 handler、`sp` 改为帧、`di/si/dx` 放参数、`ax` 置 0。
4. **恢复（D02）。** `rt_sigreturn` 从 `regs->sp - 8` 取帧，恢复掩码与寄存器，把 `orig_ax` 置为 -1，返回恢复出的 `ax`，分派再把它写回 `regs->ax`；随后返回尾照样再跑一遍信号与重启处理。读用户帧没有容错（RS-06）；坏帧只 `return 0`，不发 SIGSEGV（RS-07）。
5. **重启（D03）。** “来自系统调用”的判定是 `(int)orig_ax != -1`；错误码取自 `regs->ax`。有 handler 时，RESTARTBLOCK 与 NOHAND 改为 -EINTR，ERESTARTSYS 视 `SA_RESTART` 而定，NOINTR 改为 `ax = orig_ax`、`ip -= 2`；没有 handler 时，NOHAND、SYS、NOINTR 都改为 `ax = orig_ax`、`ip -= 2`，RESTARTBLOCK 改为 `ax = 219`、`ip -= 2`。219 号在活动分派表里没有表项，按 08 的分派规则会进入缺项处理的 `while (1)`（RS-05，再次进入属推断）。按名字与字面量检索，没有活动处理函数返回这些重启码（没有活动 return 的处理函数，返回值不能排除，见 D03.5）；目前已知、能由源码确定构造的途径是越界号的 64 位 rax 恰为 -512、-513、-514、-516（追加到 07 UB-09，并以 ER-03 收窄其原句）。“`ip -= 2`、号写回”只是改写帧的意图，是否重新执行属运行层（NOT_RUN）。
6. **与 07、08 的衔接。** 08 §3.2 表中“用户看到的 rax”应读作“分派之后的 `regs->ax`”（ER-02）。“返回段装入 rax 的就是分派值”的条件见“衔接”一节的 L2（编号与 stdio-08-disposition.md §3.3 相同）：四条同时成立是充分条件，不是必要条件；以“返回尾部不写帧并到达返回段”为准，第 1、3、4 条必要，第 2 条（`get_signal` 为假）不必要，放宽为“没有成功建立信号帧（`get_signal` 为假，或建帧在改写寄存器之前失败）”后，四条合起来是充要条件；越界号的换法与只比数值时三种相等而帧已改写的情形也在 L2 写明。GAP-U04 只追加静态证据，仍为 narrowed；GAP-U10 保持 open。
7. **D00。** SR08-01、SR08-02 都接受（accept）。条件表、受影响位置与附注全文见 `stdio-08-disposition.md`，对应勘误 ER-01、ER-02；07 的同类表述由 ER-03 处理。

## 1. 建议的源码阅读顺序（任务书允许的阅读顺序，不是学习路线或课程排序）

1. `mykernel/arch/x86_64/entry/common.c::do_syscall_64` 与 `mykernel/entry/common.c` 全文：返回尾。
2. `mykernel/arch/x86_64/lock_IPC/signal/signal.c`：`arch_do_signal_or_restart`、`handle_signal`、`setup_rt_frame`、`get_sigframe`、`get_nr_restart_syscall`；`mykernel/arch/x86_64/include/asm/syscall.h` 的 `syscall_get_nr`、`syscall_get_error`。
3. `mykernel/lock_IPC/signal/signal.c`：`get_signal`、`dequeue_signal`、`dequeue_synchronous_signal`、`do_sigaction`、`sigprocmask`、`set_current_blocked`；`mykernel/lock_IPC/syscall.c` 的 `rt_sigaction`、`rt_sigprocmask`。
4. `mykernel/arch/x86_64/lock_IPC/signal/signal_64.c`：`x64_setup_rt_frame`、`restore_sigcontext`、`rt_sigreturn`。
5. `mykernel/arch/x86_64/include/asm/uaccess.h`、`uaccess_64.h`、`mykernel/include/linux/kernel/uaccess.h`：用户内存访问的活动实现。
6. 最后回到 07 的返回段（`entry_64.S::syscall_return_via_sysret`）与 08 的分派表，按“衔接”一节把几段连起来。

## D00 处置（摘要）

SR08-01（fdput 引用计数措辞的条件）与 SR08-02（分派局部的 ax 与最终返回值）都接受，处置正文、条件表、受影响位置清单与附注全文在 `stdio-08-disposition.md`；结构化记录见 `facts-and-boundaries.yaml` 的 `sr08_dispositions` 与本批勘误 ER-01、ER-02。处置只追加，不改 08 原文件，也没有重跑 08 的检查器。


## D01 系统调用返回尾：从写回 ax 到 sysretq

本节看系统调用返回的最后一段：从 do_syscall_64 写 regs->ax 起，到 syscall_return_via_sysret 执行 sysretq 为止。要回答四个问题：哪些语句真的会执行；ti_work 有没有作用；返回前有没有调度门和信号门；do_syscall_64 返回的 bool 有没有人用。

入口建帧沿用 CORE-USER-BOUNDARIES-07::FP-3 与 CORE-USER-BOUNDARIES-07::SB-01，号检查与分派沿用 CORE-USER-SYSCALL-STDIO-08::entry.syscall_dispatch，这里不重做。信号内部（选择、建帧、sigreturn）见 D02，重启条件逐项见 D03。本节只读 a039d9803ade 的源码，没有构建，也没有运行（NOT_RUN）。

### D01.1 结论

1. 返回工作只有一条活动链：do_syscall_64 → syscall_exit_to_user_mode → __syscall_exit_to_user_mode_work → exit_to_user_mode_loop(regs, 0) → arch_do_signal_or_restart(regs)。之后逐层回到 do_syscall_64，做一次 SYSRET 判断，再按位置落入 syscall_return_via_sysret。
2. exit_to_user_mode_loop 原来的 while 循环和全部标志判断都在 // 注释里。活动语句只有两句：无条件的 arch_do_signal_or_restart(regs)，和 return ti_work。
3. ti_work 不控制任何东西。调用方传字面量 0，函数体只在 return 里读它一次，返回值也没人接。
4. 返回尾里没有活动的调度门，也没有活动的信号门。每次经 syscall_exit_to_user_mode 返回，都会进入信号处理；get_signal 自己的快路径也被注释，所以每次都要取一次 siglock（RS-04）。
5. do_syscall_64 返回的 bool 没有人用，返回一律走 sysretq，没有 IRET 后备。这一点 07 已登记（CORE-USER-BOUNDARIES-07::UB-09）。本节补的新证据是：15 号 rt_sigreturn 能把用户内存里的值写进这个判断要检查的字段。这条证据追加到 UB-09，不另立条目。
6. ret_from_fork 也调用 syscall_exit_to_user_mode，走同一条返回尾，只是没有 SYSRET 判断（CORE-USER-BOUNDARIES-07::FP-2）。

### D01.2 活动链逐级

**分派之后。** do_syscall_64 只在 (uint)nr < 453 时写 regs->ax（CORE-USER-SYSCALL-STDIO-08::AS2-05）；整个函数只有这一处写 ax（CORE-USER-SYSCALL-STDIO-08::SS-15）。号越界（不写 ax）或处理函数返回（写回 ax）之后，都接着调用返回尾；这个调用在 if 块之外（AD1-01）。表内缺项的处理函数不返回，到不了这里（CORE-USER-BOUNDARIES-07::UB-09）。

**syscall_exit_to_user_mode。** 函数体只有一条活动语句（SD-07），就是 72 行的 __syscall_exit_to_user_mode_work(regs)（CORE-USER-BOUNDARIES-07::QB-65）。instrumentation_begin/end 和收尾的 exit_to_user_mode() 都是注释。

**__syscall_exit_to_user_mode_work。** 三个准备步骤被注释。唯一的活动语句以字面量 0 调用循环函数（SD-06，SD-05）。

[VERIFIED mykernel/entry/common.c::__syscall_exit_to_user_mode_work]
```c
	// syscall_exit_to_user_mode_prepare(regs);
	// local_irq_disable_exit_to_user();
	// exit_to_user_mode_prepare(regs);
	exit_to_user_mode_loop(regs, 0);
```

在 mykernel/ 里，exit_to_user_mode_loop 只有这一处活动调用；其余命中是 15 行的定义与注释（QD-18）。

**exit_to_user_mode_loop。** 33 行的信号判断是注释，34 行的 arch_do_signal_or_restart(regs) 因此成了无条件语句（CORE-USER-BOUNDARIES-07::AB2-12）。函数体里没有活动的 if 或 while（CORE-USER-BOUNDARIES-07::SB-14）；活动语句只有 34 行和 56 行两句（SD-04）。arch_do_signal_or_restart 在 mykernel/ 里也只有这一个调用点（QD-19）。

**arch_do_signal_or_restart。** get_signal 为真时调用 handle_signal，然后直接 return；为假时进入“没有处理函数”的重启判断（AD1-06）。get_signal 只拿到 &ksig，不拿 regs。两条分支对帧的读写见 D01.7。

**逐层返回。** 上面三层调用都以语句形式出现：syscall_exit_to_user_mode 与 __syscall_exit_to_user_mode_work 是 void 函数；exit_to_user_mode_loop 返回 ulong，但调用方 __syscall_exit_to_user_mode_work 丢弃了这个返回值。回到 do_syscall_64 后执行 SYSRET 判断（D01.6），然后返回 bool。

**汇编落点。** 175 行 call 之后只有空行和注释，下一个符号就是 syscall_return_via_sysret（CORE-USER-BOUNDARIES-07::SB-01）。

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	call		do_syscall_64		/* returns with IRQs disabled */


// R11 -> user Rflags
// RCX -> user RIP
```

返回段依次执行 cli、六次 xchg、POP_REGS、addq $0x28、popq %rsp、swapgs、sti、sysretq（CORE-USER-BOUNDARIES-07::SB-02；AD1-04，AD1-05）。

### D01.3 注释掉的代码、活动语句、与现状不符的注释

| 位置 | 类别 | 内容 |
|---|---|---|
| mykernel/entry/common.c:20–53（34 行除外） | // 注释或空行 | while 循环；local_irq_enable_exit_to_user；_TIF_NEED_RESCHED → schedule()；_TIF_UPROBE；_TIF_PATCH_PENDING；_TIF_SIGPENDING/_TIF_NOTIFY_SIGNAL 判断；_TIF_NOTIFY_RESUME；arch_exit_to_user_mode_work；local_irq_disable_exit_to_user；tick_nohz_user_enter_prepare；read_thread_flags |
| mykernel/entry/common.c:63–65、71、73–74 | // 注释 | syscall_exit_to_user_mode_prepare；local_irq_disable_exit_to_user；exit_to_user_mode_prepare；instrumentation_begin/end；exit_to_user_mode() |
| mykernel/entry/common.c:34、56、66、72 | 活动 | arch_do_signal_or_restart(regs)；return ti_work；exit_to_user_mode_loop(regs, 0)；__syscall_exit_to_user_mode_work(regs) |
| mykernel/arch/x86_64/entry/common.c:26–39、43、45、48 | // 注释或空行 | 调试打印；add_random_kstack_offset；syscall_enter_from_user_mode；instrumentation_begin/end；ni_syscall 后备；array_index_nospec；return true |
| mykernel/arch/x86_64/entry/common.c:51–55、57、61、65–72、76–80、84 | /* */ 注释 | SYSRET 各项检查的说明 |
| mykernel/arch/x86_64/entry/common.c:41–42、44、49、58–59、62–63、73–74、81–82、85 | 活动 | 号比较；写 ax；调用返回尾；四个 return false；return true |

下面这些注释描述的步骤或路径，在本树没有活动代码对应（任务书所说的 Linux-only 注释）。判断只依据本树源码。

- mykernel/entry/common.c:12 把 @ti_work 写成“调用方读到的 TIF 标志”。本树调用方传的是字面量 0，没有读任何标志。
- mykernel/entry/common.c:55 说返回值留给 arch_exit_to_user_mode()。arch_exit_to_user_mode 在 mykernel/ 里只出现在注释中（QD-26）。
- mykernel/arch/x86_64/entry/common.c:51–55 说寄存器状态不适合 SYSRET 时，改走“更慢但完整的 IRET 出口”。在两个入口文件里，“IRET”只出现在注释中（QD-27）。entry_64.S 唯一的 iretq 在中断/异常返回段 sa_entintr_retp（327 行）；系统调用返回用的是 199 行的 sysretq（QD-22）。
- entry_64.S:175 行尾注释写着“returns with IRQs disabled”。返回尾三层函数除了上面那几个调用，没有别的活动语句；arch_do_signal_or_restart 和 handle_signal 里也没有关中断语句（SD-12，SD-17）。get_signal 取锁一次、放锁一次（SD-25）；spin_lock_irq、spin_unlock_irq 分别展开为 raw_spin_lock_irq、raw_spin_unlock_irq（QD-30），后者调用 local_irq_enable 和 preempt_enable（SD-26）。推断：call 返回时中断是开着的；真正关中断的是返回段第一条 cli（CORE-USER-BOUNDARIES-07::SB-02）。
- include/linux/kernel/entry-common.h:383–411 的块注释，给 syscall_exit_to_user_mode 列了 rseq、audit、tracing、ptrace、退出循环、最终转换等步骤，并说返回时已关中断。本树这些步骤只剩信号一步。
- entry_64.S:178–183 的 // 注释写的是 sysret 的硬件约定（R11 → 用户 Rflags，RCX → 用户 RIP，用户 CS/SS 取自 IA32_STAR）。本节没有核对这些硬件语义（NVR-3）。

### D01.4 ti_work 控制什么

不控制任何东西。

调用方只有 __syscall_exit_to_user_mode_work 一处，传的是字面量 0（SD-05，QD-18）。函数体里，ti_work 的活动使用只有 56 行的 return 一处（SD-03）；重新读标志的 read_thread_flags 也是注释。

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]
```c
	// 	ti_work = read_thread_flags();
	// }

	/* Return the latest work state for arch_exit_to_user_mode() */
	return ti_work;
```

返回值被调用方丢弃（D01.2）。所以，即使只恢复 33 行的判断，ti_work 恒为 0，判断恒为假，信号处理反而一次也进不去。现状是“门被删掉”，不是“门被 0 挡住”。门与 TIF_SIGPENDING 置位端的关系见 RS-04。

### D01.5 调度门与信号门

**调度。** 在 mykernel/entry/ 和 mykernel/arch/x86_64/entry/ 里，schedule(、_TIF_NEED_RESCHED、_TIF_SIGPENDING、_TIF_NOTIFY_SIGNAL、read_thread_flags( 只出现在注释中（QD-17）。所以返回尾没有直接的调度点。在 get_signal 放锁处能看到一个间接入口：raw_spin_unlock_irq 调用 preempt_enable()（SD-26），这个宏在 preempt_count_dec_and_test() 为真时调用 preempt_schedule()（QD-25）。

[VERIFIED mykernel/sched/scheduler/preempt_macro.h::preempt_enable]
```c
	#define preempt_enable()						\
			do {									\
				barrier();							\
				if (preempt_count_dec_and_test())	\
					preempt_schedule();				\
```

raw_spin_unlock_irq 的函数体在 spinlock_smp.h 的 `#if defined(SPINLOCK_SMP_DEFINATION) || !(DEBUG)` 内；spinlock_smp.c 定义了这个宏（QD-31），所以推断两种构建用的是同一个函数体。preempt_count_dec_and_test() 何时为真，本节没有追。

**信号。** 33 行的门被注释，34 行每次都执行（CORE-USER-BOUNDARIES-07::AB2-12）。get_signal 开头的 task_sigpending 快路径也被注释（QD-20）。所以每次经 syscall_exit_to_user_mode 返回，都会在 get_signal 里取一次 siglock、最后放一次（SD-25），并做一次出队检查：先调 dequeue_synchronous_signal，它返回 0 时再调 dequeue_signal（细节见 D02）。这把锁属于所有任务共用的 init_sighand（CORE-USER-VFS-BASELINE-05::UV-14，RS-12）。TIF_SIGPENDING 的置位端不可达、也没有清除端，这一整套归入 RS-04。

**范围。** 活动代码里只有两处调用 syscall_exit_to_user_mode：do_syscall_64 和 ret_from_fork（CORE-USER-BOUNDARIES-07::QB-43）。首个程序由 kjmp_to_doexecve 直接跳到 syscall_return_via_sysret，不经过它（CORE-USER-BOUNDARIES-07::FP-1）。中断/异常返回段 sa_entintr_retp 调 excep_hwint_context 后用 iretq 返回，也不经过它（QD-22）。推断：信号只在系统调用返回和 fork 子任务第一次返回时检查。

### D01.6 do_syscall_64 的 bool

07 已登记：判断结果没人用，返回一律 sysretq，没有 IRET 后备（CORE-USER-BOUNDARIES-07::UB-09、CORE-USER-BOUNDARIES-07::SB-16、CORE-USER-BOUNDARIES-07::FP-3）。本节复核时补充以下几点。

- 判断在 syscall_exit_to_user_mode 之后执行，读到的是返回尾可能改过的帧。57–85 行里，活动语句是 58–59、62–63、73–74、81–82、85 行，其余是注释或空行。它读 cx、ip、r11、flags、cs、ss，不读 sp（AD1-02，AD1-03，SD-08，SD-09）。ip 另与 TASK_SIZE_MAX 比较，flags 另测 RF 和 TF（SD-10）。TASK_SIZE_MAX 在 page_const_arch.h 里是 (1UL << 47) − PAGE_SIZE（QD-28；源码写作 (_AC(1, UL) << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE page_const_arch.h:86（QD-L13），__VIRTUAL_MASK_SHIFT 为 47，_AC(1, UL) 在非汇编代码中展开为 (1UL)）；common.c 实际用的是不是这个定义，没有核。
- do_syscall_64 在 mykernel/ 里只有 entry_64.S:175 一个活动调用点，其余是定义、声明和 109 行的注释（QD-21）。call 之后没有读 %al/%rax 的指令（CORE-USER-BOUNDARIES-07::SB-01）。按调用约定，返回值放在 %rax（外部语义，未核）；返回段 POP_REGS 的 popq %rax 随后用帧里的 ax 覆盖它（QD-23）。
- 51–55 行注释所说的 IRET 出口，在本树的系统调用返回里没有对应分支（D01.3）。
- 新证据，作为证据追加到 CORE-USER-BOUNDARIES-07::UB-09：15 号 rt_sigreturn 是活动表项（CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB），它改写的是 current_pt_regs() 这块帧（SD-23），与入口帧是同一块（沿用 CORE-USER-BOUNDARIES-07::EB-23 的推导）。restore_sigcontext 把用户帧中 15 个通用寄存器以及 sp、ip 共 17 个字段原样写回，其中包括 cx 和 r11（SD-20）；cs、ss 取 __u16 值再或上 3（SD-21）；flags 只取 FIX_EFLAGS 所列的位，其中有 TF 和 RF（QD-29），并把 orig_ax 置为 -1（AD1-10）。它不检查取到的值，force_valid_ss 的调用被注释（SD-22）；帧地址与复制的检查见 D02（RS-06）。所以，在“同一出口没有成功建立新信号帧（get_signal 为假，或建帧在改写寄存器之前失败）、帧可读”的前提下，被检查的字段（sp 不在其中）由用户帧决定。如果同一出口建帧成功，ip、sp、cs 会被改写，flags 的 DF、RF、TF 会被清掉（RT-12、RT-13、RT-15、RT-17）。这些条件成立时 sysretq 的硬件后果没有核（NVR-3；CORE-USER-BOUNDARIES-07::NVB-4）。

### D01.7 帧字段读写

逐行记录在 return_frame_transitions 表（RT-01 至 RT-36）。按阶段归纳如下。

- **② 分派（RT-01 至 RT-05、RT-36）。** 只在 (uint)nr < 453 且处理函数返回时写 ax。(uint)nr ≥ 453 时，ax 仍是入口压入的完整用户 rax（CORE-USER-SYSCALL-STDIO-08::AS2-02）；它不一定等于按 (int)eax 理解的号，只有高 32 位不是低 32 位的符号扩展时才不同。表内缺项落到 __x64_sys_ni_syscall 的 while (1)，到不了返回尾。15 号在写 ax 之前，若 restore_sigcontext 成功（不走 badframe），已用用户帧改写 15 个通用寄存器与 sp、ip（共 17 个字段）、cs 与 ss（取用户值或上 3）以及 flags 中 FIX_EFLAGS 所列的位，并把 orig_ax 置为 -1，不写 irq_nr（SD-20、SD-21、SD-74、AD1-10）；随后 RT-01 把 return regs->ax 的同一个值再写一次（SD-24）。59 号 execve 走到 load_elf_binary 末段时，START_THREAD → start_thread_common 把同一块帧的 ip、sp、cs、ss、flags 改为 elf_entry、bprm->p、__USER_CS、__USER_DS 与 X86_EFLAGS_IF、X86_EFLAGS_FIXED 两位（RT-36；CORE-USER-BOUNDARIES-07::FP-3），随后 RT-01 写 ax = 0（推断：只注册了 ELF 一种格式）。ret_from_fork 路径进入返回尾时 ax 为 0。
- **③a 有处理函数（RT-06 至 RT-17）。** 先按重启表改 ax、ip，再建帧（SD-13，SD-14）。建帧要求 sa_flags 含 SA_RESTORER（AD1-11）。RS-01 推断，经 rt_sigaction 注册的处理函数这一位会被清掉（needs_evidence）。若成立，RT-10 至 RT-17 都不会发生，而 RT-08、RT-09 的改写会保留，已出队的信号丢失（RS-02；signal_setup_done 被注释，SD-15）。SA_RESTORER 在时，先把帧内容拷到用户信号帧（SD-18）；到达寄存器改写段后，di 放信号号、si 与 dx 放帧内地址（RT-14），ax 改为 0（AD0-15），ip 改为处理函数地址，sp 改为信号帧地址（AD0-16），cs 改为 __USER_CS，ss 不等于 __USER_DS 时经 force_valid_ss 校正（AD0-17）；建帧成功后再清 DF、RF、TF（SD-16）。
- **③b 没有处理函数（RT-18 至 RT-21）。** 要求 (int)orig_ax ≠ -1（AD1-07）。ax 为 -514、-512、-513 时写回 orig_ax，为 -516 时写 219；两种情况 ip 都减 2（AD1-08，AD1-09）。219 没有分派表项（RS-05，见 D03）。rt_sigreturn 的出口因为 orig_ax = -1，不进这里。
- **④ SYSRET 判断（RT-22 至 RT-27）。** 只读不写，结果没有人用。ret_from_fork 路径没有这一步。
- **⑤ 返回段（RT-28 至 RT-34）。** 187–192 行六次 xchg 的净效果是：R11 槽 ← flags，RCX 槽 ← ip，EFLAGS 槽 ← 原 r11 槽，RIP 槽 ← 原 cx 槽；寄存器 r11、rcx 回到 xchg 之前的值（CORE-USER-BOUNDARIES-07::AB2-16）。r11 = flags、rcx = ip，是 193 行 POP_REGS 里的 popq %r11、popq %rcx 装进去的（QD-23）。addq $0x28 跳过 irq_nr、orig_ax、RIP、CS、EFLAGS 五格，这五格不再被读；ss 槽在 popq %rsp 读的 sp 槽之上，之后也不再被读（CORE-USER-BOUNDARIES-07::VT-PTREGS）。所以 07 FP-3 列为“未消费”的 orig_ax，只是返回段汇编不读；返回尾的 C 部分会读它（RT-06、RT-18）。
- **⑥ 落地（RT-35）。** sysretq 之后用户态看到的寄存器值属硬件语义，没有核（NVR-3）。

**最终装入 rax 的值（与 D00 的 ER-02 对应）。** POP_REGS 装进 rax 的，是 ③ 之后帧里的 ax。“衔接”一节 L2 的四条（号在表内且处理函数返回、get_signal 为假、重启 switch 不命中、正常到达返回段）同时成立时，它就是分派写入的值。这是充分条件，不是必要条件：第 2 条可以放宽为“没有成功建立信号帧（get_signal 为假，或建帧在改写寄存器之前失败）”，例如缺 SA_RESTORER 时 x64_setup_rt_frame 直接返回，非 ERESTART 的 ax 保持分派值；放宽后四条合起来是返回尾部不写帧并到达返回段的充要条件（L2 末段；越界号的换法与只比数值时的例外也在那里）。有投递时，ax 可能不变，也可能被改为 -EINTR、orig_ax 或 0。sysretq 之后用户态实际拿到什么，属 NOT_RUN。

**越界号（与 ER-03 对应）。** (uint)nr ≥ 453 时分派不写 ax，帧里是完整的用户 rax。它若恰为 -512、-513、-514、-516（64 位补码），③b 会把它当作重启码，四种情况 ip 都减 2：-516 时 ax 改为 219，其余三种 ax 写回同一个值。之后是否回到同一条 syscall 指令，属 ISA 推断，逐项见 D03。这条交互作为证据追加到 CORE-USER-BOUNDARIES-07::UB-09；UB-09 中“号不小于 453 时 regs->ax 不被写，返回的是原 rax（即系统调用号）”一句要受它限定。

### D01.8 ret_from_fork 进入同一返回尾

C 层 ret_from_fork 在 79 行对所有子任务调用 syscall_exit_to_user_mode；之后汇编按 rbx 分流，用户线程 jmp 到 syscall_return_via_sysret（CORE-USER-BOUNDARIES-07::AB2-11、CORE-USER-BOUNDARIES-07::SB-03、CORE-USER-BOUNDARIES-07::EB-20、CORE-USER-BOUNDARIES-07::FP-2）。这里不重做 07 的分析，只记三点差别。

- 这条路径不经过 do_syscall_64，所以没有 SYSRET 判断（RT-22 至 RT-27 不出现）。
- 进入返回尾时 ax 为 0：用户子任务由 copy_thread 置 0，内核线程在 fn 返回后置 0（RT-05）。推断：syscall_get_error 取到 0，两处重启 switch 都不会命中。
- 内核线程 fn 返回后，返回尾处理的是 copy_thread 清零的帧；其后果仍按 CORE-USER-BOUNDARIES-07::UB-06 的未知项保留。

### D01.9 未覆盖与未知

- get_signal、出队与默认动作的内部，信号帧与 sigreturn 的内部：D02。
- 重启条件逐项与 219 的去向：D03（RS-05）。
- sysretq、swapgs、sti 的硬件语义，以及用户态最终寄存器值：NVR-3；CORE-USER-VFS-BASELINE-05::GAP-U04 的剩余部分。
- asm-offsets.h 里槽偏移的数值是否与结构一致：沿用 CORE-USER-BOUNDARIES-07::VT-PTREGS、CORE-USER-BOUNDARIES-07::VT-KFRAME 的假设。
- get_signal 放锁处会不会进入 preempt_schedule：未追。
- 运行时行为：全部 NOT_RUN（NVR-1）。

### D01.10 返回阶段表（return_frame_transitions）

下表按阶段列出实际被读写的帧字段；没有被读的字段不写“保持不变”。条件一栏给出该行成立的前提。

| 编号 | 阶段 | 字段 | 读/写 | 值 | 条件 | 证据 |
|---|---|---|---|---|---|---|
| RT-01 | ② 分派（do_syscall_64） | ax | write | x64_sys_call(regs, unr) 的返回值 | (uint)nr < 453 且处理函数返回；(uint)nr ≥ 453 时不写，ax 仍是入口压入的完整用户 rax（见 RT-19、RT-20） | CORE-USER-SYSCALL-STDIO-08::AS2-05、CORE-USER-SYSCALL-STDIO-08::SS-15、CORE-USER-SYSCALL-STDIO-08::AS2-02 |
| RT-02 | ② 分派·不返回分支（__x64_sys_ni_syscall） | orig_ax | read | 打印 orig_ax，随后 while (1)；返回尾不会到达 | (uint)nr < 453 但表中没有这个号（包括重启写回 219 后再次进入，推断） | CORE-USER-SYSCALL-STDIO-08::AS2-06 |
| RT-03 | ② 分派中的 15 号（sys_rt_sigreturn → restore_sigcontext，早于 RT-01） | others | write | 用户帧 sc 的 15 个通用寄存器与 sp、ip 原样写入 current_pt_regs() 这块帧；cs、ss 取 __u16 值或上 3；flags 只取 FIX_EFLAGS 位（含 TF、RF）；之后 RT-01 再写一次同值的 ax（return regs->ax） | 帧地址与复制的检查通过（见 D02）；取到的值不校验，force_valid_ss 被注释 | SD-23、SD-20、SD-21、QD-29、AD1-10、SD-22、SD-24、CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB |
| RT-04 | ② 分派中的 15 号（restore_sigcontext） | orig_ax | write | -1（signal_64.c 第 82 行的注释：disable syscall checks） | 同 RT-03 | AD1-10 |
| RT-36 | ② 分派中的 59 号（sys_execve → … → load_elf_binary → START_THREAD → start_thread_common，早于 RT-01） | others | write | 写 current_pt_regs() 这块帧：ip = elf_entry；sp = bprm->p；cs = __USER_CS；ss = __USER_DS；flags = X86_EFLAGS_IF 与 X86_EFLAGS_FIXED 相或；ELF_PLAT_INIT 被注释，通用寄存器字段不写；之后 RT-01 写 ax = 0（推断：只注册了 ELF 一种格式，load_elf_binary 置 retval = 0 后原样返回） | execve 走到 load_elf_binary 末段的 START_THREAD；没走到时不写这些字段 | CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB、CORE-USER-BOUNDARIES-07::QB-37、CORE-USER-BOUNDARIES-07::AB2-15、CORE-USER-BOUNDARIES-07::QB-38、CORE-USER-BOUNDARIES-07::FP-3、CORE-USER-BOUNDARIES-07::CL-C3 |
| RT-05 | ②′ ret_from_fork（进入返回尾之前） | ax | write | 0 | 内核线程 fn 返回后；用户子任务的 ax 已由 copy_thread 置 0 | CORE-USER-BOUNDARIES-07::AB2-11、CORE-USER-BOUNDARIES-07::FP-2 |
| RT-06 | ③a 返回尾·有处理函数（handle_signal） | orig_ax | read | syscall_get_nr：(int)orig_ax 与 -1 比较 | get_signal 返回真 | AD1-06、SD-13、QD-24 |
| RT-07 | ③a 返回尾·有处理函数（handle_signal） | ax | read | syscall_get_error：IS_ERR_VALUE(ax) 时取原值，否则 0 | (int)orig_ax ≠ -1 | QD-24、SD-13 |
| RT-08 | ③a 返回尾·有处理函数（handle_signal 重启表） | ax | write | -EINTR（-516、-514，或 -512 且无 SA_RESTART）；orig_ax（-513，或 -512 且有 SA_RESTART） | 同 RT-07 且 ax 为上述码；在建帧之前执行，建帧失败也不回滚 | SD-14、SD-15 |
| RT-09 | ③a 返回尾·有处理函数（handle_signal 重启表） | ip | read_write | ip − 2 | ax 为 -513，或 -512 且有 SA_RESTART；同样不回滚 | SD-14 |
| RT-10 | ③a 建帧（x64_setup_rt_frame → __unsafe_setup_sigcontext） | others | read | di、si、bp、sp、bx、dx、cx、ax、r8–r15、ip、flags、cs、ss 拷入用户信号帧的 sigcontext（ax、ip 是 RT-08、RT-09 之后的值）；get_sigframe 另读 sp；sa_flags 不含 SA_ONSTACK 且 nested_altstack 为假时，旧式备用栈分支的条件还读 ss（arch signal.c:65；该分支进不去，见 D02-c 第 4 点与 FN-02） | sa_flags 含 SA_RESTORER，否则 168–169 行直接返回 -EFAULT、不改寄存器；写用户内存不做 access_ok、没有缺页修复，帧地址不可访问时推断（#PF、#GP 是否产生属硬件语义）停在内核自旋：myos_bad_area（小于 TASK_SIZE_MAX 且不在任何 VMA 内）、do_kern_addr_fault（不小于 TASK_SIZE_MAX 的规范地址）或 excep_gen_prot（非规范地址）的 while (1)（ED-20；分段见 RS-06、RS-08）；地址小于 TASK_SIZE_MAX 而落在某个 VMA 内时交给 myos_handle_mm_fault，结果非 0 时在 do_user_addr_fault 中自旋，为 0 时去向未追 | AD1-11、SD-18 |
| RT-11 | ③a 建帧后改寄存器（x64_setup_rt_frame） | ax | write | 0 | 到达寄存器改写段：SA_RESTORER 在，且 SA_SIGINFO 时 copy_siginfo_to_user 成功 | AD0-15 |
| RT-12 | ③a 建帧后改寄存器（x64_setup_rt_frame） | ip | write | ksig->ka.sa.sa_handler（地址不校验） | 同 RT-11 | AD0-16 |
| RT-13 | ③a 建帧后改寄存器（x64_setup_rt_frame） | sp | write | 信号帧地址 frame | 同 RT-11 | AD0-16 |
| RT-14 | ③a 建帧后改寄存器（x64_setup_rt_frame） | others | write | di = 信号号；si = &frame->info；dx = &frame->uc | 同 RT-11 | AD0-15、AD0-16、SD-19 |
| RT-15 | ③a 建帧后改寄存器（x64_setup_rt_frame） | cs | write | __USER_CS | 同 RT-11 | AD0-17 |
| RT-16 | ③a 建帧后改寄存器（x64_setup_rt_frame） | ss | read_write | 读 ss；不等于 __USER_DS 时调 force_valid_ss，后者判为无效时改为 __USER_DS（判定逻辑在 signal_64.c:23–45，本包未展开） | 同 RT-11 | AD0-17 |
| RT-17 | ③a 建帧之后（handle_signal） | flags | read_write | 清 DF、RF、TF | setup_rt_frame 返回 ≥ 0 | SD-16 |
| RT-18 | ③b 返回尾·无处理函数（arch_do_signal_or_restart） | orig_ax | read | syscall_get_nr：(int)orig_ax 与 -1 比较；rt_sigreturn 的出口为 -1，跳过下面的 switch | get_signal 返回假（包括只剩默认动作或忽略动作的信号时） | AD1-07、QD-24、SD-11 |
| RT-19 | ③b 返回尾·无处理函数（arch_do_signal_or_restart） | ax | read | syscall_get_error：IS_ERR_VALUE(ax) 时取原值，否则 0 | (int)orig_ax ≠ -1 | AD1-07、QD-24 |
| RT-20 | ③b 重启意图（arch_do_signal_or_restart） | ax | write | orig_ax（-514、-512、-513）；219 = __NR_restart_syscall（-516） | 同 RT-19 且 ax 为上述码（包括 (uint)nr ≥ 453 时恰为这些值的用户 rax） | AD1-07、AD1-08、AD1-09 |
| RT-21 | ③b 重启意图（arch_do_signal_or_restart） | ip | read_write | ip − 2（推断：回到 syscall 指令；ISA 语义未核） | 同 RT-20 | AD1-08、AD1-09 |
| RT-22 | ④ SYSRET 判断（do_syscall_64 尾部） | cx | read | 与 ip 比较；结果只进入无人使用的 bool | 只在 do_syscall_64 路径；判断的第一个读 | AD1-02 |
| RT-23 | ④ SYSRET 判断（do_syscall_64 尾部） | ip | read | 与 cx 比较；之后再与 TASK_SIZE_MAX 比较 | 比上界要求前两个判断都没有先返回 | AD1-02、SD-10 |
| RT-24 | ④ SYSRET 判断（do_syscall_64 尾部） | r11 | read | 与 flags 比较 | cx == ip 时（\|\| 短路） | AD1-02 |
| RT-25 | ④ SYSRET 判断（do_syscall_64 尾部） | flags | read | 与 r11 比较；之后再测 RF、TF | 与 r11 比较的条件同 RT-24；测 RF、TF 要求前三个判断都没有先返回 | AD1-02、SD-10 |
| RT-26 | ④ SYSRET 判断（do_syscall_64 尾部） | cs | read | 与 __USER_CS 比较 | 第一个判断没有先返回 | AD1-03 |
| RT-27 | ④ SYSRET 判断（do_syscall_64 尾部） | ss | read | 与 __USER_DS 比较 | 另要求 cs == __USER_CS（\|\| 短路） | AD1-03 |
| RT-28 | ⑤ 返回段（syscall_return_via_sysret） | flags | read_write | xchg 把 flags 换到 R11 槽（187、189 行）；EFLAGS 槽最后存原 r11 槽的值（191 行），随后被 addq 跳过 | 进入返回段后无条件：do_syscall_64 按位置落入；ret_from_fork_asm 在 274–275 行检查 rbx，为 0（用户子任务）时经 282 行 jmp 进入；rbx 非 0 的内核线程转到 1:，经 callq 调用 rbx 所指函数，之后停在 297 行的 jmp .，到不了返回段 | CORE-USER-BOUNDARIES-07::AB2-16、CORE-USER-BOUNDARIES-07::SB-03 |
| RT-29 | ⑤ 返回段（syscall_return_via_sysret） | ip | read_write | xchg 把 ip 换到 RCX 槽（188、190 行）；RIP 槽最后存原 cx 槽的值（192 行），随后被跳过 | 无条件 | CORE-USER-BOUNDARIES-07::AB2-16、AD1-04 |
| RT-30 | ⑤ 返回段（syscall_return_via_sysret） | r11 | read_write | 槽被写成 flags（189 行），再由 POP_REGS 的 popq %r11 装入 r11 | 无条件 | CORE-USER-BOUNDARIES-07::AB2-16、QD-23 |
| RT-31 | ⑤ 返回段（syscall_return_via_sysret） | cx | read_write | 槽被写成 ip（190 行），再由 popq %rcx 装入 rcx | 无条件 | CORE-USER-BOUNDARIES-07::AB2-16、AD1-04、QD-23 |
| RT-32 | ⑤ 返回段（syscall_return_via_sysret） | ax | read | popq %rax 装入 ③ 之后帧中的 ax；%rax 里 do_syscall_64 的返回值被覆盖（调用约定属外部语义） | 无条件 | AD1-04、QD-23 |
| RT-33 | ⑤ 返回段（syscall_return_via_sysret） | others | read | POP_REGS 装入 r15、r14、r13、r12、bp、bx、r10、r9、r8、dx、si、di | 无条件 | AD1-04、CORE-USER-BOUNDARIES-07::VT-PTREGS |
| RT-34 | ⑤ 返回段（syscall_return_via_sysret） | sp | read | addq $0x28 先跳过 irq_nr、orig_ax、RIP、CS、EFLAGS 五格（不读）；popq %rsp 取 sp；ss 槽此后不再被读 | 无条件 | AD1-04、CORE-USER-BOUNDARIES-07::VT-PTREGS |
| RT-35 | ⑥ 落地（sysretq 之后） | others | read | swapgs、sti、sysretq；按 178–183 行作者注释，sysretq 用 rcx、r11 作用户 ip、flags；用户态实际得到的 ip、flags、sp 与 CS/SS 未核 | 无条件；SYSRET 不适用时的后果未知（NVR-3） | AD1-05、CORE-USER-BOUNDARIES-07::SB-02 |

## D02 信号：设置、选取、建帧与恢复

本节只依据 a039d9803ade 的源码文本，运行行为一律 NOT_RUN。13、14、15 号表项已由 CORE-USER-SYSCALL-STDIO-08::entry.syscall_table 列出，这里不重复。返回尾怎样进入信号代码见 D01 节；重启条件表见 D03 节。帧里每个字段的来去见帧字段表 FF-01 至 FF-19。

### D02-a rt_sigaction 与 rt_sigprocmask

**rt_sigaction 保存什么。**
- 先查 sigsetsize，必须等于 sizeof(sigset_t)，否则返回 -EINVAL syscall.c:30（QD-L37）。sigset_t 只有一个字，合法值是 8（推断）。
- act 非空时，先用 copy_from_user 把整个 struct sigaction 读进内核局部变量；随后不论 act 是否为空都调用 do_sigaction，act 为空时传 NULL AD2-19。保存的四项是 sa_handler、sa_flags、sa_restorer、sa_mask signal_types.h:50（QD-L35） signal_types.h:57（QD-L35）。do_sigaction 只有这一个调用者 QD-72。
- 校验：sig 必须在 1 到 64 之间；安装新处置时拒绝 SIGKILL、SIGSTOP；只查询旧处置不受此限 signal.c:1170（QD-L31）。
- 存放位置是 current->sighand->action[sig-1]。读写表项期间持 siglock：1175 行加锁 signal.c:1175（QD-L31），1225 行解锁 signal.c:1225（QD-L31）；1177 行只是 SA_IMMUTABLE 分支的提前解锁 signal.c:1177（QD-L31）。没有活动代码设置 SA_IMMUTABLE，所以 1176 行的拒绝 signal.c:1176（QD-L31）不会发生（推断）QD-73。
- 旧处置按值拷给调用方 AD2-16。新、旧两份的 sa_flags 都与 UAPI_SA_FLAGS 相与；sigaction_compat_abi 被注释，之后没有代码把位加回 signal.c:1197（QD-L31）。
- 新处置先从 sa_mask 去掉 SIGKILL、SIGSTOP，再整项写入（1173 行取址，1204 行写入）AD2-17。新处置会忽略该信号时只清共享 pending，清每线程 pending 的循环被注释 signal.c:1219（QD-L31）。
- oact 非空时用 copy_to_user 写回旧处置 AD2-20。

**rt_sigprocmask 保存什么。**
- 同样先查 sigsetsize，然后不加锁地快照 current->blocked AD2-21。
- nset 非空时读入新掩码，就地去掉 SIGKILL、SIGSTOP，再调内核 sigprocmask syscall.c:69（QD-L37）。内核 sigprocmask 自己不剔除这两个信号 SD-58，所以 69 行是这条路上唯一的保护点。
- sigprocmask 按 how 合成新掩码，how 不是 0、1、2 时返回 -EINVAL AD2-15，然后交给 __set_current_blocked signal.c:1157（QD-L31）。
- __set_current_blocked 在新旧相等时直接返回，否则持 siglock 写入 signal.c:1105（QD-L31）。写入就是 tsk->blocked = *newset；retarget_shared_pending 与 recalc_sigpending 都被注释 signal.c:1093（QD-L31）。
- oset 非空时写回修改前的快照 AD2-22。

**用户内存的读写（RS-11）。**
- 前提是 sigsetsize 等于 sizeof(sigset_t)（单字，推断为 8），否则两条调用在复制前就返回。两条调用自己都不查指针 SD-56 SD-57。
- copy_from_user 全树只有一个活动实现 QD-74。进入 _inline_copy_from_user 后，access_ok 的 if 行和右括号都被注释，复制语句无条件执行 AD2-10 AD2-11 QD-75 SD-59。
- copy_to_user 保留了 access_ok AD2-12 SD-60。x86 的 __access_ok 有两支 AD2-05 AD2-04：__builtin_constant_p(size <= PAGE_SIZE) 为真且 size <= PAGE_SIZE 时，只看起始地址的符号位；否则要求起始地址加长度后仍非负且不回绕。两支都拒绝负的起始地址。这里的长度在调用点是 sizeof 常量 AD2-20 AD2-22，但要经 _inline_copy_to_user 再传进 __access_ok。走哪一支取决于编译器是否内联并折叠 __builtin_constant_p，源码上不能确定（推断，未构建，同 RS-07）。仓库默认脚本以 Debug 构建类型调用内核构建，构建文本中没有 -O 选项 QD-115 QD-116。
- 底层 copy_user_generic 登记了异常表项 uaccess_64.h:115（QD-L07），但 mykernel 中没有任何消费者 QD-76。
- 所以 act、nset 不经 valid_user_address 判定就被当作源地址读取，高半区地址（含内核地址）也不会被拒绝。读取先于信号号与 how 的校验；sig 或 how 无效时读取照样发生，只是内容不落表。地址无效时，源码里没有转成 -EFAULT 的路径，缺页去向是已登记的内核自旋（CORE-MM-BASELINE-04::MC-23，推断）；非规范地址按外部语义产生的是 #GP，经 excep_gen_prot 停在 while (1)（AD2-09，推断）。写回方向只有半区检查；低半区中通过检查却不可访问的地址照样在内核里出错，去向按地址分（推断）：小于 TASK_SIZE_MAX 且不在任何 VMA 内时停在 myos_bad_area 的 while (1)，TASK_SIZE_MAX 到 2^47 之间停在 do_kern_addr_fault 的 while (1)，2^47 以上的非规范地址产生 #GP 后停在 excep_gen_prot 的 while (1)；小于 TASK_SIZE_MAX 而命中 VMA 时交给 myos_handle_mm_fault，结果非 0 时在 do_user_addr_fault 中自旋，为 0 时去向未追（同 RS-06、RS-08）。
- 与 CORE-USER-BOUNDARIES-07::UB-07、CORE-USER-SYSCALL-STDIO-08::US-05 不同：那两条是根本没调复制原语，本条是复制原语自己少了检查。copy_from_user 的其余调用者（fs/fat/myos_fat32.c、time/misc/time_misc.c）未追；rt_sigreturn 在调用前自己做了 access_ok。

**SA_RESTORER 被掩掉（RS-01，包含顺序推断，needs_evidence）。**
- UAPI_SA_FLAGS 包含 __ARCH_UAPI_SA_FLAGS AD2-18。后者在 signal_const.h 第一次被处理时定值：那时 SA_RESTORER 已定义就取它，否则取 0 QD-77。
- SA_RESTORER 全树只有一个定义 AD2-01 QD-78。头文件中只有 uapi/linux/signal.h 包含 asm/signal.h QD-79；uapi/linux/signal.h 只被 lock_IPC/signal/signal.h 第 10 行包含 QD-80。
- signal.c 的第一个包含就是这个 signal.h QD-83。它在第 6 行先经 lock_ipc_const.h 进入 signal_const.h（唯一包含者）QD-81 QD-82。signal.h 有包含卫哨，第 10 行之前的任何传递包含都到不了 asm/signal.h 第 65 行。构建文本里也没有 -include、-imacros，也没有这两个宏的命令行定义 QD-84。
- 推断：在 signal.c 这个翻译单元里，__ARCH_UAPI_SA_FLAGS 被定为 0。其余各位来自 signal-defs.h（经 semaphore_const.h 提前进入），都不等于 0x04000000。于是 do_sigaction 清掉 SA_RESTORER，回填给用户的旧处置也没有这一位。
- 后果在建帧侧：x64_setup_rt_frame 在 168 行检查这一位，没有就在 169 行返回 -EFAULT，此时还没改任何寄存器：

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal_64.c::x64_setup_rt_frame]
```c
	/* x86-64 should always use SA_RESTORER. */
	if (!(ksig->ka.sa.sa_flags & SA_RESTORER))
		return -EFAULT;
```

- 所以若推断成立，经 rt_sigaction 安装的处理函数按源码控制流都停在 169 行，到不了 get_sigframe（唯一调用点在 171 行）和之后的帧写入。
- 反向可能：有本次没看到的强制包含；或首次配置 CMake 时经环境变量 CFLAGS 注入了宏，静态检索看不到。signal_64.c 那个翻译单元里 SA_RESTORER 一定已定义，否则 168 行编译不过；差别只在 signal.c。要定论需要预处理该翻译单元（NVR-2，NOT_RUN）。libc 是否设置 SA_RESTORER 在仓库外（CORE-USER-VFS-BASELINE-05::GAP-U10）。

### D02-b get_signal：选取信号

**调用点与锁。**
- 唯一调用点在 arch_do_signal_or_restart：为真就交给 handle_signal 并返回；为假走重启判断（D03 节）AD1-06 QD-85。ksig 是未初始化的栈变量：signal.c:246（QD-L09）只声明、不给初值。
- 入口按 TIF_SIGPENDING 提前返回的判断被注释 QD-20，所以每次都在 relock 处取 siglock AD2-14。这是关中断、关抢占的自旋锁 spinlock_smp.h:77（QD-L36）。
- 整个 for(;;) 期间一直持锁，只在循环后解锁一次 signal.c:1042（QD-L31） SD-61。relock、fatal、out 三个标签都没有活动的 goto SD-62。fatal 处的解锁也被注释，所以默认动作分支带锁回到循环头，加解锁仍然配对。

**每轮怎么挑。**
- 先 dequeue_synchronous_signal，取不到再 dequeue_signal，掩码用 curr->blocked signal.c:916（QD-L31）；都取不到就 break，返回假 AD3-17。
- dequeue_signal 先查私有 pending，再查共享 pending signal.c:237（QD-L31）。next_signal 只看第一个字的 pending & ~blocked；有同步类（SEGV、BUS、ILL、TRAP、FPE、SYS）就只在其中挑，取编号最小者 AD2-13。
- collect_signal 摘下第一个同号节点；没有第二个同号节点时才清 pending 位 signal.c:175（QD-L31） signal.c:183（QD-L31）。找不到节点时合成 siginfo：si_code 为 SI_USER，si_pid、si_uid 为 0 signal.c:205（QD-L31）。

**按动作表分流。** 表项是 sighand->action[signr-1] signal.c:931（QD-L31）。

| 条件 | 结果 | 行 |
|---|---|---|
| SIG_IGN | 信号已出队，丢弃，下一轮 | 936–937 |
| 用户处理函数 | 整项复制到 ksig->ka；带 SA_RESETHAND 时把共享表项改回 SIG_DFL；返回真 | 938–945 |
| SIG_DFL 且默认忽略（CONT、CHLD、WINCH、URG） | 丢弃，下一轮 | 951–952 |
| SIG_DFL，任务带 SIGNAL_UNKILLABLE，且不是 SIGKILL、SIGSTOP | 丢弃，下一轮 | 964–966 |
| 其余 SIG_DFL | 只置 PF_SIGNALED，下一轮 | 1002–1041 |

依次的证据：AD3-18 signal.c:945（QD-L31） signal.c:951（QD-L31） signal.c:964（QD-L31） signal.c:1010（QD-L31）。只有处理函数分支返回真 signal.c:1049（QD-L31）。

ksig 有三个字段 signal_types.h:69（QD-L35）：sig 总在 1044 行写入（0 或信号号）；info 每次出队都被覆盖；ka 只在处理函数分支写入。返回假时调用方不读 ksig。

**默认动作只剩一个标志（RS-03）。**
- 停止、核心转储与 do_group_exit 都被注释 QD-86。fatal 处唯一的活动语句是置 PF_SIGNALED SD-63，而 PF_SIGNALED 全树只有定义和这一处写，没有读者 QD-87。
- 所以 SIGKILL、SIGTERM、SIGINT、SIGSEGV、停止类信号，以及 32 号以上的实时信号（siginmask 只覆盖 1 到 31 signal_macro.h:108（QD-L34）），在默认动作下被出队后，任务既不退出也不停止。消耗发生在目标任务下一次经 syscall_exit_to_user_mode 返回时；发送端对 complete_signal 的调用被注释，不会唤醒目标 QD-88。
- 1 号任务带 SIGNAL_UNKILLABLE：is_child_reaper 分支先把它记为 child_reaper fork.c:1321（QD-L39），下一行置位 fork.c:1322（QD-181）。对它，默认忽略的 CONT、CHLD、WINCH、URG 先在 951–952 行丢弃；其余默认信号中，除 SIGKILL、SIGSTOP 外都在 964–966 行丢弃，只有这两个到 1010 行（哪个任务拿到 1 号未追，推断）。
- 「表项是 SIG_DFL」是全系统条件，因为动作表被所有任务共用（见 RS-12）。SIGKILL、SIGSTOP 的表项恒为 SIG_DFL：初值如此 init_task.c:58（QD-195），do_sigaction 也拒绝修改 signal.c:1170（QD-L31）。
- get_signal 返回假后，是否改写 ax、ip 取决于两点：(int)orig_ax ≠ -1，且 ax 恰为 -512、-513、-514、-516 四个重启码之一（SD-28，D03 节）；与刚被消耗的信号无关。rt_sigreturn 成功恢复后 orig_ax 为 -1，即使恢复出的 ax 是重启码也不改写（RR-01）。即使接上 do_group_exit，父进程能否看到退出状态仍受 CORE-USER-VFS-BASELINE-05::UV-15 限制。

**TIF_SIGPENDING 整体停用（RS-04）。**
- 返回尾的门禁被注释，arch_do_signal_or_restart 无条件执行 common.c:34（QD-L17）；get_signal 自己的门禁也被注释（见上）。所以经 syscall_exit_to_user_mode 的两条路（do_syscall_64 返回、ret_from_fork），每次都取一次 siglock、做一次出队检查。没有同步信号时，dequeue_synchronous_signal 只做一次位图判断就返回 signal.c:317（QD-L31）。首个程序经 kjmp_to_doexecve 直接进用户态，不走这条路（CORE-USER-BOUNDARIES-07::FP-1）。
- 三处置位都到不了：signal_wake_up_state 只经 complete_signal 调用 signal.c:368（QD-L31）；calculate_sigpending 的唯一调用被注释 signal.c:108（QD-L31） QD-89；restart_syscall 没有调用者 signal.h:463（QD-L32） QD-38。
- 在 mykernel 内按名检索，没有清除语句 QD-90；recalc_sigpending 只有声明，没有定义，调用全被注释 QD-91。全部命中的分布见 QD-92。
- 推断：signal_pending() 恒为假。这一位只在 task_sigpending 里被读 QD-92；等待路径上只有 signal_pending_state 与 msleep_interruptible 会读到它 QD-225。signal_pending_state 的活动调用只有 completion.h、semaphore.h 两处，调度器里按 prev_state 判断的那一处是注释 QD-224；wait_for_common 唯一的活动调用者 wait_for_completion 传 TASK_UNINTERRUPTIBLE，signal_pending_state 对它先返回 0，不读这一位 AD3-16 AD3-20；msleep_interruptible 与 down 系列的可中断、可杀入口都没有活动调用者 QD-93 QD-172。
- 活动代码里另有以 TASK_INTERRUPTIBLE 睡眠的循环：kthreadd、devtmpfsd 的 devtmpfs_work_loop，以及 NVMErqd、ATArqd、XHCIrqd 三个请求线程（kthread.c:342、devtmpfs.c:373、MineOS_NVMe.c:522、ide.c:360、myos_XHCI.c:631，QD-223）。它们所在文件没有活动的 signal_pending 调用 QD-225，调度器也不按睡眠状态检查信号（__schedule 中按 prev_state 调 signal_pending_state 的一段 scheduler_core.c:1079-1114 整段是 // 注释，1086 行的调用见 QD-224），所以同样不读这一位（推断）。
- 所以眼下没有等待受影响，这部分是潜伏的：只恢复置位端，取空后位会一直保持；只恢复门禁，信号就取不出来。恢复置位端后，signal_wake_up_state 对上述 TASK_INTERRUPTIBLE 线程的唤醒及其后果属唤醒与调度，本包未覆盖。

**同步出队留下 pending 位（RS-14，潜伏）。**
- 第一遍从表头找到 sync 后跳到 next；第二遍同样从表头开始 signal.c:338（QD-L31），而 sync 还在链表里 signal.c:327（QD-L31） SD-64 double_list_macro.h:346（QD-L30）：

[VERIFIED mykernel/lock_IPC/signal/signal.c::dequeue_synchronous_signal]
```c
	// list_for_each_entry_continue(q, &pending->list, list) {
	list_header_for_each_container(q, &pending->list, list) {
		if (q->info.si_signo == sync->info.si_signo)
			goto still_pending;
```

- 被注释的原写法从 sync 之后继续查；树内该宏的定义也只在注释里 QD-94。现在第二遍必在 sync 自身或排在它之前的同号节点处命中 still_pending，343 行的 sigdelset 到不了，节点照样被摘下、释放 signal.c:347（QD-L31）。
- 后果有前提：私有链表里没有别的同号节点（同号多节点可能出现）。这时节点已释放而位还在。下次经 dequeue_signal 取到这一位时，collect_signal 在 183 行清掉它，并合成与原 siginfo 不符的 SI_USER siginfo，也就是只多出队一次。处理函数情形下重复发生在下一次 get_signal；带 SA_ONESHOT 时第一次已复位为 SIG_DFL，第二次走默认动作。
- 前提目前满足不了：pending 链表唯一的活动插入点在发送函数里 QD-95，唯一的活动发送链固定用 PIDTYPE_TGID，信号进共享 pending，si_code 为 SI_USER QD-96；tkill、tgkill、rt_sigqueueinfo 也不在表中 QD-97。

**fork 继承的信号状态（RS-12、RS-13）。**
- dup_task_struct 整体复制父任务，含 sighand 指针、blocked 与私有 pending 位图 fork.c:288（QD-L39）。copy_sighand 分配并复制了新动作表，却不装到任务上 fork.c:758（QD-L39） SD-65。全树唯一的 sighand 赋值是 init_task 的静态初值 init_task.c:124（QD-L29） QD-98。
- RS-12：推断所有经 fork 产生的任务都指向 init_sighand。get_signal 取的锁和读写的表 signal.c:805（QD-L31）、do_sigaction 写的表项、__set_current_blocked 取的锁、copy_sighand 与 copy_process 失败出口取的锁 fork.c:764（QD-197） fork.c:1429（QD-197），都是同一个对象。exec 也不复位处理函数、不私有化动作表 QD-99。
- 后果：任一任务改了某个信号，所有任务都看到；SA_RESETHAND 在一个任务触发就复位全体；exec 后的新映像可能取到旧映像的处理函数地址。对 fork 后没有 exec 的子任务，地址本身有效，问题在于双方共写一张表。这些处理函数相关的后果目前潜伏：仓库内用户程序唯一的 signal() 在 test_all 里，而 test_all 唯一的调用被注释 QD-100；libc 内部属 CORE-USER-VFS-BASELINE-05::GAP-U10。共享 siglock 不依赖处理函数；是否形成争用取决于并发前提，未评估。若 RS-01 成立，被遮蔽的只有「跳到别的地址空间的处理函数」一项；出队和 SA_ONESHOT 复位都在建帧之前。
- 共享 init_sighand 本身 05 已记为事实（CORE-USER-VFS-BASELINE-05::UV-14、CORE-USER-VFS-BASELINE-05::sched.forkexec.copy_process_resources）。本条只补信号语义上的后果和 exec 一侧，主线可以决定并入 UV-14。
- RS-13（潜伏）：init_sigpending 只重置链表，清位图的 sigemptyset 被注释 signal.h:390（QD-L32） SD-66；copy_process 对子任务调用它 fork.c:1024（QD-L39）。共享 pending 不受影响，copy_signal 新建的是清零对象 fork.c:795（QD-L39）。子任务因此带着父任务的私有挂起位，却没有对应节点。blocked 也一起继承，所以这一位要不在子任务的 blocked 里才会被选出：fork 时未屏蔽的位，子任务第一次返回就选出；已屏蔽的位，要等子任务解除屏蔽后。按 collect_signal 的回退分支，子任务会收到一个从未发给它的信号（推断）。前提目前不成立：私有 pending 位图没有活动的写入来源（同 RS-14 的检索）。

### D02-c 建帧

**调用链。** handle_signal → setup_rt_frame → x64_setup_rt_frame。setup_rt_frame 只剩 64 位一个活动分支 signal.c:161（QD-L09） SD-67。handle_signal 先按重启表改写 ax、ip（D03 节），再建帧 signal.c:176（QD-L09） AD0-07。sa_handler、sa_restorer 都来自 rt_sigaction 安装的表项（get_signal 940 行复制），一路上没有对这两个地址的校验。

**帧地址（get_sigframe）。**
1. sp 取 regs->sp，即入口保存的用户 rsp（07 已确立）signal.c:44（QD-L09）；函数内没有地址检查 SD-68。
2. 无条件减 128 字节红区，ia32 判断被注释 signal.c:49（QD-L09）。
3. SA_ONSTACK 时，只有 sas_ss_flags(sp) 为 0 才换到备用栈 signal.c:59（QD-L09）。sas_ss_size 为 0 时它返回 SS_DISABLE signal.h:436（QD-L32）；sas_ss_* 只被置 0，表里也没有 sigaltstack QD-101 QD-97。推断：这个分支不会进入。
4. 旧式栈切换分支要求没有 SA_RESTORER，和 168 行的门互斥，恒不进入（事实记录，不立条目）signal.c:66（QD-L09）。sp 本来就来自用户 rsp，这条分支不给用户新的控制能力。
5. *fpstate 取减去帧大小之前的 sp；预留 FPU 区的 fpu__alloc_mathframe 被注释 signal.c:74（QD-L09）。
6. 再减去帧大小：sp -= frame_size signal.c:76（QD-L09）；frame_size 只来自唯一调用点 x64_setup_rt_frame 传入的 sizeof(rt_sigframe_s) signal_64.c:171（QD-L10） QD-71。
7. 对齐为 round_down(sp, 16) - 8 signal.c:85（QD-L09）。
8. 备用栈溢出时返回 -1L signal.c:98（QD-L09）。这一支要求 nested_altstack 或 entering_altstack 为真（91 行）：按第 3、4 点，entering_altstack 恒为假；nested_altstack 取自 41 行的 on_sig_stack(regs->sp)，而 sas_ss_sp、sas_ss_size 只被置 0 QD-101，此时 __on_sig_stack 恒为假，所以它也为假。因此 98 行不可达（推断）。保存 FPU 的调用被注释，最后返回 sp signal.c:105（QD-L09）。

帧布局：pretcode 在偏移 0，后面是 ucontext 和 siginfo signal_types_arch.h:61（QD-L11） ucontext.h:10（QD-L28）。

**写入的字段**（逐项见 FF-01 至 FF-16）。
- uc_flags、uc_link、uc_stack 依次写在 178、179、180 行 signal_64.c:178（QD-L10） signal_64.c:179（QD-L10） signal_64.c:180（QD-L10）；pretcode、sigcontext、uc_sigmask 依次写在 184、185、186 行 signal_64.c:184（QD-L10） signal_64.c:185（QD-L10） signal_64.c:186（QD-L10）。
- 掩码 set 来自 sigmask_to_save signal_64.c:162（QD-L10） signal.h:566（QD-L32）。置 restore_sigmask 的函数没有活动调用者 QD-102，推断 set 就是 current->blocked。
- sigcontext：通用寄存器、ip、flags、cs、ss 取自 regs；trapno、err、cr2 取自 current->thread；gs、fs 写 0；另写 fpstate 与 oldmask signal_64.c:118（QD-L10） signal_64.c:120（QD-L10） signal_64.c:121（QD-L10） signal_64.c:127（QD-L10） SD-18 SD-53。逐行位置：di 至 r15 共 16 个字段（含 sp）在 101–116 行，trapno、err 在 118–119 行 AD3-11，ip、flags、cs 在 120–122 行，gs、fs 在 123–124 行，ss 在 125 行，fpstate 在 127 行，oldmask、cr2 在 130–131 行。
- 只有 SA_SIGINFO 时才写 siginfo，走 copy_siginfo_to_user signal_64.c:190（QD-L10） signal.c:1063（QD-L31）。

**处理函数入口寄存器。**

| 字段 | 新值 | 行 |
|---|---|---|
| di | 信号号 | 198 |
| ax | 0 | 200 |
| si、dx | &frame->info、&frame->uc | 204–205 |
| ip | sa_handler | 206 |
| sp | frame | 208 |
| cs | __USER_CS | 227 |
| ss | 不等于 __USER_DS 时经 force_valid_ss 修正 | 229–230 |
| flags | 建帧成功后清 DF、RF、TF（在 handle_signal） | 216 |

证据：AD0-15 AD0-16 AD0-17 signal.c:216（QD-L09）。这些值随后由 07 记录的 sysret 返回段消费（ED-19）。

**失败返回与消费（RS-02）。**
- x64_setup_rt_frame 有三个 return -EFAULT SD-69：169 行（缺 SA_RESTORER）；191 行（SA_SIGINFO 时 copy_siginfo_to_user 失败，源码上只对应 copy_to_user 的 access_ok 或 clear_user 的 __access_ok 拒绝该地址（常量分支下即高半区地址，两支见 D02-a）；拷贝或清零留下剩余字节要靠异常表修复，树内没有消费者 QD-76，按 D02-d 第 3 点的同一推断到不了）；Efault 标号 signal_64.c:236（QD-L10）。Efault 到不了：unsafe_put_user 展开成 put_user，put_user 是裸存储，表达式值恒为 -ENOERR，即 0 AD2-02 errno-base.h:5（QD-L27）。__unsafe_setup_sigcontext 的 Efault 同理 signal_64.c:134（QD-L10）。
- handle_signal 只把结果记进 failed，成功时才清标志位。signal_setup_done 被注释，mykernel 中也没有 force_sigsegv 的活动代码 SD-72 QD-103。arch_do_signal_or_restart 随即返回。
- 因此建帧失败时：信号已出队；带 SA_ONESHOT 时共享表项已复位；没有 SIGSEGV 或其他补救。「先改写再建帧」这个顺序本身没错：成功时改写后的 ax、ip 被存进 sigcontext，sigreturn 再取回。缺陷只在失败分支无人处理：改写留在 pt_regs，经返回尾带回用户态（推断，返回段见 D01 节与 CORE-USER-BOUNDARIES-07::FP-3）。改写需要 (int)orig_ax ≠ -1，且 ax 恰为 -512、-513、-514、-516 四个重启码之一；按名字与字面量检索，目前没有内核侧的活动产生者；没有活动 return 的处理函数与按计算得出的返回值不能排除，已知能由源码确定构造的途径只有越界号（D03.5）。缺 SA_RESTORER 这条失败不改任何寄存器，非 -ERESTART* 的 ax 原样保留。若 RS-01 成立，所有经 rt_sigaction 安装的处理函数都落进这条失败路径（条件推断）。

**以下 RS-08、RS-09 以 RS-01 不成立为前提（潜伏）；RS-10 只有建帧侧以此为前提，恢复侧经用户直接发出 15 号调用可达，不依赖 RS-01。**
- RS-08：帧主体写入没有地址检查。user_access_begin 被注释 SD-80，mykernel 里也没有它的活动定义 QD-104 SD-70。写入用的宏就是裸存储：

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::unsafe_put_user]
```c
	#define unsafe_put_user(x, ptr, label)		\
			if (put_user(x, ptr) != -ENOERR)	\
				goto label;
```

- RS-08（续）：范围是经 unsafe_put_user 写的 pretcode 和 uc 各字段。siginfo 走 copy_to_user，有半区检查，但它在 189–191 行才执行，晚于主体写入，拦不住前面的写。写入若引发未能处理的缺页（地址不小于 TASK_SIZE_MAX、不在任何 VMA 内，或命中 VMA 而处理不成功），源码去向是已登记的自旋（CORE-MM-BASELINE-04::MC-23、CORE-MM-BASELINE-04::LB-05，推断）；命中 VMA 时缺页交给 myos_handle_mm_fault fault.c:323（QD-L12），返回 0 就不进紧随其后的 while 自旋，之后写入能否完成属本批未覆盖的“VMA 存在时的缺页去向”（推断；写不带 VM_WRITE 的私有映射会反复缺页，见 CORE-MM-BASELINE-04::MC-24）。帧地址是非规范地址时不走缺页：按外部语义产生 #GP，源码上经 excep_gen_prot 停在 while (1) interrupt.c:148（QD-L14） AD2-09（推断）。缺 stac/clac 在本树没有单独后果，活动源码没有开启 SMAP（推断）。缺异常表修复是全内核的情况，根因是全树唯一的活动 put_user 就是裸存储，不只在信号帧。备用栈溢出返回 -1L 的情形不可达（推断）；即使可达，第一笔写的地址回绕成低地址 7，按源码会走 do_user_addr_fault（推断）。
- RS-09：uc_flags 声明后没有赋值，frame_uc_flags 被注释，且在 mykernel 中没有定义 QD-105 SD-71，178 行却把它写进 uc.uc_flags。读取未初始化、未取地址的自动变量是未定义行为（C11 6.3.2.1p2，外部标准）；写出的值取决于编译产物（寄存器或栈槽），可能把内核残留数据交给用户（推断）。写到的是 get_sigframe 算出的帧地址。恢复侧不用这个值。
- RS-10：信号路径不保存、也不恢复 FPU、SSE 与扩展状态 QD-106。fpstate 指针照样写进 sigcontext：默认情形指向被中断代码红区的最低地址 regs->sp-128。sigcontext_64 自己约定「没有 FPU 上下文时为 0」signal_types_arch.h:46（QD-L11），这里写的却是非零地址。推断：用户代码若按约定读 fpregs，读到的是红区原有内容。rt_sigreturn 不恢复 FPU，被中断代码继续执行时，寄存器里是处理函数留下的内容（推断）。fpu__clear_user_states 被注释，处理函数入口继承被中断代码的浮点控制状态（推断）。任务切换同样不切换 FPU，信号路径只是更大缺口的一部分。用户态 SSE 是否可用取决于引导时留下的 CR4（外部，未核）。

**事实记录（不立条目）。** trapno、err、cr2 取自 current->thread 的三个字段 processor_types_arch.h:190（QD-L15），它们没有活动写入者 QD-107。任务结构经整体复制源自 init_task，其 .thread 只显式初始化 .sp init_task.c:117（QD-L29） AD3-02，推断写出的是 0。目前也没有「故障→信号」的活动生产者。

### D02-d rt_sigreturn 与 restore_sigcontext

**帧从哪里来。**
- 15 号处理函数自己用 current_pt_regs() 取帧 signal_64.c:246（QD-L10）。按 07 的推导，这就是入口建的那一帧（CORE-USER-BOUNDARIES-07::VT-KFRAME，推断）。
- 帧地址 = regs->sp - 8，再过 access_ok AD3-13。regs->sp 是发出 15 号调用时的用户 rsp。建帧侧把 pretcode 放在偏移 0、令 sp=frame；推断处理函数 ret 弹出 pretcode 后 rsp=frame+8，restorer 不改 rsp 就发 15 号时正好回到帧起点（用户态未追）。
- access_ok 在常量小尺寸分支只看符号位；另一分支要求 frame 加长度仍非负且不回绕（D02-a 的 __access_ok）。

**读了什么、先后顺序。**
1. get_user 先在 256 行读 8 字节的 uc_sigmask signal_64.c:256（QD-L10），再在 258 行读 uc_flags signal_64.c:258（QD-L10） SD-73。get_user 是直接解引用，表达式值恒为 -ENOERR 即 0；写进局部变量的是用户地址处的内容 uaccess.h:99（QD-L06）。所以这两处 goto badframe 在源码上到不了。
2. 掩码在寄存器之前写回：261 行调 set_current_blocked signal_64.c:261（QD-208），263 行才调 restore_sigcontext signal_64.c:263（QD-L10）。set_current_blocked 先去掉 SIGKILL、SIGSTOP，再走 __set_current_blocked（D02-a），不重算 pending，也不转投共享信号 signal.c:1122（QD-L31）。建帧侧 signal_setup_done 被注释，处理函数期间不施加 sa_mask 和本信号，所以帧里的掩码一般就是投递时的 blocked，写回多半走「相等即返回」（推断）。
3. restore_sigcontext 用 copy_from_user 读 uc_mcontext 到 reserved1 之前，推算 192 字节，覆盖 r8 到 fpstate AD3-08。copy_from_user 没有 access_ok；要返回非 0 只能靠异常表修复，而树内没有消费者，所以 57 行的 return false 也到不了（推断）。

**写回了哪些寄存器。**
- 共 21 处写入，都不检查值 SD-74：bx、cx、dx、si、di、bp、ax、sp、ip、r8–r15 原样写回 AD3-09；cs、ss 取用户值再或上 3；flags 只取 FIX_EFLAGS 覆盖的位 AD3-10，这个宏从 signal_64.c:12（QD-L10）起逐行列出各位，其中 TF 在第 13 行 signal_64.c:13（QD-L10），RF 在第 16 行 QD-108；orig_ax 置 -1 AD0-11。
- 不恢复的：uc_flags 的唯一用途 force_valid_ss 与 FPU 恢复都被注释，函数末尾是固定的 return true signal_64.c:93（QD-L10） SD-75，再加上 57 行唯一的 return false SD-77 到不了（推断），函数总返回 true（推断）；备用栈与影子栈的恢复也被注释 QD-109。fpstate、gs、fs、err、trapno、oldmask、cr2 读进来不用。restart_block 的复位被注释，见 D03 节的 RS-05。

**返回值怎样流回。**
- 成功时 return regs->ax，即刚写回的 sc.ax（272 行）。x64_sys_call 经 15 号的 case 分支原样上交 syscall_64.c:26（QD-L05；该宏在 switch 内展开 syscalls_64.h 的 15 号表项，QD-110），do_syscall_64 把它写回 regs->ax，值不变，然后无条件进入返回尾 common.c:44（QD-L03） common.c:49（QD-L03）。syscall_64.c:31（QD-L05）的 default 只接表中没有的号，与 15 号无关。
- 返回尾一定再跑一次 arch_do_signal_or_restart（D01 节）。orig_ax 已是 -1，重启分支跳过。get_signal 按刚写回的 blocked 出队；若放开了已挂起的信号，它会在恢复后的上下文上建新帧（推断）。
- 同一出口没有成功建立新信号帧时（get_signal 为假，或建帧在改写寄存器之前失败），写回的 ip、flags、cs、ss、cx、r11 原样到达 SYSRET 适用性判断，而判断结果被丢弃（CORE-USER-BOUNDARIES-07::UB-09；本包另作证据追加）。范围要说准：cs、ss 在帧里是 16 位并被强制 RPL=3 signal_types_arch.h:30（QD-L11）；flags 只有 FIX_EFLAGS 位来自用户；能写任意 64 位值的只有 ip、sp、cx、r11 这类通用寄存器字段，sp 不在判断之列。返回段用 ip（进 rcx）、flags（进 r11）和 sp；cs、ss 以及写回的 cx、r11 都不被使用（CORE-USER-BOUNDARIES-07::FP-3）。

**坏帧（RS-07）。**
- badframe 只 return 0；signal_fault 被注释，mykernel 中也没有它的定义 signal_64.c:274（QD-L10） QD-103。
- 由上，源码上唯一能到 badframe 的是 access_ok 失败：frame 按有符号数为负就够；非常量分支下，frame 落在 2^63 以下 sizeof(*frame) 字节以内也会失败。对应例子是用户 rsp 小于 8，或不小于 0x8000000000000008。
- 这时 rt_sigreturn 不写 regs；do_syscall_64 把 ax 写成 0，orig_ax 仍是 15，重启分支不命中（推断）。若返回尾没有投递信号，调用以 rax=0 回到 syscall 的下一条指令（推断：sysretq 以 rcx 为返回地址属 ISA 语义，未核），rsp 仍是那个非法值，寄存器没有按用户帧恢复，也没有任何信号。这条路在 254 行就返回，内核没碰用户帧，影响限于发出调用的进程。若返回尾投递了信号，get_sigframe 以同一个非法 regs->sp 为基准建帧，后果归 RS-08，不算缓解。
- 对照 Linux（不是 MyOS2 的事实）：那里这一调用会强制发送 SIGSEGV。
- 若将来 57 行的失败分支变得可达，会出现掩码已按用户帧写回、寄存器却没恢复的半恢复状态（条件推断，属同一缺口的后果）。

**读用户帧没有容错（RS-06）。**
- 非负却没有映射的帧地址到不了 badframe：两次 get_user 中先碰到它的那一次（先读的是 uc_sigmask）就在内核里访问出错（推断，依赖硬件语义）。
- 源码中的去向按地址分三段：小于 TASK_SIZE_MAX 且不在任何 VMA 内，进 myos_bad_area 打印后 while (1) AD2-08 AD2-07 fault.c:99（QD-L12）；落在 TASK_SIZE_MAX 到 2^47 之间，进 do_kern_addr_fault 的 while (1) AD2-06；2^47 以上、符号位为 0 的非规范地址按外部语义产生 #GP，源码上经 exception_handler 的 GEN_PROT_VEC 分支进 excep_gen_prot interrupt.c:148（QD-L14），以 while (1) 结束 AD2-09。前两段的 #PF 经同一函数的 PAGE_FAULT_VEC 分支进 exc_page_fault interrupt.c:151（QD-L14）。地址小于 TASK_SIZE_MAX 而落在某个 VMA 内时，缺页交给 myos_handle_mm_fault fault.c:323（QD-L12），不进 myos_bad_area：结果非 0 时在 do_user_addr_fault 中自旋，为 0 时之后的去向与该函数内部本批都未追（RS-06 的反证）。
- 这些去向不是新内容：CORE-MM-BASELINE-04::MC-23、CORE-MM-BASELINE-04::MC-22、CORE-MM-BASELINE-04::LB-05、CORE-MM-BASELINE-04::DR-01 已登记（含 simple_find_vma 只返回包含地址的 VMA、扩栈分支不可达、自旋能否被换下）。普通的用户态越界访问也能到同一个自旋，所以本条不带来新能力。CORE-USER-BOUNDARIES-07::UB-07 的未知项（内核访问用户地址出错的去向）也由 MC-23 回答。
- 本条新增的只有三点：rt_sigreturn 这个入口；get_user 的值恒为 0，使 257、259 行不可达；异常表只有生产者没有消费者 QD-76，所以 57 行的 return false 也到不了（推断）。copy_from_user 不做 access_ok 属 RS-11，在本入口没有单独后果，也不是另一个触发点：254 行已按整帧长度做过 access_ok，两次 get_user 又先执行，所读地址夹住被复制的 uc_mcontext（推断）。

### D02-e 信号帧字段表（frame_fields）

下表列出信号帧中每个字段在投递时从哪里来、存在帧的哪里、rt_sigreturn 时恢复到哪里；明确不保存、不恢复的状态也列出。只有走过 SA_RESTORER 门（RS-01 不成立）时，这些写入才会发生。

| 编号 | 字段 | 投递时取自 | 存在帧的哪里 | sigreturn 时恢复到 | 条件 | 证据 |
|---|---|---|---|---|---|---|
| FF-01 | pretcode（rt_sigframe 偏移 0） | ksig->ka.sa.sa_restorer：rt_sigaction 安装的恢复函数地址，经 get_signal 940 行复制；内核不校验地址 | frame->pretcode，帧起点 | rt_sigreturn 不读它，只按 regs->sp-8 回到帧起点（推断：处理函数 ret 弹出 pretcode 后 rsp=frame+8；用户态未追） | 越过 signal_64.c:168 的 SA_RESTORER 门；RS-01 成立时到不了 | signal_types_arch.h:61（QD-L11）、AD3-13、AD0-13、signal_64.c:184（QD-L10） |
| FF-02 | uc.uc_flags | 局部变量 uc_flags：声明后没有赋值，frame_uc_flags 被注释且没有定义 | frame->uc.uc_flags | get_user 读入局部 uc_flags 后传给 restore_sigcontext；唯一用途 force_valid_ss 被注释，不使用 | 同 FF-01 | signal_64.c:165（QD-L10）、SD-85、signal_64.c:178（QD-L10）、SD-71、QD-105、SD-73、signal_64.c:263（QD-L10）、SD-86、signal_64.c:258（QD-L10） |
| FF-03 | uc.uc_link | 常量 0 | frame->uc.uc_link | 不读 | 同 FF-01 | signal_64.c:179（QD-L10） |
| FF-04 | uc.uc_stack（备用栈状态 ss_sp、ss_flags、ss_size） | current->sas_ss_sp、sas_ss_flags、sas_ss_size（unsafe_save_altstack 宏） | frame->uc.uc_stack | 不恢复：restore_altstack 被注释 | 同 FF-01 | signal_64.c:180（QD-L10）、signal_64.c:149（QD-L10）、QD-109、QD-101、QD-97 |
| FF-05 | uc_mcontext 的 di、si、bp、bx、dx、ax、r8–r10、r12–r15 | regs 同名字段；ax 可能已被 handle_signal 的重启表改写（D03） | frame->uc.uc_mcontext（struct sigcontext_64） | restore_sigcontext 原样写回 regs 同名字段，不检查值 | 同 FF-01；恢复侧还要求 access_ok 通过 | signal_64.c:101（QD-L10）、signal_64.c:105（QD-L10）、AD3-09、SD-74、signal_64.c:111（QD-L10） |
| FF-06 | uc_mcontext.cx、r11 | regs->cx、regs->r11 | sigcontext.cx、sigcontext.r11 | 写回 regs->cx、regs->r11，但返回段把 rcx、r11 改装为 ip、flags，这两个值回不到用户寄存器 | 同 FF-05 | SD-18、AD3-09、SD-74、SD-49、SD-20、signal_64.c:107（QD-L10）、signal_64.c:112（QD-L10） |
| FF-07 | uc_mcontext.sp | regs->sp（入口保存的用户 rsp） | sigcontext.sp | regs->sp = sc.sp，不检查；返回段 popq %rsp 使用 | 同 FF-05 | SD-18、AD3-09、AD1-04、signal_64.c:104（QD-L10） |
| FF-08 | uc_mcontext.ip | regs->ip；重启改写后可能已是 ip-2（D03） | sigcontext.ip | regs->ip = sc.ip，不检查规范性；SYSRET 适用性判断结果被丢弃 | 同 FF-05；同一出口没有成功建立新信号帧（get_signal 为假，或建帧在改写寄存器之前失败） | signal_64.c:120（QD-L10）、AD3-09 |
| FF-09 | uc_mcontext.flags | regs->flags | sigcontext.flags | 只取 FIX_EFLAGS 覆盖的位（含 TF、RF）并入 regs->flags，其余位保留进入 15 号调用时的值 | 同 FF-05 | signal_64.c:121（QD-L10）、AD3-10、signal_64.c:12（QD-L10）、QD-108、signal_64.c:13（QD-L10） |
| FF-10 | uc_mcontext.cs、ss（各 16 位） | regs->cs、regs->ss | sigcontext.cs、sigcontext.ss | sc.cs、sc.ss 各与 3 按位或；恢复侧 force_valid_ss 被注释；返回段不读这两格 | 同 FF-05 | SD-18、signal_types_arch.h:30（QD-L11）、AD3-10、SD-75、signal_64.c:122（QD-L10）、signal_64.c:125（QD-L10） |
| FF-11 | uc_mcontext.gs、fs | 常量 0 | sigcontext.gs、sigcontext.fs | 读入后不用 | 同 FF-01 | signal_64.c:123（QD-L10）、signal_64.c:124（QD-L10） |
| FF-12 | uc_mcontext.trapno、err、cr2 | current->thread.trap_nr、error_code、cr2；三字段没有活动写入者，推断恒为 0 | sigcontext.trapno、err、cr2 | 读入后不用 | 同 FF-01 | AD3-11、SD-53、processor_types_arch.h:190（QD-L15）、QD-107、init_task.c:117（QD-L29）、AD3-02、signal_64.c:131（QD-L10） |
| FF-13 | uc_mcontext.fpstate | fp = get_sigframe 的 *fpstate：默认是 regs->sp-128（红区底部），不是已保存的 FPU 区 | sigcontext.fpstate（非零） | 读入 sc.fpstate 后不用；fpu__restore_sig 被注释 | 同 FF-01 | signal.c:74（QD-L09）、signal_64.c:127（QD-L10）、signal_types_arch.h:46（QD-L11）、signal_64.c:93（QD-L10） |
| FF-14 | uc_mcontext.oldmask | set->sig[0]（sigmask_to_save） | sigcontext.oldmask | 读入后不用 | 同 FF-01 | signal_64.c:185（QD-L10）、signal_64.c:162（QD-L10）、signal_64.c:130（QD-L10） |
| FF-15 | uc.uc_sigmask | set->sig[0]：sigmask_to_save 返回 current->blocked（置 restore_sigmask 的函数没有活动调用者，推断） | frame->uc.uc_sigmask（8 字节） | get_user 读入；set_current_blocked 去掉 SIGKILL、SIGSTOP 后写 tsk->blocked；在恢复寄存器之前执行；不重算 pending | 建帧侧同 FF-01；恢复侧要求 access_ok 通过 | QD-208、signal.h:566（QD-L32）、QD-102、signal_64.c:256（QD-L10）、signal_64.c:263（QD-L10）、signal.c:1122（QD-L31）、signal.c:1093（QD-L31）、signal_64.c:186（QD-L10） |
| FF-16 | info（siginfo） | ksig->info（出队时复制，或 collect_signal 合成） | frame->info | 不读 | 只在 SA_SIGINFO 时写；经 copy_siginfo_to_user→copy_to_user，有半区检查，但在主体裸写之后执行 | signal_64.c:190（QD-L10）、signal.c:1063（QD-L31） |
| FF-17 | orig_ax（不在帧中） | 不保存 | — | restore_sigcontext 置为 -1，返回尾的重启判断因此跳过（D03） | 只在 restore_sigcontext 执行时；badframe 路径上 orig_ax 仍为 15 | AD0-11 |
| FF-18 | FPU、SSE 与扩展状态 | 不保存：fpu__alloc_mathframe、copy_fpstate_to_sigframe、fpu__clear_user_states 都被注释 | —（帧里不预留浮点区） | 不恢复：fpu__restore_sig 被注释 | 无条件 | signal.c:74（QD-L09）、signal.c:105（QD-L09）、signal_64.c:93（QD-L10）、QD-106 |
| FF-19 | 影子栈 | 不保存：setup_signal_shadow_stack 被注释 | — | 不恢复：restore_signal_shadow_stack 被注释 | 无条件 | QD-109 |

## D03 系统调用重启条件（ipc.signal.syscall_restart）

本节回答：系统调用返回时，重启逻辑在什么条件下改写 `regs->ax` 和 `regs->ip`，改成什么值。

先划清边界。表中的“ip -= 2”和“写回重启号”，只是源码对 `pt_regs` 的改写。它们不等于系统调用被重新执行，更不等于重新执行成功。用户态回到哪条指令、是否再次执行，属于 ISA 语义和运行时行为，一律 NOT_RUN（NVR-3、NVR-1）。

文件简称：“arch signal.c”指 `mykernel/arch/x86_64/lock_IPC/signal/signal.c`；“signal.c”指 `mykernel/lock_IPC/signal/signal.c`；“signal_64.c”指 `mykernel/arch/x86_64/lock_IPC/signal/signal_64.c`。

### D03.1 判定的两个输入

重启逻辑有两处，判定方式相同：有 handler 时在 `handle_signal` 中（SD-27），没有 handler 时在 `arch_do_signal_or_restart` 中（SD-28）。两处都先判断“是否来自系统调用”，再按错误码分支。

**是否来自系统调用，看 orig_ax。** `syscall_get_nr` 返回 `int`，函数体只有一句（AD0-01）。`orig_ax` 是 64 位无符号的 `reg_t`。所以判定式是“orig_ax 转成 int 后不等于 -1”，不是“orig_ax ≥ 0”。转换只保留低 32 位，这是 GCC 的实现定义行为（推断）；函数上方的注释也说只有低 32 位有意义。

**错误码，看 ax。** `syscall_get_error` 以完整 64 位的 `regs->ax` 为来源（AD0-02）。函数中间的 `#ifdef CONFIG_IA32_EMULATION` 符号扩展段不编入：这个宏和 `CONFIG_X86_X32_ABI` 在 mykernel 中都没有 `#define`，也没有 `-D`（QD-34）。函数最后只做一次过滤（AD0-03）。`IS_ERR_VALUE` 的定义见 AD0-18，`MAX_ERRNO` 为 4095（值表）。结果是：
- ax 按有符号 64 位落在 [-4095, -1] 时，原样返回；
- 其余值一律返回 0，不命中任何重启分支。例如低 32 位是 -512、高 32 位为 0 的值，过滤后是 0。

**orig_ax 的来源。** 按源码，能走到重启判定的帧，orig_ax 有四种来源：

| 来源 | orig_ax | ax | 判定结果 | 证据 |
|---|---|---|---|---|
| SYSCALL 入口 | 用户 rax（64 位） | 同一个 rax；号按 uint 小于 453 时被分派结果覆盖 | 来自系统调用（rax 低 32 位为 0xFFFFFFFF 时除外） | CORE-USER-SYSCALL-STDIO-08::AS2-02、CORE-USER-SYSCALL-STDIO-08::AS2-05 |
| rt_sigreturn 成功恢复 | -1 | 用户帧中的 `sc.ax` | 不是来自系统调用（RR-01） | AD1-10 |
| fork 子任务 | 父任务帧的拷贝，即父任务的号 | 0 | 来自系统调用，但错误码为 0，不改写 | CORE-USER-BOUNDARIES-07::AB2-06 |
| 内核线程 | 0（整帧清零） | fn 返回后置 0 | 同上 | CORE-USER-BOUNDARIES-07::AB2-05、CORE-USER-BOUNDARIES-07::AB2-11 |

按字段名检索，活动 C 代码中给 orig_ax 赋值的只有 rt_sigreturn 恢复这一处（QD-42）。整帧拷贝和清零不以字段名出现，不在这次检索之内。

重启判定只在系统调用的返回尾部发生。`arch_do_signal_or_restart` 只有 entry/common.c 第 34 行一个活动调用点（QD-44，CORE-USER-BOUNDARIES-07::SB-15）；它所在的 `syscall_exit_to_user_mode` 只有 `do_syscall_64` 和 `ret_from_fork` 两个调用者（CORE-USER-BOUNDARIES-07::QB-43）。按名检索，中断和异常的返回路径都不经过这里。首个程序直接跳进返回段，也不经过（CORE-USER-BOUNDARIES-07::FP-1）。

### D03.2 重启码与相关常量

四个重启码、`SA_RESTART` 和 `__NR_restart_syscall` 在 mykernel 中各只有一处 `#define`（QD-45）。下表的值也由本包值表从源码文本重算。

| 名字 | 值 | 定义位置 | 证据 |
|---|---|---|---|
| `ERESTARTSYS` | 512 | `mykernel/include/linux/lib/errno.h` | AD0-19 |
| `ERESTARTNOINTR` | 513 | 同上 | AD0-20 |
| `ERESTARTNOHAND` | 514 | 同上 | AD0-21 |
| `ERESTART_RESTARTBLOCK` | 516 | 同上 | AD0-22 |
| `EINTR` | 4 | `mykernel/include/uapi/asm-generic/errno-base.h` | 值表 |
| `SA_RESTART` | 0x10000000 | `mykernel/include/uapi/asm-generic/signal-defs.h`（外层是 `#ifndef SA_RESTART`） | AD3-15 |
| `SA_RESTORER` | 0x04000000 | `mykernel/arch/x86_64/include/asm/signal.h` | 值表（与 RS-01 有关） |
| `__NR_restart_syscall` | 219 | `mykernel/arch/x86_64/include/asm/unistd_64.h` | AD0-04 |
| `__NR_rt_sigreturn` | 15 | 同上 | 值表；CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB |
| `MAX_ERRNO` | 4095 | `mykernel/include/linux/kernel/err.h` | 值表 |

补充三点：
- 515 是 `ENOIOCTLCMD`，不在任何一个重启 switch 中。
- switch 拿 `-ERESTARTSYS` 这类负数与 `syscall_get_error` 的 `long` 结果比较。所以 ax 必须是完整 64 位的 -512 等值才会命中。
- errno.h 在这组定义之前有一段注释，大意是：这些码不应让用户程序看到，返回它们之前必须已有待处理信号。这只是注释，活动代码不检查这条约定：重启判定每次返回都运行（CORE-USER-BOUNDARIES-07::SB-14、CORE-USER-BOUNDARIES-07::SB-15），而 TIF_SIGPENDING 没有可达的置位语句（RS-04）。

### D03.3 重启条件表

适用于 `time a039d9803ade`，`CONFIG_IA32_EMULATION`、`CONFIG_X86_X32_ABI` 都未定义。“来自系统调用”指 `(int)orig_ax != -1`；“选到 handler”指 `get_signal` 返回真；“错误码”指 `syscall_get_error` 的结果。facts 中的 restart_conditions（RR-01～RR-19）是同一张表的机器可读版本。

| 行 | 来自系统调用的判定 | 是否选到 handler | 错误码分支 | SA_RESTART | ax 变化 | ip 变化 | 重启号 | 源码 |
|---|---|---|---|---|---|---|---|---|
| RR-01 | 否：`(int)orig_ax == -1` | 任意 | 不进入 switch | 不检查 | 不改 | 不改 | 无 | SD-27、SD-28、AD0-01、AD1-10 |
| RR-02 | 是 | 是 | `-ERESTART_RESTARTBLOCK`（-516） | 不检查 | 改为 `-EINTR`（-4） | 不变 | 无（有 handler 时不写 219） | AD0-05 |
| RR-03 | 是 | 是 | `-ERESTARTNOHAND`（-514） | 不检查 | 改为 `-EINTR` | 不变 | 无 | AD0-05 |
| RR-04 | 是 | 是 | `-ERESTARTSYS`（-512） | 未置位 | 改为 `-EINTR` | 不变 | 无 | AD0-06 |
| RR-05 | 是 | 是 | `-ERESTARTSYS`（-512） | 置位 | `ax = orig_ax`（完整 64 位） | `ip -= 2` | 原号 | AD0-06、AD3-04 |
| RR-06 | 是 | 是 | `-ERESTARTNOINTR`（-513） | 不检查 | `ax = orig_ax` | `ip -= 2` | 原号 | AD3-04 |
| RR-07 | 是 | 是 | 其他：0、正值、-515 与其余负错误码、过滤成 0 的值 | 不检查 | 不变（没有 default） | 不变 | 无 | SD-29、SD-31 |
| RR-08 | 承接 RR-02～RR-07 | 是 | 建帧成功 | — | 先存入用户帧的 `sc->ax`，再置 0 | 先存入 `sc->ip`，再置为 `sa_handler` | 存入帧的改写要等 rt_sigreturn 恢复后才回到 `pt_regs`；那次返回落入 RR-01 | SD-40、SD-37、AD3-09 |
| RR-09 | 承接 RR-02～RR-07 | 是 | 建帧失败：没有 SA_RESTORER，或 SA_SIGINFO 时复制 siginfo 失败 | — | 保留改写，不回滚 | 保留改写 | 同 RR-02～RR-07；handler 不运行，已出队的信号丢失 | AD0-13、AD0-14、SD-15 |
| RR-10 | 是 | 否 | `-ERESTARTNOHAND`（-514） | 不适用（没有 `ksig.ka`） | `ax = orig_ax` | `ip -= 2` | 原号 | AD0-08、AD0-10 |
| RR-11 | 是 | 否 | `-ERESTARTSYS`（-512） | 不适用 | `ax = orig_ax` | `ip -= 2` | 原号 | AD0-10 |
| RR-12 | 是 | 否 | `-ERESTARTNOINTR`（-513） | 不适用 | `ax = orig_ax` | `ip -= 2` | 原号 | AD0-10 |
| RR-13 | 是 | 否 | `-ERESTART_RESTARTBLOCK`（-516） | 不适用 | `ax = get_nr_restart_syscall(regs)`，即 219 | `ip -= 2` | 219，活动表中没有此项（D03.4） | AD1-09、AD3-06 |
| RR-14 | 是 | 否 | 其他：0、正值、其余负错误码、过滤成 0 的值 | 不适用 | 不变（没有 default） | 不变 | 无 | SD-30、SD-32 |
| RR-15 | — | — | ia32 兼容任务的重启号 | — | 未实现：`#ifdef CONFIG_IA32_EMULATION` 段不编入，它读的 `restart_block` 成员也已被注释 | — | 未实现 | QD-34、QD-36 |
| RR-16 | — | — | x32 任务的重启号 | — | 未实现：`#ifdef CONFIG_X86_X32_ABI` 段不编入，只取 `#else` | — | 未实现 | QD-34、AD3-06 |
| RR-17 | — | — | restart_block 续做（`sys_restart_syscall`） | — | 未实现：成员、初值、复位和系统调用声明都在注释中，也没有 `struct restart_block` 的定义 | — | 未实现 | QD-35、QD-37 |
| RR-18 | — | 否 | 没有信号时恢复 saved_sigmask | — | 未实现：调用被注释 | — | — | SD-35 |
| RR-19 | — | — | 改写之后，用户态是否回到 syscall 指令、是否再次执行、是否成功 | — | 只确认改写后的 ax 由 `popq %rax` 装入 rax | 只确认改写后的 ip 经 RCX 槽由 `popq %rcx` 装入 rcx（推断） | 未知（NOT_RUN） | QD-43、CORE-USER-BOUNDARIES-07::AB2-16 |

表的读法：

1. **“选到 handler”是什么意思。** 出队信号的动作既不是 `SIG_IGN` 也不是 `SIG_DFL` 时，`get_signal` 才把动作拷入 `ksig->ka` 并返回真（AD3-18）。返回真时，`arch_do_signal_or_restart` 调用 `handle_signal` 后直接 return，不再执行无 handler 的 switch（AD0-08）。`get_signal` 中没有活动的 `do_group_exit` 调用（SD-36）：默认动作为终止、停止或转储的信号只置 `PF_SIGNALED`，被消耗后循环继续出队（RS-03）。所以，只要出队的信号都没有用户 handler，`get_signal` 就返回假，默认动作为终止的信号也一样。这时走 RR-10～RR-14。
2. **SA_RESTART 从哪里来。** `ksig->ka` 是出队时从 `sighand->action[]` 拷来的副本（同上一锚点）。这张表由 `do_sigaction` 写入，写入前按 `UAPI_SA_FLAGS` 掩码，而 `UAPI_SA_FLAGS` 含 `SA_RESTART`（ED-07，ipc.signal.action_mask_syscalls）。因此这一位会保留（推断，未经预处理核实）。fork 产生的任务都共用 `init_sighand`（RS-12，CORE-USER-VFS-BASELINE-05::UV-14），所以这一位实际上也被这些任务共用（推断）。
3. **有 handler 时的先后。** 改写发生在建帧之前。建帧成功时（RR-08），改写后的 ax、ip 先被存进用户栈上的帧（SD-40），然后 ax、ip 等字段被改成投递 handler 用的值（SD-37）。所以 -EINTR 或“原号、ip - 2”要等 rt_sigreturn 从用户帧恢复之后，才回到 `pt_regs`；恢复时 orig_ax 被置为 -1，那一次返回不会再改写（RR-01）。handler 在用户态可以改动帧，恢复出的值不一定是存进去的值（用户态行为，未追）。建帧失败时（RR-09），`signal_setup_done` 不在活动代码中（SD-15）：改写原样留在 `pt_regs`，handler 不运行，已出队的信号丢失（RS-02）。若 RS-01 成立（needs_evidence，NVR-2），经 rt_sigaction 注册的 handler 都会在 SA_RESTORER 检查处失败，有 handler 的各行就都落到 RR-09（推断）。
4. **两个 switch 的形状。** 两个 switch 各有 4 个 ERESTART case（SD-29、SD-30），都没有 default（SD-31、SD-32）。“ip -= 2”在 `handle_signal` 中出现 1 次，在 `arch_do_signal_or_restart` 中出现 2 次（SD-33、SD-34）。没有信号时恢复 saved_sigmask 的调用被注释（SD-35），saved_sigmask 的写入者没有追。

两段关键原文。有 handler 时，带 SA_RESTART 的 -ERESTARTSYS 与 -ERESTARTNOINTR：

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::handle_signal]
```c
			fallthrough;
		case -ERESTARTNOINTR:
			regs->ax = regs->orig_ax;
			regs->ip -= 2;
			break;
```

没有 handler 时的 -ERESTART_RESTARTBLOCK：

[VERIFIED mykernel/arch/x86_64/lock_IPC/signal/signal.c::arch_do_signal_or_restart]
```c
		case -ERESTART_RESTARTBLOCK:
			regs->ax = get_nr_restart_syscall(regs);
			regs->ip -= 2;
			break;
```

### D03.4 重启号 219 与活动分派表

- 两个 `#ifdef` 段都不编入，`get_nr_restart_syscall` 实际只剩 `#else` 中的一句（AD3-06，QD-34）。
- `__NR_restart_syscall` 为 219（AD0-04），也是 unistd_64.h 中唯一取值 219 的名字（QD-33）。
- 活动表 `syscalls_64.h` 和分派文件 `syscall_64.c` 中都没有 219，也没有 `__NR_restart_syscall`（QD-32）。08 重算的 41 项活动表中同样没有 219（CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB）。本包值表另按源码重算 `__NR_restart_syscall` 与 `__NR_rt_sigreturn` 是否在活动表中。
- 推断：如果用户态以 219 再次进入内核，219 < 453，会进入 `x64_sys_call`（CORE-USER-SYSCALL-STDIO-08::AS2-05）；switch 中没有对应的 case，落到 default，进入 `__x64_sys_ni_syscall`，打印后 `while (1)`（CORE-USER-SYSCALL-STDIO-08::AS2-08、CORE-USER-SYSCALL-STDIO-08::AS2-06）。这与用户直接发出 219 号的结果相同（CORE-USER-BOUNDARIES-07::UB-09）。所以在当前树中，它是一处潜伏的不一致（RS-05）：重启分支写回的号没有处理函数。
- restart_block 续做机制不在活动代码中。task_struct 的成员、init_task 中的初值、`restore_sigcontext` 中的复位、`sys_restart_syscall` 的声明，全部是注释（QD-35）。mykernel 中没有 `struct restart_block` 的定义（QD-37）。`restart_block` 唯一的非注释出现在 arch signal.c 第 228 行，位于不编入的 `#ifdef CONFIG_IA32_EMULATION` 段内（QD-36）。`sys_restart_syscall` 的声明行还位于 syscalls.h 第 316—1273 行的 `#ifndef CONFIG_ARCH_HAS_SYSCALL_WRAPPER` 段内，而构建定义了这个宏（CORE-USER-SYSCALL-STDIO-08::AS2-19）。

### D03.5 重启码能否进入某次系统调用的 regs->ax

重启表只在 ax 恰为四个 64 位负值之一时改写。下面按来源逐项核对。

| 来源 | 结论 | 依据 |
|---|---|---|
| completion.h 第 52 行，`do_wait_for_common` 的 `-ERESTARTSYS` | 不能。它只在 `signal_pending_state` 为真时产生。`wait_for_common` 唯一的活动调用来自 `wait_for_completion`，传入 `TASK_UNINTERRUPTIBLE`（AD3-16）。对这个状态，`signal_pending_state` 在读任何 TIF 位之前就返回 0（AD3-20）。`wait_for_completion` 返回 void，也丢掉了这个值。此外 TIF_SIGPENDING 没有可达的置位语句（RS-04）。 | QD-39、QD-41 |
| signal.h 第 464 行，`restart_syscall()` 的 `-ERESTARTNOINTR` | 不能：按名检索没有调用。 | QD-39、QD-38 |
| `-ERESTARTNOHAND`、`-ERESTART_RESTARTBLOCK` | 没有按名字的活动生产者，exec.c、fork.c、fs/syscall.c 中的出现都是注释。也没有以字面量 -512、-513、-514、-516 返回或赋值的代码。 | QD-39、QD-40 |
| 越界号：号按 uint 不小于 453 | 能。分派不写 ax，ax 保持入口压入的完整 64 位用户 rax；把 ax 预置为 -ENOSYS 的写法被注释。用户 rax 恰为四个值之一时命中重启表。这是目前已知、能由源码确定构造的唯一途径（推断）。 | CORE-USER-SYSCALL-STDIO-08::AS2-02、CORE-USER-SYSCALL-STDIO-08::AS2-03、CORE-USER-SYSCALL-STDIO-08::AS2-05 |
| 没有活动 return 的处理函数：`sys_wait4`（函数体为空）、`sys_dup`、`sys_dup2`；`sys_lseek` 经 `f_op->llseek` 可达的 `blkdev_llseek`、`dcache_dir_lseek`（fops.c 第 271 行、libfs.c 第 269 行把它们挂到 `.llseek`） | 不能排除。这些函数的返回值在 C 语义下没有定义，写进 ax 的值静态上无法确定（推断）。 | CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB、SD-41、SD-42 |
| 按计算得出的返回值 | 没有穷尽。上面的检索只按名字和字面量。 | — |
| rt_sigreturn 恢复出 `sc.ax` = -516 等值 | 不进入重启 switch：恢复时 orig_ax 已置为 -1（RR-01）。 | AD1-10 |
| fork 子任务、内核线程 | 错误码为 0，不命中。 | CORE-USER-BOUNDARIES-07::AB2-06、CORE-USER-BOUNDARIES-07::AB2-11 |

**越界号这条途径的后果。**
- 没有 handler 时：
  - -516：帧 ax 改为 219，ip 减 2（RR-13）。这是源码事实。之后若以 219 再次进入，会停在 `__x64_sys_ni_syscall` 的 `while (1)`（推断，D03.4）。
  - -512、-513、-514：帧 ax 写回同一个值，只有 ip 减 2（RR-10～RR-12）。“回到同一条 syscall 指令、带着同一个 rax 再进内核”是按 ISA 作的推断（NOT_RUN），而且要求每次再进入时 `get_signal` 都为假。
- 有 handler 时（推断）：-516、-514 与不带 SA_RESTART 的 -512 得到 -EINTR，ip 不变（RR-02～RR-04）；其中 -516 此时不写 219（RR-02），也就不会走到 D03.4 的 `while (1)`。建帧成功时，-EINTR 写进 ax 并存入用户帧，handler 经 rt_sigreturn 返回后，它成为这次调用的返回值；建帧失败时，-EINTR 留在 pt_regs，由这次返回段直接装入 rax（见下一条）。两种情况下，对 -514 与不带 SA_RESTART 的 -512，上面的循环都到此结束。-513 与带 SA_RESTART 的 -512 在建帧成功时经 rt_sigreturn、建帧失败时在这次返回，又回到 ip - 2 处。默认动作为终止的信号（例如 SIGKILL）打断不了这个循环（RS-03）。
- 建帧失败时（RS-02；若 RS-01 成立则总是如此），改写直接留在帧中，不回滚。
- 源码只改写调用者自己的 `pt_regs`。对同一 CPU 上其他任务的影响没有分析（中断返回处的调度没有追）。

**与旧结论的关系。** CORE-USER-BOUNDARIES-07::UB-09 说“号不小于 453 时……用户得到原系统调用号”。这句话只在分派这一步成立。对完整 64 位 rax 恰为 -512、-513、-514、-516 的四个值，返回尾部会改写帧。本包把这一交互作为对 UB-09 的证据追加，并以勘误 ER-03 限定原句；07 原文件不改。07 中 entry.syscall_frame_return 的 failure_path（号不小于 __NR_syscalls 时 ax 保持原值）在 `do_syscall_64` 这一点仍然成立，但它不是返回段最终装入的值。08 中同义的表述（§0 第 2 条、§3.2 表最后一行、entry.syscall_dispatch 的 failure_path）按 ER-02 读作分派局部结果，处置见 stdio-08-disposition.md。

### D03.6 留给运行与外部资料的问题

- 改写后的寄存器在运行时是否如源码所示被装入、用户态是否再次执行、结果如何：NVR-1（构建并在 QEMU 中观察；未授权，NOT_RUN）。
- `UAPI_SA_FLAGS` 是否含 SA_RESTORER（RS-01），以及 SA_RESTART 位是否保留：NVR-2（预处理或编译 signal.c 翻译单元）。
- ip - 2 是否正好落在 2 字节的 syscall 指令上、sysretq 是否以 rcx 为返回地址：NVR-3（ISA 一手资料）。
- libc 是否经 rt_sigaction 设置 SA_RESTART 与 SA_RESTORER、是否会以这四个值作为 rax 发出 syscall：NVR-4（读仓库外实际链接的 libc）。

## 衔接：D01–D03 接回 07 的返回帧与 08 的分派局部表

07 把 `arch_do_signal_or_restart` 的内部列为未覆盖的端点（CORE-USER-BOUNDARIES-07::entry.syscall_frame_return 的 not_covered），08 也把信号处理内部列为未覆盖（CORE-USER-SYSCALL-STDIO-08::entry.syscall_dispatch 的 not_covered）。D01–D03 补上的是这段端点的静态内容。下面先给出一次返回经过的四段，再分五种情形写：普通的未改写返回、信号投递改写帧、重启分支、rt_sigreturn 之后的再处理、失败或不返回的路径。

### L1 一次返回经过的四段

主线对 08 的审查回执 SR08-02 要求区分分派局部的 ax 与最终返回值。08 §3.2 表中“用户看到的 rax”一列，按 ER-02 读作“分派之后的 regs->ax”。段号沿用 stdio-08-disposition.md §3.2：① 分派前（入口建帧，07、08 已确立，下表从略）、② 分派后、③ 返回尾部、④ 返回段。D01.7、D01.10 返回阶段表的 ②、③ 与此相同，但那里另把 SYSRET 判断记为 ④、返回段记为 ⑤。从分派到用户态，同一块帧还要经过后面几段：

| 段 | 做什么 | 可能改写的帧字段 | 证据 |
|---|---|---|---|
| ② 分派后（08） | 号按 uint 小于 453 时 `regs->ax = x64_sys_call(...)`；否则不写。处理函数本体也可能改写同一块帧（CORE-USER-BOUNDARIES-07::FP-3） | ax；15 号成功恢复时另按用户帧改写 15 个通用寄存器与 ip、sp、cs、ss、flags，并把 orig_ax 置为 -1（RT-03、RT-04）；59 号走到 START_THREAD 时改 ip、sp、cs、ss、flags（RT-36） | CORE-USER-BOUNDARIES-07::AB2-14、CORE-USER-BOUNDARIES-07::FP-3、RT-03、RT-04、RT-36 |
| ③ 返回尾部·调用链（D01，entry.syscall_return_work；ED-01、ED-02） | `do_syscall_64` → `syscall_exit_to_user_mode` → `exit_to_user_mode_loop` → `arch_do_signal_or_restart`，中间没有活动门禁 | 本身不改，交给下一行 | CORE-USER-BOUNDARIES-07::QB-43、CORE-USER-BOUNDARIES-07::QB-65、CORE-USER-BOUNDARIES-07::AB2-12、CORE-USER-BOUNDARIES-07::SB-14、CORE-USER-BOUNDARIES-07::SB-15 |
| ③ 返回尾部·信号投递（D02）或重启（D03） | `get_signal` 为真：重启改写后建帧；为假：无 handler 的重启 switch | 建帧成功：ax、di、si、dx、ip、sp、cs，flags 清 DF/RF/TF，必要时改 ss；重启：ax、ip | SD-37、SD-39、SD-38、AD0-10、AD1-09 |
| ④ 返回段（CORE-USER-BOUNDARIES-07::FP-3；ED-03） | xchg 把帧中的 flags、ip 换到 R11、RCX 槽；POP_REGS 装入 15 个寄存器；跳过 5 格；popq 取 sp；sysretq | R11、RCX、EFLAGS、RIP 四槽被 xchg 改写（187–192 行净效果：R11 槽←flags，RCX 槽←ip，EFLAGS 槽←原 r11 槽，RIP 槽←原 cx 槽；后两格在 POP_REGS 之后被 addq 跳过，不再读）；其余槽不写 | CORE-USER-BOUNDARIES-07::AB2-16、CORE-USER-BOUNDARIES-07::SB-02、QD-43、RT-28～RT-31 |

`do_syscall_64` 在 ③ 之后计算的 SYSRET 适用性结果（D01.10 的 ④）不被汇编使用（CORE-USER-BOUNDARIES-07::SB-16、CORE-USER-BOUNDARIES-07::SB-01）。所以 ③ 写入的值都经同一个返回段带走，源码里没有按检查结果改走的另一条退出路径。

08 §3.2 各行接上 ③ 之后的样子：

| 08 §3.2 中号的情况 | 分派后的 ax | ③ 可能命中的行 | 返回段装入 rax 的值（源码层面） |
|---|---|---|---|
| 在表中，处理函数返回 | 处理函数的返回值 | 按名字和字面量检索，没有活动处理函数返回重启码，落在 RR-07 或 RR-14；15 号 rt_sigreturn 成功恢复时 orig_ax 为 -1，落在 RR-01；没有活动 return 的处理函数除外（D03.5） | 没有投递 handler 时是分派值；投递成功时是 0（交给 handler）；handler 经 rt_sigreturn 返回后，是从用户帧恢复出的值 |
| 小于 453 但不在表中 | — | 不返回，到不了 ③ | — |
| 452 | -ENOSYS | RR-07 或 RR-14，不改写 | 没有投递 handler 时为 -ENOSYS |
| 不小于 453，或负号 | 完整 64 位的用户 rax，未被写 | 低 32 位为 0xFFFFFFFF：RR-01；恰为四个 64 位值之一：RR-02～RR-06 或 RR-10～RR-13；其余：RR-07 或 RR-14 | 见 D03.5 |

### L2 普通的未改写返回：前提

下面四条同时成立，是“③ 不改写帧、返回段装入 rax 的就是分派写入的值”的充分条件，不是必要条件（编号与 stdio-08-disposition.md §3.3 相同）。第 1、3、4 条也是必要的（越界号时第 1 条换成“(uint)(int)eax ≥ 453，分派不写 ax，控制到达 ③”，分派值换成入口的 64 位 rax），第 2 条不是；第 2 条放宽后的充要条件，以及只比数值时的例外，见本节末段：
1. 号在表内，处理函数返回，控制到达 ③（表内缺项进入 `while (1)`，不返回；exit 在 `do_exit` 之后没有 return，按 08 的“若返回”写法处理，见 L6）。
2. 这次返回时 `get_signal` 为假（AD0-08）。注意 `get_signal` 每次都会运行并加解 siglock（RS-04），出队的默认动作信号会被消耗（RS-03）。
3. 重启 switch 不命中：(int)orig_ax == -1，或者错误码不是 -512、-513、-514、-516（SD-30）。
4. 返回尾部返回，执行按位置落入 `syscall_return_via_sysret`（CORE-USER-BOUNDARIES-07::SB-01）。

满足时，③ 不改写帧，返回段装入 rax 的就是分派写入的值。ip、flags 两槽经 xchg 落到 RCX、R11 槽，再由 `popq %rcx`、`popq %r11` 装入 rcx、r11，所以装入的是分派之后帧中 ip、flags 槽的值（推断：按 xchg 与 POP_REGS 的顺序推演，CORE-USER-BOUNDARIES-07::AB2-16、QD-43）。处理函数在分派中没有改写这两槽时，它们才是入口 `pushq %rcx`、`pushq %r11` 压入的值。上面几条都满足、这两槽却已被改写的情形至少有两种：15 号 rt_sigreturn 成功恢复时（不走 badframe），ip 取用户帧的 sc.ip，flags 为 (原 flags & ~FIX_EFLAGS) | (sc.flags & FIX_EFLAGS)，orig_ax 被置为 -1，正好满足第 3 条（RT-03、AD3-09、AD1-10）；59 号 execve 成功时，START_THREAD 把 ip 写为 elf_entry、flags 写为 X86_EFLAGS_IF | X86_EFLAGS_FIXED，处理函数返回 0，不是重启码（推断：只注册了 ELF 一种格式，load_elf_binary 在 START_THREAD 之后置 retval = 0 并原样返回；RT-36、CORE-USER-BOUNDARIES-07::AB2-15、CORE-USER-BOUNDARIES-07::QB-38、CORE-USER-BOUNDARIES-07::FP-3）。sysretq 之后用户态看到什么，NOT_RUN。

**必要性与充要条件。** 这里的“未改写”指 ③ 不写 pt_regs 帧的任何字段，并且到达返回段。第 1 条（越界号时换成“(uint)(int)eax ≥ 453，分派不写 ax，控制到达 ③”）与第 4 条是必要的：不成立就没有这次返回，例如建帧写用户内存时访问出错、缺页处理又不能解决，推断控制留在内核，落到已登记的自旋或反复缺页（属硬件语义，RS-08、ED-20）。第 3 条也是必要的：它不成立时，这次运行的那个重启 switch（`get_signal` 为真时在 `handle_signal` 里，为假时在 `arch_do_signal_or_restart` 里）必然命中一个 case，而每个 case 都写帧：有 handler 时 ax 由重启码改为 -EINTR（ip 不变），或者 ax = orig_ax 且 ip -= 2（SD-14）；无 handler 时 ax 写为 orig_ax 或 219，ip 都减 2（SD-02、AD1-08、AD1-09）；之后即使建帧失败也不回滚（RR-09）。第 2 条不是必要的：例如 handler 注册时没有 SA_RESTORER，`x64_setup_rt_frame` 在 169 行、写用户信号帧和寄存器之前就返回 -EFAULT（AD0-13）；ax 不是重启码时，`handle_signal` 的 switch 也不改它；这时帧的字段都不变，ax 仍是分派值，只是信号已出队而丢失（RR-07 接 RR-09，RS-02）。第 2 条可以放宽为“没有成功建立信号帧”，即“`get_signal` 为假，或建帧在改写寄存器之前失败”：`get_signal` 本身不读写帧（QD-11）；它为真时，`x64_setup_rt_frame` 若在 169 行（缺 SA_RESTORER，AD0-13）或 191 行（带 SA_SIGINFO 时 copy_siginfo_to_user 失败，AD0-14）返回 -EFAULT，就到不了 198 行起的寄存器改写（AD0-15、SD-37、SD-38）；第三个 -EFAULT（236 行，Efault 标号下）只能由 178–186 行的写入跳入，同样不经过寄存器改写，而且按 AD2-02 与 RS-08 到不了（SD-69）；三处失败返回都只能在改写之前到达，所以“建帧失败”就是“建帧在改写寄存器之前失败”；失败时 `handle_signal` 也不清 flags，清 DF、RF、TF 只在成功分支（AD0-07、SD-39）。所以，第 1、3、4 条与放宽后的第 2 条同时成立，是“③ 不写 pt_regs 帧的任何字段、并到达返回段”的充要条件；这时返回段装入 rax 的就是分派写入的值，越界号时是入口的 64 位 rax（与 stdio-08-disposition.md §3.3 一致）。这个充要条件，以及本包其他以“没有成功建立信号帧”（含“建帧失败”）为前提的说法（D01.6、D02-d、L5、L8、ED-18 等），都不计 RS-08 的一种地址重叠（推断）：191 行这一支按 D02-c 只在帧内 siginfo 的地址被地址检查拒绝（常量分支下即高半区地址）时出现，而 178–186 行的裸写入在此之前已经执行，所写地址若恰与本次的 pt_regs 帧重叠，帧也会被改到。只比 rax 的数值时，还有三种数值相等而帧已被改写的情形，都不算未改写返回：建帧成功而分派值恰为 0（ip、sp 等已改）；越界号的 64 位 rax 恰为 -512、-513、-514 且 `get_signal` 为假（ax 写回同值，ip 减 2）；越界号的 64 位 rax 恰为 -513，或带 SA_RESTART 时恰为 -512，`get_signal` 为真而建帧失败（`handle_signal` 写回同值，ip 减 2）。表内号没有与后两种对应的情形（推断：表内号的 orig_ax 低 32 位按 uint 小于 453，与这四个 64 位值都不相等，所以重启 switch 命中后 ax 不会等于分派值；按字段名给 orig_ax 赋值的活动语句只有置 -1 一处，QD-42）。若 RS-01 成立（needs_evidence，NVR-2），经 rt_sigaction 安装的 handler 都在 169 行返回，放宽后的第 2 条恒成立，充要条件就只剩第 1、3、4 条（推断）。sysretq 之后用户态实际拿到什么仍属 NOT_RUN。

### L3 信号投递怎样改写帧（ED-05、ED-19）

`handle_signal` 先做重启改写（RR-02～RR-07），再经 `setup_rt_frame` 调用 `x64_setup_rt_frame`（ED-05）。成功路径上：
- 先把当时的寄存器（包括改写后的 ax、ip）写进用户栈上的 sigcontext（SD-40）。这些写入前没有 `access_ok`，`Efault` 标号不可达（RS-08，AD2-02；以 RS-01 不成立为前提）；写入出错时也没有缺页修复，但这是全内核的情况，不只在信号帧（QD-76；§0 第 3 条）；ucontext 中的 uc_flags 是未初始化的值（RS-09）；FPU 状态不保存（RS-10）。
- 带 SA_SIGINFO 时，复制 siginfo 失败会直接返回 -EFAULT（AD0-14），走 RR-09。
- 然后改写 7 个帧字段：di = 信号号，ax = 0，si = &frame->info，dx = &frame->uc，ip = sa_handler，sp = frame，cs = __USER_CS（SD-37）；ss 不等于 __USER_DS 时还调用 `force_valid_ss`（SD-38）。
- 回到 `handle_signal`，成功时清掉 flags 中的 DF、RF、TF（SD-39）。

返回段随后消费这块帧（ED-19）：rcx 取 sa_handler，rsp 取 frame，r11 取清过标志的 flags，rax 取 0（推断：CPU 以 rcx 为返回地址属 ISA 语义）。返回段不读 cs、ss 两个槽（CORE-USER-BOUNDARIES-07::FP-3 的 fields_not_consumed），所以对它们的写入不经返回段起作用。

### L4 重启分支（ED-06、ED-17）

- 无 handler（RR-10～RR-13）：直接改写同一块帧。改写后的 ax 由 `popq %rax` 装入 rax；ip 经 xchg 落到 RCX 槽，再由 `popq %rcx` 装入 rcx（QD-43、CORE-USER-BOUNDARIES-07::AB2-16；推断）。用户态是否因此回到 syscall 指令并再次执行，NOT_RUN（NVR-3）。
- -516 写回的 219 没有活动表项（ED-17 记为 blocked；RS-05）。
- 有 handler（RR-02～RR-07）：建帧成功时（RR-08），改写先存进用户帧，要到 rt_sigreturn 才回到 `pt_regs`（L5）；建帧失败时（RR-09：没有 SA_RESTORER，或 SA_SIGINFO 时复制 siginfo 失败），改写不回滚，留在 `pt_regs`，改写后的 ax、ip 由本次返回段装入 rax、rcx（推断同上），handler 不运行，已出队的信号丢失（RS-02）。若 RS-01 成立，经 rt_sigaction 注册的 handler 都走失败这一支（推断，NVR-2）。

### L5 rt_sigreturn 之后的再处理（ED-15、ED-16、ED-18）

- 15 号是活动表项（CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB；本包值表的表成员检查）。
- 恢复寄存器之前，`rt_sigreturn` 先写回信号掩码（ED-16）。`restore_sigcontext` 从用户帧写回 ax、sp、ip 等字段（AD3-09）；flags 只取 FIX_EFLAGS 所列的位；最后把 orig_ax 置为 -1（AD1-10）。
- `sys_rt_sigreturn` 返回 `regs->ax`（SD-24），`do_syscall_64` 再把同一个值写回 `regs->ax`（CORE-USER-BOUNDARIES-07::AB2-14）。
- 随后照常进入 ③，`arch_do_signal_or_restart` 再运行一次（ED-18）：
  - `get_signal` 为真：orig_ax 为 -1，`handle_signal` 跳过重启 switch（RR-01），然后在恢复后的上下文上建新帧；建帧成功时 ip、sp 等被改写（L3），建帧失败时恢复出的值原样到达返回段（L8）。
  - `get_signal` 为假：同样跳过重启 switch（RR-01），返回段消费恢复出的 ax、ip、flags、sp。
- 所以有 handler 时的 -EINTR 或“原号、ip - 2”，在建帧成功时（RR-08）要到这一步才到达返回段；建帧失败时（RR-09）它们已由当次返回段装入（L4）。
- 失败分支：
  - 帧地址通不过 access_ok 时走 badframe，只返回 0（RS-07）。这时 orig_ax 仍是 15，ax 为 0，重启 switch 不命中。
  - 帧地址能通过 access_ok、但所读地址在页表中没有映射或是非规范地址时，读用户内存没有容错，回不到 badframe（RS-06）。之后的去向按地址分段，都是推断（#PF、#GP 是否产生属硬件语义，未核）：小于 TASK_SIZE_MAX 且不在任何 VMA 内，经 ED-12、ED-20 停在 `myos_bad_area` 的 `while (1)`（AD2-07）；在 TASK_SIZE_MAX 到 2^47 之间，停在 `do_kern_addr_fault` 的 `while (1)`（AD2-06）；2^47 以上、符号位为 0 的非规范地址不走缺页，产生 #GP 后停在 `excep_gen_prot` 的 `while (1)`（AD2-09）。地址小于 TASK_SIZE_MAX 而落在某个 VMA 内时，缺页交给 `myos_handle_mm_fault`，不进 `myos_bad_area`：结果非 0 时在 `do_user_addr_fault` 中自旋（RS-06 的反证、RS-08），为 0 时之后的去向与该函数内部本批都未追。
- 用户给出的帧决定恢复后的 ip、sp、cx、r11 以及 flags 中的 FIX_EFLAGS 位，而被丢弃的 SYSRET 检查查的正是其中几项。这一点作为对 CORE-USER-BOUNDARIES-07::UB-09 的证据追加，不另立条目。

### L6 失败或不返回的路径

| 路径 | 源码层面的结果 | 条目 |
|---|---|---|
| 号小于 453 但不在表中；以重启写回的 219 再次进入（推断） | `__x64_sys_ni_syscall` 打印后 `while (1)`，不返回 | CORE-USER-BOUNDARIES-07::UB-09、RS-05 |
| 越界号的 rax 恰为 -512、-513、-514 | 没有 handler 时（get_signal 为假）帧 ax 写回同一个值，ip - 2；是否原地反复进出内核属推断。有 handler 时先按 RR-03～RR-06 改写（-514 与不带 SA_RESTART 的 -512 得到 -EINTR，-513 与带 SA_RESTART 的 -512 仍是 ip 减 2），再按 RR-08（建帧成功）或 RR-09（建帧失败） | UB-09 的证据追加、ER-03 |
| 建帧失败 | 返回用户态，但 handler 不运行，信号丢失，重启改写保留 | RS-02（RS-01 成立时所有 handler 都如此） |
| 默认动作为终止、停止或转储 | 只置 PF_SIGNALED，任务照常返回用户态 | RS-03 |
| 信号路径在内核态访问用户地址：建帧写入、sigreturn 读帧、copy_from_user | 访问出错时没有 -EFAULT 出口。去向按地址分，都是推断（#PF、#GP 是否产生属硬件语义）：小于 TASK_SIZE_MAX 且不在任何 VMA 内，停在 `myos_bad_area` 的 `while (1)`；不小于 TASK_SIZE_MAX 的规范地址缺页，停在 `do_kern_addr_fault` 的 `while (1)`；非规范地址产生 #GP，停在 `excep_gen_prot` 的 `while (1)`；小于 TASK_SIZE_MAX 而命中 VMA 时交给 `myos_handle_mm_fault`，结果非 0 则在 `do_user_addr_fault` 中自旋（RS-08），为 0 时的去向本批未追。地址落在内核半区时，建帧写入与 copy_from_user 不会被 access_ok 拦下 | RS-06、RS-08、RS-11、ED-20 |
| exit | `sys_exit` 调 `do_exit` 后没有 return（08 已登记） | CORE-USER-SYSCALL-STDIO-08::VT-SYSTAB |
| 没有活动 return 的处理函数 | 写进 ax 的值没有定义 | CORE-USER-BOUNDARIES-07::UB-10、D03.5 |

### L7 与 07、08 条目的对应

| 07、08 中的条目 | D01–D03 补上的内容 |
|---|---|
| CORE-USER-BOUNDARIES-07::FP-3 的 frame_writer“syscall_exit_to_user_mode 可能改帧” | 改哪些字段、在什么条件下改：L1 的 ③、D03.3 |
| CORE-USER-BOUNDARIES-07::SB-14、CORE-USER-BOUNDARIES-07::SB-15 | 重启判定在每次经 `syscall_exit_to_user_mode` 的返回中都运行，不看任何待处理标志（RS-04） |
| CORE-USER-BOUNDARIES-07::SB-16、CORE-USER-BOUNDARIES-07::SB-01 | 信号投递和 rt_sigreturn 改写过的帧同样只经 sysretq 返回（rt_sigreturn 一项即对 UB-09 的第一条证据追加；第二条证据追加见下一行的四个 64 位值） |
| CORE-USER-BOUNDARIES-07::UB-09 | 四个 64 位值的例外：证据追加与 ER-03 |
| CORE-USER-SYSCALL-STDIO-08 §3.2，以及 entry.syscall_dispatch 的 failure_path | 读作分派局部结果：ER-02（处置见 stdio-08-disposition.md） |
| CORE-USER-SYSCALL-STDIO-08::entry.syscall_table | 重启写回的 219 不在表中：ED-17、RS-05 |
| 07 entry.syscall_frame_return 与 08 entry.syscall_dispatch 的 not_covered | entry.syscall_return_work 细化 CORE-USER-BOUNDARIES-07::entry.syscall_frame_return；ED-01～ED-03 把它接到 08 的分派与 07 的返回段 |

### L8 GAP-U04 与 GAP-U10

**CORE-USER-VFS-BASELINE-05::GAP-U04**（07 已收窄）：本包只追加静态证据，硬件和运行部分仍是 NOT_RUN。追加的证据有三条：
- 分派与返回段之间，返回尾部可以改写帧的 ax、ip、sp、di、si、dx、cs、flags，并可能经 `force_valid_ss` 改 ss（L1 的 ③）。
- 返回段从帧中取 ax 装入 rax，取 ip 经 RCX 槽装入 rcx，取 flags 经 R11 槽装入 r11（QD-43、CORE-USER-BOUNDARIES-07::AB2-16）。
- rt_sigreturn 成功后 orig_ax 为 -1，同一出口的两处重启 switch 都跳过（AD1-10、RR-01）。同一出口 get_signal 为假，或建帧失败（x64_setup_rt_frame 在改写寄存器之前返回负值）时，恢复出的 ip、sp 与按 FIX_EFLAGS 合并后的 flags（只有 FIX_EFLAGS 所列的位取自用户帧）原样到达返回段（AD3-09、AD1-10、ED-18）；同一出口建帧成功时，它们先存入新信号帧，ip、sp 被改为 sa_handler 与帧地址，flags 清掉 DF、RF、TF（RT-12、RT-13、RT-17；见 L5）。

仍然未知：sysretq 是否以 rcx 为返回地址、ip - 2 处是否正好是 2 字节的 syscall 指令、swapgs 与 sti 的语义、运行时的寄存器值。这些留给 NVR-3（ISA 一手资料）和 NVR-1（运行观察）；ISA 部分与 07 的 NVB-4 范围相同，生成偏移的数值仍按 07 的 NVB-3 处理。

**CORE-USER-VFS-BASELINE-05::GAP-U10** 保持 open。信号分析不能代替 libc 证据：libc 是否经 rt_sigaction 注册 handler、是否带 SA_RESTORER 与 SA_RESTART、restorer 是哪段代码、是否会以这四个值作为 rax 发出 syscall，都在仓库外。留给 NVR-4。

## 缺口状态

| 缺口 | 本批覆盖 | 静态已闭合 | 仍缺什么 | 状态 |
|---|---|---|---|---|
| GAP-U04（05；07 已收窄） | 只追加分派与返回段之间的静态证据：返回尾对帧的改写点逐项定位 | 改写点与 rt_sigreturn 恢复的去向 | sysretq/swapgs/sti 的硬件语义与 ip-2 是否正对 syscall 指令（NVR-3）；生成偏移数值（07 NVB-3）；运行值（NVR-1） | narrowed（不变） |
| GAP-U10（05） | 不读、不推断 libc；信号路径只到内核接口 | 无 | libc 是否注册 handler、用何 restorer、是否带 SA_RESTORER/SA_RESTART、是否发出越界号（仓库外，NVR-4） | open（不变） |

## 本批入库的普通问题（只列编号，正文见 deferred-findings.yaml）

RS-01 SA_RESTORER 在存入时被 UAPI_SA_FLAGS 清掉（推断，needs_evidence）；RS-02 handle_signal 先改写 ax/ip 再建帧，建帧失败无人处理，信号丢失，已做的重启改写保留；RS-03 默认终止、停止、转储动作只置 PF_SIGNALED；RS-04 TIF_SIGPENDING 机制两端停用，每次返回都无门禁地加解 siglock 并做一次出队检查（pending 为空时只是几次位图判断，不遍历队列）；RS-05 重启号 219 没有活动表项（潜伏）；RS-06 rt_sigreturn 读用户帧没有容错；RS-07 坏帧只返回 0；RS-08 帧主体写入前没有 access_ok，Efault 标号不可达（潜伏：以 RS-01 不成立为前提）；RS-09 uc_flags 未初始化即写入用户帧（潜伏：同样以 RS-01 不成立为前提）；RS-10 不保存、不恢复 FPU 与扩展状态（建帧侧以 RS-01 不成立为前提；恢复侧经用户直接发出 15 号调用可达）；RS-11 copy_from_user 的活动实现不做 access_ok；RS-12 fork 出的任务共用 init_sighand；RS-13 子任务继承父任务的私有 pending 位图（潜伏）；RS-14 dequeue_synchronous_signal 第二遍必然命中 still_pending（潜伏）。另向 CORE-USER-BOUNDARIES-07::UB-09 追加两条证据；勘误 ER-01、ER-02（08）与 ER-03（07）。FN-01、FN-02 只作事实记录。都不需要 Owner 现在决策。

## 交给主线的开放点（只形成候选，不在执行侧裁定）

1. RS-12 是否并入 CORE-USER-VFS-BASELINE-05::UV-14。
2. CORE-USER-BOUNDARIES-07::UB-09 的原句：本批用勘误 ER-03 与两条证据追加处理，也可改为新条目。
3. FN-01、FN-02 是否改登为潜伏条目。
4. RS-01 需构建侧核实（NVR-2）；它成立与否决定建帧侧事实在运行中是否可达。
5. 任务切换的 FPU 缺口只记在 RS-10 中（__switch_to 的活动文本中没有 switch_fpu，SD-91；struct fpu 成员被注释，QD-131），CORE-CHECK-01 的已有包都没有登记（MYOS2-DR-005 的 MANIFEST.md 高风险 correctness 第 5 条与 cards/fpu-xstate.md §4 有速查结论，不是延期条目）；任务书 §3 不展开调度器与 FPU/SIMD，是否在调度侧另立条目由主线决定。

## 覆盖与剩余边界索引（供后继，一页以内）

- **已覆盖（静态）：** 分派之后的返回尾与其门禁；rt_sigaction、rt_sigprocmask 的保存与校验；get_signal 的选取、默认动作与锁；建帧的地址计算、字段、寄存器改写与失败消费；rt_sigreturn 的帧来源、恢复与错误分支；重启条件、重启码定义、219 号与分派表；与 07 返回帧、08 分派局部表的衔接。
- **未覆盖：** 发送端与唤醒的完整语义（kill 只为前提定点追了 sys_kill 一条链，complete_signal、signal_wake_up 只核了活动调用状态，见 RS-03、RS-04、RS-14）；内核自身发信号未追；sigaltstack（131 号不在活动表）；FPU/XSAVE 内部；缺页处理在 VMA 存在时的去向（myos_handle_mm_fault 内部，以及它返回 0 之后）；copy_user_generic 的汇编后端与 alternatives；信号相关的调度与并发；libc 与镜像。
- **需要授权才能继续的：** NVR-1（构建 + QEMU + 测试镜像）、NVR-2（预处理/编译 signal.c）、NVR-3（ISA 一手资料）、NVR-4（读仓库外 libc），全部 NOT_RUN。
- **证据入口：** `facts-and-boundaries.yaml`（锚点 AD*、检索 QD*/QD-L*、结构检查 SD*、值表 VT-RESTART、返回阶段 RT-*、帧字段 FF-*、重启条件 RR-*、事实 FN-*、缺口 gap_status），`return-paths.dot`，`records/rs_check.json` 与 `records/queries.json`。
