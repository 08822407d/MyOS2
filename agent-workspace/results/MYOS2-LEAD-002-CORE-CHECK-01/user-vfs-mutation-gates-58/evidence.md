---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-MUTATION-GATES-58
record_type: bounded_source_evidence_with_D00_correction
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
base_snapshot: time
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---
# 第58批源锚（D00与A/B/C独立）

证据从固定time的真实函数／结构／宏定义内连续1–5行截取；保留源码原有tab与行尾，定义前的解释性注释不是定义内证据。跨片段断言应合读正文条件、完整活动函数和第57批主线回执，不把切片数当独立缺陷数。内核及仓库程序NOT_RUN。

## D00 · 第57批退回文档纠正

### D01 · msdos_dirslot_s

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
	typedef struct msdos_dir_slot {
		__u8	id;					/* sequence number for slot
										-LDIR_Ord- */
		__u8	name0_4[10];		/* first 5 characters in name
```

### D02 · msdos_dirslot_s

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__u8	alias_checksum;		/* checksum for 8.3 alias
										-LDIR_Chksum- */
		__u8	name5_10[12];		/* 6 more characters in name
										-LDIR_Name2- */
		__le16	start;				/* starting cluster number, 0 in long slots
```

### D03 · msdos_dirslot_s

[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::msdos_dirslot_s]
```c
		__u8	name11_12[4];		/* last 2 characters in name
										-LDIR_Name3- */
	}__attribute__((packed)) msdos_dirslot_s;
```

### D04 · setup

[VERIFIED mykernel/fs/fat/namei_vfat.c::setup]
```c
static void setup(super_block_s *sb)
{
	MSDOS_SB(sb)->dir_ops = &vfat_dir_inode_operations;
	// if (MSDOS_SB(sb)->options.name_check != 's')
```

### D05 · vfat_dir_inode_operations

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::vfat_dir_inode_operations]
```c
inode_ops_s vfat_dir_inode_operations = 
{
	.create = FAT32_create,
	.lookup = FAT32_lookup,
	.mkdir = FAT32_mkdir,
```

### D06 · FAT32_iobuf_write

[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return -ENOENT;
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
```

### D07 · FAT32_mark_entry_romoved

[VERIFIED mykernel/fs/fat/myos_fat32_namei.c::FAT32_mark_entry_romoved]
```c
	for (size_t i = 0; i < length; i+=sizeof(msdos_dirent_s))
	{
		char c = FAT32_DELETED_FLAG;
		FAT32_iobuf_write(iobuf, i + off, &c, 1);
	}
```

### D08 · myos_vfat_create_shortname

[VERIFIED mykernel/fs/fat/namei_vfat.c::myos_vfat_create_shortname]
```c
		int baselen = ext_idx > 6 ? 6 : ext_idx;
		int extlen = len - ext_idx > 3 ? 3 : len - ext_idx;
		memcpy(name_res, name_upper, baselen);
		memcpy(name_res + 8, name_upper + ext_idx, extlen);
```


## A · 第58批VFS限定分析

### A01 · getname

[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
filename_s *getname(const char *u_filename)
{
	size_t len = strlen_user((void *)u_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
```

### A02 · getname

[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
		filename_s *name = __myos_getname();
	// }

	name->len = len;
	name->name = kzalloc(len + 16, GFP_KERNEL);
```

### A03 · getname_kernel

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	filename_s *name = __myos_getname();

	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
```

### A04 · filename_s

[VERIFIED mykernel/include/linux/fs/fs.h::filename_s]
```c
	typedef struct filename {
		const char	*name;	/* pointer to actual string */
		const char	*uptr;	/* original userland pointer */
		int			len;
		// int			refcnt;
```

### A05 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
	nd->last_type = LAST_ROOT;
	if (IS_ERR(name))
		return PTR_ERR(name);
	while (*name == '/')
		name++;
```

### A06 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		// compute length of current dirname
		int i = 0;
		while (name[i] != 0 && name[i] != '/')
			i++;

```

### A07 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		type = LAST_NORM;
		// detect . and .. dir
		if (name[0] == '.')
			switch (i)
			{
```

### A08 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
				if (name[1] == '.')
				{
					type = LAST_DOTDOT;
				}
				break;
```

### A09 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		nd->last.len = i;
		nd->last.name = name;
		nd->last_type = type;

		name += nd->last.len;
```

### A10 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c
		 * If it wasn't NUL, we know it was '/'. Skip that
		 * slash, and continue until no more slashes.
		 */
		while (*name == '/')
			name++;
```

### A11 · link_path_walk

[VERIFIED mykernel/fs/vfs/namei.c::link_path_walk]
```c

		if (*name == 0)
			return -ENOERR;
		else
		{
```

### A12 · path_parentat

[VERIFIED mykernel/fs/vfs/namei.c::path_parentat]
```c
	const char *s = path_init(nd, flags);
	int err = link_path_walk(s, nd);
	if (err == 0)
		err = complete_walk(nd);
	if (err == 0) {
```

### A13 · path_parentat

[VERIFIED mykernel/fs/vfs/namei.c::path_parentat]
```c
		*parent = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
	terminate_walk(nd);
```

### A14 · filename_parentat

[VERIFIED mykernel/fs/vfs/namei.c::filename_parentat]
```c

	if (IS_ERR(name))
		return PTR_ERR(name);
	set_nameidata(&nd, dfd, name, NULL);
	retval = path_parentat(&nd, flags, parent);
```

### A15 · filename_parentat

[VERIFIED mykernel/fs/vfs/namei.c::filename_parentat]
```c
	if (retval == -ECHILD)
		retval = path_parentat(&nd, flags, parent);
	if (retval == 0) {
		*last = nd.last;
		*type = nd.last_type;
```

### A16 · path_init

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;

	nd->flags = flags;
```

### A17 · __lookup_hash

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_hash]
```c
	dentry_s *dentry = __d_lookup(base, name);
	dentry_s *old;
	inode_s *dir = base->d_inode;

	if (dentry)
```

### A18 · __lookup_hash

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_hash]
```c
	if (dentry)
		return dentry;

	/* Don't create child dentry for a dead directory. */
	if (IS_DEADDIR(dir))
```

### A19 · __lookup_hash

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_hash]
```c
	dentry = __myos_d_alloc(base->d_sb, name);
	if (!dentry)
		return ERR_PTR(-ENOMEM);
	dentry->d_parent = base;
	list_header_add_to_tail(&base->d_subdirs, &dentry->d_child);
```

### A20 · __lookup_hash

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_hash]
```c
	old = dir->i_op->lookup(dir, dentry, flags);
	if (old) {
		dput(dentry);
		dentry = old;
	}
```

### A21 · __d_lookup

[VERIFIED mykernel/fs/vfs/dcache.c::__d_lookup]
```c
		dir_p = container_of(dir_lp, dentry_s, d_child);
		if (dir_p != NULL && !strncmp(name->name, dir_p->d_name.name, name->len))
		{
			dentry = dir_p;
			break;
```

### A22 · __myos_d_alloc

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry_s * dentry = kzalloc(sizeof(dentry_s), GFP_KERNEL);
	if (dentry == NULL)
		return ERR_PTR(-ENOMEM);

	dentry->d_name.name = kzalloc(name->len + 1, GFP_KERNEL);
```

### A23 · __myos_d_alloc

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry->d_name.name = kzalloc(name->len + 1, GFP_KERNEL);
	if (dentry->d_name.name == NULL)
	{
		kfree(dentry);
		return ERR_PTR(-ENOMEM);
```

### A24 · __myos_d_alloc

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry->d_name.len = name->len;
	memcpy((void *)dentry->d_name.name, name->name, name->len);

	dentry->d_flags = 0;
	dentry->d_inode = NULL;
```

### A25 · qstr_s

[VERIFIED mykernel/include/linux/fs/dcache.h::qstr_s]
```c
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
```

### A26 · kzalloc

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```

### A27 · LAST_NORM

[VERIFIED mykernel/include/linux/fs/namei.h::LAST_NORM]
```c
	enum {
		LAST_NORM,
		LAST_ROOT,
		LAST_DOT,
		LAST_DOTDOT
```

### A28 · set_nameidata

[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		memset(p, 0, sizeof(nameidata_s));

		p->name = name;
		p->dfd = dfd;
		p->path.mnt = NULL;
```


## B · 第58批VFS限定分析

### B01 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	qstr_s last;
	int type;
	int err2;
	int error;
	bool is_dir = (lookup_flags & LOOKUP_DIRECTORY);
```

### B02 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	/*
	 * Note that only LOOKUP_REVAL and LOOKUP_DIRECTORY matter here. Any
	 * other flags passed in are ignored!
	 */
	lookup_flags &= LOOKUP_REVAL;
```

### B03 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	error = filename_parentat(dfd, name, lookup_flags, path, &last, &type);
	if (error)
		return ERR_PTR(error);
```

### B04 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	if (type != LAST_NORM)
		goto out;
```

### B05 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	lookup_flags |= LOOKUP_CREATE | LOOKUP_EXCL;
	dentry = __lookup_hash(&last, path->dentry, lookup_flags);
	if (IS_ERR(dentry))
		goto out;
```

### B06 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
	if (!is_dir && last.name[last.len] != '\0') {
		error = -ENOENT;
		goto fail;
	}
	return dentry;
```

### B07 · filename_create

[VERIFIED mykernel/fs/vfs/namei.c::filename_create]
```c
fail:
	dput(dentry);
	dentry = ERR_PTR(error);
out:
	path_put(path);
```

### B08 · do_creatat

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
	unsigned int lookup_flags = LOOKUP_DIRECTORY;

retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
	error = PTR_ERR(dentry);
```

### B09 · do_creatat

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		error = -ENOENT;
		if (!dir->i_op->create)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->create(dir, dentry, mode);
```

### B10 · do_creatat

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
		error = dir->i_op->create(dir, dentry, mode);
	done_path_create(&path, dentry);
```

### B11 · do_creatat

[VERIFIED mykernel/fs/vfs/namei.c::do_creatat]
```c
out_putname:
	putname(name);
	return error;
```

### B12 · do_mkdirat

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
retry:
	dentry = filename_create(dfd, name, &path, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out_putname;
```

### B13 · do_mkdirat

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		error = -ENOENT;
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
```

### B14 · do_mkdirat

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
	done_path_create(&path, dentry);
```

### B15 · do_mkdirat

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
out_putname:
	putname(name);
	return error;
```

### B16 · vfs_mkdir

[VERIFIED mykernel/fs/vfs/namei.c::vfs_mkdir]
```c
int vfs_mkdir(inode_s *dir, dentry_s *dentry, umode_t mode)
{
	int error = -ENOENT;
	if (!dir->i_op->mkdir)
		return -EPERM;
```

### B17 · vfs_mkdir

[VERIFIED mykernel/fs/vfs/namei.c::vfs_mkdir]
```c

	mode &= (S_IRWXUGO|S_ISVTX);
	error = dir->i_op->mkdir(dir, dentry, mode);
	return error;
```

### B18 · done_path_create

[VERIFIED mykernel/fs/vfs/namei.c::done_path_create]
```c
void done_path_create(path_s *path, dentry_s *dentry)
{
	dput(dentry);
	path_put(path);
}
```

### B19 · kern_path_create

[VERIFIED mykernel/fs/vfs/namei.c::kern_path_create]
```c
dentry_s *kern_path_create(int dfd, const char *pathname,
		path_s *path, unsigned int lookup_flags)
{
	filename_s *filename = getname_kernel(pathname);
	dentry_s *res = filename_create(dfd, filename, path, lookup_flags);
```

### B20 · user_path_create

[VERIFIED mykernel/fs/vfs/namei.c::user_path_create]
```c
inline dentry_s *user_path_create(int dfd, const char *pathname,
		path_s *path, unsigned int lookup_flags)
{
	filename_s *filename = getname(pathname);
	dentry_s *res = filename_create(dfd, filename, path, lookup_flags);
```

### B21 · putname

[VERIFIED mykernel/fs/vfs/namei.c::putname]
```c
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}
```

### B22 · path_put

[VERIFIED mykernel/fs/vfs/namei.c::path_put]
```c
void path_put(const path_s *path)
{
	dput(path->dentry);
	// mntput(path->mnt);
}
```

### B23 · LOOKUP_DIRECTORY

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_DIRECTORY]
```c
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
```

### B24 · LOOKUP_CREATE

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_CREATE]
```c
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```

### B25 · init_mkdir

[VERIFIED mykernel/fs/vfs/init.c::init_mkdir]
```c
	dentry = kern_path_create(AT_FDCWD, pathname, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	error = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	done_path_create(&path, dentry);
```


## C · 第58批VFS限定分析

### C01 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	error = filename_parentat(dfd, name, lookup_flags, &path, &last, &type);
	if (error)
		goto exit1;

```

### C02 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	switch (type) {
	case LAST_DOTDOT:
		error = -ENOTEMPTY;
		goto exit2;
	case LAST_DOT:
```

### C03 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
		error = -EINVAL;
		goto exit2;
	case LAST_ROOT:
		error = -EBUSY;
		goto exit2;
```

### C04 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto exit3;
	if (!dentry->d_inode) {
```

### C05 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	if (!dentry->d_inode) {
		error = -ENOENT;
		goto exit4;
	}
	if (!S_ISDIR(dentry->d_inode->i_mode))
```

### C06 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
	if (!S_ISDIR(dentry->d_inode->i_mode))
	{
		error = -ENOTDIR;
		goto exit4;
	}
```

### C07 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c

	error = vfs_rmdir(path.dentry->d_inode, dentry);
exit4:
	dput(dentry);
exit3:
```

### C08 · do_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::do_rmdir]
```c
exit2:
	path_put(&path);
exit1:
	putname(name);
	return error;
```

### C09 · vfs_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
	if (!dir->i_op->rmdir)
		return -EPERM;

	dget(dentry);
```

### C10 · vfs_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
	error = -EBUSY;
	error = dir->i_op->rmdir(dir, dentry);
	if (error)
		goto out;

```

### C11 · vfs_rmdir

[VERIFIED mykernel/fs/vfs/namei.c::vfs_rmdir]
```c
	dentry->d_inode->i_flags |= S_DEAD;

out:
	dput(dentry);
	return error;
```

### C12 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	error = filename_parentat(dfd, name, lookup_flags, &path, &last, &type);
	if (error)
		goto exit1;

	error = -EISDIR;
```

### C13 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	error = -EISDIR;
	if (type != LAST_NORM)
		goto exit2;

	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
```

### C14 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	dentry = __lookup_hash(&last, path.dentry, lookup_flags);
	error = PTR_ERR(dentry);
	if (!IS_ERR(dentry)) {
// 		/* Why not before? Because we want correct error value */
// 		if (last.name[last.len])
```

### C15 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		inode = dentry->d_inode;
		if (S_ISDIR(inode->i_mode))
		{
			error = -EISDIR;
			goto exit3;
```

### C16 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		error = vfs_unlink(path.dentry->d_inode, dentry);
exit3:
		dput(dentry);
	}
	if (inode)
```

### C17 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
		iput(inode);	/* truncate the inode here */
	inode = NULL;

exit2:
	path_put(&path);
```

### C18 · do_unlinkat

[VERIFIED mykernel/fs/vfs/namei.c::do_unlinkat]
```c
	path_put(&path);
exit1:
	putname(name);
	return error;
```

### C19 · vfs_unlink

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
{
	inode_s *target = dentry->d_inode;
	int error = 0;

	if (!dir->i_op->unlink)
```

### C20 · vfs_unlink

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
	// error = security_inode_unlink(dir, dentry);
	// if (!error) {
		error = dir->i_op->unlink(dir, dentry);
```

### C21 · vfs_unlink

[VERIFIED mykernel/fs/vfs/namei.c::vfs_unlink]
```c
	// }

	return error;
```

### C22 · dget

[VERIFIED mykernel/include/linux/fs/dcache.h::dget]
```c
	static inline dentry_s *
	dget(dentry_s *dentry) {
		// if (dentry)
		// 	lockref_get(&dentry->d_lockref);
		return dentry;
```

### C23 · dput

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
void dput(dentry_s *dentry)
{
	// while (dentry) {
	// 	might_sleep();

```

### C24 · dput

[VERIFIED mykernel/fs/vfs/dcache.c::dput]
```c
	// 	}

	// 	dentry = dentry_kill(dentry);
	// }
}
```

### C25 · iput

[VERIFIED mykernel/fs/vfs/inode.c::iput]
```c
void iput(inode_s *inode)
{
	if (inode == NULL)
		return;

```

### C26 · iput

[VERIFIED mykernel/fs/vfs/inode.c::iput]
```c
	// 		goto retry;
	// 	}
	// 	iput_final(inode);
	// }
}
```

