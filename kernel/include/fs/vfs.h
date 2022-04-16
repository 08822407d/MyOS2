#ifndef _VFS_H_
#define _VFS_H_

#include <uapi/fcntl.h>
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

	// internels
	typedef struct open_flags {
		int open_flag;
		umode_t mode;
		int acc_mode;
		int intent;
		int lookup_flags;
	} open_flags_s;

	extern super_block_s * root_sb;
	extern mount_s root_mnt;

	extern inode_ops_s	FAT32_inode_ops;
	extern file_ops_s	FAT32_file_ops;
	extern dentry_ops_s	FAT32_dentry_ops;
	extern sb_ops_s		FAT32_sb_ops;

	extern file_ops_s	tty_fops;

	#define MAY_EXEC		0x00000001
	#define MAY_WRITE		0x00000002
	#define MAY_READ		0x00000004
	#define MAY_APPEND		0x00000008
	#define MAY_ACCESS		0x00000010
	#define MAY_OPEN		0x00000020
	#define MAY_CHDIR		0x00000040
	/* called from RCU mode, don't block */
	#define MAY_NOT_BLOCK	0x00000080

	/*
	* flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
	* to O_WRONLY and O_RDWR via the strange trick in do_dentry_open()
	*/

	/* file is open for reading */
	#define FMODE_READ				((fmode_t)0x1)
	/* file is open for writing */
	#define FMODE_WRITE				((fmode_t)0x2)
	/* file is seekable */
	#define FMODE_LSEEK				((fmode_t)0x4)
	/* file can be accessed using pread */
	#define FMODE_PREAD				((fmode_t)0x8)
	/* file can be accessed using pwrite */
	#define FMODE_PWRITE			((fmode_t)0x10)
	/* File is opened for execution with sys_execve / sys_uselib */
	#define FMODE_EXEC				((fmode_t)0x20)
	/* File is opened with O_NDELAY (only set for block devices) */
	#define FMODE_NDELAY			((fmode_t)0x40)
	/* File is opened with O_EXCL (only set for block devices) */
	#define FMODE_EXCL				((fmode_t)0x80)
	/* File is opened using open(.., 3, ..) and is writeable only for ioctls
	(specialy hack for floppy.c) */
	#define FMODE_WRITE_IOCTL		((fmode_t)0x100)
	/* 32bit hashes as llseek() offset (for directories) */
	#define FMODE_32BITHASH			((fmode_t)0x200)
	/* 64bit hashes as llseek() offset (for directories) */
	#define FMODE_64BITHASH			((fmode_t)0x400)

	/*
	* Don't update ctime and mtime.
	*
	* Currently a special hack for the XFS open_by_handle ioctl, but we'll
	* hopefully graduate it to a proper O_CMTIME flag supported by open(2) soon.
	*/
	#define FMODE_NOCMTIME			((fmode_t)0x800)

	/* Expect random access pattern */
	#define FMODE_RANDOM			((fmode_t)0x1000)

	/* File is huge (eg. /dev/mem): treat loff_t as unsigned */
	#define FMODE_UNSIGNED_OFFSET	((fmode_t)0x2000)

	/* File is opened with O_PATH; almost nothing can be done with it */
	#define FMODE_PATH				((fmode_t)0x4000)

	/* File needs atomic accesses to f_pos */
	#define FMODE_ATOMIC_POS		((fmode_t)0x8000)
	/* Write access to underlying fs */
	#define FMODE_WRITER			((fmode_t)0x10000)
	/* Has read method(s) */
	#define FMODE_CAN_READ			((fmode_t)0x20000)
	/* Has write method(s) */
	#define FMODE_CAN_WRITE			((fmode_t)0x40000)

	#define FMODE_OPENED			((fmode_t)0x80000)
	#define FMODE_CREATED			((fmode_t)0x100000)

	/* File is stream-like */
	#define FMODE_STREAM			((fmode_t)0x200000)

	/* File was opened by fanotify and shouldn't generate fanotify events */
	#define FMODE_NONOTIFY			((fmode_t)0x4000000)

	/* File is capable of returning -EAGAIN if I/O will block */
	#define FMODE_NOWAIT			((fmode_t)0x8000000)

	/* File represents mount that needs unmounting */
	#define FMODE_NEED_UNMOUNT		((fmode_t)0x10000000)

	/* File does not contribute to nr_files count */
	#define FMODE_NOACCOUNT			((fmode_t)0x20000000)

	/* File supports async buffered reads */
	#define FMODE_BUF_RASYNC		((fmode_t)0x40000000)

	/*
	* Attribute flags.  These should be or-ed together to figure out what
	* has been changed!
	*/
	#define ATTR_MODE		(1 << 0)
	#define ATTR_UID		(1 << 1)
	#define ATTR_GID		(1 << 2)
	#define ATTR_SIZE		(1 << 3)
	#define ATTR_ATIME		(1 << 4)
	#define ATTR_MTIME		(1 << 5)
	#define ATTR_CTIME		(1 << 6)
	#define ATTR_ATIME_SET	(1 << 7)
	#define ATTR_MTIME_SET	(1 << 8)
	#define ATTR_FORCE		(1 << 9)	/* Not a change, but a change it */
	#define ATTR_KILL_SUID	(1 << 11)
	#define ATTR_KILL_SGID	(1 << 12)
	#define ATTR_FILE		(1 << 13)
	#define ATTR_KILL_PRIV	(1 << 14)
	#define ATTR_OPEN		(1 << 15)	/* Truncating from open(O_TRUNC) */
	#define ATTR_TIMES_SET	(1 << 16)
	#define ATTR_TOUCH		(1 << 17)

	/*
	* Whiteout is represented by a char device.  The following constants define the
	* mode and device number to use.
	*/
	#define WHITEOUT_MODE 0
	#define WHITEOUT_DEV 0


	long do_sys_open(int dfd, const char * filename, int flags, umode_t mode);
	unsigned long init_vfs(void);
	void init_mount(void);
	super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);
	unsigned long register_filesystem(fs_type_s * fs);
	unsigned long unregister_filesystem(fs_type_s * fs);
	int fill_dentry(void *buf, char *name, long namelen, long type, long offset);

	file_s * do_filp_open(int dfd, filename_s * name, open_flags_s *op);
	int __vfs_open(const path_s * path, file_s * file);
	int path_mount(const char * dev_name, path_s * path, unsigned long flags);

	filename_s *__getname(void);

	#include <include/block_dev.h>
	extern blkdev_ops_s IDE_device_operation;

	#define __FMODE_EXEC		((int) FMODE_EXEC)
	#define __FMODE_NONOTIFY	((int) FMODE_NONOTIFY)

	#define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
	#define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
							(flag & __FMODE_NONOTIFY)))

#endif /* _FS_H_ */