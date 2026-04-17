// source: linux-6.4.9

/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Logical memory blocks.
 *
 * Copyright (C) 2001 Peter Bergner, IBM Corp.
 */
#ifndef _LINUX_MEMBLOCK_H_
#define _LINUX_MEMBLOCK_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../mm_const.h"
	#include "../mm_types.h"
	#include "../mm_api.h"


	#ifdef DEBUG

		extern int __init_memblock
		memblock_search(mmblk_type_s *type, phys_addr_t addr);

		extern bool __init_memblock
		memblock_is_reserved(phys_addr_t addr);
		extern bool __init_memblock
		memblock_is_memory(phys_addr_t addr);
		extern bool
		memblock_is_hotpluggable(mmblk_rgn_s *m);
		extern bool
		memblock_is_mirror(mmblk_rgn_s *m);
		extern bool
		memblock_is_nomap(mmblk_rgn_s *m);
		extern bool
		memblock_is_driver_managed(mmblk_rgn_s *m);
		extern bool __init_memblock
		memblock_is_map_memory(phys_addr_t addr);
		
		extern __init_memblock bool
		memblock_bottom_up(void);

	#endif

	#include "memblock_macro.h"

	#if defined(MEMBLOCK_DEFINATION) || !(DEBUG)

		PREFIX_STATIC_INLINE
		int __init_memblock
		memblock_search(mmblk_type_s *type, phys_addr_t addr) {
			uint left = 0, right = type->cnt;

			do {
				uint mid = (right + left) / 2;

				if (addr < type->regions[mid].base)
					right = mid;
				else if (addr >= (type->regions[mid].base +
						type->regions[mid].size))
					left = mid + 1;
				else
					return mid;
			} while (left < right);
			return -1;
		}

		PREFIX_STATIC_INLINE
		bool __init_memblock
		memblock_is_reserved(phys_addr_t addr) {
			return memblock_search(&memblock.reserved, addr) != -1;
		}

		PREFIX_STATIC_INLINE
		bool __init_memblock
		memblock_is_memory(phys_addr_t addr) {
			return memblock_search(&memblock.memory, addr) != -1;
		}
		PREFIX_STATIC_INLINE
		bool
		memblock_is_hotpluggable(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_HOTPLUG;
		}
		PREFIX_STATIC_INLINE
		bool
		memblock_is_mirror(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_MIRROR;
		}
		PREFIX_STATIC_INLINE
		bool
		memblock_is_nomap(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_NOMAP;
		}
		PREFIX_STATIC_INLINE
		bool
		memblock_is_driver_managed(IN mmblk_rgn_s *m) {
			return m->flags & MEMBLOCK_DRIVER_MANAGED;
		}
		PREFIX_STATIC_INLINE
		bool __init_memblock
		memblock_is_map_memory(phys_addr_t addr) {
			int i = memblock_search(&memblock.memory, addr);

			if (i == -1)
				return false;
			return !memblock_is_nomap(&memblock.memory.regions[i]);
		}


		/*
		 * Check if the allocation direction is bottom-up or not.
		 * if this is true, that said, memblock will allocate memory
		 * in bottom-up direction.
		 */
		PREFIX_STATIC_INLINE
		__init_memblock bool
		memblock_bottom_up(void) {
			return memblock.bottom_up;
		}

	#endif

#endif /* _LINUX_MEMBLOCK_H_ */