#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

// EXCEPTIONS
       /* Interrupt vectors defined/reserved by processor. */
       #define DIVIDE_ERR_VEC	        0	/* divide error */
       #define DEBUG_VEC		        1	/* single step (trace) */
       #define NMI_VEC		        2	/* non-maskable interrupt */
       #define BREAKPOINT_VEC	        3	/* software breakpoint */
       #define OVERFLOW_VEC		        4	/* from INTO */
       #define BOUNDS_VEC		        5	/* bounds check failed */
       #define INVAL_OP_VEC		        6	/* invalid opcode */
       #define DEV_NOT_AVAIL_VEC   	 7	/* coprocessor not available */
       #define DOUBLE_FAULT_VEC	        8
       #define COPROC_SEG_VEC	        9	/* coprocessor segment overrun */
       #define INVAL_TSS_VEC	       10	/* invalid TSS */
       #define SEG_NOT_PRES_VEC	       11	/* segment not present */
       #define STACK_SEGFAULT_VEC	       12	/* stack exception */
       #define GEN_PROT_VEC 	       13	/* general protection */
       #define PAGE_FAULT_VEC	       14
       // EXCEPTION 15 INTEL RESERVED
       #define X87_FPU_ERR_VEC	       16	/* coprocessor error */
       #define ALIGNMENT_CHECK_VEC	       17
       #define MACHINE_CHECK_VEC	       18
       #define SIMD_EXCEP_VEC	       19     /* SIMD Floating-Point Exception (#XM) */
       #define VIRTUAL_EXCEP_VEC	       20	/* virtualization exception */
       #define CTRL_PROT_EXCEP_VEC        21

       /* Fixed system call vector. */
       #define KERN_CALL_VEC_ORIG         32     /* system calls are made with int SYSVEC */
       #define IPC_VEC_ORIG               33     /* interrupt vector for ipc */
       #define KERN_CALL_VEC_UM           34     /* user-mapped equivalent */
       #define IPC_VEC_UM                 35     /* user-mapped equivalent */

// HARDWARE INTERRUPTS
       /* 8259A interrupt controller ports. */
       #define INT_CTL                    0x20	/* I/O port for interrupt controller */
       #define INT_CTLMASK                0x21	/* setting bits in this port disables ints */
       #define INT2_CTL                   0xA0	/* I/O port for second interrupt controller */
       #define INT2_CTLMASK               0xA1	/* setting bits in this port disables ints */
       /* Magic numbers for interrupt controller. */
       #define END_OF_INT                 0x21	/* code used to re-enable after an interrupt */

       #define ICW1_AT                    0x11	/* edge triggered, cascade, need ICW4 */
       #define ICW1_PC                    0x13	/* edge triggered, no cascade, need ICW4 */
       #define ICW1_PS                    0x19	/* level triggered, cascade, need ICW4 */
       #define ICW4_AT_SLAVE              0x01	/* not SFNM, not buffered, normal EOI, 8086 */
       #define ICW4_AT_MASTER             0x05	/* not SFNM, not buffered, normal EOI, 8086 */
       #define ICW4_PC_SLAVE              0x09	/* not SFNM, buffered, normal EOI, 8086 */
       #define ICW4_PC_MASTER             0x0D	/* not SFNM, buffered, normal EOI, 8086 */
       #define ICW4_AT_AEOI_SLAVE         0x03   /* not SFNM, not buffered, auto EOI, 8086 */
       #define ICW4_AT_AEOI_MASTER        0x07   /* not SFNM, not buffered, auto EOI, 8086 */
       #define ICW4_PC_AEOI_SLAVE         0x0B   /* not SFNM, buffered, auto EOI, 8086 */
       #define ICW4_PC_AEOI_MASTER        0x0F   /* not SFNM, buffered, auto EOI, 8086 */

       /* Hardware interrupt numbers. */
       #define I8259_IRQ0_VEC             0x20   /* nice vectors to relocate IRQ0-7 to */
       #define APIC_IRQ0_VEC              0x20
       #define I8259_CASCADE_IRQ             2	/* cascade enable for 2nd AT controller */

       #ifndef USE_APIC       
              #define HWINT0_VEC                 I8259_IRQ0_VEC
              #define NR_IRQ_VECS                16

              #define CLOCK_IRQ                   0
              #define KEYBOARD_IRQ                1
              #define SERIAL_B_IRQ                3	/* default ethernet interrupt vector */
              #define SERIAL_A_IRQ                4	/* RS232 interrupt vector for port 1 */
              #define PARALLEL_B_IRQ              5	/* xt winchester */
              #define FLOPPY_IRQ                  6	/* floppy disk */
              #define PARALLEL_A_IRQ              7
              #define RTC_TIMER_IRQ               8
              #define PRESERVED_1_IRQ            10
              #define PRESERVED_2_IRQ            11
              #define PS2_MOUSE_IRQ              12	/* AUX (PS/2 mouse) port in kbd controller */
              #define FPU_IRQ                    13
              #define SATA_PRIM_IRQ              14	/* at winchester controller 0 */
              #define SATA_SCND_IRQ              15	/* at winchester controller 1 */
              #define VECTOR(vec)                (vec + I8259_IRQ0_VEC)
       #else
              #define HWINT0_VEC                 APIC_IRQ0_VEC
              #define NR_IRQ_VECS                24

              #define APIC_8259A                  0
              #define KEYBOARD_IRQ                1
              #define CLOCK_IRQ                   2
              #define SERIAL_A_IRQ                3
              #define SERIAL_B_IRQ                4
              #define PARALLEL_A_IRQ              6
              #define FLOPPY_IRQ                  6
              #define PARALLEL_B_IRQ              7
              #define RTC_TIMER_IRQ               8
              #define GENERIC_1_IRQ               9
              #define GENERIC_2_IRQ              10
              #define HPET_TIMER2_IRQ            11
              #define HPET_TIMER3_IRQ            12
              #define FERR_IRQ                   13
              #define SATA_PRIM_IRQ              14
              #define SATA_SCND_IRQ              15
              #define APIC_PIRQA                 16
              #define APIC_PIRQB                 17
              #define APIC_PIRQC                 18
              #define APIC_PIRQD                 19
              #define APIC_PIRQE                 20
              #define APIC_PIRQF                 21
              #define APIC_PIRQG                 22
              #define APIC_PIRQH                 23
              #define VECTOR(vec)                (vec + APIC_IRQ0_VEC)
       #endif
#endif /* _INTERRUPT_H_ */