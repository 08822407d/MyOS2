#ifndef _AMD64_PROTO_H_
#define _AMD64_PROTO_H_

#include <sys/types.h>
#include "archtypes.h"
#include "archconst.h"

/*==============================================================================================*
 *										external symbols							 			*
 *==============================================================================================*/
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
	void cotask_seg_overrun(void);
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
	void enter_sysenter(void);
	void ret_from_sysenter(void);
	void enter_syscall(void);
	void ret_from_syscall(void);

/*==============================================================================================*
 *										internal symbols										*
 *==============================================================================================*/
// interrupt stack
	typedef struct stack_frame {
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
	} __attribute__((packed)) stack_frame_s;

// intr gate initiate infomation
	typedef struct {
		void	(*gate_entry) (void);
		uint8_t	vec_nr;
		uint8_t	type;
		uint8_t	DPL;
		uint8_t IST;
		char	name[16];
	} gate_table_s;

// local apic information
	typedef struct lapic_info
	{
		unsigned lapic_ver;
		unsigned max_lvt;
		unsigned svr12_support;
	} lapic_info_s;
	
// ioapic information
	typedef struct IOAPIC_map
	{
		phys_addr	phys_addr;
		uint8_t *	virt_idx_addr;
		uint32_t *	virt_data_addr;
		uint32_t *	virt_EOI_addr;
	} ioapic_map_s;
// irq handler machanism 
	typedef struct {
		void (*enable)(unsigned long irq);
		void (*disable)(unsigned long irq);
		unsigned long (*install)(unsigned long irq,void * arg);
		void (*uninstall)(unsigned long irq);
		void (*ack)(unsigned long irq);
	} hw_int_controller_s;
// irq infomation
	typedef struct {
		hw_int_controller_s * controller;

		char * 			irq_name;
		unsigned long	parameter;
		unsigned long	flags;

		void (*handler)(unsigned long parameter,
						stack_frame_s * sf_regs);
	} irq_desc_s;

	/* cpu info */
	struct cputopo
	{
		uint8_t		not_use;
		uint8_t		pack_id;
		uint8_t		core_id;
		uint8_t		thd_id;
	};

	typedef struct arch_percpu_info
	{
		uint64_t	lcpu_addr;			// local apic_id
		uint16_t	lcpu_topo_flag[4];	// 3 = flag, 2 = package_id, 1 = core_id, 0 = thread_id
		tss64_T *	tss;
	} arch_cpudata_s;

	typedef struct
	{
		int		get_cpuid;
		int		memory_layout;
		int		frame_buffer_info;

		int		init_bsp_arch_data;
		int		reload_bsp_arch_data;
		int		reload_bsp_arch_page;
	} arch_init_flags_s;
	

	/* pre_init.c */
	void pre_init(void);
	/* protect.c */
	phys_addr virt2phys(virt_addr);
	virt_addr phys2virt(phys_addr);
	void load_gdt(desctblptr64_T * gdt_desc);
	void load_idt(desctblptr64_T * idt_desc);
	void load_tss(uint64_t cpu_idx);
	void init_bsp_arch_data(void);
	void load_arch_data(size_t cpu_idx);
	void init_bsp_arch_data(void);
	void reload_bsp_arch_data(void);
	void reload_percpu_arch_data(size_t cpu_idx);
	void arch_system_call_init(void);

	/* arch_page_util. */
	void arch_page_preinit(void);
	void reload_arch_page(void);
	void pg_load_cr3(PML4E_T *);
	void refresh_arch_page(void);
	void arch_page_domap(virt_addr, phys_addr, uint64_t, PML4E_T * pml4_base);
	void pg_unmap(virt_addr);

	/* i8259.c */
	void init_i8259(void);
	void i8259_unmask(const int);
	void i8259_mask(const int);
	void i8259_disable(void);
	void i8259_eoi(int);
	void i8259_do_irq(stack_frame_s * sf_regs);
	/* apic.c */
	void IOAPIC_pagetable_remap(void);
	void init_lapic(void);
	void IOAPIC_init(void);
	void IOAPIC_enable(unsigned long irq);
	void IOAPIC_disable(unsigned long irq);
	unsigned long IOAPIC_install(unsigned long irq,void * arg);
	void IOAPIC_uninstall(unsigned long irq);
	void IOAPIC_level_ack(unsigned long irq);
	void IOAPIC_edge_ack(unsigned long irq);
	uint64_t ioapic_rte_read(uint8_t index);
	void ioapic_rte_write(uint8_t index, uint64_t value);
	void apic_do_irq(stack_frame_s * sf_regs);
	void enable_x2apic(void);
	void open_lapic(void);
	unsigned get_x2apic_id(void);
	void get_lapic_ver(lapic_info_s * lapic_info);
	void disable_lvt(lapic_info_s * lapic_info);
	unsigned get_lvt_tpr(void);
	unsigned get_lvt_ppr(void);

	/* asm_wrapper.c */
	uint64_t inb(uint16_t port);
	uint64_t inw(uint16_t port);
	uint64_t inl(uint16_t port);
	void outb(uint16_t port, uint8_t value);
	void outw(uint16_t port, uint16_t value);
	void outl(uint16_t port, uint32_t value);
	void insw(uint16_t port, uint16_t * buffer, uint64_t nr);
	void outsw(uint16_t port, uint16_t * buffer, uint64_t nr);
	uint64_t rdmsr(uint64_t msr_addr);
	void wrmsr(unsigned long msr_addr,unsigned long value);
	void io_mfence(void);
	void nop(void);
	void hlt(void);
	void std(void);
	void cld(void);
	void cli(void);
	void sti(void);
	void cpuid(unsigned int Mop,unsigned int Sop,
				unsigned int * a,unsigned int * b,
				unsigned int * c,unsigned int * d);
	void wrgsbase(uint64_t addr);
	uint64_t rdgsbase(void);
	// void intr_disable(void);
	// void intr_enable(void);

	/* arch_task.c */
	unsigned long do_execve(stack_frame_s * sf_regs);
	unsigned long do_fork(stack_frame_s * sf_regs,
							unsigned long clone_flags,
							unsigned long tmp_kstack_start,
							unsigned long stack_size);
	int sys_call(int syscall_nr);
	int do_syscall(int syscall_nr);
	void schedule(void);
	int kernel_thread(unsigned long (* fn)(unsigned long), unsigned long arg, unsigned long flags);
	void userthd_test();
	void userthd_test2();
	void enter_userthd(void);

	/* interrupt.c */
	void excep_hwint_entry(stack_frame_s * sf_regs);
	void exception_handler(stack_frame_s * sf_regs);
	void hwint_irq_handler(stack_frame_s * sf_regs);
	int register_irq(unsigned long irq, void * arg, char * irq_name,
				 unsigned long parameter, hw_int_controller_s * controller,
				 void (*handler)(unsigned long parameter, stack_frame_s * sf_regs));
	int unregister_irq(unsigned long irq);
	void init_bsp_intr(void);
	void init_percpu_intr(void);

	/* smp.c */
	void init_cpu(void);
	void init_smp_env(void);
	void start_SMP(uint64_t apic_id);
	void startup_smp(void);
	void percpu_self_config(size_t cpu_idx);

#endif /* _AMD64_PROTO_H_ */