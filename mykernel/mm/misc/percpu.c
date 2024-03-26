// source: linux-6.4.9

// SPDX-License-Identifier: GPL-2.0-only
/*
 * mm/percpu.c - percpu memory allocator
 *
 * Copyright (C) 2009		SUSE Linux Products GmbH
 * Copyright (C) 2009		Tejun Heo <tj@kernel.org>
 *
 * Copyright (C) 2017		Facebook Inc.
 * Copyright (C) 2017		Dennis Zhou <dennis@kernel.org>
 *
 * The percpu allocator handles both static and dynamic areas.  Percpu
 * areas are allocated in chunks which are divided into units.  There is
 * a 1-to-1 mapping for units to possible cpus.  These units are grouped
 * based on NUMA properties of the machine.
 *
 *  c0                           c1                         c2
 *  -------------------          -------------------        ------------
 * | u0 | u1 | u2 | u3 |        | u0 | u1 | u2 | u3 |      | u0 | u1 | u
 *  -------------------  ......  -------------------  ....  ------------
 *
 * Allocation is done by offsets into a unit's address space.  Ie., an
 * area of 512 bytes at 6k in c1 occupies 512 bytes at 6k in c1:u0,
 * c1:u1, c1:u2, etc.  On NUMA machines, the mapping may be non-linear
 * and even sparse.  Access is handled by configuring percpu base
 * registers according to the cpu to unit mappings and offsetting the
 * base address using pcpu_unit_size.
 *
 * There is special consideration for the first chunk which must handle
 * the static percpu variables in the kernel image as allocation services
 * are not online yet.  In short, the first chunk is structured like so:
 *
 *                  <Static | [Reserved] | Dynamic>
 *
 * The static data is copied from the original section managed by the
 * linker.  The reserved section, if non-zero, primarily manages static
 * percpu variables from kernel modules.  Finally, the dynamic section
 * takes care of normal allocations.
 *
 * The allocator organizes chunks into lists according to free size and
 * memcg-awareness.  To make a percpu allocation memcg-aware the __GFP_ACCOUNT
 * flag should be passed.  All memcg-aware allocations are sharing one set
 * of chunks and all unaccounted allocations and allocations performed
 * by processes belonging to the root memory cgroup are using the second set.
 *
 * The allocator tries to allocate from the fullest chunk first. Each chunk
 * is managed by a bitmap with metadata blocks.  The allocation map is updated
 * on every allocation and free to reflect the current state while the boundary
 * map is only updated on allocation.  Each metadata block contains
 * information to help mitigate the need to iterate over large portions
 * of the bitmap.  The reverse mapping from page to chunk is stored in
 * the page's index.  Lastly, units are lazily backed and grow in unison.
 *
 * There is a unique conversion that goes on here between bytes and bits.
 * Each bit represents a fragment of size PCPU_MIN_ALLOC_SIZE.  The chunk
 * tracks the number of pages it is responsible for in nr_pages.  Helper
 * functions are used to convert from between the bytes, bits, and blocks.
 * All hints are managed in bits unless explicitly stated.
 *
 * To use this allocator, arch code should do the following:
 *
 * - define __addr_to_pcpu_ptr() and __pcpu_ptr_to_addr() to translate
 *   regular address to percpu pointer and back if they need to be
 *   different from the default
 *
 * - use pcpu_setup_first_chunk() during percpu area initialization to
 *   setup the first chunk containing the kernel static percpu area
 */

// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel/err.h>
#include <linux/kernel/lib.h>
#include <linux/kernel/mm.h>
#include <linux/smp/percpu.h>

#include <asm/sections.h>

#include <linux/smp/percpu.h>



// static int pcpu_unit_pages __ro_after_init;
// static int pcpu_unit_size __ro_after_init;
// static int pcpu_nr_units __ro_after_init;
// static int pcpu_atom_size __ro_after_init;
// int pcpu_nr_slots __ro_after_init;
// static int pcpu_free_slot __ro_after_init;
// int pcpu_sidelined_slot __ro_after_init;
// int pcpu_to_depopulate_slot __ro_after_init;
// static size_t pcpu_chunk_struct_size __ro_after_init;

/* the address of the first chunk which starts with the kernel static area */
void *pcpu_base_addr __ro_after_init;

// static const int *pcpu_unit_map __ro_after_init;		/* cpu -> unit */
const unsigned long *pcpu_unit_offsets __ro_after_init;	/* cpu -> unit offset */



/**
 * pcpu_setup_first_chunk - initialize the first percpu chunk
 * @ai: pcpu_alloc_info describing how to percpu area is shaped
 * @base_addr: mapped address
 *
 * Initialize the first percpu chunk which contains the kernel static
 * percpu area.  This function is to be called from arch percpu area
 * setup path.
 *
 * @ai contains all information necessary to initialize the first
 * chunk and prime the dynamic percpu allocator.
 *
 * @ai->static_size is the size of static percpu area.
 *
 * @ai->reserved_size, if non-zero, specifies the amount of bytes to
 * reserve after the static area in the first chunk.  This reserves
 * the first chunk such that it's available only through reserved
 * percpu allocation.  This is primarily used to serve module percpu
 * static areas on architectures where the addressing model has
 * limited offset range for symbol relocations to guarantee module
 * percpu symbols fall inside the relocatable range.
 *
 * @ai->dyn_size determines the number of bytes available for dynamic
 * allocation in the first chunk.  The area between @ai->static_size +
 * @ai->reserved_size + @ai->dyn_size and @ai->unit_size is unused.
 *
 * @ai->unit_size specifies unit size and must be aligned to PAGE_SIZE
 * and equal to or larger than @ai->static_size + @ai->reserved_size +
 * @ai->dyn_size.
 *
 * @ai->atom_size is the allocation atom size and used as alignment
 * for vm areas.
 *
 * @ai->alloc_size is the allocation size and always multiple of
 * @ai->atom_size.  This is larger than @ai->atom_size if
 * @ai->unit_size is larger than @ai->atom_size.
 *
 * @ai->nr_groups and @ai->groups describe virtual memory layout of
 * percpu areas.  Units which should be colocated are put into the
 * same group.  Dynamic VM areas will be allocated according to these
 * groupings.  If @ai->nr_groups is zero, a single group containing
 * all units is assumed.
 *
 * The caller should have mapped the first chunk at @base_addr and
 * copied static data to each unit.
 *
 * The first chunk will always contain a static and a dynamic region.
 * However, the static region is not managed by any chunk.  If the first
 * chunk also contains a reserved region, it is served by two chunks -
 * one for the reserved region and one for the dynamic region.  They
 * share the same vm, but use offset regions in the area allocation map.
 * The chunk serving the dynamic region is circulated in the chunk slots
 * and available for dynamic allocation like any other chunk.
 */
// void __init pcpu_setup_first_chunk(const struct pcpu_alloc_info *ai,
// 				   void *base_addr)
// {
// 	size_t size_sum = ai->static_size + ai->reserved_size + ai->dyn_size;
// 	size_t static_size, dyn_size;
// 	struct pcpu_chunk *chunk;
// 	unsigned long *group_offsets;
// 	size_t *group_sizes;
// 	unsigned long *unit_off;
// 	unsigned int cpu;
// 	int *unit_map;
// 	int group, unit, i;
// 	int map_size;
// 	unsigned long tmp_addr;
// 	size_t alloc_size;

// #define PCPU_SETUP_BUG_ON(cond)	do {					\
// 	if (unlikely(cond)) {						\
// 		pr_emerg("failed to initialize, %s\n", #cond);		\
// 		pr_emerg("cpu_possible_mask=%*pb\n",			\
// 			 cpumask_pr_args(cpu_possible_mask));		\
// 		pcpu_dump_alloc_info(KERN_EMERG, ai);			\
// 		BUG();							\
// 	}								\
// } while (0)

// 	/* sanity checks */
// 	PCPU_SETUP_BUG_ON(ai->nr_groups <= 0);
// #ifdef CONFIG_SMP
// 	PCPU_SETUP_BUG_ON(!ai->static_size);
// 	PCPU_SETUP_BUG_ON(offset_in_page(__per_cpu_start));
// #endif
// 	PCPU_SETUP_BUG_ON(!base_addr);
// 	PCPU_SETUP_BUG_ON(offset_in_page(base_addr));
// 	PCPU_SETUP_BUG_ON(ai->unit_size < size_sum);
// 	PCPU_SETUP_BUG_ON(offset_in_page(ai->unit_size));
// 	PCPU_SETUP_BUG_ON(ai->unit_size < PCPU_MIN_UNIT_SIZE);
// 	PCPU_SETUP_BUG_ON(!IS_ALIGNED(ai->unit_size, PCPU_BITMAP_BLOCK_SIZE));
// 	PCPU_SETUP_BUG_ON(ai->dyn_size < PERCPU_DYNAMIC_EARLY_SIZE);
// 	PCPU_SETUP_BUG_ON(!ai->dyn_size);
// 	PCPU_SETUP_BUG_ON(!IS_ALIGNED(ai->reserved_size, PCPU_MIN_ALLOC_SIZE));
// 	PCPU_SETUP_BUG_ON(!(IS_ALIGNED(PCPU_BITMAP_BLOCK_SIZE, PAGE_SIZE) ||
// 			    IS_ALIGNED(PAGE_SIZE, PCPU_BITMAP_BLOCK_SIZE)));
// 	PCPU_SETUP_BUG_ON(pcpu_verify_alloc_info(ai) < 0);

// 	/* process group information and build config tables accordingly */
// 	alloc_size = ai->nr_groups * sizeof(group_offsets[0]);
// 	group_offsets = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!group_offsets)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size)void __init pcpu_setup_first_chunk(const struct pcpu_alloc_info *ai,
// 				   void *base_addr)
// {
// 	size_t size_sum = ai->static_size + ai->reserved_size + ai->dyn_size;
// 	size_t static_size, dyn_size;
// 	struct pcpu_chunk *chunk;
// 	unsigned long *group_offsets;
// 	size_t *group_sizes;
// 	unsigned long *unit_off;
// 	unsigned int cpu;
// 	int *unit_map;
// 	int group, unit, i;
// 	int map_size;
// 	unsigned long tmp_addr;
// 	size_t alloc_size;

// #define PCPU_SETUP_BUG_ON(cond)	do {					\
// 	if (unlikely(cond)) {						\
// 		pr_emerg("failed to initialize, %s\n", #cond);		\
// 		pr_emerg("cpu_possible_mask=%*pb\n",			\
// 			 cpumask_pr_args(cpu_possible_mask));		\
// 		pcpu_dump_alloc_info(KERN_EMERG, ai);			\
// 		BUG();							\
// 	}								\
// } while (0)

// 	/* sanity checks */
// 	PCPU_SETUP_BUG_ON(ai->nr_groups <= 0);
// #ifdef CONFIG_SMP
// 	PCPU_SETUP_BUG_ON(!ai->static_size);
// 	PCPU_SETUP_BUG_ON(offset_in_page(__per_cpu_start));
// #endif
// 	PCPU_SETUP_BUG_ON(!base_addr);
// 	PCPU_SETUP_BUG_ON(offset_in_page(base_addr));
// 	PCPU_SETUP_BUG_ON(ai->unit_size < size_sum);
// 	PCPU_SETUP_BUG_ON(offset_in_page(ai->unit_size));
// 	PCPU_SETUP_BUG_ON(ai->unit_size < PCPU_MIN_UNIT_SIZE);
// 	PCPU_SETUP_BUG_ON(!IS_ALIGNED(ai->unit_size, PCPU_BITMAP_BLOCK_SIZE));
// 	PCPU_SETUP_BUG_ON(ai->dyn_size < PERCPU_DYNAMIC_EARLY_SIZE);
// 	PCPU_SETUP_BUG_ON(!ai->dyn_size);
// 	PCPU_SETUP_BUG_ON(!IS_ALIGNED(ai->reserved_size, PCPU_MIN_ALLOC_SIZE));
// 	PCPU_SETUP_BUG_ON(!(IS_ALIGNED(PCPU_BITMAP_BLOCK_SIZE, PAGE_SIZE) ||
// 			    IS_ALIGNED(PAGE_SIZE, PCPU_BITMAP_BLOCK_SIZE)));
// 	PCPU_SETUP_BUG_ON(pcpu_verify_alloc_info(ai) < 0);

// 	/* process group information and build config tables accordingly */
// 	alloc_size = ai->nr_groups * sizeof(group_offsets[0]);
// 	group_offsets = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!group_offsets)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	alloc_size = ai->nr_groups * sizeof(group_sizes[0]);
// 	group_sizes = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!group_sizes)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	alloc_size = nr_cpu_ids * sizeof(unit_map[0]);
// 	unit_map = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!unit_map)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	alloc_size = nr_cpu_ids * sizeof(unit_off[0]);
// 	unit_off = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!unit_off)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	for (cpu = 0; cpu < nr_cpu_ids; cpu++)
// 		unit_map[cpu] = UINT_MAX;

// 	pcpu_low_unit_cpu = NR_CPUS;
// 	pcpu_high_unit_cpu = NR_CPUS;

// 	for (group = 0, unit = 0; group < ai->nr_groups; group++, unit += i) {
// 		const struct pcpu_group_info *gi = &ai->groups[group];

// 		group_offsets[group] = gi->base_offset;
// 		group_sizes[group] = gi->nr_units * ai->unit_size;

// 		for (i = 0; i < gi->nr_units; i++) {
// 			cpu = gi->cpu_map[i];
// 			if (cpu == NR_CPUS)
// 				continue;

// 			PCPU_SETUP_BUG_ON(cpu >= nr_cpu_ids);
// 			PCPU_SETUP_BUG_ON(!cpu_possible(cpu));
// 			PCPU_SETUP_BUG_ON(unit_map[cpu] != UINT_MAX);

// 			unit_map[cpu] = unit + i;
// 			unit_off[cpu] = gi->base_offset + i * ai->unit_size;

// 			/* determine low/high unit_cpu */
// 			if (pcpu_low_unit_cpu == NR_CPUS ||
// 			    unit_off[cpu] < unit_off[pcpu_low_unit_cpu])
// 				pcpu_low_unit_cpu = cpu;
// 			if (pcpu_high_unit_cpu == NR_CPUS ||
// 			    unit_off[cpu] > unit_off[pcpu_high_unit_cpu])
// 				pcpu_high_unit_cpu = cpu;
// 		}
// 	}
// 	pcpu_nr_units = unit;

// 	for_each_possible_cpu(cpu)
// 		PCPU_SETUP_BUG_ON(unit_map[cpu] == UINT_MAX);

// 	/* we're done parsing the input, undefine BUG macro and dump config */
// #undef PCPU_SETUP_BUG_ON
// 	pcpu_dump_alloc_info(KERN_DEBUG, ai);

// 	pcpu_nr_groups = ai->nr_groups;
// 	pcpu_group_offsets = group_offsets;
// 	pcpu_group_sizes = group_sizes;
// 	pcpu_unit_map = unit_map;
// 	pcpu_unit_offsets = unit_off;

// 	/* determine basic parameters */
// 	pcpu_unit_pages = ai->unit_size >> PAGE_SHIFT;
// 	pcpu_unit_size = pcpu_unit_pages << PAGE_SHIFT;
// 	pcpu_atom_size = ai->atom_size;
// 	pcpu_chunk_struct_size = struct_size(chunk, populated,
// 					     BITS_TO_LONGS(pcpu_unit_pages));

// 	pcpu_stats_save_ai(ai);

// 	/*
// 	 * Allocate chunk slots.  The slots after the active slots are:
// 	 *   sidelined_slot - isolated, depopulated chunks
// 	 *   free_slot - fully free chunks
// 	 *   to_depopulate_slot - isolated, chunks to depopulate
// 	 */
// 	pcpu_sidelined_slot = __pcpu_size_to_slot(pcpu_unit_size) + 1;
// 	pcpu_free_slot = pcpu_sidelined_slot + 1;
// 	pcpu_to_depopulate_slot = pcpu_free_slot + 1;
// 	pcpu_nr_slots = pcpu_to_depopulate_slot + 1;
// 	pcpu_chunk_lists = memblock_alloc(pcpu_nr_slots *
// 					  sizeof(pcpu_chunk_lists[0]),
// 					  SMP_CACHE_BYTES);
// 	if (!pcpu_chunk_lists)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      pcpu_nr_slots * sizeof(pcpu_chunk_lists[0]));

// 	for (i = 0; i < pcpu_nr_slots; i++)
// 		INIT_LIST_HEAD(&pcpu_chunk_lists[i]);

// 	/*
// 	 * The end of the static region needs to be aligned with the
// 	 * minimum allocation size as this offsets the reserved and
// 	 * dynamic region.  The first chunk ends page aligned by
// 	 * expanding the dynamic region, therefore the dynamic region
// 	 * can be shrunk to compensate while still staying above the
// 	 * configured sizes.
// 	 */
// 	static_size = ALIGN(ai->static_size, PCPU_MIN_ALLOC_SIZE);
// 	dyn_size = ai->dyn_size - (static_size - ai->static_size);

// 	/*
// 	 * Initialize first chunk.
// 	 * If the reserved_size is non-zero, this initializes the reserved
// 	 * chunk.  If the reserved_size is zero, the reserved chunk is NULL
// 	 * and the dynamic region is initialized here.  The first chunk,
// 	 * pcpu_first_chunk, will always point to the chunk that serves
// 	 * the dynamic region.
// 	 */
// 	tmp_addr = (unsigned long)base_addr + static_size;
// 	map_size = ai->reserved_size ?: dyn_size;
// 	chunk = pcpu_alloc_first_chunk(tmp_addr, map_size);

// 	/* init dynamic chunk if necessary */
// 	if (ai->reserved_size) {
// 		pcpu_reserved_chunk = chunk;

// 		tmp_addr = (unsigned long)base_addr + static_size +
// 			   ai->reserved_size;
// 		map_size = dyn_size;
// 		chunk = pcpu_alloc_first_chunk(tmp_addr, map_size);
// 	}

// 	/* link the first chunk in */
// 	pcpu_first_chunk = chunk;
// 	pcpu_nr_empty_pop_pages = pcpu_first_chunk->nr_empty_pop_pages;
// 	pcpu_chunk_relocate(pcpu_first_chunk, -1);

// 	/* include all regions of the first chunk */
// 	pcpu_nr_populated += PFN_DOWN(size_sum);

// 	pcpu_stats_chunk_alloc();
// 	trace_percpu_create_chunk(base_addr);

// 	/* we're done */
// 	pcpu_base_addr = base_addr;
// };

// 	alloc_size = ai->nr_groups * sizeof(group_sizes[0]);
// 	group_sizes = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!group_sizes)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	alloc_size = nr_cpu_ids * sizeof(unit_map[0]);
// 	unit_map = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!unit_map)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	alloc_size = nr_cpu_ids * sizeof(unit_off[0]);
// 	unit_off = memblock_alloc(alloc_size, SMP_CACHE_BYTES);
// 	if (!unit_off)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      alloc_size);

// 	for (cpu = 0; cpu < nr_cpu_ids; cpu++)
// 		unit_map[cpu] = UINT_MAX;

// 	pcpu_low_unit_cpu = NR_CPUS;
// 	pcpu_high_unit_cpu = NR_CPUS;

// 	for (group = 0, unit = 0; group < ai->nr_groups; group++, unit += i) {
// 		const struct pcpu_group_info *gi = &ai->groups[group];

// 		group_offsets[group] = gi->base_offset;
// 		group_sizes[group] = gi->nr_units * ai->unit_size;

// 		for (i = 0; i < gi->nr_units; i++) {
// 			cpu = gi->cpu_map[i];
// 			if (cpu == NR_CPUS)
// 				continue;

// 			PCPU_SETUP_BUG_ON(cpu >= nr_cpu_ids);
// 			PCPU_SETUP_BUG_ON(!cpu_possible(cpu));
// 			PCPU_SETUP_BUG_ON(unit_map[cpu] != UINT_MAX);

// 			unit_map[cpu] = unit + i;
// 			unit_off[cpu] = gi->base_offset + i * ai->unit_size;

// 			/* determine low/high unit_cpu */
// 			if (pcpu_low_unit_cpu == NR_CPUS ||
// 			    unit_off[cpu] < unit_off[pcpu_low_unit_cpu])
// 				pcpu_low_unit_cpu = cpu;
// 			if (pcpu_high_unit_cpu == NR_CPUS ||
// 			    unit_off[cpu] > unit_off[pcpu_high_unit_cpu])
// 				pcpu_high_unit_cpu = cpu;
// 		}
// 	}
// 	pcpu_nr_units = unit;

// 	for_each_possible_cpu(cpu)
// 		PCPU_SETUP_BUG_ON(unit_map[cpu] == UINT_MAX);

// 	/* we're done parsing the input, undefine BUG macro and dump config */
// #undef PCPU_SETUP_BUG_ON
// 	pcpu_dump_alloc_info(KERN_DEBUG, ai);

// 	pcpu_nr_groups = ai->nr_groups;
// 	pcpu_group_offsets = group_offsets;
// 	pcpu_group_sizes = group_sizes;
// 	pcpu_unit_map = unit_map;
// 	pcpu_unit_offsets = unit_off;

// 	/* determine basic parameters */
// 	pcpu_unit_pages = ai->unit_size >> PAGE_SHIFT;
// 	pcpu_unit_size = pcpu_unit_pages << PAGE_SHIFT;
// 	pcpu_atom_size = ai->atom_size;
// 	pcpu_chunk_struct_size = struct_size(chunk, populated,
// 					     BITS_TO_LONGS(pcpu_unit_pages));

// 	pcpu_stats_save_ai(ai);

// 	/*
// 	 * Allocate chunk slots.  The slots after the active slots are:
// 	 *   sidelined_slot - isolated, depopulated chunks
// 	 *   free_slot - fully free chunks
// 	 *   to_depopulate_slot - isolated, chunks to depopulate
// 	 */
// 	pcpu_sidelined_slot = __pcpu_size_to_slot(pcpu_unit_size) + 1;
// 	pcpu_free_slot = pcpu_sidelined_slot + 1;
// 	pcpu_to_depopulate_slot = pcpu_free_slot + 1;
// 	pcpu_nr_slots = pcpu_to_depopulate_slot + 1;
// 	pcpu_chunk_lists = memblock_alloc(pcpu_nr_slots *
// 					  sizeof(pcpu_chunk_lists[0]),
// 					  SMP_CACHE_BYTES);
// 	if (!pcpu_chunk_lists)
// 		panic("%s: Failed to allocate %zu bytes\n", __func__,
// 		      pcpu_nr_slots * sizeof(pcpu_chunk_lists[0]));

// 	for (i = 0; i < pcpu_nr_slots; i++)
// 		INIT_LIST_HEAD(&pcpu_chunk_lists[i]);

// 	/*
// 	 * The end of the static region needs to be aligned with the
// 	 * minimum allocation size as this offsets the reserved and
// 	 * dynamic region.  The first chunk ends page aligned by
// 	 * expanding the dynamic region, therefore the dynamic region
// 	 * can be shrunk to compensate while still staying above the
// 	 * configured sizes.
// 	 */
// 	static_size = ALIGN(ai->static_size, PCPU_MIN_ALLOC_SIZE);
// 	dyn_size = ai->dyn_size - (static_size - ai->static_size);

// 	/*
// 	 * Initialize first chunk.
// 	 * If the reserved_size is non-zero, this initializes the reserved
// 	 * chunk.  If the reserved_size is zero, the reserved chunk is NULL
// 	 * and the dynamic region is initialized here.  The first chunk,
// 	 * pcpu_first_chunk, will always point to the chunk that serves
// 	 * the dynamic region.
// 	 */
// 	tmp_addr = (unsigned long)base_addr + static_size;
// 	map_size = ai->reserved_size ?: dyn_size;
// 	chunk = pcpu_alloc_first_chunk(tmp_addr, map_size);

// 	/* init dynamic chunk if necessary */
// 	if (ai->reserved_size) {
// 		pcpu_reserved_chunk = chunk;

// 		tmp_addr = (unsigned long)base_addr + static_size +
// 			   ai->reserved_size;
// 		map_size = dyn_size;
// 		chunk = pcpu_alloc_first_chunk(tmp_addr, map_size);
// 	}

// 	/* link the first chunk in */
// 	pcpu_first_chunk = chunk;
// 	pcpu_nr_empty_pop_pages = pcpu_first_chunk->nr_empty_pop_pages;
// 	pcpu_chunk_relocate(pcpu_first_chunk, -1);

// 	/* include all regions of the first chunk */
// 	pcpu_nr_populated += PFN_DOWN(size_sum);

// 	pcpu_stats_chunk_alloc();
// 	trace_percpu_create_chunk(base_addr);

// 	/* we're done */
// 	pcpu_base_addr = base_addr;
// }
void simple_pcpu_setup_first_chunk()
{
	unsigned long pcpuarea_size =
		(unsigned long)__per_cpu_end - (unsigned long)__per_cpu_load;
	pcpuarea_size = ALIGN(pcpuarea_size, PAGE_SIZE);

	pcpu_unit_offsets = myos_memblock_alloc_normal(nr_cpu_ids * 8, 8);
	pcpu_base_addr = __per_cpu_start;
	// 不为bsp的percpu变量重新申请空间，直接使用初始声明的
	for (int i = 0; i < nr_cpu_ids; i++)
		((unsigned long *)pcpu_unit_offsets)[i] =
			(unsigned long)myos_memblock_alloc_DMA32(pcpuarea_size, PAGE_SIZE) -
				(unsigned long)__per_cpu_load;
}