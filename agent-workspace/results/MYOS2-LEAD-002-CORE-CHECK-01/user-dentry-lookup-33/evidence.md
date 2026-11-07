---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DENTRY-LOOKUP-33
record_type: direct_fixed_source_definition_quotes
date: 2026-11-03
source_snapshot: time
source_pin_short12: a039d9803ade
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
runtime_evidence: NOT_RUN
---

# 第33批直接源码证据

本文件共48个直接定义体锚点，每段一至五行；均从固定time的实际源码读取而来。各标题仅说明引文直接支持的有限事实，分支条件、上游有效对象和下游可能性参见正文与facts。编号、路径和符号都不能替代真正的引文。

### DL33-A001：qstr长度和借用名字指针

[VERIFIED mykernel/include/linux/fs/dcache.h::qstr_s]
```c
	typedef struct qstr {
		size_t				len;
		const unsigned char	*name;
	} qstr_s;
```

本次源范围：47–50行，符号定义体内；不是运行证据。

### DL33-A002：dentry父指针、名字与inode

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		dentry_s			*d_parent;		/* parent directory */
		qstr_s				d_name;
		inode_s				*d_inode;		/* Where the name belongs to - NULL is
											* negative */
```

本次源范围：72–75行，符号定义体内；不是运行证据。

### DL33-A003：child与subdirs成员

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		List_s				d_child;		/* child of parent list */
		List_hdr_s			d_subdirs;		/* our children */
```

本次源范围：85–86行，符号定义体内；不是运行证据。

### DL33-A004：快查类型与初始未命中NULL

[VERIFIED mykernel/fs/vfs/dcache.c::__d_lookup]
```c
dentry_s * __d_lookup(const dentry_s * parent, const qstr_s * name)
{
	dentry_s *	dentry = NULL;
```

本次源范围：133–135行，符号定义体内；不是运行证据。

### DL33-A005：遍历父d_subdirs子链表

[VERIFIED mykernel/fs/vfs/dcache.c::__d_lookup]
```c
	for (dir_lp = parent->d_subdirs.anchor.next;
			dir_lp != &parent->d_subdirs.anchor;
			dir_lp = dir_lp->next)
	{
		dir_p = container_of(dir_lp, dentry_s, d_child);
```

本次源范围：139–143行，符号定义体内；不是运行证据。

### DL33-A006：仅用查询长度strncmp决定首次命中

[VERIFIED mykernel/fs/vfs/dcache.c::__d_lookup]
```c
		dir_p = container_of(dir_lp, dentry_s, d_child);
		if (dir_p != NULL && !strncmp(name->name, dir_p->d_name.name, name->len))
		{
			dentry = dir_p;
			break;
```

本次源范围：143–147行，符号定义体内；不是运行证据。

### DL33-A007：命中break后返回或未命中NULL

[VERIFIED mykernel/fs/vfs/dcache.c::__d_lookup]
```c
			dentry = dir_p;
			break;
		}
	}
	return dentry;
```

本次源范围：146–150行，符号定义体内；不是运行证据。

### DL33-A008：本仓条件编译的strncmp定义体与长度循环

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
	int strncmp(const char *cs, const char *ct, size_t count)
	{
		unsigned char c1, c2;
		while (count) {
			c1 = *cs++;
```

本次源范围：361–365行，符号定义体内；不是运行证据。

### DL33-A009：逐字节比较不等则返回

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			c1 = *cs++;
			c2 = *ct++;
			if (c1 != c2)
				return c1 < c2 ? -1 : 1;
			if (!c1)
```

本次源范围：365–369行，符号定义体内；不是运行证据。

### DL33-A010：遇NUL或到达限制长度的返回

[VERIFIED mykernel/lib/string/string.c::strncmp]
```c
			if (!c1)
				break;
			count--;
		}
		return 0;
```

本次源范围：369–373行，符号定义体内；不是运行证据。

### DL33-A011：分配对象和ERR_PTR(-ENOMEM)失败

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
dentry_s * __myos_d_alloc(super_block_s *sb, const qstr_s * name)
{
	dentry_s * dentry = kzalloc(sizeof(dentry_s), GFP_KERNEL);
	if (dentry == NULL)
		return ERR_PTR(-ENOMEM);
```

本次源范围：164–168行，符号定义体内；不是运行证据。

### DL33-A012：再次分配名字字节并释放对象

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry->d_name.name = kzalloc(name->len + 1, GFP_KERNEL);
	if (dentry->d_name.name == NULL)
	{
		kfree(dentry);
		return ERR_PTR(-ENOMEM);
```

本次源范围：170–174行，符号定义体内；不是运行证据。

### DL33-A013：保存长度、拷贝len字节

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry->d_name.len = name->len;
	memcpy((void *)dentry->d_name.name, name->name, name->len);

```

本次源范围：177–179行，符号定义体内；不是运行证据。

### DL33-A014：flags/inode/parent/sb/op初始化

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	dentry->d_flags = 0;
	dentry->d_inode = NULL;
	dentry->d_parent = dentry;
	dentry->d_sb = sb;
	dentry->d_op = NULL;
```

本次源范围：180–184行，符号定义体内；不是运行证据。

### DL33-A015：两个链表成员初始化

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	INIT_LIST_S(&dentry->d_child);
	INIT_LIST_HEADER_S(&dentry->d_subdirs);

```

本次源范围：186–188行，符号定义体内；不是运行证据。

### DL33-A016：分配器正常返回原对象

[VERIFIED mykernel/fs/vfs/dcache.c::__myos_d_alloc]
```c
	// d_set_d_op(dentry, dentry->d_sb->s_d_op);

	return dentry;
```

本次源范围：189–191行，符号定义体内；不是运行证据。

### DL33-A017：包装器仅检查NULL

[VERIFIED mykernel/fs/vfs/dcache.c::d_alloc]
```c
dentry_s *d_alloc(dentry_s * parent, const qstr_s *name)
{
	dentry_s *dentry = __myos_d_alloc(parent->d_sb, name);
	if (dentry == NULL)
		return NULL;
```

本次源范围：203–207行，符号定义体内；不是运行证据。

### DL33-A018：包装器修改parent并把child加到链表

[VERIFIED mykernel/fs/vfs/dcache.c::d_alloc]
```c
	dentry->d_parent = parent;
	list_header_add_to_head(&parent->d_subdirs, &dentry->d_child);

	return dentry;
```

本次源范围：212–215行，符号定义体内；不是运行证据。

### DL33-A019：慢查函数参数和本地指针

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
static dentry_s *__lookup_slow(IN qstr_s *name,
		IN dentry_s *dir, unsigned flags)
{
	dentry_s *dentry;
	inode_s *inode;
```

本次源范围：308–312行，符号定义体内；不是运行证据。

### DL33-A020：慢查分配器与IS_ERR前门

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
				dentry = __myos_d_alloc(dir->d_sb, name);
	//		}
	// }
	if (IS_ERR(dentry))
		return dentry;
```

本次源范围：320–324行，符号定义体内；不是运行证据。

### DL33-A021：回调只测试NULL，NULL返ENOENT错误指针

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	if (dir->d_inode->i_op->lookup(dir->d_inode, dentry, flags) == NULL)
	{
		// color_printk(RED, WHITE, "can not find file or dir:%s\n", dentry->d_name);
		kfree(dentry);
		return ERR_PTR(-ENOENT);
```

本次源范围：326–330行，符号定义体内；不是运行证据。

### DL33-A022：读取原dentry的inode和页数

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	inode = dentry->d_inode;
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
```

本次源范围：333–335行，符号定义体内；不是运行证据。

### DL33-A023：正页数的页数组分配

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	if (inode->i_mapping->nrpages > 0)
		inode->i_mapping->page_array =
			kzalloc(inode->i_mapping->nrpages * sizeof(page_s *), GFP_KERNEL);

```

本次源范围：335–338行，符号定义体内；不是运行证据。

### DL33-A024：加入父子列表、设置parent、返回原对象

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	list_header_add_to_tail(&dir->d_subdirs, &dentry->d_child);
	dentry->d_parent = dir;

	return dentry;
```

本次源范围：339–342行，符号定义体内；不是运行证据。

### DL33-A025：lookup回调返回dentry_s*、三参数类型

[VERIFIED mykernel/include/linux/fs/fs.h::inode_ops_s]
```c
	typedef struct inode_operations {
		dentry_s	*(*lookup) (inode_s *, dentry_s *, unsigned int);
		// const char	*(*get_link) (dentry_s *, inode_s *, struct delayed_call *);
```

本次源范围：1910–1912行，符号定义体内；不是运行证据。

### DL33-A026：inode操作表与映射成员

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		const inode_ops_s	*i_op;
		super_block_s		*i_sb;
		addr_spc_s			*i_mapping;
```

本次源范围：560–562行，符号定义体内；不是运行证据。

### DL33-A027：inode长度成员

[VERIFIED mykernel/include/linux/fs/fs.h::inode_s]
```c
		dev_t				i_rdev;
		loff_t				i_size;
```

本次源范围：581–582行，符号定义体内；不是运行证据。

### DL33-A028：mapping的page_array字段

[VERIFIED mykernel/include/linux/fs/fs.h::addr_spc_s]
```c
	typedef struct address_space {
		inode_s				*host;
		page_s				**page_array;
		// xarray_s			i_pages;
```

本次源范围：386–389行，符号定义体内；不是运行证据。

### DL33-A029：mapping页数成员

[VERIFIED mykernel/include/linux/fs/fs.h::addr_spc_s]
```c
		rwsem_t				i_mmap_rwsem;
		ulong				nrpages;
		pgoff_t				writeback_index;
```

本次源范围：398–400行，符号定义体内；不是运行证据。

### DL33-A030：非普通末分量先处理点分支

[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
	if (nd->last_type != LAST_NORM)
	{
		return handle_dots(nd);
	}
```

本次源范围：439–442行，符号定义体内；不是运行证据。

### DL33-A031：调用快查并对错误指针转换

[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
		dentry = __d_lookup(parent, &nd->last);
	// }
	if (IS_ERR(dentry))
		return ERR_CAST(dentry);
```

本次源范围：448–451行，符号定义体内；不是运行证据。

### DL33-A032：快查NULL才进入慢查及错误检查

[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
			dentry = __lookup_slow(&nd->last, parent, nd->flags);
		// }
		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
```

本次源范围：459–462行，符号定义体内；不是运行证据。

### DL33-A033：普通查找结果传给step_into

[VERIFIED mykernel/fs/vfs/namei.c::walk_component]
```c
	return step_into(nd, dentry);
```

本次源范围：465–465行，符号定义体内；不是运行证据。

### DL33-A034：数值O_CREAT测试门及快查

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
	if (!(open_flag & O_CREAT))
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
```

本次源范围：672–675行，符号定义体内；不是运行证据。

### DL33-A035：快查NULL触发慢查

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
		if (dentry == NULL)
		{
			dentry = __lookup_slow(&nd->last, dir, nd->flags);
		}
```

本次源范围：675–678行，符号定义体内；不是运行证据。

### DL33-A036：错误与非NULL结果分派到finish

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
		if (IS_ERR(dentry))
			return ERR_CAST(dentry);
		if (dentry)
			goto finish_lookup;
	}
```

本次源范围：680–684行，符号定义体内；不是运行证据。

### DL33-A037：step_into消费局部dentry

[VERIFIED mykernel/fs/vfs/namei.c::open_last_lookups]
```c
finish_lookup:
	res = step_into(nd, dentry);
	return res;
```

本次源范围：690–692行，符号定义体内；不是运行证据。

### DL33-A038：外层循环赋值s及后继比较

[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
```

本次源范围：724–726行，符号定义体内；不是运行证据。

### DL33-A039：flags实际来自op.lookup_flags

[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	int flags = op->lookup_flags;
	file_s *filp;

```

本次源范围：756–758行，符号定义体内；不是运行证据。

### DL33-A040：step_into直接消费dentry

[VERIFIED mykernel/fs/vfs/namei.c::step_into]
```c
	int err = handle_mounts(nd, dentry, &path);
	if (err < 0)
		return ERR_PTR(err);
```

本次源范围：296–298行，符号定义体内；不是运行证据。

### DL33-A041：正常分支ERR_PTR(-ENOERR)返回

[VERIFIED mykernel/fs/vfs/namei.c::step_into]
```c
	return ERR_PTR(-ENOERR);
```

本次源范围：302–302行，符号定义体内；不是运行证据。

### DL33-A042：错误指针编码区间

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
				)
```

本次源范围：18–20行，符号定义体内；不是运行证据。

### DL33-A043：ERR_PTR将整数转成指针

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```

本次源范围：26–29行，符号定义体内；不是运行证据。

### DL33-A044：IS_ERR判断编码区间

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

本次源范围：36–39行，符号定义体内；不是运行证据。

### DL33-A045：ERR_CAST错误指针类别转换

[VERIFIED mykernel/include/linux/kernel/err.h::ERR_CAST]
```c
		ERR_CAST(const void *ptr) {
			/* cast away the const */
			return (void *) ptr;
```

本次源范围：60–62行，符号定义体内；不是运行证据。

### DL33-A046：ENOENT固定数值

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOENT]
```c
	#define	ENOENT		2	/* No such file or directory */
```

本次源范围：8–8行，符号定义体内；不是运行证据。

### DL33-A047：ENOMEM固定数值

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOMEM]
```c
	#define	ENOMEM		12	/* Out of memory */
```

本次源范围：18–18行，符号定义体内；不是运行证据。

### DL33-A048：kzalloc把__GFP_ZERO加入flags

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```

本次源范围：170–172行，符号定义体内；不是运行证据。

