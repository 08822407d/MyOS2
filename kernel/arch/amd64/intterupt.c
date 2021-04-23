#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "../../include/proto.h"

#include "../../include/printk.h"

extern char *(intr_name[IDT_SIZE]);

/*===========================================================================*
 *								exception handlers							 *
 *===========================================================================*/
void excep_inval_tss(stack_frame_s * sf_regs)
{
	int error_code = sf_regs->err_code;
	color_printk(RED,BLACK,"do_invalid_TSS(10),ERROR_CODE:%#018lx\n",error_code);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);

}

void excep_seg_not_pres(stack_frame_s * sf_regs)
{
	int error_code = sf_regs->err_code;
	color_printk(RED,BLACK,"do_segment_not_present(11),ERROR_CODE:%#018lx\n",error_code);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);
}

void excep_stack_segfault(stack_frame_s * sf_regs)
{
	int error_code = sf_regs->err_code;
	color_printk(RED,BLACK,"do_stack_segment_fault(12),ERROR_CODE:%#018lx\n",error_code);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);
}

void excep_gen_prot(stack_frame_s * sf_regs)
{
	int error_code = sf_regs->err_code;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx , code address:%#018lx\n",error_code, sf_regs->rip);

	if(error_code & 0x01)
		color_printk(RED,BLACK,"The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Refers to a gate descriptor in the IDT;\n");
	else
		color_printk(RED,BLACK,"Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			color_printk(RED,BLACK,"Refers to a segment or gate descriptor in the LDT;\n");
		else
			color_printk(RED,BLACK,"Refers to a descriptor in the current GDT;\n");

	color_printk(RED,BLACK,"Segment Selector Index:%#010x\n",error_code & 0xfff8);
}

void excep_page_fault(stack_frame_s * sf_regs)
{
	int error_code = sf_regs->err_code;
	unsigned long cr2 = 0;

	__asm__	__volatile__("movq	%%cr2,	%0":"=r"(cr2)::"memory");

	color_printk(RED,BLACK,"do_page_fault(14),ERROR_CODE: %#018lx\n",error_code);

	if(!(error_code & 0x01))
		color_printk(RED,BLACK,"Page Not-Present,\t");

	if(error_code & 0x02)
		color_printk(RED,BLACK,"Write Cause Fault,\t");
	else
		color_printk(RED,BLACK,"Read Cause Fault,\t");

	if(error_code & 0x04)
		color_printk(RED,BLACK,"Fault in user(3)\t");
	else
		color_printk(RED,BLACK,"Fault in supervisor(0,1,2)\t");

	if(error_code & 0x08)
		color_printk(RED,BLACK,",Reserved Bit Cause Fault\t");

	if(error_code & 0x10)
		color_printk(RED,BLACK,",Instruction fetch Cause Fault");

	color_printk(RED,BLACK,"Code address: %#018lx\n", sf_regs->rip);

	color_printk(RED,BLACK,"CR2:%#018lx\n",cr2);
}

/*===========================================================================*
 *								hwint handlers							     *
 *===========================================================================*/


/*===========================================================================*
 *									entrys									 *
 *===========================================================================*/
void excep_hwint_entry(stack_frame_s * sf_regs)
{
	int vec = sf_regs->vec_nr;

	if (vec < HWINT_VEC)
		do_exception_handler(sf_regs);
	else
		do_hwint_irq_handler(sf_regs);
}

void do_exception_handler(stack_frame_s * sf_regs)
{
	int vec = sf_regs->vec_nr;
	color_printk(WHITE, BLUE,"INTR: 0x%02x - %s ;", vec, intr_name[vec]);

	switch (vec)
	{
	case INVAL_TSS_VEC:
		excep_inval_tss(sf_regs);
		break;
	case SEG_NOT_PRES_VEC:
		excep_seg_not_pres(sf_regs);
		break;
	case STACK_SEGFAULT_VEC:
		excep_stack_segfault(sf_regs);
		break;
	case GEN_PROT_VEC:
		excep_gen_prot(sf_regs);
		break;
	case PAGE_FAULT_VEC:
		excep_page_fault(sf_regs);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
	}

	while (1);
}

void do_hwint_irq_handler(stack_frame_s * sf_regs)
{
	int vec = sf_regs->vec_nr;
	color_printk(WHITE, BLUE,"INTR: 0x%02x - %s ;", vec, intr_name[vec]);

#ifndef USE_APIC
	i8259_do_irq(sf_regs);
#else
	apic_do_irq(sf_regs);
#endif
}