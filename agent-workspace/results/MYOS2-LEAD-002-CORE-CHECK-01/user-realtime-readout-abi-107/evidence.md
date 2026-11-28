# 第107批固定源码引文：D00证据纠正

固定 `time@a039d9803ade`；`RT107-E001` 起为新引文。定义体内引文仅证明对应直接来源；函数签名、include或全注释位置不计作函数体P2锚。

### RT107-E001

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cpu_khz]
```c
uint __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */
```

固定辅助行36–36；用途：D00两个频率对象之一；运行、预处理及ABI选择均待证。

### RT107-E002

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_khz]
```c
uint __read_mostly tsc_khz;
```

固定辅助行38–38；用途：D00第二频率对象；运行、预处理及ABI选择均待证。

### RT107-E003

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;
```

固定辅助行52–53；用途：D00零值结构与rem局部；运行、预处理及ABI选择均待证。

### RT107-E004

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
```

固定辅助行55–57；用途：D00正值条件；运行、预处理及ABI选择均待证。

### RT107-E005

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	} else if (nsec < 0) {
```

固定辅助行58–58；用途：D00负值条件门；运行、预处理及ABI选择均待证。

### RT107-E006

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
	}
```

固定辅助行64–66；用途：D00负值分支及后括号；运行、预处理及ABI选择均待证。

### RT107-E007

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	return ts;
```

固定辅助行68–68；用途：D00末尾返回；运行、预处理及ABI选择均待证。

### RT107-E008

[VERIFIED mykernel/include/linux/sched/clock.h::local_clock]
```c
		return native_sched_clock();
```

固定辅助行53–53；用途：D00实际返回native；运行、预处理及ABI选择均待证。

### RT107-E009

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
		*remainder = dividend % divisor;
		return dividend / divisor;
```

固定辅助行43–44；用途：D00函数体商余；运行、预处理及ABI选择均待证。

### RT107-E010

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cycles_2_ns]
```c
	__cyc2ns_read(&data);

	ns = data.cyc2ns_offset;
	ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);
```

固定辅助行127–130；用途：D00别名实际使用；运行、预处理及ABI选择均待证。

### RT107-E011

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	const size_t static_size = __per_cpu_end - __per_cpu_start;
	size_t reserved_size = 2 * PAGE_SIZE;
	size_t dyn_size = 0;
	size_t size_sum = static_size + reserved_size + dyn_size;
	chunk_size = roundup_pow_of_two(size_sum);
```

固定辅助行340–344；用途：D00 helper内的static_size；运行、预处理及ABI选择均待证。

### RT107-E012

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::get_loops_per_jiffy]
```c
	u64 lpj = (u64)tsc_khz * KHZ;

	do_div(lpj, HZ);
	return lpj;
```

固定辅助行417–420；用途：D00无签名正文除法；运行、预处理及ABI选择均待证。

### RT107-E013

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_enable_sched_clock]
```c
	loops_per_jiffy = get_loops_per_jiffy();
	// use_tsc_delay();
```

固定辅助行425–426；用途：D00覆盖全局及注释；运行、预处理及ABI选择均待证。

### RT107-E014

[VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
	u32 eax, ebx, ecx;
	u64 lpj, tsc_khz;

	eax = vmware_hypercall3(VMWARE_CMD_GETHZ, UINT_MAX, &ebx, &ecx);
```

固定辅助行154–157；用途：D00局部声明与请求；运行、预处理及ABI选择均待证。

### RT107-E015

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__sdelay]
```c
	__const_udelay(secs * 0x100000000);	/* 2**32 (rounded up) */
```

固定辅助行203–203；用途：D00秒包装体；运行、预处理及ABI选择均待证。

### RT107-E016

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__mdelay]
```c
	__const_udelay(msecs * 0x00418938);	/* 2**32 / 1000 (rounded up) */
```

固定辅助行207–207；用途：D00毫秒包装体；运行、预处理及ABI选择均待证。

### RT107-E017

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__udelay]
```c
	__const_udelay(usecs * 0x000010c7);	/* 2**32 / 1000000 (rounded up) */
```

固定辅助行211–211；用途：D00微秒包装体；运行、预处理及ABI选择均待证。

### RT107-E018

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__ndelay]
```c
	__const_udelay(nsecs * 0x00005);	/* 2**32 / 1000000000 (rounded up) */
```

固定辅助行215–215；用途：D00纳秒包装体；运行、预处理及ABI选择均待证。

### RT107-E019

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	unsigned long lpj = loops_per_jiffy;
	int d0;
```

固定辅助行191–192；用途：D00全局读取及局部；运行、预处理及ABI选择均待证。

### RT107-E020

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__delay]
```c
	delay_fn(loops);
```

固定辅助行186–186；用途：D00函数指针实际调用；运行、预处理及ABI选择均待证。

### RT107-E021

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
	unsigned long loops = (unsigned long)__loops;

	asm volatile(	"	test	%0,	%0	\n"
```

固定辅助行39–41；用途：D00零输入门到汇编开始；运行、预处理及ABI选择均待证。

### RT107-E022

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	u32 mult = clksrc->mult;
	u32 shift = clksrc->shift;
```

固定辅助行208–209；用途：D00实际的mult/shift字段；运行、预处理及ABI选择均待证。

### RT107-E023

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	ioapic_retentry_T entry;
	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
	entry.deliver_mode = APIC_DELIVERY_MODE_FIXED;
	entry.mask = APIC_LVT_MASKED >> 16;
```

固定辅助行541–545；用途：D00另一IRQ入口活动体；运行、预处理及ABI选择均待证。

### RT107-E024

[VERIFIED mykernel/init/main.c::loops_per_jiffy]
```c
unsigned long loops_per_jiffy = 10371364;
```

固定辅助行68–68；用途：D00单条活动初始化；运行、预处理及ABI选择均待证。

### RT107-E025

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_fn]
```c
static void (*delay_fn)(u64) = delay_loop;
```

固定辅助行33–33；用途：D00实际函数指针初值；运行、预处理及ABI选择均待证。

### RT107-E026

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::KHZ]
```c
#define KHZ	1000
```

固定辅助行43–43；用途：D00 KHZ真实定义；运行、预处理及ABI选择均待证。

### RT107-E027

[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```

固定辅助行9–9；用途：D00 HZ活动宏；运行、预处理及ABI选择均待证。

### RT107-E028

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::mdelay]
```c
						__const_udelay((n) * 0x418938ul);	\
				} else {									\
					__mdelay(n);							\
```

固定辅助行41–43；用途：D00 mdelay活动0x418938；运行、预处理及ABI选择均待证。

### RT107-E029

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::udelay]
```c
	#define udelay(n) ({								\
				if (__builtin_constant_p(n)) {			\
					if ((n) / 20000 >= 1)				\
						__bad_udelay();					\
					else								\
```

固定辅助行48–52；用途：D00 udelay商判定宏；运行、预处理及ABI选择均待证。

### RT107-E030

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::udelay]
```c
						__const_udelay((n) * 0x10c7ul);	\
				} else {								\
					__udelay(n);						\
				}										\
			})
```

固定辅助行53–57；用途：D00 udelay正常/变量分支；运行、预处理及ABI选择均待证。

### RT107-E031

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	xloops *= 4;
	asm(	"mull	%%edx	\n\t"
		:	"=d"(xloops), "=&a"(d0)
		:	"1"(xloops), "0"(lpj * (HZ / 4)));
```

固定辅助行194–197；用途：D00高字汇编绑定；运行、预处理及ABI选择均待证。

### RT107-E032

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	__delay(++xloops);
```

固定辅助行199–199；用途：D00末尾自增；运行、预处理及ABI选择均待证。

### RT107-E033

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
	asm volatile(	"	test	%0,	%0	\n"
					"	jz		3f		\n"
					"	jmp		1f		\n"
					".align	16			\n"
					"1:	jmp		2f		\n"
```

固定辅助行41–45；用途：D00 test与jz；运行、预处理及ABI选择均待证。

### RT107-E034

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
					"2:	dec		%0		\n"
					"	jnz		2b		\n"
					"3:	dec		%0		\n"
				: /* we don't need output */
				:	"a"(loops));
```

固定辅助行47–51；用途：D00内部/末尾dec与输入约束；运行、预处理及ABI选择均待证。

### RT107-E035

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
	udelay(1);
```

固定辅助行222–224；用途：D00第一次比较器写与udelay；运行、预处理及ABI选择均待证。

### RT107-E036

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
	hpet_start_counter();
```

固定辅助行232–233；用途：D00第二比较器写与启动；运行、预处理及ABI选择均待证。

### RT107-E037

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_stop_counter();
	delta = ((uint64_t)(NSEC_PER_SEC / HZ)) * mult;
	delta >>= shift + 7;
	now = hpet_readl(HPET_COUNTER);
	cmp = now + (uint)delta;
```

固定辅助行214–218；用途：D00 delta和cmp计算；运行、预处理及ABI选择均待证。

### RT107-E038

[VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	hpet_time_init(); /* == x86_init.timers.timer_init() */
```

固定辅助行51–51；用途：D00晚期hpet同级调用；运行、预处理及ABI选择均待证。

### RT107-E039

[VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	tsc_init();
```

固定辅助行58–58；用途：D00晚期tsc同级调用；运行、预处理及ABI选择均待证。

### RT107-E040

[VERIFIED mykernel/include/linux/kernel/math.h::__abs_choose_expr]
```c
				__builtin_types_compatible_p(typeof(x),   signed type) ||	\
				__builtin_types_compatible_p(typeof(x), unsigned type),		\
				({ signed type __x = (x); __x < 0 ? -__x : __x; }), other	\
```

固定辅助行137–139；用途：D00 abs有符号临时；运行、预处理及ABI选择均待证。

# 第107批固定源码引文：双墙钟读取接口 A/B/C

固定源码 `time@a039d9803ade`。与D00之RT107-E001～E040独立账相接，本节从E041开始；源码宏定义、struct字段、对象初始化和函数体内连续行分别作为定义单元。实际宏实例化行、外部签名和include位置另列POSITION_ONLY而不是伪造函数体。

### RT107-E041

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_gettimeofday]
```c
	#define __NR_gettimeofday	96
```

固定time辅助行119–119；用途：A系统调用号96。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E042

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_clock_gettime]
```c
	#define __NR_clock_gettime		228
```

固定time辅助行277–277；用途：A系统调用号228。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E043

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL declaration macro]
```c
#define __SYSCALL(nr, sym) extern long __x64_##sym(const pt_regs_s *);
```

固定time辅助行10–10；用途：A同一清单生成外部入口声明。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E044

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL table macro]
```c
#define __SYSCALL(nr, sym)	[nr] = __x64_##sym,
```

固定time辅助行19–19；用途：A同一清单生成表项。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E045

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL switch macro]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

固定time辅助行26–26；用途：A同一清单生成switch case。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E046

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

固定time辅助行29–31；用途：A活动switch的include及default。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E047

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE2]
```c
	#define MYOS_SYSCALL_DEFINE2(name, ...)	__MYOS_SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
```

固定time辅助行1385–1385；用途：A两参数实例到x架构宏。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E048

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__MAP2]
```c
	#define __MAP2(m, t, a, ...)	m(t, a), __MAP1(m, __VA_ARGS__)
```

固定time辅助行115–115；用途：A类型名字对逐项映射。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E049

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__SC_DECL]
```c
	#define __SC_DECL(t, a)		t a
```

固定time辅助行122–122；用途：A函数参数声明宏。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E050

[VERIFIED mykernel/include/linux/kernel/syscalls.h::__SC_CAST]
```c
	#define __SC_CAST(t, a)		(__force t) a
```

固定time辅助行128–128；用途：A实参类型转换宏。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E051

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__X86_64_MAP2]
```c
	#define __X86_64_MAP2(m, t1, a1, t2, a2)									\
				m(t1, regs->di), m(t2, regs->si)
```

固定time辅助行17–18；用途：A寄存器di/si两个实参。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E052

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__ARCH_MAP]
```c
	#define __ARCH_MAP(n, ...)	__X86_64_MAP##n(__VA_ARGS__)
```

固定time辅助行30–30；用途：A按参数个数的宏拼接。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E053

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

固定time辅助行104–108；用途：A生成stub和转调。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E054

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

固定time辅助行241–245；用途：A创建se/do和x64 stub声明。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E055

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
```

固定time辅助行246–249；用途：A se包装实际调用do。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E056

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				}														\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
```

固定time辅助行250–251；用途：A返回和do函数体实例。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E057

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
	if (likely(tv != NULL)) {
		timespec64_s ts;

		ktime_get_real_ts64(&ts);
```

固定time辅助行16–19；用途：A gettimeofday tv门与直接取样。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E058

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
```

固定time辅助行20–22；用途：C两次put_user和EFAULT。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E059

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
	// if (unlikely(tz != NULL)) {
	// 	if (copy_to_user(tz, &sys_tz, sizeof(sys_tz)))
	// 		return -EFAULT;
	// }
	return 0;
```

固定time辅助行24–28；用途：C tz注释及正常return0。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E060

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	const k_clock_s *kc = clockid_to_kclock(which_clock);
	timespec64_s kernel_tp;
	int error;

	if (!kc)
```

固定time辅助行37–41；用途：A/B clockid到回调结构以及null门。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E061

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	if (!kc)
		return -EINVAL;

	error = kc->clock_get_timespec(which_clock, &kernel_tp);
```

固定time辅助行41–44；用途：B回调取得时间数据。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E062

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	error = kc->clock_get_timespec(which_clock, &kernel_tp);

	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;
```

固定time辅助行44–47；用途：C调用put_timespec64及EFAULT。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E063

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	return error;
```

固定time辅助行49–49；用途：C返回error。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E064

[VERIFIED mykernel/include/uapi/linux/time.h::CLOCK_REALTIME]
```c
	#define CLOCK_REALTIME				0
```

固定time辅助行49–49；用途：B CLOCK_REALTIME宏为0。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E065

[VERIFIED mykernel/time/timer/posix-timers.c::posix_get_realtime_timespec]
```c
	ktime_get_real_ts64(tp);
	return 0;
```

固定time辅助行30–31；用途：B POSIX REALTIME回调调用公共读者。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E066

[VERIFIED mykernel/time/timer/posix-timers.c::clock_realtime]
```c
	.clock_get_timespec		= posix_get_realtime_timespec,
```

固定time辅助行92–92；用途：B REALTIME时钟对象绑定。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E067

[VERIFIED mykernel/time/timer/posix-timers.c::posix_clocks]
```c
	[CLOCK_REALTIME]		    = &clock_realtime,
```

固定time辅助行175–175；用途：B 索引0绑定clock_realtime。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E068

[VERIFIED mykernel/time/timer/posix-timers.c::clockid_to_kclock]
```c
	clockid_t idx = id;

	if (id < 0) {
		pr_err("Dynamic timer currently Not supported.\n");
		while (1);
```

固定time辅助行190–194；用途：B负clockid处理位置非本域。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E069

[VERIFIED mykernel/time/timer/posix-timers.c::clockid_to_kclock]
```c
	if (id >= ARRAY_SIZE(posix_clocks))
		return NULL;

	// return posix_clocks[array_index_nospec(idx, ARRAY_SIZE(posix_clocks))];
	return posix_clocks[idx];
```

固定time辅助行199–203；用途：B范围门和数组返回。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E070

[VERIFIED mykernel/time/timer/timer_types.h::k_clock.clock_get_timespec]
```c
		/* Returns the clock value in the current time namespace. */
		int	(*clock_get_timespec)(const clockid_t which_clock,
				timespec64_s *tp);
```

固定time辅助行90–92；用途：B回调字段声明。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E071

[VERIFIED mykernel/time/timekeeping/timekeeping.c::ktime_get_real_ts64]
```c
	timekeeper_s *tk = &tk_core.timekeeper;
	uint seq;
	u64 nsecs;
```

固定time辅助行75–77；用途：B核心timekeeper对象与局部量。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E072

[VERIFIED mykernel/time/timekeeping/timekeeping.c::ktime_get_real_ts64]
```c
		ts->tv_sec = tk->xtime_sec;
		nsecs = timekeeping_get_ns(&tk->tkr_mono);
```

固定time辅助行84–85；用途：B读取xtime秒与nsecs。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E073

[VERIFIED mykernel/time/timekeeping/timekeeping.c::ktime_get_real_ts64]
```c
	// } while (read_seqcount_retry(&tk_core.seq, seq));

	ts->tv_nsec = 0;
	timespec64_add_ns(ts, nsecs);
```

固定time辅助行87–90；用途：B注释seqretry与归一化请求。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E074

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_get_ns]
```c
	// return __timekeeping_get_ns(tkr);
	return timekeeping_cycles_to_ns(tkr, tk_clock_read(tkr));
}
```

固定time辅助行227–229；用途：B源级clock读取调用。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E075

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	/* Calculate the delta since the last update_wall_time() */
	u64 mask = tkr->mask, delta = (cycles - tkr->cycle_last) & mask;
```

固定time辅助行199–200；用途：B从当前clock计算相对delta。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E076

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c

		return delta_to_ns_safe(tkr, delta);
	}

	return ((delta * tkr->mult) + tkr->xtime_nsec) >> tkr->shift;
```

固定time辅助行214–218；用途：B有条件的cycle转纳秒值。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E077

[VERIFIED mykernel/time/timekeeping/timekeeping.h::tk_clock_read]
```c
			clocksrc_s *clock = READ_ONCE(tkr->clock);
			return clock->read(clock);
		}
```

固定time辅助行81–83；用途：B字段READ_ONCE后经函数指针调用。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E078

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_add_ns]
```c
			a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
			a->tv_nsec = ns;
		}
```

固定time辅助行154–156；用途：B纳秒求商归一化。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E079

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
		u32 ret = 0;
		while (dividend >= divisor) {
```

固定time辅助行13–14；用途：B整除循环门。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E080

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
			dividend -= divisor;
			ret++;
		}
		*remainder = dividend;
		return ret;
```

固定time辅助行19–23；用途：B逐次减法与秒溢出余数。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E081

[VERIFIED mykernel/time/misc/time_misc_types.h::timespec64]
```c
		time64_t	tv_sec;			/* seconds */
		long		tv_nsec;		/* nanoseconds */
```

固定time辅助行8–9；用途：C内核timespec字段。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E082

[VERIFIED mykernel/include/uapi/linux/time_types.h::__kernel_old_timeval]
```c
			__kernel_long_t tv_sec;
			__kernel_long_t tv_usec;
```

固定time辅助行26–27；用途：C用户老timeval为两__kernel_long_t。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E083

[VERIFIED mykernel/include/uapi/linux/time_types.h::__kernel_timespec]
```c
		__kernel_time64_t       tv_sec;                 /* seconds */
		long long               tv_nsec;                /* nanoseconds */
```

固定time辅助行8–9；用途：C用户timespec为64秒和64纳秒字段。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E084

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_long_t]
```c
		typedef long			__kernel_long_t;
```

固定time辅助行16–16；用途：C long类型别名。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E085

[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_time64_t]
```c
	typedef long long			__kernel_time64_t;
```

固定time辅助行94–94；用途：C long long时间类型。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E086

[VERIFIED mykernel/time/misc/time_misc.c::put_timespec64]
```c
	__kernel_timespec_s kts = {
		.tv_sec = ts->tv_sec,
		.tv_nsec = ts->tv_nsec
	};
```

固定time辅助行307–310；用途：C局部UAPI对象字段赋值。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E087

[VERIFIED mykernel/time/misc/time_misc.c::put_timespec64]
```c
	return copy_to_user(uts, &kts, sizeof(kts)) ? -EFAULT : 0;
```

固定time辅助行312–312；用途：C按sizeof(kts)复制并转换错误码。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E088

[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
	#define put_user(x, ptr) ({					\
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```

固定time辅助行191–194；用途：C两次标量写的宏实现。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E089

[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```

固定time辅助行176–180；用途：C复制前检查和底层残量。仅作源级定义或调用证据，运行/ABI实测未执行。

### RT107-E090

[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
		return _inline_copy_to_user(to, from, n);
```

固定time辅助行201–201；用途：C包装到inline请求。仅作源级定义或调用证据，运行/ABI实测未执行。


## POSITION_ONLY：定义体外的签名、宏实例、头文件位置与注释

以下短片段属于原位定位或声明，**不是函数体内P2引文**，不能与上方源级证据标签混计。所有文件均取固定time@a039d9803ade，行号为本轮实际取回的1-based位置。

- RT107-P01，mykernel/arch/x86_64/include/asm/syscalls_64.h:50：宏实例（不是生成后的实际链接对象）。
```c
__SYSCALL(__NR_gettimeofday, sys_gettimeofday)
```
- RT107-P02，同文件:71：宏实例。
```c
__SYSCALL(__NR_clock_gettime, sys_clock_gettime)
```
- RT107-P03，mykernel/time/syscall.c:12–14：gettimeofday的宏实例/形参签名位置。
```c
MYOS_SYSCALL_DEFINE2(gettimeofday,
		struct __kernel_old_timeval __user *, tv,
		struct timezone __user *, tz)
```
- RT107-P04，同文件:33–35：clock_gettime的宏实例/形参签名位置。
```c
MYOS_SYSCALL_DEFINE2(clock_gettime,
		const clockid_t, which_clock,
		__kernel_timespec_s __user *, tp)
```
- RT107-P05，mykernel/include/linux/kernel/syscalls.h:93–102：CONFIG_ARCH_HAS_SYSCALL_WRAPPER 下的 #include <asm/syscall_wrapper.h>。配置开关需与同一time options_flags.cmake:65及最终编译分开登记。
- RT107-P06，mykernel/arch/x86_64/include/asm/syscall_wrapper.h:241–251：__MYOS_SYSCALL_DEFINEx宏定义；RT107-E054/E055/E056分别为分段的宏体证据。
- RT107-P07，mykernel/time/misc/time_misc.c:50：返回类型timespec64_s、形参s64 nsec的函数签名；RT107-E003～E007为内部计算和return；同页第58行是真正负数门。
- RT107-P08，mykernel/arch/x86_64/kernel/tsc.c:36及38：分别是cpu_khz、tsc_khz的独立全局定义，RT107-E001/E002；禁止复用旧BD106-E007组合名。
- RT107-P09，mykernel/include/linux/kernel/delay.h:24（extern unsigned long loops_per_jiffy），:26（#include <asm/delay.h>）；mykernel/arch/x86_64/include/asm/delay.h:7（#include <asm-generic/delay.h>）；这些属于声明或头文件导入位置，不是函数体源锚。
- RT107-P10，mykernel/include/linux/kernel/asm-generic/delay.h:6–9分别为__bad_sdelay、__bad_mdelay、__bad_udelay、__bad_ndelay的四条独立extern声明，不用通配__bad_*冒充单一定义。实际udelay宏中的调用由RT107-E029/E030支持。
- RT107-P11，mykernel/arch/x86_64/kernel/tsc.c:40–43里DUMMY_TSC_KHZ与KHZ是两种不同宏定义，活动KHZ=1000由RT107-E026单行取证。
- RT107-P12，mykernel/include/linux/kernel/asm-generic/param.h:7的#undef HZ与:8注释旧宏仅位置，不与活动:9 RT107-E027混成一条定义。
- RT107-P13，mykernel/init/main.c:67的初始(1<<12)为注释；活动第68行由RT107-E024取证。mykernel/arch/x86_64/lib/delay.c:32的__ro_after_init候选是注释，活动第33行由RT107-E025取证。
- RT107-P14，mykernel/arch/x86_64/lib/delay.c:162–165的use_tsc_delay整体注释；mykernel/arch/x86_64/kernel/tsc.c:426为注释调用，源级仅证明已读函数没有活动切换。
- RT107-P15，mykernel/include/linux/kernel/asm-generic/delay.h:35注释写0x418937，但宏体:41的数值为0x418938，后者见RT107-E028。
- RT107-P16，mykernel/arch/x86_64/kernel/hpet.c:203函数头与:205–207注释旧mult/shift仅作位置；活动字段读取:208–209见RT107-E022。:539函数头与:541–545活动配置体须分账，后者RT107-E023。
- RT107-P17，mykernel/time/misc/time_misc.c:304–306是put_timespec64的真实函数签名；:307–312才是内层结构赋值和copy_to_user调用，见RT107-E086/E087。

- RT107-P18，mykernel/include/linux/sched/clock.h:51：`static __always_inline u64 local_clock(void)`为真实返回类型签名；调用体内实际return见RT107-E008。
- RT107-P19，mykernel/include/linux/kernel/math64.h:41–42：`div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)`为实际签名与类型位置；其函数体写余数及return见RT107-E009。
