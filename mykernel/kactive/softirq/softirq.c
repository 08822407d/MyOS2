#define SOFTIRQ_DEFINATION
#include "softirq.h"

#include <linux/init/init.h>


const char * const softirq_to_name[NR_SOFTIRQS] = {
	"HI", "TIMER", "NET_TX", "NET_RX", "BLOCK", "IRQ_POLL",
	"TASKLET", "SCHED", "HRTIMER", "RCU"
};

/*
 * we cannot loop indefinitely here to avoid userspace starvation,
 * but we also don't want to introduce a worst case 1/HZ latency
 * to the pending events, so lets the scheduler to balance
 * the softirq load for us.
 */
static void wakeup_softirqd(void) {
	/* Interrupts are disabled: no need to stop preemption */
	task_s *tsk = __this_cpu_read(ksoftirqd);
	if (tsk)
		wake_up_process(tsk);
}


void __raise_softirq_irqoff(uint nr)
{
	// lockdep_assert_irqs_disabled();
	// trace_softirq_raise(nr);
	or_softirq_pending(1UL << nr);
}

/*
 * This function must run with irqs disabled!
 */
inline void
raise_softirq_irqoff(uint nr) {
	__raise_softirq_irqoff(nr);

	// /*
	//  * If we're in an interrupt or softirq, we're done
	//  * (this also catches softirq-disabled code). We will
	//  * actually run the softirq once we return from
	//  * the irq or softirq.
	//  *
	//  * Otherwise we wake up ksoftirqd to make sure we
	//  * schedule the softirq soon.
	//  */
	// if (!in_interrupt() && should_wake_ksoftirqd())
	// 	wakeup_softirqd();
}

void raise_softirq(uint nr)
{
	ulong flags;

	local_irq_save(flags);
	raise_softirq_irqoff(nr);
	local_irq_restore(flags);
}





void __init softirq_init(void)
{
	int cpu;

	// for_each_possible_cpu(cpu) {
	// 	per_cpu(tasklet_vec, cpu).tail =
	// 		&per_cpu(tasklet_vec, cpu).head;
	// 	per_cpu(tasklet_hi_vec, cpu).tail =
	// 		&per_cpu(tasklet_hi_vec, cpu).head;
	// }

	// open_softirq(TASKLET_SOFTIRQ, tasklet_action);
	// open_softirq(HI_SOFTIRQ, tasklet_hi_action);
}