
// SPDX-License-Identifier: GPL-2.0
/*
 *  Kernel internal timers
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  1997-01-28  Modified by Finn Arne Gangstad to make timers scale better.
 *
 *  1997-09-10  Updated NTP code according to technical memorandum Jan '96
 *              "A Kernel Model for Precision Timekeeping" by Dave Mills
 *  1998-12-24  Fixed a xtime SMP race (we need the xtime_lock rw spinlock to
 *              serialize accesses to xtime/lost_ticks).
 *                              Copyright (C) 1998  Andrea Arcangeli
 *  1999-03-10  Improved NTP compatibility by Ulrich Windl
 *  2002-05-31	Move sys_sysinfo here and make its locking sane, Robert Love
 *  2000-10-05  Implemented scalable SMP per-CPU timer handling.
 *                              Copyright (C) 2000, 2001, 2002  Ingo Molnar
 *              Designed by David S. Miller, Alexey Kuznetsov and Ingo Molnar
 */

// #include <linux/kernel_stat.h>
// #include <linux/export.h>
// #include <linux/interrupt.h>
#include <linux/smp/percpu.h>
#include <linux/init/init.h>
#include <linux/mm/mm.h>
// #include <linux/swap.h>
// #include <linux/pid_namespace.h>
// #include <linux/notifier.h>
// #include <linux/thread_info.h>
#include <linux/kernel/time.h>
#include <linux/kernel/jiffies.h>
// #include <linux/posix-timers.h>
#include <linux/kernel/cpu.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/delay.h>
// #include <linux/tick.h>
// #include <linux/kallsyms.h>
// #include <linux/irq_work.h>
#include <linux/sched/signal.h>
// #include <linux/sched/sysctl.h>
// #include <linux/sched/nohz.h>
#include <linux/sched/debug.h>
#include <linux/kernel/slab.h>
// #include <linux/compat.h>
// #include <linux/random.h>
// #include <linux/sysctl.h>

#include <linux/kernel/uaccess.h>
#include <asm/unistd.h>
#include <asm/div64.h>
// #include <asm/timex.h>
#include <asm/io.h>

// #include "tick-internal.h"

// #define CREATE_TRACE_POINTS
// #include <trace/events/timer.h>

__visible u64 jiffies_64 __cacheline_aligned_in_smp = INITIAL_JIFFIES;