// SPDX-License-Identifier: GPL-2.0
/*
 *	Intel IO-APIC support for multi-Pentium hosts.
 *
 *	Copyright (C) 1997, 1998, 1999, 2000, 2009 Ingo Molnar, Hajnalka Szabo
 *
 *	Many thanks to Stig Venaas for trying out countless experimental
 *	patches and reporting/debugging problems patiently!
 *
 *	(c) 1999, Multiple IO-APIC support, developed by
 *	Ken-ichi Yaku <yaku@css1.kbnes.nec.co.jp> and
 *      Hidemi Kishimoto <kisimoto@css1.kbnes.nec.co.jp>,
 *	further tested and cleaned up by Zach Brown <zab@redhat.com>
 *	and Ingo Molnar <mingo@redhat.com>
 *
 *	Fixes
 *	Maciej W. Rozycki	:	Bits for genuine 82489DX APICs;
 *					thanks to Eric Gilmore
 *					and Rolf G. Tews
 *					for testing these extensively
 *	Paul Diefenbaugh	:	Added full ACPI support
 *
 * Historical information which is worth to be preserved:
 *
 * - SiS APIC rmw bug:
 *
 *	We used to have a workaround for a bug in SiS chips which
 *	required to rewrite the index register for a read-modify-write
 *	operation as the chip lost the index information which was
 *	setup for the read already. We cache the data now, so that
 *	workaround has been removed.
 */

#include <linux/mm/mm.h>
// #include <linux/interrupt.h>
// #include <linux/irq.h>
#include <linux/init/init.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/sched.h>
// #include <linux/pci.h>
// #include <linux/mc146818rtc.h>
#include <linux/kernel/compiler.h>
// #include <linux/acpi.h>
// #include <linux/export.h>
// #include <linux/syscore_ops.h>
// #include <linux/freezer.h>
#include <linux/kernel/kthread.h>
// #include <linux/jiffies.h>	/* time_after() */
#include <linux/kernel/slab.h>
#include <linux/mm/memblock.h>
// #include <linux/msi.h>

// #include <asm/irqdomain.h>
#include <asm/io.h>
#include <asm/smp.h>
#include <asm/cpu.h>
#include <asm/desc.h>
// #include <asm/proto.h>
// #include <asm/acpi.h>
#include <asm/dma.h>
// #include <asm/timer.h>
// #include <asm/time.h>
// #include <asm/i8259.h>
#include <asm/setup.h>
// #include <asm/irq_remapping.h>
// #include <asm/hw_irq.h>
#include <asm/apic.h>
#include <asm/pgtable.h>
// #include <asm/x86_init.h>

// #define	for_each_ioapic(idx) \
// 			for ((idx) = 0; (idx) < nr_ioapics; (idx)++)


// void __init io_apic_init_mappings(void)
void __init init_io_apic_mappings(void)
{
	// unsigned long ioapic_phys, idx = FIX_IO_APIC_BASE_0;
	// struct resource *ioapic_res;
	// int i;

	// ioapic_res = ioapic_setup_resources();
	// for_each_ioapic(i) {
	// 	if (smp_found_config) {
	// 		ioapic_phys = mpc_ioapic_addr(i);
	// 	} else {
	// 		ioapic_phys = (unsigned long)memblock_alloc(PAGE_SIZE,
	// 								PAGE_SIZE);
	// 		if (!ioapic_phys)
	// 			panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
	// 				  __func__, PAGE_SIZE, PAGE_SIZE);
	// 		ioapic_phys = __pa(ioapic_phys);
	// 	}
	// 	io_apic_set_fixmap(idx, ioapic_phys);
		phys_addr_t pa = IO_APIC_DEFAULT_PHYS_BASE;
		myos_kernel_physical_mapping_init(pa, pa + PAGE_SIZE);
	// 	apic_printk(APIC_VERBOSE, "mapped IOAPIC to %08lx (%08lx)\n",
	// 		__fix_to_virt(idx) + (ioapic_phys & ~PAGE_MASK),
	// 		ioapic_phys);
	// 	idx++;

	// 	ioapic_res->start = ioapic_phys;
	// 	ioapic_res->end = ioapic_phys + IO_APIC_SLOT_SIZE - 1;
	// 	ioapic_res++;
	// }
}