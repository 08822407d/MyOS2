#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_task.h"

#include "../../include/task.h"

#define SEG_NR	6

typedef struct mm_pair
{
	virt_addr	startp;
	long		pgnr;
} mmpr_s;


void set_allpage_readonly(task_s * task)
{
	mmpr_s mmpr[SEG_NR];

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

	for (int seg_idx = 0; seg_idx < SEG_NR; seg_idx++)
	{
		for (int pgnr = 0; pgnr < mmpr[seg_idx].pgnr; pgnr++)
		{
			arch_page_clearattr(mmpr[seg_idx].startp + pgnr * CONFIG_PAGE_SIZE,
								ARCH_PG_RW, mm->cr3);
		}
	}

	refresh_arch_page();
}