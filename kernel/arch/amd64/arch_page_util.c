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
PDPTE_T	KERN_PDPT[PGENT_NR] __aligned(PGENT_SIZE);
PDE_T	KERN_PD[PDE_NR] __aligned(PGENT_SIZE);

phys_addr kernel_cr3 = 0;

void creat_fixed_kernel_pgmap();

void map_fix_kernel_pages()
{
	// map kernel software used pages
	phys_addr k_phy_pgbase = 0;
	virt_addr k_vir_pgbase = (virt_addr)phys2virt(0);
	uint64_t attr = ARCH_PG_PRESENT | ARCH_PG_RW;
	long pde_nr   = CONFIG_PAGE_ALIGH(kparam.kernel_vir_end - k_vir_pgbase) / CONFIG_PAGE_SIZE;
	for (long i = 0; i < pde_nr; i++)
	{
		fill_pde(&KERN_PD[i], k_phy_pgbase, attr);
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
	//map video used pages
	k_phy_pgbase = framebuffer.FB_phybase;
	pde_nr = CONFIG_PAGE_ALIGH(framebuffer.FB_size) / CONFIG_PAGE_SIZE;
	unsigned long start_idx = (unsigned long)k_phy_pgbase / CONFIG_PAGE_SIZE;
	for (long i = start_idx; i < pde_nr + start_idx; i++)
	{
		fill_pde(&KERN_PD[i], k_phy_pgbase, attr | ARCH_PG_USER);
		k_phy_pgbase += CONFIG_PAGE_SIZE;
	}
}

void reload_arch_page()
{
	int pdpte_nr = PDPTE_NR;
	int pde_nr = PDE_NR;

	kernel_cr3 = virt2phys(&KERN_PML4);

	creat_fixed_kernel_pgmap();
	map_fix_kernel_pages();

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

//
inline PML4E_T * get_cr3(PML4E_T * pmlt4_ptr)
{
	return (PML4E_T *)virt2phys((virt_addr)pmlt4_ptr);
}

inline void fill_pml4e(PML4E_T * pml4e_ptr, PDPTE_T pdpt_ptr[PGENT_NR], uint64_t attr)
{
	pml4e_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pdpt_ptr)) | ARCH_PGE_NOT_LAST(attr);
}
PDPTE_T * get_pdpt(PML4E_T * pml4_ptr, uint64_t pml4e_idx)
{
	PDPTE_T * ret_val = NULL;
	if (pml4e_idx < PGENT_NR)
		ret_val = (PDPTE_T *)phys2virt((phys_addr)((uint64_t)pml4_ptr[pml4e_idx].defs.PHYADDR << 12));
	return ret_val;
}

inline void fill_pdpte(PDPTE_T * pdpte_ptr, PDE_T pd_ptr[PGENT_NR], uint64_t attr)
{
	pdpte_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pd_ptr)) | ARCH_PGE_NOT_LAST(attr);
}
PDE_T * get_pd(PDPTE_T * pdpt_ptr, uint64_t pdpte_idx)
{
	PDE_T * ret_val = NULL;
	if (pdpte_idx < PGENT_NR)
		ret_val = (PDE_T *)phys2virt((phys_addr)((uint64_t)pdpt_ptr[pdpte_idx].defs.PHYADDR << 12));
	return ret_val;
}

inline void fill_pde(PDE_T * pde_ptr, phys_addr paddr, uint64_t attr)
{
	pde_ptr->ENT = MASKF_2M((uint64_t)paddr) | ARCH_PGE_IS_LAST(attr);
}
phys_addr get_pgpaddr(PDE_T * pd_ptr, uint64_t pde_idx)
{
	phys_addr ret_val = NULL;
	if (pde_idx < PGENT_NR)
		ret_val = (phys_addr)((uint64_t)pd_ptr[pde_idx].defs.PHYADDR << 12);
	return ret_val;
}

//
void creat_fixed_kernel_pgmap()
{
	uint64_t attr = ARCH_PG_PRESENT | ARCH_PG_RW;
	fill_pml4e(&KERN_PML4[0], KERN_PDPT, attr);
	fill_pml4e(&KERN_PML4[256], KERN_PDPT, attr);

	for (int i = 0; i < PDPTE_NR; i++)
		fill_pdpte(&KERN_PDPT[i], &KERN_PD[i * PGENT_NR], attr);
}

void arch_page_domap(virt_addr virt, phys_addr phys, uint64_t attr, PML4E_T * kernel_cr3)
{
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// get pml4e
	PML4E_T * pml4e_ptr = kernel_cr3 + pml4e_idx;
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
		pdpte_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(&KERN_PD[pml4e_idx * pdpte_idx])) | ARCH_PGE_NOT_LAST(attr);
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

}