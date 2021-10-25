/*
 * libc/errno/errno.c
 */

#include <errno.h>
// #include <xboot/task.h>

volatile int * __task_errno_location(void)
{
	// return &(task_self()->__errno);
}
