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

#include <linux/mm/mm.h>
// #include <linux/export.h>
#include <linux/kernel/slab.h>
#include <linux/init/init.h>
// #include <linux/rculist.h>
#include <linux/mm/memblock.h>
// #include <linux/pid_namespace.h>
// #include <linux/init_task.h>
#include <linux/kernel/syscalls.h>
// #include <linux/proc_ns.h>
// #include <linux/refcount.h>
// #include <linux/anon_inodes.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
// #include <linux/idr.h>
// #include <net/sock.h>
// #include <uapi/linux/pidfd.h>


LIST_HDR_S(pid_list_hdr);

/*
 * attach_pid() must be called with the tasklist_lock write-held.
 */
void attach_pid(task_s *task, enum pid_type type) {
	// struct pid *pid = *task_pid_ptr(task, type);
	// hlist_add_head_rcu(&task->pid_links[type], &pid->tasks[type]);
	list_hdr_append(&pid_list_hdr, &task->pid_links);
}


void detach_pid(task_s *task, enum pid_type type) {
	// __change_pid(task, type, NULL);
	list_hdr_delete(&pid_list_hdr, &task->pid_links);
}


task_s *myos_find_task_by_pid(pid_t nr) {
	task_s *tsk;

	list_for_each_entry(tsk, &pid_list_hdr, pid_links) {
		if (tsk->pid == nr)
			return tsk;
	}
	return NULL;
}