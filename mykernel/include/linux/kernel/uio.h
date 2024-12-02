/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *	Berkeley style UIO structures	-	Alan Cox 1994.
 */
#ifndef __LINUX_UIO_H
#define __LINUX_UIO_H

#include <linux/kernel/kernel.h>
// #include <linux/thread_info.h>
#include <linux/kernel/mm_types.h>
#include <uapi/linux/uio.h>

	typedef unsigned int __bitwise iov_iter_extraction_t;

	typedef struct kvec {
		void    *iov_base; /* and that should *never* hold a userland pointer */
		size_t  iov_len;
	} kvec_s;

	enum iter_type {
		/* iter types */
		ITER_IOVEC,
		ITER_KVEC,
		ITER_BVEC,
		ITER_XARRAY,
		ITER_DISCARD,
		ITER_UBUF,
	};

	#define ITER_SOURCE	1	// == WRITE
	#define ITER_DEST	0	// == READ

	typedef struct iov_iter_state {
		size_t  iov_offset;
		size_t  count;
		ulong   nr_segs;
	} iov_iter_state_s;

	typedef struct iov_iter {
		u8		iter_type;
		// bool	copy_mc;
		// bool	nofault;
		// bool	data_source;
		// bool	user_backed;
		union {
			size_t	iov_offset;
			int		last_offset;
		};
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
			iov_s	__ubuf_iovec;
			struct {
				union {
					/* use iter_iov() to get the current vec */
					// const iov_s		*__iov;
					const kvec_s	*kvec;
					// const struct bio_vec	*bvec;
					// const struct folio_queue *folioq;
					// struct xarray	*xarray;
					void __user		*ubuf;
				};
				size_t	count;
			};
		};
		union {
			ulong	nr_segs;
			// u8		folioq_slot;
			// loff_t	xarray_start;
		};
	} iov_iter_s;


	static inline size_t
	iov_iter_count(const iov_iter_s *i) {
		return i->count;
	}

	static inline void
	iov_iter_ubuf(iov_iter_s *i, uint direction,
			void __user *buf, size_t count) {

		// WARN_ON(direction & ~(READ | WRITE));
		*i = (iov_iter_s) {
			.iter_type		= ITER_UBUF,
			// .copy_mc		= false,
			// .user_backed	= true,
			// .data_source	= direction,
			.ubuf			= buf,
			.count			= count,
			.nr_segs		= 1,
		};
	}

	static inline void
	iov_iter_kvec(iov_iter_s *i, uint direction,
			const kvec_s *kvec, ulong nr_segs, size_t count) {

		// WARN_ON(direction & ~(READ | WRITE));
		*i = (iov_iter_s){
			.iter_type		= ITER_KVEC,
			// .copy_mc		= false,
			// .data_source	= direction,
			.kvec			= kvec,
			.nr_segs		= nr_segs,
			.iov_offset		= 0,
			.count			= count,
		};
	}

#endif /* __LINUX_UIO_H */