#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "../../include/ktypes.h"
#include "../../include/const.h"

extern char _end;

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

void pg_mapkernel(void)
{
	// map low and high part of linear address to same physical address
	for (int i = 0; i < PDPT_NR; i++)
	{
		PML4[i].Pflag = PML4[i + PGENT_NR / 2].Pflag =
		PML4[i].RWflag = PML4[i + PGENT_NR / 2].RWflag = 1;
		PML4[i].PHYADDR = PML4[i + PGENT_NR / 2].PHYADDR =
						(uint64_t)vir2phy(&PDPT[i]) >> SHIFT_PTE;

		for (int j = 0; j < PGENT_NR; j++)
		{
			PDPT[i][j].Pflag = PDPT[i][j].RWflag = 1;
			PDPT[i][j].PHYADDR = (uint64_t)vir2phy(&PD[i][j]) >> SHIFT_PTE;
		}
	}

	// map physical pages for kernel
	vir_addr k_vir_pgbase = phy2vir(0);
	phy_addr k_phy_pgbase = 0;
	long pde_nr   = ((long)vir2phy(&_end) >> CONFIG_PAGE_SHIFT) + 1;
	for (long i = 0; i < pde_nr; i++)
	{
		pg_domap(k_vir_pgbase, k_phy_pgbase);
		k_vir_pgbase += CONFIG_PAGE_SIZE;
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
}

void pg_load(void)
{
	__asm__ __volatile__("movq %%rax, %%cr3"
						 :
						 :"rax"(vir2phy(PML4))
						 :);
}

void pg_domap(vir_addr vir, phy_addr phy)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	if (pml4e_idx > 255)
		pml4e_idx -= 256;
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);

	PML4E *pml4e_ptr = PML4 + pml4e_idx;
	if (*((uint64_t *)pml4e_ptr) == 0)
		while(1);

	PDPTE *pdpte_ptr = (PDPTE *)phy2vir((phy_addr)((uint64_t)pml4e_ptr->PHYADDR << SHIFT_PTE)) + pdpte_idx;
	if (*((uint64_t *)pdpte_ptr) == 0)
	{
		pdpte_ptr->Pflag = pdpte_ptr->RWflag = 1;
		pdpte_ptr->PHYADDR = ((uint64_t)vir2phy(PD[pml4e_idx][pdpte_idx]) >> SHIFT_PTE);
	}
	PDPTE *pde_ptr = (PDE *)phy2vir((phy_addr)((uint64_t)pdpte_ptr->PHYADDR << SHIFT_PTE)) + pde_idx;
	if (*((uint64_t *)pde_ptr) == 0)
	{
		pde_ptr->Pflag =
		pde_ptr->RWflag =
		pde_ptr->PATflag = 1;
		pde_ptr->PHYADDR = (uint64_t)phy >> SHIFT_PTE;
	}
	else
	{
		while (1);
	}
}

void pg_unmap(vir_addr vir)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);
}