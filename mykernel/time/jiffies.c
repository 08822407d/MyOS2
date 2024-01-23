// SPDX-License-Identifier: GPL-2.0+
/*
 * This file contains the jiffies based clocksource.
 *
 * Copyright (C) 2004, 2005 IBM, John Stultz (johnstul@us.ibm.com)
 */
#include <linux/kernel/clocksource.h>
#include <linux/kernel/jiffies.h>
// #include <linux/module.h>
#include <linux/init/init.h>

// #include "timekeeping.h"
// #include "tick-internal.h"


static u64 jiffies_read(clocksource_s *cs)
{
	return (u64) jiffies;
}

// /*
//  * The Jiffies based clocksource is the lowest common
//  * denominator clock source which should function on
//  * all systems. It has the same coarse resolution as
//  * the timer interrupt frequency HZ and it suffers
//  * inaccuracies caused by missed or lost timer
//  * interrupts and the inability for the timer
//  * interrupt hardware to accurately tick at the
//  * requested HZ value. It is also not recommended
//  * for "tick-less" systems.
//  */
// static struct clocksource clocksource_jiffies = {
// 	.name			= "jiffies",
// 	.rating			= 1, /* lowest valid rating*/
// 	.uncertainty_margin	= 32 * NSEC_PER_MSEC,
// 	.read			= jiffies_read,
// 	.mask			= CLOCKSOURCE_MASK(32),
// 	.mult			= TICK_NSEC << JIFFIES_SHIFT, /* details above */
// 	.shift			= JIFFIES_SHIFT,
// 	.max_cycles		= 10,
// };