#ifndef _LINUX_PAGE_ALLOC_MACRO_H_
#define _LINUX_PAGE_ALLOC_MACRO_H_


	#define page_private(page)	((page)->private)


	#define ptdesc_page(pt)		(_Generic((pt),				\
				const ptdesc_s *:	(const page_s *)(pt),	\
				ptdesc_s *:			(page_s *)(pt))			\
			)

	#define ptdesc_folio(pt)	(_Generic((pt),				\
				const ptdesc_s *:	(const folio_s *)(pt),	\
				ptdesc_s *:			(folio_s *)(pt))		\
			)

	#define page_ptdesc(p)		(_Generic((p),				\
				const page_s *:		(const ptdesc_s *)(p),	\
				page_s *:			(ptdesc_s *)(p))		\
			)

#endif /* _LINUX_PAGE_ALLOC_MACRO_H_ */