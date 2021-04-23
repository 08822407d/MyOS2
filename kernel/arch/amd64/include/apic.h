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

#ifndef __APIC_H__
#define __APIC_H__

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

	//delivery mode
	#define	APIC_ICR_IOAPIC_Fixed			0	//	LAPIC	IOAPIC 	ICR
	#define	IOAPIC_ICR_Lowest_Priority		1	//	IOAPIC 	ICR
	#define	APIC_ICR_IOAPIC_SMI				2	//	LAPIC	IOAPIC 	ICR

	#define	APIC_ICR_IOAPIC_NMI				4	//	LAPIC	IOAPIC 	ICR
	#define	APIC_ICR_IOAPIC_INIT			5	//	LAPIC	IOAPIC 	ICR
	#define	ICR_Start_up					6	//	ICR
	#define	IOAPIC_ExtINT					7	//	IOAPIC
	*/

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

#endif
