/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PAGE_FLAGS_TYPES_H_
#define _LINUX_PAGE_FLAGS_TYPES_H_

	#include "../mm_type_declaration.h"

	struct pgflag_defs {
		ulong
			PG_locked		:1,
			PG_writeback	:1,
			PG_referenced	:1,
			PG_uptodate		:1,
			PG_dirty		:1,
			PG_lru			:1,
			PG_head			:1,
			PG_waiters		:1,
			PG_active		:1,
			PG_workingset	:1,
			PG_error		:1,
			PG_slab			:1,
			PG_owner_priv_1	:1,
			PG_arch_1		:1,
			PG_reserved		:1,
			PG_private		:1,
			PG_private_2	:1,
			PG_mappedtodisk	:1,
			PG_reclaim		:1,
			PG_swapbacked	:1,
			PG_unevictable	:1,
			PG_mlocked		:1,/* Page is vma mlocked */
		#ifdef CONFIG_ARCH_USES_PG_UNCACHED
			PG_uncached		:1,/* Page has been mapped as uncached */
		#endif
		#ifdef CONFIG_MEMORY_FAILURE
			PG_hwpoison		:1,/* hardware poisoned page. Don't touch */
		#endif
		#if defined(CONFIG_PAGE_IDLE_FLAG)
			PG_young		:1,
			PG_idle			:1,
		#endif
			PG_arch_2		:1,
			PG_arch_3		:1
			;
	}__attribute__((packed));

#endif /* _LINUX_PAGE_FLAGS_TYPES_H_ */
