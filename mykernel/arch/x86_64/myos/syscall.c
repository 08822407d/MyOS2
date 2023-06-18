#include <linux/sched/task.h>
#include <linux/kernel/slab.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/syscalls.h>
#include <linux/fs/file.h>
#include <linux/fs/namei.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/myos_sysreboot.h>
#include <asm/syscall.h>
#include <asm/setup.h>
#include <asm/io.h>

#include <obsolete/printk.h>
#include <obsolete/archconst.h>
#include <obsolete/arch_proto.h>

MYOS_SYSCALL_DEFINE0(no_syscall)
{
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
}

MYOS_SYSCALL_DEFINE1(myos_putstring, char *, string)
{
	// __MAP(3,__SC_CAST, const char *, filename, int, flags, umode_t, mode);
	// __X64_SYS_STUBx(1, myos_putstring, char *, string);
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

//	color_printk(GREEN,BLACK,"sys_close:%d\n",fd);
	if(fd < 0 || fd >= curr->files->fd_count)
		return -EBADF;

	fp = curr->files->fd_array[fd];
	if(fp->f_op && fp->f_op->close)
		fp->f_op->close(fp->f_path.dentry->d_inode, fp);

	kfree(fp);
	curr->files->fd_array[fd] = NULL;

	return 0;
}

MYOS_SYSCALL_DEFINE3(read, unsigned int, fd,
		char *,buf, size_t, count) {
	return ksys_read(fd, buf, count);
}

MYOS_SYSCALL_DEFINE3(write, unsigned int, fd,
		const char, *buf, size_t, count)
{
	task_s * curr = current;
	file_s * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_write:%d\n",fd);
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

//	color_printk(GREEN,BLACK,"sys_lseek:%d\n",filds);
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

long sys_execve(const char *filename,
		const char *const __user *argv,
		const char *const __user *envp);
long myos_do_execve(const char *filename, const char *const *argv,
				const char *const *envp)
{
	char * pathname = NULL;
	long pathlen = 0;
	long error = 0;
	pt_regs_s * curr_context = (pt_regs_s *)current->stack - 1;

	// color_printk(GREEN,BLACK,"sys_execve\n");
	pathname = (char *)kzalloc(CONST_4K, GFP_KERNEL);
	if(pathname == NULL)
		return -ENOMEM;
	pathlen = strnlen_user((char *)curr_context->di, CONST_4K);
	if(pathlen <= 0)
	{
		kfree(pathname);
		return -EFAULT;
	}
	else if(pathlen >= CONST_4K)
	{
		kfree(pathname);
		return -ENAMETOOLONG;
	}
	strncpy_from_user(pathname, (void *)curr_context->di, pathlen);
	
	error = sys_execve(pathname, (const char *const *)curr_context->si, NULL);

	kfree(pathname);
	return error;
}

MYOS_SYSCALL_DEFINE1(exit, int, error_code)
{
	return do_exit(error_code);
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
MYOS_SYSCALL_DEFINE1(sbrk, unsigned long, brk)
{
	virt_addr_t new_brk = round_up(brk, PAGE_SIZE);

//	color_printk(GREEN,BLACK,"sys_brk\n");
//	color_printk(RED,BLACK,"brk:%#018lx,new_brk:%#018lx,current->mm->end_brk:%#018lx\n",brk,new_brk,current->mm->end_brk);
	if(new_brk == 0)
		return (virt_addr_t)current->mm->start_brk;
	else if(new_brk < current->mm->brk)	//release  brk space
		return 0;	
	else
		new_brk = do_brk(current->mm->brk, new_brk - current->mm->brk);	//expand brk space

	current->mm->brk = new_brk;
	return new_brk;
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