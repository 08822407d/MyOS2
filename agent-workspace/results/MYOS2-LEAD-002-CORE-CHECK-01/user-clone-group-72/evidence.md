# CORE-USER-CLONE-GROUP-72 — 固定 time 源码短引文

全部片段由GitHub连接器直接读取固定 `time@a039d9803ade`，每段非空、连续1–5物理行且属于单一真实定义。源码中出现调用或分支不等于实际线程被创建、计数合法、组成员完整或CPU完成调度。

## A — 直接引文

### A01 — 真实fork用户入口及前置barrier (L51–L53)

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)]
```c
MYOS_SYSCALL_DEFINE0(fork)
{
	barrier();
```

### A02 — fork显式只写exit_signal并调用kernel_clone (L60–L64)

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)]
```c
	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

### A03 — clone参数struct定义的flags及PID指针成员 (L8–L12)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
	struct kernel_clone_args {
		u64				flags;
		int __user		*pidfd;
		int __user		*child_tid;
		int __user		*parent_tid;
```

### A04 — clone参数name exit_signal kthread与其它位 (L13–L17)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		const char		*name;
		int				exit_signal;
		u32				kthread			: 1;
		u32				io_thread		: 1;
		u32				user_worker		: 1;
```

### A05 — clone参数stack stack_size tls成员 (L19–L21)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		ulong			stack;
		ulong			stack_size;
		ulong			tls;
```

### A06 — clone参数fn和fn_arg成员 (L27–L28)

[VERIFIED mykernel/sched/misc/sched_misc_types.h::kernel_clone_args]
```c
		int				(*fn)(void *);
		void			*fn_arg;
```

### A07 — kclone_args_s是kernel_clone_args类型别名 (L25–L25)

[VERIFIED mykernel/sched/sched_type_declaration.h::kclone_args_s]
```c
	typedef struct kernel_clone_args kclone_args_s;
```

### A08 — kernel_thread独立函数参数与args初始化 (L1535–L1538)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
pid_t kernel_thread(int (*fn)(void *), void *arg,
		const char *name, unsigned long flags)
{
	kclone_args_s args = {
```

### A09 — kernel_thread掩码加入CLONE_VM UNTRACED并分exit_signal (L1539–L1541)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.flags			= ((lower_32_bits(flags) | CLONE_VM |
							CLONE_UNTRACED) & ~CSIGNAL),
		.exit_signal	= (lower_32_bits(flags) & CSIGNAL),
```

### A10 — kernel_thread设置fn fn_arg name和kthread位 (L1542–L1545)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.fn				= fn,
		.fn_arg			= arg,
		.name			= name,
		.kthread		= 1,
```

### A11 — kernel_thread额外整数化stack和stack_size (L1546–L1548)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.stack			= (unsigned long)fn,
		.stack_size		= (unsigned long)arg,
	};
```

### A12 — kernel_thread也调用kernel_clone (L1550–L1551)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
	return kernel_clone(&args);
}
```

### A13 — kernel_clone入口及clone_flags来源 (L1444–L1448)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
pid_t kernel_clone(kclone_args_s *args)
{
	u64 clone_flags = args->flags;
	completion_s vfork;
	pid_s *pid_struct;
```

### A14 — PIDFD与PARENT_SETTID同地址冲突拒绝 (L1462–L1465)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if ((args->flags & CLONE_PIDFD) &&
		(args->flags & CLONE_PARENT_SETTID) &&
		(args->pidfd == args->parent_tid))
		return -EINVAL;
```

### A15 — kernel_clone经copy_process并消费ERR_PTR (L1485–L1489)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
	// add_latent_entropy();

	if (IS_ERR(p))
		return PTR_ERR(p);
```

### A16 — CLONE_VFORK条件设置完成对象 (L1503–L1506)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (clone_flags & CLONE_VFORK) {
		p->vfork_done = &vfork;
		init_completion(&vfork);
		// get_task_struct(p);
```

### A17 — 正常路径发出wake_up_new_task调用请求 (L1516–L1516)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

### A18 — CLONE_VFORK等待代码是注释 (L1522–L1525)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	// if (clone_flags & CLONE_VFORK) {
	// 	if (!wait_for_vfork_done(p, &vfork))
	// 		ptrace_event_pid(PTRACE_EVENT_VFORK_DONE, pid_struct);
	// }
```

### A19 — pid对象put与数值返回 (L1527–L1529)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
```

### A20 — copy_process入口与task局部 (L868–L872)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
*copy_process(pid_s *pid_struct, int trace, int node, kclone_args_s *args)
{
	int pidfd = -1, retval;
	task_s *p, *curr = current;
	file_s *pidfile = NULL;
```

### A21 — clone_flags来自args并取当前nsproxy (L872–L874)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	file_s *pidfile = NULL;
	u64 clone_flags = args->flags;
	nsproxy_s *nsp = current->nsproxy;
```

### A22 — NEWNS与FS、NEWUSER与FS互斥门 (L880–L884)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if ((clone_flags & (CLONE_NEWNS|CLONE_FS)) == (CLONE_NEWNS|CLONE_FS))
		return ERR_PTR(-EINVAL);

	if ((clone_flags & (CLONE_NEWUSER|CLONE_FS)) == (CLONE_NEWUSER|CLONE_FS))
		return ERR_PTR(-EINVAL);
```

### A23 — THREAD缺SIGHAND直接EINVAL (L890–L891)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if ((clone_flags & CLONE_THREAD) && !(clone_flags & CLONE_SIGHAND))
		return ERR_PTR(-EINVAL);
```

### A24 — SIGHAND缺VM直接EINVAL (L898–L899)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if ((clone_flags & CLONE_SIGHAND) && !(clone_flags & CLONE_VM))
		return ERR_PTR(-EINVAL);
```

### A25 — CLONE_PARENT遇UN KILLABLE拒绝门 (L907–L909)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if ((clone_flags & CLONE_PARENT) &&
				(current->signal->flags & SIGNAL_UNKILLABLE))
		return ERR_PTR(-EINVAL);
```

### A26 — THREAD遇NEWUSER或NEWPID或命名空间不同拒绝 (L915–L918)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (clone_flags & CLONE_THREAD) {
		if ((clone_flags & (CLONE_NEWUSER | CLONE_NEWPID)) ||
			(task_active_pid_ns(current) != nsp->pid_ns_for_children))
			return ERR_PTR(-EINVAL);
```

### A27 — PIDFD条件与CLONE_THREAD/DETACHED组合限制说明 (L930–L934)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (clone_flags & CLONE_PIDFD) {
		/*
		 * - CLONE_DETACHED is blocked so that we can potentially
		 *   reuse it later for CLONE_PIDFD.
		 * - CLONE_THREAD is blocked until someone really needs it.
```

### A28 — PIDFD与THREAD或DETACHED拒绝 (L936–L937)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		if (clone_flags & (CLONE_DETACHED | CLONE_THREAD))
			return ERR_PTR(-EINVAL);
```

### A29 — dup_task_struct成功后按args.kthread实际清与设置PF_KTHREAD (L960–L964)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (!p)
		goto fork_out;
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```

### A30 — x64_sys_call内的switch从asm/syscalls_64.h展开分派 (L27–L31)

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

### A31 — __SYSCALL源宏将编号分派成对应处理函数调用 (L26–L26)

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

### A32 — sys_call_table通过引用syscalls_64头展开初始化项 (L20–L22)

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::sys_call_table]
```c
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
```

### A33 — CSIGNAL独立宏 (L10–L10)

[VERIFIED mykernel/include/uapi/linux/sched.h::CSIGNAL]
```c
	#define CSIGNAL					0x000000ff		/* signal mask to be sent at exit */
```

### A34 — CLONE_VM独立宏 (L11–L11)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_VM]
```c
	#define CLONE_VM				0x00000100		/* set if VM shared between processes */
```

### A35 — CLONE_SIGHAND独立宏 (L14–L14)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_SIGHAND]
```c
	#define CLONE_SIGHAND			0x00000800		/* set if signal handlers and blocked signals shared */
```

### A36 — CLONE_PIDFD独立宏 (L15–L15)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_PIDFD]
```c
	#define CLONE_PIDFD				0x00001000		/* set if a pidfd should be placed in parent */
```

### A37 — CLONE_VFORK独立宏 (L17–L17)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_VFORK]
```c
	#define CLONE_VFORK				0x00004000		/* set if the parent wants the child to wake it up on mm_release */
```

### A38 — CLONE_PARENT独立宏 (L18–L18)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_PARENT]
```c
	#define CLONE_PARENT			0x00008000		/* set if we want to have the same parent as the cloner */
```

### A39 — CLONE_THREAD独立宏 (L19–L19)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_THREAD]
```c
	#define CLONE_THREAD			0x00010000		/* Same thread group? */
```

### A40 — CLONE_NEWUSER独立宏 (L31–L31)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_NEWUSER]
```c
	#define CLONE_NEWUSER			0x10000000		/* New user namespace */
```

### A41 — CLONE_NEWPID独立宏 (L32–L32)

[VERIFIED mykernel/include/uapi/linux/sched.h::CLONE_NEWPID]
```c
	#define CLONE_NEWPID			0x20000000		/* New pid namespace */
```

## B — 直接引文

### B01 — dup_task_struct入口任务和node参数 (L273–L277)

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
static task_s *dup_task_struct(task_s *orig, int node)
{
	task_s *tsk;
	int err;

```

### B02 — 新task执行结构整体赋值继承指针 (L285–L289)

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	INIT_LIST_S(&tsk->tasks);
	// int __weak arch_dup_task_struct(task_s *dst, task_s *src)
	// {
		*tsk = *orig;
		err = 0;
```

### B03 — copy_files与copy_fs在信号复制之前 (L1122–L1126)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_files(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_semundo;
	retval = copy_fs(clone_flags, p);
	if (retval)
```

### B04 — copy_sighand失败跳到cleanup_fs (L1128–L1130)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_sighand(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_fs;
```

### B05 — copy_signal失败跳到cleanup_sighand (L1131–L1133)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_signal(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_sighand;
```

### B06 — CLONE_SIGHAND分支仅增加current.sighand.count (L752–L756)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	sighand_s *sig;

	if (clone_flags & CLONE_SIGHAND) {
		refcount_inc(&current->sighand->count);
		return 0;
```

### B07 — 非共享分配且tsk.sighand安装语句为注释 (L758–L761)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	sig = kmem_cache_alloc(sighand_cachep, GFP_KERNEL);
	// RCU_INIT_POINTER(tsk->sighand, sig);
	if (!sig)
		return -ENOMEM;
```

### B08 — 非共享分配失败门与新count初值1 (L759–L763)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	// RCU_INIT_POINTER(tsk->sighand, sig);
	if (!sig)
		return -ENOMEM;

	refcount_set(&sig->count, 1);
```

### B09 — 非共享sig.action在旧siglock下复制 (L763–L766)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	refcount_set(&sig->count, 1);
	spin_lock_irq(&current->sighand->siglock);
	memcpy(sig->action, current->sighand->action, sizeof(sig->action));
	spin_unlock_irq(&current->sighand->siglock);
```

### B10 — CLONE_CLEAR_SIGHAND刷新被注释及正常return (L768–L772)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	// /* Reset all signal handler not set to SIG_IGN to SIG_DFL. */
	// if (clone_flags & CLONE_CLEAR_SIGHAND)
	// 	flush_signal_handlers(tsk, 0);

	return 0;
```

### B11 — 归还时先做refcount_dec_and_test (L775–L778)

[VERIFIED mykernel/sched/forkexec/fork.c::__cleanup_sighand]
```c
void __cleanup_sighand(sighand_s *sighand)
{
	if (refcount_dec_and_test(&sighand->count)) {
		// signalfd_cleanup(sighand);
```

### B12 — 计数归零后请求释放sighand缓存对象 (L781–L784)

[VERIFIED mykernel/sched/forkexec/fork.c::__cleanup_sighand]
```c
		 * without an RCU grace period, see __lock_task_sighand().
		 */
		kmem_cache_free(sighand_cachep, sighand);
	}
```

### B13 — CLONE_THREAD直接return0不新分配signal (L789–L793)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
copy_signal(ulong clone_flags, task_s *tsk) {
	signal_s *sig;

	if (clone_flags & CLONE_THREAD)
		return 0;
```

### B14 — 非CLONE_THREAD先写tsk.signal然后判分配NULL (L795–L799)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	sig = kmem_cache_zalloc(signal_cachep, GFP_KERNEL);
	tsk->signal = sig;
	if (!sig)
		return -ENOMEM;

```

### B15 — 新signal.nr_threads=1且sigcnt=1 (L800–L803)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	sig->nr_threads = 1;
	// sig->quick_threads = 1;
	// atomic_set(&sig->live, 1);
	refcount_set(&sig->sigcnt, 1);
```

### B16 — signal.thread_head与tsk.thread_node初始化均注释 (L805–L807)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	// /* list_add(thread_node, thread_head) without INIT_LIST_HEAD() */
	// sig->thread_head = (struct list_head)LIST_HEAD_INIT(tsk->thread_node);
	// tsk->thread_node = (struct list_head)LIST_HEAD_INIT(sig->thread_head);
```

### B17 — curr_target为tsk并初始化shared_pending (L810–L812)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	sig->curr_target = tsk;
	init_sigpending(&sig->shared_pending);
	// INIT_HLIST_HEAD(&sig->multiprocess);
```

### B18 — 新signal.rlim复制自current.signal (L822–L824)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	// task_lock(current->group_leader);
	memcpy(sig->rlim, current->signal->rlim, sizeof sig->rlim);
	// task_unlock(current->group_leader);
```

### B19 — signal中两锁初始化注释且正常return (L834–L837)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_signal]
```c
	// mutex_init(&sig->cred_guard_mutex);
	// init_rwsem(&sig->exec_update_lock);

	return 0;
```

### B20 — signal_s有sigcnt及nr_threads且thread_head注释 (L80–L84)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```c
	struct signal_struct {
		refcount_t		sigcnt;
		// atomic_t		live;
		int				nr_threads;
		// struct list_head	thread_head;
```

### B21 — signal_s的curr_target和shared_pending不同成员 (L88–L92)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```c
		/* current thread group signal load-balancing target: */
		task_s			*curr_target;

		/* shared signal handling: */
		sigpending_s	shared_pending;
```

### B22 — signal_s另存组PID类别指针数组 (L149–L149)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```c
		pid_s			*pids[PIDTYPE_MAX];
```

### B23 — signal_s有rlim字段数组 (L199–L199)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::signal_struct]
```c
		rlimit_s		rlim[RLIM_NLIMITS];
```

### B24 — sighand有锁 count和action数组 (L240–L244)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sighand_struct]
```c
	struct sighand_struct {
		spinlock_t		siglock;
		refcount_t		count;
		// wait_queue_head_t	signalfd_wqh;
		k_sigaction_s	action[_NSIG];
```

### B25 — sigpending由list和signal位集合组成 (L43–L46)

[VERIFIED mykernel/lock_IPC/signal/signal_types.h::sigpending]
```c
	struct sigpending {
		List_hdr_s	list;
		sigset_t	signal;
	};
```

### B26 — init_sigpending只在所读体初始化list而signal清零注释 (L387–L390)

[VERIFIED mykernel/lock_IPC/signal/signal.h::init_sigpending]
```c
		void
		init_sigpending(sigpending_s *sig) {
			// sigemptyset(&sig->signal);
			INIT_LIST_HEADER_S(&sig->list);
```

### B27 — same_thread_group比较signal指针同址 (L208–L212)

[VERIFIED mykernel/lock_IPC/signal/signal.h::same_thread_group]
```c
		PREFIX_STATIC_INLINE
		bool
		same_thread_group(task_s *p1, task_s *p2) {
			return p1->signal == p2->signal;
		}
```

### B28 — task.signal和task.sighand是两个指针 (L351–L353)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		/* Signal handlers: */
		signal_s		*signal;
		sighand_s __rcu	*sighand;
```

### B29 — task.blocked及task.pending为独立字段 (L354–L358)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		sigset_t		blocked;
		sigset_t		real_blocked;
		/* Restored if set_restore_sigmask() was used: */
		sigset_t		saved_sigmask;
		sigpending_s	pending;
```

### B30 — 静态init_signals初始nr_threads=1 (L27–L29)

[VERIFIED mykernel/init/init_task.c::init_signals]
```c
static signal_s init_signals = {
	.nr_threads			= 1,
	// .thread_head		= LIST_HEAD_INIT(init_task.thread_node),
```

### B31 — 静态init_sighand计数1和action及锁 (L56–L59)

[VERIFIED mykernel/init/init_task.c::init_sighand]
```c
static sighand_s init_sighand = {
	.count			= REFCOUNT_INIT(1),
	.action			= { { { .sa_handler = SIG_DFL, } }, },
	.siglock		= __SPIN_LOCK_UNLOCKED(init_sighand.siglock),
```

### B32 — 静态init_task指向两个初始信号对象 (L123–L125)

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.signal				= &init_signals,
	.sighand			= &init_sighand,
	.nsproxy			= &init_nsproxy,
```

### B33 — copy_process另行调用init_sigpending处理子任务私有pending (L1024–L1024)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	init_sigpending(&p->pending);
```

## C — 直接引文

### C01 — 新task.pid实际取pid_nr且CLONE_THREAD分支写tgid (L1205–L1209)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	/* ok, now we should be set up.. */
	p->pid = pid_nr(pid_struct);
	if (clone_flags & CLONE_THREAD) {
		// p->group_leader = current->group_leader;
		p->tgid = current->tgid;
```

### C02 — 非THREAD分支写tgid=pid而group_leader赋值注释 (L1210–L1213)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	} else {
		// p->group_leader = p;
		p->tgid = p->pid;
	}
```

### C03 — thread_group链表初始化与任务工作初始化为注释 (L1219–L1222)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	// p->pdeath_signal = 0;
	// INIT_LIST_HEAD(&p->thread_group);
	// p->task_works = NULL;
	// clear_posix_cputimers_work(p);
```

### C04 — CLONE_PARENT或THREAD取current.real_parent (L1266–L1269)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	/* CLONE_PARENT re-uses the old parent */
	if (clone_flags & (CLONE_PARENT|CLONE_THREAD)) {
		p->real_parent = current->real_parent;
		// p->parent_exec_id = current->parent_exec_id;
```

### C05 — THREAD/非THREAD的exit_signal设置是注释 (L1270–L1273)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		// if (clone_flags & CLONE_THREAD)
		// 	p->exit_signal = -1;
		// else
		// 	p->exit_signal = current->group_leader->exit_signal;
```

### C06 — 非PARENT分支设置real_parent=current但exit_signal注释 (L1274–L1277)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	} else {
		p->real_parent = current;
		// p->parent_exec_id = current->self_exec_id;
		// p->exit_signal = args->exit_signal;
```

### C07 — 按PIDTYPE_MAX清每类task.pid_links (L840–L843)

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid_links]
```c
void init_task_pid_links(task_s *task) {
	enum pid_type type;
	for (type = PIDTYPE_PID; type < PIDTYPE_MAX; ++type)
		INIT_HLIST_NODE(&task->pid_links[type]);
```

### C08 — init_task_pid区分thread_pid和signal.pids字段 (L847–L851)

[VERIFIED mykernel/sched/forkexec/fork.c::init_task_pid]
```c
init_task_pid(task_s *task, enum pid_type type, pid_s *pid_struct) {
	if (type == PIDTYPE_PID)
		task->thread_pid = pid_struct;
	else
		task->signal->pids[type] = pid_struct;
```

### C09 — 真实leader判据是exit_signal非负 (L203–L206)

[VERIFIED mykernel/lock_IPC/signal/signal.h::thread_group_leader]
```c
		bool
		thread_group_leader(task_s *p) {
			return p->exit_signal >= 0;
		}
```

### C10 — get_nr_threads读取signal.nr_threads (L197–L200)

[VERIFIED mykernel/lock_IPC/signal/signal.h::get_nr_threads]
```c
		int
		get_nr_threads(task_s *task) {
			return task->signal->nr_threads;
		}
```

### C11 — task.pid和task.tgid是两个独立整数 (L217–L218)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		pid_t			pid;
		pid_t			tgid;
```

### C12 — task的real_parent与parent是两个指针 (L230–L233)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		/* Real parent process: */
		task_s __rcu	*real_parent;
		/* Recipient of SIGCHLD, wait4() reports: */
		task_s __rcu	*parent;
```

### C13 — task的children/sibling存在但group_leader成员注释 (L238–L240)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		List_hdr_s		children;
		List_s			sibling;
		// task_s			*group_leader;
```

### C14 — task.thread_pid/pid_links存在但thread_node注释 (L251–L254)

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		/* PID/PID hash table linkage. */
		pid_s			*thread_pid;
		HList_s			pid_links[PIDTYPE_MAX];
		// List_s thread_node;
```

### C15 — 初始化PID链并对非0pid建立PIDTYPE_PID指针 (L1310–L1314)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	init_task_pid_links(p);
	if (likely(p->pid != 0)) {
		ptrace_init_task(p, (clone_flags & CLONE_PTRACE) || trace);

		init_task_pid(p, PIDTYPE_PID, pid_struct);
```

### C16 — 按thread_group_leader判据写TGID/PGID/SID来源 (L1315–L1318)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		if (thread_group_leader(p)) {
			init_task_pid(p, PIDTYPE_TGID, pid_struct);
			init_task_pid(p, PIDTYPE_PGID, task_pgrp(current));
			init_task_pid(p, PIDTYPE_SID, task_session(current));
```

### C17 — child_reaper条件设置signal.UNKILLABLE (L1320–L1323)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
			if (is_child_reaper(pid_struct)) {
				ns_of_pid(pid_struct)->child_reaper = p;
				p->signal->flags |= SIGNAL_UNKILLABLE;
			}
```

### C18 — leader分支注册children/all_task并挂接TGID和PGID (L1333–L1337)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
			list_header_add_to_tail(&p->real_parent->children, &p->sibling);
			// list_add_tail_rcu(&p->tasks, &init_task.tasks);
			list_header_add_to_tail(&all_task_lhdr, &p->tasks);
			attach_pid(p, PIDTYPE_TGID);
			attach_pid(p, PIDTYPE_PGID);
```

### C19 — leader分支挂接SID (L1338–L1338)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
			attach_pid(p, PIDTYPE_SID);
```

### C20 — 非leader分支signal.nr_threads/live/sigcnt增量均注释 (L1340–L1343)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		} else {
			// current->signal->nr_threads++;
			// atomic_inc(&current->signal->live);
			// refcount_inc(&current->signal->sigcnt);
```

### C21 — 非leader分支group/thread_node链表追加均注释 (L1344–L1348)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
			// task_join_group_stop(p);
			// list_add_tail_rcu(&p->thread_group,
			// 		  &p->group_leader->thread_group);
			// list_add_tail_rcu(&p->thread_node,
			// 		  &p->signal->thread_head);
```

### C22 — 一般PID链接和全局nr_threads增量 (L1350–L1353)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		attach_pid(p, PIDTYPE_PID);
		nr_threads++;
	}
	total_forks++;
```

### C23 — 成功return p阻断后续错误标签 (L1370–L1372)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	// copy_oom_score_adj(clone_flags, p);

	return p;
```

### C24 — attach_pid取关联pid对象后添加task.pid_links到pid.tasks (L325–L327)

[VERIFIED mykernel/sched/task/pid.c::attach_pid]
```c
void attach_pid(task_s *task, enum pid_type type) {
	pid_s *pid = *task_pid_ptr(task, type);
	hlist_add_head_rcu(&task->pid_links[type], &pid->tasks[type]);
```

### C25 — task_pid_ptr区分thread_pid与signal.pid指针来源 (L315–L319)

[VERIFIED mykernel/sched/task/pid.c::task_pid_ptr]
```c
static pid_s **task_pid_ptr(task_s *task, enum pid_type type) {
	if (type == PIDTYPE_PID)
		return &task->thread_pid;
	else
		return &task->signal->pids[type];
```

### C26 — kernel_clone消费copy_process错误返回 (L1485–L1489)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
	// add_latent_entropy();

	if (IS_ERR(p))
		return PTR_ERR(p);
```

### C27 — kernel_clone从task取得PID对象及数值 (L1497–L1498)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```

### C28 — VFORK时设置vfork_done并init_completion (L1503–L1506)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	if (clone_flags & CLONE_VFORK) {
		p->vfork_done = &vfork;
		init_completion(&vfork);
		// get_task_struct(p);
```

### C29 — 只有copy_process成功后发出wake_up_new_task请求 (L1516–L1516)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	wake_up_new_task(p);
```

### C30 — VFORK等待调用为注释 (L1522–L1525)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	// if (clone_flags & CLONE_VFORK) {
	// 	if (!wait_for_vfork_done(p, &vfork))
	// 		ptrace_event_pid(PTRACE_EVENT_VFORK_DONE, pid_struct);
	// }
```

### C31 — put_pid后向调用者返回nr (L1527–L1529)

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);

	return nr;
```

### C32 — copy_signal失败跳到sighand清理及copy_mm开始 (L1131–L1135)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_signal(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_sighand;
	retval = copy_mm(clone_flags, p);
	if (retval)
```

### C33 — copy_mm或copy_namespaces失败清理的门 (L1134–L1138)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_mm(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_signal;
	retval = copy_namespaces(clone_flags, p);
	if (retval)
```

### C34 — copy_thread失败进入cleanup_io (L1143–L1145)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	retval = copy_thread(p, args);
	if (retval)
		goto bad_fork_cleanup_io;
```

### C35 — alloc_pid错误去cleanup_thread不跳free_pid (L1149–L1153)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (pid_struct != &init_struct_pid) {
		pid_struct = alloc_pid(p->nsproxy->pid_ns_for_children,args->set_tid, args->set_tid_size);
		if (IS_ERR(pid_struct)) {
			retval = PTR_ERR(pid_struct);
			goto bad_fork_cleanup_thread;
```

### C36 — bad_fork_free_pid并到cleanup_thread (L1385–L1389)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_free_pid:
	if (pid_struct != &init_struct_pid)
		free_pid(pid_struct);
bad_fork_cleanup_thread:
	exit_thread(p);
```

### C37 — cleanup_mm按p.mm条件调用mmput (L1395–L1399)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_mm:
	if (p->mm) {
		// mm_clear_owner(p->mm, p);
		mmput(p->mm);
	}
```

### C38 — free_signal_struct注释但__cleanup_sighand活动 (L1400–L1404)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_signal:
	// if (!(clone_flags & CLONE_THREAD))
	// 	free_signal_struct(p->signal);
bad_fork_cleanup_sighand:
	__cleanup_sighand(p->sighand);
```

### C39 — sighand清理后是exit_fs与exit_files请求 (L1405–L1408)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_fs:
	exit_fs(p); /* blocking */
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
```

### C40 — fork_out最终return ERR_PTR(retval) (L1428–L1432)

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
fork_out:
	spin_lock_irq(&current->sighand->siglock);
	// hlist_del_init(&delayed.node);
	spin_unlock_irq(&current->sighand->siglock);
	return ERR_PTR(retval);
```

