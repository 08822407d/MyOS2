---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-ARGUMENT-VECTORS-139
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: source_evidence
produced_by: "GPT-6（模型族，后台不可认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
date: 2026-11-23
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第139批 · 固定time正式源码窗

本批正式窗口各为固定time定义体内1–5行真实物理文本，标题只对应局部窄断言。没有预处理或真实运行，不因条件宏或注释推断最终编译二进制。

## E139-001 · read user pointer slot with get_user

`mykernel/sched/forkexec/exec.c::get_user_arg_ptr`，物理行187–191，角色`A_USER_SLOT`。

[VERIFIED mykernel/sched/forkexec/exec.c::get_user_arg_ptr]
```c
static const char __user
*get_user_arg_ptr(const char __user *const __user *argv, int nr) {
	const char __user *native;
	if (get_user(native, argv + nr))
		return ERR_PTR(-EFAULT);
```

## E139-002 · slot fault and native pointer return

`mykernel/sched/forkexec/exec.c::get_user_arg_ptr`，物理行190–193，角色`A_USER_SLOT_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::get_user_arg_ptr]
```c
	if (get_user(native, argv + nr))
		return ERR_PTR(-EFAULT);

	return native;
```

## E139-003 · user count init and null array gate

`mykernel/sched/forkexec/exec.c::count`，物理行199–203，角色`A_USER_COUNT_INIT`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
static int
count(const char *const *argv, int max) {
	int i = 0;

	if (argv != NULL) {
```

## E139-004 · user count reads vector pointer and begins null test

`mykernel/sched/forkexec/exec.c::count`，物理行204–208，角色`A_USER_COUNT_LOOP`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
		for (;;) {
			const char __user *p =
					get_user_arg_ptr(argv, i);

			if (!p)
```

## E139-005 · user count error and max predicates

`mykernel/sched/forkexec/exec.c::count`，物理行211–215，角色`A_USER_COUNT_GUARD`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			if (IS_ERR(p))
				return -EFAULT;

			if (i >= max)
				return -E2BIG;
```

## E139-006 · user count increments i

`mykernel/sched/forkexec/exec.c::count`，物理行216–216，角色`A_USER_COUNT_INC`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			++i;
```

## E139-007 · user count returns i

`mykernel/sched/forkexec/exec.c::count`，物理行221–223，角色`A_USER_COUNT_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
		}
	}
	return i;
```

## E139-008 · kernel count null array early return

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行227–231，角色`A_KCOUNT_NULL`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
count_strings_kernel(const char *const *argv) {
	int i;

	if (!argv)
		return 0;
```

## E139-009 · kernel direct argv[i] read with max gate

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行233–235，角色`A_KCOUNT_LOOP`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
			return -E2BIG;
```

## E139-010 · kernel count return i

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行239–240，角色`A_KCOUNT_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	}
	return i;
```

## E139-011 · temporary stack initial vm_end and vm_start

`mykernel/sched/forkexec/exec.c::__bprm_mm_init`，物理行122–124，角色`B_VMA`。

[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	vma->vm_end = STACK_TOP_MAX;
	vma->vm_start = vma->vm_end - PAGE_SIZE;
	vma->vm_flags = VM_SOFTDIRTY | VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP;
```

## E139-012 · initial bprm.p is vm_end-sizeof(void*)

`mykernel/sched/forkexec/exec.c::__bprm_mm_init`，物理行133–134，角色`B_T`。

[VERIFIED mykernel/sched/forkexec/exec.c::__bprm_mm_init]
```c
	bprm->p = vma->vm_end - sizeof(void *);
	return 0;
```

## E139-013 · _STK_LIM is 8MiB

`mykernel/include/uapi/linux/resource.h::_STK_LIM`，物理行65–65，角色`B_STKLIM`。

[VERIFIED mykernel/include/uapi/linux/resource.h::_STK_LIM]
```c
	#define _STK_LIM			(8*1024*1024)
```

## E139-014 · ARG_MAX constant 131072 bytes

`mykernel/include/uapi/linux/limits.h::ARG_MAX`，物理行8–8，角色`B_ARGMAX`。

[VERIFIED mykernel/include/uapi/linux/limits.h::ARG_MAX]
```c
	#define ARG_MAX			131072	/* # bytes of args + environ for exec() */
```

## E139-015 · active stack limit 3/4 _STK_LIM

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行268–268，角色`B_LIMIT34`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = _STK_LIM / 4 * 3;
```

## E139-016 · active max_t limit with ARG_MAX

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行274–274，角色`B_LIMITMAX`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = max_t(ulong, limit, ARG_MAX);
```

## E139-017 · pointer budget and string limit subtraction

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行291–294，角色`B_PTRBUDGET`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	ptr_size = (max(bprm->argc, 1) + bprm->envc) * sizeof(void *);
	if (limit <= ptr_size)
		return -E2BIG;
	limit -= ptr_size;
```

## E139-018 · stack limit helper invocation

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行296–296，角色`B_LIMITCALL`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	return bprm_set_stack_limit(bprm, limit);
```

## E139-019 · argmin guard and active p-minus-limit store

`mykernel/sched/forkexec/exec.c::bprm_set_stack_limit`，物理行246–249，角色`B_ARGMIN`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_set_stack_limit]
```c
	if (bprm->p < limit)
		return -E2BIG;
	bprm->argmin = bprm->p - limit;
	return 0;
```

## E139-020 · AT_FDCWD selects filename.name

`mykernel/sched/forkexec/exec.c::alloc_bprm`，物理行1117–1119，角色`B_BPRM_NAME`。

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```

## E139-021 · initial interp aliases filename

`mykernel/sched/forkexec/exec.c::alloc_bprm`，物理行1142–1142，角色`B_INTERP`。

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	bprm->interp = bprm->filename;
```

## E139-022 · ordinary do_execve AT_FDCWD

`mykernel/sched/forkexec/exec.c::do_execve`，物理行1438–1442，角色`B_DO_EXEC`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execve]
```c
do_execve(filename_s *filename,
		const char *const *__argv,
		const char *const *__envp) {
	return do_execveat_common(AT_FDCWD,
			filename, __argv, __envp, 0);
```

## E139-023 · complete execve entry and active return

`mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve, const char *,filename,`，物理行1460–1464，角色`B_SYSCALL`。

[VERIFIED mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve, const char *,filename,]
```c
MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
}
```

## E139-024 · bprm allocation and error gate

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1316–1319，角色`B_ALLOC`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	bprm = alloc_bprm(fd, filename, flags);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
```

## E139-025 · user argc counter and warning call

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1322–1325，角色`B_ARGC_WARN`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv:"
				" empty string added\n", current->comm, bprm->filename);
```

## E139-026 · user argc negative guard and store

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1326–1328，角色`B_ARGC`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;
```

## E139-027 · user environment count and store

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1330–1333，角色`B_ENVC`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

## E139-028 · limit computation call and error gate

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1335–1337，角色`B_LIMIT_CHECK`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;
```

## E139-029 · filename copied and bprm.exec saved

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1339–1342，角色`B_EXEC_SAVE`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

## E139-030 · user environment string copy call

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1344–1346，角色`B_COPY_ENV`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

## E139-031 · user argument string copy call

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1348–1350，角色`B_COPY_ARG`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

## E139-032 · user copy_strings signature

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行305–309，角色`B_UCOPY_INIT`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
copy_strings(int argc, const char *const *argv, linux_bprm_s *bprm) {
	page_s *kmapped_page = NULL;
	char *kaddr = NULL;
	ulong kpos = 0;
	int ret;
```

## E139-033 · reverse user slot walk

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行311–313，角色`B_UCOPY_REVERSE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	while (argc-- > 0) {
		const char __user *str;
		int len;
```

## E139-034 · user slot re-read in copy phase

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行316–318，角色`B_UCOPY_GET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
```

## E139-035 · strnlen_user and zero fault gate

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行321–323，角色`B_UCOPY_LEN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		len = strnlen_user(str, MAX_ARG_STRLEN);
		if (!len)
			goto out;
```

## E139-036 · string length validity gate

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行325–327，角色`B_UCOPY_LENMAX`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -E2BIG;
		if (!valid_arg_len(bprm, len))
			goto out;
```

## E139-037 · user copy captures old p and subtracts len

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行330–332，角色`B_UCOPY_P`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		pos = bprm->p;
		str += len;
		bprm->p -= len;
```

## E139-038 · argument minimum guard in user copy

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行333–334，角色`B_UCOPY_ARGMIN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		if (bprm->p < bprm->argmin)
			goto out;
```

## E139-039 · user page byte chunk calculation

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行349–353，角色`B_UCOPY_CHUNK`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			bytes_to_copy = offset;
			if (bytes_to_copy > len)
				bytes_to_copy = len;

			offset -= bytes_to_copy;
```

## E139-040 · user copy pos/src/len update

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行354–356，角色`B_UCOPY_CHUNKSTEP`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
			pos -= bytes_to_copy;
			str -= bytes_to_copy;
			len -= bytes_to_copy;
```

## E139-041 · get_arg_page for user string

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行361–365，角色`B_UCOPY_PAGE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				page = get_arg_page(bprm, pos, 1);
				if (!page) {
					ret = -E2BIG;
					goto out;
				}
```

## E139-042 · memcpy_to_page from user and put page

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行367–368，角色`B_UCOPY_MEMCPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);
```

## E139-043 · user copy returns zero on successful loop

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行385–387，角色`B_UCOPY_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	}
	ret = 0;
out:
```

## E139-044 · user length source strnlen + 1 including NUL

`mykernel/lib/string/strnlen_user.c::strnlen_user`，物理行120–120，角色`B_STRNLENUSER`。

[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	return strnlen(str, count) + 1;
```

## E139-045 · kernel string length strnlen plus one

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行399–402，角色`B_KCOPY_LEN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
int copy_string_kernel(const char *arg, linux_bprm_s *bprm)
{
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	ulong pos = bprm->p;
```

## E139-046 · kernel copy advances source end and decreases p

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行410–413，角色`B_KCOPY_P`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	arg += len;
	bprm->p -= len;
	if (bprm->p < bprm->argmin)
		return -E2BIG;
```

## E139-047 · kernel single-string reverse chunk

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行415–419，角色`B_KCOPY_CHUNK`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	while (len > 0) {
		uint bytes_to_copy = min_t(uint, len,
				min_not_zero(offset_in_page(pos), PAGE_SIZE));
		page_s *page;

```

## E139-048 · kernel source and pos decrease together

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行420–422，角色`B_KCOPY_CHUNKSTEP`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		pos -= bytes_to_copy;
		arg -= bytes_to_copy;
		len -= bytes_to_copy;
```

## E139-049 · kernel page acquired

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行424–426，角色`B_KCOPY_PAGE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		page = get_arg_page(bprm, pos, 1);
		if (!page)
			return -E2BIG;
```

## E139-050 · kernel memcpy_to_page and put page

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行428–429，角色`B_KCOPY_MEMCPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
		memcpy_to_page(page, offset_in_page(pos), arg, bytes_to_copy);
		put_arg_page(page);
```

## E139-051 · kernel string success return

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行432–432，角色`B_KCOPY_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	return 0;
```

## E139-052 · kernel reverse array pointer access

`mykernel/sched/forkexec/exec.c::copy_strings_kernel`，物理行437–441，角色`B_KARRAY_REVERSE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
copy_strings_kernel(int argc, const char *const *argv,
		linux_bprm_s *bprm) {

	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
```

## E139-053 · kernel helper uses argv[argc]

`mykernel/sched/forkexec/exec.c::copy_strings_kernel`，物理行440–443，角色`B_KARRAY_COPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
```

## E139-054 · kernel vector copy success

`mykernel/sched/forkexec/exec.c::copy_strings_kernel`，物理行448–448，角色`B_KARRAY_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
	return 0;
```

## E139-055 · later setup shifts p and arg_start

`mykernel/sched/forkexec/exec.c::setup_arg_pages`，物理行478–481，角色`B_RELOC_P`。

[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
```

## E139-056 · later setup shifts exec pointer

`mykernel/sched/forkexec/exec.c::setup_arg_pages`，物理行483–485，角色`B_RELOC_EXEC`。

[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (bprm->loader)
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;
```

## E139-057 · ELF later uses bprm.exec for AT_EXECFN

`mykernel/fs/vfs/binfmt_elf.c::AT_EXECFN`，物理行229–229，角色`B_ELF_AUX`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::AT_EXECFN]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```

## E139-058 · AT_EXECFN macro value

`mykernel/include/uapi/linux/auxvec.h::AT_EXECFN`，物理行38–38，角色`B_AUX_VALUE`。

[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EXECFN]
```c
	#define AT_EXECFN		31		/* filename of program */
```

## E139-059 · user zero argc synthesized empty string

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1358–1360，角色`C_USER_SYNTH`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
```

## E139-060 · user argc becomes one after copy

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1361–1363，角色`C_USER_ONE`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
			goto out_free;
		bprm->argc = 1;
	}
```

## E139-061 · user reaches bprm_execve and cleanup

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1365–1367，角色`C_USER_BOUND`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = bprm_execve(bprm, fd, filename, flags);
out_free:
	free_bprm(bprm);
```

## E139-062 · user putname and retval return

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1369–1371，角色`C_USER_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
out_ret:
	putname(filename);
	return retval;
```

## E139-063 · kernel_execve active PF_KTHREAD gate

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1386–1387，角色`C_KTHREAD_GATE`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	if (WARN_ON_ONCE(current->flags & PF_KTHREAD))
		return -EINVAL;
```

## E139-064 · kernel filename getname_kernel

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1389–1391，角色`C_KNAME`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	filename = getname_kernel(kernel_filename);
	if (IS_ERR(filename))
		return PTR_ERR(filename);
```

## E139-065 · kernel bprm allocation error handling

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1393–1397，角色`C_KALLOC`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm = alloc_bprm(fd, filename, 0);
	if (IS_ERR(bprm)) {
		retval = PTR_ERR(bprm);
		goto out_ret;
	}
```

## E139-066 · kernel argc zero to -EINVAL before env

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1399–1403，角色`C_KZERO`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

## E139-067 · kernel stores bprm.argc only for positive count

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1404–1404，角色`C_KARGC`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm->argc = retval;
```

## E139-068 · kernel environment counting after argc gate

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1406–1409，角色`C_KENV`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

## E139-069 · kernel pointer budget check

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1411–1413，角色`C_KBUDGET`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_stack_limits(bprm);
	if (retval < 0)
		goto out_free;
```

## E139-070 · kernel filename copy and exec marker

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1415–1418，角色`C_KEXECSAVE`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

## E139-071 · kernel environment copy

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1420–1422，角色`C_KCOPY_ENV`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

## E139-072 · kernel argv copy

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1424–1426，角色`C_KCOPY_ARG`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

## E139-073 · kernel bprm_execve call and cleanup label

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1428–1430，角色`C_KBOUND`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

## E139-074 · kernel putname and retval return

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行1432–1434，角色`C_KRETURN`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
```

## E139-075 · caller flags shift then assign RQCF_UPDATED

`mykernel/sched/scheduler/scheduler_core.c::__schedule`，物理行1065–1067，角色`D00_FLAGS`。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::__schedule]
```c
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;
```

## E139-076 · RQCF_UPDATED is 0x04

`mykernel/sched/scheduler/scheduler_const.h::RQCF_UPDATED`，物理行48–48，角色`D00_UPDATED`。

[VERIFIED mykernel/sched/scheduler/scheduler_const.h::RQCF_UPDATED]
```c
	#define RQCF_UPDATED		0x04
```

## E139-077 · strscpy inner byte store before return

`mykernel/lib/string/string.c::strscpy`，物理行216–220，角色`D00_A_WRITE`。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
			c = src[res];
			dest[res] = c;
			if (!c)
				return res;
			res++;
```

## E139-078 · strscpy overwrite last byte before negative return

`mykernel/lib/string/string.c::strscpy`，物理行225–228，角色`D00_A_REWRITE`。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		if (res)
			dest[res-1] = '\0';

		return -E2BIG;
```

## E139-079 · pad wrapper returns negative without memset

`mykernel/lib/string/string_helpers.c::strscpy_pad`，物理行835–837，角色`D00_PAD_EARLY`。

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	written = strscpy(dest, src, count);
	if (written < 0 || written == count - 1)
		return written;
```

## E139-080 · pad wrapper zero-fills after short success

`mykernel/lib/string/string_helpers.c::strscpy_pad`，物理行839–841，角色`D00_PAD_MEMSET`。

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	memset(dest + written + 1, 0, count - written - 1);

	return written;
```

## E139-081 · independent child structure assignment

`mykernel/sched/forkexec/fork.c::dup_task_struct`，物理行288–289，角色`D00_FORK_COPY`。

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```

## E139-082 · getter locks target tsk before copy

`mykernel/sched/forkexec/exec.c::__get_task_comm`，物理行820–822，角色`D00_GET_LOCK`。

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
char *__get_task_comm(char *buf, size_t buf_size, task_s *tsk)
{
	task_lock(tsk);
```

## E139-083 · getter copy and return buf

`mykernel/sched/forkexec/exec.c::__get_task_comm`，物理行824–826，角色`D00_GET_COPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
	strscpy_pad(buf, tsk->comm, buf_size);
	task_unlock(tsk);
	return buf;
```

## E139-084 · setter void with task lock

`mykernel/sched/forkexec/exec.c::__set_task_comm`，物理行836–838，角色`D00_SET_LOCK`。

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
void __set_task_comm(task_s *tsk, const char *buf)
{
	task_lock(tsk);
```

## E139-085 · setter copy to comm then unlock

`mykernel/sched/forkexec/exec.c::__set_task_comm`，物理行840–841，角色`D00_SET_COPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
```

## E139-086 · error filename early return

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行1297–1298，角色`D00_EXEC_EARLY`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (IS_ERR(filename))
		return PTR_ERR(filename);
```

## E139-087 · selected active CMake flags as bounded config evidence

`mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS`，物理行62–66，角色`D00_CONFIG`。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

## E139-088 · E138-010 active max=count, res=0

`mykernel/lib/string/string.c::strscpy`，物理行175–176，角色`D00_E010`。

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		size_t max = count;
		long res = 0;
```

## E139-089 · E138-006 active fork aggregate exit_signal

`mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork`，物理行62–64，角色`D00_E006`。

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork]
```c
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

## E139-090 · E138-018 active getter header and lock

`mykernel/sched/forkexec/exec.c::__get_task_comm`，物理行820–822，角色`D00_E018`。

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
char *__get_task_comm(char *buf, size_t buf_size, task_s *tsk)
{
	task_lock(tsk);
```

## E139-091 · user count breaks on null pointer

`mykernel/sched/forkexec/exec.c::count`，物理行208–209，角色`A_USER_NULL_BREAK`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			if (!p)
				break;
```

## E139-092 · stack limits explicitly reject negative argc/envc

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行276–277，角色`B_NEGCOUNT_GUARD`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	if (bprm->argc < 0 || bprm->envc < 0)
		return -E2BIG;
```

## 冻结E138的47窗口索引对照（不计入本批正式锚）

| 旧ID | 冻结symbol/首行 | 处置 | 新角色 |
|---|---|---|---|
| E138-001 | `TASK_COMM_LEN` 11 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-002 | `task_struct` 326 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-003 | `dup_task_struct` 288 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-004 | `copy_process` 958 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-005 | `copy_process` 976 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-006 | `MYOS_SYSCALL_DEFINE0(fork)` 60 | COMMENT_SPLIT_ACTIVE_ONLY | E139-089 |
| E138-007 | `kernel_thread` 1538 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-008 | `kernel_thread` 1542 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-009 | `kernel_thread` 1548 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-010 | `strscpy` 172 | COMMENT_SPLIT_ACTIVE_ONLY | E139-088 |
| E138-011 | `strscpy` 178 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-012 | `strscpy` 213 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-013 | `strscpy` 218 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-014 | `strscpy` 224 | COMMENT_SPLIT_ACTIVE_ONLY | E139-078 |
| E138-015 | `strscpy_pad` 831 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-016 | `strscpy_pad` 835 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-017 | `strscpy_pad` 839 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-018 | `__get_task_comm` 820 | COMMENT_SPLIT_ACTIVE_ONLY | E139-090, E139-083 |
| E138-019 | `__get_task_comm` 824 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-020 | `__set_task_comm` 836 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-021 | `__set_task_comm` 840 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-022 | `begin_new_exec` 851 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-023 | `begin_new_exec` 979 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-024 | `alloc_bprm` 1117 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-025 | `alloc_bprm` 1140 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-026 | `do_execve` 1438 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-027 | `MYOS_SYSCALL_DEFINE3(execve,` 1460 | FULL_MACRO_ENTRY_AND_RETURN | E139-023 |
| E138-028 | `do_execveat_common` 1297 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-029 | `do_execveat_common` 1314 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-030 | `task_lock` 93 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-031 | `task_unlock` 98 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-032 | `update_rq_clock` 20 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-033 | `update_rq_clock` 24 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-034 | `__schedule` 1064 | COMMENT_SPLIT_ACTIVE_ONLY | E139-075 |
| E138-035 | `pick_next_task_myos` 12 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-036 | `pick_next_task_myos` 14 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-037 | `pick_next_task_myos` 56 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-038 | `sched_rt_entity` 43 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-039 | `sched_rt_entity` 46 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-040 | `myos_rq` 195 | KEEP_INLINE_COMMENT_TEXT | 冻结旧源定位 |
| E138-041 | `pick_next_task_myos` 24 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-042 | `pick_next_task_myos` 44 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-043 | `list_add_to_prev` 221 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-044 | `pick_next_task_myos` 52 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-045 | `pick_next_task_myos` 56 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-046 | `list_header_remove_head` 653 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |
| E138-047 | `list_del_init` 280 | OLD_SOURCE_ANCHOR | 冻结旧源定位 |

## 来源范围与非正式定位

- 旧E138-006、010、014、018、034原证据有独立注释；本批另取只含活动代码的窄窗，注释不得代替行为生产者。
- 旧E138-040定义中的原行内文字保持源码原样，不借注释推导行为。
- `mykernel/lib/string/string.c` 中与非对齐访问有关的预处理分支仅作文本条件定位；本批不执行预处理。CMake正式活动文本见本批D00_CONFIG窗口，不从单一选项页推出全仓或实际二进制缺少其它选项。
- 本批直接验证正式窗物理条数；相同物理区间可能由两个独立窄断言复用，实际不同坐标数需另行报告。
