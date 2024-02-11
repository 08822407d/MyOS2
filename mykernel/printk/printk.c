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

// #include "printk_ringbuffer.h"
// #include "console_cmdline.h"
// #include "braille.h"
// #include "internal.h"


#include <linux/kernel/kconfig.h>
#include <obsolete/printk.h>


/*
 * Architectures can override it:
 */
void asmlinkage early_printk(const char *fmt, ...)
{
}

#define __LOG_BUF_LEN				(1 << CONFIG_LOG_BUF_SHIFT)

/* printk's without a loglevel use this.. */
// #define DEFAULT_MESSAGE_LOGLEVEL CONFIG_DEFAULT_MESSAGE_LOGLEVEL
#define DEFAULT_MESSAGE_LOGLEVEL	4

/* We show everything that is MORE important than this.. */
#define MINIMUM_CONSOLE_LOGLEVEL	1 /* Minimum loglevel we let people use */
#define DEFAULT_CONSOLE_LOGLEVEL	7 /* anything MORE serious than KERN_DEBUG */

// DECLARE_WAIT_QUEUE_HEAD(log_wait);

int console_printk[4] = {
	DEFAULT_CONSOLE_LOGLEVEL,	/* console_loglevel */
	DEFAULT_MESSAGE_LOGLEVEL,	/* default_message_loglevel */
	MINIMUM_CONSOLE_LOGLEVEL,	/* minimum_console_loglevel */
	DEFAULT_CONSOLE_LOGLEVEL,	/* default_console_loglevel */
};

// /*
//  * Low level drivers may need that to know if they can schedule in
//  * their unblank() callback or not. So let's export it.
//  */
// int oops_in_progress;

// /*
//  * console_sem protects the console_drivers list, and also
//  * provides serialisation for access to the entire console
//  * driver system.
//  */
// static DEFINE_SEMAPHORE(console_sem);
// struct console *console_drivers;

/*
 * This is used for debugging the mess that is the VT code by
 * keeping track if we have the console semaphore held. It's
 * definitely not the perfect debug tool (we don't know if _WE_
 * hold it are racing, but it helps tracking those weird code
 * path in the console code where we end up in places I want
 * locked without the console sempahore held
 */
static int console_locked, console_suspended;

/*
 * logbuf_lock protects log_buf, log_start, log_end, con_start and logged_chars
 * It is also used in interesting ways to provide interlocking in
 * console_unlock();.
 */
static DEFINE_SPINLOCK(logbuf_lock);

/*
 * The indices into log_buf are not constrained to log_buf_len - they
 * must be masked before subscripting
 */
static unsigned log_start;	/* Index into log_buf: next char to be read by syslog() */
static unsigned con_start;	/* Index into log_buf: next char to be sent to consoles */
static unsigned log_end;	/* Index into log_buf: most-recently-written-char + 1 */



static char		__log_buf[__LOG_BUF_LEN];
static char		*log_buf = __log_buf;
static int		log_buf_len = __LOG_BUF_LEN;
static unsigned	logged_chars; /* Number of chars produced since last read+clear operation */
static int		saved_console_loglevel = -1;

#define LOG_BUF_MASK	(log_buf_len-1)
#define LOG_BUF(idx)	(log_buf[(idx) & LOG_BUF_MASK])



/*
 * Write out chars from start to end - 1 inclusive
 */
static void _call_console_drivers(unsigned start,
		unsigned end, int msg_log_level)
{
	color_printk(WHITE, BLACK, "%s", start);
	// if ((msg_log_level < console_loglevel || ignore_loglevel) &&
	// 		console_drivers && start != end) {
	// 	if ((start & LOG_BUF_MASK) > (end & LOG_BUF_MASK)) {
	// 		/* wrapped write */
	// 		__call_console_drivers(start & LOG_BUF_MASK,
	// 					log_buf_len);
	// 		__call_console_drivers(0, end & LOG_BUF_MASK);
	// 	} else {
	// 		__call_console_drivers(start, end);
	// 	}
	// }
}

/*
 * Parse the syslog header <[0-9]*>. The decimal value represents 32bit, the
 * lower 3 bit are the log level, the rest are the log facility. In case
 * userspace passes usual userspace syslog messages to /dev/kmsg or
 * /dev/ttyprintk, the log prefix might contain the facility. Printk needs
 * to extract the correct log level for in-kernel processing, and not mangle
 * the original value.
 *
 * If a prefix is found, the length of the prefix is returned. If 'level' is
 * passed, it will be filled in with the log level without a possible facility
 * value. If 'special' is passed, the special printk prefix chars are accepted
 * and returned. If no valid header is found, 0 is returned and the passed
 * variables are not touched.
 */
static size_t log_prefix(const char *p, unsigned int *level, char *special)
{
	unsigned int lev = 0;
	char sp = '\0';
	size_t len;

	if (p[0] != '<' || !p[1])
		return 0;
	if (p[2] == '>') {
		/* usual single digit level number or special char */
		switch (p[1]) {
		case '0' ... '7':
			lev = p[1] - '0';
			break;
		case 'c': /* KERN_CONT */
		case 'd': /* KERN_DEFAULT */
			sp = p[1];
			break;
		default:
			return 0;
		}
		len = 3;
	} else {
		/* multi digit including the level and facility number */
		char *endp = NULL;

		if (p[1] < '0' && p[1] > '9')
			return 0;

		lev = (simple_strtoul(&p[1], &endp, 10) & 7);
		if (endp == NULL || endp[0] != '>')
			return 0;
		len = (endp + 1) - p;
	}

	/* do not accept special char if not asked for */
	if (sp && !special)
		return 0;

	if (special) {
		*special = sp;
		/* return special char, do not touch level */
		if (sp)
			return len;
	}

	if (level)
		*level = lev;
	return len;
}

/*
 * Call the console drivers, asking them to write out
 * log_buf[start] to log_buf[end - 1].
 * The console_lock must be held.
 */
static void call_console_drivers(unsigned start, unsigned end)
{
	unsigned cur_index, start_print;
	static int msg_level = -1;

	BUG_ON(((int)(start - end)) > 0);

	cur_index = start;
	start_print = start;
	while (cur_index != end) {
		if (msg_level < 0 && ((end - cur_index) > 2)) {
			/* strip log prefix */
			cur_index += log_prefix(&LOG_BUF(cur_index), &msg_level, NULL);
			start_print = cur_index;
		}
		while (cur_index != end) {
			char c = LOG_BUF(cur_index);

			cur_index++;
			if (c == '\n') {
				if (msg_level < 0) {
					/*
					 * printk() has already given us loglevel tags in
					 * the buffer.  This code is here in case the
					 * log buffer has wrapped right round and scribbled
					 * on those tags
					 */
					msg_level = default_message_loglevel;
				}
				_call_console_drivers(start_print, cur_index, msg_level);
				msg_level = -1;
				start_print = cur_index;
				break;
			}
		}
	}
	// _call_console_drivers(start_print, end, msg_level);
}

static void emit_log_char(char c)
{
	LOG_BUF(log_end) = c;
	log_end++;
	if (log_end - log_start > log_buf_len)
		log_start = log_end - log_buf_len;
	if (log_end - con_start > log_buf_len)
		con_start = log_end - log_buf_len;
	if (logged_chars < log_buf_len)
		logged_chars++;
}


/**
 * printk - print a kernel message
 * @fmt: format string
 *
 * This is printk().  It can be called from any context.  We want it to work.
 *
 * We try to grab the console_lock.  If we succeed, it's easy - we log the output and
 * call the console drivers.  If we fail to get the semaphore we place the output
 * into the log buffer and return.  The current holder of the console_sem will
 * notice the new output in console_unlock(); and will send it to the
 * consoles before releasing the lock.
 *
 * One effect of this deferred printing is that code which calls printk() and
 * then changes console_loglevel may break. This is because console_loglevel
 * is inspected when the actual printing occurs.
 *
 * See also:
 * printf(3)
 *
 * See the vsnprintf() documentation for format string extensions over C99.
 */

asmlinkage int printk(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

	return r;
}


/* cpu currently holding logbuf_lock */
static volatile unsigned int printk_cpu = UINT_MAX;

/*
 * Can we actually use the console at this time on this cpu?
 *
 * Console drivers may assume that per-cpu resources have
 * been allocated. So unless they're explicitly marked as
 * being able to cope (CON_ANYTIME) don't call them until
 * this CPU is officially up.
 */
static inline int can_use_console(unsigned int cpu) {
	// return cpu_online(cpu) || have_callable_console();
	return myos_console_available;
}

/*
 * Try to get console ownership to actually show the kernel
 * messages from a 'printk'. Return true (and with the
 * console_lock held, and 'console_locked' set) if it
 * is successful, false otherwise.
 *
 * This gets called with the 'logbuf_lock' spinlock held and
 * interrupts disabled. It should return with 'lockbuf_lock'
 * released but interrupts still disabled.
 */
static int console_trylock_for_printk(unsigned int cpu)
{
	int retval = 0;

	// if (console_trylock()) {
		retval = 1;

		/*
		 * If we can't use the console, we need to release
		 * the console semaphore by hand to avoid flushing
		 * the buffer. We need to hold the console semaphore
		 * in order to do this test safely.
		 */
		if (!can_use_console(cpu)) {
			console_locked = 0;
			// up(&console_sem);
			retval = 0;
		}
	// }
	// printk_cpu = UINT_MAX;
	// spin_unlock(&logbuf_lock);
	return retval;
}
static const char	recursion_bug_msg [] =
		KERN_CRIT "BUG: recent printk recursion!\n";
static int		recursion_bug;
static int		new_text_line = 1;
static char		printk_buf[1024];

int printk_delay_msec __read_mostly;

static inline void printk_delay(void)
{
	if (unlikely(printk_delay_msec)) {
		int m = printk_delay_msec;

		while (m--) {
			mdelay(1);
			// touch_nmi_watchdog();
		}
	}
}

asmlinkage int vprintk(const char *fmt, va_list args)
{
	int printed_len = 0;
	int current_log_level = default_message_loglevel;
	unsigned long flags;
	int this_cpu;
	char *p;
	size_t plen;
	char special;

	// boot_delay_msec();
	printk_delay();

	// preempt_disable();
	// /* This stops the holder of console_sem just where we want him */
	// raw_local_irq_save(flags);
	// this_cpu = smp_processor_id();

	// /*
	//  * Ouch, printk recursed into itself!
	//  */
	// if (unlikely(printk_cpu == this_cpu)) {
	// 	/*
	// 	 * If a crash is occurring during printk() on this CPU,
	// 	 * then try to get the crash message out but make sure
	// 	 * we can't deadlock. Otherwise just return to avoid the
	// 	 * recursion and return - but flag the recursion so that
	// 	 * it can be printed at the next appropriate moment:
	// 	 */
	// 	if (!oops_in_progress) {
	// 		recursion_bug = 1;
	// 		goto out_restore_irqs;
	// 	}
	// 	zap_locks();
	// }

	// lockdep_off();
	// spin_lock(&logbuf_lock);
	// printk_cpu = this_cpu;

	if (recursion_bug) {
		recursion_bug = 0;
		strcpy(printk_buf, recursion_bug_msg);
		printed_len = strlen(recursion_bug_msg);
	}
	/* Emit the output into the temporary buffer */
	printed_len += vscnprintf(printk_buf + printed_len,
				  sizeof(printk_buf) - printed_len, fmt, args);

	p = printk_buf;

	/* Read log level and handle special printk prefix */
	plen = log_prefix(p, &current_log_level, &special);
	if (plen) {
		p += plen;

		switch (special) {
		case 'c': /* Strip <c> KERN_CONT, continue line */
			plen = 0;
			break;
		case 'd': /* Strip <d> KERN_DEFAULT, start new line */
			plen = 0;
		default:
			if (!new_text_line) {
				emit_log_char('\n');
				new_text_line = 1;
			}
		}
	}

	/*
	 * Copy the output into log_buf. If the caller didn't provide
	 * the appropriate log prefix, we insert them here
	 */
	for (; *p; p++) {
		if (new_text_line) {
			new_text_line = 0;

			if (plen) {
				/* Copy original log prefix */
				int i;

				for (i = 0; i < plen; i++)
					emit_log_char(printk_buf[i]);
				printed_len += plen;
			} else {
				/* Add log prefix */
				emit_log_char('<');
				emit_log_char(current_log_level + '0');
				emit_log_char('>');
				printed_len += 3;
			}

			// if (printk_time) {
				/* Add the current time stamp */
				char tbuf[50], *tp;
				unsigned tlen;
				unsigned long long t;
				unsigned long nanosec_rem;

				// t = cpu_clock(printk_cpu);
				t = local_clock();
				nanosec_rem = do_div(t, 1000000000);
				tlen = sprintf(tbuf, "[%5lu.%06lu] ",
						(unsigned long) t,
						nanosec_rem / 1000);

				for (tp = tbuf; tp < tbuf + tlen; tp++)
					emit_log_char(*tp);
				printed_len += tlen;
			// }

			if (!*p)
				break;
		}

		emit_log_char(*p);
		if (*p == '\n')
			new_text_line = 1;
	}

	/*
	 * Try to acquire and then immediately release the
	 * console semaphore. The release will do all the
	 * actual magic (print out buffers, wake up klogd,
	 * etc). 
	 *
	 * The console_trylock_for_printk() function
	 * will release 'logbuf_lock' regardless of whether it
	 * actually gets the semaphore or not.
	 */
	if (console_trylock_for_printk(this_cpu))
		console_unlock();

	// lockdep_on();
out_restore_irqs:
	// raw_local_irq_restore(flags);

	// preempt_enable();
	return printed_len;
}




/**
 * console_unlock - unlock the console system
 *
 * Releases the console_lock which the caller holds on the console system
 * and the console driver list.
 *
 * While the console_lock was held, console output may have been buffered
 * by printk().  If this is the case, console_unlock(); emits
 * the output prior to releasing the lock.
 *
 * If there is output waiting for klogd, we wake it up.
 *
 * console_unlock(); may be called from any context.
 */
void console_unlock(void)
{
	unsigned long flags;
	unsigned _con_start, _log_end;
	unsigned wake_klogd = 0;

	// if (console_suspended) {
	// 	up(&console_sem);
	// 	return;
	// }

	// console_may_schedule = 0;

	for ( ; ; ) {
		// spin_lock_irqsave(&logbuf_lock, flags);
		wake_klogd |= log_start - log_end;
		if (con_start == log_end)
			break;			/* Nothing to print */
		_con_start = con_start;
		_log_end = log_end;
		con_start = log_end;		/* Flush */
		// spin_unlock(&logbuf_lock);
		// stop_critical_timings();	/* don't trace print latency */
		call_console_drivers(_con_start, _log_end);
		// start_critical_timings();
		// local_irq_restore(flags);
	}
	// console_locked = 0;

	// /* Release the exclusive_console once it is used */
	// if (unlikely(exclusive_console))
	// 	exclusive_console = NULL;

	// up(&console_sem);
	// spin_unlock_irqrestore(&logbuf_lock, flags);
	// if (wake_klogd)
	// 	wake_up_klogd();
}
EXPORT_SYMBOL(console_unlock);