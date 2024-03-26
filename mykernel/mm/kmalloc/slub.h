/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Written by Mark Hemment, 1996 (markhe@nextd.demon.co.uk).
 *
 * (C) SGI 2006, Christoph Lameter
 * 	Cleaned up and restructured to ease the addition of alternative
 * 	implementations of SLAB allocators.
 * (C) Linux Foundation 2008-2013
 *      Unified interface for all slab allocators
 */

#ifndef _LINUX_SLUB_H_
#define	_LINUX_SLUB_H_

	#include <linux/kernel/types.h>
	#include <linux/compiler/compiler_types.h>

	#include "slub_const.h"
	#include "slub_types.h"

	/**
	 * kmem_cache_alloc - Allocate an object
	 * @cachep: The cache to allocate from.
	 * @flags: See kmalloc().
	 *
	 * Allocate an object from this cache.
	 * See kmem_cache_zalloc() for a shortcut of adding __GFP_ZERO to flags.
	 *
	 * Return: pointer to the new object or %NULL in case of error
	 */
	void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags) __malloc;
	// void *kmem_cache_alloc_lru(struct kmem_cache *s, struct list_lru *lru,
	// 			gfp_t gfpflags) __assume_slab_alignment __malloc;
	void kmem_cache_free(struct kmem_cache *s, void *objp);


	/*
	 * Shortcuts
	 */
	static inline void
	*kmem_cache_zalloc(struct kmem_cache *k, gfp_t flags) {
		return kmem_cache_alloc(k, flags | __GFP_ZERO);
	}

#endif	/* _LINUX_SLUB_H_ */