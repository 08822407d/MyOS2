#ifndef _LINUX_SLUB_MACRO_H_
#define _LINUX_SLUB_MACRO_H_


	/**
	 * folio_slab - Converts from folio to slab.
	 * @folio: The folio.
	 *
	 * Currently struct slab is a different representation of a folio where
	 * folio_test_slab() is true.
	 *
	 * Return: The slab which contains this folio.
	 */
	#define folio_to_slab(folio)	((slab_s *)(folio))
	#define folio_slab(folio)		folio_to_slab(folio)

	/**
	 * slab_folio - The folio allocated for a slab
	 * @slab: The slab.
	 *
	 * Slabs are allocated as folios that contain the individual objects and are
	 * using some fields in the first page_s of the folio - those fields are
	 * now accessed by struct slab. It is occasionally necessary to convert back to
	 * a folio in order to communicate with the rest of the mm.  Please use this
	 * helper function instead of casting yourself, as the implementation may change
	 * in the future.
	 */
	// #define slab_to_folio(slab)		((folio_s *)(slab))
	// #define slab_folio(slab)		slab_to_folio(slab)
	#define slab_folio(s)		(_Generic((s),				\
				const slab_s *:		(const folio_s *)s,		\
				slab_s *:			(folio_s *)s)			\
			)

	/**
	 * page_slab - Converts from first page_s to slab.
	 * @p: The first (either head of compound or single) page of slab.
	 *
	 * A temporary wrapper to convert page_s to struct slab in situations where
	 * we know the page is the compound head, or single order-0 page.
	 *
	 * Long-term ideally everything would work with struct slab directly or go
	 * through folio to struct slab.
	 *
	 * Return: The slab which contains this page
	 */
	#define page_to_slab(page)		((slab_s *)(page))
	#define page_slab(page)			page_to_slab(page)

	/**
	 * slab_page - The first page_s allocated for a slab
	 * @slab: The slab.
	 *
	 * A convenience wrapper for converting slab to the first page_s of the
	 * underlying folio, to communicate with code not yet converted to folio or
	 * struct slab.
	 */
	#define slab_page(slab)			folio_page(slab_folio(s), 0)





	/*
	 * Please use this macro to create slab caches. Simply specify the
	 * name of the structure and maybe some flags that are listed above.
	 *
	 * The alignment of the struct determines object alignment. If you
	 * f.e. add ____cacheline_aligned_in_smp to the struct declaration
	 * then the objects will be properly aligned in SMP configurations.
	 */
	#define KMEM_CACHE(__struct, __flags)		\
			kmem_cache_create(#__struct,		\
				sizeof(struct __struct),		\
				__alignof__(struct __struct),	\
				(__flags),						\
				NULL							\
			)

	/*
	 * To whitelist a single field for copying to/from usercopy, use this
	 * macro instead for KMEM_CACHE() above.
	 */
	#define KMEM_CACHE_USERCOPY(__struct, __flags, __field)	\
			kmem_cache_create_usercopy(#__struct,			\
				sizeof(struct __struct),					\
				__alignof__(struct __struct),				\
				(__flags),									\
				offsetof(struct __struct, __field),			\
				sizeof_field(struct __struct, __field),		\
				NULL										\
			)


	#define kmalloc_node(_size, _flags, _node)	\
				kmalloc(_size, _flags)

	#define kzalloc_node(_size, _flags, _node)	\
				kmalloc_node(_size, (_flags)|__GFP_ZERO, _node)

#endif /* _LINUX_SLUB_MACRO_H_ */