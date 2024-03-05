// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#include <linux/sched/sched.h>

// #include <trace/events/power.h>


#include <linux/smp/smp.h>


/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
static void do_idle(void)
{
	int cpu = smp_processor_id();

	// /*
	//  * Check if we need to update blocked load
	//  */
	// nohz_run_idle_balance(cpu);

	// /*
	//  * If the arch has a polling bit, we maintain an invariant:
	//  *
	//  * Our polling bit is clear if we're not scheduled (i.e. if rq->curr !=
	//  * rq->idle). This means that, if rq->idle has the polling bit set,
	//  * then setting need_resched is guaranteed to cause the CPU to
	//  * reschedule.
	//  */

	// __current_set_polling();
	// tick_nohz_idle_enter();

	// while (!need_resched()) {
	// 	rmb();

	// 	local_irq_disable();

	// 	if (cpu_is_offline(cpu)) {
	// 		tick_nohz_idle_stop_tick();
	// 		cpuhp_report_idle_dead();
	// 		arch_cpu_idle_dead();
	// 	}

	// 	arch_cpu_idle_enter();
	// 	rcu_nocb_flush_deferred_wakeup();

	// 	/*
	// 	 * In poll mode we reenable interrupts and spin. Also if we
	// 	 * detected in the wakeup from idle path that the tick
	// 	 * broadcast device expired for us, we don't want to go deep
	// 	 * idle as we know that the IPI is going to arrive right away.
	// 	 */
	// 	if (cpu_idle_force_poll || tick_check_broadcast_expired()) {
	// 		tick_nohz_idle_restart_tick();
	// 		cpu_idle_poll();
	// 	} else {
	// 		cpuidle_idle_call();
	// 	}
	// 	arch_cpu_idle_exit();
	// }

	/*
	 * Since we fell out of the loop above, we know TIF_NEED_RESCHED must
	 * be set, propagate it into PREEMPT_NEED_RESCHED.
	 *
	 * This is required because for polling idle loops we will not have had
	 * an IPI to fold the state for us.
	 */
	set_preempt_need_resched();
	// tick_nohz_idle_exit();
	// __current_clr_polling();

	// /*
	//  * We promise to call sched_ttwu_pending() and reschedule if
	//  * need_resched() is set while polling is set. That means that clearing
	//  * polling needs to be visible before doing these things.
	//  */
	// smp_mb__after_atomic();

	// /*
	//  * RCU relies on this call to be done outside of an RCU read-side
	//  * critical section.
	//  */
	// flush_smp_call_function_from_idle();
	preempt_disable();
	schedule_idle();
	preempt_enable_no_resched();

	// if (unlikely(klp_patch_pending(current)))
	// 	klp_update_patch_state(current);
}


// void cpu_startup_entry(enum cpuhp_state state)
void cpu_startup_entry()
{	
	asm volatile("sti");

	// arch_cpu_idle_prepare();
	// cpuhp_online_idle(state);

	preempt_enable();
	while (1)
		do_idle();
}