#include <linux/kernel/slab.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <linux/lib/errno.h>
#include <linux/lib/string.h>
#include <uapi/kernel/msdos_fs.h>

#include <linux/fs/fat32.h>
#include <obsolete/printk.h>
#include <obsolete/device.h>
#include <obsolete/ide.h>


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
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(parent_inode);
	iobuf->iter_init(iobuf);
	// iterate all fat32 entries in this cluster
	while ((de_start = iobuf->iter_cursor) < FAT_MAX_DIR_SIZE &&
			!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;

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
	p = fat_build_inode(parent_inode->i_sb, (msdos_dirent_s *)tmpde, 0);
	if (IS_ERR(p))
	{
		dest_dentry = NULL;
		goto build_inode_fail;
	}
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;

	p->private_idx_info = kzalloc(sizeof(FAT32_inode_info_s), GFP_KERNEL);
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	finode->dentry_location	= iobuf->clusters[de_start / iobuf->bufsize];
	finode->dentry_position	= de_start;
	finode->lcase			= tmpde->lcase;
	finode->create_time_cs	= tmpde->ctime_cs;
	finode->create_date		= tmpde->ctime;
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;
	finode->access_date		= tmpde->adate;
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

extern int vfat_add_entry(inode_s *dir, const qstr_s *qname,
			  int is_dir, int cluster, fat_slot_info_s *sinfo);
int FAT32_create(inode_s * dir, dentry_s * dentry, umode_t mode)
{
	int err;
	fat_slot_info_s sinfo;

	err = vfat_add_entry(dir, &dentry->d_name, false, 0, &sinfo);
	if (err)
		goto out;

	out:
	return err;
}

int FAT32_mkdir(inode_s * dir, dentry_s * dentry, umode_t mode)
{
	fat_slot_info_s sinfo;
	int err, cluster = 0;

	cluster = FAT32_alloc_new_dir(dir);
	if (cluster < 0) {
		err = cluster;
		goto out;
	}
	err = vfat_add_entry(dir, &dentry->d_name, true, cluster, &sinfo);
out:
	return err;
}

static int FAT32_release_clusters(inode_s *dir)
{
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
	for (int i = iobuf->nr_clus - 1; i >= 0; i--)
		iobuf->flags[i] = FAT32_IOBUF_DELETE;

	FAT32_iobuf_release(iobuf);
}

static int FAT32_mark_entry_romoved(inode_s *inode, loff_t off, loff_t length)
{
	char *buf = NULL;
	size_t bufsize = 0;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(inode);
	iobuf->iter_init(iobuf);
	for (size_t i = 0; i < length; i+=sizeof(msdos_dirent_s))
	{
		char c = FAT32_DELETED_FLAG;
		FAT32_iobuf_write(iobuf, i + off, &c, 1);
	}
	FAT32_iobuf_release(iobuf);
}

int FAT32_rmdir(inode_s * parent, dentry_s * dest)
{
	int error = 0;
	inode_s *inode = dest->d_inode;
	FAT32_inode_info_s *dest_finode = inode->private_idx_info;

	error = FAT32_dir_empty(inode);
	if (error != 0)
		return error;
	// err = vfat_find(dir, &dentry->d_name, &sinfo);
	// if (err)
	// 	goto out;

	FAT32_release_clusters(inode);
	FAT32_mark_entry_romoved(parent, dest_finode->dentry_position,
			dest_finode->dentry_length);

	return error;
}

int FAT32_unlink(inode_s *parent, dentry_s *dest)
{
	int error = 0;
	inode_s *inode = dest->d_inode;
	FAT32_inode_info_s *dest_finode = inode->private_idx_info;
	// err = vfat_find(dir, &dentry->d_name, &sinfo);
	// if (err)
	// 	goto out;

	FAT32_release_clusters(inode);
	FAT32_mark_entry_romoved(parent, dest_finode->dentry_position,
			dest_finode->dentry_length);

	return error;
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

inode_ops_s vfat_dir_inode_operations = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
	.rmdir = FAT32_rmdir,
	// .rename = FAT32_rename,
	// .getattr = FAT32_getattr,
	// .setattr = FAT32_setattr,
	.unlink = FAT32_unlink,
};