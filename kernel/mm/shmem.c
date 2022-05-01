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
// #include <linux/fs_parser.h>
// #include <linux/swapfile.h>

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
// 	.create		= shmem_create,
// 	.lookup		= simple_lookup,
// 	.link		= shmem_link,
// 	.unlink		= shmem_unlink,
// 	.symlink	= shmem_symlink,
// 	.mkdir		= shmem_mkdir,
// 	.rmdir		= shmem_rmdir,
// 	.mknod		= shmem_mknod,
// 	.rename		= shmem_rename2,
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

static fs_type_s shmem_fs_type = {
// 	.owner		= THIS_MODULE,
// 	.name		= "tmpfs",
// 	.init_fs_context = shmem_init_fs_context,
// #ifdef CONFIG_TMPFS
// 	.parameters	= shmem_fs_parameters,
// #endif
// 	.kill_sb	= kill_litter_super,
// 	.fs_flags	= FS_USERNS_MOUNT,
};

int shmem_init(void)
{
	int error;

	error = register_filesystem(&shmem_fs_type);
	if (error) {
		goto out2;
	}

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