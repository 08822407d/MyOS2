#include <linux/fs/fs.h>
#include <linux/fs/MBR.h>
#include <linux/fs/GPT.h>
#include <linux/fs/fat32.h>
#include <linux/fs/ext2.h>
#include <linux/fs/sysfs.h>
#include <linux/fs/namei.h>
#include <linux/fs/internal.h>
#include <linux/lib/string.h>


#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include <obsolete/glo.h>
#include "../../arch/amd64/include/device.h"
#include "../../arch/amd64/include/ide.h"


#include <linux/kernel/init_syscalls.h>
#include <linux/fs/mount.h>
#include <uapi/mount.h>

mount_s root_mnt;

void set_init_mount()
{
	list_hdr_init(&root_mnt.mnt_mounts);
	list_init(&root_mnt.mnt_child, &root_mnt);

	root_mnt.mnt.mnt_sb = root_sb;
	root_mnt.mnt_parent = &root_mnt;
	root_mnt.mnt_mountpoint =
	root_mnt.mnt.mnt_root = root_sb->s_root;
	root_mnt.mnt_mp = NULL;
}

void set_init_taskfs()
{
	task_s * curr = curr_tsk;
	// set cwd and root-dir of task1
	taskfs_s * taskfs_p = curr->fs;
	taskfs_p->pwd.dentry = 
	taskfs_p->root.dentry = root_sb->s_root;
	taskfs_p->pwd.mnt = 
	taskfs_p->root.mnt = &root_mnt.mnt;

	memcpy(task0_PCB.task.fs, taskfs_p, sizeof(taskfs_s));
}




#define BOOT_FS_IDX 0

super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);

MBR_s		*boot_sec;
GPT_H_s		*gpt_hdr;
GPT_PE_s	*gpt_pes;

super_block_s *root_sb = NULL;
fs_type_s filesystem = { .name = "filesystem", .fs_flags = 0};
extern fs_type_s *file_systems;

#include <linux/fs/fat.h>

void register_diskfs(void)
{
	init_ext2_fs();
	init_fat32_fs();
	// init_vfat_fs();
}

unsigned long switch_to_root_disk()
{
	// int test = kparam.init_flags.vfs;
	// kparam.init_flags.vfs = 0;
	// load the boot sector
	boot_sec = (MBR_s *)kmalloc(sizeof(MBR_s));
	memset(boot_sec, 0, sizeof(MBR_s));
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
					0, 1, (unsigned char *)boot_sec);

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
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
					1, 1, (unsigned char *)gpt_hdr);
	// load all the gpt_entries
	u32 gptent_nr = gpt_hdr->NumberOfPartitionEntries;
	gpt_pes = (GPT_PE_s *)kmalloc(gptent_nr * sizeof(GPT_PE_s));
	memset(gpt_pes, 0, gptent_nr * sizeof(GPT_PE_s));
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, gpt_hdr->PartitionEntryLBA,
									gptent_nr / 4, (unsigned char *)gpt_pes);
	// load all the gpt_entries

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
				ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
								gpt_pes[i].StartingLBA, 1, (unsigned char *)fat32_sb);
				if (i == BOOT_FS_IDX)
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

	set_init_mount();
	set_init_taskfs();

	int err = init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT);
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