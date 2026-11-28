---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
batch: "CORE-USER-EXEC-AUXV-PAYLOAD-141"
executor: "MYOS2-LEAD-002-CHAT-EXEC-22"
date: "2026-11-24"
base_snapshot: "time@a039d9803ade"
phase: 2
produced_by: "GPT-6.2 system family; backend/tier/effort unknown"
model_per_owner: "GPT-6.2 Sol Extra/high requested; actual unknown"
evidence_class: "pinned source/static model/prior accepted result"
status: "AUTHOR_STATIC_CONTENT_PREPARED_PENDING_PUBLICATION_AND_G04"
mainline_acceptance: "PENDING"
full_local_P9: "NOT_RUN"
record_type: "evidence"
---
# 固定源码逐窗证据

1–5连续物理行；定义、引用、注释、条件和构建声明区分。仅静态文本。

### E141-001 STACK_ADD
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:110–111；macro
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ADD]
```c
#define STACK_ADD(sp, items)	\
			((elf_addr_t __user *)(sp) - (items))
```

### E141-002 STACK_ROUND
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:112–113；macro
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ROUND]
```c
#define STACK_ROUND(sp, items)	\
			(((unsigned long) (sp - items)) &~ 15UL)
```

### E141-003 STACK_ALLOC
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:114–114；macro
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::STACK_ALLOC]
```c
#define STACK_ALLOC(sp, len) (sp -= len)
```

### E141-004 ELF_BASE_PLATFORM
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:122–122；macro
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_BASE_PLATFORM]
```c
#  define ELF_BASE_PLATFORM	NULL
```

### E141-005 NEW_AUX_ENT
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:194–198；macro
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::NEW_AUX_ENT]
```c
#define NEW_AUX_ENT(id, val)	\
		do {					\
			*elf_info++ = id;	\
			*elf_info++ = val;	\
		} while (0)
```

### E141-006 signature
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:126–127；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
create_elf_tables(linux_bprm_s *bprm, const elfhdr_s *exec,
		ulong interp_load_addr, ulong e_entry, ulong phdr_addr) {
```

### E141-007 entry_init
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:129–133；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm_s *mm = current->mm;
	ulong p = bprm->p;
	int argc = bprm->argc;
	int envc = bprm->envc;
	elf_addr_t __user *sp = (void *)p;
```

### E141-008 platform_decl
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:134–138；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_addr_t __user *u_platform;
	elf_addr_t __user *u_base_platform;
	elf_addr_t __user *u_rand_bytes;
	const char *k_platform = ELF_PLATFORM;
	const char *k_base_platform = ELF_BASE_PLATFORM;
```

### E141-009 flags
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:141–143；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_addr_t *elf_info;
	elf_addr_t flags = 0;
	int ei_index;
```

### E141-010 vma_decl
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:145–145；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	vma_s *vma;
```

### E141-011 align
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:152–152；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = arch_align_stack(p);
```

### E141-012 platform_test
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:160–164；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	u_platform = NULL;
	if (k_platform) {
		size_t len = strlen(k_platform) + 1;

		u_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
```

### E141-013 platform_copy
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:165–167；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		if (copy_to_user(u_platform, k_platform, len))
			return -EFAULT;
	}
```

### E141-014 base_test
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:173–177；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	u_base_platform = NULL;
	if (k_base_platform) {
		size_t len = strlen(k_base_platform) + 1;

		u_base_platform = (elf_addr_t __user *)STACK_ALLOC(p, len);
```

### E141-015 base_copy
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:178–180；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		if (copy_to_user(u_base_platform, k_base_platform, len))
			return -EFAULT;
	}
```

### E141-016 random_comment
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:185–189；comment
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// get_random_bytes(k_rand_bytes, sizeof(k_rand_bytes));
	// u_rand_bytes = (elf_addr_t __user *)
	// 	       STACK_ALLOC(p, sizeof(k_rand_bytes));
	// if (copy_to_user(u_rand_bytes, k_rand_bytes, sizeof(k_rand_bytes)))
	// 	return -EFAULT;
```

### E141-017 saved_start
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:192–192；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	elf_info = (elf_addr_t *)mm->saved_auxv;
```

### E141-018 arch_gate
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:200–200；conditional
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
#ifdef ARCH_DLINFO
```

### E141-019 arch_add
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:207–208；conditional
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	ARCH_DLINFO;
#endif
```

### E141-020 hwcap
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:209–209；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_HWCAP, ELF_HWCAP);
```

### E141-021 pagesz
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:210–210；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PAGESZ, ELF_EXEC_PAGESIZE);
```

### E141-022 clktck
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:211–211；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_CLKTCK, CLOCKS_PER_SEC);
```

### E141-023 phdr
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:212–212；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PHDR, phdr_addr);
```

### E141-024 phent
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:213–213；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PHENT, sizeof(struct elf_phdr));
```

### E141-025 phnum
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:214–214；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PHNUM, exec->e_phnum);
```

### E141-026 base
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:215–215；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_BASE, interp_load_addr);
```

### E141-027 flags_test
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:216–217；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (bprm->interp_flags & BINPRM_FLAGS_PRESERVE_ARGV0)
		flags |= AT_FLAGS_PRESERVE_ARGV0;
```

### E141-028 flags_emit
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:218–218；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_FLAGS, flags);
```

### E141-029 entry
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:219–219；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_ENTRY, e_entry);
```

### E141-030 cred_comment
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:220–223；comment
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// NEW_AUX_ENT(AT_UID, from_kuid_munged(cred->user_ns, cred->uid));
	// NEW_AUX_ENT(AT_EUID, from_kuid_munged(cred->user_ns, cred->euid));
	// NEW_AUX_ENT(AT_GID, from_kgid_munged(cred->user_ns, cred->gid));
	// NEW_AUX_ENT(AT_EGID, from_kgid_munged(cred->user_ns, cred->egid));
```

### E141-031 secure
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:224–224；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_SECURE, bprm->secureexec);
```

### E141-032 rand_comment
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:225–225；comment
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// NEW_AUX_ENT(AT_RANDOM, (elf_addr_t)(ulong)u_rand_bytes);
```

### E141-033 hwcap2
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:226–228；conditional
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
#ifdef ELF_HWCAP2
	NEW_AUX_ENT(AT_HWCAP2, ELF_HWCAP2);
#endif
```

### E141-034 execfn
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:229–229；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```

### E141-035 platform
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:230–233；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (k_platform) {
		NEW_AUX_ENT(AT_PLATFORM,
			    (elf_addr_t)(ulong)u_platform);
	}
```

### E141-036 base_platform
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:234–237；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (k_base_platform) {
		NEW_AUX_ENT(AT_BASE_PLATFORM,
			    (elf_addr_t)(ulong)u_base_platform);
	}
```

### E141-037 execfd
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:238–240；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (bprm->have_execfd) {
		NEW_AUX_ENT(AT_EXECFD, bprm->execfd);
	}
```

### E141-038 rseq
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:241–244；conditional
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
#ifdef CONFIG_RSEQ
	NEW_AUX_ENT(AT_RSEQ_FEATURE_SIZE, offsetof(struct rseq, end));
	NEW_AUX_ENT(AT_RSEQ_ALIGN, __alignof__(struct rseq));
#endif
```

### E141-039 clear
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:246–248；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* AT_NULL is zero; clear the rest too */
	memset(elf_info, 0, (char *)mm->saved_auxv +
			sizeof(mm->saved_auxv) - (char *)elf_info);
```

### E141-040 advance
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:250–251；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* And advance past the AT_NULL entry.  */
	elf_info += 2;
```

### E141-041 ei
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:253–254；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	ei_index = elf_info - (elf_addr_t *)mm->saved_auxv;
	sp = STACK_ADD(p, ei_index);
```

### E141-042 items
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:256–260；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	items = (argc + 1) + (envc + 1) + 1;
	bprm->p = STACK_ROUND(sp, items);

	/* Point sp at the lowest address on the stack */
	sp = (elf_addr_t __user *)bprm->p;
```

### E141-043 lock
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:266–267；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
```

### E141-044 vma_gate
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:268–271；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	// vma = find_extend_vma_locked(mm, bprm->p);
	mmap_write_unlock(mm);
	if (!vma)
		return -EFAULT;
```

### E141-045 argc
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:274–275；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(argc, sp++))
		return -EFAULT;
```

### E141-046 argv
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:278–282；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

### E141-047 argv_end
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:288–290；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->arg_end = p;
```

### E141-048 envp
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:293–297；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm->env_end = mm->env_start = p;
	while (envc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

### E141-049 envp_end
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:303–305；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->env_end = p;
```

### E141-050 copy
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:307–310；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	/* Put the elf_info on the stack in the right place.  */
	if (copy_to_user(sp, mm->saved_auxv, ei_index * sizeof(elf_addr_t)))
		return -EFAULT;
	return 0;
```

### E141-051 interp_init
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:614–614；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	file_s		*interpreter = NULL; /* to shut gcc up */
```

### E141-052 phdr_init
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:615–617；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	ulong		load_addr,
				load_bias = 0,
				phdr_addr = 0;
```

### E141-053 entry_decl
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:627–629；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	ulong		elf_entry;
	ulong		e_entry;
	ulong		interp_load_addr = 0;
```

### E141-054 header
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:636–637；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	elfhdr_s	*elf_ex = (elfhdr_s *)bprm->buf;
	elfhdr_s	*interp_elf_ex = NULL;
```

### E141-055 ptload_gate
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:838–839；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_type != PT_LOAD)
			continue;
```

### E141-056 bias
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:977–981；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (first_pt_load) {
			first_pt_load = 0;
			if (elf_ex->e_type == ET_DYN) {
				load_bias += error - ELF_PAGESTART(load_bias + vaddr);
				reloc_func_desc = load_bias;
```

### E141-057 phdr_gate
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:989–993；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_offset <= elf_ex->e_phoff &&
		    elf_ex->e_phoff < elf_ppnt->p_offset + elf_ppnt->p_filesz) {
			phdr_addr = elf_ex->e_phoff - elf_ppnt->p_offset +
							elf_ppnt->p_vaddr;
		}
```

### E141-058 add_bias
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1025–1026；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	e_entry = elf_ex->e_entry + load_bias;
	phdr_addr += load_bias;
```

### E141-059 interp_call
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1035–1038；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
		if (!IS_ERR_VALUE(elf_entry)) {
```

### E141-060 interp_sum
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1043–1045；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
		}
```

### E141-061 interp_bad
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1046–1050；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (BAD_ADDR(elf_entry)) {
			retval = IS_ERR_VALUE(elf_entry) ?
					(int)elf_entry : -EINVAL;
			goto out_free_dentry;
		}
```

### E141-062 no_interp
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1057–1061；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
			retval = -EINVAL;
			goto out_free_dentry;
```

### E141-063 table_call
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1075–1078；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```

### E141-064 regs
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1115–1115；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	regs = current_pt_regs();
```

### E141-065 start
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:1130–1134；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```

### E141-066 interp_ret
固定time@a039d9803ade mykernel/fs/vfs/binfmt_elf.c:606–608；active
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	error = load_addr;
out:
	return error;
```

### E141-067 ELF_PLATFORM
固定time@a039d9803ade mykernel/arch/x86_64/include/asm/elf.h:151–151；macro
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_PLATFORM]
```c
	#define ELF_PLATFORM ("x86_64")
```

### E141-068 ELF_EXEC_PAGESIZE
固定time@a039d9803ade mykernel/arch/x86_64/include/asm/elf.h:156–156；macro
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_EXEC_PAGESIZE]
```c
	#define ELF_EXEC_PAGESIZE	4096
```

### E141-069 ELF_HWCAP
固定time@a039d9803ade mykernel/arch/x86_64/include/asm/elf.h:169–169；macro
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::ELF_HWCAP]
```c
	#define ELF_HWCAP			(boot_cpu_data.x86_capability[CPUID_1_EDX])
```

### E141-070 elf_addr_t
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:47–47；alias
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_addr_t]
```c
	#define elf_addr_t	Elf64_Off
```

### E141-071 elfhdr
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:43–43；alias
[VERIFIED mykernel/include/linux/kernel/elf.h::elfhdr]
```c
	#define elfhdr		elf64_hdr
```

### E141-072 elf_phdr
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:44–44；alias
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_phdr]
```c
	#define elf_phdr	elf64_phdr
```

### E141-073 elfhdr_s
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:52–52；alias
[VERIFIED mykernel/include/linux/kernel/elf.h::elfhdr_s]
```c
	typedef struct elfhdr		elfhdr_s;
```

### E141-074 elf_phdr_s
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:53–53；alias
[VERIFIED mykernel/include/linux/kernel/elf.h::elf_phdr_s]
```c
	typedef struct elf_phdr		elf_phdr_s;
```

### E141-075 START_THREAD
固定time@a039d9803ade mykernel/include/linux/kernel/elf.h:29–30；macro
[VERIFIED mykernel/include/linux/kernel/elf.h::START_THREAD]
```c
	#	define START_THREAD(elf_ex, regs, elf_entry, start_stack) \
					start_thread(regs, elf_entry, start_stack)
```

### E141-076 Elf64_Addr
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:16–16；type
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Addr]
```c
	typedef __u64		Elf64_Addr;
```

### E141-077 Elf64_Half
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:17–17；type
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Half]
```c
	typedef __u16		Elf64_Half;
```

### E141-078 Elf64_Off
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:19–19；type
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Off]
```c
	typedef __u64		Elf64_Off;
```

### E141-079 Elf64_Word
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:21–21；type
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Word]
```c
	typedef __u32		Elf64_Word;
```

### E141-080 Elf64_Xword
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:22–22；type
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Xword]
```c
	typedef __u64		Elf64_Xword;
```

### E141-081 ehdr_entry
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:238–240；member
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
		Elf64_Addr	e_entry;	/* Entry point virtual address */
		Elf64_Off	e_phoff;	/* Program header table file offset */
		Elf64_Off	e_shoff;	/* Section header table file offset */
```

### E141-082 ehdr_sizes
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:242–245；member
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
		Elf64_Half	e_ehsize;
		Elf64_Half	e_phentsize;
		Elf64_Half	e_phnum;
		Elf64_Half	e_shentsize;
```

### E141-083 phdr_first
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:270–274；member
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Word	p_type;
		Elf64_Word	p_flags;
		Elf64_Off	p_offset;	/* Segment file offset */
		Elf64_Addr	p_vaddr;	/* Segment virtual address */
		Elf64_Addr	p_paddr;	/* Segment physical address */
```

### E141-084 phdr_last
固定time@a039d9803ade mykernel/include/uapi/linux/elf.h:275–278；member
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Xword	p_filesz;	/* Segment size in file */
		Elf64_Xword	p_memsz;	/* Segment size in memory */
		Elf64_Xword	p_align;	/* Segment alignment, file & memory */
	} Elf64_Phdr;
```

### E141-085 AT_NULL
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:9–9；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_NULL]
```c
	#define AT_NULL		0		/* end of vector */
```

### E141-086 AT_IGNORE
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:10–10；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_IGNORE]
```c
	#define AT_IGNORE		1		/* entry should be ignored */
```

### E141-087 AT_EXECFD
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:11–11；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EXECFD]
```c
	#define AT_EXECFD		2		/* file descriptor of program */
```

### E141-088 AT_PHDR
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:12–12；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PHDR]
```c
	#define AT_PHDR		3		/* program headers for program */
```

### E141-089 AT_PHENT
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:13–13；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PHENT]
```c
	#define AT_PHENT		4		/* size of program header entry */
```

### E141-090 AT_PHNUM
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:14–14；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PHNUM]
```c
	#define AT_PHNUM		5		/* number of program headers */
```

### E141-091 AT_PAGESZ
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:15–15；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PAGESZ]
```c
	#define AT_PAGESZ		6		/* system page size */
```

### E141-092 AT_BASE
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:16–16；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_BASE]
```c
	#define AT_BASE		7		/* base address of interpreter */
```

### E141-093 AT_FLAGS
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:17–17；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_FLAGS]
```c
	#define AT_FLAGS		8		/* flags */
```

### E141-094 AT_ENTRY
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:18–18；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_ENTRY]
```c
	#define AT_ENTRY		9		/* entry point of program */
```

### E141-095 AT_NOTELF
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:19–19；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_NOTELF]
```c
	#define AT_NOTELF		10		/* program is not ELF */
```

### E141-096 AT_UID
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:20–20；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_UID]
```c
	#define AT_UID			11		/* real uid */
```

### E141-097 AT_EUID
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:21–21；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EUID]
```c
	#define AT_EUID		12		/* effective uid */
```

### E141-098 AT_GID
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:22–22；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_GID]
```c
	#define AT_GID			13		/* real gid */
```

### E141-099 AT_EGID
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:23–23；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EGID]
```c
	#define AT_EGID		14		/* effective gid */
```

### E141-100 AT_PLATFORM
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:24–24；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_PLATFORM]
```c
	#define AT_PLATFORM	15		/* string identifying CPU for optimizations */
```

### E141-101 AT_HWCAP
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:25–25；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_HWCAP]
```c
	#define AT_HWCAP		16		/* arch dependent hints at CPU capabilities */
```

### E141-102 AT_CLKTCK
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:26–26；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_CLKTCK]
```c
	#define AT_CLKTCK		17		/* frequency at which times() increments */
```

### E141-103 AT_SECURE
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:28–28；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_SECURE]
```c
	#define AT_SECURE		23		/* secure mode boolean */
```

### E141-104 AT_BASE_PLATFORM
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:29–30；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_BASE_PLATFORM]
```c
	#define AT_BASE_PLATFORM	24		/* string identifying real platform, may
												 * differ from AT_PLATFORM. */
```

### E141-105 AT_RANDOM
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:31–31；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_RANDOM]
```c
	#define AT_RANDOM		25		/* address of 16 random bytes */
```

### E141-106 AT_HWCAP2
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:32–32；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_HWCAP2]
```c
	#define AT_HWCAP2		26		/* extension of AT_HWCAP */
```

### E141-107 AT_RSEQ_FEATURE_SIZE
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:33–33；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_RSEQ_FEATURE_SIZE]
```c
	#define AT_RSEQ_FEATURE_SIZE	27		/* rseq supported feature size */
```

### E141-108 AT_RSEQ_ALIGN
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:34–34；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_RSEQ_ALIGN]
```c
	#define AT_RSEQ_ALIGN		28			/* rseq allocation alignment */
```

### E141-109 AT_HWCAP3
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:35–35；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_HWCAP3]
```c
	#define AT_HWCAP3		29		/* extension of AT_HWCAP */
```

### E141-110 AT_HWCAP4
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:36–36；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_HWCAP4]
```c
	#define AT_HWCAP4		30		/* extension of AT_HWCAP */
```

### E141-111 AT_EXECFN
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:38–38；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EXECFN]
```c
	#define AT_EXECFN		31		/* filename of program */
```

### E141-112 AT_MINSIGSTKSZ
固定time@a039d9803ade mykernel/include/uapi/linux/auxvec.h:41–41；macro
[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_MINSIGSTKSZ]
```c
	#  define AT_MINSIGSTKSZ	51		/* minimal stack size for signal delivery */
```

### E141-113 have_execfd
固定time@a039d9803ade mykernel/include/linux/fs/binfmts.h:24–25；member
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
			/* Should an execfd be passed to userspace? */
			have_execfd:1,
```

### E141-114 secureexec
固定time@a039d9803ade mykernel/include/linux/fs/binfmts.h:30–34；member
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
			 * Set by bprm_creds_for_exec hook to indicate a
			 * privilege-gaining exec has happened. Used to set
			 * AT_SECURE auxv for glibc.
			 */
			secureexec:1,
```

### E141-115 bprm_fields
固定time@a039d9803ade mykernel/include/linux/fs/binfmts.h:55–57；member
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		uint			interp_flags;
		int				execfd;		/* File descriptor of the executable */
		ulong			loader, exec;
```

### E141-116 BINPRM_FLAGS_PRESERVE_ARGV0_BIT
固定time@a039d9803ade mykernel/include/linux/fs/binfmts.h:72–72；macro
[VERIFIED mykernel/include/linux/fs/binfmts.h::BINPRM_FLAGS_PRESERVE_ARGV0_BIT]
```c
	#define BINPRM_FLAGS_PRESERVE_ARGV0_BIT		3
```

### E141-117 BINPRM_FLAGS_PRESERVE_ARGV0
固定time@a039d9803ade mykernel/include/linux/fs/binfmts.h:73–73；macro
[VERIFIED mykernel/include/linux/fs/binfmts.h::BINPRM_FLAGS_PRESERVE_ARGV0]
```c
	#define BINPRM_FLAGS_PRESERVE_ARGV0			(1 << BINPRM_FLAGS_PRESERVE_ARGV0_BIT)
```

### E141-118 AT_FLAGS_PRESERVE_ARGV0_BIT
固定time@a039d9803ade mykernel/include/uapi/linux/binfmts.h:22–22；macro
[VERIFIED mykernel/include/uapi/linux/binfmts.h::AT_FLAGS_PRESERVE_ARGV0_BIT]
```c
	#define AT_FLAGS_PRESERVE_ARGV0_BIT		0
```

### E141-119 AT_FLAGS_PRESERVE_ARGV0
固定time@a039d9803ade mykernel/include/uapi/linux/binfmts.h:23–23；macro
[VERIFIED mykernel/include/uapi/linux/binfmts.h::AT_FLAGS_PRESERVE_ARGV0]
```c
	#define AT_FLAGS_PRESERVE_ARGV0			(1 << AT_FLAGS_PRESERVE_ARGV0_BIT)
```

### E141-120 AT_VECTOR_SIZE_ARCH
固定time@a039d9803ade mykernel/mm/misc/mm_misc_const.h:6–6；macro
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE_ARCH]
```c
	#  define AT_VECTOR_SIZE_ARCH	0
```

### E141-121 AT_VECTOR_SIZE_BASE
固定time@a039d9803ade mykernel/mm/misc/mm_misc_const.h:9–9；macro
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE_BASE]
```c
	#define AT_VECTOR_SIZE_BASE		22 /* NEW_AUX_ENT entries in auxiliary table */
```

### E141-122 AT_VECTOR_SIZE
固定time@a039d9803ade mykernel/mm/misc/mm_misc_const.h:12–12；macro
[VERIFIED mykernel/mm/misc/mm_misc_const.h::AT_VECTOR_SIZE]
```c
	#define AT_VECTOR_SIZE			(2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))
```

### E141-123 HZ
固定time@a039d9803ade mykernel/include/linux/kernel/asm-generic/param.h:9–9；macro
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::HZ]
```c
	#define HZ				1000		/* Internal kernel timer frequency */
```

### E141-124 USER_HZ
固定time@a039d9803ade mykernel/include/linux/kernel/asm-generic/param.h:10–10；macro
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::USER_HZ]
```c
	#define USER_HZ			250			/* some user interfaces are */
```

### E141-125 CLOCKS_PER_SEC
固定time@a039d9803ade mykernel/include/linux/kernel/asm-generic/param.h:11–11；macro
[VERIFIED mykernel/include/linux/kernel/asm-generic/param.h::CLOCKS_PER_SEC]
```c
	#define CLOCKS_PER_SEC	(USER_HZ)	/* in "ticks" like times() */
```

### E141-126 saved_auxv
固定time@a039d9803ade mykernel/mm/misc/mm_misc_types.h:103–103；member
[VERIFIED mykernel/mm/misc/mm_misc_types.h::mm_struct]
```c
			ulong			saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */
```

### E141-127 build_arch
固定time@a039d9803ade mykernel/scripts/options_flags.cmake:7–9；build_declaration
[VERIFIED mykernel/scripts/options_flags.cmake::ARCH]
```c
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
```

### E141-128 build_64
固定time@a039d9803ade mykernel/scripts/options_flags.cmake:39–43；build_declaration
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

### E141-129 build_config
固定time@a039d9803ade mykernel/scripts/options_flags.cmake:58–62；build_declaration
[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

## 非活动候选（不计正式窗）
arch elf.h 178行：
```c
	// #define ELF_HWCAP2		(elf_hwcap2)
```
arch elf.h 247–253行：
```c
	// #define ARCH_DLINFO							\
	// do {									\
	// 	if (vdso64_enabled)						\
	// 		NEW_AUX_ENT(AT_SYSINFO_EHDR,				\
	// 				(unsigned long __force)current->mm->context.vdso); \
	// 	NEW_AUX_ENT(AT_MINSIGSTKSZ, get_sigframe_size());		\
	// } while (0)
```
