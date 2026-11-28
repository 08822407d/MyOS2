# CORE-USER-KTHREAD-ENTRY-68：固定time源码短锚

全部片段来自GitHub直接读取的`time@a039d9803ade`固定源码对象；每段是所属单一函数、结构体、typedef或宏的非空连续1–5物理行。保留原始制表符、注释和空白。源码级条件成立不代表任何编译、调度、CPU、实体任务或设备已实际运行。

## A 组证据

### A01 — kernel_thread函数及本地clone参数构造 (L1535–L1538)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
pid_t kernel_thread(int (*fn)(void *), void *arg,
		const char *name, unsigned long flags)
{
	kclone_args_s args = {
```

### A02 — flags及exit_signal实际取值表达式 (L1539–L1541)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.flags			= ((lower_32_bits(flags) | CLONE_VM |
							CLONE_UNTRACED) & ~CSIGNAL),
		.exit_signal	= (lower_32_bits(flags) & CSIGNAL),
```

### A03 — fn、fn_arg、name、kthread输入赋值 (L1542–L1545)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.fn				= fn,
		.fn_arg			= arg,
		.name			= name,
		.kthread		= 1,
```

### A04 — stack/stack_size存放fn/arg整数表示 (L1546–L1548)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.stack			= (unsigned long)fn,
		.stack_size		= (unsigned long)arg,
	};
```

### A05 — kernel_thread调用kernel_clone并返回 (L1550–L1551)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
	return kernel_clone(&args);
}
```

### A06 — clone参数中name、exit_signal和kthread成员 (L11–L15)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		int __user		*child_tid;
		int __user		*parent_tid;
		const char		*name;
		int				exit_signal;
		u32				kthread			: 1;
```

### A07 — kthread位与stack成员定义 (L15–L19)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		u32				kthread			: 1;
		u32				io_thread		: 1;
		u32				user_worker		: 1;
		u32				no_files		: 1;
		ulong			stack;
```

### A08 — stack、stack_size、tls定义 (L19–L21)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		ulong			stack;
		ulong			stack_size;
		ulong			tls;
```

### A09 — fn与fn_arg成员定义 (L27–L28)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		int				(*fn)(void *);
		void			*fn_arg;
```

### A10 — kernel_clone入口和局部变量定义 (L1444–L1448)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
pid_t kernel_clone(kclone_args_s *args)
{
	u64 clone_flags = args->flags;
	completion_s vfork;
	pid_s *pid_struct;
```

### A11 — PIDFD组合的有限参数拒绝门 (L1462–L1465)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if ((args->flags & CLONE_PIDFD) &&
		(args->flags & CLONE_PARENT_SETTID) &&
		(args->pidfd == args->parent_tid))
		return -EINVAL;
```

### A12 — copy_process创建及IS_ERR返回 (L1485–L1489)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
	// add_latent_entropy();

	if (IS_ERR(p))
		return PTR_ERR(p);
```

### A13 — PID与nr取值 (L1497–L1498)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

### A14 — CLONE_VFORK条件完成对象 (L1503–L1506)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (clone_flags & CLONE_VFORK) {
		p->vfork_done = &vfork;
		init_completion(&vfork);
		// get_task_struct(p);
```

### A15 — wake_up_new_task调用 (L1516–L1516)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

### A16 — 归还PID对象并返回nr (L1527–L1530)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
}
```

### A17 — 从当前任务复制出新task的成功门 (L958–L961)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = -ENOMEM;
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```

### A18 — 清再条件设置PF_KTHREAD (L962–L964)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```

### A19 — PF_KTHREAD构造kthread_struct失败处理 (L1060–L1063)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (p->flags & PF_KTHREAD) {
		if (!set_kthread_struct(p))
			goto bad_fork_cleanup_delayacct;
	}
```

### A20 — copy_thread调用并检查返回错误 (L1143–L1145)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_thread(p, args);
	if (retval)
		goto bad_fork_cleanup_io;
```

### A21 — 非空name更新任务comm (L976–L977)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (args->name)
		strscpy_pad(p->comm, args->name, sizeof(p->comm));
```

### A22 — copy_thread读取flags、stack、tls (L82–L86)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
int copy_thread(task_s *p, const kclone_args_s *args)
{
	ulong clone_flags = args->flags;
	ulong sp = args->stack;
	ulong tls = args->tls;
```

### A23 — childregs与fork_frame的定位 (L92–L94)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, fork_frame_s, regs);
	frame = &fork_frame->frame;
```

### A24 — bp/ret_addr/thread.sp在首帧上的写入 (L97–L99)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bp = (reg_t)childregs + 1;
	frame->ret_addr = (reg_t)ret_from_fork_asm;
	p->thread.sp = (reg_t)fork_frame;
```

### A25 — PF_KTHREAD零化pt_regs再初始化bx/r12 (L120–L124)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
```

### A26 — kthread首帧中bx/r12分别存fn/arg (L38–L41)

[VERIFIED mykernel/arch/x86_64/sched/context/switch_to_arch.h::kthread_frame_init]
```c
		kthread_frame_init(task_kframe_s *frame, int (*fun)(void *), void *arg) {
			frame->bx = (reg_t)fun;
			frame->r12 = (reg_t)arg;
		}
```

### A27 — PF_KTHREAD早退之后的普通用户分支才按sp覆盖childregs (L133–L137)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;
```

### A28 — PF_KTHREAD宏定义 (L112–L112)

[VERIFIED mykernel/sched/task/task_const.h::PF_KTHREAD]
```c
	#define PF_KTHREAD				0x00200000	/* I am a kernel thread */
```

### A29 — kclone_args_s结构类型别名 (L25–L25)

[VERIFIED mykernel/sched/sched_type_declaration.h::kclone_args_s]
```c
	typedef struct kernel_clone_args kclone_args_s;
```

## B 组证据

### B01 — 请求包含full_name、threadfn与data (L20–L24)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_info]
```c
typedef struct kthread_create_info {
	/* Information passed to kthread() from kthreadd. */
	char			*full_name;
	int				(*threadfn)(void *data);
	void			*data;
```

### B02 — 请求含result与done指针及队列节点 (L27–L31)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_info]
```c
	/* Result passed back to kthread_create() from kthreadd. */
	task_s			*result;
	completion_s	*done;

	List_s			list;
```

### B03 — kthread包装体的flags、cpu、result、业务函数 (L34–L38)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_s]
```c
typedef struct kthread {
	unsigned long	flags;
	unsigned int	cpu;
	int				result;
	int				(*threadfn)(void *);
```

### B04 — 包装体业务threadfn与data (L38–L39)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_s]
```c
	int				(*threadfn)(void *);
	void			*data;
```

### B05 — 包装体结束通知和full_name (L41–L45)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_s]
```c
	// completion_s	parked;
	completion_s	exited;
	/* To store the full name if task comm is truncated. */
	char			*full_name;
} kthread_s;
```

### B06 — KTHREAD_SHOULD_STOP为本地标志位 (L47–L51)

[VERIFIED mykernel/sched/forkexec/kthread.c::KTHREAD_BITS]
```c
enum KTHREAD_BITS {
	KTHREAD_IS_PER_CPU = 0,
	KTHREAD_SHOULD_STOP,
	KTHREAD_SHOULD_PARK,
};
```

### B07 — 从task的worker_private读取包装体 (L53–L56)

[VERIFIED mykernel/sched/forkexec/kthread.c::to_kthread]
```c
static inline kthread_s *to_kthread(task_s *k) {
	WARN_ON(!(k->flags & PF_KTHREAD));
	return k->worker_private;
}
```

### B08 — 可选读取包装体的任务标志门 (L69–L73)

[VERIFIED mykernel/sched/forkexec/kthread.c::__to_kthread]
```c
static inline kthread_s *__to_kthread(task_s *p) {
	void *kthread = p->worker_private;
	if (kthread && !(p->flags & PF_KTHREAD))
		kthread = NULL;
	return kthread;
```

### B09 — set_kthread_struct检查现有包装体 (L88–L92)

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
bool set_kthread_struct(task_s *p)
{
	kthread_s *kthread;

	if (WARN_ON_ONCE(to_kthread(p)))
```

### B10 — 分配kthread结构并初始化exited (L95–L99)

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
	kthread = kzalloc(sizeof(*kthread), GFP_KERNEL);
	if (!kthread)
		return false;

	init_completion(&kthread->exited);
```

### B11 — 绑定vfork_done及worker_private (L100–L104)

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
	// init_completion(&kthread->parked);
	p->vfork_done = &kthread->exited;

	p->worker_private = kthread;
	return true;
```

### B12 — 终止包装函数写result然后调用do_exit (L135–L139)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_exit]
```c
void __noreturn kthread_exit(long result)
{
	kthread_s *kthread = to_kthread(current);
	kthread->result = result;
	do_exit(0);
```

### B13 — 可选完成通知后退出 (L156–L160)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_complete_and_exit]
```c
{
	if (comp)
		complete(comp);

	kthread_exit(code);
```

### B14 — 包装入口kthread与create参数 (L164–L168)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
int kthread(void *_create)
{
	// static const struct sched_param param = { .sched_priority = 0 };
	/* Copy data: it's on kthread's stack */
	kthd_create_info_s *create = _create;
```

### B15 — 从请求读取业务threadfn和data (L169–L170)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	int (*threadfn)(void *data) = create->threadfn;
	void *data = create->data;
```

### B16 — 读取self并以xchg取得done (L175–L179)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	self = to_kthread(current);

	/* If user was SIGKILLed, I release the structure. */
	done = xchg(&create->done, NULL);
	if (!done) {
```

### B17 — done为空释放请求并直接kthread_exit (L180–L183)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
		kfree(create->full_name);
		kfree(create);
		kthread_exit(-EINTR);
	}
```

### B18 — 绑定包装体的full_name、threadfn、data (L185–L187)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	self->full_name = create->full_name;
	self->threadfn = threadfn;
	self->data = data;
```

### B19 — 设TASK_UNINTERRUPTIBLE并公布task结果 (L196–L199)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	/* OK, tell user we're spawned, wait for stop or wakeup */
	__set_current_state(TASK_UNINTERRUPTIBLE);
	create->result = current;
	/*
```

### B20 — 完成创建通知、等待调度、恢复抢占 (L203–L206)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
	preempt_enable();
```

### B21 — SHOULD_STOP门与业务回调调用 (L208–L212)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	ret = -EINTR;
	if (!test_bit(KTHREAD_SHOULD_STOP, &self->flags)) {
		// cgroup_kthread_ready();
		// __kthread_parkme(self);
		ret = threadfn(data);
```

### B22 — 业务返回值进入kthread_exit (L212–L215)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
		ret = threadfn(data);
	}
	kthread_exit(ret);
}
```

### B23 — create_kthread把包装器kthread作为入口 (L219–L223)

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	int pid;

	/* We want our own signal handler (we take no signals by default). */
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
```

### B24 — kernel_thread创建错误门取done (L224–L228)

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	if (pid < 0) {
		/* If user was SIGKILLed, I release the structure. */
		completion_s *done = xchg(&create->done, NULL);

		kfree(create->full_name);
```

### B25 — 错误结果回传及完成通知 (L231–L235)

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
			return;
		}
		create->result = ERR_PTR(pid);
		complete(done);
	}
```

### B26 — 通用创建入口及栈上completion (L239–L243)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
task_s *__kthread_create_on_node(int (*threadfn)(void *data),
		void *data, int node, const char namefmt[], va_list args)
{
	DECLARE_COMPLETION_ONSTACK(done);
	task_s *task;
```

### B27 — kmalloc请求及ENOMEM返回门 (L244–L248)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	kthd_create_info_s *create =
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);

	if (!create)
		return ERR_PTR(-ENOMEM);
```

### B28 — 请求绑定threadfn、data、done及名称 (L249–L253)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	create->threadfn	= threadfn;
	create->data		= data;
	create->node		= node;
	create->done		= &done;
	create->full_name	= kvasprintf(GFP_KERNEL, namefmt, args);
```

### B29 — 名称分配失败清理路径与队列节点 (L254–L258)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	if (!create->full_name) {
		task = ERR_PTR(-ENOMEM);
		goto free_create;
	}
	INIT_LIST_S(&create->list);
```

### B30 — 队列发布及唤醒kthreadd (L260–L264)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	spin_lock(&kthread_create_lock);
	list_header_add_to_tail(&kthread_create_list, &create->list);
	spin_unlock(&kthread_create_lock);

	wake_up_process(kthreadd_task);
```

### B31 — wait_for_completion与读取result (L280–L284)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
		 * shortly.
		 */
		wait_for_completion(&done);
	// }
	task = create->result;
```

### B32 — 释放创建请求并返回result (L285–L287)

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
free_create:
	kfree(create);
	return task;
```

### B33 — 公开包装API真实形参入口 (L313–L315)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]
```c
task_s *kthread_create_on_node(int (*threadfn)(void *data),
		void *data, int node, const char namefmt[], ...)
{
```

### B34 — 变参转到内部创建API (L319–L323)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]
```c
	va_start(args, namefmt);
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
	va_end(args);

	return task;
```

### B35 — kthreadd循环按队列空与否调度 (L341–L345)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (list_header_is_empty(&kthread_create_list))
			schedule();
		__set_current_state(TASK_RUNNING);
```

### B36 — 队列非空时锁内逐个取请求 (L347–L351)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
		spin_lock(&kthread_create_lock);
		while (!list_header_is_empty(&kthread_create_list)) {
			kthd_create_info_s *create;

			List_s *lp = list_header_remove_tail(&kthread_create_list);
```

### B37 — 取出请求后交create_kthread (L352–L356)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			create = container_of(lp, kthd_create_info_s, list);
			spin_unlock(&kthread_create_lock);

			create_kthread(create);

```

### B38 — 再上锁直到循环完成 (L357–L360)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			spin_lock(&kthread_create_lock);
		}
		spin_unlock(&kthread_create_lock);
	}
```

## C 组证据

### C01 — rest_init入口及任务与pid局部 (L82–L85)

[VERIFIED mykernel/init/main.c::rest_init]
```c
noinline void __ref rest_init(void)
{
	task_s *tsk;
	int pid;
```

### C02 — rest_init启动kernel_init真实函数目标 (L93–L93)

[VERIFIED mykernel/init/main.c::rest_init]
```c
	pid = kernel_thread(kernel_init, NULL, "kthd: kernel_init", CLONE_FS);
```

### C03 — pid查task并使用tsk指针 (L100–L102)

[VERIFIED mykernel/init/main.c::rest_init]
```c
	tsk = find_task_by_pid_ns(pid, &init_pid_ns);
	tsk->flags |= PF_NO_SETAFFINITY;
	// set_cpus_allowed_ptr(tsk, cpumask_of(smp_processor_id()));
```

### C04 — rest_init启动kthreadd并记录kthreadd_task (L106–L109)

[VERIFIED mykernel/init/main.c::rest_init]
```c
	pid = kernel_thread(kthreadd, NULL, "kthreadd", CLONE_FS | CLONE_FILES);
	// rcu_read_lock();
	kthreadd_task = find_task_by_pid_ns(pid, &init_pid_ns);
	// rcu_read_unlock();
```

### C05 — 完成kthreadd_done通知 (L120–L120)

[VERIFIED mykernel/init/main.c::rest_init]
```c
	complete(&kthreadd_done);
```

### C06 — 首次主动调度与进入idle接口 (L126–L129)

[VERIFIED mykernel/init/main.c::rest_init]
```c
	schedule_preempt_disabled();
	/* Call into cpu_idle with preempt disabled */
	cpu_startup_entry();
}
```

### C07 — kernel_init源码定义与ret变量 (L270–L273)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
int kernel_init(void *unused)
{
	int ret;

```

### C08 — kernel_init等待kthreadd_done (L274–L277)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	/*
	 * Wait until kthreadd is all set-up.
	 */
	wait_for_completion(&kthreadd_done);
```

### C09 — do_name与kernel_init_freeable调用 (L279–L281)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
extern void do_name(void);
	do_name();
	kernel_init_freeable();
```

### C10 — 设备请求队列初始化三函数 (L284–L286)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	init_NVMErqd();
	init_ATArqd();
	init_XHCIrqd();
```

### C11 — 设备探测与队列初始化 (L293–L295)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	myos_ata_probe();
	NVMe_IOqueue_init();
	// USB_Keyborad_init();
```

### C12 — root disk切换调用位于跳转前 (L297–L299)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	// color_printk(GREEN, BLACK, "Enter task init.\n");
	myos_switch_to_root_disk();
	// color_printk(GREEN, BLACK, "VFS initiated.\n");
```

### C13 — kjmp_to_doexecve之后为while true与ret语句 (L304–L308)

[VERIFIED mykernel/init/main.c::kernel_init]
```c
	kjmp_to_doexecve();

	while (true);
	
	return 1;
```

### C14 — kjmp_to_doexecve真实入口 (L9–L12)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
void kjmp_to_doexecve()
{
	// while (1);
	pr_info("Now jump to initd\n");
```

### C15 — 设置当前任务并清PF_KTHREAD及init路径常量 (L15–L19)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	task_s * curr = current;
	pt_regs_s *curr_ptregs = task_pt_regs(curr);
	curr->flags &= ~PF_KTHREAD;

	const char *initd_name = "/boot/init";
```

### C16 — 设置mm并调用kernel_execve (L30–L33)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	// 因为这里是内核线程，要通过execve进用户态
	// 需要通过current->mm和active_mm的检查，所以设置下mm
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

### C17 — 源码汇编请求改rsp并跳sysret标签 (L35–L39)

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	hwtss_cpu0 = this_cpu_ptr(&(cpu_tss_rw.x86_tss));

	asm volatile(	"movq	%0,		%%rsp		\n\t"
					// "jmp	sysexit_entp		\n\t"
					"jmp	syscall_return_via_sysret	\n\t"
```

### C18 — kthreadd真实入口 (L328–L330)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
int kthreadd(void *unused)
{
	task_s *tsk = current;
```

### C19 — kthreadd进入for无限循环 (L338–L342)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
	current->flags |= PF_NOFREEZE;
	// cgroup_init_kthreadd();

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
```

### C20 — 创建队列空则schedule (L343–L345)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
		if (list_header_is_empty(&kthread_create_list))
			schedule();
		__set_current_state(TASK_RUNNING);
```

### C21 — 队列非空时锁内取队尾请求 (L347–L351)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
		spin_lock(&kthread_create_lock);
		while (!list_header_is_empty(&kthread_create_list)) {
			kthd_create_info_s *create;

			List_s *lp = list_header_remove_tail(&kthread_create_list);
```

### C22 — 释放锁、调用create_kthread然后重加锁 (L352–L356)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			create = container_of(lp, kthd_create_info_s, list);
			spin_unlock(&kthread_create_lock);

			create_kthread(create);

```

### C23 — 循环尾锁释放与for闭合 (L357–L360)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
			spin_lock(&kthread_create_lock);
		}
		spin_unlock(&kthread_create_lock);
	}
```

### C24 — 源码有循环之外return0但正常控制不可达 (L362–L363)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthreadd]
```c
	return 0;
}
```

### C25 — 通用包装入口为kthread而非threadfn (L222–L223)

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
```

### C26 — 业务回调由SHOULD_STOP决定 (L208–L212)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	ret = -EINTR;
	if (!test_bit(KTHREAD_SHOULD_STOP, &self->flags)) {
		// cgroup_kthread_ready();
		// __kthread_parkme(self);
		ret = threadfn(data);
```

### C27 — 业务返回后调用kthread_exit (L212–L214)

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
		ret = threadfn(data);
	}
	kthread_exit(ret);
```

### C28 — do_exit真实终止函数入口 (L68–L71)

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
void __noreturn do_exit(long code)
{
	task_s *tsk = current;
	int group_dead;
```

### C29 — do_exit禁抢占先于do_task_dead (L177–L180)

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// check_stack_usage();
	preempt_disable();
	// if (tsk->nr_dirtied)
	// 	__this_cpu_add(dirty_throttle_leaks, tsk->nr_dirtied);
```

### C30 — do_exit最后调用do_task_dead (L184–L186)

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// lockdep_free_task(tsk);
	do_task_dead();
}
```

### C31 — do_task_dead函数入口和置DEAD (L1172–L1175)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
void __noreturn do_task_dead(void)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	set_special_state(TASK_DEAD);
```

### C32 — __schedule后进入BUG的停止层 (L1178–L1181)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();
```

### C33 — BUG若返回时仍有本地死循环 (L1183–L1186)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	for (;;)
		cpu_relax();
}
```

### C34 — 调度禁抢占接口直接动作 (L1247–L1251)

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule_preempt_disabled]
```c
void schedule_preempt_disabled(void)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
```

### C35 — 首次fn在C层条件调用 (L66–L70)

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::ret_from_fork]
```c
	schedule_tail(prev);

	/* Is this a kernel thread? */
	if (unlikely(fn)) {
		fn(fn_arg);
```

### C36 — 首次汇编参数交接与ret_from_fork调用 (L268–L272)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	movq		%rax,		%rdi		/* prev */
	movq		%rsp,		%rsi		/* regs */
	movq		%rbx,		%rdx		/* fn */
	movq		%r12,		%rcx		/* fn_arg */
	call		ret_from_fork			/* rdi: 'prev' task parameter */
```

### C37 — 非空RBX路径到二次call (L285–L289)

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
	/* kernel thread */
	// UNWIND_HINT_EMPTY
	movq		%r12,		%rdi
	// CALL_NOSPEC rbx
	callq		*%rbx
```

