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

struct FAT32_BootSector
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
}__attribute__((packed));


struct FAT32_FSInfo
{
	uint32_t	FSI_LeadSig;
	uint8_t		FSI_Reserved1[480];
	uint32_t	FSI_StrucSig;
	uint32_t	FSI_Free_Count;
	uint32_t	FSI_Nxt_Free;
	uint8_t		FSI_Reserved2[12];
	uint32_t	FSI_TrailSig;
}__attribute__((packed));

#define	ATTR_READ_ONLY	(1 << 0)
#define ATTR_HIDDEN	(1 << 1)
#define ATTR_SYSTEM	(1 << 2)
#define ATTR_VOLUME_ID	(1 << 3)
#define ATTR_DIRECTORY	(1 << 4)
#define ATTR_ARCHIVE	(1 << 5)
#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

struct FAT32_Directory
{
	uint8_t		DIR_Name[11];
	uint8_t		DIR_Attr;
	uint8_t		DIR_NTRes;	//EXT|BASE => 8(BASE).3(EXT)
					//BASE:LowerCase(8),UpperCase(0)
					//EXT:LowerCase(16),UpperCase(0)
	uint8_t		DIR_CrtTimeTenth;
	uint16_t	DIR_CrtTime;	
	uint16_t	DIR_CrtDate;
	uint16_t	DIR_LastAccDate;
	uint16_t	DIR_FstClusHI;
	uint16_t	DIR_WrtTime;
	uint16_t	DIR_WrtDate;
	uint16_t	DIR_FstClusLO;
	uint32_t	DIR_FileSize;
}__attribute__((packed));

#define LOWERCASE_BASE (8)
#define LOWERCASE_EXT (16)

struct FAT32_LongDirectory
{
	uint8_t		LDIR_Ord;
	uint16_t	LDIR_Name1[5];
	uint8_t		LDIR_Attr;
	uint8_t		LDIR_Type;
	uint8_t		LDIR_Chksum;
	uint16_t	LDIR_Name2[6];
	uint16_t	LDIR_FstClusLO;
	uint16_t	LDIR_Name3[2];
}__attribute__((packed));

void DISK1_FAT32_FS_init();

/////////////FAT32 for VFS

struct FAT32_sb_info
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
	
	struct FAT32_FSInfo * fat_fsinfo;
};

struct FAT32_inode_info
{
	uint64_t	first_cluster;
	uint64_t	dentry_location;	////dentry struct in cluster(0 is root,1 is invalid)
	uint64_t	dentry_position;	////dentry struct offset in cluster

	uint16_t	create_date;
	uint16_t	create_time;

	uint16_t	write_date;
	uint16_t	write_time;
};

uint32_t DISK1_FAT32_read_FAT_Entry(struct FAT32_sb_info * fsbi,uint32_t	fat_entry);
uint64_t DISK1_FAT32_write_FAT_Entry(struct FAT32_sb_info * fsbi,uint32_t	fat_entry,uint32_t	value);
uint64_t FAT32_find_available_cluster(struct FAT32_sb_info * fsbi);

extern struct index_node_operations	FAT32_inode_ops;
extern struct file_operations		FAT32_file_ops;
extern struct dir_entry_operations	FAT32_dentry_ops;
extern struct super_block_operations	FAT32_sb_ops;

#endif /* _FAT32_H_ */
