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
	memset(vir2phy(&PML4), 0, sizeof(PML4));
	memset(vir2phy(&PDPT), 0, sizeof(PDPT));
	memset(vir2phy(&PD), 0, sizeof(PD));
}

void pg_mapkernel(void)
{
	// map low and high part of linear address to same physical address
	for (int i = 0; i < PDPT_NR; i++)
	{
		PML4[i].Pflag = PML4[i + PGENT_NR / 2].Pflag =
		PML4[i].RWflag = PML4[i + PGENT_NR / 2].RWflag = 1;
		PML4[i].PHYADDR = PML4[i + PGENT_NR / 2].PHYADDR = ((uint64_t)vir2phy(&PDPT[i]) >> 12);

		// count how many PDPTE does kernel use, each PDPTE asign 1GB memory
		for (int j = 0; j < PGENT_NR; j++)
		{
			PDPT[i][j].Pflag = PDPT[i][j].RWflag = 1;
			PDPT[i][j].PHYADDR = ((uint64_t)vir2phy(&PD[i][j]) >> 12);

			// count how many PDE does kernel use, each PDE asign 2MB memory
			for (int k = 0; k < PGENT_NR; k++)
			{
				
				PD[i][j][k].Pflag =
				PD[i][j][k].RWflag =
				PD[i][j][k].Pflag =
				PD[i][j][k].PATflag = 1;
				PD[i][j][k].PHYADDR = (k * CONFIG_PAGE_SIZE) >> 12;
			}
		}
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
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);

	PML4E *pml4e_ptr = &PML4[pml4e_idx];
	if (*((uint64_t *)pml4e_ptr) == 0)
		while(1);
	PDPTE *pdpte_ptr = &PDPT[pml4e_idx][pdpte_idx];
}

void pg_unmap()
{

}