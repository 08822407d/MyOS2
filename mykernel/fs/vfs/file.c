// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/file.c
 *
 *  Copyright (C) 1998-1999, Stephen Tweedie and Bill Hawes
 *
 *  Manage the dynamic fd arrays in the process files_struct.
 */

#include <linux/kernel/syscalls.h>
#include <linux/kernel/export.h>
#include <linux/fs/fs.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/mm.h>
#include <linux/sched/signal.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/lock_ipc.h>
#include <linux/fs/internal.h>
#include <linux/kernel/sched_api.h>


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

	*errorp = -ENOMEM;
	newf = kmem_cache_alloc(files_cachep, GFP_KERNEL);
	if (!newf)
		goto out;

	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;

	spin_lock_init(&newf->file_lock);
	// newf->resize_in_progress = false;
	// init_waitqueue_head(&newf->resize_wait);
	// newf->next_fd = 0;

	old_fds = oldf->fd_array;
	new_fds = newf->fd_array;

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

	// rcu_assign_pointer(newf->fdt, new_fdt);

	return newf;

out_release:
	kmem_cache_free(files_cachep, newf);
out:
	return NULL;
}

void put_files_struct(files_struct_s *files)
{
	if (atomic_dec_and_test(&files->refcount)) {
	// static fdtable_s *close_files(files_struct_s * files)
	// {
		/*
		 * It is safe to dereference the fd table without RCU or
		 * ->file_lock because this is the last reference to the
		 * files structure.
		 */
		// fdtable_s *fdt = rcu_dereference_raw(files->fdt);
		unsigned int i = 0;
		for(i = 0; i < files->fd_count; i++) {
			file_s *file = files->fd_array[i];
			if(file != NULL)
				filp_close(file);
		}
	// }

		/* free the arrays if they are not embedded */
		// if (fdt != &files->fdtab)
		// 	__free_fdtable(fdt);
		kmem_cache_free(files_cachep, files);
	}
}

void exit_files(task_s *tsk)
{
	files_struct_s * files = tsk->files;

	if (files) {
		task_lock(tsk);
		tsk->files = NULL;
		task_unlock(tsk);
		put_files_struct(files);
	}
}

files_struct_s init_files = {
	.refcount		= ATOMIC_INIT(1),
	// .resize_wait	= __WAIT_QUEUE_HEAD_INITIALIZER(init_files.resize_wait),
	// .fdt			= &init_files.fdtab,
	// .fdtab			= {
	// 	.max_fds		= NR_OPEN_DEFAULT,
	// 	.fd				= &init_files.fd_array[0],
	// 	.close_on_exec	= init_files.close_on_exec_init,
	// 	.open_fds		= init_files.open_fds_init,
	// 	.full_fds_bits	= init_files.full_fds_bits_init,
	// },
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
};


/*
 * allocate a file descriptor, mark it busy.
 */
static int alloc_fd(unsigned start, unsigned end, unsigned flags)
{
	files_struct_s *files = current->files;
	unsigned int fd;
	int error;

	spin_lock(&files->file_lock);
	for(int i = start; i < end; i++)
		if(files->fd_array[i] == NULL)
		{
			fd = i;
			break;
		}

	spin_unlock(&files->file_lock);
	return fd;
}

int get_unused_fd_flags(unsigned flags)
{
	return alloc_fd(0, current->files->fd_count, flags);
}
EXPORT_SYMBOL(get_unused_fd_flags);

void put_unused_fd(unsigned int fd)
{
	files_struct_s *files = current->files;
	spin_lock(&files->file_lock);
// static void __put_unused_fd(struct files_struct *files, unsigned int fd)
// {
	// struct fdtable *fdt = files_fdtable(files);
	// __clear_open_fd(fd, fdt);
	// if (fd < files->next_fd)
	// 	files->next_fd = fd;
// }
	spin_unlock(&files->file_lock);
}
EXPORT_SYMBOL(put_unused_fd);

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

void fd_install(unsigned int fd, file_s *file)
{
	files_struct_s *files = current->files;
	file_s ** fps = files->fd_array;

	/* coupled with smp_wmb() in expand_fdtable() */
	smp_rmb();
	BUG_ON(fps[fd] != NULL);
	fps[fd] = file;
}
EXPORT_SYMBOL(fd_install);


fd_s myos_fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
}



static inline file_s
*__simple_fget(uint fd, fmode_t mask) {
	// return __fget_files(current->files, fd, mask);
	file_s *retval = NULL;
	files_struct_s *files = current->files;
	if (fd < files->fd_count) {
		retval = files->fd_array[fd];
		atomic_long_inc(&retval->f_count);
	}
	return retval;
}

file_s *fget(uint fd) {
	return __simple_fget(fd, FMODE_PATH);
}
EXPORT_SYMBOL(fget);
