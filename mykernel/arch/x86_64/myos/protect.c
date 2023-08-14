#include <linux/lib/string.h>
#include <asm/fsgsbase.h>
#include <asm/setup.h>
#include <asm/desc.h>

#include <obsolete/glo.h>
#include <obsolete/proto.h>
#include <obsolete/ktypes.h>
#include <linux/kernel/sched.h>
#include <linux/mm/memblock.h>

#include <obsolete/arch_glo.h>
#include <obsolete/archtypes.h>
#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
#include <obsolete/apic.h>


/* Storage for gdt, idt and tss. */
segdesc64_T		gdt[GDT_ENTRIES] __aligned(PAGE_SIZE);
gatedesc64_T	idt_table[IDT_ENTRIES] __aligned(sizeof(gatedesc64_T));

tss64_T			tss_ptr_arr[CONFIG_NR_CPUS] __aligned(sizeof(size_t));

/*==============================================================================================*
 *										private datas									 		*
 *==============================================================================================*/
gate_table_s exception_init_table[] = {
	{ divide_error, DIVIDE_ERR_VEC, GATE_TRAP, KRNL_RPL, 0, "#DE" },
	{ debug, DEBUG_VEC, GATE_TRAP, KRNL_RPL, 0, "#DB" },
	{ nmi, NMI_VEC, GATE_INTERRUPT, KRNL_RPL, 0, "NMI" },
	{ breakpoint_exception, BREAKPOINT_VEC, GATE_TRAP, USER_RPL, 0, "#BP" },
	{ overflow, OVERFLOW_VEC, GATE_TRAP, USER_RPL, 0, "#OF" },
	{ bounds_exceed, BOUNDS_VEC, GATE_TRAP, USER_RPL, 0, "#BR" },
	{ invalid_opcode, INVAL_OP_VEC, GATE_TRAP, KRNL_RPL, 0, "#UD" },
	{ dev_not_available, DEV_NOT_AVAIL_VEC, GATE_TRAP, KRNL_RPL, 0, "#NM" },
	{ double_fault, DOUBLE_FAULT_VEC, GATE_TRAP, KRNL_RPL, 0, "#DF" },
	{ cotask_seg_overrun, COtask_sEG_VEC, GATE_TRAP, KRNL_RPL, 0, "FPU-SO" },
	{ invalid_tss, INVAL_TSS_VEC, GATE_TRAP, KRNL_RPL, 0, "#TS" },
	{ segment_not_present, SEG_NOT_PRES_VEC, GATE_TRAP, KRNL_RPL, 0, "#NP" },
	{ stack_segfault, STACK_SEGFAULT_VEC, GATE_TRAP, KRNL_RPL, 0, "#SS" },
	{ general_protection, GEN_PROT_VEC, GATE_TRAP, KRNL_RPL, 0, "#GP" },
	{ page_fault, PAGE_FAULT_VEC, GATE_TRAP, KRNL_RPL, 0, "#PF" },
	// EXCEPTION 15 INTEL RESERVED
	{ x87_fpu_error, X87_FPU_ERR_VEC, GATE_TRAP, KRNL_RPL, 0, "#MF" },
	{ alignment_check, ALIGNMENT_CHECK_VEC, GATE_TRAP, KRNL_RPL, 0, "#AC" },
	{ machine_check, MACHINE_CHECK_VEC, GATE_TRAP, KRNL_RPL, 0, "#MC" },
	{ simd_exception, SIMD_EXCEP_VEC, GATE_TRAP, KRNL_RPL, 0, "#XM" },
	{ virtualization_exception, VIRTUAL_EXCEP_VEC, GATE_TRAP, KRNL_RPL, 0, "#VE" },
	{ control_protection_exception, CTRL_PROT_EXCEP_VEC, GATE_TRAP, KRNL_RPL, 0, "#CP" },
	{ NULL, 0, 0, 0}
};

gate_table_s hwint_init_table[] = {
	{ hwint00, VECTOR_IRQ( 0), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-00" },
	{ hwint01, VECTOR_IRQ( 1), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-01" },
	{ hwint02, VECTOR_IRQ( 2), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-02" },
	{ hwint03, VECTOR_IRQ( 3), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-03" },
	{ hwint04, VECTOR_IRQ( 4), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-04" },
	{ hwint05, VECTOR_IRQ( 5), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-05" },
	{ hwint06, VECTOR_IRQ( 6), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-06" },
	{ hwint07, VECTOR_IRQ( 7), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-07" },
	{ hwint08, VECTOR_IRQ( 8), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-08" },
	{ hwint09, VECTOR_IRQ( 9), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-09" },
	{ hwint10, VECTOR_IRQ(10), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-10" },
	{ hwint11, VECTOR_IRQ(11), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-11" },
	{ hwint12, VECTOR_IRQ(12), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-12" },
	{ hwint13, VECTOR_IRQ(13), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-13" },
	{ hwint14, VECTOR_IRQ(14), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-14" },
	{ hwint15, VECTOR_IRQ(15), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-15" },
#ifndef USE_I8259
	{ hwint16, VECTOR_IRQ(16), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-16" },
	{ hwint17, VECTOR_IRQ(17), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-17" },
	{ hwint18, VECTOR_IRQ(18), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-18" },
	{ hwint19, VECTOR_IRQ(19), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-19" },
	{ hwint20, VECTOR_IRQ(20), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-20" },
	{ hwint21, VECTOR_IRQ(21), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-21" },
	{ hwint22, VECTOR_IRQ(22), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-22" },
	{ hwint23, VECTOR_IRQ(23), GATE_INTERRUPT, KRNL_RPL, 0, "IRQ-23" },
#endif
	{ NULL, 0, 0, 0}
};

gate_table_s lapic_ipi_init_table[] = {
	{ lapic_ipi00, VECTOR_IPI( 0), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-00"},
	{ lapic_ipi01, VECTOR_IPI( 1), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-01"},
	{ lapic_ipi02, VECTOR_IPI( 2), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-02"},
	{ lapic_ipi03, VECTOR_IPI( 3), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-03"},
	{ lapic_ipi04, VECTOR_IPI( 4), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-04"},
	{ lapic_ipi05, VECTOR_IPI( 5), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-05"},
	{ lapic_ipi06, VECTOR_IPI( 6), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-06"},
	{ lapic_ipi07, VECTOR_IPI( 7), GATE_INTERRUPT, KRNL_RPL, 0, "IPI-07"},
	{ NULL, 0, 0, 0}
};
/*==============================================================================================*
 *										initiate segs							     			*
 *==============================================================================================*/

static void set_commseg_desc(uint32_t index, CommSegType_E type, uint8_t privil, uint8_t Lflag)
{
	segdesc64_T *sd = &(gdt[index]);
	sd->S		= 1;
	sd->P		= 1;
	// changable bits
	sd->L		= Lflag;
	sd->Type	= type;
	sd->DPL		= privil;
}

static void set_codeseg_desc(uint32_t index, CommSegType_E type, uint8_t privil)
{
	set_commseg_desc(index, type, privil, 1);
}

static void set_dataseg_desc(uint32_t index, CommSegType_E type, uint8_t privil)
{
	set_commseg_desc(index, type, privil, 0);
}	

static void set_TSSseg_desc(u64 type, uint8_t privil)
{
	// size_t gdt_idx = 0;
	switch (type)
	{
		case DESC_TSS:
			{
				tss64_T * curr_tss = tss_ptr_arr;
				TSSsegdesc_T * tss_segdesc = (TSSsegdesc_T *)&gdt[GDT_ENTRY_TSS];
				tss_segdesc->Limit1	= sizeof(*curr_tss) & 0xFFFF;
				tss_segdesc->Limit2	= (sizeof(*curr_tss) >> 16) & 0xF;
				tss_segdesc->Base1	= (uint64_t)curr_tss & 0xFFFFFF;
				tss_segdesc->Base2	= ((uint64_t)curr_tss >> 24) & 0xFFFFFFFFFF;
				tss_segdesc->Type	= type;
				tss_segdesc->DPL	= privil;
				tss_segdesc->AVL	=
				tss_segdesc->P		= 1;
			}
			break;
		case GATE_INTERRUPT:
		case GATE_TRAP:
			{

			}
			break;

		default:
			break;
	}
}

/*==============================================================================================*
 *											prot_bsp_init								     	*
 *==============================================================================================*/
static void init_gdt()
{
	// gdt[0] has been set to 0 by memset
	set_codeseg_desc(GDT_ENTRY_KERNEL_CS, E_CODE, 0);
	set_dataseg_desc(GDT_ENTRY_KERNEL_DS, RW_DATA, 0);
	set_codeseg_desc(GDT_ENTRY_DEFAULT_USER_CS, E_CODE, 3);
	set_dataseg_desc(GDT_ENTRY_DEFAULT_USER_DS, RW_DATA, 3);
	set_codeseg_desc(GDT_ENTRY_DEFAULT_USER_CS_DUP, E_CODE, 3);
	set_dataseg_desc(GDT_ENTRY_DEFAULT_USER_DS_DUP, RW_DATA, 3);
}

static void init_idt_inner(gate_table_s * gtbl)
{
	while (gtbl->gate_entry != NULL)
	{
		gatedesc64_T *curr_gate = &(idt_table[gtbl->vec_nr]);
		// fixed bits
		curr_gate->Present = 1;
		curr_gate->segslct = __KERNEL_CS;
		curr_gate->IST 	   = gtbl->ist;
		// changable bits
		curr_gate->offs1 = (size_t)(gtbl->gate_entry) & 0xFFFF;
		curr_gate->offs2 = ((size_t)(gtbl->gate_entry) >> 16) & 0xFFFFFFFFFFFF;
		curr_gate->Type  = gtbl->type;
		curr_gate->DPL   = gtbl->DPL;
		
		gtbl++;
	}
}

static void init_idt()
{
	init_idt_inner(exception_init_table);
	init_idt_inner(hwint_init_table);
	init_idt_inner(lapic_ipi_init_table);
}

void myos_reload_arch_data(size_t cpu_idx)
{
	// set TSS-desc in GDT
	set_TSSseg_desc(DESC_TSS, KRNL_RPL);
	u16 tss_desc_idx = (uint16_t)__TSS_SEG;
	load_tr(tss_desc_idx);
}

void myos_init_arch(size_t cpu_idx)
{
	for (int i = 0; i < kparam.nr_lcpu; i++)
	{
		tss64_T *curr_tss = tss_ptr_arr + i;
		curr_tss->rsp0 = (reg_t)(idle_tasks[i] + 1);
	}
	init_gdt();
	init_idt();
}

/*==============================================================================================*
 *									systemcall mechanism env initiate							*
 *==============================================================================================*/
void myos_arch_system_call_init()
{
	cpudata_u * cpudata_u_p = (cpudata_u *)rdgsbase();
	// init MSR sf_regs related to sysenter/sysexit
	wrmsrl(MSR_IA32_SYSENTER_CS, __KERNEL_CS);
	wrmsrl(MSR_IA32_SYSENTER_EIP, (u64)sysenter_entp);
	wrmsrl(MSR_IA32_SYSENTER_ESP, (u64)task0_PCB.task.stack);
	uint64_t kstack;
	rdmsrl(MSR_IA32_SYSENTER_ESP, kstack);

	// // init MSR sf_regs related to syscall/sysret
	// wrmsrl(MSR_IA32_LSTAR, (uint64_t)enter_syscall);
	// wrmsrl(MSR_IA32_STAR, ((uint64_t)(__KERNEL_DS | 3) << 48) | ((uint64_t)__KERNEL_CS << 32));

	// // wrmsrl(MSR_IA32_FMASK, EFL_DF | EFL_IF | EFL_TF | EFL_NT);
	// wrmsrl(MSR_IA32_FMASK, 2);
}