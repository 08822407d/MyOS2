---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SMP-BOOT-PATH-100
record_type: fixed_time_single_definition_short_quotes
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-14
produced_by: GPT-6（当前可见模型家族；后台UNATTESTED）
model_per_owner: GPT-6＋Extra（Owner指定次一档）
date: 2026-11-18
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第100批：定义体、ASM标签与构建唯一语句的真实短锚

每条短锚严格对应固定time的一个真实C函数体/对象/结构/宏定义、ASM实际入口与所属局部基本块/数据标签、或一个CMake及链接器唯一语句块。原99的13条include和一条原型不作为本批定义验证前缀，D00另在facts追踪其真实原文与类别。行号只作为复查辅助，不能替代符号定义验证。不得从静态指令、函数名、注释或配置字符串推出AP硬件上线、映射已成功、真实链接或固件行为。

### SP100-E001 [VERIFIED myloader/uefi/bootloader.c::UefiMain]
```c
	mb2_infotag_ptr = fill_framebuffer_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_mmap_info(ImageHandle, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_acpi_RSDT_info(SystemTable, mb2_infotag_ptr);
	mb2_infotag_ptr = fill_cputopo_info(mb2_infotag_ptr);
```
类别：C_FUNCTION；专题A；固定time辅助行段51-54。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E002 [VERIFIED myloader/uefi/bootloader.c::fill_mmap_info]
```c
	gBS->FreePool(MemMap);
	gBS->ExitBootServices(ImageHandle, MapKey);
```
类别：C_FUNCTION；专题A；固定time辅助行段281-282。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E003 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	myos2_tag_smpinfo_s *smp_info = (myos2_tag_smpinfo_s *)mb2_infotag_ptr;

	UINTN i = 0;
	UINTN nCores = 0, nRunning = 0;
	EFI_MP_SERVICES_PROTOCOL *mpp = 0;
```
类别：C_FUNCTION；专题A；固定time辅助行段374-378。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E004 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mpp);
	mpp->GetNumberOfProcessors(mpp, &nCores, &nRunning);
	smp_info->core_num = nCores;
	smp_info->core_available = nRunning;
```
类别：C_FUNCTION；专题A；固定time辅助行段380-383。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E005 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	for(i =0; i< nCores; i++){
		EFI_PROCESSOR_INFORMATION mcpuInfo;
		mpp -> GetProcessorInfo( mpp, i, &mcpuInfo);
		smp_info->cpus[i].proccessor_id	= mcpuInfo.ProcessorId;
		smp_info->cpus[i].status		= mcpuInfo.StatusFlag;
```
类别：C_FUNCTION；专题A；固定time辅助行段385-389。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E006 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
		smp_info->cpus[i].status		= mcpuInfo.StatusFlag;
		smp_info->cpus[i].pack_id		= mcpuInfo.Location.Package;
		smp_info->cpus[i].core_id		= mcpuInfo.Location.Core;
		smp_info->cpus[i].thd_id		= mcpuInfo.Location.Thread;
	}
```
类别：C_FUNCTION；专题A；固定time辅助行段389-393。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E007 [VERIFIED myloader/uefi/bootloader.c::fill_cputopo_info]
```c
	smp_info->type = MYOS2_TAG_TYPE_CPU_TOPO;
	smp_info->size = sizeof(myos2_tag_smpinfo_s) + nCores * sizeof(myos2_cpudesc_s);
```
类别：C_FUNCTION；专题A；固定time辅助行段395-396。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E008 [VERIFIED myloader/uefi/multiboot2.h::MYOS2_TAG_TYPE_CPU_TOPO]
```c
	#define MYOS2_TAG_TYPE_CPU_TOPO					1000
```
类别：MACRO；专题A；固定time辅助行段67-67。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E009 [VERIFIED myloader/uefi/multiboot2.h::CPU_DESCRIPTOR]
```c
		struct CPU_DESCRIPTOR
		{
			multiboot_uint32_t	pack_id;
			multiboot_uint32_t	core_id;
			multiboot_uint32_t	thd_id;
```
类别：STRUCT；专题A；固定time辅助行段457-461。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E010 [VERIFIED myloader/uefi/multiboot2.h::CPU_DESCRIPTOR]
```c
			multiboot_uint32_t	status;
			multiboot_uint64_t	proccessor_id;
		};
```
类别：STRUCT；专题A；固定time辅助行段462-464。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E011 [VERIFIED myloader/uefi/multiboot2.h::myos2_tag_smpinfo]
```c
		struct myos2_tag_smpinfo
		{
			multiboot_uint32_t	type;
			multiboot_uint32_t	size;
			multiboot_uint32_t	core_num;
```
类别：STRUCT；专题A；固定time辅助行段466-470。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E012 [VERIFIED myloader/uefi/multiboot2.h::myos2_tag_smpinfo]
```c
			multiboot_uint32_t	core_num;
			multiboot_uint32_t	core_available;
			myos2_cpudesc_s		cpus[0];
		};
```
类别：STRUCT；专题A；固定time辅助行段470-473。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E013 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::nr_cpu_ids]
```c
unsigned			nr_cpu_ids;
```
类别：GLOBAL_OBJECT；专题A；固定time辅助行段16-16。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E014 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::get_smpinfo]
```c
static void get_smpinfo(myos2_tag_smpinfo_s *smp_info)
{
	nr_cpu_ids = smp_info->core_num;
}
```
类别：C_FUNCTION；专题A；固定time辅助行段24-27。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E015 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c

	/*  Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		return;
	}
```
类别：C_FUNCTION；专题A；固定time辅助行段57-61。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E016 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
	if (addr & 7) {
		return;
	}
```
类别：C_FUNCTION；专题A；固定time辅助行段63-65。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E017 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
	mbi_tag_header = (mbi_tags_header_s *) addr;
	size = mbi_tag_header->total_size;
	for (tag = (mbi_tag_s *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (mbi_tag_s *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
	{
```
类别：C_FUNCTION；专题A；固定time辅助行段67-71。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E018 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::parse_tag]
```c
		case MYOS2_TAG_TYPE_CPU_TOPO:
			myos2_tag_smpinfo_s *smpinfo = (myos2_tag_smpinfo_s *)tag;
			get_smpinfo(smpinfo);
			break;
```
类别：C_FUNCTION；专题A；固定time辅助行段112-115。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E019 [VERIFIED mykernel/arch/x86_64/myos/pre_init.c::myos_early_init_system]
```c
	parse_tag(mbi_magic, (ulong)&multiboot_MBI);
```
类别：C_FUNCTION；专题A；固定time辅助行段47-47。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E020 [VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_cpu]
```c
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)
```
类别：MACRO；专题A；固定time辅助行段19-20。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E021 [VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]
```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
```
类别：MACRO；专题A；固定time辅助行段22-22。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E022 [VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_online_cpu]
```c
	#define for_each_online_cpu(cpu)	for_each_cpu((cpu))
```
类别：MACRO；专题A；固定time辅助行段23-23。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E023 [VERIFIED mykernel/include/linux/kernel/cpumask.h::num_possible_cpus]
```c
	#define num_possible_cpus()		nr_cpu_ids
```
类别：MACRO；专题A；固定time辅助行段27-27。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E024 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::__per_cpu_offset]
```c
unsigned long __per_cpu_offset[NR_CPUS] __ro_after_init = {
	// [0 ... NR_CPUS-1] = BOOT_PERCPU_OFFSET,
	[0 ... NR_CPUS-1] = 0,
};
```
类别：GLOBAL_OBJECT；专题A；固定time辅助行段23-26。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E025 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
	simple_pcpu_setup_first_chunk();
	/* alrighty, percpu areas up and running */
	delta = (ulong)pcpu_base_addr - (ulong)__per_cpu_start;
	for_each_possible_cpu(cpu) {
```
类别：C_FUNCTION；专题A；固定time辅助行段73-76。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E026 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
		per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
```
类别：C_FUNCTION；专题A；固定time辅助行段77-79。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E027 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
		if (cpu == 0)
		{
```
类别：C_FUNCTION；专题A；固定time辅助行段112-113。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E028 [VERIFIED mykernel/arch/x86_64/smp/setup_percpu.c::setup_per_cpu_areas]
```c
				// wrmsrl(MSR_GS_BASE, cpu_kernelmode_gs_base(cpu));
				wrmsrl(MSR_GS_BASE, per_cpu_offset(cpu));
```
类别：C_FUNCTION；专题A；固定time辅助行段141-142。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E029 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::apicid_to_cpunr]
```c
uint64_t apicid_to_cpunr[CONFIG_NR_CPUS] = {
	[0 ... CONFIG_NR_CPUS-1] = -1,
};
```
类别：GLOBAL_OBJECT；专题A；固定time辅助行段81-83。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E030 [VERIFIED mykernel/include/linux/kernel/threads.h::CONFIG_NR_CPUS]
```c
	#	define CONFIG_NR_CPUS	1
```
类别：CONDITIONAL_MACRO；专题A；固定time辅助行段17-17。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E031 [VERIFIED mykernel/include/linux/kernel/threads.h::NR_CPUS]
```c
	#define NR_CPUS				CONFIG_NR_CPUS
```
类别：MACRO；专题A；固定time辅助行段21-21。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。

### SP100-E032 [VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS_CONFIG)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```
类别：CMAKE_UNIQUE_COMMAND；专题A；固定time辅助行段58-61。只证明定义/命令原文，不证明EFI、实际对象或CPU上线。


### SP100-E033 [VERIFIED mykernel/arch/x86_64/myos/smp.c::myos_early_init_smp]
```c
void myos_early_init_smp(void)
{
	// copy ap_boot entry code to its address
	extern char _APboot_phys_start;
```
类别：C_FUNCTION；专题B；固定time辅助行段16-19。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E034 [VERIFIED mykernel/arch/x86_64/myos/smp.c::myos_early_init_smp]
```c
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apboot_len = &_APboot_etext - &_APboot_text;
	memcpy((void *)phys_to_virt((phys_addr_t)&_APboot_phys_start),
			(void *)&_APboot_text, apboot_len);
```
类别：C_FUNCTION；专题B；固定time辅助行段20-24。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E035 [VERIFIED mykernel/arch/x86_64/myos/smp.c::myos_early_init_smp]
```c
	memcpy((void *)phys_to_virt((phys_addr_t)&_APboot_phys_start),
			(void *)&_APboot_text, apboot_len);
	cr3_paddr = kernel_cr3;
}
```
类别：C_FUNCTION；专题B；固定time辅助行段23-26。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E036 [VERIFIED mykernel/arch/x86_64/myos/smp.c::myos_startup_smp]
```c
void myos_startup_smp()
{
	wrmsrl(0x830,0xc4500);	//INIT IPI
	wrmsrl(0x830,0xc4620);	//Start-up IPI
```
类别：C_FUNCTION；专题B；固定time辅助行段28-31。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E037 [VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
asmlinkage __visible void __init
x86_64_start_kernel(char * real_mode_data)
{
	clear_bss();
```
类别：C_FUNCTION；专题B；固定time辅助行段155-158。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E038 [VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
	myos_early_init_system();

	/* set init_top_pgt kernel high mapping*/
	init_top_pgt[511] = early_top_pgt[511];
```
类别：C_FUNCTION；专题B；固定time辅助行段171-174。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E039 [VERIFIED mykernel/arch/x86_64/kernel/head64.c::x86_64_start_kernel]
```c
		start_kernel();
	// }
}
```
类别：C_FUNCTION；专题B；固定time辅助行段179-181。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E040 [VERIFIED mykernel/init/main.c::start_kernel]
```c
asmlinkage void __init start_kernel(void)
{
	char *command_line;
	char *after_dashes;

```
类别：C_FUNCTION；专题B；固定time辅助行段132-136。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E041 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	setup_arch(NULL);

	setup_per_cpu_areas();
```
类别：C_FUNCTION；专题B；固定time辅助行段138-140。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E042 [VERIFIED mykernel/init/main.c::start_kernel]
```c
	check_bugs();

	myos_init_percpu_intr();

	rest_init();
```
类别：C_FUNCTION；专题B；固定time辅助行段190-194。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E043 [VERIFIED mykernel/init/main.c::kernel_init_freeable]
```c
	// smp_prepare_cpus(setup_max_cpus);

	// workqueue_init();

	// rcu_init_tasks_generic();
```
类别：C_FUNCTION；专题B；固定time辅助行段323-327。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E044 [VERIFIED mykernel/init/main.c::kernel_init_freeable]
```c
	do_pre_smp_initcalls();
	// lockup_detector_init();

	// smp_init();
	// sched_init_smp();
```
类别：C_FUNCTION；专题B；固定time辅助行段328-332。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E045 [VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
void __init setup_arch(char **cmdline_p)
{
	// printk(KERN_INFO "Command line: %s\n", boot_command_line);
	boot_cpu_data.x86_phys_bits = MAX_PHYSMEM_BITS;
	
```
类别：C_FUNCTION；专题B；固定time辅助行段169-173。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E046 [VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	max_possible_pfn = max_pfn;

	myos_try_enable_x2apic();

	check_x2apic();
```
类别：C_FUNCTION；专题B；固定time辅助行段221-225。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E047 [VERIFIED mykernel/arch/x86_64/myos/interrupt.c::myos_init_percpu_intr]
```c
void myos_init_percpu_intr()
{
	LVT_timer_init();
}
```
类别：C_FUNCTION；专题B；固定time辅助行段229-232。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E048 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::__x2apic_enable]
```c
static void
__x2apic_enable(void) {
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, &msr);
```
类别：C_FUNCTION；专题B；固定time辅助行段255-259。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E049 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::__x2apic_enable]
```c
	if (msr & X2APIC_ENABLE)
		return;
	if (!(msr & XAPIC_ENABLE))
		msr |= XAPIC_ENABLE;
	wrmsrl(MSR_IA32_APICBASE, msr | X2APIC_ENABLE);
```
类别：C_FUNCTION；专题B；固定time辅助行段260-264。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E050 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::myos_try_enable_x2apic]
```c
void __init myos_try_enable_x2apic(void)
{
	if (boot_cpu_data.x86_capa_bits.x2APIC) {
		__x2apic_enable();
		if(apic_is_x2apic_enabled()) {
```
类别：C_FUNCTION；专题B；固定time辅助行段292-296。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E051 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::myos_try_enable_x2apic]
```c
		if(apic_is_x2apic_enabled()) {
			pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
			x2apic_mode = 1;
			if (x2apic_hw_locked())
				x2apic_state = X2APIC_ON_LOCKED;
```
类别：C_FUNCTION；专题B；固定time辅助行段296-300。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E052 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::myos_try_enable_x2apic]
```c
				x2apic_state = X2APIC_ON_LOCKED;
			else
				x2apic_state = X2APIC_ON;
		}
		init_bsp_APIC();
```
类别：C_FUNCTION；专题B；固定time辅助行段300-304。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E053 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::myos_try_enable_x2apic]
```c
	} else if (!boot_cpu_data.x86_capa_bits.x2APIC) {
		x2apic_state = X2APIC_DISABLED;
	}
}
```
类别：C_FUNCTION；专题B；固定time辅助行段305-308。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E054 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::check_x2apic]
```c
void __init check_x2apic(void)
{
	if (x2apic_enabled()) {
		pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
		x2apic_mode = 1;
```
类别：C_FUNCTION；专题B；固定time辅助行段312-316。引用只证明函数内原文及活跃/注释区别，不证明实际到达。

### SP100-E055 [VERIFIED mykernel/arch/x86_64/kernel/apic/apic.c::check_x2apic]
```c
		if (x2apic_hw_locked())
			x2apic_state = X2APIC_ON_LOCKED;
		else
			x2apic_state = X2APIC_ON;
		// apic_read_boot_cpu_id(true);
```
类别：C_FUNCTION；专题B；固定time辅助行段317-321。引用只证明函数内原文及活跃/注释区别，不证明实际到达。


### SP100-E056 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	EFI_STATUS status = EFI_SUCCESS;
	// Check Format
	PHYSICAL_ADDRESS raw_img_addr = 0x2000000;
	LoadFile(LoadedImage, filename, raw_img_addr, 0);
	BOOLEAN isElf = IsElfFormat((UINT8 *)raw_img_addr);
```
类别：C_FUNCTION；专题D00/B；固定time辅助行段334-338。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E057 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	EFI_PHYSICAL_ADDRESS loaded_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.ImageAddress;
	*entry_addr = (EFI_PHYSICAL_ADDRESS)ElfCtx.EntryPoint;
	// Alloc Image Memory
	ElfCtx.ImageAddress = ElfCtx.PreferredImageAddress;
```
类别：C_FUNCTION；专题D00/B；固定time辅助行段361-364。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E058 [VERIFIED myloader/uefi/bootloader.c::load_elf_kernel]
```c
	gBS->AllocatePages(AllocateAddress, EfiLoaderData,
			(ElfCtx.ImageSize + PAGE_SIZE - 1) / PAGE_SIZE, &loaded_addr);
	LoadElfImage(&ElfCtx);

	return status;
```
类别：C_FUNCTION；专题D00/B；固定time辅助行段365-369。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E059 [VERIFIED myloader/uefi/utils.c::LoadFile]
```c
	bufsize = size;
	if (size == 0)
		bufsize = FileInfo->FileSize;
```
类别：C_FUNCTION；专题D00/B；固定time辅助行段29-31。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E060 [VERIFIED myloader/uefi/ElfLib/ElfLib.c::CalculateElfFileSize]
```c
  } else if (ElfCt->EiClass == ELFCLASS64) {
    Elf64Hdr  = (Elf64_Ehdr *)ElfCt->FileBase;
    FileSize2 = ((UINTN)Elf64Hdr->e_shoff + (UINTN)(Elf64Hdr->e_shentsize * Elf64Hdr->e_shnum));
  }
```
类别：C_FUNCTION；专题D00/B；固定time辅助行段150-153。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E061 [VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE_KC)]
```cmake
file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
```
类别：CMAKE_UNIQUE_COMMAND；专题D00/B；固定time辅助行段30-30。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E062 [VERIFIED mykernel/CMakeLists.txt::file(GLOB_RECURSE_KASM)]
```cmake
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
```
类别：CMAKE_UNIQUE_COMMAND；专题D00/B；固定time辅助行段31-31。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E063 [VERIFIED mykernel/CMakeLists.txt::list(REMOVE_ITEM)]
```cmake
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})
```
类别：CMAKE_UNIQUE_COMMAND_BLOCK；专题D00/B；固定time辅助行段32-33。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E064 [VERIFIED mykernel/CMakeLists.txt::include(target_kernel.cmake)]
```cmake
include(${SCRIPTS_PATH}/target_kernel.cmake)
```
类别：CMAKE_UNIQUE_COMMAND；专题D00/B；固定time辅助行段35-35。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E065 [VERIFIED mykernel/scripts/target_kernel.cmake::target_sources(kernel)]
```cmake
target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
```
类别：CMAKE_UNIQUE_COMMAND；专题D00/B；固定time辅助行段1-1。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E066 [VERIFIED mykernel/scripts/target_kernel.cmake::target_link_options(kernel)]
```cmake
target_link_options(kernel PRIVATE
    -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds
)
```
类别：CMAKE_UNIQUE_COMMAND；专题D00/B；固定time辅助行段6-8。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E067 [VERIFIED mykernel/arch/x86_64/kernel.lds::_APboot_phys_start]
```ld
_APboot_phys_start	= 0xE000;
```
类别：LINKER_ASSIGNMENT；专题D00/B；固定time辅助行段8-8。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E068 [VERIFIED mykernel/arch/x86_64/kernel.lds::ENTRY(phys_startup_64)]
```ld
ENTRY(phys_startup_64)
```
类别：LINKER_COMMAND；专题D00/B；固定time辅助行段4-4。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E069 [VERIFIED mykernel/arch/x86_64/kernel.lds::.text_output]
```ld
	.text ALIGN(_Page_Size) : AT(ADDR(.text) - __START_KERNEL_map)
	{
		_text = .;
		_stext = .;
		KEEP(*(.head.text.startup))
```
类别：LINKER_OUTPUT_SECTION；专题D00/B；固定time辅助行段28-32。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。

### SP100-E070 [VERIFIED mykernel/arch/x86_64/kernel.lds::.text_output]
```ld
		KEEP(*(.head.text))
		*(.text*)
	} : text = 0xcccccccc
```
类别：LINKER_OUTPUT_SECTION；专题D00/B；固定time辅助行段33-35。该锚仅证定义或唯一声明实际语义，非构建或硬件证据。


### SP100-E071 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_start]
```asm
SYM_CODE_START(_APboot_start)
_APboot_base = .
	cli
	wbinvd
```
类别：ASM_ENTRY；专题C；固定time辅助行段11-14。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E072 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_start]
```asm
	mov		$0,			%ax
	mov		%ax,		%ss
	mov		$_APboot_tmp_Stack_top,	%esp
	mov		$_APboot_base,			%esi
```
类别：ASM_ENTRY；专题C；固定time辅助行段15-18。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E073 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_start]
```asm
	mov		%cs,		%ax
	mov		%ax,		%ds
	mov		%ax,		%es
	mov		%ax,		%fs
	mov		%ax,		%gs
```
类别：ASM_ENTRY；专题C；固定time辅助行段22-26。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E074 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_start]
```asm
	// load idt gdt
	lgdtl	_AP_tmp_GDTptr
	lidtl	_AP_tmp_IDTptr
	// enable protected mode
	smsw	%ax
```
类别：ASM_ENTRY；专题C；固定time辅助行段27-31。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E075 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_start]
```asm
	bts		$0,			%ax
	lmsw	%ax
	// goto 32bit code
	jmp		$0x18, $_AP_code32
```
类别：ASM_ENTRY；专题C；固定time辅助行段32-35。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E076 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
_AP_code32:
	movl	$0x20,		%eax
	movl	%eax,		%ds
	movl	%eax,		%es
	movl	%eax,		%ss
```
类别：ASM_LABEL；专题C；固定time辅助行段43-47。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E077 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	movl	%eax,		%fs
	movl	%eax,		%gs
	// open PAE
```
类别：ASM_LABEL；专题C；固定time辅助行段48-50。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E078 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	movl	%cr4,		%eax
	bts		$5,			%eax
	movl	%eax,		%cr4
```
类别：ASM_LABEL；专题C；固定time辅助行段51-53。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E079 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	movl	$cr3_paddr,	%eax
	movl	(%eax),	%eax
	movl	%eax,		%cr3
```
类别：ASM_LABEL；专题C；固定time辅助行段55-57。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E080 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	movl	$0xC0000080,%ecx
	rdmsr
	bts		$8,			%eax
	wrmsr
```
类别：ASM_LABEL；专题C；固定time辅助行段59-62。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E081 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	movl	%cr0,		%eax
	bts		$0,			%eax
	bts		$31,		%eax
	movl	%eax,		%cr0
```
类别：ASM_LABEL；专题C；固定time辅助行段64-67。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E082 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code32]
```asm
	jmp		$0x08,		$_AP_code64
```
类别：ASM_LABEL；专题C；固定time辅助行段69-69。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E083 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
_AP_code64:
	movq	$0x10,		%rax
	movq	%rax,		%ds
	movq	%rax,		%es
	movq	%rax,		%fs
```
类别：ASM_LABEL；专题C；固定time辅助行段77-81。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E084 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
	movq	%rax,		%gs
	movq	%rax,		%ss

	movabsq	kernel_cr3,	%rax
	movq	%rax,		%cr3
```
类别：ASM_LABEL；专题C；固定time辅助行段82-86。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E085 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
	movq 	$0x1b,		%rcx
	rdmsr
	bts		$10,		%rax
	bts		$11,		%rax
	wrmsr
```
类别：ASM_LABEL；专题C；固定time辅助行段89-93。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E086 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
	movq	$0x802,		%rcx
	rdmsr
```
类别：ASM_LABEL；专题C；固定time辅助行段94-95。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E087 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
	// enable fsgsbase instrutctions
	movq	%cr4,		%rcx
	bts		$16,		%rcx
	movq	%rcx,		%cr4
```
类别：ASM_LABEL；专题C；固定time辅助行段96-99。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E088 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_code64]
```asm
	// compair lapic_id in table to get current AP's index
	movq	%rax,		%r8
	movabsq	$apicid_to_cpunr,	%rax
	movq	$0,			%rbx
```
类别：ASM_LABEL；专题C；固定time辅助行段101-104。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E089 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::l1]
```asm
l1:
	incq	%rbx	//	here is a pre-requisit that BSP's apic_id is 0
	movq	(%rax, %rbx, 8),	%r9
	cmpq	%r9,		%r8
	jne		l1
```
类别：ASM_LOCAL_BASIC_BLOCK；专题C；固定time辅助行段105-109。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E090 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::AP_DEBUG]
```asm
SYM_CODE_START(AP_DEBUG)
	// get idle_task_PCB for current AP and set rsp to task's kstack
	// movabsq	idle_tasks,		%rax
	movq	(%rax, %rbx, 8),%rax
	addq	$THREAD_SIZE,	%rax
```
类别：ASM_ENTRY；专题C；固定time辅助行段111-115。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E091 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::AP_DEBUG]
```asm
	movq	(%rax, %rbx, 8),%rax
	addq	$THREAD_SIZE,	%rax
	movq	%rax,		%rsp
```
类别：ASM_ENTRY；专题C；固定time辅助行段114-116。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E092 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::AP_DEBUG]
```asm
	jmp		.
```
类别：ASM_ENTRY；专题C；固定time辅助行段124-124。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E093 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_tmp_IDTptr]
```asm
_AP_tmp_IDTptr:
	.word	0
	.word	0,0
```
类别：ASM_DATA_LABEL；专题C；固定time辅助行段131-133。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E094 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_tmp_GDTptr]
```asm
_AP_tmp_GDTptr:
	.short	_AP_tmp_GDT_end - _AP_tmp_GDT - 1
	.long	_AP_tmp_GDT
```
类别：ASM_DATA_LABEL；专题C；固定time辅助行段136-138。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E095 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_tmp_GDT]
```asm
_AP_tmp_GDT:
	.quad	0x0000000000000000			/*0	NULL descriptor		       		00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	18*/
```
类别：ASM_DATA_LABEL；专题C；固定time辅助行段139-143。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E096 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_AP_tmp_GDT]
```asm
	.quad	0x00cf9a000000ffff			/*1	KERNEL	Code	32-bit	Segment	18*/
	.quad	0x00cf92000000ffff			/*2	KERNEL	Data	32-bit	Segment	20*/
_AP_tmp_GDT_end:
```
类别：ASM_DATA_LABEL；专题C；固定time辅助行段143-145。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E097 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::cr3_paddr]
```asm
SYM_DATA(cr3_paddr, .long 0)
```
类别：ASM_DATA_MACRO；专题C；固定time辅助行段147-147。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。

### SP100-E098 [VERIFIED mykernel/arch/x86_64/kernel/myos_APboot.S::_APboot_tmp_Stack_start]
```asm
.balign 4
_APboot_tmp_Stack_start:
	.org	0x400
_APboot_tmp_Stack_top:
```
类别：ASM_DATA_LABEL；专题C；固定time辅助行段149-152。ASM入口/局部基本块/独立数据标签仅证语句与位置，不认证模式切换、寄存器真实值或CPU启动。


## 证明上限

所有短锚仅记录固定源码文本及有条件运算，不认证所选真实编译/ABI/ELF映射、寄存器执行结果、两核同步或实机启动。G00、D00、R99-S01/S02/S03、R97-S03、R86/87-S01/S02/S03的独立审查仍为必读边界。

### SP100-E099 [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE_ORDER]
```c
	#define THREAD_SIZE_ORDER		3
```
类别：MACRO；专题C；固定time辅助行段13-13。只能证明本源码定义文本，不认证实际链接值、CPU栈、对象与映射。

### SP100-E100 [VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE]
```c
	#define THREAD_SIZE				(PAGE_SIZE << THREAD_SIZE_ORDER)
```
类别：MACRO；专题C；固定time辅助行段14-14。只能证明本源码定义文本，不认证实际链接值、CPU栈、对象与映射。

### SP100-E101 [VERIFIED mykernel/arch/x86_64/mm/init.c::kernel_cr3]
```c
phys_addr_t kernel_cr3 = 0;
```
类别：GLOBAL_OBJECT；专题C；固定time辅助行段102-102。只能证明本源码定义文本，不认证实际链接值、CPU栈、对象与映射。

