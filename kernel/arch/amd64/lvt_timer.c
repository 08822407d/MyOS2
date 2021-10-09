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

hw_int_controller_s LVT_timer_int_controller = 
{
	.enable		= NULL,
	.disable	= NULL,
	.install	= NULL,
	.uninstall	= NULL,
	.ack		= LVT_ack,
};

void LVT_timer_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
	color_printk(WHITE, BLUE, "(LVT_timer)");
}

void LVT_timer_init()
{
	ioapic_retentry_T entry;
	
	// set LVT
	apic_lvt_u LVT_timer;
	LVT_timer.def.deliver_mode =
	LVT_timer.def.deliver_status =
	LVT_timer.def.irr =
	LVT_timer.def.polarity =
	LVT_timer.def.res_1 =
	LVT_timer.def.res_2 =
	LVT_timer.def.trigger =
	LVT_timer.def.mask = 0;
	LVT_timer.def.timer_mode = APIC_LVT_Timer_Periodic;
	LVT_timer.def.vector = VECTOR_IPI(LAPIC_LVT_TIMER_IRQ);

	register_IPI(LAPIC_LVT_TIMER_IRQ, &entry , "LVT_timer",
				 LVT_timer.value, &LVT_timer_int_controller,
				 &LVT_timer_handler);

	wrmsr(LVT_TIMER_INIT_COUNT_REG_MSR, 0x10);
	wrmsr(LAPIC_LVT_TIMER_REG_MSR, LVT_timer.value);
	wrmsr(LVT_TIMER_DIV_CONF_REG_MSR, 0x03);
}