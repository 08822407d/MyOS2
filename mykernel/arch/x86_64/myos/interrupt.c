#include <linux/kernel/stddef.h>

#include <obsolete/proto.h>
#include <obsolete/printk.h>

#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>
#include <obsolete/device.h>

#include <asm/idtentry.h>

myos_irq_desc_s	irq_descriptors[NR_VECTORS];

/*==============================================================================================*
 *										exception handlers								 		*
 *==============================================================================================*/
void excep_inval_tss(pt_regs_s *regs)
{
	unsigned long error_code = (unsigned long)regs->orig_ax;
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

void excep_seg_not_pres(pt_regs_s *regs)
{
	unsigned long error_code = (unsigned long)regs->orig_ax;
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

void excep_stack_segfault(pt_regs_s *regs)
{
	unsigned long error_code = (unsigned long)regs->orig_ax;
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

void excep_gen_prot(pt_regs_s *regs)
{
	unsigned long error_code = (unsigned long)regs->orig_ax;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx , code address:%#018lx\n",error_code, regs->ip);

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


/*==============================================================================================*
 *											entrys									 			*
 *==============================================================================================*/
void excep_hwint_context(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;

	if (vec < FIRST_EXTERNAL_VECTOR)
		exception_handler(regs);
	else
		hwint_irq_handler(regs);
}

void exception_handler(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;

	switch (vec)
	{
	case INVAL_TSS_VEC:
		excep_inval_tss(regs);
		break;
	case SEG_NOT_PRES_VEC:
		excep_seg_not_pres(regs);
		break;
	case STACK_SEGFAULT_VEC:
		excep_stack_segfault(regs);
		break;
	case GEN_PROT_VEC:
		excep_gen_prot(regs);
		break;
	case PAGE_FAULT_VEC:
		exc_page_fault(regs, regs->orig_ax);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
	}
}

void hwint_irq_handler(pt_regs_s *regs)
{
	unsigned long vec = (unsigned long)regs->irq_nr;
	int irq_nr = 0;
	myos_irq_desc_s *irq_d = &irq_descriptors[vec];	

	if(irq_d->handler != NULL)
		irq_d->handler(irq_d->parameter, regs);
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
				 void (*handler)(unsigned long parameter, pt_regs_s *regs))
{
	myos_irq_desc_s *p = &irq_descriptors[irq];
	p->controller = controller;
	p->irq_name = irq_name;
	p->parameter = parameter;
	p->flags = 0;
	p->handler = handler;
	if (p->controller != NULL)
	{
		if (p->controller->install != NULL)
			p->controller->install(irq, arg);
		if (p->controller->enable != NULL)
			p->controller->enable(irq);
	}
	return 1;
}

int unregister_irq(unsigned long irq)
{
	myos_irq_desc_s *p = &irq_descriptors[irq];

	if (p->controller != NULL)
	{
		if (p->controller->disable != NULL)
			p->controller->disable(irq);
		if (p->controller->uninstall != NULL)
			p->controller->uninstall(irq);
	}
	
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
	i8259_disable();
	//init ioapic
	IOAPIC_init();
}

void myos_init_percpu_intr()
{
	// LVT_timer_init();
}

// __attribute__ ((interrupt)) __attribute__ ((target("general-regs-only")))
// void intr_call_test(pt_regs_s *regs, unsigned long errcode)
// {
// 	regs->irq_nr = 14;
// 	excep_hwint_context(regs);
// }