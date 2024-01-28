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


/*
 * HPET address is set in acpi/boot.c, when an ACPI entry exists
 */
unsigned long		hpet_address;

// static void __iomem		*hpet_virt_address;
void __iomem		*hpet_virt_address;

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
	struct hpet_channel *hc;
	u64 freq;

	// if (!is_hpet_capable())
	// 	return 0;

	hpet_set_mapping();
	HPET_init();
	if (!hpet_virt_address)
		return 0;

	// /* Validate that the config register is working */
	// if (!hpet_cfg_working())
	// 	goto out_nohpet;

	// /*
	//  * Read the period and check for a sane value:
	//  */
	// hpet_period = hpet_readl(HPET_PERIOD);
	// if (hpet_period < HPET_MIN_PERIOD || hpet_period > HPET_MAX_PERIOD)
	// 	goto out_nohpet;

	// /* The period is a femtoseconds value. Convert it to a frequency. */
	// freq = FSEC_PER_SEC;
	// do_div(freq, hpet_period);
	// hpet_freq = freq;

	// /*
	//  * Read the HPET ID register to retrieve the IRQ routing
	//  * information and the number of channels
	//  */
	// id = hpet_readl(HPET_ID);
}