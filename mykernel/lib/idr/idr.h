/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_H_
#define _LINUX_IDR_H_

	#include <linux/compiler/myos_debug_option.h>

	#include "../lib_const.h"
	#include "../lib_types.h"
	#include "../lib_api.h"


	#ifdef DEBUG

		extern void
		idr_init_base(idr_s *idr, int base);
		extern void
		idr_init(idr_s *idr);
		extern uint
		idr_get_cursor(const idr_s *idr);
		extern void
		idr_set_cursor(idr_s *idr, uint val);

		extern int
		ida_alloc(ida_s *ida, gfp_t gfp);
		extern int
		ida_alloc_min(ida_s *ida, uint min, gfp_t gfp);
		extern int
		ida_alloc_max(ida_s *ida, uint max, gfp_t gfp);
		extern void
		ida_init(ida_s *ida);

	#endif

	#include "idr_macro.h"

	extern void *kzalloc(size_t size, gfp_t flags);
	
	#if defined(IDR_DEFINATION) || !(DEBUG)

		/**
		 * idr_init_base() - Initialise an IDR.
		 * @idr: IDR handle.
		 * @base: The base value for the IDR.
		 *
		 * This variation of idr_init() creates an IDR which will allocate IDs
		 * starting at %base.
		 */
		PREFIX_STATIC_INLINE
		void
		idr_init_base(idr_s *idr, int base) {
			// INIT_RADIX_TREE(&idr->idr_rt, IDR_RT_MARKER);
			idr->idr_rt = kzalloc(sizeof(void *) * MYOS_IDR_BUF_SIZE, GFP_KERNEL);
			idr->idr_base = base;
			idr->idr_next = 0;

			idr->initiated = true;
		}

		/**
		 * idr_init() - Initialise an IDR.
		 * @idr: IDR handle.
		 *
		 * Initialise a dynamically allocated IDR.  To initialise a
		 * statically allocated IDR, use DEFINE_IDR().
		 */
		PREFIX_STATIC_INLINE
		void
		idr_init(idr_s *idr) {
			idr_init_base(idr, 0);
		}

		/**
		 * idr_get_cursor - Return the current position of the cyclic allocator
		 * @idr: idr handle
		 *
		 * The value returned is the value that will be next returned from
		 * idr_alloc_cyclic() if it is free (otherwise the search will start from
		 * this position).
		 */
		PREFIX_STATIC_INLINE
		uint
		idr_get_cursor(const idr_s *idr) {
			return READ_ONCE(idr->idr_next);
		}

		/**
		 * idr_set_cursor - Set the current position of the cyclic allocator
		 * @idr: idr handle
		 * @val: new position
		 *
		 * The next call to idr_alloc_cyclic() will return @val if it is free
		 * (otherwise the search will start from this position).
		 */
		PREFIX_STATIC_INLINE
		void
		idr_set_cursor(idr_s *idr, uint val) {
			WRITE_ONCE(idr->idr_next, val);
		}


		/**
		 * ida_alloc() - Allocate an unused ID.
		 * @ida: IDA handle.
		 * @gfp: Memory allocation flags.
		 *
		 * Allocate an ID between 0 and %INT_MAX, inclusive.
		 *
		 * Context: Any context. It is safe to call this function without
		 * locking in your code.
		 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
		 * or %-ENOSPC if there are no free IDs.
		 */
		PREFIX_STATIC_INLINE
		int
		ida_alloc(ida_s *ida, gfp_t gfp) {
			return ida_alloc_range(ida, 0, ~0, gfp);
		}

		/**
		 * ida_alloc_min() - Allocate an unused ID.
		 * @ida: IDA handle.
		 * @min: Lowest ID to allocate.
		 * @gfp: Memory allocation flags.
		 *
		 * Allocate an ID between @min and %INT_MAX, inclusive.
		 *
		 * Context: Any context. It is safe to call this function without
		 * locking in your code.
		 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
		 * or %-ENOSPC if there are no free IDs.
		 */
		PREFIX_STATIC_INLINE
		int
		ida_alloc_min(ida_s *ida, uint min, gfp_t gfp) {
			return ida_alloc_range(ida, min, ~0, gfp);
		}

		/**
		 * ida_alloc_max() - Allocate an unused ID.
		 * @ida: IDA handle.
		 * @max: Highest ID to allocate.
		 * @gfp: Memory allocation flags.
		 *
		 * Allocate an ID between 0 and @max, inclusive.
		 *
		 * Context: Any context. It is safe to call this function without
		 * locking in your code.
		 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
		 * or %-ENOSPC if there are no free IDs.
		 */
		PREFIX_STATIC_INLINE
		int
		ida_alloc_max(ida_s *ida, uint max, gfp_t gfp) {
			return ida_alloc_range(ida, 0, max, gfp);
		}

		PREFIX_STATIC_INLINE
		void
		ida_init(ida_s *ida) {
			ida->bitmap = kzalloc(IDA_BITMAP_LONGS * sizeof(long), GFP_KERNEL);

			ida->initiated = true;
		}

	#endif

#endif /* _LINUX_IDR_H_ */