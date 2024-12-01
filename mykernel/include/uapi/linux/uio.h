/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
/*
 *	Berkeley style UIO structures	-	Alan Cox 1994.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _UAPI__LINUX_UIO_H
#define _UAPI__LINUX_UIO_H

#include <linux/compiler/compiler.h>
#include <linux/kernel/types.h>


	typedef struct iovec {
		void __user     *iov_base;	/* BSD uses caddr_t (1003.1g requires void *) */
		__kernel_size_t iov_len; /* Must be size_t (1003.1g) */
	} iov_s;

	/*
	 *	UIO_MAXIOV shall be at least 16 1003.1g (5.4.1.1)
	 */
	
	#define UIO_FASTIOV	8
	#define UIO_MAXIOV	1024

	struct iov_iter {
		u8 iter_type;
		bool nofault;
		bool data_source;
		size_t iov_offset;
		/*
		* Hack alert: overlay ubuf_iovec with iovec + count, so
		* that the members resolve correctly regardless of the type
		* of iterator used. This means that you can use:
		*
		* &iter->__ubuf_iovec or iter->__iov
		*
		* interchangably for the user_backed cases, hence simplifying
		* some of the cases that need to deal with both.
		*/
		union {
			/*
			* This really should be a const, but we cannot do that without
			* also modifying any of the zero-filling iter init functions.
			* Leave it non-const for now, but it should be treated as such.
			*/
			struct iovec __ubuf_iovec;
			struct {
				union {
					/* use iter_iov() to get the current vec */
					const struct iovec *__iov;
					const struct kvec *kvec;
					const struct bio_vec *bvec;
					const struct folio_queue *folioq;
					struct xarray *xarray;
					void __user *ubuf;
				};
				size_t count;
			};
		};
		union {
			unsigned long nr_segs;
			u8 folioq_slot;
			loff_t xarray_start;
		};
	};

#endif /* _UAPI__LINUX_UIO_H */