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
#include <errno.h>

#include <stdio.h>
#include <string.h>

#include <include/printk.h>
#include <include/proto.h>
#include <include/fs/vfs.h>
#include <include/fs/fat32.h>

#include "arch/amd64/include/device.h"
#include "arch/amd64/include/ide.h"

uint32_t FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry)
{
	uint32_t buf[128];
	memset(buf, 0, 512);
	IDE_device_operation.transfer(ATA_READ_CMD, fsbi->FAT1_firstsector + (fat_entry >> 7),
									1, (unsigned char *)buf);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
}

uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry, uint32_t value)
{
	uint32_t buf[128];
	int i;

	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD, fsbi->FAT1_firstsector + (fat_entry >> 7),
									1, (unsigned char *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);

	for(i = 0; i < fsbi->NumFATs; i++)
		IDE_device_operation.transfer(ATA_WRITE_CMD,
				fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
				1, (unsigned char *)buf);
	return 1;	
}

long FAT32_open(inode_s * inode, file_s * file_p)
{
	return 1;
}

long FAT32_close(inode_s * inode, file_s * file_p)
{
	return 1;
}

long FAT32_read(file_s * filp, char * buf, unsigned long count, long * position)
{
	FAT32_inode_info_s * finode = filp->dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->dentry->d_inode->sb->private_sb_info;

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

	if(*position + count > filp->dentry->d_inode->file_size)
		index = count = filp->dentry->d_inode->file_size - *position;
	else
		index = count;

	do
	{
		memset(buffer, 0, fsbi->bytes_per_cluster);
		sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
		if(!IDE_device_operation.transfer(ATA_READ_CMD,
					sector, fsbi->sector_per_cluster,
					(unsigned char *)buffer))
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

uint64_t FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi)
{
	int i, j;
	int fat_entry;
	unsigned long sector_per_fat = fsbi->sector_per_FAT;
	unsigned int buf[128];

//	fsbi->fat_fsinfo->FSI_Free_Count & fsbi->fat_fsinfo->FSI_Nxt_Free not exactly,so unuse

	for(i = 0; i < sector_per_fat; i++)
	{
		memset(buf, 0, 512);
		IDE_device_operation.transfer(ATA_READ_CMD,
				fsbi->FAT1_firstsector + i,
				1, (unsigned char *)buf);

		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
				return (i << 7) + j;
		}
	}
	return 0;
}

long FAT32_write(file_s * filp, char * buf, unsigned long count, long * position)
{
	FAT32_inode_info_s * finode = filp->dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->dentry->d_inode->sb->private_sb_info;

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
		filp->dentry->d_inode->sb->sb_ops->write_inode(filp->dentry->d_inode);
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
	}

	index = count;

	do
	{
		if(!flags)
		{
			memset(buffer, 0, fsbi->bytes_per_cluster);
			sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
			if(!IDE_device_operation.transfer(ATA_READ_CMD,
						sector, fsbi->sector_per_cluster,
						(unsigned char *)buffer))
			{
				color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
				ret_val = -EIO;
				break;
			}
		}

		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < USERADDR_LIMIT)
			copy_from_user(buffer + offset, buf, length);
		else
			memcpy(buffer + offset, buf, length);

		if(!IDE_device_operation.transfer(ATA_WRITE_CMD,
					sector, fsbi->sector_per_cluster,
					(unsigned char *)buffer))
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

	if(*position > filp->dentry->d_inode->file_size)
	{
		filp->dentry->d_inode->file_size = *position;
		filp->dentry->d_inode->sb->sb_ops->write_inode(filp->dentry->d_inode);
	}

	kfree(buffer);
	if(!index)
		ret_val = count;
	return ret_val;
}

long FAT32_lseek(file_s * filp, long offset, long origin)
{
	inode_s *inode = filp->dentry->d_inode;
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
				pos = filp->dentry->d_inode->file_size + offset;
			break;

		default:
			return -EINVAL;
			break;
	}

	if(pos < 0 || pos > filp->dentry->d_inode->file_size)
		return -EOVERFLOW;

	filp->f_pos = pos;
	// color_printk(GREEN, BLACK, "FAT32 FS(lseek) alert position:%d\n", filp->position);

	return pos;
}

long FAT32_ioctl(inode_s * inode, file_s * filp, unsigned long cmd, unsigned long arg)
{}

long FAT32_readdir(file_s * filp, void * dirent, filldir_t filler)
{
	FAT32_inode_info_s * finode = filp->dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->dentry->d_inode->sb->private_sb_info;

	unsigned int cluster = 0;
	unsigned long sector = 0;
	unsigned char * buf =NULL; 
	char *name = NULL;
	int namelen = 0;
	int i = 0, j = 0, x = 0, y = 0;
	FAT32_dir_s * tmpdentry = NULL;
	FAT32_ldir_s * tmpldentry = NULL;

	buf = kmalloc(fsbi->bytes_per_cluster);

	cluster = finode->first_cluster;

	j = filp->f_pos / fsbi->bytes_per_cluster;
	
	for(i = 0; i < j; i++)
	{
		cluster = FAT32_read_FAT_Entry(fsbi, cluster);
		if(cluster > 0x0ffffff7)
		{
			color_printk(RED, BLACK, "FAT32 FS(readdir) cluster didn`t exist\n");
			return 0;
		}
	}

next_cluster:
	sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
	if(!IDE_device_operation.transfer(ATA_READ_CMD,
				sector, fsbi->sector_per_cluster,
				(unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(readdir) read disk ERROR!!!!!!!!!!\n");
		kfree(buf);
		return 0;
	}

	tmpdentry = (FAT32_dir_s *)(buf + filp->f_pos%fsbi->bytes_per_cluster);

	for(i = filp->f_pos%fsbi->bytes_per_cluster;
		i < fsbi->bytes_per_cluster;
		i += 32, tmpdentry++, filp->f_pos += 32)
	{
		if(tmpdentry->DIR_Attr == ATTR_LONG_NAME)
			continue;
		if(tmpdentry->DIR_Name[0] == 0xe5 ||
			tmpdentry->DIR_Name[0] == 0x00 ||
			tmpdentry->DIR_Name[0] == 0x05)
			continue;

		namelen = 0;
		tmpldentry = (FAT32_ldir_s *)tmpdentry - 1;

		if(tmpldentry->LDIR_Attr == ATTR_LONG_NAME &&
			tmpldentry->LDIR_Ord != 0xe5 &&
			tmpldentry->LDIR_Ord != 0x00 &&
			tmpldentry->LDIR_Ord != 0x05)
		{
			j = 0;
			//long file/dir name read
			while(tmpldentry->LDIR_Attr == ATTR_LONG_NAME &&
					tmpldentry->LDIR_Ord != 0xe5 &&
					tmpldentry->LDIR_Ord != 0x00 &&
					tmpldentry->LDIR_Ord != 0x05)
			{
				j++;
				if(tmpldentry->LDIR_Ord & 0x40)
					break;
				tmpldentry --;
			}

			name = kmalloc(j * 13+1);
			memset(name, 0, j * 13+1);
			tmpldentry = (FAT32_ldir_s *)tmpdentry-1;

			for(x = 0; x < j; x++, tmpldentry--)
			{
				for(y = 0; y < 5; y++)
					if(tmpldentry->LDIR_Name1[y] != 0xffff &&
						tmpldentry->LDIR_Name1[y] != 0x0000)
						name[namelen++] = (char)tmpldentry->LDIR_Name1[y];

				for(y = 0; y < 6; y++)
					if(tmpldentry->LDIR_Name2[y] != 0xffff &&
						tmpldentry->LDIR_Name2[y] != 0x0000)
						name[namelen++] = (char)tmpldentry->LDIR_Name2[y];

				for(y = 0; y < 2; y++)
					if(tmpldentry->LDIR_Name3[y] != 0xffff &&
						tmpldentry->LDIR_Name3[y] != 0x0000)
						name[namelen++] = (char)tmpldentry->LDIR_Name3[y];
			}
			goto find_lookup_success;
		}

		name = kmalloc(15);
		memset(name, 0, 15);
		//short file/dir base name compare
		for(x=0; x < 8; x++)
		{
			if(tmpdentry->DIR_Name[x] == ' ')
				break;
			if(tmpdentry->DIR_NTRes & LOWERCASE_BASE)
				name[namelen++] = tmpdentry->DIR_Name[x] + 32;
			else
				name[namelen++] = tmpdentry->DIR_Name[x];
		}

		if(tmpdentry->DIR_Attr & ATTR_DIRECTORY)
			goto find_lookup_success;

		name[namelen++] = '.';

		//short file ext name compare
		for(x=8; x < 11; x++)
		{
			if(tmpdentry->DIR_Name[x] == ' ')
				break;
			if(tmpdentry->DIR_NTRes & LOWERCASE_EXT)
				name[namelen++] = tmpdentry->DIR_Name[x] + 32;
			else
				name[namelen++] = tmpdentry->DIR_Name[x];
		}
		if(x == 8)
			name[--namelen] = 0;
		goto find_lookup_success;
	}
	
	cluster = FAT32_read_FAT_Entry(fsbi, cluster);
	if(cluster < 0x0ffffff7)
		goto next_cluster;

	kfree(buf);
	return 0;

find_lookup_success:

	filp->f_pos += 32;
	if (filler != NULL)
		return filler(dirent, name, namelen, 0, 0);
	else
		return 0;
}

file_ops_s FAT32_file_ops = 
{
	.open = FAT32_open,
	.close = FAT32_close,
	.read = FAT32_read,
	.write = FAT32_write,
	.lseek = FAT32_lseek,
	.ioctl = FAT32_ioctl,

	.readdir = FAT32_readdir,
};

long FAT32_create(inode_s * inode, dentry_s * dentry, int mode)
{}

dentry_s * FAT32_lookup(inode_s * parent_inode, dentry_s * dest_dentry)
{
	FAT32_inode_info_s * finode = parent_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = parent_inode->sb->private_sb_info;

	unsigned int cluster = 0;
	unsigned long sector = 0;
	unsigned char * buf =NULL; 
	int i = 0,j = 0,x = 0;
	FAT32_dir_s * tmpdentry = NULL;
	FAT32_ldir_s * tmpldentry = NULL;
	inode_s * p = NULL;

	buf = kmalloc(fsbi->bytes_per_cluster);

	cluster = finode->first_cluster;

next_cluster:
	sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
	// color_printk(BLUE,BLACK,"lookup cluster:%#010x,sector:%#018lx\n",cluster,sector);
	if(!IDE_device_operation.transfer(ATA_READ_CMD,
				sector,fsbi->sector_per_cluster,
				(unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(lookup) read disk ERROR!!!!!!!!!!\n");
		kfree(buf);
		return NULL;
	}

	tmpdentry = (FAT32_dir_s *)buf;

	for(i = 0; i < fsbi->bytes_per_cluster; i+= 32,tmpdentry++)
	{
		if(tmpdentry->DIR_Attr == ATTR_LONG_NAME)
			continue;
		if(tmpdentry->DIR_Name[0] == 0xe5 ||
			tmpdentry->DIR_Name[0] == 0x00 ||
			tmpdentry->DIR_Name[0] == 0x05)
			continue;

		tmpldentry = (FAT32_ldir_s *)tmpdentry-1;
		j = 0;

		//long file/dir name compare
		while(tmpldentry->LDIR_Attr == ATTR_LONG_NAME &&
				tmpldentry->LDIR_Ord != 0xe5)
		{
			for(x=0; x<5; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmpldentry->LDIR_Name1[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmpldentry->LDIR_Name1[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0; x<6; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmpldentry->LDIR_Name2[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmpldentry->LDIR_Name2[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0; x<2; x++)
			{
				if(j>dest_dentry->d_name.len &&
					tmpldentry->LDIR_Name3[x] == 0xffff)
					continue;
				else if(j>dest_dentry->d_name.len ||
						tmpldentry->LDIR_Name3[x] != (unsigned short)(dest_dentry->d_name.name[j++]))
					goto continue_cmp_fail;
			}

			if(j >= dest_dentry->d_name.len)
			{
				goto find_lookup_success;
			}

			tmpldentry --;
		}

		//short file/dir base name compare
		j = 0;
		for(x=0; x<8; x++)
		{
			switch(tmpdentry->DIR_Name[x])
			{
				case ' ':
					if(!(tmpdentry->DIR_Attr & ATTR_DIRECTORY))
					{
						if(dest_dentry->d_name.name[j]=='.')
							continue;
						else if(tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
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
							tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
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
					if(tmpdentry->DIR_NTRes & LOWERCASE_BASE)
						if(j < dest_dentry->d_name.len &&
							tmpdentry->DIR_Name[x] + 32 == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					else
					{
						if(j < dest_dentry->d_name.len &&
							tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}

				case '0' ... '9':
					if(j < dest_dentry->d_name.len &&
						tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
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
		if(!(tmpdentry->DIR_Attr & ATTR_DIRECTORY))
		{
			j++;
			for(x=8; x<11; x++)
			{
				switch(tmpdentry->DIR_Name[x])
				{
					case 'A' ... 'Z':
					case 'a' ... 'z':
						if(tmpdentry->DIR_NTRes & LOWERCASE_EXT)
							if(tmpdentry->DIR_Name[x] + 32 == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						else
						{
							if(tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						}

					case '0' ... '9':
						if(tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					case ' ':
						if(tmpdentry->DIR_Name[x] == dest_dentry->d_name.name[j])
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
	p = (inode_s *)kmalloc(sizeof(inode_s));
	memset(p, 0, sizeof(inode_s));
	p->file_size = tmpdentry->DIR_FileSize;
	p->blocks = (p->file_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;
	p->attribute = (tmpdentry->DIR_Attr & ATTR_DIRECTORY) ? FS_ATTR_DIR : FS_ATTR_FILE;
	p->sb = parent_inode->sb;
	p->f_ops = &FAT32_file_ops;
	p->inode_ops = &FAT32_inode_ops;

	p->private_idx_info = (FAT32_inode_info_s *)kmalloc(sizeof(FAT32_inode_info_s));
	memset(p->private_idx_info,0,sizeof(FAT32_inode_info_s));
	finode = p->private_idx_info;

	finode->first_cluster = (tmpdentry->DIR_FstClusHI<< 16 | tmpdentry->DIR_FstClusLO) & 0x0fffffff;
	finode->dentry_location = cluster;
	finode->dentry_position = tmpdentry - (FAT32_dir_s *)buf;
	finode->create_date = tmpdentry->DIR_CrtTime;
	finode->create_time = tmpdentry->DIR_CrtDate;
	finode->write_date = tmpdentry->DIR_WrtTime;
	finode->write_time = tmpdentry->DIR_WrtDate;

	if((tmpdentry->DIR_FstClusHI >> 12) && (p->attribute & FS_ATTR_FILE))
	{
		p->attribute |= FS_ATTR_DEVICE;
	}

	dest_dentry->d_inode = p;
	kfree(buf);
	return dest_dentry;	
}

long FAT32_mkdir(inode_s * inode, dentry_s * dentry, int mode)
{}


long FAT32_rmdir(inode_s * inode, dentry_s * dentry)
{}

long FAT32_rename(inode_s * old_inode, dentry_s * old_dentry, inode_s * new_inode, dentry_s * new_dentry)
{}

long FAT32_getattr(dentry_s * dentry, unsigned long * attr)
{}

long FAT32_setattr(dentry_s * dentry, unsigned long * attr)
{}

inode_ops_s FAT32_inode_ops = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
	.rmdir = FAT32_rmdir,
	.rename = FAT32_rename,
	.getattr = FAT32_getattr,
	.setattr = FAT32_setattr,
};

// these operation need cache and list
long FAT32_compare(dentry_s * parent_dentry, char * source_filename, char * destination_filename){}
long FAT32_hash(dentry_s * dentry,char * filename){}
long FAT32_release(dentry_s * dentry){}
long FAT32_iput(dentry_s * dentry, inode_s * inode){}

dentry_ops_s FAT32_dentry_ops = 
{
	.compare = FAT32_compare,
	.hash = FAT32_hash,
	.release = FAT32_release,
	.iput = FAT32_iput,
};

void fat32_write_superblock(super_block_s * sbp){}

void fat32_put_superblock(super_block_s * sbp)
{
	kfree(sbp->private_sb_info);
	kfree(sbp->root->d_inode->private_idx_info);
	kfree(sbp->root->d_inode);
	kfree(sbp->root);
	kfree(sbp);
}

void fat32_write_inode(inode_s * inode)
{
	FAT32_dir_s * fdentry = NULL;
	FAT32_dir_s * buf = NULL;
	FAT32_inode_info_s * finode = inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = inode->sb->private_sb_info;
	unsigned long sector = 0;

	if(finode->dentry_location == 0)
	{
		color_printk(RED,BLACK,"FS ERROR:write root inode!\n");	
		return ;
	}

	sector = fsbi->Data_firstsector + (finode->dentry_location - 2) * fsbi->sector_per_cluster;
	buf = (FAT32_dir_s *)kmalloc(fsbi->bytes_per_cluster);
	memset(buf, 0, fsbi->bytes_per_cluster);
	IDE_device_operation.transfer(ATA_READ_CMD, sector, fsbi->sector_per_cluster, (unsigned char *)buf);
	fdentry = buf+finode->dentry_position;

	////alert fat32 dentry data
	fdentry->DIR_FileSize = inode->file_size;
	fdentry->DIR_FstClusLO = finode->first_cluster & 0xffff;
	fdentry->DIR_FstClusHI = (fdentry->DIR_FstClusHI & 0xf000) | (finode->first_cluster >> 16);

	IDE_device_operation.transfer(ATA_WRITE_CMD, sector, fsbi->sector_per_cluster, (unsigned char *)buf);
	kfree(buf);
}

sb_ops_s FAT32_sb_ops = 
{
	.write_superblock = fat32_write_superblock,
	.put_superblock = fat32_put_superblock,
	.write_inode = fat32_write_inode,
};

super_block_s * read_fat32_superblock(GPT_PE_s * DPTE, void * buf)
{
	super_block_s * sbp = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_BS_s * fbs = NULL;
	FAT32_SBinfo_s * fsbi = NULL;

	//super block
	sbp = (super_block_s *)kmalloc(sizeof(super_block_s));
	memset(sbp, 0, sizeof(super_block_s));

	sbp->sb_ops = &FAT32_sb_ops;
	sbp->private_sb_info = (FAT32_SBinfo_s *)kmalloc(sizeof(FAT32_SBinfo_s));
	memset(sbp->private_sb_info, 0, sizeof(FAT32_SBinfo_s));

	//fat32 boot sector
	fbs = (FAT32_BS_s *)buf;
	fsbi = sbp->private_sb_info;	
	fsbi->start_sector = DPTE->StartingLBA;
	fsbi->sector_count = DPTE->EndingLBA - DPTE->StartingLBA;
	fsbi->sector_per_cluster = fbs->BPB_SecPerClus;
	fsbi->bytes_per_cluster = fbs->BPB_SecPerClus * fbs->BPB_BytesPerSec;
	fsbi->bytes_per_sector = fbs->BPB_BytesPerSec;
	fsbi->Data_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt + fbs->BPB_FATSz32 * fbs->BPB_NumFATs;
	fsbi->FAT1_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt;
	fsbi->sector_per_FAT = fbs->BPB_FATSz32;
	fsbi->NumFATs = fbs->BPB_NumFATs;
	fsbi->fsinfo_sector_infat = fbs->BPB_FSInfo;
	fsbi->bootsector_bk_infat = fbs->BPB_BkBootSec;	
	
	//fat32 fsinfo sector
	fsbi->fat_fsinfo = (FAT32_FSinfo_s *)kmalloc(sizeof(FAT32_FSinfo_s));
	memset(fsbi->fat_fsinfo, 0, 512);
	IDE_device_operation.transfer(ATA_READ_CMD, DPTE->StartingLBA + fbs->BPB_FSInfo,
									1, (unsigned char *)fsbi->fat_fsinfo);
	
	//directory entry
	sbp->root = (dentry_s *)kmalloc(sizeof(dentry_s));
	memset(sbp->root, 0, sizeof(dentry_s));

	list_init(&sbp->root->dirent_list, sbp->root);
	list_hdr_init(&sbp->root->childdir_lhdr);
	sbp->root->d_parent = sbp->root;
	sbp->root->dir_ops = &FAT32_dentry_ops;
	sbp->root->d_name.name = (char *)kmalloc(2);
	((char *)sbp->root->d_name.name)[0] = '/';
	((char *)sbp->root->d_name.name)[1] = 0;
	sbp->root->d_name.len = 1;

	//index node
	sbp->root->d_inode = (inode_s*)kmalloc(sizeof(inode_s));
	memset(sbp->root->d_inode, 0, sizeof(inode_s));
	sbp->root->d_inode->inode_ops = &FAT32_inode_ops;
	sbp->root->d_inode->f_ops = &FAT32_file_ops;
	sbp->root->d_inode->file_size = 0;
	sbp->root->d_inode->blocks = (sbp->root->d_inode->file_size + fsbi->bytes_per_cluster - 1) /
										fsbi->bytes_per_sector;
	sbp->root->d_inode->attribute = FS_ATTR_DIR;
	sbp->root->d_inode->sb = sbp;

	//fat32 root inode
	sbp->root->d_inode->private_idx_info = (FAT32_inode_info_s *)kmalloc(sizeof(FAT32_inode_info_s));
	memset(sbp->root->d_inode->private_idx_info, 0, sizeof(FAT32_inode_info_s));
	finode = (FAT32_inode_info_s *)sbp->root->d_inode->private_idx_info;
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
	.read_superblock = read_fat32_superblock,
	.next = NULL,
};

void init_FAT32_FS()
{
	register_filesystem(&FAT32_fs_type);
} 