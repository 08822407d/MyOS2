#ifndef _KMALLOC_CONST_H_
#define _KMALLOC_CONST_H_

	/*
	 * SLUB directly allocates requests fitting in to an order-1 page
	 * (PAGE_SIZE*2).  Larger requests are passed to the page allocator.
	 */
	#define KMALLOC_SHIFT_HIGH	(PAGE_SHIFT + 1)
	#define KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT - 1)
	#ifndef KMALLOC_SHIFT_LOW
	#	define KMALLOC_SHIFT_LOW	3
	#endif

	/* Maximum allocatable size */
	#define KMALLOC_MAX_SIZE		(1UL << KMALLOC_SHIFT_MAX)
	#define KMALLOC_MAX_CACHE_SIZE	PAGE_SIZE
	/* Maximum order allocatable via the slab allocator */
	#define KMALLOC_MAX_ORDER		(KMALLOC_SHIFT_MAX - PAGE_SHIFT)

	/*
	 * Kmalloc subsystem.
	 */
	#ifndef KMALLOC_MIN_SIZE
	#	define KMALLOC_MIN_SIZE		(1 << KMALLOC_SHIFT_LOW)
	#endif

	/*
	 * This restriction comes from byte sized index implementation.
	 * Page size is normally 2^12 bytes and, in this case, if we want to use
	 * byte sized index which can represent 2^8 entries, the size of the object
	 * should be equal or greater to 2^12 / 2^8 = 2^4 = 16.
	 * If minimum size of kmalloc is less than 16, we use it as minimum object
	 * size and give up to use byte sized index.
	 */
	#define SLAB_OBJ_MIN_SIZE		(KMALLOC_MIN_SIZE < 16 ? (KMALLOC_MIN_SIZE) : 16)

#endif /* _KMALLOC_CONST_H_ */