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

int FAT32_create(inode_s * inode, dentry_s * dentry, umode_t mode, bool excl)
{}

dentry_s *FAT32_lookup(inode_s * parent_inode, dentry_s * dest_dentry, unsigned int flags)
{
	int error = 0;
	inode_s * p = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;

	int destlen = dest_dentry->d_name.len;
	const char * destname = dest_dentry->d_name.name;
	loff_t off = 0;
	loff_t de_start = 0;

	int entlen = sizeof(msdos_dirent_s);
	msdos_dirent_s *tmpde = NULL;
	msdos_dirslot_s *tmplde = NULL;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(parent_inode);
	iobuf->iter_init(iobuf);
	// iterate all fat32 entries in this cluster
	while ((de_start = iobuf->iter_cursor) != -1 &&
			!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_getent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;

		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_get_longname(&namelen, iobuf);
		else
			name = FAT32_get_shortname(&namelen, tmpde);

		int len = namelen > destlen ? namelen : destlen;
		bool equal = !strncmp(name, destname, len);
		kfree(name);
		if (equal)
			break;
	}

	if (IS_ERR(tmpde))
	{
		dest_dentry = NULL;
		goto ent_not_found;
	}
	p = fat_build_inode(parent_inode->i_sb, tmpde, 0);
	if (IS_ERR(p))
	{
		dest_dentry = NULL;
		goto build_inode_fail;
	}
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;

	p->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s));
	memset(p->private_idx_info,0,sizeof(FAT32_inode_info_s));
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	// finode->dentry_location	= cluster;
	finode->dentry_position	= iobuf->iter_cursor - entlen;
	finode->create_date		= tmpde->ctime;
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;
	finode->dentry_length	= off - de_start;

	if((tmpde->starthi >> 12) && (p->i_mode & S_IFMT))
	{
		p->i_mode |= S_IFBLK;
	}

	dest_dentry->d_inode = p;

build_inode_fail:
ent_not_found:
	FAT32_iobuf_release(iobuf);
	return dest_dentry;	
}

int FAT32_mkdir(inode_s * inode, dentry_s * dentry, umode_t mode)
{
	uint64_t cluster = 0;

	cluster = FAT32_alloc_new_dir(inode);
}

static int FAT32_mark_entry_romoved(inode_s *inode, loff_t off, loff_t length)
{
	char *buf = NULL;
	size_t bufsize = 0;
	buf = FAT32_read_entirety(inode, &bufsize);
	msdos_dirent_s *de = (msdos_dirent_s *)(buf + off);
}

int FAT32_rmdir(inode_s * parent, dentry_s * dentry)
{
	int error = 0;
	inode_s *inode = dentry->d_inode;
	FAT32_inode_info_s *dest_finode = inode->private_idx_info;

	error = FAT32_dir_empty(inode);
	if (error != 0)
		return error;

	FAT32_mark_entry_romoved(parent, dest_finode->dentry_position,
			dest_finode->dentry_length);
}

int FAT32_rename(inode_s * old_inode, dentry_s * old_dentry,
				inode_s * new_inode, dentry_s * new_dentry,
				unsigned int flags)
{}

// int FAT32_getattr(const path_s *path, kstat_s *stat,
// 				uint32_t request_mask, unsigned int flags)
// {}

// int FAT32_setattr(dentry_s *dentry, iattr_s *attr)
// {}

int FAT32_unlink(inode_s *parent, dentry_s *dest)
{

}

inode_ops_s vfat_dir_inode_operations = 
{
	// .create = FAT32_create,
	.lookup = FAT32_lookup,
	// .mkdir = FAT32_mkdir,
	.rmdir = FAT32_rmdir,
	// .rename = FAT32_rename,
	// .getattr = FAT32_getattr,
	// .setattr = FAT32_setattr,
	// .unlink = FAT32_unlink,
};