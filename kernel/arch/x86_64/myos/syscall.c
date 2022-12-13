#include <linux/kernel/slab.h>
#include <linux/kernel/fcntl.h>
#include <linux/fs/file.h>
#include <linux/fs/namei.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/kernel/myos_sysreboot.h>
#include <asm/syscall.h>
#include <asm/setup.h>
#include <asm/io.h>

#include <obsolete/printk.h>
#include <obsolete/wait_queue.h>

#include <obsolete/archconst.h>
#include <obsolete/arch_proto.h>

long myos_no_system_call(void)
{
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
}

long myos_sys_putstring(char *string)
{
	color_printk(WHITE, BLACK, string);
	return 0;
}

/*==============================================================================================*
 *										file operations											*
 *==============================================================================================*/
long sys_open(const char *filename, int flags, umode_t mode)
{
	return do_sys_open(0, filename, flags, mode);
}

long sys_close(unsigned int fd)
{
	task_s * curr = current;

	file_s * fp = NULL;

//	color_printk(GREEN,BLACK,"sys_close:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;

	fp = curr->fps[fd];
	if(fp->f_op && fp->f_op->close)
		fp->f_op->close(fp->f_path.dentry->d_inode, fp);

	kfree(fp);
	curr->fps[fd] = NULL;

	return 0;
}

long sys_read(unsigned int fd, char *buf, size_t count)
{
	task_s * curr = current;
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_read:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = current->fps[fd];
	if(fp->f_op && fp->f_op->read)
		ret = fp->f_op->read(fp, buf, count, &fp->f_pos);
	return ret;
}

long sys_write(unsigned int fd, const char *buf, size_t count)
{
	task_s * curr = current;
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_write:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = current->fps[fd];
	if(fp->f_op && fp->f_op->write)
		ret = fp->f_op->write(fp, buf, count, &fp->f_pos);
	return ret;
}

long sys_lseek(unsigned int fd, loff_t offset, unsigned int whence)
{
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_lseek:%d\n",filds);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	fp = current->fps[fd];
	if(fp->f_op && fp->f_op->llseek)
		ret = fp->f_op->llseek(fp, offset, whence);
	return ret;
}

long sys_fork()
{
	stack_frame_s * curr_context = (stack_frame_s *)current->arch_struct.tss_rsp0 - 1;
	return do_fork(curr_context, 0, curr_context->rsp, 0, NULL, NULL);	
}

long sys_vfork()
{
	stack_frame_s * curr_context = (stack_frame_s *)current->arch_struct.tss_rsp0 - 1;
	return do_fork(curr_context, CLONE_VM | CLONE_FS | CLONE_SIGHAND, curr_context->rsp, 0, NULL, NULL);
}

long sys_execve(const char *filename, const char *const *argv,
				const char *const *envp)
{
	char * pathname = NULL;
	long pathlen = 0;
	long error = 0;
	stack_frame_s * curr_context = (stack_frame_s *)current->arch_struct.tss_rsp0 -1;

	// color_printk(GREEN,BLACK,"sys_execve\n");
	pathname = (char *)kzalloc(CONST_4K, GFP_KERNEL);
	if(pathname == NULL)
		return -ENOMEM;
	pathlen = strnlen_user((char *)curr_context->rdi, CONST_4K);
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
	strncpy_from_user(pathname, (void *)curr_context->rdi, pathlen);
	
	error = do_execve(curr_context, pathname, (char **)curr_context->rsi, NULL);

	kfree(pathname);
	return error;
}

long sys_exit(int error_code)
{
	return do_exit(error_code);
}

int exit_mm(task_s * new_tsk);
long sys_wait4(pid_t pid, int *start_addr, int options, void *rusage)
{
	long retval = 0;
	task_s * child = NULL;
	task_s * tsk = NULL;

	// color_printk(GREEN,BLACK,"sys_wait4\n");
	List_s * child_lp;
	for (child_lp = current->children.header.next;
			child_lp != &current->children.header;
			child_lp = child_lp->next)
	{
		task_s * child_p = container_of(child_lp, task_s, sibling);
		if (child_p->pid == pid)
		{
			child = child_p;
			break;
		}
	}

	if(child == NULL)
		return -ECHILD;
	if(options != 0)
		return -EINVAL;

	if(child->__state != EXIT_ZOMBIE)
	{
		wq_sleep_on_intrable(&child->wait_childexit);
	}
	copy_to_user(start_addr, &child->exit_code, sizeof(int));
	exit_mm(child);
	list_delete(&child->sibling);
	kfree(child);
	return retval;
}

/*==============================================================================================*
 *									user memory manage											*
 *==============================================================================================*/
virt_addr_t sys_sbrk(unsigned long brk)
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
long sys_getpid()
{
	return current->pid;
}

long sys_getppid()
{
	return current->parent->pid;
}


/*==============================================================================================*
 *									special functions											*
 *==============================================================================================*/
long sys_reboot(unsigned int cmd, void *arg)
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