#include <linux/kernel/fcntl.h>
#include <uapi/sysreboot.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <include/proto.h>
#include <include/printk.h>
#include <include/fs/file.h>
#include <include/fs/namei.h>

#include "include/archconst.h"
#include "include/arch_proto.h"
#include "include/syscall.h"

unsigned long no_system_call(void)
{
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
}

unsigned long sys_putstring(char *string)
{
	color_printk(WHITE, BLACK, string);
	return 0;
}

/*==============================================================================================*
 *										file operations											*
 *==============================================================================================*/
unsigned long sys_open(char *filename, int flags, umode_t mode)
{
	return do_sys_open(0, filename, flags, mode);
}

unsigned long sys_close(int fd)
{
	task_s * curr = curr_tsk;

	struct file * fp = NULL;

//	color_printk(GREEN,BLACK,"sys_close:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;

	fp = curr->fps[fd];
	if(fp->f_ops && fp->f_ops->close)
		fp->f_ops->close(fp->dentry->d_inode, fp);

	kfree(fp);
	curr->fps[fd] = NULL;

	return 0;
}

unsigned long sys_read(int fd, void * buf, long count)
{
	task_s * curr = curr_tsk;
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_read:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = curr_tsk->fps[fd];
	if(fp->f_ops && fp->f_ops->read)
		ret = fp->f_ops->read(fp, buf, count, &fp->f_pos);
	return ret;
}

unsigned long sys_write(int fd, void *buf, long count)
{
	task_s * curr = curr_tsk;
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_write:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = curr_tsk->fps[fd];
	if(fp->f_ops && fp->f_ops->write)
		ret = fp->f_ops->write(fp, buf, count, &fp->f_pos);
	return ret;
}


unsigned long sys_lseek(int filds, long offset, int whence)
{
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_lseek:%d\n",filds);
	if(filds < 0 || filds >= MAX_FILE_NR)
		return -EBADF;
	if(whence < 0 || whence >= SEEK_MAX)
		return -EINVAL;

	fp = curr_tsk->fps[filds];
	if(fp->f_ops && fp->f_ops->lseek)
		ret = fp->f_ops->lseek(fp, offset, whence);
	return ret;
}

unsigned long sys_fork()
{
	stack_frame_s * curr_context = (stack_frame_s *)curr_tsk->arch_struct.tss_rsp0 - 1;
	return do_fork(curr_context, 0, curr_context->rsp, 0);	
}

unsigned long sys_vfork()
{
	stack_frame_s * curr_context = (stack_frame_s *)curr_tsk->arch_struct.tss_rsp0 - 1;
	return do_fork(curr_context, CLONE_VM | CLONE_FS | CLONE_SIGNAL, curr_context->rsp, 0);
}

unsigned long sys_execve()
{
	char * pathname = NULL;
	long pathlen = 0;
	long error = 0;
	stack_frame_s * curr_context = (stack_frame_s *)curr_tsk->arch_struct.tss_rsp0 -1;

	// color_printk(GREEN,BLACK,"sys_execve\n");
	pathname = (char *)kmalloc(CONST_4K);
	if(pathname == NULL)
		return -ENOMEM;
	memset(pathname, 0, CONST_4K);
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

unsigned long sys_exit(int exit_code)
{
	return do_exit(exit_code);
}

errno_t exit_mm(task_s * new_tsk);
unsigned long sys_wait4(unsigned long pid, int *status, int options, void *rusage)
{
	long retval = 0;
	task_s * child = NULL;
	task_s * tsk = NULL;

	// color_printk(GREEN,BLACK,"sys_wait4\n");
	List_s * child_lp;
	for (child_lp = curr_tsk->child_lhdr.header.next;
			child_lp != &curr_tsk->child_lhdr.header;
			child_lp = child_lp->next)
	{
		task_s * child_p = container_of(child_lp, task_s, child_list);
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

	if(child->state != PS_ZOMBIE)
	{
		wq_sleep_on_intrable(&child->wait_childexit);
	}
	copy_to_user(status, &child->exit_code, sizeof(int));
	exit_mm(child);
	list_delete(&child->child_list);
	kfree(child);
	return retval;
}

/*==============================================================================================*
 *									user memory manage											*
 *==============================================================================================*/
virt_addr_t sys_brk(const void * brk)
{
	unsigned long new_brk = PAGE_ROUND_UP((reg_t)brk);

//	color_printk(GREEN,BLACK,"sys_brk\n");
//	color_printk(RED,BLACK,"brk:%#018lx,new_brk:%#018lx,current->mm->end_brk:%#018lx\n",brk,new_brk,current->mm->end_brk);
	if(new_brk == 0)
		return (virt_addr_t)curr_tsk->mm_struct->start_brk;
	else if(new_brk < curr_tsk->mm_struct->end_brk)	//release  brk space
		return 0;	
	else
		new_brk = do_brk(curr_tsk->mm_struct->end_brk, new_brk - curr_tsk->mm_struct->end_brk);	//expand brk space

	curr_tsk->mm_struct->end_brk = new_brk;
	return (virt_addr_t)new_brk;
}


/*==============================================================================================*
 *									get task infomation											*
 *==============================================================================================*/
unsigned long sys_getpid()
{
	return curr_tsk->pid;
}

unsigned long sys_getppid()
{
	return curr_tsk->parent->pid;
}


/*==============================================================================================*
 *									special functions											*
 *==============================================================================================*/
unsigned long sys_reboot(unsigned long cmd, void * arg)
{
	color_printk(GREEN,BLACK,"sys_reboot\n");
	switch(cmd)
	{
		case SYSTEM_REBOOT:
			outb(0x64, 0xFE);
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