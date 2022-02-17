#ifndef _FS_S_DEFS_H_
#define _FS_S_DEFS_H_

	struct dentry;
	typedef struct dentry dentry_s;
	struct dentry_ops;
	typedef struct dentry_ops dentry_ops_s;

	struct super_block;
	typedef struct super_block super_block_s;
	struct sb_ops;
	typedef struct sb_ops sb_ops_s;

	struct mount;
	typedef struct mount mount_s;

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

#endif /* _FS_S_DEFS_H_ */