#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "../../proto.h"

/* Storage for gdt, idt and tss. */
segdesc64_s gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
uint16_t gdtslctr[GDT_SIZE] __aligned(sizeof(uint16_t));
gatedesc64_s idt[IDT_SIZE] __aligned(GATEDESC_SIZE);
tss64_s tss[CONFIG_MAX_CPUS];
desctblptr64_s gdt_ptr;
desctblptr64_s idt_ptr;

gate_table_s idt_init_table[] = {
	{ divide_error, DIVIDE_ERR_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ debug, DEBUG_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ nmi, NMI_VEC, INTRGATE, KERN_PRIVILEGE },
	{ breakpoint_exception, BREAKPOINT_VEC, TRAPGATE, USER_PRIVILEGE },
	{ overflow, OVERFLOW_VEC, TRAPGATE, USER_PRIVILEGE },
	{ bounds_exceed, BOUNDS_VEC, TRAPGATE, USER_PRIVILEGE },
	{ invalid_opcode, INVAL_OP_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ dev_not_available, DEV_NOT_AVAIL_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ double_fault, DOUBLE_FAULT_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ coproc_seg_overrun, COPROC_SEG_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ invalid_tss, INVAL_TSS_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ segment_not_present, SEG_NOT_PRES_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ stack_segfault, STACK_SEGFAULT_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ general_protection, GEN_PROT_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ page_fault, PAGE_FAULT_VEC, TRAPGATE, KERN_PRIVILEGE },
	// EXCEPTION 15 INTEL RESERVED
	{ x87_fpu_error, X87_FPU_ERR_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ alignment_check, ALIGNMENT_CHECK_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ machine_check, MACHINE_CHECK_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ simd_exception, SIMD_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ virtualization_exception, VIRTUAL_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ control_protection_exception, CTRL_PROT_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE },
	{ NULL, 0, 0, 0}
};
/*===========================================================================*
 *								initiate segs							     *
 *===========================================================================*/

void set_commseg(uint32_t index, CommSegType type, uint8_t privil, uint8_t Lflag)
{
	segdesc64_s *sd = &(gdt[index]);
	sd->Sflag	= 1;
	sd->Present	= 1;
	// changable bits
	sd->Lflag	= Lflag;
	sd->Type	= type;
	sd->Privil	= privil;

	gdtslctr[index] = (index << 3) | privil;
}

void set_codeseg(uint32_t index, CommSegType type, uint8_t privil)
{
	set_commseg(index, type, privil, 1);
}

void set_dataseg(uint32_t index, CommSegType type, uint8_t privil)
{
	set_commseg(index, type, privil, 0);
}	


/*===========================================================================*
 *								prot_init								     *
 *===========================================================================*/

void init_gdt()
{
	memset(gdt, 0, sizeof(gdt));
	memset(gdtslctr, 0, sizeof(gdtslctr));

	// gdt[0] has been set to 0 by memset
	set_codeseg(KERN_CS_INDEX, E_CODE, 0);
	set_dataseg(KERN_DS_INDEX, RW_DATA, 0);
	set_codeseg(USER_CS_INDEX, E_CODE, 3);
	set_dataseg(USER_DS_INDEX, RW_DATA, 3);

	// &gdt[TSS_INDEX_FIRST]	= 0x0000000000000000;

	gdt_ptr.base = &gdt[NULL_DESC_INDEX];
	gdt_ptr.limit = sizeof(gdt) - 1;
}

void init_idt()
{
	memset(gdt, 0, sizeof(idt));

	gate_table_s *gtbl;
	for ( gtbl = &(idt_init_table[0]); gtbl->gate_entry; gtbl++)
	{
		gatedesc64_s *curr_gate = &(idt[gtbl->vec_nr]);
		// fixed bits
		curr_gate->Present = 1;
		curr_gate->segslct = gdtslctr[KERN_CS_INDEX];
		curr_gate->IST 	   = 0;
		// changable bits
		curr_gate->offs1 = (uint16_t)(gtbl->gate_entry) & 0xFFFF;
		curr_gate->offs2 = (uint64_t)(gtbl->gate_entry) >> 16;
		curr_gate->Type  = gtbl->type;
		curr_gate->DPL   = gtbl->DPL;
	}

}

void prot_init(void)
{
	init_gdt();
	init_idt();


	__asm__ __volatile__("	lgdt	%0			\n\
							mov		%1, %%ds	\n\
							mov		%1, %%es	\n\
							mov		%1, %%fs	\n\
							mov		%1, %%gs	\n\
							mov		%1, %%ss	\n	"
						 :
						 :	"m"(gdt_ptr),
						 	"a"(gdtslctr[KERN_DS_INDEX]));

	__asm__ __volatile__("lidt %0": : "m"(idt_ptr));

	// /* Set up a new post-relocate bootstrap pagetable so that
	// * we can map in VM, and we no longer rely on pre-relocated
	// * data.
	// */

	// pg_clear();
	// pg_identity(&kinfo); /* Still need 1:1 for lapic and video mem and such. */
	// pg_mapkernel();
	// pg_load();

	// prot_init_done = 1;
}