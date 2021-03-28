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

	outb(INT_CTLMASK,0);
	outb(INT2_CTLMASK,0);
	__asm__ __volatile__("sti	\n");
}