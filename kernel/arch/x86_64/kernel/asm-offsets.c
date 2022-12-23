#include <linux/kernel/sched.h>

#include <linux/kernel/ptrace.h>


loff_t TASK_threadsp = 0;

void __used asm_offsets(void)
{
	TASK_threadsp = offsetof(task_s, thread.sp);
}