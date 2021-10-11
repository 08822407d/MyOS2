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

void LVT_timer_ack(unsigned long param);

// set LVT
apic_lvt_u LVT_timer = {
	.def.deliver_mode = 0,
	.def.deliver_status = 0,
	.def.irr = 0,
	.def.polarity = 0,
	.def.res_1 = 0,
	.def.res_2 = 0,
	.def.trigger = 0,
	.def.mask = 0,
	.def.timer_mode = APIC_LVT_Timer_Periodic,
	.def.vector = VECTOR_IPI(LAPIC_LVT_TIMER_IRQ)
};

hw_int_controller_s LVT_timer_int_controller = 
{
	.enable		= NULL,
	.disable	= NULL,
	.install	= NULL,
	.uninstall	= NULL,
	.ack		= LVT_timer_ack,
};

void LVT_timer_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
	// color_printk(WHITE, BLUE, "(LVT_timer)");
}

void LVT_timer_init()
{
	ioapic_retentry_T entry;
	
	register_IPI(LAPIC_LVT_TIMER_IRQ, &entry , "LVT_timer",
				 LVT_timer.value, &LVT_timer_int_controller,
				 &LVT_timer_handler);

	LVT_timer_ack(0);
}

void LVT_timer_ack(unsigned long param)
{
	wrmsr(LVT_TIMER_INIT_COUNT_REG_MSR, 0x10);
	wrmsr(LAPIC_LVT_TIMER_REG_MSR, LVT_timer.value);
	wrmsr(LVT_TIMER_DIV_CONF_REG_MSR, 0x03);

	IOAPIC_edge_ack(param);
}