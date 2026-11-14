---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-DIRECTORY-MUTATION-57
record_type: direct_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
produced_by: GPT-6（聊天界面模型族；后端身份不可独立认证）
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
source_pin_short12: a039d9803ade
base_snapshot: time（分支名）
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第57批直接源码锚点

每个标签引用固定time源码中对应定义体内的1–5行连续原文，带原tab；CRLF源保留对应行尾。句子只是与证据的局部关联，多行/跨文件结论须合读正文中的前提及最早停止点。字符串实现的`strrchr`受条件编译控制，此处不认证编译选择；所有结果均为静态源事实或显式条件模型，不是内核运行。

## A：名称与槽生成

### A01 vfat_striptail_len

qstr长度与尾点剪裁。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_striptail_len]
```c
	size_t len = qname->len;
	const unsigned char *name = qname->name;
	while (len && name[len - 1] == '.')
		len--;
	return len;
```

### A02 myos_vfat_create_shortname

临时大写串申请和复制。

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
	memset(name_res, ' ', MSDOS_NAME);
	char *name_upper = kzalloc(len + 1, GFP_KERNEL);
	for (int i = 0; i < len; i++)
		name_upper[i] = toupper(name[i]);
	name_upper[len] = '\0';
```

### A03 myos_vfat_create_shortname

NUL字符串末点检索。

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
	char *last = strrchr(name, '.');
	if (last == NULL)
		ext_idx = len;
	else
		ext_idx = last - name + 1;
```

### A04 myos_vfat_create_shortname

短长度直接复制原串字节。

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
	if (len <= 8)
	{
		memcpy(name_res, name_upper, len);
		// ret_val = 1;
```

### A05 myos_vfat_create_shortname

长分支长度和短别名字段。

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
		int baselen = ext_idx > 6 ? 6 : ext_idx;
		int extlen = len - ext_idx > 3 ? 3 : len - ext_idx;
		memcpy(name_res, name_upper, baselen);
		memcpy(name_res + 8, name_upper + ext_idx, extlen);
		name_res[6] = '~';
```

### A06 myos_vfat_create_shortname

固定~1及ret_val返回。

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
		name_res[6] = '~';
		name_res[7] = '1';
	}

	return ret_val;
```

### A07 strrchr

fallback定义扫描至NUL，编译选择未证。

[VERIFIED mykernel/lib/string/string.c::strrchr]
```c
		do {
			if (*s == (char)c)
				last = s;
		} while (*s++);
		return (char *)last;
```

### A08 ascii_to16

逐字节扩充为低字节加零高字节。

[VERIFIED mykernel/fs/fat/namei_vfat.c::ascii_to16]
```c
	for (int i = 0; i < len; i++)
	{
		buf[2 * i] = name[i];
		buf[2 * i + 1] = 0;
	}
```

### A09 vfat_build_slots

短别名返回值选择。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	err = myos_vfat_create_shortname(name, len, msdos_name);
	if (err < 0)
		goto out_free;
	else if (err == 1) {
		de = (msdos_dirent_s *)slots;
```

### A10 vfat_build_slots

长槽数及namebuf的申请。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	cksum = fat_checksum(msdos_name);

	*nr_slots = (len + 12) / 13;
	char *namebuf = kzalloc(*nr_slots * 13, GFP_KERNEL);
	memcpy(namebuf, name, len);
```

### A11 vfat_build_slots

倒序长槽和校验和。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	for (ps = slots, i = *nr_slots; i > 0; i--, ps++) {
		ps->id = i;
		ps->attr = ATTR_EXT;
		ps->reserved = 0;
		ps->alias_checksum = cksum;
```

### A12 vfat_build_slots

长槽三个名字片段。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
		offset = (i - 1) * 13;
		ascii_to16(ps->name0_4, namebuf + offset, 5);
		ascii_to16(ps->name5_10, namebuf + offset + 5, 6);
		ascii_to16(ps->name11_12, namebuf + offset + 11, 2);
```

### A13 vfat_build_slots

首槽0x40以及短槽指针。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	slots[0].id |= 0x40;
	de = (msdos_dirent_s *)ps;
```

### A14 vfat_build_slots

短槽计入数量和父inode元信息。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	(*nr_slots)++;
	memcpy(de->name, msdos_name, MSDOS_NAME);
	de->attr		= is_dir ? ATTR_DIR : ATTR_ARCH;
	de->lcase		= finode->lcase;
	de->ctime_cs	= finode->create_time_cs;
```

### A15 vfat_build_slots

短槽簇号与size=0。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_build_slots]
```c
	fat_set_start(de, cluster);
	de->size = 0;
out_free:
	return err;
```

### A16 vfat_add_entry

剪裁后的零长拒绝与slot申请前缀。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_add_entry]
```c
	len = vfat_striptail_len((qstr_s *)qname);
	if (len == 0)
		return -ENOENT;

	slots = kzalloc(MSDOS_SLOTS * sizeof(msdos_dirent_s), GFP_KERNEL);
```

### A17 vfat_add_entry

构造结果与插入。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_add_entry]
```c
	err = vfat_build_slots(dir, qname->name, len, is_dir, cluster, slots, &nr_slots);
	if (err)
		goto cleanup;

	err = fat_add_entries(dir, slots, nr_slots);
```

### A18 vfat_add_entry

目录插入返回与仅释放slots。

[VERIFIED mykernel/fs/fat/namei_vfat.c::vfat_add_entry]
```c
	err = fat_add_entries(dir, slots, nr_slots);

cleanup:
	kfree(slots);
	return err;
```

### A19 fat_add_entries

iobuf初始化及首次循环。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
	iobuf->iter_init(iobuf);

	do
```

### A20 fat_add_entries

到end后未检查expand结果就再next。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		de = iobuf->next(iobuf);
		if (iobuf->iter_reach_end)
		{
			FAT32_iobuf_expand(iobuf, 1);
			de = iobuf->next(iobuf);
```

### A21 fat_add_entries

首空位置与后继计数。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		if (FAT32_ent_empty(de))
		{
			if (empty_off == -1)
				empty_off = iobuf->iter_cursor - sizeof(msdos_dirent_s);
			else
```

### A22 fat_add_entries

占用项重置连续空槽状态。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		}
		else
		{
			empty_off = -1;
			emtpy_count = 0;
```

### A23 fat_add_entries

nr_slots比较和目录上界。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		if (emtpy_count == nr_slots)
			break;
	}
	while (iobuf->iter_cursor < FAT_MAX_DIR_SIZE);
```

### A24 fat_add_entries

仅局部error并跳清理。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	if (emtpy_count == 0 && empty_off == -1)
	{
		error = -ENOENT;
		goto not_found;
	}
```

### A25 fat_add_entries

按nr_slots固定长度调用iobuf_write。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	else
	{
		FAT32_iobuf_write(iobuf, empty_off, slots,
				nr_slots * sizeof(msdos_dirent_s));
	}
```

### A26 fat_add_entries

返回固定0。

[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
not_found:
	FAT32_iobuf_release(iobuf);
	return 0;
```

### A27 FAT32_iobuf_write

仅初始off门与bufidx计算。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return -ENOENT;
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
```

### A28 FAT32_iobuf_write

单buffer逐字写与DIRTY位。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
	for (size_t i = 0; i < size; i++)
	{
		iobuf->buffers[bufidx][bufoff + i] = content[i];
		iobuf->flags[bufidx] |= FAT32_IOBUF_DIRTY;
	}
```

### A29 MSDOS_NAME

短项11字节名字宏。

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_NAME]
```c
	#define MSDOS_NAME			11				/* maximum name length */
```

### A30 MSDOS_SLOTS

固定21个slot容量。

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MSDOS_SLOTS]
```c
	#define MSDOS_SLOTS			21				/* max # of slots for short and long names */
```

### A31 msdos_dirent_s

短项packed结构首部。

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirent_s]
```c
	typedef struct msdos_dir_entry {
		__u8	name[MSDOS_NAME];	/* name and extension 
										-DIR_Name- */
		__u8	attr;				/* attribute bits
										-DIR_Attr- */
```

### A32 msdos_dirslot_s

长项packed结构首部。

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
	/* Up to 13 characters of the name */
	typedef struct msdos_dir_slot {
		__u8	id;					/* sequence number for slot
										-LDIR_Ord- */
		__u8	name0_4[10];		/* first 5 characters in name
```

### A33 qstr_s

长度与name指针独立。

[VERIFIED mykernel/include/linux/fs/dcache.h::qstr_s]
```c
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
```


## B：创建与发布

### B01 FAT32_create

创建传入cluster=0的调用。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_create]
```c
	int err;
	fat_slot_info_s sinfo;

	err = vfat_add_entry(dir, &dentry->d_name, false, 0, &sinfo);
	if (err)
```

### B02 FAT32_create

直接返回err。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_create]
```c

	out:
	return err;
}
```

### B03 FAT32_mkdir

u32返回窄化到int并做负值门。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mkdir]
```c

	cluster = FAT32_alloc_new_dir(dir);
	if (cluster < 0) {
		err = cluster;
		goto out;
```

### B04 FAT32_mkdir

插入父目录与返回。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mkdir]
```c
	err = vfat_add_entry(dir, &dentry->d_name, true, cluster, &sinfo);
out:
	return err;
```

### B05 FAT32_alloc_new_dir

扇区缓冲长度及申请。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	char *buf;
	size_t bufsize = fsbi->bytes_per_sector;
	msdos_dirent_s *de;

	buf = kzalloc(bufsize, GFP_KERNEL);
```

### B06 FAT32_alloc_new_dir

malloc失败和簇候选失败出口。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	if (buf == NULL)
		return -(ENOMEM);
	cluster = FAT32_find_available_cluster(fsbi);
	if (cluster < 2)
		return cluster;
```

### B07 FAT32_alloc_new_dir

点与点点的name和属性。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	memcpy(de[0].name, MSDOS_DOT, MSDOS_NAME);
	memcpy(de[1].name, MSDOS_DOTDOT, MSDOS_NAME);
	de[0].attr = de[1].attr = ATTR_DIR;
	de[0].lcase = de[1].lcase = 0;
	de[0].size = de[1].size = 0;
```

### B08 FAT32_alloc_new_dir

两个目录项起始簇与盘WRITE请求。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	fat_set_start(&de[0], cluster);
	fat_set_start(&de[1], finode->first_cluster);

	sector = FAT32_clus_to_blknr(fsbi, cluster);
	if(ROOTBLK_TRANSFER(CMD_WRITE, sector, 1, (unchar *)buf))
```

### B09 FAT32_alloc_new_dir

块写异常局部返回。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	if(ROOTBLK_TRANSFER(CMD_WRITE, sector, 1, (unchar *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
```

### B10 FAT32_alloc_new_dir

FAT EOF标记与簇返回。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	FAT32_write_FAT_Entry(fsbi, cluster, FAT_ENT_EOF);
	return cluster;
```

### B11 fat_set_start

起始簇两半写入。

[VERIFIED mykernel/include/linux/fs/fat.h::fat_set_start]
```c
	fat_set_start(msdos_dirent_s *de, int cluster) {
		de->start   = cluster;
		de->starthi = cluster >> 16;
	}
```

### B12 FAT32_write_FAT_Entry

FAT更新与循环WRITE。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);

	for(i = 0; i < fsbi->NumFATs; i++)
		ROOTBLK_TRANSFER(CMD_WRITE,
			fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
```

### B13 FAT32_write_FAT_Entry

FAT setter固定1。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
			SECT_PER_PG, (unchar *)buf);
	return 1;	
```

### B14 vfat_dir_inode_operations

目录操作表create/lookup/mkdir/rmdir。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::vfat_dir_inode_operations]
```c
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
	.rmdir = FAT32_rmdir,
	// .rename = FAT32_rename,
```

### B15 do_creatat

creat的直接回调及返回。

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

### B16 do_mkdirat

mkdir的直接回调及清理。

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```

### B17 done_path_create

仅dput/path_put的邻接清理。

[VERIFIED mykernel/fs/vfs/namei.c::done_path_create]
```c
void done_path_create(path_s *path, dentry_s *dentry)
{
	dput(dentry);
	path_put(path);
}
```

### B18 dentry_s

d_name与d_inode是独立字段。

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		dentry_s			*d_parent;		/* parent directory */
		qstr_s				d_name;
		inode_s				*d_inode;		/* Where the name belongs to - NULL is
											* negative */
```


## C：判空与删除

### C01 FAT32_dir_empty

判空的正常初值与iobuf。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_dir_empty]
```c
	int result = ENOERR;
	size_t pos = 0;
	size_t bufsize = 0;
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
```

### C02 FAT32_dir_empty

普通项检测须先过getter和两个名字门。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_dir_empty]
```c
	while (!IS_ERR(de = FAT32_get_full_ent(iobuf)))
	{
		if (!FAT32_ent_empty(de) &&
			strncmp(de->name, MSDOS_DOT   , MSDOS_NAME) &&
			strncmp(de->name, MSDOS_DOTDOT, MSDOS_NAME))
```

### C03 FAT32_dir_empty

非空时返回前的break与pos计数。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_dir_empty]
```c
			result = -ENOTEMPTY;
			break;
		}
		pos += sizeof(msdos_dirent_s);
	}
```

### C04 FAT32_dir_empty

释放缓冲与result返回。

[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_dir_empty]
```c
	FAT32_iobuf_release(iobuf);
	return result;
```

### C05 FAT32_ent_empty

空项首字节的局部判据。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_ent_empty]
```c
	return (de->name[0] == 0xe5 ||
			de->name[0] == 0x00 ||
			de->name[0] == 0x05);
```

### C06 FAT32_get_full_ent

错误指针上可能先读attr的入口。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
		ret_val = iobuf->next(iobuf);
		tmpval = (msdos_dirslot_s *)ret_val;
	}
	while ((ret_val->attr == ATTR_LONG_NAME ||
			FAT32_ent_empty(ret_val)) &&
```

### C07 FAT32_get_full_ent

end时覆盖当前返回为-ENOENT。

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_get_full_ent]
```c
	if (iobuf->iter_reach_end)
		ret_val = ERR_PTR(-ENOENT);

	return ret_val;
```

### C08 FAT32_release_clusters

倒序置DELETE并调用release。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_release_clusters]
```c
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
	for (int i = iobuf->nr_clus - 1; i >= 0; i--)
		iobuf->flags[i] = FAT32_IOBUF_DELETE;

	FAT32_iobuf_release(iobuf);
```

### C09 FAT32_mark_entry_romoved

每32字节向父目录起点写删除标志。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mark_entry_romoved]
```c
	for (size_t i = 0; i < length; i+=sizeof(msdos_dirent_s))
	{
		char c = FAT32_DELETED_FLAG;
		FAT32_iobuf_write(iobuf, i + off, &c, 1);
	}
```

### C10 FAT32_mark_entry_romoved

忽略write结果并release。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mark_entry_romoved]
```c
		FAT32_iobuf_write(iobuf, i + off, &c, 1);
	}
	FAT32_iobuf_release(iobuf);
}
```

### C11 FAT32_rmdir

先判空以及非零早返回。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_rmdir]
```c
	FAT32_inode_info_s *dest_finode = inode->private_idx_info;

	error = FAT32_dir_empty(inode);
	if (error != 0)
		return error;
```

### C12 FAT32_rmdir

释放簇后标记父目录并返回error。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_rmdir]
```c
	FAT32_release_clusters(inode);
	FAT32_mark_entry_romoved(parent, dest_finode->dentry_position,
			dest_finode->dentry_length);

	return error;
```

### C13 FAT32_unlink

unlink从dest绑定子inode。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_unlink]
```c
int FAT32_unlink(inode_s *parent, dentry_s *dest)
{
	int error = 0;
	inode_s *inode = dest->d_inode;
	FAT32_inode_info_s *dest_finode = inode->private_idx_info;
```

### C14 FAT32_unlink

无判空，删除后返回0。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_unlink]
```c
	FAT32_release_clusters(inode);
	FAT32_mark_entry_romoved(parent, dest_finode->dentry_position,
			dest_finode->dentry_length);

	return error;
```

### C15 FAT32_lookup

所读lookup产生目录项offset字段。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	finode = p->private_idx_info;

	finode->first_cluster	= (tmpde->starthi << 16 | tmpde->start) & 0x0fffffff;
	finode->dentry_location	= iobuf->clusters[de_start / iobuf->bufsize];
	finode->dentry_position	= de_start;
```

### C16 FAT32_lookup

dentry_length由off减去de_start。

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_lookup]
```c
	finode->create_time		= tmpde->cdate;
	finode->write_date		= tmpde->time;
	finode->write_time		= tmpde->date;
	finode->access_date		= tmpde->adate;
	finode->dentry_length	= off - de_start;
```

### C17 FAT32_inode_info_s

u64目录位置长度字段。

[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_inode_info_s]
```c
		u64	first_cluster;
		u64	dentry_location;	// dentry struct in cluster(0 is root,1 is invalid)
		u64	dentry_position;	// dentry struct offset in cluster
		u64 dentry_start;		// the whole dentry start offset in directory
		u64 dentry_length;		// the whole dentry length
```

### C18 FAT32_iobuf_release

每簇的flags和buffer门。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			bool clus_end_change = false;
			u32 cluster = iobuf->clusters[i];
			u32 flags = iobuf->flags[i];
			char *bufp = iobuf->buffers[i];
			if (bufp != NULL)
```

### C19 FAT32_iobuf_release

DIRTY块写与buffer释放。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
				if (flags == FAT32_IOBUF_DIRTY)
					ROOTBLK_TRANSFER(CMD_WRITE,
						FAT32_clus_to_blknr(iobuf->fsbi, cluster),
						iobuf->bufsize / iobuf->fsbi->bytes_per_sector, bufp);
				kfree(iobuf->buffers[i]);
```

### C20 FAT32_iobuf_release

DELETE的FAT_FREE请求。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			if (flags == FAT32_IOBUF_DELETE)
			{
				clus_end_change = true;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_FREE);
			}
```

### C21 FAT32_iobuf_release

EOF修补受局部互斥条件。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			if (flags != FAT32_IOBUF_DELETE && clus_end_change)
			{
				clus_end_change = false;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_EOF);
			}
```

### C22 FAT32_iobuf_iter_next

readent后游标和end前进。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
		iobuf->iter_reach_end = true;
```

### C23 FAT32_iobuf_readent

超出总长返回错误指针。

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
	int	bufidx = off / iobuf->bufsize;
```

### C24 vfs_rmdir

rmdir回0后VFS置S_DEAD。

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
	error = dir->i_op->rmdir(dir, dentry);
	if (error)
		goto out;

	dentry->d_inode->i_flags |= S_DEAD;
```

### C25 vfs_unlink

VFS消费unlink返回。

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
	// error = security_inode_unlink(dir, dentry);
	// if (!error) {
		error = dir->i_op->unlink(dir, dentry);
	// 	if (!error) {
```

### C26 do_unlinkat

外层unlink后的dput流程。

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		error = vfs_unlink(path.dentry->d_inode, dentry);
exit3:
		dput(dentry);
	}
	if (inode)
```

