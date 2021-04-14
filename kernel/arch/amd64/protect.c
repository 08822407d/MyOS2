#include <lib/string.h>
#include <lib/assert.h>
#include <sys/types.h>

#include "../../include/param.h"
#include "include/archconst.h"
#include "include/archtypes.h"
#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "../../include/proto.h"
#include "../../include/ktypes.h"

extern kinfo_s kparam;
/* Storage for gdt, idt and tss. */
segdesc64_s		gdt[GDT_SIZE] __aligned(SEGDESC_SIZE);
gatedesc64_s	idt[IDT_SIZE] __aligned(GATEDESC_SIZE);
tss64_s			tss[CONFIG_MAX_CPUS];
// char			cpu_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE] __aligned(CONFIG_KSTACK_SIZE);
char			ist_stacks[CONFIG_MAX_CPUS][CONFIG_KSTACK_SIZE] __aligned(CONFIG_KSTACK_SIZE);
desctblptr64_s	gdt_ptr;
desctblptr64_s	idt_ptr;

char *(intr_name[IDT_SIZE]);

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
	{ coproc_seg_overrun, COPROC_SEG_VEC, TRAPGATE, KERN_PRIVILEGE, "FPU-SO" },
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
				tss64_s * curr_tss = &tss[(index - TSS_INDEX_FIRST) / 2];
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
 *								prot_init								     *
 *===========================================================================*/

void init_gdt()
{
	memset(gdt, 0, sizeof(gdt));

	// gdt[0] has been set to 0 by memset
	set_codeseg(KERN_CS_INDEX, E_CODE, 0);
	set_dataseg(KERN_SS_INDEX, RW_DATA, 0);
	set_codeseg(USER_CS_INDEX, E_CODE, 3);
	set_dataseg(USER_SS_INDEX, RW_DATA, 3);

	for (int i = 0; i < 1; i++)
	{
		set_sysseg(TSS_INDEX(i), TSS_AVAIL, KERN_PRIVILEGE);
	}

	gdt_ptr.limit = (uint16_t)sizeof(gdt) - 1;
	gdt_ptr.base  = (uint64_t)gdt;
}

void init_idt()
{
	memset(idt, 0, sizeof(idt));

	gate_table_s *gtbl;
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

		intr_name[gtbl->vec_nr] = gtbl->name;
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

		intr_name[gtbl->vec_nr] = gtbl->name;
	}

	idt_ptr.limit = (uint16_t)(sizeof(idt) - 1);
	idt_ptr.base  = (uint64_t)idt;
}

void init_tss(unsigned int cpu_idx)
{
	tss64_s *curr_tss = &tss[cpu_idx];
	curr_tss->rsp0 = 0;
	curr_tss->rsp1 =
	curr_tss->rsp2 =
	curr_tss->ist1 =
	curr_tss->ist2 =
	curr_tss->ist3 =
	curr_tss->ist4 =
	curr_tss->ist5 =
	curr_tss->ist6 =
	curr_tss->ist7 = (uint64_t)&ist_stacks[cpu_idx] + CONFIG_KSTACK_SIZE;
}

void prot_init(void)
{
	init_gdt();
	init_idt();
	init_tss(0);

	__asm__ __volatile__("	lgdt	%0						\n\
							lidt	%1						\n\
							movq	%%rsp, %%rax			\n\
							mov 	%2, %%ss				\n\
							movq	%%rax, %%rsp			\n\
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
							reload_cs:						\n\
							xorq	%%rax, %%rax			\n\
							mov		%4, %%ax				\n\
							ltr		%%ax					\n	"
						 :
						 :	"m"(gdt_ptr),
						 	"m"(idt_ptr),
						 	"r"(KERN_SS_SELECTOR),
							"rsi"((uint64_t)KERN_CS_SELECTOR),
							"r"((uint16_t)TSS_SELECTOR(0))
						 :  "rax");


	// /* Set up a new post-relocate bootstrap pagetable so that
	// * we can map in VM, and we no longer rely on pre-relocated
	// * data.
	// */

	pg_clear();
	mem_init();
	extern PML4E PML4[PGENT_NR];
	pg_load_cr3(PML4);

	// prot_init_done = 1;
	init_i8259();
}