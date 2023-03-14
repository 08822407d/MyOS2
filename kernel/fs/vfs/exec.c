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
#include <linux/kernel/spinlock.h>
// #include <linux/key.h>
// #include <linux/personality.h>
#include <linux/fs/binfmts.h>
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



// static int do_execveat_common(int fd, struct filename *filename,
// 		struct user_arg_ptr argv, struct user_arg_ptr envp, int flags)
static int do_execveat_common(int fd, filename_s *filename,
	const char *const *argv, const char *const *envp, int flags) {
	linux_bprm_s *bprm;
	int retval;

	if (IS_ERR(filename))
		return PTR_ERR(filename);

// 	/*
// 	 * We move the actual failure in case of RLIMIT_NPROC excess from
// 	 * set*uid() to execve() because too many poorly written programs
// 	 * don't check setuid() return code.  Here we additionally recheck
// 	 * whether NPROC limit is still exceeded.
// 	 */
// 	if ((current->flags & PF_NPROC_EXCEEDED) &&
// 	    is_ucounts_overlimit(current_ucounts(), UCOUNT_RLIMIT_NPROC, rlimit(RLIMIT_NPROC))) {
// 		retval = -EAGAIN;
// 		goto out_ret;
// 	}

	/* We're below the limit (still or again), so we don't want to make
	 * further execve() calls fail. */
	current->flags &= ~PF_NPROC_EXCEEDED;

// 	bprm = alloc_bprm(fd, filename);
// 	if (IS_ERR(bprm)) {
// 		retval = PTR_ERR(bprm);
// 		goto out_ret;
// 	}

// 	retval = count(argv, MAX_ARG_STRINGS);
// 	if (retval == 0)
// 		pr_warn_once("process '%s' launched '%s' with NULL argv: empty string added\n",
// 			     current->comm, bprm->filename);
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->argc = retval;

// 	retval = count(envp, MAX_ARG_STRINGS);
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->envc = retval;

// 	retval = bprm_stack_limits(bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	retval = copy_string_kernel(bprm->filename, bprm);
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->exec = bprm->p;

// 	retval = copy_strings(bprm->envc, envp, bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	retval = copy_strings(bprm->argc, argv, bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	/*
// 	 * When argv is empty, add an empty string ("") as argv[0] to
// 	 * ensure confused userspace programs that start processing
// 	 * from argv[1] won't end up walking envp. See also
// 	 * bprm_stack_limits().
// 	 */
// 	if (bprm->argc == 0) {
// 		retval = copy_string_kernel("", bprm);
// 		if (retval < 0)
// 			goto out_free;
// 		bprm->argc = 1;
// 	}

// 	retval = bprm_execve(bprm, fd, filename, flags);
// out_free:
// 	free_bprm(bprm);

// out_ret:
// 	putname(filename);
// 	return retval;
}


int kernel_execve(const char *kernel_filename,
	const char *const *argv, const char *const *envp)
{
	filename_s *filename;
	linux_bprm_s *bprm;
	int fd = AT_FDCWD;
	int retval;

	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);

// 	bprm = alloc_bprm(fd, filename);
// 	if (IS_ERR(bprm)) {
// 		retval = PTR_ERR(bprm);
// 		goto out_ret;
// 	}

// 	retval = count_strings_kernel(argv);
// 	if (WARN_ON_ONCE(retval == 0))
// 		retval = -EINVAL;
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->argc = retval;

// 	retval = count_strings_kernel(envp);
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->envc = retval;

// 	retval = bprm_stack_limits(bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	retval = copy_string_kernel(bprm->filename, bprm);
// 	if (retval < 0)
// 		goto out_free;
// 	bprm->exec = bprm->p;

// 	retval = copy_strings_kernel(bprm->envc, envp, bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	retval = copy_strings_kernel(bprm->argc, argv, bprm);
// 	if (retval < 0)
// 		goto out_free;

// 	retval = bprm_execve(bprm, fd, filename, 0);
// out_free:
// 	free_bprm(bprm);
// out_ret:
// 	putname(filename);
// 	return retval;
}

static int do_execve(filename_s *filename,
	const char *const *__argv, const char *const *__envp) {
	// struct user_arg_ptr argv = { .ptr.native = __argv };
	// struct user_arg_ptr envp = { .ptr.native = __envp };
	return do_execveat_common(AT_FDCWD, filename, __argv, __envp, 0);
}


extern long myos_do_execve(const char *filename,
	const char *const *argv, const char *const *envp);
long sys_execve(const char *filename,
	const char *const __user *argv, const char *const __user *envp)
{
	// return do_execve(getname(filename), argv, envp);
	return myos_do_execve(filename, argv, envp);
}