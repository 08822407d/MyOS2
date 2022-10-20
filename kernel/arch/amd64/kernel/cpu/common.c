// SPDX-License-Identifier: GPL-2.0-only
/* cpu_feature_enabled() cannot be used this early */
// #define USE_EARLY_PGTABLE_L5

#include <linux/mm/memblock.h>
#include <linux/kernel/linkage.h>
#include <linux/kernel/bitops.h>
#include <linux/kernel/kernel.h>
// #include <linux/export.h>
// #include <linux/percpu.h>
#include <linux/lib/string.h>
#include <linux/kernel/ctype.h>
#include <linux/kernel/delay.h>
// #include <linux/sched/mm.h>
// #include <linux/sched/clock.h>
// #include <linux/sched/task.h>
// #include <linux/sched/smt.h>
#include <linux/init/init.h>
// #include <linux/kprobes.h>
// #include <linux/kgdb.h>
// #include <linux/smp.h>
// #include <linux/io.h>
// #include <linux/syscore_ops.h>
// #include <linux/pgtable.h>

// #include <asm/cmdline.h>
// #include <asm/stackprotector.h>
// #include <asm/perf_event.h>
// #include <asm/mmu_context.h>
// #include <asm/doublefault.h>
// #include <asm/archrandom.h>
// #include <asm/hypervisor.h>
// #include <asm/processor.h>
// #include <asm/tlbflush.h>
// #include <asm/debugreg.h>
// #include <asm/sections.h>
// #include <asm/vsyscall.h>
// #include <linux/topology.h>
// #include <linux/cpumask.h>
// #include <linux/atomic.h>
// #include <asm/proto.h>
#include <asm/setup.h>
#include <asm/apic.h>
// #include <asm/desc.h>
// #include <asm/fpu/api.h>
// #include <asm/mtrr.h>
// #include <asm/hwcap2.h>
// #include <linux/numa.h>
// #include <asm/numa.h>
// #include <asm/asm.h>
#include <asm/bugs.h>
#include <asm/cpu.h>
// #include <asm/mce.h>
#include <asm/msr.h>
// #include <asm/memtype.h>
// #include <asm/microcode.h>
// #include <asm/microcode_intel.h>
// #include <asm/intel-family.h>
// #include <asm/cpu_device_id.h>
// #include <asm/uv/uv.h>
// #include <asm/sigframe.h>

#include "cpu.h"

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

static void get_cpu_vendor(cpuinfo_x86_s *c)
{
	// char *v = c->x86_vendor_id;
	// int i;

	// for (i = 0; i < X86_VENDOR_NUM; i++) {
	// 	if (!cpu_devs[i])
	// 		break;

	// 	if (!strcmp(v, cpu_devs[i]->c_ident[0]) ||
	// 	    (cpu_devs[i]->c_ident[1] &&
	// 	     !strcmp(v, cpu_devs[i]->c_ident[1]))) {

	// 		this_cpu = cpu_devs[i];
	// 		c->x86_vendor = this_cpu->c_x86_vendor;
	// 		return;
	// 	}
	// }

	// // pr_err_once("CPU: vendor_id '%s' unknown, using generic init.\n" \
	// // 	    "CPU: Your system may be unstable.\n", v);

	// c->x86_vendor = X86_VENDOR_UNKNOWN;
	// this_cpu = &default_cpu;
}

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
// 	const struct cpu_dev *const *cdev;
// 	int count = 0;

// #ifdef CONFIG_PROCESSOR_SELECT
// 	pr_info("KERNEL supported cpus:\n");
// #endif

// 	for (cdev = __x86_cpu_dev_start; cdev < __x86_cpu_dev_end; cdev++)
// 	{
// 		const struct cpu_dev *cpudev = *cdev;

// 		if (count >= X86_VENDOR_NUM)
// 			break;
// 		cpu_devs[count] = cpudev;
// 		count++;

// #ifdef CONFIG_PROCESSOR_SELECT
// 		{
// 			unsigned int j;

// 			for (j = 0; j < 2; j++)
// 			{
// 				if (!cpudev->c_ident[j])
// 					continue;
// 				pr_info("  %s %s\n", cpudev->c_vendor,
// 						cpudev->c_ident[j]);
// 			}
// 		}
// #endif
// 	}

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
		get_cpu_vendor(&boot_cpu_data);
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
	get_cpu_vendor(c);
	get_cpu_cap(c);
	get_cpu_address_sizes(c);

	if (c->cpuid_level >= 0x00000001) {
		c->initial_apicid = (cpuid_ebx(1) >> 24) & 0xFF;
		c->phys_proc_id = c->initial_apicid;
	}

	get_model_name(c); /* Default name */
}

// /*
//  * Validate that ACPI/mptables have the same information about the
//  * effective APIC id and update the package map.
//  */
// static void validate_apic_and_package_id(cpuinfo_x86_s *c)
// {
// 	unsigned int apicid, cpu = smp_processor_id();

// 	apicid = apic->cpu_present_to_apicid(cpu);

// 	if (apicid != c->apicid) {
// 		pr_err(FW_BUG "CPU%u: APIC id mismatch. Firmware: %x APIC: %x\n",
// 		       cpu, apicid, c->initial_apicid);
// 	}
// 	BUG_ON(topology_update_package_map(c->phys_proc_id, cpu));
// 	BUG_ON(topology_update_die_map(c->cpu_die_id, cpu));
// }

/*
 * This does the hard work of actually picking apart the CPU stuff...
 */
static void identify_cpu(struct cpuinfo_x86 *c)
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

void identify_secondary_cpu(struct cpuinfo_x86 *c)
{
	// BUG_ON(c == &boot_cpu_data);
	identify_cpu(c);
	// mtrr_ap_init();
	// validate_apic_and_package_id(c);
	// x86_spec_ctrl_setup_ap();
	// update_srbds_msr();

	// tsx_ap_init();
}