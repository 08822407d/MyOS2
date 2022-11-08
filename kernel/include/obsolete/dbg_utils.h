#ifndef _DBG_UTILS_H_
#define _DBG_UTILS_H_

#include <obsolete/archtypes.h>
#include <asm/pgtable_types.h>

	typedef struct pgmapset
	{
		pgd_t	pml4e;
		pud_t	pdpte;
		PDE_T	pde;
		PTE_T	pte;
		phys_addr_t	paddr;
	} pgmapset_s;

	pgmapset_s DBG_get_pgmapset(IN reg_t cr3, IN virt_addr_t virt);

#endif /* _DBG_UTILS_H_ */