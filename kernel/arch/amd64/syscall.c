#include <sys/errno.h>

#include <stdio.h>
#include <string.h>
#include <lib/fcntl.h>

#include "include/archconst.h"
#include "include/arch_proto.h"
#include "include/syscall.h"

#include "../../include/proto.h"
#include "../../include/printk.h"


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
unsigned long sys_open(char *filename, int flags)
{
	char * path = NULL;
	long pathlen = 0;
	long error = 0;
	dirent_s * dentry = NULL;
	file_s * fp = NULL;
	file_s ** f = NULL;
	int fd = -1;
	int i;

//	color_printk(GREEN,BLACK,"sys_open\n");
	path = (char *)kmalloc(CONST_4K);
	if(path == NULL)
		return -ENOMEM;
	memset(path, 0, CONST_4K);
	pathlen = strnlen_user(filename, CONST_4K);
	if(pathlen <= 0)
	{
		kfree(path);
		return -EFAULT;
	}	
	else if(pathlen >= CONST_4K)
	{
		kfree(path);
		return -ENAMETOOLONG;
	}
	strncpy_from_user(filename, path, pathlen);

	dentry = path_walk(path, 0);
	kfree(path);

	if(dentry == NULL)
		return -ENOENT;

	if((flags & O_DIRECTORY) && (dentry->dir_inode->attribute != FS_ATTR_DIR))
		return -ENOTDIR;
	if(!(flags & O_DIRECTORY) && (dentry->dir_inode->attribute == FS_ATTR_DIR))
		return -EISDIR;

	fp = (file_s *)kmalloc(sizeof(file_s));
	memset(fp, 0, sizeof(struct file));
	fp->dentry = dentry;
	fp->mode = flags;

	// if(dentry->dir_inode->attribute & FS_ATTR_DEVICE)
	// 	fp->f_ops = &tty_fops;	//////	find device file operation function
	// else
	fp->f_ops = dentry->dir_inode->f_ops;
	if(fp->f_ops && fp->f_ops->open)
		error = fp->f_ops->open(dentry->dir_inode,fp);
	if(error != 1)
	{
		kfree(fp);
		return -EFAULT;
	}

	if(fp->mode & O_TRUNC)
	{
		fp->dentry->dir_inode->file_size = 0;
	}
	if(fp->mode & O_APPEND)
	{
		fp->position = fp->dentry->dir_inode->file_size;
	}

	f = curr_tsk->fps;
	for(i = 0;i < MAX_FILE_NR;i++)
		if(f[i] == NULL)
		{
			fd = i;
			break;
		}
	if(i == MAX_FILE_NR)
	{
		kfree(fp);
		//// reclaim struct index_node & struct dir_entry
		return -EMFILE;
	}
	f[fd] = fp;

	return fd;
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
		fp->f_ops->close(fp->dentry->dir_inode,fp);

	kfree(fp);
	curr->fps[fd] = NULL;

	return 0;
}

unsigned long sys_read(int fd,void * buf, long count)
{
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_read:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = curr_tsk->fps[fd];
	if(fp->f_ops && fp->f_ops->read)
		ret = fp->f_ops->read(fp,buf,count,&fp->position);
	return ret;
}

unsigned long sys_write(int fd,void * buf,long count)
{
	struct file * fp = NULL;
	unsigned long ret = 0;

//	color_printk(GREEN,BLACK,"sys_write:%d\n",fd);
	if(fd < 0 || fd >= MAX_FILE_NR)
		return -EBADF;
	if(count < 0)
		return -EINVAL;

	fp = curr_tsk->fps[fd];
	if(fp->f_ops && fp->f_ops->write)
		ret = fp->f_ops->write(fp, buf, count, &fp->position);
	return ret;
}


unsigned long sys_lseek(int filds,long offset,int whence)
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
		ret = fp->f_ops->lseek(fp,offset,whence);
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
	strncpy_from_user((char *)curr_context->rdi, pathname, pathlen);
	
	error = do_execve(curr_context, pathname, (char **)curr_context->rsi, NULL);

	kfree(pathname);
	return error;
}

unsigned long sys_exit(int exit_code)
{
	return do_exit(exit_code);
}

void exit_mm(task_s * new_tsk);
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
unsigned long sys_reboot(unsigned long cmd,void * arg)
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