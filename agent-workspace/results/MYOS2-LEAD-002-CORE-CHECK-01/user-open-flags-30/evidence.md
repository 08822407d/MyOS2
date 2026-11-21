---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-OPEN-FLAGS-30
record_type: source_exact_quote_register
executor: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-03
source_ref: time
source_pin_short12: a039d9803ade
read_channel: connector
phase: 2
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 30号固定源码证据登记

本轮所有引文从MyOS2 `time@a039d9803ade`连接器直接读取；每个锚点含**1–5行**源代码，空行也算，保留原始缩进、宏尾反斜线与字节。标题供索引，不是引文本身；实际C语义依赖所选定义、调用者、返回门及有效对象前提。没有因同名Linux接口自动附加实现保证。

### OF30-A001 · WILL_CREATE与O_PATH_FLAGS两个宏
[VERIFIED mykernel/fs/vfs/open.c::WILL_CREATE]
```c
#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
#define O_PATH_FLAGS		(O_DIRECTORY | O_NOFOLLOW | O_PATH | O_CLOEXEC)
```
固定源行23–24。

### OF30-A002 · build_open_how返回open_how_s并初始化flags和mode
[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
open_how_s build_open_how(int flags, umode_t mode)
{
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
```
固定源行27–31。

### OF30-A003 · 复合字面量仅显式给flags/mode
[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	struct open_how how = {
		.flags = flags & VALID_OPEN_FLAGS,
		.mode = mode & S_IALLUGO,
	};
```
固定源行29–32。

### OF30-A004 · O_PATH先筛除不在O_PATH_FLAGS中的位
[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	/* O_PATH beats everything else. */
	if (how.flags & O_PATH)
		how.flags &= O_PATH_FLAGS;
```
固定源行34–36。

### OF30-A005 · 非create标志下mode归零并返回对象
[VERIFIED mykernel/fs/vfs/open.c::build_open_how]
```c
	/* Modes should only be set for create-like flags. */
	if (!WILL_CREATE(how.flags))
		how.mode = 0;
	return how;
```
固定源行37–40。

### OF30-A006 · build_open_flags开始定义flags、strip、lookup_flags及ACC_MODE初值
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
{
	uint64_t flags = how->flags;
	uint64_t strip = FMODE_NONOTIFY | O_CLOEXEC;
	int lookup_flags = 0;
	int acc_mode = ACC_MODE(flags);
```
固定源行68–72。
### OF30-A007 · 活动flags &= ~strip实际执行
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	flags &= ~strip;
```
固定源行79–79。
### OF30-A008 · 未知open标志和resolve位各自返回EINVAL
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & ~VALID_OPEN_FLAGS)
		return -EINVAL;
	if (how->resolve & ~VALID_RESOLVE_FLAGS)
		return -EINVAL;
```
固定源行86–89。

### OF30-A009 · BENEATH与IN_ROOT互斥直接返回EINVAL
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	/* Scoping flags are mutually exclusive. */
	if ((how->resolve & RESOLVE_BENEATH) && (how->resolve & RESOLVE_IN_ROOT))
		return -EINVAL;
```
固定源行91–93。

### OF30-A010 · create-like mode位检查并赋op.mode
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	/* Deal with the mode. */
	if (WILL_CREATE(flags)) {
		if (how->mode & ~S_IALLUGO)
			return -EINVAL;
		op->mode = how->mode | S_IFREG;
```
固定源行95–99。

### OF30-A011 · 非create时mode必须0和赋op.mode=0
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
		op->mode = how->mode | S_IFREG;
	} else {
		if (how->mode != 0)
			return -EINVAL;
		op->mode = 0;
```
固定源行99–103。

### OF30-A012 · TMPFILE组合完整性门与MAY_WRITE门
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & __O_TMPFILE) {
		if ((flags & O_TMPFILE_MASK) != O_TMPFILE)
			return -EINVAL;
		if (!(acc_mode & MAY_WRITE))
			return -EINVAL;
```
固定源行112–116。

### OF30-A013 · O_PATH允许位掩码和acc_mode=0
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_PATH) {
		/* O_PATH only permits certain other flags to be set. */
		if (flags & ~O_PATH_FLAGS)
			return -EINVAL;
		acc_mode = 0;
```
固定源行118–122。

### OF30-A014 · __O_SYNC设置O_DSYNC并写op.open_flag
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & __O_SYNC)
		flags |= O_DSYNC;

	op->open_flag = flags;
```
固定源行131–134。

### OF30-A015 · O_TRUNC后置增加写访问需求
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	/* O_TRUNC implies we need access checks for write permissions */
	if (flags & O_TRUNC)
		acc_mode |= MAY_WRITE;
```
固定源行136–138。

### OF30-A016 · O_APPEND后置增加MAY_APPEND
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	/* Allow the LSM permission hook to distinguish append
	   access from general write access. */
	if (flags & O_APPEND)
		acc_mode |= MAY_APPEND;
```
固定源行140–143。

### OF30-A017 · 赋op.acc_mode与op.intent
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	op->acc_mode = acc_mode;

	op->intent = flags & O_PATH ? 0 : LOOKUP_OPEN;
```
固定源行145–147。

### OF30-A018 · O_CREAT/O_EXCL设置op.intent并修改局部flags
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_CREAT) {
		op->intent |= LOOKUP_CREATE;
		if (flags & O_EXCL) {
			op->intent |= LOOKUP_EXCL;
			flags |= O_NOFOLLOW;
```
固定源行149–153。

### OF30-A019 · O_EXCL仅改后面的局部flags中的O_NOFOLLOW
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
		if (flags & O_EXCL) {
			op->intent |= LOOKUP_EXCL;
			flags |= O_NOFOLLOW;
		}
	}
```
固定源行151–155。

### OF30-A020 · O_DIRECTORY与O_NOFOLLOW映射lookup标志
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (flags & O_DIRECTORY)
		lookup_flags |= LOOKUP_DIRECTORY;
	if (!(flags & O_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
```
固定源行157–160。

### OF30-A021 · RESOLVE_NO_XDEV和MAGICLINKS映射
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_NO_XDEV)
		lookup_flags |= LOOKUP_NO_XDEV;
	if (how->resolve & RESOLVE_NO_MAGICLINKS)
		lookup_flags |= LOOKUP_NO_MAGICLINKS;
```
固定源行162–165。

### OF30-A022 · RESOLVE_NO_SYMLINKS和BENEATH映射
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_NO_SYMLINKS)
		lookup_flags |= LOOKUP_NO_SYMLINKS;
	if (how->resolve & RESOLVE_BENEATH)
		lookup_flags |= LOOKUP_BENEATH;
```
固定源行166–169。

### OF30-A023 · RESOLVE_IN_ROOT映射
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_IN_ROOT)
		lookup_flags |= LOOKUP_IN_ROOT;
```
固定源行170–171。

### OF30-A024 · RESOLVE_CACHED拒绝的实际复合O_TMPFILE位运算
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
	if (how->resolve & RESOLVE_CACHED) {
		/* Don't bother even trying for create/truncate/tmpfile open */
		if (flags & (O_TRUNC | O_CREAT | O_TMPFILE))
			return -EAGAIN;
		lookup_flags |= LOOKUP_CACHED;
```
固定源行172–176。

### OF30-A025 · 成功CACHED映射及最终op.lookup_flags/return0
[VERIFIED mykernel/fs/vfs/open.c::build_open_flags]
```c
		lookup_flags |= LOOKUP_CACHED;
	}

	op->lookup_flags = lookup_flags;
	return 0;
```
固定源行176–180。

### OF30-A026 · 内核file_open_name先构造how后取得校验返回
[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
file_s *file_open_name(filename_s *name, int flags, umode_t mode)
{
	open_flags_s op;
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
```
固定源行197–201。

### OF30-A027 · file_open_name检查err后才调用do_filp_open
[VERIFIED mykernel/fs/vfs/open.c::file_open_name]
```c
	open_how_s how = build_open_how(flags, mode);
	int err = build_open_flags(&how, &op);
	if (err)
		return ERR_PTR(err);
	return do_filp_open(AT_FDCWD, name, &op);
```
固定源行200–204。

### OF30-A028 · filp_open取得getname_kernel后对有效filename调用file_open_name
[VERIFIED mykernel/fs/vfs/open.c::filp_open]
```c
	filename_s *name = getname_kernel(filename);
	file_s *file = ERR_CAST(name);
	
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
```
固定源行220–224。
### OF30-A029 · filp_open在file_open_name返回后putname
[VERIFIED mykernel/fs/vfs/open.c::filp_open]
```c
	if (!IS_ERR(name)) {
		file = file_open_name(name, flags, mode);
		putname(name);
	}
	return file;
```
固定源行223–227。

### OF30-A030 · 局部open_flags及fd先接build_open_flags返回
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
static long do_sys_openat2(int dfd, const char *filename, open_how_s *how)
{
	open_flags_s op;
	int fd = build_open_flags(how, &op);
	filename_s *tmp;
```
固定源行241–245。

### OF30-A031 · getname错误可先阻断后续fd覆盖
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	tmp = getname(filename);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);
```
固定源行247–249。

### OF30-A032 · get_unused_fd_flags覆盖fd并在非负后调用do_filp_open
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
	fd = get_unused_fd_flags(how->flags);
	if (fd >=0)
	{
		file_s * f = do_filp_open(dfd, tmp, &op);
```
固定源行251–254。

### OF30-A033 · 下层返回及fd_install的条件路径
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
		file_s * f = do_filp_open(dfd, tmp, &op);
		if (IS_ERR(f))
			fd = PTR_ERR(f);
		else
			fd_install(fd, f);
```
固定源行254–258。

### OF30-A034 · 正常尾部putname并返回覆盖后的fd
[VERIFIED mykernel/fs/vfs/open.c::do_sys_openat2]
```c
			fd_install(fd, f);
	}
	putname(tmp);
	return fd;
}
```
固定源行258–262。

### OF30-A035 · 旧open先build_open_how再调用do_sys_openat2
[VERIFIED mykernel/fs/vfs/open.c::do_sys_open]
```c
long do_sys_open(int dfd, const char * filename, int flags, umode_t mode)
{
	open_how_s how = build_open_how(flags, mode);
	return do_sys_openat2(dfd, filename, &how);
}
```
固定源行266–270。

### OF30-A036 · 当前sys_open入口只转do_sys_open
[VERIFIED mykernel/fs/syscall.c::sys_open]
```c
// #define __NR_open			2
MYOS_SYSCALL_DEFINE3(open, const char *, filename,
		int, flags, umode_t, mode)
{
	return do_sys_open(0, filename, flags, mode);
```
固定源行37–41。

### OF30-A037 · open_flags_s五个字段与类型
[VERIFIED mykernel/include/linux/fs/internal.h::open_flags_s]
```c
typedef struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
```
固定源行91–95。

### OF30-A038 · 两帮助函数及do_filp_open直接声明
[VERIFIED mykernel/include/linux/fs/internal.h::build_open_how]
```c
extern file_s *
do_filp_open(int dfd, filename_s *pathname, const open_flags_s *op);
extern file_s *
do_file_open_root(const path_s, const char *, const open_flags_s *);
extern struct open_how build_open_how(int flags, umode_t mode);
```
固定源行98–102。

### OF30-A039 · 有效open标志定义前半
[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS]
```c
	/* List of all valid flags for the open/openat flags argument: */
	#define VALID_OPEN_FLAGS (											\
				O_RDONLY | O_WRONLY | O_RDWR | O_CREAT |				\
				O_EXCL | O_NOCTTY | O_TRUNC |							\
				O_APPEND | O_NDELAY | O_NONBLOCK | __O_SYNC | O_DSYNC | \
```
固定源行8–12。

### OF30-A040 · 有效open标志含O_PATH和__O_TMPFILE
[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_OPEN_FLAGS]
```c
				O_APPEND | O_NDELAY | O_NONBLOCK | __O_SYNC | O_DSYNC | \
				FASYNC	| O_DIRECT | O_LARGEFILE |						\
				O_DIRECTORY | O_NOFOLLOW |								\
				O_NOATIME | O_CLOEXEC | O_PATH | __O_TMPFILE			\
			)
```
固定源行12–16。

### OF30-A041 · 合法RESOLVE位集合
[VERIFIED mykernel/include/linux/kernel/fcntl.h::VALID_RESOLVE_FLAGS]
```c
	/* List of all valid flags for the how->resolve argument: */
	#define VALID_RESOLVE_FLAGS (							\
				RESOLVE_NO_XDEV | RESOLVE_NO_MAGICLINKS |	\
				RESOLVE_NO_SYMLINKS | RESOLVE_BENEATH |		\
				RESOLVE_IN_ROOT | RESOLVE_CACHED			\
```
固定源行18–22。

### OF30-A042 · O_RDONLY/O_WRONLY/O_RDWR及ACCMODE位
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
	#define O_RDONLY			00000000
	#define O_WRONLY			00000001
	#define O_RDWR				00000002
```
固定源行19–22。

### OF30-A043 · O_CREAT和O_EXCL八进制数值
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CREAT]
```c
	#ifndef O_CREAT
	#	define O_CREAT			00000100	/* not fcntl */
	#endif
	#ifndef O_EXCL
	#	define O_EXCL			00000200	/* not fcntl */
```
固定源行23–27。

### OF30-A044 · O_TRUNC和O_APPEND值
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TRUNC]
```c
	#ifndef O_TRUNC
	#	define O_TRUNC			00001000	/* not fcntl */
	#endif
	#ifndef O_APPEND
	#	define O_APPEND			00002000
```
固定源行32–36。

### OF30-A045 · O_DSYNC八进制值
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_DSYNC]
```c
	#ifndef O_DSYNC
	#	define O_DSYNC			00010000	/* used to be O_SYNC, see below */
```
固定源行41–42。

### OF30-A046 · O_LARGEFILE/O_DIRECTORY位
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_LARGEFILE]
```c
	#ifndef O_LARGEFILE
	#	define O_LARGEFILE		00100000
	#endif
	#ifndef O_DIRECTORY
	#	define O_DIRECTORY		00200000	/* must be a directory */
```
固定源行50–54。

### OF30-A047 · O_NOFOLLOW标志位
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NOFOLLOW]
```c
	#ifndef O_NOFOLLOW
	#	define O_NOFOLLOW		00400000	/* don't follow links */
```
固定源行56–57。

### OF30-A048 · O_CLOEXEC原始fd标志
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_CLOEXEC]
```c
	#ifndef O_CLOEXEC
	#	define O_CLOEXEC		02000000	/* set close_on_exec */
```
固定源行62–63。

### OF30-A049 · __O_SYNC与O_SYNC别名定义
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_SYNC]
```c
	#ifndef O_SYNC
	#	define __O_SYNC			04000000
	#	define O_SYNC			(__O_SYNC|O_DSYNC)
	#endif
```
固定源行79–82。

### OF30-A050 · O_PATH标志
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_PATH]
```c
	#ifndef O_PATH
	#	define O_PATH			010000000
```
固定源行84–85。

### OF30-A051 · O_TMPFILE与O_TMPFILE_MASK复合表达式
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_TMPFILE]
```c
	#endif

	/* a horrid kludge trying to make sure that this will fail on old kernels */
	#define O_TMPFILE			(__O_TMPFILE | O_DIRECTORY)
	#define O_TMPFILE_MASK		(__O_TMPFILE | O_DIRECTORY | O_CREAT)      
```
固定源行90–94。
### OF30-A052 · O_NDELAY与O_NONBLOCK别名
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_NDELAY]
```c
	#ifndef O_NDELAY
	#	define O_NDELAY			O_NONBLOCK
	#endif
```
固定源行96–98。

### OF30-A053 · open_how_s含三个__u64字段
[VERIFIED mykernel/include/uapi/linux/openat2.h::open_how_s]
```c
	typedef struct open_how {
		__u64 flags;
		__u64 mode;
		__u64 resolve;
	} open_how_s;
```
固定源行19–23。

### OF30-A054 · RESOLVE_NO_XDEV及MAGICLINKS位定义
[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_XDEV]
```c
	/* how->resolve flags for openat2(2). */
	#define RESOLVE_NO_XDEV			0x01 /* Block mount-point crossings
											(includes bind-mounts). */
	#define RESOLVE_NO_MAGICLINKS	0x02 /* Block traversal through procfs-style
											"magic-links". */
```
固定源行25–29。

### OF30-A055 · RESOLVE_NO_SYMLINKS及BENEATH位定义
[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_NO_SYMLINKS]
```c
	#define RESOLVE_NO_SYMLINKS		0x04 /* Block traversal through all symlinks
											(implies OEXT_NO_MAGICLINKS) */
	#define RESOLVE_BENEATH			0x08 /* Block "lexical" trickery like
											"..", symlinks, and absolute
											paths which escape the dirfd. */
```
固定源行30–34。

### OF30-A056 · RESOLVE_IN_ROOT及CACHED值
[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_IN_ROOT]
```c
	#define RESOLVE_IN_ROOT			0x10 /* Make all jumps to "/" and ".."
											be scoped inside the dirfd
											(similar to chroot(2)). */
	#define RESOLVE_CACHED			0x20 /* Only complete if resolution can be
```
固定源行35–38。

### OF30-A057 · RESOLVE_CACHED宏及注释边界
[VERIFIED mykernel/include/uapi/linux/openat2.h::RESOLVE_CACHED]
```c
	#define RESOLVE_CACHED			0x20 /* Only complete if resolution can be
											completed through cached lookup. May
											return -EAGAIN if that's not
											possible. */
```
固定源行38–41。

### OF30-A058 · uapi fcntl包含真实generic标志与openat2定义
[VERIFIED mykernel/include/uapi/linux/fcntl.h::fcntl]
```c
	#include <uapi/asm-generic/fcntl.h>
	#include <uapi/linux/openat2.h>
```
固定源行5–6。

### OF30-A059 · S_IALLUGO的位组合
[VERIFIED mykernel/include/linux/kernel/stat.h::S_IALLUGO]
```c
	#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
	#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
```
固定源行9–10。

### OF30-A060 · S_IFREG和特殊权限位实际定义
[VERIFIED mykernel/include/uapi/linux/stat.h::S_IFREG]
```c
	#	define S_IFSOCK		0140000		// 11-- ---- ---- ---- b
	#	define S_IFLNK		0120000		// 1-1- ---- ---- ---- b
	#	define S_IFREG		0100000		// 1--- ---- ---- ---- b
	#	define S_IFBLK		0060000		// -11- ---- ---- ---- b
	#	define S_IFDIR		0040000		// -1-- ---- ---- ---- b
```
固定源行9–13。

### OF30-A061 · S_IRWXG和用户单项权限位
[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXG]
```c
	#	define S_IRUSR		00400		// ---- ---1 ---- ---- b
	#	define S_IWUSR		00200		// ---- ---- 1--- ---- b
	#	define S_IXUSR		00100		// ---- ---- -1-- ---- b

	#	define S_IRWXG		00070		// ---- ---- --11 1--- b
```
固定源行29–33。
### OF30-A062 · 其它用户rwx位
[VERIFIED mykernel/include/uapi/linux/stat.h::S_IRWXO]
```c
	#	define S_IXGRP		00010		// ---- ---- ---- 1--- b

	#	define S_IRWXO		00007		// ---- ---- ---- -111 b
	#	define S_IROTH		00004		// ---- ---- ---- -1-- b
```
固定源行36–39。

### OF30-A063 · MAY_EXEC/WRITE/READ/APPEND数值
[VERIFIED mykernel/include/linux/fs/fs.h::MAY_READ]
```c

	#define MAY_EXEC		0x00000001
	#define MAY_WRITE		0x00000002
	#define MAY_READ		0x00000004
	#define MAY_APPEND		0x00000008
```
固定源行44–48。

### OF30-A064 · FMODE_NONOTIFY内部位
[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_NONOTIFY]
```c

	/* File was opened by fanotify and shouldn't generate fanotify events */
	#define FMODE_NONOTIFY			((__force fmode_t)0x4000000)

```
固定源行115–118。

### OF30-A065 · ACC_MODE四元素字符映射
[VERIFIED mykernel/include/linux/fs/fs.h::ACC_MODE]
```c
	#define ACC_MODE(x)			("\004\002\006\006"[(x)&O_ACCMODE])
```
固定源行3424–3424。

### OF30-A066 · LOOKUP_FOLLOW及DIRECTORY位
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_FOLLOW]
```c
	/* pathwalk mode */
	#define LOOKUP_FOLLOW			0x0001	/* follow links at the end */
	#define LOOKUP_DIRECTORY		0x0002	/* require a directory */
	#define LOOKUP_AUTOMOUNT		0x0004  /* force terminal automount */
	#define LOOKUP_EMPTY			0x4000	/* accept empty path [user_... only] */
```
固定源行38–42。

### OF30-A067 · LOOKUP_OPEN/CREATE/EXCL位
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_OPEN]
```c
	/* These tell filesystem methods that we are dealing with the final component... */
	#define LOOKUP_OPEN				0x0100	/* ... in open */
	#define LOOKUP_CREATE			0x0200	/* ... in object creation */
	#define LOOKUP_EXCL				0x0400	/* ... in exclusive creation */
```
固定源行49–52。

### OF30-A068 · 解析范围标志映射目标值
[VERIFIED mykernel/include/linux/fs/namei.h::LOOKUP_NO_SYMLINKS]
```c
	/* Scoping flags for lookup. */
	#define LOOKUP_NO_SYMLINKS		0x010000 /* No symlink crossing. */
	#define LOOKUP_NO_MAGICLINKS	0x020000 /* No nd_jump_link() crossing. */
	#define LOOKUP_NO_XDEV			0x040000 /* No mountpoint crossing. */
	#define LOOKUP_BENEATH			0x080000 /* No escaping from starting point. */
```
固定源行58–62。

### OF30-A069 · do_filp_open从op读取lookup_flags
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
file_s *do_filp_open(int dfd, IN filename_s * name, const open_flags_s *op)
{
	nameidata_s nd;
	int flags = op->lookup_flags;
	file_s *filp;
```
固定源行753–757。

### OF30-A070 · do_filp_open把op及借入filename交给path_openat
[VERIFIED mykernel/fs/vfs/namei.c::do_filp_open]
```c
	set_nameidata(&nd, dfd, name, NULL);
	filp = path_openat(&nd, op, flags);
	// if (unlikely(filp == ERR_PTR(-ECHILD)))
	// 	filp = path_openat(&nd, op, flags);
	restore_nameidata();
```
固定源行759–763。
### OF30-A071 · path_openat第一个实际字段读是op.open_flag
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
{
	file_s *file;
	int error;

	file = alloc_empty_file(op->open_flag);
```
固定源行715–719。
### OF30-A072 · path_openat将lookup flags传给path_init与循环
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	const char *s = path_init(nd, flags);
	while (!(error = link_path_walk(s, nd)) &&
		(s = open_last_lookups(nd, flags)) != NULL)
		;
	if (error == 0)
```
固定源行723–727。
### OF30-A073 · do_open调用vfs_open后没有显式返回值
[VERIFIED mykernel/fs/vfs/namei.c::do_open]
```c
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

	vfs_open(&nd->path, file);
}
```
固定源行703–707。
### OF30-A074 · 执行用open_flags局部对象初始化
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
static file_s
*do_open_execat(int fd, filename_s *name, int flags) {
	file_s *file;
	int err;
	open_flags_s open_exec_flags = {
```
固定源行554–558。

### OF30-A075 · 执行标志四字段赋值及mode未列出
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
	};
```
固定源行559–563。

### OF30-A076 · 执行入口AT_*有效值和LOOKUP标志修改
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
```
固定源行565–569。

### OF30-A077 · 执行入口直接do_filp_open和IS_ERR门
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```
固定源行572–574。

### OF30-A078 · open_exec取得filename并在非错误分支调用do_open_execat
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
	filename_s *filename = getname_kernel(name);
	file_s *f = ERR_CAST(filename);

	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
```
固定源行606–610。
### OF30-A079 · open_exec正常filename后调用putname
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
```
固定源行609–613。

### OF30-A080 · umode_t类型为unsigned int
[VERIFIED mykernel/include/linux/kernel/types.h::umode_t]
```c
		typedef __kernel_dev_t		dev_t;
		typedef __kernel_ulong_t	ino_t;
		typedef __kernel_mode_t		mode_t;
		typedef unsigned int		umode_t;
		typedef u32					nlink_t;
```
固定源行32–36。

## 已取范围与证据边界

固定time的14个实际文件直接参与本证据登记，涵盖open.c、fs/internal、fcntl三层头、open_how/RESOLVE声明、stat/权限位、fs.h的ACC_MODE、namei第一层、syscall及exec直接入口。标准`MANIFEST.md`中的`inputs_read`另列29主线回执、05::UV-04、08::US-03及任务52和固定配置金丝雀。

`#include`及宏定义引文证明定义链，**不是已运行函数体**；`MYOS_SYSCALL_DEFINE3(open)`是所选声明/宏实例而非外部openat2实现认证。`build_open_how`的`resolve`未显式赋值依C聚合初始化规则为零，仅是源码模型，未实际预处理或编译。

`do_filp_open`之后的路径遍历、文件系统打开和硬件/权限并未重审。文本、64位算术、JSON/图一致性由Code Mode JS检查，与内核运行是不同证据类别。无真实用户flags样本、无仓库C/ASM执行、无编译/QEMU/硬件/额外API模型。
