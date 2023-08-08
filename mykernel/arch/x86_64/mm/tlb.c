// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init/init.h>

#include <linux/mm/mm.h>
#include <linux/kernel/spinlock.h>
// #include <linux/smp.h>
// #include <linux/interrupt.h>
// #include <linux/export.h>
#include <linux/kernel/cpu.h>
// #include <linux/debugfs.h>
// #include <linux/sched/smt.h>

// #include <asm/tlbflush.h>
#include <asm/mmu_context.h>
// #include <asm/nospec-branch.h>
#include <asm/cache.h>
// #include <asm/cacheflush.h>
#include <asm/apic.h>
// #include <asm/perf_event.h>

// #include "mm_internal.h"


static void load_new_mm_cr3(pgd_t *pgdir, u16 new_asid, bool need_flush)
{
	unsigned long new_mm_cr3;
	new_mm_cr3 = (unsigned long)myos_virt2phys((virt_addr_t)pgdir);

	// if (need_flush) {
	// 	invalidate_user_asid(new_asid);
	// 	new_mm_cr3 = build_cr3(pgdir, new_asid);
	// } else {
	// 	new_mm_cr3 = build_cr3_noflush(pgdir, new_asid);
	// }

	/*
	 * Caution: many callers of this function expect
	 * that load_cr3() is serializing and orders TLB
	 * fills with respect to the mm_cpumask writes.
	 */
	write_cr3(new_mm_cr3);
}


void switch_mm(mm_s *prev, mm_s *next, task_s*tsk)
{
	unsigned long flags;

	local_irq_save(flags);
	switch_mm_irqs_off(prev, next, tsk);
	local_irq_restore(flags);
}


void switch_mm_irqs_off(mm_s *prev, mm_s *next, task_s *tsk)
{
	// struct mm_struct *real_prev = this_cpu_read(cpu_tlbstate.loaded_mm);
	// u16 prev_asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);
	// bool was_lazy = this_cpu_read(cpu_tlbstate_shared.is_lazy);
	// unsigned cpu = smp_processor_id();
	// u64 next_tlb_gen;
	bool need_flush = false;
	u16 new_asid = 0;

	// /*
	//  * NB: The scheduler will call us with prev == next when switching
	//  * from lazy TLB mode to normal mode if active_mm isn't changing.
	//  * When this happens, we don't assume that CR3 (and hence
	//  * cpu_tlbstate.loaded_mm) matches next.
	//  *
	//  * NB: leave_mm() calls us with prev == NULL and tsk == NULL.
	//  */

	// /* We don't want flush_tlb_func() to run concurrently with us. */
	// if (IS_ENABLED(CONFIG_PROVE_LOCKING))
	// 	WARN_ON_ONCE(!irqs_disabled());

	// /*
	//  * Verify that CR3 is what we think it is.  This will catch
	//  * hypothetical buggy code that directly switches to swapper_pg_dir
	//  * without going through leave_mm() / switch_mm_irqs_off() or that
	//  * does something like write_cr3(read_cr3_pa()).
	//  *
	//  * Only do this check if CONFIG_DEBUG_VM=y because __read_cr3()
	//  * isn't free.
	//  */
// #ifdef CONFIG_DEBUG_VM
	// if (WARN_ON_ONCE(__read_cr3() != build_cr3(real_prev->pgd, prev_asid))) {
	// 	/*
	// 	 * If we were to BUG here, we'd be very likely to kill
	// 	 * the system so hard that we don't see the call trace.
	// 	 * Try to recover instead by ignoring the error and doing
	// 	 * a global flush to minimize the chance of corruption.
	// 	 *
	// 	 * (This is far from being a fully correct recovery.
	// 	 *  Architecturally, the CPU could prefetch something
	// 	 *  back into an incorrect ASID slot and leave it there
	// 	 *  to cause trouble down the road.  It's better than
	// 	 *  nothing, though.)
	// 	 */
	// 	__flush_tlb_all();
	// }
// #endif
	// if (was_lazy)
	// 	this_cpu_write(cpu_tlbstate_shared.is_lazy, false);

	// /*
	//  * The membarrier system call requires a full memory barrier and
	//  * core serialization before returning to user-space, after
	//  * storing to rq->curr, when changing mm.  This is because
	//  * membarrier() sends IPIs to all CPUs that are in the target mm
	//  * to make them issue memory barriers.  However, if another CPU
	//  * switches to/from the target mm concurrently with
	//  * membarrier(), it can cause that CPU not to receive an IPI
	//  * when it really should issue a memory barrier.  Writing to CR3
	//  * provides that full memory barrier and core serializing
	//  * instruction.
	//  */
	// if (real_prev == next) {
	// 	VM_WARN_ON(this_cpu_read(cpu_tlbstate.ctxs[prev_asid].ctx_id) !=
	// 		   next->context.ctx_id);

	// 	/*
	// 	 * Even in lazy TLB mode, the CPU should stay set in the
	// 	 * mm_cpumask. The TLB shootdown code can figure out from
	// 	 * cpu_tlbstate_shared.is_lazy whether or not to send an IPI.
	// 	 */
	// 	if (WARN_ON_ONCE(real_prev != &init_mm &&
	// 			 !cpumask_test_cpu(cpu, mm_cpumask(next))))
	// 		cpumask_set_cpu(cpu, mm_cpumask(next));

	// 	/*
	// 	 * If the CPU is not in lazy TLB mode, we are just switching
	// 	 * from one thread in a process to another thread in the same
	// 	 * process. No TLB flush required.
	// 	 */
	// 	if (!was_lazy)
	// 		return;

	// 	/*
	// 	 * Read the tlb_gen to check whether a flush is needed.
	// 	 * If the TLB is up to date, just use it.
	// 	 * The barrier synchronizes with the tlb_gen increment in
	// 	 * the TLB shootdown code.
	// 	 */
	// 	smp_mb();
	// 	next_tlb_gen = atomic64_read(&next->context.tlb_gen);
	// 	if (this_cpu_read(cpu_tlbstate.ctxs[prev_asid].tlb_gen) ==
	// 			next_tlb_gen)
	// 		return;

	// 	/*
	// 	 * TLB contents went out of date while we were in lazy
	// 	 * mode. Fall through to the TLB switching code below.
	// 	 */
	// 	new_asid = prev_asid;
	// 	need_flush = true;
	// } else {
	// 	/*
	// 	 * Apply process to process speculation vulnerability
	// 	 * mitigations if applicable.
	// 	 */
	// 	cond_mitigation(tsk);

	// 	/*
	// 	 * Stop remote flushes for the previous mm.
	// 	 * Skip kernel threads; we never send init_mm TLB flushing IPIs,
	// 	 * but the bitmap manipulation can cause cache line contention.
	// 	 */
	// 	if (real_prev != &init_mm) {
	// 		VM_WARN_ON_ONCE(!cpumask_test_cpu(cpu,
	// 					mm_cpumask(real_prev)));
	// 		cpumask_clear_cpu(cpu, mm_cpumask(real_prev));
	// 	}

	// 	/*
	// 	 * Start remote flushes and then read tlb_gen.
	// 	 */
	// 	if (next != &init_mm)
	// 		cpumask_set_cpu(cpu, mm_cpumask(next));
	// 	next_tlb_gen = atomic64_read(&next->context.tlb_gen);

	// 	choose_new_asid(next, next_tlb_gen, &new_asid, &need_flush);

	// 	/* Let nmi_uaccess_okay() know that we're changing CR3. */
	// 	this_cpu_write(cpu_tlbstate.loaded_mm, LOADED_MM_SWITCHING);
	// 	barrier();
	// }

	if (need_flush) {
		// this_cpu_write(cpu_tlbstate.ctxs[new_asid].ctx_id, next->context.ctx_id);
		// this_cpu_write(cpu_tlbstate.ctxs[new_asid].tlb_gen, next_tlb_gen);
		load_new_mm_cr3(next->pgd, new_asid, true);

		// trace_tlb_flush(TLB_FLUSH_ON_TASK_SWITCH, TLB_FLUSH_ALL);
	} else {
		/* The new ASID is already up to date. */
		load_new_mm_cr3(next->pgd, new_asid, false);

		// trace_tlb_flush(TLB_FLUSH_ON_TASK_SWITCH, 0);
	}

	// /* Make sure we write CR3 before loaded_mm. */
	// barrier();

	// this_cpu_write(cpu_tlbstate.loaded_mm, next);
	// this_cpu_write(cpu_tlbstate.loaded_mm_asid, new_asid);

	// if (next != real_prev) {
	// 	cr4_update_pce_mm(next);
	// 	switch_ldt(real_prev, next);
	// }
}