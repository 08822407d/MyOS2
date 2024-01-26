// SPDX-License-Identifier: GPL-2.0
/*
 * RTC related functions
 */
// #include <linux/platform_device.h>
#include <linux/kernel/mc146818rtc.h>
// #include <linux/export.h>
// #include <linux/pnp.h>

// #include <asm/vsyscall.h>
// #include <asm/x86_init.h>
// #include <asm/time.h>
// #include <asm/intel-mid.h>
#include <asm/setup.h>


/* Routines for accessing the CMOS RAM/RTC. */
unsigned char rtc_cmos_read(unsigned char addr)
{
	unsigned char val;

	// lock_cmos_prefix(addr);
	outb(addr, RTC_PORT(0));
	val = inb(RTC_PORT(1));
	// lock_cmos_suffix(addr);

	return val;
}

void rtc_cmos_write(unsigned char val, unsigned char addr)
{
	// lock_cmos_prefix(addr);
	outb(addr, RTC_PORT(0));
	outb(val, RTC_PORT(1));
	// lock_cmos_suffix(addr);
}

/* not static: needed by APM */
void read_persistent_clock64(timespec64_s *ts)
{
	// void mach_get_cmos_time(struct timespec64 *now)
	// {
		rtc_time_s tm;

		mc146818_get_time(&tm);

		ts->tv_sec = rtc_tm_to_time64(&tm);
		ts->tv_nsec = 0;
	// }
}