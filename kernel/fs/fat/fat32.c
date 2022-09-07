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
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/proto.h>
#include "../../arch/amd64/include/device.h"
#include "../../arch/amd64/include/ide.h"

bool FAT32_ent_empty(msdos_dirent_s *de)
{
	return (de->name[0] == 0xe5 ||
			de->name[0] == 0x00 ||
			de->name[0] == 0x05);
}

u32 FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry)
{
	uint32_t buf[128];
	memset(buf, 0, 512);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, fsbi->FAT1_firstsector + (fat_entry >> 7),
									1, (unsigned char *)buf);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
}

msdos_dirent_s *FAT32_get_full_ent(char *buf, size_t bufsize, loff_t *pos)
{
	size_t len = 0;
	msdos_dirent_s *ret_val;
	do
	{
		ret_val = (msdos_dirent_s *)(buf + *pos);
		*pos += sizeof(msdos_dirent_s);
	}
	while ((ret_val->attr == ATTR_LONG_NAME ||
			FAT32_ent_empty(ret_val)) &&
			*pos < bufsize);
	
	return ret_val;
}

static List_hdr_s *get_cluster_chain(inode_s *inode)
{
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	unsigned long cluster = finode->first_cluster;
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s));
	list_hdr_init(clus_lhdrp);

	do
	{
		clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s));
		list_init(&clus_sp->list, clus_sp);

		clus_sp->cluster = cluster;
		list_hdr_enqueue(clus_lhdrp, &clus_sp->list);
	} while ((cluster = FAT32_read_FAT_Entry(fsbi, cluster)) <= MAX_FAT32);

	if (cluster == BAD_FAT32)
	{
		list_hdr_dump(clus_lhdrp);
		kfree(clus_lhdrp);
		clus_lhdrp = NULL;
	}

	return clus_lhdrp;
}

char *FAT32_read_entirety(inode_s *inode ,size_t *size)
{
	char *ret_val = NULL;
	size_t bufsize;

	*size = 0;
	size_t pos = 0;
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	size_t clus_size = fsbi->bytes_per_cluster;

	List_hdr_s *clus_lhdrp = get_cluster_chain(inode);
	if (clus_lhdrp == NULL)
	{
		ret_val = ERR_PTR(-EIO);
		goto get_clus_chain_fail;
	}
	bufsize = clus_lhdrp->count * clus_size;
	char *buf = kmalloc(bufsize);
	if (buf == NULL)
	{
		ret_val = ERR_PTR(-ENOMEM);
		goto alloc_buf_fail;
	}
	memset(buf, 0, bufsize);

	while (clus_lhdrp->count > 0)
	{
		size_t offset = 0;
		List_s *lp = list_hdr_dequeue(clus_lhdrp);
		clus_list_s *clus_lp = container_of(lp, clus_list_s, list);
		u32 cluster = clus_lp->cluster;

		if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
				FAT32_clus_to_blknr(fsbi, cluster),
				fsbi->sector_per_cluster, (unsigned char *)buf + pos))
		{
			color_printk(RED, BLACK, "FAT32 FS(read) read disk ERROR!!!!!!!!!!\n");
			ret_val = ERR_PTR(-EIO);
			goto read_clus_fail;
		}
		pos += clus_size;
	}
	ret_val = buf;
	*size = bufsize;
	goto success;

read_clus_fail:
	kfree(buf);
alloc_buf_fail:	
	list_hdr_dump(clus_lhdrp);
	kfree(clus_lhdrp);
get_clus_chain_fail:
success:
	return ret_val;
}

/* See if directory is empty */
int FAT32_dir_empty(inode_s *dir)
{
	char *buf;
	msdos_dirent_s *de;
	int result = ENOERR;
	size_t pos = 0;
	size_t bufsize = 0;

	buf = FAT32_read_entirety(dir, &bufsize);
	
	while (pos < bufsize)
	{
		de = (msdos_dirent_s *)(buf + pos);
		if (!FAT32_ent_empty(de) &&
			strncmp(de->name, MSDOS_DOT   , MSDOS_NAME) &&
			strncmp(de->name, MSDOS_DOTDOT, MSDOS_NAME))
		{
			result = -ENOTEMPTY;
			break;
		}
		pos += sizeof(msdos_dirent_s);
	}
	kfree(buf);
	return result;
}

s64 FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi)
{
	int i, j;
	int fat_entry;
	unsigned long sector_per_fat = fsbi->sector_per_FAT;
	unsigned int buf[128];

//	fsbi->fat_fsinfo->FSI_Free_Count & fsbi->fat_fsinfo->FSI_Nxt_Free not exactly,so unuse

	for(i = 0; i < sector_per_fat; i++)
	{
		memset(buf, 0, 512);
		ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, fsbi->FAT1_firstsector + i,
						1, (unsigned char *)buf);

		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
				return (i << 7) + j;
		}
	}
	return 0;
}

uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry, uint32_t value)
{
	uint32_t buf[128];
	int i;

	memset(buf,0,512);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
					fsbi->FAT1_firstsector + (fat_entry >> 7),
					1, (unsigned char *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);

	for(i = 0; i < fsbi->NumFATs; i++)
		ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD,
				fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
				1, (unsigned char *)buf);
	return 1;	
}

char *FAT32_get_shortname(int *namelen, msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kmalloc(15);
	memset(ret_val, 0, 15);

	//short file/dir base name compare
	for(x=0; x < 8; x++)
	{
		if(de->name[x] == ' ')
			break;
		if(de->lcase & LOWERCASE_BASE)
			ret_val[(*namelen)++] = de->name[x] + 32;
		else
			ret_val[(*namelen)++] = de->name[x];
	}

	if(!(de->attr & ATTR_DIRECTORY))
	{
		ret_val[(*namelen)++] = '.';

		//short file ext name compare
		for(x=8; x < 11; x++)
		{
			if(de->name[x] == ' ')
				break;
			if(de->lcase & LOWERCASE_EXT)
				ret_val[(*namelen)++] = de->name[x] + 32;
			else
				ret_val[(*namelen)++] = de->name[x];
		}
		if(x == 8)
			ret_val[--(*namelen)] = 0;
	}

	return ret_val;
}

char *FAT32_get_longname(int *namelen, msdos_dirslot_s *lde)
{
	int i = 0, x, y;
	char *ret_val;
	msdos_dirslot_s *lde_cursor = lde;
	while(lde_cursor->attr == ATTR_LONG_NAME &&
		!FAT32_ent_empty((msdos_dirent_s *)lde_cursor))
	{
		i++;
		if(lde_cursor->id & 0x40)
			break;
		lde_cursor--;
	}

	//long file/dir name read
	ret_val = kmalloc(i * 13 + 1);
	memset(ret_val, 0, i * 13 + 1);
	for(x = 0; x < i; x++, lde--)
	{
		for(y = 0; y < 10; y += 2)
			if(*(u16 *)(&lde->name0_4[y]) != 0xffff &&
				*(u16 *)(&lde->name0_4[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name0_4[y];

		for(y = 0; y < 12; y += 2)
			if(*(u16 *)(&lde->name5_10[y]) != 0xffff &&
				*(u16 *)(&lde->name5_10[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name5_10[y];

		for(y = 0; y < 4; y += 2)
			if(*(u16 *)(&lde->name11_12[y]) != 0xffff &&
				*(u16 *)(&lde->name11_12[y]) != 0x0000)
				ret_val[(*namelen)++] = (char)lde->name11_12[y];
	}

	return ret_val;
}

s64 FAT32_alloc_new_dir(inode_s *dir)
{
	s64 cluster = 0;
	s64 sector = 0;
	char *clus_buf = NULL;
	FAT32_SBinfo_s *fsbi = NULL;
	FAT32_inode_info_s * finode;
	msdos_dirent_s *de;

	fsbi = (FAT32_SBinfo_s *)dir->i_sb->private_sb_info;
	finode = (FAT32_inode_info_s *)dir->private_idx_info;
	sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
	clus_buf = kmalloc(fsbi->bytes_per_cluster);
	if (clus_buf == NULL)
		return -(ENOMEM);
	memset(clus_buf, 0, fsbi->bytes_per_cluster);

	de = (msdos_dirent_s *)clus_buf;
	/* filling the new directory slots ("." and ".." entries) */
	memcpy(de[0].name, MSDOS_DOT, MSDOS_NAME);
	memcpy(de[1].name, MSDOS_DOTDOT, MSDOS_NAME);
	de->attr = de[1].attr = ATTR_DIR;
	de[0].lcase = de[1].lcase = 0;
	de[0].size = de[1].size = 0;
	fat_set_start(&de[0], cluster);
	fat_set_start(&de[1], finode->first_cluster);

	if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)clus_buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
	}

	return cluster;
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
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster);

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
		if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
						fsbi->sector_per_cluster, (unsigned char *)buffer))
		{
			color_printk(RED, BLACK, "FAT32 FS(read) read disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
		}

		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < USERADDR_LIMIT)
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

	unsigned long cluster = finode->first_cluster;
	unsigned long next_cluster = 0;
	unsigned long sector = 0;
	int i,length = 0;
	long ret_val = 0;
	long flags = 0;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster);

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
			if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
							fsbi->sector_per_cluster, (unsigned char *)buffer))
			{
				color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
				ret_val = -EIO;
				break;
			}
		}

		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < USERADDR_LIMIT)
			copy_from_user(buffer + offset, (void *)buf, length);
		else
			memcpy(buffer + offset, buf, length);

		if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD, sector,
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

int FAT32_getdents64(file_s * filp, dir_ctxt_s *ctx)
{
	if (ctx->pos == -1)
		return 0;

	char *buf;
	int error = 0;
	size_t bufsize = 0;
	msdos_dirent_s * tmpde = NULL;
	msdos_dirslot_s * tmplde = NULL;

	buf = FAT32_read_entirety(filp->f_path.dentry->d_inode, &bufsize);
	// iterate all fat32 entries in this cluster
	while (ctx->pos < bufsize)
	{
		tmpde = FAT32_get_full_ent(buf, bufsize, &(ctx->pos));
		tmplde = (msdos_dirslot_s *)tmpde - 1;
		// parse current fat32_dir_entry
		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
		else if(tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_get_longname(&namelen, tmplde);
		else
			name = FAT32_get_shortname(&namelen, tmpde);

		error = ctx->actor(ctx, name, namelen, 0, 0, 0);

		kfree(name);
		if (error !=0)
			break;
	}

	if (error == 0)
		ctx->pos = -1;
	
	kfree(buf);
	return error;
}

file_ops_s FAT32_file_ops = 
{
	.open = FAT32_open,
	.close = FAT32_close,
	.read = FAT32_read,
	.write = FAT32_write,
	.llseek = FAT32_lseek,
	.ioctl = FAT32_ioctl,

	.iterate_shared = FAT32_getdents64,
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
	buf = kmalloc(fsbi->bytes_per_cluster);
	memset(buf, 0, fsbi->bytes_per_cluster);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buf);
	fdentry = buf+finode->dentry_position;

	////alert fat32 dentry data
	fdentry->size = inode->i_size;
	fdentry->start = finode->first_cluster & 0xffff;
	fdentry->starthi = (fdentry->starthi & 0xf000) | (finode->first_cluster >> 16);

	ATA_master_ops.transfer(MASTER, SLAVE, ATA_WRITE_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buf);
	kfree(buf);
}

super_ops_s FAT32_sb_ops = 
{
	.write_super = fat32_write_superblock,
	.put_super = fat32_put_superblock,
	.write_inode = fat32_write_inode,
};

super_block_s * read_fat32_superblock(GPT_PE_s * DPTE, void * buf)
{
	super_block_s * sbp = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_BS_s * fbs = NULL;
	FAT32_SBinfo_s * fsbi = NULL;

	//super block
	sbp = kmalloc(sizeof(super_block_s));
	memset(sbp, 0, sizeof(super_block_s));

	sbp->s_op = &FAT32_sb_ops;
	sbp->private_sb_info = kmalloc(sizeof(FAT32_SBinfo_s));
	memset(sbp->private_sb_info, 0, sizeof(FAT32_SBinfo_s));

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
	fsbi->fat_fsinfo = kmalloc(sizeof(FAT32_FSinfo_s));
	memset(fsbi->fat_fsinfo, 0, 512);
	ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD,
					DPTE->StartingLBA + fbs->BPB_FSInfo,
					1, (unsigned char *)fsbi->fat_fsinfo);
	
	//directory entry
	sbp->s_root = kmalloc(sizeof(dentry_s));
	memset(sbp->s_root, 0, sizeof(dentry_s));

	list_init(&sbp->s_root->d_child, sbp->s_root);
	list_hdr_init(&sbp->s_root->d_subdirs);
	sbp->s_root->d_parent = sbp->s_root;
	sbp->s_root->d_op = &vfat_dentry_ops;
	sbp->s_root->d_name.name = (char *)kmalloc(2);
	((char *)sbp->s_root->d_name.name)[0] = '/';
	((char *)sbp->s_root->d_name.name)[1] = 0;
	sbp->s_root->d_name.len = 1;

	//index node
	sbp->s_root->d_inode = (inode_s*)kmalloc(sizeof(inode_s));
	memset(sbp->s_root->d_inode, 0, sizeof(inode_s));
	sbp->s_root->d_inode->i_op = &vfat_dir_inode_operations;
	sbp->s_root->d_inode->i_fop = &FAT32_file_ops;
	sbp->s_root->d_inode->i_size = 0;
	sbp->s_root->d_inode->i_blocks =
					(sbp->s_root->d_inode->i_size + fsbi->bytes_per_cluster - 1) /
							fsbi->bytes_per_sector;
	sbp->s_root->d_inode->i_mode = S_IFDIR;
	sbp->s_root->d_inode->i_sb = sbp;

	//fat32 root inode
	sbp->s_root->d_inode->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s));
	memset(sbp->s_root->d_inode->private_idx_info, 0, sizeof(FAT32_inode_info_s));
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

fs_type_s FAT32_fs_type=
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

dentry_s * FAT32_lookup_old(inode_s * parent_inode, dentry_s * dest_dentry, unsigned int flags)
{
	FAT32_inode_info_s * finode = parent_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;

	unsigned int cluster = 0;
	unsigned long sector = 0;
	unsigned char * buf =NULL; 
	int i = 0,j = 0,x = 0;
	msdos_dirent_s *tmpde = NULL;
	msdos_dirslot_s * tmplde = NULL;
	inode_s * p = NULL;

	buf = kmalloc(fsbi->bytes_per_cluster);

	cluster = finode->first_cluster;

next_cluster:
	sector = FAT32_clus_to_blknr(fsbi, cluster);
	// color_printk(BLUE,BLACK,"lookup cluster:%#010x,sector:%#018lx\n",cluster,sector);
	if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(lookup) read disk ERROR!!!!!!!!!!\n");
		kfree(buf);
		return NULL;
	}

	tmpde = (msdos_dirent_s *)buf;

	for(i = 0; i < fsbi->bytes_per_cluster; i+= 32,tmpde++)
	{
		if(tmpde->attr== ATTR_LONG_NAME)
			continue;
		if(FAT32_ent_empty(tmpde))
			continue;

		tmplde = (msdos_dirslot_s *)tmpde-1;
		j = 0;

		//long file/dir name compare
		while(tmplde->attr == ATTR_LONG_NAME &&
				tmplde->id != 0xe5)
		{
			for(x=0; x<5; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmplde->name0_4[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmplde->name0_4[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0; x<6; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmplde->name5_10[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmplde->name5_10[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0; x<2; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmplde->name11_12[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmplde->name11_12[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}

			if(j >= dest_dentry->d_name.len)
			{
				goto find_lookup_success;
			}

			tmplde --;
		}

		//short file/dir base name compare
		j = 0;
		for(x=0; x<8; x++)
		{
			switch(tmpde->name[x])
			{
				case ' ':
					if(!(tmpde->attr & ATTR_DIRECTORY))
					{
						if(dest_dentry->d_name.name[j]=='.')
							continue;
						else if(tmpde->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}
					else
					{
						if(j < dest_dentry->d_name.len &&
							tmpde->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else if(j == dest_dentry->d_name.len)
							continue;
						else
							goto continue_cmp_fail;
					}

				case 'A' ... 'Z':
				case 'a' ... 'z':
					if(tmpde->lcase & LOWERCASE_BASE)
						if(j < dest_dentry->d_name.len &&
							tmpde->name[x] + 32 == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					else
					{
						if(j < dest_dentry->d_name.len &&
							tmpde->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}

				case '0' ... '9':
					if(j < dest_dentry->d_name.len &&
						tmpde->name[x] == dest_dentry->d_name.name[j])
					{
						j++;
						break;
					}
					else
						goto continue_cmp_fail;

				default :
					j++;
					break;
			}
		}
		//short file ext name compare
		if(!(tmpde->attr & ATTR_DIRECTORY))
		{
			j++;
			for(x=8; x<11; x++)
			{
				switch(tmpde->name [x])
				{
					case 'A' ... 'Z':
					case 'a' ... 'z':
						if(tmpde->lcase & LOWERCASE_EXT)
							if(tmpde->name[x] + 32 == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						else
						{
							if(tmpde->name[x] == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						}

					case '0' ... '9':
						if(tmpde->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					case ' ':
						if(tmpde->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					default :
						goto continue_cmp_fail;
				}
			}
		}
		goto find_lookup_success;

continue_cmp_fail:;
	}
	
	cluster = FAT32_read_FAT_Entry(fsbi, cluster);
	if(cluster < 0x0ffffff7)
		goto next_cluster;

	kfree(buf);
	return NULL;

find_lookup_success:
	p = fat_build_inode(parent_inode->i_sb, tmpde, 0);
	if (IS_ERR(p))
		return ERR_CAST(p);
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;


	p->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s));
	memset(p->private_idx_info,0,sizeof(FAT32_inode_info_s));
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	finode->dentry_location	= cluster;
	finode->dentry_position	= tmpde - (msdos_dirent_s *)buf;
	finode->create_date		= tmpde->ctime;
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;

	if((tmpde->starthi >> 12) && (p->i_mode & S_IFMT))
	{
		p->i_mode |= S_IFBLK;
	}

	dest_dentry->d_inode = p;
	kfree(buf);
	return dest_dentry;	
}