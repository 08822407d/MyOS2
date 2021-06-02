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

#include <sys/types.h>

#include "MBR.h"

#define EFI_PART_GUID_LOW	0x11D2F81FC12A7328
#define EFI_PART_GUID_HIGH	0x3BC93EC9A0004BBA

#define EXT4_PART_GUID_LOW	0x477284830FC63DAF
#define EXT4_PART_GUID_HIGH	0xE47D47D8693D798E

typedef struct GPT_Header
{
	uint64_t	Signature;
	uint32_t	Revision;
	uint32_t	HeaderSize;
	uint32_t	HeaderCRC32;
	uint32_t	Reserved;
	uint64_t	MyLBA;
	uint64_t	AlternateLBA;

	uint64_t	FirstUsableLBA;
	uint64_t	LastUsableLBA;

	uint64_t	DiskGUID[2];

	uint64_t	PartitionEntryLBA;
	uint32_t	NumberOfPartitionEntries;
	uint32_t	SizeOfPartitionEntry;
	uint32_t	PartitionEntryArrayCRC32;
}__attribute__((packed)) GPT_H_s;

typedef struct GPT_Partition_Entry
{
	uint64_t	PartitionTypeGUID[2];
	uint64_t	UniquePartitionGUID[2];

	uint64_t	StartingLBA;
	uint64_t	EndingLBA;
	uint64_t	Attributes;
	uint16_t	PartitionName[36];
}__attribute__((packed)) GPT_PE_s;

#endif /* _GPT_H_ */