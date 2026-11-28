---
task_id: CORE-USER-MMAP-PROTECTION-20
track_id: MYOS2-LEAD-002
record_type: fixed_source_anchor_ledger
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra
source_branch: time
source_pin_short12: a039d9803ade
contract_short12: 038217219faa
initial_execution_head_short12: 6166b3e2f5b9
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
verified_claims: 155
quotes_reconfirmed: 155
downgraded_to_inferred: 0
date: 2026-11-01
---

# MP20 固定源码证据账

本账从固定 `time @ a039d9803ade` 的各文件逐字取1–5行源码。某个引文标题可能只是多段控制流之一，须以相邻锚点/完整已读函数理解其条件；**逐字相同不等于编译、运行、端到端正确或所有前提已闭合**。注释仅可证明对应语句本次非活动。系统调用包装宏属配置/静态绑定文本而非真实二进制映射认证。

### MP20-A001 · syscall表mmap绑定 (源码行12–12；分段须合读)
[VERIFIED mykernel/arch/x86_64/include/asm/syscalls_64.h::sys_mmap]
```c
__SYSCALL(__NR_mmap, sys_mmap)
```

### MP20-A002 · x64分派代码 (源码行26–30；分段须合读)
[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
```

### MP20-A003 · x64分派代码 (源码行31–31；分段须合读)
[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	default: return __x64_sys_ni_syscall(regs);
```

### MP20-A004 · 6寄存器参数顺序 (源码行27–29；分段须合读)
[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__X86_64_MAP6]
```c
	#define __X86_64_MAP6(m, t1, a1, t2, a2, t3, a3, t4, a4, t5, a5, t6, a6)	\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10), m(t5, regs->r8), m(t6, regs->r9)
```

### MP20-A005 · wrapper宏声明 (源码行241–245；分段须合读)
[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
				long __se_sys##name(const pt_regs_s *regs)				\
```

### MP20-A006 · wrapper实现到__do_sys (源码行246–250；分段须合读)
[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				{														\
					long ret = __do_sys##name(							\
								__ARCH_MAP(x,__SC_CAST,__VA_ARGS__));	\
					return ret;											\
				}														\
```

### MP20-A007 · wrapper实现到__do_sys (源码行251–251；分段须合读)
[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
```

### MP20-A008 · 系统调用宏绑定 (源码行1387–1389；分段须合读)
[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE6]
```c
	#define MYOS_SYSCALL_DEFINE4(name, ...)	__MYOS_SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
	#define MYOS_SYSCALL_DEFINE5(name, ...)	__MYOS_SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
	#define MYOS_SYSCALL_DEFINE6(name, ...)	__MYOS_SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)
```

### MP20-A009 · mmap入口形参 (源码行1312–1315；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
MYOS_SYSCALL_DEFINE6(mmap,
		unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, off)
```

### MP20-A010 · off页对齐 (源码行1316–1318；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
{
	if (off & ~PAGE_MASK)
		return -EINVAL;
```

### MP20-A011 · off字节转pgoff (源码行1320–1323；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
	// return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
	ulong pgoff = off >> PAGE_SHIFT;
	file_s *file = NULL;
	ulong retval;
```

### MP20-A012 · 匿名标志选择fget (源码行1325–1329；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
	if (!(flags & MAP_ANONYMOUS)) {
		// audit_mmap_fd(fd, flags);
		file = fget(fd);
		if (!file)
			return -EBADF;
```

### MP20-A013 · 匿名HUGETLB暂停 (源码行1330–1333；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
	} else if (flags & MAP_HUGETLB) {
		// currently unsupported
		while (1);
	}
```

### MP20-A014 · 文件临时引用fput与返回 (源码行1335–1339；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::mmap]
```c
	retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
out_fput:
	if (file != NULL)
		fput(file);
	return retval;
```

### MP20-A015 · fd边界条件 (源码行208–212；分段须合读)
[VERIFIED mykernel/fs/vfs/file.c::__simple_fget]
```c
*__simple_fget(uint fd, fmode_t mask) {
	// return __fget_files(current->files, fd, mask);
	file_s *retval = NULL;
	files_struct_s *files = current->files;
	if (fd < files->fd_count) {
```

### MP20-A016 · fd槽取file并增引用 (源码行212–216；分段须合读)
[VERIFIED mykernel/fs/vfs/file.c::__simple_fget]
```c
	if (fd < files->fd_count) {
		retval = files->fd_array[fd];
		atomic_long_inc(&retval->f_count);
	}
	return retval;
```

### MP20-A017 · fget传FMODE_PATH但内部mask未消费 (源码行219–220；分段须合读)
[VERIFIED mykernel/fs/vfs/file.c::fget]
```c
file_s *fget(uint fd) {
	return __simple_fget(fd, FMODE_PATH);
```

### MP20-A018 · VMA再持有file引用 (源码行905–908；分段须合读)
[VERIFIED mykernel/include/linux/fs/fs.h::get_file]
```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
	}
```

### MP20-A019 · fput减少引用 (源码行108–110；分段须合读)
[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
```

### MP20-A020 · pgoff包装参数 (源码行88–92；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
ulong vm_mmap_pgoff(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong pgoff)
{
	ulong ret;
	mm_s *mm = current->mm;
```

### MP20-A021 · 包装锁、do_mmap、解锁、返回 (源码行94–98；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
	ret = do_mmap(file, addr, len, prot, flag, 0, pgoff);
	mmap_write_unlock(mm);

```

### MP20-A022 · 包装锁、do_mmap、解锁、返回 (源码行99–99；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
	return ret;
```

### MP20-A023 · 另一字节offset包装 (源码行102–104；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap]
```c
ulong vm_mmap(file_s *file, ulong addr, ulong len,
		ulong prot, ulong flag, ulong offset)
{
```

### MP20-A024 · vm_mmap特有校验 (源码行105–108；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap]
```c
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;
```

### MP20-A025 · vm_mmap字节到pgoff (源码行110–110；分段须合读)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap]
```c
	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
```

### MP20-A026 · mmap写锁接口目前返回0 (源码行118–122；分段须合读)
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_lock_killable]
```c
	static inline int mmap_write_lock_killable(mm_s *mm)
	{
		int ret;

		// __mmap_lock_trace_start_locking(mm, true);
```

### MP20-A027 · mmap写锁接口目前返回0 (源码行123–126；分段须合读)
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_lock_killable]
```c
		// ret = down_write_killable(&mm->mmap_lock);
		// __mmap_lock_trace_acquire_returned(mm, true, ret == 0);
		// return ret;
		return 0;
```

### MP20-A028 · mmap unlock活动体为空 (源码行129–133；分段须合读)
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_unlock]
```c
	static inline void mmap_write_unlock(mm_s *mm)
	{
		// __mmap_lock_trace_released(mm, true);
		// vma_end_write_all(mm);
		// up_write(&mm->mmap_lock);
```

### MP20-A029 · mmap unlock活动体为空 (源码行134–134；分段须合读)
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_unlock]
```c
	}
```

### MP20-A030 · prot位定义 (源码行10–12；分段须合读)
[VERIFIED mykernel/include/uapi/asm-generic/mman-common.h::PROT_READ]
```c
	#define PROT_READ	0x1		/* page can be read */
	#define PROT_WRITE	0x2		/* page can be written */
	#define PROT_EXEC	0x4		/* page can be executed */
```

### MP20-A031 · PROT_NONE=0 (源码行13–16；分段须合读)
[VERIFIED mykernel/include/uapi/asm-generic/mman-common.h::PROT_NONE]
```c
	#define PROT_SEM	0x8		/* page may be used for atomic ops */
	/*			0x10		   reserved for arch-specific use */
	/*			0x20		   reserved for arch-specific use */
	#define PROT_NONE	0x0		/* page can not be accessed */
```

### MP20-A032 · MAP_TYPE/FIXED/ANONYMOUS (源码行20–23；分段须合读)
[VERIFIED mykernel/include/uapi/asm-generic/mman-common.h::MAP_TYPE]
```c
	/* 0x01 - 0x03 are defined in linux/mman.h */
	#define MAP_TYPE	0x0f		/* Mask for type of mapping */
	#define MAP_FIXED	0x10		/* Interpret addr exactly */
	#define MAP_ANONYMOUS	0x20		/* don't use a file */
```

### MP20-A033 · MAP_HUGETLB/SYNC/FIXED_NOREPLACE (源码行29–31；分段须合读)
[VERIFIED mykernel/include/uapi/asm-generic/mman-common.h::MAP_HUGETLB]
```c
	#define MAP_HUGETLB		0x040000	/* create a huge page mapping */
	#define MAP_SYNC		0x080000 /* perform synchronous page faults for the mapping */
	#define MAP_FIXED_NOREPLACE	0x100000	/* MAP_FIXED which doesn't unmap underlying mapping */
```

### MP20-A034 · MAP_SHARED/PRIVATE/VALIDATE位值 (源码行16–19；分段须合读)
[VERIFIED mykernel/include/uapi/linux/mman.h::MAP_SHARED]
```c
	#define MAP_SHARED				0x01	/* Share changes */
	#define MAP_PRIVATE				0x02	/* Changes are private */
	#define MAP_SHARED_VALIDATE		0x03	/* share + validate extension flags */
	#define MAP_DROPPABLE			0x08	/* Zero memory under memory pressure. */
```

### MP20-A035 · do_mmap参数/len0 (源码行469–473；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
do_mmap(file_s *file, ulong addr, ulong len, ulong prot,
		ulong flags, vm_flags_t vm_flags, ulong pgoff)
{
	task_s *curr = current;
	mm_s *mm = curr->mm;
```

### MP20-A036 · do_mmap参数/len0 (源码行474–477；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	int pkey = 0;

	if (len == 0)
		return -EINVAL;
```

### MP20-A037 · PROT_READ增加PROT_EXEC (源码行485–488；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	// if ((prot & PROT_READ) && (current->personality & READ_IMPLIES_EXEC))
	// 	if (!(file && path_noexec(&file->f_path)))
	if ((prot & PROT_READ) && !(file && path_noexec(&file->f_path)))
			prot |= PROT_EXEC;
```

### MP20-A038 · MAP_FIXED_NOREPLACE及提示 (源码行490–494；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/* force arch specific MAP_FIXED handling in get_unmapped_area */
	if (flags & MAP_FIXED_NOREPLACE)
		flags |= MAP_FIXED;

	if (!(flags & MAP_FIXED))
```

### MP20-A039 · MAP_FIXED_NOREPLACE及提示 (源码行495–495；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		addr = round_hint_to_min(addr);
```

### MP20-A040 · 页对齐len与检查 (源码行497–500；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/* Careful about overflows.. */
	len = PAGE_ALIGN(len);
	if (!len)
		return -ENOMEM;
```

### MP20-A041 · 偏移溢出和map_count (源码行502–506；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
		return -EOVERFLOW;

	/* Too many mappings? */
```

### MP20-A042 · 偏移溢出和map_count (源码行507–508；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (mm->map_count > sysctl_max_map_count)
		return -ENOMEM;
```

### MP20-A043 · vm_flags组合含VM_MAY* (源码行515–516；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	vm_flags |= calc_vm_prot_bits(prot, pkey) | calc_vm_flag_bits(flags) |
				mm->def_flags | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC;
```

### MP20-A044 · 单bit平移转换 (源码行18–22；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::_calc_vm_trans]
```c
	#define _calc_vm_trans(x, bit1, bit2) (						\
				(!(bit1) || !(bit2)) ? 0 : 						\
					((bit1) <= (bit2) ?							\
						((x) & (bit1)) * ((bit2) / (bit1))		\
						: ((x) & (bit1)) / ((bit1) / (bit2)))	\
```

### MP20-A045 · 单bit平移转换 (源码行23–23；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map_macro.h::_calc_vm_trans]
```c
			)
```

### MP20-A046 · prot转VM权限 (源码行413–417；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map.h::calc_vm_prot_bits]
```c
		ulong
		calc_vm_prot_bits(ulong prot, ulong pkey) {
			return _calc_vm_trans(prot, PROT_READ, VM_READ) |
					_calc_vm_trans(prot, PROT_WRITE, VM_WRITE) |
					_calc_vm_trans(prot, PROT_EXEC, VM_EXEC) |
```

### MP20-A047 · prot转VM权限 (源码行418–418；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map.h::calc_vm_prot_bits]
```c
					arch_calc_vm_prot_bits(prot, pkey);
```

### MP20-A048 · MAP转VM属性 (源码行425–429；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map.h::calc_vm_flag_bits]
```c
		ulong
		calc_vm_flag_bits(ulong flags) {
			return _calc_vm_trans(flags, MAP_GROWSDOWN, VM_GROWSDOWN) |
					_calc_vm_trans(flags, MAP_LOCKED, VM_LOCKED) |
					_calc_vm_trans(flags, MAP_SYNC, VM_SYNC) |
```

### MP20-A049 · MAP转VM属性 (源码行430–430；分段须合读)
[VERIFIED mykernel/mm/vm_map/vm_map.h::calc_vm_flag_bits]
```c
					arch_calc_vm_flag_bits(flags);
```

### MP20-A050 · 地址选择与错误返回 (源码行518–522；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	/*
	 * Obtain the address to map to. we verify (or select) it and ensure
	 * that it represents a valid section of the address space.
	 */
	addr = __get_unmapped_area(file, addr, len, pgoff, flags, vm_flags);
```

### MP20-A051 · 地址选择与错误返回 (源码行523–524；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (IS_ERR_VALUE(addr))
		return addr;
```

### MP20-A052 · MAP_FIXED_NOREPLACE冲突 (源码行526–528；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (flags & MAP_FIXED_NOREPLACE)
		if (find_vma_intersection(mm, addr, addr + len))
			return -EEXIST;
```

### MP20-A053 · 文件长度和页偏移边界 (源码行452–456；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::file_mmap_ok]
```c
static inline bool
file_mmap_ok(file_s *file, inode_s *inode, ulong pgoff, ulong len) {
	u64 maxsize = file_mmap_size_max(file, inode);

	if (maxsize && len > maxsize)
```

### MP20-A054 · 文件长度和页偏移边界 (源码行457–461；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::file_mmap_ok]
```c
		return false;
	maxsize -= len;
	if (pgoff > maxsize >> PAGE_SHIFT)
		return false;
	return true;
```

### MP20-A055 · file元数据与file_mmap_ok (源码行530–534；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (file) {
		inode_s *inode = file_inode(file);
		ulong flags_mask;

		if (!file_mmap_ok(file, inode, pgoff, len))
```

### MP20-A056 · file元数据与file_mmap_ok (源码行535–535；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			return -EOVERFLOW;
```

### MP20-A057 · legacy掩码及FOP_MMAP_SYNC (源码行537–540；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		flags_mask = LEGACY_MAP_MASK;
		if (file->f_op->fop_flags & FOP_MMAP_SYNC)
			flags_mask |= MAP_SYNC;

```

### MP20-A058 · MAP_SHARED与VALIDATE分支 (源码行541–545；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			/*
			 * Force use of MAP_SHARED_VALIDATE with non-legacy
			 * flags. E.g. MAP_SYNC is dangerous to use with
```

### MP20-A059 · MAP_SHARED与VALIDATE分支 (源码行546–550；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			 * MAP_SHARED as you don't know which consistency model
			 * you will get. We silently ignore unsupported flags
			 * with MAP_SHARED to preserve backward compatibility.
			 */
			flags &= LEGACY_MAP_MASK;
```

### MP20-A060 · MAP_SHARED与VALIDATE分支 (源码行551–554；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			fallthrough;
		case MAP_SHARED_VALIDATE:
			if (flags & ~flags_mask)
				return -EOPNOTSUPP;
```

### MP20-A061 · 共享PROT_WRITE需FMODE_WRITE (源码行555–559；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			if (prot & PROT_WRITE) {
				if (!(file->f_mode & FMODE_WRITE))
					return -EACCES;
				// if (IS_SWAPFILE(file->f_mapping->host))
				// 	return -ETXTBSY;
```

### MP20-A062 · 共享IS_APPEND且f_mode可写拒绝 (源码行564–567；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			 * file..
			 */
			if (IS_APPEND(inode) && (file->f_mode & FMODE_WRITE))
				return -EACCES;
```

### MP20-A063 · 共享只读fd的VM_SHARED退位及FMODE_READ (源码行569–573；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			vm_flags |= VM_SHARED | VM_MAYSHARE;
			if (!(file->f_mode & FMODE_WRITE))
				vm_flags &= ~(VM_MAYWRITE | VM_SHARED);
			fallthrough;
		case MAP_PRIVATE:
```

### MP20-A064 · 共享只读fd的VM_SHARED退位及FMODE_READ (源码行574–575；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			if (!(file->f_mode & FMODE_READ))
				return -EACCES;
```

### MP20-A065 · noexec有条件清VM_MAYEXEC (源码行576–580；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			if (path_noexec(&file->f_path)) {
				if (vm_flags & VM_EXEC)
					return -EPERM;
				vm_flags &= ~VM_MAYEXEC;
			}
```

### MP20-A066 · 文件回调和GROWSDOWN限制 (源码行582–586；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			if (!file->f_op->mmap)
				return -ENODEV;
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
			break;
```

### MP20-A067 · 文件回调和GROWSDOWN限制 (源码行587–589；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c

		default:
			return -EINVAL;
```

### MP20-A068 · 无文件MAP_SHARED源分支 (源码行591–595；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	} else {
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
```

### MP20-A069 · 无文件MAP_SHARED源分支 (源码行596–600；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			/*
			 * Ignore pgoff.
			 */
			pgoff = 0;
			vm_flags |= VM_SHARED | VM_MAYSHARE;
```

### MP20-A070 · 无文件private及未知类型拒绝 (源码行632–636；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		case MAP_PRIVATE:
			/*
			 * Set pgoff according to addr for anon_vma.
			 */
			pgoff = addr >> PAGE_SHIFT;
```

### MP20-A071 · 无文件private及未知类型拒绝 (源码行637–640；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			break;
		default:
			return -EINVAL;
		}
```

### MP20-A072 · do_mmap返回simple_mmap_region结果 (源码行643–645；分段须合读)
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	addr = simple_mmap_region(file, addr, len, vm_flags, pgoff);

	return addr;
```

### MP20-A073 · f_mode读/写状态位 (源码行55–59；分段须合读)
[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_READ]
```c
	/*
	* flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
	* to O_WRONLY and O_RDWR via the strange trick in do_dentry_open()
	*/

```

### MP20-A074 · f_mode读/写状态位 (源码行60–63；分段须合读)
[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_READ]
```c
	/* file is open for reading */
	#define FMODE_READ			((__force fmode_t)0x1)
	/* file is open for writing */
	#define FMODE_WRITE			((__force fmode_t)0x2)
```


### MP20-A075 · 先调用旧映射解除帮助函数 (源码行146–149；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_munmap_vma_range]
```c
static inline int
simple_munmap_vma_range(mm_s *mm, ulong start, ulong len, vma_s **pprev) {
	*pprev = find_vma_prev_to_addr(mm, start);
	return simple_do_vma_munmap(mm, start, start + len);
```

### MP20-A076 · 合并VMA按flags/file/close比较 (源码行241–245；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_is_mergeable_vma]
```c
	if ((vma->vm_flags ^ vm_flags) & ~VM_SOFTDIRTY)
		return false;
	if (vma->vm_file != file)
		return false;
	if (vma->vm_ops != NULL && vma->vm_ops->close != NULL)
```

### MP20-A077 · 合并VMA按flags/file/close比较 (源码行246–247；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_is_mergeable_vma]
```c
		return false;
	return true;
```

### MP20-A078 · 特殊VMA拒绝合并 (源码行380–381；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (vm_flags & VM_SPECIAL)
		return NULL;
```

### MP20-A079 · 既存前后VMA候选合并条件 (源码行392–396；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (prev != NULL && addr == prev->vm_end
			&& simple_can_vma_merge_after(prev, vm_flags, file, pgoff)) {
		vma_start = prev->vm_start;
		vma_pgoff = prev->vm_pgoff;
		adjust_target = prev;
```

### MP20-A080 · 既存前后VMA候选合并条件 (源码行397–401；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		merge_prev = true;
	}
	/* Can we merge the successor? */
	if (next != NULL && simple_can_vma_merge_before(next,
			vm_flags, file, pgoff+pglen)) {
```

### MP20-A081 · 既存前后VMA候选合并条件 (源码行402–405；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
		vma_end = next->vm_end;
		if (!prev)
			adjust_target = next;
		merge_next = true;
```

### MP20-A082 · 合并对象边界更新及返回 (源码行408–412；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c
	if (!merge_prev && !merge_next)
		return NULL;	/* Not mergeable. */

	if (adjust_target == NULL)
		adjust_target = curr;
```

### MP20-A083 · 合并对象边界更新及返回 (源码行413–416；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_merge]
```c

	while (prev == NULL && curr == NULL && next == NULL);
	__simple_vma_adjust(adjust_target, vma_start, vma_end);
	return adjust_target;
```

### MP20-A084 · VMA入链操作 (源码行173–177；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
simple_vma_link(mm_s *mm, vma_s *vma, vma_s *prev) {
	// VMA_ITERATOR(vmi, mm, 0);

	// vma_iter_config(&vmi, vma->vm_start, vma->vm_end);
	// if (vma_iter_prealloc(&vmi, vma))
```

### MP20-A085 · VMA入链操作 (源码行178–182；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
	// 	return -ENOMEM;

	__vma_link_to_list(mm, vma, prev);
	vma_link_file(vma);

```

### MP20-A086 · VMA入链操作 (源码行183–183；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
	// validate_mm(mm);
```

### MP20-A087 · 创建/合并前先尝试解除旧映射 (源码行662–664；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;
```

### MP20-A088 · 已有VMA可直接合并返回out (源码行669–672；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma = simple_vma_merge(mm, prev, addr,
			addr + len, vm_flags, file, pgoff);
	if (vma != NULL)
		goto out;
```

### MP20-A089 · 新VMA分配错误 (源码行679–683；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma = vm_area_alloc(mm);
	if (vma == NULL) {
		error = -ENOMEM;
		goto unacct_error;
	}
```

### MP20-A090 · 新VMA写入vm_flags/vm_page_prot (源码行685–689；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;
```

### MP20-A091 · 新文件映射持有VMA引用并调用回调 (源码行691–695；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error != -ENOERR)
			goto unmap_and_free_vma;
```

### MP20-A092 · 回调后根据VM_SHARED|MAYWRITE做mapping检查 (源码行697–701；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		if (vma_is_shared_maywrite(vma)) {
			error = mapping_map_writable(file->f_mapping);
			if (error)
				goto close_and_free_vma;

```

### MP20-A093 · 回调后根据VM_SHARED|MAYWRITE做mapping检查 (源码行702–703；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
			writable_file_mapping = true;
		}
```

### MP20-A094 · 文件回调修改开始地址的错误检查 (源码行709–711；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		error = -EINVAL;
		if (WARN_ON((addr != vma->vm_start)))
			goto close_and_free_vma;
```

### MP20-A095 · 回调改变vm_flags后可再次合并 (源码行717–720；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		if (unlikely(vm_flags != vma->vm_flags && prev != NULL)) {
			merge = simple_vma_merge(mm, prev, vma->vm_start, vma->vm_end,
					vma->vm_flags, vma->vm_file, vma->vm_pgoff);
			if (merge) {
```

### MP20-A096 · 再次合并成功时更新vm_flags变量 (源码行728–732；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
				fput(vma->vm_file);
				vm_area_free(vma);
				vma = merge;
				/* Update vm_flags to pick up the change. */
				vm_flags = vma->vm_flags;
```

### MP20-A097 · 再次合并成功时更新vm_flags变量 (源码行733–733；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
				goto unmap_writable;
```

### MP20-A098 · 无文件共享shmem与私有匿名不同 (源码行737–741；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		vm_flags = vma->vm_flags;
	} else if (vm_flags & VM_SHARED) {
		error = shmem_zero_setup(vma);
		if (error)
			goto free_vma;
```

### MP20-A099 · 无文件共享shmem与私有匿名不同 (源码行742–744；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	} else {
		vma_set_anonymous(vma);
	}
```

### MP20-A100 · 新VMA入链与映射共享写计数退出 (源码行746–750；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	simple_vma_link(mm, vma, prev);
	/* Once vma denies write, undo our temporary denial count */
unmap_writable:
	if (file && (vm_flags & VM_SHARED))
		mapping_unmap_writable(file->f_mapping);
```

### MP20-A101 · 新VMA入链与映射共享写计数退出 (源码行751–752；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	file = vma->vm_file;
out:
```

### MP20-A102 · 最终SOFTDIRTY增位但vma_set_page_prot注释 (源码行773–776；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_flags |= VM_SOFTDIRTY;
	// vma_set_page_prot(vma);

	return addr;
```

### MP20-A103 · file mmap错误分支与局部清理 (源码行778–782；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
close_and_free_vma:
	if (file && vma->vm_ops && vma->vm_ops->close)
		vma->vm_ops->close(vma);

unmap_and_free_vma:
```

### MP20-A104 · file mmap错误分支与局部清理 (源码行783–787；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	fput(vma->vm_file);
	vma->vm_file = NULL;

	/* Undo any partial mapping done by a device driver. */
	simple_unmap_region(mm, vma, prev, vma->vm_start, vma->vm_end);
```

### MP20-A105 · file mmap错误分支与局部清理 (源码行788–792；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	if (vm_flags & VM_SHARED)
		mapping_unmap_writable(file->f_mapping);
free_vma:
	vm_area_free(vma);
unacct_error:
```

### MP20-A106 · file mmap错误分支与局部清理 (源码行793–793；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	return error;
```

### MP20-A107 · 文件回调是file->f_op->mmap (源码行1954–1956；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::call_mmap]
```c
	static inline int call_mmap(file_s *file, vma_s *vma) {
		return file->f_op->mmap(file, vma);
	}
```

### MP20-A108 · f_mode、f_count与f_flags独立字段 (源码行869–873；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		spinlock_t			f_lock;
		fmode_t				f_mode;
		atomic_long_t		f_count;
		// mutex_s				f_pos_lock;
		loff_t				f_pos;
```

### MP20-A109 · f_mode、f_count与f_flags独立字段 (源码行874–875；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::file_s]
```c
		uint 				f_flags;
		// fown_struct_s		f_owner;
```

### MP20-A110 · 文件映射写许可的mapping原语 (源码行484–488；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::mapping_map_writable]
```c
	static inline int mapping_map_writable(addr_spc_s *mapping)
	{
		return atomic_inc_unless_negative(&mapping->i_mmap_writable) ?
			0 : -EPERM;
	}
```

### MP20-A111 · 退出映射计数减少 (源码行490–493；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::mapping_unmap_writable]
```c
	static inline void mapping_unmap_writable(addr_spc_s *mapping)
	{
		atomic_dec(&mapping->i_mmap_writable);
	}
```

### MP20-A112 · MAP_SYNC能力由file_op标志表达 (源码行1900–1903；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::FOP_MMAP_SYNC]
```c
	/* Supports async buffered writes */
	#define FOP_BUFFER_WASYNC		((__force fop_flags_t)(1 << 1))
	/* Supports synchronous page faults for mappings */
	#define FOP_MMAP_SYNC			((__force fop_flags_t)(1 << 2))
```

### MP20-A113 · IS_APPEND指inode的S_APPEND而非O_APPEND (源码行2074–2075；必要时合读连续锚点)
[VERIFIED mykernel/include/linux/fs/fs.h::IS_APPEND]
```c
	#define IS_NOQUOTA(inode)		((inode)->i_flags & S_NOQUOTA)
	#define IS_APPEND(inode)		((inode)->i_flags & S_APPEND)
```

### MP20-A114 · 代表性活动文件mmap只改变vm_ops (源码行411–415；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap]
```c
int generic_file_mmap(file_s *file, vma_s *vma)
{
	addr_spc_s *mapping = file->f_mapping;

	// if (!mapping->a_ops->read_folio)
```

### MP20-A115 · 代表性活动文件mmap只改变vm_ops (源码行416–419；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap]
```c
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
```

### MP20-A116 · FAT32文件操作表绑定generic_file_mmap (源码行354–358；必要时合读连续锚点)
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
file_ops_s FAT32_file_ops = 
{
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
```

### MP20-A117 · FAT32文件操作表绑定generic_file_mmap (源码行359–363；必要时合读连续锚点)
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.llseek			= FAT32_lseek,
	.read_iter		= generic_file_read_iter,
	.write_iter		= generic_file_write_iter,
	.mmap			= generic_file_mmap,
	.iterate_shared	= FAT32_getdents64,
```

### MP20-A118 · 私有16项保护模板 (源码行9–13；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
static pgprot_t protection_map[16] __ro_after_init = {
	[VM_NONE]									= PAGE_NONE,
	[VM_READ]									= PAGE_READONLY,
	[VM_WRITE]									= PAGE_COPY,
	[VM_WRITE | VM_READ]						= PAGE_COPY,
```

### MP20-A119 · 私有16项保护模板 (源码行14–17；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_EXEC]									= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_READ]							= PAGE_READONLY_EXEC,
	[VM_EXEC | VM_WRITE]						= PAGE_COPY_EXEC,
	[VM_EXEC | VM_WRITE | VM_READ]				= PAGE_COPY_EXEC,
```

### MP20-A120 · 共享保护模板 (源码行18–22；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_SHARED]									= PAGE_NONE,
	[VM_SHARED | VM_READ]						= PAGE_READONLY,
	[VM_SHARED | VM_WRITE]						= PAGE_SHARED,
	[VM_SHARED | VM_WRITE | VM_READ]			= PAGE_SHARED,
	[VM_SHARED | VM_EXEC]						= PAGE_READONLY_EXEC,
```

### MP20-A121 · 共享保护模板 (源码行23–25；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::protection_map]
```c
	[VM_SHARED | VM_EXEC | VM_READ]				= PAGE_READONLY_EXEC,
	[VM_SHARED | VM_EXEC | VM_WRITE]			= PAGE_SHARED_EXEC,
	[VM_SHARED | VM_EXEC | VM_WRITE | VM_READ]	= PAGE_SHARED_EXEC,
```

### MP20-A122 · 以低4位VM_*选择模板 (源码行36–39；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
pgprot_t vm_get_page_prot(ulong vm_flags)
{
	ulong val = pgprot_val(protection_map[vm_flags &
					(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]);
```

### MP20-A123 · PKEY条件编译对保护位可增位 (源码行41–45；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	/*
	 * Take the 4 protection key bits out of the vma->vm_flags value and
	 * turn them in to the bits that we can put in to a pte.
	 *
```

### MP20-A124 · PKEY条件编译对保护位可增位 (源码行46–50；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
	 * Only override these if Protection Keys are available (which is only
	 * on 64-bit).
	 */
	if (vm_flags & VM_PKEY_BIT0)
		val |= _PAGE_PKEY_BIT0;
```

### MP20-A125 · PKEY条件编译对保护位可增位 (源码行51–55；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
	if (vm_flags & VM_PKEY_BIT1)
		val |= _PAGE_PKEY_BIT1;
	if (vm_flags & VM_PKEY_BIT2)
		val |= _PAGE_PKEY_BIT2;
	if (vm_flags & VM_PKEY_BIT3)
```

### MP20-A126 · PKEY条件编译对保护位可增位 (源码行56–56；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
		val |= _PAGE_PKEY_BIT3;
```

### MP20-A127 · PRESENT时通过__supported_pte_mask筛选 (源码行59–62；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/vm_map/pgprot.c::vm_get_page_prot]
```c
	// val = __sme_set(val);
	if (val & _PAGE_PRESENT)
		val &= __supported_pte_mask;
	return __pgprot(val);
```

### MP20-A128 · x86_64各类模板的PRESENT/USER/RW/NX (源码行180–184；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_NONE]
```c
	#define PAGE_NONE					__pg(   0|   0|   0|___A|   0|   0|   0|___G)
	#define PAGE_SHARED					__pg(__PP|__RW|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_SHARED_EXEC			__pg(__PP|__RW|_USR|___A|   0|   0|   0|   0)
	#define PAGE_COPY_NOEXEC			__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_COPY_EXEC				__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
```

### MP20-A129 · x86_64各类模板的PRESENT/USER/RW/NX (源码行185–187；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_const_arch.h::PAGE_NONE]
```c
	#define PAGE_COPY					__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY				__pg(__PP|   0|_USR|___A|__NX|   0|   0|   0)
	#define PAGE_READONLY_EXEC			__pg(__PP|   0|_USR|___A|   0|   0|   0|   0)
```

### MP20-A130 · 支持掩码定义的初始化值 (源码行872–875；必要时合读连续锚点)
[VERIFIED mykernel/arch/x86_64/mm/init.c::__supported_pte_mask]
```c
/* Bits supported by the hardware: */
pteval_t __supported_pte_mask __read_mostly = ~0;
/* Bits allowed in normal kernel mappings: */
pteval_t __default_kernel_pte_mask __read_mostly = ~0;
```

### MP20-A131 · 新VMA默认dummy vm_ops初始化 (源码行95–99；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
		vma_init(vma_s *vma, mm_s *mm) {
			static const vm_ops_s dummy_vm_ops = {};
			memset(vma, 0, sizeof(*vma));
			INIT_LIST_S(&vma->list);
			// INIT_LIST_HEAD(&vma->anon_vma_chain);
```

### MP20-A132 · 新VMA默认dummy vm_ops初始化 (源码行100–102；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
			vma->vm_mm = mm;
			vma->vm_ops = &dummy_vm_ops;
		}
```

### MP20-A133 · 匿名VMA显式清除vm_ops (源码行161–165；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_set_anonymous]
```c
		void
		vma_set_anonymous(vma_s *vma) {
			vma->vm_ops = NULL;
		}

```

### MP20-A134 · 匿名VMA显式清除vm_ops (源码行166–169；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_set_anonymous]
```c
		PREFIX_STATIC_INLINE
		bool
		vma_is_anonymous(vma_s *vma) {
			return !vma->vm_ops;
```

### MP20-A135 · 当前VM_READ/WRITE/EXEC与VM_MAY*不同位 (源码行76–80；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_READ]
```c
	#	define VM_NONE				0x00000000

	#	define VM_READ				0x00000001	/* currently active flags */
	#	define VM_WRITE				0x00000002
	#	define VM_EXEC				0x00000004
```

### MP20-A136 · 当前VM_READ/WRITE/EXEC与VM_MAY*不同位 (源码行81–85；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_READ]
```c
	#	define VM_SHARED			0x00000008

	/* mprotect() hardcodes VM_MAYREAD >> 4 == VM_READ, and so for r/w/x bits. */
	#	define VM_MAYREAD			0x00000010	/* limits for mprotect() etc */
	#	define VM_MAYWRITE			0x00000020
```

### MP20-A137 · 当前VM_READ/WRITE/EXEC与VM_MAY*不同位 (源码行86–87；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_READ]
```c
	#	define VM_MAYEXEC			0x00000040
	#	define VM_MAYSHARE			0x00000080
```

### MP20-A138 · 实际锁接口调用与返回 (源码行94–98；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
	ret = do_mmap(file, addr, len, prot, flag, 0, pgoff);
	mmap_write_unlock(mm);

```

### MP20-A139 · 实际锁接口调用与返回 (源码行99–99；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
	return ret;
```

### MP20-A140 · 成功路径结束未活动保护刷新 (源码行752–756；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
out:
	// vm_stat_account(mm, vm_flags, len >> PAGE_SHIFT);
	// if (vm_flags & VM_LOCKED) {
	// 	if ((vm_flags & VM_SPECIAL) || vma_is_dax(vma) ||
	// 				is_vm_hugetlb_page(vma) ||
```

### MP20-A141 · 成功路径结束未活动保护刷新 (源码行757–761；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	// 				vma == get_gate_vma(current->mm))
	// 		vma->vm_flags &= VM_LOCKED_CLEAR_MASK;
	// 	else
	// 		mm->locked_vm += (len >> PAGE_SHIFT);
	// }
```

### MP20-A142 · 成功路径结束未活动保护刷新 (源码行762–766；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c

	// if (file)
	// 	uprobe_mmap(vma);

	/*
```

### MP20-A143 · 成功路径结束未活动保护刷新 (源码行767–771；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	 * New (or expanded) vma always get soft dirty status.
	 * Otherwise user-space soft-dirty page tracker won't
	 * be able to distinguish situation when vma area unmapped,
	 * then new mapped in-place (which must be aimed as
	 * a completely new data area).
```

### MP20-A144 · 成功路径结束未活动保护刷新 (源码行772–776；必要时合读连续锚点)
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	 */
	vma->vm_flags |= VM_SOFTDIRTY;
	// vma_set_page_prot(vma);

	return addr;
```


### MP20-A145 · 新file的引用计数和f_flags/f_mode按OPEN_FMODE设置 (行55–59；请合读相邻片段)
[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	atomic_long_set(&f->f_count, 1);
	// rwlock_init(&f->f_owner.lock);
	spin_lock_init(&f->f_lock);
	// mutex_init(&f->f_pos_lock);
	f->f_flags = flags;
```

### MP20-A146 · 新file的引用计数和f_flags/f_mode按OPEN_FMODE设置 (行60–60；请合读相邻片段)
[VERIFIED mykernel/fs/vfs/file_table.c::alloc_empty_file]
```c
	f->f_mode = OPEN_FMODE(flags);
```

### MP20-A147 · OPEN_FMODE根据O_ACCMODE变换为FMODE位 (行3424–3428；请合读相邻片段)
[VERIFIED mykernel/include/linux/fs/fs.h::OPEN_FMODE]
```c
	#define ACC_MODE(x)			("\004\002\006\006"[(x)&O_ACCMODE])
	#define OPEN_FMODE(flag) (									\
				(__force fmode_t)(((flag + 1) & O_ACCMODE) |	\
					(flag & __FMODE_NONOTIFY))					\
				)
```

### MP20-A148 · O_RDONLY/O_WRONLY/O_RDWR的独立O值 (行19–22；请合读相邻片段)
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_ACCMODE]
```c
	#define O_ACCMODE			00000003
	#define O_RDONLY			00000000
	#define O_WRONLY			00000001
	#define O_RDWR				00000002
```

### MP20-A149 · O_APPEND用户打开标志 (行32–36；请合读相邻片段)
[VERIFIED mykernel/include/uapi/asm-generic/fcntl.h::O_APPEND]
```c
	#ifndef O_TRUNC
	#	define O_TRUNC			00001000	/* not fcntl */
	#endif
	#ifndef O_APPEND
	#	define O_APPEND			00002000
```

### MP20-A150 · noexec来自mnt_flags或superblock s_iflags (行60–64；请合读相邻片段)
[VERIFIED mykernel/sched/forkexec/exec.c::path_noexec]
```c
bool path_noexec(const path_s *path)
{
	return (path->mnt->mnt_flags & MNT_NOEXEC) ||
	       (path->mnt->mnt_sb->s_iflags & SB_I_NOEXEC);
}
```

### MP20-A151 · 遗留mmap标志允许集合前半 (行252–256；请合读相邻片段)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::LEGACY_MAP_MASK]
```c
	#define LEGACY_MAP_MASK	(		\
				MAP_SHARED			\
				| MAP_PRIVATE		\
				| MAP_FIXED			\
				| MAP_ANONYMOUS		\
```

### MP20-A152 · 遗留mmap标志允许集合前半 (行257–257；请合读相邻片段)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::LEGACY_MAP_MASK]
```c
				| MAP_DENYWRITE		\
```

### MP20-A153 · 遗留标志允许集合中段 (行258–262；请合读相邻片段)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::LEGACY_MAP_MASK]
```c
				| MAP_EXECUTABLE	\
				| MAP_UNINITIALIZED	\
				| MAP_GROWSDOWN		\
				| MAP_LOCKED		\
				| MAP_NORESERVE		\
```

### MP20-A154 · 遗留标志允许集合后半 (行263–267；请合读相邻片段)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::LEGACY_MAP_MASK]
```c
				| MAP_POPULATE		\
				| MAP_NONBLOCK		\
				| MAP_STACK			\
				| MAP_HUGETLB		\
				| MAP_32BIT			\
```

### MP20-A155 · 遗留标志允许集合后半 (行268–269；请合读相邻片段)
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::LEGACY_MAP_MASK]
```c
				| MAP_HUGE_2MB		\
				| MAP_HUGE_1GB		\
```


## 读取、自查与未做

- 已完整读取39号固定合同、CORE-USER-ANON-ZERO-19-REVIEW-001及其R19-S01/S02/S03；复用08/14的限定边界而不重开旧分析。固定time的options_flags.cmake全文先读；默认分支搜索只作导航；实证锚点均来自冻结time。
- 实际读取的固定文件覆盖系统调用表与包装、mmap.c、vm_map/util、宏与UAPI位、路径挂载noexec、文件模式的OPEN_FMODE、文件引用/fget/fput、锁包装、VMA合并与生成、代表性FAT32→generic_file_mmap、pgprot与PTE模板、__supported_pte_mask定义。受限接口、回调集合与全部错误回滚未做全树穷尽。
- 对每个MP20-A编号逐字确认源文本；本次使用允许的临时文本工具进行JSON集合、ID和有界按位值检查，不执行MyOS2仓库脚本、预处理、编译、C或ASM。
- 没有对链接产物、实际用户程序、真实文件系统、QEMU、硬件、全局锁、SMP、TLB、CPU执行权限或完整W^X提供实测或认证；runtime_evidence=NOT_RUN。数据边与实际写入边必须分开。
