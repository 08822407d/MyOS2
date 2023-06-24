#include <linux/kernel/slab.h>
#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>
#include <linux/kernel/math.h>
#include <linux/mm/mm.h>
#include <linux/mm/memblock.h>
#include <linux/lib/string.h>
#include <asm/setup.h>
#include <asm/processor.h>
#include <asm/pgtable_64.h>

#include <obsolete/glo.h>
#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/dbg_utils.h>

#include <obsolete/archconst.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_glo.h>
#include <obsolete/arch_proto.h>

static int __init early_arch_page_domap(virt_addr_t virt, phys_addr_t phys, uint64_t attr)
{
	unsigned long ret_val = 0;

	attr = ARCH_PGS_ATTR(attr) & ~_PAGE_PAT;
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	while (init_top_pgt == NULL);
	
	// set pml4e
	phys_addr_t pml4_pa	= myos_virt2phys((virt_addr_t)init_top_pgt);
	pgd_t *PML4_ptr		= (pgd_t *)pml4_pa;
	pgd_t *pml4e_ptr	= PML4_ptr;
	if (pml4e_ptr->val == 0)
	{
		pud_t *pdpt_ptr = (pud_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pdpt_ptr != NULL)
		{
			// higher half
			(pml4e_ptr + pml4e_idx)->val =
			// lower half
			pml4e_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		pml4e_ptr->val | attr;
	}
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->val));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->val == 0)
	{
		pmd_t *pd_ptr = (pmd_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		pdpte_ptr->val | attr;
	}
	// set pde
	pmd_t *PD_ptr 	= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->val));
	pmd_t *pde_ptr	= PD_ptr + pde_idx;
	if (pde_ptr->val == 0)
	{
		pte_t *pt_ptr = (pte_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pt_ptr != NULL)
		{
			pde_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pt_ptr)) | attr;
		}
		else
		{
			ret_val = 2;
			goto fail_return;
		}
	}
	else
	{
		pde_ptr->val | attr;
	}
	// set pte
	pte_t *PT_ptr	= (pte_t *)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->val));	
	pte_t *pte_ptr	= PT_ptr + pte_idx;
	if (pte_ptr->val == 0)
	{
		pte_ptr->val = round_down(phys, PAGE_SIZE) | attr | _PAGE_PAT;
	}

fail_return:
	return ret_val;
}

int __init myos_init_memory_mapping(phys_addr_t base, size_t size)
{
	phys_addr_t pg_base = round_down(base, PAGE_SIZE);
	size_t pg_nr = (round_up(base + size, PAGE_SIZE) - pg_base) / PAGE_SIZE;
	u64 attr = PAGE_KERNEL;
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
void myos_unmap_kernel_lowhalf(atomic_t *um_flag)
{
	memset(init_top_pgt, 0, PGENT_SIZE / 2);
	atomic_inc(um_flag);
}


void myos_refresh_arch_page(void)
{
	uint64_t tempreg;
	asm volatile(	"movq	%%cr3,	%0	\n\t"
					"nop				\n\t"
					"movq	%0,	%%cr3	\n\t"
					"nop				\n\t"
				:	"=r"(tempreg)
				:
				:
				);
}

/*==============================================================================================*
 *																								*
 *==============================================================================================*/
int arch_page_domap(virt_addr_t virt, phys_addr_t phys, uint64_t attr, reg_t *cr3)
{
	unsigned long cr3_val = (unsigned long)*cr3;
	unsigned long ret_val = 0;
	if (kparam.init_flags.slab == 0)
	{
		color_printk(BLACK, ORANGE, "Slab not init yet.(arch_pg_domap())");
		while (1);
	}

	attr = ARCH_PGS_ATTR(attr) & ~_PAGE_PAT;
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		cr3_val |= attr;
	// set pml4e
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(cr3_val);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->val == 0)
	{
		pud_t *pdpt_ptr = (pud_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pdpt_ptr != NULL)
		{
			pml4e_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		pml4e_ptr->val | attr;
	}
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->val));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->val == 0)
	{
		pmd_t *pd_ptr = (pmd_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		pdpte_ptr->val | attr;
	}
	// set pde
	pmd_t *PD_ptr 	= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->val));
	pmd_t *pde_ptr	= PD_ptr + pde_idx;
	if (pde_ptr->val == 0)
	{
		pte_t *pt_ptr = (pte_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pt_ptr != NULL)
		{
			pde_ptr->val = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pt_ptr)) | attr;
		}
		else
		{
			ret_val = 2;
			goto fail_return;
		}
	}
	else
	{
		pde_ptr->val | attr;
	}
	// set pte
	pte_t *PT_ptr		= (pte_t *)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->val));	
	pte_t *pte_ptr		= PT_ptr + pte_idx;
	if (pte_ptr->val == 0)
	{
		pte_ptr->val = round_down(phys, PAGE_SIZE) | attr | _PAGE_PAT;
	}

	myos_refresh_arch_page();
	*cr3 = (reg_t)cr3_val;

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_setattr(virt_addr_t virt, uint64_t attr, reg_t *cr3)
{
	unsigned long cr3_val = (unsigned long)*cr3;
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr) & ~_PAGE_PAT;
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		cr3_val |= attr;
	// set pml4e
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(cr3_val);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->val == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->val |= attr;
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->val));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->val == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->val |= attr;
	// set pde
	pmd_t *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->val));	
	pmd_t *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->val == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->val |= attr;
	// set pte
	pte_t *PT_ptr		= (pte_t *)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->val));	
	pte_t *pte_ptr		= PT_ptr + pte_idx;
	if (pte_ptr->val == 0)
	{
		ret_val = 1;
		goto fail_return;
	}
	else
		pte_ptr->val |= attr | _PAGE_PAT;

	*cr3 = (reg_t)cr3_val;

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_clearattr(virt_addr_t virt, uint64_t attr, reg_t *cr3)
{
	unsigned long cr3_val = (unsigned long)*cr3;
	int ret_val = 0;
	attr = ARCH_PGS_ATTR(attr) & ~_PAGE_PAT;
	unsigned int pml4e_idx	= GETF_PGENT(virt >> SHIFT_PML4E);
	unsigned int pdpte_idx	= GETF_PGENT(virt >> SHIFT_PDPTE);
	unsigned int pde_idx	= GETF_PGENT(virt >> SHIFT_PDE);
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	// set cr3 attribute, although CPU ignored it,
	// software will use it
	if (cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}
	else
		cr3_val &= ~attr;
	// set pml4e
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(cr3_val);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->val == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->val &= ~attr;
	// set pdpte
	pud_t  *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->val));
	pud_t  *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->val == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->val &= ~attr;
	// set pde
	pmd_t *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->val));	
	pmd_t *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->val == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->val &= ~attr;
	// set pte
	pte_t *PT_ptr		= (pte_t *)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->val));	
	pte_t *pte_ptr		= PT_ptr + pte_idx;
	if (pte_ptr->val == 0)
	{
		ret_val = 1;
		goto fail_return;
	}
	else
		pte_ptr->val &= ~attr;
	// make sure the clear step will not clear the PAT flag
	pte_ptr->val |= _PAGE_PAT;

	*cr3 = (reg_t)cr3_val;

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
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	if (orig_cr3 == 0)
	{
		ret_val = -1;
		goto fail_return;
	}

	// if cr3 set bit read-only, copy PML4
	reg_t tmp_ret_cr3 = 0;
	virt_addr_t orig_pml4_addr	=
			myos_phys2virt(ARCH_PGS_ADDR((unsigned long)orig_cr3));
	virt_addr_t new_pml4_addr	= 0;
	if ((unsigned long)orig_cr3 & ~_PAGE_RW)
	{
		new_pml4_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pml4_addr != 0)
		{
			memcpy((void *)new_pml4_addr, (void *)orig_pml4_addr, PGENT_SIZE);
			tmp_ret_cr3 = (reg_t)(myos_virt2phys(new_pml4_addr) |
							ARCH_PGS_ATTR((unsigned long) orig_cr3));
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
	pgd_t *orig_pml4e_ptr	= (pgd_t *)orig_pml4_addr + pml4e_idx;
	pgd_t *new_pml4e_ptr	= (pgd_t *)new_pml4_addr+ pml4e_idx;
	virt_addr_t orig_pdpt_addr = myos_phys2virt(
			ARCH_PGS_ADDR(orig_pml4e_ptr->val));
	virt_addr_t new_pdpt_addr = 0;
	if (orig_pml4e_ptr->val & ~_PAGE_RW)
	{
		new_pdpt_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pdpt_addr != 0)
		{
			memcpy((void *)new_pdpt_addr, (void *)orig_pdpt_addr, PGENT_SIZE);
			new_pml4e_ptr->val = myos_virt2phys(new_pdpt_addr) |
					ARCH_PGS_ATTR(orig_pml4e_ptr->val);
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
	pud_t *orig_pdpte_ptr	= (pud_t *)myos_phys2virt(
			ARCH_PGS_ADDR(orig_pml4e_ptr->val)) + pdpte_idx;
	pud_t *new_pdpte_ptr	= (pud_t *)myos_phys2virt(
			ARCH_PGS_ADDR(new_pml4e_ptr->val)) + pdpte_idx;
	virt_addr_t orig_pd_addr = myos_phys2virt(
			ARCH_PGS_ADDR(orig_pdpte_ptr->val));
	virt_addr_t new_pd_addr = 0;
	if (orig_pdpte_ptr->val & ~_PAGE_RW)
	{
		new_pd_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pd_addr != 0)
		{
			memcpy((void *)new_pd_addr, (void *)orig_pd_addr, PGENT_SIZE);
			new_pdpte_ptr->val = myos_virt2phys(new_pd_addr) |
					ARCH_PGS_ATTR(orig_pdpte_ptr->val);
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

	// if pdpte set bit read-only, copy PT
	pmd_t *orig_pde_ptr	= (pmd_t *)myos_phys2virt(
			ARCH_PGS_ADDR(orig_pdpte_ptr->val)) + pde_idx;
	pmd_t *new_pde_ptr	= (pmd_t *)myos_phys2virt(
			ARCH_PGS_ADDR(new_pdpte_ptr->val)) + pde_idx;
	virt_addr_t orig_pt_addr = myos_phys2virt(
			ARCH_PGS_ADDR(orig_pde_ptr->val));
	virt_addr_t new_pt_addr = 0;
	if (orig_pde_ptr->val & ~_PAGE_RW)
	{
		new_pt_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pt_addr != 0)
		{
			memcpy((void *)new_pt_addr, (void *)orig_pt_addr, PGENT_SIZE);
			new_pde_ptr->val = myos_virt2phys(new_pt_addr) |
					ARCH_PGS_ATTR(orig_pde_ptr->val);
		}
		else
		{
			ret_val = 2;
			goto fail_return;
		}
	}
	else
	{
		new_pt_addr = orig_pt_addr;
	}

	// set pte
	pte_t *orig_pte_ptr = (pte_t *)myos_phys2virt(
			ARCH_PGS_ADDR(orig_pde_ptr->val)) + pte_idx;
	pte_t *new_pte_ptr = (pte_t *)myos_phys2virt(
			ARCH_PGS_ADDR(new_pde_ptr->val)) + pte_idx;
	if (orig_pte_ptr->val & ~_PAGE_RW)
	{
		new_pte_ptr->val = phys | ARCH_PGS_ATTR(orig_pte_ptr->val);
	}
	else
	{
		ret_val = 1;
		goto fail_return;
	}

	*ret_cr3 = (reg_t)((unsigned long)tmp_ret_cr3 |
						ARCH_PGS_ATTR((unsigned long)orig_cr3));
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
	unsigned int pte_idx	= GETF_PGENT(virt >> SHIFT_PTE);

	// get pml4e
	phys_addr_t pml4_pa	= (phys_addr_t)ARCH_PGS_ADDR((unsigned long)cr3);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->val == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	// get pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->val));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->val == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	// get pde
	pmd_t *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->val));	
	pmd_t *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->val == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	// get pte
	pte_t *PT_ptr		= (pte_t*)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->val));	
	pte_t *pte_ptr		= PT_ptr + pte_idx;
	if (pte_ptr->val == 0)
	{
		ret_val = 1;
		goto fail_return;
	}

	*ret_phys = (pte_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}