---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-SUPERBLOCK-BINDING-38
record_type: fixed_source_single_definition_evidence
source_pin_short12: a039d9803ade
read_channel: connector
date: 2026-11-04
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 第38批直接证据

各摘录限一个真实函数、对象表、结构或单宏定义内1–5行。注册和完整表定义已按固定源阅读；省略成员的NULL含义另作为所选C聚合初始化模型的推导，不把缺写的堆字段等同静态对象。编号是引用单位，不是独立缺陷数量。历史标签不计入本批。引文依据连接器返回的逐行文本（行尾按连接器范围输出规范化）；不是整个仓库原始字节或构建认证。

### SB38-A001｜启动索引宏

[VERIFIED mykernel/fs/vfs/myos_vfs.c::BOOT_FS_IDX]
```c
#define BOOT_FS_IDX 0
```
固定time行23–23。

### SB38-A002｜只调用init_fat32_fs；vfat调用是注释

[VERIFIED mykernel/fs/vfs/myos_vfs.c::register_diskfs]
```c
void register_diskfs(void)
{
	init_fat32_fs();
	// init_vfat_fs();
}
```
固定time行68–72。

### SB38-A003｜分区GUID的两个直接比较门

[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
		switch (*puid_p)
		{
		case EFIBOOT_PART_GUID_LOW:
			if (*(puid_p + 1) == EFIBOOT_PART_GUID_HIGH)
			{
```
固定time行117–121。

### SB38-A004｜读请求后按BOOT_FS_IDX调用mount_fs

[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
				FAT32_BS_s *fat32_sb = (FAT32_BS_s *)kmalloc(PAGE_SIZE, GFP_KERNEL);
				ROOTBLK_TRANSFER(CMD_READ, gpt_pes[i].StartingLBA, SECT_PER_PG, (unchar *)fat32_sb);
				if (i == BOOT_FS_IDX)
					myos_root_sb = mount_fs("FAT32", gpt_pe, fat32_sb);
```
固定time行123–126。

### SB38-A005｜循环后直接执行根挂载/任务设置

[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
	set_init_mount();
	set_init_taskfs();

	int err = init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT);
```
固定time行142–145。

### SB38-A006｜消费myos_root_sb的s_root

[VERIFIED mykernel/fs/vfs/myos_vfs.c::set_init_mount]
```c
void set_init_mount()
{
	myos_root_mnt.mnt.mnt_sb = myos_root_sb;
	myos_root_mnt.mnt_mountpoint =
	myos_root_mnt.mnt.mnt_root = myos_root_sb->s_root;
```
固定time行42–46。

### SB38-A007｜链表匹配后调用read_super，而非mount

[VERIFIED mykernel/fs/vfs/myos_vfs.c::mount_fs]
```c
	for(p = file_systems; p; p = p->next)
		if(!strcmp(p->name, name))
		{
			return p->read_super(DPTE, buf);
		}
```
固定time行155–159。

### SB38-A008｜找不到名称返回零指针

[VERIFIED mykernel/fs/vfs/myos_vfs.c::mount_fs]
```c
	return 0;
}
```
固定time行160–161。

### SB38-A009｜实际注册链表头指向tmp

[VERIFIED mykernel/fs/vfs/filesystems.c::file_systems]
```c
fs_type_s *file_systems = &tmp;
```
固定time行36–36。

### SB38-A010｜注册查重按名字和结束位置匹配

[VERIFIED mykernel/fs/vfs/filesystems.c::find_filesystem]
```c
	for (p = &file_systems; *p; p = &(*p)->next)
		if (strncmp((*p)->name, name, len) == 0 &&
		    !(*p)->name[len])
			break;
	return p;
```
固定time行51–55。

### SB38-A011｜next非NULL被拒绝并查重

[VERIFIED mykernel/fs/vfs/filesystems.c::register_filesystem]
```c
	if (fs->next)
		return -EBUSY;
	p = find_filesystem(fs->name, strlen(fs->name));
	if (*p)
```
固定time行82–85。

### SB38-A012｜有重名则EBUSY，否则链接对象并返回res

[VERIFIED mykernel/fs/vfs/filesystems.c::register_filesystem]
```c
		res = -EBUSY;
	else
		*p = fs;
	return res;
}
```
固定time行86–90。

### SB38-A013｜FAT32名字与read_super槽

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_fs_type]
```c
fs_type_s FAT32_fs_type =
{
	.name = "FAT32",
	.fs_flags = 0,
	.read_super = read_fat32_superblock,
```
固定time行499–503。

### SB38-A014｜next初值明确为NULL

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_fs_type]
```c
	.next = NULL,
};
```
固定time行504–505。

### SB38-A015｜注册返回值未被void包装消费

[VERIFIED mykernel/fs/fat/myos_fat32.c::init_fat32_fs]
```c
void init_fat32_fs()
{
	register_filesystem(&FAT32_fs_type);
} 
```
固定time行507–510。

### SB38-A016｜vfat名字与mount槽

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_fs_type]
```c
fs_type_s vfat_fs_type = {
	.name		= "vfat",
	.mount		= vfat_mount,
	.fs_flags	= FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
};
```
固定time行309–313。

### SB38-A017｜独立vfat注册函数

[VERIFIED mykernel/fs/fat/namei_vfat.c::init_vfat_fs]
```c
int init_vfat_fs(void)
{
	return register_filesystem(&vfat_fs_type);
}
```
固定time行315–318。

### SB38-A018｜委托mount_bdev并传vfat_fill_super

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_mount]
```c
static dentry_s *vfat_mount(fs_type_s *fs_type, int flags,
		const char *dev_name, void *data)
{
	return mount_bdev(&vfat_fs_type, flags, dev_name, data, vfat_fill_super);
}
```
固定time行303–307。

### SB38-A019｜填充回调直接调用fat_fill_super

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_fill_super]
```c
static int vfat_fill_super(super_block_s *sb, void *data, int silent)
{
	return fat_fill_super(sb, data, 1, setup);
}
```
固定time行297–300。

### SB38-A020｜setup经MSDOS_SB写dir_ops

[VERIFIED mykernel/fs/fat/namei_vfat.c::setup]
```c
static void setup(super_block_s *sb)
{
	MSDOS_SB(sb)->dir_ops = &vfat_dir_inode_operations;
```
固定time行288–290。

### SB38-A021｜read_super返回超级块指针

[VERIFIED mykernel/include/linux/fs/fs.h::fs_type_s]
```c
	typedef struct file_system_type {
		super_block_s	*(*read_super)(GPT_PE_s * DPTE, void * buf);
```
固定time行2312–2313。

### SB38-A022｜mount返回dentry；与read_super不同槽

[VERIFIED mykernel/include/linux/fs/fs.h::fs_type_s]
```c
		int				(*init_fs_context)(fs_ctxt_s *);
		dentry_s		*(*mount) (fs_type_s *, int, const char *, void *);
		void			(*kill_sb) (super_block_s *);
		fs_type_s		*next;
```
固定time行2325–2328。

### SB38-A023｜FAT32_sb_ops完整成员初始化列表，无alloc_inode

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_sb_ops]
```c
{
	.write_super = fat32_write_superblock,
	.put_super = fat32_put_superblock,
	.write_inode = fat32_write_inode,
};
```
固定time行414–418。

### SB38-A024｜fat_sops明确绑定fat_alloc_inode

[VERIFIED mykernel/fs/fat/inode.c::fat_sops]
```c
static const super_ops_s fat_sops = {
	.alloc_inode	= fat_alloc_inode,
	.free_inode		= fat_free_inode,
	.write_inode	= fat_write_inode,
```
固定time行304–307。

### SB38-A025｜alloc/destroy/free三成员

[VERIFIED mykernel/include/linux/fs/fs.h::super_ops_s]
```c
		inode_s	*(*alloc_inode)(super_block_s *sb);
		void	(*destroy_inode)(inode_s *);
		void	(*free_inode)(inode_s *);
```
固定time行1986–1988。

### SB38-A026｜private_sb_info字段

[VERIFIED mykernel/include/linux/fs/fs.h::super_block_s]
```c
	typedef struct super_block {
		void				*private_sb_info;
```
固定time行1331–1332。

### SB38-A027｜独立s_fs_info字段

[VERIFIED mykernel/include/linux/fs/fs.h::super_block_s]
```c
		void			*s_fs_info;	/* Filesystem private info */
```
固定time行1383–1383。

### SB38-A028｜MSDOS_SB实际只取s_fs_info

[VERIFIED mykernel/include/linux/fs/fat.h::MSDOS_SB]
```c
	static inline msdos_sb_info_s *
	MSDOS_SB(super_block_s *sb) {
		return sb->s_fs_info;
	}
```
固定time行141–144。

### SB38-A029｜分配sizeof(super_block_s)

[VERIFIED mykernel/fs/vfs/super.c::alloc_super]
```c
super_block_s *alloc_super(fs_type_s *type, int flags)
{
	super_block_s *s = kzalloc(sizeof(super_block_s), GFP_KERNEL);
	static const super_ops_s default_op;
	int i;
```
固定time行109–113。

### SB38-A030｜实际失败出口ERR_PTR(-ENOMEM)

[VERIFIED mykernel/fs/vfs/super.c::alloc_super]
```c
	if (s == NULL)
		return ERR_PTR(-ENOMEM);
```
固定time行115–116。

### SB38-A031｜初始化超级块inode列表

[VERIFIED mykernel/fs/vfs/super.c::alloc_super]
```c
	INIT_LIST_HEADER_S(&s->s_inodes);
```
固定time行157–157。

### SB38-A032｜设置count、maxbytes、默认操作表

[VERIFIED mykernel/fs/vfs/super.c::alloc_super]
```c
	s->s_count = 1;
	s->s_maxbytes = MAX_NON_LFS;
	s->s_op = &default_op;
```
固定time行162–164。

### SB38-A033｜调用alloc_super后只测NULL

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp = alloc_super(&FAT32_fs_type, 0);
	if (sbp == NULL)
		return ERR_PTR(-ENOMEM);
```
固定time行429–431。

### SB38-A034｜写FAT32_sb_ops并单独分配private_sb_info

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_op = &FAT32_sb_ops;
	sbp->private_sb_info = kzalloc(sizeof(FAT32_SBinfo_s), GFP_KERNEL);
```
固定time行433–434。

### SB38-A035｜私有信息分配之后即消费字段

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	fbs = (FAT32_BS_s *)buf;
	fsbi = sbp->private_sb_info;	
	fsbi->start_sector = DPTE->StartingLBA;
	fsbi->sector_count = DPTE->EndingLBA - DPTE->StartingLBA;
	fsbi->sector_per_cluster = fbs->BPB_SecPerClus;
```
固定time行437–441。

### SB38-A036｜根dentry分配与直接初始化

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_root = kzalloc(sizeof(dentry_s), GFP_KERNEL);

	INIT_LIST_S(&sbp->s_root->d_child);
	INIT_LIST_HEADER_S(&sbp->s_root->d_subdirs);
	sbp->s_root->d_sb = sbp;
```
固定time行462–466。

### SB38-A037｜独立根名字缓冲及长度

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_root->d_name.name = (char *)kmalloc(2, GFP_KERNEL);
	((char *)sbp->s_root->d_name.name)[0] = '/';
	((char *)sbp->s_root->d_name.name)[1] = 0;
	sbp->s_root->d_name.len = 1;
```
固定time行469–472。

### SB38-A038｜根是独立inode_s分配，直接写操作表和size

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_root->d_inode = (inode_s*)kmalloc(sizeof(inode_s), GFP_KERNEL);
	sbp->s_root->d_inode->i_op = &vfat_dir_inode_operations;
	sbp->s_root->d_inode->i_fop = &FAT32_file_ops;
	sbp->s_root->d_inode->i_size = 0;
```
固定time行475–478。

### SB38-A039｜blocks算术、mode和i_sb

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_root->d_inode->i_blocks =
					(sbp->s_root->d_inode->i_size + fsbi->bytes_per_cluster - 1) /
							fsbi->bytes_per_sector;
	sbp->s_root->d_inode->i_mode = S_IFDIR;
	sbp->s_root->d_inode->i_sb = sbp;
```
固定time行479–483。

### SB38-A040｜根私有FAT32_inode_info_s缓冲，非包含对象

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->s_root->d_inode->private_idx_info = kmalloc(sizeof(FAT32_inode_info_s), GFP_KERNEL);
	finode = (FAT32_inode_info_s *)sbp->s_root->d_inode->private_idx_info;
	finode->first_cluster = fbs->BPB_RootClus;
	finode->dentry_location = 0;
	finode->dentry_position = 0; 
```
固定time行486–490。

### SB38-A041｜有效完成返回sbp

[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	return sbp;
}
```
固定time行496–497。

### SB38-A042｜另一入口给s_fs_info分配msdos_sb_info_s并检查NULL

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	sbi = kzalloc(sizeof(msdos_sb_info_s), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	sb->s_fs_info = sbi;
```
固定time行431–434。

### SB38-A043｜另一入口绑定fat_sops后调用setup

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	sb->s_flags |= SB_NODIRATIME;
	sb->s_magic = MSDOS_SUPER_MAGIC;
	sb->s_op = (super_ops_s *)&fat_sops;

	setup(sb); /* flavour-specific stuff that needs options */
```
固定time行436–440。

### SB38-A044｜另一入口根inode经new_inode

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	root_inode = new_inode(sb);
	if (!root_inode)
		goto out_fail;
	root_inode->i_ino = MSDOS_ROOT_INO;
```
固定time行600–603。

### SB38-A045｜fat_read_root结果门

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	error = fat_read_root(root_inode);
	if (error < 0) {
		iput(root_inode);
		goto out_fail;
	}
```
固定time行605–609。

### SB38-A046｜正常尾部落到out_invalid设EINVAL

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
out_invalid:
	error = -EINVAL;
```
固定time行620–621。

### SB38-A047｜清空s_fs_info并返回error

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_super]
```c
	sb->s_fs_info = NULL;
	kfree(sbi);
	return error;
}
```
固定time行628–631。

### SB38-A048｜根的i_op表实际绑定FAT32_lookup

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::vfat_dir_inode_operations]
```c
inode_ops_s vfat_dir_inode_operations = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
```
固定time行199–202。

### SB38-A049｜查找取父inode的private_sb_info

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	inode_s * p = NULL;
	FAT32_inode_info_s * finode = NULL;
	FAT32_SBinfo_s * fsbi = parent_inode->i_sb->private_sb_info;
```
固定time行17–19。

### SB38-A050｜父超级块进入fat_build_inode

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	p = fat_build_inode(parent_inode->i_sb, (msdos_dirent_s *)tmpde, 0);
	if (IS_ERR(p))
	{
		dest_dentry = NULL;
		goto build_inode_fail;
```
固定time行63–67。

### SB38-A051｜子inode走new_inode及正常NULL错误返回

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	inode = new_inode(sb);
	if (inode == NULL) {
		inode = ERR_PTR(-ENOMEM);
		goto out;
	}
```
固定time行210–214。

### SB38-A052｜调用fat_fill_inode前后顺序

[VERIFIED mykernel/fs/fat/inode.c::fat_build_inode]
```c
	// inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
	err = fat_fill_inode(inode, de);
	if (err) {
		iput(inode);
		inode = ERR_PTR(err);
```
固定time行215–219。

### SB38-A053｜alloc_inode回调缺席就使用inode_cachep

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
	if (ops->alloc_inode != NULL)
		inode = ops->alloc_inode(sb);
	else
		inode = kmem_cache_alloc(inode_cachep, GFP_KERNEL);
```
固定time行148–151。

### SB38-A054｜NULL正常失败与每次初始化调用

[VERIFIED mykernel/fs/vfs/inode.c::alloc_inode]
```c
	if (inode == NULL)
		return NULL;

	if (unlikely(inode_init_always(sb, inode))) {
```
固定time行153–156。

### SB38-A055｜fallback缓存对象大小inode_s且构造器参数NULL

[VERIFIED mykernel/fs/vfs/inode.c::inode_init]
```c
	inode_cachep = kmem_cache_create("inode_cache",
					sizeof(inode_s), 0,
					(SLAB_RECLAIM_ACCOUNT | SLAB_PANIC |
						SLAB_MEM_SPREAD | SLAB_ACCOUNT),
					NULL);
```
固定time行356–360。

### SB38-A056｜mapping取内嵌i_data地址

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
int inode_init_always(super_block_s *sb, inode_s *inode)
{
	static const inode_ops_s empty_iops;
	static const file_ops_s no_open_fops = {.open = no_open};
	addr_spc_s *const mapping = &inode->i_data;
```
固定time行85–89。

### SB38-A057｜每次初始化size和blocks为零

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	inode->i_size = 0;
	// inode->i_write_hint = WRITE_LIFE_NOT_SET;
	inode->i_blocks = 0;
```
固定time行104–106。

### SB38-A058｜每次初始化最终赋i_mapping

[VERIFIED mykernel/fs/vfs/inode.c::inode_init_always]
```c
	mapping->private_data = NULL;
	mapping->writeback_index = 0;
	inode->i_private = NULL;
	inode->i_mapping = mapping;
```
固定time行134–137。

### SB38-A059｜new_inode非NULL后列表节点初始化

[VERIFIED mykernel/fs/vfs/inode.c::new_inode]
```c
	if (inode != NULL)
	{
		inode->i_sb = sb;
		INIT_LIST_S(&inode->i_sb_list);
	}
```
固定time行279–283。

### SB38-A060｜new_inode发布列表后返回

[VERIFIED mykernel/fs/vfs/inode.c::new_inode]
```c
	if (inode != NULL)
		inode_sb_list_add(inode);

	return inode;
}
```
固定time行286–290。

### SB38-A061｜FAT外层对象实际包含vfs_inode

[VERIFIED mykernel/include/linux/fs/fat.h::msdos_inode_info_s]
```c
		inode_s vfs_inode;
	} msdos_inode_info_s;
```
固定time行130–131。

### SB38-A062｜container_of要求真实包含对象

[VERIFIED mykernel/include/linux/fs/fat.h::MSDOS_I]
```c
	static inline msdos_inode_info_s *
	MSDOS_I(inode_s *inode) {
		return container_of(inode, msdos_inode_info_s, vfs_inode);
	}
```
固定time行174–177。

### SB38-A063｜先写i_mode，再写外层i_pos

[VERIFIED mykernel/fs/fat/inode.c::fat_fill_inode]
```c
	inode->i_mode = 0;
	MSDOS_I(inode)->i_pos = 0;
```
固定time行144–145。

### SB38-A064｜明确申请外层msdos_inode_info_s

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c
{
	msdos_inode_info_s *ei;
	ei = kmalloc(sizeof(msdos_inode_info_s), GFP_KERNEL);
	if (ei == NULL)
		return NULL;
```
固定time行238–242。

### SB38-A065｜只在外层五字段写零

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c
	ei->mmu_private = 0;
	ei->i_start = 0;
	ei->i_logstart = 0;
	ei->i_attrs = 0;
	ei->i_pos = 0;
```
固定time行245–249。

### SB38-A066｜返回外层对象的内嵌inode地址

[VERIFIED mykernel/fs/fat/inode.c::fat_alloc_inode]
```c
	return &ei->vfs_inode;
}
```
固定time行251–252。

