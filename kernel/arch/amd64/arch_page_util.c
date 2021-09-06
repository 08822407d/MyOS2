#include <sys/types.h>
#include <lib/string.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_glo.h"
#include "include/arch_proto.h"

#include "../../include/glo.h"
#include "../../include/param.h"
#include "../../include/ktypes.h"
#include "../../include/const.h"

/* Storage for PML4, PDPT and PD. */
PML4E_T	KERN_PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE_T	KERN_PDPT[PDPT_NR][PGENT_NR] __aligned(PGENT_SIZE);
PDE_T	KERN_PD[PDPT_NR][PGENT_NR][PGENT_NR] __aligned(PGENT_SIZE);
phys_addr pml4_base = 0;

void arch_page_preinit(void)
{
	pml4_base = virt2phys(&KERN_PML4);
}

void arch_page_init(void)
{
	phys_addr k_phy_pgbase = 0;
	virt_addr k_vir_pgbase = (virt_addr)phys2virt(0);
	// uint64_t arch_page_attr = ARCH_PG_PRESENT | ARCH_PG_USER | ARCH_PG_RW;
	uint64_t arch_page_attr = ARCH_PG_PRESENT | ARCH_PG_RW;
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_vir_end - k_vir_pgbase) / CONFIG_PAGE_SIZE;
	for (long i = 0; i < pde_nr; i++)
	{
		unsigned long pg_idx = (unsigned long)k_phy_pgbase / CONFIG_PAGE_SIZE;
		// map lower mem
		arch_page_domap(k_phy_pgbase, k_phy_pgbase, arch_page_attr, KERN_PML4);
		// map higher mem
		arch_page_domap(k_vir_pgbase, k_phy_pgbase, arch_page_attr, KERN_PML4);
		// set page struct
		k_vir_pgbase += CONFIG_PAGE_SIZE;
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
}

void reload_arch_page()
{
	arch_page_preinit();
	arch_page_init();
	pg_load_cr3(KERN_PML4);
	// set init flag
	kparam.arch_init_flags.reload_bsp_arch_page = 1;
}

void pg_load_cr3(PML4E_T * PML4)
{
	__asm__ __volatile__(	"movq %%rax, %%cr3	\n\t"
							"nop				\n\t"
						:
						:	"rax"(virt2phys(PML4))
						:
						);
}

void refresh_arch_page(void)
{
	uint64_t tempreg;
	__asm__ __volatile__(	"movq %%cr3, %0		\n\t"
							"nop				\n\t"
							"movq %0, %%cr3		\n\t"
							"nop				\n\t"
						:	"=r"(tempreg)
						:
						:
						);
}

void arch_page_domap(virt_addr virt, phys_addr phys, uint64_t attr, PML4E_T * pml4_base)
{
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// get pml4e
	PML4E_T * pml4e_ptr = pml4_base + pml4e_idx;
	// so called "Higher Half Kernel" mapping
	// map higher half memory to lower half
	if (pml4e_idx > 255)
		pml4e_idx -= 256;
	// set pml4e
	if (pml4e_ptr->ENT == 0)
	{
		pml4e_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(&KERN_PDPT[pml4e_idx])) | ARCH_PGE_NOT_LAST(attr);
	}

	// get pdpte
	PDPTE_T * pdpte_ptr = (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->ENT)) + pdpte_idx;
	// set pdpte
	if (pdpte_ptr->ENT == 0)
	{
		pdpte_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(KERN_PD[pml4e_idx][pdpte_idx])) | ARCH_PGE_NOT_LAST(attr);
	}

	// get pde
	PDE_T * pde_ptr = (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->ENT)) + pde_idx;
	// set pte
	if (*((uint64_t *)pde_ptr) == 0)
	{
		pde_ptr->ENT = MASKF_2M((uint64_t)phys) | ARCH_PGE_IS_LAST(attr);
	}

	refresh_arch_page();
}

void pg_unmap(virt_addr virt)
{
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);
}