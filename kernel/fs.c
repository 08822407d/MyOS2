#include <lib/string.h>

#include "include/proto.h"
#include "include/MBR.h"
#include "include/GPT.h"

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"

MBR_s *		boot_sec;
GPT_H_s *	gpt_header;
GPT_PE_s *	gpt_partent;

unsigned long load_fs(unsigned long param)
{
	boot_sec = (MBR_s *)kmalloc(sizeof(MBR_s));
	gpt_header = (GPT_H_s *)kmalloc(sizeof(MBR_s));
	memset(boot_sec, 0, sizeof(MBR_s));
	memset(gpt_header, 0, sizeof(MBR_s));
	// load the boot sector
	IDE_device_operation.transfer(ATA_READ_CMD, 0, 1, (unsigned char *)boot_sec);
	
	// check partition type, only support GPT
	if (boot_sec->DPTE[0].type != 0xee &&
		boot_sec->DPTE[0].type != 0xef)
	{
		while (1);
	}
	// load the gpt_header
	IDE_device_operation.transfer(ATA_READ_CMD, 1, 1, (unsigned char *)gpt_header);
	unsigned ent_fisrt_idx = gpt_header->PartitionEntryLBA;
	unsigned gptent_nr = gpt_header->NumberOfPartitionEntries;
	gpt_partent = (GPT_PE_s *)kmalloc(gptent_nr * sizeof(GPT_PE_s));
	memset(gpt_partent, 0, gptent_nr * sizeof(GPT_PE_s));
	// load all the gpt_entries
	IDE_device_operation.transfer(ATA_READ_CMD, ent_fisrt_idx, gptent_nr / 4, (unsigned char *)gpt_partent);
}