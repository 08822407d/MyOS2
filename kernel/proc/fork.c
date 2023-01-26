// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/kernel/fork.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 *  'fork.c' contains the help-routines for the 'fork' system call
 * (see also entry.S and others).
 * Fork is rather simple, once you get the hang of it, but the memory
 * management can be a bitch. See 'mm/memory.c': 'copy_page_range()'
 */

// #include <linux/anon_inodes.h>
#include <linux/kernel/slab.h>
// #include <linux/sched/autogroup.h>
#include <linux/sched/mm.h>
// #include <linux/sched/coredump.h>
// #include <linux/sched/user.h>
// #include <linux/sched/numa_balancing.h>
// #include <linux/sched/stat.h>
#include <linux/sched/task.h>
// #include <linux/sched/task_stack.h>
// #include <linux/sched/cputime.h>
// #include <linux/seq_file.h>
// #include <linux/rtmutex.h>
#include <linux/init/init.h>
#include <uapi/linux/unistd.h>
// #include <linux/module.h>
// #include <linux/vmalloc.h>
#include <linux/kernel/completion.h>
// #include <linux/personality.h>
// #include <linux/mempolicy.h>
// #include <linux/sem.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
// #include <linux/iocontext.h>
// #include <linux/key.h>
// #include <linux/binfmts.h>
// #include <linux/mman.h>
// #include <linux/mmu_notifier.h>
#include <linux/fs/fs.h>
#include <linux/mm/mm.h>
// #include <linux/mm_inline.h>
// #include <linux/vmacache.h>
// #include <linux/nsproxy.h>
// #include <linux/capability.h>
// #include <linux/cpu.h>
// #include <linux/cgroup.h>
// #include <linux/security.h>
// #include <linux/hugetlb.h>
// #include <linux/seccomp.h>
// #include <linux/swap.h>
#include <linux/kernel/syscalls.h>
// #include <linux/jiffies.h>
// #include <linux/futex.h>
// #include <linux/compat.h>
#include <linux/kernel/kthread.h>
// #include <linux/task_io_accounting_ops.h>
// #include <linux/rcupdate.h>
#include <linux/kernel/ptrace.h>
#include <linux/kernel/mount.h>
// #include <linux/audit.h>
// #include <linux/memcontrol.h>
// #include <linux/ftrace.h>
// #include <linux/proc_fs.h>
// #include <linux/profile.h>
// #include <linux/rmap.h>
// #include <linux/ksm.h>
// #include <linux/acct.h>
// #include <linux/userfaultfd_k.h>
// #include <linux/tsacct_kern.h>
// #include <linux/cn_proc.h>
// #include <linux/freezer.h>
// #include <linux/delayacct.h>
// #include <linux/taskstats_kern.h>
// #include <linux/random.h>
#include <linux/device/tty.h>
#include <linux/sched/fs_struct.h>
#include <uapi/linux/magic.h>
// #include <linux/perf_event.h>
// #include <linux/posix-timers.h>
// #include <linux/user-return-notifier.h>
// #include <linux/oom.h>
// #include <linux/khugepaged.h>
// #include <linux/signalfd.h>
// #include <linux/uprobes.h>
// #include <linux/aio.h>
#include <linux/kernel/compiler.h>
// #include <linux/sysctl.h>
// #include <linux/kcov.h>
// #include <linux/livepatch.h>
// #include <linux/thread_info.h>
// #include <linux/stackleak.h>
// #include <linux/kasan.h>
// #include <linux/scs.h>
// #include <linux/io_uring.h>
// #include <linux/bpf.h>

// #include <asm/pgalloc.h>
// #include <linux/uaccess.h>
// #include <asm/mmu_context.h>
// #include <asm/cacheflush.h>
// #include <asm/tlbflush.h>

// #include <trace/events/sched.h>

// #define CREATE_TRACE_POINTS
// #include <trace/events/task.h>


static inline task_s *myos_alloc_task_struct() {
	PCB_u *ret_val = kmalloc(sizeof(PCB_u), GFP_KERNEL);
	memset(&(ret_val->task), 0, sizeof(task_s));
	return &ret_val->task;
}

static inline void free_task_struct(task_s *tsk) {
	kfree(tsk);
}


static void release_task_stack(task_s *tsk)
{
// 	if (WARN_ON(READ_ONCE(tsk->__state) != TASK_DEAD))
// 		return;  /* Better to leak the stack than to free prematurely */

// 	account_kernel_stack(tsk, -1);
// 	free_thread_stack(tsk);
// 	tsk->stack = NULL;
// #ifdef CONFIG_VMAP_STACK
// 	tsk->stack_vm_area = NULL;
// #endif
}

// #ifdef CONFIG_THREAD_INFO_IN_TASK
void put_task_stack(task_s *tsk)
{
	// if (refcount_dec_and_test(&tsk->stack_refcount))
		release_task_stack(tsk);
}
// #endif

void free_task(task_s *tsk)
{
	// release_user_cpus_ptr(tsk);
	// scs_release(tsk);

// #ifndef CONFIG_THREAD_INFO_IN_TASK
	// /*
	//  * The task is finally done with both the stack and thread_info,
	//  * so free both.
	//  */
	// release_task_stack(tsk);
// #else
	// /*
	//  * If the task had a separate stack allocation, it should be gone
	//  * by now.
	//  */
	// WARN_ON_ONCE(refcount_read(&tsk->stack_refcount) != 0);
// #endif
	// rt_mutex_debug_task_free(tsk);
	// ftrace_graph_exit_task(tsk);
	// arch_release_task_struct(tsk);
	// if (tsk->flags & PF_KTHREAD)
	// 	free_kthread_struct(tsk);
	free_task_struct(tsk);
}


static inline mm_s *allocate_mm() {
	return kzalloc(sizeof(mm_s), GFP_KERNEL);
}

static inline void free_mm(mm_s *mm) {
	kfree(mm);
}


void set_task_stack_end_magic(task_s *tsk)
{
	unsigned long *stackend;

	// stackend = end_of_stack(tsk);
	// *stackend = STACK_END_MAGIC;	/* for overflow detection */
}

static task_s *dup_task_struct(task_s *orig)
{
	task_s *tsk;
	unsigned long *stack;
	// struct vm_struct *stack_vm_area __maybe_unused;
	int err;

	// tsk = alloc_task_struct_node(node);
	tsk = myos_alloc_task_struct();
	if (!tsk)
		return NULL;

	// stack = alloc_thread_stack_node(tsk, node);
	stack = (unsigned long *)((unsigned long)tsk + THREAD_SIZE);
	if (!stack)
		goto free_tsk;

	// if (memcg_charge_kernel_stack(tsk))
	// 	goto free_stack;

	// stack_vm_area = task_stack_vm_area(tsk);

	// err = arch_dup_task_struct(tsk, orig);
	*tsk = *orig;
	err = 0;

	/*
	 * arch_dup_task_struct() clobbers the stack-related fields.  Make
	 * sure they're properly initialized before using any stack-related
	 * functions again.
	 */
	tsk->stack = stack;
// #ifdef CONFIG_VMAP_STACK
	// tsk->stack_vm_area = stack_vm_area;
// #endif
// #ifdef CONFIG_THREAD_INFO_IN_TASK
	// refcount_set(&tsk->stack_refcount, 1);
// #endif

	if (err)
		goto free_stack;

	// err = scs_prepare(tsk, node);
	// if (err)
	// 	goto free_stack;

// #ifdef CONFIG_SECCOMP
	// /*
	//  * We must handle setting up seccomp filters once we're under
	//  * the sighand lock in case orig has changed between now and
	//  * then. Until then, filter must be NULL to avoid messing up
	//  * the usage counts on the error path calling free_task.
	//  */
	// tsk->seccomp.filter = NULL;
// #endif

	// setup_thread_stack(tsk, orig);
	// clear_user_return_notifier(tsk);
	// clear_tsk_need_resched(tsk);
	set_task_stack_end_magic(tsk);
	// clear_syscall_work_syscall_user_dispatch(tsk);

// #ifdef CONFIG_STACKPROTECTOR
	// tsk->stack_canary = get_random_canary();
// #endif
	// if (orig->cpus_ptr == &orig->cpus_mask)
	// 	tsk->cpus_ptr = &tsk->cpus_mask;
	// dup_user_cpus_ptr(tsk, orig, node);

	// /*
	//  * One for the user space visible state that goes away when reaped.
	//  * One for the scheduler.
	//  */
	// refcount_set(&tsk->rcu_users, 2);
	// /* One for the rcu users */
	// refcount_set(&tsk->usage, 1);
// #ifdef CONFIG_BLK_DEV_IO_TRACE
	// tsk->btrace_seq = 0;
// #endif
	// tsk->splice_pipe = NULL;
	// tsk->task_frag.page = NULL;
	// tsk->wake_q.next = NULL;
	// tsk->worker_private = NULL;

	// account_kernel_stack(tsk, 1);

	// kcov_task_init(tsk);
	// kmap_local_fork(tsk);

// #ifdef CONFIG_FAULT_INJECTION
	// tsk->fail_nth = 0;
// #endif

// #ifdef CONFIG_BLK_CGROUP
	// tsk->throttle_queue = NULL;
	// tsk->use_memdelay = 0;
// #endif

// #ifdef CONFIG_MEMCG
	// tsk->active_memcg = NULL;
// #endif
	return tsk;

free_stack:
	// free_thread_stack(tsk);
free_tsk:
	free_task_struct(tsk);
	return NULL;
}

// #include <linux/init_task.h>

static __always_inline void mm_clear_owner(mm_s *mm, task_s *p)
{
// #ifdef CONFIG_MEMCG
	if (mm->owner == p)
		WRITE_ONCE(mm->owner, NULL);
// #endif
}

static void mm_init_owner(mm_s *mm, task_s *p)
{
// #ifdef CONFIG_MEMCG
	mm->owner = p;
// #endif
}

static mm_s *mm_init(mm_s *mm, task_s *p)
{
// 	mm->mmap = NULL;
// 	mm->mm_rb = RB_ROOT;
// 	mm->vmacache_seqnum = 0;
	atomic_set(&mm->mm_users, 1);
	atomic_set(&mm->mm_refcount, 1);
// 	seqcount_init(&mm->write_protect_seq);
// 	mmap_init_lock(mm);
// 	INIT_LIST_HEAD(&mm->mmlist);
// 	mm_pgtables_bytes_init(mm);
// 	mm->map_count = 0;
// 	mm->locked_vm = 0;
// 	atomic64_set(&mm->pinned_vm, 0);
// 	memset(&mm->rss_stat, 0, sizeof(mm->rss_stat));
// 	spin_lock_init(&mm->page_table_lock);
// 	spin_lock_init(&mm->arg_lock);
// 	mm_init_cpumask(mm);
// 	mm_init_aio(mm);
// 	mm_init_owner(mm, p);
// 	mm_init_pasid(mm);
// 	RCU_INIT_POINTER(mm->exe_file, NULL);
// 	mmu_notifier_subscriptions_init(mm);
// 	init_tlb_flush_pending(mm);
// #if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
// 	mm->pmd_huge_pte = NULL;
// #endif
// 	mm_init_uprobes_state(mm);
// 	hugetlb_count_init(mm);

// 	if (current->mm) {
// 		mm->flags = current->mm->flags & MMF_INIT_MASK;
// 		mm->def_flags = current->mm->def_flags & VM_INIT_DEF_MASK;
// 	} else {
// 		mm->flags = default_dump_filter;
// 		mm->def_flags = 0;
// 	}

// 	if (mm_alloc_pgd(mm))
// 		goto fail_nopgd;

// 	if (init_new_context(p, mm))
// 		goto fail_nocontext;

// 	mm->user_ns = get_user_ns(user_ns);
// 	return mm;

// fail_nocontext:
// 	mm_free_pgd(mm);
// fail_nopgd:
// 	free_mm(mm);
// 	return NULL;
}

static inline void __mmput(mm_s *mm)
{
	// VM_BUG_ON(atomic_read(&mm->mm_users));

	// uprobe_clear_state(mm);
	// exit_aio(mm);
	// ksm_exit(mm);
	// khugepaged_exit(mm); /* must run before exit_mmap */
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
	// set_mm_exe_file(mm, NULL);
	// if (!list_empty(&mm->mmlist)) {
	// 	spin_lock(&mmlist_lock);
	// 	list_del(&mm->mmlist);
	// 	spin_unlock(&mmlist_lock);
	// }
	// if (mm->binfmt)
	// 	module_put(mm->binfmt->module);
	// mmdrop(mm);
}

/*
 * Decrement the use count and release all resources for an mm.
 */
void mmput(mm_s *mm)
{
	// might_sleep();

	if (atomic_dec_and_test(&mm->mm_users))
		__mmput(mm);
}


// /**
//  * get_task_mm - acquire a reference to the task's mm
//  *
//  * Returns %NULL if the task has no mm.  Checks PF_KTHREAD (meaning
//  * this kernel workthread has transiently adopted a user mm with use_mm,
//  * to do its AIO) is not set and if so returns a reference to it, after
//  * bumping up the use count.  User must release the mm via mmput()
//  * after use.  Typically used by /proc and ptrace.
//  */
// struct mm_struct *get_task_mm(struct task_struct *task)
// {
// 	struct mm_struct *mm;

// 	task_lock(task);
// 	mm = task->mm;
// 	if (mm) {
// 		if (task->flags & PF_KTHREAD)
// 			mm = NULL;
// 		else
// 			mmget(mm);
// 	}
// 	task_unlock(task);
// 	return mm;
// }

// struct mm_struct *mm_access(struct task_struct *task, unsigned int mode)
// {
// 	struct mm_struct *mm;
// 	int err;

// 	err =  down_read_killable(&task->signal->exec_update_lock);
// 	if (err)
// 		return ERR_PTR(err);

// 	mm = get_task_mm(task);
// 	if (mm && mm != current->mm &&
// 			!ptrace_may_access(task, mode)) {
// 		mmput(mm);
// 		mm = ERR_PTR(-EACCES);
// 	}
// 	up_read(&task->signal->exec_update_lock);

// 	return mm;
// }

// static void complete_vfork_done(struct task_struct *tsk)
// {
// 	completion_s *vfork;

// 	task_lock(tsk);
// 	vfork = tsk->vfork_done;
// 	if (likely(vfork)) {
// 		tsk->vfork_done = NULL;
// 		complete(vfork);
// 	}
// 	task_unlock(tsk);
// }

// static int wait_for_vfork_done(struct task_struct *child,
// 				completion_s *vfork)
// {
// 	int killed;

// 	freezer_do_not_count();
// 	cgroup_enter_frozen();
// 	killed = wait_for_completion_killable(vfork);
// 	cgroup_leave_frozen(false);
// 	freezer_count();

// 	if (killed) {
// 		task_lock(child);
// 		child->vfork_done = NULL;
// 		task_unlock(child);
// 	}

// 	put_task_struct(child);
// 	return killed;
// }

// /* Please note the differences between mmput and mm_release.
//  * mmput is called whenever we stop holding onto a mm_struct,
//  * error success whatever.
//  *
//  * mm_release is called after a mm_struct has been removed
//  * from the current process.
//  *
//  * This difference is important for error handling, when we
//  * only half set up a mm_struct for a new process and need to restore
//  * the old one.  Because we mmput the new mm_struct before
//  * restoring the old one. . .
//  * Eric Biederman 10 January 1998
//  */
// static void mm_release(struct task_struct *tsk, struct mm_struct *mm)
// {
// 	uprobe_free_utask(tsk);

// 	/* Get rid of any cached register state */
// 	deactivate_mm(tsk, mm);

// 	/*
// 	 * Signal userspace if we're not exiting with a core dump
// 	 * because we want to leave the value intact for debugging
// 	 * purposes.
// 	 */
// 	if (tsk->clear_child_tid) {
// 		if (atomic_read(&mm->mm_users) > 1) {
// 			/*
// 			 * We don't check the error code - if userspace has
// 			 * not set up a proper pointer then tough luck.
// 			 */
// 			put_user(0, tsk->clear_child_tid);
// 			do_futex(tsk->clear_child_tid, FUTEX_WAKE,
// 					1, NULL, NULL, 0, 0);
// 		}
// 		tsk->clear_child_tid = NULL;
// 	}

// 	/*
// 	 * All done, finally we can wake up parent and return this mm to him.
// 	 * Also kthread_stop() uses this completion for synchronization.
// 	 */
// 	if (tsk->vfork_done)
// 		complete_vfork_done(tsk);
// }

// void exit_mm_release(struct task_struct *tsk, struct mm_struct *mm)
// {
// 	futex_exit_release(tsk);
// 	mm_release(tsk, mm);
// }

// void exec_mm_release(struct task_struct *tsk, struct mm_struct *mm)
// {
// 	futex_exec_release(tsk);
// 	mm_release(tsk, mm);
// }

/**
 * dup_mm() - duplicates an existing mm structure
 * @tsk: the task_struct with which the new mm will be associated.
 * @oldmm: the mm to duplicate.
 *
 * Allocates a new mm structure and duplicates the provided @oldmm structure
 * content into it.
 *
 * Return: the duplicated mm or NULL on failure.
 */
static mm_s *dup_mm(task_s *tsk, mm_s *oldmm)
{
	mm_s *mm;
	int err;

	mm = allocate_mm();
	if (!mm)
		goto fail_nomem;

	memcpy(mm, oldmm, sizeof(*mm));

	if (!mm_init(mm, tsk))
		goto fail_nomem;

	// err = dup_mmap(mm, oldmm);
	// if (err)
	// 	goto free_pt;

// 	mm->hiwater_rss = get_mm_rss(mm);
// 	mm->hiwater_vm = mm->total_vm;

// 	if (mm->binfmt && !try_module_get(mm->binfmt->module))
// 		goto free_pt;

	return mm;

free_pt:
	/* don't put binfmt in mmput, we haven't got module yet */
	// mm->binfmt = NULL;
	mm_init_owner(mm, NULL);
	mmput(mm);

fail_nomem:
	return NULL;
}

static int copy_mm(unsigned long clone_flags, task_s *tsk)
{
	mm_s *mm, *oldmm;

	// tsk->min_flt = tsk->maj_flt = 0;
	// tsk->nvcsw = tsk->nivcsw = 0;
// #ifdef CONFIG_DETECT_HUNG_TASK
	// tsk->last_switch_count = tsk->nvcsw + tsk->nivcsw;
	// tsk->last_switch_time = 0;
// #endif

	tsk->mm = NULL;
	tsk->active_mm = NULL;

	/*
	 * Are we cloning a kernel thread?
	 *
	 * We need to steal a active VM for that..
	 */
	oldmm = current->mm;
	if (!oldmm)
		return 0;

	/* initialize the new vmacache entries */
	// vmacache_flush(tsk);

	if (clone_flags & CLONE_VM) {
		mmget(oldmm);
		mm = oldmm;
	} else {
		mm = dup_mm(tsk, current->mm);
		if (!mm)
			return -ENOMEM;
	}

	tsk->mm = mm;
	tsk->active_mm = mm;
	return 0;
}

static int copy_fs(unsigned long clone_flags, task_s *tsk)
{
	taskfs_s *fs = current->fs;
	if (clone_flags & CLONE_FS) {
		/* tsk->fs is already what we want */
		// spin_lock(&fs->lock);
		if (fs->in_exec) {
			// spin_unlock(&fs->lock);
			return -EAGAIN;
		}
		fs->users++;
		// spin_unlock(&fs->lock);
		return 0;
	}
	tsk->fs = copy_fs_struct(fs);
	if (!tsk->fs)
		return -ENOMEM;
	return 0;
}

static int copy_files(unsigned long clone_flags, task_s *tsk)
{
	files_struct_s *oldf, *newf;
	int error = 0;

	/*
	 * A background process may not have any files ...
	 */
	oldf = current->files;
	if (!oldf)
		goto out;

	if (clone_flags & CLONE_FILES) {
		atomic_inc(&oldf->refcount);
		goto out;
	}

	newf = dup_fd(oldf, NR_OPEN_MAX, &error);
	if (!newf)
		goto out;

	tsk->files = newf;
	error = 0;
out:
	return error;
}

static int copy_sighand(unsigned long clone_flags, task_s *tsk)
{
	// struct sighand_struct *sig;

	// if (clone_flags & CLONE_SIGHAND) {
	// 	refcount_inc(&current->sighand->count);
	// 	return 0;
	// }
	// sig = kmem_cache_alloc(sighand_cachep, GFP_KERNEL);
	// RCU_INIT_POINTER(tsk->sighand, sig);
	// if (!sig)
	// 	return -ENOMEM;

	// refcount_set(&sig->count, 1);
	// spin_lock_irq(&current->sighand->siglock);
	// memcpy(sig->action, current->sighand->action, sizeof(sig->action));
	// spin_unlock_irq(&current->sighand->siglock);

	// /* Reset all signal handler not set to SIG_IGN to SIG_DFL. */
	// if (clone_flags & CLONE_CLEAR_SIGHAND)
	// 	flush_signal_handlers(tsk, 0);

	return 0;
}

// void __cleanup_sighand(struct sighand_struct *sighand)
// {
// 	if (refcount_dec_and_test(&sighand->count)) {
// 		signalfd_cleanup(sighand);
// 		/*
// 		 * sighand_cachep is SLAB_TYPESAFE_BY_RCU so we can free it
// 		 * without an RCU grace period, see __lock_task_sighand().
// 		 */
// 		kmem_cache_free(sighand_cachep, sighand);
// 	}
// }

// /*
//  * Initialize POSIX timer handling for a thread group.
//  */
// static void posix_cpu_timers_init_group(struct signal_struct *sig)
// {
// 	struct posix_cputimers *pct = &sig->posix_cputimers;
// 	unsigned long cpu_limit;

// 	cpu_limit = READ_ONCE(sig->rlim[RLIMIT_CPU].rlim_cur);
// 	posix_cputimers_group_init(pct, cpu_limit);
// }

static int copy_signal(unsigned long clone_flags, task_s *tsk)
{
	// struct signal_struct *sig;

	// if (clone_flags & CLONE_THREAD)
	// 	return 0;

	// sig = kmem_cache_zalloc(signal_cachep, GFP_KERNEL);
	// tsk->signal = sig;
	// if (!sig)
	// 	return -ENOMEM;

	// sig->nr_threads = 1;
	// atomic_set(&sig->live, 1);
	// refcount_set(&sig->sigcnt, 1);

	// /* list_add(thread_node, thread_head) without INIT_LIST_HEAD() */
	// sig->thread_head = (struct list_head)LIST_HEAD_INIT(tsk->thread_node);
	// tsk->thread_node = (struct list_head)LIST_HEAD_INIT(sig->thread_head);

	// init_waitqueue_head(&sig->wait_chldexit);
	// sig->curr_target = tsk;
	// init_sigpending(&sig->shared_pending);
	// INIT_HLIST_HEAD(&sig->multiprocess);
	// seqlock_init(&sig->stats_lock);
	// prev_cputime_init(&sig->prev_cputime);

// #ifdef CONFIG_POSIX_TIMERS
	// INIT_LIST_HEAD(&sig->posix_timers);
	// hrtimer_init(&sig->real_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	// sig->real_timer.function = it_real_fn;
// #endif

	// task_lock(current->group_leader);
	// memcpy(sig->rlim, current->signal->rlim, sizeof sig->rlim);
	// task_unlock(current->group_leader);

	// posix_cpu_timers_init_group(sig);

	// tty_audit_fork(sig);
	// sched_autogroup_fork(sig);

	// sig->oom_score_adj = current->signal->oom_score_adj;
	// sig->oom_score_adj_min = current->signal->oom_score_adj_min;

	// mutex_init(&sig->cred_guard_mutex);
	// init_rwsem(&sig->exec_update_lock);

	return 0;
}


// static void __delayed_free_task(struct rcu_head *rhp)
// {
// 	struct task_struct *tsk = container_of(rhp, struct task_struct, rcu);

// 	free_task(tsk);
// }

static __always_inline void delayed_free_task(task_s *tsk)
{
	// if (IS_ENABLED(CONFIG_MEMCG))
	// 	call_rcu(&tsk->rcu, __delayed_free_task);
	// else
		free_task(tsk);
}

static void myos_pcb_init(task_s *p, u64 clone_flags)
{
	list_hdr_init(&p->wait_childexit);
	list_init(&p->tasks, p);
	if(clone_flags & CLONE_VM)
		p->flags |= CLONE_VFORK;
}
int myos_copy_mm(unsigned long clone_flags, task_s * new_tsk);
/*
 * This creates a new process as a copy of the old one,
 * but does not actually start it yet.
 *
 * It copies the registers, and all the appropriate
 * parts of the process environment (as per the clone
 * flags). The actual kick-off is left to the caller.
 */
static __latent_entropy task_s
*copy_process(pid_s *pid, kclone_args_s *args)
{
	int pidfd = -1, retval;
	task_s *p;
	// struct multiprocess_signals delayed;
	file_s *pidfile = NULL;
	u64 clone_flags = args->flags;
	// struct nsproxy *nsp = current->nsproxy;

	/*
	 * Don't allow sharing the root directory with processes in a different
	 * namespace
	 */
	if ((clone_flags & (CLONE_NEWNS|CLONE_FS)) == (CLONE_NEWNS|CLONE_FS))
		return ERR_PTR(-EINVAL);

	if ((clone_flags & (CLONE_NEWUSER|CLONE_FS)) == (CLONE_NEWUSER|CLONE_FS))
		return ERR_PTR(-EINVAL);

	/*
	 * Thread groups must share signals as well, and detached threads
	 * can only be started up within the thread group.
	 */
	if ((clone_flags & CLONE_THREAD) && !(clone_flags & CLONE_SIGHAND))
		return ERR_PTR(-EINVAL);

	/*
	 * Shared signal handlers imply shared VM. By way of the above,
	 * thread groups also imply shared VM. Blocking this case allows
	 * for various simplifications in other code.
	 */
	if ((clone_flags & CLONE_SIGHAND) && !(clone_flags & CLONE_VM))
		return ERR_PTR(-EINVAL);

	/*
	//  * Siblings of global init remain as zombies on exit since they are
	//  * not reaped by their parent (swapper). To solve this and to avoid
	//  * multi-rooted process trees, prevent global and container-inits
	//  * from creating siblings.
	//  */
	// if ((clone_flags & CLONE_PARENT) &&
	// 			current->signal->flags & SIGNAL_UNKILLABLE)
	// 	return ERR_PTR(-EINVAL);

	// /*
	//  * If the new process will be in a different pid or user namespace
	//  * do not allow it to share a thread group with the forking task.
	//  */
	// if (clone_flags & CLONE_THREAD) {
	// 	if ((clone_flags & (CLONE_NEWUSER | CLONE_NEWPID)) ||
	// 		(task_active_pid_ns(current) != nsp->pid_ns_for_children))
	// 		return ERR_PTR(-EINVAL);
	// }

	// /*
	//  * If the new process will be in a different time namespace
	//  * do not allow it to share VM or a thread group with the forking task.
	//  */
	// if (clone_flags & (CLONE_THREAD | CLONE_VM)) {
	// 	if (nsp->time_ns != nsp->time_ns_for_children)
	// 		return ERR_PTR(-EINVAL);
	// }

	if (clone_flags & CLONE_PIDFD) {
		/*
		 * - CLONE_DETACHED is blocked so that we can potentially
		 *   reuse it later for CLONE_PIDFD.
		 * - CLONE_THREAD is blocked until someone really needs it.
		 */
		if (clone_flags & (CLONE_DETACHED | CLONE_THREAD))
			return ERR_PTR(-EINVAL);
	}

	// /*
	//  * Force any signals received before this point to be delivered
	//  * before the fork happens.  Collect up signals sent to multiple
	//  * processes that happen during the fork and delay them so that
	//  * they appear to happen after the fork.
	//  */
	// sigemptyset(&delayed.signal);
	// INIT_HLIST_NODE(&delayed.node);

	// spin_lock_irq(&current->sighand->siglock);
	// if (!(clone_flags & CLONE_THREAD))
	// 	hlist_add_head(&delayed.node, &current->signal->multiprocess);
	// recalc_sigpending();
	// spin_unlock_irq(&current->sighand->siglock);
	// retval = -ERESTARTNOINTR;
	// if (task_sigpending(current))
	// 	goto fork_out;

	retval = -ENOMEM;
	p = dup_task_struct(current);
	if (!p)
		goto fork_out;
	// if (args->io_thread) {
	// 	/*
	// 	 * Mark us an IO worker, and block any signal that isn't
	// 	 * fatal or STOP
	// 	 */
	// 	p->flags |= PF_IO_WORKER;
	// 	siginitsetinv(&p->blocked, sigmask(SIGKILL)|sigmask(SIGSTOP));
	// }

	// p->set_child_tid = (clone_flags & CLONE_CHILD_SETTID) ? args->child_tid : NULL;
	// /*
	//  * Clear TID on mm_release()?
	//  */
	// p->clear_child_tid = (clone_flags & CLONE_CHILD_CLEARTID) ? args->child_tid : NULL;

	// ftrace_graph_init_task(p);

	// rt_mutex_init_task(p);

	// lockdep_assert_irqs_enabled();
// #ifdef CONFIG_PROVE_LOCKING
	// DEBUG_LOCKS_WARN_ON(!p->softirqs_enabled);
// #endif
	// retval = copy_creds(p, clone_flags);
	// if (retval < 0)
	// 	goto bad_fork_free;

	// retval = -EAGAIN;
	// if (is_ucounts_overlimit(task_ucounts(p), UCOUNT_RLIMIT_NPROC, rlimit(RLIMIT_NPROC))) {
	// 	if (p->real_cred->user != INIT_USER &&
	// 		!capable(CAP_SYS_RESOURCE) && !capable(CAP_SYS_ADMIN))
	// 		goto bad_fork_cleanup_count;
	// }
	current->flags &= ~PF_NPROC_EXCEEDED;

	// /*
	//  * If multiple threads are within copy_process(), then this check
	//  * triggers too late. This doesn't hurt, the check is only there
	//  * to stop root fork bombs.
	//  */
	// retval = -EAGAIN;
	// if (data_race(nr_threads >= max_threads))
	// 	goto bad_fork_cleanup_count;

	// delayacct_tsk_init(p);	/* Must remain after dup_task_struct() */
	p->flags &= ~(PF_SUPERPRIV | PF_WQ_WORKER | PF_IDLE | PF_NO_SETAFFINITY);
	p->flags |= PF_FORKNOEXEC;
	list_hdr_init(&p->children);
	list_init(&p->sibling, p);
	myos_pcb_init(p, clone_flags);
	// rcu_copy_process(p);
	// p->vfork_done = NULL;
	// spin_lock_init(&p->alloc_lock);

	// init_sigpending(&p->pending);

	// p->utime = p->stime = p->gtime = 0;
// #ifdef CONFIG_ARCH_HAS_SCALED_CPUTIME
	// p->utimescaled = p->stimescaled = 0;
// #endif
	// prev_cputime_init(&p->prev_cputime);

// #ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
	// seqcount_init(&p->vtime.seqcount);
	// p->vtime.starttime = 0;
	// p->vtime.state = VTIME_INACTIVE;
// #endif

// #ifdef CONFIG_IO_URING
	// p->io_uring = NULL;
// #endif

// #if defined(SPLIT_RSS_COUNTING)
	// memset(&p->rss_stat, 0, sizeof(p->rss_stat));
// #endif

	// p->default_timer_slack_ns = current->timer_slack_ns;

// #ifdef CONFIG_PSI
	// p->psi_flags = 0;
// #endif

	// task_io_accounting_init(&p->ioac);
	// acct_clear_integrals(p);

	// posix_cputimers_init(&p->posix_cputimers);

	// p->io_context = NULL;
	// audit_set_context(p, NULL);
	// cgroup_fork(p);
	// if (p->flags & PF_KTHREAD) {
	// 	if (!set_kthread_struct(p))
	// 		goto bad_fork_cleanup_delayacct;
	// }
// #ifdef CONFIG_NUMA
	// p->mempolicy = mpol_dup(p->mempolicy);
	// if (IS_ERR(p->mempolicy)) {
	// 	retval = PTR_ERR(p->mempolicy);
	// 	p->mempolicy = NULL;
	// 	goto bad_fork_cleanup_delayacct;
	// }
// #endif
// #ifdef CONFIG_CPUSETS
	// p->cpuset_mem_spread_rotor = NUMA_NO_NODE;
	// p->cpuset_slab_spread_rotor = NUMA_NO_NODE;
	// seqcount_spinlock_init(&p->mems_allowed_seq, &p->alloc_lock);
// #endif
// #ifdef CONFIG_TRACE_IRQFLAGS
	// memset(&p->irqtrace, 0, sizeof(p->irqtrace));
	// p->irqtrace.hardirq_disable_ip	= _THIS_IP_;
	// p->irqtrace.softirq_enable_ip	= _THIS_IP_;
	// p->softirqs_enabled		= 1;
	// p->softirq_context		= 0;
// #endif

	// p->pagefault_disabled = 0;

// #ifdef CONFIG_LOCKDEP
	// lockdep_init_task(p);
// #endif

// #ifdef CONFIG_DEBUG_MUTEXES
	// p->blocked_on = NULL; /* not blocked yet */
// #endif
// #ifdef CONFIG_BCACHE
	// p->sequential_io	= 0;
	// p->sequential_io_avg	= 0;
// #endif
// #ifdef CONFIG_BPF_SYSCALL
	// RCU_INIT_POINTER(p->bpf_storage, NULL);
	// p->bpf_ctx = NULL;
// #endif

	// /* Perform scheduler related setup. Assign this task to a CPU. */
	// retval = sched_fork(clone_flags, p);
	// if (retval)
	// 	goto bad_fork_cleanup_policy;

	// retval = perf_event_init_task(p, clone_flags);
	// if (retval)
	// 	goto bad_fork_cleanup_policy;
	// retval = audit_alloc(p);
	// if (retval)
	// 	goto bad_fork_cleanup_perf;
	// /* copy all the process information */
	// shm_init_task(p);
	// retval = security_task_alloc(p, clone_flags);
	// if (retval)
	// 	goto bad_fork_cleanup_audit;
	// retval = copy_semundo(clone_flags, p);
	// if (retval)
	// 	goto bad_fork_cleanup_security;
	retval = copy_files(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_semundo;
	retval = copy_fs(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_files;
	retval = copy_sighand(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_fs;
	retval = copy_signal(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_sighand;
	// retval = copy_mm(clone_flags, p);
	retval = myos_copy_mm(clone_flags, p);
	if (retval)
		goto bad_fork_cleanup_signal;
	// retval = copy_namespaces(clone_flags, p);
	// if (retval)
	// 	goto bad_fork_cleanup_mm;
	// retval = copy_io(clone_flags, p);
	// if (retval)
	// 	goto bad_fork_cleanup_namespaces;
	retval = copy_thread(clone_flags, args->stack, args->stack_size, p);
	if (retval)
		goto bad_fork_cleanup_io;

	// stackleak_task_init(p);

	// if (pid != &init_struct_pid) {
	// 	pid = alloc_pid(p->nsproxy->pid_ns_for_children, args->set_tid,
	// 			args->set_tid_size);
	// 	if (IS_ERR(pid)) {
	// 		retval = PTR_ERR(pid);
	// 		goto bad_fork_cleanup_thread;
	// 	}
	// }

	// /*
	//  * This has to happen after we've potentially unshared the file
	//  * descriptor table (so that the pidfd doesn't leak into the child
	//  * if the fd table isn't shared).
	//  */
	// if (clone_flags & CLONE_PIDFD) {
	// 	retval = get_unused_fd_flags(O_RDWR | O_CLOEXEC);
	// 	if (retval < 0)
	// 		goto bad_fork_free_pid;

	// 	pidfd = retval;

	// 	pidfile = anon_inode_getfile("[pidfd]", &pidfd_fops, pid,
	// 					  O_RDWR | O_CLOEXEC);
	// 	if (IS_ERR(pidfile)) {
	// 		put_unused_fd(pidfd);
	// 		retval = PTR_ERR(pidfile);
	// 		goto bad_fork_free_pid;
	// 	}
	// 	get_pid(pid);	/* held by pidfile now */

	// 	retval = put_user(pidfd, args->pidfd);
	// 	if (retval)
	// 		goto bad_fork_put_pidfd;
	// }

// #ifdef CONFIG_BLOCK
	// p->plug = NULL;
// #endif
	// futex_init_task(p);

	/*
	 * sigaltstack should be cleared when sharing the same VM
	 */
	// if ((clone_flags & (CLONE_VM|CLONE_VFORK)) == CLONE_VM)
	// 	sas_ss_reset(p);

	// /*
	//  * Syscall tracing and stepping should be turned off in the
	//  * child regardless of CLONE_PTRACE.
	//  */
	// user_disable_single_step(p);
	// clear_task_syscall_work(p, SYSCALL_TRACE);
// #if defined(CONFIG_GENERIC_ENTRY) || defined(TIF_SYSCALL_EMU)
	// clear_task_syscall_work(p, SYSCALL_EMU);
// #endif
	// clear_tsk_latency_tracing(p);

	// /* ok, now we should be set up.. */
	// p->pid = pid_nr(pid);
	p->pid = myos_pid_nr();
	// if (clone_flags & CLONE_THREAD) {
	// 	p->group_leader = current->group_leader;
	// 	p->tgid = current->tgid;
	// } else {
	// 	p->group_leader = p;
	// 	p->tgid = p->pid;
	// }

	// p->nr_dirtied = 0;
	// p->nr_dirtied_pause = 128 >> (PAGE_SHIFT - 10);
	// p->dirty_paused_when = 0;

	// p->pdeath_signal = 0;
	// INIT_LIST_HEAD(&p->thread_group);
	// p->task_works = NULL;
	// clear_posix_cputimers_work(p);

// #ifdef CONFIG_KRETPROBES
	// p->kretprobe_instances.first = NULL;
// #endif

	// /*
	//  * Ensure that the cgroup subsystem policies allow the new process to be
	//  * forked. It should be noted that the new process's css_set can be changed
	//  * between here and cgroup_post_fork() if an organisation operation is in
	//  * progress.
	//  */
	// retval = cgroup_can_fork(p, args);
	// if (retval)
	// 	goto bad_fork_put_pidfd;

	// /*
	//  * Now that the cgroups are pinned, re-clone the parent cgroup and put
	//  * the new task on the correct runqueue. All this *before* the task
	//  * becomes visible.
	//  *
	//  * This isn't part of ->can_fork() because while the re-cloning is
	//  * cgroup specific, it unconditionally needs to place the task on a
	//  * runqueue.
	//  */
	// sched_cgroup_fork(p, args);

	// /*
	//  * From this point on we must avoid any synchronous user-space
	//  * communication until we take the tasklist-lock. In particular, we do
	//  * not want user-space to be able to predict the process start-time by
	//  * stalling fork(2) after we recorded the start_time but before it is
	//  * visible to the system.
	//  */

	// p->start_time = ktime_get_ns();
	// p->start_boottime = ktime_get_boottime_ns();

	// /*
	//  * Make it visible to the rest of the system, but dont wake it up yet.
	//  * Need tasklist lock for parent etc handling!
	//  */
	// write_lock_irq(&tasklist_lock);

	/* CLONE_PARENT re-uses the old parent */
	if (clone_flags & (CLONE_PARENT|CLONE_THREAD)) {
		p->real_parent = current->real_parent;
		// p->parent_exec_id = current->parent_exec_id;
		// if (clone_flags & CLONE_THREAD)
		// 	p->exit_signal = -1;
		// else
		// 	p->exit_signal = current->group_leader->exit_signal;
	} else {
		p->real_parent = current;
		// p->parent_exec_id = current->self_exec_id;
		// p->exit_signal = args->exit_signal;
	}

	// klp_copy_process(p);

	// sched_core_fork(p);

	// spin_lock(&current->sighand->siglock);

	// /*
	//  * Copy seccomp details explicitly here, in case they were changed
	//  * before holding sighand lock.
	//  */
	// copy_seccomp(p);

	// rseq_fork(p, clone_flags);

	// /* Don't start children in a dying pid namespace */
	// if (unlikely(!(ns_of_pid(pid)->pid_allocated & PIDNS_ADDING))) {
	// 	retval = -ENOMEM;
	// 	goto bad_fork_cancel_cgroup;
	// }

	// /* Let kill terminate clone/fork in the middle */
	// if (fatal_signal_pending(current)) {
	// 	retval = -EINTR;
	// 	goto bad_fork_cancel_cgroup;
	// }

	// init_task_pid_links(p);
	if (p->pid) {
	// 	ptrace_init_task(p, (clone_flags & CLONE_PTRACE) || trace);
		ptrace_init_task(p, (clone_flags & CLONE_PTRACE));

	// 	init_task_pid(p, PIDTYPE_PID, pid);
	// 	if (thread_group_leader(p)) {
	// 		init_task_pid(p, PIDTYPE_TGID, pid);
	// 		init_task_pid(p, PIDTYPE_PGID, task_pgrp(current));
	// 		init_task_pid(p, PIDTYPE_SID, task_session(current));

	// 		if (is_child_reaper(pid)) {
	// 			ns_of_pid(pid)->child_reaper = p;
	// 			p->signal->flags |= SIGNAL_UNKILLABLE;
	// 		}
	// 		p->signal->shared_pending.signal = delayed.signal;
	// 		p->signal->tty = tty_kref_get(current->signal->tty);
	// 		/*
	// 		 * Inherit has_child_subreaper flag under the same
	// 		 * tasklist_lock with adding child to the process tree
	// 		 * for propagate_has_child_subreaper optimization.
	// 		 */
	// 		p->signal->has_child_subreaper = p->real_parent->signal->has_child_subreaper ||
	// 						 p->real_parent->signal->is_child_subreaper;
	// 		list_add_tail(&p->sibling, &p->real_parent->children);
			list_hdr_append(&p->real_parent->children, &p->sibling);
	// 		list_add_tail_rcu(&p->tasks, &init_task.tasks);
	// 		attach_pid(p, PIDTYPE_TGID);
	// 		attach_pid(p, PIDTYPE_PGID);
	// 		attach_pid(p, PIDTYPE_SID);
	// 		__this_cpu_inc(process_counts);
	// 	} else {
	// 		current->signal->nr_threads++;
	// 		atomic_inc(&current->signal->live);
	// 		refcount_inc(&current->signal->sigcnt);
	// 		task_join_group_stop(p);
	// 		list_add_tail_rcu(&p->thread_group,
	// 				  &p->group_leader->thread_group);
	// 		list_add_tail_rcu(&p->thread_node,
	// 				  &p->signal->thread_head);
	// 	}
	// 	attach_pid(p, PIDTYPE_PID);
	// 	nr_threads++;
	}
	// total_forks++;
	// hlist_del_init(&delayed.node);
	// spin_unlock(&current->sighand->siglock);
	// syscall_tracepoint_update(p);
	// write_unlock_irq(&tasklist_lock);

	// if (pidfile)
	// 	fd_install(pidfd, pidfile);

	// proc_fork_connector(p);
	// sched_post_fork(p);
	// cgroup_post_fork(p, args);
	// perf_event_fork(p);

	// trace_task_newtask(p, clone_flags);
	// uprobe_copy_process(p, clone_flags);

	// copy_oom_score_adj(clone_flags, p);

	return p;

bad_fork_cancel_cgroup:
	// sched_core_free(p);
	// spin_unlock(&current->sighand->siglock);
	// write_unlock_irq(&tasklist_lock);
	// cgroup_cancel_fork(p, args);
bad_fork_put_pidfd:
	// if (clone_flags & CLONE_PIDFD) {
	// 	fput(pidfile);
	// 	put_unused_fd(pidfd);
	// }
bad_fork_free_pid:
	// if (pid != &init_struct_pid)
	// 	free_pid(pid);
bad_fork_cleanup_thread:
	// exit_thread(p);
bad_fork_cleanup_io:
	// if (p->io_context)
	// 	exit_io_context(p);
bad_fork_cleanup_namespaces:
	// exit_task_namespaces(p);
bad_fork_cleanup_mm:
	if (p->mm) {
		mm_clear_owner(p->mm, p);
		mmput(p->mm);
	}
bad_fork_cleanup_signal:
	// if (!(clone_flags & CLONE_THREAD))
	// 	free_signal_struct(p->signal);
bad_fork_cleanup_sighand:
	// __cleanup_sighand(p->sighand);
bad_fork_cleanup_fs:
	exit_fs(p); /* blocking */
bad_fork_cleanup_files:
	exit_files(p); /* blocking */
bad_fork_cleanup_semundo:
	// exit_sem(p);
bad_fork_cleanup_security:
	// security_task_free(p);
bad_fork_cleanup_audit:
	// audit_free(p);
bad_fork_cleanup_perf:
	// perf_event_free_task(p);
bad_fork_cleanup_policy:
	// lockdep_free_task(p);
bad_fork_cleanup_delayacct:
	// delayacct_tsk_free(p);
bad_fork_cleanup_count:
	// dec_rlimit_ucounts(task_ucounts(p), UCOUNT_RLIMIT_NPROC, 1);
	// exit_creds(p);
bad_fork_free:
	WRITE_ONCE(p->__state, TASK_DEAD);
	put_task_stack(p);
	delayed_free_task(p);
fork_out:
	// spin_lock_irq(&current->sighand->siglock);
	// hlist_del_init(&delayed.node);
	// spin_unlock_irq(&current->sighand->siglock);
	return ERR_PTR(retval);
}

static inline void init_idle_pids(task_s *idle)
{
	// enum pid_type type;

	// for (type = PIDTYPE_PID; type < PIDTYPE_MAX; ++type) {
	// 	INIT_HLIST_NODE(&idle->pid_links[type]); /* not really needed */
	// 	init_task_pid(idle, type, &init_struct_pid);
	// }
}

/*
 *  Ok, this is the main fork-routine.
 *
 * It copies the process, and if successful kick-starts
 * it and waits for it to finish using the VM if required.
 *
 * args->exit_signal is expected to be checked for sanity by the caller.
 */
pid_t kernel_clone(kclone_args_s *args)
{
	u64 clone_flags = args->flags;
	// completion_s vfork;
	// pid_s *pid;
	task_s *p;
	// int trace = 0;
	pid_t nr;

	/*
	 * For legacy clone() calls, CLONE_PIDFD uses the parent_tid argument
	 * to return the pidfd. Hence, CLONE_PIDFD and CLONE_PARENT_SETTID are
	 * mutually exclusive. With clone3() CLONE_PIDFD has grown a separate
	 * field in struct clone_args and it still doesn't make sense to have
	 * them both point at the same memory location. Performing this check
	 * here has the advantage that we don't need to have a separate helper
	 * to check for legacy clone().
	 */
	if ((args->flags & CLONE_PIDFD) &&
		(args->flags & CLONE_PARENT_SETTID) &&
		(args->pidfd == args->parent_tid))
		return -EINVAL;

	/*
	 * Determine whether and which event to report to ptracer.  When
	 * called from kernel_thread or CLONE_UNTRACED is explicitly
	 * requested, no event is reported; otherwise, report if the event
	 * for the type of forking is enabled.
	 */
	// if (!(clone_flags & CLONE_UNTRACED)) {
	// 	if (clone_flags & CLONE_VFORK)
	// 		trace = PTRACE_EVENT_VFORK;
	// 	else if (args->exit_signal != SIGCHLD)
	// 		trace = PTRACE_EVENT_CLONE;
	// 	else
	// 		trace = PTRACE_EVENT_FORK;

	// 	if (likely(!ptrace_event_enabled(current, trace)))
	// 		trace = 0;
	// }

	p = copy_process(NULL, args);
	// add_latent_entropy();

	if (IS_ERR(p))
		return PTR_ERR(p);

	// /*
	//  * Do this prior waking up the new thread - the thread pointer
	//  * might get invalid after that point, if the thread exits quickly.
	//  */
	// trace_sched_process_fork(current, p);

	// pid = get_task_pid(p, PIDTYPE_PID);
	// nr = pid_vnr(pid);
	nr = p->pid;

	// if (clone_flags & CLONE_PARENT_SETTID)
	// 	put_user(nr, args->parent_tid);

	// if (clone_flags & CLONE_VFORK) {
	// 	p->vfork_done = &vfork;
	// 	init_completion(&vfork);
	// 	get_task_struct(p);
	// }

	// wake_up_new_task(p);
	WRITE_ONCE(p->__state, TASK_NEW);
	myos_wake_up_new_task(p);

	// /* forking complete and child started to run, tell ptracer */
	// if (unlikely(trace))
	// 	ptrace_event_pid(trace, pid);

	// if (clone_flags & CLONE_VFORK) {
	// 	if (!wait_for_vfork_done(p, &vfork))
	// 		ptrace_event_pid(PTRACE_EVENT_VFORK_DONE, pid);
	// }

	// put_pid(pid);

	if (args->stack_size != 0)
		((kthd_create_info_s *)(args->stack_size))->result = p;

	return nr;
}

/*
 * Create a kernel thread.
 */
pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	kclone_args_s args = {
		.flags			= ((lower_32_bits(flags) | CLONE_VM |
							CLONE_UNTRACED) & ~CSIGNAL),
		.exit_signal	= (lower_32_bits(flags) & CSIGNAL),
		.stack			= (unsigned long)fn,
		.stack_size		= (unsigned long)arg,
	};

	return kernel_clone(&args);
}


#include <obsolete/arch_proto.h>
/*==============================================================================================*
 *									subcopy & exit funcstions									*
 *==============================================================================================*/
int myos_copy_mm(unsigned long clone_flags, task_s * new_tsk)
{
	int err = -ENOERR;
	mm_s *curr_mm = current->mm;

	page_s *page = NULL;
	pgd_t *new_cr3 = NULL;
	reg_t curr_endstack = task_pt_regs(current)->sp;

	if(clone_flags & CLONE_VM)
	{
		// if vfork(), share the mm_struct
		new_tsk->mm = curr_mm;
		goto exit_cpmm;
	}
	else
	{
		new_tsk->mm = (mm_s *)kzalloc(sizeof(mm_s), GFP_KERNEL);
		memcpy(new_tsk->mm, curr_mm, sizeof(mm_s));
		// pgd_t *new_pgd = kzalloc(sizeof(pgd_t), GFP_KERNEL);
		// new_tsk->mm->pgd_ptr = new_pgd;
		prepair_COW(current);
	}

exit_cpmm:
	return err;
}
int myos_exit_mm(task_s *new_tsk)
{
	int err = -ENOERR;

	if(new_tsk->flags & CLONE_VFORK)
		err = -ENOERR;

	return err;
}
int myos_exit_thread(task_s * new_task)
{
	int err = -ENOERR;
	return err;
}