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

#include <sys/types.h>

typedef struct Disk_Partition_Table_Entry
{
	uint8_t		flags;
	uint8_t		start_head;
	uint16_t	start_sector	:6,		//0~5
				start_cylinder	:10;	//6~15
	uint8_t		type;
	uint8_t		end_head;
	uint16_t	end_sector		:6,		//0~5
				end_cylinder	:10;	//6~15
	uint32_t	start_LBA;
	uint32_t	sectors_limit;
} __attribute__((packed)) legacy_DPTE_s;

typedef struct Disk_Partition_Table
{
	uint8_t			BS_Reserved[446];
	legacy_DPTE_s	DPTE[4];
	uint16_t		BS_TrailSig;
} __attribute__((packed)) MBR_s;

#endif /* _MRB_H_ */