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
#include <obsolete/archtypes.h>

#define DPL0		0x0
#define DPL3		0x3

#define DEFAULT_STACK	0


#define IDT_TABLE_SIZE		(IDT_ENTRIES * sizeof(gate_desc))


extern gatedesc64_T	idt_table[];
struct desc_ptr idt_descr __ro_after_init = {
	.size		= IDT_TABLE_SIZE - 1,
	.address	= (unsigned long)&idt_table,
};