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

void init_cpu(void)
{
	int i,j;
	unsigned int CpuFacName[4] = {0,0,0,0};
	char	FactoryName[17] = {0};

	//vendor_string
	cpuid(0,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);

	*(unsigned int*)&FactoryName[0] = CpuFacName[1];

	*(unsigned int*)&FactoryName[4] = CpuFacName[3];

	*(unsigned int*)&FactoryName[8] = CpuFacName[2];	

	FactoryName[12] = '\0';
	color_printk(YELLOW,BLACK,"%s\t%#010x\t%#010x\t%#010x\n",FactoryName,CpuFacName[1],CpuFacName[3],CpuFacName[2]);
	
	//brand_string
	for(i = 0x80000002;i < 0x80000005;i++)
	{
		cpuid(i,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);

		*(unsigned int*)&FactoryName[0] = CpuFacName[0];

		*(unsigned int*)&FactoryName[4] = CpuFacName[1];

		*(unsigned int*)&FactoryName[8] = CpuFacName[2];

		*(unsigned int*)&FactoryName[12] = CpuFacName[3];

		FactoryName[16] = '\0';
		color_printk(YELLOW,BLACK,"%s",FactoryName);
	}
	color_printk(YELLOW,BLACK,"\n");

	//Version Informatin Type,Family,Model,and Stepping ID
	cpuid(1,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	color_printk(YELLOW,BLACK,"Family Code:%#010x,Extended Family:%#010x,Model Number:%#010x,Extended Model:%#010x,Processor Type:%#010x,Stepping ID:%#010x\n",(CpuFacName[0] >> 8 & 0xf),(CpuFacName[0] >> 20 & 0xff),(CpuFacName[0] >> 4 & 0xf),(CpuFacName[0] >> 16 & 0xf),(CpuFacName[0] >> 12 & 0x3),(CpuFacName[0] & 0xf));

	//get Linear/Physical Address size
	cpuid(0x80000008,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	color_printk(YELLOW,BLACK,"Physical Address size:%08d,Linear Address size:%08d\n",(CpuFacName[0] & 0xff),(CpuFacName[0] >> 8 & 0xff));

	//max cpuid operation code
	cpuid(0,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	color_printk(WHITE,BLACK,"MAX Basic Operation Code :%#010x\t",CpuFacName[0]);

	cpuid(0x80000000,0,&CpuFacName[0],&CpuFacName[1],&CpuFacName[2],&CpuFacName[3]);
	color_printk(WHITE,BLACK,"MAX Extended Operation Code :%#010x\n",CpuFacName[0]);


}

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
	unsigned int x,y;

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

	hlt();
}