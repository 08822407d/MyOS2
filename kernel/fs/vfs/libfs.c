// SPDX-License-Identifier: GPL-2.0-only
/*
 *	fs/libfs.c
 *	Library for filesystems writers.
 */

#include <linux/block/blkdev.h>
// #include <linux/export.h>
// #include <linux/pagemap.h>
#include <linux/kernel/slab.h>
#include <linux/kernel/cred.h>
#include <linux/kernel/mount.h>
// #include <linux/vfs.h>
// #include <linux/quotaops.h>
#include <linux/kernel/mutex.h>
#include <linux/fs/namei.h>
// #include <linux/exportfs.h>
// #include <linux/writeback.h>
// #include <linux/block/buffer_head.h> /* sync_mapping_buffers */
#include <linux/fs/fs_context.h>
#include <linux/fs/pseudo_fs.h>
// #include <linux/fsnotify.h>
// #include <linux/unicode.h>
// #include <linux/fscrypt.h>
// #include <linux/uaccess.h>
#include "internal.h"


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

	ctx = kzalloc(sizeof(pseudo_fs_ctxt_s), GFP_KERNEL);
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

int dcache_dir_open(inode_s *inode, file_s *file)
{
	file->private_data = d_alloc_cursor(file->f_path.dentry);

	return file->private_data ? 0 : -ENOMEM;
}

int dcache_dir_close(inode_s *inode, file_s *file)
{
	dput(file->private_data);
	return 0;
}

/* parent is locked at least shared */
/*
 * Returns an element of siblings' list.
 * We are looking for <count>th positive after <p>; if
 * found, dentry is grabbed and returned to caller.
 * If no such element exists, NULL is returned.
 */
// static struct dentry *scan_positives(struct dentry *cursor,
//			struct list_head *p, loff_t count, struct dentry *last)
static dentry_s *scan_positives(dentry_s *cursor, List_s *p)
{
	dentry_s	*dentry = cursor->d_parent,
				*found = NULL;
	List_s *lp = p;
	while ((lp = lp->next) != &(dentry->d_subdirs.header)) {
		dentry_s *d = container_of(lp, dentry_s, d_child);
		// we must at least skip cursors, to avoid livelocks
		if (d->d_flags & DCACHE_DENTRY_CURSOR)
			continue;
		if (d != NULL) {
			found = d;
			break;
		}
	}

	return found;
}

loff_t dcache_dir_lseek(file_s *file, loff_t offset, int whence)
{
	// struct dentry *dentry = file->f_path.dentry;
	// switch (whence) {
	// 	case 1:
	// 		offset += file->f_pos;
	// 		fallthrough;
	// 	case 0:
	// 		if (offset >= 0)
	// 			break;
	// 		fallthrough;
	// 	default:
	// 		return -EINVAL;
	// }
	// if (offset != file->f_pos) {
	// 	struct dentry *cursor = file->private_data;
	// 	struct dentry *to = NULL;

	// 	inode_lock_shared(dentry->d_inode);

	// 	if (offset > 2)
	// 		to = scan_positives(cursor, &dentry->d_subdirs,
	// 				    offset - 2, NULL);
	// 	spin_lock(&dentry->d_lock);
	// 	if (to)
	// 		list_move(&cursor->d_child, &to->d_child);
	// 	else
	// 		list_del_init(&cursor->d_child);
	// 	spin_unlock(&dentry->d_lock);
	// 	dput(to);

	// 	file->f_pos = offset;

	// 	inode_unlock_shared(dentry->d_inode);
	// }
	// return offset;
}

/* Relationship between i_mode and the DT_xxx types */
static inline unsigned char dt_type(inode_s *inode)
{
	return (inode->i_mode >> 12) & 15;
}

/*
 * Directory is locked and all positive dentries in it are safe, since
 * for ramfs-type trees they can't go away without unlink() or rmdir(),
 * both impossible due to the lock on directory.
 */
int dcache_readdir(file_s *file, dir_ctxt_s *ctx)
{
	dentry_s *cursor = file->private_data;
	dentry_s *next = NULL;
	List_s *p;

	if (!dir_emit_dots(file, ctx))
		return 0;

	if (ctx->pos == 2)
		p = &file->f_path.dentry->d_subdirs.header;
	else if (!list_empty(&cursor->d_child))
		p = &cursor->d_child;
	else
		return 0;

	while ((next = scan_positives(cursor, p)) != NULL) {
		if (ctx->actor(ctx, next->d_name.name, next->d_name.len,
					ctx->pos, 0, 0) != 0)
			break;
		ctx->pos++;
		p = &next->d_child;
	}
	if (next)
		list_insert_prev(&next->d_child, &cursor->d_child);
	else
		list_delete(&cursor->d_child);
	dput(next);

	return 0;
}

ssize_t generic_read_dir(file_s *filp, char *buf, size_t siz, loff_t *ppos)
{
	return -EISDIR;
}

const file_ops_s simple_dir_operations = {
	.open			= dcache_dir_open,
	.release		= dcache_dir_close,
	.llseek			= dcache_dir_lseek,
	.read			= generic_read_dir,
	.iterate_shared	= dcache_readdir,
	// .fsync		= noop_fsync,
};

const inode_ops_s simple_dir_inode_operations = {
	.lookup		= simple_lookup,
};