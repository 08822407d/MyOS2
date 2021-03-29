#include "include/arch_proto.h"
#include "include/interrupt.h"

void init_i8259()
{
	//8259A-master	ICW1-4
	outb(INT_CTL,ICW1_AT);
	outb(INT_CTLMASK,IRQ0_VEC);
	outb(INT_CTLMASK,1 << CASCADE_IRQ);
	outb(INT_CTLMASK,0x01);

	//8259A-slave	ICW1-4
	outb(INT2_CTL,ICW1_AT);
	outb(INT2_CTLMASK,IRQ8_VEC);
	outb(INT2_CTLMASK,CASCADE_IRQ);
	outb(INT2_CTLMASK,0x01);

	//8259A-M/S	OCW1
	// outb(INT_CTLMASK,~(1 << CASCADE_IRQ));
	// outb(INT2_CTLMASK,~0);

	outb(INT_CTLMASK,~2);
	outb(INT2_CTLMASK,~0);
	__asm__ __volatile__("sti	\n");
}

void i8259_unmask(const int irq)
{
	const unsigned ctl_mask = irq < 8 ? INT_CTLMASK : INT2_CTLMASK;
	outb(ctl_mask, inb(ctl_mask) & ~(1 << (irq & 0x7)));
}

void i8259_mask(const int irq)
{
	const unsigned ctl_mask = irq < 8 ? INT_CTLMASK : INT2_CTLMASK;
	outb(ctl_mask, inb(ctl_mask) | (1 << (irq & 0x7)));
}

/* Disable 8259 - write 0xFF in OCW1 master and slave. */
void i8259_disable(void)
{
	outb(INT2_CTLMASK, 0xFF);
	outb(INT_CTLMASK, 0xFF);
	inb(INT_CTLMASK);
}

void i8259_eoi(int irq)
{
	outb(INT_CTL, END_OF_INT);
	if (irq >= 8)
		outb(INT2_CTL, END_OF_INT);
}