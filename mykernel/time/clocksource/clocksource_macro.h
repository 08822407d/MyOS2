/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CLOCKSOURCE_MACRO_H_
#define _LINUX_CLOCKSOURCE_MACRO_H_


	/* simplify initialization of mask field */
	#define CLOCKSOURCE_MASK(bits)	GENMASK_ULL((bits) - 1, 0)

	#define clocksource_register_hz(cs, hz) \
				__clocksource_register_scale(cs, 1, hz);
	#define clocksource_register_khz(cs, khz) \
				__clocksource_register_scale(cs, 1000, khz)

#endif /* _LINUX_CLOCKSOURCE_API_H_ */