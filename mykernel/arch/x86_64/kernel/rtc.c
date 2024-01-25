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

/* not static: needed by APM */
void read_persistent_clock64(timespec64_s *ts)
{
}