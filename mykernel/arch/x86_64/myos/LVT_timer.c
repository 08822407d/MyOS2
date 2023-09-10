#include <linux/kernel/stddef.h>
#include <linux/kernel/kernel.h>
#include <linux/device/tty.h>
#include <linux/lib/string.h>
#include <asm/processor.h>
#include <asm/apic.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>
#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/device.h>


#define SCALE 100

void LVT_timer_ack(unsigned long param);
static long lvt_count = 0;

lvt_timer_t lvt_timer;
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
	*((u32 *)&lvt_timer) = VECTOR_IPI(LAPIC_LVT_TIMER_IRQ);
	*((u32 *)&lvt_timer) |= APIC_LVT_TIMER_PERIODIC;
	
	register_IPI(LAPIC_LVT_TIMER_IRQ, &lvt_timer, "LVT_timer",
				 *((u32 *)&lvt_timer), &LVT_timer_int_controller,
				 &LVT_timer_handler);

	LVT_timer_ack(0);
}

void LVT_timer_ack(unsigned long param)
{
	apic_msr_write(APIC_TMICT, 0xAFF);
	apic_msr_write(APIC_LVTT, *((u32 *)&lvt_timer));
	apic_msr_write(APIC_TDCR, APIC_TDR_DIV_128);
	IOAPIC_edge_ack(param);
}