#include <linux/kernel/types.h>
#include <linux/lib/string.h>

#include <obsolete/dbg_utils.h>
#include <obsolete/archconst.h>
#include <obsolete/arch_proto.h>

pgmapset_s DBG_get_pgmapset(IN reg_t cr3, IN virt_addr_t virt)
{
	pgmapset_s ret_val;
	memset(&ret_val, 0, sizeof(pgmapset_s));

	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	phys_addr_t pml4_pa	= (phys_addr_t)ARCH_PGS_ADDR(cr3);
	// get pml4e
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	ret_val.pml4e			= *pml4e_ptr;
	if (pml4e_ptr->pgd == 0)
		goto fail_return;
	// get pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt((phys_addr_t)ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	ret_val.pdpte			= *pdpte_ptr;
	if (pdpte_ptr->pud == 0)
		goto fail_return;
	// get pde
	pmd_t *PD_ptr		= (pmd_t *)myos_phys2virt((phys_addr_t)ARCH_PGS_ADDR(pdpte_ptr->pud));	
	pmd_t *pde_ptr		= PD_ptr + pde_idx;
	ret_val.pde				= *pde_ptr;
	if (pde_ptr->pmd == 0)
		goto fail_return;

	ret_val.paddr = (phys_addr_t)((uint64_t)pde_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}