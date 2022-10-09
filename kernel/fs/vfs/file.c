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
// #include <linux/slab.h>
#include <linux/fs/file.h>
// #include <linux/fdtable.h>
#include <linux/kernel/bitops.h>
// #include <linux/spinlock.h>
// #include <linux/rcupdate.h>
// #include <linux/close_range.h>
// #include <net/sock.h>
#include <linux/fs/internal.h>


#include <linux/kernel/sched.h>

/*
 * allocate a file descriptor, mark it busy.
 */
static int alloc_fd(unsigned start, unsigned end, unsigned flags)
{
	file_s ** fps = curr_tsk->fps;
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
	file_s ** fps = curr_tsk->fps;
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
	task_s *curr = curr_tsk;
	file_s *fp = curr->fps[fd];
	
	return (fd_s){.file = fp, .flags = 0};
}