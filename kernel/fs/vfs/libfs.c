// SPDX-License-Identifier: GPL-2.0-only
/*
 *	fs/libfs.c
 *	Library for filesystems writers.
 */

#include <linux/block/blkdev.h>
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
#include <linux/block/buffer_head.h> /* sync_mapping_buffers */
#include <linux/fs/fs_context.h>
#include <linux/fs/pseudo_fs.h>
// #include <linux/fsnotify.h>
// #include <linux/unicode.h>
// #include <linux/fscrypt.h>
// #include <linux/uaccess.h>
#include <linux/fs/internal.h>


#include <linux/kernel/err.h>
#include <include/proto.h>

int simple_statfs(dentry_s *dentry, kstatfs_s *buf)
{
	buf->f_type = dentry->d_sb->s_magic;
	buf->f_bsize = PAGE_SIZE;
	buf->f_namelen = NAME_MAX;
	return 0;
}

static const super_ops_s simple_super_operations = {
	.statfs		= simple_statfs,
};

static int pseudo_fs_fill_super(super_block_s *s, fs_ctxt_s *fc)
{
	pseudo_fs_ctxt_s *ctx = fc->fs_private;
	inode_s *root;

	s->s_maxbytes = MAX_LFS_FILESIZE;
	s->s_blocksize = PAGE_SIZE;
	// s->s_blocksize_bits = PAGE_SHIFT;
	s->s_magic = ctx->magic;
	s->s_op = ctx->ops ?: &simple_super_operations;
	root = new_inode(s);
	if (!root)
		return -ENOMEM;

	/*
	 * since this is the first inode, make it number 1. New inodes created
	 * after this must take care not to collide with it (by passing
	 * max_reserved of 1 to iunique).
	 */
	// root->i_ino = 1;
	root->i_mode = S_IFDIR | S_IRUSR | S_IWUSR;
	// root->i_atime = root->i_mtime = root->i_ctime = current_time(root);
	s->s_root = d_make_root(root);
	if (!s->s_root)
		return -ENOMEM;
	s->s_d_op = ctx->dops;
	return 0;
}

static int pseudo_fs_get_tree(fs_ctxt_s *fc)
{
	return get_tree_nodev(fc, pseudo_fs_fill_super);
}

static void pseudo_fs_free(fs_ctxt_s *fc)
{
	kfree(fc->fs_private);
}

static const fs_ctxt_ops_s pseudo_fs_context_ops = {
	.free		= pseudo_fs_free,
	.get_tree	= pseudo_fs_get_tree,
};

/*
 * Common helper for pseudo-filesystems (sockfs, pipefs, bdev - stuff that
 * will never be mountable)
 */
pseudo_fs_ctxt_s *init_pseudo(fs_ctxt_s *fc, unsigned long magic)
{
	struct pseudo_fs_context *ctx;

	ctx = kmalloc(sizeof(pseudo_fs_ctxt_s));
	if (ctx) {
		ctx->magic = magic;
		fc->fs_private = ctx;
		fc->ops = &pseudo_fs_context_ops;
		fc->sb_flags |= SB_NOUSER;
		fc->global = true;
	}
	return ctx;
}

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
	if (dentry->d_sb->s_d_op == NULL)
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