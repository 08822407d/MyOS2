/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#include <linux/kernel/slab.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/linux/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>

bool FAT32_ent_empty(const msdos_dirent_s *de)
{
	return (de->name[0] == 0xe5 ||
			de->name[0] == 0x00 ||
			de->name[0] == 0x05);
}

u32 FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry)
{
	uint32_t buf[128];
	memset(buf, 0, 512);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
			fsbi->FAT1_firstsector + (fat_entry >> 7), 1, (unsigned char *)buf);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
}

const msdos_dirent_s *FAT32_get_full_ent(FAT32_iobuf_s *iobuf)
{
	size_t len = 0;
	const msdos_dirent_s *ret_val;
	const msdos_dirslot_s *tmpval;
	do
	{
		ret_val = iobuf->next(iobuf);
		tmpval = (msdos_dirslot_s *)ret_val;
	}
	while ((ret_val->attr == ATTR_LONG_NAME ||
			FAT32_ent_empty(ret_val)) &&
			!iobuf->iter_reach_end);
	
	if (iobuf->iter_reach_end)
		ret_val = ERR_PTR(-ENOENT);

	return ret_val;
}

u32 FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi)
{
	int i, j;
	int fat_entry;
	unsigned long sector_per_fat = fsbi->sector_per_FAT;
	unsigned int buf[128];

//	fsbi->fat_fsinfo->FSI_Free_Count & fsbi->fat_fsinfo->FSI_Nxt_Free not exactly,so unuse

	for(i = 0; i < sector_per_fat; i++)
	{
		memset(buf, 0, 512);
		ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
				fsbi->FAT1_firstsector + i, 1, (unsigned char *)buf);

		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
				return (i << 7) + j;
		}
	}
	return 0;
}

uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry, u32 value)
{
	uint32_t buf[128];
	int i;

	memset(buf,0,512);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
			fsbi->FAT1_firstsector + (fat_entry >> 7), 1, (unsigned char *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);

	for(i = 0; i < fsbi->NumFATs; i++)
		ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
				fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
				1, (unsigned char *)buf);
	return 1;	
}

int FAT32_open(inode_s *inode, file_s *file_p)
{
	file_p->f_pos = 0;
	return 1;
}

int FAT32_close(inode_s *inode, file_s *file_p)
{
	file_p->f_pos = 0;
	return 1;
}

ssize_t FAT32_read(file_s *filp, char *buf, size_t count, loff_t *position)
{
	FAT32_inode_info_s * finode = filp->f_path.dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->f_path.dentry->d_inode->i_sb->private_sb_info;

	unsigned long cluster = finode->first_cluster;
	unsigned long sector = 0;
	int i, length = 0;
	long ret_val = 0;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);

	if(!cluster)
		return -EFAULT;
	for(i = 0; i < index; i++)
		cluster = FAT32_read_FAT_Entry(fsbi, cluster);

	if(*position + count > filp->f_path.dentry->d_inode->i_size)
		index = count = filp->f_path.dentry->d_inode->i_size - *position;
	else
		index = count;

	do
	{
		memset(buffer, 0, fsbi->bytes_per_cluster);
		sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
		if(ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buffer))
		{
			color_printk(RED, BLACK, "FAT32 FS(read) read disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
		}

		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < TASK_SIZE_MAX)
			copy_to_user(buf, buffer + offset, length);
		else
			memcpy(buf, buffer + offset, length);

		index -= length;
		buf += length;
		offset -= offset;
		*position += length;
	} while(index && (cluster = FAT32_read_FAT_Entry(fsbi, cluster)));

	kfree(buffer);
	if(!index)
		ret_val = count;
	return ret_val;
}

ssize_t FAT32_write(file_s *filp, const char *buf, size_t count, loff_t *position)
{
	FAT32_inode_info_s * finode = filp->f_path.dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->f_path.dentry->d_inode->i_sb->private_sb_info;

	u32 cluster = finode->first_cluster;
	u32 next_cluster = 0;
	unsigned long sector = 0;
	int i,length = 0;
	long ret_val = 0;
	long flags = 0;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);

	if(!cluster)
	{
		cluster = FAT32_find_available_cluster(fsbi);
		flags = 1;
	}
	else
		for(i = 0; i < index; i++)
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);

	if(!cluster)
	{
		kfree(buffer);
		return -ENOSPC;
	}

	if(flags)
	{
		finode->first_cluster = cluster;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
	}

	index = count;

	do
	{
		if(!flags)
		{
			memset(buffer, 0, fsbi->bytes_per_cluster);
			sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
			if(ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
						fsbi->sector_per_cluster, (unsigned char *)buffer))
			{
				color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
				ret_val = -EIO;
				break;
			}
		}

		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < TASK_SIZE_MAX)
			copy_from_user(buffer + offset, (void *)buf, length);
		else
			memcpy(buffer + offset, buf, length);

		if(ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buffer))
		{
			color_printk(RED, BLACK, "FAT32 FS(write) write disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
		}

		index -= length;
		buf += length;
		offset -= offset;
		*position += length;

		if(index)
			next_cluster = FAT32_read_FAT_Entry(fsbi, cluster);
		else
			break;

		if(next_cluster >= 0x0ffffff8)
		{
			next_cluster = FAT32_find_available_cluster(fsbi);
			if(!next_cluster)
			{
				kfree(buffer);
				return -ENOSPC;
			}			
				
			FAT32_write_FAT_Entry(fsbi, cluster, next_cluster);
			FAT32_write_FAT_Entry(fsbi, next_cluster, 0x0ffffff8);
			cluster = next_cluster;
			flags = 1;
		}

	}while(index);

	if(*position > filp->f_path.dentry->d_inode->i_size)
	{
		filp->f_path.dentry->d_inode->i_size = *position;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
	}

	kfree(buffer);
	if(!index)
		ret_val = count;
	return ret_val;
}

loff_t FAT32_lseek(file_s *filp, loff_t offset, int origin)
{
	inode_s *inode = filp->f_path.dentry->d_inode;
	long pos = 0;

	switch(origin)
	{
		case SEEK_SET:
				pos = offset;
			break;

		case SEEK_CUR:
				pos =  filp->f_pos + offset;
			break;

		case SEEK_END:
				pos = filp->f_path.dentry->d_inode->i_size + offset;
			break;

		default:
			return -EINVAL;
			break;
	}

	if(pos < 0 || pos > filp->f_path.dentry->d_inode->i_size)
		return -EOVERFLOW;

	filp->f_pos = pos;
	// color_printk(GREEN, BLACK, "FAT32 FS(lseek) alert position:%d\n", filp->position);

	return pos;
}

int FAT32_ioctl(inode_s * inode, file_s * filp, unsigned long cmd, unsigned long arg)
{}

int FAT32_getdents64(file_s *filp, dir_ctxt_s *ctx)
{
	if (ctx->pos == -1)
		return 0;

	int error = 0;
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	inode_s *inode = filp->f_path.dentry->d_inode;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(inode);
	iobuf->iter_init(iobuf);
	// iterate all fat32 entries in this cluster
	while (!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * sizeof(msdos_dirent_s));

		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_parse_long(&namelen, iobuf);
		else
			name = FAT32_parse_short(&namelen, tmpde);

		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
	}

	if (error == 0)
		ctx->pos = -1;
	
	FAT32_iobuf_release(iobuf);
	return error;
}

file_ops_s FAT32_file_ops = 
{
	.open			= FAT32_open,
	.close			= FAT32_close,
	.read			= FAT32_read,
	.write			= FAT32_write,
	.llseek			= FAT32_lseek,
	.ioctl			= FAT32_ioctl,

	.mmap			= generic_file_mmap,
	.iterate_shared	= FAT32_getdents64,
};

// these operation need cache and list
long FAT32_compare(dentry_s * parent_dentry, char * source_filename, char * destination_filename){}
long FAT32_hash(dentry_s * dentry,char * filename){}
long FAT32_release(dentry_s * dentry){}
long FAT32_iput(dentry_s * dentry, inode_s * inode){}

void fat32_write_superblock(super_block_s * sbp){}

void fat32_put_superblock(super_block_s * sbp)
{
	kfree(sbp->private_sb_info);
	kfree(sbp->s_root->d_inode->private_idx_info);
	kfree(sbp->s_root->d_inode);
	kfree(sbp->s_root);
	kfree(sbp);
}

int fat32_write_inode(inode_s * inode)
{
	msdos_dirent_s * fdentry = NULL;
	msdos_dirent_s * buf = NULL;
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	unsigned long sector = 0;

	if(finode->dentry_location == 0)
	{
		color_printk(RED,BLACK,"FS ERROR:write root inode!\n");	
		return -1;
	}

	sector = fsbi->Data_firstsector + (finode->dentry_location - 2) * fsbi->sector_per_cluster;
	buf = kzalloc(fsbi->bytes_per_cluster, GFP_KERNEL);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
			sector, fsbi->sector_per_cluster, (unsigned char *)buf);
	fdentry = buf+finode->dentry_position;

	////alert fat32 dentry data
	fdentry->size = inode->i_size;
	fdentry->start = finode->first_cluster & 0xffff;
	fdentry->starthi = (fdentry->starthi & 0xf000) | (finode->first_cluster >> 16);

	ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
			sector, fsbi->sector_per_cluster, (unsigned char *)buf);
	kfree(buf);
}

super_ops_s FAT32_sb_ops = 
{
	.write_super = fat32_write_superblock,
	.put_super = fat32_put_superblock,
	.write_inode = fat32_write_inode,
};

extern fs_type_s FAT32_fs_type;
super_block_s * read_fat32_superblock(GPT_PE_s * DPTE, void * buf)
{
	super_block_s * sbp = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_BS_s * fbs = NULL;
	FAT32_SBinfo_s * fsbi = NULL;

	//super block
	sbp = alloc_super(&FAT32_fs_type, 0);
	if (sbp == NULL)
		return ERR_PTR(-ENOMEM);

	sbp->s_op = &FAT32_sb_ops;
	sbp->private_sb_info = kzalloc(sizeof(FAT32_SBinfo_s), GFP_KERNEL);

	//fat32 boot sector
	fbs = (FAT32_BS_s *)buf;
	fsbi = sbp->private_sb_info;	
	fsbi->start_sector = DPTE->StartingLBA;
	fsbi->sector_count = DPTE->EndingLBA - DPTE->StartingLBA;
	fsbi->sector_per_cluster = fbs->BPB_SecPerClus;
	fsbi->bytes_per_cluster = fbs->BPB_SecPerClus * fbs->BPB_BytesPerSec;
	fsbi->bytes_per_sector = fbs->BPB_BytesPerSec;
	fsbi->Data_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt +
								fbs->BPB_FATSz32 * fbs->BPB_NumFATs;
	fsbi->FAT1_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt;
	fsbi->sector_per_FAT = fbs->BPB_FATSz32;
	fsbi->NumFATs = fbs->BPB_NumFATs;
	fsbi->fsinfo_sector_infat = fbs->BPB_FSInfo;
	fsbi->bootsector_bk_infat = fbs->BPB_BkBootSec;	
	
	//fat32 fsinfo sector
	fsbi->fat_fsinfo = kzalloc(sizeof(FAT32_FSinfo_s), GFP_KERNEL);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
			DPTE->StartingLBA + fbs->BPB_FSInfo, 1, (unsigned char *)fsbi->fat_fsinfo);
	
	//directory entry
	sbp->s_root = kzalloc(sizeof(dentry_s), GFP_KERNEL);

	list_init(&sbp->s_root->d_child, sbp->s_root);
	list_hdr_init(&sbp->s_root->d_subdirs);
	sbp->s_root->d_sb = sbp;
	sbp->s_root->d_parent = sbp->s_root;
	sbp->s_root->d_op = &vfat_dentry_ops;
	sbp->s_root->d_name.name = (char *)kmalloc(2, GFP_KERNEL);
	((char *)sbp->s_root->d_name.name)[0] = '/';
	((char *)sbp->s_root->d_name.name)[1] = 0;
	sbp->s_root->d_name.len = 1;

	//index node
	sbp->s_root->d_inode = (inode_s*)kmalloc(sizeof(inode_s), GFP_KERNEL);
	sbp->s_root->d_inode->i_op = &vfat_dir_inode_operations;
	sbp->s_root->d_inode->i_fop = &FAT32_file_ops;
	sbp->s_root->d_inode->i_size = 0;
	sbp->s_root->d_inode->i_blocks =
					(sbp->s_root->d_inode->i_size + fsbi->bytes_per_cluster - 1) /
							fsbi->bytes_per_sector;
	sbp->s_root->d_inode->i_mode = S_IFDIR;
	sbp->s_root->d_inode->i_sb = sbp;

	//fat32 root inode
	sbp->s_root->d_inode->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s), GFP_KERNEL);
	finode = (FAT32_inode_info_s *)sbp->s_root->d_inode->private_idx_info;
	finode->first_cluster = fbs->BPB_RootClus;
	finode->dentry_location = 0;
	finode->dentry_position = 0; 
	finode->create_date = 0;
	finode->create_time = 0;
	finode->write_date = 0;
	finode->write_time = 0;

	return sbp;
}

fs_type_s FAT32_fs_type =
{
	.name = "FAT32",
	.fs_flags = 0,
	.read_super = read_fat32_superblock,
	.next = NULL,
};

void init_fat32_fs()
{
	register_filesystem(&FAT32_fs_type);
} 