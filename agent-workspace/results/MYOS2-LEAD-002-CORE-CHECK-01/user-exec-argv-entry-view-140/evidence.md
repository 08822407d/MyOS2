---
record_type: source_evidence
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-ARGV-ENTRY-VIEW-140
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
base_snapshot: "time（分支名）"
read_channel: connector
phase: 2
status: READY_FOR_MAINLINE_REVIEW
---

# 第140批固定源码窗

每条正式窗为固定 time 的逐字 1–5 行。宏调用身份按完整跨行调用折叠空白；实际引文保持原行。CMake 源窗是构建声明，不是实际编译证明；全局 envs 为声明/对象定义，不是调用。不得将已核收的旧窗重复计入本批 P6。没有从包装宏推断生成函数名。

## E140-001 · 完整 execve 宏调用定义与活动 return；身份按跨行调用压缩空白

`mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve, const char *,filename, const char *const __user *, argv, const char *const __user *, envp)`，物理行 1460–1464；角色 `D_EXEC`。

[VERIFIED mykernel/sched/forkexec/exec.c::MYOS_SYSCALL_DEFINE3(execve, const char *,filename, const char *const __user *, argv, const char *const __user *, envp)]
```c
MYOS_SYSCALL_DEFINE3(execve, const char *,filename,
		const char *const __user *, argv,
		const char *const __user *, envp) {
	return do_execve(getname(filename), argv, envp);
}
```

## E140-002 · 完整 fork 宏调用定义头

`mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)`，物理行 51–52；角色 `D_FORK_HEAD`。

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)]
```c
MYOS_SYSCALL_DEFINE0(fork)
{
```

## E140-003 · fork 活动 exit_signal 和 kernel_clone return

`mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)`，物理行 62–64；角色 `D_FORK_BODY`。

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE0(fork)]
```c
		.exit_signal = SIGCHLD,
	};
	return kernel_clone(&args);
```

## E140-004 · 固定 CMake 选项文本，含 syscall wrapper 与 CONFIG_BUG

`mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS`，物理行 64–66；角色 `D_CONFIG`。

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```

## E140-005 · 用户计数 i=0，外门 argv != NULL

`mykernel/sched/forkexec/exec.c::count`，物理行 199–203；角色 `D_COUNT_GATE`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
static int
count(const char *const *argv, int max) {
	int i = 0;

	if (argv != NULL) {
```

## E140-006 · 取用户槽后以空指针 break

`mykernel/sched/forkexec/exec.c::count`，物理行 205–209；角色 `D_COUNT_SLOT`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			const char __user *p =
					get_user_arg_ptr(argv, i);

			if (!p)
				break;
```

## E140-007 · 非空槽错误及 max 门

`mykernel/sched/forkexec/exec.c::count`，物理行 211–215；角色 `D_COUNT_ERR`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			if (IS_ERR(p))
				return -EFAULT;

			if (i >= max)
				return -E2BIG;
```

## E140-008 · 计数器增一

`mykernel/sched/forkexec/exec.c::count`，物理行 216–216；角色 `D_COUNT_INC`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
			++i;
```

## E140-009 · 用户 count 返回 i

`mykernel/sched/forkexec/exec.c::count`，物理行 223–223；角色 `D_COUNT_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::count]
```c
	return i;
```

## E140-010 · get_user 与 native return

`mykernel/sched/forkexec/exec.c::get_user_arg_ptr`，物理行 190–193；角色 `D_GET_SLOT`。

[VERIFIED mykernel/sched/forkexec/exec.c::get_user_arg_ptr]
```c
	if (get_user(native, argv + nr))
		return ERR_PTR(-EFAULT);

	return native;
```

## E140-011 · 内核 NULL 向量直接 return 0

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行 230–231；角色 `D_K_NULL`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	if (!argv)
		return 0;
```

## E140-012 · 非空内核向量直接索引及上限

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行 233–235；角色 `D_K_LOOP`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	for (i = 0; argv[i]; ++i) {
		if (i >= MAX_ARG_STRINGS)
			return -E2BIG;
```

## E140-013 · 内核正常返回 i

`mykernel/sched/forkexec/exec.c::count_strings_kernel`，物理行 239–240；角色 `D_K_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::count_strings_kernel]
```c
	}
	return i;
```

## E140-014 · 预留指针预算而不是 p 写点

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行 291–294；角色 `D_PTR`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	ptr_size = (max(bprm->argc, 1) + bprm->envc) * sizeof(void *);
	if (limit <= ptr_size)
		return -E2BIG;
	limit -= ptr_size;
```

## E140-015 · argmin = p - limit 与边界

`mykernel/sched/forkexec/exec.c::bprm_set_stack_limit`，物理行 246–249；角色 `D_ARGMIN`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_set_stack_limit]
```c
	if (bprm->p < limit)
		return -E2BIG;
	bprm->argmin = bprm->p - limit;
	return 0;
```

## E140-016 · 内核字符串长度含 NUL

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行 399–402；角色 `D_K_LEN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
int copy_string_kernel(const char *arg, linux_bprm_s *bprm)
{
	int len = strnlen(arg, MAX_ARG_STRLEN) + 1 /* terminating NUL */;
	ulong pos = bprm->p;
```

## E140-017 · 内核字符串 p 下移

`mykernel/sched/forkexec/exec.c::copy_string_kernel`，物理行 410–413；角色 `D_K_P`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_string_kernel]
```c
	arg += len;
	bprm->p -= len;
	if (bprm->p < bprm->argmin)
		return -E2BIG;
```

## E140-018 · 用户复制逆序循环

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 311–314；角色 `D_U_REVERSE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	while (argc-- > 0) {
		const char __user *str;
		int len;
		ulong pos;
```

## E140-019 · 复制阶段重新取槽

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 316–318；角色 `D_U_GET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		ret = -EFAULT;
		str = get_user_arg_ptr(argv, argc);
		if (IS_ERR(str))
```

## E140-020 · 用户测长

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 321–323；角色 `D_U_LEN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		len = strnlen_user(str, MAX_ARG_STRLEN);
		if (!len)
			goto out;
```

## E140-021 · 用户字符串 p 下移及下限

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 330–334；角色 `D_U_P`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
		pos = bprm->p;
		str += len;
		bprm->p -= len;
		if (bprm->p < bprm->argmin)
			goto out;
```

## E140-022 · 用户页内复制和释放页引用

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 367–368；角色 `D_U_PAGE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
				memcpy_to_page(page, offset_in_page(pos), str, bytes_to_copy);
				put_arg_page(page);
```

## E140-023 · 正常循环后 ret=0，公共出口标签

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 385–387；角色 `D_U_RET_ASSIGN`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	}
	ret = 0;
out:
```

## E140-024 · 公共出口实际 return ret

`mykernel/sched/forkexec/exec.c::copy_strings`，物理行 393–393；角色 `D_U_RET`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings]
```c
	return ret;
```

## E140-025 · 先复制文件名再保存 exec

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1339–1342；角色 `D_SAVE_EXEC`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

## E140-026 · 环境复制调用

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1344–1346；角色 `D_ENV`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

## E140-027 · argv 复制调用

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1348–1350；角色 `D_ARGV`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = copy_strings(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

## E140-028 · 用户零计数才合成空串，成功才写 argc=1

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1358–1362；角色 `D_SYNTH`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (bprm->argc == 0) {
		retval = copy_string_kernel("", bprm);
		if (retval < 0)
			goto out_free;
		bprm->argc = 1;
```

## E140-029 · 内核零计数置 -EINVAL，负值跳清理

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1399–1403；角色 `D_K_GATE`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(argv);
	if (retval == 0)
		retval = -EINVAL;
	if (retval < 0)
		goto out_free;
```

## E140-030 · 守卫之后的 argc 赋值语句；正数条件需联读 D_K_GATE

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1404–1404；角色 `D_K_ARGC`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	bprm->argc = retval;
```

## E140-031 · 之后才数环境并赋 envc

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1406–1409；角色 `D_K_ENV`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = count_strings_kernel(envp);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

## E140-032 · 内核路径复制文件名和保存 exec

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1415–1418；角色 `D_K_SAVE`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_string_kernel(bprm->filename, bprm);
	if (retval < 0)
		goto out_free;
	bprm->exec = bprm->p;
```

## E140-033 · 内核环境向量复制

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1420–1422；角色 `D_K_ENV_COPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->envc, envp, bprm);
	if (retval < 0)
		goto out_free;
```

## E140-034 · 内核参数向量复制

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1424–1426；角色 `D_K_ARG_COPY`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = copy_strings_kernel(bprm->argc, argv, bprm);
	if (retval < 0)
		goto out_free;
```

## E140-035 · 内核逆序调用 copy_string_kernel

`mykernel/sched/forkexec/exec.c::copy_strings_kernel`，物理行 440–443；角色 `D_K_REVERSE`。

[VERIFIED mykernel/sched/forkexec/exec.c::copy_strings_kernel]
```c
	while (argc-- > 0) {
		int ret = copy_string_kernel(argv[argc], bprm);
		if (ret < 0)
			return ret;
```

## E140-036 · 调用边界与 out_free

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1428–1430；角色 `D_CLEANUP1`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
	retval = bprm_execve(bprm, fd, filename, 0);
out_free:
	free_bprm(bprm);
```

## E140-037 · out_ret、putname、返回 retval

`mykernel/sched/forkexec/exec.c::kernel_execve`，物理行 1432–1434；角色 `D_CLEANUP2`。

[VERIFIED mykernel/sched/forkexec/exec.c::kernel_execve]
```c
out_ret:
	putname(filename);
	return retval;
```

## E140-038 · 释放 interp 条件、fdpath、最终 bprm

`mykernel/sched/forkexec/exec.c::free_bprm`，物理行 1091–1094；角色 `D_FREE`。

[VERIFIED mykernel/sched/forkexec/exec.c::free_bprm]
```c
	if (bprm->interp != bprm->filename)
		kfree(bprm->interp);
	kfree(bprm->fdpath);
	kfree(bprm);
```

## E140-039 · AT_EXECFN 使用点；不是宏定义

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 229–229；角色 `D_EXECFN_USE`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_EXECFN, bprm->exec);
```

## E140-040 · AT_EXECFN 独立宏定义值

`mykernel/include/uapi/linux/auxvec.h::AT_EXECFN`，物理行 38–38；角色 `D_EXECFN_DEF`。

[VERIFIED mykernel/include/uapi/linux/auxvec.h::AT_EXECFN]
```c
	#define AT_EXECFN		31		/* filename of program */
```

## E140-041 · stack_shift 与 p、mm.arg_start 平移

`mykernel/sched/forkexec/exec.c::setup_arg_pages`，物理行 478–481；角色 `A_SHIFT`。

[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	stack_shift = vma->vm_end - stack_top;

	bprm->p -= stack_shift;
	mm->arg_start = bprm->p;
```

## E140-042 · 后续 exec 指针平移

`mykernel/sched/forkexec/exec.c::setup_arg_pages`，物理行 483–485；角色 `A_SHIFT_EXEC`。

[VERIFIED mykernel/sched/forkexec/exec.c::setup_arg_pages]
```c
	if (bprm->loader)
		bprm->loader -= stack_shift;
	bprm->exec -= stack_shift;
```

## E140-043 · 读取 bprm 的 p、argc、envc，初始 sp

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 129–133；角色 `A_INPUT`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm_s *mm = current->mm;
	ulong p = bprm->p;
	int argc = bprm->argc;
	int envc = bprm->envc;
	elf_addr_t __user *sp = (void *)p;
```

## E140-044 · auxv 索引、表空间 items 及 bprm.p

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 253–257；角色 `A_TABLE_SIZE`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	ei_index = elf_info - (elf_addr_t *)mm->saved_auxv;
	sp = STACK_ADD(p, ei_index);

	items = (argc + 1) + (envc + 1) + 1;
	bprm->p = STACK_ROUND(sp, items);
```

## E140-045 · sp 取 bprm.p 表底

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 260–260；角色 `A_SP`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	sp = (elf_addr_t __user *)bprm->p;
```

## E140-046 · mmap 写锁错误门

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 266–267；角色 `A_LOCK_GATE`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (mmap_write_lock_killable(mm))
		return -EINTR;
```

## E140-047 · vma 门，后续只作独立片段分析

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 270–271；角色 `A_VMA_GATE`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (!vma)
		return -EFAULT;
```

## E140-048 · argc 写入后 sp 自增

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 274–275；角色 `A_ARGC`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(argc, sp++))
		return -EFAULT;
```

## E140-049 · arg_end 初写、逐项 argv 指针写入

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 278–282；角色 `A_ARG_LOOP`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	p = mm->arg_end = mm->arg_start;
	while (argc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

## E140-050 · 含 NUL 测长及 p 递增

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 283–286；角色 `A_ARG_LEN`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
```

## E140-051 · argv 终止零和 arg_end 末写

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 288–290；角色 `A_ARG_END`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->arg_end = p;
```

## E140-052 · env_start/env_end 初写、envp 指针写入

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 293–297；角色 `A_ENV_LOOP`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	mm->env_end = mm->env_start = p;
	while (envc-- > 0) {
		size_t len;
		if (put_user((elf_addr_t)p, sp++))
			return -EFAULT;
```

## E140-053 · 环境含 NUL 测长及 p 递增

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 298–301；角色 `A_ENV_LEN`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
		len = strnlen_user((void __user *)p, MAX_ARG_STRLEN);
		if (!len || len > MAX_ARG_STRLEN)
			return -EINVAL;
		p += len;
```

## E140-054 · envp 终止零和 env_end 末写

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 303–305；角色 `A_ENV_END`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (put_user(0, sp++))
		return -EFAULT;
	mm->env_end = p;
```

## E140-055 · auxv 下一段从当前 sp 写，return 0 条件

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 308–310；角色 `A_AUX_START`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	if (copy_to_user(sp, mm->saved_auxv, ei_index * sizeof(elf_addr_t)))
		return -EFAULT;
	return 0;
```

## E140-056 · 活动 strnlen+1 含终止字节

`mykernel/lib/string/strnlen_user.c::strnlen_user`，物理行 120–120；角色 `A_NUL_LEN`。

[VERIFIED mykernel/lib/string/strnlen_user.c::strnlen_user]
```c
	return strnlen(str, count) + 1;
```

## E140-057 · 全局环境指针对象声明

`myinitramfs/mylib/csu/crt1_c.c::envs`，物理行 12–12；角色 `B_ENVS_DECL`。

[VERIFIED myinitramfs/mylib/csu/crt1_c.c::envs]
```c
const char **envs;
```

## E140-058 · 候选 C 启动函数形参、终止槽固定检查与 envs 地址计算

`myinitramfs/mylib/csu/crt1_c.c::_start`，物理行 14–17；角色 `B_START`。

[VERIFIED myinitramfs/mylib/csu/crt1_c.c::_start]
```c
__attribute__((section(".crt1.text"))) void _start(int argc, const char **argv)
{
	while (argv[argc] != NULL);
	envs = argv + argc + 1;
```

## E140-059 · 三个标准流顺序赋值

`myinitramfs/mylib/csu/crt1_c.c::_start`，物理行 19–21；角色 `B_STREAMS`。

[VERIFIED myinitramfs/mylib/csu/crt1_c.c::_start]
```c
	stdin = fopen("/dev/console", "r");
	stdout = fopen("/dev/console", "w");
	stderr = fopen("/dev/console", "w");
```

## E140-060 · exit 的实参来自 main 返回

`myinitramfs/mylib/csu/crt1_c.c::_start`，物理行 31–31；角色 `B_EXIT`。

[VERIFIED myinitramfs/mylib/csu/crt1_c.c::_start]
```c
	exit(main(argc, argv));
```

## E140-061 · init main 的 C 形参和控制台重开

`myinitramfs/myinit/initd.c::main`，物理行 9–13；角色 `C_INIT_MAIN`。

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
int main(int argc, const char *argv[])
{
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);
```

## E140-062 · 向 normal_boot 转发原 argc 和 argv

`myinitramfs/myinit/initd.c::main`，物理行 18–18；角色 `C_FORWARD`。

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	return normal_boot(argc, argv);
```

## E140-063 · 新 prog_name 及独立 args 数组

`myinitramfs/myinit/initd.c::normal_boot`，物理行 22–26；角色 `C_ARG_SOURCE`。

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
int normal_boot(int argc, const char *argv[])
{
	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
```

## E140-064 · 新环境数组和 fork 调用

`myinitramfs/myinit/initd.c::normal_boot`，物理行 27–30；角色 `C_ENV_SOURCE`。

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };

	int rv = fork();
```

## E140-065 · 子分支条件及 getpid

`myinitramfs/myinit/initd.c::normal_boot`，物理行 32–33；角色 `C_CHILD`。

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	if (rv == 0) {
		pid_t pid = getpid();
```

## E140-066 · execve 三个原实参

`myinitramfs/myinit/initd.c::normal_boot`，物理行 35–35；角色 `C_EXEC`。

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		execve(prog_name, args, envs);
```

## E140-067 · shell main 接收入口参数

`myinitramfs/myshell/myshell.c::main`，物理行 66–68；角色 `C_SH_ENTRY`。

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
int main(int argc, const char *argv[])
{
	test_PrintDateTime();
```

## E140-068 · 命令行读取 buf

`myinitramfs/myshell/myshell.c::main`，物理行 92–92；角色 `C_SH_READ`。

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		read_line(buf);
```

## E140-069 · 新 __argc/__argv、解析及 index<0 分支

`myinitramfs/myshell/myshell.c::main`，物理行 96–100；角色 `C_SH_PARSE`。

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		int __argc = 0;
		char ** __argv = NULL;
		index = parse_command(buf, &__argc, &__argv);
		if(index < 0)
			printf("Input Error, No Command Found!\n");
```

## E140-070 · else 分派新解析的参数

`myinitramfs/myshell/myshell.c::main`，物理行 101–102；角色 `C_SH_RUN`。

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
		else
			run_cmd(index, __argc, __argv);	//argc,argv
```

## E140-071 · 扫描命令行输入条件

`myinitramfs/myshell/myshell.c::parse_command`，物理行 162–166；角色 `C_PARSE_SCAN`。

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	for(i = j;i<256;i++)
	{
		if(!buf[i])
			break;
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
```

## E140-072 · 计数和空命令拒绝

`myinitramfs/myshell/myshell.c::parse_command`，物理行 166–170；角色 `C_PARSE_COUNT`。

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		if(buf[i] != ' ' && (buf[i+1] == ' ' || buf[i+1] == '\0'))
			(*argc)++;
	}
	if(!*argc)
		return -1;
```

## E140-073 · 分配向量、清零、槽指向 buf

`myinitramfs/myshell/myshell.c::parse_command`，物理行 172–176；角色 `C_PARSE_SLOTS`。

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	*argv = (char **)malloc(sizeof(char **) * (*argc + 1));
	memset(*argv, 0, sizeof(char **) * (*argc + 1));
	for(i = 0; i < *argc && j < 256; i++)
	{
		*((*argv)+i) = &buf[j];
```

## E140-074 · 按空格推进与写终止 NUL

`myinitramfs/myshell/myshell.c::parse_command`，物理行 178–182；角色 `C_PARSE_SPLIT`。

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
		while(buf[j] && buf[j] != ' ')
			j++;
		buf[j++] = '\0';
		while(buf[j] == ' ')
			j++;
```

## E140-075 · find_cmd 的返回作为 index

`myinitramfs/myshell/myshell.c::parse_command`，物理行 185–185；角色 `C_PARSE_RETURN`。

[VERIFIED myinitramfs/myshell/myshell.c::parse_command]
```c
	return find_cmd(**argv);
```

## E140-076 · 使用 index 表项分派 argc/argv

`myinitramfs/myshell/myshell.c::run_cmd`，物理行 139–141；角色 `C_RUN_CMD`。

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
```

## E140-077 · 顶层 musl-gcc 编译器路径声明

`myinitramfs/CMakeLists.txt::set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)`，物理行 41–41；角色 `C_MUSL_CFG`。

[VERIFIED myinitramfs/CMakeLists.txt::set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)]
```cmake
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

## E140-078 · 顶层加入 myinit 子目录

`myinitramfs/CMakeLists.txt::add_subdirectory(myinit)`，物理行 47–47；角色 `C_TOP_SUBDIR`。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myinit)]
```cmake
add_subdirectory(myinit)
```

## E140-079 · init 安装到 boot 声明

`myinitramfs/CMakeLists.txt::install(TARGETS init DESTINATION boot)`，物理行 50–50；角色 `C_INSTALL`。

[VERIFIED myinitramfs/CMakeLists.txt::install(TARGETS init DESTINATION boot)]
```cmake
install(TARGETS init DESTINATION boot)
```

## E140-080 · init 可执行目标声明

`myinitramfs/myinit/CMakeLists.txt::add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})`，物理行 4–4；角色 `C_INIT_TARGET`。

[VERIFIED myinitramfs/myinit/CMakeLists.txt::add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})]
```cmake
add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
```

## E140-081 · sh 可执行目标声明

`myinitramfs/myshell/CMakeLists.txt::add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})`，物理行 4–4；角色 `C_SH_TARGET`。

[VERIFIED myinitramfs/myshell/CMakeLists.txt::add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})]
```cmake
add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
```

## E140-082 · 独立 mylib 加入 csu

`myinitramfs/mylib/CMakeLists.txt::add_subdirectory(csu)`，物理行 2–2；角色 `C_LIB_SUBDIR`。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory(csu)]
```cmake
add_subdirectory(csu)
```

## E140-083 · 独立 mycrt 静态库声明

`myinitramfs/mylib/csu/CMakeLists.txt::add_library(mycrt STATIC)`，物理行 4–4；角色 `C_MYCRT`。

[VERIFIED myinitramfs/mylib/csu/CMakeLists.txt::add_library(mycrt STATIC)]
```cmake
add_library(mycrt STATIC)
```

## E140-084 · 顶层 MUSL_PATH 的固定文本

`myinitramfs/CMakeLists.txt::set(MUSL_PATH /home/cheyh/projs/musl/build)`，物理行 40–40；角色 `C_MUSL_PATH`。

[VERIFIED myinitramfs/CMakeLists.txt::set(MUSL_PATH /home/cheyh/projs/musl/build)]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
```

## E140-085 · 顶层头文件路径 include 声明

`myinitramfs/CMakeLists.txt::include_directories("${MUSL_INC_PATH}")`，物理行 46–46；角色 `C_INCLUDE`。

[VERIFIED myinitramfs/CMakeLists.txt::include_directories("${MUSL_INC_PATH}")]
```cmake
include_directories("${MUSL_INC_PATH}")
```

## E140-086 · 顶层加入 myshell

`myinitramfs/CMakeLists.txt::add_subdirectory(myshell)`，物理行 48–48；角色 `C_TOP_SH`。

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory(myshell)]
```cmake
add_subdirectory(myshell)
```

## E140-087 · sh 安装到 boot 声明

`myinitramfs/CMakeLists.txt::install(TARGETS sh DESTINATION boot)`，物理行 51–51；角色 `C_INSTALL_SH`。

[VERIFIED myinitramfs/CMakeLists.txt::install(TARGETS sh DESTINATION boot)]
```cmake
install(TARGETS sh DESTINATION boot)
```

## E140-088 · 独立 mylib 加入 myglibc

`myinitramfs/mylib/CMakeLists.txt::add_subdirectory(myglibc)`，物理行 1–1；角色 `C_LIB_GLIBC`。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory(myglibc)]
```cmake
add_subdirectory(myglibc)
```

## E140-089 · 独立 mylib 加入 util

`myinitramfs/mylib/CMakeLists.txt::add_subdirectory(util)`，物理行 3–3；角色 `C_LIB_UTIL`。

[VERIFIED myinitramfs/mylib/CMakeLists.txt::add_subdirectory(util)]
```cmake
add_subdirectory(util)
```

## E140-090 · mycrt C 源声明

`myinitramfs/mylib/csu/CMakeLists.txt::target_sources(mycrt PRIVATE ${CSU_C_SRCS})`，物理行 6–6；角色 `C_MYCRT_C`。

[VERIFIED myinitramfs/mylib/csu/CMakeLists.txt::target_sources(mycrt PRIVATE ${CSU_C_SRCS})]
```cmake
target_sources(mycrt PRIVATE ${CSU_C_SRCS})
```

## E140-091 · mycrt ASM 源声明

`myinitramfs/mylib/csu/CMakeLists.txt::target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})`，物理行 7–7；角色 `C_MYCRT_ASM`。

[VERIFIED myinitramfs/mylib/csu/CMakeLists.txt::target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})]
```cmake
target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})
```

## E140-092 · AT_FDCWD 路径中 bprm filename 取 filename->name

`mykernel/sched/forkexec/exec.c::alloc_bprm`，物理行 1117–1119；角色 `C_FILENAME_ALIAS`。

[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	if (fd == AT_FDCWD || filename->name[0] == '/') {
		bprm->filename = filename->name;
	} else {
```

## E140-093 · 用户数 argv 与零计数警告调用点

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1322–1325；角色 `D_U_WARN`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(argv, MAX_ARG_STRINGS);
	if (retval == 0)
		pr_warn_once("process '%s' launched '%s' with NULL argv:"
				" empty string added\n", current->comm, bprm->filename);
```

## E140-094 · 用户 argc 守卫与赋值

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1326–1328；角色 `D_U_COUNT`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	if (retval < 0)
		goto out_free;
	bprm->argc = retval;
```

## E140-095 · 用户环境计数与赋值

`mykernel/sched/forkexec/exec.c::do_execveat_common`，物理行 1330–1333；角色 `D_U_ENV_COUNT`。

[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	retval = count(envp, MAX_ARG_STRINGS);
	if (retval < 0)
		goto out_free;
	bprm->envc = retval;
```

## E140-096 · 限额起始三分之四 _STK_LIM

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行 268–268；角色 `D_BASE_LIMIT`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = _STK_LIM / 4 * 3;
```

## E140-097 · 与 ARG_MAX 取最大值

`mykernel/sched/forkexec/exec.c::bprm_stack_limits`，物理行 274–274；角色 `D_LIMIT_MAX`。

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_stack_limits]
```c
	limit = max_t(ulong, limit, ARG_MAX);
```

## E140-098 · vma 局部声明未带初始化表达式

`mykernel/fs/vfs/binfmt_elf.c::create_elf_tables`，物理行 145–145；角色 `A_VMA_DECL`。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	vma_s *vma;
```

## 证据边界

- create_elf_tables 的 vma 局部声明及所读体中的查找语句（第145、268行，后者为注释）另作定位；第25批已裁定实际前置门未证。A、C 指针表独立给定有效存储，不能反推生产路径成功。
- _start 的 while 是空循环体且固定读取 argv[argc]；三例给定该槽为 NULL，因此只检测一次。没有据源码声明推出实际 CRT 链接或入口 ABI。
- 旧139的92/91、旧138的47/46计量保留为历史核收范围；新机器索引将每个旧 ID 与原位置和修订关系分别列出。
