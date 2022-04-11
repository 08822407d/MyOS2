#ifndef _VFS_H_
#define _VFS_H_

#include <lib/utils.h>

#include <include/fs/dcache.h>
#include <include/fs/mount.h>
#include <include/fs/MBR.h>
#include <include/fs/GPT.h>

#include "vfs_s_defs.h"

	// sysconfigs
	#define MAX_FILE_NR		32

	#define BOOT_FS_IDX		1
	#define ROOT_FS_IDX		2

	typedef struct fs_type
	{
		char *	name;
		int		fs_flags;
		super_block_s *	(*read_superblock)(GPT_PE_s * DPTE, void * buf);
		fs_type_s *		next;
	} fs_type_s;

	typedef struct super_block
	{
		dentry_s *	root;
		sb_ops_s *	sb_ops;
		void *		private_sb_info;
	} super_block_s;

	typedef struct inode
	{
		unsigned long	file_size;
		unsigned long	blocks;
		unsigned long	attribute;

		super_block_s *	sb;
		file_ops_s *	f_ops;
		inode_ops_s *	inode_ops;

		void * private_idx_info;
	} inode_s;

	#define FS_ATTR_FILE	(1UL << 0)
	#define FS_ATTR_DIR		(1UL << 1)
	#define	FS_ATTR_DEVICE	(1UL << 2)

	typedef struct file
	{
		long			position;
		unsigned long	mode;

		dentry_s *		dentry;
		file_ops_s *	f_ops;

		void * 			private_data;
	} file_s;

	typedef struct path {
		vfsmount_s *	mnt;
		dentry_s *		dentry;
	} path_s;

	typedef struct taskfs {
		int users;
		int umask;
		int in_exec;
		path_s root, pwd;
	} taskfs_s;


	typedef struct sb_ops
	{
		void	(*write_superblock)(super_block_s * sb);
		void	(*put_superblock)(super_block_s * sb);

		void	(*write_inode)(inode_s * inode);
	} sb_ops_s;

	typedef struct inode_ops
	{
		dentry_s *	(*lookup)(inode_s * parent_inode, dentry_s * dest_dentry);
		long		(*create)(inode_s * inode, dentry_s * dentry, int mode);
		long		(*mkdir)(inode_s * inode, dentry_s * dentry, int mode);
		long		(*rmdir)(inode_s * inode, dentry_s * dentry);
		long		(*rename)(inode_s * old_inode, dentry_s * old_dentry, inode_s * new_inode, dentry_s * new_dentry);
		long		(*getattr)(dentry_s * dentry, unsigned long * attr);
		long		(*setattr)(dentry_s * dentry, unsigned long * attr);
	} inode_ops_s;

	typedef struct dentry_ops
	{
		long	(*compare)(dentry_s * parent_dentry, char * source_filename, char * destination_filename);
		long	(*hash)(dentry_s * dentry, char * filename);
		long	(*release)(dentry_s * dentry);
		long	(*iput)(dentry_s * dentry, inode_s * inode);
	} dentry_ops_s;

	typedef int (*filldir_t)(void *buf, char *name, long namelen, long type, long offset);

	typedef struct file_ops
	{
		long	(*open)(inode_s * inode, file_s * filp);
		long	(*close)(inode_s * inode, file_s * filp);
		long	(*read)(file_s * filp, char * buf, unsigned long count, long * position);
		long	(*write)(file_s * filp, char * buf, unsigned long count, long * position);
		long	(*lseek)(file_s * filp, long offset, long origin);
		long	(*ioctl)(inode_s * inode, file_s * filp, unsigned long cmd, unsigned long arg);

		long	(*readdir)(file_s * filp, void * dirent, filldir_t filler);
	} file_ops_s;

	typedef struct filename {
		const char *	name;	/* pointer to actual string */
		const char *	uptr;	/* original userland pointer */
		int				refcnt;
		size_t			len;
		// struct audit_names	*aname;
		// const char		iname[];
	} filename_s;

	extern super_block_s * root_sb;
	extern mount_s root_mnt;

	extern inode_ops_s	FAT32_inode_ops;
	extern file_ops_s	FAT32_file_ops;
	extern dentry_ops_s	FAT32_dentry_ops;
	extern sb_ops_s		FAT32_sb_ops;

	extern file_ops_s	tty_fops;

	long do_sys_open(int dfd, const char * filename, int flags);
	unsigned long init_vfs(void);
	void init_mount(void);
	super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);
	unsigned long register_filesystem(fs_type_s * fs);
	unsigned long unregister_filesystem(fs_type_s * fs);
	int fill_dentry(void *buf, char *name, long namelen, long type, long offset);

	file_s * do_filp_open(int dfd, filename_s * name, int flags);
	int __vfs_open(const path_s * path, file_s * file);
	long kopen(const char * filename, int flags);
	int path_mount(const char * dev_name, path_s * path, unsigned long flags);

	filename_s *__getname(void);

	#include <include/block_dev.h>
	extern blkdev_ops_s IDE_device_operation;

#endif /* _FS_H_ */