#include <linux/kernel/stddef.h>

#include <obsolete/proto.h>
#include <obsolete/printk.h>

#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>
#include <obsolete/apic.h>
#include <obsolete/device.h>

#include <asm/idtentry.h>

irq_desc_s	irq_descriptors[NR_IRQ_VECS];
irq_desc_s	ipi_descriptors[NR_LAPIC_IPI_VECS];

/*==============================================================================================*
 *										exception handlers								 		*
 *==============================================================================================*/
void excep_inval_tss(pt_regs_s *sf_regs)
{
	unsigned long error_code = (unsigned long)sf_regs->orig_ax;
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

void excep_seg_not_pres(pt_regs_s *sf_regs)
{
	unsigned long error_code = (unsigned long)sf_regs->orig_ax;
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

void excep_stack_segfault(pt_regs_s *sf_regs)
{
	unsigned long error_code = (unsigned long)sf_regs->orig_ax;
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

void excep_gen_prot(pt_regs_s *sf_regs)
{
	unsigned long error_code = (unsigned long)sf_regs->orig_ax;
	color_printk(RED,BLACK,"do_general_protection(13),ERROR_CODE:%#018lx , code address:%#018lx\n",error_code, sf_regs->ip);

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
extern void do_softirq(void);
void excep_hwint_context(pt_regs_s *sf_regs)
{
	unsigned long vec = (unsigned long)sf_regs->irq_nr;

	if (vec < HWINT0_VEC)
		exception_handler(sf_regs);
	else
		hwint_irq_handler(sf_regs);

	try_sched();
}

void exception_handler(pt_regs_s *sf_regs)
{
	per_cpudata_s *cpudata_p = curr_cpu;
	unsigned long vec = (unsigned long)sf_regs->irq_nr;
	task_s *curr = current;

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
		exc_page_fault(sf_regs, sf_regs->orig_ax);
		break;

	default:
		color_printk(GREEN, BLACK, "Handler not yet implemented!\n");
		break;
	}
}

void hwint_irq_handler(pt_regs_s *sf_regs)
{
	unsigned long vec = (unsigned long)sf_regs->irq_nr;
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
				 void (*handler)(unsigned long parameter, pt_regs_s *sf_regs))
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
				 void (*handler)(unsigned long parameter, pt_regs_s *sf_regs))
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