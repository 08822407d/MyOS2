#define PGALLOC_DEFINATION
#include "pgalloc.h"



/*
 * Allocate page upper directory.
 * We've already handled the fast-path in-line.
 */
int __pud_alloc(mm_s *mm, p4d_t *p4d_entp)
{
	spinlock_t *ptl = p4d_lock(mm, p4d_entp);
	if (p4d_ent_none_or_clear_bad(p4d_entp) ||
			!p4d_present(*p4d_entp)) {

		pud_t *new = pud_alloc_one(mm);
		if (!new)
			return -ENOMEM;

		mm_inc_nr_puds(mm);
		smp_wmb();	/* See comment in pmd_install() */
		p4d_populate(mm, p4d_entp, new);
	}
	spin_unlock_no_resched(ptl);
	return 0;
}

/*
 * Allocate page middle directory.
 * We've already handled the fast-path in-line.
 */
int __pmd_alloc(mm_s *mm, pud_t *pud_entp)
{
	spinlock_t *ptl = pud_lock(mm, pud_entp);
	if (pud_ent_none_or_clear_bad(pud_entp) ||
			!pud_present(*pud_entp)) {

		pmd_t *new = pmd_alloc_one(mm);
		if (!new)
			return -ENOMEM;

		mm_inc_nr_pmds(mm);
		smp_wmb();	/* See comment in pmd_install() */
		pud_populate(mm, pud_entp, new);
	}
	spin_unlock_no_resched(ptl);
	return 0;
}

int __pgtbl_alloc(mm_s *mm, pmd_t *pmd_entp)
{
	spinlock_t *ptl = pmd_lock(mm, pmd_entp);
	if (pmd_ent_none_or_clear_bad(pmd_entp) ||
			!pmd_present(*pmd_entp)) {

		pte_t *new = pte_alloc_one(mm);
		if (!new)
			return -ENOMEM;

		mm_inc_nr_ptes(mm);
		smp_wmb();	/* See comment in pmd_install() */
		pmd_populate(mm, pmd_entp, new);
	}
	spin_unlock_no_resched(ptl);
	return 0;
}