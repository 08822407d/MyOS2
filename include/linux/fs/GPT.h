/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
#ifndef _GPT_H_
#define _GPT_H_

#include <linux/kernel/types.h>

#include "MBR.h"

#define EFIBOOT_PART_GUID_LOW	0x11D2F81FC12A7328
#define EFIBOOT_PART_GUID_HIGH	0x3BC93EC9A0004BBA

#define EXT4_PART_GUID_LOW		0x477284830FC63DAF
#define EXT4_PART_GUID_HIGH		0xE47D47D8693D798E

typedef struct GPT_Header
{
	u64	Signature;
	u32	Revision;
	u32	HeaderSize;
	u32	HeaderCRC32;
	u32	Reserved;
	u64	MyLBA;
	u64	AlternateLBA;

	u64	FirstUsableLBA;
	u64	LastUsableLBA;

	u64	DiskGUID[2];

	u64	PartitionEntryLBA;
	u32	NumberOfPartitionEntries;
	u32	SizeOfPartitionEntry;
	u32	PartitionEntryArrayCRC32;
}__attribute__((packed)) GPT_H_s;

typedef struct GPT_Partition_Entry
{
	u64	PartitionTypeGUID[2];
	u64	UniquePartitionGUID[2];

	u64	StartingLBA;
	u64	EndingLBA;
	u64	Attributes;
	u16	PartitionName[36];
} GPT_PE_s;

#endif /* _GPT_H_ */