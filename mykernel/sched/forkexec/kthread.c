// SPDX-License-Identifier: GPL-2.0-only
/* Kernel thread helper functions.
 *   Copyright (C) 2004 IBM Corporation, Rusty Russell.
 *   Copyright (C) 2009 Red Hat, Inc.
 *
 * Creation is done via kthreadd, so that we get a clean environment
 * even if we're invoked from userspace (think modprobe, hotplug cpu,
 * etc.).
 */
#include <linux/kernel/mm.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/completion.h>
#include <linux/kernel/err.h>
#include <uapi/linux/unistd.h>
#include <linux/fs/file.h>
#include <linux/kernel/export.h>
#include <linux/kernel/ptrace.h>
#include <linux/kernel/uaccess.h>
#include <linux/kernel/numa.h>
#include <asm/signal.h>


static DEFINE_SPINLOCK(kthread_create_lock);
static LIST_HDR_S(kthread_create_list);
task_s *kthreadd_task;

typedef struct kthread_create_info {
	/* Information passed to kthread() from kthreadd. */
	char			*full_name;
	int				(*threadfn)(void *data);
	void			*data;
	int				node;

	/* Result passed back to kthread_create() from kthreadd. */
	task_s			*result;
	completion_s	*done;

	List_s			list;
} kthd_create_info_s;

typedef struct kthread {
	unsigned long	flags;
	unsigned int	cpu;
	int				result;
	int				(*threadfn)(void *);
	void			*data;
	// mm_segment_t oldfs;
	// completion_s	parked;
	completion_s	exited;
	/* To store the full name if task comm is truncated. */
	char			*full_name;
} kthread_s;

enum KTHREAD_BITS {
	KTHREAD_IS_PER_CPU = 0,
	KTHREAD_SHOULD_STOP,
	KTHREAD_SHOULD_PARK,
};

static inline kthread_s *to_kthread(task_s *k) {
	WARN_ON(!(k->flags & PF_KTHREAD));
	return k->worker_private;
}

/*
 * Variant of to_kthread() that doesn't assume @p is a kthread.
 *
 * Per construction; when:
 *
 *   (p->flags & PF_KTHREAD) && p->worker_private
 *
 * the task is both a kthread and struct kthread is persistent. However
 * PF_KTHREAD on it's own is not, kernel_thread() can exec() (See umh.c and
 * begin_new_exec()).
 */
static inline kthread_s *__to_kthread(task_s *p) {
	void *kthread = p->worker_private;
	if (kthread && !(p->flags & PF_KTHREAD))
		kthread = NULL;
	return kthread;
}

void get_kthread_comm(char *buf, size_t buf_size, task_s *tsk)
{
	kthread_s *kthread = to_kthread(tsk);

	if (!kthread || !kthread->full_name) {
		get_task_comm(buf, buf_size, tsk);
		return;
	}

	strscpy_pad(buf, kthread->full_name, buf_size);
}

bool set_kthread_struct(task_s *p)
{
	kthread_s *kthread;

	if (WARN_ON_ONCE(to_kthread(p)))
		return false;

	kthread = kzalloc(sizeof(*kthread), GFP_KERNEL);
	if (!kthread)
		return false;

	init_completion(&kthread->exited);
	// init_completion(&kthread->parked);
	p->vfork_done = &kthread->exited;

	p->worker_private = kthread;
	return true;
}

void free_kthread_struct(task_s *k)
{
	kthread_s *kthread;

	/*
	 * Can be NULL if kmalloc() in set_kthread_struct() failed.
	 */
	kthread = to_kthread(k);
	if (!kthread)
		return;

	k->worker_private = NULL;
	kfree(kthread->full_name);
	kfree(kthread);
}



/**
 * kthread_exit - Cause the current kthread return @result to kthread_stop().
 * @result: The integer value to return to kthread_stop().
 *
 * While kthread_exit can be called directly, it exists so that
 * functions which do some additional work in non-modular code such as
 * module_put_and_kthread_exit can be implemented.
 *
 * Does not return.
 */
void __noreturn kthread_exit(long result)
{
	kthread_s *kthread = to_kthread(current);
	kthread->result = result;
	do_exit(0);
}

/**
 * kthread_complete_and_exit - Exit the current kthread.
 * @comp: Completion to complete
 * @code: The integer value to return to kthread_stop().
 *
 * If present complete @comp and the reuturn code to kthread_stop().
 *
 * A kernel thread whose module may be removed after the completion of
 * @comp can use this function exit safely.
 *
 * Does not return.
 */
void __noreturn kthread_complete_and_exit(
		completion_s *comp, long code)
{
	if (comp)
		complete(comp);

	kthread_exit(code);
}
EXPORT_SYMBOL(kthread_complete_and_exit);

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

	self = to_kthread(current);

	/* If user was SIGKILLed, I release the structure. */
	done = xchg(&create->done, NULL);
	if (!done) {
		kfree(create->full_name);
		kfree(create);
		kthread_exit(-EINTR);
	}

	self->full_name = create->full_name;
	self->threadfn = threadfn;
	self->data = data;

	// /*
	//  * The new thread inherited kthreadd's priority and CPU mask. Reset
	//  * back to default in case they have been changed.
	//  */
	// sched_setscheduler_nocheck(current, SCHED_NORMAL, &param);
	// set_cpus_allowed_ptr(current, housekeeping_cpumask(HK_FLAG_KTHREAD));

	/* OK, tell user we're spawned, wait for stop or wakeup */
	__set_current_state(TASK_UNINTERRUPTIBLE);
	create->result = current;
	/*
	 * Thread is going to call schedule(), do not preempt it,
	 * or the creator may spend more time in wait_task_inactive().
	 */
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
	preempt_enable();

	ret = -EINTR;
	if (!test_bit(KTHREAD_SHOULD_STOP, &self->flags)) {
		// cgroup_kthread_ready();
		// __kthread_parkme(self);
		ret = threadfn(data);
	}
	kthread_exit(ret);
}

static void create_kthread(kthd_create_info_s *create)
{
	int pid;

	/* We want our own signal handler (we take no signals by default). */
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
		/* If user was SIGKILLed, I release the structure. */
		completion_s *done = xchg(&create->done, NULL);

		kfree(create->full_name);
		if (!done) {
			kfree(create);
			return;
		}
		create->result = ERR_PTR(pid);
		complete(done);
	}
}

static __printf(4, 0)
task_s *__kthread_create_on_node(int (*threadfn)(void *data),
		void *data, int node, const char namefmt[], va_list args)
{
	DECLARE_COMPLETION_ONSTACK(done);
	task_s *task;
	kthd_create_info_s *create =
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);

	if (!create)
		return ERR_PTR(-ENOMEM);
	create->threadfn	= threadfn;
	create->data		= data;
	create->node		= node;
	create->done		= &done;
	create->full_name	= kvasprintf(GFP_KERNEL, namefmt, args);
	if (!create->full_name) {
		task = ERR_PTR(-ENOMEM);
		goto free_create;
	}
	INIT_LIST_S(&create->list);

	spin_lock(&kthread_create_lock);
	list_header_enqueue(&kthread_create_list, &create->list);
	spin_unlock(&kthread_create_lock);

	wake_up_process(kthreadd_task);
	/*
	 * Wait for completion in killable state, for I might be chosen by
	 * the OOM killer while kthreadd is trying to allocate memory for
	 * new kernel thread.
	 */
	// if (unlikely(wait_for_completion_killable(&done))) {
	// 	/*
	// 	 * If I was killed by a fatal signal before kthreadd (or new
	// 	 * kernel thread) calls complete(), leave the cleanup of this
	// 	 * structure to that thread.
	// 	 */
	// 	if (xchg(&create->done, NULL))
	// 		return ERR_PTR(-EINTR);
		/*
		 * kthreadd (or new kernel thread) will call complete()
		 * shortly.
		 */
		wait_for_completion(&done);
	// }
	task = create->result;
free_create:
	kfree(create);
	return task;
}

/**
 * kthread_create_on_node - create a kthread.
 * @threadfn: the function to run until signal_pending(current).
 * @data: data ptr for @threadfn.
 * @node: task and thread structures for the thread are allocated on this node
 * @namefmt: printf-style name for the thread.
 *
 * Description: This helper function creates and names a kernel
 * thread.  The thread will be stopped: use wake_up_process() to start
 * it.  See also kthread_run().  The new thread has SCHED_NORMAL policy and
 * is affine to all CPUs.
 *
 * If thread is going to be bound on a particular cpu, give its node
 * in @node, to get NUMA affinity for kthread stack, or else give NUMA_NO_NODE.
 * When woken, the thread will run @threadfn() with @data as its
 * argument. @threadfn() can either return directly if it is a
 * standalone thread for which no one will call kthread_stop(), or
 * return when 'kthread_should_stop()' is true (which means
 * kthread_stop() has been called).  The return value should be zero
 * or a negative error number; it will be passed to kthread_stop().
 *
 * Returns a task_struct or ERR_PTR(-ENOMEM) or ERR_PTR(-EINTR).
 */
task_s *kthread_create_on_node(int (*threadfn)(void *data),
		void *data, int node, const char namefmt[], ...)
{
	task_s *task;
	va_list args;

	va_start(args, namefmt);
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
	va_end(args);

	return task;
}
EXPORT_SYMBOL(kthread_create_on_node);


int kthreadd(void *unused)
{
	task_s *tsk = current;

	/* Setup a clean context for our children to inherit. */
	// set_task_comm(tsk, "kthreadd");
	// ignore_signals(tsk);
	// set_cpus_allowed_ptr(tsk, housekeeping_cpumask(HK_FLAG_KTHREAD));
	// set_mems_allowed(node_states[N_MEMORY]);

	current->flags |= PF_NOFREEZE;
	// cgroup_init_kthreadd();

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (list_header_is_empty(&kthread_create_list))
			schedule();
		__set_current_state(TASK_RUNNING);

		spin_lock(&kthread_create_lock);
		while (!list_header_is_empty(&kthread_create_list)) {
			kthd_create_info_s *create;

			List_s *lp = list_header_dequeue(&kthread_create_list);
			create = container_of(lp, kthd_create_info_s, list);
			spin_unlock(&kthread_create_lock);

			create_kthread(create);

			spin_lock(&kthread_create_lock);
		}
		spin_unlock(&kthread_create_lock);
	}

	return 0;
}