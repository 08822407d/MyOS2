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

#include "include/arch_proto.h"
#include "include/arch_glo.h"
#include "include/smp.h"

#include "../../include/printk.h"
#include "../../include/proc.h"


void SMP_init()
{
	int i;
	unsigned int a,b,c,d;

	//get local APIC ID
	for(i = 0;;i++)
	{
		cpuid(0xb,i,&a,&b,&c,&d);
		if((c >> 8 & 0xff) == 0)
			break;
		color_printk(WHITE,BLACK,"local APIC ID Package_../Core_2/SMT_1,type(%x) Width:%#010x,num of logical processor(%x)\n",c >> 8 & 0xff,a & 0x1f,b & 0xff);
	}
	
	color_printk(WHITE,BLACK,"x2APIC ID level:%#010x\tx2APIC ID the current logical processor:%#010x\n",c & 0xff,d);
	
	extern char _APboot_phy_start;
	extern char _APboot_text;
	extern char _APboot_etext;
	size_t apbbot_len = &_APboot_etext - &_APboot_text;
	memcpy(phy2vir((phy_addr)&_APboot_phy_start), (vir_addr)&_APboot_text, apbbot_len);
}

void start_SMP(uint64_t aptable_idx)
{
	PCB_u * curr_proc = (PCB_u *)get_current();
	tss64_s * tss_p = tss_ptr_arr[aptable_idx];
	tss_p->rsp0 = (reg_t)curr_proc + PROC_KSTACK_SIZE;
	tss_p->rsp1 =
	tss_p->rsp2 =
	tss_p->ist1 =
	tss_p->ist2 =
	tss_p->ist3 =
	tss_p->ist4 =
	tss_p->ist5 =
	tss_p->ist6 =
	tss_p->ist7 = 0;

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
	
	color_printk(RED,YELLOW,"APU starting...... INDEX: %d; x2APIC ID:%#010x\n", aptable_idx, x);

	while (1)
	{
		hlt();
	}
}