#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "../../include/proto.h"

#include "../../include/printk.h"

extern char *(intr_name[IDT_SIZE]);

/*===========================================================================*
 *								exception handlers							 *
 *===========================================================================*/
void excep_inval_tss(stack_frame_s * sf)
{
	int error_code = sf->err_code;
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

void excep_seg_not_pres(stack_frame_s * sf)
{
	int error_code = sf->err_code;
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

void excep_stack_segfault(stack_frame_s * sf)
{
	int error_code = sf->err_code;
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

void excep_gen_prot(stack_frame_s * sf)
{
	int error_code = sf->err_code;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx , code address:%#018lx\n",error_code, sf->rip);

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

void excep_page_fault(stack_frame_s * sf)
{
	int error_code = sf->err_code;
	unsigned long cr2 = 0;

	__asm__	__volatile__("movq	%%cr2,	%0":"=r"(cr2)::"memory");

	color_printk(RED,BLACK,"do_page_fault(14),ERROR_CODE:%#018lx\n",error_code);

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

	color_printk(RED,BLACK,"\n");

	color_printk(RED,BLACK,"CR2:%#018lx\n",cr2);
}

/*===========================================================================*
 *								hwint handlers							     *
 *===========================================================================*/
void hwint_kbd(stack_frame_s * sf)
{
	uint8_t scan_code = inb(0x60);
	color_printk(GREEN, BLACK, "KBD SCAN CODE = %#04x\n", scan_code);
}

/*===========================================================================*
 *									entrys									 *
 *===========================================================================*/
void do_exception_entry(stack_frame_s * sf)
{
	int vec = sf->vec_nr;
	color_printk(WHITE, BLUE,"INTR: 0x%02x - %s ;", vec, intr_name[vec]);

	switch (vec)
	{
	case INVAL_TSS_VEC:
		excep_inval_tss(sf);
		break;
	case SEG_NOT_PRES_VEC:
		excep_seg_not_pres(sf);
		break;
	case STACK_SEGFAULT_VEC:
		excep_stack_segfault(sf);
		break;
	case GEN_PROT_VEC:
		excep_gen_prot(sf);
		break;
	case PAGE_FAULT_VEC:
		excep_page_fault(sf);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
	}

	while (1);
}

void do_hwint_irq_entry(stack_frame_s * sf)
{
	int vec = sf->vec_nr;
	color_printk(WHITE, BLUE,"INTR: 0x%02x - %s ;", vec, intr_name[vec]);

	switch (vec)
	{
#ifndef USE_APIC
	case VECTOR(KEYBOARD_IRQ):
		hwint_kbd(sf);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
#else

#endif
	}

	int irq = vec < IRQ8_VEC ? (vec - IRQ0_VEC) : (vec - IRQ8_VEC);
	i8259_eoi(irq);
}