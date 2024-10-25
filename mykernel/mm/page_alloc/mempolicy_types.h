#ifndef _MEMPOLICY_TYPES_H_
#define _MEMPOLICY_TYPES_H_

	#include "../mm_type_declaration.h"

	#include "page-flags_types.h"


	/*
	 * Describe a memory policy.
	 *
	 * A mempolicy can be either associated with a process or with a VMA.
	 * For VMA related allocations the VMA policy is preferred, otherwise
	 * the process policy is used. Interrupts ignore the memory policy
	 * of the current process.
	 *
	 * Locking policy for interleave:
	 * In process context there is no locking because only the process accesses
	 * its own state. All vma manipulation is somewhat protected by a down_read on
	 * mmap_lock.
	 *
	 * Freeing policy:
	 * Mempolicy objects are reference counted.  A mempolicy will be freed when
	 * mpol_put() decrements the reference count to zero.
	 *
	 * Duplicating policy objects:
	 * mpol_dup() allocates a new mempolicy and copies the specified mempolicy
	 * to the new storage.  The reference count of the new object is initialized
	 * to 1, representing the caller of mpol_dup().
	 */
	struct mempolicy {
		atomic_t	refcnt;
		ushort		mode; 			/* See MPOL_* above */
		ushort		flags;			/* See set_mempolicy() MPOL_F_* above */
		// nodemask_t	nodes;			/* interleave/bind/perfer */
		// int			home_node;		/* Home node to use for MPOL_BIND and MPOL_PREFERRED_MANY */

		// union {
		// 	nodemask_t	cpuset_mems_allowed;	/* relative to these nodes */
		// 	nodemask_t	user_nodemask;			/* nodemask passed by user */
		// } w;
	};

#endif /* _MEMPOLICY_TYPES_H_ */