#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "../../include/proto.h"

#include "../../include/printk.h"

extern char *(intr_name[IDT_SIZE]);

void excep_inval_tss(stack_frame_s * sf)
{
	color_printk(RED, BLACK, "Err Code : sele-%d, TI-%d, IDT-%d, EXT-%d \n",
					sf->vec_nr >> 3, 1 & (sf->vec_nr >> 2), 1 & (sf->vec_nr >> 1));

}

void excep_page_fault(stack_frame_s * sf)
{

}

void excep_gen_prot(stack_frame_s * sf)
{
	int error_code = sf->err_code;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx\n",error_code);

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

void excep_intr_c_entry(stack_frame_s * sf)
{
		color_printk(WHITE, BLUE,"INTR: 0x%02x - %s \n", sf->vec_nr, intr_name[sf->vec_nr]);
		if(sf->vec_nr < 32)
		{
			switch (sf->vec_nr)
			{
			case INVAL_TSS_VEC:
				excep_inval_tss(sf);
				break;
			case PAGE_FAULT_VEC:
				excep_page_fault(sf);
				break;
			case GEN_PROT_VEC:
				excep_gen_prot(sf);
				break;
			
			default:
				break;
			}
			while (1);
		}
		outb(INT_CTL, END_OF_INT);
}