#include <linux/kernel/stddef.h>
#include <linux/kernel/kernel.h>
#include <linux/device/tty.h>
#include <linux/lib/string.h>
#include <asm/processor.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>
#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
#include <obsolete/apic.h>
#include <obsolete/device.h>

#define DIVISOR_2	0x00
#define DIVISOR_4	0x01
#define DIVISOR_8	0x02
#define DIVISOR_16	0x03
#define DIVISOR_32	0x08
#define DIVISOR_64	0x09
#define DIVISOR_128	0x0A
#define DIVISOR_1	0x0B

#define SCALE 100

void LVT_timer_ack(unsigned long param);
static long lvt_count = 0;

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

void LVT_timer_handler(unsigned long parameter, pt_regs_s *sf_regs)
{
	lvt_count++;
	if ((lvt_count % SCALE) == 0)
	{
		char buf[30];
		memset(buf, 0 , sizeof(buf));
		snprintf(buf, sizeof(buf), "(LVT_timer: %ld)", lvt_count / SCALE);
		myos_tty_write_color_at(buf, sizeof(buf), BLACK, GREEN, 60, 0);
	}
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
	wrmsrl(LVT_TIMER_INIT_COUNT_REG_MSR, 0xAFF);
	wrmsrl(LAPIC_LVT_TIMER_REG_MSR, LVT_timer.value);
	wrmsrl(LVT_TIMER_DIV_CONF_REG_MSR, DIVISOR_128);

	IOAPIC_edge_ack(param);
}