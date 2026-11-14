---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-BOUNDARIES-07
record_type: user_entry_mount_console_source_guide
produced_by: "Claude Code desktop app (Code tab), local session f464c1b9-9b25-4af2-8f2d-d419b3b601fd (MYOS2-A-CC02 内核分析本地执行)"
model: unknown_or_not_attestable
model_declared_by_runtime: claude-opus-5-5
date: "2026-10-28"
base_snapshot: "kernel=time a039d9803ade；执行分支起点 b12572f14eae；主线 agent/MYOS2-LEAD-002 f5d0423da491（均为短 SHA）"
inputs_read: "见 evidence.md §2"
status: final_for_user_boundaries_07
runtime_evidence: NOT_RUN
owner_decisions_requested_now: []
open_questions:
  - "镜像根中是否有名为 dev 的项（扩展 GAP-U02 的镜像内容范围）是 /dev 能否挂上的条件之一；本批只读宿主脚本文本，未见建立语句。"
  - "init 链接的 libc 在仓库外，freopen/printf 发出哪些系统调用仍是 GAP-U10。"
  - "TASK_threadsp、X86_top_of_stack、TSS_sp2 的数值需要编译生成才能核；sysretq/swapgs/sti 的硬件语义未用一手资料核对。"
---

# 用户入口、根与 /dev、控制台：三处接合的源码说明（CORE-USER-BOUNDARIES-07）

本文接在 05 的六条链（`user-vfs-baseline-05/user-vfs-baseline.md`）后面，补 05 留下的 GAP-U03、GAP-U04、GAP-U05 三处断点。所有结论都来自 `time a039d9803ade` 的源码文本与规则推导，**没有编译或运行任何东西**。源码能讲清楚，不等于运行时可以依赖：下文每个“会怎样”都只说到源码表达式为止，机器上的实际结果一律记为 NOT_RUN。

机器可读的事实、证据编号与核对规则在 `facts-and-boundaries.yaml`；本批发现的普通问题在 `deferred-findings.yaml`（UB-01…UB-14，另有一项文档勘误 ER-01），当前都不需要 Owner 决策。

**编号约定：** 文中不带前缀的旧编号（UV-xx、GAP-Uxx、DRU-xx、A-Cx-xx、Qxx）都指 CORE-USER-VFS-BASELINE-05 中的同名条目，全名形如 `CORE-USER-VFS-BASELINE-05::UV-01`；SF-C5-xx 指 CORE-SCHED-INTEGRATION-03；UB-xx、ER-xx 指本批 `deferred-findings.yaml`，AB/QB/SB/VT/FP/NVB 编号指本批 `facts-and-boundaries.yaml`。

## 0. 三个问题的答案

1. **第一程序的上下文从何而来？** 来自 kernel_init 线程内核栈顶的那块 `pt_regs`。它在 kernel_init 作为内核线程被创建时由 `copy_thread` 整块清零；exec 成功走到 `START_THREAD` 时写入入口地址、用户栈、段选择子和 flags；`kjmp_to_doexecve` 把 rsp 设到这块地址后跳进 `syscall_return_via_sysret`。fork 子任务和普通系统调用也用同一个返回段，但帧的来源和进入方式不同（§2）。
2. **根目录和 /dev 对象怎样连接？** FAT32 超级块装进静态挂载 `myos_root_mnt`，再直接写进**共享的** `init_fs`（kernel_init exec 成为 init 后仍共用它）作为 root/pwd；之后把 devtmpfs 挂到 FAT32 根下名为 `dev` 的项上。devtmpfs 的“对外类型”每次挂载都返回同一个内部 shmem 实例的根，而设备节点早在根切换之前就由 kdevtmpfs 线程建在这个内部根下。所以在根分区被选中、devtmpfs 建立成功的前提下，`/dev/console` 能否解析还取决于 FAT32 根中能否查到 `dev`（源码不检查它是否为目录）（§3）。
3. **控制台请求实际落到哪个实现？** `/dev/console` 是 (5,1) 字符设备节点，初始 `i_fop` 是 `def_chr_fops`；打开时 `chrdev_open` 把文件的 `f_op` 换成 `console_fops`。read 落到 `myos_tty_read`，在键盘环形缓冲上等完成量；write 回调落到 `myos_tty_write`，直接写帧缓冲像素。但对以写方式打开的文件，`write(2)` 在 `vfs_write` 的 `FMODE_CAN_WRITE` 检查处就返回 -EINVAL，只有 `writev(2)` 能到达这个回调（§4）。libc 用哪一个，仓库里查不到（GAP-U10）。

## 1. 建议的源码阅读顺序（任务书允许的阅读顺序，不是学习路线或课程排序）

1. `init/main.c`：`start_kernel` 中的 `vfs_caches_init`、`myos_devices_init`，`rest_init` 创建 kernel_init 与 kthreadd，`kernel_init` → `do_basic_setup`（`driver_init` 再 `do_initcalls`）→ `myos_switch_to_root_disk` → `kjmp_to_doexecve`。
2. `sched/misc/fs_struct.c` 与 `sched/forkexec/fork.c::copy_fs`：先弄清 `init_fs` 被谁共享。
3. `drivers/base/devtmpfs.c`：两个 devtmpfs 类型、`devtmpfsd`、请求队列。
4. `fs/vfs/myos_vfs.c`、`fs/vfs/init.c`、`fs/vfs/namespace.c`、`fs/vfs/fs_context.c`、`fs/vfs/namei.c`（`path_init`、`handle_dots`、`traverse_mounts`）。
5. `drivers/char/tty_io.c`、`char_dev.c`、`fs/vfs/inode.c::init_special_inode`、`fs/vfs/shmem.c`。
6. `drivers/char/myos_tty.c`、`arch/x86_64/myos/keyboard.c`、`fs/vfs/read_write.c`。
7. `arch/x86_64/sched/context/process_arch.c`（`copy_thread`、`ret_from_fork`）、`switch_to_types_arch.h`、`process64_arch.c`（`__switch_to`、`start_thread_common`）、`arch/x86_64/entry/entry_64.S`、`arch/x86_64/entry/common.c`、`arch/x86_64/include/asm/ptrace.h` 与 `include/generated/asm-offsets.h`。

## 2. 问题一：第一程序的上下文从何而来（GAP-U04）

### 2.1 三条路径共用的返回段

三条路径最后都执行 `syscall_return_via_sysret`。它先用六次 `xchg` 把帧里的 flags、ip 换到 r11、rcx 两个槽（前五次如下），再由 `POP_REGS` 一次装进寄存器：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	xchgq		%r11,		EFLAGS(%rsp)
	xchgq		%rcx,		RIP(%rsp)
	xchgq		%r11,		R11(%rsp)
	xchgq		%rcx,		RCX(%rsp)
	xchgq		%r11,		EFLAGS(%rsp)
```

第六次 `xchg` 之后，`POP_REGS` 装入 15 个寄存器，`addq $0x28` 跳过 irq_nr、orig_ax、ip、cs、flags 五格，从帧里取用户栈，再 swapgs：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	xchgq		%rcx,		RIP(%rsp)
	POP_REGS
	addq		$0x28,		%rsp
	popq		%rsp
	swapgs
```

最后开中断并返回：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::syscall_return_via_sysret]
```asm
	sti
	sysretq
```

**消费哪些字段：** 帧中 13 个通用寄存器字段（经 POP_REGS）、ip（装入 rcx）、flags（装入 r11）、sp。帧里原来的 r11、cx 两个字段在 xchg 后落到 flags、ip 槽，随五格一起被跳过；cs、ss、irq_nr、orig_ax 也不被这一段读取。`pt_regs` 的 22 个字段顺序、提交入库的生成偏移（`RIP=136`、`EFLAGS=152`、`RSP=160` 等）、`PUSH_REGS`/`POP_REGS` 的顺序以及“跳过 0x28=40 字节”逐项对得上（VT-PTREGS）。这些偏移来自提交的生成件，构建时会重新生成；本批只比较文本。

### 2.2 三条路径对照

| | 路径一：首个程序 | 路径二：用户 fork 子任务 | 路径三：普通系统调用 |
|---|---|---|---|
| 谁写帧 | `copy_thread` 内核线程分支整块清零；exec 末段 `start_thread_common` 写 ip/sp/cs/ss/flags | `copy_thread` 复制父任务当前帧，ax 置 0，给定新栈时改 sp | `entry_SYSCALL_64` 压入 22 格；系统调用本体也可能写帧（如 execve 末段 START_THREAD）；`do_syscall_64` 在号小于 453 时写 ax |
| 何时建立 | 创建 kernel_init 时清零；exec 走到 `START_THREAD` 时写入 | fork 的 `copy_process` 调 `copy_thread` 时 | 每次 syscall 指令进入内核时 |
| 谁取栈指针 | `kjmp_to_doexecve` 在 exec 前保存 `task_pt_regs(curr)`，exec 后内联汇编 `movq` 到 rsp | `__switch_to_asm` 从 `TASK_threadsp` 取 `thread.sp`，弹 6 个寄存器，`__switch_to` 返回后 rsp 正好落在帧起点 | 入口从 `pcpu_hot.top_of_stack` 取内核栈 |
| 谁消费 | `syscall_return_via_sysret` | `ret_from_fork` → `syscall_exit_to_user_mode` → `syscall_return_via_sysret` | `do_syscall_64` → `syscall_exit_to_user_mode` → 落入 `syscall_return_via_sysret` |
| 与别的路径不同 | 不经过 `do_syscall_64` 与 `syscall_exit_to_user_mode`（没有信号检查）；返回段执行 swapgs 时，该 CPU 的 GS 状态取决于先前的进入路径或启动设置（未核）；exec 不清通用寄存器，除 rcx/r11（换成 ip/flags）与 rsp（用户栈）外都是清零值 | 帧是父任务入口时的寄存器；多了 `schedule_tail` 与信号检查；返回 swapgs 时该 CPU 的 GS 状态同样取决于先前的进入路径（未核） | 唯一由同一任务先后执行入口与返回 swapgs 的路径；swapgs 作用于每 CPU 状态，是否成对待 NVB-4 |

**路径一。** kernel_init 是 `kernel_thread` 创建的（`.kthread = 1`），所以 `copy_thread` 走内核线程分支：

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	if (unlikely(p->flags & PF_KTHREAD)) {
		// p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(pt_regs_s));
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
```

kernel_init 本身就是从这个 fork 帧开始运行的：`ret_from_fork` 对内核线程直接调用 `fn(fn_arg)`，即 `kernel_init`。到 `kjmp_to_doexecve` 时，先取帧地址、去掉内核线程标志（`kernel_execve` 拒绝带 PF_KTHREAD 的任务）：

[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	task_s * curr = current;
	pt_regs_s *curr_ptregs = task_pt_regs(curr);
	curr->flags &= ~PF_KTHREAD;
```

exec 成功时，`load_elf_binary` 取 `current_pt_regs()`（就是同一块栈顶帧）并 `START_THREAD`，写入 ip、sp、cs、ss 和 `flags = X86_EFLAGS_IF | X86_EFLAGS_FIXED`。`ELF_PLAT_INIT` 被注释（QB-50），exec 不清其余寄存器。所以首个程序进入用户态时：rcx 是入口地址（帧中 ip），r11 是 flags（IF|FIXED），rsp 是用户栈；其余 13 个通用寄存器（含 rax）是 `copy_thread` 清零后的 0。exec 失败时 `kernel_execve` 的返回值被丢弃，帧里仍是全零：ip、sp、flags 都为 0。这条已追加到 05 的 UV-01（不改原条目）；全零帧在机器上的结果属于外部语义，NOT_RUN。

**路径二。** 用户 fork 时，子任务复制父任务的系统调用帧：

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::copy_thread]
```c
	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = (reg_t)sp;
```

帧下方是 7 格的 `inactive_task_frame`（r15、r14、r13、r12、bx、bp、ret_addr），返回地址设为 `ret_from_fork_asm`。`__switch_to_asm` 弹出前六格后 `jmp __switch_to`，`__switch_to` 返回时弹出 ret_addr，此时 rsp 恰好前进 56 字节，落在 `pt_regs` 起点（VT-KFRAME）。`__switch_to` 同时更新每 CPU 的当前任务和栈顶：

[VERIFIED mykernel/arch/x86_64/sched/context/process64_arch.c::__switch_to]
```c
	*this_cpu_ptr(&pcpu_hot.current_task) = next_p;
	*this_cpu_ptr(&pcpu_hot.top_of_stack) = task_top_of_stack(next_p);
```

`ret_from_fork_asm` 以 rsp 作为 regs 调 C 层 `ret_from_fork`，回来后按 rbx 分流；用户子任务的 rbx 为 0，走到：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::ret_from_fork_asm]
```asm
swapgs_restore_regs_and_return_to_usermode:
	// UNWIND_HINT_REGS
	// movq		%rsp,		%rdi
	// call		syscall_exit_to_user_mode	/* returns with IRQs disabled */
	jmp			syscall_return_via_sysret
```

所有新任务首次切入都经 `ret_from_fork_asm` 调 `ret_from_fork`、不回 `__schedule` 尾部（INTEGRATION-03 SF-C5-26）；只有 rbx 为 0 的用户子任务走到上面这个标签，内核线程走 `1:` 分支。内核线程如果函数返回，会被 C 层和汇编各调用一次，然后停在 `jmp .`（UB-06）；kernel_init 不返回，`kthread_create` 建的线程以 `kthread()` 为线程函数，其结尾调用声明为 `__noreturn` 的 `kthread_exit`（QB-62）。

**路径三。** 入口先换栈：

[VERIFIED mykernel/arch/x86_64/entry/entry_64.S::entry_SYSCALL_64]
```asm
	movq		%rsp,		PER_CPU_VAR(cpu_tss_rw + TSS_sp2)
	// SWITCH_TO_KERNEL_CR3 scratch_reg=%rsp
	movq		PER_CPU_VAR(pcpu_hot + X86_top_of_stack),	%rsp
```

然后压入 ss、sp、flags(r11)、cs、ip(rcx)、orig_ax(rax)、irq_nr($0x80) 和 15 个寄存器，共 22 格。因为 `top_of_stack` 等于 `task_pt_regs(current) + 1`，这 22 格正好写在当前任务的 `pt_regs` 上，所以 fork 时 `current_pt_regs()` 读到的就是它。`call do_syscall_64` 之后没有 ret 或 jmp，紧随其后的就是 `syscall_return_via_sysret`（`SYM_CODE_START` 只展开为对齐与标签，见 `include/linux/kernel/linkage.h`），所以返回后落进它；入口也没有 `SYM_CODE_END`，那只影响符号的类型与大小（SB-01）。`do_syscall_64` 计算的 SYSRET 适用性 bool 没人检查（UB-09）。

**返回前的信号检查。** 路径二和路径三在返回前都调用 `syscall_exit_to_user_mode`，其中的条件判断被注释，`arch_do_signal_or_restart(regs)` 成为无条件调用（SB-14、SB-15）；它可能改写帧（例如 ax），内部属于信号子系统，本批不追。路径一没有这一步。

**仍未闭合：** `TASK_threadsp=552`、`X86_top_of_stack=24`、`TSS_sp2=20` 只核到符号映射（`thread.sp`、`pcpu_hot.top_of_stack`、`x86_tss.sp2`），数值是否与当前结构体布局一致需要编译生成；swapgs 配对、`sti` 紧接 `sysretq` 的中断窗口和全零帧的硬件结果属于外部语义，没有一手资料，保留待核。

## 3. 问题二：根目录和 /dev 对象怎样连接（GAP-U03）

### 3.1 对象与时序

| 对象 | 何时赋值 | 由谁消费 |
|---|---|---|
| `init_fs`（root/pwd） | `init_task.fs = &init_fs`；kernel_init、kthreadd、所有 kthread 都以 CLONE_FS 共用它（`copy_fs` 只把 `users` 加一，QB-63）。exec 不拆分 fs（QB-57、QB-67），所以 kernel_init exec 成为 init（pid 1）后，在用户态仍共用它（05 DRU-06 中 fs 的一半按 ER-01 更正）。① `mnt_init` → `init_mount_tree` 设为 rootfs 根；② kdevtmpfs 的 `chdir("/..")`、`chroot(".")` 设为 devtmpfs 根；③ 根盘切换时 `set_init_taskfs` 直接赋值为 FAT32 根 | `path_init`：绝对路径从 root、相对路径从 pwd 开始；pid 1 的 chdir 直接改写共享的 pwd（QB-68） |
| devtmpfs 内部实例 | `devtmpfs_init` 中 `vfs_kern_mount(&internal_fs_type)`，shmem 实现 | 设备节点建在它的根下；`public_dev_mount` 每次返回它的根 |
| devtmpfs 对外类型 `dev_fs_type` | `register_filesystem(&dev_fs_type)` | `get_fs_type("devtmpfs")`；挂载时经 legacy 上下文调 `.mount` |
| `myos_root_sb` | 文件作用域初值 NULL；只在第 126 行由 `mount_fs("FAT32")` 赋值 | `set_init_mount`、`set_init_taskfs` |
| `myos_root_mnt` | 静态 `mount_s`，`mnt_parent` 指向自身；`set_init_mount` 填 sb 与根 | 作为根挂载，被 `/dev` 挂载接成父挂载 |
| `/dev` 挂载 | 根切换末尾 `init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT)` | 之后解析 `/dev/...` 时由 `traverse_mounts` 跨入 |

时序（按源码顺序；start_kernel 部分由 0 号任务执行；挂载 "/"、chdir/chroot 与 mknod 由 kdevtmpfs 线程代为执行，kernel_init 在 `setup_done`、`req->done` 上同步等待）：

```text
start_kernel: myos_devices_init → init_keyboard                   分配 p_kb
              vfs_caches_init → mnt_init → init_mount_tree      init_fs 指向 rootfs 根
              vfs_caches_init → chrdev_init                       建 cdev_map
kernel_init:  do_basic_setup → driver_init → devtmpfs_init
                 内部 shmem 实例；注册 dev_fs_type；建 kdevtmpfs 并等待 setup_done
  [kdevtmpfs]    把 devtmpfs 挂到 "/"，chdir("/..") 越过 rootfs 根进入 devtmpfs 根，chroot(".")
                                                                  init_fs 指向 devtmpfs 根（共享）
              do_basic_setup → do_initcalls → chr_dev_init → tty_init
                 console_cdev(5,1) 进 cdev_map；myos_device_create → device_add
                 → devtmpfs_create_node，提交请求并等待 req->done
  [kdevtmpfs]    在其 cwd（devtmpfs 根）下 mknod "console"
              myos_switch_to_root_disk
                 读 MBR/GPT；第 0 项 EFI 启动分区 → mount_fs("FAT32") → myos_root_sb
                 set_init_mount；set_init_taskfs                  init_fs 指向 FAT32 根（直接赋值）
                 init_mount("devtmpfs", "/dev")                   FAT32 根下的 dev ← devtmpfs 根
              kjmp_to_doexecve → /boot/init
```

kdevtmpfs 的那三步：

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
```

“越过”是怎样发生的：在挂载根上解析 `..` 时，`follow_dotdot` 找不到上层挂载就返回 NULL，`handle_dots` 于是对当前 dentry 调 `step_into`，而 `step_into` 每一步都直接查 `lookup_mnt`，所以跨进了刚叠在 rootfs 根上的 devtmpfs：

[VERIFIED mykernel/fs/vfs/namei.c::handle_dots]
```c
		if (parent == NULL)
			error = step_into(nd, nd->path.dentry);
```

`/dev` 挂载拿到的根与节点所在的根是同一个 dentry：

[VERIFIED mykernel/drivers/base/devtmpfs.c::public_dev_mount]
```c
	super_block_s *s = mnt->mnt_sb;
	int err;

	return dget(s->s_root);
```

### 3.2 失败与未知边界

根盘切换末尾不检查前面任何一步：

[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
	set_init_mount();
	set_init_taskfs();

	int err = init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT);
```

| 输入 | 已有的检查 | 源码推到哪里为止 |
|---|---|---|
| 正常：第 0 项是 EFI 启动分区，镜像有 dev 目录 | — | 根挂载与 /dev 挂载都建立 |
| 未选中根分区（第 0 项不是 EFI 启动分区，或表项为空） | 无 | `myos_root_sb` 保持 NULL，`set_init_mount` 读取 `myos_root_sb->s_root`；静态推导到这个空指针成员读取为止 |
| 文件系统类型找不到 | `mount_fs` 返回 0 | 同上 |
| 读超级块失败 | 只核到 `alloc_super` 失败时返回 `ERR_PTR(-ENOMEM)`；`read_fat32_superblock` 中其他分配的失败处理与 BPB 字段校验未核（不重析 FAT32） | 错误指针被当作超级块读取 s_root |
| 读盘失败 | `ROOTBLK_TRANSFER` 的返回值都不检查 | 若 MBR 缓冲保持 kzalloc 的零值 → 类型不符 → 打印后原地 `while (1)`（05 A-C1-05）；若 GPT 缓冲保持零值 → 不进循环 → 同“未选中”。FAT32 引导扇区缓冲用 kmalloc（不清零），读失败时 `read_fat32_superblock` 解析的是未初始化内容（未追）。读失败时缓冲是否保持原值取决于 ATA 层（未重析） |
| `/dev` 查找失败（镜像无 dev） | `__lookup_slow` 把 FAT32 找不到转成 -ENOENT，`init_mount` 返回它 | `err` 不再使用；之后 `/dev/console` 也解析不到 |
| 挂载失败（类型未注册等） | `do_new_mount` 返回 -ENODEV 等 | 同样被丢弃 |
| devtmpfs 自身建立失败 | 内部实例或类型注册失败即返回，`driver_init` 不看返回值；kthread 失败 | 前两种情况下静态变量 `thread` 保持 NULL，之后建节点时 `devtmpfs_submit_req` 以 NULL 调 `wake_up_process`（未追）；kthread 失败时 `devtmpfs_init` 进入 `while (err);`；`devtmpfsd` 挂载失败时不再 complete，`devtmpfs_init` 永远等待 |

[VERIFIED mykernel/fs/vfs/myos_vfs.c::set_init_mount]
```c
	myos_root_mnt.mnt.mnt_sb = myos_root_sb;
	myos_root_mnt.mnt_mountpoint =
	myos_root_mnt.mnt.mnt_root = myos_root_sb->s_root;
```

另外：配置文本中 `-DROOTBLK_NVME` 被注释，`ROOTBLK_TRANSFER` 取 ATA 分支（只核文本，无构建产物）；GPT 表项的复制长度按表项数计算、不受读缓冲约束，遍历上界不受表项数约束（UB-12）；挂载点是否为目录不被检查（`graft_tree` 中的 `d_is_dir` 判断被注释，QB-53）；共享 `init_fs` 被内核线程改写、根切换时绕过引用计数，pid 1 也带着它进入用户态（UB-03、ER-01）。

## 4. 问题三：控制台请求实际落到哪个实现（GAP-U05）

### 4.1 节点从哪里来

`tty_init` 把 `console_cdev` 绑到 `console_fops`，以 (5,1) 加入 `cdev_map`，再 `myos_device_create(tty_class, MKDEV(5,1), "console")`。`device_add` 在主设备号非 0 时请求 devtmpfs 建节点，模式固定为 `0600 | S_IFCHR`。kdevtmpfs 以 `AT_FDCWD` 相对路径（它的 cwd 此时是 devtmpfs 根）`kern_path_create`，再 `vfs_mknod`。目录 inode 的 `->mknod` 是 `shmem_mknod`，它建的非普通文件 inode 走 `init_special_inode`：

[VERIFIED mykernel/fs/vfs/inode.c::init_special_inode]
```c
	if (S_ISCHR(mode)) {
		inode->i_fop = &def_chr_fops;
		inode->i_rdev = rdev;
```

“出现字符串 /dev/console”不等于节点存在：节点要靠上面这条链在根切换之前建好，而且要 `/dev` 挂载成功才能从 init 的视角看到。这条链上还有两处入库问题：请求对象由工作线程在 complete 之后立即释放，提交方此后仍要写其完成量并读取 err（UB-02）；`device_add` 总是返回 -EINVAL（UB-04）。

### 4.2 打开时换表：前后两张表

| 时点 | 文件的 f_op | 内容 |
|---|---|---|
| `myos_do_dentry_open` 取 `inode->i_fop` 之后 | `def_chr_fops` | `.open = chrdev_open`、`.llseek = noop_llseek` |
| `chrdev_open` 换表之后 | `console_fops` | `.read = myos_tty_read`、`.write = myos_tty_write`、`.open = myos_tty_open` |

`chrdev_open` 按 `i_rdev` 在 `cdev_map` 中找回 `console_cdev`，先换表再调 open。`kobj_map` 原样保存登记时给的指针（QB-60）；console 与 tty 用静态 cdev，而 mem 设备（null、zero、full）登记的是循环内局部副本的地址（UB-13）。

[VERIFIED mykernel/drivers/char/char_dev.c::chrdev_open]
```c
	filp->f_op = fops;
	if (filp->f_op->open) {
		ret = filp->f_op->open(inode, filp);
		if (ret)
			goto out_cdev_put;
```

[VERIFIED mykernel/drivers/char/tty_io.c::console_fops]
```c
	.read				= myos_tty_read,
	.write				= myos_tty_write,
	.open				= myos_tty_open,
```

`myos_tty_open` 重置全局键盘缓冲后返回 1，`chrdev_open` 因此走错误出口返回 1；而上一层的 `do_open` 与 `myos_do_dentry_open` 两层都没有 return（05 UV-04）。打开在 C 语义下是否“成功”仍由 UV-04 决定（UB-05）。

### 4.3 读写落点

**读。** console 没有 `read_iter`，`vfs_read` 调 `->read`，即 `myos_tty_read`：

[VERIFIED mykernel/drivers/char/myos_tty.c::myos_tty_read]
```c
	while (counter < count)
	{
		char key = kbd_parse_scancode();
		if (key != 0)
		{
```

`kbd_parse_scancode` → `kbd_get_scancode`，在完成量上等待键盘缓冲有数据；生产者是键盘中断处理 `keyboard_handler`，它把扫描码放进 `p_kb` 并 `complete(&getcode_done)`：

[VERIFIED mykernel/arch/x86_64/myos/keyboard.c::kbd_get_scancode]
```c
	do {
		wait_for_completion(&getcode_done);
	}
	while (p_kb->count <= 0);
```

读要攒满 `count` 个非零键值才返回，不按行结束（UB-08）。

**写。** console 的 write 回调只转交 `myos_tty_write_color`，逐字节直接读调用方的 `buf`，用 `kputchar` 按字模写 `Pos.FB_addr` 起的像素（`Pos.FB_addr` 在 `myos_init_video` 中取 `framebuffer.FB_virbase`）。问题在于两条系统调用路径不一样：

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_write]
```c
	if (!(file->f_mode & FMODE_WRITE))
		return -EBADF;
	if (!(file->f_mode & FMODE_CAN_WRITE))
		return -EINVAL;
```

[VERIFIED mykernel/fs/vfs/read_write.c::vfs_writev]
```c
	for (int i = 0; i < vlen; i++)
		ret += file->f_op->write(file, vec[i].iov_base, vec[i].iov_len, pos);
```

`FMODE_CAN_WRITE` 在 mykernel/ 中只有定义和检查，没有设置者（QB-23）；`f_mode` 的赋值只有 `OPEN_FMODE(flags)`（访问位加 `__FMODE_NONOTIFY`）与块设备 open 中的几处 `|=`，都不含 `FMODE_CAN_WRITE`（QB-24、VT-FLAGS）。所以对以写方式打开的文件，`write(2)` 都在这里返回 -EINVAL（fd 无效或没有写权限时更早返回 -EBADF）；`writev(2)` 则不检查这个位，逐段直接调用 `->write`（UB-01、UB-07）。

### 4.4 与普通 FAT32 文件的分叉

两者共用 05 C4 的打开路径（`sys_open → … → path_openat → do_open → vfs_open → myos_do_dentry_open`），分叉点就在 `f_op = inode->i_fop` 之后：

| | FAT32 普通文件（05 C4） | `/dev/console` |
|---|---|---|
| inode 的 i_fop | `FAT32_file_ops`（有 read_iter） | `def_chr_fops`，open 时换成 `console_fops` |
| read | `vfs_read` 优先 `read_iter` → `simple_filemap_read`（05 UV-05：读目标取自用户缓冲前 16 字节） | 无 `read_iter` → `myos_tty_read` → 键盘缓冲 |
| write | 同样被 `vfs_write` 拦截 | 同左；writev 可达 `myos_tty_write` |

所以 05 的普通文件读缺陷（UV-05）不适用于 console。还有一处细节：`path_openat` 把查找标志（不是打开标志）传给 `open_last_lookups`，后者检查的是 `open_flag & O_CREAT`；`O_CREAT` 与 `LOOKUP_RCU` 数值相同（都是 64），而 `build_open_flags` 设置的查找标志里没有 `LOOKUP_RCU`（QB-51），对 open(2) 来说 `build_open_how` 不设 resolve，只可能出现 `LOOKUP_DIRECTORY`、`LOOKUP_FOLLOW`；`do_filp_open` 把 `op->lookup_flags` 原样传入 `path_openat`，不另加 `LOOKUP_RCU`（QB-52）。因此只要 `build_open_flags` 执行到给 `lookup_flags` 赋值的那一行，无论 libc 是否带 `O_CREAT`，最后一段都走查找分支（QB-32、VT-FLAGS）。所以带 `O_CREAT` 的 open 不会新建文件：不存在的最后一段由 `__lookup_slow` 返回 -ENOENT，经 `link_path_walk` 开头的 `IS_ERR` 检查成为 `path_openat` 的错误返回（UB-14）；创建分支 `lookup_open` 整段被注释，新建文件只能经 creat(2) 的 `do_creatat`（QB-69）。`/dev/console` 已存在，查找分支照样找到它。它的错误返回会被 `do_sys_openat2` 随后的赋值覆盖（05 已在 fs.vfs.path_open 中记录），提前返回时 `op` 的字段未初始化。

### 4.5 libc 缺口（GAP-U10 维持 open）

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);
```

构建文本把编译器设为仓库外的 `musl-gcc`，只加入 `myinit` 与 `myshell`；仓库内的 `mylib/myglibc` 不在这次构建中，而且其中与 `freopen` 有关的几行（`libio/stdio.h` 第 251、258、262 行）都被注释，没有活动定义（QB-30、QB-31）。能确定的只有内核侧：系统调用表有 open、close、read、write、writev、ioctl、fcntl、dup、dup2，没有 openat、dup3；dup/dup2 是空实现（UB-10）；表外而编号小于 453 的调用会进 `__x64_sys_ni_syscall` 的 `while (1)`（UB-09）。freopen 实际走哪几个调用、printf 用 write 还是 writev，要等读到 init 实际链接的 libc（候选 NVB-2）。

## 5. 两层图

### 5.1 正常路径（源码可讲解）

```text
[B01] FAT32 第 0 项 ──mount_fs──▶ myos_root_sb ──set_init_mount──▶ myos_root_mnt
                                     │
                                     └─set_init_taskfs──▶ init_fs.root/pwd = FAT32 根（kernel_init、kthread 共享）
      init_mount("/dev") ──kern_path──▶ FAT32:/dev ──do_new_mount──▶ legacy ->mount ──▶ devtmpfs 内部根
                                                                                         ▲
[B03] tty_init ──cdev_add(5,1)──▶ cdev_map              kdevtmpfs mknod "console" ────────┘（根切换之前）
      init: freopen("/dev/console") ─?libc?─▶ sys_open ─▶ … ─▶ i_fop=def_chr_fops ─▶ chrdev_open
            ─▶ f_op=console_fops ─▶ read: myos_tty_read ─▶ kbd_get_scancode ◀─ keyboard_handler(IRQ)
                                   └▶ writev: myos_tty_write ─▶ kputchar ─▶ 帧缓冲
[B02] copy_thread(清零) ─▶ exec START_THREAD(ip/sp/cs/ss/flags) ─▶ kjmp: rsp=帧 ─▶ syscall_return_via_sysret
      fork: copy_thread(复制父帧) ─▶ __switch_to_asm/__switch_to ─▶ ret_from_fork_asm ─▶ ret_from_fork ─▶ 同上
      syscall: entry_SYSCALL_64(22 格) ─▶ do_syscall_64(ax) ─▶ syscall_exit_to_user_mode ─▶ 同上
```

### 5.2 失败与未知边界（运行不可依赖）

```text
根未选中/类型缺失/读超级块失败 ─▶ set_init_mount 读 NULL 或错误指针的成员 ......... 静态止于此，NOT_RUN
镜像根中查不到 dev ─▶ init_mount 返回 -ENOENT（被丢弃）─▶ /dev/console 解析失败 ......... 扩展 GAP-U02
exec 失败 ─▶ 全零帧 ─▶ sysretq ............................................ 外部语义待核，NOT_RUN
open /dev/console ─▶ chrdev_open 返回 1 ─▶ myos_do_dentry_open 无 return ............ UV-04/UB-05
write(2) ─▶ vfs_write：FMODE_CAN_WRITE 不成立 ─▶ -EINVAL ............................ UB-01
open(O_CREAT) 不存在的文件 ─▶ 只查找分支 ─▶ -ENOENT，不新建 ......................... UB-14
devtmpfs 建节点 ─▶ 工作线程 kfree 请求后提交方仍访问其完成量 .......................... UB-02
freopen/printf 发出哪些系统调用 ─▶ libc 在仓库外 ................................... GAP-U10
openat/dup3 等表外调用 ─▶ __x64_sys_ni_syscall while (1) .......................... UB-09
TASK_threadsp/X86_top_of_stack/TSS_sp2 数值 ─▶ 需编译生成 ............................ VT-KFRAME，NVB-3
swapgs 配对、sti 后中断窗口 ─▶ 无一手资料 ........................................... NVB-4
```

## 6. 与 05 六条链的衔接

| 05 链 | 本批接上的部分 | 仍沿用 05 的部分 |
|---|---|---|
| C1 启动到首个用户程序 | 根对象与任务 root/pwd、/dev 来源（§3）；首个程序的帧（§2 路径一）；init 打开 console 后的落点（§4） | MBR/GPT 读盘、FAT32 超级块解析、镜像内容（GAP-U02） |
| C2 创建 / fork | 子任务从 copy_thread 到回用户态的帧路径（§2 路径二）；kernel_init、kthreadd、kthread 的 CLONE_FS；DRU-06 中 fs 的一半按 ER-01 更正（pid 1 带着共享 init_fs 进入用户态） | pid 分配、wake_up_new_task 入队（GAP-U09 其余部分） |
| C3 exec / ELF 装载 | 末段 START_THREAD 写当前帧，由 syscall_return_via_sysret 消费 | begin_new_exec 及之前（UV-02 等） |
| C4 打开 / 读取 / 关闭 | 字符设备在 ->open 处换表；console 读写落点；write 被拦截而 writev 可达（§4.4） | 打开路径缺 return（UV-04，所有文件）；FAT32 普通文件读缺陷（UV-05 等） |
| C5 文件映射到读完成边界 | 不涉及；console_fops 没有 mmap 回调 | 全部沿用 05 |
| C6 退出 / 回收 | 不涉及；只提示 kernel_init（exec 后即 pid 1）与内核线程共享 init_fs | 全部沿用 05 |

## 7. 缺口状态

| 缺口 | 原问题 | 本批覆盖 | 静态已闭合 | 剩余未知 | 状态 |
|---|---|---|---|---|---|
| GAP-U03 | set_init_mount、set_init_taskfs、init_mount(devtmpfs) 正文未逐行读；找不到根分区时行为未知 | 三个函数逐行读完；init_mount 追到挂载接入；devtmpfs 建立与共享 init_fs | 对象来源与消费者；未选中根、读超级块失败、/dev 查找失败、挂载失败各自停在哪里 | 镜像根中有无 dev 项（扩展 GAP-U02）；空指针/错误指针读取的机器结果 | open → **closed_static**（运行部分 NOT_RUN） |
| GAP-U04 | syscall_return_via_sysret、ret_from_fork 与 pt_regs 来源未追 | 三条路径的写帧、建立、取栈、消费；返回段指令；布局对照 | pt_regs 字段/偏移/压弹顺序一致；三条路径的数据流 | TASK_threadsp 等数值；sysretq/swapgs/sti 语义；运行值 | open → **narrowed** |
| GAP-U05 | devtmpfs 与 /dev/console 的 f_op 未追 | 设备登记 → 建节点 → i_fop → 换表 → 读写后端 | 节点来源与设备号；前后两张表；读写落点 | libc 发出的调用（GAP-U10）；镜像根中的 dev 项；运行行为 | open → **closed_static**（运行部分 NOT_RUN） |
| GAP-U06（相交部分） | 写路径、目录读取、dcache 回收、O_CREAT、namespace/mount 细节未覆盖 | 只更新相交子范围：write/writev 分派与 FMODE_CAN_WRITE 拦截；打开最后一段的 O_CREAT 判断；init_mount 的挂载接入与跨挂载查找 | 上述三项 | generic_file_write_iter、FAT32_write、目录读取、dcache 回收、命名空间与传播 | open → **narrowed** |
| GAP-U09（相交部分） | copy_thread、kernel_thread flags、pid、入队未追 | 只更新 copy_thread 的帧建立与线程创建 flags | 上述两项 | pid 分配；wake_up_new_task 入队 | open → **narrowed** |
| GAP-U10（相交部分） | musl 系统调用选择未读 | 只追加证据：仓库外 musl-gcc；myglibc 不在构建；内核侧入口行为 | — | freopen/printf 的实际调用 | **open**（证据追加） |

## 8. 本批入库的普通问题（只列编号，正文见 deferred-findings.yaml）

UB-01 write(2) 到不了 ->write；UB-02 devtmpfs 请求释放后仍被访问；UB-03 共享 init_fs；UB-04 device_add 不返回 0；UB-05 console open 返回 1；UB-06 内核线程函数被调用两次；UB-07 console 写与 writev 直接用用户指针；UB-08 console 读需攒满 count；UB-09 系统调用分派边角；UB-10 dup/dup2 空实现；UB-11 __d_lookup 前缀比较；UB-12 GPT 解析上界；UB-13 mem 设备登记栈上副本；UB-14 open 的 O_CREAT 不新建文件。文档勘误 ER-01：05 DRU-06 中“共享 fs 不会经用户态出现”按 pid 1 的实际情况更正，05 原文件不改。另向 05 的 UV-01、UV-04 追加证据，原条目不改。都不需要 Owner 现在决策。

## 9. 覆盖与剩余边界索引（供后继，一页以内）

- **已覆盖（静态）：** 根对象与共享 init_fs、devtmpfs 的两种类型与建立线程、/dev 挂载与路径跨挂载、console 设备登记与建节点、字符设备换表、console 读写的第一层后端、三条用户返回路径的帧数据流、pt_regs 与切换帧布局对照。
- **未覆盖：** FAT32/ATA 内部；shmem 超级块细节；kobj_map 内部（只核到保存 data 指针一行）；信号投递 `arch_do_signal_or_restart`；中断入口 `sa_entintr_retp`；键盘扫描码表、USB 键盘、TTY 行规程、帧缓冲映射；pid 分配与入队；libc；镜像内容。
- **需要授权才能继续的：** NVB-1（构建 + QEMU + 测试镜像）、NVB-2（读仓库外 musl）、NVB-3（编译生成 asm-offsets）、NVB-4（ISA 手册研究），全部 NOT_RUN。
- **证据入口：** `facts-and-boundaries.yaml`（锚点 AB*、检索 QB*、结构检查 SB*、值表 VT-*、帧路径 FP-*、缺口 gap_status），`records/ub_check.json` 与 `records/queries.json`。
