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

	u64				BARs_addr[6];
	u64				BARs_limit_max[6];
	u64				BARs_limit_min[6];
	u64				BARs_raw[6];
/////////////////32-bit BAR * 6
	unsigned int	Base32Address0;
	unsigned int	Base32Limit0;
	unsigned int	Base32RAWData0;

	unsigned int	Base32Address1;
	unsigned int	Base32Limit1;
	unsigned int	Base32RAWData1;

	unsigned int	Base32Address2;
	unsigned int	Base32Limit2;
	unsigned int	Base32RAWData2;

	unsigned int	Base32Address3;
	unsigned int	Base32Limit3;
	unsigned int	Base32RAWData3;

	unsigned int	Base32Address4;
	unsigned int	Base32Limit4;
	unsigned int	Base32RAWData4;

	unsigned int	Base32Address5;
	unsigned int	Base32Limit5;
	unsigned int	Base32RAWData5;
/////////////////64-bit BAR * 3
	unsigned int	Base64Address0;
	unsigned int	Base64Limit0;
	unsigned int	Base64RAWData0;

	unsigned int	Base64Address1;
	unsigned int	Base64Limit1;
	unsigned int	Base64RAWData1;

	unsigned int	Base64Address2;
	unsigned int	Base64Limit2;
	unsigned int	Base64RAWData2;

	unsigned int	CardBusCISPointer;

	unsigned short	SubSystemVendorID;
	unsigned short	SubSystemID;

	unsigned int	ExpansionROMBaseAddress;

	unsigned int	CapabilitiesPointer:8,
			Reserved0:24;

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


