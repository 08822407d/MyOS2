// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/file_table.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/lib/string.h>
#include <linux/kernel/slab.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/init/init.h>
// #include <linux/module.h>
#include <linux/fs/fs.h>
// #include <linux/security.h>
// #include <linux/cred.h>
// #include <linux/eventpoll.h>
// #include <linux/rcupdate.h>
#include <linux/kernel/mount.h>
// #include <linux/capability.h>
#include <linux/device/cdev.h>
// #include <linux/fsnotify.h>
// #include <linux/sysctl.h>
// #include <linux/percpu_counter.h>
// #include <linux/percpu.h>
// #include <linux/task_work.h>
// #include <linux/ima.h>
// #include <linux/swap.h>
// #include <linux/kmemleak.h>
#include "internal.h"
#include <linux/kernel/atomic.h>


// static struct file *__alloc_file(int flags, const struct cred *cred)
static file_s *__alloc_file(int flags)
{
	struct file *f;
	int error;

	// f = kmem_cache_zalloc(filp_cachep, GFP_KERNEL);
	f = kzalloc(sizeof(file_s), GFP_KERNEL);
	if (unlikely(!f))
		return ERR_PTR(-ENOMEM);

	// f->f_cred = get_cred(cred);
	// error = security_file_alloc(f);
	// if (unlikely(error)) {
	// 	file_free_rcu(&f->f_u.fu_rcuhead);
	// 	return ERR_PTR(error);
	// }

	atomic_long_set(&f->f_count, 1);
	// rwlock_init(&f->f_owner.lock);
	spin_lock_init(&f->f_lock);
	// mutex_init(&f->f_pos_lock);
	f->f_flags = flags;
	f->f_mode = OPEN_FMODE(flags);
	/* f->f_version: 0 */

	return f;
}

/* Find an unused file structure and return a pointer to it.
 * Returns an error pointer if some error happend e.g. we over file
 * structures limit, run out of memory or operation is not permitted.
 *
 * Be very careful using this.  You are responsible for
 * getting write access to any mount that you might assign
 * to this filp, if it is opened for write.  If this is not
 * done, you will imbalance int the mount's writer count
 * and a warning at __fput() time.
 */
file_s *alloc_empty_file(int flags)
{
	static long old_max;
	file_s *f;

	// /*
	//  * Privileged users can go above max_files
	//  */
	// if (get_nr_files() >= files_stat.max_files && !capable(CAP_SYS_ADMIN)) {
	// 	/*
	// 	 * percpu_counters are inaccurate.  Do an expensive check before
	// 	 * we go and fail.
	// 	 */
	// 	if (percpu_counter_sum_positive(&nr_files) >= files_stat.max_files)
	// 		goto over;
	// }

	f = __alloc_file(flags);
	// if (!IS_ERR(f))
	// 	percpu_counter_inc(&nr_files);

	return f;

over:
	// /* Ran out of filps - report that */
	// if (get_nr_files() > old_max) {
	// 	pr_info("VFS: file-max limit %lu reached\n", get_max_files());
	// 	old_max = get_nr_files();
	// }
	return ERR_PTR(-ENFILE);
}

void fput_many(file_s *file, unsigned int refs)
{
	if (atomic_long_sub_and_test(refs, &file->f_count)) {
		kfree (file);

		task_s *task = current;

		// if (likely(!in_interrupt() && !(task->flags & PF_KTHREAD))) {
		// 	init_task_work(&file->f_u.fu_rcuhead, ____fput);
		// 	if (!task_work_add(task, &file->f_u.fu_rcuhead, TWA_RESUME))
		// 		return;
		// 	/*
		// 	 * After this task has run exit_task_work(),
		// 	 * task_work_add() will fail.  Fall through to delayed
		// 	 * fput to avoid leaking *file.
		// 	 */
		// }

		// if (llist_add(&file->f_u.fu_llist, &delayed_fput_list))
		// 	schedule_delayed_work(&delayed_fput_work, 1);
	}
}

void fput(file_s *file)
{
	fput_many(file, 1);
}