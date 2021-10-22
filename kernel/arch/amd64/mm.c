#include <sys/_null.h>

#include <string.h>
#include <stdbool.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"

#include "../../include/task.h"
#include "../../include/proto.h"
#include "../../include/ktypes.h"

#define SEG_NR	6

typedef struct mm_pair
{
	virt_addr	startp;
	long		pgnr;
} mmpr_s;

mmpr_s mmpr[SEG_NR];

mmpr_s * get_seginfo(task_s * task)
{
	mm_s * mm = task->mm_struct;
	reg_t user_rsp = get_stackframe(task)->rsp;

	virt_addr codepg_p = mmpr[0].startp = (virt_addr)CONFIG_PAGE_MASKF(mm->start_code);
	long codepg_nr = mmpr[0].pgnr = (CONFIG_PAGE_ALIGH(mm->end_code) - (reg_t)codepg_p) / CONFIG_PAGE_SIZE;
	virt_addr rodatapg_p = mmpr[1].startp = (virt_addr)CONFIG_PAGE_MASKF(mm->start_rodata);
	long rodatapg_nr = mmpr[1].pgnr = (CONFIG_PAGE_ALIGH(mm->end_rodata) - (reg_t)rodatapg_p) / CONFIG_PAGE_SIZE;
	virt_addr datapg_p = mmpr[2].startp = (virt_addr)CONFIG_PAGE_MASKF(mm->start_data);
	long datapg_nr = mmpr[2].pgnr = (CONFIG_PAGE_ALIGH(mm->end_data) - (reg_t)datapg_p) / CONFIG_PAGE_SIZE;
	virt_addr bsspg_p = mmpr[3].startp = (virt_addr)CONFIG_PAGE_MASKF(mm->start_bss);
	long bsspg_nr = mmpr[3].pgnr = (CONFIG_PAGE_ALIGH(mm->end_bss) - (reg_t)bsspg_p) / CONFIG_PAGE_SIZE;
	virt_addr brkpg_p = mmpr[4].startp = (virt_addr)CONFIG_PAGE_MASKF(mm->start_brk);
	long brkpg_nr = mmpr[4].pgnr = (CONFIG_PAGE_ALIGH(mm->end_brk) - (reg_t)brkpg_p) / CONFIG_PAGE_SIZE;
	virt_addr stackpg_p = mmpr[5].startp = (virt_addr)CONFIG_PAGE_MASKF(user_rsp);
	long stackpg_nr = mmpr[5].pgnr = (CONFIG_PAGE_ALIGH(mm->start_stack) - (reg_t)stackpg_p) / CONFIG_PAGE_SIZE;
}

void creat_exec_addrspace(task_s * task)
{
	stack_frame_s * context = get_stackframe(task);
	mm_s * mm = task->mm_struct;
	context->rsp = CONFIG_PAGE_MASKF(mm->start_stack) - CONFIG_PAGE_SIZE;
	get_seginfo(task);
	unsigned long attr = ARCH_PG_PRESENT | ARCH_PG_RW | ARCH_PG_USER;

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			Page_s * pgp = page_alloc();
			pgp->map_count++;
			pgp->attr = PG_PTable_Maped;
			arch_page_domap(mmpr[seg_idx].startp + pgnr * CONFIG_PAGE_SIZE,
							pgp->page_start_addr, attr, &mm->cr3);
		}
	}
}

void prepair_COW(task_s * task)
{
	mm_s * mm = task->mm_struct;
	get_seginfo(task);

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			// set all pages to read-only
			arch_page_clearattr(mmpr[seg_idx].startp + pgnr * CONFIG_PAGE_SIZE,
								ARCH_PG_RW, &mm->cr3);
			phys_addr paddr = 0;
			get_paddr(ARCH_PGS_ADDR(mm->cr3),
						mmpr[seg_idx].startp + pgnr * CONFIG_PAGE_SIZE, &paddr);
			Page_s * pgp = get_page(paddr);
			pgp->map_count++;
		}
	}

	refresh_arch_page();
}

int do_COW(task_s * task, virt_addr virt)
{
	int ret_val = 0;
	mm_s * mm = task->mm_struct;
	get_seginfo(task);

	reg_t orig_cr3 = read_cr3();
	reg_t new_cr3 = 0;
	phys_addr orig_paddr = 0;
	get_paddr(orig_cr3, virt, &orig_paddr);
	Page_s * orig_pgp = get_page(orig_paddr);
	Page_s * new_pgp = page_alloc();

	if (new_pgp != NULL)
	{
		new_pgp->attr = PG_PTable_Maped | PG_Referenced;
		if (!arch_page_duplicate(virt, new_pgp->page_start_addr, orig_cr3, &new_cr3))
		{
			new_pgp->map_count++;
			arch_page_setattr(virt, ARCH_PG_RW, &new_cr3);

			orig_pgp->map_count--;
			if (orig_pgp->map_count < 2)
				arch_page_setattr(virt, ARCH_PG_RW, &orig_cr3);

			virt_addr orig_pg_vaddr = (virt_addr)phys2virt(orig_pgp->page_start_addr);
			virt_addr new_pg_vaddr = (virt_addr)phys2virt(new_pgp->page_start_addr);
			memcpy(new_pg_vaddr, orig_pg_vaddr, CONFIG_PAGE_SIZE);

			pg_load_cr3(new_cr3);
			task->mm_struct->cr3 = new_cr3;
		}
	}

	return ret_val;
}

int check_addr_writable(reg_t cr2, task_s * task)
{
	int ret_val = false;
	mm_s * mm = task->mm_struct;
	reg_t rsp = get_stackframe(task)->rsp;

	if ((cr2 > mm->start_data && cr2 < mm->end_data) ||
		(cr2 > mm->start_bss && cr2 < mm->end_bss) ||
		(cr2 > mm->start_brk && cr2 < mm->end_brk) ||
		(cr2 > rsp && cr2 < mm->start_stack))
		ret_val = true;

	return ret_val;
}