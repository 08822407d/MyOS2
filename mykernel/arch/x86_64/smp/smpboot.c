// SPDX-License-Identifier: GPL-2.0-only
/*
 * Common SMP CPU bringup/teardown functions
 */
#include <linux/kernel/cpu.h>
#include <linux/kernel/err.h>
#include <linux/smp/smp.h>
#include <linux/kernel/delay.h>
#include <linux/init/init.h>
#include <linux/kernel/lib_api.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/sched_api.h>
// #include <linux/sched/task.h>
#include <linux/kernel/export.h>
#include <linux/smp/percpu.h>
// #include <linux/kthread.h>
// #include <linux/smpboot.h>


#include <obsolete/glo.h>


void __init idle_thread_set_boot_cpu(void)
{
	per_cpu(idle_threads, smp_processor_id()) = current;
}