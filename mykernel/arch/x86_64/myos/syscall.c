#include <linux/kernel/fcntl.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/file.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/sched_api.h>
#include <linux/kernel/sched_types.h>
#include <linux/fs/namei.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/myos_sysreboot.h>
#include <asm/syscall.h>
#include <asm/setup.h>
#include <asm/insns.h>
#include <asm/signal.h>
#include <asm/proto.h>
#include <asm/delay.h>

#include <obsolete/printk.h>
#include <obsolete/arch_proto.h>




MYOS_SYSCALL_DEFINE0(no_syscall)
{
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
}

MYOS_SYSCALL_DEFINE1(myos_putstring, char *, string)
{
	color_printk(WHITE, BLACK, string);
	return 0;
}

/*==============================================================================================*
 *										file operations											*
 *==============================================================================================*/
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
}

MYOS_SYSCALL_DEFINE1(close, unsigned int, fd)
{
	task_s * curr = current;

	file_s * fp = NULL;

	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;

	fp = curr->files->fd_array[fd];
	// if(fp->f_op && fp->f_op->close)
	// 	fp->f_op->close(fp->f_path.dentry->d_inode, fp);

	kfree(fp);
	curr->files->fd_array[fd] = NULL;

	// int retval = close_fd(fd);

	// /* can't restart close syscall because file table entry was cleared */
	// if (unlikely(retval == -ERESTARTSYS ||
	// 	     retval == -ERESTARTNOINTR ||
	// 	     retval == -ERESTARTNOHAND ||
	// 	     retval == -ERESTART_RESTARTBLOCK))
	// 	retval = -EINTR;

	return 0;
}

MYOS_SYSCALL_DEFINE3(read, unsigned int, fd,
		char *,buf, size_t, count) {
	return ksys_read(fd, buf, count);
}

MYOS_SYSCALL_DEFINE3(write, unsigned int, fd,
		const char *, buf, size_t, count)
{
	task_s * curr = current;
	file_s * fp = NULL;
	ulong ret = 0;

	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = curr->files->fd_array[fd];
	if(fp->f_op && fp->f_op->write)
		ret = fp->f_op->write(fp, buf, count, &fp->f_pos);
	return ret;
}

MYOS_SYSCALL_DEFINE3(lseek, unsigned int, fd,
		off_t, offset, unsigned int, whence)
{
	task_s * curr = current;
	file_s * fp = NULL;
	ulong ret = 0;

	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	fp = current->files->fd_array[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
}

MYOS_SYSCALL_DEFINE0(fork)
{
	barrier();
	ulong counter = 0;
	for (long i = 0; i < 0x1000; i++)
		for (long j = 0; j < 0x1000; j++)
			counter++;
	barrier();

	kclone_args_s args = {
		// .flags = CLONE_FILES,
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
}

MYOS_SYSCALL_DEFINE1(exit, int, error_code)
{
	do_exit(error_code);
}


/*======================================================================================*
 *									user memory manage									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE1(brk, unsigned long, brk)
{
	ulong newbrk, oldbrk, origbrk;
	mm_s *mm = current->mm;
	vma_s *brkvma, *next = NULL;
	ulong min_brk;
	bool downgraded = false;

	origbrk = mm->brk;
	min_brk = mm->start_brk;
	if (brk < min_brk)
		goto out;

	// /*
	//  * Check against rlimit here. If this check is done later after the test
	//  * of oldbrk with newbrk then it can escape the test and let the data
	//  * segment grow beyond its set limit the in case where the limit is
	//  * not page aligned -Ram Gupta
	//  */
	// if (check_data_rlimit(rlimit(RLIMIT_DATA), brk,
	// 		mm->start_brk, mm->end_data, mm->start_data))
	// 	goto out;

	newbrk = PAGE_ALIGN(brk);
	oldbrk = PAGE_ALIGN(mm->brk);
	if (oldbrk == newbrk) {
		mm->brk = brk;
		goto success;
	}

	/* Always allow shrinking brk. */
	if (brk <= mm->brk) {
		// /* Search one past newbrk */
		// vma_iter_init(&vmi, mm, newbrk);
		// brkvma = vma_find(&vmi, oldbrk);
		// if (!brkvma || brkvma->vm_start >= oldbrk)
		// 	goto out; /* mapping intersects with an existing non-brk vma. */

		/*
		 * mm->brk must be protected by write mmap_lock.
		 * do_vma_munmap() will drop the lock on success,  so update it
		 * before calling do_vma_munmap().
		 */
		mm->brk = brk;
		if (simple_do_vma_munmap(mm, newbrk, oldbrk))
			goto out;

		goto success_unlocked;
	}

	// if (check_brk_limits(oldbrk, newbrk - oldbrk))
	// 	goto out;

	// /*
	//  * Only check if the next VMA is within the stack_guard_gap of the
	//  * expansion area
	//  */
	// vma_iter_init(&vmi, mm, oldbrk);
	// next = vma_find(&vmi, newbrk + PAGE_SIZE + stack_guard_gap);

	// if (next && newbrk + PAGE_SIZE > vm_start_gap(next))
	// 	goto out;

	/* Ok, looks good - let it rip. */
	if (do_brk_flags(oldbrk, newbrk-oldbrk, 0) < 0)
		goto out;

	mm->brk = brk;
	// if (mm->def_flags & VM_LOCKED)
	// 	populate = true;

	// next = simple_find_vma(mm, oldbrk);
	// while (next == NULL);
	
success:
	// mmap_write_unlock(mm);
success_unlocked:
	// userfaultfd_unmap_complete(mm, &uf);
	// if (populate)
	// 	mm_populate(oldbrk, newbrk - oldbrk);
	return brk;

out:
	mm->brk = origbrk;
	// mmap_write_unlock(mm);
	return origbrk;
}


/*======================================================================================*
 *									get task infomation									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
{
	ALERT_DUMMY_SYSCALL(ioctl, IF_ALERT_DUMMY_SYSCALL);

// 	struct fd f = fdget(fd);
// 	int error;

// 	if (!f.file)
// 		return -EBADF;

// 	error = security_file_ioctl(f.file, cmd, arg);
// 	if (error)
// 		goto out;

// 	error = do_vfs_ioctl(f.file, fd, cmd, arg);
// 	if (error == -ENOIOCTLCMD)
// 		error = vfs_ioctl(f.file, cmd, arg);

// out:
// 	fdput(f);
// 	return error;

	return 0;
}

/**
 * sys_sched_yield - yield the current processor to other threads.
 *
 * This function yields the current CPU to other tasks. If there are no
 * other threads running on this CPU then this function will return.
 *
 * Return: 0.
 */
MYOS_SYSCALL_DEFINE0(sched_yield)
{
	// do_sched_yield();
	// {
		// struct rq_flags rf;
		// struct rq *rq;

		// rq = this_rq_lock_irq(&rf);

		// schedstat_inc(rq->yld_count);
		// current->sched_class->yield_task(rq);

		preempt_disable();
		// rq_unlock_irq(rq, &rf);
		sched_preempt_enable_no_resched();

		schedule();
	// }
	return 0;
}


/*======================================================================================*
 *									special functions									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE2(reboot, unsigned int, cmd, void *, arg)
{
	color_printk(GREEN,BLACK,"sys_reboot\n");
	switch(cmd)
	{
		case SYSTEM_REBOOT:
			outb(0xFE, 0x64);
			break;

		case SYSTEM_POWEROFF:
			color_printk(RED,BLACK, "sys_reboot cmd SYSTEM_POWEROFF\n");
			break;

		default:
			color_printk(RED,BLACK, "sys_reboot cmd ERROR!\n");
			break;
	}
	return 0;
}



/*======================================================================================*
 *									temp dummy syscall									*
 *======================================================================================*/
MYOS_SYSCALL_DEFINE3(mprotect, ulong, start, size_t, len, ulong, prot)
{
	ALERT_DUMMY_SYSCALL(mprotect, IF_ALERT_DUMMY_SYSCALL);

	return 0;
}

/**
 *  sys_rt_sigprocmask - change the list of currently blocked signals
 *  @how: whether to add, remove, or set signals
 *  @nset: stores pending signals
 *  @oset: previous value of signal mask if non-null
 *  @sigsetsize: size of sigset_t type
 */
MYOS_SYSCALL_DEFINE4(rt_sigprocmask, int, how, sigset_t __user *, nset, sigset_t __user *, oset, size_t, sigsetsize)
{
	ALERT_DUMMY_SYSCALL(rt_sigprocmask, IF_ALERT_DUMMY_SYSCALL);

	return 0;
}


MYOS_SYSCALL_DEFINE0(getpid)
{
	// return current->pid;
	pid_s *pid_struct = get_task_pid(current, PIDTYPE_PID);
	return pid_vnr(pid_struct);
}

MYOS_SYSCALL_DEFINE4(wait4, pid_t, pid, int *, start_addr,
		int, options, void *, rusage)
{

}

MYOS_SYSCALL_DEFINE0(getppid)
{
	// return current->parent->pid;
	pid_s *pid_struct = get_task_pid(current->parent, PIDTYPE_PID);
	return pid_vnr(pid_struct);
}

// MYOS_SYSCALL_DEFINE0(getuid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getuid ---\n");

// 	return 1000;
// }

// MYOS_SYSCALL_DEFINE0(geteuid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- geteuid ---\n");

// 	return 1000;
// }

// MYOS_SYSCALL_DEFINE0(getgid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getgid ---\n");

// 	return 1000;
// }

// MYOS_SYSCALL_DEFINE0(getegid)
// {
// 	pr_alert("\t!!! Dummy Syscall --- getegid ---\n");

// 	return 1000;
// }

MYOS_SYSCALL_DEFINE2(arch_prctl, int, option, unsigned long, arg2)
{
	long ret;

	ret = do_arch_prctl_64(current, option, arg2);
	// if (ret == -EINVAL)
	// 	ret = do_arch_prctl_common(option, arg2);

	return ret;
}

MYOS_SYSCALL_DEFINE1(set_tid_address, int *, tidptr)
{
	ALERT_DUMMY_SYSCALL(set_tid_address, IF_ALERT_DUMMY_SYSCALL);

	// current->clear_child_tid = tidptr;

	// return task_pid_vnr(current);
	return 0;
}

// MYOS_SYSCALL_DEFINE2(set_robust_list, void *, head, size_t, len)
// {
// 	pr_alert("\t!!! Dummy Syscall --- set_robust_list ---\n");

// 	return 0;
// }

// MYOS_SYSCALL_DEFINE4(rseq, void *, rseq, u32, rseq_len, int, flags, u32, sig)
// {
// 	pr_alert("\t!!! Dummy Syscall --- set_robust_list ---\n");

// 	return 0;
// }

