#include <string.h>

#include <include/printk.h>
#include <include/proto.h>
#include <include/glo.h>
#include <linux/fs/fs.h>
#include <linux/fs/MBR.h>
#include <linux/fs/GPT.h>
#include <linux/fs/fat32.h>
#include <linux/fs/ext2.h>
#include <linux/fs/sysfs.h>
#include <linux/fs/internal.h>

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"


#define BOOT_FS_IDX 1

super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);

MBR_s		*boot_sec;
GPT_H_s		*gpt_hdr;
GPT_PE_s	*gpt_pes;

super_block_s *root_sb = NULL;
fs_type_s filesystem = { .name = "filesystem", .fs_flags = 0};
extern fs_type_s *file_systems;


#include <linux/fs/fat.h>
unsigned long init_vfs()
{
	mnt_init();
	int test = kparam.init_flags.vfs;
	kparam.init_flags.vfs = 0;
	// load the boot sector
	boot_sec = (MBR_s *)kmalloc(sizeof(MBR_s));
	memset(boot_sec, 0, sizeof(MBR_s));
	IDE_device_operation.transfer(ATA_READ_CMD, 0, 1, (unsigned char *)boot_sec);

	// check partition type, only support GPT
	if (boot_sec->DPTE[0].type != 0xee &&
		boot_sec->DPTE[0].type != 0xef)
	{
		color_printk(RED, BLACK, "Read MBR failed!\n");
		while (1);
	}
	// load the gpt_hdr
	gpt_hdr = (GPT_H_s *)boot_sec;
	memset(gpt_hdr, 0, sizeof(MBR_s));
	IDE_device_operation.transfer(ATA_READ_CMD, 1, 1, (unsigned char *)gpt_hdr);
	// load all the gpt_entries
	unsigned gptent_nr = gpt_hdr->NumberOfPartitionEntries;
	gpt_pes = (GPT_PE_s *)kmalloc(gptent_nr * sizeof(GPT_PE_s));
	memset(gpt_pes, 0, gptent_nr * sizeof(GPT_PE_s));
	IDE_device_operation.transfer(ATA_READ_CMD, gpt_hdr->PartitionEntryLBA,
									gptent_nr / 4, (unsigned char *)gpt_pes);
	// load all the gpt_entries
	init_ext2_fs();
	init_fat32_fs();
	init_vfat_fs();

	GPT_PE_s *gpt_pe = NULL;
	for (int i = 0; gpt_pes[i].PartitionTypeGUID[0] != 0; i++)
	{
		gpt_pe = &gpt_pes[i];
		uint64_t * puid_p = gpt_pe->PartitionTypeGUID;
		switch (*puid_p)
		{
		case EFIBOOT_PART_GUID_LOW:
			if (*(puid_p + 1) == EFIBOOT_PART_GUID_HIGH)
			{
				// mount partitions
				FAT32_BS_s * fat32_sb = (FAT32_BS_s *)kmalloc(sizeof(FAT32_BS_s));
				IDE_device_operation.transfer(ATA_READ_CMD, gpt_pes[i].StartingLBA,
												1, (unsigned char *)fat32_sb);
				if (i = BOOT_FS_IDX)
					root_sb = mount_fs("FAT32", gpt_pe, fat32_sb);
			}
			break;

		case EXT4_PART_GUID_LOW:
			if (*(puid_p + 1) == EXT4_PART_GUID_HIGH)
			{

			}
			break;

		default:
			break;
		}
	}
	kparam.init_flags.vfs = 1;
}

super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf)
{
	fs_type_s * p = NULL;
	for(p = file_systems; p; p = p->next)
		if(!strcmp(p->name, name))
		{
			return p->read_super(DPTE, buf);
		}
	return 0;
}