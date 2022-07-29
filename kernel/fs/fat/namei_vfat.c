#include <linux/kernel/err.h>
#include <linux/lib/errno.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/fs/fat32.h>
#include <uapi/msdos_fs.h>

#include <obsolete/proto.h>
#include <obsolete/printk.h>
#include "../../arch/amd64/include/device.h"
#include "../../arch/amd64/include/ide.h"


static int vfat_revalidate(dentry_s *dentry, unsigned int flags)
{
	// if (flags & LOOKUP_RCU)
	// 	return -ECHILD;

	// /* This is not negative dentry. Always valid. */
	// if (d_really_is_positive(dentry))
	// 	return 1;
	// return vfat_revalidate_shortname(dentry);
}

/*
 * Compute the hash for the vfat name corresponding to the dentry.
 * Note: if the name is invalid, we leave the hash code unchanged so
 * that the existing dentry can be used. The vfat fs routines will
 * return ENOENT or EINVAL as appropriate.
 */
static int vfat_hash(const dentry_s *dentry, qstr_s *qstr)
{
	// qstr->hash = full_name_hash(dentry, qstr->name, vfat_striptail_len(qstr));
	return 0;
}

/*
 * Case sensitive compare of two vfat names.
 */
static int vfat_cmp(const dentry_s *dentry, unsigned int len,
				const char *str, const qstr_s *name)
{
	unsigned int alen, blen;

	// /* A filename cannot end in '.' or we treat it like it has none */
	// alen = vfat_striptail_len(name);
	// blen = __vfat_striptail_len(len, str);
	// if (alen == blen) {
	// 	if (strncmp(name->name, str, alen) == 0)
	// 		return 0;
	// }
	return 1;
}

dentry_ops_s vfat_dentry_ops = {
	.d_revalidate	= vfat_revalidate,
	.d_hash			= vfat_hash,
	.d_compare		= vfat_cmp,
};

static int vfat_find(inode_s *dir, const qstr_s *qname,
				fat_slot_info_s *sinfo)
{
	// unsigned int len = vfat_striptail_len(qname);
	// if (len == 0)
	// 	return -ENOENT;
	// return fat_search_long(dir, qname->name, len, sinfo);
}

static dentry_s *vfat_lookup(inode_s *dir, dentry_s *dentry,
				unsigned int flags)
{
	super_block_s	*sb = dir->i_sb;
	fat_slot_info_s	sinfo;
	inode_s			*inode;
	dentry_s		*alias;
	int err;

	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err) {
		if (err == -ENOENT) {
			inode = NULL;
			goto out;
		}
		goto error;
	}

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
// 	brelse(sinfo.bh);
// 	if (IS_ERR(inode)) {
// 		err = PTR_ERR(inode);
// 		goto error;
// 	}

// 	alias = d_find_alias(inode);
// 	/*
// 	 * Checking "alias->d_parent == dentry->d_parent" to make sure
// 	 * FS is not corrupted (especially double linked dir).
// 	 */
// 	if (alias && alias->d_parent == dentry->d_parent) {
// 		/*
// 		 * This inode has non anonymous-DCACHE_DISCONNECTED
// 		 * dentry. This means, the user did ->lookup() by an
// 		 * another name (longname vs 8.3 alias of it) in past.
// 		 *
// 		 * Switch to new one for reason of locality if possible.
// 		 */
// 		if (!S_ISDIR(inode->i_mode))
// 			d_move(alias, dentry);
// 		iput(inode);
// 		mutex_unlock(&MSDOS_SB(sb)->s_lock);
// 		return alias;
// 	} else
// 		dput(alias);

out:
	if (inode == NULL)
		// vfat_d_version_set(dentry, inode_query_iversion(dir));
	// return d_splice_alias(inode, dentry);
error:
	return ERR_PTR(err);
}

int FAT32_create(inode_s * inode, dentry_s * dentry, umode_t mode, bool excl)
{}

dentry_s * FAT32_lookup(inode_s * parent_inode, dentry_s * dest_dentry, unsigned int flags)
{
	FAT32_inode_info_s * finode = parent_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;

	unsigned int cluster = 0;
	unsigned long sector = 0;
	unsigned char * buf =NULL; 
	int i = 0,j = 0,x = 0;
	msdos_dir_entry_s *tmpdentry = NULL;
	FAT32_ldir_s * tmpldentry = NULL;
	inode_s * p = NULL;

	buf = kmalloc(fsbi->bytes_per_cluster);

	cluster = finode->first_cluster;

next_cluster:
	sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
	// color_printk(BLUE,BLACK,"lookup cluster:%#010x,sector:%#018lx\n",cluster,sector);
	if(!ATA_master_ops.transfer(MASTER, SLAVE, ATA_READ_CMD, sector,
					fsbi->sector_per_cluster, (unsigned char *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(lookup) read disk ERROR!!!!!!!!!!\n");
		kfree(buf);
		return NULL;
	}

	tmpdentry = (msdos_dir_entry_s *)buf;

	for(i = 0; i < fsbi->bytes_per_cluster; i+= 32,tmpdentry++)
	{
		if(tmpdentry->attr== ATTR_LONG_NAME)
			continue;
		if(tmpdentry->name[0] == 0xe5 ||
			tmpdentry->name[0] == 0x00 ||
			tmpdentry->name[0] == 0x05)
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
			switch(tmpdentry->name[x])
			{
				case ' ':
					if(!(tmpdentry->attr & ATTR_DIRECTORY))
					{
						if(dest_dentry->d_name.name[j]=='.')
							continue;
						else if(tmpdentry->name[x] == dest_dentry->d_name.name[j])
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
							tmpdentry->name[x] == dest_dentry->d_name.name[j])
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
					if(tmpdentry->lcase & LOWERCASE_BASE)
						if(j < dest_dentry->d_name.len &&
							tmpdentry->name[x] + 32 == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					else
					{
						if(j < dest_dentry->d_name.len &&
							tmpdentry->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}

				case '0' ... '9':
					if(j < dest_dentry->d_name.len &&
						tmpdentry->name[x] == dest_dentry->d_name.name[j])
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
		if(!(tmpdentry->attr & ATTR_DIRECTORY))
		{
			j++;
			for(x=8; x<11; x++)
			{
				switch(tmpdentry->name [x])
				{
					case 'A' ... 'Z':
					case 'a' ... 'z':
						if(tmpdentry->lcase & LOWERCASE_EXT)
							if(tmpdentry->name[x] + 32 == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						else
						{
							if(tmpdentry->name[x] == dest_dentry->d_name.name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						}

					case '0' ... '9':
						if(tmpdentry->name[x] == dest_dentry->d_name.name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					case ' ':
						if(tmpdentry->name[x] == dest_dentry->d_name.name[j])
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
	p = fat_build_inode(parent_inode->i_sb, tmpdentry, 0);
	if (IS_ERR(p))
		return ERR_CAST(p);
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;


	p->private_idx_info = (FAT32_inode_info_s *)kmalloc(sizeof(FAT32_inode_info_s));
	memset(p->private_idx_info,0,sizeof(FAT32_inode_info_s));
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpdentry->starthi << 16 | tmpdentry->start) & 0x0fffffff;
	finode->dentry_location	= cluster;
	finode->dentry_position	= tmpdentry - (msdos_dir_entry_s *)buf;
	finode->create_date		= tmpdentry->ctime;
	finode->create_time		= tmpdentry->cdate;
	finode->write_date		= tmpdentry->time;
	finode->write_time		= tmpdentry->date;

	if((tmpdentry->starthi >> 12) && (p->i_mode & S_IFMT))
	{
		p->i_mode |= S_IFBLK;
	}

	dest_dentry->d_inode = p;
	kfree(buf);
	return dest_dentry;	
}

int FAT32_mkdir(inode_s * inode, dentry_s * dentry, umode_t mode)
{}


int FAT32_rmdir(inode_s * inode, dentry_s * dentry)
{}

int FAT32_rename(inode_s * old_inode, dentry_s * old_dentry,
				inode_s * new_inode, dentry_s * new_dentry,
				unsigned int flags)
{}

// int FAT32_getattr(const path_s *path, kstat_s *stat,
// 				uint32_t request_mask, unsigned int flags)
// {}

// int FAT32_setattr(dentry_s *dentry, iattr_s *attr)
// {}

inode_ops_s vfat_dir_inode_operations = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
	.rmdir = FAT32_rmdir,
	.rename = FAT32_rename,
	// .getattr = FAT32_getattr,
	// .setattr = FAT32_setattr,
};

static void setup(super_block_s *sb)
{
	MSDOS_SB(sb)->dir_ops = &vfat_dir_inode_operations;
	// if (MSDOS_SB(sb)->options.name_check != 's')
	// 	sb->s_d_op = &vfat_ci_dentry_ops;
	// else
		sb->s_d_op = &vfat_dentry_ops;
}

static int vfat_fill_super(super_block_s *sb, void *data, int silent)
{
	return fat_fill_super(sb, data, 1, setup);
}

extern fs_type_s vfat_fs_type;
static dentry_s *vfat_mount(fs_type_s *fs_type, int flags,
				const char *dev_name, void *data)
{
	return mount_bdev(&vfat_fs_type, flags, dev_name, data, vfat_fill_super);
}

fs_type_s vfat_fs_type = {
	.name		= "vfat",
	.mount		= vfat_mount,
	.fs_flags	= FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
};

int init_vfat_fs(void)
{
	return register_filesystem(&vfat_fs_type);
}

void exit_vfat_fs(void)
{
	unregister_filesystem(&vfat_fs_type);
}