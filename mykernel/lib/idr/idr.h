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

	#endif

	#include "idr_macro.h"
	
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
			memset(idr->idr_rt, 0, sizeof(idr->idr_rt));
			idr->idr_base = base;
			idr->idr_next = 0;
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

	#endif

#endif /* _LINUX_IDR_H_ */