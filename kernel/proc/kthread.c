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
// #include <linux/completion.h>
#include <linux/kernel/err.h>
// #include <linux/cgroup.h>
// #include <linux/cpuset.h>
#include <uapi/kernel/unistd.h>
#include <linux/fs/file.h>
// #include <linux/export.h>
#include <linux/kernel/mutex.h>
#include <linux/kernel/slab.h>
// #include <linux/freezer.h>
// #include <linux/ptrace.h>
// #include <linux/uaccess.h>
// #include <linux/numa.h>
// #include <linux/sched/isolation.h>
// #include <trace/events/sched.h>


// static DEFINE_SPINLOCK(kthread_create_lock);
// static LIST_HEAD(kthread_create_list);
// task_s *kthreadd_task;

typedef struct kthread_create_info
{
	/* Information passed to kthread() from kthreadd. */
	int		(*threadfn)(void *data);
	void	*data;
	int		node;

	/* Result passed back to kthread_create() from kthreadd. */
	task_s	*result;
	// struct completion *done;

	List_s	list;
} kthd_create_info_s;

typedef struct kthread {
	unsigned long flags;
	unsigned int cpu;
	int result;
	int (*threadfn)(void *);
	void *data;
	// mm_segment_t oldfs;
	// struct completion parked;
	// struct completion exited;
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


static int kthread(void *_create)
{
	// static const struct sched_param param = { .sched_priority = 0 };
	// /* Copy data: it's on kthread's stack */
	// struct kthread_create_info *create = _create;
	// int (*threadfn)(void *data) = create->threadfn;
	// void *data = create->data;
	// struct completion *done;
	// struct kthread *self;
	// int ret;

	// self = to_kthread(current);

	// /* If user was SIGKILLed, I release the structure. */
	// done = xchg(&create->done, NULL);
	// if (!done) {
	// 	kfree(create);
	// 	kthread_exit(-EINTR);
	// }

	// self->threadfn = threadfn;
	// self->data = data;

	// /*
	//  * The new thread inherited kthreadd's priority and CPU mask. Reset
	//  * back to default in case they have been changed.
	//  */
	// sched_setscheduler_nocheck(current, SCHED_NORMAL, &param);
	// set_cpus_allowed_ptr(current, housekeeping_cpumask(HK_FLAG_KTHREAD));

	// /* OK, tell user we're spawned, wait for stop or wakeup */
	// __set_current_state(TASK_UNINTERRUPTIBLE);
	// create->result = current;
	// /*
	//  * Thread is going to call schedule(), do not preempt it,
	//  * or the creator may spend more time in wait_task_inactive().
	//  */
	// preempt_disable();
	// complete(done);
	// schedule_preempt_disabled();
	// preempt_enable();

	// ret = -EINTR;
	// if (!test_bit(KTHREAD_SHOULD_STOP, &self->flags)) {
	// 	cgroup_kthread_ready();
	// 	__kthread_parkme(self);
	// 	ret = threadfn(data);
	// }
	// kthread_exit(ret);
}

static void create_kthread(kthd_create_info_s *create)
{
	int pid;

	/* We want our own signal handler (we take no signals by default). */
	pid = kernel_thread(kthread, create, CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
		/* If user was SIGKILLed, I release the structure. */
	// 	struct completion *done = xchg(&create->done, NULL);

	// 	if (!done) {
	// 		kfree(create);
	// 		return;
	// 	}
	// 	create->result = ERR_PTR(pid);
	// 	complete(done);
	}
}