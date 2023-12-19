#include <asm/io.h>

#include <obsolete/arch_proto.h>
#include <obsolete/myos_irq_vectors.h>

void init_i8259()
{
	//8259A-master	ICW1-4
	outb(ICW1_AT, INT_CTL);
	outb(FIRST_EXTERNAL_VECTOR, INT_CTLMASK);
	outb(1 << I8259_CASCADE_IRQ, INT_CTLMASK);
	outb(0x01, INT_CTLMASK);

	//8259A-slave	ICW1-4
	outb(ICW1_AT, INT2_CTL);
	outb(FIRST_EXTERNAL_VECTOR + 0x08, INT2_CTLMASK);
	outb(I8259_CASCADE_IRQ, INT2_CTLMASK);
	outb(0x01, INT2_CTLMASK);

	//8259A-M/S	OCW1
	// outb(~(1 << CASCADE_IRQ), INT_CTLMASK);
	// outb(~0, INT2_CTLMASK);

	outb(~0, INT_CTLMASK);
	outb(~0, INT2_CTLMASK);
}

void i8259_unmask(const int irq_nr)
{
	const unsigned ctl_mask = irq_nr < 8 ? INT_CTLMASK : INT2_CTLMASK;
	outb(inb(ctl_mask) & ~(1 << (irq_nr & 0x7)), ctl_mask);
}

void i8259_mask(const int irq_nr)
{
	const unsigned ctl_mask = irq_nr < 8 ? INT_CTLMASK : INT2_CTLMASK;
	outb(inb(ctl_mask) | (1 << (irq_nr & 0x7)), ctl_mask);
}

/* Disable 8259 - write 0xFF in OCW1 master and slave. */
void i8259_disable(void)
{
	outb(0xFF, INT2_CTLMASK);
	outb(0xFF, INT_CTLMASK);
	inb(INT_CTLMASK);
}

void i8259_eoi(int irq_nr)
{
	outb(END_OF_INT, INT_CTL);
	if (irq_nr >= 8)
		outb(END_OF_INT, INT2_CTL);
}