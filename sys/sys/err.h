/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ERR_H_
#define _LINUX_ERR_H_

#include <stddef.h>
#include <stdbool.h>
#include <errno.h>

	/*
	* Kernel pointers have redundant information, so we can use a
	* scheme where we can return either an error code or a normal
	* pointer with the same return value.
	*
	* This should be a per-architecture thing, to allow different
	* error and pointer decisions.
	*/
	#define MAX_ERRNO	4095

	#ifndef __ASSEMBLY__

		#define IS_ERR_VALUE(x) ((size_t)(void *)(x) >= (size_t)-MAX_ERRNO)

		static inline void * ERR_PTR(long error)
		{
			return (void *) error;
		}

		static inline long PTR_ERR(const void *ptr)
		{
			return (long) ptr;
		}

		static inline bool IS_ERR(const void *ptr)
		{
			return IS_ERR_VALUE((size_t)ptr);
		}

		static inline bool IS_ERR_OR_NULL(const void *ptr)
		{
			return (!ptr) || IS_ERR_VALUE((size_t)ptr);
		}

		/**
		 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
		 * @ptr: The pointer to cast.
		 *
		 * Explicitly cast an error-valued pointer to another pointer type in such a
		 * way as to make it clear that's what's going on.
		 */
		static inline void * ERR_CAST(const void *ptr)
		{
			/* cast away the const */
			return (void *) ptr;
		}

		static inline int PTR_ERR_OR_ZERO(const void *ptr)
		{
			if (IS_ERR(ptr))
				return PTR_ERR(ptr);
			else
				return 0;
		}

	#endif

#endif /* _LINUX_ERR_H_ */