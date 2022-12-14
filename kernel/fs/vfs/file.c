// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/file.c
 *
 *  Copyright (C) 1998-1999, Stephen Tweedie and Bill Hawes
 *
 *  Manage the dynamic fd arrays in the process files_struct.
 */

// #include <linux/syscalls.h>
// #include <linux/export.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
// #include <linux/sched/signal.h>
#include <linux/kernel/slab.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/kernel/bitops.h>
// #include <linux/spinlock.h>
// #include <linux/rcupdate.h>
// #include <linux/close_range.h>
// #include <net/sock.h>
#include "internal.h"


static fdtable_s *close_files(files_struct_s * files)
{
	/*
	 * It is safe to dereference the fd table without RCU or
	 * ->file_lock because this is the last reference to the
	 * files structure.
	 */
	// fdtable_s *fdt = rcu_dereference_raw(files->fdt);
	fdtable_s *fdt = files->fdt;
	unsigned int i, j = 0;

	// for (;;) {
	// 	unsigned long set;
	// 	i = j * BITS_PER_LONG;
	// 	if (i >= fdt->max_fds)
	// 		break;
	// 	set = fdt->open_fds[j++];
	// 	while (set) {
	// 		if (set & 1) {
	// 			file_s * file = &fdt->fd[i];
	// 			if (file) {
	// 				filp_close(file);
	// 				cond_resched();
	// 			}
	// 		}
	// 		i++;
	// 		set >>= 1;
	// 	}
	// }

	for(i = 0; i < MAX_FILE_NR; i++)
	{
		file_s *file = files->fd_array[i];
		if(file != NULL)
			filp_close(file);
	}

	return fdt;
}

void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->count)) {
		fdtable_s *fdt = close_files(files);

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
	return alloc_fd(0, MAX_FILE_NR, flags);
}

/*
 * Install a file pointer in the fd array.
 *
 * The VFS is full of places where we drop the files lock between
 * setting the open_fds bitmap and installing the file in the file
 * array.  At any such point, we are vulnerable to a dup2() race
 * installing a file in the array before us.  We need to detect this and
 * fput() the struct file we are about to overwrite in this case.
 *
 * It should never happen - if we allow dup2() do it, _really_ bad things
 * will follow.
 *
 * This consumes the "file" refcount, so callers should treat it
 * as if they had called fput(file).
 */

void fd_install(unsigned int fd, file_s *file)
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

fd_s fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
}