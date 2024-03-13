#include <linux/kernel/mm.h>

long
myos_get_user_pages(mm_s *mm, virt_addr_t start,
	unsigned long nr_pages, page_s **pages)
{
	for (int i = 0; i < nr_pages; i++)
	{
		virt_addr_t addr = start + i * PAGE_SIZE;
		vma_s *vma = myos_find_vma(mm, addr);
		if (vma == NULL)
			continue;
		
		page_s *pg = myos_get_one_page_from_mapping(mm, addr);
		if (pg == NULL)
		{
			pg = alloc_page(GFP_USER);
			if (pg != NULL)
			{
				myos_creat_one_page_mapping(mm, addr, pg);
				get_page(pg);
			}
		}

		pages[i] = pg;
	}
}