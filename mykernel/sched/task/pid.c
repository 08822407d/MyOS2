// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic pidhash and scalable, time-bounded PID allocator
 *
 * (C) 2002-2003 Nadia Yvette Chambers, IBM
 * (C) 2004 Nadia Yvette Chambers, Oracle
 * (C) 2002-2004 Ingo Molnar, Red Hat
 *
 * pid-structures are backing objects for tasks sharing a given ID to chain
 * against. There is very little to them aside from hashing them and
 * parking tasks using given ID's on a list.
 *
 * The hash is always changed with the tasklist_lock write-acquired,
 * and the hash is only accessed with the tasklist_lock at least
 * read-acquired, so there's no additional SMP locking needed here.
 *
 * We have a list of bitmap pages, which bitmaps represent the PID space.
 * Allocating and freeing PIDs is completely lockless. The worst-case
 * allocation scenario when all but one out of 1 million PIDs possible are
 * allocated already: the scanning of 32 list entries and at most PAGE_SIZE
 * bytes. The typical fastpath is a single successful setbit. Freeing is O(1).
 *
 * Pid namespaces:
 *    (C) 2007 Pavel Emelyanov <xemul@openvz.org>, OpenVZ, SWsoft Inc.
 *    (C) 2007 Sukadev Bhattiprolu <sukadev@us.ibm.com>, IBM
 *     Many thanks to Oleg Nesterov for comments and help
 *
 */
#define PID_DEFINATION
#include "pid.h"


#include <linux/kernel/mm_api.h>
#include <linux/kernel/export.h>
#include <linux/init/init.h>
#include <linux/kernel/syscalls.h>
#include <linux/sched/signal.h>


#include <generated/conf.h>



pid_s init_struct_pid = {
	.count		= ATOMIC_INIT(1),
	.tasks		= {
		// LIST_HEADER_INIT(init_struct_pid.tasks[0]),
		// LIST_HEADER_INIT(init_struct_pid.tasks[1]),
		// LIST_HEADER_INIT(init_struct_pid.tasks[2]),
		{ .first = NULL },
		{ .first = NULL },
		{ .first = NULL },
	},
	.level		= 0,
	.numbers	= {
		{
			.nr		= 0,
			.ns		= &init_pid_ns,
		},
	}
};


int pid_max			= PID_MAX_DEFAULT;

int pid_max_min		= RESERVED_PIDS + 1;
int pid_max_max		= PID_MAX_LIMIT;
/*
 * Pseudo filesystems start inode numbering after one. We use Reserved
 * PIDs as a natural offset.
 */
static u64 pidfs_ino	= RESERVED_PIDS;

/*
 * PID-map pages start out as NULL, they get allocated upon
 * first use and are never deallocated. This way a low pid_max
 * value does not cause lots of bitmaps to be allocated, but
 * the scheme scales to up to 4 million PIDs, runtime.
 */
pid_ns_s init_pid_ns = {
	// .ns.count		= REFCOUNT_INIT(2),
	// .idr			= IDR_INIT(init_pid_ns.idr),
	.pid_allocated	= PIDNS_ADDING,
	.level			= 0,
	.child_reaper	= &init_task,
	// .user_ns		= &init_user_ns,
	// .ns.inum		= PROC_PID_INIT_INO,
// #ifdef CONFIG_PID_NS
// 	.ns.ops			= &pidns_operations,
// #endif
// #if defined(CONFIG_SYSCTL) && defined(CONFIG_MEMFD_CREATE)
// 	.memfd_noexec_scope		= MEMFD_NOEXEC_SCOPE_EXEC,
// #endif
};
EXPORT_SYMBOL_GPL(init_pid_ns);

/*
 * Note: disable interrupts while the pidmap_lock is held as an
 * interrupt might come in and do read_lock(&tasklist_lock).
 *
 * If we don't disable interrupts there is a nasty deadlock between
 * detach_pid()->free_pid() and another cpu that does
 * spin_lock(&pidmap_lock) followed by an interrupt routine that does
 * read_lock(&tasklist_lock);
 *
 * After we clean up the tasklist_lock and know there are no
 * irq handlers that take it we can leave the interrupts enabled.
 * For now it is easier to be safe than to prove it can't happen.
 */

static  __cacheline_aligned_in_smp DEFINE_SPINLOCK(pidmap_lock);


HLIST_HEAD(pid_list_hdr);


void put_pid(pid_s *pid)
{
	// struct pid_namespace *ns;

	if (!pid)
		return;

	// ns = pid->numbers[pid->level].ns;
	if (atomic_dec_and_test(&pid->count)) {
		// kmem_cache_free(ns->pid_cachep, pid);
		kfree(pid);
		// put_pid_ns(ns);
	}
}
EXPORT_SYMBOL_GPL(put_pid);

void free_pid(pid_s *pid)
{
	// /* We can be called with write_lock_irq(&tasklist_lock) held */
	// int i;
	// unsigned long flags;

	// spin_lock_irqsave(&pidmap_lock, flags);
	// for (i = 0; i <= pid->level; i++) {
	// 	struct upid *upid = pid->numbers + i;
	// 	struct pid_namespace *ns = upid->ns;
	// 	switch (--ns->pid_allocated) {
	// 	case 2:
	// 	case 1:
	// 		/* When all that is left in the pid namespace
	// 		 * is the reaper wake up the reaper.  The reaper
	// 		 * may be sleeping in zap_pid_ns_processes().
	// 		 */
	// 		wake_up_process(ns->child_reaper);
	// 		break;
	// 	case PIDNS_ADDING:
	// 		/* Handle a fork failure of the first process */
	// 		WARN_ON(ns->child_reaper);
	// 		ns->pid_allocated = 0;
	// 		break;
	// 	}

	// 	idr_remove(&ns->idr, upid->nr);
	// }
	// spin_unlock_irqrestore(&pidmap_lock, flags);

	// call_rcu(&pid->rcu, delayed_put_pid);
	// static void delayed_put_pid(struct rcu_head *rhp)
	// {
	// 	struct pid *pid = container_of(rhp, struct pid, rcu);
		put_pid(pid);
	// }
}

pid_s *alloc_pid(pid_ns_s *ns, pid_t *set_tid, size_t set_tid_size)
{
	pid_s *pid;
	enum pid_type type;
	int i, nr;
	pid_ns_s *tmp;
	upid_s *upid;
	int retval = -ENOMEM;

	/*
	 * set_tid_size contains the size of the set_tid array. Starting at
	 * the most nested currently active PID namespace it tells alloc_pid()
	 * which PID to set for a process in that most nested PID namespace
	 * up to set_tid_size PID namespaces. It does not have to set the PID
	 * for a process in all nested PID namespaces but set_tid_size must
	 * never be greater than the current ns->level + 1.
	 */
	if (set_tid_size > ns->level + 1)
		return ERR_PTR(-EINVAL);

	pid = kmem_cache_alloc(ns->pid_cachep, GFP_KERNEL);
	if (!pid)
		return ERR_PTR(retval);

	tmp = ns;
	pid->level = ns->level;

	for (i = ns->level; i >= 0; i--) {
		int tid = 0;

		if (set_tid_size) {
			tid = set_tid[ns->level - i];

			retval = -EINVAL;
			if (tid < 1 || tid >= pid_max)
				goto out_free;
			/*
			 * Also fail if a PID != 1 is requested and
			 * no PID 1 exists.
			 */
			if (tid != 1 && !tmp->child_reaper)
				goto out_free;
			retval = -EPERM;
			// if (!checkpoint_restore_ns_capable(tmp->user_ns))
			// 	goto out_free;
			set_tid_size--;
		}

		// idr_preload(GFP_KERNEL);
		spin_lock_irq(&pidmap_lock);

		// if (tid) {
		// 	nr = idr_alloc(&tmp->idr, NULL, tid,
		// 			   tid + 1, GFP_ATOMIC);
			nr = myos_idr_alloc();
		// 	/*
		// 	 * If ENOSPC is returned it means that the PID is
		// 	 * alreay in use. Return EEXIST in that case.
		// 	 */
		// 	if (nr == -ENOSPC)
		// 		nr = -EEXIST;
		// } else {
		// 	int pid_min = 1;
		// 	/*
		// 	 * init really needs pid 1, but after reaching the
		// 	 * maximum wrap back to RESERVED_PIDS
		// 	 */
		// 	if (idr_get_cursor(&tmp->idr) > RESERVED_PIDS)
		// 		pid_min = RESERVED_PIDS;

		// 	/*
		// 	 * Store a null pointer so find_pid_ns does not find
		// 	 * a partially initialized PID (see below).
		// 	 */
		// 	nr = idr_alloc_cyclic(&tmp->idr, NULL, pid_min,
		// 				  pid_max, GFP_ATOMIC);
		// }
		spin_unlock_irq(&pidmap_lock);
		// idr_preload_end();

		if (nr < 0) {
			retval = (nr == -ENOSPC) ? -EAGAIN : nr;
			goto out_free;
		}

		pid->numbers[i].nr = nr;
		pid->numbers[i].ns = tmp;
		tmp = tmp->parent;
	}

	/*
	 * ENOMEM is not the most obvious choice especially for the case
	 * where the child subreaper has already exited and the pid
	 * namespace denies the creation of any new processes. But ENOMEM
	 * is what we have exposed to userspace for a long time and it is
	 * documented behavior for pid namespaces. So we can't easily
	 * change it even if there were an error code better suited.
	 */
	retval = -ENOMEM;

	// get_pid_ns(ns);
	atomic_set(&pid->count, 1);
	spin_lock_init(&pid->lock);
	for (type = 0; type < PIDTYPE_MAX; ++type)
		INIT_HLIST_HEAD(&pid->tasks[type]);

	// init_waitqueue_head(&pid->wait_pidfd);
	INIT_HLIST_HEAD(&pid->inodes);

	upid = pid->numbers + ns->level;
	// spin_lock_irq(&pidmap_lock);
	// if (!(ns->pid_allocated & PIDNS_ADDING))
	// 	goto out_unlock;
	// for ( ; upid >= pid->numbers; --upid) {
	// 	/* Make the PID visible to find_pid_ns. */
	// 	idr_replace(&upid->ns->idr, pid, upid->nr);
	// 	upid->ns->pid_allocated++;
	// }
	// spin_unlock_irq(&pidmap_lock);

	return pid;

out_unlock:
	// spin_unlock_irq(&pidmap_lock);
	// put_pid_ns(ns);

out_free:
	// spin_lock_irq(&pidmap_lock);
	// while (++i <= ns->level) {
	// 	upid = pid->numbers + i;
	// 	idr_remove(&upid->ns->idr, upid->nr);
	// }

	// /* On failure to allocate the first pid, reset the state */
	// if (ns->pid_allocated == PIDNS_ADDING)
	// 	idr_set_cursor(&ns->idr, 0);

	// spin_unlock_irq(&pidmap_lock);

	kmem_cache_free(ns->pid_cachep, pid);
	return ERR_PTR(retval);
}

pid_s *find_pid_ns(int nr, pid_ns_s *ns)
{
	// return idr_find(&ns->idr, nr);
	return NULL;
}
EXPORT_SYMBOL_GPL(find_pid_ns);

pid_s *find_vpid(int nr)
{
	return find_pid_ns(nr, task_active_pid_ns(current));
}
EXPORT_SYMBOL_GPL(find_vpid);

static pid_s **task_pid_ptr(task_s *task, enum pid_type type) {
	if (type == PIDTYPE_PID)
		return &task->thread_pid;
	else
		return &task->signal->pids[type];
}

/*
 * attach_pid() must be called with the tasklist_lock write-held.
 */
void attach_pid(task_s *task, enum pid_type type) {
	pid_s *pid = *task_pid_ptr(task, type);
	// hlist_add_head_rcu(&task->pid_links[type], &pid->tasks[type]);

	hlist_add_head(&task->pid_links[type], &pid_list_hdr);
}


void detach_pid(task_s *task, enum pid_type type) {
	// __change_pid(task, type, NULL);

	hlist_del_init(&task->pid_links[type]);
}


task_s *pid_task(pid_s *pid, enum pid_type type)
{
	task_s *result = NULL;
	if (pid) {
		HList_s *first;
		// first = rcu_dereference_check(hlist_first_rcu(&pid->tasks[type]),
		// 			      lockdep_tasklist_lock_is_held());
		first = &pid->tasks[type].first;
		if (first)
			result = hlist_entry(first, task_s, pid_links[(type)]);
	}
	return result;
}
EXPORT_SYMBOL(pid_task);

/*
 * Must be called under rcu_read_lock().
 */
task_s *find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)
{
	// RCU_LOCKDEP_WARN(!rcu_read_lock_held(),
	// 		 "find_task_by_pid_ns() needs rcu_read_lock() protection");
	return pid_task(find_pid_ns(nr, ns), PIDTYPE_PID);
}



pid_ns_s *task_active_pid_ns(task_s *tsk)
{
	return ns_of_pid(task_pid(tsk));
}
EXPORT_SYMBOL_GPL(task_active_pid_ns);

pid_t pid_nr_ns(pid_s *pid, pid_ns_s *ns)
{
	upid_s *upid;
	pid_t nr = 0;

	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
	}
	return nr;
}
EXPORT_SYMBOL_GPL(pid_nr_ns);

pid_t pid_vnr(pid_s *pid)
{
	return pid_nr_ns(pid, task_active_pid_ns(current));
}
EXPORT_SYMBOL_GPL(pid_vnr);

pid_t __task_pid_nr_ns(task_s *task,
		enum pid_type type, pid_ns_s *ns)
{
	pid_t nr = 0;

	// rcu_read_lock();
	if (!ns)
		ns = task_active_pid_ns(current);
	// nr = pid_nr_ns(rcu_dereference(*task_pid_ptr(task, type)), ns);
	nr = pid_nr_ns(*task_pid_ptr(task, type), ns);
	// rcu_read_unlock();

	return nr;
}
EXPORT_SYMBOL(__task_pid_nr_ns);


pid_s *get_task_pid(task_s *task, enum pid_type type)
{
	pid_s *pid;
	// rcu_read_lock();
	// pid = get_pid(rcu_dereference(*task_pid_ptr(task, type)));
	pid = get_pid(*task_pid_ptr(task, type));
	// rcu_read_unlock();
	return pid;
}
EXPORT_SYMBOL_GPL(get_task_pid);



task_s *myos_find_task_by_pid(pid_t nr) {
	task_s *tsk;

	hlist_for_each_entry(tsk,
			&pid_list_hdr, pid_links[PIDTYPE_PID]) {

		if (tsk->pid == nr) return tsk;
	}
	return NULL;
}




void __init pid_idr_init(void)
{
	void myos_init_pid_allocator(void);
	myos_init_pid_allocator();


	/* Verify no one has done anything silly: */
	BUILD_BUG_ON(PID_MAX_LIMIT >= PIDNS_ADDING);

	/* bump default and minimum pid_max based on number of cpus */
	pid_max = min(pid_max_max, max_t(int, pid_max,
				PIDS_PER_CPU_DEFAULT * num_possible_cpus()));
	pid_max_min = max_t(int, pid_max_min,
				PIDS_PER_CPU_MIN * num_possible_cpus());
	pr_info("pid_max: default: %u minimum: %u\n", pid_max, pid_max_min);

	// idr_init(&init_pid_ns.idr);

	init_pid_ns.pid_cachep = kmem_cache_create("pid",
			struct_size_t(pid_s, numbers, 1), __alignof__(pid_s),
			SLAB_HWCACHE_ALIGN | SLAB_PANIC | SLAB_ACCOUNT);
}


/*==============================================================================================*
 *																								*
 *==============================================================================================*/
#include <klib/utils.h>

bitmap_t	pid_bm[PID_MAX_DEFAULT / sizeof(bitmap_t)];
ulong		curr_pid;

void myos_init_pid_allocator()
{
	// init pid bitmap
	memset(&pid_bm, 0, sizeof(pid_bm));
	bm_set_bit(pid_bm, 0);
	curr_pid = 1;
}

ulong myos_idr_alloc()
{
	ulong newpid = bm_get_freebit_idx(
			pid_bm, curr_pid, PID_MAX_DEFAULT);
	if (newpid >= PID_MAX_DEFAULT || newpid < curr_pid)
	{
		newpid = bm_get_freebit_idx(pid_bm, 0, curr_pid);
		if (newpid >= curr_pid || newpid == 0)
		{
			// pid bitmap used out
			while (1);
		}
	}
	curr_pid = newpid;
	bm_set_bit(pid_bm, newpid);

	return curr_pid;
}