---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FD-LOOKUP-REF-41
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
runtime_evidence: NOT_RUN
---

# 第41批 evidence：固定源单定义短引文

全部40段来自time@a039d9803ade的函数活动体、类型定义或单宏定义。每段1–5行，第一、第二轮连接器读取逐行重新确认；重复/相邻片段只是证据切片，不等于独立根因或运行验证。每个标题的符号是定义所在函数/类型/宏，并已合读上下文；没有将extern、EXPORT或注释单独充当定义体。

### FG41-A001 — 函数签名、mask与初始化

[VERIFIED mykernel/fs/vfs/file.c::__simple_fget]（207–211行）

```c
static inline file_s
*__simple_fget(uint fd, fmode_t mask) {
	// return __fget_files(current->files, fd, mask);
	file_s *retval = NULL;
	files_struct_s *files = current->files;
```

### FG41-A002 — 逻辑范围、槽读取和原子增加

[VERIFIED mykernel/fs/vfs/file.c::__simple_fget]（212–216行）

```c
	if (fd < files->fd_count) {
		retval = files->fd_array[fd];
		atomic_long_inc(&retval->f_count);
	}
	return retval;
```

### FG41-A003 — 传入FMODE_PATH

[VERIFIED mykernel/fs/vfs/file.c::fget]（219–221行）

```c
file_s *fget(uint fd) {
	return __simple_fget(fd, FMODE_PATH);
}
```

### FG41-A004 — int索引及直接槽读取

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]（197–200行）

```c
fd_s myos_fdget_pos(int fd)
{
	task_s *curr = current;
	file_s *fp = curr->files->fd_array[fd];
```

### FG41-A005 — flags零标签

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]（202–203行）

```c
	return (fd_s){.file = fp, .flags = 0};
}
```

### FG41-A006 — 宏256

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]（22–22行）

```c
	#define NR_OPEN_DEFAULT	256
```

### FG41-A007 — fd_count与固定数组

[VERIFIED mykernel/include/linux/kernel/fdtable.h::files_struct_s]（67–69行）

```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
	} files_struct_s;
```

### FG41-A008 — init_files初始fd_count零

[VERIFIED mykernel/fs/vfs/file.c::init_files]（121–123行）

```c
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
};
```

### FG41-A009 — 新表fd_count设256

[VERIFIED mykernel/fs/vfs/file.c::dup_fd]（40–42行）

```c
	atomic_set(&newf->refcount, 1);
	newf->fd_count = NR_OPEN_DEFAULT;

```

### FG41-A010 — 结构的指针与标记

[VERIFIED mykernel/include/linux/fs/file.h::fd_s]（29–32行）

```c
	typedef struct fd {
		file_s		*file;
		uint		flags;
	} fd_s;
```

### FG41-A011 — FDPUT_FPUT位

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_FPUT]（33–33行）

```c
	#define FDPUT_FPUT			1
```

### FG41-A012 — FDPUT_POS_UNLOCK位

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_POS_UNLOCK]（34–34行）

```c
	#define FDPUT_POS_UNLOCK	2
```

### FG41-A013 — 索引与返回flags3

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]（52–56行）

```c
	__to_fd(ulong v) {
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
```

### FG41-A014 — fdget的uint入口

[VERIFIED mykernel/include/linux/fs/file.h::fdget]（59–63行）

```c
	static inline fd_s
	fdget(uint fd) {
		// return __to_fd(__fdget(fd));
		return __to_fd(fd);
	}
```

### FG41-A015 — raw分支

[VERIFIED mykernel/include/linux/fs/file.h::fdget_raw]（65–69行）

```c
	static inline fd_s
	fdget_raw(uint fd) {
		// return __to_fd(__fdget_raw(fd));
		return __to_fd(fd);
	}
```

### FG41-A016 — int入口

[VERIFIED mykernel/include/linux/fs/file.h::fdget_pos]（71–75行）

```c
	static inline fd_s
	fdget_pos(int fd) {
		// return __to_fd(__fdget_pos(fd));
		return __to_fd(fd);
	}
```

### FG41-A017 — flag与fput调用

[VERIFIED mykernel/include/linux/fs/file.h::fdput]（36–40行）

```c
	static inline void fdput(fd_s fd)
	{
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
	}
```

### FG41-A018 — POS解锁注释与转发

[VERIFIED mykernel/include/linux/fs/file.h::fdput_pos]（78–82行）

```c
	fdput_pos(fd_s f) {
		// if (f.flags & FDPUT_POS_UNLOCK)
		// 	__f_unlock_pos(f.file);
		fdput(f);
	}
```

### FG41-A019 — 条件归还

[VERIFIED mykernel/include/linux/fs/file.h::fput_light]（24–27行）

```c
	static inline void
	fput_light(file_s *file, int fput_needed) {
		if (fput_needed) fput(file);
	}
```

### FG41-A020 — 正常get_file增加引用

[VERIFIED mykernel/include/linux/fs/fs.h::get_file]（905–908行）

```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### FG41-A021 — 路径模式位

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_PATH]（99–99行）

```c
	#define FMODE_PATH				((__force fmode_t)0x4000)
```

### FG41-A022 — file计数与模式字段

[VERIFIED mykernel/include/linux/fs/fs.h::file_s]（870–874行）

```c
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
		uint 				f_flags;
```

### FG41-A023 — fput减计数

[VERIFIED mykernel/fs/vfs/file_table.c::fput]（108–111行）

```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

### FG41-A024 — 零计数释放动作

[VERIFIED mykernel/fs/vfs/file_table.c::fput]（126–129行）

```c

		kmem_cache_free(filp_cachep, file);
	}
}
```

### FG41-A025 — 整数宽度类型别名

[VERIFIED mykernel/include/linux/kernel/types.h::uint/ulong]（103–105行）

```c
		typedef unsigned short		ushort;
		typedef unsigned int		uint;
		typedef unsigned long		ulong;
```

### FG41-A026 — mmap系统调用ulong fd

[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]（1312–1316行）

```c
MYOS_SYSCALL_DEFINE6(mmap,
		unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, off)
{
```

### FG41-A027 — offset前置门

[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]（1317–1321行）

```c
	if (off & ~PAGE_MASK)
		return -EINVAL;

	// return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
	ulong pgoff = off >> PAGE_SHIFT;
```

### FG41-A028 — 非匿名fget及之后的NULL门

[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]（1325–1329行）

```c
	if (!(flags & MAP_ANONYMOUS)) {
		// audit_mmap_fd(fd, flags);
		file = fget(fd);
		if (!file)
			return -EBADF;
```

### FG41-A029 — 匿名巨页不返回分支

[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]（1330–1333行）

```c
	} else if (flags & MAP_HUGETLB) {
		// currently unsupported
		while (1);
	}
```

### FG41-A030 — opaque映射与fput配对

[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]（1335–1339行）

```c
	retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
out_fput:
	if (file != NULL)
		fput(file);
	return retval;
```

### FG41-A031 — 流文件位置条件

[VERIFIED mykernel/fs/vfs/read_write.c::file_ppos]（175–178行）

```c
/* file_ppos returns &file->f_pos or NULL if file is stream */
static inline loff_t *file_ppos(file_s *file) {
	return file->f_mode & FMODE_STREAM ? NULL : &file->f_pos;
}
```

### FG41-A032 — read的int形参转换

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]（180–184行）

```c
ssize_t ksys_read(uint fd, char __user *buf, size_t count)
{
	fd_s f = myos_fdget_pos(fd);
	ssize_t ret = -EBADF;

```

### FG41-A033 — read的NULL门

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]（185–189行）

```c
	if (f.file) {
		loff_t pos, *ppos = file_ppos(f.file);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

### FG41-A034 — vfs_read和局部fdput_pos

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_read]（191–195行）

```c
		ret = vfs_read(f.file, buf, count, ppos);
		if (ret >= 0 && ppos)
			f.file->f_pos = pos;
		fdput_pos(f);
	}
```

### FG41-A035 — write的fdget_pos及flags

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]（319–323行）

```c
ssize_t ksys_write(unsigned int fd, const char __user *buf, size_t count)
{
	fd_s f = fdget_pos(fd);
	ssize_t ret = -EBADF;
	file_s *filp = f.file;
```

### FG41-A036 — write的NULL门

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]（325–329行）

```c
	if (filp != NULL) {
		loff_t pos, *ppos = file_ppos(filp);
		if (ppos) {
			pos = *ppos;
			ppos = &pos;
```

### FG41-A037 — vfs_write后归还

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]（331–335行）

```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
	}
```

### FG41-A038 — stream掩码

[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_STREAM]（114–114行）

```c
	#define FMODE_STREAM			((__force fmode_t)0x200000)
```

### FG41-A039 — 旧40安装关系

[VERIFIED mykernel/fs/vfs/file.c::fd_install]（189–193行）

```c
	/* coupled with smp_wmb() in expand_fdtable() */
	smp_rmb();
	BUG_ON(fps[fd] != NULL);
	fps[fd] = file;
}
```

### FG41-A040 — 旧F_DUPFD显式get_file

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]（235–239行）

```c
	err = alloc_fd(from, nofile, flags);
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
```

## 引文支持强度与反证

- A001/A002须合读为完整__simple_fget，逻辑门之后未先检查NULL；mask虽列形参而活动体没有消费。调用者后置判空不保护callee先发生的无效成员访问。
- A013–A018中flags=3同时包含FDPUT_FPUT与FDPUT_POS_UNLOCK，但实际位置解锁被注释；有标记不等于获得了引用或执行了解锁。
- A023/A024只证f_count变零时源码调用kmem_cache_free，并不证明内存回收时机、slab正确性或后续UAF已经发生。
- A026–A030使匿名fget绕过与巨大页无正常返回路径可分开；vm_mmap_pgoff为opaque。
- A032–A037仅记录read/write入口的局部调用与归还；vfs_read/vfs_write返回值及内部对象生命周期不展开。
- A006–A009的容量256和已知fd_count源0/256不构成所有任务当前fd_count值的证明。

## 已执行与未执行

已由GitHub连接器两次读取8个固定源文件、40个选定行区间，复核引文原文及目标符号身份。阅读另含源配置、panic与13/14旧历史文档，不计入本40引文。只对有效整数及正引用初值做独立纯文本/整数模型；无任何仓库C/ASM/脚本执行、预处理/编译/QEMU/硬件、真实fd/IO、完整运行P9、外部YAML解析或Graphviz渲染；这些均为NOT_RUN。源码引用与对象有效性/控制流前提区分，不做内核正确性裁定。
