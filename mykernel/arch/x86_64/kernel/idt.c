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
// #include <asm/hw_irq.h>
#include <asm/idtentry.h>


#include <linux/kernel/cache.h>

#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
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

/**
 * idt_setup_early_handler - Initializes the idt table with early handlers
 */
void __init idt_setup_early_handler(void)
{
	// for (int i = 0; i < NUM_EXCEPTION_VECTORS; i++)
	// 	set_intr_gate(i, early_idt_handler_array[i]);

	pack_gate(&idt_table[VECTOR_EXCEP_00], GATE_TRAP     , cpu_excep_handler_00, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_01], GATE_TRAP     , cpu_excep_handler_01, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_02], GATE_INTERRUPT, cpu_excep_handler_02, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_03], GATE_TRAP     , cpu_excep_handler_03, USER_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_04], GATE_TRAP     , cpu_excep_handler_04, USER_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_05], GATE_TRAP     , cpu_excep_handler_05, USER_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_06], GATE_TRAP     , cpu_excep_handler_06, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_07], GATE_TRAP     , cpu_excep_handler_07, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_08], GATE_TRAP     , cpu_excep_handler_08, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_09], GATE_TRAP     , cpu_excep_handler_09, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_10], GATE_TRAP     , cpu_excep_handler_10, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_11], GATE_TRAP     , cpu_excep_handler_11, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_12], GATE_TRAP     , cpu_excep_handler_12, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_13], GATE_TRAP     , cpu_excep_handler_13, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_14], GATE_TRAP     , cpu_excep_handler_14, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_16], GATE_TRAP     , cpu_excep_handler_16, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_17], GATE_TRAP     , cpu_excep_handler_17, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_18], GATE_TRAP     , cpu_excep_handler_18, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_19], GATE_TRAP     , cpu_excep_handler_19, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_20], GATE_TRAP     , cpu_excep_handler_20, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_EXCEP_21], GATE_TRAP     , cpu_excep_handler_21, KRNL_RPL, 0);

	load_idt(&idt_descr);
}


/**
 * idt_setup_apic_and_irq_gates - Setup APIC/SMP and normal interrupt gates
 */
void __init idt_setup_apic_and_irq_gates(void)
{
	// int i = FIRST_EXTERNAL_VECTOR;
	// void *entry;

	// static __init void
	// idt_setup_from_table(gate_desc *idt, const struct idt_data *t, int size, bool sys)
	// {
	// }

	// for_each_clear_bit_from(i, system_vectors, FIRST_SYSTEM_VECTOR) {
	// 	entry = irq_entries_start + IDT_ALIGN * (i - FIRST_EXTERNAL_VECTOR);
	// 	set_intr_gate(i, entry);
		pack_gate(&idt_table[VECTOR_IRQ( 0)], GATE_INTERRUPT, hwint00, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 1)], GATE_INTERRUPT, hwint01, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 2)], GATE_INTERRUPT, hwint02, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 3)], GATE_INTERRUPT, hwint03, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 4)], GATE_INTERRUPT, hwint04, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 5)], GATE_INTERRUPT, hwint05, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 6)], GATE_INTERRUPT, hwint06, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 7)], GATE_INTERRUPT, hwint07, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 8)], GATE_INTERRUPT, hwint08, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ( 9)], GATE_INTERRUPT, hwint09, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(10)], GATE_INTERRUPT, hwint10, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(11)], GATE_INTERRUPT, hwint11, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(12)], GATE_INTERRUPT, hwint12, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(13)], GATE_INTERRUPT, hwint13, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(14)], GATE_INTERRUPT, hwint14, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(15)], GATE_INTERRUPT, hwint15, KRNL_RPL, 0);
	#ifndef USE_I8259
		pack_gate(&idt_table[VECTOR_IRQ(16)], GATE_INTERRUPT, hwint16, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(17)], GATE_INTERRUPT, hwint17, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(18)], GATE_INTERRUPT, hwint18, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(19)], GATE_INTERRUPT, hwint19, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(20)], GATE_INTERRUPT, hwint20, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(21)], GATE_INTERRUPT, hwint21, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(22)], GATE_INTERRUPT, hwint22, KRNL_RPL, 0);
		pack_gate(&idt_table[VECTOR_IRQ(23)], GATE_INTERRUPT, hwint23, KRNL_RPL, 0);
	#endif
	// }

	// for_each_clear_bit_from(i, system_vectors, NR_VECTORS) {
	// 	/*
	// 	 * Don't set the non assigned system vectors in the
	// 	 * system_vectors bitmap. Otherwise they show up in
	// 	 * /proc/interrupts.
	// 	 */
	// 	entry = spurious_entries_start + IDT_ALIGN * (i - FIRST_SYSTEM_VECTOR);
	// 	set_intr_gate(i, entry);
	pack_gate(&idt_table[VECTOR_IPI( 0)], GATE_INTERRUPT, lapic_ipi00, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 1)], GATE_INTERRUPT, lapic_ipi01, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 2)], GATE_INTERRUPT, lapic_ipi02, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 3)], GATE_INTERRUPT, lapic_ipi03, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 4)], GATE_INTERRUPT, lapic_ipi04, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 5)], GATE_INTERRUPT, lapic_ipi05, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 6)], GATE_INTERRUPT, lapic_ipi06, KRNL_RPL, 0);
	pack_gate(&idt_table[VECTOR_IPI( 7)], GATE_INTERRUPT, lapic_ipi07, KRNL_RPL, 0);
	// }
	// /* Map IDT into CPU entry area and reload it. */
	// idt_map_in_cea();
	load_idt(&idt_descr);

	// /* Make the IDT table read only */
	// set_memory_ro((unsigned long)&idt_table, 1);

	// idt_setup_done = true;
}

