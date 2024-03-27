// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel/bcd.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/export.h>
#include <linux/kernel/mc146818rtc.h>


static void simple_mc146818_gettime(rtc_time_s *time)
{
	// struct mc146818_get_time_callback_param *p = param_in;

	/*
	 * Only the values that we read from the RTC are set. We leave
	 * tm_wday, tm_yday and tm_isdst untouched. Even though the
	 * RTC has RTC_DAY_OF_WEEK, we ignore it, as it is only updated
	 * by the RTC when initially set to a non-zero value.
	 */
	time->tm_sec = CMOS_READ(RTC_SECONDS);
	time->tm_min = CMOS_READ(RTC_MINUTES);
	time->tm_hour = CMOS_READ(RTC_HOURS);
	time->tm_mday = CMOS_READ(RTC_DAY_OF_MONTH);
	time->tm_mon = CMOS_READ(RTC_MONTH);
	time->tm_year = CMOS_READ(RTC_YEAR);
// #ifdef CONFIG_MACH_DECSTATION
// 	p->real_year = CMOS_READ(RTC_DEC_YEAR);
// #endif
// #ifdef CONFIG_ACPI
// 	if (acpi_gbl_FADT.anchor.revision >= FADT2_REVISION_ID &&
// 	    acpi_gbl_FADT.century) {
// 		p->century = CMOS_READ(acpi_gbl_FADT.century);
// 	} else {
// 		p->century = 0;
// 	}
// #endif

// 	p->ctrl = CMOS_READ(RTC_CONTROL);
}

int mc146818_get_time(rtc_time_s *time)
{
	// struct mc146818_get_time_callback_param p = {
	// 	.time = time
	// };

	simple_mc146818_gettime(time);
	// if (!mc146818_avoid_UIP(mc146818_get_time_callback, &p)) {
	// 	memset(time, 0, sizeof(*time));
	// 	return -EIO;
	// }

	time->tm_sec = bcd2bin(time->tm_sec);
	time->tm_min = bcd2bin(time->tm_min);
	time->tm_hour = bcd2bin(time->tm_hour);
	time->tm_mday = bcd2bin(time->tm_mday);
	time->tm_mon = bcd2bin(time->tm_mon);
	time->tm_year = bcd2bin(time->tm_year);
// #ifdef CONFIG_ACPI
// 	p.century = bcd2bin(p.century);
// #endif

// #ifdef CONFIG_MACH_DECSTATION
// 	time->tm_year += p.real_year - 72;
// #endif

// #ifdef CONFIG_ACPI
// 	if (p.century > 19)
// 		time->tm_year += (p.century - 19) * 100;
// #endif

	/*
	 * Account for differences between how the RTC uses the values
	 * and how they are defined in a rtc_time;
	 */
	if (time->tm_year <= 69)
		time->tm_year += 100;

	time->tm_mon--;

	return 0;
}