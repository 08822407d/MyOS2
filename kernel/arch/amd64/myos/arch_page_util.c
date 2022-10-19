#include <linux/kernel/slab.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/math.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>
#include <asm/setup.h>
#include <asm/processor.h>

#include <obsolete/glo.h>
#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/dbg_utils.h>

#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_glo.h>
#include <obsolete/arch_proto.h>

PML4E_T	KERN_PML4[PGENT_NR] __aligned(PGENT_SIZE);
PDPTE_T	*KERN_PDPT;
PDE_T	*KERN_PD;


static int __init early_arch_page_domap(virt_addr_t virt, phys_addr_t phys, uint64_t attr)
{
	unsigned long ret_val = 0;

	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	while (KERN_PML4 == NULL);
	
	// set pml4e
	phys_addr_t pml4_paddr	= myos_virt2phys((virt_addr_t)KERN_PML4);
	PML4E_T *	PML4_ptr	= (PML4E_T *)pml4_paddr;
	PML4E_T *	pml4e_ptr	= PML4_ptr;
	if (pml4e_ptr->ENT == 0)
	{
		PDPTE_T * pdpt_ptr = (PDPTE_T *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pdpt_ptr != NULL)
		{
			// higher half
			(pml4e_ptr + pml4e_idx)->ENT =
			// lower half
			pml4e_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
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
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		PDE_T * pd_ptr = (PDE_T *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
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
	PDE_T *		PD_ptr		= (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		pde_ptr->ENT = round_down(phys, PAGE_SIZE) | attr | ARCH_PG_PAT;
	}

fail_return:
	return ret_val;
}

int __init myos_init_memory_mapping(phys_addr_t base, size_t size)
{
	phys_addr_t pg_base = round_down(base, PAGE_SIZE);
	size_t pg_nr = (round_up(base + size, PAGE_SIZE) - pg_base) / PAGE_SIZE;
	u64 attr = ARCH_PG_PRESENT | ARCH_PG_RW;
	for (long i = 0; i < pg_nr; i++)
	{
		early_arch_page_domap(
				myos_phys2virt(pg_base), pg_base, attr);
		pg_base += PAGE_SIZE;
	}
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
void myos_unmap_kernel_lowhalf(atomic_T *um_flag)
{
	memset(KERN_PML4, 0, PGENT_SIZE / 2);
	atomic_inc(um_flag);
}


void myos_refresh_arch_page(void)
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
	pml4e_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | ARCH_PGS_ATTR(attr);
}
PDPTE_T * get_pdpt(PML4E_T * pml4_ptr, uint64_t pml4e_idx)
{
	PDPTE_T * ret_val = NULL;
	if (pml4e_idx < PGENT_NR)
		ret_val = (PDPTE_T *)myos_phys2virt(pml4_ptr[pml4e_idx].defs.PHYADDR << 12);
	return ret_val;
}

inline void fill_pdpte(PDPTE_T * pdpte_ptr, PDE_T pd_ptr[PGENT_NR], uint64_t attr)
{
	pdpte_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | ARCH_PGS_ATTR(attr);
}
PDE_T * get_pd(PDPTE_T * pdpt_ptr, uint64_t pdpte_idx)
{
	PDE_T * ret_val = NULL;
	if (pdpte_idx < PGENT_NR)
		ret_val = (PDE_T *)myos_phys2virt((uint64_t)pdpt_ptr[pdpte_idx].defs.PHYADDR << 12);
	return ret_val;
}

inline void fill_pde(PDE_T * pde_ptr, phys_addr_t paddr, uint64_t attr)
{
	pde_ptr->ENT = round_down(paddr, PAGE_SIZE) | ARCH_PGS_ATTR(attr) | ARCH_PG_PAT;
}


/*==============================================================================================*
 *																								*
 *==============================================================================================*/
int arch_page_domap(virt_addr_t virt, phys_addr_t phys, uint64_t attr, reg_t * cr3)
{
	unsigned long ret_val = 0;
	if (kparam.init_flags.slab == 0)
	{
		color_printk(BLACK, ORANGE, "Slab not init yet.(arch_pg_domap())");
		while (1);
	}

	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

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
	phys_addr_t pml4_paddr	= ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr	= (PML4E_T *)myos_phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		PDPTE_T * pdpt_ptr = (PDPTE_T *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pdpt_ptr != NULL)
		{
			pml4e_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
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
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		PDE_T * pd_ptr = (PDE_T *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->ENT = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
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
	PDE_T *		PD_ptr		= (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		pde_ptr->ENT = round_down(phys, PAGE_SIZE) | attr | ARCH_PG_PAT;
	}

	myos_refresh_arch_page();

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_setattr(virt_addr_t virt, uint64_t attr, reg_t * cr3)
{
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

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
	phys_addr_t pml4_paddr	= ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr	= (PML4E_T *)myos_phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->ENT |= attr;
	// set pdpte
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->ENT |= attr;
	// set pte
	PDE_T *		PD_ptr		= (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
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
int arch_page_clearattr(virt_addr_t virt, uint64_t attr, reg_t * cr3)
{
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr);
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

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
	phys_addr_t pml4_paddr	= ARCH_PGS_ADDR(*cr3);
	PML4E_T *	PML4_ptr	= (PML4E_T *)myos_phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->ENT &= ~attr;
	// set pdpte
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->ENT &= ~attr;
	// set pte
	PDE_T *		PD_ptr		= (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
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
int arch_page_duplicate(virt_addr_t virt, phys_addr_t phys, reg_t orig_cr3, reg_t * ret_cr3)
{
	int ret_val = 0;
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

	if (orig_cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}

	// if cr3 set bit read-only, copy PML4
	reg_t tmp_ret_cr3 = 0;
	virt_addr_t orig_pml4_addr	= myos_phys2virt(ARCH_PGS_ADDR(orig_cr3));
	virt_addr_t new_pml4_addr	= 0;
	if (orig_cr3 & ~ARCH_PG_RW)
	{
		new_pml4_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pml4_addr != 0)
		{
			memcpy((void *)new_pml4_addr, (void *)orig_pml4_addr, PGENT_SIZE);
			tmp_ret_cr3 = myos_virt2phys(new_pml4_addr) | ARCH_PGS_ATTR(orig_cr3);
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
	virt_addr_t orig_pdpt_addr = myos_phys2virt(ARCH_PGS_ADDR(orig_pml4e_ptr->ENT));
	virt_addr_t new_pdpt_addr = 0;
	if (orig_pml4e_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pdpt_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pdpt_addr != 0)
		{
			memcpy((void *)new_pdpt_addr, (void *)orig_pdpt_addr, PGENT_SIZE);
			new_pml4e_ptr->ENT = myos_virt2phys(new_pdpt_addr) | ARCH_PGS_ATTR(orig_pml4e_ptr->ENT);
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
	PDPTE_T *	orig_pdpte_ptr	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(orig_pml4e_ptr->ENT)) + pdpte_idx;
	PDPTE_T *	new_pdpte_ptr	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(new_pml4e_ptr->ENT)) + pdpte_idx;
	virt_addr_t orig_pd_addr = myos_phys2virt(ARCH_PGS_ADDR(orig_pdpte_ptr->ENT));
	virt_addr_t new_pd_addr = 0;
	if (orig_pdpte_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pd_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pd_addr != 0)
		{
			memcpy((void *)new_pd_addr, (void *)orig_pd_addr, PGENT_SIZE);
			new_pdpte_ptr->ENT = myos_virt2phys(new_pd_addr) | ARCH_PGS_ATTR(orig_pdpte_ptr->ENT);
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
	PDE_T * orig_pde_ptr = (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(orig_pdpte_ptr->ENT)) + pde_idx;
	PDE_T * new_pde_ptr = (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(new_pdpte_ptr->ENT)) + pde_idx;
	if (orig_pde_ptr->ENT & ~ARCH_PG_RW)
	{
		new_pde_ptr->ENT = phys | ARCH_PGS_ATTR(orig_pde_ptr->ENT);
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
int get_paddr(reg_t cr3, virt_addr_t virt, phys_addr_t *ret_phys)
{
	int ret_val = 0;

	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);

	// get pml4e
	phys_addr_t pml4_paddr	= (phys_addr_t)ARCH_PGS_ADDR(cr3);
	PML4E_T *	PML4_ptr	= (PML4E_T *)myos_phys2virt(pml4_paddr);
	PML4E_T *	pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->ENT == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	// get pdpte
	PDPTE_T *	PDPT_ptr 	= (PDPTE_T *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->ENT));
	PDPTE_T *	pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->ENT == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	// get pte
	PDE_T *		PD_ptr		= (PDE_T *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->ENT));	
	PDE_T *		pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->ENT == 0)
	{
		ret_val = 2;
		goto fail_return;
	}

	*ret_phys = (pde_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}