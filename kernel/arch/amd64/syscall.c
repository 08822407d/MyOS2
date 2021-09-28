#include <lib/string.h>
#include <sys/errno.h>

#include "include/archconst.h"
#include "include/arch_proto.h"
#include "include/syscall.h"

#include "../../include/proto.h"
#include "../../include/printk.h"


unsigned long sys_fork()
{
	stack_frame_s * curr_context = (stack_frame_s *)curr_tsk->arch_struct.tss_rsp0 - 1;
	// color_printk(GREEN,BLACK,"sys_fork\n");
	return do_fork(curr_context, 0, curr_context->rsp, 0);	
}

unsigned long sys_vfork()
{
	stack_frame_s * curr_context = (stack_frame_s *)curr_tsk->arch_struct.tss_rsp0 - 1;
	// color_printk(GREEN,BLACK,"sys_vfork\n");
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
	// color_printk(GREEN,BLACK,"sys_exit\n");
	return do_exit(exit_code);
}