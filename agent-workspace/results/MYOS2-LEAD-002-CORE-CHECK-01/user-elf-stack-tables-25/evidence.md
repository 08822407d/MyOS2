---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-ELF-STACK-TABLES-25
record_type: fixed_source_quote_register
source: "time @ a039d9803ade"
read_channel: connector
runtime_evidence: NOT_RUN
---
# 第25批逐字源码证据
引文均取固定time源码，而不是默认分支搜索摘录。每段至多5行；各项只证明固定代码片段，不证明该代码能完成实际进程启动。

### ET25-A01 · arg_start
[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
```
固定源码行480–481；直接引文。

### ET25-A02 · argc/envc
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	bprm->argc = retval;

	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
```
固定源码行1328–1332；直接引文。

### ET25-A03 · exec字符串位置
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```
固定源码行1339–1342；直接引文。

### ET25-A04 · 复制env、argv
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;

	retval = copy_strings(bprm->argc, argv, bprm);
```
固定源码行1344–1348；直接引文。

### ET25-A05 · begin_new_exec
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```
固定源码行798–801；直接引文。

### ET25-A06 · setup_arg_pages
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = setup_arg_pages(bprm,
				randomize_stack_top(STACK_TOP), executable_stack);
	if (retval < 0)
		goto out_free_dentry;
```
固定源码行817–820；直接引文。

### ET25-A07 · phdr_addr
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_offset <= elf_ex->e_phoff &&
		    elf_ex->e_phoff < elf_ppnt->p_offset + elf_ppnt->p_filesz) {
			phdr_addr = elf_ex->e_phoff - elf_ppnt->p_offset +
							elf_ppnt->p_vaddr;
		}
```
固定源码行989–993；直接引文。

### ET25-A08 · e_entry
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	e_entry = elf_ex->e_entry + load_bias;
	phdr_addr += load_bias;
	elf_brk += load_bias;
```
固定源码行1025–1027；直接引文。

### ET25-A09 · interp入口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
```
固定源码行1043–1044；直接引文。

### ET25-A10 · 无interp入口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
```
固定源码行1057–1059；直接引文。

### ET25-A11 · 真实表构造调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```
固定源码行1075–1078；直接引文。

### ET25-A12 · start_stack
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	mm->start_code = start_code;
	mm->start_data = start_data;
	mm->end_data = end_data;
	mm->start_stack = bprm->p;
```
固定源码行1082–1085；直接引文。

### ET25-A13 · 最终交接
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
```
固定源码行1130–1132；直接引文。

### ET25-A14 · 局部p/argc/envc
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm_s *mm = current->mm;
	ulong p = bprm->p;
	int argc = bprm->argc;
	int envc = bprm->envc;
	elf_addr_t __user *sp = (void *)p;
```
固定源码行129–133；直接引文。

### ET25-A15 · vma未初始化
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_addr_t *elf_info;
	elf_addr_t flags = 0;
	int ei_index;
	// const struct cred *cred = current_cred();
	vma_s *vma;
```
固定源码行141–145；直接引文。

### ET25-A16 · 向下16字节对齐
[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::arch_align_stack]
```c
	// if (!(current->personality & ADDR_NO_RANDOMIZE) &&
	// 		randomize_va_space)
	// 	sp -= get_random_u32_below(8192);
	return sp & ~0xf;
```
固定源码行170–173；直接引文。

### ET25-A17 · 平台字符串
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_PLATFORM]
```c
	#define ELF_PLATFORM ("x86_64")
```
固定源码行151–151；直接引文。

### ET25-A18 · ELF页大小
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_EXEC_PAGESIZE]
```c
	#define ELF_EXEC_PAGESIZE	4096
```
固定源码行156–156；直接引文。

### ET25-A19 · 按元素减法
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ADD]
```c
#define STACK_ADD(sp, items)	\
			((elf_addr_t __user *)(sp) - (items))
```
固定源码行110–111；直接引文。

### ET25-A20 · 16字节栈表对齐
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ROUND]
```c
#define STACK_ROUND(sp, items)	\
			(((unsigned long) (sp - items)) &~ 15UL)
```
固定源码行112–113；直接引文。

### ET25-A21 · 平台字节预留
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ALLOC]
```c
#define STACK_ALLOC(sp, len) (sp -= len)
```
固定源码行114–114；直接引文。

### ET25-A22 · 平台写入
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (k_platform) {
		size_t len = strlen(k_platform) + 1;

		u_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
		if (copy_to_user(u_platform, k_platform, len))
```
固定源码行161–165；直接引文。

### ET25-A23 · base平台写入
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (k_base_platform) {
		size_t len = strlen(k_base_platform) + 1;

		u_base_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
		if (copy_to_user(u_base_platform, k_base_platform, len))
```
固定源码行174–178；直接引文。

### ET25-A24 · 辅助向量双字条目
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::NEW_AUX_ENT]
```c
#define NEW_AUX_ENT(id, val)	\
		do {					\
			*elf_info++ = id;	\
			*elf_info++ = val;	\
		} while (0)
```
固定源码行194–198；直接引文。

### ET25-A25 · 五个基础aux条目
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_HWCAP, ELF_HWCAP);
	NEW_AUX_ENT(AT_PAGESZ, ELF_EXEC_PAGESIZE);
	NEW_AUX_ENT(AT_CLKTCK, CLOCKS_PER_SEC);
	NEW_AUX_ENT(AT_PHDR, phdr_addr);
	NEW_AUX_ENT(AT_PHENT, sizeof(struct elf_phdr));
```
固定源码行209–213；直接引文。

### ET25-A26 · BASE/FLAGS/ENTRY
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_BASE, interp_load_addr);
	if (bprm->interp_flags & BINPRM_FLAGS_PRESERVE_ARGV0)
		flags |= AT_FLAGS_PRESERVE_ARGV0;
	NEW_AUX_ENT(AT_FLAGS, flags);
	NEW_AUX_ENT(AT_ENTRY, e_entry);
```
固定源码行215–219；直接引文。

### ET25-A27 · 注释身份/RANDOM项
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// NEW_AUX_ENT(AT_EUID, from_kuid_munged(cred->user_ns, cred->euid));
	// NEW_AUX_ENT(AT_GID, from_kgid_munged(cred->user_ns, cred->gid));
	// NEW_AUX_ENT(AT_EGID, from_kgid_munged(cred->user_ns, cred->egid));
	NEW_AUX_ENT(AT_SECURE, bprm->secureexec);
	// NEW_AUX_ENT(AT_RANDOM, (elf_addr_t)(ulong)u_rand_bytes);
```
固定源码行221–225；直接引文。

### ET25-A28 · HWCAP2条件及EXECFN
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
#ifdef ELF_HWCAP2
	NEW_AUX_ENT(AT_HWCAP2, ELF_HWCAP2);
#endif
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```
固定源码行226–229；直接引文。

### ET25-A29 · AT_PLATFORM
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (k_platform) {
		NEW_AUX_ENT(AT_PLATFORM,
			    (elf_addr_t)(ulong)u_platform);
	}
```
固定源码行230–233；直接引文。

### ET25-A30 · AT_BASE_PLATFORM/AT_EXECFD
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		NEW_AUX_ENT(AT_BASE_PLATFORM,
			    (elf_addr_t)(ulong)u_base_platform);
	}
	if (bprm->have_execfd) {
		NEW_AUX_ENT(AT_EXECFD, bprm->execfd);
```
固定源码行235–239。
### ET25-A31 · CONFIG_RSEQ条件
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
#ifdef CONFIG_RSEQ
	NEW_AUX_ENT(AT_RSEQ_FEATURE_SIZE, offsetof(struct rseq, end));
	NEW_AUX_ENT(AT_RSEQ_ALIGN, __alignof__(struct rseq));
#endif
```
固定源码行241–244；直接引文。

### ET25-A32 · 内核缓冲尾清零
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* AT_NULL is zero; clear the rest too */
	memset(elf_info, 0, (char *)mm->saved_auxv +
			sizeof(mm->saved_auxv) - (char *)elf_info);

```
固定源码行246–249；直接引文。

### ET25-A33 · AT_NULL两字计数
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* And advance past the AT_NULL entry.  */
	elf_info += 2;

	ei_index = elf_info - (elf_addr_t *)mm->saved_auxv;
	sp = STACK_ADD(p, ei_index);
```
固定源码行250–254；直接引文。

### ET25-A34 · items、bprm.p
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	items = (argc + 1) + (envc + 1) + 1;
	bprm->p = STACK_ROUND(sp, items);

	/* Point sp at the lowest address on the stack */
	sp = (elf_addr_t __user *)bprm->p;
```
固定源码行256–260；直接引文。

### ET25-A35 · VMA查找注释
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
	// vma = find_extend_vma_locked(mm, bprm->p);
	mmap_write_unlock(mm);
	if (!vma)
```
固定源码行266–270；直接引文。

### ET25-A36 · vma使用和argc
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mmap_write_unlock(mm);
	if (!vma)
		return -EFAULT;

	/* Now, let's put argc (and argv, envp if appropriate) on the stack */
```
固定源码行269–273；直接引文。

### ET25-A37 · argv槽
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```
固定源码行278–282；直接引文。

### ET25-A38 · argv长度
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
	}
```
固定源码行283–287；直接引文。

### ET25-A39 · env槽
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* Populate list of envp pointers back to envp strings. */
	mm->env_end = mm->env_start = p;
	while (envc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
```
固定源码行292–296；直接引文。

### ET25-A40 · env长度
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
```
固定源码行296–300；直接引文。

### ET25-A41 · env边界
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		p += len;
	}
	if (put_user(0, sp++))
		return -EFAULT;
	mm->env_end = p;
```
固定源码行301–305；直接引文。

### ET25-A42 · aux复制和返回
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* Put the elf_info on the stack in the right place.  */
	if (copy_to_user(sp, mm->saved_auxv, ei_index * sizeof(elf_addr_t)))
		return -EFAULT;
	return 0;
```
固定源码行307–310；直接引文。

### ET25-A43 · saved_auxv字段
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			ulong			start_code, end_code, start_data, end_data;
			ulong			start_brk, brk, start_stack;
			ulong			arg_start, arg_end, env_start, env_end;

			ulong			saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */
```
固定源码行99–103；直接引文。

### ET25-A44 · aux容量
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE]
```c
	#define AT_VECTOR_SIZE_BASE		22 /* NEW_AUX_ENT entries in auxiliary table */
	/* number of "#define AT_.*" above, minus {AT_NULL, AT_IGNORE, AT_NOTELF} */

	#define AT_VECTOR_SIZE			(2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))
```
固定源码行9–12；直接引文。

### ET25-A45 · 类型别名
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_addr_t]
```c
	#define elfhdr		elf64_hdr
	#define elf_phdr	elf64_phdr
	#define elf_shdr	elf64_shdr
	#define elf_note	elf64_note
	#define elf_addr_t	Elf64_Off
```
固定源码行43–47；直接引文。

### ET25-A46 · 64位单位
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Off]
```c
	typedef __u64		Elf64_Addr;
	typedef __u16		Elf64_Half;
	typedef __s16		Elf64_SHalf;
	typedef __u64		Elf64_Off;
```
固定源码行16–19；直接引文。

### ET25-A47 · HWCAP来源
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_HWCAP]
```c
	#define ELF_HWCAP			(boot_cpu_data.x86_capability[CPUID_1_EDX])
```
固定源码行169–169；直接引文。

### ET25-A48 · CLOCKS_PER_SEC=250
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::CLOCKS_PER_SEC]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
	#define USER_HZ			250			/* some user interfaces are */
	#define CLOCKS_PER_SEC	(USER_HZ)	/* in "ticks" like times() */
```
固定源码行9–11；直接引文。

### ET25-A49 · bprm的mm/p
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
	typedef struct linux_binprm {
		vma_s			*vma;
		ulong			vma_pages;
		mm_s			*mm;
		ulong			p; /* current top of mem */
```
固定源码行17–21；直接引文。

### ET25-A50 · bprm的argc/envc/flags
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		int				argc, envc;
		const char		*filename;	/* Name of binary as seen by procps */
		const char		*interp;	/* Name of the binary really executed. Most
								   of the time same as filename, but could be
								   different for binfmt_{misc,script} */
```
固定源码行49–53；直接引文。

### ET25-A51 · START_THREAD宏
[VERIFIED mykernel/include/linux/kernel/elf.h::START_THREAD]
```c
	#ifndef START_THREAD
	#	define START_THREAD(elf_ex, regs, elf_entry, start_stack) \
					start_thread(regs, elf_entry, start_stack)
```
固定源码行28–30；直接引文。

### ET25-A52 · regs.sp赋值
[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread_common]
```c
	loadsegment(ds, _ds);
	load_gs_index(0);

	regs->ip		= new_ip;
	regs->sp		= new_sp;
```
固定源码行266–270；直接引文。

### ET25-A53 · start_thread调用
[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::start_thread]
```c
start_thread(pt_regs_s *regs, ulong new_ip, ulong new_sp)
{
	start_thread_common(regs, new_ip, new_sp, __USER_CS, __USER_DS, 0);
```
固定源码行302–304；直接引文。

### ET25-A54 · 直接赋值而非保护写入
[VERIFIED mykernel/arch/x86_64/include/asm/uaccess.h::put_user]
```c
	#define put_user(x, ptr) ({					\
				*(ptr) = (__typeof(*(ptr)))(x);	\
				-ENOERR;						\
			})
```
固定源码行191–194；直接引文。

### ET25-A55 · ENOERR=0
[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```
固定源码行5–5；直接引文。

### ET25-A56 · copy返回剩余
[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```
固定源码行176–180；直接引文。

### ET25-A57 · copy包装
[VERIFIED mykernel/include/linux/kernel/uaccess.h::copy_to_user]
```c
	copy_to_user(void __user *to, const void *from, ulong n) {
		// if (!check_copy_size(from, n, true))
		// 	return n;
	// #ifdef INLINE_COPY_TO_USER
		return _inline_copy_to_user(to, from, n);
```
固定源码行197–201；直接引文。

### ET25-A58 · strnlen+1
[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	// return 0;

	return strnlen(str, count) + 1;
```
固定源码行118–120；直接引文。

### ET25-A59 · 参数长度上限
[VERIFIED mykernel/include/uapi/linux/binfmts.h::MAX_ARG_STRLEN]
```c
	#define MAX_ARG_STRLEN		(PAGE_SIZE * 32)
```
固定源码行15–15；直接引文。

### ET25-A60 · ARGV0位
[VERIFIED mykernel/include/linux/fs/binfmts.h::BINPRM_FLAGS_PRESERVE_ARGV0]
```c
	/* preserve argv0 for the interpreter  */
	#define BINPRM_FLAGS_PRESERVE_ARGV0_BIT		3
	#define BINPRM_FLAGS_PRESERVE_ARGV0			(1 << BINPRM_FLAGS_PRESERVE_ARGV0_BIT)
```
固定源码行71–73；直接引文。

### ET25-A61 · 随机字节逻辑注释
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// get_random_bytes(k_rand_bytes, sizeof(k_rand_bytes));
	// u_rand_bytes = (elf_addr_t __user *)
	// 	       STACK_ALLOC(p, sizeof(k_rand_bytes));
	// if (copy_to_user(u_rand_bytes, k_rand_bytes, sizeof(k_rand_bytes)))
	// 	return -EFAULT;
```
固定源码行185–189；直接引文。

### ET25-A62 · base平台缺省NULL
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_BASE_PLATFORM]
```c
 * If the arch defines ELF_BASE_PLATFORM (in asm/elf.h), the value
 * will be copied to the user stack in the same manner as AT_PLATFORM.
 */
#  define ELF_BASE_PLATFORM	NULL
#endif
```
固定源码行119–123。
## 检查方式及限制
- 本轮定点读取固定time的16份直接相关源文件，摘录和源文件文本逐段一致；create_elf_tables(125–311)整函数已通读。默认分支搜索仅导航。
- vma局部没有活动赋值且后续被读取；不能为未定义行为后的状态制定确定机器结论。历史23/24页映射、返回值和引用限制保留。
- ELF_HWCAP2与ARCH_DLINFO在读取的arch头中只有注释定义；ELF_BASE_PLATFORM为本文件的NULL兜底；当前显式编译选项未设置CONFIG_RSEQ。这不是完整预处理认证。
- 没有运行代码、预处理、编译、模拟器、硬件、真实用户态、CPU寄存器或用户内存保护验证，均NOT_RUN。
