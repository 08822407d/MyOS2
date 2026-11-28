---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-LOOKUP-CALLBACK-34
record_type: fixed_time_direct_definition_quote_ledger
date: 2026-11-03
source_ref: time
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
status: READY_FOR_MAINLINE_REVIEW
---

# 第34批直接源码证据

本轮单定义体直接引文64段，每段一至五行；来源为固定time源码。标签标题只限引文中实际可见的语句，不凭声明、注释或include代替定义体。行号是本次读取定位，不代表源码在其它分支或以后稳定；图、有限模型与源码运行需分开认定。

### CB34-A001｜上层分配原对象并检查错误

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
				dentry = __myos_d_alloc(dir->d_sb, name);
	//		}
	// }
	if (IS_ERR(dentry))
		return dentry;
```

固定源码定位：`mykernel/fs/vfs/namei.c`，本轮第320–324行。

### CB34-A002｜上层只将回调结果与NULL比较

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	if (dir->d_inode->i_op->lookup(dir->d_inode, dentry, flags) == NULL)
	{
		// color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->d_name);
		kfree(dentry);
		return ERR_PTR(-ENOENT);
```

固定源码定位：`mykernel/fs/vfs/namei.c`，本轮第326–330行。

### CB34-A003｜上层从原对象读取inode和mapping

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	inode = dentry->d_inode;
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
		inode->i_mapping->page_array =
			kzalloc(inode->i_mapping->nrpages * sizeof(page_s *), GFP_KERNEL);
```

固定源码定位：`mykernel/fs/vfs/namei.c`，本轮第333–337行。

### CB34-A004｜上层返回原dentry

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	list_header_add_to_tail(&dir->d_subdirs, &dentry->d_child);
	dentry->d_parent = dir;

	return dentry;
```

固定源码定位：`mykernel/fs/vfs/namei.c`，本轮第339–342行。

### CB34-A005｜函数指针的真实类型

[VERIFIED mykernel/include/linux/fs/fs.h::inode_ops_s]
```c
	typedef struct inode_operations {
		dentry_s	*(*lookup) (inode_s *, dentry_s *, unsigned int);
		// const char	*(*get_link) (dentry_s *, inode_s *, struct delayed_call *);
```

固定源码定位：`mykernel/include/linux/fs/fs.h`，本轮第1910–1912行。

### CB34-A006｜inode的操作表和映射字段

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		const inode_ops_s	*i_op;
		super_block_s		*i_sb;
		addr_spc_s			*i_mapping;
```

固定源码定位：`mykernel/include/linux/fs/fs.h`，本轮第560–562行。

### CB34-A007｜superblock私有字段

[VERIFIED mykernel/include/linux/fs/fs.h::super_block_s]
```c
	typedef struct super_block {
		void				*private_sb_info;


```

固定源码定位：`mykernel/include/linux/fs/fs.h`，本轮第1331–1334行。

### CB34-A008｜VFAT操作表中的lookup绑定

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::vfat_dir_inode_operations]
```c
inode_ops_s vfat_dir_inode_operations = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第199–203行。

### CB34-A009｜目录分支判断

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	if ((de->attr & ATTR_DIR) && !FAT32_IS_FREE(de->name)) {
		// inode->i_generation &= ~1;
		// inode->i_mode	= fat_make_mode(sbi, de->attr, S_IRWXUGO);
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第151–153行。

### CB34-A010｜目录分支设置操作表

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		// inode->i_op		= sbi->dir_ops;
		// inode->i_fop = &fat_dir_operations;
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第154–157行。

### CB34-A011｜非目录分支设置大小和操作表

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		inode->i_size	= de->size;
		// inode->i_op = &fat_file_inode_operations;
		// inode->i_fop = &fat_file_operations;
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第175–179行。

### CB34-A012｜非目录分支设置mapping操作

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		inode->i_mapping->a_ops = &fat_aops;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第180–180行。

### CB34-A013｜fat_fill_inode函数局部

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
/* doesn't deal with root inode */
int fat_fill_inode(inode_s *inode, msdos_dirent_s *de)
{
	// msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	int error;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第138–142行。

### CB34-A014｜将inode.mode先清零

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	inode->i_mode = 0;
	MSDOS_I(inode)->i_pos = 0;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第144–145行。

### CB34-A015｜该函数正常结束返回零

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	// 	inode->i_atime = fat_truncate_atime(sbi, &inode->i_mtime);

	return 0;
}
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第199–202行。

### CB34-A016｜setup设置根目录操作表来源

[VERIFIED mykernel/fs/fat/namei_vfat.c::setup]
```c
static void setup(super_block_s *sb)
{
	MSDOS_SB(sb)->dir_ops = &vfat_dir_inode_operations;
	// if (MSDOS_SB(sb)->options.name_check != 's')
```

固定源码定位：`mykernel/fs/fat/namei_vfat.c`，本轮第288–291行。

### CB34-A017｜fat_read_root直接赋根inode操作表

[VERIFIED mykernel/fs/fat/inode.c::fat_read_root]
```c
	// inode->i_generation = 0;
	inode->i_mode = fat_make_mode(sbi, ATTR_DIR, S_IRWXUGO);
	inode->i_op = (inode_ops_s *)sbi->dir_ops;
	inode->i_fop = (file_ops_s *)&fat_dir_operations;
	if (is_fat32(sbi)) {
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第325–329行。

### CB34-A018｜回调形参与局部对象

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
dentry_s *FAT32_lookup(inode_s * parent_inode, dentry_s * dest_dentry, unsigned int flags)
{
	int error = 0;
	inode_s * p = NULL;
	FAT32_inode_info_s * finode = NULL;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第14–18行。

### CB34-A019｜父inode的私有superblock指针

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第19–19行。

### CB34-A020｜查询名字长度及偏移变量

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	int destlen = dest_dentry->d_name.len;
	const char * destname = dest_dentry->d_name.name;
	loff_t off = 0;
	loff_t de_start = 0;

```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第21–25行。

### CB34-A021｜临时目录项和iobuf初始化

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	int entlen = sizeof(msdos_dirent_s);
	const msdos_dirent_s *tmpde = NULL;
	const msdos_dirslot_s *tmplde = NULL;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(parent_inode);
	iobuf->iter_init(iobuf);
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第26–30行。

### CB34-A022｜循环上界和获取目录项的短路条件

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	while ((de_start = iobuf->iter_cursor) < FAT_MAX_DIR_SIZE &&
			!IS_ERR(tmpde = FAT32_get_full_ent(iobuf)))
	{
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第32–36行。

### CB34-A023｜循环内目录项指针与off

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		tmplde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf,
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第35–37行。

### CB34-A024｜循环内每次重新初始化解析名

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		char *name = NULL;
		int namelen = 0;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第39–40行。

### CB34-A025｜空项continue以及长名谓词

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		if (FAT32_ent_empty(tmpde))
			continue;
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第42–46行。

### CB34-A026｜长名/短名解析二选一

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_parse_long(&namelen, iobuf);
		else
			name = FAT32_parse_short(&namelen, tmpde);
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第45–49行。

### CB34-A027｜取两个int长度的较大值来比较并break

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		int len = namelen > destlen ? namelen : destlen;
		bool equal = !strncmp(name, destname, len);
		kfree(name);
		if (equal)
			break;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第51–55行。

### CB34-A028｜循环后检查IS_ERR(tmpde)

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	if (IS_ERR(tmpde))
	{
		dest_dentry = NULL;
		goto ent_not_found;
	}
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第58–62行。

### CB34-A029｜构造inode与错误检查

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	p = fat_build_inode(parent_inode->i_sb, (msdos_dirent_s *)tmpde, 0);
	if (IS_ERR(p))
	{
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第63–65行。

### CB34-A030｜构造错误置NULL并跳标签

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		dest_dentry = NULL;
		goto build_inode_fail;
	}
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第66–68行。

### CB34-A031｜块数和私有对象分配

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;

	p->private_idx_info = kzalloc(sizeof(FAT32_inode_info_s), GFP_KERNEL);
	finode = p->private_idx_info;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第69–72行。

### CB34-A032｜私有inode簇定位字段

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	finode->dentry_location	= iobuf->clusters[de_start / iobuf->bufsize];
	finode->dentry_position	= de_start;
	finode->lcase			= tmpde->lcase;
	finode->create_time_cs	= tmpde->ctime_cs;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第74–78行。

### CB34-A033｜私有inode时间日期字段

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	finode->create_date		= tmpde->ctime;
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第79–82行。

### CB34-A034｜记录目录项长度

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	finode->access_date		= tmpde->adate;
	finode->dentry_length	= off - de_start;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第83–84行。

### CB34-A035｜写回原dentry的inode在末尾

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	if((tmpde->starthi >> 12) && (p->i_mode & S_IFMT))
	{
		p->i_mode |= S_IFBLK;
	}
	dest_dentry->d_inode = p;
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第86–90行。

### CB34-A036｜两个共用标签和最终return

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
build_inode_fail:
ent_not_found:
	FAT32_iobuf_release(iobuf);
	return dest_dentry;	
```

固定源码定位：`mykernel/fs/fat/myos_fat32_namei.c`，本轮第92–95行。

### CB34-A037｜fat_build_inode函数入口

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
inode_s *fat_build_inode(super_block_s *sb,
				msdos_dirent_s *de, loff_t i_pos)
{
	inode_s *inode;
	int err;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第204–208行。

### CB34-A038｜new_inode失败转成错误指针

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	inode = new_inode(sb);
	if (inode == NULL) {
		inode = ERR_PTR(-ENOMEM);
		goto out;
	}
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第210–214行。

### CB34-A039｜填充inode并判断返回值

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	// inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
	err = fat_fill_inode(inode, de);
	if (err) {
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第215–217行。

### CB34-A040｜fat_fill_inode错误后的iput与编码

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
		iput(inode);
		inode = ERR_PTR(err);
		goto out;
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第218–220行。

### CB34-A041｜成功/错误统一return

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	// fat_attach(inode, i_pos);
out:
	return inode;
}
```

固定源码定位：`mykernel/fs/fat/inode.c`，本轮第222–225行。

### CB34-A042｜目录项位移量定义

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_DIR_BITS]
```c
	#define MSDOS_DIR_BITS		5			/* log2(sizeof(struct msdos_dir_entry)) */
```

固定源码定位：`mykernel/include/uapi/linux/msdos_fs.h`，本轮第29–29行。

### CB34-A043｜目录项最大条数定义

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_MAX_DIR_ENTRIES]
```c
	#define FAT_MAX_DIR_ENTRIES	(65536)
```

固定源码定位：`mykernel/include/uapi/linux/msdos_fs.h`，本轮第32–32行。

### CB34-A044｜目录字节上界定义

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_MAX_DIR_SIZE]
```c
	#define FAT_MAX_DIR_SIZE	(FAT_MAX_DIR_ENTRIES << MSDOS_DIR_BITS)
```

固定源码定位：`mykernel/include/uapi/linux/msdos_fs.h`，本轮第33–33行。

### CB34-A045｜目录属性位定义

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::ATTR_DIR]
```c
	#define ATTR_DIR			16			/* directory */
```

固定源码定位：`mykernel/include/uapi/linux/msdos_fs.h`，本轮第40–40行。

### CB34-A046｜空闲目录项名字判别

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT32_IS_FREE]
```c
	#define FAT32_IS_FREE(n)	(!*(n) || *(n) == FAT32_DELETED_FLAG)
```

固定源码定位：`mykernel/include/uapi/linux/msdos_fs.h`，本轮第52–52行。

### CB34-A047｜FAT32私有superblock起始字段

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
	typedef struct FAT32_SBinfo
	{
		u64	start_sector;
		u64	sector_count;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第86–89行。

### CB34-A048｜private_sb_info包含簇/扇区大小

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c

		u64	sector_per_cluster;
		u64	bytes_per_cluster;
		u64	bytes_per_sector;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第90–93行。

### CB34-A049｜私有inode簇与目录偏移

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_inode_info_s]
```c
	typedef struct FAT32_inode_info
	{
		u64	first_cluster;
		u64	dentry_location;	// dentry struct in cluster(0 is root,1 is invalid)
		u64	dentry_position;	// dentry struct offset in cluster
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第106–110行。

### CB34-A050｜私有inode的长度字段

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_inode_info_s]
```c
		u64 dentry_start;		// the whole dentry start offset in directory
		u64 dentry_length;		// the whole dentry length
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第111–112行。

### CB34-A051｜私有inode创建时间字段

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_inode_info_s]
```c
		u8	lcase;

		u8	create_time_cs;
		u16	create_date;
		u16	create_time;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第114–118行。

### CB34-A052｜私有inode修改/访问时间字段

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_inode_info_s]
```c

		u16	write_date;
		u16	write_time;

		u16	access_date;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第119–123行。

### CB34-A053｜iobuf簇数与缓冲区大小

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
	typedef struct FAT32_IO_BUFFER
	{
		int		nr_clus;
		int		max_nr_clus;
		size_t	bufsize;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第155–159行。

### CB34-A054｜iobuf簇数组和游标

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		u32		*clusters;
		u32		*flags;
		char	**buffers;

		loff_t	iter_cursor;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第161–165行。

### CB34-A055｜iobuf迭代初始化函数指针

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		loff_t	iter_cursor;
		bool	iter_reach_end;
		FAT32_SBinfo_s	*fsbi;

		int		(*iter_init) (FAT32_iobuf_s *this);
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第165–169行。

### CB34-A056｜iobuf next函数指针与类型闭合

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
		const msdos_dirent_s	*(*next) (FAT32_iobuf_s *this);
	} FAT32_iobuf_s;
```

固定源码定位：`mykernel/include/linux/fs/fat32.h`，本轮第170–171行。

### CB34-A057｜错误指针判定范围

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

固定源码定位：`mykernel/include/linux/kernel/err.h`，本轮第18–20行。

### CB34-A058｜错误整数编码为指针

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

固定源码定位：`mykernel/include/linux/kernel/err.h`，本轮第26–29行。

### CB34-A059｜IS_ERR判断错误编码

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

固定源码定位：`mykernel/include/linux/kernel/err.h`，本轮第36–39行。

### CB34-A060｜ENOENT值为2

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOENT]
```c
	#define	ENOENT		2	/* No such file or directory */
```

固定源码定位：`mykernel/include/uapi/asm-generic/errno-base.h`，本轮第8–8行。

### CB34-A061｜ENOMEM值为12

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

固定源码定位：`mykernel/include/uapi/asm-generic/errno-base.h`，本轮第18–18行。

### CB34-A062｜选定条件strncmp函数主体

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
	int strncmp(const char *cs, const char *ct, size_t count)
	{
		unsigned char c1, c2;
		while (count) {
			c1 = *cs++;
```

固定源码定位：`mykernel/lib/string/string.c`，本轮第361–365行。

### CB34-A063｜有限比较的不同分支

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			c1 = *cs++;
			c2 = *ct++;
			if (c1 != c2)
				return c1 < c2 ? -1 : 1;
			if (!c1)
```

固定源码定位：`mykernel/lib/string/string.c`，本轮第365–369行。

### CB34-A064｜相等或提前NUL的退出

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			if (!c1)
				break;
			count--;
		}
		return 0;
```

固定源码定位：`mykernel/lib/string/string.c`，本轮第369–373行。

