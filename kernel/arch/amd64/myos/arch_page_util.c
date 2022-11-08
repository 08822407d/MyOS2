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

	attr = ARCH_PGS_ATTR(attr);
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
	if (pml4e_ptr->pgd == 0)
	{
		pud_t *pdpt_ptr = (pud_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pdpt_ptr != NULL)
		{
			// higher half
			(pml4e_ptr + pml4e_idx)->pgd =
			// lower half
			pml4e_ptr->pgd = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		pml4e_ptr->pgd | attr;
	}
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->pud == 0)
	{
		pmd_t *pd_ptr = (pmd_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->pud = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		pdpte_ptr->pud | attr;
	}
	// set pde
	pmd_t *PD_ptr 	= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->pud));
	pmd_t *pde_ptr	= PD_ptr + pde_idx;
	if (pde_ptr->pmd == 0)
	{
		pte_t *pt_ptr = (pte_t *)myos_memblock_alloc_normal(PGENT_SIZE, PGENT_SIZE);
		if (pt_ptr != NULL)
		{
			pde_ptr->pmd = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pt_ptr)) | attr;
		}
		else
		{
			ret_val = 2;
			goto fail_return;
		}
	}
	else
	{
		pde_ptr->pmd | attr;
	}
	// set pte
	pte_t *PT_ptr	= (pte_t *)myos_phys2virt(ARCH_PGS_ADDR(pde_ptr->pmd));	
	pte_t *pte_ptr	= PT_ptr + pte_idx;
	if (pte_ptr->pte == 0)
	{
		pte_ptr->pte = round_down(phys, PAGE_SIZE) | attr | _PAGE_PAT;
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
void myos_unmap_kernel_lowhalf(myos_atomic_T *um_flag)
{
	memset(init_top_pgt, 0, PGENT_SIZE / 2);
	myos_atomic_inc(um_flag);
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

//
inline void fill_pml4e(pgd_t *pml4e_ptr, pud_t pdpt_ptr[PGENT_NR], uint64_t attr)
{
	pml4e_ptr->pgd = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | ARCH_PGS_ATTR(attr);
}
pud_t *get_pdpt(pgd_t *pml4_ptr, uint64_t pml4e_idx)
{
	pud_t *ret_val = NULL;
	if (pml4e_idx < PGENT_NR)
		ret_val = (pud_t *)myos_phys2virt(pml4_ptr[pml4e_idx].defs.PHYADDR << 12);
	return ret_val;
}

inline void fill_pdpte(pud_t *pdpte_ptr, pmd_t pd_ptr[PGENT_NR], uint64_t attr)
{
	pdpte_ptr->pud = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | ARCH_PGS_ATTR(attr);
}
pmd_t *get_pd(pud_t *pdpt_ptr, uint64_t pdpte_idx)
{
	pmd_t *ret_val = NULL;
	if (pdpte_idx < PGENT_NR)
		ret_val = (pmd_t *)myos_phys2virt((uint64_t)pdpt_ptr[pdpte_idx].defs.PHYADDR << 12);
	return ret_val;
}

inline void fill_pde(pmd_t *pde_ptr, phys_addr_t paddr, uint64_t attr)
{
	pde_ptr->pmd = round_down(paddr, PAGE_SIZE) | ARCH_PGS_ATTR(attr) | _PAGE_PAT;
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
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(*cr3);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->pgd == 0)
	{
		pud_t *pdpt_ptr = (pud_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pdpt_ptr != NULL)
		{
			pml4e_ptr->pgd = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pdpt_ptr)) | attr;
		}
		else
		{
			ret_val = 4;
			goto fail_return;
		}
	}
	else
	{
		pml4e_ptr->pgd | attr;
	}
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->pud == 0)
	{
		pmd_t *pd_ptr = (pmd_t *)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (pd_ptr != NULL)
		{
			pdpte_ptr->pud = ARCH_PGS_ADDR(myos_virt2phys((virt_addr_t)pd_ptr)) | attr;
		}
		else
		{
			ret_val = 3;
			goto fail_return;
		}
	}
	else
	{
		pdpte_ptr->pud | attr;
	}
	// set pte
	pmd_t  *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->pud));	
	pmd_t  *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->pmd == 0)
	{
		pde_ptr->pmd = round_down(phys, PAGE_SIZE) | attr | _PAGE_PAT;
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
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(*cr3);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->pgd == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->pgd |= attr;
	// set pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->pud == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->pud |= attr;
	// set pte
	pmd_t  *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->pud));	
	pmd_t  *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->pmd == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->pmd |= attr | _PAGE_PAT;

fail_return:
	return ret_val;
}

// ret_val = 0 : success
int arch_page_clearattr(virt_addr_t virt, uint64_t attr, reg_t *cr3)
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
	phys_addr_t pml4_pa	= ARCH_PGS_ADDR(*cr3);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->pgd == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	else
		pml4e_ptr->pgd &= ~attr;
	// set pdpte
	pud_t  *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t  *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->pud == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	else
		pdpte_ptr->pud &= ~attr;
	// set pte
	pmd_t  *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->pud));	
	pmd_t  *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->pmd == 0)
	{
		ret_val = 2;
		goto fail_return;
	}
	else
		pde_ptr->pmd &= ~attr;
	// make sure the clear step will not clear the PAT flag
	pde_ptr->pmd |= _PAGE_PAT;

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
	if (orig_cr3 & ~_PAGE_RW)
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
	pgd_t *orig_pml4e_ptr	= (pgd_t *)orig_pml4_addr + pml4e_idx;
	pgd_t *new_pml4e_ptr	= (pgd_t *)new_pml4_addr+ pml4e_idx;
	virt_addr_t orig_pdpt_addr = myos_phys2virt(
			ARCH_PGS_ADDR(orig_pml4e_ptr->pgd));
	virt_addr_t new_pdpt_addr = 0;
	if (orig_pml4e_ptr->pgd & ~_PAGE_RW)
	{
		new_pdpt_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pdpt_addr != 0)
		{
			memcpy((void *)new_pdpt_addr, (void *)orig_pdpt_addr, PGENT_SIZE);
			new_pml4e_ptr->pgd = myos_virt2phys(new_pdpt_addr) |
					ARCH_PGS_ATTR(orig_pml4e_ptr->pgd);
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
			ARCH_PGS_ADDR(orig_pml4e_ptr->pgd)) + pdpte_idx;
	pud_t *new_pdpte_ptr	= (pud_t *)myos_phys2virt(
			ARCH_PGS_ADDR(new_pml4e_ptr->pgd)) + pdpte_idx;
	virt_addr_t orig_pd_addr = myos_phys2virt(
			ARCH_PGS_ADDR(orig_pdpte_ptr->pud));
	virt_addr_t new_pd_addr = 0;
	if (orig_pdpte_ptr->pud & ~_PAGE_RW)
	{
		new_pd_addr = (virt_addr_t)kzalloc(PGENT_SIZE, GFP_KERNEL);
		if (new_pd_addr != 0)
		{
			memcpy((void *)new_pd_addr, (void *)orig_pd_addr, PGENT_SIZE);
			new_pdpte_ptr->pud = myos_virt2phys(new_pd_addr) |
					ARCH_PGS_ATTR(orig_pdpte_ptr->pud);
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
	pmd_t *orig_pde_ptr = (pmd_t *)myos_phys2virt(
			ARCH_PGS_ADDR(orig_pdpte_ptr->pud)) + pde_idx;
	pmd_t *new_pde_ptr = (pmd_t *)myos_phys2virt(
			ARCH_PGS_ADDR(new_pdpte_ptr->pud)) + pde_idx;
	if (orig_pde_ptr->pmd & ~_PAGE_RW)
	{
		new_pde_ptr->pmd = phys | ARCH_PGS_ATTR(orig_pde_ptr->pmd);
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
	phys_addr_t pml4_pa	= (phys_addr_t)ARCH_PGS_ADDR(cr3);
	pgd_t *PML4_ptr		= (pgd_t *)myos_phys2virt(pml4_pa);
	pgd_t *pml4e_ptr	= PML4_ptr + pml4e_idx;
	if (pml4e_ptr->pgd == 0)
	{
		ret_val = 4;
		goto fail_return;
	}
	// get pdpte
	pud_t *PDPT_ptr 	= (pud_t *)myos_phys2virt(ARCH_PGS_ADDR(pml4e_ptr->pgd));
	pud_t *pdpte_ptr	= PDPT_ptr + pdpte_idx;
	if (pdpte_ptr->pud == 0)
	{
		ret_val = 3;
		goto fail_return;
	}
	// get pte
	pmd_t *PD_ptr		= (pmd_t *)myos_phys2virt(ARCH_PGS_ADDR(pdpte_ptr->pud));	
	pmd_t *pde_ptr		= PD_ptr + pde_idx;
	if (pde_ptr->pmd == 0)
	{
		ret_val = 2;
		goto fail_return;
	}

	*ret_phys = (pde_ptr->defs.PHYADDR << 12);

fail_return:
	return ret_val;
}