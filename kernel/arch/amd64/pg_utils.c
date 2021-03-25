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

// void pg_mapkernel(void)
// {
	// // map low and high part of linear address to same physical address
	// for (int i = 0; i < PDPT_NR; i++)
	// {
	// 	PML4[i].Pflag = PML4[i + PGENT_NR / 2].Pflag =
	// 	PML4[i].RWflag = PML4[i + PGENT_NR / 2].RWflag = 1;
	// 	PML4[i].PHYADDR = PML4[i + PGENT_NR / 2].PHYADDR =
	// 					(uint64_t)vir2phy(&PDPT[i]) >> SHIFT_PTE;

	// 	for (int j = 0; j < PGENT_NR; j++)
	// 	{
	// 		PDPT[i][j].Pflag = PDPT[i][j].RWflag = 1;
	// 		PDPT[i][j].PHYADDR = (uint64_t)vir2phy(&PD[i][j]) >> SHIFT_PTE;
	// 	}
	// }

	// map physical pages for kernel
	// phy_addr k_phy_pgbase = (phy_addr)CONFIG_PAGE_MASKF((uint64_t)kparam.kernel_phy_base);
	// vir_addr k_vir_pgbase = (vir_addr)CONFIG_PAGE_MASKF((uint64_t)kparam.kernel_vir_base);
	// long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_size) / CONFIG_PAGE_SIZE;
	// for (long i = 0; i < pde_nr; i++)
	// {
	// 	// map lower mem
	// 	pg_domap(k_phy_pgbase, k_phy_pgbase, 0);
	// 	// map higher mem
	// 	pg_domap(k_vir_pgbase, k_phy_pgbase, 0);
	// 	k_vir_pgbase += CONFIG_PAGE_SIZE;
	// 	k_phy_pgbase += CONFIG_PAGE_SIZE;
	// }
// }

void pg_load(void)
{
	__asm__ __volatile__("movq %%rax, %%cr3"
						 :
						 :"rax"(vir2phy(PML4))
						 :);
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
		pml4e_ptr->Pflag = pml4e_ptr->RWflag = 1;
		pml4e_ptr->PHYADDR = (uint64_t)vir2phy(PDPT[pml4e_idx]) >> SHIFT_PTE;
	}

	PDPTE *pdpte_ptr = (PDPTE *)phy2vir((phy_addr)((uint64_t)pml4e_ptr->PHYADDR << SHIFT_PTE)) + pdpte_idx;
	if (*((uint64_t *)pdpte_ptr) == 0)
	{
		pdpte_ptr->Pflag = pdpte_ptr->RWflag = 1;
		pdpte_ptr->PHYADDR = (uint64_t)vir2phy(PD[pml4e_idx][pdpte_idx]) >> SHIFT_PTE;
	}
	PDPTE *pde_ptr = (PDE *)phy2vir((phy_addr)((uint64_t)pdpte_ptr->PHYADDR << SHIFT_PTE)) + pde_idx;
	if (*((uint64_t *)pde_ptr) == 0)
	{
		pde_ptr->Pflag =
		pde_ptr->RWflag =
		pde_ptr->PATflag = 1;
		pde_ptr->PHYADDR = (uint64_t)phy >> SHIFT_PTE;
	}
}

void pg_unmap(vir_addr vir)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);
}