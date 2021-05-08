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

#include "include/arch_proto.h"
#include "include/archtypes.h"
#include "include/archconst.h"
#include "include/interrupt.h"
#include "include/apic.h"

#include "../../include/ktypes.h"
#include "../../include/printk.h"

ioapic_map_s ioapic_map;

/*===========================================================================*
 *							repeatedly use functions						 *
 *===========================================================================*/
void IOAPIC_enable(unsigned long irq_nr)
{
	unsigned long value = 0;
	value = ioapic_rte_read(irq_nr * 2 + 0x10);
	value = value & (~0x10000UL); 
	ioapic_rte_write(irq_nr * 2 + 0x10, value);
}

void IOAPIC_disable(unsigned long irq_nr)
{
	unsigned long value = 0;
	value = ioapic_rte_read(irq_nr * 2 + 0x10);
	value = value | 0x10000UL; 
	ioapic_rte_write(irq_nr * 2 + 0x10, value);
}

unsigned long IOAPIC_install(unsigned long irq_nr, void * arg)
{
	ioapic_retentry_s *entry = (ioapic_retentry_s *)arg;
	ioapic_rte_write(irq_nr * 2 + 0x10, *(unsigned long *)entry);

	return 1;
}

void IOAPIC_uninstall(unsigned long irq_nr)
{
	ioapic_rte_write(irq_nr * 2 + 0x10,0x10000UL);
}

void IOAPIC_level_ack(unsigned long irq_nr)
{
	__asm__ __volatile__("movq	$0x00,	%%rdx	\n\t"
						 "movq	$0x00,	%%rax	\n\t"
						 "movq 	$0x80b,	%%rcx	\n\t"
						 "wrmsr	\n\t"
						 :::"memory","rax","rcx","rdx");
				
	*ioapic_map.virt_EOI_addr = irq_nr;
}

void IOAPIC_edge_ack(unsigned long irq)
{
	__asm__ __volatile__("movq	$0x00,	%%rdx	\n\t"
						 "movq	$0x00,	%%rax	\n\t"
						 "movq 	$0x80b,	%%rcx	\n\t"
						 "wrmsr	\n\t"
						:::"memory","rax","rcx","rdx");
}

uint64_t ioapic_rte_read(uint8_t index)
{
	uint64_t ret;

	*ioapic_map.virt_idx_addr = index + 1;
	io_mfence();
	ret = *ioapic_map.virt_data_addr;
	ret <<= 32;
	io_mfence();

	*ioapic_map.virt_idx_addr= index;		
	io_mfence();
	ret |= *ioapic_map.virt_data_addr;
	io_mfence();

	return ret;
}

void ioapic_rte_write(uint8_t index, uint64_t value)
{
	*ioapic_map.virt_idx_addr = index;
	io_mfence();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	io_mfence();
	
	value >>= 32;

	*ioapic_map.virt_idx_addr = index + 1;
	io_mfence();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	io_mfence();
}

/*===========================================================================*
 *								once functions 1							 *
 *===========================================================================*/
inline __always_inline void enable_x2apic()
{
	//enable xAPIC & x2APIC
	__asm__ __volatile__(	"movq 	$0x1b,	%%rcx	\n\t"
							"rdmsr					\n\t"
							"bts	$10,	%%rax	\n\t"
							"bts	$11,	%%rax	\n\t"
							"wrmsr					\n\t"
							"movq 	$0x1b,	%%rcx	\n\t"
							"rdmsr					\n\t"
						:
						:
						:	"rcx", "rax", "rdx");
}

inline __always_inline void open_lapic()
{
	__asm__ __volatile__(	"movq 	$0x80f,	%%rcx	\n\t"
							"rdmsr					\n\t"
							"bts	$8,		%%rax	\n\t"
							"bts	$12,	%%rax	\n\t"
							"wrmsr					\n\t"
							"movq 	$0x80f,	%%rcx	\n\t"
							"rdmsr					\n\t"
						:
						:
						:	"rcx", "rax", "rdx");
}

inline __always_inline unsigned get_x2apic_id()
{
	unsigned ret_val;
	__asm__ __volatile__(	"movq $0x802,	%%rcx	\n\t"
							"rdmsr					\n\t"
						:	"=a"(ret_val)
						:
						:	"rcx", "rdx");
	return ret_val;
}

inline __always_inline void get_lapic_ver(lapic_info_s * lapic_info)
{
	unsigned x, y;
	__asm__ __volatile__(	"movq $0x803,	%%rcx	\n\t"
							"rdmsr	\n\t"
						:	"=a"(x),"=d"(y)
						:
						:	"memory");	
	lapic_info->lapic_ver = x & 0xFF;
	lapic_info->max_lvt = (x >> 16 & 0xff) + 1;
	lapic_info->svr12_support = x >> 24 & 0x1;
}

inline __always_inline void disable_lvt(lapic_info_s * lapic_info)
{
	__asm__ __volatile__(	"cmpq	$0x07,	%%rbx	\n\t"	// if max_lvt smaller than 7,it means the paltform does not
							"jb		(. + 0xB)		\n\t"	// support CMCI register,for example bochs, vbox and qemu
							"movq 	$0x82f,	%%rcx	\n\t"	// CMCI
 							"wrmsr					\n\t"
							"movq 	$0x832,	%%rcx	\n\t"	// Timer
 							"wrmsr					\n\t"
							"movq 	$0x833,	%%rcx	\n\t"	// Thermal Monitor
							"wrmsr					\n\t"
							"movq 	$0x834,	%%rcx	\n\t"	// Performance Counter
							"wrmsr					\n\t"
							"movq 	$0x835,	%%rcx	\n\t"	// LINT0
							"wrmsr					\n\t"
							"movq 	$0x836,	%%rcx	\n\t"	// LINT1
							"wrmsr					\n\t"
							"movq 	$0x837,	%%rcx	\n\t"	// Error
							"wrmsr					\n\t"
						:
						:	"a"(0x10000),"d"(0x00),"b"(lapic_info->max_lvt)
						:	"memory");	
}

inline __always_inline unsigned get_lvt_tpr()
{
	unsigned ret_val;
	__asm__ __volatile__(	"movq 	$0x808,	%%rcx	\n\t"
							"rdmsr					\n\t"
						:	"=a"(ret_val)
						:
						:	"rcx", "rdx");
	return ret_val;
}

inline __always_inline unsigned get_lvt_ppr()
{
	unsigned ret_val;
	__asm__ __volatile__(	"movq 	$0x80a,	%%rcx	\n\t"
							"rdmsr					\n\t"
						:	"=a"(ret_val)
						:
						:	"rcx", "rdx");
	return ret_val;
}

/*===========================================================================*
 *								once functions 2							 *
 *===========================================================================*/
void IOAPIC_pagetable_remap()
{
	unsigned long * tmp;
	uint8_t * IOAPIC_addr = (uint8_t *)phys2virt((phys_addr)0xfec00000);

	ioapic_map.phys_addr = (phys_addr)0xfec00000;
	ioapic_map.virt_idx_addr = IOAPIC_addr;
	ioapic_map.virt_data_addr = (uint32_t *)(IOAPIC_addr + 0x10);
	ioapic_map.virt_EOI_addr = (uint32_t *)(IOAPIC_addr + 0x40);
	
	uint64_t page_attr = ARCH_PG_PRESENT | ARCH_PG_RW | ARCH_PG_PWT | ARCH_PG_PCD | ARCH_PG_PAT;
	pg_domap(ioapic_map.virt_idx_addr, ioapic_map.phys_addr, page_attr);
}

void LAPIC_init()
{
	lapic_info_s lapic_info;

	unsigned int x,y;
	unsigned int a,b,c,d;
	//check APIC & x2APIC support
	cpuid(1,0,&a,&b,&c,&d);

	enable_x2apic();

	open_lapic();

	unsigned x2apic_id = get_x2apic_id();

	get_lapic_ver(&lapic_info);

	disable_lvt(&lapic_info);

	unsigned lvt_tpr = get_lvt_tpr();

	unsigned lvt_ppr = get_lvt_ppr();

// #ifdef DEBUG
// 	if((1<<9) & d)
// 		color_printk(WHITE, BLACK, "APIC&xAPIC supported;\t");
// 	else
// 		color_printk(WHITE, BLACK, "APIC&xAPIC unsupported;\t");
	
// 	if((1<<21) & c)
// 		color_printk(WHITE, BLACK, "x2APIC supported\n");
// 	else
// 		color_printk(WHITE, BLACK, "x2APIC unsupported\n");

// 	if(x_x2_apic_enabled )
// 		color_printk(WHITE,BLACK,"xAPIC & x2APIC enabled\n");

// 	if(svr8_enabled)
// 		color_printk(WHITE,BLACK,"SVR[8] enabled\n");
// 	if(svr12_enabled)
// 		color_printk(WHITE,BLACK,"SVR[12] enabled\n");

// 	color_printk(WHITE,BLACK,"x2APIC ID:%#010x\n", x2apic_id);

// 	color_printk(WHITE,BLACK,"local APIC Version:%#010x,Max LVT Entry:%#010x,SVR(Suppress EOI Broadcast):%#04x\t",
// 					lapic_ver, max_lvt, svr12_support);
// 	if(lapic_ver < 0x10)
// 		color_printk(WHITE,BLACK,"82489DX discrete APIC\n");
// 	else if( (lapic_ver >= 0x10) && (lapic_ver <= 0x15) )
// 		color_printk(WHITE,BLACK,"Integrated APIC\n");
// 	color_printk(GREEN,BLACK,"Mask ALL LVT\n");

// 	color_printk(GREEN,BLACK,"Set LVT TPR:%#010x\t",x);
// 	color_printk(GREEN,BLACK,"Set LVT PPR:%#010x\n", lvt_ppr);
// #endif
}


void IOAPIC_init()
{
	int i ;
	//	I/O APIC
	//	I/O APIC	ID	
	*ioapic_map.virt_idx_addr = 0x00;
	io_mfence();
	*ioapic_map.virt_data_addr = 0x0f000000;
	io_mfence();
	io_mfence();

	//	I/O APIC	Version
	*ioapic_map.virt_idx_addr = 0x01;
	io_mfence();

	//RTE	
	for(i = 0x10;i < 0x40;i += 2)
		ioapic_rte_write(i, 0x10000 + APIC_IRQ0_VEC + ((i - 0x10) >> 1));

#ifdef DEBUG
	color_printk(GREEN, BLACK, "Get IOAPIC ID REG:%#010x,ID:%#010x\n",
					*ioapic_map.virt_data_addr, *ioapic_map.virt_data_addr>> 24 & 0xf);
	color_printk(GREEN, BLACK, "Get IOAPIC Version REG:%#010x,MAX redirection enties:%#08d\n",
					*ioapic_map.virt_data_addr, ((*ioapic_map.virt_data_addr>> 16) & 0xff) + 1);
	color_printk(GREEN, BLACK, "I/O APIC Redirection Table Entries Set Finished.\n");	
#endif
}

void LAPIC_IOAPIC_init()
{
	IOAPIC_pagetable_remap();

	i8259_disable();

	//init local apic
	LAPIC_init();

	//init ioapic
	IOAPIC_init();

	sti();
}