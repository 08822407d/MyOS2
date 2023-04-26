// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/file.c
 *
 *  Copyright (C) 1998-1999, Stephen Tweedie and Bill Hawes
 *
 *  Manage the dynamic fd arrays in the process files_struct.
 */

#include <linux/kernel/syscalls.h>
// #include <linux/export.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
#include <linux/sched/signal.h>
#include <linux/kernel/slab.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/spinlock.h>
// #include <linux/rcupdate.h>
// #include <linux/close_range.h>
// #include <net/sock.h>
#include "internal.h"


/*
 * Allocate a new files structure and copy contents from the
 * passed in files structure.
 * errorp will be valid only when the returned files_struct is NULL.
 */
files_struct_s *dup_fd(files_struct_s *oldf, unsigned int max_fds, int *errorp)
{
	files_struct_s *newf;
	file_s **old_fds, **new_fds;
	unsigned int open_files, i;
	// fdtable_s *old_fdt, *new_fdt;

	*errorp = -ENOMEM;
	// newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	newf = kzalloc(sizeof(files_struct_s), GFP_KERNEL);
	if (!newf)
		goto out;

	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;

	// spin_lock_init(&newf->file_lock);
	// newf->resize_in_progress = false;
	// init_waitqueue_head(&newf->resize_wait);
	// newf->next_fd = 0;
	// new_fdt = &newf->fdtab;
	// new_fdt->max_fds = NR_OPEN_DEFAULT;
	// new_fdt->close_on_exec = newf->close_on_exec_init;
	// new_fdt->open_fds = newf->open_fds_init;
	// new_fdt->full_fds_bits = newf->full_fds_bits_init;
	// new_fdt->fd = &newf->fd_array[0];

	// spin_lock(&oldf->file_lock);
	// old_fdt = files_fdtable(oldf);
	// open_files = sane_fdtable_size(old_fdt, max_fds);

	/*
	 * Check whether we need to allocate a larger fd array and fd set.
	 */
	// while (unlikely(open_files > new_fdt->max_fds)) {
	// 	spin_unlock(&oldf->file_lock);

	// 	if (new_fdt != &newf->fdtab)
	// 		__free_fdtable(new_fdt);

	// 	new_fdt = alloc_fdtable(open_files - 1);
	// 	if (!new_fdt) {
	// 		*errorp = -ENOMEM;
	// 		goto out_release;
	// 	}

	// 	/* beyond sysctl_nr_open; nothing to do */
	// 	if (unlikely(new_fdt->max_fds < open_files)) {
	// 		__free_fdtable(new_fdt);
	// 		*errorp = -EMFILE;
	// 		goto out_release;
	// 	}

	// 	/*
	// 	 * Reacquire the oldf lock and a pointer to its fd table
	// 	 * who knows it may have a new bigger fd table. We need
	// 	 * the latest pointer.
	// 	 */
	// 	spin_lock(&oldf->file_lock);
	// 	old_fdt = files_fdtable(oldf);
	// 	open_files = sane_fdtable_size(old_fdt, max_fds);
	// }

	// copy_fd_bitmaps(new_fdt, old_fdt, open_files);

	// old_fds = old_fdt->fd;
	// new_fds = new_fdt->fd;
	old_fds = oldf->fd_array;
	new_fds = newf->fd_array;

	// for (i = open_files; i != 0; i--) {
	// 	file_s *f = *old_fds++;
	// 	if (f) {
	// 		get_file(f);
	// 	} else {
	// 		/*
	// 		 * The fd may be claimed in the fd bitmap but not yet
	// 		 * instantiated in the files array if a sibling thread
	// 		 * is partway through open().  So make sure that this
	// 		 * fd is available to the new process.
	// 		 */
	// 		__clear_open_fd(open_files - i, new_fdt);
	// 	}
	// 	rcu_assign_pointer(*new_fds++, f);
	// }
	// spin_unlock(&oldf->file_lock);

	for (i = 0; i < max_fds; i++)
	{
		file_s *old_fp = old_fds[i];
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
		}	
	}

	// /* clear the remainder */
	// memset(new_fds, 0, (new_fdt->max_fds - open_files) * sizeof(file_s *));

	// rcu_assign_pointer(newf->fdt, new_fdt);

	return newf;

out_release:
	// kmem_cache_free(files_cachep, newf);
	kfree(newf);
out:
	return NULL;
}

void myos_close_files(files_struct_s * files)
{
	/*
	 * It is safe to dereference the fd table without RCU or
	 * ->file_lock because this is the last reference to the
	 * files structure.
	 */
	// fdtable_s *fdt = rcu_dereference_raw(files->fdt);
	unsigned int i = 0;

	for(i = 0; i < files->fd_count; i++)
	{
		file_s *file = files->fd_array[i];
		if(file != NULL)
			filp_close(file);
	}
}

void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
		myos_close_files(files);

		/* free the arrays if they are not embedded */
		// if (fdt != &files->fdtab)
		// 	__free_fdtable(fdt);
		// kmem_cache_free(files_cachep, files);
		kfree(files);
	}
}

void exit_files(task_s *tsk)
{
	files_struct_s * files = tsk->files;

	if (files) {
		// task_lock(tsk);
		tsk->files = NULL;
		// task_unlock(tsk);
		put_files_struct(files);
	}
}


/*
 * allocate a file descriptor, mark it busy.
 */
static int alloc_fd(unsigned start, unsigned end, unsigned flags)
{
	task_s *curr = current;
	file_s ** fps = current->files->fd_array;
	int fd = -1;

	for(int i = start; i < end; i++)
		if(fps[i] == NULL)
		{
			fd = i;
			break;
		}

	return fd;
}

int get_unused_fd_flags(unsigned flags)
{
	return alloc_fd(0, current->files->fd_count, flags);
}

/*
 * Install a file pointer in the fd array.
 *
 * The VFS is full of places where we drop the files lock between
 * setting the open_fds bitmap and installing the file in the file
 * array.  At any such point, we are vulnerable to a dup2() race
 * installing a file in the array before us.  We need to detect this and
 * fput() the file_s we are about to overwrite in this case.
 *
 * It should never happen - if we allow dup2() do it, _really_ bad things
 * will follow.
 *
 * This consumes the "file" refcount, so callers should treat it
 * as if they had called fput(file).
 */

void myos_fd_install(unsigned int fd, file_s *file)
{
	file_s ** fps = current->files->fd_array;
	if (fps[fd] == NULL)
	{
		fps[fd] = file;
	}
	else
	{
		while (1);
	}
}

fd_s myos_fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
}