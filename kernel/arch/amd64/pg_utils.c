#include <sys/types.h>
#include <lib/string.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_glo.h"
#include "include/arch_proto.h"

#include "../../include/param.h"
#include "../../include/ktypes.h"
#include "../../include/const.h"

/* Storage for PML4, PDPT and PD. */
PML4E_u	KERN_PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE_u	KERN_PDPT[PDPT_NR][PGENT_NR] __aligned(PGENT_SIZE);
PDE_u	KERN_PD[PDPT_NR][PGENT_NR][PGENT_NR] __aligned(PGENT_SIZE);
phys_addr pml4_base = 0;

void pg_pre_init(void)
{
	pml4_base = virt2phys(&KERN_PML4);
}

void refresh_arch_page()
{
	pg_pre_init();
	init_page_manage();
	pg_load_cr3(KERN_PML4);
}

void pg_load_cr3(PML4E_u * PML4)
{
	__asm__ __volatile__("movq %%rax, %%cr3	\n\
						  nop"
						 :
						 :"rax"(virt2phys(PML4))
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

void pg_domap(virt_addr vir, phys_addr phy, uint64_t attr, PML4E_u * pml4_base)
{
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);

	// get pml4e
	PML4E_u * pml4e_ptr = pml4_base + pml4e_idx;
	// so called "Higher Half Kernel" mapping
	// map higher half memory to lower half
	if (pml4e_idx > 255)
		pml4e_idx -= 256;
	// set pml4e
	if (pml4e_ptr->PML4E == 0)
	{
		pml4e_ptr->PML4E = ARCH_PGS_ADDR((uint64_t)virt2phys(&KERN_PDPT[pml4e_idx])) | ARCH_PGE_NOT_LAST(attr);
	}

	// get pdpte
	PDPTE_u * pdpte_ptr = (PDPTE_u *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->PML4E)) + pdpte_idx;
	// set pdpte
	if (pdpte_ptr->PDPTE == 0)
	{
		pdpte_ptr->PDPTE = ARCH_PGS_ADDR((uint64_t)virt2phys(KERN_PD[pml4e_idx][pdpte_idx])) | ARCH_PGE_NOT_LAST(attr);
	}

	// get pde
	PDE_u * pde_ptr = (PDE_u *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->PDPTE)) + pde_idx;
	// set pte
	if (*((uint64_t *)pde_ptr) == 0)
	{
		pde_ptr->PDE = MASKF_2M((uint64_t)phy) | ARCH_PGE_IS_LAST(attr);
	}

	pg_flush_tlb();
}

void pg_unmap(virt_addr vir)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)vir >> SHIFT_PDE);
}