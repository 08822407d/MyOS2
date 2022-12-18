// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/fs/exec.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * #!-checking implemented by tytso.
 */
/*
 * Demand-loading implemented 01.12.91 - no need to read anything but
 * the header into memory. The inode of the executable is put into
 * "current->executable", and page faults do the actual loading. Clean.
 *
 * Once more I can proudly say that linux stood up to being changed: it
 * was less than 2 hours work to get demand-loading completely implemented.
 *
 * Demand loading changed July 1993 by Eric Youngdale.   Use mmap instead,
 * current->executable is only used by the procfs.  This allows a dispatch
 * table to check for several different types  of binary formats.  We keep
 * trying until we recognize the file or we run out of supported binary
 * formats.
 */

// #include <linux/kernel_read_file.h>
#include <linux/kernel/slab.h>
#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/mm/mm.h>
// #include <linux/vmacache.h>
#include <linux/kernel/stat.h>
#include <linux/kernel/fcntl.h>
// #include <linux/swap.h>
#include <linux/lib/string.h>
#include <linux/init/init.h>
#include <linux/sched/mm.h>
// #include <linux/sched/coredump.h>
// #include <linux/sched/signal.h>
// #include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
// #include <linux/pagemap.h>
// #include <linux/perf_event.h>
// #include <linux/highmem.h>
// #include <linux/spinlock.h>
// #include <linux/key.h>
// #include <linux/personality.h>
// #include <linux/binfmts.h>
// #include <linux/utsname.h>
// #include <linux/pid_namespace.h>
// #include <linux/module.h>
#include <linux/fs/namei.h>
#include <linux/kernel/mount.h>
// #include <linux/security.h>
#include <linux/kernel/syscalls.h>
// #include <linux/tsacct_kern.h>
// #include <linux/cn_proc.h>
// #include <linux/audit.h>
// #include <linux/tracehook.h>
// #include <linux/kmod.h>
// #include <linux/fsnotify.h>
#include <linux/sched/fs_struct.h>
// #include <linux/oom.h>
// #include <linux/compat.h>
// #include <linux/vmalloc.h>
// #include <linux/io_uring.h>
// #include <linux/syscall_user_dispatch.h>
// #include <linux/coredump.h>

// #include <linux/uaccess.h>
// #include <asm/mmu_context.h>
// #include <asm/tlb.h>

// #include <trace/events/task.h>
// #include "internal.h"

// #include <trace/events/sched.h>

/*
 * These functions flushes out all traces of the currently running executable
 * so that a new one can be started
 */

// void __set_task_comm(struct task_struct *tsk, const char *buf, bool exec)
void set_task_comm(task_s *tsk, const char *buf)
{
	// task_lock(tsk);
	// trace_task_rename(tsk, buf);
	// strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	
	// task_unlock(tsk);
	// perf_event_comm(tsk, exec);
}