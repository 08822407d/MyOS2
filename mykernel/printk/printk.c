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
#include <linux/kernel/jiffies.h>
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

#include "printk_ringbuffer.h"
// #include "console_cmdline.h"
// #include "braille.h"
#include "internal.h"


#include <linux/kernel/kconfig.h>



/*
 * The printk log buffer consists of a sequenced collection of records, each
 * containing variable length message text. Every record also contains its
 * own meta-data (@info).
 *
 * Every record meta-data carries the timestamp in microseconds, as well as
 * the standard userspace syslog level and syslog facility. The usual kernel
 * messages use LOG_KERN; userspace-injected messages always carry a matching
 * syslog facility, by default LOG_USER. The origin of every message can be
 * reliably determined that way.
 *
 * The human readable log message of a record is available in @text, the
 * length of the message text in @text_len. The stored message is not
 * terminated.
 *
 * Optionally, a record can carry a dictionary of properties (key/value
 * pairs), to provide userspace with a machine-readable message context.
 *
 * Examples for well-defined, commonly used property names are:
 *   DEVICE=b12:8               device identifier
 *                                b12:8         block dev_t
 *                                c127:3        char dev_t
 *                                n8            netdev ifindex
 *                                +sound:card0  subsystem:devname
 *   SUBSYSTEM=pci              driver-core subsystem name
 *
 * Valid characters in property names are [a-zA-Z0-9.-_]. Property names
 * and values are terminated by a '\0' character.
 *
 * Example of record values:
 *   record.text_buf                = "it's a line" (unterminated)
 *   record.info.seq                = 56
 *   record.info.ts_nsec            = 36863
 *   record.info.text_len           = 11
 *   record.info.facility           = 0 (LOG_KERN)
 *   record.info.flags              = 0
 *   record.info.level              = 3 (LOG_ERR)
 *   record.info.caller_id          = 299 (task 299)
 *   record.info.dev_info.subsystem = "pci" (terminated)
 *   record.info.dev_info.device    = "+pci:0000:00:01.0" (terminated)
 *
 * The 'printk_info_s' buffer must never be directly exported to
 * userspace, it is a kernel-private implementation detail that might
 * need to be changed in the future, when the requirements change.
 *
 * /dev/kmsg exports the structured data in the following line format:
 *   "<level>,<sequnum>,<timestamp>,<contflag>[,additional_values, ... ];<message text>\n"
 *
 * Users of the export format should ignore possible additional values
 * separated by ',', and find the message after the ';' character.
 *
 * The optional key/value pairs are attached as continuation lines starting
 * with a space character and terminated by a newline. All possible
 * non-prinatable characters are escaped in the "\xff" notation.
 */

// /* syslog_lock protects syslog_* variables and write access to clear_seq. */
// static DEFINE_MUTEX(syslog_lock);

// DECLARE_WAIT_QUEUE_HEAD(log_wait);
// /* All 3 protected by @syslog_lock. */
// /* the next printk record to read by syslog(READ) or /proc/kmsg */
// static u64 syslog_seq;
// static size_t syslog_partial;
// static bool syslog_time;

// struct latched_seq {
// 	seqcount_latch_t	latch;
// 	u64			val[2];
// };

// /*
//  * The next printk record to read after the last 'clear' command. There are
//  * two copies (updated with seqcount_latch) so that reads can locklessly
//  * access a valid value. Writers are synchronized by @syslog_lock.
//  */
// static struct latched_seq clear_seq = {
// 	.latch		= SEQCNT_LATCH_ZERO(clear_seq.latch),
// 	.val[0]		= 0,
// 	.val[1]		= 0,
// };

#define LOG_LEVEL(v)		((v) & 0x07)
#define LOG_FACILITY(v)		((v) >> 3 & 0xff)

/* record buffer */
#define LOG_ALIGN			__alignof__(unsigned long)
#define __LOG_BUF_LEN		(1 << CONFIG_LOG_BUF_SHIFT)
#define LOG_BUF_LEN_MAX		(u32)(1 << 31)
static char		__log_buf[__LOG_BUF_LEN] __aligned(LOG_ALIGN);
static char		*log_buf = __log_buf;
static u32		log_buf_len = __LOG_BUF_LEN;
	

/*
 * Define the average message size. This only affects the number of
 * descriptors that will be available. Underestimating is better than
 * overestimating (too many available descriptors is better than not enough).
 */
#define PRB_AVGBITS 5	/* 32 character average length */

#if CONFIG_LOG_BUF_SHIFT <= PRB_AVGBITS
#error CONFIG_LOG_BUF_SHIFT value too small.
#endif
_DEFINE_PRINTKRB(printk_rb_static, CONFIG_LOG_BUF_SHIFT - PRB_AVGBITS,
		PRB_AVGBITS, &__log_buf[0]);

static prb_s printk_rb_dynamic;

static prb_s *prb = &printk_rb_static;



/**
 * printk_parse_prefix - Parse level and control flags.
 *
 * @text:     The terminated text message.
 * @level:    A pointer to the current level value, will be updated.
 * @flags:    A pointer to the current printk_info flags, will be updated.
 *
 * @level may be NULL if the caller is not interested in the parsed value.
 * Otherwise the variable pointed to by @level must be set to
 * LOGLEVEL_DEFAULT in order to be updated with the parsed value.
 *
 * @flags may be NULL if the caller is not interested in the parsed value.
 * Otherwise the variable pointed to by @flags will be OR'd with the parsed
 * value.
 *
 * Return: The length of the parsed level and control flags.
 */
u16 printk_parse_prefix(const char *text, int *level,
			enum printk_info_flags *flags)
{
	u16 prefix_len = 0;
	int kern_level;

	while (*text) {
		kern_level = printk_get_level(text);
		if (!kern_level)
			break;

		switch (kern_level) {
		case '0' ... '7':
			if (level && *level == LOGLEVEL_DEFAULT)
				*level = kern_level - '0';
			break;
		case 'c':	/* KERN_CONT */
			if (flags)
				*flags |= LOG_CONT;
		}

		prefix_len += 2;
		text += 2;
	}

	return prefix_len;
}

__printf(5, 0)
static u16 printk_sprint(char *text, u16 size, int facility,
			 enum printk_info_flags *flags, const char *fmt,
			 va_list args)
{
	u16 text_len;

	text_len = vscnprintf(text, size, fmt, args);

	/* Mark and strip a trailing newline. */
	if (text_len && text[text_len - 1] == '\n') {
		text_len--;
		*flags |= LOG_NEWLINE;
	}

	/* Strip log level and control flags. */
	if (facility == 0) {
		u16 prefix_len;

		prefix_len = printk_parse_prefix(text, NULL, NULL);
		if (prefix_len) {
			text_len -= prefix_len;
			memmove(text, text + prefix_len, text_len);
		}
	}

	// trace_console(text, text_len);

	return text_len;
}

__printf(4, 0)
int vprintk_store(int facility, int level,
		const dev_printk_info_s *dev_info,
		const char *fmt, va_list args)
{
	prb_rsvd_ent_s e;
	enum printk_info_flags flags = 0;
	printk_record_s r;
	unsigned long irqflags;
	u16 trunc_msg_len = 0;
	char prefix_buf[8];
	u8 *recursion_ptr;
	u16 reserve_size;
	va_list args2;
	u32 caller_id;
	u16 text_len;
	int ret = 0;
	u64 ts_nsec;

	// if (!printk_enter_irqsave(recursion_ptr, irqflags))
	// 	return 0;

	/*
	 * Since the duration of printk() can vary depending on the message
	 * and state of the ringbuffer, grab the timestamp now so that it is
	 * close to the call of printk(). This provides a more deterministic
	 * timestamp with respect to the caller.
	 */
	ts_nsec = local_clock();

	// caller_id = printk_caller_id();

	/*
	 * The sprintf needs to come first since the syslog prefix might be
	 * passed in as a parameter. An extra byte must be reserved so that
	 * later the vscnprintf() into the reserved buffer has room for the
	 * terminating '\0', which is not counted by vsnprintf().
	 */
	va_copy(args2, args);
	reserve_size = vsnprintf(&prefix_buf[0], sizeof(prefix_buf), fmt, args2) + 1;
	va_end(args2);

	if (reserve_size > PRINTKRB_RECORD_MAX)
		reserve_size = PRINTKRB_RECORD_MAX;

	/* Extract log level or control flags. */
	if (facility == 0)
		printk_parse_prefix(&prefix_buf[0], &level, &flags);

	// if (level == LOGLEVEL_DEFAULT)
	// 	level = default_message_loglevel;

	if (dev_info)
		flags |= LOG_NEWLINE;

	// if (flags & LOG_CONT) {
	// 	prb_rec_init_wr(&r, reserve_size);
	// 	if (prb_reserve_in_last(&e, prb, &r, caller_id, PRINTKRB_RECORD_MAX)) {
	// 		text_len = printk_sprint(&r.text_buf[r.info->text_len], reserve_size,
	// 					 facility, &flags, fmt, args);
	// 		r.info->text_len += text_len;

	// 		if (flags & LOG_NEWLINE) {
	// 			r.info->flags |= LOG_NEWLINE;
	// 			prb_final_commit(&e);
	// 		} else {
	// 			prb_commit(&e);
	// 		}

	// 		ret = text_len;
	// 		goto out;
	// 	}
	// }

	/*
	 * Explicitly initialize the record before every prb_reserve() call.
	 * prb_reserve_in_last() and prb_reserve() purposely invalidate the
	 * structure when they fail.
	 */
	prb_rec_init_wr(&r, reserve_size);
	// if (!prb_reserve(&e, prb, &r)) {
	// 	/* truncate the message if it is too long for empty buffer */
	// 	truncate_msg(&reserve_size, &trunc_msg_len);

	// 	prb_rec_init_wr(&r, reserve_size + trunc_msg_len);
	// 	if (!prb_reserve(&e, prb, &r))
	// 		goto out;
	// }

	/* fill message */
	text_len = printk_sprint(&r.text_buf[0], reserve_size, facility, &flags, fmt, args);
	// if (trunc_msg_len)
	// 	memcpy(&r.text_buf[text_len], trunc_msg, trunc_msg_len);
	r.info->text_len = text_len + trunc_msg_len;
	r.info->facility = facility;
	r.info->level = level & 7;
	r.info->flags = flags & 0x1f;
	r.info->ts_nsec = ts_nsec;
	r.info->caller_id = caller_id;
	if (dev_info)
		memcpy(&r.info->dev_info, dev_info, sizeof(r.info->dev_info));

	// /* A message without a trailing newline can be continued. */
	// if (!(flags & LOG_NEWLINE))
	// 	prb_commit(&e);
	// else
	// 	prb_final_commit(&e);

	// ret = text_len + trunc_msg_len;
out:
	// printk_exit_irqrestore(recursion_ptr, irqflags);
	return ret;
}

asmlinkage int vprintk_emit(int facility, int level,
		const dev_printk_info_s *dev_info,
		const char *fmt, va_list args)
{
	int printed_len;
	bool in_sched = false;

	// /* Suppress unimportant messages after panic happens */
	// if (unlikely(suppress_printk))
	// 	return 0;

	// if (unlikely(suppress_panic_printk) &&
	//     atomic_read(&panic_cpu) != raw_smp_processor_id())
	// 	return 0;

	if (level == LOGLEVEL_SCHED) {
		level = LOGLEVEL_DEFAULT;
		in_sched = true;
	}

	// printk_delay(level);

	printed_len = vprintk_store(facility, level, dev_info, fmt, args);

	// /* If called from the scheduler, we can not call up(). */
	// if (!in_sched) {
	// 	/*
	// 	 * The caller may be holding system-critical or
	// 	 * timing-sensitive locks. Disable preemption during
	// 	 * printing of all remaining records to all consoles so that
	// 	 * this context can return as soon as possible. Hopefully
	// 	 * another printk() caller will take over the printing.
	// 	 */
	// 	preempt_disable();
	// 	/*
	// 	 * Try to acquire and then immediately release the console
	// 	 * semaphore. The release will print out buffers. With the
	// 	 * spinning variant, this context tries to take over the
	// 	 * printing from another printing context.
	// 	 */
	// 	if (console_trylock_spinning())
	// 		console_unlock();
	// 	preempt_enable();
	// }

	// wake_up_klogd();
	return printed_len;
}



/*
 * content of <printk/printk_safe.c>
 *===================================================================================
 */
// static DEFINE_PER_CPU(int, printk_context);

// /* Can be preempted by NMI. */
// void __printk_safe_enter(void)
// {
// 	this_cpu_inc(printk_context);
// }

// /* Can be preempted by NMI. */
// void __printk_safe_exit(void)
// {
// 	this_cpu_dec(printk_context);
// }

asmlinkage int vprintk(const char *fmt, va_list args)
{
	// /*
	//  * Use the main logbuf even in NMI. But avoid calling console
	//  * drivers that might have their own locks.
	//  */
	// if (this_cpu_read(printk_context) || in_nmi()) {
	// 	int len;

	// 	len = vprintk_store(0, LOGLEVEL_DEFAULT, NULL, fmt, args);
	// 	defer_console_output();
	// 	return len;
	// }

	/* No obstacles. */
	// int vprintk_default(const char *fmt, va_list args)
	// {
		return vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, fmt, args);
	// }
}
// /*
//  *===================================================================================
//  */

asmlinkage __visible int _printk(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

	return r;
}



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