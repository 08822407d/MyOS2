#include <linux/kernel/kbuild.h>
// #include <linux/kernel/ptrace.h>
#include <sched/task/task_types.h>


loff_t TASK_threadsp = 0;

void __used asm_offsets(void)
{
	TASK_threadsp = offsetof(task_s, thread.sp);
}
