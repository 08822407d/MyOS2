/* SPDX-License-Identifier: GPL-2.0-or-later */
/* fs/ internal definitions
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 */

#ifndef _LINUX_FS_INTERNELS_H
#define _LINUX_FS_INTERNELS_H

	struct dentry;
	typedef struct dentry dentry_s;
	struct dentry_ops;
	typedef struct dentry_ops dentry_ops_s;

	struct super_block;
	typedef struct super_block super_block_s;
	struct super_ops;
	typedef struct super_ops super_ops_s;

	struct mount;
	typedef struct mount mount_s;

	struct vfsmount;
	typedef struct vfsmount vfsmount_s;

	struct inode;
	typedef struct inode inode_s;
	struct inode_ops;
	typedef struct inode_ops inode_ops_s;

	struct fs_type;
	typedef struct fs_type fs_type_s;

	struct file;
	typedef struct file file_s;
	struct file_ops;
	typedef struct file_ops file_ops_s;

	struct path;
	typedef struct path path_s;

	struct qstr;
	typedef struct qstr qstr_s;

	struct filename;
	typedef struct filename filename_s;

	struct iattr;
	typedef struct iattr iattr_s;

	struct buffer_head;
	typedef struct buffer_head buffer_head_s;

	struct block_device;
	typedef struct block_device block_device_s;

#include <linux/kernel/types.h>

	// /*
	// * block_dev.c
	// */
	// #ifdef CONFIG_BLOCK
	// 	extern void __init bdev_cache_init(void);

	// 	extern int __sync_blockdev(block_device_s *bdev, int wait);
	// 	void iterate_bdevs(void (*)(block_device_s *, void *), void *);
	// 	void emergency_thaw_bdev(super_block_s *sb);
	// #else
	// 	static inline void bdev_cache_init(void)
	// 	{
	// 	}

	// 	static inline int __sync_blockdev(block_device_s *bdev, int wait)
	// 	{
	// 		return 0;
	// 	}
	// 	static inline void iterate_bdevs(void (*f)(block_device_s *, void *),
	// 			void *arg)
	// 	{
	// 	}
	// 	static inline int emergency_thaw_bdev(super_block_s *sb)
	// 	{
	// 		return 0;
	// 	}
	// #endif /* CONFIG_BLOCK */

	/*
	* buffer.c
	*/
	// extern int __block_write_begin_int(struct page *page, loff_t pos, unsigned len,
	// 		get_block_t *get_block, struct iomap *iomap);

	/*
	* char_dev.c
	*/
	extern void chrdev_init(void);

	/*
	* fs_context.c
	*/
	// extern const struct fs_context_operations legacy_fs_context_ops;
	// extern int parse_monolithic_mount_data(struct fs_context *, void *);
	// extern void vfs_clean_context(struct fs_context *fc);
	// extern int finish_clean_context(struct fs_context *fc);

	/*
	* namei.c
	*/
	extern int filename_lookup(int dfd, filename_s *name, unsigned flags,
					path_s *path, path_s *root);
	extern int vfs_path_lookup(dentry_s *, vfsmount_s *,
					const char *, unsigned int, path_s *);
	long do_rmdir(int dfd, filename_s *name);
	long do_unlinkat(int dfd, filename_s *name);
	// int may_linkat(struct user_namespace *mnt_userns, path_s *link);
	int do_renameat2(int olddfd, filename_s *oldname, int newdfd,
					filename_s *newname, unsigned int flags);

	/*
	* namespace.c
	*/
	extern vfsmount_s *lookup_mnt(const path_s *);
	extern int finish_automount(vfsmount_s *, path_s *);

	extern int sb_prepare_remount_readonly(super_block_s *);

	extern void mnt_init(void);

	extern int __mnt_want_write_file(file_s *);
	extern void __mnt_drop_write_file(file_s *);

	extern void dissolve_on_fput(vfsmount_s *);

	int path_mount(const char *dev_name, path_s *path, unsigned long flags);
	int path_umount(path_s *path, int flags);

	/*
	* fs_struct.c
	*/
	extern void chroot_fs_refs(const path_s *, const path_s *);

	/*
	* file_table.c
	*/
	extern file_s *alloc_empty_file(int);
	extern file_s *alloc_empty_file_noaccount(int);

	/*
	* super.c
	*/
	// extern int reconfigure_super(struct fs_context *);
	extern bool trylock_super(super_block_s *sb);
	super_block_s *user_get_super(dev_t, bool excl);
	void put_super(super_block_s *sb);
	// extern bool mount_capable(struct fs_context *);

	/*
	* open.c
	*/
	typedef struct open_flags {
		int open_flag;
		umode_t mode;
		int acc_mode;
		int intent;
		int lookup_flags;
	} open_flags_s;
	extern file_s *do_filp_open(int dfd, filename_s *pathname,
					const open_flags_s *op);
	extern file_s *do_file_open_root(const path_s *,
					const char *, const open_flags_s *);
	extern struct open_how build_open_how(int flags, umode_t mode);
	extern int build_open_flags(const struct open_how *how, open_flags_s *op);
	extern int __close_fd_get_file(unsigned int fd, file_s **res);

	// long do_sys_ftruncate(unsigned int fd, loff_t length, int small);
	int chmod_common(const path_s *path, umode_t mode);
	int do_fchownat(int dfd, const char *filename, uid_t user, gid_t group,
					int flag);
	int chown_common(const path_s *path, uid_t user, gid_t group);
	extern int vfs_open(const path_s *, file_s *);

	/*
	* inode.c
	*/
	// extern long prune_icache_sb(super_block_s *sb, struct shrink_control *sc);
	extern void inode_add_lru(inode_s *inode);
	extern int dentry_needs_remove_privs(dentry_s *dentry);

	/*
	* fs-writeback.c
	*/
	extern long get_nr_dirty_inodes(void);
	extern int invalidate_inodes(super_block_s *, bool);

	/*
	* dcache.c
	*/
	extern int d_set_mounted(dentry_s *dentry);
	// extern long prune_dcache_sb(super_block_s *sb, struct shrink_control *sc);
	extern dentry_s *d_alloc_cursor(dentry_s *);
	extern dentry_s * d_alloc_pseudo(super_block_s *, const qstr_s *);
	extern char *simple_dname(dentry_s *, char *, int);
	// extern void dput_to_list(dentry_s *, struct list_head *);
	// extern void shrink_dentry_list(struct list_head *);

	/*
	* read_write.c
	*/
	// extern int rw_verify_area(int, file_s *, const loff_t *, size_t);

	/*
	* pipe.c
	*/
	extern const file_ops_s pipefifo_fops;

	/*
	* fs_pin.c
	*/
	// extern void group_pin_kill(struct hlist_head *p);
	extern void mnt_pin_kill(struct mount *m);

	/*
	* fs/nsfs.c
	*/
	extern const dentry_ops_s ns_dentry_operations;

	/* direct-io.c: */
	int sb_init_dio_done_wq(super_block_s *sb);

	/*
	* fs/stat.c:
	*/
	// int do_statx(int dfd, const char *filename, unsigned flags,
	// 		unsigned int mask, struct statx *buffer);

	/*
	* fs/splice.c:
	*/
	// long splice_file_to_pipe(file_s *in,
	// 			struct pipe_inode_info *opipe,
	// 			loff_t *offset,
	// 			size_t len, unsigned int flags);

#endif /* _LINUX_FS_INTERNELS_H */