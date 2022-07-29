#include <linux/kernel/types.h>
#include <linux/lib/string.h>

#include <obsolete/dbg_utils.h>
#include "../arch/amd64/include/archconst.h"
#include "../arch/amd64/include/arch_proto.h"

pgmapset_s DBG_get_pgmapset(IN reg_t cr3, IN virt_addr_t virt)
{
	pgmapset_s ret_val;
	memset(&ret_val, 0, sizeof(pgmapset_s));

	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	phys_addr_t pml4_paddr 	= (phys_addr_t)ARCH_PGS_ADDR(cr3);
	// get pml4e
	PML4E_T *	PML4_ptr	= (PML4E_T *)phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr	= PML4_ptr + pml4e_idx;
	ret_val.pml4e			= *pml4e_ptr;
	if (pml4e_ptr->ENT == 0)
		goto fail_return;
	// get pdpte
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)phys2virt((phys_addr_t)ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	ret_val.pdpte			= *pdpte_ptr;
	if (pdpte_ptr->ENT == 0)
		goto fail_return;
	// get pde
	PDE_T *		PD_ptr		= (PDE_T *)phys2virt((phys_addr_t)ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
	ret_val.pde				= *pde_ptr;
	if (pde_ptr->ENT == 0)
		goto fail_return;

	ret_val.paddr = (phys_addr_t)((uint64_t)pde_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}