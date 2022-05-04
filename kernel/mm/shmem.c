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
// #include <linux/init.h>
#include <linux/kernel/statfs.h>
#include <linux/kernel/mount.h>
// #include <linux/ramfs.h>
// #include <linux/pagemap.h>
#include <linux/fs/file.h>
// #include <linux/mm.h>
// #include <linux/random.h>
// #include <linux/sched/signal.h>
// #include <linux/export.h>
// #include <linux/swap.h>
// #include <linux/uio.h>
// #include <linux/khugepaged.h>
// #include <linux/hugetlb.h>
#include <linux/fs/fs_parser.h>
// #include <linux/swapfile.h>


#include <uapi/magic.h>

static vfsmount_s *shm_mnt;

/*
 * This virtual memory filesystem is heavily based on the ramfs. It
 * extends ramfs by the ability to use swap and honor resource limits
 * which makes it a completely usable filesystem.
 */

// #include <linux/xattr.h>
// #include <linux/exportfs.h>
// #include <linux/posix_acl.h>
// #include <linux/posix_acl_xattr.h>
// #include <linux/mman.h>
// #include <linux/string.h>
// #include <linux/slab.h>
// #include <linux/backing-dev.h>
#include <linux/mm/shmem_fs.h>
// #include <linux/writeback.h>
// #include <linux/pagevec.h>
// #include <linux/percpu_counter.h>
// #include <linux/falloc.h>
// #include <linux/splice.h>
// #include <linux/security.h>
// #include <linux/swapops.h>
// #include <linux/mempolicy.h>
#include <linux/fs/namei.h>
// #include <linux/ctype.h>
// #include <linux/migrate.h>
// #include <linux/highmem.h>
// #include <linux/seq_file.h>
// #include <linux/magic.h>
// #include <linux/syscalls.h>
#include <linux/kernel/fcntl.h>
// #include <uapi/linux/memfd.h>
// #include <linux/userfaultfd_k.h>
// #include <linux/rmap.h>
#include <linux/kernel/uuid.h>
// #include <linux/uaccess.h>
#include <linux/mm/internal.h>


#include <linux/kernel/err.h>
#include <include/proto.h>

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
shmem_mknod(inode_s *dir, dentry_s *dentry,
				umode_t mode, dev_t dev)
{
// 	inode_s *inode;
// 	int error = -ENOSPC;

// 	inode = shmem_get_inode(dir->i_sb, dir, mode, dev, VM_NORESERVE);
// 	if (inode) {
// 		error = simple_acl_create(dir, inode);
// 		if (error)
// 			goto out_iput;
// 		error = security_inode_init_security(inode, dir,
// 						     &dentry->d_name,
// 						     shmem_initxattrs, NULL);
// 		if (error && error != -EOPNOTSUPP)
// 			goto out_iput;

// 		error = 0;
// 		dir->i_size += BOGO_DIRENT_SIZE;
// 		dir->i_ctime = dir->i_mtime = current_time(dir);
// 		d_instantiate(dentry, inode);
// 		dget(dentry); /* Extra count - pin the dentry in core */
// 	}
// 	return error;
// out_iput:
// 	iput(inode);
// 	return error;
}

// static int
// shmem_tmpfile(struct user_namespace *mnt_userns, inode_s *dir,
// 	      dentry_s *dentry, umode_t mode)
// {
// 	inode_s *inode;
// 	int error = -ENOSPC;

// 	inode = shmem_get_inode(dir->i_sb, dir, mode, 0, VM_NORESERVE);
// 	if (inode) {
// 		error = security_inode_init_security(inode, dir,
// 						     NULL,
// 						     shmem_initxattrs, NULL);
// 		if (error && error != -EOPNOTSUPP)
// 			goto out_iput;
// 		error = simple_acl_create(dir, inode);
// 		if (error)
// 			goto out_iput;
// 		d_tmpfile(dentry, inode);
// 	}
// 	return error;
// out_iput:
// 	iput(inode);
// 	return error;
// }

static int shmem_mkdir(inode_s *dir, dentry_s *dentry,
				umode_t mode)
{
	// int error;

	// if ((error = shmem_mknod(&init_user_ns, dir, dentry,
	// 			 mode | S_IFDIR, 0)))
	// 	return error;
	// inc_nlink(dir);
	// return 0;
}

static int shmem_create(inode_s *dir, dentry_s *dentry,
				umode_t mode, bool excl)
{
	// return shmem_mknod(&init_user_ns, dir, dentry, mode | S_IFREG, 0);
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
				inode_s *new_dir, dentry_s *new_dentry,
				unsigned int flags)
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
	sbinfo = kmalloc(sizeof(shmem_sb_info_s));
	if (!sbinfo)
		return -ENOMEM;

	sbinfo->uid = KUIDT_INIT(0);
	sbinfo->gid = KGIDT_INIT(0);
	sb->s_fs_info = sbinfo;
	sb->s_flags |= SB_NOUSER;
	sb->s_flags |= SB_NOSEC;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = PAGE_SIZE;
	sb->s_magic = TMPFS_MAGIC;
	sb->s_op = &shmem_ops;

	inode = shmem_get_inode(sb, NULL, S_IFDIR | sbinfo->mode, 0, VM_NORESERVE);
	if (!inode)
		goto failed;
	inode->i_uid = sbinfo->uid;
	inode->i_gid = sbinfo->gid;
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
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
// 	.free			= shmem_free_fc,
	.get_tree		= shmem_get_tree,
// #ifdef CONFIG_TMPFS
// 	.parse_monolithic	= shmem_parse_options,
// 	.parse_param		= shmem_parse_one,
// 	.reconfigure		= shmem_reconfigure,
// #endif
};

// const struct address_space_operations shmem_aops = {
// 	.writepage	= shmem_writepage,
// 	.set_page_dirty	= __set_page_dirty_no_writeback,
// #ifdef CONFIG_TMPFS
// 	.write_begin	= shmem_write_begin,
// 	.write_end	= shmem_write_end,
// #endif
// #ifdef CONFIG_MIGRATION
// 	.migratepage	= migrate_page,
// #endif
// 	.error_remove_page = shmem_error_remove_page,
// };

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
// 	.alloc_inode	= shmem_alloc_inode,
// 	.free_inode	= shmem_free_in_core_inode,
// 	.destroy_inode	= shmem_destroy_inode,
// #ifdef CONFIG_TMPFS
// 	.statfs		= shmem_statfs,
// 	.show_options	= shmem_show_options,
// #endif
// 	.evict_inode	= shmem_evict_inode,
// 	.drop_inode	= generic_delete_inode,
// 	.put_super	= shmem_put_super,
// #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// 	.nr_cached_objects	= shmem_unused_huge_count,
// 	.free_cached_objects	= shmem_unused_huge_scan,
// #endif
};

// static const struct vm_operations_struct shmem_vm_ops = {
// 	.fault		= shmem_fault,
// 	.map_pages	= filemap_map_pages,
// #ifdef CONFIG_NUMA
// 	.set_policy     = shmem_set_policy,
// 	.get_policy     = shmem_get_policy,
// #endif
// };

static const file_ops_s shmem_file_operations = {
// 	.mmap				= shmem_mmap,
// 	.get_unmapped_area	= shmem_get_unmapped_area,
// #ifdef CONFIG_TMPFS
// 	.llseek				= shmem_file_llseek,
// 	.read_iter			= shmem_file_read_iter,
// 	.write_iter			= generic_file_write_iter,
// 	.fsync				= noop_fsync,
// 	.splice_read		= generic_file_splice_read,
// 	.splice_write		= iter_file_splice_write,
// 	.fallocate			= shmem_fallocate,
// #endif
};

static const inode_ops_s shmem_inode_operations = {
// 	.getattr	= shmem_getattr,
// 	.setattr	= shmem_setattr,
// #ifdef CONFIG_TMPFS_XATTR
// 	.listxattr	= shmem_listxattr,
// 	.set_acl	= simple_set_acl,
// #endif
};

static const inode_ops_s shmem_dir_inode_operations = {
// #ifdef CONFIG_TMPFS
	.create		= shmem_create,
	.lookup		= simple_lookup,
// 	.link		= shmem_link,
// 	.unlink		= shmem_unlink,
// 	.symlink	= shmem_symlink,
	.mkdir		= shmem_mkdir,
	.rmdir		= shmem_rmdir,
	.mknod		= shmem_mknod,
	.rename		= shmem_rename2,
// 	.tmpfile	= shmem_tmpfile,
// #endif
// #ifdef CONFIG_TMPFS_XATTR
// 	.listxattr	= shmem_listxattr,
// #endif
// #ifdef CONFIG_TMPFS_POSIX_ACL
// 	.setattr	= shmem_setattr,
// 	.set_acl	= simple_set_acl,
// #endif
};

int shmem_init_fs_context(fs_ctxt_s *fc)
{
	shmem_opts_s *ctx;

	ctx = kmalloc(sizeof(shmem_opts_s));
	if (!ctx)
		return -ENOMEM;

	ctx->mode = 0777 | S_ISVTX;
	// ctx->uid = current_fsuid();
	// ctx->gid = current_fsgid();

	fc->fs_private = ctx;
	fc->ops = &shmem_fs_context_ops;
	return 0;
}

static fs_type_s shmem_fs_type = {
// 	.owner		= THIS_MODULE,
	.name		= "tmpfs",
	.init_fs_context = shmem_init_fs_context,
// #ifdef CONFIG_TMPFS
// 	.parameters	= shmem_fs_parameters,
// #endif
// 	.kill_sb	= kill_litter_super,
// 	.fs_flags	= FS_USERNS_MOUNT,
};


int tmp_shmemfs_mount(void);
int shmem_init(void)
{
	int error;

	error = register_filesystem(&shmem_fs_type);
	if (error) {
		goto out2;
	}

	// tmp_shmemfs_mount();

	shm_mnt = kern_mount(&shmem_fs_type);
	if (IS_ERR(shm_mnt)) {
		error = PTR_ERR(shm_mnt);
		goto out1;
	}

// #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// 	if (has_transparent_hugepage() && shmem_huge > SHMEM_HUGE_DENY)
// 		SHMEM_SB(shm_mnt->mnt_sb)->huge = shmem_huge;
// 	else
// 		shmem_huge = SHMEM_HUGE_NEVER; /* just in case it was patched */
// #endif
// 	return 0;

out1:
	unregister_filesystem(&shmem_fs_type);
out2:
	// shmem_destroy_inodecache();
	shm_mnt = ERR_PTR(error);
	return error;
}

int tmp_shmemfs_mount()
{
	inode_s *inode;
	super_block_s *sb;
	shmem_sb_info_s *sbinfo;

	/* Round up to L1_CACHE_BYTES to resist false sharing */
	sb = kmalloc(sizeof(super_block_s));
	if (!sb)
		return -ENOMEM;
	sbinfo = kmalloc(sizeof(shmem_sb_info_s));
	if (!sbinfo)
		return -ENOMEM;

	sbinfo->uid = KUIDT_INIT(0);
	sbinfo->gid = KGIDT_INIT(0);
	sb->s_fs_info = sbinfo;
	sb->s_flags |= SB_NOUSER;
	sb->s_flags |= SB_NOSEC;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = PAGE_SIZE;
	sb->s_magic = TMPFS_MAGIC;
	sb->s_op = &shmem_ops;

	inode = shmem_get_inode(sb, NULL, S_IFDIR | sbinfo->mode, 0, VM_NORESERVE);
	if (!inode)
		goto failed;
	inode->i_uid = sbinfo->uid;
	inode->i_gid = sbinfo->gid;
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
		goto failed;
	return 0;

failed:
	shmem_put_super(sb);
	return -ENOMEM;
}