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
	 * using some fields in the first struct page of the folio - those fields are
	 * now accessed by struct slab. It is occasionally necessary to convert back to
	 * a folio in order to communicate with the rest of the mm.  Please use this
	 * helper function instead of casting yourself, as the implementation may change
	 * in the future.
	 */
	#define slab_to_folio(slab)		((folio_s *)(slab))
	#define slab_folio(slab)		slab_to_folio(slab)

	/**
	 * page_slab - Converts from first struct page to slab.
	 * @p: The first (either head of compound or single) page of slab.
	 *
	 * A temporary wrapper to convert struct page to struct slab in situations where
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
	 * slab_page - The first struct page allocated for a slab
	 * @slab: The slab.
	 *
	 * A convenience wrapper for converting slab to the first struct page of the
	 * underlying folio, to communicate with code not yet converted to folio or
	 * struct slab.
	 */
	#define slab_first_page(slab)	folio_page(slab_folio(slab), 0)
	#define slab_page(slab)			slab_first_page(slab)

#endif /* _LINUX_SLUB_MACRO_H_ */