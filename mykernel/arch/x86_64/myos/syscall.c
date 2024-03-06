#include <linux/sched/task.h>
// #include <linux/kernel/slab.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/file.h>
#include <linux/kernel/mm.h>
#include <linux/fs/namei.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/myos_sysreboot.h>
#include <asm/syscall.h>
#include <asm/setup.h>
#include <asm/insns.h>
#include <asm/signal.h>

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
	if(fp->f_op && fp->f_op->close)
		fp->f_op->close(fp->f_path.dentry->d_inode, fp);

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
	unsigned long ret = 0;

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
	unsigned long ret = 0;

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

extern int myos_exit_mm(task_s * new_tsk);
MYOS_SYSCALL_DEFINE4(wait4, pid_t, pid, int *, start_addr,
		int, options, void *, rusage)
{
	// long retval = 0;
	// task_s * child = NULL;
	// task_s * tsk = NULL;

	// // color_printk(GREEN,BLACK,"sys_wait4\n");
	// List_s * child_lp;
	// for (child_lp = current->children.header.next;
	// 		child_lp != &current->children.header;
	// 		child_lp = child_lp->next)
	// {
	// 	task_s * child_p = container_of(child_lp, task_s, sibling);
	// 	if (child_p->pid == pid)
	// 	{
	// 		child = child_p;
	// 		break;
	// 	}
	// }

	// if(child == NULL)
	// 	return -ECHILD;
	// if(options != 0)
	// 	return -EINVAL;

	// if(child->__state != EXIT_ZOMBIE)
	// {
	// 	wq_sleep_on_intrable(&child->wait_childexit);
	// }
	// copy_to_user(start_addr, &child->exit_code, sizeof(int));
	// myos_exit_mm(child);
	// list_delete(&child->sibling);
	// kfree(child);
	// return retval;
}

/*==============================================================================================*
 *									user memory manage											*
 *==============================================================================================*/
MYOS_SYSCALL_DEFINE1(brk, unsigned long, brk)
{
	unsigned long newbrk, oldbrk, origbrk;
	mm_s *mm = current->mm;
	vma_s *next;
	unsigned long min_brk;
	bool populate;
	bool downgraded = false;
	
	origbrk = mm->brk;
	min_brk = mm->start_brk;

	if (brk < min_brk)
		goto out;

	newbrk = PAGE_ALIGN(brk);
	oldbrk = PAGE_ALIGN(mm->brk);
	if (oldbrk == newbrk) {
		mm->brk = brk;
		goto success;
	}

	/*
	 * Always allow shrinking brk.
	 * __do_munmap() may downgrade mmap_lock to read.
	 */
	if (brk <= mm->brk) {
		int ret;

		/*
		 * mm->brk must to be protected by write mmap_lock so update it
		 * before downgrading mmap_lock. When __do_munmap() fails,
		 * mm->brk will be restored from origbrk.
		 */
		mm->brk = brk;
		ret = __do_munmap(mm, newbrk, oldbrk-newbrk, true);
		if (ret < 0) {
			mm->brk = origbrk;
			goto out;
		} else if (ret == 1) {
			downgraded = true;
		}
		goto success;
	}

	/* Check against existing mmap mappings. */
	next = myos_find_vma(mm, oldbrk);
	// if (next && newbrk + PAGE_SIZE > vm_start_gap(next))
	// 	goto out;

	/* Ok, looks good - let it rip. */
	if (do_brk_flags(oldbrk, newbrk-oldbrk, 0) < 0)
		goto out;
	mm->brk = brk;

success:
	// populate = newbrk > oldbrk && (mm->def_flags & VM_LOCKED) != 0;
	// if (downgraded)
	// 	mmap_read_unlock(mm);
	// else
	// 	mmap_write_unlock(mm);
	// userfaultfd_unmap_complete(mm, &uf);
	// if (populate)
	// 	mm_populate(oldbrk, newbrk - oldbrk);
	return brk;

out:
	// mmap_write_unlock(mm);
	return origbrk;
}


/*==============================================================================================*
 *									get task infomation											*
 *==============================================================================================*/
MYOS_SYSCALL_DEFINE0(getpid)
{
	return current->pid;
}

MYOS_SYSCALL_DEFINE0(getppid)
{
	return current->parent->pid;
}


/*==============================================================================================*
 *									special functions											*
 *==============================================================================================*/
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