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

hw_int_controller_s HPET_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

void HPET_handler(unsigned long parameter, stack_frame_s * sf_regs)
{
	jiffies++;

	if(timer_list_head.next->expire_jiffies <= jiffies)
		set_softirq_status(HPET_TIMER0_IRQ);
	
	// switch(current->priority)
	// {
	// 	case 0:
	// 	case 1:
	// 		task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies--;
	// 		current->vrun_time += 1;
	// 		break;
	// 	case 2:
	// 	default:

	// 		task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies -= 2;
	// 		current->vrun_time += 2;
	// 		break;
	// }

	// if(task_schedule[SMP_cpu_id()].CPU_exec_task_jiffies <= 0)
	// 	current->flags |= NEED_SCHEDULE;
}
	
void HPET_init()
{
	unsigned int x;
	unsigned int * p = NULL;
	unsigned char * HPET_addr = (unsigned char *)phy2vir((phy_addr)0xfed00000);
	ioapic_retentry_s entry;
	
	//get RCBA address
	outl(0xcf8,0x8000f8f0);
	x = inl(0xcfc);
	x = x & 0xffffc000;	

	//get HPTC address
	if(x > 0xfec00000 && x < 0xfee00000)
	{
		p = (unsigned int *)phy2vir((phy_addr)(x + 0x3404UL));
	}

	//enable HPET
	*p = 0x80;
	io_mfence();

	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = VECTOR(HPET_TIMER0_IRQ);
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

	register_irq(HPET_TIMER0_IRQ, &entry , "HPET",
				 NULL, &HPET_int_controller,
				 &HPET_handler);
	
	color_printk(RED,BLACK,"HPET - GCAP_ID:<%#018lx>\n",*(unsigned long *)HPET_addr);
	*(unsigned long *)(HPET_addr + 0x10) = 3;
	io_mfence();

	//edge triggered & periodic
	*(unsigned long *)(HPET_addr + 0x100) = 0x004c;
	io_mfence();

	//1S
	*(unsigned long *)(HPET_addr + 0x108) = 14318179;
	io_mfence();

	//init MAIN_CNT & get CMOS time
	get_cmos_time(&time);
	*(unsigned long *)(HPET_addr + 0xf0) = 0;
	io_mfence();
}