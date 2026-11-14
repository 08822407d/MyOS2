# CORE-USER-FIRST-RUN-FRAME-67 — 源码原样短锚

固定源码为 time@a039d9803ade。以下标签每段引用单一真实函数、宏、结构体或对象定义中的连续1–5物理行，保留制表符与空行，且全部含至少一行有效非空文本。行号仅用于二次核查，不能替代符号归属和语义核验。编译、生成器、模拟器、运行中的硬件帧均 NOT_RUN。

## D00 与 A 源证据

### D01 — 替代原66 A10空围栏，真实task.stack成员 (L32-32)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		void			*stack;
```

### A01 — copy_thread入口及clone flags、stack局部来源 (L82-86)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
int copy_thread(task_s *p, const kclone_args_s *args)
{
	ulong clone_flags = args->flags;
	ulong sp = args->stack;
	ulong tls = args->tls;
```

### A02 — frame、fork_frame与childregs声明 (L87-90)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	task_kframe_s *frame;
	fork_frame_s *fork_frame;
	pt_regs_s *childregs;
	int ret = 0;
```

### A03 — task_pt_regs、container_of和frame绑定 (L92-94)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, fork_frame_s, regs);
	frame = &fork_frame->frame;
```

### A04 — bp整数加一、ret_addr及thread.sp写入 (L97-99)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bp = (reg_t)childregs + 1;
	frame->ret_addr = (reg_t)ret_from_fork_asm;
	p->thread.sp = (reg_t)fork_frame;
```

### A05 — 保存fs/gs源级前置操作 (L104-108)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	current_save_fsgs();
	p->thread.fsindex = current->thread.fsindex;
	p->thread.fsbase = current->thread.fsbase;
	p->thread.gsindex = current->thread.gsindex;
	p->thread.gsbase = current->thread.gsbase;
```

### A06 — 保存ES、DS选择子 (L110-111)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	savesegment(es, p->thread.es);
	savesegment(ds, p->thread.ds);
```

### A07 — PF_KTHREAD的清零帧及frame初始化 (L120-123)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
```

### A08 — kernel thread早退 (L123-125)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}
```

### A09 — 普通分支bx零、复制current帧和ax/sp覆盖 (L133-137)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;
```

### A10 — 普通任务中fn非空另一路入口 (L139-142)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(args->fn)) {
		/*
		 * A user space thread, but it doesn't return to
		 * ret_after_fork().
```

### A11 — fn非空清ip/sp并设置kframe (L150-154)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
		childregs->sp = 0;
		childregs->ip = 0;
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}
```

### A12 — CLONE_SETTLS所在语句是注释 (L156-158)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	// /* Set a new TLS for the child thread? */
	// if (clone_flags & CLONE_SETTLS)
	// 	ret = set_new_tls(p, tls);
```

### A13 — copy_thread普通出口ret (L163-164)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	return ret;
}
```

### A14 — 函数指针与参数写到bx和r12 (L38-41)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_arch.h::kthread_frame_init]
```c
		kthread_frame_init(task_kframe_s *frame, int (*fun)(void *), void *arg) {
			frame->bx = (reg_t)fun;
			frame->r12 = (reg_t)arg;
		}
```

### A15 — inactive帧前四个保存寄存器 (L12-16)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
	struct inactive_task_frame {
		reg_t	r15;
		reg_t	r14;
		reg_t	r13;
		reg_t	r12;
```

### A16 — inactive帧bx字段 (L17-17)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
		reg_t	bx;
```

### A17 — inactive帧bp和ret_addr (L22-24)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
		reg_t	bp;
		reg_t	ret_addr;
	};
```

### A18 — fork_frame对象中frame和regs顺序 (L26-29)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::fork_frame]
```c
	struct fork_frame {
		task_kframe_s	frame;
		pt_regs_s		regs;
	};
```

### A19 — kframe真实typedef (L10-10)

[VERIFIED mykernel/arch/x86_64/sched/sched_type_declaration_arch.h::task_kframe_s]
```c
	typedef struct inactive_task_frame task_kframe_s;
```

### A20 — fork_frame真实typedef (L12-12)

[VERIFIED mykernel/arch/x86_64/sched/sched_type_declaration_arch.h::fork_frame_s]
```c
	typedef struct fork_frame fork_frame_s;
```

### A21 — childregs位置计算的真实宏 (L17-21)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::task_pt_regs]
```c
	#define task_pt_regs(task)	({									\
				loff_t __ptr = (loff_t)task_stack_page(task);		\
				__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;	\
				((pt_regs_s *)__ptr) - 1;							\
			})
```

### A22 — current_pt_regs取当前task_pt_regs (L385-385)

[VERIFIED mykernel/include/linux/kernel/ptrace.h::current_pt_regs]
```c
	#	define current_pt_regs()	task_pt_regs(current)
```

### A23 — container_of返回frame基址的数学表达 (L9-13)

[VERIFIED mykernel/include/uapi/linux/stddef.h::container_of]
```c
	#	define container_of(member_ptr, container_type, member_name) ({				\
					typeof(((container_type *)0)->member_name) * p = (member_ptr);	\
					(container_type *)((unsigned long)p -							\
						(unsigned long)&(((container_type *)0)->member_name));		\
				})
```

### A24 — 任务描述符字段复制 (L288-289)

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```

### A25 — 在父task复制后申请新栈 (L294-296)

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	err = alloc_thread_stack_node(tsk, node);
	if (err)
		goto free_tsk;
```

### A26 — copy_process取得新task并检测失败 (L959-961)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

### A27 — copy_process先清后按args.kthread设PF_KTHREAD (L962-964)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```

### A28 — copy_thread调用及错误路径 (L1143-1145)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_thread(p, args);
	if (retval)
		goto bad_fork_cleanup_io;
```

### A29 — 用户参数的kthread位 (L15-15)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		u32				kthread			: 1;
```

### A30 — clone stack及tls字段 (L19-21)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		ulong			stack;
		ulong			stack_size;
		ulong			tls;
```

### A31 — fn与fn_arg参数来源 (L27-28)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		int				(*fn)(void *);
		void			*fn_arg;
```

### A32 — 已存在生成头记录176 (L56-56)

[VERIFIED mykernel/include/generated/asm-offsets.h::PTREGS_SIZE]
```c
	#define PTREGS_SIZE 176
```

### A33 — reg_t是64位无符号整数类型 (L16-16)

[VERIFIED mykernel/include/linux/kernel/types.h::reg_t]
```c
		typedef __UINT64_TYPE__ reg_t;
```

## B 与 C 源证据

### B01 — switch_to宏中实际调用asm切换入口 (L8-11)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_macro_arch.h::switch_to]
```c
	#define switch_to(prev, next, last)						\
			do {											\
				((last) = __switch_to_asm((prev), (next)));	\
			} while (0)
```

### B02 — 切换汇编真实标签入口 (L210-210)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
SYM_FUNC_START(__switch_to_asm)
```

### B03 — 六个callee保存中的前五条push (L215-219)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	pushq		%rbp
	pushq		%rbx
	pushq		%r12
	pushq		%r13
	pushq		%r14
```

### B04 — 第六条push r15 (L220-220)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	pushq		%r15
```

### B05 — 保存prev.thread.sp并装入next.thread.sp (L223-224)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	movq		%rsp,		TASK_threadsp(%rdi)
	movq		TASK_threadsp(%rsi),	%rsp
```

### B06 — 新栈依次恢复前五个callee寄存器 (L236-240)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	popq		%r15
	popq		%r14
	popq		%r13
	popq		%r12
	popq		%rbx
```

### B07 — 第六个pop与尾跳至__switch_to (L240-244)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::__switch_to_asm]
```asm
	popq		%rbx
	popq		%rbp

	jmp			__switch_to
SYM_FUNC_END(__switch_to_asm)
```

### B08 — __switch_to的prev和next形参及CPU上下文 (L319-323)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
*__switch_to(task_s *prev_p, task_s *next_p)
{
	thread_s *prev = &prev_p->thread;
	thread_s *next = &next_p->thread;
	int cpu = smp_processor_id();
```

### B09 — 切换前置save_fsgs调用 (L336-336)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	save_fsgs(prev_p);
```

### B10 — 切换前置x86_fsgsbase_load调用 (L373-373)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	x86_fsgsbase_load(prev, next);
```

### B11 — 后续发布每CPUcurrent与栈顶 (L380-382)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	*this_cpu_ptr(&pcpu_hot.current_task) = next_p;
	*this_cpu_ptr(&pcpu_hot.top_of_stack) = task_top_of_stack(next_p);

```

### B12 — 后续调用update_task_stack (L385-386)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	/* Reload sp0. */
	update_task_stack(next_p);
```

### B13 — __switch_to在正常路径返回prev_p (L421-422)

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	return prev_p;
}
```

### B14 — 新任务初入场的真实汇编目标 (L255-255)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
SYM_CODE_START(ret_from_fork_asm)
```

### B15 — prev、regs、fn、fn_arg四个实参寄存器的生成 (L268-272)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	movq		%rax,		%rdi		/* prev */
	movq		%rsp,		%rsi		/* regs */
	movq		%rbx,		%rdx		/* fn */
	movq		%r12,		%rcx		/* fn_arg */
	call		ret_from_fork			/* rdi: 'prev' task parameter */
```

### B16 — C函数返回后测试rbx决定分流 (L274-275)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	testq		%rbx,		%rbx		/* from kernel_thread? */
	jnz			1f						/* kernel threads are uncommon */
```

### B17 — 用户向分支标签 (L277-278)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
2:
swapgs_restore_regs_and_return_to_usermode:
```

### B18 — 跳到限定的syscall_return_via_sysret (L282-282)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	jmp			syscall_return_via_sysret
```

### B19 — 非空fn的汇编分支标签与rdi参数 (L284-287)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
1:
	/* kernel thread */
	// UNWIND_HINT_EMPTY
	movq		%r12,		%rdi
```

### B20 — 非空rbx二次间接调用点 (L289-289)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	callq		*%rbx
```

### B21 — 返回后原地jmp点与标签尾 (L295-298)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	// movq	$0, RAX(%rsp)
	// jmp	2b
	jmp		.
SYM_CODE_END(ret_from_fork_asm)
```

### B22 — 首帧结构frame在regs之前 (L26-29)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::fork_frame]
```c
	struct fork_frame {
		task_kframe_s	frame;
		pt_regs_s		regs;
	};
```

### B23 — six pop后的ret_addr在bp之后 (L22-24)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_types_arch.h::inactive_task_frame]
```c
		reg_t	bp;
		reg_t	ret_addr;
	};
```

### B24 — 静态已生成thread.sp偏移 (L53-53)

[VERIFIED mykernel/include/generated/asm-offsets.h::TASK_threadsp]
```c
	#define TASK_threadsp 552
```

### B25 — 静态已生成pt_regs尺寸 (L56-56)

[VERIFIED mykernel/include/generated/asm-offsets.h::PTREGS_SIZE]
```c
	#define PTREGS_SIZE 176
```

### C01 — ret_from_fork函数入口并先调用schedule_tail (L62-66)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
__visible void
ret_from_fork(task_s *prev, pt_regs_s *regs,
		int (*fn)(void *), void *fn_arg)
{
	schedule_tail(prev);
```

### C02 — 可选fn第一次C层调用点 (L68-70)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
	/* Is this a kernel thread? */
	if (unlikely(fn)) {
		fn(fn_arg);
```

### C03 — fn正常返回后设置ax且无条件做用户返回工作 (L76-79)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
		regs->ax = 0;
	}

	syscall_exit_to_user_mode(regs);
```

### C04 — C函数尾与用户返回工作出口 (L79-80)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
	syscall_exit_to_user_mode(regs);
}
```

### C05 — 首切入schedule_tail函数入口 (L873-875)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule_tail]
```c
asmlinkage __visible void
schedule_tail(task_s *prev) __releases(rq->lock)
{
```

### C06 — finish_task_switch注释而preempt_enable活动 (L885-886)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule_tail]
```c
	// finish_task_switch(prev);
	preempt_enable();
```

### C07 — 用户返回工作函数内直接调用 (L69-72)

[VERIFIED mykernel/entry/common.c::syscall_exit_to_user_mode]
```c
__visible noinstr void
syscall_exit_to_user_mode(pt_regs_s *regs) {
	// instrumentation_begin();
	__syscall_exit_to_user_mode_work(regs);
```

### C08 — 返回工作结束及注释exit_to_user_mode (L72-75)

[VERIFIED mykernel/entry/common.c::syscall_exit_to_user_mode]
```c
	__syscall_exit_to_user_mode_work(regs);
	// instrumentation_end();
	// exit_to_user_mode();
}
```

### C09 — 中间函数调用exit_to_user_mode_loop (L62-66)

[VERIFIED mykernel/entry/common.c::__syscall_exit_to_user_mode_work]
```c
__syscall_exit_to_user_mode_work(pt_regs_s *regs) {
	// syscall_exit_to_user_mode_prepare(regs);
	// local_irq_disable_exit_to_user();
	// exit_to_user_mode_prepare(regs);
	exit_to_user_mode_loop(regs, 0);
```

### C10 — 该源体内arch_do_signal_or_restart调用活动 (L33-34)

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]
```c
	// 	if (ti_work & (_TIF_SIGPENDING | _TIF_NOTIFY_SIGNAL))
			arch_do_signal_or_restart(regs);
```

### C11 — 用户退出工作返回路径 (L55-57)

[VERIFIED mykernel/entry/common.c::exit_to_user_mode_loop]
```c
	/* Return the latest work state for arch_exit_to_user_mode() */
	return ti_work;
}
```

### C12 — 用户与内核分流条件 (L274-275)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	testq		%rbx,		%rbx		/* from kernel_thread? */
	jnz			1f						/* kernel threads are uncommon */
```

### C13 — 用户分流入口 (L277-278)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
2:
swapgs_restore_regs_and_return_to_usermode:
```

### C14 — 用户分流跳向sysret标签 (L282-282)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	jmp			syscall_return_via_sysret
```

### C15 — 六行需缩至285至289 (L284-288)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
1:
	/* kernel thread */
	// UNWIND_HINT_EMPTY
	movq		%r12,		%rdi
	// CALL_NOSPEC rbx
```

### C16 — 非空分流的后一次call (L289-289)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	callq		*%rbx
```

### C17 — 后一次call正常返回后的原地跳转 (L295-298)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	// movq	$0, RAX(%rsp)
	// jmp	2b
	jmp		.
SYM_CODE_END(ret_from_fork_asm)
```

### C18 — SYSCALL汇编返用户的后继标签 (L184-185)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
SYM_CODE_START(syscall_return_via_sysret)
	cli
```

### C19 — 限定用户返回的末尾指令 (L195-199)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	popq		%rsp
	swapgs

	sti
	sysretq
```

### C20 — PF_KTHREAD的清零与bx/r12生产者 (L120-123)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
```

### C21 — 普通用户fn为空的bx零和ax/sp处理 (L133-137)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;
```

### C22 — 普通用户fn非空覆盖ip/sp及bx/r12 (L150-153)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
		childregs->sp = 0;
		childregs->ip = 0;
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
```

### C23 — args.kthread对PF_KTHREAD的实际设置 (L962-964)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```

