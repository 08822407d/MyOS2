#include <linux/kernel/mm_api.h>
#include <linux/fs/fs.h>
#include <linux/fs/MBR.h>
#include <linux/fs/GPT.h>
#include <linux/fs/fat32.h>
#include <linux/fs/namei.h>
#include <linux/fs/internal.h>
#include <linux/lib/string.h>
#include <linux/init/init.h>
#include <linux/drivers/myos_ide.h>


#include <obsolete/printk.h>
#include <obsolete/glo.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>

#include <linux/kernel/init_syscalls.h>
#include <linux/fs/mount.h>
#include <uapi/linux/mount.h>

#define BOOT_FS_IDX 0

extern blkdev_ops_s NVMe_ops;
super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);

MBR_s		*boot_sec;
GPT_H_s		*gpt_hdr;
GPT_PE_s	*gpt_pes;

super_block_s *myos_root_sb = NULL;

mount_s myos_root_mnt =
{
	.mnt_parent		= &myos_root_mnt,
	.mnt_mp			= NULL,
	.mnt_mounts		= LIST_HEADER_INIT(myos_root_mnt.mnt_mounts),
	.mnt_child		= LIST_INIT(myos_root_mnt.mnt_child),
};

void set_init_mount()
{
	myos_root_mnt.mnt.mnt_sb = myos_root_sb;
	myos_root_mnt.mnt_mountpoint =
	myos_root_mnt.mnt.mnt_root = myos_root_sb->s_root;
}

void set_init_taskfs()
{
	task_s * curr = current;
	// set cwd and root-dir of task1
	taskfs_s *taskfs_p = curr->fs;
	taskfs_p->pwd.dentry = 
	taskfs_p->root.dentry = myos_root_sb->s_root;
	taskfs_p->pwd.mnt = 
	taskfs_p->root.mnt = &myos_root_mnt.mnt;

	*curr->fs = *taskfs_p;
}


fs_type_s filesystem = { .name = "filesystem", .fs_flags = 0};
extern fs_type_s *file_systems;

#include <linux/fs/fat.h>

void register_diskfs(void)
{
	init_fat32_fs();
	// init_vfat_fs();
}


ulong myos_switch_to_root_disk()
{
	// int test = init_flags.vfs;
	// init_flags.vfs = 0;
	// load the boot sector
	boot_sec = (MBR_s *)kzalloc(SZ_4K, GFP_KERNEL);
	memset(boot_sec, 0, SZ_4K);
	ROOTBLK_TRANSFER(CMD_READ, 0, SECT_PER_PG, (unchar *)boot_sec);
	// check partition type, only support GPT
	if (boot_sec->DPTE[0].type != 0xee &&
		boot_sec->DPTE[0].type != 0xef)
	{
		color_printk(RED, BLACK, "Read MBR failed!\n");
		while (1);
	}

	// load the gpt_hdr
	gpt_hdr = (GPT_H_s *)(boot_sec + 1);
	// load all the gpt_entries
	u32 gptent_nr = gpt_hdr->NumberOfPartitionEntries;
	gpt_pes = (GPT_PE_s *)kzalloc(gptent_nr * sizeof(GPT_PE_s), GFP_KERNEL);
	void *gpt_pes_buf = kzalloc(5 * SZ_4K, GFP_KERNEL);
	// read first 40 sectors(512B) of the disk
	ROOTBLK_TRANSFER(CMD_READ, 0, 5 * SECT_PER_PG, gpt_pes_buf);
	memcpy(gpt_pes, gpt_pes_buf + 2 * SZ_512, gptent_nr * sizeof(GPT_PE_s));
	kfree(gpt_pes_buf);

	GPT_PE_s *gpt_pe = NULL;
	for (int i = 0; gpt_pes[i].PartitionTypeGUID[0] != 0; i++)
	{
		gpt_pe = &gpt_pes[i];
		uint64_t *puid_p = gpt_pe->PartitionTypeGUID;
		switch (*puid_p)
		{
		case EFIBOOT_PART_GUID_LOW:
			if (*(puid_p + 1) == EFIBOOT_PART_GUID_HIGH)
			{
				// mount partitions
				FAT32_BS_s *fat32_sb = (FAT32_BS_s *)kmalloc(SZ_4K, GFP_KERNEL);
				ROOTBLK_TRANSFER(CMD_READ, gpt_pes[i].StartingLBA, SECT_PER_PG, (unchar *)fat32_sb);
				if (i == BOOT_FS_IDX)
					myos_root_sb = mount_fs("FAT32", gpt_pe, fat32_sb);
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

	set_init_mount();
	set_init_taskfs();

	int err = init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT);

	// unchar *buf = kzalloc(PAGE_SIZE, GFP_KERNEL);
	// NVMe_ops.transfer(0, 0, 0x02, 0, 1, buf);
	// NVMe_ops.transfer(0, 0, 0x02, 0, 2, buf);
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