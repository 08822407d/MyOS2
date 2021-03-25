#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "../../include/proto.h"
#include "../../include/ktypes.h"

/* Storage for gdt, idt and tss. */
segdesc64_s		gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
gatedesc64_s	idt[IDT_SIZE] __aligned(GATEDESC_SIZE);
tss64_s			tss[CONFIG_MAX_CPUS];
desctblptr64_s	gdt_ptr;
desctblptr64_s	idt_ptr;

phy_addr vir2phy(vir_addr vir)
{
	extern char _k_phy_start, _k_vir_start;	/* in kernel.lds */
	uint64_t offset = (vir_addr) &_k_vir_start -
						(vir_addr) &_k_phy_start;
	return (phy_addr)(vir - offset);
}
vir_addr phy2vir(phy_addr phy)
{
	extern char _k_phy_start, _k_vir_start;	/* in kernel.lds */
	uint64_t offset = (vir_addr) &_k_vir_start -
						(vir_addr) &_k_phy_start;
	return (phy_addr)(phy + offset);
}

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
	sd->Pflag	= 1;
	// changable bits
	sd->Lflag	= Lflag;
	sd->Type	= type;
	sd->DPL		= privil;
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

	// gdt[0] has been set to 0 by memset
	set_codeseg(KERN_CS_INDEX, E_CODE, 0);
	set_dataseg(KERN_DS_INDEX, RW_DATA, 0);
	set_codeseg(USER_CS_INDEX, E_CODE, 3);
	set_dataseg(USER_DS_INDEX, RW_DATA, 3);

	gdt_ptr.limit = (uint16_t)sizeof(gdt) - 1;
	gdt_ptr.base  = (uint64_t)gdt;
}

void init_idt()
{
	memset(idt, 0, sizeof(idt));

	gate_table_s *gtbl;
	for ( gtbl = &(idt_init_table[0]); gtbl->gate_entry != NULL; gtbl++)
	{
		gatedesc64_s *curr_gate = &(idt[gtbl->vec_nr]);
		// fixed bits
		curr_gate->Present = 1;
		curr_gate->segslct = KERN_CS_SELECTOR;
		curr_gate->IST 	   = 0;
		// changable bits
		curr_gate->offs1 = (size_t)(gtbl->gate_entry) & 0xFFFF;
		curr_gate->offs2 = ((size_t)(gtbl->gate_entry) >> 16) & 0xFFFFFFFFFFFF;
		curr_gate->Type  = gtbl->type;
		curr_gate->DPL   = gtbl->DPL;
	}

	idt_ptr.limit = (uint16_t)(sizeof(idt) - 1);
	idt_ptr.base  = (uint64_t)idt;
}

void prot_init(void)
{
	init_gdt();
	init_idt();

	__asm__ __volatile__("	lgdt	%0						\n\
							lidt	%1						\n\
							mov 	%2, %%ss				\n\
							mov		$0, %%ax				\n\
							mov 	%%ax, %%ds				\n\
							mov 	%%ax, %%es				\n\
							mov 	%%ax, %%fs				\n\
							mov 	%%ax, %%gs				\n\
							xor		%%rax, %%rax			\n\
							movq	%3, %%rax				\n\
							pushq	%%rax					\n\
							leaq	reload_cs(%%rip), %%rax	\n\
							pushq	%%rax					\n\
							lretq							\n\
							reload_cs:						\n	"
						 :
						 :	"m"(gdt_ptr),
						 	"m"(idt_ptr),
						 	"r"(KERN_DS_SELECTOR),
							"rsi"((uint64_t)KERN_CS_SELECTOR)
						 :  "rax");


	// /* Set up a new post-relocate bootstrap pagetable so that
	// * we can map in VM, and we no longer rely on pre-relocated
	// * data.
	// */

	pg_clear();
	mem_init();
	pg_load();

	// prot_init_done = 1;
}