#include <linux/kernel/slab.h>
#include <linux/kernel/ctype.h>
#include <linux/lib/errno.h>
#include <linux/fs/fs.h>
#include <linux/fs/fat.h>
#include <uapi/kernel/msdos_fs.h>

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


static int vfat_create_shortname(const char *name, int len, char *name_res)
{
	int ret_val = 0;
	loff_t ext_idx;
	memset(name_res, ' ', MSDOS_NAME);
	char *name_upper = myos_kmalloc(len + 1);
	for (int i = 0; i < len; i++)
		name_upper[i] = toupper(name[i]);
	name_upper[len] = '\0';

	char *last = strrchr(name, '.');
	if (last == NULL)
		ext_idx = len;
	else
		ext_idx = last - name + 1;

	if (len <= 8)
	{
		memcpy(name_res, name_upper, len);
		// ret_val = 1;
	}
	else
	{
		int baselen = ext_idx > 6 ? 6 : ext_idx;
		int extlen = len - ext_idx > 3 ? 3 : len - ext_idx;
		memcpy(name_res, name_upper, baselen);
		memcpy(name_res + 8, name_upper + ext_idx, extlen);
		name_res[6] = '~';
		name_res[7] = '1';
	}

	return ret_val;
}

static void ascii_to16(u8 *buf, const char *name, int len)
{
	for (int i = 0; i < len; i++)
	{
		buf[2 * i] = name[i];
		buf[2 * i + 1] = 0;
	}
}

static int vfat_build_slots(inode_s *dir, const unsigned char *name, int len,
				int is_dir, int cluster, msdos_dirslot_s *slots, int *nr_slots)
{
	msdos_dirslot_s *ps;
	msdos_dirent_s *de;
	FAT32_inode_info_s *finode = dir->private_idx_info;
	unsigned char cksum, lcase;
	unsigned char msdos_name[MSDOS_NAME];
	int err, i;
	loff_t offset;

	*nr_slots = 0;

	err = vfat_create_shortname(name, len, msdos_name);
	if (err < 0)
		goto out_free;
	else if (err == 1) {
		de = (msdos_dirent_s *)slots;
		err = 0;
		goto shortname;
	}

	/* build the entry of long file name */
	cksum = fat_checksum(msdos_name);

	*nr_slots = (len + 12) / 13;
	char *namebuf = myos_kmalloc(*nr_slots * 13);
	memset(namebuf, 0, *nr_slots * 13);
	memcpy(namebuf, name, len);
	for (ps = slots, i = *nr_slots; i > 0; i--, ps++) {
		ps->id = i;
		ps->attr = ATTR_EXT;
		ps->reserved = 0;
		ps->alias_checksum = cksum;
		ps->start = 0;
		offset = (i - 1) * 13;
		ascii_to16(ps->name0_4, namebuf + offset, 5);
		ascii_to16(ps->name5_10, namebuf + offset + 5, 6);
		ascii_to16(ps->name11_12, namebuf + offset + 11, 2);
	}
	slots[0].id |= 0x40;
	de = (msdos_dirent_s *)ps;

shortname:
	/* build the entry of 8.3 alias name */
	(*nr_slots)++;
	memcpy(de->name, msdos_name, MSDOS_NAME);
	de->attr		= is_dir ? ATTR_DIR : ATTR_ARCH;
	de->lcase		= finode->lcase;
	de->ctime_cs	= finode->create_time_cs;
	de->time		= finode->write_time;
	de->date		= finode->write_date;
	de->ctime		= finode->create_time;
	de->cdate		= finode->create_date;
	de->adate		= finode->access_date;
	fat_set_start(de, cluster);
	de->size = 0;
out_free:
	return err;
}

// static int vfat_add_entry(struct inode *dir, const struct qstr *qname,
// 			  int is_dir, int cluster, struct timespec64 *ts,
// 			  struct fat_slot_info *sinfo)
int vfat_add_entry(inode_s *dir, const qstr_s *qname,
			  int is_dir, int cluster, fat_slot_info_s *sinfo)
{
	msdos_dirslot_s *slots;
	unsigned int len;
	int err, nr_slots;

	len = vfat_striptail_len((qstr_s *)qname);
	if (len == 0)
		return -ENOENT;

	slots = myos_kmalloc(MSDOS_SLOTS * sizeof(msdos_dirent_s));
	if (slots == NULL)
		return -ENOMEM;
	memset(slots, 0, MSDOS_SLOTS * sizeof(msdos_dirent_s));

	err = vfat_build_slots(dir, qname->name, len, is_dir, cluster, slots, &nr_slots);
	if (err)
		goto cleanup;

	err = fat_add_entries(dir, slots, nr_slots);

cleanup:
	kfree(slots);
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