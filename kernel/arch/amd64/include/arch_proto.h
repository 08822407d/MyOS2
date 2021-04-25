#ifndef _AMD64_PROTO_H_
#define _AMD64_PROTO_H_

#include <sys/types.h>
#include "archtypes.h"
#include "../../../include/ktypes.h"

/*===========================================================================*
*								external symbols							 *
*===========================================================================*/
	/* Exception handlers (real or protected mode), in numerical order. */
	void divide_error (void);
	void debug(void);
	void nmi (void);
	void breakpoint_exception (void);
	void overflow (void);
	void bounds_exceed (void);
	void invalid_opcode (void);
	void dev_not_available (void);
	void double_fault(void);
	void coproc_seg_overrun(void);
	void invalid_tss(void);
	void segment_not_present(void);
	void stack_segfault(void);
	void general_protection(void);
	void page_fault(void);
	// EXCEPTION 15 INTEL RESERVED
	void x87_fpu_error(void);
	void alignment_check(void);
	void machine_check(void);
	void simd_exception(void);
	void virtualization_exception(void);
	void control_protection_exception(void);
	/* Hardware interrupt handlers. */
	void hwint00(void);
	void hwint01(void);
	void hwint02(void);
	void hwint03(void);
	void hwint04(void);
	void hwint05(void);
	void hwint06(void);
	void hwint07(void);
	void hwint08(void);
	void hwint09(void);
	void hwint10(void);
	void hwint11(void);
	void hwint12(void);
	void hwint13(void);
	void hwint14(void);
	void hwint15(void);
	/* APIC Hardware interrupt handlers. */
	void hwint16(void);
	void hwint17(void);
	void hwint18(void);
	void hwint19(void);
	void hwint20(void);
	void hwint21(void);
	void hwint22(void);
	void hwint23(void);

	/* syscalls and others about intr */
	void ret_from_intr(void);
	void kernel_thread_func(void);
	void enter_syscall(void);
	void ret_from_syscall(void);

/*===========================================================================*
*								internal symbols							 *
*===========================================================================*/
	typedef struct __attribute__((packed)) {
		reg_t ds;
		reg_t es;
		reg_t r15;
		reg_t r14;
		reg_t r13;
		reg_t r12;
		reg_t r11;
		reg_t r10;
		reg_t r9;
		reg_t r8;
		reg_t rsi;
		reg_t rdi;
		reg_t rbp;
		reg_t rdx;
		reg_t rcx;
		reg_t rbx;
		reg_t rax;
		reg_t vec_nr;
		reg_t err_code;
		reg_t rip;
		reg_t cs;
		reg_t rflags;
		reg_t rsp;
		reg_t ss;
	} stack_frame_s;

	typedef struct {
		void	(*gate_entry) (void);
		uint8_t	vec_nr;
		uint8_t	type;
		uint8_t	DPL;
		char	name[16];
	} gate_table_s;

	typedef struct IOAPIC_map
	{
		phy_addr	phys_addr;
		uint8_t *	virt_idx_addr;
		uint32_t *	virt_data_addr;
		uint32_t *	virt_EOI_addr;
	} ioapic_map_s;

	typedef struct {
		void (*enable)(unsigned long irq);
		void (*disable)(unsigned long irq);
		unsigned long (*install)(unsigned long irq,void * arg);
		void (*uninstall)(unsigned long irq);
		void (*ack)(unsigned long irq);
	} hw_int_controller_s;

	typedef struct {
		hw_int_controller_s * controller;

		char * 			irq_name;
		unsigned long	parameter;
		unsigned long	flags;

		void (*handler)(unsigned long parameter,
						stack_frame_s * sf_regs);
	} irq_desc_s;

	/* protect.c */
	phy_addr vir2phy(vir_addr);
	vir_addr phy2vir(phy_addr);

	/* pg_util.c */
	void pg_clear(void);
	void pg_load_cr3(PML4E_u *);
	void pg_flush_tlb(void);
	void pg_domap(vir_addr, phy_addr, uint64_t);
	void pg_unmap(vir_addr);

	/* i8259.c */
	void init_i8259(void);
	void i8259_unmask(const int);
	void i8259_mask(const int);
	void i8259_disable(void);
	void i8259_eoi(int);
	void i8259_do_irq(stack_frame_s * sf_regs);
	/* apic.c */
	unsigned long ioapic_rte_read(unsigned char index);
	void ioapic_rte_write(unsigned char index,unsigned long value);
	void IOAPIC_pagetable_remap(void);
	void LAPIC_IOAPIC_init(void);
	void LAPIC_init(void);
	void IOAPIC_init(void);
	void IOAPIC_enable(unsigned long irq);
	void IOAPIC_disable(unsigned long irq);
	unsigned long IOAPIC_install(unsigned long irq,void * arg);
	void IOAPIC_uninstall(unsigned long irq);
	void IOAPIC_level_ack(unsigned long irq);
	void IOAPIC_edge_ack(unsigned long irq);
	void apic_do_irq(stack_frame_s * sf_regs);
	uint64_t ioapic_rte_read(uint8_t index);
	void ioapic_rte_write(uint8_t index, uint64_t value);
	void apic_do_irq(stack_frame_s * sf_regs);

	/* port_io.c */
	uint64_t inb(uint16_t port);
	uint64_t inw(uint16_t port);
	uint64_t inl(uint16_t port);
	void outb(uint16_t port, uint8_t value);
	void outw(uint16_t port, uint16_t value);
	void outl(uint16_t port, uint32_t value);
	uint64_t rdmsr(uint64_t msr_addr);
	void wrmsr(unsigned long msr_addr,unsigned long value);
	void io_mfence(void);
	void nop(void);
	void std(void);
	void cld(void);
	void cli(void);
	void sti(void);
	void cpuid(unsigned int Mop,unsigned int Sop,
				unsigned int * a,unsigned int * b,
				unsigned int * c,unsigned int * d);
	// void intr_disable(void);
	// void intr_enable(void);

	/* arch_proc.c */
	unsigned long do_execve(stack_frame_s * sf_regs);
	unsigned long do_fork(stack_frame_s * sf_regs,
							unsigned long clone_flags,
							unsigned long stack_start,
							unsigned long stack_size);
	int sys_call(int syscall_nr);
	int do_syscall(int syscall_nr);

	/* interrupt.c */
	void excep_hwint_entry(stack_frame_s * sf_regs);
	void exception_handler(stack_frame_s * sf_regs);
	void hwint_irq_handler(stack_frame_s * sf_regs);
	int register_irq(unsigned long irq, void * arg, char * irq_name,
				 unsigned long parameter, hw_int_controller_s * controller,
				 void (*handler)(unsigned long parameter, stack_frame_s * sf_regs));
	int unregister_irq(unsigned long irq);

	/* smp.c */
	void init_cpu(void);
	void SMP_init(void);

#endif /* _AMD64_PROTO_H_ */