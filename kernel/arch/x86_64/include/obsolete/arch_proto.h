#ifndef _AMD64_PROTO_H_
#define _AMD64_PROTO_H_

#include <linux/kernel/types.h>
#include <linux/kernel/sched.h>

#include "archtypes.h"
#include "mutex.h"

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
	/* LAPIC IPI handlers. */
	void lapic_ipi00(void);
	void lapic_ipi01(void);
	void lapic_ipi02(void);
	void lapic_ipi03(void);
	void lapic_ipi04(void);
	void lapic_ipi05(void);
	void lapic_ipi06(void);
	void lapic_ipi07(void);

	/* syscalls and others about intr */
	void sysenter_entp(void);
	void sysexit_entp(void);
	void ra_sysex_retp(void);
	void entp_kernel_thread(void);
	void ra_kthd_retp(void);

/*==============================================================================================*
 *										internal symbols										*
 *==============================================================================================*/
// interrupt stack
	typedef struct stack_frame {
		reg_t	r15;
		reg_t	r14;
		reg_t	r13;
		reg_t	r12;
		reg_t	r11;
		reg_t	r10;
		reg_t	r9;
		reg_t	r8;
		reg_t	rsi;
		reg_t	rdi;
		reg_t	rbp;
		reg_t	rdx;
		reg_t	rcx;
		reg_t	rbx;
		reg_t	rax;
		virt_addr_t		restore_retp;
		reg_t	vec_nr;
		reg_t	err_code;
		reg_t	rip;
		reg_t	cs;
		reg_t	rflags;
		reg_t	rsp;
		reg_t	ss;
	} __attribute__((packed)) stack_frame_s;
// intr gate initiate infomation
	typedef struct {
		void	(*gate_entry) (void);
		uint8_t	vec_nr;
		uint8_t	type;
		uint8_t	DPL;
		uint8_t	ist;
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
		phys_addr_t	phys_addr;
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
		int		framebuffer;
		int		arch_data;
	} arch_init_flags_s;

	/* early_init.c */
	void myos_early_init_system(void);
	/* protect.c */
	phys_addr_t myos_virt2phys(virt_addr_t);
	virt_addr_t myos_phys2virt(phys_addr_t);
	#define __phys_addr myos_virt2phys
	#define __virt_addr myos_phys2virt
	void myos_early_init_arch_data(size_t lcpu_nr);
	void load_gdt(desctblptr64_T * gdt_desc);
	void load_idt(desctblptr64_T * idt_desc);
	void load_tss(uint64_t cpu_idx);
	void myos_init_arch(size_t cpu_idx);
	void myos_reload_arch_data(size_t cpu_idx);
	void myos_arch_system_call_init(void);

	/* arch_page_util. */
	// struct mm;
	// typedef struct mm mm_s;
	void arch_page_preinit(void);
	int myos_init_memory_mapping(phys_addr_t base, size_t size);
	void myos_refresh_arch_page(void);
	void myos_unmap_kernel_lowhalf(atomic_t *um_flag);
	int arch_page_domap(virt_addr_t virt, phys_addr_t phys, uint64_t attr, reg_t * kernel_cr3);
	int arch_page_setattr(virt_addr_t virt, uint64_t attr, reg_t * cr3);
	int arch_page_clearattr(virt_addr_t virt, uint64_t attr, reg_t * cr3);
	int arch_page_duplicate(virt_addr_t virt, phys_addr_t phys, reg_t orig_cr3, reg_t * ret_cr3);
	int get_paddr(reg_t cr3, virt_addr_t virt, phys_addr_t *ret_phys);

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
	void LVT_ack(unsigned long lvt_nr);
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
	/* apic_timer.c */
	void LVT_timer_init(void);

	/* arch_task.c */
	stack_frame_s * get_stackframe(task_s * task_p);
	unsigned long do_fork(stack_frame_s * sf_regs, unsigned long clone_flags,
			unsigned long tmp_kstack_start, unsigned long stack_size,
			const char *taskname, task_s **ret_child);
	unsigned long do_execve(stack_frame_s * curr_context, char *exec_filename,
			char *argv[], char *envp[]);
	unsigned long do_exit(unsigned long exit_code);
	void myos_schedule(void);
	void try_sched(void);
	task_s *myos_kernel_thread(unsigned long (* fn)(unsigned long),
			unsigned long arg, unsigned long flags, const char *taskname);
	unsigned long kernel_init(unsigned long arg);
	int user_thread_test(unsigned long (* fn)(unsigned long),
			unsigned long arg, unsigned long flags);
	unsigned long user_func(unsigned long arg);

	/* interrupt.c */
	void excep_hwint_entry(stack_frame_s * sf_regs);
	void exception_handler(stack_frame_s * sf_regs);
	void hwint_irq_handler(stack_frame_s * sf_regs);
	int register_irq(unsigned long irq, void * arg, char * irq_name,
			unsigned long parameter, hw_int_controller_s * controller,
			void (*handler)(unsigned long parameter, stack_frame_s * sf_regs));
	int unregister_irq(unsigned long irq);
	int register_IPI(unsigned long irq, void * arg, char * irq_name,
			unsigned long parameter, hw_int_controller_s * controller,
			void (*handler)(unsigned long parameter, stack_frame_s * sf_regs));
	int unregister_IPI(unsigned long irq);
	void myos_init_bsp_intr(void);
	void myos_init_percpu_intr(void);

	/* smp.c */
	void init_cpu(void);
	void myos_early_init_smp(size_t lcpu_nr);
	void myos_init_smp(size_t lcpu_nr);
	void start_SMP(uint64_t apic_id);
	void myos_startup_smp(void);
	void myos_percpu_self_config(size_t cpu_idx);

	/* utils.c */
	bool verify_area(unsigned char* addr, unsigned long size);
	long copy_from_user(void * to, void * from, unsigned long size);
	long copy_to_user(void * to, void * from, unsigned long size);
	long strncpy_from_user(void * to, void * from, unsigned long size);
	long strlen_user(void * src);
	long strnlen_user(void * src, unsigned long maxlen);

	/*  mm.c */
	void creat_exec_addrspace(task_s * task);
	void prepair_COW(task_s * task);
	int do_COW(task_s * task, virt_addr_t vaddr);
	int check_addr_writable(reg_t cr2, task_s * task);
	reg_t do_brk(reg_t start, reg_t length);

#endif /* _AMD64_PROTO_H_ */