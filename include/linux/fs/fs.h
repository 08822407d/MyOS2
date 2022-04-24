#ifndef _VFS_H_
#define _VFS_H_

#include <linux/kernel/types.h>
#include <linux/kernel/mount.h>
#include <linux/kernel/stat.h>
#include <linux/kernel/uidgid.h>
#include <linux/kernel/time64.h>
#include <linux/kernel/blk_types.h>

#include <uapi/fcntl.h>
#include <uapi/fs.h>

#include <lib/utils.h>

#include <linux/fs/internels.h>
#include <linux/fs/path.h>
#include <linux/fs/dcache.h>
#include <linux/fs/MBR.h>
#include <linux/fs/GPT.h>

	// sysconfigs
	#define MAX_FILE_NR		32

	#define BOOT_FS_IDX		1
	#define ROOT_FS_IDX		2

	typedef struct fs_type
	{
		const char	*name;
		int			fs_flags;
	#define FS_REQUIRES_DEV			1 
	#define FS_BINARY_MOUNTDATA		2
	#define FS_HAS_SUBTYPE			4
	#define FS_USERNS_MOUNT			8		/* Can be mounted by userns root */
	#define FS_DISALLOW_NOTIFY_PERM	16		/* Disable fanotify permission events */
	#define FS_ALLOW_IDMAP			32      /* FS has been updated to handle vfs idmappings. */
	#define FS_THP_SUPPORT			8192	/* Remove once all fs converted */
	#define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move() during rename() internally. */
		dentry_s *(*mount) (fs_type_s *, int,
						const char *, void *);
		void (*kill_sb) (super_block_s *);
		super_block_s *	(*read_superblock)(GPT_PE_s * DPTE, void * buf);
		fs_type_s *		next;
	} fs_type_s;

	typedef struct super_block
	{
		List_s			s_list;		/* Keep this first */
		dev_t			s_dev;		/* search index; _not_ kdev_t */
		unsigned char	s_blocksize_bits;
		unsigned long	s_blocksize;
		loff_t			s_maxbytes;	/* Max file size */
		fs_type_s		*s_type;
		unsigned long	s_flags;
		unsigned long	s_iflags;	/* internal SB_I_* flags */
		unsigned long	s_magic;
		super_ops_s		*s_op;

		dentry_s		*s_root;
		List_hdr_s		s_mounts;	/* list of mounts; _not_ for fs use */
		int				s_count;
		block_device_s	*s_bdev;
		/*
		* Keep s_fs_info, s_time_gran, s_fsnotify_mask, and
		* s_fsnotify_marks together for cache efficiency. They are frequently
		* accessed and rarely modified.
		*/
		void			*s_fs_info;	/* Filesystem private info */

		char			s_id[32];	/* Informational name */
		// uuid_t			s_uuid;		/* UUID */

		unsigned int	s_max_links;
		fmode_t			s_mode;
		/*
		* Filesystem subtype.  If non-empty the filesystem type field
		* in /proc/mounts will be "type.subtype"
		*/
		const char		*s_subtype;
		const dentry_ops_s	*s_d_op; /* default d_op for dentries */

		void			*private_sb_info;
	} super_block_s;

	#define MODULE_ALIAS_FS(NAME) MODULE_ALIAS("fs-" NAME)

	extern dentry_s *mount_bdev(fs_type_s *fs_type, int flags,
			const char *dev_name, void *data,
			int (*fill_super)(super_block_s *, void *, int));
	extern dentry_s *mount_single(fs_type_s *fs_type, int flags,
			void *data, int (*fill_super)(super_block_s *, void *, int));
	extern dentry_s *mount_nodev(fs_type_s *fs_type, int flags,
			void *data, int (*fill_super)(super_block_s *, void *, int));
	extern dentry_s *mount_subtree(vfsmount_s *mnt, const char *path);
	void generic_shutdown_super(super_block_s *sb);
	void kill_block_super(super_block_s *sb);
	void kill_anon_super(super_block_s *sb);
	void kill_litter_super(super_block_s *sb);
	void deactivate_super(super_block_s *sb);
	void deactivate_locked_super(super_block_s *sb);
	int set_anon_super(super_block_s *s, void *data);
	// int set_anon_super_fc(super_block_s *s, fs_context_s *fc);
	int get_anon_bdev(dev_t *);
	void free_anon_bdev(dev_t);
	// super_block_s *sget_fc(fs_context_s *fc,
	// 		int (*test)(super_block_s *, fs_context_s *),
	// 		int (*set)(super_block_s *, fs_context_s *));
	super_block_s *sget(fs_type_s *type, int (*test)(super_block_s *,void *),
			int (*set)(super_block_s *,void *), int flags, void *data);

	/* Alas, no aliases. Too much hassle with bringing module.h everywhere */
	#define fops_get(fops) \
			(((fops) && try_module_get((fops)->owner) ? (fops) : NULL))
	#define fops_put(fops) \
			do { if (fops) module_put((fops)->owner); } while(0)
	/*
	* This one is to be used *ONLY* from ->open() instances.
	* fops must be non-NULL, pinned down *and* module dependencies
	* should be sufficient to pin the caller down as well.
	*/
	#define replace_fops(f, fops) \
		do {	\
			struct file *__file = (f); \
			fops_put(__file->f_op); \
			BUG_ON(!(__file->f_op = (fops))); \
		} while(0)

	extern int register_filesystem(fs_type_s *);
	extern int unregister_filesystem(fs_type_s *);
	extern vfsmount_s *kern_mount(fs_type_s *);
	extern void kern_unmount(vfsmount_s *mnt);
	extern int may_umount_tree(vfsmount_s *);
	extern int may_umount(vfsmount_s *);
	extern long do_mount(const char *, const char *, unsigned long);
	extern vfsmount_s *collect_mounts(const path_s *);
	extern void drop_collected_mounts(vfsmount_s *);
	extern int iterate_mounts(int (*)(vfsmount_s *, void *), void *,
				vfsmount_s *);
	// extern int vfs_statfs(const path_s *, kstatfs_s *);
	// extern int user_statfs(const char *, kstatfs_s *);
	// extern int fd_statfs(int, kstatfs_s *);
	extern int freeze_super(super_block_s *super);
	extern int thaw_super(super_block_s *super);
	extern bool our_mnt(vfsmount_s *mnt);
	// extern __printf(2, 3)
	int super_setup_bdi_name(super_block_s *sb, char *fmt, ...);
	extern int super_setup_bdi(super_block_s *sb);

	extern int current_umask(void);

	extern void ihold(inode_s * inode);
	extern void iput(inode_s *);
	extern int generic_update_time(inode_s *, timespec64_s *, int);


	typedef struct inode
	{
		umode_t			i_mode;
		unsigned short	i_opflags;
		kuid_t			i_uid;
		kgid_t			i_gid;
		unsigned int	i_flags;

		inode_ops_s		*i_op;
		super_block_s	*i_sb;
		/* Stat data, not accessed from path walking */
		unsigned long	i_ino;
		dev_t			i_rdev;
		loff_t			i_size;
		timespec64_s	i_atime;
		timespec64_s	i_mtime;
		timespec64_s	i_ctime;
		unsigned short	i_bytes;
		uint8_t			i_blkbits;
		uint8_t			i_write_hint;
		blkcnt_t		i_blocks;

		unsigned long	attribute;

		file_ops_s		*i_fop;
		void			*private_idx_info;
	} inode_s;

	#define FS_ATTR_FILE	(1UL << 0)
	#define FS_ATTR_DIR		(1UL << 1)
	#define	FS_ATTR_DEVICE	(1UL << 2)

	typedef struct file
	{
		path_s		f_path;
		inode_s		*f_inode;
		file_ops_s	*f_ops;
		dentry_s *	dentry;

		/*
		* Protects f_ep, f_flags.
		* Must not be taken from IRQ context.
		*/
		unsigned int	f_flags;
		fmode_t		f_mode;
		long		f_pos;

		void * 		private_data;
	} file_s;

	typedef struct taskfs {
		int users;
		int umask;
		int in_exec;
		path_s root, pwd;
	} taskfs_s;


	typedef struct super_ops
	{
		void	(*write_super)(super_block_s * sb);
		int		(*write_inode)(inode_s * inode);

		inode_s	*(*alloc_inode)(super_block_s *sb);
		void	(*destroy_inode)(inode_s *);
		void	(*free_inode)(inode_s *);

		void	(*dirty_inode) (inode_s *, int flags);
		// int		(*write_inode) (inode_s *, writeback_control_s *wbc);
		int		(*drop_inode) (inode_s *);
		void	(*evict_inode) (inode_s *);
		void	(*put_super) (super_block_s *);
		int		(*sync_fs)(super_block_s *sb, int wait);
		int		(*freeze_super) (super_block_s *);
		int		(*freeze_fs) (super_block_s *);
		int		(*thaw_super) (super_block_s *);
		int		(*unfreeze_fs) (super_block_s *);
		// int		(*statfs) (dentry_s *, kstatfs_s *);
		int		(*remount_fs) (super_block_s *, int *, char *);
		void	(*umount_begin) (super_block_s *);

		// int		(*show_options)(seq_file_s *, dentry_s *);
		// int		(*show_devname)(seq_file_s *, dentry_s *);
		// int		(*show_path)(seq_file_s *, dentry_s *);
		// int		(*show_stats)(seq_file_s *, dentry_s *);

		// long (*nr_cached_objects)(super_block_s *, shrink_control_s *);
		// long (*free_cached_objects)(super_block_s *, shrink_control_s *);
	} super_ops_s;

	typedef struct inode_ops
	{
		dentry_s*(*lookup) (inode_s *dir, dentry_s *dentry,
						unsigned int flags);
		int		(*permission) (inode_s *, int);

		int		(*readlink) (dentry_s *, char *, int);

		int		(*create) (inode_s *dir, dentry_s *dnetry,
						umode_t mode, bool excl);
		int		(*link) (dentry_s *, inode_s *, dentry_s *);
		int		(*unlink) (inode_s *dir, dentry_s *dentry);
		int		(*symlink) (inode_s *, dentry_s *, const char *);
		int		(*mkdir) (inode_s *dir, dentry_s *dentry,
						umode_t mode);
		int		(*rmdir) (inode_s *dir, dentry_s *dentry);
		int		(*mknod) (inode_s *, dentry_s *, umode_t, dev_t);
		int		(*rename) (inode_s *old_dir, dentry_s *old_dentry,
						inode_s *new_dir, dentry_s *new_dentry,
						unsigned int flags);
		int		(*setattr) (dentry_s *dentry, iattr_s *attr);
		int		(*getattr) (const path_s *path, kstat_s *stat,
						uint32_t request_mask, unsigned int flags);
		ssize_t	(*listxattr) (dentry_s *, char *, size_t);
		// int (*fiemap)(inode_s *, fiemap_extent_info_s *, uint64_t start,
		// 				uint64_t len);
		// int (*update_time)(inode_s *, timespec64_s *, int);
		int		(*atomic_open)(inode_s *, dentry_s *, file_s *,
						unsigned open_flag, umode_t create_mode);
		int		(*tmpfile) (inode_s *, dentry_s *, umode_t);
		// int (*set_acl)(inode_s *, posix_acl_s *, int);
		// int (*fileattr_set)(dentry_s *dentry, fileattr_s *fa);
		// int (*fileattr_get)(dentry_s *dentry, fileattr_s *fa);
	} inode_ops_s;

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

	/*
	* This is the Inode Attributes structure, used for notify_change().  It
	* uses the above definitions as flags, to know which values have changed.
	* Also, in this manner, a Filesystem can look at only the values it cares
	* about.  Basically, these are the attributes that the VFS layer can
	* request to change from the FS layer.
	*
	* Derek Atkins <warlord@MIT.EDU> 94-10-20
	*/
	typedef struct iattr {
		unsigned int	ia_valid;
		umode_t			ia_mode;
		kuid_t			ia_uid;
		kgid_t			ia_gid;
		loff_t			ia_size;
		timespec64_s	ia_atime;
		timespec64_s	ia_mtime;
		timespec64_s	ia_ctime;

		/*
		* Not an attribute, but an auxiliary info for filesystems wanting to
		* implement an ftruncate() like method.  NOTE: filesystem should
		* check for (ia_valid & ATTR_FILE), and not for (ia_file != NULL).
		*/
		file_s			*ia_file;
	} iattr_s;

	extern super_block_s	*root_sb;
	extern mount_s			root_mnt;

	extern file_ops_s		FAT32_file_ops;
	extern super_ops_s		FAT32_sb_ops;

	extern file_ops_s		tty_fops;

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
	#define WHITEOUT_MODE	0
	#define WHITEOUT_DEV	0

	/*
	* sb->s_flags.  Note that these mirror the equivalent MS_* flags where
	* represented in both.
	*/
	#define SB_RDONLY		1		/* Mount read-only */
	#define SB_NOSUID		2		/* Ignore suid and sgid bits */
	#define SB_NODEV		4		/* Disallow access to device special files */
	#define SB_NOEXEC		8		/* Disallow program execution */
	#define SB_SYNCHRONOUS	16		/* Writes are synced at once */
	#define SB_MANDLOCK		64		/* Allow mandatory locks on an FS */
	#define SB_DIRSYNC		128		/* Directory modifications are synchronous */
	#define SB_NOATIME		1024	/* Do not update access times. */
	#define SB_NODIRATIME	2048	/* Do not update directory access times */
	#define SB_SILENT		32768
	#define SB_POSIXACL		(1<<16)	/* VFS does not apply the umask */
	#define SB_INLINECRYPT	(1<<17)	/* Use blk-crypto for encrypted files */
	#define SB_KERNMOUNT	(1<<22) /* this is a kern_mount call */
	#define SB_I_VERSION	(1<<23) /* Update inode I_version field */
	#define SB_LAZYTIME		(1<<25) /* Update the on-disk [acm]times lazily */

	/* These sb flags are internal to the kernel */
	#define SB_SUBMOUNT		(1<<26)
	#define SB_FORCE		(1<<27)
	#define SB_NOSEC		(1<<28)
	#define SB_BORN			(1<<29)
	#define SB_ACTIVE		(1<<30)
	#define SB_NOUSER		(1<<31)

	/* These flags relate to encoding and casefolding */
	#define SB_ENC_STRICT_MODE_FL	(1 << 0)

	#define sb_has_strict_encoding(sb) \
				(sb->s_encoding_flags & SB_ENC_STRICT_MODE_FL)

	/*
	*	Umount options
	*/

	#define MNT_FORCE			0x00000001	/* Attempt to forcibily umount */
	#define MNT_DETACH			0x00000002	/* Just detach from the tree */
	#define MNT_EXPIRE			0x00000004	/* Mark for expiry */
	#define UMOUNT_NOFOLLOW		0x00000008	/* Don't follow symlink on umount */
	#define UMOUNT_UNUSED		0x80000000	/* Flag guaranteed to be unused */

	/* sb->s_iflags */
	#define SB_I_CGROUPWB		0x00000001	/* cgroup-aware writeback enabled */
	#define SB_I_NOEXEC			0x00000002	/* Ignore executables on this fs */
	#define SB_I_NODEV			0x00000004	/* Ignore devices on this fs */
	#define SB_I_STABLE_WRITES	0x00000008	/* don't modify blks until WB is done */

	/* sb->s_iflags to limit user namespace mounts */
	#define SB_I_USERNS_VISIBLE				0x00000010 /* fstype already mounted */
	#define SB_I_IMA_UNVERIFIABLE_SIGNATURE	0x00000020
	#define SB_I_UNTRUSTED_MOUNTER			0x00000040

	#define SB_I_SKIP_SYNC		0x00000100	/* Skip superblock at global sync */

	/* Possible states of 'frozen' field */
	enum {
		SB_UNFROZEN			= 0,	/* FS is unfrozen */
		SB_FREEZE_WRITE		= 1,	/* Writes, dir ops, ioctls frozen */
		SB_FREEZE_PAGEFAULT	= 2,	/* Page faults stopped as well */
		SB_FREEZE_FS		= 3,	/* For internal FS use (e.g. to stop
										* internal threads if needed) */
		SB_FREEZE_COMPLETE	= 4,	/* ->freeze_fs finished successfully */
	};

	#define SB_FREEZE_LEVELS (SB_FREEZE_COMPLETE - 1)

	/*
	* Inode flags - they have no relation to superblock flags now
	*/
	#define S_SYNC			(1 << 0)  /* Writes are synced at once */
	#define S_NOATIME		(1 << 1)  /* Do not update access times */
	#define S_APPEND		(1 << 2)  /* Append-only file */
	#define S_IMMUTABLE		(1 << 3)  /* Immutable file */
	#define S_DEAD			(1 << 4)  /* removed, but still open directory */
	#define S_NOQUOTA		(1 << 5)  /* Inode is not counted to quota */
	#define S_DIRSYNC		(1 << 6)  /* Directory modifications are synchronous */
	#define S_NOCMTIME		(1 << 7)  /* Do not update file c/mtime */
	#define S_SWAPFILE		(1 << 8)  /* Do not truncate: swapon got its bmaps */
	#define S_PRIVATE		(1 << 9)  /* Inode is fs-internal */
	#define S_IMA			(1 << 10) /* Inode has an associated IMA struct */
	#define S_AUTOMOUNT		(1 << 11) /* Automount/referral quasi-directory */
	#define S_NOSEC			(1 << 12) /* no suid or xattr security attributes */
	#ifdef CONFIG_FS_DAX
		#define S_DAX			(1 << 13) /* Direct Access, avoiding the page cache */
	#else
		#define S_DAX			0	  /* Make all the DAX code disappear */
	#endif
	#define S_ENCRYPTED		(1 << 14) /* Encrypted file (using fs/crypto/) */
	#define S_CASEFOLD		(1 << 15) /* Casefolded file */
	#define S_VERITY		(1 << 16) /* Verity file (using fs/verity/) */

	/*
	* Note that nosuid etc flags are inode-specific: setting some file-system
	* flags just means all the inodes inherit those flags by default. It might be
	* possible to override it selectively if you really wanted to with some
	* ioctl() that is not currently implemented.
	*
	* Exception: SB_RDONLY is always applied to the entire file system.
	*
	* Unfortunately, it is possible to change a filesystems flags with it mounted
	* with files in use.  This means that all of the inodes will not have their
	* i_flags updated.  Hence, i_flags no longer inherit the superblock mount
	* flags, so these have to be checked separately. -- rmk@arm.uk.linux.org
	*/
	#define __IS_FLG(inode, flg)((inode)->i_sb->s_flags & (flg))

	// static inline bool sb_rdonly(const struct super_block *sb) { return sb->s_flags & SB_RDONLY; }
	// #define IS_RDONLY(inode)	sb_rdonly((inode)->i_sb)
	#define IS_SYNC(inode)		(__IS_FLG(inode, SB_SYNCHRONOUS) || \
										((inode)->i_flags & S_SYNC))
	#define IS_DIRSYNC(inode)	(__IS_FLG(inode, SB_SYNCHRONOUS|SB_DIRSYNC) || \
										((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
	#define IS_MANDLOCK(inode)	__IS_FLG(inode, SB_MANDLOCK)
	#define IS_NOATIME(inode)	__IS_FLG(inode, SB_RDONLY|SB_NOATIME)
	#define IS_I_VERSION(inode)	__IS_FLG(inode, SB_I_VERSION)

	#define IS_NOQUOTA(inode)	((inode)->i_flags & S_NOQUOTA)
	#define IS_APPEND(inode)	((inode)->i_flags & S_APPEND)
	#define IS_IMMUTABLE(inode)	((inode)->i_flags & S_IMMUTABLE)
	#define IS_POSIXACL(inode)	__IS_FLG(inode, SB_POSIXACL)

	#define IS_DEADDIR(inode)	((inode)->i_flags & S_DEAD)
	#define IS_NOCMTIME(inode)	((inode)->i_flags & S_NOCMTIME)
	#define IS_SWAPFILE(inode)	((inode)->i_flags & S_SWAPFILE)
	#define IS_PRIVATE(inode)	((inode)->i_flags & S_PRIVATE)
	#define IS_IMA(inode)		((inode)->i_flags & S_IMA)
	#define IS_AUTOMOUNT(inode)	((inode)->i_flags & S_AUTOMOUNT)
	#define IS_NOSEC(inode)		((inode)->i_flags & S_NOSEC)
	#define IS_DAX(inode)		((inode)->i_flags & S_DAX)
	#define IS_ENCRYPTED(inode)	((inode)->i_flags & S_ENCRYPTED)
	#define IS_CASEFOLDED(inode)((inode)->i_flags & S_CASEFOLD)
	#define IS_VERITY(inode)	((inode)->i_flags & S_VERITY)

	#define IS_WHITEOUT(inode)	(S_ISCHR(inode->i_mode) && \
										(inode)->i_rdev == WHITEOUT_DEV)	


	unsigned long init_vfs(void);
	void init_mount(void);
	super_block_s * mount_fs(char * name, GPT_PE_s * DPTE, void * buf);
	int fill_dentry(void *buf, char *name, long namelen, long type, long offset);

	extern long do_sys_open(int dfd, const char * filename, int flags, umode_t mode);
	extern file_s *file_open_name(filename_s *name, int flags, umode_t mode);
	extern file_s *filp_open(const char *filename, int flags, umode_t mode);
	extern file_s *file_open_root(const path_s *root, const char *filename,
					int flage, umode_t mode);
	static inline file_s *file_open_root_mnt(vfsmount_s *mnt,
					const char *filename, int flags, umode_t mode)
	{
		return file_open_root(&(path_s){.mnt = mnt, .dentry = mnt->mnt_root},
						filename, flags, mode);
	}
	int __vfs_open(const path_s * path, file_s * file);

	filename_s *getname(const char *u_filename);
	filename_s *getname_kernel(const char *k_filename);

	#include <include/block_dev.h>
	extern blkdev_ops_s IDE_device_operation;

	#define __FMODE_EXEC		((int) FMODE_EXEC)
	#define __FMODE_NONOTIFY	((int) FMODE_NONOTIFY)

	#define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
	#define OPEN_FMODE(flag) ((fmode_t)(((flag + 1) & O_ACCMODE) | \
							(flag & __FMODE_NONOTIFY)))

	/*
	* Userspace may rely on the the inode number being non-zero. For example, glibc
	* simply ignores files with zero i_ino in unlink() and other places.
	*
	* As an additional complication, if userspace was compiled with
	* _FILE_OFFSET_BITS=32 on a 64-bit kernel we'll only end up reading out the
	* lower 32 bits, so we need to check that those aren't zero explicitly. With
	* _FILE_OFFSET_BITS=64, this may cause some harmless false-negatives, but
	* better safe than sorry.
	*/
	// static inline bool is_zero_ino(ino_t ino)
	// {
	// 	return (uint32_t)ino == 0;
	// }

	// extern void __iget(struct inode * inode);
	// extern void iget_failed(struct inode *);
	// extern void clear_inode(struct inode *);
	// extern void __destroy_inode(struct inode *);
	// extern inode_s *new_inode_pseudo(struct super_block *sb);
	extern inode_s *new_inode(super_block_s *sb);
	extern void iput(inode_s *);
	// extern void free_inode_nonrcu(struct inode *inode);
	// extern int should_remove_suid(struct dentry *);
	// extern int file_remove_privs(struct file *);

	// extern void __insert_inode_hash(struct inode *, unsigned long hashval);
	// static inline void insert_inode_hash(struct inode *inode)
	// {
	// 	__insert_inode_hash(inode, inode->i_ino);
	// }

	// extern void __remove_inode_hash(struct inode *);
	// static inline void remove_inode_hash(struct inode *inode)
	// {
	// 	if (!inode_unhashed(inode) && !hlist_fake(&inode->i_hash))
	// 		__remove_inode_hash(inode);
	// }

	void putname(filename_s * name);

	// extern void inode_sb_list_add(struct inode *inode);

#endif /* _FS_H_ */