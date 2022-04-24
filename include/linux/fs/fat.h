/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FAT_H_
#define _FAT_H_

#include <linux/kernel/stat.h>
#include <linux/kernel/uidgid.h>

#include <uapi/msdos_fs.h>

#include <string.h>
#include <stddef.h>

	/*
	* vfat shortname flags
	*/
	#define VFAT_SFN_DISPLAY_LOWER	0x0001 /* convert to lowercase for display */
	#define VFAT_SFN_DISPLAY_WIN95	0x0002 /* emulate win95 rule for display */
	#define VFAT_SFN_DISPLAY_WINNT	0x0004 /* emulate winnt rule for display */
	#define VFAT_SFN_CREATE_WIN95	0x0100 /* emulate win95 rule for create */
	#define VFAT_SFN_CREATE_WINNT	0x0200 /* emulate winnt rule for create */

	#define FAT_ERRORS_CONT			1      /* ignore error and continue */
	#define FAT_ERRORS_PANIC		2      /* panic on error */
	#define FAT_ERRORS_RO			3      /* remount r/o on error */

	#define FAT_NFS_STALE_RW		1      /* NFS RW support, can cause ESTALE */
	#define FAT_NFS_NOSTALE_RO		2      /* NFS RO support, no ESTALE issue */

	typedef struct fat_mount_options {
		kuid_t			fs_uid;
		kgid_t			fs_gid;
		unsigned short	fs_fmask;
		unsigned short	fs_dmask;
		unsigned short	codepage;				/* Codepage for shortname conversions */
		int				time_offset;			/* Offset of timestamps from UTC (in minutes) */
		char			*iocharset;				/* Charset used for filename input/display */
		unsigned short	shortname;				/* flags for shortname display/create rule */
		unsigned char	name_check;				/* r = relaxed, n = normal, s = strict */
		unsigned char	errors;					/* On error: continue, panic, remount-ro */
		unsigned char	nfs;					/* NFS support: nostale_ro, stale_rw */
		unsigned short	allow_utime;			/* permission for setting the [am]time */
		unsigned		quiet			:1,		/* set = fake successful chmods and chowns */
						showexec		:1,		/* set = only set x bit for com/exe/bat */
						sys_immutable	:1,		/* set = system files are immutable */
						dotsOK			:1,		/* set = hidden and system files are named '.filename' */
						isvfat			:1,		/* 0=no vfat long filename support, 1=vfat support */
						utf8			:1,		/* Use of UTF-8 character set (Default) */
						unicode_xlate	:1,		/* create escape sequences for unhandled Unicode */
						numtail			:1,		/* Does first alias have a numeric '~1' type tail? */
						flush			:1,		/* write things quickly */
						nocase			:1,		/* Does this need case conversion? 0=need case conversion*/
						usefree			:1,		/* Use free_clusters for FAT32 */
						tz_set			:1,		/* Filesystem timestamps' offset set */
						rodir			:1,		/* allow ATTR_RO for directory */
						discard			:1,		/* Issue discard requests on deletions */
						dos1xfloppy		:1;		/* Assume default BPB for DOS 1.x floppies */
	} fat_mount_options_s;

	#define FAT_HASH_BITS		8
	#define FAT_HASH_SIZE		(1UL << FAT_HASH_BITS)

	/*
	* MS-DOS file system in-core superblock data
	*/
	typedef struct msdos_sb_info {
		unsigned short	sec_per_clus;		/* sectors/cluster */
		unsigned short	cluster_bits;		/* log2(cluster_size) */
		unsigned int	cluster_size;		/* cluster size */
		unsigned char	fats, fat_bits;		/* number of FATs, FAT bits (12,16 or 32) */
		unsigned short	fat_start;
		unsigned long	fat_length;			/* FAT start & length (sec.) */
		unsigned long	dir_start;
		unsigned short	dir_entries;		/* root dir start & entries */
		unsigned long	data_start;			/* first data sector */
		unsigned long	max_cluster;		/* maximum cluster number */
		unsigned long	root_cluster;		/* first cluster of the root directory */
		unsigned long	fsinfo_sector;		/* sector number of FAT32 fsinfo */
		// struct mutex	fat_lock;
		// struct mutex	nfs_build_inode_lock;
		// struct mutex	s_lock;
		unsigned int	prev_free;			/* previously allocated cluster number */
		unsigned int	free_clusters;		/* -1 if undefined */
		unsigned int	free_clus_valid;	/* is free_clusters valid? */
		fat_mount_options_s	options;
		// nls_table_s		*nls_disk;			/* Codepage used on disk */
		// nls_table_s		*nls_io;			/* Charset used for input and display */
		const void		*dir_ops;			/* Opaque; default directory operations */
		int				dir_per_block;		/* dir entries per block */
		int				dir_per_block_bits;	/* log2(dir_per_block) */
		unsigned int	vol_id;				/*volume ID*/

		int fatent_shift;
		// const fatent_operations_s *fatent_ops;
		inode_s			*fat_inode;
		inode_s			*fsinfo_inode;

		// struct ratelimit_state ratelimit;

		// spinlock_t inode_hash_lock;
		// struct hlist_head inode_hashtable[FAT_HASH_SIZE];

		// spinlock_t dir_hash_lock;
		// struct hlist_head dir_hashtable[FAT_HASH_SIZE];

		unsigned int dirty;           /* fs state before mount */
		// struct rcu_head rcu;
	} msdos_sb_info_s;

	#define FAT_CACHE_VALID		0	/* special case for valid cache */

	/*
	* MS-DOS file system inode data in memory
	*/
	typedef struct msdos_inode_info {
		// spinlock_t cache_lru_lock;
		// struct list_head cache_lru;
		int		nr_caches;
		/* for avoiding the race between fat_free() and fat_get_cluster() */
		unsigned int cache_valid_id;

		/* NOTE: mmu_private is 64bits, so must hold ->i_mutex to access */
		loff_t	mmu_private;		/* physically allocated size */

		int		i_start;			/* first cluster or 0 */
		int		i_logstart;			/* logical first cluster */
		int		i_attrs;			/* unused attribute bits */
		loff_t	i_pos;				/* on-disk position of directory entry or 0 */
		// struct hlist_node i_fat_hash;	/* hash by i_location */
		// struct hlist_node i_dir_hash;	/* hash by i_logstart */
		// struct rw_semaphore truncate_lock; /* protect bmap against truncate */
		inode_s vfs_inode;
	} msdos_inode_info_s;

	typedef struct fat_slot_info {
		loff_t	i_pos;			/* on-disk position of directory entry */
		loff_t	slot_off;		/* offset for slot or de start */
		int		nr_slots;		/* number of slots + 1(de) in filename */
		msdos_dir_entry_s	*de;
		// struct buffer_head *bh;
	} fat_slot_info_s;

	static inline msdos_sb_info_s *MSDOS_SB(super_block_s *sb)
	{
		return sb->s_fs_info;
	}

	/*
	* Functions that determine the variant of the FAT file system (i.e.,
	* whether this is FAT12, FAT16 or FAT32.
	*/
	static inline bool is_fat12(const msdos_sb_info_s *sbi)
	{
		return sbi->fat_bits == 12;
	}

	static inline bool is_fat16(const msdos_sb_info_s *sbi)
	{
		return sbi->fat_bits == 16;
	}

	static inline bool is_fat32(const msdos_sb_info_s *sbi)
	{
		return sbi->fat_bits == 32;
	}

	/* Maximum number of clusters */
	static inline uint32_t max_fat(super_block_s *sb)
	{
		msdos_sb_info_s *sbi = MSDOS_SB(sb);

		return is_fat32(sbi) ? MAX_FAT32 :
				is_fat16(sbi) ? MAX_FAT16 : MAX_FAT12;
	}

	static inline msdos_inode_info_s *MSDOS_I(inode_s *inode)
	{
		return container_of(inode, msdos_inode_info_s, vfs_inode);
	}

	/*
	* If ->i_mode can't hold S_IWUGO (i.e. ATTR_RO), we use ->i_attrs to
	* save ATTR_RO instead of ->i_mode.
	*
	* If it's directory and !sbi->options.rodir, ATTR_RO isn't read-only
	* bit, it's just used as flag for app.
	*/
	static inline int fat_mode_can_hold_ro(inode_s *inode)
	{
		msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
		umode_t mask;

		if (S_ISDIR(inode->i_mode)) {
			if (!sbi->options.rodir)
				return 0;
			mask = ~sbi->options.fs_dmask;
		} else
			mask = ~sbi->options.fs_fmask;

		if (!(mask & S_IWUGO))
			return 0;
		return 1;
	}

	/* Convert attribute bits and a mask to the UNIX mode. */
	static inline umode_t fat_make_mode(msdos_sb_info_s *sbi,
					uint8_t attrs, umode_t mode)
	{
		if (attrs & ATTR_RO && !((attrs & ATTR_DIR) && !sbi->options.rodir))
			mode &= ~S_IWUGO;

		if (attrs & ATTR_DIR)
			return (mode & ~sbi->options.fs_dmask) | S_IFDIR;
		else
			return (mode & ~sbi->options.fs_fmask) | S_IFREG;
	}

	/* Return the FAT attribute byte for this inode */
	static inline uint8_t fat_make_attrs(inode_s *inode)
	{
		uint8_t attrs = MSDOS_I(inode)->i_attrs;
		if (S_ISDIR(inode->i_mode))
			attrs |= ATTR_DIR;
		if (fat_mode_can_hold_ro(inode) && !(inode->i_mode & S_IWUGO))
			attrs |= ATTR_RO;
		return attrs;
	}

	static inline void fat_save_attrs(inode_s *inode, uint8_t attrs)
	{
		if (fat_mode_can_hold_ro(inode))
			MSDOS_I(inode)->i_attrs = attrs & ATTR_UNUSED;
		else
			MSDOS_I(inode)->i_attrs = attrs & (ATTR_UNUSED | ATTR_RO);
	}

	static inline unsigned char fat_checksum(const uint8_t *name)
	{
		unsigned char s = name[0];
		s = (s<<7) + (s>>1) + name[1];	s = (s<<7) + (s>>1) + name[2];
		s = (s<<7) + (s>>1) + name[3];	s = (s<<7) + (s>>1) + name[4];
		s = (s<<7) + (s>>1) + name[5];	s = (s<<7) + (s>>1) + name[6];
		s = (s<<7) + (s>>1) + name[7];	s = (s<<7) + (s>>1) + name[8];
		s = (s<<7) + (s>>1) + name[9];	s = (s<<7) + (s>>1) + name[10];
		return s;
	}

	// static inline sector_t fat_clus_to_blknr(struct msdos_sb_info *sbi, int clus)
	// {
	// 	return ((sector_t)clus - FAT_START_ENT) * sbi->sec_per_clus
	// 		+ sbi->data_start;
	// }

	// static inline void fat_get_blknr_offset(struct msdos_sb_info *sbi,
	// 				loff_t i_pos, sector_t *blknr, int *offset)
	// {
	// 	*blknr = i_pos >> sbi->dir_per_block_bits;
	// 	*offset = i_pos & (sbi->dir_per_block - 1);
	// }

	static inline loff_t fat_i_pos_read(msdos_sb_info_s *sbi,
						inode_s *inode)
	{
		loff_t i_pos;
	#if BITS_PER_LONG == 32
		spin_lock(&sbi->inode_hash_lock);
	#endif
		i_pos = MSDOS_I(inode)->i_pos;
	#if BITS_PER_LONG == 32
		spin_unlock(&sbi->inode_hash_lock);
	#endif
		return i_pos;
	}

	static inline void fat16_towchar(wchar_t *dst, const uint8_t *src, size_t len)
	{
	#ifdef __BIG_ENDIAN
		while (len--) {
			*dst++ = src[0] | (src[1] << 8);
			src += 2;
		}
	#else
		memcpy(dst, src, len * 2);
	#endif
	}

	static inline int fat_get_start(msdos_sb_info_s *sbi,
					const msdos_dir_entry_s *de)
	{
		int cluster = de->start;
		if (is_fat32(sbi))
			cluster |= (de->starthi << 16);
		return cluster;
	}

	static inline void fat_set_start(msdos_dir_entry_s *de, int cluster)
	{
		de->start   = cluster;
		de->starthi = cluster >> 16;
	}

	static inline void fatwchar_to16(uint8_t *dst, const wchar_t *src, size_t len)
	{
	#ifdef __BIG_ENDIAN
		while (len--) {
			dst[0] = *src & 0x00FF;
			dst[1] = (*src & 0xFF00) >> 8;
			dst += 2;
			src++;
		}
	#else
		memcpy(dst, src, len * 2);
	#endif
	}

	/* fat/cache.c */
	extern void fat_cache_inval_inode(inode_s *inode);
	extern int fat_get_cluster(inode_s *inode, int cluster,
					int *fclus, int *dclus);
	// extern int fat_get_mapped_cluster(struct inode *inode, sector_t sector,
	// 				sector_t last_block,
	// 				unsigned long *mapped_blocks, sector_t *bmap);
	// extern int fat_bmap(struct inode *inode, sector_t sector, sector_t *phys,
	// 			unsigned long *mapped_blocks, int create, bool from_bmap);

	/* fat/dir.c */
	extern const file_ops_s fat_dir_operations;
	extern int fat_search_long(inode_s *inode, const unsigned char *name,
					int name_len, fat_slot_info_s *sinfo);
	extern int fat_dir_empty(inode_s *dir);
	extern int fat_subdirs(inode_s *dir);
	extern int fat_scan(inode_s *dir, const unsigned char *name,
					fat_slot_info_s *sinfo);
	extern int fat_scan_logstart(inode_s *dir, int i_logstart,
					fat_slot_info_s *sinfo);
	// extern int fat_get_dotdot_entry(inode_s *dir, buffer_head_s **bh,
	// 				msdos_dir_entry_s **de);
	// extern int fat_alloc_new_dir(inode_s *dir, timespec64_s *ts);
	extern int fat_add_entries(inode_s *dir, void *slots, int nr_slots,
					fat_slot_info_s *sinfo);
	extern int fat_remove_entries(inode_s *dir, fat_slot_info_s *sinfo);

	/* fat/fatent.c */
	typedef struct fat_entry {
		int entry;
		union {
			uint8_t		*ent12_p[2];
			uint16_t	*ent16_p;
			uint32_t	*ent32_p;
		} u;
		int		nr_bhs;
		// buffer_head_s *bhs[2];
		inode_s *fat_inode;
	} fat_entry_s;

	static inline void fatent_init(fat_entry_s *fatent)
	{
		fatent->nr_bhs = 0;
		fatent->entry = 0;
		fatent->u.ent32_p = NULL;
		// fatent->bhs[0] = fatent->bhs[1] = NULL;
		fatent->fat_inode = NULL;
	}

	static inline void fatent_set_entry(fat_entry_s *fatent, int entry)
	{
		fatent->entry = entry;
		fatent->u.ent32_p = NULL;
	}

	static inline void fatent_brelse(fat_entry_s *fatent)
	{
		int i;
		fatent->u.ent32_p = NULL;
		// for (i = 0; i < fatent->nr_bhs; i++)
		// 	brelse(fatent->bhs[i]);
		fatent->nr_bhs = 0;
		// fatent->bhs[0] = fatent->bhs[1] = NULL;
		fatent->fat_inode = NULL;
	}

	static inline bool fat_valid_entry(msdos_sb_info_s *sbi, int entry)
	{
		return FAT_START_ENT <= entry && entry < sbi->max_cluster;
	}

	extern void fat_ent_access_init(super_block_s *sb);
	extern int fat_ent_read(inode_s *inode, fat_entry_s *fatent,
					int entry);
	extern int fat_ent_write(inode_s *inode, fat_entry_s *fatent,
					int new, int wait);
	extern int fat_alloc_clusters(inode_s *inode, int *cluster,
					int nr_cluster);
	extern int fat_free_clusters(inode_s *inode, int cluster);
	extern int fat_count_free_clusters(super_block_s *sb);
	// extern int fat_trim_fs(inode_s *inode, fstrim_range_s *range);

	/* fat/file.c */
	extern long fat_generic_ioctl(file_s *filp, unsigned int cmd,
					unsigned long arg);
	// extern int fat_setattr(dentry_s *dentry,
	// 				iattr_s *attr);
	extern void fat_truncate_blocks(struct inode *inode, loff_t offset);
	// extern int fat_getattr(const path_s *path, kstat_s *stat,
	// 				uint32_t request_mask, unsigned int flags);
	extern int fat_file_fsync(file_s *file, loff_t start, loff_t end,
					int datasync);

	/* fat/inode.c */
	extern int fat_block_truncate_page(inode_s *inode, loff_t from);
	extern void fat_attach(inode_s *inode, loff_t i_pos);
	extern void fat_detach(inode_s *inode);
	extern inode_s *fat_iget(super_block_s *sb, loff_t i_pos);
	extern inode_s *fat_build_inode(super_block_s *sb,
					msdos_dir_entry_s *de, loff_t i_pos);
	extern int fat_sync_inode(inode_s *inode);
	extern int fat_fill_super(super_block_s *sb, void *data,
					int isvfat, void (*setup)(super_block_s *));
	extern int fat_fill_inode(inode_s *inode, msdos_dir_entry_s *de);

	extern int fat_flush_inodes(super_block_s *sb, inode_s *i1,
					inode_s *i2);
	// static inline unsigned long fat_dir_hash(int logstart)
	// {
	// 	return hash_32(logstart, FAT_HASH_BITS);
	// }
	extern int fat_add_cluster(inode_s *inode);

	/* fat/misc.c */
	// extern __printf(3, 4) __cold
	// void __fat_fs_error(super_block_s *sb, int report, const char *fmt, ...);
	// #define fat_fs_error(sb, fmt, args...)		\
	// 	__fat_fs_error(sb, 1, fmt , ## args)
	// #define fat_fs_error_ratelimit(sb, fmt, args...) \
	// 	__fat_fs_error(sb, __ratelimit(&MSDOS_SB(sb)->ratelimit), fmt , ## args)
	// __printf(3, 4) __cold
	// void fat_msg(super_block_s *sb, const char *level, const char *fmt, ...);
	// #define fat_msg_ratelimit(sb, level, fmt, args...)	\
	// 	do {	\
	// 			if (__ratelimit(&MSDOS_SB(sb)->ratelimit))	\
	// 				fat_msg(sb, level, fmt, ## args);	\
	// 	} while (0)
	// extern int fat_clusters_flush(struct super_block *sb);
	// extern int fat_chain_add(struct inode *inode, int new_dclus, int nr_cluster);
	// extern void fat_time_fat2unix(struct msdos_sb_info *sbi, struct timespec64 *ts,
	// 				uint16_t __time, uint16_t __date, uint8_t time_cs);
	// extern void fat_time_unix2fat(struct msdos_sb_info *sbi, struct timespec64 *ts,
	// 				uint16_t *time, uint16_t *date, uint8_t *time_cs);
	// extern int fat_truncate_time(struct inode *inode, struct timespec64 *now,
	// 				int flags);
	// extern int fat_update_time(struct inode *inode, struct timespec64 *now,
	// 			int flags);
	// extern int fat_sync_bhs(struct buffer_head **bhs, int nr_bhs);

	int fat_cache_init(void);
	void fat_cache_destroy(void);

	/* fat/nfs.c */
	// extern const struct export_operations fat_export_ops;
	// extern const struct export_operations fat_export_ops_nostale;

	/* helper for printk */
	typedef unsigned long long	llu;

	extern inode_ops_s	vfat_dir_inode_operations;
	extern dentry_ops_s vfat_dentry_ops;

	int init_vfat_fs(void);
	void exit_vfat_fs(void);

#endif /* !_FAT_H_ */