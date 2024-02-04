// SPDX-License-Identifier: GPL-2.0
/*
 *  Kernel timekeeping code and accessor functions. Based on code from
 *  timer.c, moved in commit 8524070b7982.
 */
#include <linux/kernel/timekeeper_internal.h>
// #include <linux/module.h>
// #include <linux/interrupt.h>
#include <linux/smp/percpu.h>
#include <linux/init/init.h>
#include <linux/mm/mm.h>
#include <linux/kernel/nmi.h>
#include <linux/kernel/sched.h>
// #include <linux/sched/loadavg.h>
#include <linux/sched/clock.h>
// #include <linux/syscore_ops.h>
#include <linux/kernel/clocksource.h>
#include <linux/kernel/jiffies.h>
#include <linux/kernel/time.h>
// #include <linux/timex.h>
// #include <linux/tick.h>
// #include <linux/stop_machine.h>
// #include <linux/pvclock_gtod.h>
#include <linux/kernel/compiler.h>
// #include <linux/audit.h>
// #include <linux/random.h>

// #include "tick-internal.h"
// #include "ntp_internal.h"
#include "timekeeping_internal.h"


#include <linux/kernel/timekeeping.h>


#define TK_CLEAR_NTP		(1 << 0)
#define TK_MIRROR			(1 << 1)
#define TK_CLOCK_WAS_SET	(1 << 2)

enum timekeeping_adv_mode {
	/* Update timekeeper when a tick has passed */
	TK_ADV_TICK,
	/* Update timekeeper on a direct frequency change */
	TK_ADV_FREQ
};

/*
 * The most important data for readout fits into a single 64 byte
 * cache line.
 */
static struct {
	// seqcount_raw_spinlock_t	seq;
	timekeeper_s	timekeeper;
} tk_core ____cacheline_aligned = {
	// .seq = SEQCNT_RAW_SPINLOCK_ZERO(tk_core.seq, &timekeeper_lock),
};

static struct timekeeper shadow_timekeeper;

/* flag for if timekeeping is suspended */
int __read_mostly timekeeping_suspended;



/*
 * timekeeping_init - Initializes the clocksource and common timekeeping values
 */
void __init timekeeping_init(void)
{
	timespec64_s wall_time, boot_offset, wall_to_mono;
	timekeeper_s *tk = &tk_core.timekeeper;
	clocksrc_s *clock;
	unsigned long flags;

	read_persistent_clock64(&wall_time);
	boot_offset = ns_to_timespec64(local_clock());
}



/*
 * Must hold jiffies_lock
 */
void do_timer(unsigned long ticks)
{
	jiffies_64 += ticks;
	// calc_global_load();
}