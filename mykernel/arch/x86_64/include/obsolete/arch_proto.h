#ifndef _AMD64_PROTO_H_
#define _AMD64_PROTO_H_

#include <linux/kernel/types.h>
#include <linux/kernel/sched.h>
#include <linux/kernel/ptrace.h>
#include <linux/sched/task.h>

#include "mutex.h"

/*==============================================================================================*
 *										external symbols							 			*
 *==============================================================================================*/
	/* Exception handlers (real or protected mode), in numerical order. */
	extern void divide_error(void);
	#define cpu_excep_handler_00	divide_error
	extern void debug(void);
	#define cpu_excep_handler_01	debug
	extern void nmi(void);
	#define cpu_excep_handler_02	nmi
	extern void breakpoint_exception(void);
	#define cpu_excep_handler_03	breakpoint_exception
	extern void overflow(void);
	#define cpu_excep_handler_04	overflow
	extern void bounds_exceed(void);
	#define cpu_excep_handler_05	bounds_exceed
	extern void invalid_opcode(void);
	#define cpu_excep_handler_06	invalid_opcode
	extern void dev_not_available(void);
	#define cpu_excep_handler_07	dev_not_available
	extern void double_fault(void);
	#define cpu_excep_handler_08	double_fault
	extern void cotask_seg_overrun(void);
	#define cpu_excep_handler_09	cotask_seg_overrun
	extern void invalid_tss(void);
	#define cpu_excep_handler_10	invalid_tss
	extern void segment_not_present(void);
	#define cpu_excep_handler_11	segment_not_present
	extern void stack_segfault(void);
	#define cpu_excep_handler_12	stack_segfault
	extern void general_protection(void);
	#define cpu_excep_handler_13	general_protection
	extern void page_fault(void);
	#define cpu_excep_handler_14	page_fault
	// EXCEPTION 15 INTEL RESERVED
	extern void x87_fpu_error(void);
	#define cpu_excep_handler_16	x87_fpu_error
	extern void alignment_check(void);
	#define cpu_excep_handler_17	alignment_check
	extern void machine_check(void);
	#define cpu_excep_handler_18	machine_check
	extern void simd_exception(void);
	#define cpu_excep_handler_19	simd_exception
	extern void virtualization_exception(void);
	#define cpu_excep_handler_20	virtualization_exception
	extern void control_protection_exception(void);
	#define cpu_excep_handler_21	control_protection_exception
	/* Hardware interrupt handlers. */
	extern void hwint00(void);
	extern void hwint01(void);
	extern void hwint02(void);
	extern void hwint03(void);
	extern void hwint04(void);
	extern void hwint05(void);
	extern void hwint06(void);
	extern void hwint07(void);
	extern void hwint08(void);
	extern void hwint09(void);
	extern void hwint10(void);
	extern void hwint11(void);
	extern void hwint12(void);
	extern void hwint13(void);
	extern void hwint14(void);
	extern void hwint15(void);
	/* APIC Hardware interrupt handlers. */
	extern void hwint16(void);
	extern void hwint17(void);
	extern void hwint18(void);
	extern void hwint19(void);
	extern void hwint20(void);
	extern void hwint21(void);
	extern void hwint22(void);
	extern void hwint23(void);
	/* LAPIC IPI handlers. */
	extern void lapic_ipi00(void);
	extern void lapic_ipi01(void);
	extern void lapic_ipi02(void);
	extern void lapic_ipi03(void);
	extern void lapic_ipi04(void);
	extern void lapic_ipi05(void);
	extern void lapic_ipi06(void);
	extern void lapic_ipi07(void);

	/* syscalls and others about intr */
	extern void sysenter_entp(void);
	extern void sysexit_entp(void);

/*==============================================================================================*
 *										internal symbols										*
 *==============================================================================================*/
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
						pt_regs_s *stack_frame);
	} irq_desc_s;

	/* cpu info */
	struct cputopo
	{
		uint8_t		not_use;
		uint8_t		pack_id;
		uint8_t		core_id;
		uint8_t		thd_id;
	};

	/* init.c */
	void myos_unmap_kernel_lowhalf(atomic_t *um_flag);

	/* i8259.c */
	void init_i8259(void);
	void i8259_unmask(const int);
	void i8259_mask(const int);
	void i8259_disable(void);
	void i8259_eoi(int);
	/* apic.c */
	void IOAPIC_init(void);
	void IOAPIC_enable(unsigned long irq);
	void IOAPIC_disable(unsigned long irq);
	unsigned long IOAPIC_install(unsigned long irq,void * arg);
	void IOAPIC_uninstall(unsigned long irq);
	void IOAPIC_level_ack(unsigned long irq);
	void IOAPIC_edge_ack(unsigned long irq);
	uint64_t ioapic_rte_read(uint8_t index);
	void ioapic_rte_write(uint8_t index, uint64_t value);
	/* apic_timer.c */
	void LVT_timer_init(void);

	/* arch_task.c */
	unsigned long do_exit(unsigned long exit_code);
	void try_sched(void);
	int kernel_init(void *arg);

	/* interrupt.c */
	void excep_hwint_context(pt_regs_s *sf_regs);
	void exception_handler(pt_regs_s *sf_regs);
	void hwint_irq_handler(pt_regs_s *sf_regs);
	int register_irq(unsigned long irq, void * arg, char * irq_name,
			unsigned long parameter, hw_int_controller_s * controller,
			void (*handler)(unsigned long parameter, pt_regs_s *sf_regs));
	int unregister_irq(unsigned long irq);
	int register_IPI(unsigned long irq, void * arg, char * irq_name,
			unsigned long parameter, hw_int_controller_s * controller,
			void (*handler)(unsigned long parameter, pt_regs_s *sf_regs));
	int unregister_IPI(unsigned long irq);
	void myos_init_bsp_intr(void);
	void myos_init_percpu_intr(void);

	/* smp.c */
	void myos_early_init_smp(void);
	void myos_init_smp(size_t lcpu_nr);
	void myos_startup_smp(void);
	void myos_percpu_self_config(size_t cpu_idx);

	/* utils.c */
	long copy_from_user(void * to, void * from, unsigned long size);
	long copy_to_user(void * to, void * from, unsigned long size);
	long strncpy_from_user(void * to, void * from, unsigned long size);
	long strlen_user(void * src);
	long myos_strnlen_user(void * src, unsigned long maxlen);

#endif /* _AMD64_PROTO_H_ */