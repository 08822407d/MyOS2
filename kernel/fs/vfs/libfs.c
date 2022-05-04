// SPDX-License-Identifier: GPL-2.0-only
/*
 *	fs/libfs.c
 *	Library for filesystems writers.
 */

#include <linux/kernel/blkdev.h>
// #include <linux/export.h>
// #include <linux/pagemap.h>
// #include <linux/slab.h>
#include <linux/kernel/cred.h>
#include <linux/kernel/mount.h>
// #include <linux/vfs.h>
// #include <linux/quotaops.h>
// #include <linux/mutex.h>
#include <linux/fs/namei.h>
// #include <linux/exportfs.h>
// #include <linux/writeback.h>
#include <linux/kernel/buffer_head.h> /* sync_mapping_buffers */
#include <linux/fs/fs_context.h>
// #include <linux/pseudo_fs.h>
// #include <linux/fsnotify.h>
// #include <linux/unicode.h>
// #include <linux/fscrypt.h>
// #include <linux/uaccess.h>
#include <linux/fs/internal.h>


#include <linux/kernel/err.h>

/*
 * Retaining negative dentries for an in-memory filesystem just wastes
 * memory and lookup time: arrange for them to be deleted immediately.
 */
int always_delete_dentry(const dentry_s *dentry)
{
	return 1;
}

const dentry_ops_s simple_dentry_operations = {
	.d_delete = always_delete_dentry,
};

/*
 * Lookup the data. This is trivial - if the dentry didn't already
 * exist, we know it is negative.  Set d_op to delete negative dentries.
 */
dentry_s *simple_lookup(inode_s *dir, dentry_s *dentry, unsigned int flags)
{
	if (dentry->d_name.len > NAME_MAX)
		return ERR_PTR(-ENAMETOOLONG);
	if (!dentry->d_sb->s_d_op)
		d_set_d_op(dentry, &simple_dentry_operations);
	// d_add(dentry, NULL);
	return NULL;
}

const file_ops_s simple_dir_operations = {
	// .open		= dcache_dir_open,
	// .release	= dcache_dir_close,
	// .llseek		= dcache_dir_lseek,
	// .read		= generic_read_dir,
	// .iterate_shared	= dcache_readdir,
	// .fsync		= noop_fsync,
};

const inode_ops_s simple_dir_inode_operations = {
	.lookup		= simple_lookup,
};