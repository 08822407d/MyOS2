#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "include/arch_glo.h"
#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"

#include "../../include/glo.h"
#include "../../include/param.h"
#include "../../include/proto.h"
#include "../../include/ktypes.h"
#include "../../include/task.h"


/* Storage for gdt, idt and tss. */
segdesc64_s		gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
gatedesc64_s	idt[IDT_SIZE] __aligned(GATEDESC_SIZE);

tss64_s **		tss_ptr_arr = NULL;
tss64_s			bsp_tmp_tss;
desctblptr64_s	gdt_ptr;
desctblptr64_s	idt_ptr;

/*===========================================================================*
 *								global functions							 *
 *===========================================================================*/
phys_addr virt2phys(virt_addr virt)
{
	extern char _k_phys_start, _k_virt_start;	/* in kernel.lds */
	uint64_t offset = (virt_addr) &_k_virt_start -
						(virt_addr) &_k_phys_start;
	return (phys_addr)(virt - offset);
}
virt_addr phys2virt(phys_addr phys)
{
	extern char _k_phys_start, _k_virt_start;	/* in kernel.lds */
	uint64_t offset = (virt_addr) &_k_virt_start -
						(virt_addr) &_k_phys_start;
	return (phys_addr)(phys + offset);
}

inline __always_inline void reload_gdt(desctblptr64_s * gdt_desc)
{
	__asm__ __volatile__("	lgdt	(%0)					\n\
							movq	%%rsp, %%rax			\n\
							mov 	%1, %%ss				\n\
							movq	%%rax, %%rsp			\n\
							mov		$0, %%ax				\n\
							mov 	%%ax, %%ds				\n\
							mov 	%%ax, %%es				\n\
							mov 	%%ax, %%fs				\n\
							mov 	%%ax, %%gs				\n\
							xor		%%rax, %%rax			\n\
							leaq	(. + 0xc)(%%rip), %%rax	\n\
							pushq	%2						\n\
							pushq	%%rax					\n\
							lretq							\n\
							xorq	%%rax, %%rax			\n	"
						 :
						 :	"r"(gdt_desc),
						 	"r"(KERN_SS_SELECTOR),
							"rsi"((uint64_t)KERN_CS_SELECTOR)
						 :  "rax");
}

inline __always_inline void reload_idt(desctblptr64_s * idt_desc)
{
	__asm__ __volatile__("	lidt	(%0)					\n	"
						 :
						 :	"r"(idt_desc)
						 :  );
}

inline __always_inline void reload_tss(uint64_t cpu_idx)
{
	__asm__ __volatile__("	xorq	%%rax,	%%rax			\n\
							mov		%0,		%%ax			\n\
							ltr		%%ax					\n	"
						 :
						 :	"r"((uint16_t)TSS_SELECTOR(cpu_idx))
						 :	"rax");
}

/*===========================================================================*
 *								private datas								 *
 *===========================================================================*/
gate_table_s exception_init_table[] = {
	{ divide_error, DIVIDE_ERR_VEC, TRAPGATE, KERN_PRIVILEGE, "#DE" },
	{ debug, DEBUG_VEC, TRAPGATE, KERN_PRIVILEGE, "#DB" },
	{ nmi, NMI_VEC, INTRGATE, KERN_PRIVILEGE, "NMI" },
	{ breakpoint_exception, BREAKPOINT_VEC, TRAPGATE, USER_PRIVILEGE, "#BP" },
	{ overflow, OVERFLOW_VEC, TRAPGATE, USER_PRIVILEGE, "#OF" },
	{ bounds_exceed, BOUNDS_VEC, TRAPGATE, USER_PRIVILEGE, "#BR" },
	{ invalid_opcode, INVAL_OP_VEC, TRAPGATE, KERN_PRIVILEGE, "#UD" },
	{ dev_not_available, DEV_NOT_AVAIL_VEC, TRAPGATE, KERN_PRIVILEGE, "#NM" },
	{ double_fault, DOUBLE_FAULT_VEC, TRAPGATE, KERN_PRIVILEGE, "#DF" },
	{ cotask_seg_overrun, COtask_sEG_VEC, TRAPGATE, KERN_PRIVILEGE, "FPU-SO" },
	{ invalid_tss, INVAL_TSS_VEC, TRAPGATE, KERN_PRIVILEGE, "#TS" },
	{ segment_not_present, SEG_NOT_PRES_VEC, TRAPGATE, KERN_PRIVILEGE, "#NP" },
	{ stack_segfault, STACK_SEGFAULT_VEC, TRAPGATE, KERN_PRIVILEGE, "#SS" },
	{ general_protection, GEN_PROT_VEC, TRAPGATE, KERN_PRIVILEGE, "#GP" },
	{ page_fault, PAGE_FAULT_VEC, TRAPGATE, KERN_PRIVILEGE, "#PF" },
	// EXCEPTION 15 INTEL RESERVED
	{ x87_fpu_error, X87_FPU_ERR_VEC, TRAPGATE, KERN_PRIVILEGE, "#MF" },
	{ alignment_check, ALIGNMENT_CHECK_VEC, TRAPGATE, KERN_PRIVILEGE, "#AC" },
	{ machine_check, MACHINE_CHECK_VEC, TRAPGATE, KERN_PRIVILEGE, "#MC" },
	{ simd_exception, SIMD_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE, "#XM" },
	{ virtualization_exception, VIRTUAL_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE, "#VE" },
	{ control_protection_exception, CTRL_PROT_EXCEP_VEC, TRAPGATE, KERN_PRIVILEGE, "#CP" },
	{ NULL, 0, 0, 0}
};

gate_table_s hwint_init_table[] = {
	{ hwint00, VECTOR( 0), INTRGATE, KERN_PRIVILEGE, "IRQ-00" },
	{ hwint01, VECTOR( 1), INTRGATE, KERN_PRIVILEGE, "IRQ-01" },
	{ hwint02, VECTOR( 2), INTRGATE, KERN_PRIVILEGE, "IRQ-02" },
	{ hwint03, VECTOR( 3), INTRGATE, KERN_PRIVILEGE, "IRQ-03" },
	{ hwint04, VECTOR( 4), INTRGATE, KERN_PRIVILEGE, "IRQ-04" },
	{ hwint05, VECTOR( 5), INTRGATE, KERN_PRIVILEGE, "IRQ-05" },
	{ hwint06, VECTOR( 6), INTRGATE, KERN_PRIVILEGE, "IRQ-06" },
	{ hwint07, VECTOR( 7), INTRGATE, KERN_PRIVILEGE, "IRQ-07" },
	{ hwint08, VECTOR( 8), INTRGATE, KERN_PRIVILEGE, "IRQ-08" },
	{ hwint09, VECTOR( 9), INTRGATE, KERN_PRIVILEGE, "IRQ-09" },
	{ hwint10, VECTOR(10), INTRGATE, KERN_PRIVILEGE, "IRQ-10" },
	{ hwint11, VECTOR(11), INTRGATE, KERN_PRIVILEGE, "IRQ-11" },
	{ hwint12, VECTOR(12), INTRGATE, KERN_PRIVILEGE, "IRQ-12" },
	{ hwint13, VECTOR(13), INTRGATE, KERN_PRIVILEGE, "IRQ-13" },
	{ hwint14, VECTOR(14), INTRGATE, KERN_PRIVILEGE, "IRQ-14" },
	{ hwint15, VECTOR(15), INTRGATE, KERN_PRIVILEGE, "IRQ-15" },
#ifdef USE_APIC
	{ hwint16, VECTOR(16), INTRGATE, KERN_PRIVILEGE, "IRQ-16" },
	{ hwint17, VECTOR(17), INTRGATE, KERN_PRIVILEGE, "IRQ-17" },
	{ hwint18, VECTOR(18), INTRGATE, KERN_PRIVILEGE, "IRQ-18" },
	{ hwint19, VECTOR(19), INTRGATE, KERN_PRIVILEGE, "IRQ-19" },
	{ hwint20, VECTOR(20), INTRGATE, KERN_PRIVILEGE, "IRQ-20" },
	{ hwint21, VECTOR(21), INTRGATE, KERN_PRIVILEGE, "IRQ-21" },
	{ hwint22, VECTOR(22), INTRGATE, KERN_PRIVILEGE, "IRQ-22" },
	{ hwint23, VECTOR(23), INTRGATE, KERN_PRIVILEGE, "IRQ-23" },
#endif
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

void set_sysseg(uint32_t index, SysSegType type, uint8_t privil)
{
	switch (type)
	{
		case TSS_AVAIL:
			{
				tss64_s * curr_tss = tss_ptr_arr[(index - TSS_INDEX(0)) / 2];
				TSSsegdesc_s * tss_segdesc = (TSSsegdesc_s *)&gdt[index];
				tss_segdesc->Limit1	= sizeof(*curr_tss) & 0xFFFF;
				tss_segdesc->Limit2	= (sizeof(*curr_tss) >> 16) & 0xF;
				tss_segdesc->Base1	= (uint64_t)curr_tss & 0xFFFFFF;
				tss_segdesc->Base2	= ((uint64_t)curr_tss >> 24) & 0xFFFFFFFFFF;
				tss_segdesc->Type	= type;
				tss_segdesc->DPL	= privil;
				tss_segdesc->AVL	=
				tss_segdesc->Pflag	= 1;
			}
			break;
		case INTRGATE:
		case TRAPGATE:
			{

			}
			break;

		default:
			break;
	}
}

/*===========================================================================*
 *								prot_bsp_init								     *
 *===========================================================================*/

void init_gdt()
{
	// gdt[0] has been set to 0 by memset
	set_codeseg(KERN_CS_INDEX, E_CODE, 0);
	set_dataseg(KERN_SS_INDEX, RW_DATA, 0);
	set_codeseg(USER_CS_INDEX, E_CODE, 3);
	set_dataseg(USER_SS_INDEX, RW_DATA, 3);

	gdt_ptr.limit = (uint16_t)sizeof(gdt) - 1;
	gdt_ptr.base  = (uint64_t)gdt;
}

void init_idt()
{
	gate_table_s * gtbl;
	for ( gtbl = &(exception_init_table[0]); gtbl->gate_entry != NULL; gtbl++)
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

	for ( gtbl = &(hwint_init_table[0]); gtbl->gate_entry != NULL; gtbl++)
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

void init_bsp_tss()
{
	extern char tmp_kstack_top;
	tss64_s *curr_tss = &bsp_tmp_tss;
	curr_tss->rsp0 = (reg_t)phys2virt(&tmp_kstack_top);
	curr_tss->rsp1 =
	curr_tss->rsp2 =
	curr_tss->ist1 =
	curr_tss->ist2 =
	curr_tss->ist3 =
	curr_tss->ist4 =
	curr_tss->ist5 =
	curr_tss->ist6 =
	curr_tss->ist7 = 0;
	// init bsp's tss by hand
	TSSsegdesc_s * tss_segdesc = (TSSsegdesc_s *)&gdt[TSS_INDEX(0)];
	tss_segdesc->Limit1	= sizeof(*curr_tss) & 0xFFFF;
	tss_segdesc->Limit2	= (sizeof(*curr_tss) >> 16) & 0xF;
	tss_segdesc->Base1	= (uint64_t)curr_tss & 0xFFFFFF;
	tss_segdesc->Base2	= ((uint64_t)curr_tss >> 24) & 0xFFFFFFFFFF;
	tss_segdesc->Type	= TSS_AVAIL;
	tss_segdesc->DPL	= KERN_PRIVILEGE;
	tss_segdesc->AVL	=
	tss_segdesc->Pflag	= 1;
}

void init_bsp_arch_data()
{
	// initate global architechture data
	init_gdt();
	init_idt();
	init_bsp_tss();
}

void reload_bsp_arch_data()
{
	reload_idt(&idt_ptr);
	reload_gdt(&gdt_ptr);
	reload_tss(0);
}

/*===========================================================================*
 *								prot_smp_init								     *
 *===========================================================================*/
void reload_percpu_arch_env(size_t cpu_idx)
{
	reload_idt(&idt_ptr);
	reload_gdt(&gdt_ptr);
	set_sysseg(TSS_INDEX(cpu_idx), TSS_AVAIL, KERN_PRIVILEGE);	// init bsp's gsbase
	reload_tss(cpu_idx);
}