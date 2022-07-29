#ifndef _DBG_UTILS_H_
#define _DBG_UTILS_H_

#include <arch/amd64/include/archtypes.h>

	typedef struct pgmapset
	{
		PML4E_T	pml4e;
		PDPTE_T	pdpte;
		PDE_T	pde;
		PTE_T	pte;
		phys_addr_t	paddr;
	} pgmapset_s;

	pgmapset_s DBG_get_pgmapset(IN reg_t cr3, IN virt_addr_t virt);

#endif /* _DBG_UTILS_H_ */