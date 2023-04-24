// SPDX-License-Identifier: GPL-2.0-only
/* Kernel thread helper functions.
 *   Copyright (C) 2004 IBM Corporation, Rusty Russell.
 *   Copyright (C) 2009 Red Hat, Inc.
 *
 * Creation is done via kthreadd, so that we get a clean environment
 * even if we're invoked from userspace (think modprobe, hotplug cpu,
 * etc.).
 */
// #include <uapi/linux/sched/types.h>
#include <linux/mm/mm.h>
// #include <linux/mmu_context.h>
#include <linux/kernel/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/kernel/kthread.h>
#include <linux/kernel/completion.h>
#include <linux/kernel/err.h>
// #include <linux/cgroup.h>
// #include <linux/cpuset.h>
#include <uapi/linux/unistd.h>
#include <linux/fs/file.h>
// #include <linux/export.h>
#include <linux/kernel/mutex.h>
#include <linux/kernel/slab.h>
// #include <linux/freezer.h>
#include <linux/kernel/ptrace.h>
// #include <linux/uaccess.h>
// #include <linux/numa.h>
// #include <linux/sched/isolation.h>
// #include <trace/events/sched.h>


#include <obsolete/arch_proto.h>

static DEFINE_SPINLOCK(kthread_create_lock);
static LIST_HDR_S(kthread_create_list);
task_s *kthreadd_task;

// typedef struct kthread_create_info
// {
// 	/* Information passed to kthread() from kthreadd. */
// 	int		(*threadfn)(void *data);
// 	void	*data;
// 	int		node;

// 	/* Result passed back to kthread_create() from kthreadd. */
// 	task_s	*result;
// 	// completion_s *done;

// 	List_s	list;
// } kthd_create_info_s;

typedef struct kthread {
	unsigned long flags;
	unsigned int cpu;
	int result;
	int (*threadfn)(void *);
	void *data;
	// mm_segment_t oldfs;
	// completion_s parked;
	// completion_s exited;
// #ifdef CONFIG_BLK_CGROUP
	// struct cgroup_subsys_state *blkcg_css;
// #endif
	/* To store the full name if task comm is truncated. */
	char *full_name;
} kthread_s;

enum KTHREAD_BITS {
	KTHREAD_IS_PER_CPU = 0,
	KTHREAD_SHOULD_STOP,
	KTHREAD_SHOULD_PARK,
};


int kthread(void *_create)
{
	// static const struct sched_param param = { .sched_priority = 0 };
	/* Copy data: it's on kthread's stack */
	kthd_create_info_s *create = _create;
	int (*threadfn)(void *data) = create->threadfn;
	void *data = create->data;
	completion_s *done;
	kthread_s *self;
	int ret;

	// self = to_kthread(current);

	/* If user was SIGKILLed, I release the structure. */
	done = xchg(&create->done, NULL);
	if (!done) {
		kfree(create);
	// 	kthread_exit(-EINTR);
	}

	// self->threadfn = threadfn;
	// self->data = data;

	// /*
	//  * The new thread inherited kthreadd's priority and CPU mask. Reset
	//  * back to default in case they have been changed.
	//  */
	// sched_setscheduler_nocheck(current, SCHED_NORMAL, &param);
	// set_cpus_allowed_ptr(current, housekeeping_cpumask(HK_FLAG_KTHREAD));

	/* OK, tell user we're spawned, wait for stop or wakeup */
	// __set_current_state(TASK_UNINTERRUPTIBLE);
	create->result = current;
	/*
	 * Thread is going to call schedule(), do not preempt it,
	 * or the creator may spend more time in wait_task_inactive().
	 */
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
	preempt_enable_no_resched();

	// ret = -EINTR;
	// if (!test_bit(KTHREAD_SHOULD_STOP, &self->flags)) {
	// 	cgroup_kthread_ready();
	// 	__kthread_parkme(self);
		ret = threadfn(data);
	// }
	// kthread_exit(ret);
}

static void create_kthread(kthd_create_info_s *create) {
	int pid;

	/* We want our own signal handler (we take no signals by default). */
	pid = kernel_thread(kthread, create, CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
		/* If user was SIGKILLed, I release the structure. */
		completion_s *done = xchg(&create->done, NULL);

		if (!done) {
			kfree(create);
			return;
		}
		create->result = ERR_PTR(pid);
		complete(done);
	}
}

task_s *myos_kthread_create(int (*threadfn)(void *data),
		void *data, char *threadname)
{
	DECLARE_COMPLETION_ONSTACK(done);
	task_s *task;
	kthd_create_info_s *create =
			kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);

	if (!create)
		return ERR_PTR(-ENOMEM);
	create->threadfn = threadfn;
	create->data = data;
	create->done = &done;
	list_init(&create->list, create);

	spin_lock(&kthread_create_lock);
	list_hdr_enqueue(&kthread_create_list, &create->list);
	spin_unlock_no_resched(&kthread_create_lock);

	wake_up_process(kthreadd_task);
	// /*
	//  * Wait for completion in killable state, for I might be chosen by
	//  * the OOM killer while kthreadd is trying to allocate memory for
	//  * new kernel thread.
	//  */
	// if (unlikely(wait_for_completion_killable(&done))) {
	// 	/*
	// 	 * If I was SIGKILLed before kthreadd (or new kernel thread)
	// 	 * calls complete(), leave the cleanup of this structure to
	// 	 * that thread.
	// 	 */
	// 	if (xchg(&create->done, NULL))
	// 		return ERR_PTR(-EINTR);
	// 	/*
	// 	 * kthreadd (or new kernel thread) will call complete()
	// 	 * shortly.
	// 	 */
		wait_for_completion(&done);
	// }
	task = create->result;
	// if (!IS_ERR(task))   
	// 	char name[TASK_COMM_LEN];
	// 	va_list aq;
	// 	int len;

	// 	/*
	// 	 * task is already visible to other tasks, so updating
	// 	 * COMM must be protected.
	// 	 */
	// 	va_copy(aq, args);
	// 	len = vsnprintf(name, sizeof(name), namefmt, aq);
	// 	va_end(aq);
	// 	if (len >= TASK_COMM_LEN) {
	// 		struct kthread *kthread = to_kthread(task);

	// 		/* leave it truncated when out of memory. */
	// 		kthread->full_name = kvasprintf(GFP_KERNEL, namefmt, args);
	// 	}
	// 	set_task_comm(task, name);
		set_task_comm(task, threadname);
	// }
	kfree(create);
	return task;
}



int kthreadd(void *unused)
{
	// task_s *tsk = current;

	/* Setup a clean context for our children to inherit. */
	// set_task_comm(tsk, "kthreadd");
	// ignore_signals(tsk);
	// set_cpus_allowed_ptr(tsk, housekeeping_cpumask(HK_FLAG_KTHREAD));
	// set_mems_allowed(node_states[N_MEMORY]);

	current->flags |= PF_NOFREEZE;
	// cgroup_init_kthreadd();

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (kthread_create_list.count == 0)
			schedule();
		__set_current_state(TASK_RUNNING);

		spin_lock(&kthread_create_lock);
		while (kthread_create_list.count != 0) {
			kthd_create_info_s *create;

			List_s *lp = list_hdr_dequeue(&kthread_create_list);
			create = container_of(lp, kthd_create_info_s, list);
			spin_unlock_no_resched(&kthread_create_lock);

			create_kthread(create);

			spin_lock(&kthread_create_lock);
		}
		spin_unlock_no_resched(&kthread_create_lock);
	}

	return 0;
}