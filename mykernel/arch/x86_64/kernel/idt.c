// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0-only
/*
 * Interrupt descriptor table related code
 */
// #include <linux/interrupt.h>

// #include <asm/cpu_entry_area.h>
// #include <asm/set_memory.h>
#include <asm/traps.h>
// #include <asm/proto.h>
#include <asm/desc.h>
#include <asm/hw_irq.h>
#include <asm/idtentry.h>


#include <linux/kernel/cache.h>

#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/glo.h>


#define DPL0		0x0
#define DPL3		0x3

#define DEFAULT_STACK	0

#define IDT_TABLE_SIZE		(IDT_ENTRIES * sizeof(gate_desc))


/* Must be page-aligned because the real IDT is used in the cpu entry area */
static gate_desc idt_table[IDT_ENTRIES] __page_aligned_bss;
struct desc_ptr idt_descr __ro_after_init = {
	.size		= IDT_TABLE_SIZE - 1,
	.address	= (unsigned long)&idt_table,
};

// void load_current_idt(void)
// {
// 	// lockdep_assert_irqs_disabled();
// 	load_idt(&idt_descr);
// }

static __init void set_intr_gate(unsigned int n, const void *addr)
{
	struct idt_data data;
	gate_desc desc;

	init_idt_data(&data, n, addr);

	// idt_setup_from_table(idt_table, &data, 1, false);
	// {
		idt_init_desc(&desc, &data);
		write_idt_entry(idt_table, data.vector, &desc);
	// }
}


/**
 * idt_setup_early_handler - Initializes the idt table with early handlers
 */
void __init idt_setup_early_handler(void)
{
	for (int i = 0; i < NR_EXCEPTION_VECTORS; i++) {
		void *entry = exc_entries_start + IDT_ALIGN * i;
		set_intr_gate(i, entry);
	}
	load_idt(&idt_descr);
}


/**
 * idt_setup_apic_and_irq_gates - Setup APIC/SMP and normal interrupt gates
 */
void __init idt_setup_apic_and_irq_gates(void)
{
	int i;
	void *entry;

	// static __init void
	// idt_setup_from_table(gate_desc *idt, const struct idt_data *t, int size, bool sys)
	// {
	for (i = 0; i < NR_EXCEPTION_VECTORS; i++)
	{
		entry = exc_entries_start + IDT_ALIGN * i;
		set_intr_gate(i, entry);
	}
	// }

	for (i = FIRST_EXTERNAL_VECTOR; i < NR_VECTORS; i++)
	{
		entry = irq_entries_start + IDT_ALIGN * (i - FIRST_EXTERNAL_VECTOR);
		set_intr_gate(i, entry);
	}


	// /* Map IDT into CPU entry area and reload it. */
	// idt_map_in_cea();
	load_idt(&idt_descr);

	// /* Make the IDT table read only */
	// set_memory_ro((unsigned long)&idt_table, 1);

	// idt_setup_done = true;
}

