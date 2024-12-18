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

#include <linux/fs/file.h>
#include <linux/kernel/fdtable.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/mmap_lock.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/binfmts.h>
#include <linux/fs/fs.h>
#include <linux/fs/namei.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/uaccess.h>


#include <linux/fs/internal.h>


static DECLARE_LIST_HDR_S(formats);
// static DEFINE_RWLOCK(binfmt_lock);

void register_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	list_header_add_to_tail(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
}

void unregister_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	// list_del_init(&fmt->lh);
	list_header_delete_node(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
}


bool path_noexec(const path_s *path)
{
	return (path->mnt->mnt_flags & MNT_NOEXEC) ||
	       (path->mnt->mnt_sb->s_iflags & SB_I_NOEXEC);
}


static page_s
*get_arg_page(linux_bprm_s *bprm, ulong pos, int write) {
	page_s *page;
	vma_s *vma = bprm->vma;
	mm_s *mm = bprm->mm;
	int ret;

	mmap_read_lock(mm);
	/*
	 * We are doing an exec().  'current' is the process
	 * doing the exec and 'mm' is the new process's mm.
	 */
	ret = myos_get_user_pages(mm, pos, 1, &page);
	mmap_read_unlock(mm);
	if (ret <= 0)
		return NULL;

	// if (write)
	// 	acct_arg_size(bprm, vma_pages(vma));

	return page;
}

static void
put_arg_page(page_s *page) {
	put_page(page);
}

static void
free_arg_pages(linux_bprm_s *bprm) {
}

static int
__bprm_mm_init(linux_bprm_s *bprm) {
	int err;
	vma_s *vma = NULL;
	mm_s *mm = bprm->mm;

	bprm->vma = vma = vm_area_alloc(mm);
	if (!vma)
		return -ENOMEM;
	vma_set_anonymous(vma);

	if (mmap_write_lock_killable(mm)) {
		err = -EINTR;
		goto err_free;
	}

	/*
	 * Place the stack at the largest stack address the architecture
	 * supports. Later, we'll move this to an appropriate place. We don't
	 * use STACK_TOP because that can depend on attributes which aren't
	 * configured yet.
	 */
	// BUILD_BUG_ON(VM_STACK_FLAGS & VM_STACK_INCOMPLETE_SETUP);
	vma->vm_end = STACK_TOP_MAX;
	vma->vm_start = vma->vm_end - PAGE_SIZE;
	vma->vm_flags = VM_SOFTDIRTY | VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP;
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);

	err = insert_vm_struct(mm, vma);
	if (err)
		goto err;

	mm->stack_vm = mm->total_vm = 1;
	mmap_write_unlock(mm);
	bprm->p = vma->vm_end - sizeof(void *);
	return 0;
err:
	// ksm_exit(mm);
err_ksm:
	mmap_write_unlock(mm);
err_free:
	bprm->vma = NULL;
	vm_area_free(vma);
	return err;
}

static bool
valid_arg_len(linux_bprm_s *bprm, long len) {
	return len <= MAX_ARG_STRLEN;
}

/*
 * Create a new mm_struct and populate it with a temporary stack
 * vm_area_struct.  We don't have enough context at this point to set the stack
 * flags, permissions, and offset, so we use temporary values.  We'll update
 * them later in setup_arg_pages().
 */
static int
bprm_mm_init(linux_bprm_s *bprm) {
	int err;
	mm_s *mm = NULL;

	bprm->mm = mm = mm_alloc();
	err = -ENOMEM;
	if (!mm)
		goto err;

	// /* Save current stack limit for all calculations made during exec. */
	// task_lock(current->group_leader);
	// bprm->rlim_stack = current->signal->rlim[RLIMIT_STACK];
	// task_unlock(current->group_leader);

	err = __bprm_mm_init(bprm);
	if (err)
		goto err;

	return 0;

err:
	if (mm) {
		bprm->mm = NULL;
		mmdrop(mm);
	}

	return err;
}


static const char __user
*get_user_arg_ptr(const char __user *const __user *argv, int nr) {
	const char __user *native;
	if (get_user(native, argv + nr))
		return ERR_PTR(-EFAULT);

	return native;
}

/*
 * count() counts the number of strings in array ARGV.
 */
static int
count(const char *const *argv, int max) {
	int i = 0;

	if (argv != NULL) {
		for (;;) {
			const char __user *p =
					get_user_arg_ptr(argv, i);

			if (!p)
				break;

			if (IS_ERR(p))
				return -EFAULT;

			if (i >= max)
				return -E2BIG;
			++i;

			// if (fatal_signal_pending(current))
			// 	return -ERESTARTNOHAND;
			// cond_resched();
		}
	}
	return i;
}

static int
count_strings_kernel(const char *const *argv) {
	int i;

	if (!argv)
		return 0;

	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
			return -E2BIG;
		// if (fatal_signal_pending(current))
		// 	return -ERESTARTNOHAND;
		// cond_resched();
	}
	return i;
}

static inline int
bprm_set_stack_limit(linux_bprm_s *bprm, ulong limit) {
	/* Avoid a pathological bprm->p. */
	if (bprm->p < limit)
		return -E2BIG;
	bprm->argmin = bprm->p - limit;
	return 0;
}
static inline bool
bprm_hit_stack_limit(linux_bprm_s *bprm) {
	return bprm->p < bprm->argmin;
}

static int
bprm_stack_limits(linux_bprm_s *bprm) {
	ulong limit, ptr_size;

	/*
	 * Limit to 1/4 of the max stack size or 3/4 of _STK_LIM
	 * (whichever is smaller) for the argv+env strings.
	 * This ensures that:
	 *  - the remaining binfmt code will not run out of stack space,
	 *  - the program will have a reasonable amount of stack left
	 *    to work from.
	 */
	limit = _STK_LIM / 4 * 3;
	// limit = min(limit, bprm->rlim_stack.rlim_cur / 4);
	/*
	 * We've historically supported up to 32 pages (ARG_MAX)
	 * of argument strings even with small stacks
	 */
	limit = max_t(ulong, limit, ARG_MAX);
	/* Reject totally pathological counts. */
	if (bprm->argc < 0 || bprm->envc < 0)
		return -E2BIG;
	/*
	 * We must account for the size of all the argv and envp pointers to
	 * the argv and envp strings, since they will also take up space in
	 * the stack. They aren't stored until much later when we can't
	 * signal to the parent that the child has run out of stack space.
	 * Instead, calculate it here so it's possible to fail gracefully.
	 *
	 * In the case of argc = 0, make sure there is space for adding a
	 * empty string (which will bump argc to 1), to ensure confused
	 * userspace programs don't start processing from argv[1], thinking
	 * argc can never be 0, to keep them from walking envp by accident.
	 * See do_execveat_common().
	 */
	ptr_size = (max(bprm->argc, 1) + bprm->envc) * sizeof(void *);
	if (limit <= ptr_size)
		return -E2BIG;
	limit -= ptr_size;

	return bprm_set_stack_limit(bprm, limit);
}

/*
 * 'copy_strings()' copies argument/environment strings from the old
 * processes's memory to the new process's stack.  The call to get_user_pages()
 * ensures the destination page is created and not swapped out.
 */
static int
copy_strings(int argc, const char *const *argv, linux_bprm_s *bprm) {
	page_s *kmapped_page = NULL;
	char *kaddr = NULL;
	ulong kpos = 0;
	int ret;

	while (argc-- > 0) {
		const char __user *str;
		int len;
		ulong pos;

		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
			goto out;

		len = strnlen_user(str, MAX_ARG_STRLEN);
		if (!len)
			goto out;

		ret = -E2BIG;
		if (!valid_arg_len(bprm, len))
			goto out;

		/* We're going to work our way backwords. */
		pos = bprm->p;
		str += len;
		bprm->p -= len;
		if (bprm->p < bprm->argmin)
			goto out;

		while (len > 0) {
			int offset, bytes_to_copy;

			// if (fatal_signal_pending(current)) {
			// 	ret = -ERESTARTNOHAND;
			// 	goto out;
			// }
			// cond_resched();

			offset = pos % PAGE_SIZE;
			if (offset == 0)
				offset = PAGE_SIZE;

			bytes_to_copy = offset;
			if (bytes_to_copy > len)
				bytes_to_copy = len;

			offset -= bytes_to_copy;
			pos -= bytes_to_copy;
			str -= bytes_to_copy;
			len -= bytes_to_copy;

			// if (!kmapped_page || kpos != (pos & PAGE_MASK)) {
				page_s *page;

				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
				}

				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);

				// if (kmapped_page) {
				// 	flush_dcache_page(kmapped_page);
				// 	kunmap(kmapped_page);
				// 	put_arg_page(kmapped_page);
				// }
				// kmapped_page = page;
				// kaddr = kmap(kmapped_page);
				// kpos = pos & PAGE_MASK;
				// flush_arg_page(bprm, kpos, kmapped_page);
			// }
			// if (copy_from_user(kaddr+offset, str, bytes_to_copy)) {
			// 	ret = -EFAULT;
			// 	goto out;
			// }
		}
	}
	ret = 0;
out:
	// if (kmapped_page) {
	// 	flush_dcache_page(kmapped_page);
	// 	kunmap(kmapped_page);
	// 	put_arg_page(kmapped_page);
	// }
	return ret;
}

/*
 * Copy and argument/environment string from the kernel to the processes stack.
 */
int copy_string_kernel(const char *arg, linux_bprm_s *bprm)
{
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	ulong pos = bprm->p;

	if (len == 0)
		return -EFAULT;
	if (!valid_arg_len(bprm, len))
		return -E2BIG;

	/* We're going to work our way backwards. */
	arg += len;
	bprm->p -= len;
	if (bprm->p < bprm->argmin)
		return -E2BIG;

	while (len > 0) {
		uint bytes_to_copy = min_t(uint, len,
				min_not_zero(offset_in_page(pos), PAGE_SIZE));
		page_s *page;

		pos -= bytes_to_copy;
		arg -= bytes_to_copy;
		len -= bytes_to_copy;

		page = get_arg_page(bprm, pos, 1);
		if (!page)
			return -E2BIG;
		// flush_arg_page(bprm, pos & PAGE_MASK, page);
		memcpy_to_page(page, offset_in_page(pos), arg, bytes_to_copy);
		put_arg_page(page);
	}

	return 0;
}
EXPORT_SYMBOL(copy_string_kernel);

static int
copy_strings_kernel(int argc, const char *const *argv,
		linux_bprm_s *bprm) {

	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
		// if (fatal_signal_pending(current))
		// 	return -ERESTARTNOHAND;
		// cond_resched();
	}
	return 0;
}



/*
 * Finalizes the stack vm_area_struct. The flags and permissions are updated,
 * the stack is optionally relocated, and some extra space is added.
 */
int setup_arg_pages(linux_bprm_s *bprm,
		ulong stack_top, int executable_stack)
{
	ulong ret;
	ulong stack_shift;
	mm_s *mm = current->mm;
	vma_s *vma = bprm->vma;
	vma_s *prev = NULL;
	ulong vm_flags;
	ulong stack_base;
	ulong stack_size;
	ulong stack_expand;
	ulong rlim_stack;

	stack_top = arch_align_stack(stack_top);

	// if (unlikely(stack_top < mmap_min_addr) ||
	// 		unlikely(vma->vm_end - vma->vm_start >=
	// 			stack_top - mmap_min_addr))
	// 	return -ENOMEM;

	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;

	if (bprm->loader)
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	vm_flags = VM_STACK_FLAGS;

	/*
	 * Adjust stack execute permissions; explicitly enable for
	 * EXSTACK_ENABLE_X, disable for EXSTACK_DISABLE_X and leave alone
	 * (arch default) otherwise.
	 */
	if (unlikely(executable_stack == EXSTACK_ENABLE_X))
		vm_flags |= VM_EXEC;
	else if (executable_stack == EXSTACK_DISABLE_X)
		vm_flags &= ~VM_EXEC;
	vm_flags |= mm->def_flags;
	vm_flags |= VM_STACK_INCOMPLETE_SETUP;


	// tlb_gather_mmu(&tlb, mm);
	// ret = mprotect_fixup(vma, &prev, vma->vm_start, vma->vm_end,
	// 		vm_flags);
	// tlb_finish_mmu(&tlb);

	// if (ret)
	// 	goto out_unlock;
	// BUG_ON(prev != vma);

	if (unlikely(vm_flags & VM_EXEC)) {
		pr_warn_once("process '%pD4' started with"
			" executable stack\n", bprm->file);
	}

	// /* Move stack pages down in memory. */
	// if (stack_shift) {
	// 	ret = shift_arg_pages(vma, stack_shift);
	// 	if (ret)
	// 		goto out_unlock;
	// }

	/* mprotect_fixup is overkill to remove the temporary stack flags */
	vm_flags_clear(vma, VM_STACK_INCOMPLETE_SETUP);

	stack_expand = 32 * PAGE_SIZE; /* randomly 32*4k (or 2*64k) pages */
	stack_size = vma->vm_end - vma->vm_start;
	// /*
	//  * Align this down to a page boundary as expand_stack
	//  * will align it up.
	//  */
	// rlim_stack = bprm->rlim_stack.rlim_cur & PAGE_MASK;
	// stack_expand = min(rlim_stack, stack_size + stack_expand);

	stack_base = vma->vm_end - stack_expand;
	current->mm->start_stack = bprm->p;
	ret = expand_stack(vma, stack_base);
	if (ret)
		ret = -EFAULT;

out_unlock:
	mmap_write_unlock(mm);
	return ret;
}
EXPORT_SYMBOL(setup_arg_pages);

/*
 * On success, caller must call do_close_execat() on the returned
 * struct file to close it.
 */
static file_s
*do_open_execat(int fd, filename_s *name, int flags) {
	file_s *file;
	int err;
	open_flags_s open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
	};

	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
		open_exec_flags.lookup_flags |= LOOKUP_EMPTY;

	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;

	// /*
	//  * may_open() has already checked for this, so it should be
	//  * impossible to trip now. But we need to be extra cautious
	//  * and check again at the very end too.
	//  */
	// err = -EACCES;
	// if (WARN_ON_ONCE(!S_ISREG(file_inode(file)->i_mode) ||
	// 		path_noexec(&file->f_path)))
	// 	goto exit;

out:
	return file;

exit:
	fput(file);
	return ERR_PTR(err);
}

/**
 * open_exec - Open a path name for execution
 *
 * @name: path name to open with the intent of executing it.
 *
 * Returns ERR_PTR on failure or allocated struct file on success.
 *
 * As this is a wrapper for the internal do_open_execat(). Also see
 * do_close_execat().
 */
file_s *open_exec(const char *name)
{
	filename_s *filename = getname_kernel(name);
	file_s *f = ERR_CAST(filename);

	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
}

/*
 * Maps the mm_struct mm into the current task struct.
 * On success, this function returns with exec_update_lock
 * held for writing.
 */
static int
exec_mmap(mm_s *mm) {
	task_s *tsk;
	mm_s *old_mm, *active_mm;
	int ret;

	/* Notify parent that we're no longer interested in the old VM */
	tsk = current;
	old_mm = current->mm;
	exec_mm_release(tsk, old_mm);

	// ret = down_write_killable(&tsk->signal->exec_update_lock);
	// if (ret)
	// 	return ret;

	if (old_mm) {
		/*
		 * If there is a pending fatal signal perhaps a signal
		 * whose default action is to create a coredump get
		 * out and die instead of going through with the exec.
		 */
		ret = mmap_read_lock_killable(old_mm);
		// if (ret) {
		// 	up_write(&tsk->signal->exec_update_lock);
		// 	return ret;
		// }
	}

	task_lock(tsk);
	// membarrier_exec_mmap(mm);

	local_irq_disable();
	active_mm = tsk->active_mm;
	tsk->active_mm = mm;
	tsk->mm = mm;
	// mm_init_cid(mm);
	// /*
	//  * This prevents preemption while active_mm is being loaded and
	//  * it and mm are being updated, which could cause problems for
	//  * lazy tlb mm refcounting when these are updated by context
	//  * switches. Not all architectures can handle irqs off over
	//  * activate_mm yet.
	//  */
	// if (!IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
	// 	local_irq_enable();
	activate_mm(active_mm, mm);
	// if (IS_ENABLED(CONFIG_ARCH_WANT_IRQS_OFF_ACTIVATE_MM))
		local_irq_enable();
	// tsk->mm->vmacache_seqnum = 0;
	// vmacache_flush(tsk);
	task_unlock(tsk);
	if (old_mm) {
		mmap_read_unlock(old_mm);
		BUG_ON(active_mm != old_mm);
		// setmax_mm_hiwater_rss(&tsk->signal->maxrss, old_mm);
		// mm_update_next_owner(old_mm);
		mmput(old_mm);
		return 0;
	}
	mmdrop(active_mm);
	return 0;
}

static int
de_thread(task_s *tsk) {
	// struct signal_struct *sig = tsk->signal;
	// struct sighand_struct *oldsighand = tsk->sighand;
	// spinlock_t *lock = &oldsighand->siglock;

	// if (thread_group_empty(tsk))
	// 	goto no_thread_group;

	// /*
	//  * Kill all other threads in the thread group.
	//  */
	// spin_lock_irq(lock);
	// if ((sig->flags & SIGNAL_GROUP_EXIT) || sig->group_exec_task) {
	// 	/*
	// 	 * Another group action in progress, just
	// 	 * return so that the signal is processed.
	// 	 */
	// 	spin_unlock_irq(lock);
	// 	return -EAGAIN;
	// }

	// sig->group_exec_task = tsk;
	// sig->notify_count = zap_other_threads(tsk);
	// if (!thread_group_leader(tsk))
	// 	sig->notify_count--;

	// while (sig->notify_count) {
	// 	__set_current_state(TASK_KILLABLE);
	// 	spin_unlock_irq(lock);
	// 	schedule();
	// 	if (__fatal_signal_pending(tsk))
	// 		goto killed;
	// 	spin_lock_irq(lock);
	// }
	// spin_unlock_irq(lock);

	// /*
	//  * At this point all other threads have exited, all we have to
	//  * do is to wait for the thread group leader to become inactive,
	//  * and to assume its PID:
	//  */
	// if (!thread_group_leader(tsk)) {
	// 	struct task_struct *leader = tsk->group_leader;

	// 	for (;;) {
	// 		cgroup_threadgroup_change_begin(tsk);
	// 		write_lock_irq(&tasklist_lock);
	// 		/*
	// 		 * Do this under tasklist_lock to ensure that
	// 		 * exit_notify() can't miss ->group_exec_task
	// 		 */
	// 		sig->notify_count = -1;
	// 		if (likely(leader->exit_state))
	// 			break;
	// 		__set_current_state(TASK_KILLABLE);
	// 		write_unlock_irq(&tasklist_lock);
	// 		cgroup_threadgroup_change_end(tsk);
	// 		schedule();
	// 		if (__fatal_signal_pending(tsk))
	// 			goto killed;
	// 	}

	// 	/*
	// 	 * The only record we have of the real-time age of a
	// 	 * process, regardless of execs it's done, is start_time.
	// 	 * All the past CPU time is accumulated in signal_struct
	// 	 * from sister threads now dead.  But in this non-leader
	// 	 * exec, nothing survives from the original leader thread,
	// 	 * whose birth marks the true age of this process now.
	// 	 * When we take on its identity by switching to its PID, we
	// 	 * also take its birthdate (always earlier than our own).
	// 	 */
	// 	tsk->start_time = leader->start_time;
	// 	tsk->start_boottime = leader->start_boottime;

	// 	BUG_ON(!same_thread_group(leader, tsk));
	// 	/*
	// 	 * An exec() starts a new thread group with the
	// 	 * TGID of the previous thread group. Rehash the
	// 	 * two threads with a switched PID, and release
	// 	 * the former thread group leader:
	// 	 */

	// 	/* Become a process group leader with the old leader's pid.
	// 	 * The old leader becomes a thread of the this thread group.
	// 	 */
	// 	exchange_tids(tsk, leader);
	// 	transfer_pid(leader, tsk, PIDTYPE_TGID);
	// 	transfer_pid(leader, tsk, PIDTYPE_PGID);
	// 	transfer_pid(leader, tsk, PIDTYPE_SID);

	// 	list_replace_rcu(&leader->tasks, &tsk->tasks);
	// 	list_replace_init(&leader->sibling, &tsk->sibling);

	// 	tsk->group_leader = tsk;
	// 	leader->group_leader = tsk;

	// 	tsk->exit_signal = SIGCHLD;
	// 	leader->exit_signal = -1;

	// 	BUG_ON(leader->exit_state != EXIT_ZOMBIE);
	// 	leader->exit_state = EXIT_DEAD;
	// 	/*
	// 	 * We are going to release_task()->ptrace_unlink() silently,
	// 	 * the tracer can sleep in do_wait(). EXIT_DEAD guarantees
	// 	 * the tracer won't block again waiting for this thread.
	// 	 */
	// 	if (unlikely(leader->ptrace))
	// 		__wake_up_parent(leader, leader->parent);
	// 	write_unlock_irq(&tasklist_lock);
	// 	cgroup_threadgroup_change_end(tsk);

	// 	release_task(leader);
	// }

	// sig->group_exec_task = NULL;
	// sig->notify_count = 0;

no_thread_group:
	// /* we have changed execution domain */
	// tsk->exit_signal = SIGCHLD;

	// BUG_ON(!thread_group_leader(tsk));
	return 0;

killed:
	// /* protects against exit_notify() and __exit_signal() */
	// read_lock(&tasklist_lock);
	// sig->group_exec_task = NULL;
	// sig->notify_count = 0;
	// read_unlock(&tasklist_lock);
	return -EAGAIN;
}


char *__get_task_comm(char *buf, size_t buf_size, task_s *tsk)
{
	task_lock(tsk);
	/* Always NUL terminated and zero-padded */
	strscpy_pad(buf, tsk->comm, buf_size);
	task_unlock(tsk);
	return buf;
}
EXPORT_SYMBOL_GPL(__get_task_comm);

/*
 * These functions flushes out all traces of the currently running executable
 * so that a new one can be started
 */

// void __set_task_comm(task_s *tsk, const char *buf, bool exec)
void __set_task_comm(task_s *tsk, const char *buf)
{
	task_lock(tsk);
	// trace_task_rename(tsk, buf);
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
	// perf_event_comm(tsk, exec);
}

/*
 * Calling this is the point of no return. None of the failures will be
 * seen by userspace since either the process is already taking a fatal
 * signal (via de_thread() or coredump), or will have SEGV raised
 * (after exec_mmap()) by search_binary_handler (see below).
 */
int begin_new_exec(linux_bprm_s * bprm)
{
	task_s *me = current;
	int retval;

	// /* Once we are committed compute the creds */
	// retval = bprm_creds_from_file(bprm);
	// if (retval)
	// 	return retval;

	// /*
	//  * This tracepoint marks the point before flushing the old exec where
	//  * the current task is still unchanged, but errors are fatal (point of
	//  * no return). The later "sched_process_exec" tracepoint is called after
	//  * the current task has successfully switched to the new exec.
	//  */
	// trace_sched_prepare_exec(current, bprm);

	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;

	/*
	 * Make this the only thread in the thread group.
	 */
	retval = de_thread(me);
	if (retval != -ENOERR)
		goto out;

	// /*
	//  * Cancel any io_uring activity across execve
	//  */
	// io_uring_task_cancel();

	/* Ensure the files table is not shared. */
	retval = unshare_files();
	if (retval)
		goto out;

	/*
	 * Must be called _before_ exec_mmap() as bprm->mm is
	 * not visibile until then. This also enables the update
	 * to be lockless.
	 */
	retval = set_mm_exe_file(bprm->mm, bprm->file);
	if (retval)
		goto out;

	// /* If the binary is not readable then enforce mm->dumpable=0 */
	// would_dump(bprm, bprm->file);
	// if (bprm->have_execfd)
	// 	would_dump(bprm, bprm->executable);

	/*
	 * Release all of the old mmap stuff
	 */
	// acct_arg_size(bprm, 0);
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;

	// retval = exec_task_namespaces();
	// if (retval)
	// 	goto out_unlock;

// #ifdef CONFIG_POSIX_TIMERS
	// spin_lock_irq(&me->sighand->siglock);
	// posix_cpu_timers_exit(me);
	// spin_unlock_irq(&me->sighand->siglock);
	// exit_itimers(me);
	// flush_itimer_signals();
// #endif

	// /*
	//  * Make the signal table private.
	//  */
	// retval = unshare_sighand(me);
	// if (retval)
	// 	goto out_unlock;

	me->flags &= ~(PF_RANDOMIZE | PF_FORKNOEXEC |
					PF_NOFREEZE | PF_NO_SETAFFINITY);
	// flush_thread();
	// me->personality &= ~bprm->per_clear;

	// clear_syscall_work_syscall_user_dispatch(me);

	// /*
	//  * We have to apply CLOEXEC before we change whether the process is
	//  * dumpable (in setup_new_exec) to avoid a race with a process in userspace
	//  * trying to access the should-be-closed file descriptors of a process
	//  * undergoing exec(2).
	//  */
	// do_close_on_exec(me->files);

	// if (bprm->secureexec) {
	// 	/* Make sure parent cannot signal privileged process. */
	// 	me->pdeath_signal = 0;

	// 	/*
	// 	 * For secureexec, reset the stack limit to sane default to
	// 	 * avoid bad behavior from the prior rlimits. This has to
	// 	 * happen before arch_pick_mmap_layout(), which examines
	// 	 * RLIMIT_STACK, but after the point of no return to avoid
	// 	 * needing to clean up the change on failure.
	// 	 */
	// 	if (bprm->rlim_stack.rlim_cur > _STK_LIM)
	// 		bprm->rlim_stack.rlim_cur = _STK_LIM;
	// }

	me->sas_ss_sp = me->sas_ss_size = 0;

	// /*
	//  * Figure out dumpability. Note that this checking only of current
	//  * is wrong, but userspace depends on it. This should be testing
	//  * bprm->secureexec instead.
	//  */
	// if (bprm->interp_flags & BINPRM_FLAGS_ENFORCE_NONDUMP ||
	//     !(uid_eq(current_euid(), current_uid()) &&
	//       gid_eq(current_egid(), current_gid())))
	// 	set_dumpable(current->mm, suid_dumpable);
	// else
	// 	set_dumpable(current->mm, SUID_DUMP_USER);

	// perf_event_exec();
	__set_task_comm(me, bprm->filename);

	// /* An exec changes our domain. We are no longer part of the thread
	//    group */
	// WRITE_ONCE(me->self_exec_id, me->self_exec_id + 1);
	// flush_signal_handlers(me, 0);

	// retval = set_cred_ucounts(bprm->cred);
	// if (retval < 0)
	// 	goto out_unlock;

	// /*
	//  * install the new credentials for this executable
	//  */
	// security_bprm_committing_creds(bprm);

	// commit_creds(bprm->cred);
	// bprm->cred = NULL;

	// /*
	//  * Disable monitoring for regular users
	//  * when executing setuid binaries. Must
	//  * wait until new credentials are committed
	//  * by commit_creds() above
	//  */
	// if (get_dumpable(me->mm) != SUID_DUMP_USER)
	// 	perf_event_exit_task(me);
	// /*
	//  * cred_guard_mutex must be held at least to this point to prevent
	//  * ptrace_attach() from altering our determination of the task's
	//  * credentials; any time after this it may be unlocked.
	//  */
	// security_bprm_committed_creds(bprm);

	/* Pass the opened binary to the interpreter. */
	if (bprm->have_execfd) {
		retval = get_unused_fd_flags(0);
		if (retval < 0)
			goto out_unlock;
		fd_install(retval, bprm->executable);
		bprm->executable = NULL;
		bprm->execfd = retval;
	}
	return 0;

out_unlock:
	// up_write(&me->signal->exec_update_lock);
	// if (!bprm->cred)
	// 	mutex_unlock(&me->signal->cred_guard_mutex);

out:
	return retval;
}
EXPORT_SYMBOL(begin_new_exec);

void setup_new_exec(linux_bprm_s * bprm)
{
	/* Setup things that can depend upon the personality */
	task_s *me = current;

	arch_pick_mmap_layout(me->mm, &bprm->rlim_stack);

	// arch_setup_new_exec();

	/*
	 * Set the new mm task size. We have to do that late because it may
	 * depend on TIF_32BIT which is only updated in flush_thread() on
	 * some architectures like powerpc
	 */
	me->mm->task_size = TASK_SIZE;
	// up_write(&me->signal->exec_update_lock);
	// mutex_unlock(&me->signal->cred_guard_mutex);
}
EXPORT_SYMBOL(setup_new_exec);

/* Runs immediately before start_thread() takes over. */
void finalize_exec(linux_bprm_s *bprm)
{
	// /* Store any stack rlimit changes before starting thread. */
	// task_lock(current->group_leader);
	// current->signal->rlim[RLIMIT_STACK] = bprm->rlim_stack;
	// task_unlock(current->group_leader);
}
EXPORT_SYMBOL(finalize_exec);


/* Matches do_open_execat() */
static void
do_close_execat(file_s *file) {
	if (file)
		fput(file);
}

static void
free_bprm(linux_bprm_s *bprm) {
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
	free_arg_pages(bprm);
	// if (bprm->cred) {
	// 	mutex_unlock(&current->signal->cred_guard_mutex);
	// 	abort_creds(bprm->cred);
	// }
	if (bprm->file) {
		// allow_write_access(bprm->file);
		fput(bprm->file);
	}
	do_close_execat(bprm->file);
	if (bprm->executable)
		fput(bprm->executable);
	/* If a binfmt changed the interp, free it. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
}

static linux_bprm_s
*alloc_bprm(int fd, filename_s *filename, int flags) {
	task_s * curr = current;

	linux_bprm_s *bprm;
	file_s *file;
	int retval = -ENOMEM;

	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);

	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (bprm == NULL) {
		do_close_execat(file);
		return ERR_PTR(-ENOMEM);
	}

	bprm->file = file;

	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
		if (filename->name[0] == '\0')
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d", fd);
		else
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d/%s",
						  fd, filename->name);
		if (bprm->fdpath == NULL)
			goto out_free;

		// /*
		//  * Record that a name derived from an O_CLOEXEC fd will be
		//  * inaccessible after exec.  This allows the code in exec to
		//  * choose to fail when the executable is not mmaped into the
		//  * interpreter and an open file descriptor is not passed to
		//  * the interpreter.  This makes for a better user experience
		//  * than having the interpreter start and then immediately fail
		//  * when it finds the executable is inaccessible.
		//  */
		// if (get_close_on_exec(fd))
		// 	bprm->interp_flags |= BINPRM_FLAGS_PATH_INACCESSIBLE;

		bprm->filename = bprm->fdpath;
	}
	bprm->interp = bprm->filename;

	retval = bprm_mm_init(bprm);
	if (retval == 0)
		return bprm;

out_free:
	free_bprm(bprm);
	return ERR_PTR(retval);
}


/*
 * Fill the binprm structure from the inode.
 * Read the first BINPRM_BUF_SIZE bytes
 *
 * This may be called multiple times for binary chains (scripts for example).
 */
static int
prepare_binprm(linux_bprm_s *bprm) {
	loff_t pos = 0;

	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, bprm->buf, BINPRM_BUF_SIZE, &pos);
}


#define printable(c) (						\
			((c)=='\t') || ((c)=='\n') ||	\
				(0x20<=(c) && (c)<=0x7e)	\
		)
/*
 * cycle the list of binary formats handler, until one recognizes the image
 */
static int
search_binary_handler(linux_bprm_s *bprm) {
	mm_s *currmm = current->mm;

	// bool need_retry = IS_ENABLED(CONFIG_MODULES);
	linux_bfmt_s *fmt;
	int retval;

	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;

	// retval = security_bprm_check(bprm);
	// if (retval)
	// 	return retval;

	retval = -ENOENT;
//  retry:
	// read_lock(&binfmt_lock);
	list_header_for_each_container(fmt, &formats, lh) {
		// if (!try_module_get(fmt->module))
		// 	continue;
		// read_unlock(&binfmt_lock);

		retval = fmt->load_binary(bprm);

		// read_lock(&binfmt_lock);
		// put_binfmt(fmt);
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
		// 	read_unlock(&binfmt_lock);
		// 	return retval;
		// }
	}
	// read_unlock(&binfmt_lock);

	// if (need_retry) {
	// 	if (printable(bprm->buf[0]) && printable(bprm->buf[1]) &&
	// 	    printable(bprm->buf[2]) && printable(bprm->buf[3]))
	// 		return retval;
	// 	if (request_module("binfmt-%04x", *(ushort *)(bprm->buf + 2)) < 0)
	// 		return retval;
	// 	need_retry = false;
	// 	goto retry;
	// }

	return retval;
}

/*
 * sys_execve() executes a new program.
 */
static int
bprm_execve(linux_bprm_s *bprm, int fd,
		filename_s *filename, int flags) {

	int retval;
	file_s *file;
	task_s *curr = current;

	// retval = prepare_bprm_creds(bprm);
	// if (retval)
	// 	return retval;

	// /*
	//  * Check for unsafe execution states before exec_binprm(), which
	//  * will call back into begin_new_exec(), into bprm_creds_from_file(),
	//  * where setuid-ness is evaluated.
	//  */
	// check_unsafe_exec(bprm);
	curr->in_execve = 1;
	// sched_mm_cid_before_execve(current);

	// sched_exec();

	// /* Set the unchanging part of bprm->cred */
	// retval = security_bprm_creds_for_exec(bprm);
	// if (retval)
	// 	goto out;

	// retval = exec_binprm(bprm);
	// static int exec_binprm(linux_bprm_s *bprm)
	// {
		retval = search_binary_handler(bprm);
	// }
	if (retval < 0)
		goto out;

	// sched_mm_cid_after_execve(current);
	/* execve succeeded */
	curr->fs->in_exec = 0;
	curr->in_execve = 0;
	// rseq_execve(current);
	// user_events_execve(current);
	// acct_update_integrals(current);
	// task_numa_free(current, false);
	return retval;

out:
	// /*
	//  * If past the point of no return ensure the code never
	//  * returns to the userspace process.  Use an existing fatal
	//  * signal if present otherwise terminate the process with
	//  * SIGSEGV.
	//  */
	// if (bprm->point_of_no_return && !fatal_signal_pending(current))
	// 	force_fatal_sig(SIGSEGV);

	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
}

static int
do_execveat_common(int fd, filename_s *filename,
	const char *const *argv, const char *const *envp, int flags) {

	linux_bprm_s *bprm;
	int retval;

	if (IS_ERR(filename))
		return PTR_ERR(filename);

	// /*
	//  * We move the actual failure in case of RLIMIT_NPROC excess from
	//  * set*uid() to execve() because too many poorly written programs
	//  * don't check setuid() return code.  Here we additionally recheck
	//  * whether NPROC limit is still exceeded.
	//  */
	// if ((current->flags & PF_NPROC_EXCEEDED) &&
	//     is_ucounts_overlimit(current_ucounts(), UCOUNT_RLIMIT_NPROC, rlimit(RLIMIT_NPROC))) {
	// 	retval = -EAGAIN;
	// 	goto out_ret;
	// }

	/* We're below the limit (still or again), so we don't want to make
	 * further execve() calls fail. */
	current->flags &= ~PF_NPROC_EXCEEDED;

	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}

	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv:"
				" empty string added\n", current->comm, bprm->filename);
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;

	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;

	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;

	/*
	 * When argv is empty, add an empty string ("") as argv[0] to
	 * ensure confused userspace programs that start processing
	 * from argv[1] won't end up walking envp. See also
	 * bprm_stack_limits().
	 */
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
			goto out_free;
		bprm->argc = 1;
	}
	
	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
}


int kernel_execve(const char *kernel_filename,
		const char *const *argv, const char *const *envp)
{
	task_s * curr = current;

	filename_s *filename;
	linux_bprm_s *bprm;
	int fd = AT_FDCWD;
	int retval;

	/* It is non-sense for kernel threads to call execve */
	if (WARN_ON_ONCE(current->flags & PF_KTHREAD))
		return -EINVAL;

	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);

	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}

	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;

	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;

	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;

	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;

	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
}

static int
do_execve(filename_s *filename,
		const char *const *__argv,
		const char *const *__envp) {
	return do_execveat_common(AT_FDCWD,
			filename, __argv, __envp, 0);
}


void set_binfmt(linux_bfmt_s *new)
{
	mm_s *mm = current->mm;

	// if (mm->binfmt)
	// 	module_put(mm->binfmt->module);

	mm->binfmt = new;
	// if (new)
	// 	__module_get(new->module);
}
EXPORT_SYMBOL(set_binfmt);


MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
}