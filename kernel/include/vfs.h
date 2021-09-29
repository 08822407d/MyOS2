#ifndef _VFS_H_
#define _VFS_H_

#include "MBR.h"
#include "GPT.h"

#include "../klib/data_structure.h"

	#define BOOT_FS_IDX	1
	#define ROOT_FS_IDX	2

	struct fs_type;
	typedef struct fs_type fs_type_s;

	struct superblock;
	typedef struct superblock superblock_s;

	struct inode;
	typedef struct inode inode_s;

	struct dirent;
	typedef struct dirent dirent_s;

	struct file;
	typedef struct file file_s;

	struct sb_ops;
	typedef struct sb_ops sb_ops_s;

	struct inode_ops;
	typedef struct inode_ops inode_ops_s;

	struct dirent_ops;
	typedef struct dirent_ops dirent_ops_s;

	struct file_ops;
	typedef struct file_ops file_ops_s;


	typedef struct fs_type
	{
		char *	name;
		int		fs_flags;
		superblock_s *	(*read_superblock)(GPT_PE_s * DPTE, void * buf);
		fs_type_s *		next;
	} fs_type_s;

	typedef struct superblock
	{
		dirent_s *	root;
		sb_ops_s *	sb_ops;
		void *		private_sb_info;
	} superblock_s;

	typedef struct inode
	{
		unsigned long	file_size;
		unsigned long	blocks;
		unsigned long	attribute;

		superblock_s *	sb;
		file_ops_s *	f_ops;
		inode_ops_s *	inode_ops;

		void * private_idx_info;
	} inode_s;

	#define FS_ATTR_FILE	(1UL << 0)
	#define FS_ATTR_DIR		(1UL << 1)
	#define	FS_ATTR_DEVICE	(1UL << 2)

	m_define_list_header(dirent);
	typedef struct dirent
	{
		dirent_s *	prev;
		dirent_s *	next;
		dirent_list_s *	list_header;

		char *	name;
		int		name_length;

		inode_s *	dir_inode;
		dirent_s *	parent;

		dirent_list_s	child_list;
		dirent_ops_s *	dir_ops;
	} dirent_s;

	typedef struct file
	{
		long position;
		unsigned long mode;

		dirent_s *		dentry;
		file_ops_s *	f_ops;

		void * private_data;
	} file_s;

	typedef struct sb_ops
	{
		void	(*write_superblock)(superblock_s * sb);
		void	(*put_superblock)(superblock_s * sb);

		void	(*write_inode)(inode_s * inode);
	} sb_ops_s;

	typedef struct inode_ops
	{
		dirent_s * (*lookup)(inode_s * parent_inode, dirent_s * dest_dentry);
		long (*create)(inode_s * inode, dirent_s * dentry, int mode);
		long (*mkdir)(inode_s * inode, dirent_s * dentry, int mode);
		long (*rmdir)(inode_s * inode, dirent_s * dentry);
		long (*rename)(inode_s * old_inode, dirent_s * old_dentry, inode_s * new_inode, dirent_s * new_dentry);
		long (*getattr)(dirent_s * dentry, unsigned long * attr);
		long (*setattr)(dirent_s * dentry, unsigned long * attr);
	} inode_ops_s;

	typedef struct dirent_ops
	{
		long (*compare)(dirent_s * parent_dentry, char * source_filename, char * destination_filename);
		long (*hash)(dirent_s * dentry, char * filename);
		long (*release)(dirent_s * dentry);
		long (*iput)(dirent_s * dentry, inode_s * inode);
	} dirent_ops_s;

	typedef int (*filldir_t)(void *buf, char *name, long namelen, long type, long offset);

	typedef struct file_ops
	{
		long (*open)(inode_s * inode, file_s * filp);
		long (*close)(inode_s * inode, file_s * filp);
		long (*read)(file_s * filp, char * buf, unsigned long count, long * position);
		long (*write)(file_s * filp, char * buf, unsigned long count, long * position);
		long (*lseek)(file_s * filp, long offset, long origin);
		long (*ioctl)(inode_s * inode, file_s * filp, unsigned long cmd, unsigned long arg);

		long (*readdir)(file_s * filp, void * dirent, filldir_t filler);
	} file_ops_s;

	// dirent_s * path_walk(char * name, unsigned long flags);
	// int fill_dentry(void *buf, char *name, long namelen, long type, long offset);
	// extern superblock_s * root_sb;

	extern inode_ops_s	FAT32_inode_ops;
	extern file_ops_s	FAT32_file_ops;
	extern dirent_ops_s	FAT32_dentry_ops;
	extern sb_ops_s		FAT32_sb_ops;

	extern file_ops_s	keyboard_fops;

	unsigned long init_vfs();
	dirent_s * path_walk(char * name,unsigned long flags);
	superblock_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);
	unsigned long register_filesystem(fs_type_s * fs);
	unsigned long unregister_filesystem(fs_type_s * fs);

	//
	#include "block.h"
	extern blkdev_ops_s IDE_device_operation;
#endif /* _FS_H_ */