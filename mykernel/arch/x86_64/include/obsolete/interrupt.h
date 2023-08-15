#ifndef _AMD64_PC_INTERRUPT_H_
#define _AMD64_PC_INTERRUPT_H_

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

	#define VECTOR_EXCEP_00				DIVIDE_ERR_VEC
	#define VECTOR_EXCEP_01				DEBUG_VEC
	#define VECTOR_EXCEP_02				NMI_VEC
	#define VECTOR_EXCEP_03				BREAKPOINT_VEC
	#define VECTOR_EXCEP_04				OVERFLOW_VEC
	#define VECTOR_EXCEP_05				BOUNDS_VEC
	#define VECTOR_EXCEP_06				INVAL_OP_VEC
	#define VECTOR_EXCEP_07				DEV_NOT_AVAIL_VEC
	#define VECTOR_EXCEP_08				DOUBLE_FAULT_VEC
	#define VECTOR_EXCEP_09				COtask_sEG_VEC
	#define VECTOR_EXCEP_10				INVAL_TSS_VEC
	#define VECTOR_EXCEP_11				SEG_NOT_PRES_VEC
	#define VECTOR_EXCEP_12				STACK_SEGFAULT_VEC
	#define VECTOR_EXCEP_13				GEN_PROT_VEC
	#define VECTOR_EXCEP_14				PAGE_FAULT_VEC
	#define VECTOR_EXCEP_15
	#define VECTOR_EXCEP_16				X87_FPU_ERR_VEC
	#define VECTOR_EXCEP_17				ALIGNMENT_CHECK_VEC
	#define VECTOR_EXCEP_18				MACHINE_CHECK_VEC
	#define VECTOR_EXCEP_19				SIMD_EXCEP_VEC
	#define VECTOR_EXCEP_20				VIRTUAL_EXCEP_VEC
	#define VECTOR_EXCEP_21				CTRL_PROT_EXCEP_VEC
	#define VECTOR_EXCEP_22
	#define VECTOR_EXCEP_23
	#define VECTOR_EXCEP_24
	#define VECTOR_EXCEP_25
	#define VECTOR_EXCEP_26
	#define VECTOR_EXCEP_27
	#define VECTOR_EXCEP_28
	#define VECTOR_EXCEP_29
	#define VECTOR_EXCEP_30
	#define VECTOR_EXCEP_31
			
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
	#define I8259_IRQ0_VEC				0x20	/* nice vectors to relocate IRQ0-7 to */
	#define APIC_IRQ0_VEC				0x20
	#define APIC_IPI0_VEC				0xC8
	/* same irqs in i8259 and apic */
	#define KEYBOARD_IRQ				1
	#define SERIAL_2_4_IRQ				3		/* default ethernet interrupt vector */
	#define SERIAL_1_3_IRQ				4		/* RS232 interrupt vector for port 1 */
	#define PARALLEL_B_IRQ				5		/* xt winchester */
	#define FLOPPY_IRQ					6		/* floppy disk */
	#define PARALLEL_A_IRQ				7
	#define PS2_MOUSE_IRQ				12		/* AUX (PS/2 mouse) port in kbd controller */
	#define SATA_MAST_IRQ				14		/* at winchester controller 0 */
	#define SATA_SLAV_IRQ				15		/* at winchester controller 1 */
	/* different irqs in i8259 and apic */
	#define I8259_CASCADE_IRQ			2		/* casade enable for 2nd AT controller */

	#ifdef USE_I8259	// Then use i8259a
		#define NR_IRQ_VECS				16
		#define HWINT0_VEC				I8259_IRQ0_VEC
		#define VECTOR_IRQ(irq)			(irq + I8259_IRQ0_VEC)

		#define RTC_IRQ					0
		#define I8254_TIMER_IRQ			8
		#define PRESERVED_1_IRQ			10
		#define PRESERVED_2_IRQ			11
		#define FPU_IRQ					13
	#else	// Then use APIC
		#define NR_IRQ_VECS				24
		#define HWINT0_VEC				APIC_IRQ0_VEC
		#define VECTOR_IRQ(irq)			(irq + APIC_IRQ0_VEC)

		#define APIC_8259A				0
		#define HPET_TIMER0_IRQ			2
		#define HPET_TIMER1_IRQ			8
		#define GENERIC_1_IRQ			9
		#define GENERIC_2_IRQ			10
		#define HPET_TIMER2_IRQ			11
		#define DMA_IRQ					13
		// apic has 8 more irqs than i8259
		#define APIC_PIRQA				16
		#define APIC_PIRQB				17
		#define APIC_PIRQC				18
		#define APIC_PIRQD				19
		#define APIC_PIRQE				20
		#define APIC_PIRQF				21
		#define APIC_PIRQG				22
		#define APIC_PIRQH				23
	#endif
	// lapic vectors
	#define NR_LAPIC_IPI_VECS			0x07
	#define VECTOR_IPI(irq)				(irq + APIC_IPI0_VEC)
	#define LAPIC_LVT_CMCI_IRQ			0
	// skip ICR
	#define LAPIC_LVT_TIMER_IRQ			2
	#define LAPIC_LVT_THERMAL_IRQ		3
	#define LAPIC_LVT_PERFORM_IRQ		4
	#define LAPIC_LVT_LINT0_IRQ			5
	#define LAPIC_LVT_LINT1_IRQ			6
	#define LAPIC_LVT_ERROR_IRQ			7

	/* EXCEPTIONS ERROR CODES */
	#define ARCH_PF_EC_P				(1 << 0)
	#define ARCH_PF_EC_WR				(1 << 1)
	#define ARCH_PF_EC_US				(1 << 2)
	#define ARCH_PF_EC_RSVD				(1 << 3)
	#define ARCH_PF_EC_ID				(1 << 4)

#endif /* _AMD64_PC_INTERRUPT_H_ */