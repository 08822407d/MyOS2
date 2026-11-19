# 第106批固定 time 源级证据

固定来源 time@a039d9803ade，通道 GitHub connector。全部新引文系固定源中一个真实函数、对象、宏或声明的连续1–5行；引文只证源码层面，非编译或运行实测。

### BD106-E001

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-DCONFIG_HYPERVISOR_GUEST \
	-DCONFIG_KVM_GUEST \
```

固定辅助行 68–69；消费边界：D00配置；未证编译或实机执行。

### BD106-E002

[VERIFIED mykernel/cpu/hypervisor.c::hypervisors[]]
```c
	&x86_hyper_vmware,
	// &x86_hyper_ms_hyperv,
#ifdef CONFIG_KVM_GUEST
	&x86_hyper_kvm,
#endif
```

固定辅助行 39–43；消费边界：D00候选；未证编译或实机执行。

### BD106-E003

[VERIFIED mykernel/cpu/hypervisor.c::detect_hypervisor_vendor]
```c
	if (h)
		pr_info("Hypervisor detected: %s\n", h->name);

	return h;
```

固定辅助行 75–78；消费边界：D00返回；未证编译或实机执行。

### BD106-E004

[VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	h = detect_hypervisor_vendor();

	if (!h)
		return;
```

固定辅助行 98–101；消费边界：D00接收；未证编译或实机执行。

### BD106-E005

[VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	copy_array(&h->init, &x86_init.hyper, sizeof(h->init));
	copy_array(&h->runtime, &x86_platform.hyper, sizeof(h->runtime));

	x86_hyper_type = h->type;
	x86_init.hyper.init_platform();
```

固定辅助行 103–107；消费边界：D00复制调用；未证编译或实机执行。

### BD106-E006

[VERIFIED mykernel/cpu/hypervisor.c::copy_array]
```c
	for (i = 0; i < n; i++)
		if (from[i])
			to[i] = from[i];
```

固定辅助行 89–91；消费边界：D00非空；未证编译或实机执行。

### BD106-E007

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cpu_khz/tsc_khz]
```c
uint __read_mostly cpu_khz;	/* TSC clocks / usec, not used here */

uint __read_mostly tsc_khz;
```

固定辅助行 36–38；消费边界：D00全局；未证编译或实机执行。

### BD106-E008

[VERIFIED mykernel/include/linux/kernel/math.h::abs]
```c
	#define abs(x)																			\
				__abs_choose_expr(x, long long,												\
					__abs_choose_expr(x, long,												\
						__abs_choose_expr(x, int,											\
							__abs_choose_expr(x, short,										\
```

固定辅助行 119–123；消费边界：D00宏；未证编译或实机执行。

### BD106-E009

[VERIFIED mykernel/include/linux/kernel/math.h::__abs_choose_expr]
```c
	#define __abs_choose_expr(x, type, other) __builtin_choose_expr(		\
				__builtin_types_compatible_p(typeof(x),   signed type) ||	\
				__builtin_types_compatible_p(typeof(x), unsigned type),		\
				({ signed type __x = (x); __x < 0 ? -__x : __x; }), other	\
			)
```

固定辅助行 136–140；消费边界：D00有符号暂存；未证编译或实机执行。

### BD106-E010

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	} else {
		/* We should not be here with non-native cpu calibration */
		// WARN_ON(x86_platform.calibrate_cpu != native_calibrate_cpu);
		// cpu_khz = pit_hpet_ptimer_calibrate_cpu();
	}
```

固定辅助行 384–388；消费边界：D00晚期else；未证编译或实机执行。

### BD106-E011

[VERIFIED mykernel/include/linux/sched/clock.h::local_clock]
```c
	static __always_inline u64 local_clock(void) {
		// return sched_clock();
		return native_sched_clock();
	}
```

固定辅助行 51–54；消费边界：D00 u64；未证编译或实机执行。

### BD106-E012

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
timespec64_s ns_to_timespec64(s64 nsec)
{
	timespec64_s ts = { 0, 0 };
	s32 rem;
```

固定辅助行 50–53；消费边界：D00 s64；未证编译或实机执行。

### BD106-E013

[VERIFIED mykernel/include/linux/kernel/math64.h::div_u64_rem]
```c
	static inline u64
	div_u64_rem(u64 dividend, u32 divisor, u32 *remainder) {
		*remainder = dividend % divisor;
		return dividend / divisor;
	}
```

固定辅助行 41–45；消费边界：D00 u32*；未证编译或实机执行。

### BD106-E014

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

固定辅助行 64–65；消费边界：D00负分支；未证编译或实机执行。

### BD106-E015

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cyc2ns_read]
```c
#define __cyc2ns_read		__simple_cyc2ns_read
```

固定辅助行 32–32；消费边界：D00别名；未证编译或实机执行。

### BD106-E016

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cycles_2_ns]
```c
__cycles_2_ns(ulonglong cyc) {
	struct cyc2ns_data data;
	ulonglong ns;

	__cyc2ns_read(&data);
```

固定辅助行 123–127；消费边界：D00真实别名调用；未证编译或实机执行。

### BD106-E017

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	/* No locking but a rare wrong value is not a big deal: */
	return (jiffies_64 - INITIAL_JIFFIES) * (1000000000 / HZ);
```

固定辅助行 236–237；消费边界：D00计数器输入；未证编译或实机执行。

### BD106-E018

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	simple_pcpu_setup_first_chunk();
	/* alrighty, percpu areas up and running */
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
```

固定辅助行 73–77；消费边界：D00delta；未证编译或实机执行。

### BD106-E019

[VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```

固定辅助行 76–79；消费边界：D00发布；未证编译或实机执行。

### BD106-E020

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
void simple_pcpu_setup_first_chunk()
{
	// 计算关键全局数据
	const size_t static_size = __per_cpu_end - __per_cpu_start;
	size_t reserved_size = 2 * PAGE_SIZE;
```

固定辅助行 337–341；消费边界：D00 helper；未证编译或实机执行。

### BD106-E021

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	pcpu_first_chunk = early_pcpu_create_chunk();
	pcpu_base_addr = pcpu_first_chunk->data;
	// 手工初始化 pcpu_first_chunk
	pcpu_first_chunk->free_bytes = chunk_size - size_sum;
	pcpu_first_chunk->free_offset = size_sum;
```

固定辅助行 348–352；消费边界：D00基址；未证编译或实机执行。

### BD106-E022

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	// 复制“模板”percpu段到运行时cpu0的percpu段
	// 主要是将静态初始化的一些percpu变量的值复制过来
	memcpy(pcpu_first_chunk->data, &__per_cpu_load, static_size);
```

固定辅助行 354–356；消费边界：D00模板；未证编译或实机执行。

### BD106-E023

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	// 计算AP的percpu空间相对BSP的偏移量
	pcpu_unit_offsets =
		myos_memblock_alloc_normal(nr_cpu_ids * sizeof(void *), 8);
	for (int i = 0; i < nr_cpu_ids; i++)
		((ulong *)pcpu_unit_offsets)[i] = i * static_size;
```

固定辅助行 359–363；消费边界：D00偏移；未证编译或实机执行。

### BD106-E024

[VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	/* Setup the legacy timers */
	hpet_time_init(); /* == x86_init.timers.timer_init() */
```

固定辅助行 50–51；消费边界：D00 hpet sibling；未证编译或实机执行。

### BD106-E025

[VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	// x86_init.irqs.intr_mode_init();
	tsc_init();
```

固定辅助行 57–58；消费边界：D00 tsc sibling；未证编译或实机执行。

### BD106-E026

[VERIFIED mykernel/init/main.c::loops_per_jiffy]
```c
// unsigned long loops_per_jiffy = (1<<12);
unsigned long loops_per_jiffy = 10371364;
```

固定辅助行 67–68；消费边界：A初始对象；未证编译或实机执行。

### BD106-E027

[VERIFIED mykernel/include/linux/kernel/delay.h::loops_per_jiffy]
```c
	#include <linux/kernel/math.h>

	extern unsigned long loops_per_jiffy;

	#include <asm/delay.h>
```

固定辅助行 22–26；消费边界：A extern；未证编译或实机执行。

### BD106-E028

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::KHZ]
```c
#ifndef DUMMY_TSC_KHZ
#  define DUMMY_TSC_KHZ (3LL * 1000LL * 1000LL)	/* 3 GHz */
#endif // !DUMMY_TSC_KHZ
#define KHZ	1000
```

固定辅助行 40–43；消费边界：A KHZ；未证编译或实机执行。

### BD106-E029

[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#undef HZ
	// #define HZ		CONFIG_HZ	/* Internal kernel timer frequency */
	#define HZ				1000		/* Internal kernel timer frequency */
```

固定辅助行 7–9；消费边界：A HZ；未证编译或实机执行。

### BD106-E030

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::get_loops_per_jiffy]
```c
get_loops_per_jiffy(void) {
	u64 lpj = (u64)tsc_khz * KHZ;

	do_div(lpj, HZ);
	return lpj;
```

固定辅助行 416–420；消费边界：A除法；未证编译或实机执行。

### BD106-E031

[VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]
```c
				uint32_t __base = (base);			\
				uint32_t __rem;						\
				__rem = ((uint64_t)(n)) % __base;	\
				(n) = ((uint64_t)(n)) / __base;		\
				__rem;								\
```

固定辅助行 45–49；消费边界：A do_div宏；未证编译或实机执行。

### BD106-E032

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_enable_sched_clock]
```c
static void __init
tsc_enable_sched_clock(void) {
	loops_per_jiffy = get_loops_per_jiffy();
	// use_tsc_delay();

```

固定辅助行 423–427；消费边界：A全局写者；未证编译或实机执行。

### BD106-E033

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_enable_sched_clock]
```c
	// /* Sanitize TSC ADJUST before cyc2ns gets initialized */
	// tsc_store_and_check_tsc_adjust(true);
	cyc2ns_init_boot_cpu();
	// static_branch_enable(&__use_tsc);
	__use_tsc = true;
```

固定辅助行 428–432；消费边界：A计时门；未证编译或实机执行。

### BD106-E034

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_early_init]
```c
	// 	return;
	if (!determine_cpu_tsc_frequencies(true))
		return;
	tsc_enable_sched_clock();
}
```

固定辅助行 441–445；消费边界：A早期；未证编译或实机执行。

### BD106-E035

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
	if (!tsc_khz) {
		/* We failed to determine frequencies earlier, try again */
		if (!determine_cpu_tsc_frequencies(false)) {
			// mark_tsc_unstable("could not calculate TSC khz");
			// setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
```

固定辅助行 461–465；消费边界：A晚期重试；未证编译或实机执行。

### BD106-E036

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
			return;
		}
		tsc_enable_sched_clock();
	}
```

固定辅助行 466–469；消费边界：A晚期成功；未证编译或实机执行。

### BD106-E037

[VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
vmware_platform_setup(void) {
	u32 eax, ebx, ecx;
	u64 lpj, tsc_khz;

	eax = vmware_hypercall3(VMWARE_CMD_GETHZ, UINT_MAX, &ebx, &ecx);
```

固定辅助行 153–157；消费边界：A局部lpj；未证编译或实机执行。

### BD106-E038

[VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
	if (ebx != UINT_MAX) {
		lpj = tsc_khz = eax | (((u64)ebx) << 32);
		do_div(tsc_khz, 1000);
```

固定辅助行 159–161；消费边界：A取频；未证编译或实机执行。

### BD106-E039

[VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
		// if (!preset_lpj) {
		// 	do_div(lpj, HZ);
		// 	preset_lpj = lpj;
		// }
```

固定辅助行 167–170；消费边界：A注释；未证编译或实机执行。

### BD106-E040

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_fn]
```c
// static void (*delay_fn)(u64) __ro_after_init = delay_loop;
static void (*delay_fn)(u64) = delay_loop;
```

固定辅助行 32–33；消费边界：A分发初始；未证编译或实机执行。

### BD106-E041

[VERIFIED mykernel/arch/x86_64/lib/delay.c::use_tsc_delay]
```c
// void __init use_tsc_delay(void) {
// 	if (delay_fn == delay_loop)
// 		delay_fn = delay_tsc;
// }
```

固定辅助行 162–165；消费边界：A候选注释；未证编译或实机执行。

### BD106-E042

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::__bad_*]
```c
	extern void __bad_sdelay(void);
	extern void __bad_mdelay(void);
	extern void __bad_udelay(void);
	extern void __bad_ndelay(void);
```

固定辅助行 6–9；消费边界：B bad声明；未证编译或实机执行。

### BD106-E043

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::sdelay]
```c
	#define sdelay(n) ({										\
				if (__builtin_constant_p(n)) {					\
					if ((n) / 20000 >= 1)						\
						__bad_sdelay();							\
					else										\
```

固定辅助行 24–28；消费边界：B秒常量；未证编译或实机执行。

### BD106-E044

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::sdelay]
```c
						__const_udelay((n) * 0x100000000ul);	\
				} else {										\
					__sdelay(n);								\
				}												\
			})
```

固定辅助行 29–33；消费边界：B秒变量；未证编译或实机执行。

### BD106-E045

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::mdelay]
```c
	#define mdelay(n) ({									\
				if (__builtin_constant_p(n)) {				\
					if ((n) / 20000 >= 1)					\
						__bad_mdelay();						\
					else									\
```

固定辅助行 36–40；消费边界：B毫秒常量；未证编译或实机执行。

### BD106-E046

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::mdelay]
```c
						__const_udelay((n) * 0x418938ul);	\
				} else {									\
					__mdelay(n);							\
				}											\
			})
```

固定辅助行 41–45；消费边界：B毫秒变量；未证编译或实机执行。

### BD106-E047

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::udelay]
```c
	#define udelay(n) ({								\
				if (__builtin_constant_p(n)) {			\
					if ((n) / 20000 >= 1)				\
						__bad_udelay();					\
					else								\
```

固定辅助行 48–52；消费边界：B微秒常量；未证编译或实机执行。

### BD106-E048

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::udelay]
```c
						__const_udelay((n) * 0x10c7ul);	\
				} else {								\
					__udelay(n);						\
				}										\
			})
```

固定辅助行 53–57；消费边界：B微秒变量；未证编译或实机执行。

### BD106-E049

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::ndelay]
```c
	#define ndelay(n) ({							\
				if (__builtin_constant_p(n)) {		\
					if ((n) / 20000 >= 1)			\
						__bad_ndelay();				\
					else							\
```

固定辅助行 60–64；消费边界：B纳秒常量；未证编译或实机执行。

### BD106-E050

[VERIFIED mykernel/include/linux/kernel/asm-generic/delay.h::ndelay]
```c
						__const_udelay((n) * 5ul);	\
				} else {							\
					__ndelay(n);					\
				}									\
			})
```

固定辅助行 65–69；消费边界：B纳秒变量；未证编译或实机执行。

### BD106-E051

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__sdelay]
```c
void __sdelay(unsigned long secs) {
	__const_udelay(secs * 0x100000000);	/* 2**32 (rounded up) */
}
```

固定辅助行 202–204；消费边界：B秒包装；未证编译或实机执行。

### BD106-E052

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__mdelay]
```c
void __mdelay(unsigned long msecs) {
	__const_udelay(msecs * 0x00418938);	/* 2**32 / 1000 (rounded up) */
}
```

固定辅助行 206–208；消费边界：B毫秒包装；未证编译或实机执行。

### BD106-E053

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__udelay]
```c
void __udelay(unsigned long usecs) {
	__const_udelay(usecs * 0x000010c7);	/* 2**32 / 1000000 (rounded up) */
}
```

固定辅助行 210–212；消费边界：B微秒包装；未证编译或实机执行。

### BD106-E054

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__ndelay]
```c
void __ndelay(unsigned long nsecs) {
	__const_udelay(nsecs * 0x00005);	/* 2**32 / 1000000000 (rounded up) */
}
```

固定辅助行 214–216；消费边界：B纳秒包装；未证编译或实机执行。

### BD106-E055

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
noinline void __const_udelay(unsigned long xloops) {
	// unsigned long lpj = this_cpu_read(cpu_info.loops_per_jiffy) ? : loops_per_jiffy;
	unsigned long lpj = loops_per_jiffy;
	int d0;

```

固定辅助行 189–193；消费边界：B全局读取；未证编译或实机执行。

### BD106-E056

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	xloops *= 4;
	asm(	"mull	%%edx	\n\t"
		:	"=d"(xloops), "=&a"(d0)
		:	"1"(xloops), "0"(lpj * (HZ / 4)));

```

固定辅助行 194–198；消费边界：B汇编操作数；未证编译或实机执行。

### BD106-E057

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__const_udelay]
```c
	__delay(++xloops);
}
```

固定辅助行 199–200；消费边界：B++及调用；未证编译或实机执行。

### BD106-E058

[VERIFIED mykernel/arch/x86_64/lib/delay.c::__delay]
```c
void __delay(unsigned long loops) {
	delay_fn(loops);
}
```

固定辅助行 185–187；消费边界：B间接调用；未证编译或实机执行。

### BD106-E059

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
static void delay_loop(u64 __loops)
{
	unsigned long loops = (unsigned long)__loops;

	asm volatile(	"	test	%0,	%0	\n"
```

固定辅助行 37–41；消费边界：B零值门；未证编译或实机执行。

### BD106-E060

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
	asm volatile(	"	test	%0,	%0	\n"
					"	jz		3f		\n"
					"	jmp		1f		\n"
					".align	16			\n"
					"1:	jmp		2f		\n"
```

固定辅助行 41–45；消费边界：B循环入；未证编译或实机执行。

### BD106-E061

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
					"1:	jmp		2f		\n"
					".align	16			\n"
					"2:	dec		%0		\n"
					"	jnz		2b		\n"
					"3:	dec		%0		\n"
```

固定辅助行 45–49；消费边界：B循环出；未证编译或实机执行。

### BD106-E062

[VERIFIED mykernel/arch/x86_64/lib/delay.c::delay_loop]
```c
					"3:	dec		%0		\n"
				: /* we don't need output */
				:	"a"(loops));
```

固定辅助行 49–51；消费边界：B约束；未证编译或实机执行。

### BD106-E063

[VERIFIED mykernel/scripts/options_flags.cmake::set(INCLUDE_DIRS)]
```c
set(INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
```

固定辅助行 78–82；消费边界：B include路径；未证编译或实机执行。

### BD106-E064

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
static int myos_hpet_clkevt_set_state_periodic(clocksrc_s *clksrc)
{
	uint channel = 0;
	// u32 mult = 429496730;
	// u32 shift = 32;
```

固定辅助行 203–207；消费边界：C函数入口；未证编译或实机执行。

### BD106-E065

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_stop_counter();
	delta = ((uint64_t)(NSEC_PER_SEC / HZ)) * mult;
	delta >>= shift + 7;
	now = hpet_readl(HPET_COUNTER);
	cmp = now + (uint)delta;
```

固定辅助行 214–218；消费边界：C delta；未证编译或实机执行。

### BD106-E066

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	cfg = hpet_readl(HPET_Tn_CFG(channel));
	// cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL | HPET_TN_32BIT;
	cfg |= HPET_TN_ENABLE | HPET_TN_PERIODIC | HPET_TN_SETVAL;
	hpet_writel(cfg, HPET_Tn_CFG(channel));
	hpet_writel(cmp, HPET_Tn_CMP(channel));
```

固定辅助行 219–223；消费边界：C首写；未证编译或实机执行。

### BD106-E067

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel(cmp, HPET_Tn_CMP(channel));
	udelay(1);
	/*
```

固定辅助行 223–225；消费边界：C udelay；未证编译或实机执行。

### BD106-E068

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	 * bit is automatically cleared after the first write.
	 * (See AMD-8111 HyperTransport I/O Hub Data Sheet,
	 * Publication # 24674)
	 */
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
```

固定辅助行 228–232；消费边界：C二写；未证编译或实机执行。

### BD106-E069

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_hpet_clkevt_set_state_periodic]
```c
	hpet_writel((uint)delta, HPET_Tn_CMP(channel));
	hpet_start_counter();

	return 0;
```

固定辅助行 232–235；消费边界：C计数器重启；未证编译或实机执行。

### BD106-E070

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	clocksource_register_hz(&clocksource_hpet, (u32)hpet_freq);

	if (id & HPET_ID_LEGSUP) {
		// hpet_legacy_clockevent_register(&hpet_base.channels[0]);
		myos_hpet_clkevt_set_state_periodic(&clocksource_hpet);
```

固定辅助行 479–483；消费边界：C LEGACY判断；未证编译或实机执行。

### BD106-E071

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_enable]
```c
	if (id & HPET_ID_LEGSUP) {
		// hpet_legacy_clockevent_register(&hpet_base.channels[0]);
		myos_hpet_clkevt_set_state_periodic(&clocksource_hpet);
		hpet_base.channels[0].mode = HPET_MODE_LEGACY;
		// if (IS_ENABLED(CONFIG_HPET_EMULATE_RTC))
```

固定辅助行 481–485；消费边界：C调用；未证编译或实机执行。

### BD106-E072

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::hpet_start_counter]
```c
{
	uint cfg = hpet_readl(HPET_CFG);

	cfg |= HPET_CFG_ENABLE;
	hpet_writel(cfg, HPET_CFG);
```

固定辅助行 149–153；消费边界：C enable；未证编译或实机执行。

### BD106-E073

[VERIFIED mykernel/arch/x86_64/kernel/time.c::hpet_time_init]
```c
	hpet_enable(NULL);

	// setup_default_timer_irq();
extern void myos_HPET_init(void);
		myos_HPET_init();
```

固定辅助行 34–38；消费边界：C晚期入口；未证编译或实机执行。

### BD106-E074

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
void myos_HPET_init()
{
	ioapic_retentry_T entry;
	//init I/O APIC IRQ2 => HPET Timer 0
	entry.vector = HPET_TIMER0_IRQ;
```

固定辅助行 539–543；消费边界：C IRQ另一工作；未证编译或实机执行。

### BD106-E075

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c::myos_HPET_init]
```c
	register_irq(HPET_TIMER0_IRQ, &entry , "HPET0",
			0, &HPET_int_controller, &HPET_handler);
}
```

固定辅助行 552–554；消费边界：C IRQ注册；未证编译或实机执行。

### BD106-E076

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	if (early) {
		cpu_khz = x86_platform.calibrate_cpu();
		if (tsc_early_khz) {
			tsc_khz = tsc_early_khz;
		} else {
```

固定辅助行 376–380；消费边界：A早期来源；未证编译或实机执行。

### BD106-E077

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
			tsc_khz = x86_platform.calibrate_tsc();
			clocksource_tsc.freq_khz = tsc_khz;
		}
	} else {
		/* We should not be here with non-native cpu calibration */
```

固定辅助行 381–385；消费边界：A两支；未证编译或实机执行。

### BD106-E078

[VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
		vmware_tsc_khz = tsc_khz;
		x86_platform.calibrate_tsc = vmware_get_tsc_khz;
		x86_platform.calibrate_cpu = vmware_get_tsc_khz;
```

固定辅助行 172–174；消费边界：A替代回调；未证编译或实机执行。

### BD106-E079

[VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	copy_array(&h->init, &x86_init.hyper, sizeof(h->init));
	copy_array(&h->runtime, &x86_platform.hyper, sizeof(h->runtime));
```

固定辅助行 103–104；消费边界：D00两个回调；未证编译或实机执行。
