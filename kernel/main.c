#include <lib/string.h>
#include <lib/assert.h>

#include "include/glo.h"
#include "include/proto.h"
#include "include/proc.h"
#include "include/printk.h"

#include "arch/amd64/include/arch_proto.h"
#include "arch/amd64/include/archtypes.h"
#include "arch/amd64/include/apic.h"
#include "arch/amd64/include/device.h"

void kmain(size_t cpu_idx)
{
    prot_bsp_init();

	SMP_init();

	proc_init();

	wrmsr(0x830,0xc4500);	//INIT IPI
	wrmsr(0x830,0xc4620);	//Start-up IPI

	// intcmd_reg_s icr_entry;
	// icr_entry.vector = 0x00;
	// icr_entry.deliver_mode =  APIC_ICR_IOAPIC_INIT;
	// icr_entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	// icr_entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	// icr_entry.res_1 = 0;
	// icr_entry.level = ICR_LEVEL_DE_ASSERT;
	// icr_entry.trigger = APIC_ICR_IOAPIC_Edge;
	// icr_entry.res_2 = 0;
	// icr_entry.dest_shorthand = ICR_ALL_EXCLUDE_Self;
	// icr_entry.res_3 = 0;
	// icr_entry.dst.x2apic_dst = 0x00;
	
	// wrmsr(0x830, *(unsigned long *)&icr_entry);	//INIT IPI

	// icr_entry.vector = 0x20;
	// icr_entry.deliver_mode = ICR_Start_up;
	
	// wrmsr(0x830, *(unsigned long *)&icr_entry);	//Start-up IPI
	// wrmsr(0x830, *(unsigned long *)&icr_entry);	//Start-up IPI

	schedule();

	softirq_init();
	timer_init();

	devices_init();

	// int i = 1 / 0;
    while(1){
		// if(p_kb->count)
		// 	analysis_keycode();
	};
}