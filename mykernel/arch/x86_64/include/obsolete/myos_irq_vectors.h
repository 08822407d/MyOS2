#ifndef _AMD64_PC_INTERRUPT_H_
#define _AMD64_PC_INTERRUPT_H_

#include <asm/irq_vectors.h>

/* EXCEPTIONS */
	#define INTEL_RESERVED_VEC			-1
	/* Interrupt vectors defined/reserved by processor. */
	#define DIVIDE_ERR_VEC				0		/* divide error */
	#define DEBUG_VEC					1		/* single step (trace) */
	#define NMI_VEC						2		/* non-maskable interrupt */
	#define BREAKPOINT_VEC				3		/* software breakpoint */
	#define OVERFLOW_VEC				4		/* from INTO */
	#define BOUNDS_VEC					5		/* bounds check failed */
	#define INVAL_OP_VEC				6		/*invalid opcode */
	#define DEV_NOT_AVAIL_VEC			7		/* coprocessor not available */
	#define DOUBLE_FAULT_VEC			8
	#define COtask_sEG_VEC				9		/* coprocessor segment overrun */
	#define INVAL_TSS_VEC				10		/* invalid TSS */
	#define SEG_NOT_PRES_VEC			11		/* segment not present */
	#define STACK_SEGFAULT_VEC			12		/* stack exception */
	#define GEN_PROT_VEC				13		/* general protection */
	#define PAGE_FAULT_VEC				14
	// EXCEPTION 15 INTEL RESERVED
	#define X87_FPU_ERR_VEC				16		/* coprocessor error */
	#define ALIGNMENT_CHECK_VEC			17
	#define MACHINE_CHECK_VEC			18
	#define SIMD_EXCEP_VEC				19		/* SIMD Floating-Point Exception (#XM) */
	#define VIRTUAL_EXCEP_VEC			20		/* virtualization exception */
	#define CTRL_PROT_EXCEP_VEC			21

	/* Fixed system call vector. */
	#define KERN_CALL_VEC_ORIG			32		/* system calls are made with int SYSVEC */
	#define IPC_VEC_ORIG				33		/* interrupt vector for ipc */
	#define KERN_CALL_VEC_UM			34		/* user-mapped equivalent */
	#define IPC_VEC_UM					35		/* user-mapped equivalent */


/* HARDWARE INTERRUPTSa*/
	/* 8259A interrupt controller ports. */
	#define INT_CTL						0x20	/* I/O port for interrupt controller */
	#define INT_CTLMASK					0x21	/* setting bits in this port disables ints */
	#define INT2_CTL					0xA0	/* I/O port for second interrupt controller */
	#define INT2_CTLMASK				0xA1	/* setting bits in this port disables ints */
	/* Magic numbers for interrupt controller. */
	#define END_OF_INT					0x21	/* code used to re-enable after an interrupt */
	#define ICW1_AT						0x11	/* edge triggered, cascade, need ICW4 */
	#define ICW1_PC						0x13	/* edge triggered, no cascade, need ICW4 */
	#define ICW1_PS						0x19	/* level triggered, cascade, need ICW4 */
	#define ICW4_AT_SLAVE				0x01	/* not SFNM, not buffered, normal EOI, 8086 */
	#define ICW4_AT_MASTER				0x05	/* not SFNM, not buffered, normal EOI, 8086 */
	#define ICW4_PC_SLAVE				0x09	/* not SFNM, buffered, normal EOI, 8086 */
	#define ICW4_PC_MASTER				0x0D	/* not SFNM, buffered, normal EOI, 8086 */
	#define ICW4_AT_AEOI_SLAVE			0x03	/* not SFNM, not buffered, auto EOI, 8086 */
	#define ICW4_AT_AEOI_MASTER			0x07	/* not SFNM, not buffered, auto EOI, 8086 */
	#define ICW4_PC_AEOI_SLAVE			0x0B	/* not SFNM, buffered, auto EOI, 8086 */
	#define ICW4_PC_AEOI_MASTER			0x0F	/* not SFNM, buffered, auto EOI, 8086 */

	/* Hardware interrupt numbers. */
	#define APIC_IPI0_VEC				0xC8
	#define VECTOR_IRQ(irq)				(irq + FIRST_EXTERNAL_VECTOR)
	/* same irqs in i8259 and apic */
	#define KEYBOARD_IRQ				VECTOR_IRQ(1)
	#define SERIAL_2_4_IRQ				VECTOR_IRQ(3)		/* default ethernet interrupt vector */
	#define SERIAL_1_3_IRQ				VECTOR_IRQ(4)		/* RS232 interrupt vector for port 1 */
	#define PARALLEL_B_IRQ				VECTOR_IRQ(5)		/* xt winchester */
	#define FLOPPY_IRQ					VECTOR_IRQ(6)		/* floppy disk */
	#define PARALLEL_A_IRQ				VECTOR_IRQ(7)
	#define PS2_MOUSE_IRQ				VECTOR_IRQ(12)		/* AUX (PS/2 mouse) port in kbd controller */
	#define SATA_MAST_IRQ				VECTOR_IRQ(14)		/* at winchester controller 0 */
	#define SATA_SLAV_IRQ				VECTOR_IRQ(15)		/* at winchester controller 1 */
	/* different irqs in i8259 and apic */
	#define I8259_CASCADE_IRQ			2		/* casade enable for 2nd AT controller */

	#ifdef USE_I8259	// Then use i8259a
		#define NR_IRQ_VECS				16

		#define RTC_IRQ					VECTOR_IRQ(0)
		#define I8254_TIMER_IRQ			VECTOR_IRQ(8)
		#define PRESERVED_1_IRQ			VECTOR_IRQ(10)
		#define PRESERVED_2_IRQ			VECTOR_IRQ(11)
		#define FPU_IRQ					VECTOR_IRQ(13)
	#else	// Then use APIC
		#define NR_IRQ_VECS				24

		#define APIC_8259A				VECTOR_IRQ(0)
		#define HPET_TIMER0_IRQ			VECTOR_IRQ(2)
		#define HPET_TIMER1_IRQ			VECTOR_IRQ(8)
		#define GENERIC_1_IRQ			VECTOR_IRQ(9)
		#define GENERIC_2_IRQ			VECTOR_IRQ(10)
		#define HPET_TIMER2_IRQ			VECTOR_IRQ(11)
		#define DMA_IRQ					VECTOR_IRQ(13)
		// apic has 8 more irqs than i8259
		#define APIC_PIRQA				VECTOR_IRQ(16)
		#define APIC_PIRQB				VECTOR_IRQ(17)
		#define APIC_PIRQC				VECTOR_IRQ(18)
		#define APIC_PIRQD				VECTOR_IRQ(19)
		#define APIC_PIRQE				VECTOR_IRQ(20)
		#define APIC_PIRQF				VECTOR_IRQ(21)
		#define APIC_PIRQG				VECTOR_IRQ(22)
		#define APIC_PIRQH				VECTOR_IRQ(23)
	#endif
	// lapic vectors
	#define NR_LAPIC_IPI_VECS			0x07
	#define VECTOR_IPI(irq)				(irq + FIRST_SYSTEM_VECTOR)
	#define LAPIC_LVT_CMCI_IRQ			0
	// skip ICR
	#define LAPIC_LVT_TIMER_IRQ			VECTOR_IPI(2)
	#define LAPIC_LVT_THERMAL_IRQ		VECTOR_IPI(3)
	#define LAPIC_LVT_PERFORM_IRQ		VECTOR_IPI(4)
	#define LAPIC_LVT_LINT0_IRQ			VECTOR_IPI(5)
	#define LAPIC_LVT_LINT1_IRQ			VECTOR_IPI(6)
	#define LAPIC_LVT_ERROR_IRQ			VECTOR_IPI(7)

	/* EXCEPTIONS ERROR CODES */
	#define ARCH_PF_EC_P				(1 << 0)
	#define ARCH_PF_EC_WR				(1 << 1)
	#define ARCH_PF_EC_US				(1 << 2)
	#define ARCH_PF_EC_RSVD				(1 << 3)
	#define ARCH_PF_EC_ID				(1 << 4)

#endif /* _AMD64_PC_INTERRUPT_H_ */