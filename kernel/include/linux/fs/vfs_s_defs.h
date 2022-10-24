#ifndef __ASSEMBLY__

	struct dentry;
	typedef struct dentry dentry_s;
	struct dentry_operations;
	typedef struct dentry_operations dentry_ops_s;

	struct super_block;
	typedef struct super_block super_block_s;
	struct super_operations;
	typedef struct super_operations super_ops_s;

	struct mount;
	typedef struct mount mount_s;

	struct vfsmount;
	typedef struct vfsmount vfsmount_s;

	struct inode;
	typedef struct inode inode_s;
	struct inode_operations;
	typedef struct inode_operations inode_ops_s;

	struct file_system_type;
	typedef struct file_system_type fs_type_s;

	struct file;
	typedef struct file file_s;
	struct file_operations;
	typedef struct file_operations file_ops_s;

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

	struct fs_context;
	typedef struct fs_context fs_ctxt_s;
	struct fs_context_operations;
	typedef struct fs_context_operations fs_ctxt_ops_s;

#endif /* __ASSEMBLY__ */