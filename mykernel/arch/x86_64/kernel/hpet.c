// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/export.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/hpet.h>
#include <linux/kernel/cpu.h>

#include <asm/hpet.h>
#include <asm/time.h>

#include <linux/kernel/mm.h>
#include <linux/kernel/jiffies.h>

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
	uint			num;
	uint			cpu;
	uint			irq;
	uint			in_use;
	enum hpet_mode	mode;
	uint			boot_cfg;
	char			name[10];
} hpet_channel_s;

typedef struct hpet_base {
	uint			nr_channels;
	uint			nr_clockevents;
	uint			boot_cfg;
	hpet_channel_s	*channels;
} hpet_base_s;

#define HPET_MASK				CLOCKSOURCE_MASK(32)

#define HPET_MIN_CYCLES			128
#define HPET_MIN_PROG_DELTA		(HPET_MIN_CYCLES + (HPET_MIN_CYCLES >> 1))

/*
 * HPET address is set in acpi/boot.c, when an ACPI entry exists
 */
ulong					hpet_address;

static void __iomem		*hpet_virt_address;
static hpet_base_s		hpet_base;

static ulong			hpet_freq;


inline uint hpet_readl(uint a) {
	return readl(hpet_virt_address + a);
}

static inline void hpet_writel(uint d, uint a) {
	writel(d, hpet_virt_address + a);
}	

static inline void hpet_set_mapping(void) {
	hpet_address = 0xfed00000;
	hpet_virt_address = myos_ioremap(hpet_address, HPET_MMAP_SIZE);
	flush_tlb_local();
}

static inline void hpet_clear_mapping(void) {
	iounmap(hpet_virt_address);
	hpet_virt_address = NULL;
}

static void _hpet_print_config(const char *function, int line)
{
	u32 i, id, period, cfg, status, channels, l, h;

	pr_info("%s(%d):\n", function, line);

	id = hpet_readl(HPET_ID);
	period = hpet_readl(HPET_PERIOD);
	pr_info("ID: 0x%x, PERIOD: 0x%x\n", id, period);

	cfg = hpet_readl(HPET_CFG);
	status = hpet_readl(HPET_STATUS);
	pr_info("CFG: 0x%x, STATUS: 0x%x\n", cfg, status);

	l = hpet_readl(HPET_COUNTER);
	h = hpet_readl(HPET_COUNTER+4);
	pr_info("COUNTER_l: 0x%x, COUNTER_h: 0x%x\n", l, h);

	channels = ((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1;

	for (i = 0; i < channels; i++) {
		l = hpet_readl(HPET_Tn_CFG(i));
		h = hpet_readl(HPET_Tn_CFG(i)+4);
		pr_info("T%d: CFG_l: 0x%x, CFG_h: 0x%x\n", i, l, h);

		l = hpet_readl(HPET_Tn_CMP(i));
		h = hpet_readl(HPET_Tn_CMP(i)+4);
		pr_info("T%d: CMP_l: 0x%x, CMP_h: 0x%x\n", i, l, h);

		l = hpet_readl(HPET_Tn_ROUTE(i));
		h = hpet_readl(HPET_Tn_ROUTE(i)+4);
		pr_info("T%d ROUTE_l: 0x%x, ROUTE_h: 0x%x\n", i, l, h);
	}
}

#define hpet_print_config()								\
		do {											\
			/* if (hpet_verbose) */							\
				_hpet_print_config(__func__, __LINE__);	\
		} while (0)



/* Common HPET functions */
static void hpet_stop_counter(void)
{
	u32 cfg = hpet_readl(HPET_CFG);

	cfg &= ~HPET_CFG_ENABLE;
	hpet_writel(cfg, HPET_CFG);
}

static void hpet_reset_counter(void)
{
	hpet_writel(0, HPET_COUNTER);
	hpet_writel(0, HPET_COUNTER + 4);
}

static void hpet_start_counter(void)
{
	uint cfg = hpet_readl(HPET_CFG);

	cfg |= HPET_CFG_ENABLE;
	hpet_writel(cfg, HPET_CFG);
}

static void hpet_restart_counter(void)
{
	hpet_stop_counter();
	hpet_reset_counter();
	hpet_start_counter();
}


static void hpet_enable_legacy_int(void)
{
	uint cfg = hpet_readl(HPET_CFG);

	cfg |= HPET_CFG_LEGACY;
	hpet_writel(cfg, HPET_CFG);
	// hpet_legacy_int_enabled = true;
}

// static int hpet_clkevt_set_state_periodic(struct clock_event_device *evt)
// {
// 	unsigned int channel = clockevent_to_channel(evt)->num;
// 	unsigned int cfg, cmp, now;
// 	uint64_t delta;

// 	hpet_stop_counter();
// 	delta = ((uint64_t)(NSEC_PER_SEC / HZ)) * evt->mult;
// 	delta >>= evt->shift;
// 	now = hpet_readl(HPET_COUNTER);
// 	cmp = now + (unsigned int)delta;
// 	cfg = hpet_readl(HPET_Tn_CFG(channel));
// 	cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL |
// 	       HPET_TN_32BIT;
// 	hpet_writel(cfg, HPET_Tn_CFG(channel));
// 	hpet_writel(cmp, HPET_Tn_CMP(channel));
// 	udelay(1);
// 	/*
// 	 * HPET on AMD 81xx needs a second write (with HPET_TN_SETVAL
// 	 * cleared) to T0_CMP to set the period. The HPET_TN_SETVAL
// 	 * bit is automatically cleared after the first write.
// 	 * (See AMD-8111 HyperTransport I/O Hub Data Sheet,
// 	 * Publication # 24674)
// 	 */
// 	hpet_writel((unsigned int)delta, HPET_Tn_CMP(channel));
// 	hpet_start_counter();
// 	hpet_print_config();

// 	return 0;
// }
static int myos_hpet_clkevt_set_state_periodic(void)
{
	uint channel = 0;
	u32 mult = 429496730;
	u32 shift = 32;
	uint cfg, cmp, now;
	uint64_t delta;

	hpet_stop_counter();
	delta = ((uint64_t)(NSEC_PER_SEC / HZ)) * mult;
	delta >>= shift;
	now = hpet_readl(HPET_COUNTER);
	cmp = now + (uint)delta;
	cfg = hpet_readl(HPET_Tn_CFG(channel));
	// cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL | HPET_TN_32BIT;
	cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL;
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
	udelay(1);
	/*
	 * HPET on AMD 81xx needs a second write (with HPET_TN_SETVAL
	 * cleared) to T0_CMP to set the period. The HPET_TN_SETVAL
	 * bit is automatically cleared after the first write.
	 * (See AMD-8111 HyperTransport I/O Hub Data Sheet,
	 * Publication # 24674)
	 */
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
	hpet_start_counter();

	return 0;
}


/*
 * Reading the HPET counter is a very slow operation. If a large number of
 * CPUs are trying to access the HPET counter simultaneously, it can cause
 * massive delays and slow down system performance dramatically. This may
 * happen when HPET is the default clock source instead of TSC. For a
 * really large system with hundreds of CPUs, the slowdown may be so
 * severe, that it can actually crash the system because of a NMI watchdog
 * soft lockup, for example.
 *
 * If multiple CPUs are trying to access the HPET counter at the same time,
 * we don't actually need to read the counter multiple times. Instead, the
 * other CPUs can use the counter value read by the first CPU in the group.
 *
 * This special feature is only enabled on x86-64 systems. It is unlikely
 * that 32-bit x86 systems will have enough CPUs to require this feature
 * with its associated locking overhead. We also need 64-bit atomic read.
 *
 * The lock and the HPET value are stored together and can be read in a
 * single atomic 64-bit read. It is explicitly assumed that arch_spinlock_t
 * is 32 bits in size.
 */
union hpet_lock {
	struct {
		arch_spinlock_t	lock;
		u32				value;
	};
	u64					lockval;
};

static union hpet_lock hpet __cacheline_aligned = {
	{ .lock = __ARCH_SPIN_LOCK_UNLOCKED, },
};

static u64 read_hpet(clocksrc_s *cs)
{
	// unsigned long flags;
	// union hpet_lock old, new;

	// BUILD_BUG_ON(sizeof(union hpet_lock) != 8);

	// /*
	//  * Read HPET directly if in NMI.
	//  */
	// if (in_nmi())
		return (u64)hpet_readl(HPET_COUNTER);

// 	/*
// 	 * Read the current state of the lock and HPET value atomically.
// 	 */
// 	old.lockval = READ_ONCE(hpet.lockval);

// 	if (arch_spin_is_locked(&old.lock))
// 		goto contended;

// 	local_irq_save(flags);
// 	if (arch_spin_trylock(&hpet.lock)) {
// 		new.value = hpet_readl(HPET_COUNTER);
// 		/*
// 		 * Use WRITE_ONCE() to prevent store tearing.
// 		 */
// 		WRITE_ONCE(hpet.value, new.value);
// 		arch_spin_unlock(&hpet.lock);
// 		local_irq_restore(flags);
// 		return (u64)new.value;
// 	}
// 	local_irq_restore(flags);

// contended:
// 	/*
// 	 * Contended case
// 	 * --------------
// 	 * Wait until the HPET value change or the lock is free to indicate
// 	 * its value is up-to-date.
// 	 *
// 	 * It is possible that old.value has already contained the latest
// 	 * HPET value while the lock holder was in the process of releasing
// 	 * the lock. Checking for lock state change will enable us to return
// 	 * the value immediately instead of waiting for the next HPET reader
// 	 * to come along.
// 	 */
// 	do {
// 		cpu_relax();
// 		new.lockval = READ_ONCE(hpet.lockval);
// 	} while ((new.value == old.value) && arch_spin_is_locked(&new.lock));

// 	return (u64)new.value;
}

static clocksrc_s clocksource_hpet = {
	.name		= "hpet",
	.rating		= 250,
	.read		= read_hpet,
	.mask		= HPET_MASK,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	// .resume		= hpet_resume_counter,
};

/*
 * AMD SB700 based systems with spread spectrum enabled use a SMM based
 * HPET emulation to provide proper frequency setting.
 *
 * On such systems the SMM code is initialized with the first HPET register
 * access and takes some time to complete. During this time the config
 * register reads 0xffffffff. We check for max 1000 loops whether the
 * config register reads a non-0xffffffff value to make sure that the
 * HPET is up and running before we proceed any further.
 *
 * A counting loop is safe, as the HPET access takes thousands of CPU cycles.
 *
 * On non-SB700 based machines this check is only done once and has no
 * side effects.
 */
static bool __init hpet_cfg_working(void)
{
	int i;

	for (i = 0; i < 1000; i++) {
		if (hpet_readl(HPET_CFG) != 0xFFFFFFFF)
			return true;
	}

	pr_warn("Config register invalid. Disabling HPET\n");
	return false;
}

static bool __init hpet_counting(void)
{
	u64 start, now, t1;

	hpet_restart_counter();

	t1 = hpet_readl(HPET_COUNTER);
	start = rdtsc();

	/*
	 * We don't know the TSC frequency yet, but waiting for
	 * 200000 TSC cycles is safe:
	 * 4 GHz == 50us
	 * 1 GHz == 200us
	 */
	do {
		if (t1 != hpet_readl(HPET_COUNTER))
			return true;
		now = rdtsc();
	} while ((now - start) < 200000UL);

	pr_warn("Counter not counting. HPET disabled\n");
	return false;
}



extern void myos_HPET_init(void);
/**
 * hpet_enable - Try to setup the HPET timer. Returns 1 on success.
 */
int __init hpet_enable(void)
{
	u32 hpet_period, cfg, id, irq;
	uint i, channels;
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
	// hpet_print_config();

	/* This is the HPET channel number which is zero based */
	channels = ((id & HPET_ID_NUMBER) >> HPET_ID_NUMBER_SHIFT) + 1;

	// /*
	//  * The legacy routing mode needs at least two channels, tick timer
	//  * and the rtc emulation channel.
	//  */
	// if (IS_ENABLED(CONFIG_HPET_EMULATE_RTC) && channels < 2)
	// 	goto out_nohpet;

	hc = kcalloc(channels, sizeof(*hc), GFP_KERNEL);
	if (!hc) {
		pr_warn("Disabling HPET.\n");
		goto out_nohpet;
	}
	hpet_base.channels = hc;
	hpet_base.nr_channels = channels;

	/* Read, store and sanitize the global configuration */
	cfg = hpet_readl(HPET_CFG);
	hpet_base.boot_cfg = cfg;
	cfg &= ~(HPET_CFG_ENABLE | HPET_CFG_LEGACY);
	hpet_writel(cfg, HPET_CFG);
	if (cfg)
		pr_warn("Global config: Unknown bits %#x\n", cfg);

	/* Read, store and sanitize the per channel configuration */
	for (i = 0; i < channels; i++, hc++) {
		hc->num = i;

		cfg = hpet_readl(HPET_Tn_CFG(i));
		hc->boot_cfg = cfg;
		irq = (cfg & Tn_INT_ROUTE_CNF_MASK) >> Tn_INT_ROUTE_CNF_SHIFT;
		hc->irq = irq;

		cfg &= ~(HPET_TN_ENABLE | HPET_TN_LEVEL | HPET_TN_FSB);
		hpet_writel(cfg, HPET_Tn_CFG(i));

		cfg &= ~(HPET_TN_PERIODIC | HPET_TN_PERIODIC_CAP
			 | HPET_TN_64BIT_CAP | HPET_TN_32BIT | HPET_TN_ROUTE
			 | HPET_TN_FSB | HPET_TN_FSB_CAP);
		if (cfg)
			pr_warn("Channel #%u config: Unknown bits %#x\n", i, cfg);
	}
	// hpet_print_config();

	/*
	 * Validate that the counter is counting. This needs to be done
	 * after sanitizing the config registers to properly deal with
	 * force enabled HPETs.
	 */
	if (!hpet_counting())
		goto out_nohpet;

	// if (tsc_clocksource_watchdog_disabled())
	// 	clocksource_hpet.flags |= CLOCK_SOURCE_MUST_VERIFY;
	clocksource_register_hz(&clocksource_hpet, (u32)hpet_freq);

	if (id & HPET_ID_LEGSUP) {
		// hpet_legacy_clockevent_register(&hpet_base.channels[0]);
		myos_hpet_clkevt_set_state_periodic();
		myos_HPET_init();
		hpet_base.channels[0].mode = HPET_MODE_LEGACY;
		// if (IS_ENABLED(CONFIG_HPET_EMULATE_RTC))
		// 	hpet_base.channels[1].mode = HPET_MODE_LEGACY;
		return 1;
	}
	return 0;

out_nohpet:
	kfree(hpet_base.channels);
	hpet_base.channels = NULL;
	hpet_base.nr_channels = 0;
	hpet_clear_mapping();
	hpet_address = 0;
	return 0;
}



#include <linux/device/tty.h>
#include <linux/kernel/jiffies.h>

#include <asm/apicdef.h>

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
void HPET_handler(unsigned long parameter, pt_regs_s * regs)
{
	jiffies++;
	do_timer(1);

	// timer_s * tmr = timer_lhdr.anchor.next->owner_p;
	// if(tmr->expire_jiffies <= jiffies)
	// 	set_softirq_status(HPET_TIMER0_IRQ);

	char buf[20];
	memset(buf, 0 , sizeof(buf));
	snprintf(buf, sizeof(buf), "(HPET: %08ld)   ", jiffies);
	myos_tty_write_color_at(buf, sizeof(buf), BLACK, GREEN, 48, 0);
}
	
void myos_HPET_init()
{
	ioapic_retentry_T entry;
	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
	entry.reserved = 0;

	entry.dst.physical.reserved1 = 0;
	entry.dst.physical.phy_dest = 0;
	entry.dst.physical.reserved2 = 0;

	register_irq(HPET_TIMER0_IRQ, &entry , "HPET0",
			0, &HPET_int_controller, &HPET_handler);
	
	u32 accuracy = *(u32 *)(hpet_virt_address + 4);
}