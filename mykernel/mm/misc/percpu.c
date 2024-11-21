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

#include <linux/smp/percpu.h>

#include "mm_misc.h"


#define PCPU_GFP(gfp)	((gfp) & (GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN))

// static int pcpu_unit_pages __ro_after_init;
// static int pcpu_unit_size __ro_after_init;
// static int pcpu_nr_units __ro_after_init;
// static int pcpu_atom_size __ro_after_init;
// int pcpu_nr_slots __ro_after_init;
// static int pcpu_free_slot __ro_after_init;
// int pcpu_sidelined_slot __ro_after_init;
// int pcpu_to_depopulate_slot __ro_after_init;
// static size_t pcpu_chunk_struct_size __ro_after_init;

size_t chunk_size = 0;

/* the address of the first chunk which starts with the kernel static area */
void *pcpu_base_addr __ro_after_init;

// static const int *pcpu_unit_map __ro_after_init;		/* cpu -> unit */
const ulong *pcpu_unit_offsets __ro_after_init;	/* cpu -> unit offset */

/*
 * The first chunk which always exists.  Note that unlike other
 * chunks, this one can be allocated and mapped in several different
 * ways and thus often doesn't live in the vmalloc area.
 */
pcpu_chunk_s *pcpu_first_chunk __ro_after_init;

/*
 * Optional reserved chunk.  This chunk reserves part of the first
 * chunk and serves it for reserved allocations.  When the reserved
 * region doesn't exist, the following variable is NULL.
 */
pcpu_chunk_s *pcpu_reserved_chunk __ro_after_init;

DEFINE_SPINLOCK(pcpu_lock);	/* all internal data structures */
// static DEFINE_MUTEX(pcpu_alloc_mutex);	/* chunk create/destroy, [de]pop, map ext */

DECLARE_LIST_HDR_S(pcpu_chunk_lists); /* chunk list slots */



// static struct pcpu_chunk *pcpu_create_chunk(gfp_t gfp)
static pcpu_chunk_s
*simple_pcpu_create_chunk(gfp_t gfp) {
	gfp = PCPU_GFP(gfp);
	pcpu_chunk_s *chunk = kzalloc(sizeof(*chunk), gfp);
	if (!chunk)
		return NULL;

	INIT_LIST_HEAD(&chunk->list);
	chunk->free_offset = 0;
	chunk->free_bytes = chunk_size;
	chunk->data = kzalloc(nr_cpu_ids * chunk_size, gfp);
	chunk->base_addr = chunk->data - (ulong)pcpu_base_addr;
	list_header_add_to_tail(&pcpu_chunk_lists, &chunk->list);

	return chunk;
}
static pcpu_chunk_s
*early_pcpu_create_chunk() {
	pcpu_chunk_s *chunk =
		myos_memblock_alloc_DMA32(sizeof(*chunk), sizeof(*chunk));
	if (!chunk)
		return NULL;

	INIT_LIST_HEAD(&chunk->list);
	chunk->free_offset = 0;
	chunk->free_bytes = chunk_size;
	chunk->data =
		myos_memblock_alloc_DMA32(nr_cpu_ids * chunk_size, chunk_size);
	list_header_add_to_tail(&pcpu_chunk_lists, &chunk->list);

	return chunk;
}
/**
 * pcpu_find_block_fit - finds the block index to start searching
 * @chunk: chunk of interest
 * @alloc_bits: size of request in allocation units
 * @align: alignment of area (max PAGE_SIZE bytes)
 * @pop_only: use populated regions only
 *
 * Given a chunk and an allocation spec, find the offset to begin searching
 * for a free region.  This iterates over the bitmap metadata blocks to
 * find an offset that will be guaranteed to fit the requirements.  It is
 * not quite first fit as if the allocation does not fit in the contig hint
 * of a block or chunk, it is skipped.  This errs on the side of caution
 * to prevent excess iteration.  Poor alignment can cause the allocator to
 * skip over blocks and chunks that have valid free areas.
 *
 * RETURNS:
 * The offset in the bitmap to begin searching.
 * -1 if no offset is found.
 */
// static int pcpu_find_block_fit(struct pcpu_chunk *chunk,
// 		int alloc_bits, size_t align, bool pop_only)
static int
simple_pcpu_find_block_fit(pcpu_chunk_s *chunk, int size, size_t align) {
	int alloc_offset = ALIGN(chunk->free_offset, align);
	int need_size = alloc_offset + size - chunk->free_offset;
	if (chunk->free_bytes < need_size)
		return -ENOMEM;
	else
		return alloc_offset;
}

/**
 * pcpu_alloc_area - allocates an area from a pcpu_chunk
 * @chunk: chunk of interest
 * @alloc_bits: size of request in allocation units
 * @align: alignment of area (max PAGE_SIZE)
 * @start: bit_off to start searching
 *
 * This function takes in a @start offset to begin searching to fit an
 * allocation of @alloc_bits with alignment @align.  It needs to scan
 * the allocation map because if it fits within the block's contig hint,
 * @start will be block->first_free. This is an attempt to fill the
 * allocation prior to breaking the contig hint.  The allocation and
 * boundary maps are updated accordingly if it confirms a valid
 * free area.
 *
 * RETURNS:
 * Allocated addr offset in @chunk on success.
 * -1 if no matching area is found.
 */
// static int pcpu_alloc_area(struct pcpu_chunk *chunk,
// 		int alloc_bits, size_t align, int start)
static int
simple_pcpu_alloc_area(pcpu_chunk_s *chunk, int size, int start)
{
	while (start < chunk->free_offset);
	
	chunk->free_offset = start + size;
	chunk->free_bytes -= size;
	return start;
}

/**
 * pcpu_alloc - the percpu allocator
 * @size: size of area to allocate in bytes
 * @align: alignment of area (max PAGE_SIZE)
 * @reserved: allocate from the reserved chunk if available
 * @gfp: allocation flags
 *
 * Allocate percpu area of @size bytes aligned at @align.  If @gfp doesn't
 * contain %GFP_KERNEL, the allocation is atomic. If @gfp has __GFP_NOWARN
 * then no warning will be triggered on invalid or failed allocation
 * requests.
 *
 * RETURNS:
 * Percpu pointer to the allocated area on success, NULL on failure.
 */
// void __percpu *pcpu_alloc_noprof(size_t size,
// 		size_t align, bool reserved, gfp_t gfp)
void __percpu *simple_pcpu_alloc_noprof(size_t size,
		size_t align, bool reserved, gfp_t gfp)
{
	gfp_t pcpu_gfp;
	pcpu_chunk_s *chunk;
	int off, cpu, ret;
	ulong flags;
	void __percpu *ptr;

	/* whitelisted flags that can be passed to the backing allocators */
	pcpu_gfp = gfp & (GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN);


	// spin_lock_irqsave(&pcpu_lock, flags);

	/* serve reserved allocations from the reserved chunk if available */
	if (reserved && pcpu_reserved_chunk) {
		chunk = pcpu_reserved_chunk;

		off = simple_pcpu_find_block_fit(chunk, size, align);
		if (off < 0)
			goto fail_unlock;

		off = simple_pcpu_alloc_area(chunk, size, off);
		if (off >= 0)
			goto area_found;

		goto fail_unlock;
	}

restart:
	/* search through normal chunks */
	list_header_for_each_container(chunk, &pcpu_chunk_lists, list) {
		off = simple_pcpu_find_block_fit(chunk, size, align);
		if (off < 0)
			continue;

		off = simple_pcpu_alloc_area(chunk, size, off);
		if (off >= 0)
			goto area_found;
	}

	// spin_unlock_irqrestore(&pcpu_lock, flags);

	/* No space left.  Create a new chunk. */
	chunk = simple_pcpu_create_chunk(pcpu_gfp);
	if (!chunk)
		goto fail;

	// spin_lock_irqsave(&pcpu_lock, flags);

	goto restart;

area_found:
	// spin_unlock_irqrestore(&pcpu_lock, flags);


	// /* clear the areas and return address relative to base address */
	// for_each_possible_cpu(cpu)
	// 	memset((void *)pcpu_chunk_addr(chunk, cpu, 0) + off, 0, size);

	// ptr = __addr_to_pcpu_ptr(chunk->base_addr + off);
	ptr = chunk->base_addr + off;
	// kmemleak_alloc_percpu(ptr, size, gfp);

	// trace_percpu_alloc_percpu(_RET_IP_, reserved, is_atomic, size, align,
	// 			  chunk->base_addr, off, ptr,
	// 			  pcpu_obj_full_size(size), gfp);

	// pcpu_memcg_post_alloc_hook(objcg, chunk, off, size);

	// pcpu_alloc_tag_alloc_hook(chunk, off, size);

	return ptr;

fail_unlock:
	// spin_unlock_irqrestore(&pcpu_lock, flags);
fail:
	// trace_percpu_alloc_percpu_fail(reserved, is_atomic, size, align);

	// if (do_warn && warn_limit) {
	// 	pr_warn("allocation failed, size=%zu align=%zu atomic=%d, %s\n",
	// 		size, align, is_atomic, err);
	// 	if (!is_atomic)
	// 		dump_stack();
	// 	if (!--warn_limit)
	// 		pr_info("limit reached, disable warning\n");
	// }

	// if (is_atomic) {
	// 	/* see the flag handling in pcpu_balance_workfn() */
	// 	pcpu_atomic_alloc_failed = true;
	// 	pcpu_schedule_balance_work();
	// } else {
	// 	mutex_unlock(&pcpu_alloc_mutex);
	// }

	// pcpu_memcg_post_alloc_hook(objcg, NULL, 0, size);

	return NULL;
}
EXPORT_SYMBOL_GPL(pcpu_alloc_noprof);




void simple_pcpu_setup_first_chunk()
{
	// 计算关键全局数据
	const size_t static_size = __per_cpu_end - __per_cpu_start;
	size_t reserved_size = 2 * PAGE_SIZE;
	size_t dyn_size = 0;
	size_t size_sum = static_size + reserved_size + dyn_size;
	chunk_size = roundup_pow_of_two(size_sum);

	
	// 创建pcpu_first_chunk
	pcpu_first_chunk = early_pcpu_create_chunk();
	pcpu_base_addr = pcpu_first_chunk->data;
	// 手工初始化 pcpu_first_chunk
	pcpu_first_chunk->free_bytes = chunk_size - size_sum;
	pcpu_first_chunk->free_offset = size_sum;
	pcpu_first_chunk->base_addr = 0;
	// 复制“模板”percpu段到运行时cpu0的percpu段
	// 主要是将静态初始化的一些percpu变量的值复制过来
	memcpy(pcpu_first_chunk->data, &__per_cpu_load, static_size);


	// 计算AP的percpu空间相对BSP的偏移量
	pcpu_unit_offsets =
		myos_memblock_alloc_normal(nr_cpu_ids * sizeof(void *), 8);
	for (int i = 0; i < nr_cpu_ids; i++)
		((ulong *)pcpu_unit_offsets)[i] = i * static_size;


	// 创建pcpu_reserved_chunk
	pcpu_reserved_chunk = early_pcpu_create_chunk();
	pcpu_reserved_chunk->base_addr =
		pcpu_reserved_chunk->data - (ulong)pcpu_base_addr;
}