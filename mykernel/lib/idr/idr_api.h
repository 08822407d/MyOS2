/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IDR_API_H_
#define _LINUX_IDR_API_H_

	#include "idr.h"

	extern int idr_alloc_u32(idr_s *idr, void *ptr,
			u32 *nextid, ulong max, gfp_t gfp);

	extern int idr_alloc(idr_s *idr, void *ptr,
			int start, int end, gfp_t gfp);

	extern int idr_alloc_cyclic(idr_s *idr, void *ptr,
			int start, int end, gfp_t gfp);

	extern void *idr_remove(idr_s *idr, ulong id);

	extern void *idr_find(const idr_s *idr, ulong id);



	extern void *idr_replace(idr_s *idr, void *ptr, ulong id);

#endif /* _LINUX_IDR_API_H_ */