#include <lib/string.h>

#include "include/printk.h"
#include "include/proto.h"

#include "include/glo.h"
#include "include/fat32.h"
#include "include/ext2.h"
#include "include/vfs.h"
#include "include/MBR.h"
#include "include/GPT.h"

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"

MBR_s *		boot_sec;
GPT_H_s *	gpt_hdr;
GPT_PE_s *	gpt_pes;

superblock_s * root_sb = NULL;
fs_type_s filesystem = {"filesystem", 0};


unsigned long init_vfs(unsigned long param)
{
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
	init_FAT32_FS();
	init_EXT2_FS();

	GPT_PE_s *	gpt_pe = NULL;
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

dirent_s * path_walk(char * name,unsigned long flags)
{
	char * tmpname = NULL;
	int tmpnamelen = 0;
	dirent_s * parent = root_sb->root;
	dirent_s * path = NULL;

	while(*name == '/')
		name++;

	if(!*name)
	{
		return parent;
	}

	for(;;)
	{
		tmpname = name;
		while(*name && (*name != '/'))
			name++;
		tmpnamelen = name - tmpname;

		path = (dirent_s *)kmalloc(sizeof(dirent_s));
		memset(path, 0, sizeof(dirent_s));

		path->name = kmalloc(tmpnamelen+1);
		memset(path->name,0,tmpnamelen+1);
		memcpy(path->name, tmpname, tmpnamelen);
		path->name_length = tmpnamelen;

		if(parent->dir_inode->inode_ops->lookup(parent->dir_inode, path) == NULL)
		{
			color_printk(RED, WHITE, "can not find file or dir:%s\n", path->name);
			kfree(path->name);
			kfree(path);
			return NULL;
		}

		m_init_list_header(&path->child_list);
		path->parent = parent;
		m_append_to_list(path, &parent->child_list);

		if(!*name)
			goto last_component;
		while(*name == '/')
			name++;
		if(!*name)
			goto last_slash;

		parent = path;
	}

last_slash:
last_component:

	if(flags & 1)
	{
		return parent;
	}

	return path;
}

int fill_dentry(void *buf, char *name, long namelen, long type, long offset)
{
	// struct dirent* dent = (struct dirent*)buf;
	
	// if((unsigned long)buf < TASK_SIZE && !verify_area(buf,sizeof(struct dirent) + namelen))
	// 	return -EFAULT;
	
	// memcpy(name,dent->d_name,namelen);
	// dent->d_namelen = namelen;
	// dent->d_type = type;
	// dent->d_offset = offset;
	// return sizeof(struct dirent) + namelen;
}

superblock_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf)
{
	fs_type_s * p = NULL;
	for(p = &filesystem; p; p = p->next)
		if(!strcmp(p->name, name))
		{
			return p->read_superblock(DPTE, buf);
		}
	return 0;
}

unsigned long register_filesystem(fs_type_s * fs)
{
	fs_type_s * p = NULL;
	for(p = &filesystem; p; p = p->next)
		if(!strcmp(fs->name, p->name))
			return 0;

	fs->next = filesystem.next;
	filesystem.next = fs;
	return 1;
}

unsigned long unregister_filesystem(fs_type_s * fs)
{
	fs_type_s * p = &filesystem;
	while(p->next)
		if(p->next == fs)
		{
			p->next = p->next->next;
			fs->next = NULL;
			return 1;
		}
		else
			p = p->next;
	return 0;
}