#ifndef _PAGE_H_
#define _PAGE_H_

#include <linux/mm/mm_types.h>

	void memblock_free_pages(page_s *page, unsigned long pfn, unsigned int order);
	void preinit_page(void);
	void init_page(void);

#endif /* _PAGE_H_ */