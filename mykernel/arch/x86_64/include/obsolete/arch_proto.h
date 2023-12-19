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
	} myos_irq_desc_s;

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