#include <linux/kernel/stddef.h>

#include <obsolete/proto.h>
#include <obsolete/printk.h>

#include "include/arch_proto.h"
#include "include/interrupt.h"
#include "include/apic.h"
#include "include/device.h"

extern gate_table_s exception_init_table[];

irq_desc_s	irq_descriptors[NR_IRQ_VECS];
irq_desc_s	ipi_descriptors[NR_LAPIC_IPI_VECS];

/*==============================================================================================*
 *										exception handlers								 		*
 *==============================================================================================*/
void excep_inval_tss(stack_frame_s *sf_regs)
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


	while (1);
}

void excep_seg_not_pres(stack_frame_s *sf_regs)
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

	while (1);
}

void excep_stack_segfault(stack_frame_s *sf_regs)
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

	while (1);
}

void excep_gen_prot(stack_frame_s *sf_regs)
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

	while (1);
}

void excep_page_fault(stack_frame_s *sf_regs, per_cpudata_s *cpudata_p)
{
	task_s *curr = curr_tsk;
	mm_s *mm = curr->mm_struct;
	int error_code = sf_regs->err_code;
	unsigned long cr2 = 0;
	__asm__	__volatile__(	"movq	%%cr2,	%0		\n\t"
						:	"=r"(cr2)
						:
						:	"memory"
						);

	if (error_code & (ARCH_PF_EC_WR & ~ARCH_PF_EC_P) &&
		check_addr_writable(cr2, curr))
	{
		do_COW(curr_tsk, (virt_addr_t)cr2);
		return;
	}

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

	while (1);

PF_finish:
	myos_refresh_arch_page();
	return;
}


/*==============================================================================================*
 *											entrys									 			*
 *==============================================================================================*/
void excep_hwint_entry(stack_frame_s *sf_regs)
{
	int vec = sf_regs->vec_nr;

	if (vec < HWINT0_VEC)
		exception_handler(sf_regs);
	else
		hwint_irq_handler(sf_regs);
}

void exception_handler(stack_frame_s *sf_regs)
{
	per_cpudata_s *cpudata_p = curr_cpu;
	int vec = sf_regs->vec_nr;
	task_s *curr = curr_tsk;
	// color_printk(WHITE, BLUE,"Caused by core-%d, task-%d INTR: 0x%02x - %s ; \n",
	// 				cpudata_p->cpu_idx, curr->pid, vec, exception_init_table[vec].name);

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
		excep_page_fault(sf_regs, cpudata_p);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
	}
}

void hwint_irq_handler(stack_frame_s *sf_regs)
{
	int vec = sf_regs->vec_nr;
	int irq_nr = 0;
	irq_desc_s * irq_d = NULL;
	if (vec < APIC_IPI0_VEC)
	{
		irq_nr = vec - HWINT0_VEC;
		irq_d = &irq_descriptors[irq_nr];	
	}
	else
	{
		irq_nr = vec - APIC_IPI0_VEC;
		irq_d = &ipi_descriptors[irq_nr];	
	}

	// color_printk(WHITE, BLUE,"Recieved by core-%d INTR: 0x%02x - %s ; ", cpudata_p->cpu_idx, vec, irq_descriptors[irq_nr].irq_name);

	if(irq_d->handler != NULL)
		irq_d->handler(irq_d->parameter, sf_regs);
	if(irq_d->controller != NULL && irq_d->controller->ack != NULL)
		irq_d->controller->ack(irq_nr);
}

/*==============================================================================================*
 *											hwint handlers							    		*
 *==============================================================================================*/
int register_irq(unsigned long irq,
				 void *arg,
				 char *irq_name,
				 unsigned long parameter,
				 hw_int_controller_s *controller,
				 void (*handler)(unsigned long parameter, stack_frame_s *sf_regs))
{
	irq_desc_s *p = &irq_descriptors[irq];
	p->controller = controller;
	p->irq_name = irq_name;
	p->parameter = parameter;
	p->flags = 0;
	p->handler = handler;
	p->controller->install(irq, arg);
	p->controller->enable(irq);
	return 1;
}

int unregister_irq(unsigned long irq)
{
	irq_desc_s *p = &irq_descriptors[irq];

	p->controller->disable(irq);
	p->controller->uninstall(irq);

	p->controller = 0;
	p->irq_name = 0;
	p->parameter = 0;
	p->flags = 0;
	p->handler = 0;

	return 1; 
}

/*==============================================================================================*
 *											hwint handlers							    		*
 *==============================================================================================*/
int register_IPI(unsigned long irq,
				 void *arg,
				 char *irq_name,
				 unsigned long parameter,
				 hw_int_controller_s *controller,
				 void (*handler)(unsigned long parameter, stack_frame_s *sf_regs))
{
	irq_desc_s *p = &ipi_descriptors[irq];

	p->controller = controller;
	p->irq_name = irq_name;
	p->parameter = parameter;
	p->flags = 0;
	p->handler = handler;
	return 1;
}

int unregister_IPI(unsigned long irq)
{
	irq_desc_s *p = &ipi_descriptors[irq];

	p->controller = 0;
	p->irq_name = 0;
	p->parameter = 0;
	p->flags = 0;
	p->handler = 0;

	return 1; 
}

/*==============================================================================================*
 *											init										 		*
 *==============================================================================================*/
void myos_init_bsp_intr()
{
	init_lapic();
	//init local apic
	IOAPIC_pagetable_remap();

	i8259_disable();

	//init ioapic
	IOAPIC_init();
}

void myos_init_percpu_intr()
{
	init_lapic();
	// LVT_timer_init();
}