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
#include <linux/lib/string.h>
#include <linux/mm/mm.h>
#include <asm/apic.h>

#include <asm/processor.h>

#include <obsolete/ktypes.h>
#include <obsolete/printk.h>
#include <obsolete/arch_glo.h>
#include <obsolete/arch_proto.h>
#include <obsolete/interrupt.h>

ioapic_map_s ioapic_map;

/*==============================================================================================*
 *									repeatedly use functions									*
 *==============================================================================================*/

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
	// ioapic_retentry_T *entry = (ioapic_retentry_T *)arg;
	ioapic_rte_write(irq_nr * 2 + 0x10, *(unsigned long *)arg);
	return 1;
}

void IOAPIC_uninstall(unsigned long irq_nr)
{
	ioapic_rte_write(irq_nr * 2 + 0x10,0x10000UL);
}

void IOAPIC_level_ack(unsigned long irq_nr)
{
	IOAPIC_edge_ack(irq_nr);
				
	*ioapic_map.virt_EOI_addr = irq_nr;
}

void IOAPIC_edge_ack(unsigned long irq)
{
	apic_msr_write(APIC_EOI, 0);
}

uint64_t ioapic_rte_read(uint8_t index)
{
	uint64_t ret;

	*ioapic_map.virt_idx_addr = index + 1;
	mb();
	ret = *ioapic_map.virt_data_addr;
	ret <<= 32;
	mb();

	*ioapic_map.virt_idx_addr= index;		
	mb();
	ret |= *ioapic_map.virt_data_addr;
	mb();

	return ret;
}

void ioapic_rte_write(uint8_t index, uint64_t value)
{
	*ioapic_map.virt_idx_addr = index;
	mb();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	mb();
	
	value >>= 32;

	*ioapic_map.virt_idx_addr = index + 1;
	mb();
	*ioapic_map.virt_data_addr = value & 0xffffffff;
	mb();
}

/*==============================================================================================*
 *										once functions 1										*
 *==============================================================================================*/
inline __always_inline unsigned get_x2apic_id()
{
	return apic_msr_read(APIC_ID);
}

inline __always_inline void get_lapic_ver(lapic_info_s * lapic_info)
{
	u32 x;
	x = apic_msr_read(APIC_LVR);
	lapic_info->lapic_ver = x & 0xFF;
	lapic_info->max_lvt = (x >> 16 & 0xff) + 1;
	lapic_info->svr12_support = x >> 24 & 0x1;
}

inline __always_inline void disable_lvt(lapic_info_s * lapic_info)
{
	u32 val = 0x10000;
	if (lapic_info->max_lvt >= 7)
		apic_msr_write(APIC_LVTCMCI, val);
	apic_msr_write(APIC_LVTT, val);
	apic_msr_write(APIC_LVTTHMR, val);
	apic_msr_write(APIC_LVTPC, val);
	apic_msr_write(APIC_LVT0, val);
	apic_msr_write(APIC_LVT1, val);
	apic_msr_write(APIC_LVTERR, val);
}

inline __always_inline unsigned get_lvt_tpr()
{
	return apic_msr_read(APIC_TASKPRI);
}

inline __always_inline unsigned get_lvt_ppr()
{
	return apic_msr_read(APIC_PROCPRI);
}

/*==============================================================================================*
 *										once functions 2								 		*
 *==============================================================================================*/
void init_lapic()
{
	lapic_info_s lapic_info;
	unsigned x2apic_id = get_x2apic_id();

	get_lapic_ver(&lapic_info);

	disable_lvt(&lapic_info);

	unsigned lvt_tpr = get_lvt_tpr();

	unsigned lvt_ppr = get_lvt_ppr();
}


void IOAPIC_init()
{
	uint8_t *IOAPIC_addr =
		(uint8_t *)phys_to_virt((phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE);

	ioapic_map.phys_addr = (phys_addr_t)IO_APIC_DEFAULT_PHYS_BASE;
	ioapic_map.virt_idx_addr = IOAPIC_addr;
	ioapic_map.virt_data_addr = (uint32_t *)(IOAPIC_addr + 0x10);
	ioapic_map.virt_EOI_addr = (uint32_t *)(IOAPIC_addr + 0x40);

	int i ;
	//	I/O APIC
	//	I/O APIC	ID	
	*ioapic_map.virt_idx_addr = 0x00;
	mb();
	*ioapic_map.virt_data_addr = 0x0f000000;
	mb();
	mb();

	//	I/O APIC	Version
	*ioapic_map.virt_idx_addr = 0x01;
	mb();

	//RTE	
	for(i = 0x10;i < 0x40;i += 2)
		ioapic_rte_write(i, 0x10000 + APIC_IRQ0_VEC + ((i - 0x10) >> 1));
}