// SPDX-License-Identifier: GPL-2.0-only
// #include <linux/clockchips.h>
// #include <linux/interrupt.h>
// #include <linux/export.h>
#include <linux/kernel/delay.h>
// #include <linux/hpet.h>
#include <linux/kernel/cpu.h>
// #include <linux/irq.h>

// #include <asm/irq_remapping.h>
#include <asm/hpet.h>
// #include <asm/time.h>
// #include <asm/mwait.h>


#include <linux/mm/mm.h>
#include <linux/kernel/io.h>

// #undef  pr_fmt
// #define pr_fmt(fmt) "hpet: " fmt

enum hpet_mode {
	HPET_MODE_UNUSED,
	HPET_MODE_LEGACY,
	HPET_MODE_CLOCKEVT,
	HPET_MODE_DEVICE,
};

typedef struct hpet_channel {
	// struct clock_event_device	evt;
	unsigned int		num;
	unsigned int		cpu;
	unsigned int		irq;
	unsigned int		in_use;
	enum hpet_mode		mode;
	unsigned int		boot_cfg;
	char				name[10];
} hpet_channel_s;

typedef struct hpet_base {
	unsigned int		nr_channels;
	unsigned int		nr_clockevents;
	unsigned int		boot_cfg;
	hpet_channel_s		*channels;
} hpet_base_s;

// #define HPET_MASK			CLOCKSOURCE_MASK(32)

#define HPET_MIN_CYCLES			128
#define HPET_MIN_PROG_DELTA		(HPET_MIN_CYCLES + (HPET_MIN_CYCLES >> 1))

/*
 * HPET address is set in acpi/boot.c, when an ACPI entry exists
 */
unsigned long			hpet_address;

static void __iomem		*hpet_virt_address;
static hpet_base_s		hpet_base;

static unsigned long	hpet_freq;


inline unsigned int hpet_readl(unsigned int a) {
	return readl(hpet_virt_address + a);
}

static inline void hpet_writel(unsigned int d, unsigned int a) {
	writel(d, hpet_virt_address + a);
}	

static inline void hpet_set_mapping(void) {
	hpet_address = 0xfed00000;
	hpet_virt_address = myos_ioremap(hpet_address, HPET_MMAP_SIZE);
}



extern void HPET_init(void);
/**
 * hpet_enable - Try to setup the HPET timer. Returns 1 on success.
 */
int __init hpet_enable(void)
{
	u32 hpet_period, cfg, id, irq;
	unsigned int i, channels;
	hpet_channel_s *hc;
	u64 freq;

	// if (!is_hpet_capable())
	// 	return 0;

	hpet_set_mapping();
	if (!hpet_virt_address)
		return 0;

	// /* Validate that the config register is working */
	// if (!hpet_cfg_working())
	// 	goto out_nohpet;

	/*
	 * Read the period and check for a sane value:
	 */
	hpet_period = hpet_readl(HPET_PERIOD);
	if (hpet_period < HPET_MIN_PERIOD || hpet_period > HPET_MAX_PERIOD)
		goto out_nohpet;

	/* The period is a femtoseconds value. Convert it to a frequency. */
	freq = FSEC_PER_SEC;
	do_div(freq, hpet_period);
	hpet_freq = freq;

	/*
	 * Read the HPET ID register to retrieve the IRQ routing
	 * information and the number of channels
	 */
	id = hpet_readl(HPET_ID);

	/* This is the HPET channel number which is zero based */
	channels = ((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1;

	// /*
	//  * The legacy routing mode needs at least two channels, tick timer
	//  * and the rtc emulation channel.
	//  */
	// if (IS_ENABLED(CONFIG_HPET_EMULATE_RTC) && channels < 2)
	// 	goto out_nohpet;

	// hc = kcalloc(channels, sizeof(*hc), GFP_KERNEL);
	// if (!hc) {
	// 	pr_warn("Disabling HPET.\n");
	// 	goto out_nohpet;
	// }
	// hpet_base.channels = hc;
	// hpet_base.nr_channels = channels;

	/* Read, store and sanitize the global configuration */
	cfg = hpet_readl(HPET_CFG);
	// hpet_base.boot_cfg = cfg;
	cfg &= ~(HPET_CFG_ENABLE | HPET_CFG_LEGACY);
	// hpet_writel(cfg, HPET_CFG);
	// if (cfg)
	// 	pr_warn("Global config: Unknown bits %#x\n", cfg);

	// /* Read, store and sanitize the per channel configuration */
	// for (i = 0; i < channels; i++, hc++) {
	// 	hc->num = i;

	// 	cfg = hpet_readl(HPET_Tn_CFG(i));
	// 	hc->boot_cfg = cfg;
	// 	irq = (cfg & Tn_INT_ROUTE_CNF_MASK) >> Tn_INT_ROUTE_CNF_SHIFT;
	// 	hc->irq = irq;

	// 	cfg &= ~(HPET_TN_ENABLE | HPET_TN_LEVEL | HPET_TN_FSB);
	// 	hpet_writel(cfg, HPET_Tn_CFG(i));

	// 	cfg &= ~(HPET_TN_PERIODIC | HPET_TN_PERIODIC_CAP
	// 		 | HPET_TN_64BIT_CAP | HPET_TN_32BIT | HPET_TN_ROUTE
	// 		 | HPET_TN_FSB | HPET_TN_FSB_CAP);
	// 	if (cfg)
	// 		pr_warn("Channel #%u config: Unknown bits %#x\n", i, cfg);
	// }
	// hpet_print_config();

	// /*
	//  * Validate that the counter is counting. This needs to be done
	//  * after sanitizing the config registers to properly deal with
	//  * force enabled HPETs.
	//  */
	// if (!hpet_counting())
	// 	goto out_nohpet;

	// if (tsc_clocksource_watchdog_disabled())
	// 	clocksource_hpet.flags |= CLOCK_SOURCE_MUST_VERIFY;
	// clocksource_register_hz(&clocksource_hpet, (u32)hpet_freq);

	// if (id & HPET_ID_LEGSUP) {
	// 	hpet_legacy_clockevent_register(&hpet_base.channels[0]);
	// 	hpet_base.channels[0].mode = HPET_MODE_LEGACY;
	// 	if (IS_ENABLED(CONFIG_HPET_EMULATE_RTC))
	// 		hpet_base.channels[1].mode = HPET_MODE_LEGACY;
	// 	return 1;
	// }
	// return 0;


	HPET_init();
out_nohpet:
	// kfree(hpet_base.channels);
	// hpet_base.channels = NULL;
	// hpet_base.nr_channels = 0;
	// hpet_clear_mapping();
	// hpet_address = 0;
	return 0;
}



#include <linux/device/tty.h>
#include <linux/kernel/jiffies.h>

#include <obsolete/glo.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
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
	hpet_virt_address = myos_ioremap(hpet_address, HPET_MMAP_SIZE);
	ioapic_retentry_T entry;
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
	
	// color_printk(RED,BLACK,"HPET - GCAP_ID:<%#018lx>\n",*(unsigned long *)hpet_virt_address);
	uint64_t accuracy = *(uint64_t *)hpet_virt_address >> 32;

	*(unsigned long *)(hpet_virt_address + 0x10) = 3;
	mb();

	//edge triggered & periodic
	*(unsigned long *)(hpet_virt_address + 0x100) = 0x004c;
	mb();

	// 1S qemu may have a different precision so here need a calculate
	// 0x38D7EA4C680 is hex value of 1*10^15
	unsigned long period = 0x38D7EA4C680 / accuracy;
	*(unsigned long *)(hpet_virt_address + 0x108) = period * 10;
	mb();

	//init MAIN_CNT & get CMOS time
	get_cmos_time(&time);
	*(unsigned long *)(hpet_virt_address + 0xf0) = 0;
	mb();
}