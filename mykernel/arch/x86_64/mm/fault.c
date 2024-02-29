// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (C) 1995  Linus Torvalds
 *  Copyright (C) 2001, 2002 Andi Kleen, SuSE Labs.
 *  Copyright (C) 2008-2009, Red Hat Inc., Ingo Molnar
 */
#include <linux/kernel/sched.h>		/* test_thread_flag(), ...	*/
#include <linux/sched/task_stack.h>	/* task_stack_*(), ...		*/
#include <linux/debug/kdebug.h>		/* oops_begin/end, ...		*/
// #include <linux/extable.h>		/* search_exception_tables	*/
// #include <linux/mm/memblock.h>		/* max_low_pfn			*/
#include <linux/mm/mm.h>		/* max_low_pfn			*/
// #include <linux/kfence.h>		/* kfence_handle_page_fault	*/
// #include <linux/kprobes.h>		/* NOKPROBE_SYMBOL, ...		*/
// #include <linux/mmiotrace.h>		/* kmmio_handler, ...		*/
// #include <linux/perf_event.h>		/* perf_sw_event		*/
// #include <linux/hugetlb.h>		/* hstate_index_to_shift	*/
#include <linux/kernel/prefetch.h>		/* prefetchw			*/
// #include <linux/context_tracking.h>	/* exception_enter(), ...	*/
#include <linux/kernel/uaccess.h>		/* faulthandler_disabled()	*/
// #include <linux/efi.h>			/* efi_crash_gracefully_on_page_fault()*/
#include <linux/mm/mm_types.h>

// #include <asm/cpufeature.h>		/* boot_cpu_has, ...		*/
#include <asm/traps.h>			/* dotraplinkage, ...		*/
// #include <asm/fixmap.h>			/* VSYSCALL_ADDR		*/
// #include <asm/vsyscall.h>		/* emulate_vsyscall		*/
// #include <asm/vm86.h>			/* struct vm86			*/
#include <asm/mmu_context.h>		/* vma_pkey()			*/
// #include <asm/efi.h>			/* efi_crash_gracefully_on_page_fault()*/
#include <asm/desc.h>			/* store_idt(), ...		*/
// #include <asm/cpu_entry_area.h>		/* exception stack		*/
// #include <asm/pgtable_areas.h>		/* VMALLOC_START, ...		*/
// #include <asm/kvm_para.h>		/* kvm_handle_async_pf		*/
// #include <asm/vdso.h>			/* fixup_vdso_exception()	*/
// #include <asm/irq_stack.h>

// #define CREATE_TRACE_POINTS
// #include <asm/trace/exceptions.h>


#include <linux/kernel/kernel.h>
#include <linux/kernel/slab.h>
#include <asm/idtentry.h>
#include <obsolete/printk.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/arch_proto.h>


DEFINE_SPINLOCK(pgd_lock);
LIST_HDR_S(pgd_list_hdr);


vm_fault_s myos_dump_pagetable(unsigned long address)
{
	vm_fault_s vmf;
	memset(&vmf, 0, sizeof(vm_fault_s));
	pgd_t *base = (pgd_t *)__va(read_cr3_pa());
	pgd_t *pgd = base + pgd_index(address);
	
	vmf.p4d = p4d_ent_offset(pgd, address);
	if (arch_p4d_none(*vmf.p4d))
		goto finish;
	vmf.pud = pud_ent_offset(vmf.p4d, address);
	if (arch_pud_none(*vmf.pud))
		goto finish;
	vmf.pmd = pmd_ent_offset(vmf.pud, address);
	if (arch_pmd_none(*vmf.pmd))
		goto finish;
	vmf.pte = pte_ent_offset(vmf.pmd, address);
	if (arch_pte_none(*vmf.pte))
		goto finish;
	virt_addr_t pg_vaddr = phys_to_virt(PTE_PFN_MASK & vmf.pte->val);
	vmf.page = (virt_to_page(pg_vaddr));
	
finish:
	return vmf;
}


static void
parse_PF_errcode(pt_regs_s *regs, unsigned long cr2, char *buf)
{
	char tempbuf[100];
	unsigned long error_code = (unsigned long)regs->orig_ax;

	memset(tempbuf, 0, 100);
	snprintf(tempbuf, 45, "do_page_fault(14),ERROR_CODE: %#018lx", error_code);
	strcat(buf, tempbuf);

	if(!(error_code & 0x01))
		strcat(buf, "\tPage Not-Present,\n");

	if(error_code & 0x02)
		strcat(buf, "Write Cause Fault, ");
	else
		strcat(buf, "Read Cause Fault, ");

	if(error_code & 0x04)
		strcat(buf, "Fault in user(3)\n");
	else
		strcat(buf, "Fault in supervisor(0,1,2)\n");

	if(error_code & 0x08)
		strcat(buf, ",Reserved Bit Cause Fault\n");

	if(error_code & 0x10)
		strcat(buf, ",Instruction fetch Cause Fault\n");

	memset(tempbuf, 0, 100);
	snprintf(tempbuf, 100, "Code address: %#018lx, CR2:%#018lx\n",
				regs->ip, cr2);
	strcat(buf, tempbuf);
}

static noinline void
myos_bad_area(pt_regs_s *regs, unsigned long error_code, unsigned long address)
{
	char *buf = kmalloc(500, GFP_KERNEL);
	memset(buf, 0, 500);
	parse_PF_errcode(regs, address, buf);
	// if (!(error_code & (ARCH_PF_EC_WR & ~ARCH_PF_EC_P)))
	// {
		color_printk(RED, BLACK, buf);
		while (1);
	// }	
}


bool fault_in_kernel_space(unsigned long address)
{
	/*
	 * On 64-bit systems, the vsyscall page is at an address above
	 * TASK_SIZE_MAX, but is not considered part of the kernel
	 * address space.
	 */
	// if (IS_ENABLED(CONFIG_X86_64) && is_vsyscall_vaddr(address))
	// 	return false;

	return address >= TASK_SIZE_MAX;
}

/*
 * Called for all faults where 'address' is part of the kernel address
 * space.  Might get called for faults that originate from *code* that
 * ran in userspace or the kernel.
 */
static void
do_kern_addr_fault(pt_regs_s *regs,
		unsigned long hw_error_code, unsigned long address)
{
	// /*
	//  * Protection keys exceptions only happen on user pages.  We
	//  * have no user pages in the kernel portion of the address
	//  * space, so do not expect them here.
	//  */
	// WARN_ON_ONCE(hw_error_code & X86_PF_PK);

	// if (is_f00f_bug(regs, hw_error_code, address))
	// 	return;

	// /* Was the fault spurious, caused by lazy TLB invalidation? */
	// if (spurious_kernel_fault(hw_error_code, address))
	// 	return;

	// /* kprobes don't want to hook the spurious faults: */
	// if (WARN_ON_ONCE(kprobe_page_fault(regs, X86_TRAP_PF)))
	// 	return;

	// /*
	//  * Note, despite being a "bad area", there are quite a few
	//  * acceptable reasons to get here, such as erratum fixups
	//  * and handling kernel code that can fault, like get_user().
	//  *
	//  * Don't take the mm semaphore here. If we fixup a prefetch
	//  * fault we could otherwise deadlock:
	//  */
	// bad_area_nosemaphore(regs, hw_error_code, address);
}

/*
 * Handle faults in the user portion of the address space.  Nothing in here
 * should check X86_PF_USER without a specific justification: for almost
 * all purposes, we should treat a normal kernel access to user memory
 * (e.g. get_user(), put_user(), etc.) the same as the WRUSS instruction.
 * The one exception is AC flag handling, which is, per the x86
 * architecture, special for WRUSS.
 */
static inline
void do_user_addr_fault(pt_regs_s *regs,
		unsigned long error_code, unsigned long address) {
	vma_s *vma;
	task_s *tsk;
	mm_s *mm;
	vm_fault_t fault;
	unsigned int flags = FAULT_FLAG_DEFAULT;

	tsk = current;
	mm = tsk->mm;

	// if (unlikely((error_code & (X86_PF_USER | X86_PF_INSTR)) == X86_PF_INSTR)) {
	// 	/*
	// 	 * Whoops, this is kernel mode code trying to execute from
	// 	 * user memory.  Unless this is AMD erratum #93, which
	// 	 * corrupts RIP such that it looks like a user address,
	// 	 * this is unrecoverable.  Don't even try to look up the
	// 	 * VMA or look for extable entries.
	// 	 */
	// 	if (is_errata93(regs, address))
	// 		return;

	// 	page_fault_oops(regs, error_code, address);
	// 	return;
	// }

	// /* kprobes don't want to hook the spurious faults: */
	// if (WARN_ON_ONCE(kprobe_page_fault(regs, X86_TRAP_PF)))
	// 	return;

	// /*
	//  * Reserved bits are never expected to be set on
	//  * entries in the user portion of the page tables.
	//  */
	// if (unlikely(error_code & X86_PF_RSVD))
	// 	pgtable_bad(regs, error_code, address);

	// /*
	//  * If SMAP is on, check for invalid kernel (supervisor) access to user
	//  * pages in the user address space.  The odd case here is WRUSS,
	//  * which, according to the preliminary documentation, does not respect
	//  * SMAP and will have the USER bit set so, in all cases, SMAP
	//  * enforcement appears to be consistent with the USER bit.
	//  */
	// if (unlikely(cpu_feature_enabled(X86_FEATURE_SMAP) &&
	// 	     !(error_code & X86_PF_USER) &&
	// 	     !(regs->flags & X86_EFLAGS_AC))) {
	// 	/*
	// 	 * No extable entry here.  This was a kernel access to an
	// 	 * invalid pointer.  get_kernel_nofault() will not get here.
	// 	 */
	// 	page_fault_oops(regs, error_code, address);
	// 	return;
	// }

	// /*
	//  * If we're in an interrupt, have no user context or are running
	//  * in a region with pagefaults disabled then we must not take the fault
	//  */
	// if (unlikely(faulthandler_disabled() || !mm)) {
	// 	bad_area_nosemaphore(regs, error_code, address);
	// 	return;
	// }

	/*
	 * It's safe to allow irq's after cr2 has been saved and the
	 * vmalloc fault has been handled.
	 *
	 * User-mode registers count as a user access even for any
	 * potential system fault or CPU buglet:
	 */
	if (user_mode(regs)) {
		local_irq_enable();
		flags |= FAULT_FLAG_USER;
	} else {
		if (regs->flags & X86_EFLAGS_IF)
			local_irq_enable();
	}

	// perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);

	if (error_code & X86_PF_WRITE)
		flags |= FAULT_FLAG_WRITE;
	if (error_code & X86_PF_INSTR)
		flags |= FAULT_FLAG_INSTRUCTION;

	// /*
	//  * Faults in the vsyscall page might need emulation.  The
	//  * vsyscall page is at a high address (>PAGE_OFFSET), but is
	//  * considered to be part of the user address space.
	//  *
	//  * The vsyscall page does not have a "real" VMA, so do this
	//  * emulation before we go searching for VMAs.
	//  *
	//  * PKRU never rejects instruction fetches, so we don't need
	//  * to consider the PF_PK bit.
	//  */
	// if (is_vsyscall_vaddr(address)) {
	// 	if (emulate_vsyscall(error_code, regs, address))
	// 		return;
	// }

	// /*
	//  * Kernel-mode access to the user address space should only occur
	//  * on well-defined single instructions listed in the exception
	//  * tables.  But, an erroneous kernel fault occurring outside one of
	//  * those areas which also holds mmap_lock might deadlock attempting
	//  * to validate the fault against the address space.
	//  *
	//  * Only do the expensive exception table search when we might be at
	//  * risk of a deadlock.  This happens if we
	//  * 1. Failed to acquire mmap_lock, and
	//  * 2. The access did not originate in userspace.
	//  */
	// if (unlikely(!mmap_read_trylock(mm))) {
	// 	if (!user_mode(regs) && !search_exception_tables(regs->ip)) {
	// 		/*
	// 		 * Fault from code in kernel from
	// 		 * which we do not expect faults.
	// 		 */
	// 		bad_area_nosemaphore(regs, error_code, address);
	// 		return;
	// 	}
// retry:
	// 	mmap_read_lock(mm);
	// } else {
	// 	/*
	// 	 * The above down_read_trylock() might have succeeded in
	// 	 * which case we'll have missed the might_sleep() from
	// 	 * down_read():
	// 	 */
	// 	might_sleep();
	// }

	vm_fault_s vmf = myos_dump_pagetable(address);
	vma = myos_find_vma(mm, address);
	if (unlikely(!vma)) {
		myos_bad_area(regs, error_code, address);
		return;
	}
	if (likely(vma->vm_start <= address))
		goto good_area;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		myos_bad_area(regs, error_code, address);
		return;
	}
	if (unlikely(expand_stack(vma, address))) {
		myos_bad_area(regs, error_code, address);
		return;
	}

	/*
	 * Ok, we have a good vm_area for this memory access, so
	 * we can handle it..
	 */
good_area:
	// if (unlikely(access_error(error_code, vma))) {
	// 	bad_area_access_error(regs, error_code, address, vma);
	// 	return;
	// }

	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.  Since we never set FAULT_FLAG_RETRY_NOWAIT, if
	 * we get VM_FAULT_RETRY back, the mmap_lock has been unlocked.
	 *
	 * Note that handle_userfault() may also release and reacquire mmap_lock
	 * (and not return with VM_FAULT_RETRY), when returning to userland to
	 * repeat the page fault later with a VM_FAULT_NOPAGE retval
	 * (potentially after handling any pending signal during the return to
	 * userland). The return to userland is identified whenever
	 * FAULT_FLAG_USER|FAULT_FLAG_KILLABLE are both set in flags.
	 */
	// fault = handle_mm_fault(vma, address, flags, regs);
	fault = myos_handle_mm_fault(vma, regs, address, flags);

	// if (fault_signal_pending(fault, regs)) {
	// 	/*
	// 	 * Quick path to respond to signals.  The core mm code
	// 	 * has unlocked the mm for us if we get here.
	// 	 */
	// 	if (!user_mode(regs))
	// 		kernelmode_fixup_or_oops(regs, error_code, address,
	// 					 SIGBUS, BUS_ADRERR,
	// 					 ARCH_DEFAULT_PKEY);
	// 	return;
	// }

	// /*
	//  * If we need to retry the mmap_lock has already been released,
	//  * and if there is a fatal signal pending there is no guarantee
	//  * that we made any progress. Handle this case first.
	//  */
	// if (unlikely(fault & VM_FAULT_RETRY)) {
	// 	flags |= FAULT_FLAG_TRIED;
	// 	goto retry;
	// }

	// mmap_read_unlock(mm);
	// if (likely(!(fault & VM_FAULT_ERROR)))
	// 	return;

	// if (fatal_signal_pending(current) && !user_mode(regs)) {
	// 	kernelmode_fixup_or_oops(regs, error_code, address,
	// 				 0, 0, ARCH_DEFAULT_PKEY);
	// 	return;
	// }

	// if (fault & VM_FAULT_OOM) {
	// 	/* Kernel mode? Handle exceptions or die: */
	// 	if (!user_mode(regs)) {
	// 		kernelmode_fixup_or_oops(regs, error_code, address,
	// 					 SIGSEGV, SEGV_MAPERR,
	// 					 ARCH_DEFAULT_PKEY);
	// 		return;
	// 	}

	// 	/*
	// 	 * We ran out of memory, call the OOM killer, and return the
	// 	 * userspace (which will retry the fault, or kill us if we got
	// 	 * oom-killed):
	// 	 */
	// 	pagefault_out_of_memory();
	// } else {
	// 	if (fault & (VM_FAULT_SIGBUS|VM_FAULT_HWPOISON|
	// 		     VM_FAULT_HWPOISON_LARGE))
	// 		do_sigbus(regs, error_code, address, fault);
	// 	else if (fault & VM_FAULT_SIGSEGV)
	// 		bad_area_nosemaphore(regs, error_code, address);
	// 	else
	// 		BUG();
	// }
}


static __always_inline void
handle_page_fault(pt_regs_s *regs,
		unsigned long error_code, unsigned long address) {
	// trace_page_fault_entries(regs, error_code, address);

	// if (kmmio_fault(regs, address))
	// 	return;

	/* Was the fault on kernel-controlled part of the address space? */
	if (unlikely(fault_in_kernel_space(address))) {
		do_kern_addr_fault(regs, error_code, address);
	} else {
		do_user_addr_fault(regs, error_code, address);
		/*
		 * User address page fault handling might have reenabled
		 * interrupts. Fixing up all potential exit points of
		 * do_user_addr_fault() and its leaf functions is just not
		 * doable w/o creating an unholy mess or turning the code
		 * upside down.
		 */
		local_irq_disable();
	}
}


__visible noinstr void
exc_page_fault(pt_regs_s *regs, unsigned long error_code)
{
	// myos_excep_page_fault(regs);

	unsigned long address = read_cr2();
	// irqentry_state_t state;

	// prefetchw(&current->mm->mmap_lock);

	// /*
	//  * KVM uses #PF vector to deliver 'page not present' events to guests
	//  * (asynchronous page fault mechanism). The event happens when a
	//  * userspace task is trying to access some valid (from guest's point of
	//  * view) memory which is not currently mapped by the host (e.g. the
	//  * memory is swapped out). Note, the corresponding "page ready" event
	//  * which is injected when the memory becomes available, is delivered via
	//  * an interrupt mechanism and not a #PF exception
	//  * (see arch/x86/kernel/kvm.c: sysvec_kvm_asyncpf_interrupt()).
	//  *
	//  * We are relying on the interrupted context being sane (valid RSP,
	//  * relevant locks not held, etc.), which is fine as long as the
	//  * interrupted context had IF=1.  We are also relying on the KVM
	//  * async pf type field and CR2 being read consistently instead of
	//  * getting values from real and async page faults mixed up.
	//  *
	//  * Fingers crossed.
	//  *
	//  * The async #PF handling code takes care of idtentry handling
	//  * itself.
	//  */
	// if (kvm_handle_async_pf(regs, (u32)address))
	// 	return;

	// /*
	//  * Entry handling for valid #PF from kernel mode is slightly
	//  * different: RCU is already watching and rcu_irq_enter() must not
	//  * be invoked because a kernel fault on a user space address might
	//  * sleep.
	//  *
	//  * In case the fault hit a RCU idle region the conditional entry
	//  * code reenabled RCU to avoid subsequent wreckage which helps
	//  * debuggability.
	//  */
	// state = irqentry_enter(regs);

	// instrumentation_begin();
	handle_page_fault(regs, error_code, address);
	// instrumentation_end();

	// irqentry_exit(regs, state);
}
