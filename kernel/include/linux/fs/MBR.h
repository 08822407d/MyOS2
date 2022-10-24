/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/
#ifndef _MRB_H_
#define _MRB_H_

#include <linux/kernel/types.h>

typedef struct Disk_Partition_Table_Entry
{
	u8	flags;
	u8	start_head;
	u16	start_sector	:6,		//0~5
		start_cylinder	:10;	//6~15
	u8	type;
	u8	end_head;
	u16	end_sector		:6,		//0~5
		end_cylinder	:10;	//6~15
	u32	start_LBA;
	u32	sectors_limit;
} __attribute__((packed)) legacy_DPTE_s;

typedef struct Disk_Partition_Table
{
	u8	BS_Reserved[446];
	legacy_DPTE_s	DPTE[4];
	u16	BS_TrailSig;
} __attribute__((packed)) MBR_s;

#endif /* _MRB_H_ */