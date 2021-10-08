#include <lib/string.h>
#include <sys/cdefs.h>
#include <lib/stddef.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"

#include "../../include/glo.h"
#include "../../include/printk.h"
#include "../../include/proto.h"
#include "../../include/ktypes.h"

hw_int_controller_s LA_TMR_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void LA_TMR_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
	color_printk(WHITE, BLUE, "(LA_TMR) ");
}

void LA_TMR_init()
{
	ioapic_retentry_T entry;
	
	// set LVT
	apic_lvt_T LA_TMR_lvt;
	LA_TMR_lvt.deliver_mode =
	LA_TMR_lvt.deliver_status =
	LA_TMR_lvt.irr =
	LA_TMR_lvt.polarity =
	LA_TMR_lvt.res_1 =
	LA_TMR_lvt.res_2 =
	LA_TMR_lvt.trigger =
	LA_TMR_lvt.mask = 0;
	LA_TMR_lvt.timer_mode = 0x01;
	LA_TMR_lvt.vector = VECTOR(LAPIC_TIMER_IRQ);

	register_irq(LAPIC_TIMER_IRQ, &entry , "LA_TMR",
				 NULL, &LA_TMR_int_controller,
				 &LA_TMR_handler);
	
}