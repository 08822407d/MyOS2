// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/read_write.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/kernel/slab.h>
#include <linux/kernel/stat.h>
// #include <linux/sched/xacct.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>
// #include <linux/uio.h>
// #include <linux/fsnotify.h>
// #include <linux/security.h>
// #include <linux/export.h>
// #include <linux/syscalls.h>
// #include <linux/pagemap.h>
// #include <linux/splice.h>
// #include <linux/compat.h>
#include <linux/kernel/mount.h>
#include <linux/fs/fs.h>
#include <linux/fs/internal.h>

// #include <linux/uaccess.h>
// #include <asm/unistd.h>


/**
 * noop_llseek - No Operation Performed llseek implementation
 * @file:	file structure to seek on
 * @offset:	file offset to seek to
 * @whence:	type of seek
 *
 * This is an implementation of ->llseek useable for the rare special case when
 * userspace expects the seek to succeed but the (device) file is actually not
 * able to perform the seek. In this case you use noop_llseek() instead of
 * falling back to the default implementation of ->llseek.
 */
loff_t noop_llseek(file_s *file, loff_t offset, int whence)
{
	return file->f_pos;
}