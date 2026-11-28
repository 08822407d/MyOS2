---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-INIT-PATH-SWITCH-47
record_type: two_independent_categories_R46_errata_and_47_static_quotes
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
date: 2026-11-06
read_channel: connector
runtime_evidence: NOT_RUN
---

# R46-01定点文档勘误与47独立新源码引文

每段均来自固定time@a039d9803ade的单函数/真实结构/单宏定义体1–5行；九份源文件两次连接器全文读取并按行段、符号身份比对。重叠片段并非独立根因，函数内注释仅作为非活动语句上下文。两组各自编号/自查，旧46的59标签没有全面重核。

## Ⅰ. R46-01：新增加的纠正性短引文（原46材料保持冻结）

### R46-C001 — PREFIX_STATIC_INLINE（8–8行）

[VERIFIED mykernel/include/linux/compiler/myos_debug_option.h::PREFIX_STATIC_INLINE]
```c
	#	define PREFIX_STATIC_INLINE static inline
```

### R46-C002 — PREFIX_STATIC_INLINE（13–13行）

[VERIFIED mykernel/include/linux/compiler/myos_debug_option.h::PREFIX_STATIC_INLINE]
```c
	#	define PREFIX_STATIC_INLINE
```

### R46-C003 — get_fs_root（25–26行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_root]
```c
		void
		get_fs_root(taskfs_s *fs, path_s *root) {
```

### R46-C004 — get_fs_pwd（34–35行）

[VERIFIED mykernel/sched/misc/fs_struct.h::get_fs_pwd]
```c
		void
		get_fs_pwd(taskfs_s *fs, path_s *pwd) {
```

### R46-C005 — init_mount_tree（657–658行）

[VERIFIED mykernel/fs/vfs/namespace.c::init_mount_tree]
```c
		color_printk(RED, BLACK, "Can't create rootfs");
		while (1);
```

### R46-C006 — dentry_s（78–80行）

[VERIFIED mykernel/include/linux/fs/dcache.h::dentry_s]
```c
		/* Ref lookup also touches following */
		// lockref_s		d_lockref;			/* per-dentry lock and refcount */
		const dentry_ops_s	*d_op;
```

这六个短引文中R46-C001/C002分别只含一个PREFIX_STATIC_INLINE宏，R46-C003/C004含getter完整void返回类型片段，R46-C005含实际打印及while(1)，R46-C006属于真实dentry_s结构体内对d_lockref的注释位置。**不计为VERIFIED定义体的条件上下文**：myos_debug_option.h第6行`#ifndef DEBUG`、第11行`#else /* DEBUG */`；fs_struct.h第12行`#ifdef DEBUG`外部声明、第22行`#if defined(FS_STRUCT_DEFINATION) || !(DEBUG)`真正函数定义选择。C04默认分支代码搜索未找到getter调用者属**检索方法记录**，不能让namespace源码当成搜索事实的锚点。原59条/59复读为旧作者历史申报，R46-01现在仍OPEN。

## Ⅱ. 第47批新分析独立源码引文

### IP47-A001 — init_chdir（44–47行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
int init_chdir(const char *filename)
{
	struct path path;
	int error;
```

### IP47-A002 — init_chdir（49–51行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
```

### IP47-A003 — init_chdir（52–56行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	// error = path_permission(&path, MAY_EXEC | MAY_CHDIR);
	if (!error)
		set_fs_pwd(current->fs, &path);
	path_put(&path);
	return error;
```

### IP47-A004 — init_chdir（53–56行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	if (!error)
		set_fs_pwd(current->fs, &path);
	path_put(&path);
	return error;
```

### IP47-A005 — init_chdir（50–51行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	if (error)
		return error;
```

### IP47-A006 — init_chroot（59–62行）

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
int init_chroot(const char *filename)
{
	struct path path;
	int error;
```

### IP47-A007 — init_chroot（64–66行）

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (error)
		return error;
```

### IP47-A008 — init_chroot（75–79行）

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	// 	goto dput_and_out;
	set_fs_root(current->fs, &path);
dput_and_out:
	path_put(&path);
	return error;
```

### IP47-A009 — init_chroot（76–79行）

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	set_fs_root(current->fs, &path);
dput_and_out:
	path_put(&path);
	return error;
```

### IP47-A010 — init_chdir（49–49行）

[VERIFIED mykernel/fs/vfs/init.c::init_chdir]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
```

### IP47-A011 — init_chroot（64–64行）

[VERIFIED mykernel/fs/vfs/init.c::init_chroot]
```c
	error = kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
```

### IP47-A012 — init_mount（16–20行）

[VERIFIED mykernel/fs/vfs/init.c::init_mount]
```c
int init_mount(const char *dev_name, const char *dir_name,
				const char *type_page, unsigned long flags)
{
	struct path path;
	int ret;
```

### IP47-A013 — init_mount（22–26行）

[VERIFIED mykernel/fs/vfs/init.c::init_mount]
```c
	ret = kern_path(dir_name, LOOKUP_FOLLOW, &path);
	if (ret)
		return ret;
	ret = path_mount(dev_name, &path, type_page, flags);
	path_put(&path);
```

### IP47-A014 — init_mount（25–27行）

[VERIFIED mykernel/fs/vfs/init.c::init_mount]
```c
	ret = path_mount(dev_name, &path, type_page, flags);
	path_put(&path);
	return ret;
```

### IP47-A015 — kern_path（650–654行）

[VERIFIED mykernel/fs/vfs/namei.c::kern_path]
```c
int kern_path(const char *name, unsigned int flags, OUT path_s *path)
{
	return filename_lookup(AT_FDCWD, getname_kernel(name),
					flags, path, NULL);
}
```

### IP47-A016 — kern_path（652–653行）

[VERIFIED mykernel/fs/vfs/namei.c::kern_path]
```c
	return filename_lookup(AT_FDCWD, getname_kernel(name),
					flags, path, NULL);
```

### IP47-A017 — filename_lookup（597–601行）

[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
int filename_lookup(int dfd, IN filename_s *name, unsigned flags,
				OUT path_s *path, IN path_s *root)
{
	int retval;
	nameidata_s nd;
```

### IP47-A018 — filename_lookup（602–606行）

[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	if (IS_ERR(name))
		return PTR_ERR(name);

	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
```

### IP47-A019 — filename_lookup（610–611行）

[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	putname(name);
	return retval;
```

### IP47-A020 — filename_lookup（602–603行）

[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	if (IS_ERR(name))
		return PTR_ERR(name);
```

### IP47-A021 — filename_lookup（605–606行）

[VERIFIED mykernel/fs/vfs/namei.c::filename_lookup]
```c
	set_nameidata(&nd, dfd, name, NULL);
	retval = path_lookupat(&nd, flags, path);
```

### IP47-A022 — path_lookupat（572–575行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
static int path_lookupat(IN nameidata_s *nd, unsigned flags, OUT path_s *path)
{
	const char *s = path_init(nd, flags);
	int err;
```

### IP47-A023 — path_lookupat（577–579行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	while (!(err = link_path_walk(s, nd)) &&
		   (s = lookup_last(nd)) != NULL)
		;
```

### IP47-A024 — path_lookupat（581–585行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
		err = complete_walk(nd);

	if (err == 0)
	{
```

### IP47-A025 — path_lookupat（584–588行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	if (err == 0)
	{
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
```

### IP47-A026 — path_lookupat（586–589行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
		*path = nd->path;
		nd->path.mnt = NULL;
		nd->path.dentry = NULL;
	}
```

### IP47-A027 — path_lookupat（590–591行）

[VERIFIED mykernel/fs/vfs/namei.c::path_lookupat]
```c
	terminate_walk(nd);
	return err;
```

### IP47-A028 — getname_kernel（104–108行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
filename_s *getname_kernel(const char *k_filename)
{
	filename_s *name = __myos_getname();

	size_t len = strlen((void *)k_filename);
```

### IP47-A029 — getname_kernel（108–111行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;
```

### IP47-A030 — getname_kernel（113–117行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	name->name = kzalloc(len + 1, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
```

### IP47-A031 — getname_kernel（121–125行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	if (dest != name->name)
	{
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
```

### IP47-A032 — getname_kernel（123–127行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
	}

```

### IP47-A033 — getname_kernel（128–129行）

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	return name;
}
```

### IP47-A034 — putname（133–137行）

[VERIFIED mykernel/fs/vfs/namei.c::putname]
```c
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}
```

### IP47-A035 — set_nameidata（142–143行）

[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
static void set_nameidata(OUT nameidata_s *p, int dfd,
		IN filename_s *name, IN path_s *root)
```

### IP47-A036 — set_nameidata（149–152行）

[VERIFIED mykernel/fs/vfs/namei.c::set_nameidata]
```c
		memset(p, 0, sizeof(nameidata_s));

		p->name = name;
		p->dfd = dfd;
```

### IP47-A037 — path_init（547–551行）

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
static const char *path_init(OUT nameidata_s *nd, unsigned flags)
{
	const char *s = nd->name->name;

	nd->flags = flags;
```

### IP47-A038 — path_init（553–557行）

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
	nd->root = current->fs->root;
	if (s[0] == '/')
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;
```

### IP47-A039 — path_init（555–559行）

[VERIFIED mykernel/fs/vfs/namei.c::path_init]
```c
		nd->path = nd->root;
	else
		nd->path = current->fs->pwd;

	return s;
```

### IP47-A040 — devtmpfsd（384–388行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
static int devtmpfsd(void *p)
{
	// int err = devtmpfs_setup(&p);
	// {
		int err = 0;
```

### IP47-A041 — devtmpfsd（388–392行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		int err = 0;

		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
```

### IP47-A042 — devtmpfsd（390–392行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
```

### IP47-A043 — devtmpfsd（393–394行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
```

### IP47-A044 — devtmpfsd（393–397行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
	// out:
	// 	return err;
	// }
```

### IP47-A045 — devtmpfsd（399–403行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
	complete(&setup_done);
	devtmpfs_work_loop();
out:
	return err;
}
```

### IP47-A046 — devtmpfsd（401–403行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
out:
	return err;
}
```

### IP47-A047 — LOOKUP_FOLLOW（39–39行）

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW]
```c
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
```

### IP47-A048 — LOOKUP_DIRECTORY（40–40行）

[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_DIRECTORY]
```c
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
```

### IP47-A049 — AT_FDCWD（95–97行）

[VERIFIED mykernel/include/uapi/linux/fcntl.h::AT_FDCWD]
```c
	#define AT_FDCWD				-100    /* Special value used to indicate
											   openat should use the current
											   working directory. */
```

## Ⅲ. 定义体支持强度、历史和未做事项

- init.c第3–5行说明这些是早期内核初始化内部接口，属于函数外语境注释，不计直接函数执行。init_chdir/init_chroot完整活动体分别为44–57和59–80。注释权限检查不是实际生效的权限策略；chdir第二个!error门此前没有新活动赋值，chroot的dput_and_out只有顺序贯穿，无活动goto转移。
- kern_path完整体只将AT_FDCWD和getname_kernel结果交filename_lookup并返回它的int。filename_lookup只对IS_ERR(name)做编码错误门，在正常输入下调用path_lookupat，然后putname并返回整数retval。path_lookupat仅在err==0分支写path输出和清nd.path本地指针，未证明输出被指向对象有效或引用真实取得。
- getname_kernel属于历史PN29相关分配/字符串责任区，本批仅定点读取其入口，不重新宣布新的字符串根因。参数无效、上游callee不正常返回时不可虚构后续errno或输出。
- devtmpfsd只有init_mount调用结果被写入err并检查；两次init_chdir/init_chroot的int返回不保存、不检查，亦不写本地err。complete和devtmpfs_work_loop仅作后继实际调用边界，不审它们的阻塞、唤醒或工作线程状态。
- FOLLOW/DIRECTORY位分别为0x0001/0x0002，传递并不证明完整目录检查、符号链接或权限语义；AT_FDCWD=-100只证该单宏，不重演31/32路径全链。path_get/path_put到dget/dput的直接活动体由45/46及其主线回执限定为无实际引用数增减，此处任何path_put调用都不能升格为完成物理释放。
- 直接源引文不代替完整P9、编译器或运行认证；DEBUG宏组合的具体预处理/链接绑定及原46其余标签整体自查不在此重新认证。

## 本轮实际核验

R46定点纠正性引文6段：verified_claims=6、quotes_reconfirmed=6、downgraded_to_inferred=0；新47独立引文49段：verified_claims=49、quotes_reconfirmed=49、downgraded_to_inferred=0。历史原46“59/59”保留作为历史申报（未全数重核）。NOT_RUN：完整P9、原始byte下载、预处理/编译、仓库C/ASM/脚本、Graphviz、QEMU/实机、挂载/路径解析、thread/completion、额外付费API或模型。
