// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/file_table.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/lib/string.h>
// #include <linux/slab.h>
#include <linux/fs/file.h>
// #include <linux/fdtable.h>
#include <linux/init/init.h>
// #include <linux/module.h>
#include <linux/fs/fs.h>
// #include <linux/security.h>
// #include <linux/cred.h>
// #include <linux/eventpoll.h>
// #include <linux/rcupdate.h>
#include <linux/kernel/mount.h>
// #include <linux/capability.h>
// #include <linux/cdev.h>
// #include <linux/fsnotify.h>
// #include <linux/sysctl.h>
// #include <linux/percpu_counter.h>
// #include <linux/percpu.h>
// #include <linux/task_work.h>
// #include <linux/ima.h>
// #include <linux/swap.h>
// #include <linux/kmemleak.h>
#include <linux/fs/internal.h>
// #include <linux/atomic.h>


#include <linux/kernel/err.h>
#include <include/obsolete/proto.h>

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

	// static struct file *__alloc_file(int flags, const struct cred *cred)
	// {
		f = kmalloc(sizeof(file_s));
		if (f == NULL)
			return ERR_PTR(-ENOMEM);

		memset(f, 0, sizeof(file_s));
		f->f_flags = flags;
		f->f_mode = OPEN_FMODE(flags);
		/* f->f_version: 0 */
	// }
	return f;
}

void fput(file_s *file)
{
	kfree(file);
}