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

#include <sys/types.h>

	typedef struct FAT32_BS
	{
		uint8_t		BS_jmpBoot[3];
		uint8_t		BS_OEMName[8];
		uint16_t	BPB_BytesPerSec;
		uint8_t		BPB_SecPerClus;
		uint16_t	BPB_RsvdSecCnt;
		uint8_t		BPB_NumFATs;
		uint16_t	BPB_RootEntCnt;
		uint16_t	BPB_TotSec16;
		uint8_t		BPB_Media;
		uint16_t	BPB_FATSz16;
		uint16_t	BPB_SecPerTrk;
		uint16_t	BPB_NumHeads;
		uint32_t	BPB_HiddSec;
		uint32_t	BPB_TotSec32;

		uint32_t	BPB_FATSz32;
		uint16_t	BPB_ExtFlags;
		uint16_t	BPB_FSVer;
		uint32_t	BPB_RootClus;
		uint16_t	BPB_FSInfo;
		uint16_t	BPB_BkBootSec;
		uint8_t		BPB_Reserved[12];

		uint8_t		BS_DrvNum;
		uint8_t		BS_Reserved1;
		uint8_t		BS_BootSig;
		uint32_t	BS_VolID;
		uint8_t		BS_VolLab[11];
		uint8_t		BS_FilSysType[8];

		uint8_t		BootCode[420];

		uint16_t	BS_TrailSig;
	}__attribute__((packed)) FAT32_BS_s;


	typedef struct FAT32_FSinfo
	{
		uint32_t	FSI_LeadSig;
		uint8_t		FSI_Reserved1[480];
		uint32_t	FSI_StrucSig;
		uint32_t	FSI_Free_Count;
		uint32_t	FSI_Nxt_Free;
		uint8_t		FSI_Reserved2[12];
		uint32_t	FSI_TrailSig;
	}__attribute__((packed)) FAT32_FSinfo_s;

	#define	ATTR_READ_ONLY	(1 << 0)
	#define ATTR_HIDDEN	(1 << 1)
	#define ATTR_SYSTEM	(1 << 2)
	#define ATTR_VOLUME_ID	(1 << 3)
	#define ATTR_DIRECTORY	(1 << 4)
	#define ATTR_ARCHIVE	(1 << 5)
	#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

	typedef struct FAT32_dir
	{
		uint8_t		DIR_Name[11];
		uint8_t		DIR_Attr;
		uint8_t		DIR_NTRes;	// EXT|BASE => 8(BASE).3(EXT)
								// BASE:LowerCase(8),UpperCase(0)
								// EXT:LowerCase(16),UpperCase(0)
		uint8_t		DIR_CrtTimeTenth;
		uint16_t	DIR_CrtTime;	
		uint16_t	DIR_CrtDate;
		uint16_t	DIR_LastAccDate;
		uint16_t	DIR_FstClusHI;
		uint16_t	DIR_WrtTime;
		uint16_t	DIR_WrtDate;
		uint16_t	DIR_FstClusLO;
		uint32_t	DIR_FileSize;
	}__attribute__((packed)) FAT32_dir_s;

	#define LOWERCASE_BASE (8)
	#define LOWERCASE_EXT (16)

	typedef struct FAT32_ldir
	{
		uint8_t		LDIR_Ord;
		uint16_t	LDIR_Name1[5];
		uint8_t		LDIR_Attr;
		uint8_t		LDIR_Type;
		uint8_t		LDIR_Chksum;
		uint16_t	LDIR_Name2[6];
		uint16_t	LDIR_FstClusLO;
		uint16_t	LDIR_Name3[2];
	}__attribute__((packed)) FAT32_ldir_s;

	typedef struct FAT32_SBinfo
	{
		uint64_t	start_sector;
		uint64_t	sector_count;

		uint64_t	sector_per_cluster;
		uint64_t	bytes_per_cluster;
		uint64_t	bytes_per_sector;

		uint64_t	Data_firstsector;
		uint64_t	FAT1_firstsector;
		uint64_t	sector_per_FAT;
		uint64_t	NumFATs;

		uint64_t	fsinfo_sector_infat;
		uint64_t	bootsector_bk_infat;

		FAT32_FSinfo_s *	fat_fsinfo;
	} FAT32_SBinfo_s;

	typedef struct FAT32_inode_info
	{
		uint64_t	first_cluster;
		uint64_t	dentry_location;	// dentry struct in cluster(0 is root,1 is invalid)
		uint64_t	dentry_position;	// dentry struct offset in cluster

		uint16_t	create_date;
		uint16_t	create_time;

		uint16_t	write_date;
		uint16_t	write_time;
	} FAT32_inode_info_s;

	void init_FAT32_FS(void);
	uint32_t FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry);
	uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, uint32_t fat_entry, uint32_t value);
	uint64_t FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi);

#endif /* _FAT32_H_ */
