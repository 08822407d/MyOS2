#include <linux/kernel/err.h>
#include <linux/lib/errno.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <uapi/msdos_fs.h>

#include <linux/fs/fat32.h>
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

/* returns the length of a struct qstr, ignoring trailing dots */
static unsigned int vfat_striptail_len(qstr_s *qname)
{
	size_t len = qname->len;
	const unsigned char *name = qname->name;
	while (len && name[len - 1] == '.')
		len--;
	return len;
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

// static int vfat_add_entry(struct inode *dir, const struct qstr *qname,
// 			  int is_dir, int cluster, struct timespec64 *ts,
// 			  struct fat_slot_info *sinfo)
static int vfat_add_entry(inode_s *dir, const qstr_s *qname,
			  int is_dir, int cluster, fat_slot_info_s *sinfo)
{
	msdos_dirent_s de;
	unsigned int len;
	int err, nr_slots;

	memset(de.name, 0x20, 11);
	len = vfat_striptail_len((qstr_s *)qname);
	strncpy(de.name, qname->name, len);
	if (len == 0)
		return -ENOENT;
	else if (len > 8)
	{
		len = 8;
		color_printk(RED, BLACK, "file name too long, cast to 8 bytes.\n");
	}
	de.attr = is_dir ? ATTR_DIR : ATTR_ARCH;
	// de.lcase = lcase;
	fat_set_start(&de, cluster);
	de.size = 0;

	// err = fat_add_entries(dir, &de, nr_slots, sinfo);
	return err;
}

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