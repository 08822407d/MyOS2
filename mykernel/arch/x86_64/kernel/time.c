// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright (c) 1991,1992,1995  Linus Torvalds
 *  Copyright (c) 1994  Alan Modra
 *  Copyright (c) 1995  Markus Kuhn
 *  Copyright (c) 1996  Ingo Molnar
 *  Copyright (c) 1998  Andrea Arcangeli
 *  Copyright (c) 2002,2006  Vojtech Pavlik
 *  Copyright (c) 2003  Andi Kleen
 *
 */

#include <linux/kernel/clocksource.h>
// #include <linux/clockchips.h>
// #include <linux/interrupt.h>
// #include <linux/irq.h>
// #include <linux/i8253.h>
#include <linux/kernel/time.h>
// #include <linux/export.h>

// #include <asm/vsyscall.h>
// #include <asm/x86_init.h>
// #include <asm/i8259.h>
#include <asm/timer.h>
#include <asm/hpet.h>
#include <asm/time.h>



/* Default timer init function */
void __init hpet_time_init(void)
{
	if (!hpet_enable()) {
		// if (!pit_timer_init())
			return;
	}

	// setup_default_timer_irq();
}

static __init void x86_late_time_init(void)
{
	// /*
	//  * Before PIT/HPET init, select the interrupt mode. This is required
	//  * to make the decision whether PIT should be initialized correct.
	//  */
	// x86_init.irqs.intr_mode_select();

	// /* Setup the legacy timers */
	// x86_init.timers.timer_init();
	hpet_time_init();

	// /*
	//  * After PIT/HPET timers init, set up the final interrupt mode for
	//  * delivering IRQs.
	//  */
	// x86_init.irqs.intr_mode_init();
	// tsc_init();

	// if (static_cpu_has(X86_FEATURE_WAITPKG))
	// 	use_tpause_delay();
}

/*
 * Initialize TSC and delay the periodic timer init to
 * late x86_late_time_init() so ioremap works.
 */
void __init time_init(void)
{
	late_time_init = x86_late_time_init;
}