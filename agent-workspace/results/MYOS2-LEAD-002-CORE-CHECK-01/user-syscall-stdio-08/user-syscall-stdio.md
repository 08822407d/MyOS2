---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SYSCALL-STDIO-08
record_type: user_syscall_stdio_source_guide
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 ab3b4b86662c；主线 agent/MYOS2-LEAD-002 91753f8155ee（均为短 SHA）"
inputs_read: "见 evidence.md §2"
status: final_for_user_syscall_stdio_08
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
open_questions:
  - "init 与 sh 链接的 musl 在仓库外，freopen、printf、fflush、read、write 发出哪些系统调用仍是 GAP-U10。"
  - "pid 1 的 fd 表中各槽在运行时的内容取决于 slab 对象原有数据。"
  - "释放后的 file 对象会被改写哪些字段、何时被重新分配，没有追到分配器内部。"
---

# 用户态文件操作接到系统调用边界：init/libc、分发、fd 与控制台（CORE-USER-SYSCALL-STDIO-08）

本文接在 07 的 `user-boundaries-07/user-entry-console.md` 后面，回答“用户程序的文件与标准输入输出请求，在本仓库里能追到哪里”。所有结论来自 `time a039d9803ade` 的源码与构建文本，**没有编译或运行任何东西**。源码能讲清楚的部分写到源码表达式为止；libc 内部、实际的系统调用序列和机器上的结果一律记为仓库外或 NOT_RUN。

机器可读的事实在 `facts-and-boundaries.yaml`；本批新发现的普通问题在 `deferred-findings.yaml`（US-01…US-05，另向 07 的 UB-01、UB-05 和 05 的 UV-09、UV-10、UV-11 追加证据），当前都不需要 Owner 决策。

**编号约定：** 不带前缀的 UV-xx、GAP-Uxx、DRU-xx、A-Cx-xx、Qxx 指 CORE-USER-VFS-BASELINE-05；UB-xx、AB/QB/SB、FP-x、NVB-x 指 CORE-USER-BOUNDARIES-07；US-xx 指本批 `deferred-findings.yaml`；AS/QS/SS/VT/FL/PU/ES/NVS 指本批 `facts-and-boundaries.yaml`。锚点编号 AS3-12、AS3-36 空缺：这两处与 05 已有锚点（A-C4-28、A-C4-25）是同一组行，直接引用 05，不另编号。

## 0. 结论

1. **用户程序与 libc 的边界在哪里？** 仓库里实际构建的用户程序只有 `myinit/initd.c` 与 `myshell/`。它们只调用 libc 符号（freopen、printf、fflush、open、read、write 等），没有直接发出系统调用的写法。构建文本把 C 编译器指定为仓库外 `/home/cheyh/projs/musl/build` 下的 musl-gcc，并按 `LINK_TYPE=static` 静态链接（这一设置写在 `project()` 之后，实际生效的编译器只凭文本不能确定）；仓库内的旧 libc `mylib/` 不在构建中。无论哪个编译器生效，libc 都在仓库外，所以在仓库内，追踪止于 libc 符号；GAP-U10 保持 open（§2、§8）。
2. **系统调用怎样进入、分发和返回？** `syscall_init` 把 MSR_LSTAR 设为 `entry_SYSCALL_64`。入口把号压为 orig_ax 和 ax，`do_syscall_64` 把号转为无符号，小于 453 才进入由 `syscalls_64.h` 展开的 switch，经 `__x64_sys_X` → `__se_sys_X` → `__do_sys_X` 调到处理函数，返回值写进 `regs->ax`，再落入 07 描述的 `syscall_return_via_sysret`。表中 41 项全部有唯一的定义；号不小于 453 时用户拿回的就是原号，表内缺项会原地循环（§3）。
3. **fd 怎样分配、复制、跨 exec？** open 取最低的空槽；read 用不带引用标志的取法；write、writev 与 fcntl 用的取法不加引用却在返回前 fput，write、writev 每次、fcntl 除成功的 F_DUPFD 外每次都让 f_count 少一（US-01）。dup、dup2 是空壳，dup3 不在表中，唯一能复制 fd 的是 fcntl(F_DUPFD)，而它的上界是 1024、fd 表只有 256 项（US-02）。exec 保留全部 fd，close-on-exec 没有实现（US-03）。内核不为 pid 1 打开任何 fd，pid 1 的表是从全空的 init_files 复制来的，各槽没有被写入（§4）。
4. **控制台怎样接上？** 打开 `/dev/console` 时 `chrdev_open` 把 f_op 从 `def_chr_fops` 换成 `console_fops`（07 §4.2）。之后 read(2) 到 `myos_tty_read`；write(2) 在 `vfs_write` 返回 -EINVAL；writev(2) 逐段调 `myos_tty_write`。write 与 writev 都在返回前多做一次 fput。libc 实际用哪一个，仓库里查不到（§4.6）。

## 1. 建议的源码阅读顺序（任务书允许的阅读顺序，不是学习路线或课程排序）

1. `myinitramfs/CMakeLists.txt`、`make_install.sh`、`scripts/make_install_initranfs.sh`：用户程序怎样构建、链接哪个 libc。
2. `myinitramfs/myinit/initd.c` 与 `myshell/myshell.c`：用户程序实际调用了什么。
3. `mykernel/arch/x86_64/kernel/cpu/common.c::syscall_init`、`arch/x86_64/entry/entry_64.S::entry_SYSCALL_64`、`arch/x86_64/entry/common.c::do_syscall_64`、`arch/x86_64/entry/syscall_64.c`、`arch/x86_64/include/asm/syscall_wrapper.h`、`syscalls_64.h`、`unistd_64.h`。
4. `mykernel/fs/syscall.c`、`fs/vfs/open.c`、`fs/vfs/read_write.c`、`include/linux/fs/file.h`、`fs/vfs/file.c`、`fs/vfs/file_table.c`、`fs/vfs/fcntl.c`。
5. `mykernel/init/main.c::rest_init`、`sched/forkexec/fork.c::copy_files`、`sched/forkexec/exec.c::begin_new_exec`。
6. 最后回到 07 的 `user-entry-console.md` §4，把 console 的两张 f_op 表与上面的系统调用入口连起来。

## 2. C01：用户态程序和 libc 的实际边界

### 2.1 三栏表

| 问题 | 仓库内已证实 | 仓库外依赖 | 未知 |
|---|---|---|---|
| init 怎样建立标准流 | `freopen("/dev/console", "r"/"w"/"w", stdin/stdout/stderr)`，随后 `printf`（AS1-06、AS1-07） | freopen 的实现在 musl | freopen 发出哪些系统调用、参数是什么 |
| sh 怎样用控制台 | 启动时先调用 tests.c 的 `test_PrintDateTime`（gettimeofday、localtime、strftime、printf，QS-36）；另以 `open("/dev/console", O_RDWR)` 得到 `console_fd`，逐字节 `read`、`write` 回显；提示符走 `printf` + `fflush(stdout)`（AS1-09～AS1-11） | open/read/write/printf/fflush 的实现在 musl | 这些函数各自发出哪个系统调用 |
| 能否在仓库源码中追到 printf、freopen、read、write、writev | 能追到用户源码对 freopen、printf、fflush、read、write 等 **libc 符号** 的调用；writev 在用户源码中没有出现（QS-37），只可能来自 libc 内部；用户源码中也没有 `syscall`、`SYS_*` 之类的直接写法（QS-01～QS-04） | 符号的定义在 musl | 从 libc 符号到系统调用号这一步 |
| 用户程序链接哪个 libc | 构建文本把编译器指定为 `/home/cheyh/projs/musl/build/exec/bin/musl-gcc`（第 41 行，在第 3 行 `project()` 之后），`LINK_TYPE=static` 时加 `-static`；构建只加入 myinit、myshell（AS1-01～AS1-05、AS1-12、QS-05、QS-09） | musl 本体与其构建配置 | 实际生效的编译器与 libc；musl 的版本；镜像中的 `/boot/init`、`/boot/sh` 是哪一次构建的产物 |
| 仓库内的 `mylib/` | 有自己的 syscall 包装（SYSFUNC_DEF），但不在构建中，构建与安装脚本没有引用它（QS-06、QS-07） | — | — |
| 历史材料 | changelog 记“已移植 musl、静态链接”；user-guide 的 User-Application.md 仍按 mylib 描述构建，Syscall-Table.md 说用户 API 在 mylib 的 syscall.c（QS-10）；按公约只可引用，不当现状 | — | — |

### 2.2 构建边界

构建文本把 C 编译器指定为仓库外的 musl-gcc：

[VERIFIED myinitramfs/CMakeLists.txt::CMAKE_C_COMPILER]
```cmake
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)
```

这一行写在启用 C 语言的 `project()` 之后：

[VERIFIED myinitramfs/CMakeLists.txt::project]
```cmake
project(MyOS2_Initramfs ASM C)
```

静态链接由 `LINK_TYPE` 决定，总装脚本把它设为 static：

[VERIFIED myinitramfs/CMakeLists.txt::LINK_TYPE]
```cmake
if (${LINK_TYPE} STREQUAL "static")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()
```

[VERIFIED make_install.sh::LINK_TYPE]
```bash
LINK_TYPE=static
# LINK_TYPE=dynamic
```

顶层构建只加入两个子目录，`mylib` 不在其中：

[VERIFIED myinitramfs/CMakeLists.txt::add_subdirectory]
```cmake
include_directories("${MUSL_INC_PATH}")
add_subdirectory(myinit)
add_subdirectory(myshell)
```

按构建文本，init 与 sh 里的 printf、freopen、read、write 由仓库外的 musl 解析。但 `CMAKE_C_COMPILER` 写在 `project()` 之后，CMake 是否实际采用它只凭文本不能确定，所以“链接的是 musl”是根据构建文本（以及 musl 头文件目录、安装脚本中的 musl 路径）作出的推断；无论哪个编译器生效，libc 都在仓库外。仓库里没有 musl 的版本号或构建记录；`documents/useful_cmds.md` 中的 configure 命令只是历史材料。仓库另有历史主题分支 `libc`，repo-map §4 把它列为既往轨迹、无需阅读，本批没有读，它也不是当前构建引用的路径。

### 2.3 init 与 sh 实际调用了什么

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);
```

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	if (rv == 0) {
		pid_t pid = getpid();
		// printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
```

sh 的 main 先调用同目录 tests.c 中的 `test_PrintDateTime`（gettimeofday、localtime、strftime、printf，失败时 perror，QS-36），然后自己再打开一次控制台，并用它逐字节读写：

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	console_fd = open("/dev/console", O_RDWR);
```

[VERIFIED myinitramfs/myshell/myshell.c::read_line]
```c
	while(read(console_fd, &key, 1) == 1 && key != 4)
	{
		write(console_fd, "\b", 1);
```

这些都是对 libc 函数的调用。按任务书，本批到这里停止：不猜 musl 的 `write()` 会发 write(2) 还是别的，也不把通用 Linux/glibc 的行为当成 MyOS2 现状。§4.6 只列出“如果发出某个系统调用，内核侧会怎样”。

### 2.4 仓库内旧 libc 与历史材料

`myinitramfs/mylib/myglibc/syscall/syscall.c` 用 `SYSFUNC_DEF(open)`、`(read)`、`(write)`…`(putstring)` 生成把 `__NR_*` 装入 rax、再执行 `syscall` 指令的函数（QS-07），451 号 putstring 的用户态包装只在这里（QS-17）。mylib 中与 freopen、writev、dup2、dup3、fflush 有关的行全部在注释里（QS-08）。`user-guide/User-Application.md`（2024-04-16）还按 mylib 描述用户程序构建，`Syscall-Table.md` 说用户 API 列在 mylib 的 syscall.c，而 `changelog.md`（2024-09-05）记已改为静态链接 musl。当前构建文本与后者一致；mylib 不能用来推断 init、sh 的系统调用。

## 3. C02：系统调用入口、分发与返回

### 3.1 一次系统调用经过的代码

**入口绑定。** 配置面没有定义 `CONFIG_INTEL_X64_GDT_LAYOUT`（QS-12），`syscall_init` 走 `#else` 分支：

[VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c::syscall_init]
```c
		wrmsr(MSR_STAR, 0, (__USER32_CS << 16) | __KERNEL_CS);
		wrmsrl(MSR_LSTAR, (unsigned long)entry_SYSCALL_64);
```

**入口建帧。** rax 里的系统调用号被压了两次：一次作为 orig_ax，一次随 `PUSH_REGS` 作为 ax：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	pushq		%rax					/* pt_regs->orig_ax */
	pushq		$0x80					/* pt_regs->irq_nr */
	PUSH_REGS
```

Linux 在这里把 ax 预置为 -ENOSYS，这一行被注释了（QS-18）：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	sti

	// PUSH_AND_CLEAR_REGS rax=$-ENOSYS
```

然后把帧地址和号交给 C 层：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	movq		%rsp,		%rdi
	/* Sign extend the lower 32bit as syscall numbers are treated as int */
	movslq		%eax,		%rsi
```

**检查与分派。** 号转成无符号后与 `__NR_syscalls`（453，只在 `__KERNEL__` 下定义，AS2-12、AS2-20）比较：

[VERIFIED mykernel/arch/x86_64/entry/common.c::do_syscall_64]
```c
	uint unr = nr;
	if (unr < __NR_syscalls) {
		// unr = array_index_nospec(unr, NR_syscalls);
		regs->ax = x64_sys_call(regs, unr);
```

`x64_sys_call` 是一个 switch，case 由同一个表头展开：

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

**包装与取参数。** 每个 `MYOS_SYSCALL_DEFINEn(name, …)` 生成 `__x64_sys_name(regs)` → `__se_sys_name(regs)` → `__do_sys_name(参数)`，参数依次取自 di、si、dx、r10、r8、r9：

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__SYS_STUBx]
```c
	#define __SYS_STUBx(abi, name, ...)							\
				long __##abi##_##name(const pt_regs_s *regs);	\
				long __##abi##_##name(const pt_regs_s *regs)	\
				{												\
					return __se_##name(regs);					\
```

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__X86_64_MAP4]
```c
	#define __X86_64_MAP4(m, t1, a1, t2, a2, t3, a3, t4, a4)					\
				m(t1, regs->di), m(t2, regs->si), m(t3, regs->dx),				\
				m(t4, regs->r10)
```

**返回。** 处理函数的返回值经 `x64_sys_call` 写入 `regs->ax`（SS-15 核对函数体只写这一次）；随后 `syscall_exit_to_user_mode`，并落入 07 的 `syscall_return_via_sysret`（07 的 FP-3，SB-01）。返回段把帧中的 ax 恢复到 rax，用户态在 rax 中看到返回值。

### 3.2 号的检查结果

| 号的情况 | 源码中的处理 | 用户看到的 rax |
|---|---|---|
| 在表中（41 项之一） | switch 命中 case，调处理函数 | 处理函数的返回值；没有活动 return 的处理函数（dup、dup2、wait4；exit 在 do_exit 之后也没有 return）若返回，返回值没有定义 |
| 小于 453 但不在表中（如 fstat 5、readv 19、openat 257、dup3 292，QS-35） | default 分支 `__x64_sys_ni_syscall`，打印 orig_ax 后 `while (1)` | 不返回（UB-09） |
| 452（`__NR_syscalls - 1`） | `sys_no_syscall`，打印后返回 -ENOSYS | -ENOSYS |
| 不小于 453，或负号（转成无符号后很大） | 不进 switch，ax 不被写 | 原 rax，也就是号本身（UB-09） |

[VERIFIED mykernel/arch/x86_64/entry/common.c::__x64_sys_ni_syscall]
```c
	color_printk(RED, BLACK, "Syscall not implemented: %d\n", regs->orig_ax);
	while (1);
```

[VERIFIED mykernel/arch/x86_64/myos/syscall.c::sys_no_syscall]
```c
	color_printk(RED, BLACK, "no_system_call is calling\n");
	return -ENOSYS;
```

### 3.3 系统调用表（由源码重算，VT-SYSTAB）

`syscalls_64.h` 有 41 个活动表项，号全部唯一且小于 453，每个处理函数恰有一处活动定义。下表“提示宏”指函数体中有活动的 `ALERT_DUMMY_SYSCALL`；它的提示开关固定为 0，本身什么也不做，也不返回（AS2-17、AS2-18），有它不等于空实现。“return”是函数体中活动 return 的个数。与本批有关的行加粗。

| 号 | 处理函数 | 定义位置（mykernel/ 下） | 参数 | 提示宏 | return | 说明 |
|---|---|---|---|---|---|---|
| 0 | **sys_read** | `fs/syscall.c:11` | 3 | — | 1 | console 读（§4.6） |
| 1 | **sys_write** | `fs/syscall.c:17` | 3 | — | 1 | 被 FMODE_CAN_WRITE 拦截（UB-01、US-01） |
| 2 | **sys_open** | `fs/syscall.c:38` | 3 | — | 1 | 打开与 fd 安装（§4.2） |
| 3 | **sys_close** | `fs/syscall.c:45` | 1 | — | 2 | 直接 kfree（UV-10） |
| 8 | **sys_lseek** | `fs/syscall.c:75` | 3 | — | 3 | 不查空槽（UV-11 追加） |
| 9 | sys_mmap | `mm/vm_map/mmap.c:1312` | 6 | — | 3 |  |
| 10 | sys_mprotect | `arch/x86_64/myos/syscall.c:185` | 3 | 有 | 1 | 只有提示宏与 return 0 |
| 11 | sys_munmap | `mm/vm_map/mmap.c:1342` | 2 | — | 1 |  |
| 12 | sys_brk | `arch/x86_64/myos/syscall.c:39` | 1 | — | 2 |  |
| 13 | sys_rt_sigaction | `lock_IPC/syscall.c:22` | 4 | — | 5 |  |
| 14 | sys_rt_sigprocmask | `lock_IPC/syscall.c:55` | 4 | — | 5 |  |
| 15 | sys_rt_sigreturn | `arch/x86_64/lock_IPC/signal/signal_64.c:244` | 0 | — | 2 |  |
| 16 | **sys_ioctl** | `arch/x86_64/myos/syscall.c:131` | 3 | 有 | 1 | 空实现，总返回 0（US-04） |
| 20 | **sys_writev** | `fs/syscall.c:95` | 3 | — | 1 | console 写（UB-01、US-01） |
| 24 | sys_sched_yield | `sched/syscall.c:22` | 0 | — | 1 |  |
| 32 | **sys_dup** | `fs/syscall.c:103` | 1 | 有 | 0 | 空壳（UB-10） |
| 33 | **sys_dup2** | `fs/syscall.c:121` | 2 | 有 | 0 | 空壳（UB-10） |
| 39 | sys_getpid | `sched/syscall.c:45` | 0 | — | 1 |  |
| 57 | sys_fork | `sched/syscall.c:51` | 0 | — | 1 | 按值复制 fd 表（UV-09） |
| 59 | sys_execve | `sched/forkexec/exec.c:1460` | 3 | — | 1 | 保留全部 fd（US-03） |
| 60 | sys_exit | `sched/syscall.c:68` | 1 | 有 | 0 | 提示宏后调 do_exit；无 return（05 C6） |
| 61 | sys_wait4 | `sched/syscall.c:76` | 4 | — | 0 | 函数体为空（UV-15） |
| 62 | sys_kill | `lock_IPC/syscall.c:91` | 2 | 有 | 1 | 提示宏后有实际处理 |
| 72 | **sys_fcntl** | `fs/syscall.c:140` | 3 | — | 1 | F_DUPFD（US-02、US-03） |
| 79 | sys_getcwd | `fs/vfs/d_path.c:196` | 2 | — | 1 |  |
| 80 | sys_chdir | `fs/vfs/open.c:272` | 1 | — | 1 |  |
| 83 | sys_mkdir | `fs/vfs/namei.c:999` | 2 | — | 1 |  |
| 84 | sys_rmdir | `fs/vfs/namei.c:1092` | 1 | — | 1 |  |
| 85 | sys_creat | `fs/vfs/namei.c:932` | 2 | — | 1 |  |
| 87 | sys_unlink | `fs/vfs/namei.c:1197` | 1 | — | 1 |  |
| 96 | sys_gettimeofday | `time/syscall.c:12` | 2 | — | 2 |  |
| 110 | sys_getppid | `sched/syscall.c:115` | 0 | — | 1 |  |
| 158 | sys_arch_prctl | `arch/x86_64/myos/syscall.c:192` | 2 | — | 1 |  |
| 169 | sys_reboot | `arch/x86_64/myos/syscall.c:160` | 2 | — | 1 |  |
| 202 | sys_futex | `lock_IPC/syscall.c:104` | 6 | — | 3 |  |
| 217 | sys_getdents64 | `fs/vfs/readdir.c:132` | 3 | — | 2 |  |
| 218 | sys_set_tid_address | `arch/x86_64/myos/syscall.c:203` | 1 | — | 1 |  |
| 228 | sys_clock_gettime | `time/syscall.c:33` | 2 | — | 2 |  |
| 231 | sys_exit_group | `sched/syscall.c:133` | 1 | 有 | 1 | 提示宏后调 do_group_exit（05 C6） |
| 451 | **sys_myos_putstring** | `arch/x86_64/myos/syscall.c:30` | 1 | — | 1 | 451：MyOS2 自有（US-05） |
| 452 | **sys_no_syscall** | `arch/x86_64/myos/syscall.c:24` | 0 | — | 1 | 452：返回 -ENOSYS |

被注释的表项（不参与分派）：newfstat、vfork、getdents、link、getuid（2 行）、geteuid（2 行）、init_module、delete_module、set_robust_list、rseq（共 12 行）。

### 3.4 注册表、配置项与活动分发的区别

- **活动分发**：只有 `x64_sys_call` 的 switch（AS2-08）。它的 case 来自 `syscalls_64.h` 的第三次展开（AS2-07）。
- **注册表**：`sys_call_table[]` 用同一个头文件以指定初始化器展开，但除定义与一处 extern 声明外，没有任何活动使用（QS-14），不参与分派。
- **配置项**：内核以 `-D__KERNEL__` 与 `-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER` 编译（AS2-20、AS2-19）；前者让 `__NR_syscalls` 有定义，后者对应 pt_regs 形式的包装。
- **只由名字推测的关系**：`user-guide/Syscall-Table.md` 说用户 API 在 `mylib/.../syscall.c`，那是历史描述（QS-10）；本批的表只来自 `syscalls_64.h` 与定义行。

## 4. C03：文件描述符、dup/exec 与 console 的衔接

### 4.1 pid 1 从哪里得到 fd

kernel_init（之后 exec 成为 pid 1）由 `kernel_thread` 以 CLONE_FS 调用建立（kernel_thread 另加 CLONE_VM 与 CLONE_UNTRACED），不带 CLONE_FILES：

[VERIFIED mykernel/init/main.c::rest_init]
```c
	pid = kernel_thread(kernel_init, NULL, "kthd: kernel_init", CLONE_FS);
```

所以 `copy_files` 用 `dup_fd` 为它复制一张自己的表：

[VERIFIED mykernel/sched/forkexec/fork.c::copy_files]
```c
	newf = dup_fd(oldf, NR_OPEN_MAX, &error);
	if (!newf)
		goto out;

	tsk->files = newf;
```

被复制的 `init_files` 槽全空，fd_count 为 0：

[VERIFIED mykernel/fs/vfs/file.c::init_files]
```c
	.file_lock		= __SPIN_LOCK_UNLOCKED(init_files.file_lock),
	.fd_count		= 0,
```

`dup_fd` 把新表的 fd_count 设为 256，却只写父表中非空的槽（05 UV-09），所以 pid 1 的 256 个槽都没有被写入。`console_on_rootfs` 只在注释中出现（QS-19），内核不替 pid 1 打开控制台。init 进入 main 时 fd 0、1、2 的内容就是这些没有写入的槽——如果 slab 给出的是清零的内存，它们就是空的；否则不确定（PU-01）。init 之后用 freopen 建立标准流，正是这一步在仓库外。

### 4.2 FD 生命周期表

| 阶段 | 代码路径 | 槽位 | 引用计数 | 相关条目 |
|---|---|---|---|---|
| FL-01 pid 1 初始表 | `kernel_thread(…, CLONE_FS)` → `copy_files` → `dup_fd(init_files, 256)` | 256 个槽都没有被写入；内核不打开 console | 没有 file | UV-09 |
| FL-02 open | `sys_open` → `do_sys_open(0, …)` → `do_sys_openat2` → 最低空槽 → `do_filp_open` → `fd_install` | 写入最低空槽（alloc_fd 不预先占位） | 新 file 的 f_count = 1 | UV-04、UV-11、UB-05 |
| FL-03 read | `ksys_read` → `myos_fdget_pos`（flags=0）→ `vfs_read` | 按下标读，不查上界 | 不变 | UV-11 |
| FL-04 write / writev | `ksys_write`、`do_writev` → `fdget_pos` … `fdput_pos` | 按下标读，不查上界 | 每次减一而不先加一；1 → 0 时释放，槽仍指着它（fork 继承来的 file 走 UV-09 的跨缓存路径） | **US-01**、UB-01、UV-09 |
| FL-05 fcntl / dup 系列 | fcntl：`fdget_raw` → `do_fcntl` → `f_dupfd` → `fdput`；dup、dup2 空壳；dup3 不在表中 | F_DUPFD 在 [arg, 1024) 中找空槽，但表只有 256 项；得到 fd ≥ 256 时 close、lseek 返回 -EBADF | 成功时 get_file 加一、fdput 减一，净值不变，两槽共享一个 file；F_GETFL、失败的 F_DUPFD 与不支持的命令只减一 | **US-01**、**US-02**、UB-10、UB-09 |
| FL-06 lseek / close | 直接按下标访问 | 都查 fd < fd_count；lseek 不查空槽；close 清槽 | close 不看 f_count，直接 kfree | UV-10、UV-11 |
| FL-07 fork | `copy_files` → `dup_fd` | 父表非空槽各复制一个新 file（按值），空槽不写 | 副本的 f_count 按值照抄 | UV-09 |
| FL-08 exec | `begin_new_exec` → `unshare_files` | 表未共享时原样保留，不关闭任何 fd | 不变 | **US-03** |
| FL-09 exit | `exit_files` → `put_files_struct` → `filp_close` → `fput` | 整表释放 | 每个非空槽 fput 一次 | UV-09、UV-15 |

### 4.3 取 file 的两种写法与引用计数（US-01）

write、writev、fcntl 用的 `fdget_pos`、`fdget_raw` 都是 `__to_fd`：直接取槽，不加引用，却返回 flags=3：

[VERIFIED mykernel/include/linux/fs/file.h::__to_fd]
```c
		files_struct_s *files = current->files;
		file_s *file = files->fd_array[v];
		// return (fd_s){(file_s *)(v & ~3),v & 3};
		return (fd_s){file, 3};
```

flags 的最低位就是 FDPUT_FPUT，`fdput` 据此调用 fput：

[VERIFIED mykernel/include/linux/fs/file.h::FDPUT_FPUT]
```c
	#define FDPUT_FPUT			1
```

[VERIFIED mykernel/include/linux/fs/file.h::fdput]
```c
		if (fd.flags & FDPUT_FPUT)
			fput(fd.file);
```

于是 write 不论 `vfs_write` 成败，返回前都会 fput 一次：

[VERIFIED mykernel/fs/vfs/read_write.c::ksys_write]
```c
		ret = vfs_write(filp, buf, count, ppos);
		if (ret >= 0 && ppos)
			filp->f_pos = pos;
		fdput_pos(f);
```

而 fput 在计数归零时调用 `kmem_cache_free(filp_cachep, …)`：

[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
	if (atomic_long_dec_and_test(&file->f_count)) {
```

读路径用的是另一个写法 `myos_fdget_pos`，flags=0，`fdput_pos` 不会 fput：

[VERIFIED mykernel/fs/vfs/file.c::myos_fdget_pos]
```c
	file_s *fp = curr->files->fd_array[fd];
	
	return (fd_s){.file = fp, .flags = 0};
```

由源码重算（VT-FDLIMITS）：`__to_fd` 的 flags 与 FDPUT_FPUT 相与为真，`myos_fdget_pos` 为假。所以一个 f_count 为 1 的 fd（刚打开的就是），第一次 write(2)、writev(2)，或第一次除成功的 F_DUPFD 之外的 fcntl(2) 之后，file 就被释放，而槽仍指向它；之后同一 fd 上的读写、lseek、close 都在访问已释放的对象，close 还会再 kfree 一次。成功的 F_DUPFD 先 get_file，与 fdput 相抵；F_GETFL 与不支持的命令（default 分支只打印，AS3-39）同样只剩这次 fput。fork 继承来的 fd（例如 sh 的 0、1、2）指向 dup_fd 用 kzalloc 复制出的 file，计数照抄父表，归零时这次 `kmem_cache_free(filp_cachep, …)` 走的是 05 UV-09 所述的跨缓存路径。这些都是静态推导，实际后果取决于释放后的内存被怎样改写或重新分配（NOT_RUN）。

### 4.4 dup、dup2、dup3 与 fcntl（US-02、US-03）

dup、dup2 只有提示宏，没有 return（07 UB-10、VT-SYSTAB）；dup3 不在表中，会进缺项循环（UB-09）。仓库里唯一能复制 fd 的是 fcntl 的 F_DUPFD 与 F_DUPFD_CLOEXEC：

[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	case F_DUPFD:
		err = f_dupfd(argi, filp, 0);
		break;
	case F_DUPFD_CLOEXEC:
		err = f_dupfd(argi, filp, O_CLOEXEC);
```

`f_dupfd` 的上界取 RLIMIT_NOFILE：

[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	ulong nofile = rlimit(RLIMIT_NOFILE);
	int err;
	if (from >= nofile)
		return -EINVAL;
	err = alloc_fd(from, nofile, flags);
```

RLIMIT_NOFILE 的初值是 1024（AS3-06、AS3-07，fork 时按值复制，AS3-08），而 fd 表只有 256 项：

[VERIFIED mykernel/include/linux/kernel/fdtable.h::NR_OPEN_DEFAULT]
```c
	#define NR_OPEN_DEFAULT	256
```

[VERIFIED mykernel/include/linux/kernel/fdtable.h::struct files_struct]
```c
		unsigned long	fd_count;
		file_s __rcu	*fd_array[NR_OPEN_DEFAULT];
```

F_DUPFD 的起点在 256～1023 之间、或从起点到 255 的槽都非空时，`alloc_fd` 会读到表外，`fd_install` 可能写到表外（US-02）。这样得到的 fd 不小于 256：close、lseek 因 fd ≥ fd_count 返回 -EBADF，它无法被关闭，退出时也不在扫描范围内（QS-38）；read、write、writev、fcntl 则按下标越界访问。在表内找到槽时，它 `get_file` 并装入同一个 file，两槽共享一个对象，之后任一槽 close 都直接 kfree（05 UV-10 的前提就此成立）。fcntl 的 F_GETFD、F_SETFD、F_SETFL 分支被注释，落到 err 的初值 -EINVAL（SS-17、QS-26、QS-31）。

### 4.5 exec 与 close-on-exec（US-03）

exec 对 fd 表只做一件事：确保它不被共享。

[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/* Ensure the files table is not shared. */
	retval = unshare_files();
	if (retval)
		goto out;
```

`unshare_fd` 只在表的 refcount 大于 1 时才复制（AS3-34）；`do_close_on_exec` 的调用被注释（QS-25、05 Q14），`alloc_fd` 不使用传入的 flags（SS-22）。所以 O_CLOEXEC 无处保存，FD_CLOEXEC 无法设置，exec 保留全部 fd。对 init 的子进程 exec `/boot/sh` 来说，0、1、2 被保留正是标准流需要的结果。

### 4.6 console 衔接：从系统调用入口到 07 的两张 f_op 表

07 已经说明：`/dev/console` 节点的 `i_fop` 是 `def_chr_fops`，打开时 `chrdev_open` 把文件的 f_op 换成 `console_fops`（read = `myos_tty_read`，write = `myos_tty_write`，open = `myos_tty_open`）。把它接到系统调用入口：

| 系统调用（号） | 内核路径 | 落到 console 的哪里 | 结果（源码层面） |
|---|---|---|---|
| open(2)（2） | `sys_open` → `do_sys_openat2` → `do_filp_open` → `chrdev_open` | `myos_tty_open` 返回 1 | 换表在调用 open 之前完成；打开结果取决于缺 return 的上层（UV-04、UB-05） |
| read(2)（0） | `ksys_read` → `myos_fdget_pos` → `vfs_read`（只查 FMODE_READ，AS3-24） | `myos_tty_read` | 攒满 count 个非零键值才返回（UB-08）；引用计数不变 |
| write(2)（1） | `ksys_write` → `fdget_pos` → `vfs_write` | 到不了 `myos_tty_write` | 以写方式打开时返回 -EINVAL（UB-01），并多 fput 一次（US-01） |
| writev(2)（20） | `do_writev` → `fdget_pos` → `vfs_writev` | 逐段 `myos_tty_write` | 返回值是各段相加（SS-23）；直接用用户指针（UB-07）；多 fput 一次（US-01） |
| dup2(2)（33）、dup(2)（32） | 空壳 | — | 不复制，返回值没有定义（UB-10） |
| dup3(2)（292）、openat(2)（257）、fstat(2)（5）、readv(2)（19） | 不在表中 | — | 原地循环（UB-09） |
| fcntl(2)（72） | `fdget_raw` → `do_fcntl` → `fdput` | — | F_DUPFD 可用（US-02）；F_GETFD/F_SETFD/F_SETFL 返回 -EINVAL（US-03）；除成功的 F_DUPFD 外多 fput 一次（US-01） |
| ioctl(2)（16） | 空实现 | — | 总返回 0，不填写输出参数（US-04） |
| close(2)（3）、lseek(2)（8） | 直接按下标访问 | console 没有 llseek，lseek 返回 0 | close 直接 kfree（UV-10）；lseek 不查空槽 |
| putstring（451，MyOS2 自有） | `sys_myos_putstring` | 不经 console 节点，color_printk 把它原样交给 vsnprintf（AS2-21） | 用户串作格式串（US-05） |

这张表只说“发出某个系统调用时内核侧会怎样”。init 的 freopen 与 sh 的 printf、write 实际发出哪几行，取决于仓库外的 musl（GAP-U10），本批不作选择。特别保留 07 发现的 write(2) 与 writev(2) 的差异：它是内核侧事实，不能据此推出真实用户程序一定看到哪种输出。

## 5. 调用路径图

### 5.1 正常路径（源码可讲解）

```text
[C01] initd.c: freopen×3, printf ──┐            sh: open("/dev/console",O_RDWR), read/write, printf
                                   └─▶ libc 符号 ══▶ libc（仓库外；构建文本指定 musl）══▶ syscall 指令（仓库外）
[C02]                                                                              │ MSR_LSTAR
      entry_SYSCALL_64: 压 orig_ax=rax、PUSH_REGS(ax=rax)、sti、rsi=movslq eax ◀───┘
        └─▶ do_syscall_64: (uint)nr < 453 ? regs->ax = x64_sys_call(regs, nr)
              └─▶ switch(nr) case N ─▶ __x64_sys_X ─▶ __se_sys_X ─▶ __do_sys_X(di,si,dx,r10,r8,r9)
        └─▶ syscall_exit_to_user_mode ─▶ syscall_return_via_sysret（07 FP-3）─▶ 用户态 rax = 返回值
[C03] open ─▶ do_sys_openat2 ─▶ 最低空槽 + do_filp_open ─▶ chrdev_open: f_op = console_fops ─▶ fd_install
      read ─▶ myos_fdget_pos ─▶ vfs_read ─▶ myos_tty_read ─▶ 键盘缓冲
      writev ─▶ fdget_pos ─▶ vfs_writev ─▶ myos_tty_write ─▶ 帧缓冲 ─▶ fdput_pos（多一次 fput）
```

### 5.2 失败与未知边界（运行不可依赖）

```text
libc 符号 → 系统调用 ─▶ libc 在仓库外（构建文本指定 musl）.......................... GAP-U10
pid 1 的 fd 0/1/2 ─▶ dup_fd 不写空槽，内核不开 console ................................ PU-01、UV-09
write(2) ─▶ vfs_write：FMODE_CAN_WRITE 不成立 ─▶ -EINVAL ............................ UB-01
write/writev/fcntl（成功的 F_DUPFD 除外）─▶ fdget 不取引用 + fdput 必 fput ─▶ 首次后释放 ... US-01
fcntl(F_DUPFD, 256..1023) ─▶ alloc_fd 扫描越过 256 项的表 ........................... US-02
exec ─▶ 不处理 close-on-exec；F_SETFD 返回 -EINVAL .................................. US-03
ioctl ─▶ 总返回 0 ................................................................... US-04
putstring(451) ─▶ 用户串作内核格式串 ................................................ US-05
号 ≥ 453 ─▶ 返回原号；表内缺项 ─▶ while (1) ......................................... UB-09
dup/dup2 ─▶ 空壳，返回值未定义 ...................................................... UB-10
```

## 6. 仓库外依赖与 NOT_RUN 边界

- **仓库外依赖：** 构建文本指定的 `/home/cheyh/projs/musl/build`（musl-gcc、头文件、静态库；版本与配置没有记录；实际生效的编译器只凭文本不能确定）；构建产物与 objdump 输出（`build/`，脚本把 `initd_dasm.txt`、`sh_dasm.txt` 写在那里）；磁盘镜像中的 `/boot/init`、`/boot/sh`。
- **NOT_RUN：** 实际系统调用序列与参数、fd 表各槽的运行时内容、释放后对象的去向、控制台的实际输入输出、sysretq/swapgs 的硬件语义（05 GAP-U04 经 07 收窄后的剩余部分，07 NVB-4）。
- **需要授权才能继续的：** NVS-1（构建 + QEMU + 测试镜像，观察系统调用与 fd 状态）、NVS-2（读仓库外 musl 源码，接续 07 的 NVB-2）、NVS-3（读仓库外的构建产物或镜像中的二进制并反汇编），全部 NOT_RUN。

## 7. 与 07、05 的衔接

| 已有条目 | 本批的相交更新 | 状态 |
|---|---|---|
| 07 UB-01（write(2) 到不了 ->write） | write 与 writev 返回前都多做一次 fput；writev 把各段返回值相加；sh 的 read_line 用 write 回显（只是 libc 调用） | 只追加证据，原状态不变 |
| 07 UB-05（console 的 open 返回 1） | sh 自己以 O_RDWR 打开控制台，走同一条路径；do_filp_open 返回错误指针时 do_sys_openat2 不装 fd（getname 失败或取到负 fd 时也不装） | 只追加证据，原状态不变 |
| 07 init_console_reopen 的未知项“初始 fd 0/1/2 的状态” | 内核不开 console；pid 1 的槽未被写入 | narrowed（PU-01） |
| 07 init_console_reopen 的未知项“freopen 发出的系统调用” | 仍在仓库外 | open（PU-02） |
| 05 UV-09（fork 的文件表复制） | pid 1 自己的表也来自 dup_fd，且内核不开 console | 只追加证据 |
| 05 UV-10（close 与 fput） | f_dupfd 的调用者是 fcntl(F_DUPFD)，两槽共享 file 的前提可达；fget 只被 mmap 用 | 只追加证据；其未知项 closed_static（PU-04） |
| 05 UV-11（fd 边界） | fdget_pos/fdget_raw 同样不查上界；lseek 不查空槽；fd_count 的取值 | 只追加证据；其未知项 closed_static（PU-03） |
| 07 FP-3（普通系统调用的帧） | 本批补上号的检查、switch 分派、包装与写回 ax 这一段 | 衔接，不改 07 |

## 8. 缺口状态

| 缺口 | 本批覆盖 | 静态已闭合 | 仍缺什么、在哪里 | 状态 |
|---|---|---|---|---|
| GAP-U10（musl 的系统调用选择） | 仓库内用户程序、构建清单、安装脚本、旧 libc、历史材料逐项检查；内核侧对每个候选系统调用的结果已列出 | 无：仓库源码不能闭合 libc 的选择 | 构建实际采用的编译器与 libc（文本指定 musl-gcc，写在 `project()` 之后）；musl 源码与版本（`/home/cheyh/projs/musl/build`，仓库外，无版本记录）；镜像中的实际二进制（`build/` 与镜像，仓库外）；历史分支 `libc`（未读，不是当前构建路径） | **open**（不变） |

## 9. 本批入库的普通问题（只列编号，正文见 deferred-findings.yaml）

US-01 fdget 系列不取引用却带 FDPUT_FPUT，write、writev 与除成功 F_DUPFD 外的 fcntl 每次多 fput；US-02 F_DUPFD 的上界 1024 超过 256 项的 fd 表；US-03 close-on-exec 没有实现；US-04 ioctl 空实现总返回 0；US-05 putstring 把用户串作内核格式串。另向 07 的 UB-01、UB-05 和 05 的 UV-09、UV-10、UV-11 追加证据，原条目不改。都不需要 Owner 现在决策。

## 10. 覆盖与剩余边界索引（供后继，一页以内）

- **已覆盖（静态）：** 用户程序与构建边界；syscall 入口绑定、建帧、号检查、switch 分派、包装取参与写回；41 项分派表与被注释表项；open/read/write/writev/fcntl/dup/dup2/lseek/close 的 fd 路径；pid 1 初始表、fork、exec、exit 对 fd 表的处理；console 两张 f_op 表与系统调用入口的衔接。
- **未覆盖：** musl 与镜像中的二进制；信号与 `syscall_exit_to_user_mode` 内部；color_printk 的格式化；slab 释放后的对象去向；路径解析与普通文件读写内部（05）；mmap 中的 fget。
- **证据入口：** `facts-and-boundaries.yaml`（锚点 AS*、检索 QS*、结构检查 SS*、值表 VT-SYSTAB 与 VT-FDLIMITS、生命周期 FL-*、旧未知项 PU-*、缺口 gap_status），`records/ss_check.json` 与 `records/queries.json`。
