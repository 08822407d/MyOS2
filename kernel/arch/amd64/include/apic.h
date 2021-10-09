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

#ifndef _AMD64_APIC_H_
#define _AMD64_APIC_H_

#include <sys/types.h>
#include <sys/cdefs.h>
#include <lib/utils.h>

#include "archtypes.h"
#include "../../../include/ktypes.h"

	/*
	32  ~   55	I/O APIC
		32	8259A
		33	keyboard
		34	HPET timer 0,8254 counter 0
		35	serial port A
		36	serial port B
		37	parallel port
		38	floppy
		39	parallel port
		40	RTC,HPET timer 1
		41	Generic
		42	Generic
		43	HPET timer 2
		44	HPET timer 3
		45	FERR#
		46	SATA primary
		47	SATA secondary
		48	PIRQA
		49	PIRQB
		50	PIRQC
		51	PIRQD
		52	PIRQE
		53	PIRQF
		54	PIRQG
		55	PIRQH
		
	0x80		system call

	150 ~   200	Local APIC
		150	CMCI
		151	Timer
		152	Thermal Monitor
		153	Performance Counter
		154	LINT0
		155	LINT1
		156	Error

	200 ~   255	MP IPI

	1:	LVT	CMCI
	2:	LVT	Timer
	3:	LVT	Thermal Monitor
	4:	LVT	Performace Counter
	5:	LVT	LINT0
	6:	LVT	LINT1
	7:	LVT	Error
	*/

	//delivery mode
	#define	APIC_ICR_IOAPIC_Fixed			0	//	LAPIC	IOAPIC 	ICR
	#define	IOAPIC_ICR_Lowest_Priority		1	//	IOAPIC 	ICR
	#define	APIC_ICR_IOAPIC_SMI				2	//	LAPIC	IOAPIC 	ICR

	#define	APIC_ICR_IOAPIC_NMI				4	//	LAPIC	IOAPIC 	ICR
	#define	APIC_ICR_IOAPIC_INIT			5	//	LAPIC	IOAPIC 	ICR
	#define	ICR_Start_up					6	//	ICR
	#define	IOAPIC_ExtINT					7	//	IOAPIC

	//timer mode
	#define APIC_LVT_Timer_One_Shot			0
	#define APIC_LVT_Timer_Periodic			1
	#define APIC_LVT_Timer_TSC_Deadline		2

	//mask
	#define APIC_ICR_IOAPIC_Masked			1
	#define APIC_ICR_IOAPIC_UN_Masked		0

	//trigger mode
	#define APIC_ICR_IOAPIC_Edge			0
	#define APIC_ICR_IOAPIC_Level			1

	//delivery status
	#define APIC_ICR_IOAPIC_Idle			0
	#define APIC_ICR_IOAPIC_Send_Pending	1

	//destination shorthand
	#define ICR_No_Shorthand				0
	#define ICR_Self						1
	#define ICR_ALL_INCLUDE_Self			2
	#define ICR_ALL_EXCLUDE_Self			3

	//destination mode
	#define ICR_IOAPIC_DELV_PHYSICAL		0
	#define ICR_IOAPIC_DELV_LOGIC			1

	//level
	#define ICR_LEVEL_DE_ASSERT				0
	#define ICR_LEVLE_ASSERT				1

	//remote irr
	#define APIC_IOAPIC_IRR_RESET			0
	#define APIC_IOAPIC_IRR_ACCEPT			1

	//pin polarity
	#define APIC_IOAPIC_POLARITY_HIGH		0
	#define APIC_IOAPIC_POLARITY_LOW		1

	//
	#define LAPIC_MMIO_TO_MSR(mmio)			(0x800 | mmio >> 4)

	#define LAPIC_IDREG_MMIO				0x020
	#define LAPIC_IDREG_MSR					LAPIC_MMIO_TO_MSR(LAPIC_IDREG_MMIO)

	#define LAPIC_VERREG_MMIO				0x030
	#define LAPIC_VERREG_MSR				LAPIC_MMIO_TO_MSR(LAPIC_VERREG_MMIO)

	#define LAPIC_TPR_MMIO					0x080
	#define LAPIC_TPR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_TPR_MMIO)

	#define LAPIC_APR_MMIO					0x090

	#define LAPIC_PPR_MMIO					0x0A0
	#define LAPIC_PPR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_PPR_MMIO)

	#define LAPIC_EOI_MMIO					0x0B0
	#define LAPIC_EOI_MSR					LAPIC_MMIO_TO_MSR(LAPIC_EOI_MMIO)

	#define LAPIC_RRD_MMIO					0x0C0

	#define LAPIC_LDR_MMIO					0x0D0
	#define LAPIC_LDR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_LDR_MMIO)

	#define LAPIC_DFR_MMIO					0x0E0

	#define LAPIC_SVR_MMIO					0x0F0
	#define LAPIC_SVR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_SVR_MMIO)

	#define LAPIC_ISR_31_0_MMIO				0x100
	#define LAPIC_ISR_31_0_MSR				LAPIC_MMIO_TO_MSR(LAPIC_ISR_31_0_MMIO)

	#define LAPIC_ISR_63_32_MMIO			0x110
	#define LAPIC_ISR_63_32_MSR				LAPIC_MMIO_TO_MSR(LAPIC_ISR_63_32_MMIO)
	
	#define LAPIC_ISR_95_64_MMIO			0x120
	#define LAPIC_ISR_95_64_MSR				LAPIC_MMIO_TO_MSR(LAPIC_ISR_95_64_MMIO)

	#define LAPIC_ISR_127_96_MMIO			0x130
	#define LAPIC_ISR_127_96_MSR			LAPIC_MMIO_TO_MSR(LAPIC_ISR_127_96_MMIO)

	#define LAPIC_ISR_159_128_MMIO			0x140
	#define LAPIC_ISR_159_128_MSR			LAPIC_MMIO_TO_MSR(LAPIC_ISR_159_128_MMIO)

	#define LAPIC_ISR_191_160_MMIO			0x150
	#define LAPIC_ISR_191_160_MSR			LAPIC_MMIO_TO_MSR(LAPIC_ISR_191_160_MMIO)

	#define LAPIC_ISR_223_192_MMIO			0x160
	#define LAPIC_ISR_223_192_MSR			LAPIC_MMIO_TO_MSR(LAPIC_ISR_223_192_MMIO)

	#define LAPIC_ISR_255_224_MMIO			0x170
	#define LAPIC_ISR_255_224_MSR			LAPIC_MMIO_TO_MSR(LAPIC_ISR_255_224_MMIO)

	#define LAPIC_TMR_31_0_MMIO				0x180
	#define LAPIC_TMR_31_0_MSR				LAPIC_MMIO_TO_MSR(LAPIC_TMR_31_0_MMIO)

	#define LAPIC_TMR_63_32_MMIO			0x190
	#define LAPIC_TMR_63_32_MSR				LAPIC_MMIO_TO_MSR(LAPIC_TMR_63_32_MMIO)
	
	#define LAPIC_TMR_95_64_MMIO			0x1A0
	#define LAPIC_TMR_95_64_MSR				LAPIC_MMIO_TO_MSR(LAPIC_TMR_95_64_MMIO)

	#define LAPIC_TMR_127_96_MMIO			0x1B0
	#define LAPIC_TMR_127_96_MSR			LAPIC_MMIO_TO_MSR(LAPIC_TMR_127_96_MMIO)

	#define LAPIC_TMR_159_128_MMIO			0x1C0
	#define LAPIC_TMR_159_128_MSR			LAPIC_MMIO_TO_MSR(LAPIC_TMR_159_128_MMIO)

	#define LAPIC_TMR_191_160_MMIO			0x1D0
	#define LAPIC_TMR_191_160_MSR			LAPIC_MMIO_TO_MSR(LAPIC_TMR_191_160_MMIO)

	#define LAPIC_TMR_223_192_MMIO			0x1E0
	#define LAPIC_TMR_223_192_MSR			LAPIC_MMIO_TO_MSR(LAPIC_TMR_223_192_MMIO)

	#define LAPIC_TMR_255_224_MMIO			0x1F0
	#define LAPIC_TMR_255_224_MSR			LAPIC_MMIO_TO_MSR(LAPIC_TMR_255_224_MMIO)

	#define LAPIC_IRR_31_0_MMIO				0x200
	#define LAPIC_IRR_31_0_MSR				LAPIC_MMIO_TO_MSR(LAPIC_IRR_31_0_MMIO)

	#define LAPIC_IRR_63_32_MMIO			0x210
	#define LAPIC_IRR_63_32_MSR				LAPIC_MMIO_TO_MSR(LAPIC_IRR_63_32_MMIO)
	
	#define LAPIC_IRR_95_64_MMIO			0x220
	#define LAPIC_IRR_95_64_MSR				LAPIC_MMIO_TO_MSR(LAPIC_IRR_95_64_MMIO)

	#define LAPIC_IRR_127_96_MMIO			0x230
	#define LAPIC_IRR_127_96_MSR			LAPIC_MMIO_TO_MSR(LAPIC_IRR_127_96_MMIO)

	#define LAPIC_IRR_159_128_MMIO			0x240
	#define LAPIC_IRR_159_128_MSR			LAPIC_MMIO_TO_MSR(LAPIC_IRR_159_128_MMIO)

	#define LAPIC_IRR_191_160_MMIO			0x250
	#define LAPIC_IRR_191_160_MSR			LAPIC_MMIO_TO_MSR(LAPIC_IRR_191_160_MMIO)

	#define LAPIC_IRR_223_192_MMIO			0x260
	#define LAPIC_IRR_223_192_MSR			LAPIC_MMIO_TO_MSR(LAPIC_IRR_223_192_MMIO)

	#define LAPIC_IRR_255_224_MMIO			0x270
	#define LAPIC_IRR_255_224_MSR			LAPIC_MMIO_TO_MSR(LAPIC_IRR_255_224_MMIO)

	#define LAPIC_ESR_MMIO					0x280
	#define LAPIC_ESR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_ESR_MMIO)

	#define LAPIC_LVT_CMCI_REG_MMIO			0x2F0
	#define LAPIC_LVT_CMCI_REG_MSR			LAPIC_MMIO_TO_MSR(LAPIC_LVT_CMCI_REG_MMIO)

	#define LAPIC_ICR_MMIO_LOW				0x300
	#define LAPIC_ICR_MMIO_HIGH				0x310
	#define LAPIC_ICR_MSR					LAPIC_MMIO_TO_MSR(LAPIC_ICR_MMIO_LOW)

	#define LAPIC_LVT_TIMER_REG_MMIO		0x320
	#define LAPIC_LVT_TIMER_REG_MSR			LAPIC_MMIO_TO_MSR(LAPIC_LVT_TIMER_REG_MMIO)

	#define LAPIC_LVT_THERMAL_REG_MMIO		0x330
	#define LAPIC_LVT_THERMAL_REG_MSR		LAPIC_MMIO_TO_MSR(LAPIC_LVT_THERMAL_REG_MMIO)

	#define LAPIC_LVT_PERFORM_REG_MMIO		0x340
	#define LAPIC_LVT_PERFORM_REG_MSR		LAPIC_MMIO_TO_MSR(LAPIC_LVT_PERFORM_REG_MMIO)

	#define LAPIC_LVT_LINT0_REG_MMIO		0x350
	#define LAPIC_LVT_LINT0_REG_MSR			LAPIC_MMIO_TO_MSR(LAPIC_LVT_LINT0_REG_MMIO)

	#define LAPIC_LVT_LINT1_REG_MMIO		0x360
	#define LAPIC_LVT_LINT1_REG_MSR			LAPIC_MMIO_TO_MSR(LAPIC_LVT_LINT1_REG_MMIO)

	#define LAPIC_LVT_ERROR_REG_MMIO		0x370
	#define LAPIC_LVT_ERROR_REG_MSR			LAPIC_MMIO_TO_MSR(LAPIC_LVT_ERROR_REG_MMIO)

	#define LVT_TIMER_INIT_COUNT_REG_MMIO	0x380
	#define LVT_TIMER_INIT_COUNT_REG_MSR	LAPIC_MMIO_TO_MSR(LVT_TIMER_INIT_COUNT_REG_MMIO)

	#define LVT_TIMER_CURR_COUNT_REG_MMIO	0x390
	#define LVT_TIMER_CURR_COUNT_REG_MSR	LAPIC_MMIO_TO_MSR(LVT_TIMER_CURR_COUNT_REG_MMIO)

	#define LVT_TIMER_DIV_CONF_REG_MMIO		0x3E0
	#define LVT_TIMER_DIV_CONF_REG_MSR		LAPIC_MMIO_TO_MSR(LVT_TIMER_DIV_CONF_REG_MMIO)

	#define LAPIC_SELF_IPI_MSR				0x83F


#endif /* _AMD64_APIC_H_ */