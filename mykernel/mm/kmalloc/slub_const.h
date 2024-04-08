/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SLUB_CONST_H_
#define _SLUB_CONST_H_

	/*
	 * Minimum number of partial slabs. These will be left on the partial
	 * lists even if they are empty. kmem_cache_shrink may reclaim them.
	 */
	#define MIN_PARTIAL 4
	/*
	 * Maximum number of desirable partial slabs.
	 * The existence of more partial slabs makes kmem_cache_shrink
	 * sort the partial list by the number of objects in use.
	 */
	#define MAX_PARTIAL 8

	#define MAX_OBJS_PER_PAGE	32767 /* since slab.objects is u15 */

#endif /* _SLUB_CONST_H_ */
