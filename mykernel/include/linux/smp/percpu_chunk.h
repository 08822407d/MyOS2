// source: linux-6.11

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERCPU_CHUNK_H_
#define _PERCPU_CHUNK_H_

#include <linux/compiler/compiler.h>
#include <linux/kernel/cache.h>
#include <linux/kernel/types.h>
#include <linux/kernel/lib_api.h>


	struct pcpu_chunk {
		int			free_offset;

	// #ifdef CONFIG_PERCPU_STATS
	// 	int			nr_alloc;	/* # of allocations */
	// 	size_t		max_alloc_size; /* largest allocation size */
	// #endif

		List_s		list;				/* linked to pcpu_slot lists */
		int			free_bytes;			/* free bytes in the chunk */
		// struct pcpu_block_md	chunk_md;
		// ulong		*bound_map;			/* boundary map */

		/*
		 * base_addr is the base address of this chunk.
		 * To reduce false sharing, current layout is optimized to make sure
		 * base_addr locate in the different cacheline with free_bytes and
		 * chunk_md.
		 */
		void		*base_addr ____cacheline_aligned_in_smp;

		// ulong		*alloc_map;			/* allocation map */
		// struct pcpu_block_md	*md_blocks;	/* metadata blocks */

		void		*data;				/* chunk data */
		// bool		immutable;			/* no [de]population allowed */
		// bool		isolated;			/* isolated from active chunk
		// 								   slots */
		// int			start_offset;		/* the overlap with the previous
		// 								   region to have a page aligned
		// 								   base_addr */
		// int			end_offset;			/* additional area required to
		// 								   have the region end page
		// 								   aligned */
	// #ifdef NEED_PCPUOBJ_EXT
	// 	struct pcpuobj_ext	*obj_exts;	/* vector of object cgroups */
	// #endif

		// int			nr_pages;			/* # of pages served by this chunk */
		// int			nr_populated;		/* # of populated pages */
		// int			nr_empty_pop_pages;	/* # of empty populated pages */
		// ulong		populated[];		/* populated bitmap */
	};

	struct pcpu_chunk;
	typedef struct pcpu_chunk pcpu_chunk_s;

#endif /* _PERCPU_CHUNK_H_ */
