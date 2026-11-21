# 第104批源码证据：D00／CMOS六字段／BCD与历法／启动timekeeper

固定源码：`time@a039d9803ade`（短SHA，来自连接器）。每个证据ID含真实路径、函数/宏/结构/唯一对象名称，以及连续1–5行源码。所有函数引文限制在函数体，宏与结构引文限制为对应定义单元；辅助行号不替代符号定位。已注释语句只证未活动表达式，不能拿来认证实际执行。无编译、预处理、ELF、RTC设备读取或运行观察。

### RTC104-E001 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```
用途：D00 exact selected CMake command；固定time辅助行58–61。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E002 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
	if (freq) {
```
用途：old103-E007；固定time辅助行205–205。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E003 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
		sec = cs->mask;
		do_div(sec, freq);
		do_div(sec, scale);
		if (!sec)
			sec = 1;
```
用途：old103-E008；固定time辅助行215–219。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E004 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
		else if (sec > 600 && cs->mask > UINT_MAX)
			sec = 600;

		clocks_calc_mult_shift(&cs->mult, &cs->shift, freq, NSEC_PER_SEC / scale, sec * scale);
```
用途：old103-E009；固定time辅助行220–223。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E005 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
	cs->maxadj = clocksource_max_adjustment(cs);
	while (freq && ((cs->mult + cs->maxadj < cs->mult)
		|| (cs->mult - cs->maxadj > cs->mult))) {
```
用途：old103-E010；固定time辅助行249–251。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E006 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_update_freq_scale]
```c
		cs->mult >>= 1;
		cs->shift--;
		cs->maxadj = clocksource_max_adjustment(cs);
```
用途：old103-E011；固定time辅助行252–254。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E007 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	clocksource_enqueue(cs);
	// clocksource_enqueue_watchdog(cs);
	// clocksource_watchdog_unlock(&flags);
```
用途：old103-E012；固定time辅助行420–422。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E008 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	// clocksource_select();
	// clocksource_select_watchdog(false);
	// __clocksource_suspend_select(cs);
	// mutex_unlock(&clocksource_mutex);
	return 0;
```
用途：old103-E013；固定time辅助行424–428。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E009 [VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	cfg = hpet_readl(HPET_Tn_CFG(channel));
	// cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL | HPET_TN_32BIT;
	cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL;
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
```
用途：old103-E030；固定time辅助行219–223。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E010 [VERIFIED mykernel/time/systick/systick_const.h::TICK_NSEC]
```c
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)
```
用途：old103-E089 actual macro；固定time辅助行7–7。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E011 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	/* Initialize mult/shift and max_idle_ns */
	__clocksource_update_freq_scale(cs, scale, freq);
```
用途：G102-20 actual update call；固定time辅助行413–414。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E012 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_register_scale]
```c
	clocksource_enqueue(cs);
```
用途：G102-22 distinct enqueue call；固定time辅助行420–420。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E013 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_done_booting]
```c
	clocksource_select();
```
用途：G102-31 caller site；固定time辅助行363–363。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E014 [VERIFIED mykernel/time/clocksource/clocksource_macro.h::clocksource_select]
```c
	#define clocksource_select()	\
				__clocksource_select(false)
```
用途：G102-31 selected macro declaration；固定time辅助行14–15。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E015 [VERIFIED mykernel/include/linux/init/init.h::fs_initcall]
```c
			#define fs_initcall(fn)				__define_initcall(fn, 5)
```
用途：initcall level five macro；固定time辅助行304–304。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E016 [VERIFIED mykernel/include/linux/init/init.h::__define_initcall]
```c
			#define __define_initcall(fn, id) ___define_initcall(fn, id, .initcall##id)
```
用途：initcall middle macro；固定time辅助行278–278。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E017 [VERIFIED mykernel/include/linux/init/init.h::___define_initcall]
```c
			#define ___define_initcall(fn, id, __sec)			\
						__unique_initcall(fn, id, __sec, __initcall_id(fn))
```
用途：initcall inner macro；固定time辅助行275–276。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E018 [VERIFIED mykernel/include/linux/init/init.h::__unique_initcall]
```c
			#define __unique_initcall(fn, id, __sec, __iid)			\
						____define_initcall(fn,						\
							__initcall_stub(fn, __iid, id),			\
							__initcall_name(initcall, __iid, id),	\
							__initcall_section(__sec, __iid)		\
```
用途：initcall unique macro and section；固定time辅助行268–272。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E019 [VERIFIED mykernel/include/linux/init/init.h::____define_initcall]
```c
				#define ____define_initcall(fn, __unused, __name, __sec)	\
							static initcall_t __name __used 				\
								__attribute__((__section__(__sec))) = fn;
```
用途：conditional plain section variable；固定time辅助行263–265。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E020 [VERIFIED mykernel/include/linux/init/init.h::____define_initcall]
```c
				#define ____define_initcall(fn, __stub, __name, __sec)		\
							__define_initcall_stub(__stub, fn)				\
							asm(".section	\"" __sec "\", \"a\"		\n"	\
								__stringify(__name) ":			\n"			\
								".long	" __stringify(__stub) " - .	\n"		\
```
用途：conditional PREL32 asm form；固定time辅助行255–259。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E021 [VERIFIED mykernel/include/linux/init/init.h::__initcall_section]
```c
				#define __initcall_section(__sec, __iid)			\
							#__sec ".init"
```
用途：section token macro in alternate branch；固定time辅助行245–246。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E022 [VERIFIED mykernel/time/clocksource/clocksource_types.h::clocksource]
```c
	struct clocksource {
		u64				(*read)(clocksrc_s *cs);
		u64				mask;
		u32				mult;
		u32				shift;
```
用途：clocksource u32 shift field；固定time辅助行70–74。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E023 [VERIFIED mykernel/time/timekeeping/timekeeping_types.h::tk_read_base]
```c
		u64				mask;
		u64				cycle_last;
		u32				mult;
		u32				shift;
		u64				xtime_nsec;
```
用途：read base cached shift field；固定time辅助行30–34。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E024 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
	if (old_clock) {
		int shift_change = clock->shift - old_clock->shift;
		if (shift_change < 0) {
			tk->tkr_mono.xtime_nsec >>= -shift_change;
			tk->tkr_raw.xtime_nsec >>= -shift_change;
```
用途：oldclock negative shift raw write；固定time辅助行161–165。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E025 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
		} else {
			tk->tkr_mono.xtime_nsec <<= shift_change;
			tk->tkr_raw.xtime_nsec <<= shift_change;
		}
```
用途：oldclock nonnegative shift raw write；固定time辅助行166–169。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E026 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_setup_internals]
```c
	tk->tkr_mono.clock = clock;
	tk->tkr_mono.mask = clock->mask;
	tk->tkr_mono.cycle_last = tk_clock_read(&tk->tkr_mono);

	tk->tkr_raw.clock = clock;
```
用途：initial cache field sequence；固定time辅助行135–139。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E027 [VERIFIED mykernel/include/linux/sched/clock.h::local_clock]
```c
		return native_sched_clock();
```
用途：local_clock opaque return statement；固定time辅助行53–53。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E028 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;

	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
```
用途：s32 remainder and positive path；固定time辅助行52–56。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E029 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```
用途：two unary negatives at C boundary；固定time辅助行64–65。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。

### RTC104-E030 [VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```
用途：u64 quotient u32 pointer body；固定time辅助行43–44。仅静态定义与条件范围，不认证实际预处理/ELF/硬件/运行。


### RTC104-E031 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::rtc_cmos_read]
```c
	unsigned char val;
```
用途：local byte variable；time固定源码行20–20。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E032 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::rtc_cmos_read]
```c
	// lock_cmos_prefix(addr);
	outb(addr, RTC_PORT(0));
	val = inb(RTC_PORT(1));
	// lock_cmos_suffix(addr);
```
用途：commented lock and two distinct I/O requests；time固定源码行22–25。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E033 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::rtc_cmos_read]
```c
	return val;
```
用途：return sample byte；time固定源码行27–27。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E034 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::RTC_PORT]
```c
	#	define RTC_PORT(x)			(0x70 + (x))
```
用途：index0 and data1 port selectors；time固定源码行12–12。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E035 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::CMOS_READ]
```c
	#define CMOS_READ(addr)			rtc_cmos_read(addr)
```
用途：macro to rtc_cmos_read；time固定源码行27–27。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E036 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
	time->tm_sec = CMOS_READ(RTC_SECONDS);
	time->tm_min = CMOS_READ(RTC_MINUTES);
	time->tm_hour = CMOS_READ(RTC_HOURS);
```
用途：seconds minute hour read order；time固定源码行18–20。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E037 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
	time->tm_mday = CMOS_READ(RTC_DAY_OF_MONTH);
	time->tm_mon = CMOS_READ(RTC_MONTH);
	time->tm_year = CMOS_READ(RTC_YEAR);
```
用途：mday month year read order；time固定源码行21–23。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E038 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	simple_mc146818_gettime(time);
	// if (!mc146818_avoid_UIP(mc146818_get_time_callback, &p)) {
	// 	memset(time, 0, sizeof(*time));
	// 	return -EIO;
	// }
```
用途：six field callback one scan with UIP comment disabled；time固定源码行45–49。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E039 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_SECONDS]
```c
	#define RTC_SECONDS				0
```
用途：seconds register index；time固定源码行51–51。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E040 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_MINUTES]
```c
	#define RTC_MINUTES				2
```
用途：minutes register index；time固定源码行53–53。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E041 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_HOURS]
```c
	#define RTC_HOURS				4
```
用途：hours register index；time固定源码行55–55。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E042 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_DAY_OF_MONTH]
```c
	#define RTC_DAY_OF_MONTH	7
```
用途：day register index；time固定源码行61–61。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E043 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_MONTH]
```c
	#define RTC_MONTH			8
```
用途：month register index；time固定源码行62–62。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E044 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_YEAR]
```c
	#define RTC_YEAR			9
```
用途：year register index；time固定源码行63–63。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E045 [VERIFIED mykernel/include/uapi/linux/rtc.h::rtc_time]
```c
		int		tm_sec;
		int		tm_min;
		int		tm_hour;
		int		tm_mday;
		int		tm_mon;
```
用途：six integer fields first five；time固定源码行26–30。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E046 [VERIFIED mykernel/include/uapi/linux/rtc.h::rtc_time]
```c
		int		tm_year;
		int		tm_wday;
		int		tm_yday;
		int		tm_isdst;
```
用途：year and unmodified other three；time固定源码行31–34。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E047 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::lock_cmos_prefix]
```c
	#define lock_cmos_prefix(reg)	do {} while (0)
	#define lock_cmos_suffix(reg)	do {} while (0)
```
用途：defined empty lock helpers；time固定源码行16–17。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E048 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_UIP]
```c
	#	define RTC_UIP				0x80
```
用途：update in progress bit macro；time固定源码行81–81。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E049 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_CONTROL]
```c
	#define RTC_CONTROL	RTC_REG_B
```
用途：control register index；time固定源码行96–96。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E050 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_DM_BINARY]
```c
	#	define RTC_DM_BINARY		0x04	/* all time/date values are BCD if clear */
```
用途：binary encoding control bit；time固定源码行102–102。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E051 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_24H]
```c
	#	define RTC_24H				0x02	/* 24 hour mode - else hours bit 7 means pm */
```
用途：24hour control bit；time固定源码行103–103。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E052 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::RTC_ALWAYS_BCD]
```c
	#	define RTC_ALWAYS_BCD		1	/* RTC operates in binary mode */
```
用途：define exists not actual condition branch；time固定源码行13–13。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E053 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
// 	if (acpi_gbl_FADT.anchor.revision >= FADT2_REVISION_ID &&
// 	    acpi_gbl_FADT.century) {
// 		p->century = CMOS_READ(acpi_gbl_FADT.century);
// 	} else {
// 		p->century = 0;
```
用途：commented century read；time固定源码行28–32。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E054 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
// 	p->ctrl = CMOS_READ(RTC_CONTROL);
```
用途：commented RTC_CONTROL read；time固定源码行36–36。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E055 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	// if (!mc146818_avoid_UIP(mc146818_get_time_callback, &p)) {
	// 	memset(time, 0, sizeof(*time));
	// 	return -EIO;
	// }
```
用途：commented avoid UIP and error path；time固定源码行46–49。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E056 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
	#define BUILDIO(bwl, bw, type)									\
				static inline void									\
				out##bwl(unsigned type value, int port) {			\
```
用途：outb macro declaration types；time固定源码行188–190。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E057 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
					asm volatile(	"out" #bwl " %" #bw "0, %w1"	\
								:									\
								:	"a"(value),						\
									"Nd"(port));					\
```
用途：outb expanded asm request；time固定源码行191–194。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E058 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
				static inline unsigned type							\
				in##bwl(int port) {									\
					unsigned type value;							\
					asm volatile(	"in" #bwl " %w1, %" #bw "0"		\
```
用途：inb macro declaration and asm；time固定源码行197–200。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E059 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
								:	"=a"(value)						\
								:	"Nd"(port));					\
					return value;									\
				}													\
```
用途：inb return operand and value；time固定源码行201–204。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E060 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
	BUILDIO(b, b, char)
```
用途：byte port read write macro invocation；time固定源码行239–239。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E061 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
	 * Only the values that we read from the RTC are set. We leave
	 * tm_wday, tm_yday and tm_isdst untouched. Even though the
	 * RTC has RTC_DAY_OF_WEEK, we ignore it, as it is only updated
	 * by the RTC when initially set to a non-zero value.
```
用途：wday yday isdst intentionally not assigned；time固定源码行13–16。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E062 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	return 0;
```
用途：function return zero despite no valid checker；time固定源码行79–79。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E063 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::read_persistent_clock64]
```c
		rtc_time_s tm;
```
用途：local rtc time declaration；time固定源码行43–43。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E064 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::read_persistent_clock64]
```c
		ts->tv_sec = rtc_tm_to_time64(&tm);
		ts->tv_nsec = 0;
```
用途：calendar seconds and tv_nsec0；time固定源码行47–48。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E065 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_REG_A]
```c
	#define RTC_REG_A			10
```
用途：RTC register A index declaration；time固定源码行67–67。硬件结果、编译选择、样本一致、日历合法性需另证。

### RTC104-E066 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_VRT]
```c
	#	define RTC_VRT				0x80		/* valid RAM and time */
```
用途：validity bit exists not consulted in readout；time固定源码行116–116。硬件结果、编译选择、样本一致、日历合法性需另证。


### RTC104-E067 [VERIFIED mykernel/include/linux/kernel/bcd.h::bcd2bin]
```c
	#define bcd2bin(x) (							\
				__builtin_constant_p((u8 )(x)) ?	\
				const_bcd2bin(x) : _bcd2bin(x)		\
			)
```
用途：bcd macro conditional builtin constant choice；固定time辅助行7–10。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E068 [VERIFIED mykernel/include/linux/kernel/bcd.h::const_bcd2bin]
```c
	#define const_bcd2bin(x)		(((x) & 0x0f) + ((x) >> 4) * 10)
```
用途：bcd constant arithmetic formula；固定time辅助行19–19。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E069 [VERIFIED mykernel/lib/bcd.c::_bcd2bin]
```c
	return (val & 0x0f) + (val >> 4) * 10;
```
用途：bcd runtime body formula；固定time辅助行6–6。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E070 [VERIFIED mykernel/include/linux/kernel/bcd.h::const_bcd_is_valid]
```c
	#define const_bcd_is_valid(x)	(((x) & 0x0f) < 10 && ((x) >> 4) < 10)
```
用途：bcd digit validation separate；固定time辅助行21–21。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E071 [VERIFIED mykernel/include/linux/kernel/bcd.h::bcd_is_valid]
```c
	#define bcd_is_valid(x)	const_bcd_is_valid(x)
```
用途：validity macro exists but not used in read chain；固定time辅助行17–17。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E072 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	time->tm_sec = bcd2bin(time->tm_sec);
	time->tm_min = bcd2bin(time->tm_min);
	time->tm_hour = bcd2bin(time->tm_hour);
```
用途：three BCD conversion requests；固定time辅助行51–53。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E073 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	time->tm_mday = bcd2bin(time->tm_mday);
	time->tm_mon = bcd2bin(time->tm_mon);
	time->tm_year = bcd2bin(time->tm_year);
```
用途：other three BCD conversion requests；固定time辅助行54–56。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E074 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	if (time->tm_year <= 69)
		time->tm_year += 100;
```
用途：decoded-year 69 threshold；固定time辅助行74–75。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E075 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::mc146818_get_time]
```c
	time->tm_mon--;
```
用途：tm_mon 1based to 0based；固定time辅助行77–77。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E076 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_days_in_month]
```c
static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
```
用途：days array object declaration；固定time辅助行19–21。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E077 [VERIFIED mykernel/include/linux/kernel/rtc.h::is_leap_year]
```c
		return (!(year % 4) && (year % 100)) || !(year % 400);
```
用途：leap predicate body；固定time辅助行224–224。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E078 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_month_days]
```c
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
```
用途：month day count with February leap condition；固定time辅助行35–35。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E079 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_valid_tm]
```c
	if (tm->tm_year < 70 || tm->tm_year > (INT_MAX - 1900) ||
		((unsigned int)tm->tm_mon) >= 12 || tm->tm_mday < 1 ||
		tm->tm_mday > rtc_month_days(tm->tm_mon,
```
用途：date checker year month day guards；固定time辅助行152–154。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E080 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_valid_tm]
```c
			((unsigned int)tm->tm_year + 1900)) ||
		((unsigned int)tm->tm_hour) >= 24 ||
		((unsigned int)tm->tm_min) >= 60 ||
		((unsigned int)tm->tm_sec) >= 60)
```
用途：date checker month days and time guards；固定time辅助行155–158。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E081 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_valid_tm]
```c
		return -EINVAL;
```
用途：date checker error return；固定time辅助行159–159。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E082 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_valid_tm]
```c
	return 0;
```
用途：date checker normal return；固定time辅助行161–161。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E083 [VERIFIED mykernel/include/linux/kernel/mc146818rtc.h::RTC_DAY_OF_WEEK]
```c
	#define RTC_DAY_OF_WEEK		6
```
用途：separate weekday register exists；固定time辅助行60–60。BCDs数值解码不等于完整日历有效或真实设备模式。

### RTC104-E084 [VERIFIED mykernel/include/uapi/linux/rtc.h::rtc_time]
```c
		int		tm_mon;
		int		tm_year;
		int		tm_wday;
		int		tm_yday;
		int		tm_isdst;
```
用途：month year and unused calendar fields；固定time辅助行30–34。BCDs数值解码不等于完整日历有效或真实设备模式。


### RTC104-E085 [VERIFIED mykernel/drivers/rtc/lib.c::rtc_tm_to_time64]
```c
	return mktime64(((unsigned int)tm->tm_year + 1900), tm->tm_mon + 1,
			tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
```
用途：RTC tm six argument mapping to mktime64；固定time辅助行171–172。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E086 [VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
	uint mon = mon0, year = year0;
```
用途：uint mon and year mutable local；固定time辅助行245–245。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E087 [VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
	if (0 >= (int) (mon -= 2)) {
		mon += 12;	/* Puts Feb last since it has leap day */
		year -= 1;
	}
```
用途：Jan Feb unsigned subtract and signed cast；固定time辅助行248–251。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E088 [VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
	return ((((time64_t)
				(year/4 - year/100 + year/400 + 367*mon/12 + day) +
				year*365 - 719499
```
用途：Julian offset and year*365 expression；固定time辅助行253–255。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E089 [VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
			)*24 + hour /* now have hours - midnight tomorrow handled here */
		)*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
```
用途：hour minute second accumulation；固定time辅助行256–258。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E090 [VERIFIED mykernel/time/time_type_declaration.h::time64_t]
```c
	typedef __s64 time64_t;
```
用途：time64 signed seconds typedef；固定time辅助行10–10。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E091 [VERIFIED mykernel/time/misc/time_misc_types.h::timespec64]
```c
		time64_t	tv_sec;			/* seconds */
		long		tv_nsec;		/* nanoseconds */
```
用途：wall time target signed sec long nanosecond；固定time辅助行8–9。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E092 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	read_persistent_clock64(&wall_time);
	boot_offset = ns_to_timespec64(local_clock());
```
用途：RTC wall input and local_clock separate；固定time辅助行244–245。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E093 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	wall_to_mono = timespec64_sub(boot_offset, wall_time);
```
用途：wall_to_mono computed before clock setup；固定time辅助行251–251。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E094 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	clock = clocksource_default_clock();
	if (clock->enable)
		clock->enable(clock);	
	tk_setup_internals(tk, clock);
```
用途：select default callback and setup；固定time辅助行253–256。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E095 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	tk_set_xtime(tk, &wall_time);
	tk->raw_sec = 0;

	tk_set_wall_to_mono(tk, wall_to_mono);
```
用途：wall_time copy raw_sec and save helper call；固定time辅助行258–261。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E096 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_set_xtime]
```c
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec =
		(u64)ts->tv_nsec << tk->tkr_mono.shift;
```
用途：copy wall sec and shifted nanos into tk；固定time辅助行35–37。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E097 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_set_wall_to_mono]
```c
	// set_normalized_timespec64(&tmp, -tk->wall_to_monotonic.tv_sec,
	// 				-tk->wall_to_monotonic.tv_nsec);
	// WARN_ON_ONCE(tk->offs_real != timespec64_to_ktime(tmp));
	// tk->wall_to_monotonic = wtm;
	// set_normalized_timespec64(&tmp, -wtm.tv_sec, -wtm.tv_nsec);
```
用途：wall_to_mono active assignment commented；固定time辅助行56–60。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E098 [VERIFIED mykernel/time/timekeeping/timekeeping_types.h::timekeeper]
```c
		tk_readbase_s	tkr_mono;
		tk_readbase_s	tkr_raw;
		u64				xtime_sec;
		ulong			ktime_sec;
		timespec64_s	wall_to_monotonic;
```
用途：timekeeper mono raw xtime_sec and wall_to_monotonic fields；固定time辅助行87–91。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E099 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	timespec64_s wall_time, boot_offset, wall_to_mono;
	timekeeper_s *tk = &tk_core.timekeeper;
	clocksrc_s *clock;
	ulong flags;
```
用途：local wall_time boot_offset and clock objects；固定time辅助行238–241。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E100 [VERIFIED mykernel/time/misc/time_misc.h::timespec64_sub]
```c
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
			return ts_delta;
```
用途：timespec subtraction invokes normalization；固定time辅助行80–83。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E101 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
```
用途：positive signed input divides for local clock；固定time辅助行55–57。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E102 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;
```
用途：s32 remainder local and result init；固定time辅助行52–53。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E103 [VERIFIED mykernel/include/linux/kernel/types.h::uint]
```c
		typedef unsigned int		uint;
```
用途：uint is unsigned int typedef；固定time辅助行104–104。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。

### RTC104-E104 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	// timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);
```
用途：commented timekeeping update call；固定time辅助行263–263。仅有界静态C类型/控制路径，未授权设备访问或真实时间取值。


## 引文适用性

每个短引文只能支持本身所写的定义、条件或请求。实际源链接、编译选择、设备响应、输入日期有效、六字段一致、C实现定义转换和timekeeper后继写者均分开设门。旧102/103锚按固定批号和旧文件path引用，不能把旧注释或旧错误定位符自认已纠正。完整本地P9与任何硬件/作者模型执行都未进行。
