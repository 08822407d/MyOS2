// SPDX-License-Identifier: GPL-2.0
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/kernel.h>
// #include <linux/export.h>
#include <linux/init/init.h>
#include <linux/mm/memblock.h>
#include <linux/smp/percpu.h>
// #include <linux/kexec.h>
// #include <linux/crash_dump.h>
// #include <linux/smp.h>
// #include <linux/topology.h>
#include <linux/mm/pfn.h>
// #include <linux/stackprotector.h>
#include <asm/sections.h>
#include <asm/processor.h>
#include <asm/desc.h>
#include <asm/setup.h>
// #include <asm/mpspec.h>
#include <asm/apicdef.h>
// #include <asm/highmem.h>
// #include <asm/proto.h>
// #include <asm/cpumask.h>
#include <asm/cpu.h>


#include <asm-generic/sections.h>

#define BOOT_PERCPU_OFFSET ((unsigned long)__per_cpu_load)

unsigned long __per_cpu_offset[NR_CPUS] __ro_after_init = {
	[0 ... NR_CPUS-1] = BOOT_PERCPU_OFFSET,
};

void __init setup_per_cpu_areas(void)
{
	unsigned int cpu;
	unsigned long delta;
	unsigned long pcpuarea_size =
		(unsigned long)__per_cpu_end - (unsigned long)__per_cpu_start;
	int rc;

	// pr_info("NR_CPUS:%d nr_cpumask_bits:%d nr_cpu_ids:%u nr_node_ids:%u\n",
	// 	NR_CPUS, nr_cpumask_bits, nr_cpu_ids, nr_node_ids);

	// /*
	//  * Allocate percpu area.  Embedding allocator is our favorite;
	//  * however, on NUMA configurations, it can result in very
	//  * sparse unit mapping and vmalloc area isn't spacious enough
	//  * on 32bit.  Use page in that case.
	//  */
	// rc = -EINVAL;
	// if (pcpu_chosen_fc != PCPU_FC_PAGE) {
	// 	const size_t dyn_size = PERCPU_MODULE_RESERVE +
	// 		PERCPU_DYNAMIC_RESERVE - PERCPU_FIRST_CHUNK_RESERVE;
	// 	size_t atom_size;

	// 	/*
	// 	 * On 64bit, use PMD_SIZE for atom_size so that embedded
	// 	 * percpu areas are aligned to PMD.  This, in the future,
	// 	 * can also allow using PMD mappings in vmalloc area.  Use
	// 	 * PAGE_SIZE on 32bit as vmalloc space is highly contended
	// 	 * and large vmalloc area allocs can easily fail.
	// 	 */
	// 	atom_size = PMD_SIZE;

	// 	rc = pcpu_embed_first_chunk(PERCPU_FIRST_CHUNK_RESERVE,
	// 				    dyn_size, atom_size,
	// 				    pcpu_cpu_distance,
	// 				    pcpu_cpu_to_node);
	// 	if (rc < 0)
	// 		pr_warn("%s allocator failed (%d), falling back to page size\n",
	// 			pcpu_fc_names[pcpu_chosen_fc], rc);
	// }
	// if (rc < 0)
	// 	rc = pcpu_page_first_chunk(PERCPU_FIRST_CHUNK_RESERVE,
	// 				   pcpu_cpu_to_node);
	// if (rc < 0)
	// 	panic("cannot initialize percpu area (err=%d)", rc);

	// /* alrighty, percpu areas up and running */
	// delta = (unsigned long)pcpu_base_addr - (unsigned long)__per_cpu_start;
	for_each_possible_cpu(cpu) {
		// MYOS2 alloc percpu area memory here
		if (cpu == 0)
			per_cpu_offset(cpu) = 0;
		else
			per_cpu_offset(cpu) = myos_memblock_alloc_normal(pcpuarea_size,
					SMP_CACHE_BYTES) - (void *)__per_cpu_load;

		// per_cpu_offset(cpu) = delta + pcpu_unit_offsets[cpu];
		// per_cpu(this_cpu_off, cpu) = per_cpu_offset(cpu);
		// per_cpu(pcpu_hot.cpu_number, cpu) = cpu;
		// setup_percpu_segment(cpu);
		// /*
		//  * Copy data used in early init routines from the
		//  * initial arrays to the per cpu data areas.  These
		//  * arrays then become expendable and the *_early_ptr's
		//  * are zeroed indicating that the static arrays are
		//  * gone.
		//  */
		// per_cpu(x86_cpu_to_apicid, cpu) =
		// 	early_per_cpu_map(x86_cpu_to_apicid, cpu);
		// per_cpu(x86_bios_cpu_apicid, cpu) =
		// 	early_per_cpu_map(x86_bios_cpu_apicid, cpu);
		// per_cpu(x86_cpu_to_acpiid, cpu) =
		// 	early_per_cpu_map(x86_cpu_to_acpiid, cpu);

// #ifdef CONFIG_NUMA
		// per_cpu(x86_cpu_to_node_map, cpu) =
		// 	early_per_cpu_map(x86_cpu_to_node_map, cpu);
		// /*
		//  * Ensure that the boot cpu numa_node is correct when the boot
		//  * cpu is on a node that doesn't have memory installed.
		//  * Also cpu_up() will call cpu_to_node() for APs when
		//  * MEMORY_HOTPLUG is defined, before per_cpu(numa_node) is set
		//  * up later with c_init aka intel_init/amd_init.
		//  * So set them all (boot cpu and all APs).
		//  */
		// set_cpu_numa_node(cpu, early_cpu_to_node(cpu));
// #endif
		/*
		 * Up to this point, the boot CPU has been using .init.data
		 * area.  Reload any changed state for the boot CPU.
		 */
		if (cpu == 0)
		{
			/**
			 * switch_gdt_and_percpu_base - Switch to direct GDT and runtime per CPU base
			 * @cpu:	The CPU number for which this is invoked
			 *
			 * Invoked during early boot to switch from early GDT and early per CPU to
			 * the direct GDT and the runtime per CPU area. On 32-bit the percpu base
			 * switch is implicit by loading the direct GDT. On 64bit this requires
			 * to update GSBASE.
			 */
			// void __init switch_gdt_and_percpu_base(int cpu)
			// {
				load_direct_gdt(cpu);

				/*
				 * No need to load %gs. It is already correct.
				 *
				 * Writing %gs on 64bit would zero GSBASE which would make any per
				 * CPU operation up to the point of the wrmsrl() fault.
				 *
				 * Set GSBASE to the new offset. Until the wrmsrl() happens the
				 * early mapping is still valid. That means the GSBASE update will
				 * lose any prior per CPU data which was not copied over in
				 * setup_per_cpu_areas().
				 *
				 * This works even with stackprotector enabled because the
				 * per CPU stack canary is 0 in both per CPU areas.
				 */
				// wrmsrl(MSR_GS_BASE, cpu_kernelmode_gs_base(cpu));
				wrmsrl(MSR_GS_BASE, per_cpu_offset(cpu) +
						(unsigned long)__per_cpu_load);
			// }
		}
	}

	// /* indicate the early static arrays will soon be gone */
	// early_per_cpu_ptr(x86_cpu_to_apicid) = NULL;
	// early_per_cpu_ptr(x86_bios_cpu_apicid) = NULL;
	// early_per_cpu_ptr(x86_cpu_to_acpiid) = NULL;

// #ifdef CONFIG_NUMA
// 	early_per_cpu_ptr(x86_cpu_to_node_map) = NULL;
// #endif

	// /* Setup node to cpumask map */
	// setup_node_to_cpumask_map();

	// /* Setup cpu initialized, callin, callout masks */
	// setup_cpu_local_masks();

	// /*
	//  * Sync back kernel address range again.  We already did this in
	//  * setup_arch(), but percpu data also needs to be available in
	//  * the smpboot asm and arch_sync_kernel_mappings() doesn't sync to
	//  * swapper_pg_dir on 32-bit. The per-cpu mappings need to be available
	//  * there too.
	//  *
	//  * FIXME: Can the later sync in setup_cpu_entry_areas() replace
	//  * this call?
	//  */
	// sync_initial_page_table();
}