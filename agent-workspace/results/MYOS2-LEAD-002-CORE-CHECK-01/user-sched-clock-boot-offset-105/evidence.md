# 第105批固定time源码短证据

每一引文含真实定义标识、1–5行连续固定time原文及辅助行；一条定点读取、回调字段、宏展开和实际CPU/设备执行分别判断。完整源码快照固定time@a039d9803ade，非实际编译或CPU读数。旧102/103/104证据用固定批号及原局部ID关联，不能把旧引文数加进本包。

### SC105-E001 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```
用途：D00 exact config command。固定time辅助行58–61；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E002 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::lock_cmos_prefix]
```c
	#define lock_cmos_prefix(reg)	do {} while (0)
```
用途：D00 independent prefix macro。固定time辅助行16–16；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E003 [VERIFIED mykernel/arch/x86_64/include/asm/mc146818rtc.h::lock_cmos_suffix]
```c
	#define lock_cmos_suffix(reg)	do {} while (0)
```
用途：D00 independent suffix macro。固定time辅助行17–17；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E004 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::read_persistent_clock64]
```c
		mc146818_get_time(&tm);

		ts->tv_sec = rtc_tm_to_time64(&tm);
		ts->tv_nsec = 0;
```
用途：D00 real getter call with ignored int return。固定time辅助行45–48；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E005 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
	#define BUILDIO(bwl, bw, type)									\
				static inline void									\
				out##bwl(unsigned type value, int port) {			\
```
用途：D00 definition body not instantiation。固定time辅助行188–190；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E006 [VERIFIED mykernel/arch/x86_64/insns/io_arch.h::BUILDIO]
```c
				static inline unsigned type							\
				in##bwl(int port) {									\
					unsigned type value;							\
					asm volatile(	"in" #bwl " %w1, %" #bw "0"		\
```
用途：D00 inb generated body。固定time辅助行197–200；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E007 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	wall_to_mono = timespec64_sub(boot_offset, wall_time);
```
用途：D00 boot_offset minus wall_time。固定time辅助行251–251；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E008 [VERIFIED mykernel/time/misc/time_misc.h::timespec64_sub]
```c
			timespec64_s ts_delta;
			set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
					lhs.tv_nsec - rhs.tv_nsec);
			return ts_delta;
```
用途：D00 lhs rhs difference。固定time辅助行80–83；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E009 [VERIFIED mykernel/time/misc/time_misc.c::mktime64]
```c
			)*24 + hour /* now have hours - midnight tomorrow handled here */
		)*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
```
用途：D00 hour min sec operands。固定time辅助行256–258；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E010 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
// 	p->ctrl = CMOS_READ(RTC_CONTROL);
```
用途：D00 commented RTC_CONTROL read。固定time辅助行36–36；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E011 [VERIFIED mykernel/drivers/rtc/rtc-mc146818.c::simple_mc146818_gettime]
```c
	time->tm_sec = CMOS_READ(RTC_SECONDS);
	time->tm_min = CMOS_READ(RTC_MINUTES);
	time->tm_hour = CMOS_READ(RTC_HOURS);
```
用途：D00 three index read expressions。固定time辅助行18–20；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E012 [VERIFIED mykernel/arch/x86_64/kernel/rtc.c::rtc_cmos_read]
```c
	// lock_cmos_prefix(addr);
	outb(addr, RTC_PORT(0));
	val = inb(RTC_PORT(1));
	// lock_cmos_suffix(addr);
```
用途：D00 outb then inb with commented lock。固定time辅助行22–25；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E013 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	setup_arch(NULL);

	setup_per_cpu_areas();
```
用途：A setup_arch then percpu areas。固定time辅助行138–140；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E014 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	init_IRQ();
	softirq_init();
	timekeeping_init();
	// time_init(); /* cancel this meaningless function call, directly call late_time_init */
```
用途：A timekeeping after perCPU setup。固定time辅助行163–166；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E015 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	// if (late_time_init)
		late_time_init();
```
用途：A late time initialization。固定time辅助行176–177；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E016 [VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	init_hypervisor_platform();

	tsc_early_init();
```
用途：A hypervisor before TSC early。固定time辅助行211–213；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E017 [VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	/* Setup the legacy timers */
	hpet_time_init(); /* == x86_init.timers.timer_init() */
```
用途：A late HPET caller。固定time辅助行50–51；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E018 [VERIFIED mykernel/arch/x86_64/kernel/time.c::late_time_init]
```c
	tsc_init();
```
用途：A late tsc caller。固定time辅助行58–58；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E019 [VERIFIED mykernel/arch/x86_64/kernel/x86_init.c::x86_init]
```c
	.hyper = {
		.init_platform			= x86_init_noop,
		.guest_late_init		= x86_init_noop,
		.x2apic_available		= bool_x86_init_noop,
```
用途：A hyper callback default noop。固定time辅助行115–118；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E020 [VERIFIED mykernel/arch/x86_64/kernel/x86_init.c::x86_platform]
```c
struct x86_platform_ops x86_platform __ro_after_init = {
	.calibrate_cpu			= native_calibrate_cpu_early,
	.calibrate_tsc			= native_calibrate_tsc,
```
用途：A native calibration callbacks initial。固定time辅助行147–149；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E021 [VERIFIED mykernel/arch/x86_64/include/asm/x86_init.h::x86_hyper_init]
```c
	void (*init_platform)(void);
	void (*guest_late_init)(void);
	bool (*x2apic_available)(void);
```
用途：A hyper init callback type。固定time辅助行127–129；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E022 [VERIFIED mykernel/arch/x86_64/include/asm/x86_init.h::x86_platform_ops]
```c
	ulong (*calibrate_cpu)(void);
	ulong (*calibrate_tsc)(void);
```
用途：A calibration callback types。固定time辅助行313–314；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E023 [VERIFIED mykernel/arch/x86_64/include/asm/hypervisor.h::init_hypervisor_platform]
```c
#define init_hypervisor_platform	simple_init_hypervisor_platform
```
用途：A configured macro alias。固定time辅助行73–73；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E024 [VERIFIED mykernel/arch/x86_64/include/asm/hypervisor.h::init_hypervisor_platform]
```c
static inline void init_hypervisor_platform(void) { }
```
用途：A no-hypervisor stub。固定time辅助行79–79；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E025 [VERIFIED mykernel/cpu/hypervisor.c::detect_hypervisor_vendor]
```c
	for (p = hypervisors; p < hypervisors + ARRAY_SIZE(hypervisors); p++) {
		if (unlikely(nopv) && !(*p)->ignore_nopv)
			continue;

		pri = (*p)->detect();
```
用途：A vendor table scan。固定time辅助行64–68；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E026 [VERIFIED mykernel/cpu/hypervisor.c::detect_hypervisor_vendor]
```c
		if (pri > max_pri) {
			max_pri = pri;
			h = *p;
		}
```
用途：A choose highest positive priority。固定time辅助行69–72；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E027 [VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	h = detect_hypervisor_vendor();

	if (!h)
		return;
```
用途：A no vendor early return。固定time辅助行98–101；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E028 [VERIFIED mykernel/cpu/hypervisor.c::simple_init_hypervisor_platform]
```c
	copy_array(&h->init, &x86_init.hyper, sizeof(h->init));
	copy_array(&h->runtime, &x86_platform.hyper, sizeof(h->runtime));

	x86_hyper_type = h->type;
	x86_init.hyper.init_platform();
```
用途：A copy init and call platform。固定time辅助行103–107；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E029 [VERIFIED mykernel/cpu/hypervisor.c::copy_array]
```c
	for (i = 0; i < n; i++)
		if (from[i])
			to[i] = from[i];
```
用途：A skip null callback copy。固定time辅助行89–91；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E030 [VERIFIED mykernel/cpu/vmware.c::x86_hyper_vmware]
```c
const __initconst hypervisor_x86_s x86_hyper_vmware = {
	.name						= "VMware",
	.detect						= vmware_platform,
	.type						= X86_HYPER_VMWARE,
	.init.init_platform			= vmware_platform_setup,
```
用途：A VMware detection object setup。固定time辅助行254–258；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E031 [VERIFIED mykernel/cpu/vmware.c::vmware_platform]
```c
	if (boot_cpu_data.x86_capability[CPUID_1_ECX] & (1<<31)) {
		uint eax;
		uint hyper_vendor_id[3];

```
用途：A VMware gating capability。固定time辅助行217–220；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E032 [VERIFIED mykernel/cpu/vmware.c::vmware_platform]
```c
		cpuid(CPUID_VMWARE_INFO_LEAF, &eax, (uint *)&hyper_vendor_id[0],
		      (uint *)&hyper_vendor_id[1], (uint *)&hyper_vendor_id[2]);
		if (!memcmp(hyper_vendor_id, "VMwareVMware", 12)) {
```
用途：A VMware CPUID vendor query。固定time辅助行221–223；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E033 [VERIFIED mykernel/cpu/vmware.c::vmware_platform]
```c
			if (eax >= CPUID_VMWARE_FEATURES_LEAF)
				vmware_hypercall_mode =
					vmware_select_hypercall();
```
用途：A VMware vendor conditional。固定time辅助行224–226；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E034 [VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
	eax = vmware_hypercall3(VMWARE_CMD_GETHZ, UINT_MAX, &ebx, &ecx);

	if (ebx != UINT_MAX) {
		lpj = tsc_khz = eax | (((u64)ebx) << 32);
		do_div(tsc_khz, 1000);
```
用途：A conditional hypervisor frequency。固定time辅助行157–161；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E035 [VERIFIED mykernel/cpu/vmware.c::vmware_platform_setup]
```c
		vmware_tsc_khz = tsc_khz;
		x86_platform.calibrate_tsc = vmware_get_tsc_khz;
		x86_platform.calibrate_cpu = vmware_get_tsc_khz;
```
用途：A both callbacks replaced。固定time辅助行172–174；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E036 [VERIFIED mykernel/cpu/vmware.c::vmware_get_tsc_khz]
```c
	return vmware_tsc_khz;
```
用途：A VMware stored kHz getter。固定time辅助行68–68；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E037 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_cpu_early]
```c
	ulong flags, fast_calibrate = cpu_khz_from_cpuid();
```
用途：A CPU early from CPUID。固定time辅助行341–341；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E038 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_cpu_early]
```c
	if (!fast_calibrate)
		fast_calibrate = DUMMY_TSC_KHZ;

	return fast_calibrate;
```
用途：A CPU fallback DUMMY。固定time辅助行350–353；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E039 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::DUMMY_TSC_KHZ]
```c
#  define DUMMY_TSC_KHZ (3LL * 1000LL * 1000LL)	/* 3 GHz */
```
用途：A 3GHz dummy macro。固定time辅助行41–41；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E040 [VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::CPUID_LEAF_TSC]
```c
	#define CPUID_LEAF_TSC		0x15
```
用途：A CPUID leaf 0x15。固定time辅助行229–229；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E041 [VERIFIED mykernel/arch/x86_64/processor/processor_const_arch.h::CPUID_LEAF_FREQ]
```c
	#define CPUID_LEAF_FREQ		0x16
```
用途：A CPUID leaf 0x16。固定time辅助行230–230；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E042 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cpu_khz_from_cpuid]
```c
	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		return 0;

	if (boot_cpu_data.cpuid_level < CPUID_LEAF_FREQ)
		return 0;
```
用途：A CPU vendor and leaf guard。固定time辅助行323–327；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E043 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cpu_khz_from_cpuid]
```c
	// cpuid(CPUID_LEAF_FREQ, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);
	cpuid(CPUID_LEAF_TSC, &eax_base_mhz, &ebx_max_mhz, &ecx_bus_mhz, &edx);

	return eax_base_mhz * 1000;
```
用途：A active other leaf and eax times1000。固定time辅助行331–334；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E044 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		return 0;

	if (boot_cpu_data.cpuid_level < CPUID_LEAF_TSC)
		return 0;
```
用途：A TSC vendor and level guard。固定time辅助行250–254；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E045 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	/* CPUID 15H TSC/Crystal ratio, plus optionally Crystal Hz */
	cpuid(CPUID_LEAF_TSC, &eax_denominator, &ebx_numerator, &ecx_hz, &edx);

	if (ebx_numerator == 0 || eax_denominator == 0)
```
用途：A numerator denominator CPUID。固定time辅助行258–261；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E046 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	crystal_khz = ecx_hz / 1000;
```
用途：A crystal Hz truncated kHz。固定time辅助行264–264；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E047 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	if (crystal_khz == 0 && boot_cpu_data.cpuid_level >= CPUID_LEAF_FREQ) {
		unsigned int eax_base_mhz, ebx, ecx, edx;

		cpuid(CPUID_LEAF_FREQ, &eax_base_mhz, &ebx, &ecx, &edx);
		crystal_khz = eax_base_mhz * 1000 *
```
用途：A secondary leaf and crystal inputs。固定time辅助行288–292；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E048 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
		crystal_khz = eax_base_mhz * 1000 *
			eax_denominator / ebx_numerator;
	}
```
用途：A secondary multi-stage uint computation。固定time辅助行292–294；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E049 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	if (crystal_khz == 0)
		return 0;
```
用途：A zero crystal return。固定time辅助行296–297；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E050 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_calibrate_tsc]
```c
	return crystal_khz * ebx_numerator / eax_denominator;
```
用途：A final crystal ratio。固定time辅助行316–316；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E051 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	if (early) {
		cpu_khz = x86_platform.calibrate_cpu();
		if (tsc_early_khz) {
			tsc_khz = tsc_early_khz;
		} else {
```
用途：A early CPU calibration and nonzero early TSC。固定time辅助行376–380；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E052 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
			tsc_khz = x86_platform.calibrate_tsc();
			clocksource_tsc.freq_khz = tsc_khz;
		}
```
用途：A regular TSC callback and freq_khz field write。固定time辅助行381–383；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E053 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	if (tsc_khz == 0)
		tsc_khz = cpu_khz;
	else if (abs(cpu_khz - tsc_khz) * 10 > tsc_khz)
		cpu_khz = tsc_khz;
```
用途：A zero fallback and abs sanity。固定time辅助行395–398；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E054 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::determine_cpu_tsc_frequencies]
```c
	if (tsc_khz == 0)
		return false;
```
用途：A zero TSC failure branch。固定time辅助行400–401；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E055 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_early_init]
```c
	if (!determine_cpu_tsc_frequencies(true))
		return;
	tsc_enable_sched_clock();
```
用途：A early freq success then enable。固定time辅助行442–444；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E056 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_enable_sched_clock]
```c
	cyc2ns_init_boot_cpu();
	// static_branch_enable(&__use_tsc);
	__use_tsc = true;
```
用途：A boot CPU conversion then use_tsc true。固定time辅助行430–432；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E057 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
	if (!tsc_khz) {
		/* We failed to determine frequencies earlier, try again */
		if (!determine_cpu_tsc_frequencies(false)) {
			// mark_tsc_unstable("could not calculate TSC khz");
```
用途：A late zero frequency retry。固定time辅助行461–464；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E058 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
			// setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
			return;
		}
		tsc_enable_sched_clock();
	}
```
用途：A late failure return or early enable。固定time辅助行465–469；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E059 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
	// if (boot_cpu_data.x86_capa_bits.Invariant_TSC)
	// 	clocksource_register_khz(&clocksource_tsc, tsc_khz);
```
用途：A TSC clocksource register commented。固定time辅助行491–492；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E060 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_init]
```c
	// Now MyOS2 running on vmware can't init TSC
	clocksource_mark_unstable(&clocksource_tsc);
```
用途：A clocksource marked unstable。固定time辅助行495–496；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E061 [VERIFIED mykernel/time/clocksource/clocksource.c::clocksource_mark_unstable]
```c
	if (!(cs->flags & CLOCK_SOURCE_UNSTABLE)) {
		// if (!list_empty(&cs->list) && list_empty(&cs->wd_list))
		// 	list_add(&cs->wd_list, &watchdog_list);
		__clocksource_unstable(cs);
	}
```
用途：A mark unstable when flags not already unstable。固定time辅助行113–117；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。

### SC105-E062 [VERIFIED mykernel/time/clocksource/clocksource.c::__clocksource_unstable]
```c
__clocksource_unstable(clocksrc_s *cs) {
	cs->flags &= ~(CLOCK_SOURCE_VALID_FOR_HRES | CLOCK_SOURCE_WATCHDOG);
	cs->flags |= CLOCK_SOURCE_UNSTABLE;
```
用途：A flag mask and unstable bit。固定time辅助行80–82；符号真实定义体内/独立宏/静态对象。仅源级关系，硬件与运行未证。


### SC105-E063 [VERIFIED mykernel/arch/x86_64/include/asm/timer.h::cyc2ns_data]
```c
	struct cyc2ns_data {
		u32 cyc2ns_mul;
		u32 cyc2ns_shift;
		u64 cyc2ns_offset;
	}; /* 16 bytes */
```
用途：B three field types and structure width。固定time辅助行30–34；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E064 [VERIFIED mykernel/arch/x86_64/include/asm/timer.h::cyc2ns]
```c
	struct cyc2ns {
		struct cyc2ns_data data;
		// seqcount_latch_t   seq;		/* 32 + 4    = 36 */
	}; /* fits one cacheline */
```
用途：B wrapper data object。固定time辅助行35–38；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E065 [VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::DECLARE_PER_CPU]
```c
	#define DECLARE_PER_CPU(type, name)					\
				DECLARE_PER_CPU_SECTION(type, name, "")
```
用途：B actual DECLARE_PER_CPU macro definition; its use at asm/percpu_area.h lines23–25 is only a declaration, actual initialized template object is SC105-E066。固定time辅助行87–88；macro definition not an instance or allocated CPU storage。


### SC105-E066 [VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::cyc2ns]
```c
DEFINE_PER_CPU(cyc2ns_s, cyc2ns) = {
	.data = {
		.cyc2ns_mul		= 1431655765,
		.cyc2ns_shift	= 32,
		.cyc2ns_offset	= 0,
```
用途：B perCPU template mul shift offset。固定time辅助行89–93；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E067 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__simple_cyc2ns_read]
```c
	cyc2ns_s *c = this_cpu_ptr(&cyc2ns);
	data->cyc2ns_offset = READ_ONCE(c->data.cyc2ns_offset);
	data->cyc2ns_mul    = READ_ONCE(c->data.cyc2ns_mul);
	data->cyc2ns_shift  = READ_ONCE(c->data.cyc2ns_shift);
```
用途：B three separately READ_ONCE reads of offset/mul/shift from this_cpu_ptr result; not a seqlock snapshot。固定time辅助行82–85；no runtime read performed。


### SC105-E068 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cycles_2_ns]
```c
	__cyc2ns_read(&data);

	ns = data.cyc2ns_offset;
	ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);
```
用途：B current CPU read and mul/shr。固定time辅助行127–130；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E069 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__cycles_2_ns]
```c
	ns = data.cyc2ns_offset;
	ns += mul_u64_u32_shr(cyc, data.cyc2ns_mul, data.cyc2ns_shift);

	return ns;
```
用途：B offset plus conversion result。固定time辅助行129–132；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E070 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cycles_2_ns]
```c
	// preempt_disable_notrace();
	ns = __cycles_2_ns(cyc);
	// preempt_enable_notrace();
	return ns;
```
用途：B wrapper invoke and commented preempt guard。固定time辅助行138–141；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E071 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	ns_now = cycles_2_ns(tsc_now);
```
用途：B read ns_now from existing representation。固定time辅助行150–150；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E072 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	clocks_calc_mult_shift(&data.cyc2ns_mul, &data.cyc2ns_shift, khz,
			       NSEC_PER_MSEC, 0);
```
用途：B frequency to numerator and shift。固定time辅助行157–158；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E073 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	if (data.cyc2ns_shift == 32) {
		data.cyc2ns_shift = 31;
		data.cyc2ns_mul >>= 1;
	}
```
用途：B shift32 to31 and halve mult。固定time辅助行166–169；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E074 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	data.cyc2ns_offset = ns_now -
		mul_u64_u32_shr(tsc_now, data.cyc2ns_mul, data.cyc2ns_shift);
```
用途：B offset new = ns_now minus new scale。固定time辅助行171–172；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E075 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	c2n = per_cpu_ptr(&cyc2ns, cpu);
```
用途：B target CPU resolved by per_cpu_ptr。固定time辅助行174–174；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E076 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__set_cyc2ns_scale]
```c
	// raw_write_seqcount_latch(&c2n->seq);
	// c2n->data[0] = data;
	// raw_write_seqcount_latch(&c2n->seq);
	// c2n->data[1] = data;
	c2n->data = data;
```
用途：B actual data struct publication and commented seq。固定time辅助行176–180；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E077 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::cyc2ns_init_boot_cpu]
```c
	cyc2ns_s *c2n = this_cpu_ptr(&cyc2ns);

	// seqcount_latch_init(&c2n->seq);
	__set_cyc2ns_scale(tsc_khz, smp_processor_id(), rdtsc());
```
用途：B boot CPU scale caller and sampled tsc。固定time辅助行202–205；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E078 [VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::this_cpu_ptr]
```c
	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
```
用途：B macro current CPU ptr。固定time辅助行250–250；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E079 [VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::arch_raw_cpu_ptr]
```c
	#define arch_raw_cpu_ptr(_ptr)	({							\
				ulong tcp_ptr__ =								\
						raw_cpu_read_long(this_cpu_off);		\
				tcp_ptr__ += (__force ulong)(_ptr);				\
				(typeof(*(_ptr)) __kernel __force *)tcp_ptr__;	\
```
用途：B current CPU GS based expression。固定time辅助行210–214；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E080 [VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::per_cpu_ptr]
```c
	#define per_cpu_ptr(ptr, cpu)	({		\
				__verify_pcpu_ptr(ptr);		\
				SHIFT_PERCPU_PTR((ptr),		\
					per_cpu_offset((cpu)));	\
			})
```
用途：B explicit CPU offset pointer macro。固定time辅助行244–248；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E081 [VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::raw_cpu_read_long]
```c
	#define raw_cpu_read_long(pcp)	((ulong)rdgsbase())
```
用途：B GS base input ignores macro argument。固定time辅助行208–208；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E082 [VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	// 复制“模板”percpu段到运行时cpu0的percpu段
	// 主要是将静态初始化的一些percpu变量的值复制过来
	memcpy(pcpu_first_chunk->data, &__per_cpu_load, static_size);
```
用途：B later template copied to BSP chunk。固定time辅助行354–356；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E083 [VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	pcpu_unit_offsets =
		myos_memblock_alloc_normal(nr_cpu_ids * sizeof(void *), 8);
	for (int i = 0; i < nr_cpu_ids; i++)
		((ulong *)pcpu_unit_offsets)[i] = i * static_size;
```
用途：B later unit offsets allocation and publish。固定time辅助行360–363；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E084 [VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	tmp = ((u64)maxsec * from) >> 32;
	while (tmp) {
		tmp >>=1;
		sftacc--;
	}
```
用途：B initial shift accuracy bounds。固定time辅助行57–61；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E085 [VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	for (sft = 32; sft > 0; sft--) {
		tmp = (u64) to << sft;
		tmp += from / 2;
		do_div(tmp, from);
		if ((tmp >> sftacc) == 0)
```
用途：B loop and do_div with rounded mult。固定time辅助行67–71；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E086 [VERIFIED mykernel/time/clocksource/clocksource.c::clocks_calc_mult_shift]
```c
	*mult = tmp;
	*shift = sft;
```
用途：B store u32 mult and shift。固定time辅助行74–75；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E087 [VERIFIED mykernel/include/linux/kernel/math64.h::mul_u64_u32_shr]
```c
				return (u64)(((unsigned __int128)a * mul) >> shift);
```
用途：B INT128 implementation branch return。固定time辅助行160–160；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E088 [VERIFIED mykernel/include/linux/kernel/math64.h::mul_u64_u32_shr]
```c
				al = a;
				ah = a >> 32;

				ret = mul_u32_u32(al, mul) >> shift;
```
用途：B fallback low32 high32 split。固定time辅助行177–180；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E089 [VERIFIED mykernel/include/linux/kernel/math64.h::mul_u64_u32_shr]
```c
				ret = mul_u32_u32(al, mul) >> shift;
				if (ah)
					ret += mul_u32_u32(ah, mul) << (32 - shift);

				return ret;
```
用途：B fallback highword conditional shift。固定time辅助行180–184；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E090 [VERIFIED mykernel/include/linux/kernel/asm-generic/div64.h::do_div]
```c
	#define do_div(n,base) ({						\
				uint32_t __base = (base);			\
				uint32_t __rem;						\
				__rem = ((uint64_t)(n)) % __base;	\
				(n) = ((uint64_t)(n)) / __base;		\
```
用途：B macro unsigned quotient and remainder。固定time辅助行44–48；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E091 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::__use_tsc]
```c
static bool __use_tsc = false;
```
用途：C scheduling TSC initial gate false。固定time辅助行51–51；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E092 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	if (__use_tsc) {
		u64 tsc_now = rdtsc();
		/* return the value in ns */
		return cycles_2_ns(tsc_now);
```
用途：C TSC path rdtsc then cycles2ns。固定time辅助行222–225；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E093 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::native_sched_clock]
```c
	return (jiffies_64 - INITIAL_JIFFIES) * (1000000000 / HZ);
```
用途：C fallback jiffies expression。固定time辅助行237–237；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E094 [VERIFIED mykernel/arch/x86_64/insns/x86msr.h::rdtsc]
```c
			asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

			return EAX_EDX_VAL(val, low, high);
```
用途：C rdtsc inline wrapper returns combined register value。固定time辅助行65–67；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E095 [VERIFIED mykernel/time/timer/timer.c::jiffies_64]
```c
__visible u64 jiffies_64 __cacheline_aligned_in_smp = INITIAL_JIFFIES;
```
用途：C actual jiffies_64 object initializer。固定time辅助行38–38；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E096 [VERIFIED mykernel/time/systick/systick_const.h::INITIAL_JIFFIES]
```c
	#define INITIAL_JIFFIES ((ulong)(uint) (-300*HZ))
```
用途：C INITIAL_JIFFIES macro in const header。固定time辅助行69–69；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E097 [VERIFIED mykernel/time/systick/systick_macro.h::INITIAL_JIFFIES]
```c
	#define INITIAL_JIFFIES		((ulong)(uint) (-300*HZ))
```
用途：C duplicate macro same expression in macro header。固定time辅助行227–227；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E098 [VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```
用途：C fixed HZ value in param。固定time辅助行9–9；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E099 [VERIFIED mykernel/arch/x86_64/kernel.lds::jiffies]
```c
jiffies = jiffies_64;
```
用途：C linker script jiffies alias declaration。固定time辅助行17–17；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E100 [VERIFIED mykernel/include/linux/sched/clock.h::local_clock]
```c
		return native_sched_clock();
```
用途：C public local_clock returns native path。固定time辅助行53–53；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E101 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	read_persistent_clock64(&wall_time);
	boot_offset = ns_to_timespec64(local_clock());
```
用途：C localclock consumed as ns conversion entry。固定time辅助行244–245；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E102 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	wall_to_mono = timespec64_sub(boot_offset, wall_time);
```
用途：C boot_offset minus wall_time local。固定time辅助行251–251；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E103 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	timespec64_s ts = { 0, 0 };
	s32 rem;

	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
```
用途：C signed nsec input and s32 rem mismatch。固定time辅助行52–56；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E104 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
```
用途：C positive branch conversion request。固定time辅助行55–57；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E105 [VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```
用途：C negative path old input semantics retained。固定time辅助行64–65；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E106 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_set_wall_to_mono]
```c
	// tk->wall_to_monotonic = wtm;
```
用途：C actual store commented。固定time辅助行59–59；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E107 [VERIFIED mykernel/time/timekeeping/timekeeping.c::tk_set_xtime]
```c
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec =
		(u64)ts->tv_nsec << tk->tkr_mono.shift;
```
用途：C wall_time xtime sec and fraction write。固定time辅助行35–37；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E108 [VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_init]
```c
	tk_set_xtime(tk, &wall_time);
	tk->raw_sec = 0;

	tk_set_wall_to_mono(tk, wall_to_mono);
```
用途：C actual wall_time consumer and noop wtm caller。固定time辅助行258–261；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E109 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::get_loops_per_jiffy]
```c
	u64 lpj = (u64)tsc_khz * KHZ;

	do_div(lpj, HZ);
	return lpj;
```
用途：C tsc khz times1000 over HZ。固定time辅助行417–420；源定义体/宏或对象单元，不证明实际选编和执行。

### SC105-E110 [VERIFIED mykernel/arch/x86_64/kernel/tsc.c::tsc_enable_sched_clock]
```c
	loops_per_jiffy = get_loops_per_jiffy();
```
用途：C lpj commit precedes scale enable。固定time辅助行425–425；源定义体/宏或对象单元，不证明实际选编和执行。


## 定位与运行边界

独立宏、对象初始化、函数体语句以真实源码中的定义单元为目标。文件中的宏实例化不冒充该宏的定义体：例如旧RTC104-E060在io_arch.h第239行只是BUILDIO(b,b,char)实例化，真正BUILDIO定义体对应本批SC105-E005/E006。配置宏和CPUID请求不提供实际编译/CPU/虚拟机频率的运行见证。完整本地P9、构建、指令和模型执行均NOT_RUN。
### SC105-E111 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	simple_pcpu_setup_first_chunk();
	/* alrighty, percpu areas up and running */
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
```
用途：B later direct call to simple_pcpu_setup_first_chunk and delta。固定time辅助行73–75；定义体内静态请求，GS/CPU实际状态未观察。

### SC105-E112 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	for_each_possible_cpu(cpu) {
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```
用途：B perCPU offset publishing only after first chunk。固定time辅助行76–79；定义体内静态请求，GS/CPU实际状态未观察。

### SC105-E113 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
		if (cpu == 0)
		{
```
用途：B CPU0-only GS base switch condition。固定time辅助行112–113；定义体内静态请求，GS/CPU实际状态未观察。

### SC105-E114 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
				// wrmsrl(MSR_GS_BASE, cpu_kernelmode_gs_base(cpu));
				wrmsrl(MSR_GS_BASE, per_cpu_offset(cpu));
```
用途：B active GS base update after template copy。固定time辅助行141–142；定义体内静态请求，GS/CPU实际状态未观察。

