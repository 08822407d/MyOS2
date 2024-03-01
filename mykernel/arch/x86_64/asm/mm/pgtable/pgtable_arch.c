#define ARCH_PGTABLE_DEFINATION
#include "pgtable_arch.h"

// SPDX-License-Identifier: GPL-2.0
#include <linux/lib/list.h>
#include <asm-generic/pgalloc.h>


static inline void pgd_list_add(pgd_t *pgd) {
	page_s *page = virt_to_page(pgd);
	list_hdr_append(&pgd_list_hdr, &page->lru);
}

static inline void pgd_list_del(pgd_t *pgd) {
	page_s *page = virt_to_page(pgd);
	list_hdr_delete(&pgd_list_hdr, &page->lru);
}


static inline void _pgd_free(pgd_t *pgd) {
	free_pages((unsigned long)pgd, PGD_ALLOCATION_ORDER);
}

pgd_t *pgd_alloc(mm_s *mm)
{
	pgd_t *pgd;

	pgd = (pgd_t *)get_zeroed_page(GFP_PGTABLE_USER);
	if (pgd == NULL)
		goto out;

	mm->pgd = pgd;

	/*
	 * Make sure that pre-populating the pmds is atomic with
	 * respect to anything walking the pgd_list, so that they
	 * never see a partially populated pgd.
	 */
	spin_lock(&pgd_lock);

	// static void pgd_ctor(mm_s *mm, pgd_t *pgd)
	// {
		/* If the pgd points to a shared pagetable level (either the
		   ptes in non-PAE, or shared PMD in PAE), then just copy the
		   references from swapper_pg_dir. */
		memcpy(pgd + KERNEL_PGD_BOUNDARY,
			swapper_pg_dir + KERNEL_PGD_BOUNDARY,
			KERNEL_PGD_PTRS * sizeof(pgd_t));

		/* list required to sync kernel mapping updates */
		virt_to_page(pgd)->pt_mm = mm;
		pgd_list_add(pgd);
	// }
	// pgd_prepopulate_pmd(mm, pgd, pmds);
	// pgd_prepopulate_user_pmd(mm, pgd, u_pmds);

	spin_unlock(&pgd_lock);

	return pgd;

out_free_pgd:
	_pgd_free(pgd);
out:
	return NULL;
}

void pgd_free(mm_s *mm, pgd_t *pgd)
{
	// pgd_mop_up_pmds(mm, pgd);
	// static void pgd_dtor(pgd_t *pgd)
	// {
		spin_lock(&pgd_lock);
		pgd_list_del(pgd);
		spin_unlock(&pgd_lock);
	// }
	_pgd_free(pgd);
}