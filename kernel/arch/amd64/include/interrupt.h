/* Interrupt numbers and hardware vectors. */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

       /* 8259A interrupt controller ports. */
       #define INT_CTL             0x20	/* I/O port for interrupt controller */
       #define INT_CTLMASK         0x21	/* setting bits in this port disables ints */
       #define INT2_CTL            0xA0	/* I/O port for second interrupt controller */
       #define INT2_CTLMASK        0xA1	/* setting bits in this port disables ints */

       /* Magic numbers for interrupt controller. */
       #define END_OF_INT          0x20	/* code used to re-enable after an interrupt */

       #define IRQ0_VEC            0x50   /* nice vectors to relocate IRQ0-7 to */
       #define IRQ8_VEC            0x70   /* no need to move IRQ8-15 */

       /* Interrupt vectors defined/reserved by processor. */
       #define DIVIDE_ERR_VEC	 0	/* divide error */
       #define DEBUG_VEC		 1	/* single step (trace) */
       #define NMI_VEC		 2	/* non-maskable interrupt */
       #define BREAKPOINT_VEC	 3	/* software breakpoint */
       #define OVERFLOW_VEC		 4	/* from INTO */
       #define BOUNDS_VEC		 5	/* bounds check failed */
       #define INVAL_OP_VEC		 6	/* invalid opcode */
       #define DEV_NOT_AVAIL_VEC  	 7	/* coprocessor not available */
       #define DOUBLE_FAULT_VEC	 8
       #define COPROC_SEG_VEC	 9	/* coprocessor segment overrun */
       #define INVAL_TSS_VEC	10	/* invalid TSS */
       #define SEG_NOT_PRES_VEC	11	/* segment not present */
       #define STACK_SEGFAULT_VEC	12	/* stack exception */
       #define GEN_PROT_VEC 	13	/* general protection */
       #define PAGE_FAULT_VEC	14
       // EXCEPTION 15 INTEL RESERVED
       #define X87_FPU_ERR_VEC	16	/* coprocessor error */
       #define ALIGNMENT_CHECK_VEC	17
       #define MACHINE_CHECK_VEC	18
       #define SIMD_EXCEP_VEC	19     /* SIMD Floating-Point Exception (#XM) */
       #define VIRTUAL_EXCEP_VEC	20	/* virtualization exception */
       #define CTRL_PROT_EXCEP_VEC 21

       /* Fixed system call vector. */
       #define KERN_CALL_VEC_ORIG  32     /* system calls are made with int SYSVEC */
       #define IPC_VEC_ORIG        33     /* interrupt vector for ipc */
       #define KERN_CALL_VEC_UM    34     /* user-mapped equivalent */
       #define IPC_VEC_UM          35     /* user-mapped equivalent */

       /* Hardware interrupt numbers. */
       #ifndef USE_APIC
       #define NR_IRQ_VECS         16
       #else
       #define NR_IRQ_VECS         64
       #endif
       #define CLOCK_IRQ            0
       #define KEYBOARD_IRQ         1
       #define CASCADE_IRQ          2	/* cascade enable for 2nd AT controller */
       #define ETHER_IRQ            3	/* default ethernet interrupt vector */
       #define SECONDARY_IRQ        3	/* RS232 interrupt vector for port 2 */
       #define RS232_IRQ            4	/* RS232 interrupt vector for port 1 */
       #define XT_WINI_IRQ          5	/* xt winchester */
       #define FLOPPY_IRQ           6	/* floppy disk */
       #define PRINTER_IRQ          7
       #define SPURIOUS_IRQ         7
       #define CMOS_CLOCK_IRQ       8
       #define KBD_AUX_IRQ         12	/* AUX (PS/2 mouse) port in kbd controller */
       #define AT_WINI_0_IRQ       14	/* at winchester controller 0 */
       #define AT_WINI_1_IRQ       15	/* at winchester controller 1 */

       #define VECTOR(irq)    \
              (((irq) < 8 ? IRQ0_VEC : IRQ8_VEC) + ((irq) & 0x07))

#endif /* _INTERRUPT_H_ */