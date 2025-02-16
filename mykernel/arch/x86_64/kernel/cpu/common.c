// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0-only
/* cpu_feature_enabled() cannot be used this early */
// #define USE_EARLY_PGTABLE_L5


#include <linux/kernel/ctype.h>
#include <linux/kernel/delay.h>
#include <linux/kernel/mm_api.h>
#include <linux/kernel/sched_api.h>

#include <asm/apic.h>
#include <asm/cpu.h>
#include <asm/fpu.h>
#include <asm/proto.h>


#include "cpu.h"

#include <obsolete/arch_proto.h>
#include <obsolete/glo.h>


/* Load the original GDT from the per-cpu structure */
void load_direct_gdt(int cpu)
{
	struct desc_ptr gdt_descr;

	gdt_descr.address = (ulong)get_cpu_gdt_rw(cpu);
	gdt_descr.size = GDT_SIZE - 1;
	load_gdt(&gdt_descr);

	refresh_segment_registers();
}

static void get_model_name(cpuinfo_x86_s *c)
{
	unsigned int *v;
	char *p, *q, *s;

	if (c->extended_cpuid_level < 0x80000004)
		return;

	v = (unsigned int *)c->x86_model_id;
	cpuid(0x80000002, &v[0], &v[1], &v[2], &v[3]);
	cpuid(0x80000003, &v[4], &v[5], &v[6], &v[7]);
	cpuid(0x80000004, &v[8], &v[9], &v[10], &v[11]);
	c->x86_model_id[48] = 0;

	/* Trim whitespace */
	p = q = s = &c->x86_model_id[0];

	while (*p == ' ')
		p++;

	while (*p) {
		/* Note the last non-whitespace index */
		if (!isspace(*p))
			s = q;

		*q++ = *p++;
	}

	*(s + 1) = '\0';
}

void detect_num_cpu_cores(cpuinfo_x86_s *c)
{
	unsigned int eax, ebx, ecx, edx;

	c->x86_max_cores = 1;
	if (c->cpuid_level < 4)
		return;

	cpuid_count(4, 0, &eax, &ebx, &ecx, &edx);
	if (eax & 0x1f)
		c->x86_max_cores = (eax >> 26) + 1;
}

void cpu_detect_cache_sizes(cpuinfo_x86_s *c)
{
	unsigned int n, dummy, ebx, ecx, edx, l2size;

	n = c->extended_cpuid_level;

	if (n >= 0x80000005) {
		cpuid(0x80000005, &dummy, &ebx, &ecx, &edx);
		c->x86_cache_size = (ecx>>24) + (edx>>24);
		/* On K8 L1 TLB is inclusive, so don't count it */
		c->x86_tlbsize = 0;
	}

	if (n < 0x80000006)	/* Some chips just has a large L1. */
		return;

	cpuid(0x80000006, &dummy, &ebx, &ecx, &edx);
	l2size = ecx >> 16;

	c->x86_tlbsize += ((ebx >> 16) & 0xfff) + (ebx & 0xfff);
	c->x86_cache_size = l2size;
}

static void default_init(cpuinfo_x86_s *c)
{
	cpu_detect_cache_sizes(c);
}

static const cpu_dev_s default_cpu = {
	.c_init			= default_init,
	.c_vendor		= "Unknown",
	.c_x86_vendor	= X86_VENDOR_UNKNOWN,
};

static const cpu_dev_s *this_cpu = &default_cpu;
static const cpu_dev_s *cpu_devs[X86_VENDOR_NUM] = {};

/* Aligned to unsigned long to avoid split lock in atomic bitmap ops */
__u32 cpu_caps_cleared[NCAPINTS + NBUGINTS] __aligned(sizeof(unsigned long));
__u32 cpu_caps_set[NCAPINTS + NBUGINTS] __aligned(sizeof(unsigned long));


void cpu_detect(cpuinfo_x86_s *c)
{
	/* Get vendor name */
	cpuid(0x00000000,
			(unsigned int *)&c->cpuid_level,
			(unsigned int *)&c->x86_vendor_id[0],
			(unsigned int *)&c->x86_vendor_id[8],
			(unsigned int *)&c->x86_vendor_id[4]);

	c->x86_family = 4;
	/* Intel-defined flags: level 0x00000001 */
	if (c->cpuid_level >= 0x00000001)
	{
		u32 junk, tfms, cap0, misc;

		cpuid(0x00000001, &tfms, &misc, &junk, &cap0);
		c->x86_family	= x86_family(tfms);
		c->x86_model	= x86_model(tfms);
		c->x86_stepping	= x86_stepping(tfms);

		if (cap0 & (1<<19))
		{
			c->x86_clflush_size		= ((misc >> 8) & 0xff) * 8;
			c->x86_cache_alignment	= c->x86_clflush_size;
		}
	}
}

static void apply_forced_caps(cpuinfo_x86_s *c)
{
	int i;

	for (i = 0; i < NCAPINTS + NBUGINTS; i++) {
		c->x86_capability[i] &= ~cpu_caps_cleared[i];
		c->x86_capability[i] |= cpu_caps_set[i];
	}
}

void get_cpu_cap(cpuinfo_x86_s *c)
{
	u32 eax, ebx, ecx, edx;

	/* Intel-defined flags: level 0x00000001 */
	if (c->cpuid_level >= 0x00000001) {
		cpuid(0x00000001, &eax, &ebx, &ecx, &edx);

		c->x86_capability[CPUID_1_ECX] = ecx;
		c->x86_capability[CPUID_1_EDX] = edx;
	}

	/* Thermal and Power Management Leaf: level 0x00000006 (eax) */
	if (c->cpuid_level >= 0x00000006)
		c->x86_capability[CPUID_6_EAX] = cpuid_eax(0x00000006);

	/* Additional Intel-defined flags: level 0x00000007 */
	if (c->cpuid_level >= 0x00000007) {
		cpuid_count(0x00000007, 0, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_7_0_EBX] = ebx;
		c->x86_capability[CPUID_7_ECX] = ecx;
		c->x86_capability[CPUID_7_EDX] = edx;

		/* Check valid sub-leaf index before accessing it */
		if (eax >= 1) {
			cpuid_count(0x00000007, 1, &eax, &ebx, &ecx, &edx);
			c->x86_capability[CPUID_7_1_EAX] = eax;
		}
	}

	/* Extended state features: level 0x0000000d */
	if (c->cpuid_level >= 0x0000000d) {
		cpuid_count(0x0000000d, 1, &eax, &ebx, &ecx, &edx);

		c->x86_capability[CPUID_D_1_EAX] = eax;
	}

	/* AMD-defined flags: level 0x80000001 */
	eax = cpuid_eax(0x80000000);
	c->extended_cpuid_level = eax;

	if ((eax & 0xffff0000) == 0x80000000) {
		if (eax >= 0x80000001) {
			cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

			c->x86_capability[CPUID_8000_0001_ECX] = ecx;
			c->x86_capability[CPUID_8000_0001_EDX] = edx;
		}
	}

	if (c->extended_cpuid_level >= 0x80000007) {
		cpuid(0x80000007, &eax, &ebx, &ecx, &edx);

		c->x86_capability[CPUID_8000_0007_EBX] = ebx;
		c->x86_power = edx;
	}

	if (c->extended_cpuid_level >= 0x80000008) {
		cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_8000_0008_EBX] = ebx;
	}

	if (c->extended_cpuid_level >= 0x8000000a)
		c->x86_capability[CPUID_8000_000A_EDX] = cpuid_edx(0x8000000a);

	if (c->extended_cpuid_level >= 0x8000001f)
		c->x86_capability[CPUID_8000_001F_EAX] = cpuid_eax(0x8000001f);

	// init_scattered_cpuid_features(c);
	// init_speculation_control(c);

	/*
	 * Clear/Set all flags overridden by options, after probe.
	 * This needs to happen each time we re-probe, which may happen
	 * several times during CPU initialization.
	 */
	apply_forced_caps(c);
}

void get_cpu_address_sizes(cpuinfo_x86_s *c)
{
	u32 eax, ebx, ecx, edx;

	if (c->extended_cpuid_level >= 0x80000008) {
		cpuid(0x80000008, &eax, &ebx, &ecx, &edx);

		c->x86_virt_bits = (eax >> 8) & 0xff;
		c->x86_phys_bits = eax & 0xff;
	}

	c->x86_cache_bits = c->x86_phys_bits;
}

void __init early_cpu_init(void)
{
	// const struct cpu_dev *const *cdev;
	// int count = 0;

	// for (cdev = __x86_cpu_dev_start; cdev < __x86_cpu_dev_end; cdev++)
	// {
	// 	const struct cpu_dev *cpudev = *cdev;

	// 	if (count >= X86_VENDOR_NUM)
	// 		break;
	// 	cpu_devs[count] = cpudev;
	// 	count++;
	// }

	/*
	 * Do minimum CPU detection early.
	 * Fields really needed: vendor, cpuid_level, family, model, mask,
	 * cache alignment.
	 * The others are not touched to avoid unwanted side effects.
	 *
	 * WARNING: this function is only called on the boot CPU.  Don't add code
	 * here that is supposed to run on all CPUs.
	 */
	// static void __init early_identify_cpu(cpuinfo_x86_s *c)
	// {
		boot_cpu_data.x86_clflush_size		= 64;
		boot_cpu_data.x86_phys_bits			= 36;
		boot_cpu_data.x86_virt_bits			= 48;
		boot_cpu_data.x86_cache_alignment	= boot_cpu_data.x86_clflush_size;
		// memset(&boot_cpu_data.x86_capability, 0,
		// 		sizeof(boot_cpu_data.x86_capability));
		boot_cpu_data.extended_cpuid_level	= 0;

		/* cyrix could have cpuid enabled via c_identify()*/
		cpu_detect(&boot_cpu_data);
		// get_cpu_vendor(&boot_cpu_data);
		get_cpu_cap(&boot_cpu_data);
		get_cpu_address_sizes(&boot_cpu_data);
		// setup_force_cpu_cap(X86_FEATURE_CPUID);
		// cpu_parse_early_param();

		// if (this_cpu->c_early_init)
		// 	this_cpu->c_early_init(c);

		// c->cpu_index = 0;
		// filter_cpuid_features(c, false);

		// if (this_cpu->c_bsp_init)
		// 	this_cpu->c_bsp_init(c);

		// setup_force_cpu_cap(X86_FEATURE_ALWAYS);

		// cpu_set_bug_bits(c);

		// sld_setup(c);

		// fpu__init_system(c);

		// init_sigframe_size();

		// /*
		// * Later in the boot process pgtable_l5_enabled() relies on
		// * cpu_feature_enabled(X86_FEATURE_LA57). If 5-level paging is not
		// * enabled by this point we need to clear the feature bit to avoid
		// * false-positives at the later stage.
		// *
		// * pgtable_l5_enabled() can be false here for several reasons:
		// *  - 5-level paging is disabled compile-time;
		// *  - it's 32-bit kernel;
		// *  - machine doesn't support 5-level paging;
		// *  - user specified 'no5lvl' in kernel command line.
		// */
		// if (!pgtable_l5_enabled())
		// 	setup_clear_cpu_cap(X86_FEATURE_LA57);

		// detect_nopl();
	// }
}

static void generic_identify(cpuinfo_x86_s *c)
{
	c->extended_cpuid_level = 0;

	cpu_detect(c);
	// get_cpu_vendor(c);
	get_cpu_cap(c);
	get_cpu_address_sizes(c);

	if (c->cpuid_level >= 0x00000001) {
		c->initial_apicid = (cpuid_ebx(1) >> 24) & 0xFF;
		c->phys_proc_id = c->initial_apicid;
	}

	get_model_name(c); /* Default name */
}


/*
 * This does the hard work of actually picking apart the CPU stuff...
 */
static void identify_cpu(cpuinfo_x86_s *c)
{
	int i;

	c->loops_per_jiffy = loops_per_jiffy;
	c->x86_cache_size = 0;
	c->x86_vendor = X86_VENDOR_UNKNOWN;
	c->x86_model = c->x86_stepping = 0;	/* So far unknown... */
	c->x86_vendor_id[0] = '\0'; /* Unset */
	c->x86_model_id[0] = '\0';  /* Unset */
	c->x86_max_cores = 1;
	c->x86_coreid_bits = 0;
	c->cu_id = 0xff;
	c->x86_clflush_size = 64;
	c->x86_phys_bits = 36;
	c->x86_virt_bits = 48;
	c->x86_cache_alignment = c->x86_clflush_size;
	memset(&c->x86_capability, 0, sizeof(c->x86_capability));
// #ifdef CONFIG_X86_VMX_FEATURE_NAMES
// 	memset(&c->vmx_capability, 0, sizeof(c->vmx_capability));
// #endif

	generic_identify(c);

	if (this_cpu->c_identify)
		this_cpu->c_identify(c);

	// /* Clear/Set all flags overridden by options, after probe */
	// apply_forced_caps(c);

	// c->apicid = apic->phys_pkg_id(c->initial_apicid, 0);

	// /*
	//  * Vendor-specific initialization.  In this section we
	//  * canonicalize the feature flags, meaning if there are
	//  * features a certain CPU supports which CPUID doesn't
	//  * tell us, CPUID claiming incorrect flags, or other bugs,
	//  * we handle them here.
	//  *
	//  * At the end of this section, c->x86_capability better
	//  * indicate the features this CPU genuinely supports!
	//  */
	// if (this_cpu->c_init)
	// 	this_cpu->c_init(c);

	// /* Disable the PN if appropriate */
	// squash_the_stupid_serial_number(c);

	// /* Set up SMEP/SMAP/UMIP */
	// setup_smep(c);
	// setup_smap(c);
	// setup_umip(c);

	// /* Enable FSGSBASE instructions if available. */
	// if (cpu_has(c, X86_FEATURE_FSGSBASE)) {
	// 	cr4_set_bits(X86_CR4_FSGSBASE);
	// 	elf_hwcap2 |= HWCAP2_FSGSBASE;
	// }

	// /*
	//  * The vendor-specific functions might have changed features.
	//  * Now we do "generic changes."
	//  */

	// /* Filter out anything that depends on CPUID levels we don't have */
	// filter_cpuid_features(c, true);

	// /* If the model name is still unset, do table lookup. */
	// if (!c->x86_model_id[0]) {
	// 	const char *p;
	// 	p = table_lookup_model(c);
	// 	if (p)
	// 		strcpy(c->x86_model_id, p);
	// 	else
	// 		/* Last resort... */
	// 		sprintf(c->x86_model_id, "%02x/%02x",
	// 			c->x86, c->x86_model);
	// }

	// detect_ht(c);
	// x86_init_rdrand(c);
	// setup_pku(c);

	// /*
	//  * Clear/Set all flags overridden by options, need do it
	//  * before following smp all cpus cap AND.
	//  */
	// apply_forced_caps(c);

	// /*
	//  * On SMP, boot_cpu_data holds the common feature set between
	//  * all CPUs; so make sure that we indicate which features are
	//  * common between the CPUs.  The first time this routine gets
	//  * executed, c == &boot_cpu_data.
	//  */
	// if (c != &boot_cpu_data) {
	// 	/* AND the already accumulated flags with these */
	// 	for (i = 0; i < NCAPINTS; i++)
	// 		boot_cpu_data.x86_capability[i] &= c->x86_capability[i];

	// 	/* OR, i.e. replicate the bug flags */
	// 	for (i = NCAPINTS; i < NCAPINTS + NBUGINTS; i++)
	// 		c->x86_capability[i] |= boot_cpu_data.x86_capability[i];
	// }

	// /* Init Machine Check Exception if available. */
	// mcheck_cpu_init(c);

	// select_idle_routine(c);

// #ifdef CONFIG_NUMA
// 	numa_add_cpu(smp_processor_id());
// #endif
}

void __init identify_boot_cpu(void)
{
	identify_cpu(&boot_cpu_data);
	// cpu_detect_tlb(&boot_cpu_data);
	// setup_cr_pinning();

	// tsx_init();
}

void identify_secondary_cpu(cpuinfo_x86_s *c)
{
	BUG_ON(c == &boot_cpu_data);
	identify_cpu(c);
	// mtrr_ap_init();
	// validate_apic_and_package_id(c);
	// x86_spec_ctrl_setup_ap();
	// update_srbds_msr();

	// tsx_ap_init();
}


// EXPORT_PER_CPU_SYMBOL(pcpu_hot);

// DEFINE_PER_CPU_FIRST(struct fixed_percpu_data,
// 		     fixed_percpu_data) __aligned(PAGE_SIZE) __visible;
// EXPORT_PER_CPU_SYMBOL_GPL(fixed_percpu_data);

static void wrmsrl_cstar(unsigned long val) {
	/*
	 * Intel CPUs do not support 32-bit SYSCALL. Writing to MSR_CSTAR
	 * is so far ignored by the CPU, but raises a #VE trap in a TDX
	 * guest. Avoid the pointless write on all Intel CPUs.
	 */
	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		wrmsrl(MSR_CSTAR, val);
}

/* May not be marked __init: used by software suspend */
void syscall_init(void)
{
#if defined(CONFIG_INTEL_X64_GDT_LAYOUT)
	// init MSR regs related to sysenter/sysexit
	wrmsrl(MSR_IA32_SYSENTER_CS, __KERNEL_CS);
	wrmsrl(MSR_IA32_SYSENTER_EIP, (u64)sysenter_entp);
	wrmsrl(MSR_IA32_SYSENTER_ESP, (u64)task_top_of_stack(current));
	uint64_t kstack;
	rdmsrl(MSR_IA32_SYSENTER_ESP, &kstack);
#else
		wrmsr(MSR_STAR, 0, (__USER32_CS << 16) | __KERNEL_CS);
		wrmsrl(MSR_LSTAR, (unsigned long)entry_SYSCALL_64);

		// wrmsrl_cstar((unsigned long)ignore_sysret);
		// wrmsrl_safe(MSR_IA32_SYSENTER_CS, (u64)GDT_ENTRY_INVALID_SEG);
		// wrmsrl_safe(MSR_IA32_SYSENTER_ESP, 0ULL);
		// wrmsrl_safe(MSR_IA32_SYSENTER_EIP, 0ULL);

		/*
		* Flags to clear on syscall; clear as much as possible
		* to minimize user space-kernel interference.
		*/
		wrmsrl(MSR_SYSCALL_MASK,
			X86_EFLAGS_CF|X86_EFLAGS_PF|X86_EFLAGS_AF|
			X86_EFLAGS_ZF|X86_EFLAGS_SF|X86_EFLAGS_TF|
			X86_EFLAGS_IF|X86_EFLAGS_DF|X86_EFLAGS_OF|
			X86_EFLAGS_IOPL|X86_EFLAGS_NT|X86_EFLAGS_RF|
			X86_EFLAGS_AC|X86_EFLAGS_ID);
#endif
}


/*
 * Setup everything needed to handle exceptions from the IDT, including the IST
 * exceptions which use paranoid_entry().
 */
void cpu_init_exception_handling(void)
{
	// struct tss_struct *tss = this_cpu_ptr(&cpu_tss_rw);
	int cpu = raw_smp_processor_id();

	// /* paranoid_entry() gets the CPU number from the GDT */
	// setup_getcpu(cpu);

	// /* IST vectors need TSS to be set up. */
	// tss_setup_ist(tss);
	// tss_setup_io_bitmap(tss);
	// set_tss_desc(cpu, &get_cpu_entry_area(cpu)->tss.x86_tss);
	set_tss_desc(cpu, &(per_cpu(cpu_tss_rw, cpu).x86_tss));

	load_TR_desc();

	// /* GHCB needs to be setup to handle #VC. */
	// setup_ghcb();

	// /* Finally load the IDT */
	load_current_idt();
}

/*
 * cpu_init() initializes state that is per-CPU. Some data is already
 * initialized (naturally) in the bootstrap process, such as the GDT.  We
 * reload it nevertheless, this function acts as a 'CPU state barrier',
 * nothing should get across.
 */
void cpu_init(void)
{
	task_s *curr = current;
	int cpu = raw_smp_processor_id();

	// ucode_cpu_init(cpu);

	pr_debug("Initializing CPU#%d\n", cpu);

	loadsegment(fs, 0);
	// memset(cur->thread.tls_array, 0, GDT_ENTRY_TLS_ENTRIES * 8);
	syscall_init();

	wrmsrl(MSR_FS_BASE, 0);
	wrmsrl(MSR_KERNEL_GS_BASE, 0);
	barrier();

	x2apic_setup();

	// mmgrab(&init_mm);
	curr->active_mm = &init_mm;
	BUG_ON(curr->mm);
	// initialize_tlbstate_and_flush();
	// enter_lazy_tlb(&init_mm, cur);

	// /*
	//  * sp0 points to the entry trampoline stack regardless of what task
	//  * is running.
	//  */
	// load_sp0((unsigned long)(cpu_entry_stack(cpu) + 1));

	// load_mm_ldt(&init_mm);

	// clear_all_debug_regs();
	// dbg_restore_debug_regs();

	// doublefault_init_cpu_tss();

	// if (is_uv_system())
	// 	uv_cpu_init();

	load_direct_gdt(cpu);
}



void __init arch_cpu_finalize_init(void)
{
	// struct cpuinfo_x86 *c = this_cpu_ptr(&cpu_info);

	// identify_boot_cpu();

	// select_idle_routine();

	// /*
	//  * identify_boot_cpu() initialized SMT support information, let the
	//  * core code know.
	//  */
	// cpu_smt_set_num_threads(__max_threads_per_core, __max_threads_per_core);

	// if (!IS_ENABLED(CONFIG_SMP)) {
	// 	pr_info("CPU: ");
	// 	print_cpu_info(&boot_cpu_data);
	// }

	// cpu_select_mitigations();

	// arch_smt_update();

	// if (IS_ENABLED(CONFIG_X86_32)) {
	// 	/*
	// 	 * Check whether this is a real i386 which is not longer
	// 	 * supported and fixup the utsname.
	// 	 */
	// 	if (boot_cpu_data.x86 < 4)
	// 		panic("Kernel requires i486+ for 'invlpg' and other features");

	// 	init_utsname()->machine[1] =
	// 		'0' + (boot_cpu_data.x86 > 6 ? 6 : boot_cpu_data.x86);
	// }

	/*
	 * Must be before alternatives because it might set or clear
	 * feature bits.
	 */
	fpu__init_system();
	// fpu__init_cpu();

	// /*
	//  * Ensure that access to the per CPU representation has the initial
	//  * boot CPU configuration.
	//  */
	// *c = boot_cpu_data;
	// c->initialized = true;

	// alternative_instructions();

	// if (IS_ENABLED(CONFIG_X86_64)) {
	// 	/*
	// 	 * Make sure the first 2MB area is not mapped by huge pages
	// 	 * There are typically fixed size MTRRs in there and overlapping
	// 	 * MTRRs into large pages causes slow downs.
	// 	 *
	// 	 * Right now we don't do that with gbpages because there seems
	// 	 * very little benefit for that case.
	// 	 */
	// 	if (!direct_gbpages)
	// 		set_memory_4k((unsigned long)__va(0), 1);
	// } else {
	// 	fpu__init_check_bugs();
	// }

	// /*
	//  * This needs to be called before any devices perform DMA
	//  * operations that might use the SWIOTLB bounce buffers. It will
	//  * mark the bounce buffers as decrypted so that their usage will
	//  * not cause "plain-text" data to be decrypted when accessed. It
	//  * must be called after late_time_init() so that Hyper-V x86/x64
	//  * hypercalls work when the SWIOTLB bounce buffers are decrypted.
	//  */
	// mem_encrypt_init();
}