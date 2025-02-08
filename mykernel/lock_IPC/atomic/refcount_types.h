/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_REFCOUNT_TYPES_H_
#define _LINUX_REFCOUNT_TYPES_H_

	#include "../lock_ipc_type_declaration.h"

	/**
	 * typedef refcount_t - variant of atomic_t specialized for reference counts
	 * @refs: atomic_t counter field
	 *
	 * The counter saturates at REFCOUNT_SATURATED and will not move once
	 * there. This avoids wrapping the counter and causing 'spurious'
	 * use-after-free bugs.
	 */
	struct refcount_struct {
		atomic_t refs;
	};

#endif /* _LINUX_REFCOUNT_TYPES_H_ */
