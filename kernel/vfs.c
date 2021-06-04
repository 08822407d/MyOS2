#include <lib/string.h>

#include "include/printk.h"
#include "include/proto.h"

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
GPT_PE_s *	gpt_boot_pe;

FAT32_BS_s *fat32_bs;

unsigned long init_vfs(unsigned long param)
{
	boot_sec = (MBR_s *)kmalloc(sizeof(MBR_s));
	gpt_hdr = (GPT_H_s *)kmalloc(sizeof(MBR_s));
	memset(boot_sec, 0, sizeof(MBR_s));
	memset(gpt_hdr, 0, sizeof(MBR_s));
	// load the boot sector
	IDE_device_operation.transfer(ATA_READ_CMD, 0, 1, (unsigned char *)boot_sec);
	
	// check partition type, only support GPT
	if (boot_sec->DPTE[0].type != 0xee &&
		boot_sec->DPTE[0].type != 0xef)
	{
		while (1);
	}
	// load the gpt_hdr
	IDE_device_operation.transfer(ATA_READ_CMD, 1, 1, (unsigned char *)gpt_hdr);
	unsigned ent_fisrt_idx = gpt_hdr->PartitionEntryLBA;
	unsigned gptent_nr = gpt_hdr->NumberOfPartitionEntries;
	gpt_pes = (GPT_PE_s *)kmalloc(gptent_nr * sizeof(GPT_PE_s));
	memset(gpt_pes, 0, gptent_nr * sizeof(GPT_PE_s));
	// load all the gpt_entries
	IDE_device_operation.transfer(ATA_READ_CMD, ent_fisrt_idx, gptent_nr / 4, (unsigned char *)gpt_pes);
	gpt_boot_pe = &gpt_pes[BOOT_FS_IDX];
	// load all the gpt_entries
	init_FAT32_FS();
	init_EXT2_FS();
}

// dirent_s * path_walk(char * name,unsigned long flags)
// {
// 	char * tmpname = NULL;
// 	int tmpnamelen = 0;
// 	dirent_s * parent = root_sb->root;
// 	dirent_s * path = NULL;

// 	while(*name == '/')
// 		name++;

// 	if(!*name)
// 	{
// 		return parent;
// 	}

// 	for(;;)
// 	{
// 		tmpname = name;
// 		while(*name && (*name != '/'))
// 			name++;
// 		tmpnamelen = name - tmpname;

// 		path = (dirent_s *)kmalloc(sizeof(dirent_s));
// 		memset(path, 0, sizeof(dirent_s));

// 		path->name = kmalloc(tmpnamelen+1);
// 		memset(path->name,0,tmpnamelen+1);
// 		memcpy(tmpname,path->name,tmpnamelen);
// 		path->name_length = tmpnamelen;

// 		if(parent->dir_inode->inode_ops->lookup(parent->dir_inode,path) == NULL)
// 		{
// 			color_printk(RED, WHITE, "can not find file or dir:%s\n", path->name);
// 			kfree(path->name);
// 			kfree(path);
// 			return NULL;
// 		}

// 		list_init(&path->child_node);
// 		list_init(&path->subdirs_list);
// 		path->parent = parent;
// 		list_add_to_behind(&parent->subdirs_list,&path->child_node);

// 		if(!*name)
// 			goto last_component;
// 		while(*name == '/')
// 			name++;
// 		if(!*name)
// 			goto last_slash;

// 		parent = path;
// 	}

// last_slash:
// last_component:

// 	if(flags & 1)
// 	{
// 		return parent;
// 	}

// 	return path;
// }

// int fill_dentry(void *buf,char *name, long namelen,long type,long offset)
// {
// 	// struct dirent* dent = (struct dirent*)buf;
	
// 	// if((unsigned long)buf < TASK_SIZE && !verify_area(buf,sizeof(struct dirent) + namelen))
// 	// 	return -EFAULT;
	
// 	// memcpy(name,dent->d_name,namelen);
// 	// dent->d_namelen = namelen;
// 	// dent->d_type = type;
// 	// dent->d_offset = offset;
// 	// return sizeof(struct dirent) + namelen;
// }

superblock_s * root_sb = NULL;
fs_type_s filesystem = {"filesystem", 0};

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