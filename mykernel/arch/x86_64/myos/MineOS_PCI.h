/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __PCI_H__

#define __PCI_H__

#include <linux/kernel/types.h>

struct PCI_Header_00
{
	u32				bus;
	u32				device;
	u32				function;
	char			*devtype_name;


	unsigned int	BDF;

	unsigned short	VendorID;
	unsigned short	DeviceID;

	unsigned short	Command;
	unsigned short	Status;

	unsigned char	RevisionID;
	unsigned char	ProgIF;
	unsigned char	SubClass;
	unsigned char	ClassCode;

	unsigned char	CacheLineSize;
	unsigned char	LatencyTimer;
	unsigned char	HeaderType;
	unsigned char	BIST;

	// BAR 原始值
	u32				BAR_base_raw[6];
	u32				BAR_limit_raw[6];
	// BAR 解析值
	u64				BAR_base_addr[6];
	u64				BAR_space_limit[6];

	unsigned int	CardBusCISPointer;

	unsigned short	SubSystemVendorID;
	unsigned short	SubSystemID;

	unsigned int	ExpansionROMBaseAddress;

	unsigned int	CapabilitiesPointer	:8,
					Reserved0			:24;

	unsigned int	Reserved1;

	unsigned char	InterruptLine;
	unsigned char	InterruptPIN;
	unsigned char	MinGrant;
	unsigned char	MaxLatency;
};

unsigned int Read_PCI_Config(unsigned int bus,unsigned int device,unsigned int function,unsigned int offset);

void Write_PCI_Config(unsigned int bus,unsigned int device,unsigned int function,unsigned int offset,unsigned int value);

int analysis_PCI_Config(struct PCI_Header_00 * PCI_HDR,unsigned int bus,unsigned int device,unsigned int function);

#endif


