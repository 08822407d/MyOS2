/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Filesystem superblock creation and reconfiguration context.
 *
 * Copyright (C) 2018 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 */

#ifndef _LINUX_FS_CONTEXT_H
#define _LINUX_FS_CONTEXT_H

	#include <linux/kernel/kernel.h>
	// #include <linux/refcount.h>
	#include <linux/lib/errno.h>
	// #include <linux/security.h>
	// #include <linux/kernel/mutex.h>


	#include <linux/fs/vfs_s_defs.h>

	enum fs_context_purpose {
		FS_CONTEXT_FOR_MOUNT,		/* New superblock for explicit mount */
		FS_CONTEXT_FOR_SUBMOUNT,	/* New superblock for automatic submount */
		FS_CONTEXT_FOR_RECONFIGURE,	/* Superblock reconfiguration (remount) */
	};

	/*
	 * Userspace usage phase for fsopen/fspick.
	 */
	enum fs_context_phase {
		FS_CONTEXT_CREATE_PARAMS,	/* Loading params for sb creation */
		FS_CONTEXT_CREATING,		/* A superblock is being created */
		FS_CONTEXT_AWAITING_MOUNT,	/* Superblock created, awaiting fsmount() */
		FS_CONTEXT_AWAITING_RECONF,	/* Awaiting initialisation for reconfiguration */
		FS_CONTEXT_RECONF_PARAMS,	/* Loading params for reconfiguration */
		FS_CONTEXT_RECONFIGURING,	/* Reconfiguring the superblock */
		FS_CONTEXT_FAILED,		/* Failed to correctly transition a context */
	};

	/*
	 * Type of parameter value.
	 */
	enum fs_value_type {
		fs_value_is_undefined,
		fs_value_is_flag,		/* Value not given a value */
		fs_value_is_string,		/* Value is a string */
		fs_value_is_blob,		/* Value is a binary blob */
		fs_value_is_filename,		/* Value is a filename* + dirfd */
		fs_value_is_file,		/* Value is a file* */
	};

	/*
	 * Configuration parameter.
	 */
	typedef struct fs_parameter {
		const char		*key;		/* Parameter name */
		enum fs_value_type	type:8;		/* The type of value here */
		union {
			char		*string;
			void		*blob;
			filename_s	*name;
			file_s		*file;
		};
		size_t	size;
		int	dirfd;
	} fs_param_s;

	// typedef struct p_log {
	// 	const char *prefix;
	// 	struct fc_log *log;
	// } p_log_s;

	/*
	 * Filesystem context for holding the parameters used in the creation or
	 * reconfiguration of a superblock.
	 *
	 * Superblock creation fills in ->root whereas reconfiguration begins with this
	 * already set.
	 *
	 * See Documentation/filesystems/mount_api.rst
	 */
	typedef struct fs_context {
		const fs_ctxt_ops_s	*ops;
		// struct mutex	uapi_mutex;		/* Userspace access mutex */
		fs_type_s		*fs_type;
		void			*fs_private;	/* The filesystem's context */
		void			*sget_key;
		dentry_s		*root;			/* The root and superblock */
		// struct user_namespace	*user_ns;	/* The user namespace for this mount */
		// struct net		*net_ns;		/* The network namespace for this mount */
		// const struct cred	*cred;		/* The mounter's credentials */
		// p_log_s	log;			/* Logging buffer */
		const char		*source;		/* The source name (eg. dev path) */
		void			*security;		/* Linux S&M options */
		void			*s_fs_info;		/* Proposed s_fs_info */
		unsigned int	sb_flags;		/* Proposed superblock flags (SB_*) */
		unsigned int	sb_flags_mask;	/* Superblock flags that were changed */
		unsigned int	s_iflags;		/* OR'd with sb->s_iflags */
		unsigned int	lsm_flags;		/* Information flags from the fs to the LSM */
		enum fs_context_purpose	purpose:8;
		enum fs_context_phase	phase:8;	/* The phase the context is in */
		bool			need_free:1;	/* Need to call ops->free() */
		bool			global:1;		/* Goes into &init_user_ns */
		bool			oldapi:1;		/* Coming from mount(2) */
	} fs_ctxt_s;

	typedef struct fs_context_operations {
		void	(*free)(fs_ctxt_s *fc);
		int		(*dup)(fs_ctxt_s *fc, fs_ctxt_s *src_fc);
		int		(*parse_param)(fs_ctxt_s *fc, fs_param_s *param);
		int		(*parse_monolithic)(fs_ctxt_s *fc, void *data);
		int		(*get_tree)(fs_ctxt_s *fc);
		int		(*reconfigure)(fs_ctxt_s *fc);
	} fs_ctxt_ops_s;

	/*
	 * fs_context manipulation functions.
	 */
	extern fs_ctxt_s *
	fs_context_for_mount(fs_type_s *fs_type, unsigned int sb_flags);
	// extern fs_ctxt_s *fs_context_for_reconfigure(struct dentry *dentry,
	// 						unsigned int sb_flags,
	// 						unsigned int sb_flags_mask);
	// extern fs_ctxt_s *fs_context_for_submount(fs_type_s *fs_type,
	// 						struct dentry *reference);

	// extern fs_ctxt_s *vfs_dup_fs_context(fs_ctxt_s *fc);
	// extern int vfs_parse_fs_param(fs_ctxt_s *fc, fs_param_s *param);
	// extern int vfs_parse_fs_string(fs_ctxt_s *fc, const char *key,
	// 				const char *value, size_t v_size);
	// extern int generic_parse_monolithic(fs_ctxt_s *fc, void *data);
	extern int vfs_get_tree(fs_ctxt_s *fc);
	extern void put_fs_context(fs_ctxt_s *fc);
	// extern int vfs_parse_fs_param_source(fs_ctxt_s *fc,
	// 					fs_param_s *param);
	// extern void fc_drop_locked(fs_ctxt_s *fc);
	// int reconfigure_single(struct super_block *s,
	// 			int flags, void *data);

	/*
	 * sget() wrappers to be called from the ->get_tree() op.
	 */
	enum vfs_get_super_keying {
		vfs_get_single_super,			/* Only one such superblock may exist */
		vfs_get_single_reconf_super,	/* As above, but reconfigure if it exists */
		vfs_get_keyed_super,			/* Superblocks with different s_fs_info keys may exist */
		vfs_get_independent_super,		/* Multiple independent superblocks may exist */
	};
	// extern int
	// vfs_get_super(fs_ctxt_s *fc, enum vfs_get_super_keying keying,
	// 		int (*fill_super)(super_block_s *sb, fs_ctxt_s *fc));
	extern int
	myos_vfs_get_super(fs_ctxt_s *fc, enum vfs_get_super_keying keying,
			int (*fill_super)(super_block_s *sb, fs_ctxt_s *fc));

	extern int
	get_tree_nodev(fs_ctxt_s *fc, int (*fill_super)(super_block_s *sb,
			fs_ctxt_s *fc));
	// extern int get_tree_single(fs_ctxt_s *fc,
	// 			int (*fill_super)(struct super_block *sb,
	// 					fs_ctxt_s *fc));
	// extern int get_tree_single_reconf(fs_ctxt_s *fc,
	// 			int (*fill_super)(struct super_block *sb,
	// 					fs_ctxt_s *fc));
	// extern int get_tree_keyed(fs_ctxt_s *fc,
	// 			int (*fill_super)(struct super_block *sb,
	// 					fs_ctxt_s *fc),
	// 			void *key);

	// extern int get_tree_bdev(fs_ctxt_s *fc,
	// 				int (*fill_super)(struct super_block *sb,
	// 						fs_ctxt_s *fc));

	// extern const struct file_operations fscontext_fops;

	// /*
	// * Mount error, warning and informational message logging.  This structure is
	// * shareable between a mount and a subordinate mount.
	// */
	// struct fc_log {
	// 	refcount_t	usage;
	// 	u8		head;		/* Insertion index in buffer[] */
	// 	u8		tail;		/* Removal index in buffer[] */
	// 	u8		need_free;	/* Mask of kfree'able items in buffer[] */
	// 	struct module	*owner;		/* Owner module for strings that don't then need freeing */
	// 	char		*buffer[8];
	// };

	// extern __attribute__((format(printf, 4, 5)))
	// void logfc(struct fc_log *log, const char *prefix, char level, const char *fmt, ...);

	// #define __logfc(fc, l, fmt, ...) logfc((fc)->log.log, NULL, \
	// 					l, fmt, ## __VA_ARGS__)
	// #define __plog(p, l, fmt, ...) logfc((p)->log, (p)->prefix, \
	// 					l, fmt, ## __VA_ARGS__)
	// /**
	//  * infof - Store supplementary informational message
	//  * @fc: The context in which to log the informational message
	//  * @fmt: The format string
	//  *
	//  * Store the supplementary informational message for the process if the process
	//  * has enabled the facility.
	//  */
	// #define infof(fc, fmt, ...) __logfc(fc, 'i', fmt, ## __VA_ARGS__)
	// #define info_plog(p, fmt, ...) __plog(p, 'i', fmt, ## __VA_ARGS__)
	// #define infofc(p, fmt, ...) __plog((&(fc)->log), 'i', fmt, ## __VA_ARGS__)

	// /**
	//  * warnf - Store supplementary warning message
	//  * @fc: The context in which to log the error message
	//  * @fmt: The format string
	//  *
	//  * Store the supplementary warning message for the process if the process has
	//  * enabled the facility.
	//  */
	// #define warnf(fc, fmt, ...) __logfc(fc, 'w', fmt, ## __VA_ARGS__)
	// #define warn_plog(p, fmt, ...) __plog(p, 'w', fmt, ## __VA_ARGS__)
	// #define warnfc(fc, fmt, ...) __plog((&(fc)->log), 'w', fmt, ## __VA_ARGS__)

	// /**
	//  * errorf - Store supplementary error message
	//  * @fc: The context in which to log the error message
	//  * @fmt: The format string
	//  *
	//  * Store the supplementary error message for the process if the process has
	//  * enabled the facility.
	//  */
	// #define errorf(fc, fmt, ...) __logfc(fc, 'e', fmt, ## __VA_ARGS__)
	// #define error_plog(p, fmt, ...) __plog(p, 'e', fmt, ## __VA_ARGS__)
	// #define errorfc(fc, fmt, ...) __plog((&(fc)->log), 'e', fmt, ## __VA_ARGS__)

	// /**
	//  * invalf - Store supplementary invalid argument error message
	//  * @fc: The context in which to log the error message
	//  * @fmt: The format string
	//  *
	//  * Store the supplementary error message for the process if the process has
	//  * enabled the facility and return -EINVAL.
	//  */
	// #define invalf(fc, fmt, ...) (errorf(fc, fmt, ## __VA_ARGS__), -EINVAL)
	// #define inval_plog(p, fmt, ...) (error_plog(p, fmt, ## __VA_ARGS__), -EINVAL)
	// #define invalfc(fc, fmt, ...) (errorfc(fc, fmt, ## __VA_ARGS__), -EINVAL)

#endif /* _LINUX_FS_CONTEXT_H */