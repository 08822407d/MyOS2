#include <lib/string.h>
#include <sys/cdefs.h>
#include <lib/stddef.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"

#include "../../include/printk.h"

struct keyboard_inputbuffer * p_kb = NULL;

hw_int_controller_s keyboard_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void keyboard_init()
{		
	ioapic_retentry_s entry;

	entry.vector = KEYBOARD_IRQ;
	entry.deliver_mode = APIC_ICR_IOAPIC_Fixed ;
	entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	entry.polarity = APIC_IOAPIC_POLARITY_HIGH;
	entry.irr = APIC_IOAPIC_IRR_RESET;
	entry.trigger = APIC_ICR_IOAPIC_Edge;
	entry.mask = APIC_ICR_IOAPIC_Masked;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	register_irq(KEYBOARD_IRQ , &entry , "PS/2 keyboard",
				 (unsigned long)p_kb, &keyboard_int_controller,
				 &keyboard_handler);
}

void keyboard_handler(unsigned long param, stack_frame_s * sf_regs)
{
	uint8_t scan_code = inb(0x60);
	color_printk(GREEN, BLACK, "KBD SCAN CODE = %#04x\n", scan_code);
}