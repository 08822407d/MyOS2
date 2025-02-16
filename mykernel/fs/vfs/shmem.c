/*
 * Resizable virtual memory filesystem for Linux.
 *
 * Copyright (C) 2000 Linus Torvalds.
 *		 2000 Transmeta Corp.
 *		 2000-2001 Christoph Rohland
 *		 2000-2001 SAP AG
 *		 2002 Red Hat Inc.
 * Copyright (C) 2002-2011 Hugh Dickins.
 * Copyright (C) 2011 Google Inc.
 * Copyright (C) 2002-2005 VERITAS Software Corporation.
 * Copyright (C) 2004 Andi Kleen, SuSE Labs
 *
 * Extended attribute support for tmpfs:
 * Copyright (c) 2004, Luke Kenneth Casson Leighton <lkcl@lkcl.net>
 * Copyright (c) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *
 * tiny-shmem:
 * Copyright (c) 2004, 2008 Matt Mackall <mpm@selenic.com>
 *
 * This file is released under the GPL.
 */

#include <linux/fs/fs.h>
#include <linux/init/init.h>
#include <linux/kernel/statfs.h>
#include <linux/kernel/mount.h>
// #include <linux/ramfs.h>
// #include <linux/mm/pagemap.h>
#include <linux/fs/file.h>
#include <linux/kernel/mm.h>
// #include <linux/random.h>
#include <linux/sched/signal.h>
#include <linux/kernel/export.h>
// #include <linux/swap.h>
// #include <linux/uio.h>
// #include <linux/khugepaged.h>
// #include <linux/hugetlb.h>
#include <linux/fs/fs_parser.h>
// #include <linux/swapfile.h>


#include <uapi/linux/magic.h>

static vfsmount_s *shm_mnt;

/*
 * This virtual memory filesystem is heavily based on the ramfs. It
 * extends ramfs by the ability to use swap and honor resource limits
 * which makes it a completely usable filesystem.
 */

#include <linux/lib/string.h>
#include <linux/fs/shmem_fs.h>
#include <linux/fs/namei.h>
#include <linux/kernel/ctype.h>
#include <linux/kernel/syscalls.h>
#include <linux/kernel/fcntl.h>
#include <linux/kernel/uuid.h>
#include <linux/kernel/uaccess.h>


#define BLOCKS_PER_PAGE  (PAGE_SIZE/512)
#define VM_ACCT(size)    (PAGE_ALIGN(size) >> PAGE_SHIFT)

/* Pretend that each entry is of this size in directory's i_size */
#define BOGO_DIRENT_SIZE 20

/* Symlink up to this size is kmalloc'ed instead of using a swappable page */
#define SHORT_SYMLINK_LEN 128

/*
 * shmem_fallocate communicates with shmem_fault or shmem_writepage via
 * inode->i_private (with i_rwsem making sure that it has only one user at
 * a time): we would prefer not to enlarge the shmem inode just for that.
 */
typedef struct shmem_falloc {
	pgoff_t		start;		/* start of range currently being fallocated */
	pgoff_t		next;		/* the next page offset to be fallocated */
	pgoff_t		nr_falloced;	/* how many new pages have been fallocated */
	pgoff_t		nr_unswapped;	/* how often writepage refused to swap out */
} shmem_falloc_s;

typedef struct shmem_options {
	unsigned long long	blocks;
	unsigned long long	inodes;
	kuid_t	uid;
	kgid_t	gid;
	umode_t	mode;
	bool	full_inums;
	int		huge;
	int		seen;
#define SHMEM_SEEN_BLOCKS 1
#define SHMEM_SEEN_INODES 2
#define SHMEM_SEEN_HUGE 4
#define SHMEM_SEEN_INUMS 8
} shmem_opts_s;

static kmem_cache_s *shmem_inode_cachep;

static inline shmem_sb_info_s *SHMEM_SB(super_block_s *sb)
{
	return sb->s_fs_info;
}

static const super_ops_s	shmem_ops;
static const file_ops_s		shmem_file_operations;
static const inode_ops_s	shmem_inode_operations;
static const inode_ops_s	shmem_dir_inode_operations;
static const inode_ops_s	shmem_special_inode_operations;
static fs_type_s			shmem_fs_type;


static void shmem_free_inode(super_block_s *sb)
{
	shmem_sb_info_s *sbinfo = SHMEM_SB(sb);
	if (sbinfo->max_inodes) {
		sbinfo->free_inodes++;
	}
}

static inode_s *shmem_get_inode(super_block_s *sb, const inode_s *dir,
		umode_t mode, dev_t dev, unsigned long flags)
{
	inode_s *inode;
	shmem_inode_info_s *info;
	shmem_sb_info_s *sbinfo = SHMEM_SB(sb);

	inode = new_inode(sb);
	if (inode) {
		inode->i_mode = mode;
		inode->i_blocks = 0;
		info = SHMEM_I(inode);
		memset(info, 0, (char *)inode - (char *)info);
		info->seals = F_SEAL_SEAL;
		info->flags = flags & VM_NORESERVE;

		switch (mode & S_IFMT) {
		default:
			inode->i_op = &shmem_special_inode_operations;
			init_special_inode(inode, mode, dev);
			break;
		case S_IFREG:
			inode->i_op = &shmem_inode_operations;
			inode->i_fop = &shmem_file_operations;
			break;
		case S_IFDIR:
			/* Some things misbehave if size == 0 on a directory */
			inode->i_size = 2 * BOGO_DIRENT_SIZE;
			inode->i_op = &shmem_dir_inode_operations;
			inode->i_fop = &simple_dir_operations;
			break;
		}
	} else
		shmem_free_inode(sb);
	return inode;
}

static int shmem_statfs(dentry_s *dentry, kstatfs_s *buf)
{
	shmem_sb_info_s *sbinfo = SHMEM_SB(dentry->d_sb);

	buf->f_type = TMPFS_MAGIC;
	buf->f_bsize = PAGE_SIZE;
	buf->f_namelen = NAME_MAX;
	if (sbinfo->max_blocks) {
		buf->f_blocks = sbinfo->max_blocks;
	}
	if (sbinfo->max_inodes) {
		buf->f_files = sbinfo->max_inodes;
		buf->f_ffree = sbinfo->free_inodes;
	}
	/* else leave those fields 0 like simple_statfs */

	return 0;
}

/*
 * File creation. Allocate an inode, and we're done..
 */
static int
shmem_mknod(inode_s *dir, dentry_s *dentry, umode_t mode, dev_t dev)
{
	inode_s *inode;
	int error = -ENOSPC;

	inode = shmem_get_inode(dir->i_sb, dir, mode, dev, VM_NORESERVE);
	if (inode) {
		error = 0;
		dir->i_size += BOGO_DIRENT_SIZE;
		// dir->i_ctime = dir->i_mtime = current_time(dir);
		d_instantiate(dentry, inode);
		dget(dentry); /* Extra count - pin the dentry in core */
	}
	return error;
out_iput:
	iput(inode);
	return error;
}

static int shmem_mkdir(inode_s *dir, dentry_s *dentry, umode_t mode)
{
	int error;
	error = shmem_mknod(dir, dentry, mode | S_IFDIR, 0);
	return error;
}

static int shmem_create(inode_s *dir, dentry_s *dentry, umode_t mode)
{
	return shmem_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int shmem_rmdir(inode_s *dir, dentry_s *dentry)
{
	// if (!simple_empty(dentry))
	// 	return -ENOTEMPTY;

	// drop_nlink(d_inode(dentry));
	// drop_nlink(dir);
	// return shmem_unlink(dir, dentry);
}

/*
 * The VFS layer already does all the dentry stuff for rename,
 * we just have to decrement the usage count for the target if
 * it exists so that the VFS layer correctly free's it when it
 * gets overwritten.
 */
static int shmem_rename2(inode_s *old_dir, dentry_s *old_dentry,
		inode_s *new_dir, dentry_s *new_dentry, unsigned int flags)
{
	// inode_s *inode = d_inode(old_dentry);
	// int they_are_dirs = S_ISDIR(inode->i_mode);

	// if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE | RENAME_WHITEOUT))
	// 	return -EINVAL;

	// if (flags & RENAME_EXCHANGE)
	// 	return simple_rename_exchange(old_dir, old_dentry, new_dir, new_dentry);

	// if (!simple_empty(new_dentry))
	// 	return -ENOTEMPTY;

	// if (flags & RENAME_WHITEOUT) {
	// 	int error;

	// 	error = shmem_whiteout(&init_user_ns, old_dir, old_dentry);
	// 	if (error)
	// 		return error;
	// }

	// if (d_really_is_positive(new_dentry)) {
	// 	(void) shmem_unlink(new_dir, new_dentry);
	// 	if (they_are_dirs) {
	// 		drop_nlink(d_inode(new_dentry));
	// 		drop_nlink(old_dir);
	// 	}
	// } else if (they_are_dirs) {
	// 	drop_nlink(old_dir);
	// 	inc_nlink(new_dir);
	// }

	// old_dir->i_size -= BOGO_DIRENT_SIZE;
	// new_dir->i_size += BOGO_DIRENT_SIZE;
	// old_dir->i_ctime = old_dir->i_mtime =
	// new_dir->i_ctime = new_dir->i_mtime =
	// inode->i_ctime = current_time(old_dir);
	// return 0;
}

static void shmem_put_super(super_block_s *sb)
{
	shmem_sb_info_s *sbinfo = SHMEM_SB(sb);
	kfree(sbinfo);
	sb->s_fs_info = NULL;
}

static int shmem_fill_super(super_block_s *sb, fs_ctxt_s *fc)
{
	inode_s *inode;
	shmem_sb_info_s *sbinfo;

	/* Round up to L1_CACHE_BYTES to resist false sharing */
	sbinfo = kzalloc(sizeof(shmem_sb_info_s), GFP_KERNEL);
	if (sbinfo == NULL)
		return -ENOMEM;

	sb->s_fs_info = sbinfo;
	sb->s_flags |= SB_NOUSER;
	sb->s_flags |= SB_NOSEC;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = PAGE_SIZE;
	sb->s_magic = TMPFS_MAGIC;
	sb->s_op = &shmem_ops;

	inode = shmem_get_inode(sb, NULL, S_IFDIR | sbinfo->mode, 0, VM_NORESERVE);
	if (inode == NULL)
		goto failed;
	sb->s_root = d_make_root(inode);
	if (sb->s_root == NULL)
		goto failed;
	return 0;

failed:
	shmem_put_super(sb);
	return -ENOMEM;
}

static int shmem_get_tree(fs_ctxt_s *fc)
{
	return get_tree_nodev(fc, shmem_fill_super);
}

static void shmem_free_fc(fs_ctxt_s *fc)
{
	// struct shmem_options *ctx = fc->fs_private;

	// if (ctx) {
	// 	mpol_put(ctx->mpol);
	// 	kfree(ctx);
	// }
}

static const fs_ctxt_ops_s shmem_fs_context_ops = {
	.free				= shmem_free_fc,
	.get_tree			= shmem_get_tree,
	// .parse_monolithic	= shmem_parse_options,
	// .parse_param		= shmem_parse_one,
	// .reconfigure		= shmem_reconfigure,
};

static inode_s *shmem_alloc_inode(super_block_s *sb) {
	shmem_inode_info_s *info;
	info = kmem_cache_alloc(shmem_inode_cachep, GFP_KERNEL);
	if (info == NULL)
		return ERR_PTR(-ENOMEM);
	
	inode_init_once(&info->vfs_inode);
	return &info->vfs_inode;
}

static void shmem_free_in_core_inode(inode_s *inode) {
	// if (S_ISLNK(inode->i_mode))
	// 	kfree(inode->i_link);
	kmem_cache_free(shmem_inode_cachep, SHMEM_I(inode));
}

static void shmem_destroy_inode(inode_s *inode)
{
	// if (S_ISREG(inode->i_mode))
	// 	mpol_free_shared_policy(&SHMEM_I(inode)->policy);
}

static void shmem_init_inode(void *foo)
{
	shmem_inode_info_s *info = foo;
	inode_init_once(&info->vfs_inode);
}

static void shmem_init_inodecache(void)
{
	shmem_inode_cachep =
			kmem_cache_create("shmem_inode_cache",
				sizeof(shmem_inode_info_s), 0,
				SLAB_PANIC|SLAB_ACCOUNT,
				shmem_init_inode);
}

static const file_ops_s shmem_file_operations = {
	// .mmap				= shmem_mmap,
	// .get_unmapped_area	= shmem_get_unmapped_area,
	// .llseek				= shmem_file_llseek,
	// .read_iter			= shmem_file_read_iter,
	// .write_iter			= generic_file_write_iter,
	// .fsync				= noop_fsync,
	// .splice_read		= generic_file_splice_read,
	// .splice_write		= iter_file_splice_write,
	// .fallocate			= shmem_fallocate,
};

static const inode_ops_s shmem_inode_operations = {
	// .getattr	= shmem_getattr,
	// .setattr	= shmem_setattr,
};

static const inode_ops_s shmem_dir_inode_operations = {
	.create		= shmem_create,
	.lookup		= simple_lookup,
	.mkdir		= shmem_mkdir,
	.rmdir		= shmem_rmdir,
	.mknod		= shmem_mknod,
	.rename		= shmem_rename2,
	// .tmpfile	= shmem_tmpfile,
};

static const inode_ops_s shmem_special_inode_operations = {
// #ifdef CONFIG_TMPFS_XATTR
// 	.listxattr	= shmem_listxattr,
// #endif
// #ifdef CONFIG_TMPFS_POSIX_ACL
// 	.setattr	= shmem_setattr,
// 	.set_acl	= simple_set_acl,
// #endif
};

static const super_ops_s shmem_ops = {
	.alloc_inode	= shmem_alloc_inode,
	// .free_inode		= shmem_free_in_core_inode,
	.destroy_inode	= shmem_destroy_inode,
	.statfs			= shmem_statfs,
	// .show_options	= shmem_show_options,
	// .evict_inode	= shmem_evict_inode,
	// .drop_inode		= generic_delete_inode,
	.put_super		= shmem_put_super,
};

int shmem_init_fs_context(fs_ctxt_s *fc)
{
	shmem_opts_s *ctx;

	ctx = kzalloc(sizeof(shmem_opts_s), GFP_KERNEL);
	if (ctx == NULL)
		return -ENOMEM;

	ctx->mode = 0777 | S_ISVTX;
	fc->fs_private = ctx;
	fc->ops = &shmem_fs_context_ops;
	return 0;
}

static fs_type_s shmem_fs_type = {
	// .owner		= THIS_MODULE,
	.name		= "tmpfs",
	.init_fs_context = shmem_init_fs_context,
	.kill_sb	= kill_litter_super,
	.fs_flags	= FS_USERNS_MOUNT,
};


int __init shmem_init(void)
{
	int error;
	shmem_init_inodecache();

	BUG_ON(register_filesystem(&shmem_fs_type) != 0);

	shm_mnt = kern_mount(&shmem_fs_type);
	BUG_ON(IS_ERR(shm_mnt));

	return 0;
}


/**
 * shmem_zero_setup - setup a shared anonymous mapping
 * @vma: the vma to be mmapped is prepared by do_mmap
 */
int shmem_zero_setup(vma_s *vma)
{
	file_s *file;
	loff_t size = vma->vm_end - vma->vm_start;

	// /*
	//  * Cloning a new file under mmap_lock leads to a lock ordering conflict
	//  * between XFS directory reading and selinux: since this file is only
	//  * accessible to the user through its mapping, use S_PRIVATE flag to
	//  * bypass file security, in the same way as shmem_kernel_file_setup().
	//  */
	// file = shmem_kernel_file_setup("dev/zero", size, vma->vm_flags);
	// if (IS_ERR(file))
	// 	return PTR_ERR(file);

	// if (vma->vm_file)
	// 	fput(vma->vm_file);
	// vma->vm_file = file;
	// vma->vm_ops = &shmem_anon_vm_ops;

	return 0;
}