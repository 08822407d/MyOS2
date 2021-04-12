#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "../../include/param.h"
#include "../../include/ktypes.h"
#include "../../include/const.h"

/* Storage for PML4, PDPT and PD. */
PML4E	PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE	PDPT[PDPT_NR][PGENT_NR] __aligned(PGENT_SIZE);
PDE		PD[PDPT_NR][PGENT_NR][PGENT_NR] __aligned(PGENT_SIZE);

void pg_clear(void)
{
	phy_addr pml4_base = vir2phy(&PML4);
	phy_addr pdpt_base = vir2phy(&PDPT);
	phy_addr pd_base = vir2phy(&PD);
	memset(pml4_base, 0, sizeof(PML4));
	memset(pdpt_base, 0, sizeof(PDPT));
	memset(pd_base, 0, sizeof(PD));
}

void pg_load_cr3(PML4E * PML4)
{
	__asm__ __volatile__("movq %%rax, %%cr3	\n\
						  nop"
						 :
						 :"rax"(vir2phy(PML4))
						 :);
}

void pg_flush_tlb(void)
{
	uint64_t tempreg;
	__asm__ __volatile__("movq %%cr3, %0	\n\
						  nop				\n\
						  movq %0, %%cr3	\n\
						  nop				"
						 :"=r"(tempreg)
						 :
						 :	);
}

void pg_domap(vir_addr vir, phy_addr phy, uint64_t attr)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);

	PML4E *pml4e_ptr = PML4 + pml4e_idx;
	// so called "Higher Half Kernel" mapping
	// map higher half memory to lower half
	if (pml4e_idx > 255)
		pml4e_idx -= 256;
	if (*((uint64_t *)pml4e_ptr) == 0)
	{
		pml4e_ptr->Pflag = pml4e_ptr->RWflag = pml4e_ptr->USflag = 1;
		pml4e_ptr->PHYADDR = (uint64_t)vir2phy(PDPT[pml4e_idx]) >> SHIFT_PTE;
	}

	PDPTE *pdpte_ptr = (PDPTE *)phy2vir((phy_addr)((uint64_t)pml4e_ptr->PHYADDR << SHIFT_PTE)) + pdpte_idx;
	if (*((uint64_t *)pdpte_ptr) == 0)
	{
		pdpte_ptr->Pflag = pdpte_ptr->RWflag = pdpte_ptr->USflag = 1;
		pdpte_ptr->PHYADDR = (uint64_t)vir2phy(PD[pml4e_idx][pdpte_idx]) >> SHIFT_PTE;
	}
	PDPTE *pde_ptr = (PDE *)phy2vir((phy_addr)((uint64_t)pdpte_ptr->PHYADDR << SHIFT_PTE)) + pde_idx;
	if (*((uint64_t *)pde_ptr) == 0)
	{
		pde_ptr->Pflag =
		pde_ptr->RWflag =
		pde_ptr->USflag =
		pde_ptr->PATflag = 1;
		pde_ptr->PHYADDR = (uint64_t)phy >> SHIFT_PTE;
	}

	pg_flush_tlb();
}

void pg_unmap(vir_addr vir)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);
}