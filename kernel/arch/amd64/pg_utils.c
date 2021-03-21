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
PDPTE	PDPT[PGENT_NR] __aligned(PGENT_SIZE);
PDE		PD[PGENT_NR][PGENT_NR] __aligned(PGENT_SIZE);

static	PML4E	*PML4_phy;
static	PDPTE	*PDPT_phy;
static	PDE		*(PD_phy[PGENT_NR]);

void pg_clear(void)
{
	memset((void *)vir2phy(PML4), 0, sizeof(PML4));
	memset((void *)vir2phy(PDPT), 0, sizeof(PDPT));
	memset((void *)vir2phy(PD), 0, sizeof(PD));

	PML4_phy	= (PML4E *)vir2phy(PML4);
	PDPT_phy	= (PDPTE *)vir2phy(PDPT);
	for (int i = 0; i < PGENT_NR; i++)
	{
		PD_phy[i]		= (PDE *)vir2phy(PD[i]);
	}
}

void pg_mapkernel(void)
{
	// map low and high part of linear address to same physical address
	PML4_phy[0].Pflag = PML4_phy[PGENT_NR / 2].Pflag =
	PML4_phy[0].RWflag = PML4_phy[PGENT_NR / 2].RWflag = 1;
	PML4_phy[0].PHYADDR = PML4_phy[PGENT_NR / 2].PHYADDR = ((uint64_t)PDPT_phy >> 12);

	// count how many PDPTE does kernel use, each PDPTE asign 1GB memory
	uint64_t PDPTE_count = (uint64_t)((phy_addr)vir2phy(&_end) / 0x40000000) + 1;
	for (int i = 0; i < PDPTE_count; i++)
	{
		PDPT_phy[i].Pflag = PDPT_phy[i].RWflag = 1;
		PDPT_phy[i].PHYADDR = ((uint64_t)PD_phy[i] >> 12);

	// count how many PDE does kernel use, each PDE asign 2MB memory
		uint64_t PDE_count = (uint64_t)((phy_addr)vir2phy(&_end) / 0x200000) + 1;
		for (int j = 0; j < PDE_count; j++)
		{
			
			PD_phy[i][j].Pflag =
			PD_phy[i][j].RWflag =
			PD_phy[i][j].Pflag =
			PD_phy[i][j].PATflag = 1;
			PD_phy[i][j].PHYADDR = (j * 0x200000) >> 12;
		}
	}
}

void pg_load(void)
{
	__asm__ __volatile__("movq %%rax, %%cr3"
						 :
						 :"rax"(PML4_phy)
						 :);
}