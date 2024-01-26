#include <linux/kernel/stddef.h>
#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
#include <linux/lib/string.h>
#include <linux/device/tty.h>
#include <asm/io.h>
#include <asm/apic.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>

#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/device.h>

hw_int_controller_s HPET_int_controller = 
{
	.enable		= IOAPIC_enable,
	.disable	= IOAPIC_disable,
	.install	= IOAPIC_install,
	.uninstall	= IOAPIC_uninstall,
	.ack		= IOAPIC_edge_ack,
};

extern void do_timer(unsigned long ticks);
void HPET_handler(unsigned long parameter, pt_regs_s * sf_regs)
{
	jiffies++;
	do_timer(1);

	timer_s * tmr = timer_lhdr.header.next->owner_p;
	if(tmr->expire_jiffies <= jiffies)
		set_softirq_status(HPET_TIMER0_IRQ);

	char buf[12];
	memset(buf, 0 , sizeof(buf));
	snprintf(buf, sizeof(buf), "(HPET: %ld)   ", jiffies);
	myos_tty_write_color_at(buf, sizeof(buf), BLACK, GREEN, 48, 0);
}
	
void HPET_init()
{
	unsigned int x;
	unsigned int *p = (unsigned int *)0xfed00000;
	unsigned char *HPET_addr = (unsigned char *)phys_to_virt((phys_addr_t)p);
	myos_ioremap((unsigned long)p, PAGE_SIZE);
	ioapic_retentry_T entry;
	
	//get RCBA address
	outl(0x8000f8f0, 0xcf8);
	x = inl(0xcfc);
	x = x & 0xffffc000;	

	//get HPTC address
	if(x > 0xfec00000 && x < 0xfee00000)
	{
		p = (unsigned int *)phys_to_virt((phys_addr_t)(x + 0x3404UL));
		//enable HPET
		*p = 0x80;
	}
	mb();

	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	register_irq(HPET_TIMER0_IRQ, &entry , "HPET0",
				 0, &HPET_int_controller,
				 &HPET_handler);
	
	// color_printk(RED,BLACK,"HPET - GCAP_ID:<%#018lx>\n",*(unsigned long *)HPET_addr);
	uint64_t accuracy = *(uint64_t *)HPET_addr >> 32;

	*(unsigned long *)(HPET_addr + 0x10) = 3;
	mb();

	//edge triggered & periodic
	*(unsigned long *)(HPET_addr + 0x100) = 0x004c;
	mb();

	// 1S qemu may have a different precision so here need a calculate
	// 0x38D7EA4C680 is hex value of 1*10^15
	unsigned long period = 0x38D7EA4C680 / accuracy;
	*(unsigned long *)(HPET_addr + 0x108) = period * 10;
	mb();

	//init MAIN_CNT & get CMOS time
	get_cmos_time(&time);
	*(unsigned long *)(HPET_addr + 0xf0) = 0;
	mb();
}