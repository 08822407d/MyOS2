// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/kernel/printk.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 * Modified to make sys_syslog() more flexible: added commands to
 * return the last 4k of kernel messages, regardless of whether
 * they've been read or not.  Added option to suppress kernel printk's
 * to the console.  Added hook for sending the console messages
 * elsewhere, in preparation for a serial line console (someday).
 * Ted Ts'o, 2/11/93.
 * Modified for sysctl support, 1/8/97, Chris Horn.
 * Fixed SMP synchronization, 08/08/99, Manfred Spraul
 *     manfred@colorfullife.com
 * Rewrote bits to get rid of console_lock
 *	01Mar01 Andrew Morton
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/kernel.h>
#include <linux/mm/mm.h>
#include <linux/device/tty.h>
// #include <linux/tty_driver.h>
#include <linux/device/console.h>
#include <linux/init/init.h>
// #include <linux/jiffies.h>
#include <linux/kernel/nmi.h>
// #include <linux/module.h>
// #include <linux/moduleparam.h>
#include <linux/kernel/delay.h>
#include <linux/smp/smp.h>
// #include <linux/security.h>
#include <linux/mm/memblock.h>
#include <linux/kernel/syscalls.h>
// #include <linux/crash_core.h>
// #include <linux/ratelimit.h>
// #include <linux/kmsg_dump.h>
// #include <linux/syslog.h>
#include <linux/kernel/cpu.h>
// #include <linux/rculist.h>
// #include <linux/poll.h>
// #include <linux/irq_work.h>
#include <linux/kernel/ctype.h>
// #include <linux/uio.h>
#include <linux/sched/clock.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>

// #include <linux/uaccess.h>
#include <asm/sections.h>
#include <asm/tlbflush.h>

// #include <trace/events/initcall.h>
// #define CREATE_TRACE_POINTS
// #include <trace/events/printk.h>

// #include "printk_ringbuffer.h"
// #include "console_cmdline.h"
// #include "braille.h"
// #include "internal.h"



/*
 * Initialize the console device. This is called *early*, so
 * we can't necessarily depend on lots of kernel help here.
 * Just do some early initializations, and do the complex setup
 * later.
 */
void __init console_init(void)
{
extern void myos_init_video();
	myos_init_video();


	// int ret;
	// initcall_t call;
	// initcall_entry_t *ce;

	// /* Setup the default TTY line discipline. */
	// n_tty_init();

	// /*
	//  * set up the console device so that later boot sequences can
	//  * inform about problems etc..
	//  */
	// ce = __con_initcall_start;
	// trace_initcall_level("console");
	// while (ce < __con_initcall_end) {
	// 	call = initcall_from_entry(ce);
	// 	trace_initcall_start(call);
	// 	ret = call();
	// 	trace_initcall_finish(call, ret);
	// 	ce++;
	// }
}



#include <obsolete/ktypes.h>
#include <obsolete/printk.h>

extern framebuffer_s	framebuffer;
extern position_t Pos;

void myos_init_video()
{	
	Pos.FB_addr = (unsigned int *)framebuffer.FB_virbase;
	Pos.FB_length = framebuffer.FB_size;
	Pos.XResolution = framebuffer.X_Resolution;
	Pos.YResolution = framebuffer.Y_Resolution;

	Pos.XPosition = 0;
	Pos.YPosition = 0;
	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	spin_lock_init(&Pos.lock);
	// clean screen
	memset((void *)framebuffer.FB_virbase, 0, framebuffer.FB_size);

	char linebuf[4096] = {0};
	int i;
	for (i = 0; i < Pos.XResolution / Pos.XCharSize ; i++)
		linebuf[i] = ' ';
	color_printk(BLACK, GREEN, "%s", linebuf);
	color_printk(BLACK, GREEN, "\n");
}

// map VBE frame_buffer, this part should not be
// add into any memory manage unit
void myos_init_VBE_mapping()
{
	u64 start, end;
	start = PFN_PHYS(PFN_DOWN(framebuffer.FB_phybase));
	myos_ioremap(start, PFN_PHYS(PFN_UP(framebuffer.FB_size)));
	flush_tlb_local();
}