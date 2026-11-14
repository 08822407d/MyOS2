---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-ELF-SEGMENT-LOAD-26
record_type: fixed_source_quote_evidence
produced_by: "ChatGPT普通聊天；Owner选择GPT-6＋Extra"
date: 2026-11-02
source_ref: time
source_pin_short12: a039d9803ade
read_channel: connector
status: final_static
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---

# 第26批·固定源码证据寄存

本文件每个VERIFIED标签只指本轮独立打开的 `time@a039d9803ade` 源码的定义体内部1–5行逐字引文。项目内核的运行表现、相关输入的真实可达性、完整CPU/用户异常恢复、文件物理字节及用户态结果不因此获得认证。默认分支搜索仅用于定位真实路径，关键断言均由固定源码引文支持。

### EL26-A001 · ELF基本类型及架构门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* First of all, some simple consistency checks */
	if (memcmp(elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	if (elf_ex->e_type != ET_EXEC && elf_ex->e_type != ET_DYN)
```
固定源码643–647行（行号为定位辅助）。

### EL26-A002 · ELF文件mmap方法存在门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		goto out;
	if (elf_check_fdpic(elf_ex))
		goto out;
	if (!bprm->file->f_op->mmap)
		goto out;
```
固定源码650–654行（行号为定位辅助）。

### EL26-A003 · 程序头总长度计算与上界拒绝
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	/* Sanity check the number of program headers... */
	/* ...and their total size. */
	size = sizeof(elf_phdr_s) * elf_ex->e_phnum;
	if (size == 0 || size > 65536 || size > ELF_MIN_ALIGN)
		goto out;
```
固定源码479–483行（行号为定位辅助）。
### EL26-A004 · PT_LOAD过滤与初始化前提
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		ulong total_size = 0;
		ulong alignment;

		if (elf_ppnt->p_type != PT_LOAD)
			continue;
```
固定源码835–839行（行号为定位辅助）。
### EL26-A005 · 段权限与MAP_PRIVATE标志来源
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		elf_prot = make_prot(elf_ppnt->p_flags, &arch_state,
						!!interpreter, false);

		elf_flags = MAP_PRIVATE;

```
固定源码841–845行（行号为定位辅助）。
### EL26-A006 · 后续段MAP_FIXED及ET_EXEC分支
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (!first_pt_load) {
			elf_flags |= MAP_FIXED;
		} else if (elf_ex->e_type == ET_EXEC) {
```
固定源码853–855行（行号为定位辅助）。

### EL26-A007 · ET_EXEC首段固定不覆盖标志
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			 */
			elf_flags |= MAP_FIXED_NOREPLACE;
		} else if (elf_ex->e_type == ET_DYN) {
```
固定源码860–862行（行号为定位辅助）。

### EL26-A008 · ET_DYN首段总体尺寸及零值门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			total_size = total_mapping_size(elf_phdata,
							elf_ex->e_phnum);
			if (!total_size) {
				retval = -EINVAL;
				goto out_free_dentry;
```
固定源码888–892行（行号为定位辅助）。

### EL26-A009 · ET_DYN解释器分支的对齐与NOREPLACE
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
				// 	load_bias += arch_mmap_rnd();
				alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);
				if (alignment)
					load_bias &= ~(alignment - 1);
				elf_flags |= MAP_FIXED_NOREPLACE;
```
固定源码923–927行（行号为定位辅助）。
### EL26-A010 · 大对齐无解释器时的探测性elf_load
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
				if (alignment > ELF_MIN_ALIGN) {
					load_bias = elf_load(bprm->file, 0, elf_ppnt,
									elf_prot, elf_flags, total_size);
					if (BAD_ADDR(load_bias)) {
						retval = IS_ERR_VALUE(load_bias) ?
```
固定源码942–946行（行号为定位辅助）。

### EL26-A011 · 探测返回BAD_ADDR和vm_munmap
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
						retval = IS_ERR_VALUE(load_bias) ?
									PTR_ERR((void*)load_bias) : -EINVAL;
						goto out_free_dentry;
					}
					vm_munmap(load_bias, total_size);
```
固定源码946–950行（行号为定位辅助）。

### EL26-A012 · 对齐偏移及探测后标志
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
					if (alignment)
						load_bias &= ~(alignment - 1);
					elf_flags |= MAP_FIXED_NOREPLACE;
				} else
					load_bias = 0;
```
固定源码952–956行（行号为定位辅助）。

### EL26-A013 · 最终load_bias和真实elf_load调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			load_bias = ELF_PAGESTART(load_bias - vaddr);
		}

		error = elf_load(bprm->file, load_bias + vaddr,
					elf_ppnt, elf_prot, elf_flags, total_size);
```
固定源码966–970行（行号为定位辅助）。

### EL26-A014 · 真实调用与BAD_ADDR消费
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		error = elf_load(bprm->file, load_bias + vaddr,
					elf_ppnt, elf_prot, elf_flags, total_size);
		if (BAD_ADDR(error)) {
			retval = IS_ERR_VALUE((void *)error) ?
				PTR_ERR((void*)error) : -EINVAL;
```
固定源码969–973行（行号为定位辅助）。

### EL26-A015 · 首次成功后标志/偏移更新
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (first_pt_load) {
			first_pt_load = 0;
			if (elf_ex->e_type == ET_DYN) {
				load_bias += error - ELF_PAGESTART(load_bias + vaddr);
				reloc_func_desc = load_bias;
```
固定源码977–981行（行号为定位辅助）。

### EL26-A016 · phdr起点条件计算
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_offset <= elf_ex->e_phoff &&
		    elf_ex->e_phoff < elf_ppnt->p_offset + elf_ppnt->p_filesz) {
			phdr_addr = elf_ex->e_phoff - elf_ppnt->p_offset +
							elf_ppnt->p_vaddr;
		}
```
固定源码989–993行（行号为定位辅助）。

### EL26-A017 · 映射后文件大小与地址安全条件
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (BAD_ADDR(k) || elf_ppnt->p_filesz > elf_ppnt->p_memsz ||
		    elf_ppnt->p_memsz > TASK_SIZE ||
		    TASK_SIZE - elf_ppnt->p_memsz < k) {
			/* set_brk can never work. Avoid overflows. */
			retval = -EINVAL;
```
固定源码1006–1010行（行号为定位辅助）。

### EL26-A018 · end_data和elf_brk候选上界
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (end_data < k)
			end_data = k;
		k = elf_ppnt->p_vaddr + elf_ppnt->p_memsz;
		if (k > elf_brk)
			elf_brk = k;
```
固定源码1018–1022行（行号为定位辅助）。
### EL26-A019 · 调用者末尾错误出口仅显式释放头部对象
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* error cleanup */
out_free_dentry:
	kfree(interp_elf_ex);
	kfree(interp_elf_phdata);
out_free_file:
```
固定源码1136–1140行（行号为定位辅助）。

### EL26-A020 · 页地址掩码和对齐算式
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_PAGESTART]
```c
#define ELF_PAGESTART(_v)	((_v) & ~(unsigned long)(ELF_MIN_ALIGN - 1))
#define ELF_PAGEOFFSET(_v)	((_v) & (ELF_MIN_ALIGN - 1))
#define ELF_PAGEALIGN(_v)	(((_v) + ELF_MIN_ALIGN - 1) & ~(ELF_MIN_ALIGN - 1))
```
固定源码79–81行（行号为定位辅助）。

### EL26-A021 · ELF对齐单位取PAGE_SIZE
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_MIN_ALIGN]
```c
// #if ELF_EXEC_PAGESIZE > PAGE_SIZE
// #	define ELF_MIN_ALIGN	ELF_EXEC_PAGESIZE
// #else
#  define ELF_MIN_ALIGN		PAGE_SIZE
// #endif
```
固定源码69–73行（行号为定位辅助）。

### EL26-A022 · 地址大于等于TASK_SIZE即BAD_ADDR
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::BAD_ADDR]
```c
#define BAD_ADDR(x) (unlikely((unsigned long)(x) >= TASK_SIZE))
```
固定源码91–91行（行号为定位辅助）。

### EL26-A023 · 页尾偏移及清零长度
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::padzero]
```c
padzero(ulong elf_bss) {
	ulong nbyte;
	nbyte = ELF_PAGEOFFSET(elf_bss);
	if (nbyte) {
		nbyte = ELF_MIN_ALIGN - nbyte;
```
固定源码99–103行（行号为定位辅助）。

### EL26-A024 · clear_user结果及-EFAULT
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::padzero]
```c
		nbyte = ELF_MIN_ALIGN - nbyte;
		if (clear_user((void __user *) elf_bss, nbyte))
			return -EFAULT;
	}
	return 0;
```
固定源码103–107行（行号为定位辅助）。

### EL26-A025 · size、off、地址及长度页对齐
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
	ulong map_addr;
	ulong size = eppnt->p_filesz + ELF_PAGEOFFSET(eppnt->p_vaddr);
	ulong off = eppnt->p_offset - ELF_PAGEOFFSET(eppnt->p_vaddr);
	addr = ELF_PAGESTART(addr);
	size = ELF_PAGEALIGN(size);
```
固定源码322–326行（行号为定位辅助）。
### EL26-A026 · 零长度早返，未执行vm_mmap
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
	/* mmap() will return -EINVAL if given a zero size, but a
	 * segment with zero filesize is perfectly valid */
	if (!size)
		return addr;
```
固定源码328–331行（行号为定位辅助）。
### EL26-A027 · 整体预留及未消费的裁剪调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
	if (total_size) {
		total_size = ELF_PAGEALIGN(total_size);
		map_addr = vm_mmap(filep, addr, total_size, prot, type, off);
		if (!BAD_ADDR(map_addr))
			vm_munmap(map_addr+size, total_size-size);
```
固定源码341–345行（行号为定位辅助）。

### EL26-A028 · 裁剪只由非BAD_ADDR触发；普通映射
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
		if (!BAD_ADDR(map_addr))
			vm_munmap(map_addr+size, total_size-size);
	} else
		map_addr = vm_mmap(filep, addr, size, prot, type, off);
```
固定源码344–347行（行号为定位辅助）。

### EL26-A029 · MAP_FIXED_NOREPLACE冲突日志
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
	if ((type & MAP_FIXED_NOREPLACE) &&
			PTR_ERR((void *)map_addr) == -EEXIST)
		pr_info("%d (%s): Uhuuh, elf segment at %px"
			" requested but the memory is mapped already\n",
			task_pid_nr(current), current->comm, (void *)addr);
```
固定源码349–353行（行号为定位辅助）。

### EL26-A030 · 日志后仍原样返回map_addr
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
			task_pid_nr(current), current->comm, (void *)addr);

	return(map_addr);
```
固定源码353–355行（行号为定位辅助）。

### EL26-A031 · elf_load签名与尚未赋值的局部
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
static ulong
elf_load(file_s *filep, ulong addr, const elf_phdr_s *eppnt,
		int prot, int type, ulong total_size) {

	ulong zero_start, zero_end;
```
固定源码363–367行（行号为定位辅助）。

### EL26-A032 · 有文件字节时elf_map调用与BAD_ADDR门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
	if (eppnt->p_filesz) {
		map_addr = elf_map(filep, addr, eppnt, prot, type, total_size);
		if (BAD_ADDR(map_addr))
			return map_addr;
		if (eppnt->p_memsz > eppnt->p_filesz) {
```
固定源码370–374行（行号为定位辅助）。

### EL26-A033 · 文件末尾后zero_start/end生产
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
		if (eppnt->p_memsz > eppnt->p_filesz) {
			zero_start = map_addr + eppnt->p_filesz +
							ELF_PAGEOFFSET(eppnt->p_vaddr);
			zero_end = map_addr + eppnt->p_memsz +
							ELF_PAGEOFFSET(eppnt->p_vaddr);
```
固定源码374–378行（行号为定位辅助）。

### EL26-A034 · padzero先执行后检查prot_WRITE
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
			/*
			 * Zero the end of the last mapped page but ignore
			 * any errors if the segment isn't writable.
			 */
			if (padzero(zero_start) && (prot & PROT_WRITE))
```
固定源码380–384行（行号为定位辅助）。

### EL26-A035 · p_filesz为0时纯内存分支
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
	} else {
		map_addr = zero_start = ELF_PAGESTART(addr);
		zero_end = zero_start + ELF_PAGEOFFSET(eppnt->p_vaddr) +
			eppnt->p_memsz;
	}
```
固定源码387–391行（行号为定位辅助）。

### EL26-A036 · 匿名扩展门和变量声明
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
	if (eppnt->p_memsz > eppnt->p_filesz) {
		/*
		 * Map the last of the segment.
		 * If the header is requesting these pages to be
		 * executable, honour that (ppc32 needs this).
```
固定源码392–396行（行号为定位辅助）。

### EL26-A037 · 向上页对齐及vm_brk_flags调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
		zero_start = ELF_PAGEALIGN(zero_start);
		zero_end = ELF_PAGEALIGN(zero_end);

		error = vm_brk_flags(zero_start, zero_end - zero_start,
						prot & PROT_EXEC ? VM_EXEC : 0);
```
固定源码400–404行（行号为定位辅助）。

### EL26-A038 · 匿名扩展返回转map_addr及最终返回
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
		error = vm_brk_flags(zero_start, zero_end - zero_start,
						prot & PROT_EXEC ? VM_EXEC : 0);
		if (error)
			map_addr = error;
	}
```
固定源码403–407行（行号为定位辅助）。

### EL26-A039 · 总长度函数的min/max初值
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::total_mapping_size]
```c
static ulong
total_mapping_size(const elf_phdr_s *phdr, int nr) {
	elf_addr_t min_addr = -1;
	elf_addr_t max_addr = 0;
	bool pt_load = false;
```
固定源码411–415行（行号为定位辅助）。

### EL26-A040 · PT_LOAD跨度起点与尾界
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::total_mapping_size]
```c
	for (int i = 0; i < nr; i++) {
		if (phdr[i].p_type == PT_LOAD) {
			min_addr = min(min_addr, ELF_PAGESTART(phdr[i].p_vaddr));
			max_addr = max(max_addr, phdr[i].p_vaddr + phdr[i].p_memsz);
			pt_load = true;
```
固定源码417–421行（行号为定位辅助）。

### EL26-A041 · PT_LOAD存在及max-min计算
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::total_mapping_size]
```c
			pt_load = true;
		}
	}
	return pt_load ? (max_addr - min_addr) : 0;
}
```
固定源码421–425行（行号为定位辅助）。

### EL26-A042 · 根据PF_R/PF_W构造权限
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::make_prot]
```c
	int prot = 0;
	if (p_flags & PF_R)
		prot |= PROT_READ;
	if (p_flags & PF_W)
		prot |= PROT_WRITE;
```
固定源码521–525行（行号为定位辅助）。
### EL26-A043 · PROT_EXEC及arch_adjust_prot
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::make_prot]
```c
		prot |= PROT_WRITE;
	if (p_flags & PF_X)
		prot |= PROT_EXEC;

	return arch_elf_adjust_prot(prot,
```
固定源码525–529行（行号为定位辅助）。

### EL26-A044 · 无CONFIG_ARCH_HAVE_ELF_PROT时直接保留prot
[VERIFIED mykernel/include/linux/kernel/elf.h::arch_elf_adjust_prot]
```c
	#else
		static inline int
		arch_elf_adjust_prot(int prot, const struct arch_elf_state *state,
				bool has_interp, bool is_interp) {
			return prot;
```
固定源码85–89行（行号为定位辅助）。
### EL26-A045 · 文件偏移溢出与页内偏移检查
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap]
```c
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;
```
固定源码104–108行（行号为定位辅助）。
### EL26-A046 · offset_in_page校验和off转页号
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap]
```c
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;

	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
```
固定源码106–110行（行号为定位辅助）。
### EL26-A047 · mmap包装调用do_mmap及返回
[VERIFIED mykernel/mm/vm_map/util.c::vm_mmap_pgoff]
```c
		return -EINTR;
	ret = do_mmap(file, addr, len, prot, flag, 0, pgoff);
	mmap_write_unlock(mm);

	return ret;
```
固定源码95–99行（行号为定位辅助）。
### EL26-A048 · 锁封装活动语句直接返回0
[VERIFIED mykernel/include/linux/kernel/mmap_lock.h::mmap_write_lock_killable]
```c
		// __mmap_lock_trace_start_locking(mm, true);
		// ret = down_write_killable(&mm->mmap_lock);
		// __mmap_lock_trace_acquire_returned(mm, true, ret == 0);
		// return ret;
		return 0;
```
固定源码122–126行（行号为定位辅助）。
### EL26-A049 · do_mmap拒绝零长度
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	if (len == 0)
		return -EINVAL;
```
固定源码476–477行（行号为定位辅助）。

### EL26-A050 · PROT_READ提升执行与NOREPLACE转换
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
			prot |= PROT_EXEC;

	/* force arch specific MAP_FIXED handling in get_unmapped_area */
	if (flags & MAP_FIXED_NOREPLACE)
		flags |= MAP_FIXED;
```
固定源码488–492行（行号为定位辅助）。
### EL26-A051 · do_mmap长度及页号加法检查
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		return -ENOMEM;

	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
		return -EOVERFLOW;
```
固定源码500–504行（行号为定位辅助）。
### EL26-A052 · 编码错误值和NOREPLACE冲突门
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		return addr;

	if (flags & MAP_FIXED_NOREPLACE)
		if (find_vma_intersection(mm, addr, addr + len))
			return -EEXIST;
```
固定源码524–528行（行号为定位辅助）。
### EL26-A053 · 文件私有映射读权限与noexec门
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
		case MAP_PRIVATE:
			if (!(file->f_mode & FMODE_READ))
				return -EACCES;
			if (path_noexec(&file->f_path)) {
				if (vm_flags & VM_EXEC)
```
固定源码573–577行（行号为定位辅助）。
### EL26-A054 · do_mmap向simple_mmap_region交付
[VERIFIED mykernel/mm/vm_map/mmap.c::do_mmap]
```c
	addr = simple_mmap_region(file, addr, len, vm_flags, pgoff);

	return addr;
```
固定源码643–645行（行号为定位辅助）。

### EL26-A055 · 文件映射前可能处置既有VMA
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	/* Clear old maps, set up prev */
	if (simple_munmap_vma_range(mm, addr, len, &prev))
		return -ENOMEM;

```
固定源码662–665行（行号为定位辅助）。
### EL26-A056 · 文件VMA到call_mmap绑定
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
		if (error != -ENOERR)
			goto unmap_and_free_vma;
```
固定源码691–695行（行号为定位辅助）。

### EL26-A057 · 直接VMA节点发布及其返回未检查
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
		vma_set_anonymous(vma);
	}

	simple_vma_link(mm, vma, prev);
```
固定源码743–746行（行号为定位辅助）。

### EL26-A058 · simple_mmap_region的返回值是起点
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_mmap_region]
```c
	vma->vm_flags |= VM_SOFTDIRTY;
	// vma_set_page_prot(vma);

	return addr;
```
固定源码773–776行（行号为定位辅助）。

### EL26-A059 · vm_munmap宏绑定__vm_munmap
[VERIFIED mykernel/mm/vm_map/vm_map_api.h::vm_munmap]
```c
	extern int __must_check vm_brk_flags(ulong, ulong, ulong);
	// extern int vm_munmap(ulong, size_t);
	extern int __vm_munmap(ulong start, size_t len);
	#define vm_munmap __vm_munmap
	extern ulong __must_check vm_mmap(file_s *,
```
固定源码48–52行（行号为定位辅助）。

### EL26-A060 · munmap绑定的实际内部调用与返回
[VERIFIED mykernel/mm/vm_map/mmap.c::__vm_munmap]
```c
	ret = simple_do_vma_munmap(mm, start, start + len);
	// if (ret || !unlock)
	// 	mmap_write_unlock(mm);

	return ret;
```
固定源码1178–1182行（行号为定位辅助）。
### EL26-A061 · 实际查询交集且NULL早返0
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap]
```c

	/* Find the first overlapping VMA where start < vma->vm_end */
	vma = find_vma_intersection(mm, start, end);
	if (vma == NULL)
		goto end;
```
固定源码1129–1133行（行号为定位辅助）。
### EL26-A062 · 拆分后VMA摘链与返回
[VERIFIED mykernel/mm/vm_map/mmap.c::simple_do_vma_munmap]
```c
	/* Detach vmas from vma list */
	vma_s *tmp;
	while ((tmp = vma_next_vma(prev)) != NULL && tmp->vm_end <= end) {
		BUG_ON(tmp == last);
		__vma_unlink_from_list(mm, tmp);
```
固定源码1154–1158行（行号为定位辅助）。

### EL26-A063 · 匿名请求页对齐及长度0时返回0
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_brk_flags]
```c
	len = PAGE_ALIGN(request);
	if (len < request)
		return -ENOMEM;
	if (!len)
		return 0;
```
固定源码1256–1260行（行号为定位辅助）。
### EL26-A064 · 匿名扩展包装的调用和返回处理
[VERIFIED mykernel/mm/vm_map/mmap.c::vm_brk_flags]
```c
	// 	return -EINTR;

	ret = do_brk_flags(addr, len, flags);
	// mmap_write_unlock(mm);
	return ret;
```
固定源码1263–1267行（行号为定位辅助）。
### EL26-A065 · 允许执行位并加入默认VMA权限
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	/* Until we need other flags, refuse anything except VM_EXEC. */
	if ((flags & (~VM_EXEC)) != 0)
		return -EINVAL;
	flags |= VM_DATA_DEFAULT_FLAGS | VM_ACCOUNT | mm->def_flags;

```
固定源码1202–1206行（行号为定位辅助）。

### EL26-A066 · 匿名映射合并后可直接跳出
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	/* Can we just expand an old private anonymous mapping? */
	vma = simple_vma_merge(mm, prev, addr, addr + len, flags, NULL, pgoff);
	if (vma)
		goto out;

```
固定源码1218–1222行（行号为定位辅助）。
### EL26-A067 · 新匿名VMA的范围与页保护字段
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_pgoff = pgoff;
	vma->vm_flags = flags;
	vma->vm_page_prot = vm_get_page_prot(flags);
```
固定源码1233–1237行（行号为定位辅助）。
### EL26-A068 · simple_vma_link返回值未检查
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	vma->vm_pgoff = pgoff;
	vma->vm_flags = flags;
	vma->vm_page_prot = vm_get_page_prot(flags);
	simple_vma_link(mm, vma, prev);
out:
```
固定源码1235–1239行（行号为定位辅助）。
### EL26-A069 · 匿名映射相关mm计数更新
[VERIFIED mykernel/mm/vm_map/mmap.c::do_brk_flags]
```c
	mm->total_vm += len >> PAGE_SHIFT;
	mm->data_vm += len >> PAGE_SHIFT;
	if (flags & VM_LOCKED)
		mm->locked_vm += (len >> PAGE_SHIFT);
	vma->vm_flags |= VM_SOFTDIRTY;
```
固定源码1241–1245行（行号为定位辅助）。
### EL26-A070 · 默认匿名data标志包括VM_EXEC
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_DATA_DEFAULT_FLAGS]
```c
	#define VM_DATA_FLAGS_EXEC (						\
				VM_READ | VM_WRITE | VM_EXEC |			\
				VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC	\
			)

```
固定源码204–208行（行号为定位辅助）。

### EL26-A071 · 访问地址数字门而非页权限保证
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__access_ok]
```c
	static inline bool
    __access_ok(const void __user *ptr, ulong size) {
		if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
			return valid_user_address(ptr);
		} else {
```
固定源码83–87行（行号为定位辅助）。

### EL26-A072 · 汇编选择与异常表入口，非执行认证
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__clear_user]
```c
							"call rep_stos_alternative",
							ALT_NOT(X86_FEATURE_FSRS)
						)
						"2:\n"
						_ASM_EXTABLE_UA(1b, 2b)
```
固定源码175–179行（行号为定位辅助）。
### EL26-A073 · __clear_user返回剩余size
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::__clear_user]
```c

		clac();
		return size;
	}
```
固定源码185–188行（行号为定位辅助）。

### EL26-A074 · clear_user通过__access_ok后调用
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess_64.h::clear_user]
```c
	static __always_inline ulong
	clear_user(void __user *to, ulong n) {
		if (__access_ok(to, n))
			return __clear_user(to, n);
		return n;
```
固定源码190–194行（行号为定位辅助）。

### EL26-A075 · 负错误的无符号区间判别
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#define MAX_ERRNO	4095

	#ifndef __ASSEMBLY__
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
```
固定源码15–19行（行号为定位辅助）。

### EL26-A076 · 指针编码错误转long
[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```
固定源码31–34行（行号为定位辅助）。

### EL26-A077 · ELF64 VADDR、filesz、memsz、align类型
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Addr	p_vaddr;	/* Segment virtual address */
		Elf64_Addr	p_paddr;	/* Segment physical address */
		Elf64_Xword	p_filesz;	/* Segment size in file */
		Elf64_Xword	p_memsz;	/* Segment size in memory */
		Elf64_Xword	p_align;	/* Segment alignment, file & memory */
```
固定源码273–277行（行号为定位辅助）。
### EL26-A078 · 文件偏移和长度类型为__u64
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Xword]
```c
	typedef __u64		Elf64_Off;
	typedef __s32		Elf64_Sword;
	typedef __u32		Elf64_Word;
	typedef __u64		Elf64_Xword;
	typedef __s64		Elf64_Sxword;
```
固定源码19–23行（行号为定位辅助）。
### EL26-A079 · PF_R/PF_W/PF_X权限常量
[VERIFIED mykernel/include/uapi/linux/elf.h::PF_W]
```c
	/* These constants define the permissions on sections in the program
	header, p_flags. */
	#define PF_R	0x4
	#define PF_W	0x2
	#define PF_X	0x1
```
固定源码250–254行（行号为定位辅助）。

### EL26-A080 · ulong为unsigned long
[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned short		ushort;
		typedef unsigned int		uint;
		typedef unsigned long		ulong;
		typedef unsigned long long	ulonglong;
```
固定源码103–106行（行号为定位辅助）。

### EL26-A081 · x86页大小2^12和页掩码
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```
固定源码8–11行（行号为定位辅助）。
### EL26-A082 · PROT_READ/WRITE/EXEC位值
[VERIFIED mykernel/include/uapi/asm-generic/mman-common.h::PROT_WRITE]
```c

	#define PROT_READ	0x1		/* page can be read */
	#define PROT_WRITE	0x2		/* page can be written */
	#define PROT_EXEC	0x4		/* page can be executed */
```
固定源码9–12行（行号为定位辅助）。
### EL26-A083 · 文件mmap绑定vm_ops并返回0
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap]
```c
	// if (!mapping->a_ops->read_folio)
	// 	return -ENOEXEC;
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
```
固定源码415–419行（行号为定位辅助）。
### EL26-A084 · 文件缺页read返回未消费
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_fault]
```c

	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
	vmf->page = *pgcache_ptr;

	return 0;
```
固定源码131–135行（行号为定位辅助）。
### EL26-A085 · 无解释器时elf_entry取主程序入口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
			retval = -EINVAL;
			goto out_free_dentry;
```
固定源码1057–1061行（行号为定位辅助）。
### EL26-A086 · 数据段和brk字段更新
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	end_code += load_bias;
	start_data += load_bias;
	end_data += load_bias;

	current->mm->start_brk = current->mm->brk = ELF_PAGEALIGN(elf_brk);
```
固定源码1029–1033行（行号为定位辅助）。
## 检查范围及限制
- 本轮读取了上述14份固定源文件，另核配置`mykernel/scripts/options_flags.cmake`和金丝雀`mykernel/debug/panic.c`；对`elf_map`、`elf_load`、`padzero`、`total_mapping_size`、`make_prot`及`load_elf_binary`主PT_LOAD循环按完整函数/所选分支分析。
- 相同源片段可能在本文件由不同ID重复引用，计数是逐字引文数而不是相互独立的结论数。引文所在的注释语句不充当活动调用；所有负面判断严格限定已读函数与当前宏组合。
- `mmap_write_lock_killable`在本固定实现返回0，实际锁获取代码被注释；`vm_munmap`、`do_brk_flags`、`clear_user`下层语义不能从名字推定全部副作用成功。
- `PADZERO`的清尾与匿名扩展的`vm_brk_flags`不同，`memsz>filesz`并不自动证明访问目标有有效VMA或按预期清零。
- 05和07回执、13/14/19/20/21及25主线相关限定是历史证据约束，不自动把旧作者观察写成这批重新验证。
- 无仓库脚本、源码C/ASM、预处理、编译、QEMU、硬件、外部libc或现场地址/页内容实验；运行证据NOT_RUN。
