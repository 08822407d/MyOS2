---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-DIR-ITERATOR-35
record_type: fixed_time_direct_definition_evidence
date: 2026-11-04
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 第35批：直接源码定义体锚点

以下56个短引文按固定time源码生成；每个标记只对应一个实际函数、结构体或宏定义的1至5行。描述完整流程时必须合读相邻引文及所选函数完整定义。这里只证明逐字静态语句，不证明编译绑定、实际磁盘数据或运行正确性。

### IT35-A001｜get_cluster_chain（24–28行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
	FAT32_SBinfo_s * fsbi = inode->i_sb->private_sb_info;
	unsigned long cluster = finode->first_cluster;
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s), GFP_KERNEL);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);
```

### IT35-A002｜get_cluster_chain（36–40行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			if (clus_sp == NULL)
			{
				free_cluster_chain(clus_lhdrp);
				return ERR_PTR(-ENOMEM);
			}
```

### IT35-A003｜get_cluster_chain（41–45行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			INIT_LIST_S(&clus_sp->list);

			clus_sp->cluster = cluster;
			list_header_add_to_tail(clus_lhdrp, &clus_sp->list);
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);
```

### IT35-A004｜get_cluster_chain（48–52行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
		if (cluster == FAT_ENT_BAD)
		{
			free_cluster_chain(clus_lhdrp);
			clus_lhdrp = ERR_PTR(-EIO);
		}
```

### IT35-A005｜get_cluster_chain（53–55行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
	}
	return clus_lhdrp;
}
```

### IT35-A006｜FAT32_iobuf_iter_init（57–61行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_init]
```c
int FAT32_iobuf_iter_init(FAT32_iobuf_s *iobuf)
{
	iobuf->iter_cursor = 0;
	iobuf->iter_reach_end = false;
	return -ENOERR;
```

### IT35-A007｜FAT32_iobuf_readent（64–68行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
```

### IT35-A008｜FAT32_iobuf_readent（69–71行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
```

### IT35-A009｜FAT32_iobuf_readent（71–75行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			return ERR_PTR(-ENOMEM);
```

### IT35-A010｜FAT32_iobuf_readent（77–80行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
		ROOTBLK_TRANSFER(CMD_READ,
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
```

### IT35-A011｜FAT32_iobuf_readent（80–84行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
		iobuf->buffers[bufidx] = buf;
	}

	return (msdos_dirent_s *)&(iobuf->buffers[bufidx][bufoff]);
}
```

### IT35-A012｜FAT32_iobuf_iter_next（115–119行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
const msdos_dirent_s *FAT32_iobuf_iter_next(FAT32_iobuf_s *iobuf)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

```

### IT35-A013｜FAT32_iobuf_iter_next（120–124行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
		iobuf->iter_reach_end = true;

	return ret_val;
```

### IT35-A014｜FAT32_iobuf_init（167–171行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	int error = 0;
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;
	size_t bufsize = fsbi->bytes_per_cluster;
	size_t max_nr = FAT32_MAX_FILE_SIZE / bufsize;
```

### IT35-A015｜FAT32_iobuf_init（173–177行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);

	int count = clus_lhdrp->count;
```

### IT35-A016｜FAT32_iobuf_init（179–183行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	if (iobuf == NULL)
	{
		error = -ENOMEM;
		goto alloc_iobuf_fail;
	}
```

### IT35-A017｜FAT32_iobuf_init（184–187行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->nr_clus = count;
	iobuf->max_nr_clus = max_nr;
	iobuf->bufsize = bufsize;
	if (count > 0)
```

### IT35-A018｜FAT32_iobuf_init（189–193行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->buffers = kzalloc(max_nr * sizeof(char *), GFP_KERNEL);
		if (iobuf->buffers == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_bufps_fail;
```

### IT35-A019｜FAT32_iobuf_init（195–199行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->clusters = kzalloc(max_nr * sizeof(*iobuf->clusters), GFP_KERNEL);
		if (iobuf->clusters == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_clusters_fail;
```

### IT35-A020｜FAT32_iobuf_init（201–205行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->flags = kzalloc(max_nr * sizeof(*iobuf->flags), GFP_KERNEL);
		if (iobuf->flags == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_flags_fail;
```

### IT35-A021｜FAT32_iobuf_init（209–213行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	int i = 0;
	while (clus_lhdrp->count > 0)
	{
		List_s *lp = list_header_remove_tail(clus_lhdrp);
		clus_list_s *clus_lp = container_of(lp, clus_list_s, list);
```

### IT35-A022｜FAT32_iobuf_init（214–218行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->clusters[i] = clus_lp->cluster;
		iobuf->flags[i] = 0;
		kfree(clus_lp);
		i++;
	}
```

### IT35-A023｜FAT32_iobuf_init（219–223行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->fsbi = fsbi;
	iobuf->iter_cursor = -1;
	iobuf->iter_init = FAT32_iobuf_iter_init;
	iobuf->next = FAT32_iobuf_iter_next;
	return iobuf;
```

### IT35-A024｜FAT32_iobuf_init（225–229行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
alloc_iobuf_flags_fail:
	kfree(iobuf->clusters);
alloc_iobuf_clusters_fail:
	kfree(iobuf->buffers);
alloc_iobuf_bufps_fail:
```

### IT35-A025｜FAT32_iobuf_init（229–233行）

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
alloc_iobuf_bufps_fail:
	kfree(iobuf);
alloc_iobuf_fail:
	free_cluster_chain(clus_lhdrp);
	return ERR_PTR(error);
```

### IT35-A026｜FAT32_ent_empty（28–32行）

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_ent_empty]
```c
bool FAT32_ent_empty(const msdos_dirent_s *de)
{
	return (de->name[0] == 0xe5 ||
			de->name[0] == 0x00 ||
			de->name[0] == 0x05);
```

### IT35-A027｜FAT32_get_full_ent（45–49行）

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
const msdos_dirent_s *FAT32_get_full_ent(FAT32_iobuf_s *iobuf)
{
	size_t len = 0;
	const msdos_dirent_s *ret_val;
	const msdos_dirslot_s *tmpval;
```

### IT35-A028｜FAT32_get_full_ent（50–54行）

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
	do
	{
		ret_val = iobuf->next(iobuf);
		tmpval = (msdos_dirslot_s *)ret_val;
	}
```

### IT35-A029｜FAT32_get_full_ent（55–57行）

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
	while ((ret_val->attr == ATTR_LONG_NAME ||
			FAT32_ent_empty(ret_val)) &&
			!iobuf->iter_reach_end);
```

### IT35-A030｜FAT32_get_full_ent（59–62行）

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
	if (iobuf->iter_reach_end)
		ret_val = ERR_PTR(-ENOENT);

	return ret_val;
```

### IT35-A031｜FAT32_lookup（26–30行）

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	int entlen = sizeof(msdos_dirent_s);
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(parent_inode);
	iobuf->iter_init(iobuf);
```

### IT35-A032｜FAT32_lookup（32–34行）

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	while ((de_start = iobuf->iter_cursor) < FAT_MAX_DIR_SIZE &&
			!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
```

### IT35-A033｜FAT32_lookup（35–37行）

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;
```

### IT35-A034｜FAT32_lookup（42–46行）

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		if (FAT32_ent_empty(tmpde))
			continue;
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
```

### IT35-A035｜FAT32_lookup（58–62行）

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	if (IS_ERR(tmpde))
	{
		dest_dentry = NULL;
		goto ent_not_found;
	}
```

### IT35-A036｜FAT32_MAX_FILE_SIZE（143–143行）

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_MAX_FILE_SIZE]
```c
	#define FAT32_MAX_FILE_SIZE		(SZ_1M * 64)
```

### IT35-A037｜SZ_1M（32–32行）

[VERIFIED mykernel/include/linux/kernel/sizes.h::SZ_1M]
```c
	#define SZ_1M			0x00100000
```

### IT35-A038｜FAT32_iobuf_s（155–159行）

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
	typedef struct FAT32_IO_BUFFER
	{
		int		nr_clus;
		int		max_nr_clus;
		size_t	bufsize;
```

### IT35-A039｜FAT32_iobuf_s（161–165行）

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		u32		*clusters;
		u32		*flags;
		char	**buffers;

		loff_t	iter_cursor;
```

### IT35-A040｜FAT32_iobuf_s（166–170行）

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		bool	iter_reach_end;
		FAT32_SBinfo_s	*fsbi;

		int		(*iter_init) (FAT32_iobuf_s *this);
		const msdos_dirent_s	*(*next) (FAT32_iobuf_s *this);
```

### IT35-A041｜MSDOS_DIR_BITS（29–29行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_DIR_BITS]
```c
	#define MSDOS_DIR_BITS		5			/* log2(sizeof(struct msdos_dir_entry)) */
```

### IT35-A042｜FAT_MAX_DIR_ENTRIES（32–32行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_MAX_DIR_ENTRIES]
```c
	#define FAT_MAX_DIR_ENTRIES	(65536)
```

### IT35-A043｜FAT_MAX_DIR_SIZE（33–33行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_MAX_DIR_SIZE]
```c
	#define FAT_MAX_DIR_SIZE	(FAT_MAX_DIR_ENTRIES << MSDOS_DIR_BITS)
```

### IT35-A044｜msdos_dirent_s（171–175行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
	typedef struct msdos_dir_entry {
		__u8	name[MSDOS_NAME];	/* name and extension 
										-DIR_Name- */
		__u8	attr;				/* attribute bits
										-DIR_Attr- */
```

### IT35-A045｜msdos_dirent_s（180–184行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
		__le16	ctime;				/* Creation time
										-DIR_CrtTime- */
		__le16	cdate;				/* Creation date
										-DIR_CrtDate- */
		__le16	adate;				/* Last access date
```

### IT35-A046｜msdos_dirent_s（188–192行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
		__le16	time,date,start;	/* time, date and first cluster
										-DIR_WrtTime-
										-DIR_WrtDate-
										-DIR_FstClusLO- */
		__le32	size;				/* file size (in bytes)
```

### IT35-A047｜msdos_dirent_s（192–194行）

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
		__le32	size;				/* file size (in bytes)
										-DIR_FileSize- */
	}__attribute__((packed)) msdos_dirent_s;
```

### IT35-A048｜ATTR_LONG_NAME（81–81行）

[VERIFIED mykernel/include/linux/fs/fat32.h::ATTR_LONG_NAME]
```c
	#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDE | ATTR_SYSTEM | ATTR_VOLUME_ID)
```

### IT35-A049｜MAX_ERRNO（15–15行）

[VERIFIED mykernel/include/linux/kernel/err.h::MAX_ERRNO]
```c
	#define MAX_ERRNO	4095
```

### IT35-A050｜IS_ERR_VALUE（18–20行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

### IT35-A051｜ERR_PTR（26–29行）

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

### IT35-A052｜IS_ERR（36–39行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### IT35-A053｜ENOENT（8–8行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOENT]
```c
	#define	ENOENT		2	/* No such file or directory */
```

### IT35-A054｜ENOMEM（18–18行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

### IT35-A055｜EIO（11–11行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EIO]
```c
	#define	EIO			5	/* I/O error */
```

### IT35-A056｜ENOERR（5–5行）

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

