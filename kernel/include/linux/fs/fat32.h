/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/
#ifndef _FAT32_H_
#define _FAT32_H_

#include <uapi/msdos_fs.h>

#include <linux/kernel/types.h>
#include <klib/utils.h>

	struct FAT32_IO_BUFFER;
	typedef struct FAT32_IO_BUFFER FAT32_iobuf_s;

	typedef struct FAT32_BS
	{
		u8	BS_jmpBoot[3];
		u8	BS_OEMName[8];
		u16	BPB_BytesPerSec;
		u8	BPB_SecPerClus;
		u16	BPB_RsvdSecCnt;
		u8	BPB_NumFATs;
		u16	BPB_RootEntCnt;
		u16	BPB_TotSec16;
		u8	BPB_Media;
		u16	BPB_FATSz16;
		u16	BPB_SecPerTrk;
		u16	BPB_NumHeads;
		u32	BPB_HiddSec;
		u32	BPB_TotSec32;

		u32	BPB_FATSz32;
		u16	BPB_ExtFlags;
		u16	BPB_FSVer;
		u32	BPB_RootClus;
		u16	BPB_FSInfo;
		u16	BPB_BkBootSec;
		u8	BPB_Reserved[12];

		u8	BS_DrvNum;
		u8	BS_Reserved1;
		u8	BS_BootSig;
		u32	BS_VolID;
		u8	BS_VolLab[11];
		u8	BS_FilSysType[8];

		u8	BootCode[420];

		u16	BS_TrailSig;
	}__attribute__((packed)) FAT32_BS_s;


	typedef struct FAT32_FSinfo
	{
		u32	FSI_LeadSig;
		u8	FSI_Reserved1[480];
		u32	FSI_StrucSig;
		u32	FSI_Free_Count;
		u32	FSI_Nxt_Free;
		u8		FSI_Reserved2[12];
		u32	FSI_TrailSig;
	}__attribute__((packed)) FAT32_FSinfo_s;

	#define	ATTR_READ_ONLY	(1 << 0)
	#define ATTR_HIDE		(1 << 1)
	#define ATTR_SYSTEM		(1 << 2)
	#define ATTR_VOLUME_ID	(1 << 3)
	#define ATTR_DIRECTORY	(1 << 4)
	#define ATTR_ARCHIVE	(1 << 5)
	#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDE | ATTR_SYSTEM | ATTR_VOLUME_ID)

	#define LOWERCASE_BASE (8)
	#define LOWERCASE_EXT (16)

	typedef struct FAT32_SBinfo
	{
		u64	start_sector;
		u64	sector_count;

		u64	sector_per_cluster;
		u64	bytes_per_cluster;
		u64	bytes_per_sector;

		u64	Data_firstsector;
		u64	FAT1_firstsector;
		u64	sector_per_FAT;
		u64	NumFATs;

		u64	fsinfo_sector_infat;
		u64	bootsector_bk_infat;

		FAT32_FSinfo_s *	fat_fsinfo;
	} FAT32_SBinfo_s;

	typedef struct FAT32_inode_info
	{
		u64	first_cluster;
		u64	dentry_location;	// dentry struct in cluster(0 is root,1 is invalid)
		u64	dentry_position;	// dentry struct offset in cluster
		u64 dentry_start;		// the whole dentry start offset in directory
		u64 dentry_length;		// the whole dentry length

		u16	create_date;
		u16	create_time;

		u16	write_date;
		u16	write_time;
	} FAT32_inode_info_s;

	void init_fat32_fs(void);
	bool FAT32_ent_empty(const msdos_dirent_s *de);
	char *FAT32_get_shortname(int *namelen, const msdos_dirent_s *de);
	u32 FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry);
	u64 FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry, u32 value);
	u32 FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi);
	s64 FAT32_alloc_new_dir(inode_s *dir);
	int FAT32_dir_empty(inode_s *dir);

	static inline sector_t FAT32_clus_to_blknr(FAT32_SBinfo_s *fsbi, int clus)
	{
		return ((sector_t)clus - FAT_START_ENT) * fsbi->sector_per_cluster
			+ fsbi->Data_firstsector;
	}

	#define FAT32_IOBUF_DIRTY		1
	#define FAT32_IOBUF_DELETE		2
	#define FAT32_IOBUF_NEWCLUS		3

	typedef struct cluster_list
	{
		List_s	list;
		u32		cluster;
	} clus_list_s;

	typedef struct FAT32_IO_BUFFER
	{
		int		buf_nr;
		size_t	bufsize;

		u32		*clusters;
		u32		*flags;
		char	**buffers;

		loff_t	iter_cursor;
		FAT32_SBinfo_s	*fsbi;

		int		(*iter_init) (FAT32_iobuf_s *this);
		const msdos_dirent_s	*(*next) (FAT32_iobuf_s *this);
	} FAT32_iobuf_s;
	
	FAT32_iobuf_s *FAT32_iobuf_init(inode_s *dir);
	const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off);
	int FAT32_iobuf_write(FAT32_iobuf_s *iobuf, loff_t off, char *content, size_t size);
	void FAT32_iobuf_release(FAT32_iobuf_s *iobuf);
	const msdos_dirent_s *FAT32_get_full_ent(FAT32_iobuf_s *iobuf);
	char *FAT32_get_longname(int *namelen, FAT32_iobuf_s *iobuf);

#endif /* _FAT32_H_ */
