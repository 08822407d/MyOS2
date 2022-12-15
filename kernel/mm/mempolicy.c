// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple NUMA memory policy for the Linux kernel.
 *
 * Copyright 2003,2004 Andi Kleen, SuSE Labs.
 * (C) Copyright 2005 Christoph Lameter, Silicon Graphics, Inc.
 *
 * NUMA policy allows the user to give hints in which node(s) memory should
 * be allocated.
 *
 * Support four policies per VMA and per process:
 *
 * The VMA policy has priority over the process policy for a page fault.
 *
 * interleave     Allocate memory interleaved over a set of nodes,
 *                with normal fallback if it fails.
 *                For VMA based allocations this interleaves based on the
 *                offset into the backing object or offset into the mapping
 *                for anonymous memory. For process policy an process counter
 *                is used.
 *
 * bind           Only allocate memory on a specific set of nodes,
 *                no fallback.
 *                FIXME: memory is allocated starting with the first node
 *                to the last. It would be better if bind would truly restrict
 *                the allocation to memory nodes instead
 *
 * preferred       Try a specific node first before normal fallback.
 *                As a special case NUMA_NO_NODE here means do the allocation
 *                on the local CPU. This is normally identical to default,
 *                but useful to set in a VMA when you have a non default
 *                process policy.
 *
 * preferred many Try a set of nodes first before normal fallback. This is
 *                similar to preferred without the special case.
 *
 * default        Allocate on the local node first, or when on a VMA
 *                use the process policy. This is what Linux always did
 *		  in a NUMA aware kernel and still does by, ahem, default.
 *
 * The process policy is applied for most non interrupt memory allocations
 * in that process' context. Interrupts ignore the policies and always
 * try to allocate on the local CPU. The VMA policy is only applied for memory
 * allocations for a VMA in the VM.
 *
 * Currently there are a few corner cases in swapping where the policy
 * is not applied, but the majority should be handled. When process policy
 * is used it is not remembered over swap outs/swap ins.
 *
 * Only the highest zone in the zone hierarchy gets policied. Allocations
 * requesting a lower zone just use default policy. This implies that
 * on systems with highmem kernel lowmem allocation don't get policied.
 * Same with GFP_DMA allocations.
 *
 * For shmfs/tmpfs/hugetlbfs shared memory the policy is shared between
 * all users and remembered even when nobody has memory mapped.
 */

/* Notebook:
   fix mmap readahead to honour policy and enable policy for any page cache
   object
   statistics for bigpages
   global policy for page cache? currently it uses process policy. Requires
   first item above.
   handle mremap for shared memory (currently ignored for the policy)
   grows down?
   make bind policy root only? It can trigger oom much faster and the
   kernel is not always grateful with that.
*/

// #include <linux/mempolicy.h>
// #include <linux/pagewalk.h>
// #include <linux/highmem.h>
// #include <linux/hugetlb.h>
#include <linux/kernel/kernel.h>
#include <linux/kernel/sched.h>
#include <linux/sched/mm.h>
// #include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
// #include <linux/nodemask.h>
// #include <linux/cpuset.h>
#include <linux/kernel/slab.h>
#include <linux/lib/string.h>
// #include <linux/export.h>
// #include <linux/nsproxy.h>
// #include <linux/interrupt.h>
#include <linux/init/init.h>
// #include <linux/compat.h>
// #include <linux/ptrace.h>
// #include <linux/swap.h>
// #include <linux/seq_file.h>
// #include <linux/proc_fs.h>
// #include <linux/migrate.h>
// #include <linux/ksm.h>
// #include <linux/rmap.h>
// #include <linux/security.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/ctype.h>
// #include <linux/mm_inline.h>
// #include <linux/mmu_notifier.h>
// #include <linux/printk.h>
// #include <linux/swapops.h>

// #include <asm/tlbflush.h>
// #include <linux/uaccess.h>

#include "internal.h"


/**
 * alloc_pages - Allocate pages.
 * @gfp: GFP flags.
 * @order: Power of two of number of pages to allocate.
 *
 * Allocate 1 << @order contiguous pages.  The physical address of the
 * first page is naturally aligned (eg an order-3 allocation will be aligned
 * to a multiple of 8 * PAGE_SIZE bytes).  The NUMA policy of the current
 * process is honoured when in process context.
 *
 * Context: Can be called from any context, providing the appropriate GFP
 * flags are used.
 * Return: The page on success or NULL if allocation fails.
 */
page_s *alloc_pages(gfp_t gfp, unsigned order)
{
	// struct mempolicy *pol = &default_policy;
	page_s *page;

	// if (!in_interrupt() && !(gfp & __GFP_THISNODE))
	// 	pol = get_task_policy(current);

	// /*
	//  * No reference counting needed for current->mempolicy
	//  * nor system default_policy
	//  */
	// if (pol->mode == MPOL_INTERLEAVE)
	// 	page = alloc_page_interleave(gfp, order, interleave_nodes(pol));
	// else if (pol->mode == MPOL_PREFERRED_MANY)
	// 	page = alloc_pages_preferred_many(gfp, order,
	// 			  policy_node(gfp, pol, numa_node_id()), pol);
	// else
	// 	page = __alloc_pages(gfp, order,
	// 			policy_node(gfp, pol, numa_node_id()),
	// 			policy_nodemask(gfp, pol));

	page = __alloc_pages(gfp, order);

	return page;
}