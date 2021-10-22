#include <sys/types.h>
#include <sys/_null.h>
#include <lib/string.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_glo.h"
#include "include/arch_proto.h"

#include "../../include/glo.h"
#include "../../include/proto.h"
#include "../../include/param.h"
#include "../../include/ktypes.h"
#include "../../include/const.h"
#include "../../include/printk.h"

/* Storage for PML4, PDPT and PD. */
PML4E_T	KERN_PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE_T	KERN_PDPT[PGENT_NR] __aligned(PGENT_SIZE);
PDE_T	KERN_PD[PDE_NR] __aligned(PGENT_SIZE);

// this value is also loaded by APboot assembly code
phys_addr kernel_cr3 = 0;

void creat_fixed_kernel_pgmap();
void map_fix_kernel_pages();

void reload_arch_page()
{
	int pdpte_nr = PDPTE_NR;
	int pde_nr = PDE_NR;

	kernel_cr3 = virt2phys(&KERN_PML4);

	creat_fixed_kernel_pgmap();
	map_fix_kernel_pages();

	pg_load_cr3((reg_t)kernel_cr3);
	// set init flag
	kparam.arch_init_flags.reload_bsp_arch_page = 1;
}

void map_fix_kernel_pages()
{
	// map kernel software used pages
	phys_addr k_phy_pgbase = 0;
	virt_addr k_vir_pgbase = (virt_addr)phys2virt(0);
	uint64_t attr = ARCH_PG_PRESENT | ARCH_PG_RW | ARCH_PG_USER;
	long pde_nr   = CONFIG_PAGE_MASKF(kparam.max_phys_mem) / CONFIG_PAGE_SIZE;
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

reg_t read_cr3()
{
	reg_t ret_val = 0;
	__asm__ __volatile__(	"movq	%%cr3,	%0 	\n\t"
							"nop				\n\t"
						:	"=r"(ret_val)
						:
						:
						);
	return ret_val;
}

void pg_load_cr3(reg_t cr3)
{
	__asm__ __volatile__(	"movq	%0, %%cr3	\n\t"
							"nop				\n\t"
						:
						:	"r"(cr3)
						:
						);
}

void refresh_arch_page(void)
{
	uint64_t tempreg;
	__asm__ __volatile__(	"movq	%%cr3,	%0	\n\t"
							"nop				\n\t"
							"movq	%0,	%%cr3	\n\t"
							"nop				\n\t"
						:	"=r"(tempreg)
						:
						:
						);
}

//
inline void fill_pml4e(PML4E_T * pml4e_ptr, PDPTE_T pdpt_ptr[PGENT_NR], uint64_t attr)
{
	pml4e_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pdpt_ptr)) | ARCH_PGS_ATTR(attr);
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
	pdpte_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pd_ptr)) | ARCH_PGS_ATTR(attr);
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
	pde_ptr->ENT = MASKF_2M((uint64_t)paddr) | ARCH_PGS_ATTR(attr) | ARCH_PG_PAT;
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

void unmap_kernel_lowhalf()
{
	memset(KERN_PML4, 0, PGENT_SIZE / 2);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
int arch_page_domap(virt_addr virt, phys_addr phys, uint64_t attr, reg_t * cr3)
{
	unsigned long ret_val = 0;
	if (kparam.init_flags.slab == 0)
	{
		color_printk(BLACK, ORANGE, "Slab not init yet.(arch_pg_domap())");
		while (1);
	}

	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		*cr3 |= attr;
	// set pml4e
	phys_addr pml4_paddr = (phys_addr)ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr		= (PML4E_T *)phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr		= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		PDPTE_T * pdpt_ptr = (PDPTE_T *)kmalloc(PGENT_SIZE);
		if (pdpt_ptr != NULL)
		{
			memset(pdpt_ptr, 0, PGENT_SIZE);
			pml4e_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pdpt_ptr)) | attr;
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		pml4e_ptr->ENT | attr;
	}
	// set pdpte
	PDPTE_T *	PDPT_ptr 		= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr		= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		PDE_T * pd_ptr = (PDE_T *)kmalloc(PGENT_SIZE);
		if (pd_ptr != NULL)
		{
			memset(pd_ptr, 0, PGENT_SIZE);
			pdpte_ptr->ENT = ARCH_PGS_ADDR((uint64_t)virt2phys(pd_ptr)) | attr;
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		pdpte_ptr->ENT | attr;
	}
	// set pte
	PDE_T *		PD_ptr			= (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr			= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		pde_ptr->ENT = MASKF_2M((uint64_t)phys) | attr | ARCH_PG_PAT;
	}

	refresh_arch_page();

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_setattr(virt_addr virt, uint64_t attr, reg_t * cr3)
{
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		*cr3 |= attr;
	// set pml4e
	phys_addr pml4_paddr = (phys_addr)ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr		= (PML4E_T *)phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr		= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->ENT |= attr;
	// set pdpte
	PDPTE_T *	PDPT_ptr 		= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr		= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->ENT |= attr;
	// set pte
	PDE_T *		PD_ptr			= (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr			= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->ENT |= attr | ARCH_PG_PAT;

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_clearattr(virt_addr virt, uint64_t attr, reg_t * cr3)
{
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		*cr3 &= ~attr;
	// set pml4e
	phys_addr pml4_paddr = (phys_addr)ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr		= (PML4E_T *)phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr		= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->ENT &= ~attr;
	// set pdpte
	PDPTE_T *	PDPT_ptr 		= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr		= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->ENT &= ~attr;
	// set pte
	PDE_T *		PD_ptr			= (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr			= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->ENT &= ~attr;
	// make sure the clear step will not clear the PAT flag
	pde_ptr->ENT |= ARCH_PG_PAT;

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_duplicate(virt_addr virt, phys_addr phys, reg_t orig_cr3, reg_t * ret_cr3)
{
	int ret_val = 0;
	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	if (orig_cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}

	// if cr3 set bit read-only, copy PML4
	reg_t tmp_ret_cr3 = 0;
	virt_addr orig_pml4_addr = phys2virt((phys_addr)ARCH_PGS_ADDR(orig_cr3));
	virt_addr new_pml4_addr = NULL;
	if (orig_cr3 & ~ARCH_PG_RW)
	{
		new_pml4_addr = (virt_addr)kmalloc(PGENT_SIZE);
		if (new_pml4_addr != NULL)
		{
			memset(new_pml4_addr, 0, PGENT_SIZE);
			memcpy((virt_addr)new_pml4_addr, orig_pml4_addr, PGENT_SIZE);
			tmp_ret_cr3 = (reg_t)virt2phys(new_pml4_addr) | ARCH_PGS_ATTR(orig_cr3);
		}
		else
		{
			ret_val = 5;
			goto fail_return;
		}
	}
	else
	{
		new_pml4_addr = orig_pml4_addr;
	}

	// if pml4e set bit read-only, copy PDPT
	PML4E_T *	orig_pml4e_ptr	= (PML4E_T *)orig_pml4_addr + pml4e_idx;
	PML4E_T *	new_pml4e_ptr	= (PML4E_T *)new_pml4_addr+ pml4e_idx;
	virt_addr orig_pdpt_addr = phys2virt((virt_addr)ARCH_PGS_ADDR(orig_pml4e_ptr->ENT));
	virt_addr new_pdpt_addr = NULL;
	if (orig_pml4e_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pdpt_addr = (virt_addr)kmalloc(PGENT_SIZE);
		if (new_pdpt_addr != NULL)
		{
			memset(new_pdpt_addr, 0, PGENT_SIZE);
			memcpy(new_pdpt_addr, orig_pdpt_addr, PGENT_SIZE);
			new_pml4e_ptr->ENT = (reg_t)virt2phys(new_pdpt_addr) | ARCH_PGS_ATTR(orig_pml4e_ptr->ENT);
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		new_pdpt_addr = orig_pdpt_addr;
	}

	// if pdpte set bit read-only, copy PD
	PDPTE_T *	orig_pdpte_ptr	= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(orig_pml4e_ptr->ENT)) + pdpte_idx;
	PDPTE_T *	new_pdpte_ptr	= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(new_pml4e_ptr->ENT)) + pdpte_idx;
	virt_addr orig_pd_addr = phys2virt((virt_addr)ARCH_PGS_ADDR(orig_pdpte_ptr->ENT));
	virt_addr new_pd_addr = NULL;
	if (orig_pdpte_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pd_addr = (virt_addr)kmalloc(PGENT_SIZE);
		if (new_pd_addr != NULL)
		{
			memset(new_pd_addr, 0, PGENT_SIZE);
			memcpy(new_pd_addr, orig_pd_addr, PGENT_SIZE);
			new_pdpte_ptr->ENT = (reg_t)virt2phys(new_pd_addr) | ARCH_PGS_ATTR(orig_pdpte_ptr->ENT);
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		new_pd_addr = orig_pd_addr;
	}

	// set pde
	PDE_T * orig_pde_ptr = (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(orig_pdpte_ptr->ENT)) + pde_idx;
	PDE_T * new_pde_ptr = (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(new_pdpte_ptr->ENT)) + pde_idx;
	if (orig_pde_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pde_ptr->ENT = (reg_t)phys | ARCH_PGS_ATTR(orig_pde_ptr->ENT);
	}
	else
	{
		ret_val = 2;
		goto fail_return;
	}

	*ret_cr3 = tmp_ret_cr3 | ARCH_PGS_ATTR(orig_cr3);
fail_return:
	return ret_val;
}

// ret_val = 0 : success
int get_paddr(reg_t cr3, virt_addr virt, phys_addr *ret_phys)
{
	int ret_val = 0;

	unsigned int pml4e_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT((uint64_t)virt >> SHIFT_PDE);

	// get pml4e
	phys_addr pml4_paddr = (phys_addr)ARCH_PGS_ADDR(cr3);
	PML4E_T *	PML4_ptr		= (PML4E_T *)phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr		= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	// get pdpte
	PDPTE_T *	PDPT_ptr 		= (PDPTE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr		= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	// get pte
	PDE_T *		PD_ptr			= (PDE_T *)phys2virt((phys_addr)ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr			= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		ret_val = 2;
		goto fail_return;
	}

	*ret_phys = (phys_addr)((uint64_t)pde_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}