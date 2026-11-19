# DST108 固定 time 源级证据索引

source_pin_short12: a039d9803ade；read_channel: GitHub connector。以下每项真实1–5个连续源文件行，只有属于宏定义、真实对象、类型结构体定义或函数体的源码才能被作为该定义的直接支持；符号签名/未选编、include实例及注释另作为POSITION_ONLY，不据此证明编译/设备。

### DT108-E001

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

固定time辅助行65–65；用途：D00 wrapper CONFIG option in same CMake set command。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E002

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
```

固定time辅助行10–10；用途：D00 first __SYSCALL macro definition for extern。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E003

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym)	[nr] = __x64_##sym,
```

固定time辅助行19–19；用途：D00 second __SYSCALL macro definition for array。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E004

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time辅助行26–26；用途：D00 third __SYSCALL macro definition for switch。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E005

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::sys_call_table]
```c
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
```

固定time辅助行20–22；用途：D00 actual table initializer and include。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E006

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time辅助行29–31；用途：D00 switch body and common list。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E007

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]
```c
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
```

固定time辅助行80–83；用途：D00 generic and arch header roots。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E008

[VERIFIED mykernel/scripts/target_kernel.cmake::target_include_directories(kernel PRIVATE)]
```c
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
```

固定time辅助行2–4；用途：D00 actual kernel target consumes INCLUDE_DIRS。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E009

[VERIFIED mykernel/time/timer/posix-timers.c::posix_get_realtime_timespec]
```c
	ktime_get_real_ts64(tp);
	return 0;
```

固定time辅助行30–31；用途：D00 callback samples then success。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E010

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

固定time辅助行5–5；用途：D00 scalar user return constant。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E011

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
		u32 ret = 0;
		while (dividend >= divisor) {
```

固定time辅助行13–14；用途：D00 u32 quotient counter and loop。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E012

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
			dividend -= divisor;
			ret++;
		}
		*remainder = dividend;
		return ret;
```

固定time辅助行19–23；用途：D00 decrement quotient and remainder。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E013

[VERIFIED mykernel/time/misc/time_misc_const.h::NSEC_PER_SEC]
```c
	#define NSEC_PER_SEC			1000000000L
```

固定time辅助行32–32；用途：D00 denominator B equals 1 billion。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E014

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
	// if (unlikely(tz != NULL)) {
	// 	if (copy_to_user(tz, &sys_tz, sizeof(sys_tz)))
	// 		return -EFAULT;
	// }
	return 0;
```

固定time辅助行24–28；用途：D00 timezone body commented and return 0。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E015

[VERIFIED mykernel/time/misc/time_misc.c::put_timespec64]
```c
	__kernel_timespec_s kts = {
		.tv_sec = ts->tv_sec,
		.tv_nsec = ts->tv_nsec
	};
```

固定time辅助行307–310；用途：D00 user kernel timespec assignment。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E016

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
	#define put_user(x, ptr) ({					\
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```

固定time辅助行191–194；用途：D00 direct put_user macro body。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E017

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
		ktime_get_real_ts64(&ts);
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
```

固定time辅助行19–22；用途：D00 two put_user calls and return error。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E018

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	if (unlikely(delta > tkr->clock->max_cycles)) {
```

固定time辅助行206–206；用途：D00 overflow fallback condition。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E019

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
		if (delta & ~(mask >> 1))
			return tkr->xtime_nsec >> tkr->shift;

		return delta_to_ns_safe(tkr, delta);
```

固定time辅助行212–215；用途：D00 two fallback returns。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E020

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	return ((delta * tkr->mult) + tkr->xtime_nsec) >> tkr->shift;
```

固定time辅助行218–218；用途：D00 normal branch return。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E021

[VERIFIED mykernel/time/misc/time_misc_const.h::MSEC_PER_SEC]
```c
	#define MSEC_PER_SEC			1000L
```

固定time辅助行27–27；用途：A 1000 milliseconds per second。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E022

[VERIFIED mykernel/time/misc/time_misc_const.h::USEC_PER_SEC]
```c
	#define USEC_PER_SEC			1000000L
```

固定time辅助行31–31；用途：A 1000000 microseconds per second。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E023

[VERIFIED mykernel/time/misc/time_misc_const.h::NSEC_PER_MSEC]
```c
	#define NSEC_PER_MSEC			1000000L
```

固定time辅助行30–30；用途：A 1000000 ns per ms。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E024

[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```

固定time辅助行9–9；用途：A active HZ=1000。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E025

[VERIFIED mykernel/time/systick/systick_const.h::TICK_NSEC]
```c
	#define TICK_NSEC		((NSEC_PER_SEC+HZ/2)/HZ)
```

固定time辅助行7–7；用途：A rounded TICK_NSEC expression。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E026

[VERIFIED mykernel/time/systick/systick_const.h::SHIFT_HZ]
```c
	#  define SHIFT_HZ	    10
```

固定time辅助行29–29；用途：A selected SHIFT_HZ macro definition 10; condition is position-only at P25. Complete preprocessing not executed.

### DT108-E027

[VERIFIED mykernel/time/systick/systick_const.h::SEC_JIFFIE_SC]
```c
	#define SEC_JIFFIE_SC		(31 - SHIFT_HZ)
```

固定time辅助行156–156；用途：A initial scale 31-SHIFT_HZ。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E028

[VERIFIED mykernel/time/systick/systick_const.h::SEC_JIFFIE_SC]
```c
	#  define SEC_JIFFIE_SC		(32 - SHIFT_HZ)
```

固定time辅助行159–159；用途：A selected conditional redefinition to 32-SHIFT_HZ。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E029

[VERIFIED mykernel/time/systick/systick_const.h::NSEC_JIFFIE_SC]
```c
	#define NSEC_JIFFIE_SC		(SEC_JIFFIE_SC + 29)
```

固定time辅助行161–161；用途：A nanosecond scale。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E030

[VERIFIED mykernel/time/systick/systick_const.h::SEC_CONVERSION]
```c
	#define SEC_CONVERSION		(								\
				(ulong)((((u64)NSEC_PER_SEC << SEC_JIFFIE_SC) + \
					TICK_NSEC -1) / (u64)TICK_NSEC)				\
			)
```

固定time辅助行162–165；用途：A fixed point seconds multiplier。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E031

[VERIFIED mykernel/time/systick/systick_const.h::NSEC_CONVERSION]
```c
	#define NSEC_CONVERSION		(						\
				(ulong)((((u64)1 << NSEC_JIFFIE_SC) +	\
					TICK_NSEC -1) / (u64)TICK_NSEC)		\
			)
```

固定time辅助行167–170；用途：A fixed point nanoseconds multiplier。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E032

[VERIFIED mykernel/time/systick/systick_const.h::MAX_JIFFY_OFFSET]
```c
	#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)
```

固定time辅助行83–83；用途：A max tick count using LONG_MAX。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E033

[VERIFIED mykernel/time/systick/systick_const.h::MAX_SEC_IN_JIFFIES]
```c
	#define MAX_SEC_IN_JIFFIES									\
				(SH_DIV((MAX_JIFFY_OFFSET >> SEC_JIFFIE_SC) *	\
					TICK_NSEC, NSEC_PER_SEC, 1) - 1)
```

固定time辅助行177–179；用途：A seconds upper bound macro。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E034

[VERIFIED mykernel/time/systick/systick_const.h::SH_DIV]
```c
	#define SH_DIV(NOM,DEN,LSH) (				\
				(((NOM) / (DEN)) << (LSH)) +	\
				((((NOM) % (DEN)) << (LSH)) + 	\
					(DEN) / 2) /				\
				(DEN)							\
```

固定time辅助行49–53；用途：A scaled divide macro body。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E035

[VERIFIED mykernel/include/linux/kernel/limits.h::LONG_MAX]
```c
	#define LONG_MAX		((long)(~0UL >> 1))
```

固定time辅助行33–33；用途：A target signed long maximum。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E036

[VERIFIED mykernel/time/systick/systick.h::_msecs_to_jiffies]
```c
				return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
```

固定time辅助行56–56；用途：B selected 1000Hz milli ceil expression。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E037

[VERIFIED mykernel/time/systick/systick.h::msecs_to_jiffies]
```c
			if (__builtin_constant_p(m)) {
				if ((int)m < 0)
					return MAX_JIFFY_OFFSET;
				return _msecs_to_jiffies(m);
```

固定time辅助行116–119；用途：B constant builtin gate and negative cast。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E038

[VERIFIED mykernel/time/systick/systick.h::msecs_to_jiffies]
```c
			} else {
				return __msecs_to_jiffies(m);
			}
```

固定time辅助行120–122；用途：B variable wrapper path。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E039

[VERIFIED mykernel/time/misc/time_misc.c::__msecs_to_jiffies]
```c
	if ((int)m < 0)
		return MAX_JIFFY_OFFSET;
	return _msecs_to_jiffies(m);
```

固定time辅助行102–104；用途：B variable ms negative gate and helper。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E040

[VERIFIED mykernel/time/systick/systick.h::_usecs_to_jiffies]
```c
				return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
```

固定time辅助行129–129；用途：B selected 1000Hz usec ceil。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E041

[VERIFIED mykernel/time/systick/systick.h::usecs_to_jiffies]
```c
			if (__builtin_constant_p(u)) {
				if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
					return MAX_JIFFY_OFFSET;
				return _usecs_to_jiffies(u);
```

固定time辅助行167–170；用途：B constant usec bound and helper。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E042

[VERIFIED mykernel/time/systick/systick.h::usecs_to_jiffies]
```c
			} else {
				return __usecs_to_jiffies(u);
			}
```

固定time辅助行171–173；用途：B variable usec wrapper。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E043

[VERIFIED mykernel/time/misc/time_misc.c::__usecs_to_jiffies]
```c
	if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	return _usecs_to_jiffies(u);
```

固定time辅助行116–118；用途：B variable usec upper test and helper。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E044

[VERIFIED mykernel/time/misc/time_misc.c::jiffies_to_msecs]
```c
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
	return (MSEC_PER_SEC / HZ) * j;
```

固定time辅助行170–171；用途：B selected ms return。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E045

[VERIFIED mykernel/time/misc/time_misc.c::jiffies_to_msecs]
```c
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
	return (j + (HZ / MSEC_PER_SEC) - 1)/(HZ / MSEC_PER_SEC);
#else
	return DIV_ROUND_UP(j * HZ_TO_MSEC_NUM, HZ_TO_MSEC_DEN);
#endif
```

固定time辅助行172–176；用途：B inactive alternative branches shown in source。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E046

[VERIFIED mykernel/time/misc/time_misc.c::jiffies_to_usecs]
```c
	BUILD_BUG_ON(HZ > USEC_PER_SEC);

#if !(USEC_PER_SEC % HZ)
	return (USEC_PER_SEC / HZ) * j;
```

固定time辅助行192–195；用途：B selected us return and BUILD_BUG_ON。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E047

[VERIFIED mykernel/time/misc/time_misc.c::jiffies_to_usecs]
```c
	return (j * HZ_TO_USEC_NUM) / HZ_TO_USEC_DEN;
#endif
```

固定time辅助行197–198；用途：B inactive fallback branch。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E048

[VERIFIED mykernel/time/systick/systick.h::jiffies_to_nsecs]
```c
			return (u64)jiffies_to_usecs(j) * NSEC_PER_USEC;
```

固定time辅助行44–44；用途：B optional uint-to-u64 nanos helper。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E049

[VERIFIED mykernel/time/misc/time_misc.c::timespec64_to_jiffies]
```c
	u64 sec = value->tv_sec;
	long nsec = value->tv_nsec + TICK_NSEC - 1;
```

固定time辅助行144–145；用途：C sec cast and nsec add tick-1。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E050

[VERIFIED mykernel/time/misc/time_misc.c::timespec64_to_jiffies]
```c
	if (sec >= MAX_SEC_IN_JIFFIES){
		sec = MAX_SEC_IN_JIFFIES;
		nsec = 0;
	}
```

固定time辅助行147–150；用途：C seconds cap and nsec zeroing。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E051

[VERIFIED mykernel/time/misc/time_misc.c::timespec64_to_jiffies]
```c
	return ((sec * SEC_CONVERSION) +
		(((u64)nsec * NSEC_CONVERSION) >>
		 (NSEC_JIFFIE_SC - SEC_JIFFIE_SC))) >> SEC_JIFFIE_SC;
```

固定time辅助行151–153；用途：C scaled sum shift formula。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E052

[VERIFIED mykernel/time/misc/time_misc.c::jiffies_to_timespec64]
```c
	u32 rem;
	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
						NSEC_PER_SEC, &rem);
	value->tv_nsec = rem;
}
```

固定time辅助行214–218；用途：C multiply tick then divide and save fields。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E053

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```

固定time辅助行43–44；用途：C remainder and quotient actual body。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E054

[VERIFIED mykernel/time/misc/time_misc_types.h::timespec64]
```c
	struct timespec64 {
		time64_t	tv_sec;			/* seconds */
		long		tv_nsec;		/* nanoseconds */
	};
```

固定time辅助行7–10；用途：C struct timespec64 definition and fields。结论只达源级，完整预处理、类型绑定、运行与硬件均未核定。

### DT108-E055

[VERIFIED mykernel/time/systick/systick_const.h::SH_DIV]
```c
					(DEN) / 2) /				\
				(DEN)							\
			)
```

固定time辅助行52–54；用途：A scaled quotient macro's rounded remainder and closing definition。只代表源码运算及前提，不代表执行或设备取样。

### DT108-E056

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	return true;
```

固定time辅助行412–412；用途：D00 R106-02-08 callee success return。只代表源码运算及前提，不代表执行或设备取样。

### DT108-E057

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_add_ns]
```c
			a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
			a->tv_nsec = ns;
```

固定time辅助行154–155；用途：D00 R107-02 fractional normalization with u32 quotient helper。只代表源码运算及前提，不代表执行或设备取样。

## POSITION_ONLY：不算P2短引文的函数签名、头文件/清单与编译分支定位

这些定位与真实短引文分账；实际选编/宏展开、真实类型宽度和链接均未运行。每一位置均取固定 time 源码对应文件；不把 include、函数头、extern 声明或注释冒称为函数体短锚。

- **DT108-P01**：`mykernel/include/linux/kernel/delay.h#L26`；第一条 include <asm/delay.h>：源级头引用。
- **DT108-P02**：`mykernel/arch/x86_64/include/asm/delay.h#L7`；第二条 include <asm-generic/delay.h>：源级头引用。
- **DT108-P03**：`mykernel/arch/x86_64/include/asm/syscalls_64.h#L50`；gettimeofday 宏实例 __SYSCALL(__NR_gettimeofday, sys_gettimeofday)。
- **DT108-P04**：`mykernel/arch/x86_64/include/asm/syscalls_64.h#L71`；clock_gettime 宏实例 __SYSCALL(__NR_clock_gettime, sys_clock_gettime)。
- **DT108-P05**：`mykernel/arch/x86_64/entry/syscall_64.c#L11-12`；第一次 include/undef 的位置；宏名仍为 __SYSCALL。
- **DT108-P06**：`mykernel/arch/x86_64/entry/syscall_64.c#L21-23`；第二次 include/undef 的位置；数组本体另看E005。
- **DT108-P07**：`mykernel/arch/x86_64/entry/syscall_64.c#L30-31`；第三次include在switch内；所读文件未见第三次#undef。
- **DT108-P08**：`mykernel/include/linux/kernel/syscalls.h#L93-102`；CONFIG_ARCH_HAS_SYSCALL_WRAPPER 条件包含 asm/syscall_wrapper.h。
- **DT108-P09**：`mykernel/time/systick/systick.h#L47`；HZ≤MSEC_PER_SEC及整除时选择_msecs_to_jiffies的首分支。
- **DT108-P10**：`mykernel/time/systick/systick.h#L125`；USEC_PER_SEC能整除HZ时选择_usecs_to_jiffies的首分支。
- **DT108-P11**：`mykernel/time/misc/time_misc.c#L168`；jiffies_to_msecs 函数返回类型uint；实际所选return见E044。
- **DT108-P12**：`mykernel/time/misc/time_misc.c#L186`；jiffies_to_usecs 函数返回类型uint；实际所选return见E046。
- **DT108-P13**：`mykernel/time/misc/time_misc.c#L142`；timespec64_to_jiffies 函数返回类型ulong及指针形参。
- **DT108-P14**：`mykernel/time/misc/time_misc.c#L208`；jiffies_to_timespec64 返回类型void及结构指针参数。
- **DT108-P15**：`mykernel/time/systick/systick_const.h#L157-160`；SEC_JIFFIE_SC条件判定、undef及第二次定义，强制区别初值与生效结果。
- **DT108-P16**：`mykernel/time/time_const.h#L4,6,9`；HZ→time_misc_const→systick_const 按三个独立include语句进入时间常量总头。
- **DT108-P17**：`mykernel/time/systick/systick.h#L5,7-9,31,33`；DEBUG及SYSTICK_DEFINATION选择、宏与类型总头的include位置。
- **DT108-P18**：`mykernel/time/systick/systick.c#L1-2`；该编译单元定义SYSTICK_DEFINATION后包括systick.h的源码位置。
- **DT108-P19**：`mykernel/include/linux/kernel/math64.h#L41-42`；div_u64_rem(u64,u32,u32*)是被调签名，实际体内商余见E053。
- **DT108-P20**：`mykernel/time/misc/time_misc.c#L97,114`；两个带__前缀的运行期函数签名位置：ulong返回、uint输入。
- **DT108-P21**：`mykernel/time/systick/systick.h#L53-55,113-115,126-128,164-166`；四个msec/usec宏或内联helper的签名与类型位置，非体内锚。
- **DT108-P22**：`mykernel/time/misc/time_misc_types.h#L7-10`；timespec64实际字段结构定义，已由E054支持。
- **DT108-P23**：`mykernel/scripts/options_flags.cmake#L78-85`；set(INCLUDE_DIRS)目标目录全范围，独立架构头根第83行由E007覆盖。
- **DT108-P24**：`mykernel/scripts/target_kernel.cmake#L2-4`；target_include_directories(kernel PRIVATE ${INCLUDE_DIRS})使用链，由E008覆盖。

- **DT108-P25**：`mykernel/time/systick/systick_const.h#L28`；条件 (HZ>=768 && HZ<1536) 选择E026的SHIFT_HZ=10；仅条件位置，不混入独立宏定义短锚。
