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

dentry_s * FAT32_lookup(inode_s * parent_inode, dentry_s * dest_dentry, unsigned int flags)
{
	inode_s * p = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;

	int i = 0,j = 0,x = 0;
	char *buf;
	int error = 0;
	int destlen = dest_dentry->d_name.len;
	const char * destname = dest_dentry->d_name.name;
	loff_t off = 0;
	loff_t de_start = 0;
	size_t bufsize = 0;
	msdos_dirent_s * tmpde = NULL;
	msdos_dirslot_s * tmplde = NULL;

	buf = FAT32_read_entirety(parent_inode, &bufsize);
	tmpde = (msdos_dirent_s *)buf;
	// iterate all fat32 entries
	while (off < bufsize)
	{
		de_start = off;
		tmpde = FAT32_get_full_ent(buf, bufsize, &off);
		tmplde = (msdos_dirslot_s *)tmpde - 1;
		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
		else if(tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_get_longname(&namelen, tmplde);
		else
			name = FAT32_get_shortname(&namelen, tmpde);

		int len = namelen > destlen ? namelen : destlen;
		bool equal = !strncmp(name, destname, len);
		kfree(name);
		if (equal)
			break;
	}

	p = fat_build_inode(parent_inode->i_sb, tmpde, 0);
	if (IS_ERR(p))
		return ERR_CAST(p);
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;

	p->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s));
	memset(p->private_idx_info,0,sizeof(FAT32_inode_info_s));
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	// finode->dentry_location	= cluster;
	finode->dentry_position	= tmpde - (msdos_dirent_s *)buf;
	finode->create_date		= tmpde->ctime;
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;

	finode->dentry_start	= de_start;
	finode->dentry_length	= off - de_start;

	if((tmpde->starthi >> 12) && (p->i_mode & S_IFMT))
	{
		p->i_mode |= S_IFBLK;
	}

	dest_dentry->d_inode = p;
	kfree(buf);
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

	FAT32_mark_entry_romoved(parent, dest_finode->dentry_start,
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