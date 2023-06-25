// SPDX-License-Identifier: GPL-2.0
#include <linux/mm/mm.h>
#include <linux/mm/gfp.h>
// #include <linux/hugetlb.h>
#include <asm/pgalloc.h>
// #include <asm/tlb.h>
// #include <asm/fixmap.h>
// #include <asm/mtrr.h>


static inline pgd_t *_pgd_alloc(void) {
	return (pgd_t *)__get_free_pages(GFP_PGTABLE_USER,
					 PGD_ALLOCATION_ORDER);
}

static inline void _pgd_free(pgd_t *pgd) {
	free_pages((unsigned long)pgd, PGD_ALLOCATION_ORDER);
}

pgd_t *pgd_alloc(mm_s *mm)
{
	pgd_t *pgd;
	// pmd_t *u_pmds[MAX_PREALLOCATED_USER_PMDS];
	// pmd_t *pmds[MAX_PREALLOCATED_PMDS];

	pgd = _pgd_alloc();

	if (pgd == NULL)
		goto out;

	mm->pgd = pgd;

	// if (preallocate_pmds(mm, pmds, PREALLOCATED_PMDS) != 0)
	// 	goto out_free_pgd;

	// if (preallocate_pmds(mm, u_pmds, PREALLOCATED_USER_PMDS) != 0)
	// 	goto out_free_pmds;

	// if (paravirt_pgd_alloc(mm) != 0)
	// 	goto out_free_user_pmds;

	// /*
	//  * Make sure that pre-populating the pmds is atomic with
	//  * respect to anything walking the pgd_list, so that they
	//  * never see a partially populated pgd.
	//  */
	// spin_lock(&pgd_lock);

	// pgd_ctor(mm, pgd);
	// pgd_prepopulate_pmd(mm, pgd, pmds);
	// pgd_prepopulate_user_pmd(mm, pgd, u_pmds);

	// spin_unlock(&pgd_lock);

	// return pgd;

// out_free_user_pmds:
// 	free_pmds(mm, u_pmds, PREALLOCATED_USER_PMDS);
// out_free_pmds:
// 	free_pmds(mm, pmds, PREALLOCATED_PMDS);
out_free_pgd:
	_pgd_free(pgd);
out:
	return NULL;
}

void pgd_free(mm_s *mm, pgd_t *pgd)
{
	// pgd_mop_up_pmds(mm, pgd);
	// pgd_dtor(pgd);
	// paravirt_pgd_free(mm, pgd);
	// _pgd_free(pgd);
}