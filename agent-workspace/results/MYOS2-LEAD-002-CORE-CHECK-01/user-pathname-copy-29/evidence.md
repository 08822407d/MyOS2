---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-PATHNAME-COPY-29
record_type: fixed_time_verbatim_source_evidence
executor: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-03
source_pin_short12: a039d9803ade
source_branch: time
read_channel: GitHub_connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第29批固定源码直接证据

以下每段是本轮从`time@a039d9803ade`所选文件及函数取得的**逐字C源码片段**。每段1–5行（空行计入）；`path::symbol`和邻接行号明确归属。代码片段只认证该语句存在，不自动证明调用成功、实际复制字节、物理页面或总回收。部分标题包含对该函数多处语句的摘要时，须与相邻锚点及完整定义合读。

### PN29-A001 · filename本体kmalloc并在NULL时产生ERR_PTR(-ENOMEM)
[VERIFIED mykernel/fs/vfs/namei.c::__myos_getname]
```c
static inline filename_s *__myos_getname()
{
	filename_s *name = kmalloc(sizeof(filename_s), GFP_KERNEL);
	if (name == NULL)
		return ERR_PTR(-ENOMEM);
```
固定源58–62行。

### PN29-A002 · 本体成功时返回普通指针
[VERIFIED mykernel/fs/vfs/namei.c::__myos_getname]
```c
	else
		return name;
}
```
固定源63–65行。

### PN29-A003 · 用户路径先strlen_user再以size_t比较PAGE_SIZE
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
filename_s *getname(const char *u_filename)
{
	size_t len = strlen_user((void *)u_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
```
固定源69–73行。

### PN29-A004 · 用户路径的第二阶段先取得filename对象并写name->len
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
	// {
		filename_s *name = __myos_getname();
	// }

	name->len = len;
```
固定源79–83行。

### PN29-A005 · 用户路径字符区kzalloc(len+16)与失败分支起点
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
	name->len = len;
	name->name = kzalloc(len + 16, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
```
固定源83–87行。
### PN29-A006 · 用户复制long返回赋给size_t与恒假负值门
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
	len = strncpy_from_user((void *)name->name, (void *)u_filename, len);
	if (len < 0)
	{
		kfree((void *)name->name);
		kfree(name);
```
固定源91–95行。

### PN29-A007 · 用户复制错误分支尝试释放对象及字符串并返回错误指针
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
	}

```
固定源94–98行。

### PN29-A008 · 用户路径正常返回filename对象
[VERIFIED mykernel/fs/vfs/namei.c::getname]
```c
	return name;
}
```
固定源99–100行。

### PN29-A009 · 内核入口先申请filename对象再strlen
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
filename_s *getname_kernel(const char *k_filename)
{
	filename_s *name = __myos_getname();

	size_t len = strlen((void *)k_filename);
```
固定源104–108行。
### PN29-A010 · 内核字符串超长时直接ERR_PTR而未kfree本体
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;
```
固定源108–111行。

### PN29-A011 · 内核字符区kzalloc(len+1)和失败时kfree本体
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	name->name = kzalloc(len + 1, GFP_KERNEL);
	if (name->name == NULL)
	{
		kfree(name);
		return ERR_PTR(-ENOMEM);
```
固定源113–117行。
### PN29-A012 · 内核字符串strncpy返回目标地址及不等式分支
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	char * dest = strncpy((void *)name->name, (void *)k_filename, len);
	if (dest != name->name)
	{
		kfree((void *)name->name);
		kfree(name);
```
固定源120–124行。

### PN29-A013 · 内核不等目标分支释放及正常返回
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
		kfree((void *)name->name);
		kfree(name);
		return ERR_PTR(len);
	}

```
固定源123–127行。

### PN29-A014 · putname分别kfree字符区与filename对象
[VERIFIED mykernel/fs/vfs/namei.c::putname]
```c
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
}
```
固定源133–137行。

### PN29-A015 · 选定用户复制帮助函数先作access_ok范围门
[VERIFIED mykernel/arch/x86_64/myos/utils.c::strncpy_from_user]
```c
inline __always_inline long
strncpy_from_user(void *to, void *from, ulong size) {
	if (!access_ok(from, size))
		return -EFAULT;
```
固定源8–11行。

### PN29-A016 · 用户复制选择通用strncpy再返回传入size
[VERIFIED mykernel/arch/x86_64/myos/utils.c::strncpy_from_user]
```c

	strncpy(to, from, size);
	return	size;
}
```
固定源12–15行。

### PN29-A017 · 用户测长先调用strlen读取后再access_ok
[VERIFIED mykernel/arch/x86_64/myos/utils.c::strlen_user]
```c
inline __always_inline long
strlen_user(void *src) {
	ulong size = strlen(src);
	if (!access_ok(src, size))
		return -EFAULT;
```
固定源17–21行。

### PN29-A018 · 用户测长范围门不通过返回-EFAULT而成功返回size
[VERIFIED mykernel/arch/x86_64/myos/utils.c::strlen_user]
```c
	if (!access_ok(src, size))
		return -EFAULT;

	return size;
}
```
固定源20–24行。

### PN29-A019 · 通用strncpy受__HAVE_ARCH_STRNCPY条件编译约束
[VERIFIED mykernel/lib/string/string.c::strncpy]
```c
#ifndef __HAVE_ARCH_STRNCPY
	/**
	 * strncpy - Copy a length-limited, C-string
	 * @dest: Where to copy the string to
	 * @src: Where to copy the string from
```
固定源93–97行。
### PN29-A020 · strncpy返回目标指针的局部副本并进入复制循环
[VERIFIED mykernel/lib/string/string.c::strncpy]
```c
	char *strncpy(char *dest, const char *src, size_t count)
	{
		char *tmp = dest;
		while (count) {
			if ((*tmp = *src) != 0)
```
固定源107–111行。

### PN29-A021 · strncpy最多复制count个字节的循环主体
[VERIFIED mykernel/lib/string/string.c::strncpy]
```c
		while (count) {
			if ((*tmp = *src) != 0)
				src++;
			tmp++;
			count--;
```
固定源110–114行。

### PN29-A022 · strncpy正常完成后无条件返回原dest
[VERIFIED mykernel/lib/string/string.c::strncpy]
```c
			tmp++;
			count--;
		}
		return dest;
	}
```
固定源113–117行。

### PN29-A023 · 通用strlen函数签名及主体入口
[VERIFIED mykernel/lib/string/string.c::strlen]
```c
	 */
	size_t strlen(const char *s)
	{
		const char *sc;
```
固定源477–480行。
### PN29-A024 · strlen扫描到首个NUL并返回字符差值
[VERIFIED mykernel/lib/string/string.c::strlen]
```c
	{
		const char *sc;
		for (sc = s; *sc != '\0'; ++sc)
			/* nothing */;
		return sc - s;
```
固定源479–483行。

### PN29-A025 · filename对象分离name和uptr字符指针与int长度
[VERIFIED mykernel/include/linux/fs/fs.h::filename]
```c
	typedef struct filename {
		const char	*name;	/* pointer to actual string */
		const char	*uptr;	/* original userland pointer */
		int			len;
		// int			refcnt;
```
固定源2517–2521行。

### PN29-A026 · filename的len是int且refcnt成员为注释
[VERIFIED mykernel/include/linux/fs/fs.h::filename]
```c
		int			len;
		// int			refcnt;
		// audit_names_s	*aname;
		// const char	iname[];
	} filename_s;
```
固定源2520–2524行。

### PN29-A027 · 选定x86-64小长度地址检查分支
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
	static inline bool
    __access_ok(const void __user *ptr, ulong size) {
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
```
固定源83–86行。

### PN29-A028 · 选定x86-64范围和值溢出检查
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
		} else {
			ulong sum = size + (__force ulong)ptr;
			return valid_user_address(sum) && sum >= (__force ulong)ptr;
		}
	}
```
固定源87–91行。

### PN29-A029 · 选定x86-64用户地址高位判据
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::valid_user_address]
```c
	#define valid_user_address(x) ((__force long)(x) >= 0)
```
固定源55–55行。

### PN29-A030 · asm/uaccess同时含arch和generic access_ok头
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::uaccess header]
```c
	#include <asm/uaccess_64.h>

	#include <uapi/asm-generic/access_ok.h>
```
固定源14–16行。

### PN29-A031 · access_ok宏选用__access_ok结果而非物理页检查
[VERIFIED mykernel/include/uapi/asm-generic/access_ok.h::access_ok]
```c

	#ifndef access_ok
	#  define access_ok(addr, size) likely(__access_ok(addr, size))
	#endif
```
固定源39–42行。

### PN29-A032 · kzalloc向kmalloc传入__GFP_ZERO标志
[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
		PREFIX_STATIC_INLINE
		__alloc_size(1) void
		*kzalloc(size_t size, gfp_t flags) {
			return kmalloc(size, flags | __GFP_ZERO);
		}
```
固定源168–172行。

### PN29-A033 · 分配后__GFP_ZERO的实际memset门
[VERIFIED mykernel/mm/kmalloc/slub.h::slab_post_alloc_hook]
```c
			if (flags & __GFP_ZERO)
				memset(*obj_p, 0, zero_size);
```
固定源246–247行。

### PN29-A034 · ERR_PTR由long错误值生成编码指针
[VERIFIED mykernel/include/linux/kernel/err.h::ERR_PTR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}
```
固定源26–29行。

### PN29-A035 · PTR_ERR从编码指针转换long
[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```
固定源31–34行。

### PN29-A036 · IS_ERR检查错误编码区
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```
固定源36–39行。

### PN29-A037 · MAX_ERRNO与编码错误范围
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#define MAX_ERRNO	4095

	#ifndef __ASSEMBLY__
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
```
固定源15–19行。

### PN29-A038 · filp_open取得内核filename并检验错误指针
[VERIFIED mykernel/fs/vfs/open.c::filp_open]
```c
	filename_s *name = getname_kernel(filename);
	file_s *file = ERR_CAST(name);
	
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
```
固定源220–224行。
### PN29-A039 · filp_open使用file_open_name后putname
[VERIFIED mykernel/fs/vfs/open.c::filp_open]
```c
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
		putname(name);
	}
	return file;
```
固定源223–227行。

### PN29-A040 · do_sys_openat2调用getname并立即判断ERR_PTR
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	filename_s *tmp;

	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);
```
固定源245–249行。
### PN29-A041 · open先取fd后把filename借给do_filp_open
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
```
固定源251–255行。

### PN29-A042 · do_filp_open返回file或错误并处理fd
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
			fd = PTR_ERR(f);
		else
			fd_install(fd, f);
```
固定源254–258行。

### PN29-A043 · open正常/分配fd失败后putname再return
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
			fd_install(fd, f);
	}
	putname(tmp);
	return fd;
}
```
固定源258–262行。

### PN29-A044 · do_sys_open以构造how接到do_sys_openat2
[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```
固定源266–270行。

### PN29-A045 · sys_open实际到do_sys_open且传用户filename
[VERIFIED mykernel/fs/syscall.c::sys_open]
```c
// #define __NR_open			2
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
```
固定源37–41行。

### PN29-A046 · sys_execve调用getname再传do_execve
[VERIFIED mykernel/sched/forkexec/exec.c::sys_execve]
```c
MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
}
```
固定源1460–1464行。

### PN29-A047 · do_execve把已有filename传给do_execveat_common
[VERIFIED mykernel/sched/forkexec/exec.c::do_execve]
```c
do_execve(filename_s *filename,
		const char *const *__argv,
		const char *const *__envp) {
	return do_execveat_common(AT_FDCWD,
			filename, __argv, __envp, 0);
```
固定源1438–1442行。
### PN29-A048 · execve共同入口先检测filename的IS_ERR和PTR_ERR
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	linux_bprm_s *bprm;
	int retval;

	if (IS_ERR(filename))
		return PTR_ERR(filename);
```
固定源1294–1298行。
### PN29-A049 · execve从alloc_bprm失败分支转换错误并跳out_ret
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```
固定源1316–1320行。
### PN29-A050 · exec共同出口先free_bprm后putname(filename)
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
```
固定源1367–1371行。
### PN29-A051 · alloc_bprm把filename借给do_open_execat
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	int retval = -ENOMEM;

	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);
```
固定源1103–1107行。

### PN29-A052 · bprm->filename可借用filename->name指针
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm->file = file;

	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```
固定源1115–1119行。

### PN29-A053 · 不同exec调用方式可创建fdpath字符串
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		if (filename->name[0] == '\0')
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d", fd);
		else
			bprm->fdpath = kasprintf(GFP_KERNEL, "/dev/fd/%d/%s",
						  fd, filename->name);
```
固定源1120–1124行。

### PN29-A054 · bprm->interp借用相同字符串并初始化mm
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
		bprm->filename = bprm->fdpath;
	}
	bprm->interp = bprm->filename;

	retval = bprm_mm_init(bprm);
```
固定源1140–1144行。

### PN29-A055 · free_bprm首步是MM条件释放，不包含putname
[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
free_bprm(linux_bprm_s *bprm) {
	if (bprm->mm) {
		// acct_arg_size(bprm, 0);
		mmput(bprm->mm);
	}
```
固定源1073–1077行。
### PN29-A056 · free_bprm只按不同所有权释放interp/fdpath与bprm
[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	/* If a binfmt changed the interp, free it. */
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
```
固定源1090–1094行。
### PN29-A057 · open_exec以getname_kernel取得内核路径名
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
file_s *open_exec(const char *name)
{
	filename_s *filename = getname_kernel(name);
	file_s *f = ERR_CAST(filename);

```
固定源604–608行。

### PN29-A058 · open_exec对正常filename打开后putname
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
```
固定源609–613行。

### PN29-A059 · do_open_execat实际O_RDONLY执行标志及MAY_EXEC
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	open_flags_s open_exec_flags = {
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
```
固定源558–562行。
### PN29-A060 · do_open_execat执行do_filp_open并检查IS_ERR
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```
固定源572–574行。
### PN29-A061 · kernel_execve对getname_kernel指针先IS_ERR
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);

	bprm = alloc_bprm(fd, filename, 0);
```
固定源1389–1393行。

### PN29-A062 · kernel_execve的alloc_bprm失败分支
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```
固定源1393–1397行。

### PN29-A063 · kernel_execve尾部free_bprm然后putname(filename)
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
```
固定源1430–1434行。
### PN29-A064 · do_filp_open将借来的filename传给nameidata与path_openat
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
```
固定源759–763行。
### PN29-A065 · kernel_execve调用bprm_execve并进入free_bprm出口
[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);

out_ret:
```
固定源1428–1432行。
### PN29-A066 · do_filp_open将filename借用给nameidata与path_openat
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	file_s *filp;

	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
```
固定源757–760行。
### PN29-A067 · 底层__kernel_size_t取unsigned long来源
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_size_t]
```c
		// 	typedef int				__kernel_ptrdiff_t;
		// #else
			typedef __kernel_ulong_t	__kernel_size_t;
			typedef __kernel_long_t		__kernel_ssize_t;
			typedef __kernel_long_t		__kernel_ptrdiff_t;
```
固定源71–75行。
### PN29-A068 · 选定架构帮助函数签名声明均为long
[VERIFIED mykernel/arch/x86_64/include/obsolete/arch_proto.h::strlen_user]
```c
	/* utils.c */
	long strncpy_from_user(void * to, void * from, unsigned long size);
	long strlen_user(void * src);
```
固定源98–100行。

## 来源与检查范围

本轮通过GitHub连接器从固定time完整获取并二次核验上述14份直接证据来源；另外核原51号恢复、49任务、28主线回执、旧EI28-F01/F05及05::UV-04的约定、金丝雀与PR状态。默认分支代码搜索只用于定位符号，所有上文引文取自固定time。图只代表路径取得和有限caller/return，非完整VFS/权限/页表/异常图。

所选`strlen_user`和`strncpy_from_user`定义在`arch/x86_64/myos/utils.c`，不同于仓库中注释的Linux通用`strnlen_user`契约。通用`strncpy/strlen`函数体在`!__HAVE_ARCH_STRNCPY/!__HAVE_ARCH_STRLEN`前提下适用；当前构建的所有预处理选项未实际展开，不能对架构别名完全不同配置无条件外推。access_ok按所选x86-64路径包含地址范围数值检查，无物理访问证明。

其他未执行：源码C/ASM、预处理/编译、仓库脚本、QEMU、真实用户指针、硬件、外部libc/镜像、全面文件系统权限/路径遍历/重定位和全仓问题去重。所有纸面结果均标`NOT_RUN`。
