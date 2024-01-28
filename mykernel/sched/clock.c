#include <linux/kernel/kernel.h>
#include <linux/kernel/compiler.h>

noinstr u64 local_clock(void)
{
	// u64 clock;

	// if (static_branch_likely(&__sched_clock_stable))
	// 	return sched_clock() + __sched_clock_offset;

	// if (!static_branch_likely(&sched_clock_running))
		// return sched_clock();

	// preempt_disable_notrace();
	// clock = sched_clock_local(this_scd());
	// preempt_enable_notrace();

	// return clock;
}