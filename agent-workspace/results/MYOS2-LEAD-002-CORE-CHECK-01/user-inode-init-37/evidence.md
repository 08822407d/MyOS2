---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-INODE-INIT-37
record_type: prior36_correction_and_inode37_direct_source_ledger
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
prior36_verdict: RETURN
prior36_closure: PENDING_C04_REVIEW
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 新增直接证据的两个相互独立来源

下列各段均逐字取自固定time的单个活动函数、结构或单宏定义体，宽度1–5行。N36C-A专用于R36-01/R36-02的追加纠正，NI37-A专用于独立的inode初始化源码分析。**历史NP36-A001至A056仍是旧文档索引，不是本轮证明旧56段均合格。**原A043的`#ifndef __HAVE_ARCH_STRNCMP`是上下文，不算作新VERIFIED函数/宏定义。静态源码摘录不代表编译或运行。

## N00｜36文档追加勘误的新直接引文

### N36C-A001｜LOWERCASE_BASE，固定行83–83

[VERIFIED mykernel/include/linux/fs/fat32.h::LOWERCASE_BASE]
```c
	#define LOWERCASE_BASE (8)
```

### N36C-A002｜LOWERCASE_EXT，固定行84–84

[VERIFIED mykernel/include/linux/fs/fat32.h::LOWERCASE_EXT]
```c
	#define LOWERCASE_EXT (16)
```

### N36C-A003｜MSDOS_NAME，固定行55–55

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_NAME]
```c
	#define MSDOS_NAME			11				/* maximum name length */
```

### N36C-A004｜msdos_dirslot_s，固定行197–201

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
	typedef struct msdos_dir_slot {
		__u8	id;					/* sequence number for slot
										-LDIR_Ord- */
		__u8	name0_4[10];		/* first 5 characters in name
										-LDIR_Name1- */
```

### N36C-A005｜msdos_dirslot_s，固定行202–206

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__u8	attr;				/* attribute byte
										-LDIR_Attr- */
		__u8	reserved;			/* always 0
										-LDIR_Type- */
		__u8	alias_checksum;		/* checksum for 8.3 alias
```

### N36C-A006｜msdos_dirslot_s，固定行207–209

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
										-LDIR_Chksum- */
		__u8	name5_10[12];		/* 6 more characters in name
										-LDIR_Name2- */
```

### N36C-A007｜msdos_dirslot_s，固定行210–214

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__le16	start;				/* starting cluster number, 0 in long slots
										-LDIR_FstClusLO- */
		__u8	name11_12[4];		/* last 2 characters in name
										-LDIR_Name3- */
	}__attribute__((packed)) msdos_dirslot_s;
```

### N36C-A008｜FAT32_parse_long，固定行62–66

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		i++;
		if(lde_tmp->id & 0x40)
			break;
		cursor -= entlen;
		lde_tmp = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
```

### N36C-A009｜FAT32_parse_long，固定行75–78

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 10; y += 2)
			if(*(u16 *)(&lde->name0_4[y]) != 0xffff &&
				*(u16 *)(&lde->name0_4[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name0_4[y];
```

### N36C-A010｜FAT32_parse_long，固定行80–83

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 12; y += 2)
			if(*(u16 *)(&lde->name5_10[y]) != 0xffff &&
				*(u16 *)(&lde->name5_10[y]) != 0x0000)
				ret_val[(*namelen)++] = lde->name5_10[y];
```

### N36C-A011｜FAT32_parse_long，固定行85–88

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
		for(y = 0; y < 4; y += 2)
			if(*(u16 *)(&lde->name11_12[y]) != 0xffff &&
				*(u16 *)(&lde->name11_12[y]) != 0x0000)
				ret_val[(*namelen)++] = (char)lde->name11_12[y];
```

### N36C-A012｜FAT32_parse_long，固定行73–73

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	for(x = 0; x < i; x++, cursor -= entlen, lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor))
```

### N36C-A013｜FAT32_lookup，固定行36–40

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
				iobuf->iter_cursor - 2 * entlen);
		off = iobuf->iter_cursor;

		char *name = NULL;
		int namelen = 0;
```

### N36C-A014｜FAT32_lookup，固定行44–48

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
		else if(!IS_ERR(tmplde) &&
			tmplde->attr == ATTR_LONG_NAME &&
			!FAT32_ent_empty((msdos_dirent_s *)tmplde))
			name = FAT32_parse_long(&namelen, iobuf);
		else
```

### N36C-A015｜FAT32_lookup，固定行47–49

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
			name = FAT32_parse_long(&namelen, iobuf);
		else
			name = FAT32_parse_short(&namelen, tmpde);
```

### N36C-A016｜FAT32_parse_short，固定行13–17

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
char *FAT32_parse_short(int *namelen, const msdos_dirent_s *de)
{
	int x;
	*namelen = 0;
	char *ret_val = kzalloc(15, GFP_KERNEL);
```

### N36C-A017｜FAT32_parse_short，固定行20–23

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
	for(x=0; x < 8; x++)
	{
		if(de->name[x] == ' ')
			break;
```

### N36C-A018｜FAT32_parse_short，固定行30–32

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_short]
```c
	if(!(de->attr & ATTR_DIRECTORY))
	{
		ret_val[(*namelen)++] = '.';
```

### N36C-A019｜FAT32_iobuf_readent，固定行64–68

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
```

### N36C-A020｜FAT32_parse_long，固定行69–72

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
	//long file/dir name read
	ret_val = kzalloc(i * 13 + 1, GFP_KERNEL);
	cursor = iobuf->iter_cursor - entlen * 2;
	msdos_dirslot_s *lde = (msdos_dirslot_s *)FAT32_iobuf_readent(iobuf, cursor);
```

### N36C-A021｜FAT32_parse_long，固定行52–56

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_parse_long]
```c
{
	int i = 0, x, y;
	char *ret_val;
	int entlen = sizeof(msdos_dirent_s);
	loff_t cursor = iobuf->iter_cursor - entlen * 2;
```

## N01–N03｜37 inode初始化的新直接引文

### NI37-A001｜super_block_s，固定行1338–1342

[VERIFIED mykernel/include/linux/fs/fs.h::super_block_s]
```c
		unsigned long		s_blocksize;
		loff_t				s_maxbytes;	/* Max file size */
		fs_type_s			*s_type;
		const super_ops_s	*s_op;
		// const dquot_operations_s	*dq_op;
```

### NI37-A002｜super_ops_s，固定行1984–1988

[VERIFIED mykernel/include/linux/fs/fs.h::super_ops_s]
```c


		inode_s	*(*alloc_inode)(super_block_s *sb);
		void	(*destroy_inode)(inode_s *);
		void	(*free_inode)(inode_s *);
```

### NI37-A003｜inode_s，固定行545–549

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
	typedef struct inode {
		void				*private_idx_info;


		umode_t				i_mode;
```

### NI37-A004｜inode_s，固定行560–562

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		const inode_ops_s	*i_op;
		super_block_s		*i_sb;
		addr_spc_s			*i_mapping;
```

### NI37-A005｜inode_s，固定行581–583

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		dev_t				i_rdev;
		loff_t				i_size;
		timespec64_s		i_atime;
```

### NI37-A006｜inode_s，固定行586–590

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		spinlock_t			i_lock;		/* i_blocks, i_bytes, maybe i_size */
		ushort				i_bytes;
		// u8				i_blkbits;
		// u8				i_write_hint;
		blkcnt_t			i_blocks;
```

### NI37-A007｜inode_s，固定行630–634

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
			const file_ops_s	*i_fop;	/* former ->i_op->default_file_ops */
			void				(*free_inode)(inode_s *);
		};
	// 	file_s_lock_context	*i_flctx;
		addr_spc_s			i_data;
```

### NI37-A008｜addr_spc_s，固定行386–390

[VERIFIED mykernel/include/linux/fs/fs.h::addr_spc_s]
```c
	typedef struct address_space {
		inode_s				*host;
		page_s				**page_array;
		// xarray_s			i_pages;
		// rwsem_t		invalidate_lock;
```

### NI37-A009｜addr_spc_s，固定行398–402

[VERIFIED mykernel/include/linux/fs/fs.h::addr_spc_s]
```c
		rwsem_t				i_mmap_rwsem;
		ulong				nrpages;
		pgoff_t				writeback_index;
		const addr_spc_ops_s	*a_ops;
		ulong				flags;
```

### NI37-A010｜addr_spc_s，固定行402–406

[VERIFIED mykernel/include/linux/fs/fs.h::addr_spc_s]
```c
		ulong				flags;
		// errseq_t			wb_err;
		// spinlock_t			private_lock;
		// list_head_s			private_list;
		void				*private_data;
```

### NI37-A011｜msdos_inode_info_s，固定行113–117

[VERIFIED mykernel/include/linux/fs/fat.h::msdos_inode_info_s]
```c
	typedef struct msdos_inode_info {
		// spinlock_t cache_lru_lock;
		// List_s cache_lru;
		int		nr_caches;
		/* for avoiding the race between fat_free() and fat_get_cluster() */
```

### NI37-A012｜msdos_inode_info_s，固定行123–127

[VERIFIED mykernel/include/linux/fs/fat.h::msdos_inode_info_s]
```c
		int		i_start;			/* first cluster or 0 */
		int		i_logstart;			/* logical first cluster */
		int		i_attrs;			/* unused attribute bits */
		loff_t	i_pos;				/* on-disk position of directory entry or 0 */
		// HList_s i_fat_hash;	/* hash by i_location */
```

### NI37-A013｜msdos_inode_info_s，固定行128–131

[VERIFIED mykernel/include/linux/fs/fat.h::msdos_inode_info_s]
```c
		// HList_s i_dir_hash;	/* hash by i_logstart */
		// rwsem_t truncate_lock; /* protect bmap against truncate */
		inode_s vfs_inode;
	} msdos_inode_info_s;
```

### NI37-A014｜MSDOS_I，固定行174–177

[VERIFIED mykernel/include/linux/fs/fat.h::MSDOS_I]
```c
	static inline msdos_inode_info_s *
	MSDOS_I(inode_s *inode) {
		return container_of(inode, msdos_inode_info_s, vfs_inode);
	}
```

### NI37-A015｜fat_fill_super，固定行436–440

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	sb->s_flags |= SB_NODIRATIME;
	sb->s_magic = MSDOS_SUPER_MAGIC;
	sb->s_op = (super_ops_s *)&fat_sops;

	setup(sb); /* flavour-specific stuff that needs options */
```

### NI37-A016｜fat_sops，固定行304–308

[VERIFIED mykernel/fs/fat/inode.c::fat_sops]
```c
static const super_ops_s fat_sops = {
	.alloc_inode	= fat_alloc_inode,
	.free_inode		= fat_free_inode,
	.write_inode	= fat_write_inode,
	// .evict_inode	= fat_evict_inode,
```

### NI37-A017｜fat_alloc_inode，固定行238–242

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c
{
	msdos_inode_info_s *ei;
	ei = kmalloc(sizeof(msdos_inode_info_s), GFP_KERNEL);
	if (ei == NULL)
		return NULL;
```

### NI37-A018｜fat_alloc_inode，固定行245–249

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c
	ei->mmu_private = 0;
	ei->i_start = 0;
	ei->i_logstart = 0;
	ei->i_attrs = 0;
	ei->i_pos = 0;
```

### NI37-A019｜fat_alloc_inode，固定行250–252

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c

	return &ei->vfs_inode;
}
```

### NI37-A020｜fat_free_inode，固定行254–257

[VERIFIED mykernel/fs/fat/inode.c::fat_free_inode]
```c
static void fat_free_inode(inode_s *inode)
{
	kfree(MSDOS_I(inode));
}
```

### NI37-A021｜new_inode，固定行274–278

[VERIFIED mykernel/fs/vfs/inode.c::new_inode]
```c
inode_s *new_inode(super_block_s *sb)
{
	// inode_s *new_inode_pseudo(super_block_s *sb)
	// {
	inode_s *inode = alloc_inode(sb);
```

### NI37-A022｜new_inode，固定行279–283

[VERIFIED mykernel/fs/vfs/inode.c::new_inode]
```c
	if (inode != NULL)
	{
		inode->i_sb = sb;
		INIT_LIST_S(&inode->i_sb_list);
	}
```

### NI37-A023｜new_inode，固定行286–290

[VERIFIED mykernel/fs/vfs/inode.c::new_inode]
```c
	if (inode != NULL)
		inode_sb_list_add(inode);

	return inode;
}
```

### NI37-A024｜inode_sb_list_add，固定行213–217

[VERIFIED mykernel/fs/vfs/inode.c::inode_sb_list_add]
```c
{
	// spin_lock(&inode->i_sb->s_inode_list_lock);
	// list_add_to_next(&inode->i_sb_list, &inode->i_sb->s_inodes);
	list_header_add_to_tail(&inode->i_sb->s_inodes, &inode->i_sb_list);
	// spin_unlock(&inode->i_sb->s_inode_list_lock);
```

### NI37-A025｜alloc_inode，固定行143–147

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
static inode_s *alloc_inode(super_block_s *sb)
{
	const super_ops_s *ops = sb->s_op;
	inode_s *inode;

```

### NI37-A026｜alloc_inode，固定行148–152

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
	if (ops->alloc_inode != NULL)
		inode = ops->alloc_inode(sb);
	else
		inode = kmem_cache_alloc(inode_cachep, GFP_KERNEL);

```

### NI37-A027｜alloc_inode，固定行153–156

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
	if (inode == NULL)
		return NULL;

	if (unlikely(inode_init_always(sb, inode))) {
```

### NI37-A028｜alloc_inode，固定行156–160

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
	if (unlikely(inode_init_always(sb, inode))) {
		if (ops->destroy_inode) {
			ops->destroy_inode(inode);
			if (!ops->free_inode)
				return NULL;
```

### NI37-A029｜alloc_inode，固定行162–166

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
		if (ops->free_inode)
			ops->free_inode(inode);
		else
			kmem_cache_free(inode_cachep, inode);
		return NULL;
```

### NI37-A030｜alloc_inode，固定行168–170

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c

	return inode;
}
```

### NI37-A031｜inode_init_always，固定行85–89

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
int inode_init_always(super_block_s *sb, inode_s *inode)
{
	static const inode_ops_s empty_iops;
	static const file_ops_s no_open_fops = {.open = no_open};
	addr_spc_s *const mapping = &inode->i_data;
```

### NI37-A032｜inode_init_always，固定行91–95

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	inode->i_sb = sb;
	// inode->i_blkbits = sb->s_blocksize_bits;
	inode->i_flags = 0;
	// atomic64_set(&inode->i_sequence, 0);
	// atomic_set(&inode->i_count, 1);
```

### NI37-A033｜inode_init_always，固定行96–100

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	inode->i_op = &empty_iops;
	inode->i_fop = &no_open_fops;
	inode->i_ino = 0;
	// inode->__i_nlink = 1;
	inode->i_opflags = 0;
```

### NI37-A034｜inode_init_always，固定行102–106

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	i_gid_write(inode, 0);
	// atomic_set(&inode->i_writecount, 0);
	inode->i_size = 0;
	// inode->i_write_hint = WRITE_LIFE_NOT_SET;
	inode->i_blocks = 0;
```

### NI37-A035｜inode_init_always，固定行125–129

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	mapping->a_ops = &empty_aops;
	mapping->host = inode;
	mapping->flags = 0;
	// mapping->wb_err = 0;
	atomic_set(&mapping->i_mmap_writable, 0);
```

### NI37-A036｜inode_init_always，固定行134–137

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	mapping->private_data = NULL;
	mapping->writeback_index = 0;
	inode->i_private = NULL;
	inode->i_mapping = mapping;
```

### NI37-A037｜inode_init_always，固定行136–140

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	inode->i_private = NULL;
	inode->i_mapping = mapping;
	// INIT_HLIST_HEAD(&inode->i_dentry);	/* buggered by rcu freeing */

	return 0;
```

### NI37-A038｜empty_aops，固定行35–37

[VERIFIED mykernel/fs/vfs/inode.c::empty_aops]
```c
const addr_spc_ops_s empty_aops = {
};
EXPORT_SYMBOL(empty_aops);
```

### NI37-A039｜__address_space_init_once，固定行173–177

[VERIFIED mykernel/fs/vfs/inode.c::__address_space_init_once]
```c
static void __address_space_init_once(addr_spc_s *mapping)
{
	// xa_init_flags(&mapping->i_pages, XA_FLAGS_LOCK_IRQ | XA_FLAGS_ACCOUNT);
	init_rwsem(&mapping->i_mmap_rwsem);
	// INIT_LIST_HEAD(&mapping->i_private_list);
```

### NI37-A040｜address_space_init_once，固定行182–186

[VERIFIED mykernel/fs/vfs/inode.c::address_space_init_once]
```c
void address_space_init_once(addr_spc_s *mapping)
{
	memset(mapping, 0, sizeof(*mapping));
	__address_space_init_once(mapping);
}
```

### NI37-A041｜inode_init_once，固定行195–199

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_once]
```c
{
	memset(inode, 0, sizeof(inode_s));

	// INIT_HLIST_NODE(&inode->i_hash);
	INIT_LIST_HEADER_S(&inode->i_devices);
```

### NI37-A042｜inode_init_once，固定行200–204

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_once]
```c
	// INIT_LIST_HEAD(&inode->i_io_list);
	// INIT_LIST_HEAD(&inode->i_wb_list);
	// INIT_LIST_HEAD(&inode->i_lru);
	__address_space_init_once(&inode->i_data);
	// i_size_ordered_init(inode);
```

### NI37-A043｜inode_init，固定行356–360

[VERIFIED mykernel/fs/vfs/inode.c::inode_init]
```c
	inode_cachep = kmem_cache_create("inode_cache",
					sizeof(inode_s), 0,
					(SLAB_RECLAIM_ACCOUNT | SLAB_PANIC |
						SLAB_MEM_SPREAD | SLAB_ACCOUNT),
					NULL);
```

### NI37-A044｜fat_build_inode，固定行204–208

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
inode_s *fat_build_inode(super_block_s *sb,
				msdos_dirent_s *de, loff_t i_pos)
{
	inode_s *inode;
	int err;
```

### NI37-A045｜fat_build_inode，固定行210–214

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	inode = new_inode(sb);
	if (inode == NULL) {
		inode = ERR_PTR(-ENOMEM);
		goto out;
	}
```

### NI37-A046｜fat_build_inode，固定行215–219

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	// inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
	err = fat_fill_inode(inode, de);
	if (err) {
		iput(inode);
		inode = ERR_PTR(err);
```

### NI37-A047｜fat_build_inode，固定行220–224

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
		goto out;
	}
	// fat_attach(inode, i_pos);
out:
	return inode;
```

### NI37-A048｜fat_fill_inode，固定行141–145

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	// msdos_sb_info_s *sbi = MSDOS_SB(inode->i_sb);
	int error;

	inode->i_mode = 0;
	MSDOS_I(inode)->i_pos = 0;
```

### NI37-A049｜fat_fill_inode，固定行151–152

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	if ((de->attr & ATTR_DIR) && !FAT32_IS_FREE(de->name)) {
		// inode->i_generation &= ~1;
```

### NI37-A050｜fat_fill_inode，固定行154–157

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		// inode->i_op		= sbi->dir_ops;
		// inode->i_fop = &fat_dir_operations;
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
```

### NI37-A051｜fat_fill_inode，固定行174–178

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		// MSDOS_I(inode)->i_logstart = MSDOS_I(inode)->i_start;
		inode->i_size	= de->size;
		// inode->i_op = &fat_file_inode_operations;
		// inode->i_fop = &fat_file_operations;
		inode->i_op		= &vfat_dir_inode_operations;
```

### NI37-A052｜fat_fill_inode，固定行177–180

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
		// inode->i_fop = &fat_file_operations;
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
		inode->i_mapping->a_ops = &fat_aops;
```

### NI37-A053｜fat_fill_inode，固定行199–202

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	// 	inode->i_atime = fat_truncate_atime(sbi, &inode->i_mtime);

	return 0;
}
```

### NI37-A054｜__lookup_slow，固定行323–327

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	if (IS_ERR(dentry))
		return dentry;

	if (dir->d_inode->i_op->lookup(dir->d_inode, dentry, flags) == NULL)
	{
```

### NI37-A055｜__lookup_slow，固定行333–337

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	inode = dentry->d_inode;
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
		inode->i_mapping->page_array =
			kzalloc(inode->i_mapping->nrpages * sizeof(page_s *), GFP_KERNEL);
```

### NI37-A056｜FAT32_lookup，固定行63–67

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	p = fat_build_inode(parent_inode->i_sb, (msdos_dirent_s *)tmpde, 0);
	if (IS_ERR(p))
	{
		dest_dentry = NULL;
		goto build_inode_fail;
```

### NI37-A057｜FAT32_lookup，固定行69–72

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	p->i_blocks	= (p->i_size + fsbi->bytes_per_cluster - 1)/fsbi->bytes_per_sector;

	p->private_idx_info = kzalloc(sizeof(FAT32_inode_info_s), GFP_KERNEL);
	finode = p->private_idx_info;
```

### NI37-A058｜FAT32_lookup，固定行89–90

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	}
	dest_dentry->d_inode = p;
```

### NI37-A059｜inode_s，固定行630–634

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
			const file_ops_s	*i_fop;	/* former ->i_op->default_file_ops */
			void				(*free_inode)(inode_s *);
		};
	// 	file_s_lock_context	*i_flctx;
		addr_spc_s			i_data;
```

