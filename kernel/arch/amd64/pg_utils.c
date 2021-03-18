#include <sys/types.h>
#include <lib/string.h>
#include <lib/assert.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"

extern char _end;

/* Storage for PML4, PDPT and PD. */
PML4E	PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE	PDPT[PGENT_NR] __aligned(PGENT_SIZE);
PDE		PD[PGENT_NR][PGENT_NR] __aligned(PGENT_SIZE);


void pg_clear(void)
{
	memset((void *)PML4, 0, sizeof(PML4));
	memset((void *)PDPT, 0, sizeof(PDPT));
	memset((void *)PD, 0, sizeof(PD));
}

void pg_mapkernel(void)
{
	// map low and high part of linear address to same physical address
	PML4[0].Pflag = PML4[PGENT_NR / 2].Pflag =
	PML4[0].RWflag = PML4[PGENT_NR / 2].RWflag = 1;
	PML4[0].PHYADDR = PML4[PGENT_NR / 2].PHYADDR = ((uint64_t)PDPT >> 12);

	// count how many PDPTE does kernel use, each PDPTE asign 1GB memory
	uint64_t PDPTE_count = (uint64_t)((size_t)&_end / 0x40000000) + 1;
	for (int i = 0; i < PDPTE_count; i++)
	{
		PDPT[i].Pflag = PDPT[i].RWflag = 1;
		PDPT[i].PHYADDR = ((uint64_t)PD >> 12);
	}

	// count how many PDE does kernel use, each PDE asign 2MB memory
	uint64_t PDE_count = (uint64_t)((size_t)&_end / 0x200000) + 1;
	for (int j = 0; j < PDE_count; j++)
	{
		
		PD[j]->Pflag = PD[j]->Pflag = PD[j]->PATflag = 1;
		PD[j]->PHYADDR = (j * 0x200000) >> 12;
	}
}

void pg_load(void)
{
	__asm__ __volatile__("movq %%rax, %%cr3"
						 :
						 :"rax"(PML4)
						 :);
}