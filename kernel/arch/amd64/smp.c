/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#include <lib/string.h>
#include <lib/pthread.h>
#include <lib/stddef.h>

#include "include/arch_proto.h"
#include "include/archconst.h"
#include "include/arch_glo.h"
#include "include/smp.h"

#include "../../include/glo.h"
#include "../../include/printk.h"
#include "../../include/proto.h"
#include "../../include/proc.h"
#include "../../klib/data_structure.h"

percpu_data_s ** smp_info;

void SMP_init()
{
	extern char _APboot_phy_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apbbot_len = &_APboot_etext - &_APboot_text;
	memcpy(phy2vir((phy_addr)&_APboot_phy_start), (vir_addr)&_APboot_text, apbbot_len);

	// create structs for each logical cpu
	smp_info = (percpu_data_s **)kmalloc(kparam.lcpu_nr * sizeof(percpu_data_s *));
	memset(smp_info, 0 , kparam.lcpu_nr * sizeof(percpu_data_s *));
	for (size_t i = 0; i < kparam.lcpu_nr; i++)
	{
		smp_info[i] = (percpu_data_s *)kmalloc(sizeof(percpu_data_s));
		percpu_data_s * curr_cpudata = smp_info[i];
		memset(curr_cpudata, 0, sizeof(percpu_data_s));
		curr_cpudata->cpu_idx = i;
		curr_cpudata->cpu_stack_start =
				(char (*)[CONFIG_CPUSTACK_SIZE])kmalloc(sizeof(char [CONFIG_CPUSTACK_SIZE]));

		curr_cpudata->arch_info = (arch_percpu_data_s *)kmalloc(sizeof(arch_percpu_data_s));
		memset(curr_cpudata->arch_info, 0, sizeof(arch_percpu_data_s));
		arch_percpu_data_s * arch_cpuinfo = curr_cpudata->arch_info;
		arch_cpuinfo->lcpu_addr = apic_id[i];
		arch_cpuinfo->lcpu_topo_flag[0] = smp_topos[i].thd_id;
		arch_cpuinfo->lcpu_topo_flag[1] = smp_topos[i].core_id;
		arch_cpuinfo->lcpu_topo_flag[2] = smp_topos[i].pack_id;
		arch_cpuinfo->lcpu_topo_flag[3] = smp_topos[i].not_use;
		arch_cpuinfo->tss = tss_ptr_arr[i];
		// init tss's ists
		tss64_s * tss_p = tss_ptr_arr[i];
		tss_p->rsp1 =
		tss_p->rsp2 =
		tss_p->ist1 =
		tss_p->ist2 =
		tss_p->ist3 =
		tss_p->ist4 =
		tss_p->ist5 =
		tss_p->ist6 =
		tss_p->ist7 = (reg_t)(curr_cpudata->cpu_stack_start) + CONFIG_CPUSTACK_SIZE;
	}
	// init bsp's gsbase
	__asm__ __volatile__("wrgsbase	%%rax					\n"
					:
					:"a"(smp_info[0])
					:);
}

void start_SMP(uint64_t aptable_idx)
{
	percpu_data_s * curr_cpuinfo = smp_info[aptable_idx];

	tss64_s * tss_p = curr_cpuinfo->arch_info->tss;


	proc_s * curr_proc = (proc_s *)get_current();
	curr_cpuinfo->proc_jiffies = curr_proc->proc_jiffies;

	PCB_u * pcbu = container_of(curr_proc, PCB_u, proc);
	tss_p->rsp0 = (reg_t)pcbu + PROC_KSTACK_SIZE;

	curr_cpuinfo->curr_proc = curr_proc;
	curr_cpuinfo->finished_proc =
	curr_cpuinfo->waiting_proc = NULL;

	// load formal gdt and idt and tss
	__asm__ __volatile__("	lgdt	%0						\n\
							lidt	%1						\n\
							movq	%%rsp, %%rax			\n\
							mov 	%2, %%ss				\n\
							movq	%%rax, %%rsp			\n\
							mov		$0, %%ax				\n\
							mov 	%%ax, %%ds				\n\
							mov 	%%ax, %%es				\n\
							mov 	%%ax, %%fs				\n\
							mov 	%%ax, %%gs				\n\
							xor		%%rax, %%rax			\n\
							movq	%3, %%rax				\n\
							pushq	%%rax					\n\
							leaq	reload_cs(%%rip), %%rax	\n\
							pushq	%%rax					\n\
							lretq							\n\
							reload_cs:						\n\
							xorq	%%rax, %%rax			\n\
							mov		%4, %%ax				\n\
							ltr		%%ax					\n	"
						 :
						 :	"m"(gdt_ptr),
						 	"m"(idt_ptr),
						 	"r"(KERN_SS_SELECTOR),
							"rsi"((uint64_t)KERN_CS_SELECTOR),
							"r"((uint16_t)TSS_SELECTOR(aptable_idx))
						 :  "rax");

	unsigned int x,y;
	//enable xAPIC & x2APIC
	__asm__ __volatile__("movq 	$0x1b,	%%rcx	\n\t"
						 "rdmsr					\n\t"
						 "bts	$10,	%%rax	\n\t"
						 "bts	$11,	%%rax	\n\t"
						 "wrmsr					\n\t"
						 "movq 	$0x1b,	%%rcx	\n\t"
						 "rdmsr	\n\t"
						:"=a"(x),"=d"(y)
						:
						:"memory");

	//enable SVR[8]
	__asm__ __volatile__("movq 	$0x80f,	%%rcx	\n\t"
						 "rdmsr					\n\t"
						 "bts	$8,		%%rax	\n\t"
						 "bts	$12,	%%rax	\n\t"
						 "wrmsr					\n\t"
						 "movq 	$0x80f,	%%rcx	\n\t"
						 "rdmsr					\n\t"
						:"=a"(x),"=d"(y)
						:
						:"memory");

	//get local APIC ID
	__asm__ __volatile__("movq $0x802,	%%rcx	\n\t"
						 "rdmsr					\n\t"
						:"=a"(x),"=d"(y)
						:
						:"memory");
	
	// on Intel platform, write %gs will clean gsbase, so the following
	// operation should be done after reload segment regs
	__asm__ __volatile__("wrgsbase	%%rax		\n"
						:
						:"a"(curr_cpuinfo)
						:);

	color_printk(RED,YELLOW,"APU starting...... INDEX: %d; x2APIC ID:%#010x\n", aptable_idx, x);

	int i = 1 / 0;
	while (1)
	{
		hlt();
	}
}