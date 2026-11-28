---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-NAME-PARSE-36
record_type: fixed_time_single_definition_direct_quotes
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 第36批：长短名解析与直接调用者的定义体短引文

每一段均取自固定time中一个实际函数、结构体或宏的定义体，或者用于该函数选择的真实条件编译门；长度1–5行、未改写。标题只表示所属定义及固定行号；语义必须合读完整函数，不以extern/include或字段注释冒充执行证据。原35的56段及34的64段均不计入本批新引用。

### NP36-A001｜FAT32_parse_short，第13–17行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kzalloc(15, GFP_KERNEL);
```

### NP36-A002｜FAT32_parse_short，第20–23行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
	for(x=0; x < 8; x++)
	{
		if(de->name[x] == ' ')
			break;
```

### NP36-A003｜FAT32_parse_short，第24–27行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
		if(de->lcase & LOWERCASE_BASE)
			ret_val[(*namelen)++] = de->name[x] + 32;
		else
			ret_val[(*namelen)++] = de->name[x];
```

### NP36-A004｜FAT32_parse_short，第30–32行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
	if(!(de->attr & ATTR_DIRECTORY))
	{
		ret_val[(*namelen)++] = '.';
```

### NP36-A005｜FAT32_parse_short，第35–39行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
		for(x=8; x < 11; x++)
		{
			if(de->name[x] == ' ')
				break;
			if(de->lcase & LOWERCASE_EXT)
```

### NP36-A006｜FAT32_parse_short，第39–43行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
			if(de->lcase & LOWERCASE_EXT)
				ret_val[(*namelen)++] = de->name[x] + 32;
			else
				ret_val[(*namelen)++] = de->name[x];
		}
```

### NP36-A007｜FAT32_parse_short，第44–46行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
		if(x == 8)
			ret_val[--(*namelen)] = 0;
	}
```

### NP36-A008｜FAT32_parse_short，第47–49行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c

	return ret_val;
}
```

### NP36-A009｜FAT32_parse_long，第51–55行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
char *FAT32_parse_long(int *namelen, FAT32_iobuf_s *iobuf)
{
	int i = 0, x, y;
	char *ret_val;
	int entlen = sizeof(msdos_dirent_s);
```

### NP36-A010｜FAT32_parse_long，第56–60行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	loff_t cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	while(lde_tmp->attr == ATTR_LONG_NAME &&
		!FAT32_ent_empty((msdos_dirent_s *)lde_tmp) &&
		cursor >= 0)
```

### NP36-A011｜FAT32_parse_long，第58–60行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	while(lde_tmp->attr == ATTR_LONG_NAME &&
		!FAT32_ent_empty((msdos_dirent_s *)lde_tmp) &&
		cursor >= 0)
```

### NP36-A012｜FAT32_parse_long，第61–64行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	{
		i++;
		if(lde_tmp->id & 0x40)
			break;
```

### NP36-A013｜FAT32_parse_long，第65–67行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		cursor -= entlen;
		lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
	}
```

### NP36-A014｜FAT32_parse_long，第69–72行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	//long file/dir name read
	ret_val = kzalloc(i * 13 + 1, GFP_KERNEL);
	cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
```

### NP36-A015｜FAT32_parse_long，第73–73行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	for(x = 0; x < i; x++, cursor -= entlen, lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor))
```

### NP36-A016｜FAT32_parse_long，第75–78行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 10; y += 2)
			if(*(u16 *)(&lde->name0_4[y]) != 0xffff &&
				*(u16 *)(&lde->name0_4[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name0_4[y];
```

### NP36-A017｜FAT32_parse_long，第80–83行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 12; y += 2)
			if(*(u16 *)(&lde->name5_10[y]) != 0xffff &&
				*(u16 *)(&lde->name5_10[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name5_10[y];
```

### NP36-A018｜FAT32_parse_long，第85–88行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 4; y += 2)
			if(*(u16 *)(&lde->name11_12[y]) != 0xffff &&
				*(u16 *)(&lde->name11_12[y]) != 0x0000)
				ret_val[(*namelen)++] = (char)lde->name11_12[y];
```

### NP36-A019｜FAT32_parse_long，第89–92行

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	}

	return ret_val;
}
```

### NP36-A020｜FAT32_lookup，第21–24行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	int destlen = dest_dentry->d_name.len;
	const char * destname = dest_dentry->d_name.name;
	loff_t off = 0;
	loff_t de_start = 0;
```

### NP36-A021｜FAT32_lookup，第26–30行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	int entlen = sizeof(msdos_dirent_s);
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(parent_inode);
	iobuf->iter_init(iobuf);
```

### NP36-A022｜FAT32_lookup，第32–36行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	while ((de_start = iobuf->iter_cursor) < FAT_MAX_DIR_SIZE &&
			!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
```

### NP36-A023｜FAT32_lookup，第35–37行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;
```

### NP36-A024｜FAT32_lookup，第39–43行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		char *name = NULL;
		int namelen = 0;

		if (FAT32_ent_empty(tmpde))
			continue;
```

### NP36-A025｜FAT32_lookup，第44–48行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_parse_long(&namelen, iobuf);
		else
```

### NP36-A026｜FAT32_lookup，第47–49行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
			name = FAT32_parse_long(&namelen, iobuf);
		else
			name = FAT32_parse_short(&namelen, tmpde);
```

### NP36-A027｜FAT32_lookup，第51–55行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		int len = namelen > destlen ? namelen : destlen;
		bool equal = !strncmp(name, destname, len);
		kfree(name);
		if (equal)
			break;
```

### NP36-A028｜FAT32_lookup，第58–62行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	if (IS_ERR(tmpde))
	{
		dest_dentry = NULL;
		goto ent_not_found;
	}
```

### NP36-A029｜FAT32_lookup，第92–95行

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
build_inode_fail:
ent_not_found:
	FAT32_iobuf_release(iobuf);
	return dest_dentry;	
```

### NP36-A030｜ATTR_DIRECTORY，第79–79行

[VERIFIED mykernel/include/linux/fs/fat32.h::ATTR_DIRECTORY]
```c
	#define ATTR_DIRECTORY	(1 << 4)
```

### NP36-A031｜ATTR_LONG_NAME，第81–81行

[VERIFIED mykernel/include/linux/fs/fat32.h::ATTR_LONG_NAME]
```c
	#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDE | ATTR_SYSTEM | ATTR_VOLUME_ID)
```

### NP36-A032｜LOWERCASE_BASE/LOWERCASE_EXT，第83–84行

[VERIFIED mykernel/include/linux/fs/fat32.h::LOWERCASE_BASE/LOWERCASE_EXT]
```c
	#define LOWERCASE_BASE (8)
	#define LOWERCASE_EXT (16)
```

### NP36-A033｜FAT32_iobuf_s，第155–159行

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
	typedef struct FAT32_IO_BUFFER
	{
		int		nr_clus;
		int		max_nr_clus;
		size_t	bufsize;
```

### NP36-A034｜FAT32_iobuf_s，第161–165行

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		u32		*clusters;
		u32		*flags;
		char	**buffers;

		loff_t	iter_cursor;
```

### NP36-A035｜FAT32_iobuf_s，第166–170行

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		bool	iter_reach_end;
		FAT32_SBinfo_s	*fsbi;

		int		(*iter_init) (FAT32_iobuf_s *this);
		const msdos_dirent_s	*(*next) (FAT32_iobuf_s *this);
```

### NP36-A036｜MSDOS_NAME，第54–56行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_NAME]
```c
	#define FAT_LFN_LEN			255				/* maximum long name length */
	#define MSDOS_NAME			11				/* maximum name length */
	#define MSDOS_SLOTS			21				/* max # of slots for short and long names */
```

### NP36-A037｜msdos_dirent_s，第171–175行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
	typedef struct msdos_dir_entry {
		__u8	name[MSDOS_NAME];	/* name and extension 
										-DIR_Name- */
		__u8	attr;				/* attribute bits
										-DIR_Attr- */
```

### NP36-A038｜msdos_dirent_s，第175–177行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
										-DIR_Attr- */
		__u8	lcase;				/* Case for base and extension
										-DIR_NTRes- */
```

### NP36-A039｜msdos_dirent_s，第191–194行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
										-DIR_FstClusLO- */
		__le32	size;				/* file size (in bytes)
										-DIR_FileSize- */
	}__attribute__((packed)) msdos_dirent_s;
```

### NP36-A040｜msdos_dirslot_s，第197–201行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
	typedef struct msdos_dir_slot {
		__u8	id;					/* sequence number for slot
										-LDIR_Ord- */
		__u8	name0_4[10];		/* first 5 characters in name
										-LDIR_Name1- */
```

### NP36-A041｜msdos_dirslot_s，第204–208行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__u8	reserved;			/* always 0
										-LDIR_Type- */
		__u8	alias_checksum;		/* checksum for 8.3 alias
										-LDIR_Chksum- */
		__u8	name5_10[12];		/* 6 more characters in name
```

### NP36-A042｜msdos_dirslot_s，第210–214行

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__le16	start;				/* starting cluster number, 0 in long slots
										-LDIR_FstClusLO- */
		__u8	name11_12[4];		/* last 2 characters in name
										-LDIR_Name3- */
	}__attribute__((packed)) msdos_dirslot_s;
```

### NP36-A043｜strncmp conditional compiler gate，第354–354行

[VERIFIED mykernel/lib/string/string.c::strncmp conditional compiler gate]
```c
#ifndef __HAVE_ARCH_STRNCMP
```

### NP36-A044｜strncmp，第361–365行

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
	int strncmp(const char *cs, const char *ct, size_t count)
	{
		unsigned char c1, c2;
		while (count) {
			c1 = *cs++;
```

### NP36-A045｜strncmp，第365–369行

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			c1 = *cs++;
			c2 = *ct++;
			if (c1 != c2)
				return c1 < c2 ? -1 : 1;
			if (!c1)
```

### NP36-A046｜strncmp，第369–373行

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			if (!c1)
				break;
			count--;
		}
		return 0;
```

### NP36-A047｜kzalloc，第168–172行

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```

### NP36-A048｜FAT32_iobuf_readent，第64–68行

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
```

### NP36-A049｜FAT32_iobuf_readent，第69–73行

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
```

### NP36-A050｜FAT32_iobuf_readent，第73–75行

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			return ERR_PTR(-ENOMEM);
```

### NP36-A051｜FAT32_iobuf_readent，第77–80行

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
		ROOTBLK_TRANSFER(CMD_READ,
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
```

### NP36-A052｜FAT32_iobuf_readent，第82–84行

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c

	return (msdos_dirent_s *)&(iobuf->buffers[bufidx][bufoff]);
}
```

### NP36-A053｜ERR_PTR，第26–29行

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

### NP36-A054｜IS_ERR，第36–39行

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### NP36-A055｜ENOENT，第8–8行

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOENT]
```c
	#define	ENOENT		2	/* No such file or directory */
```

### NP36-A056｜ENOMEM，第18–18行

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

