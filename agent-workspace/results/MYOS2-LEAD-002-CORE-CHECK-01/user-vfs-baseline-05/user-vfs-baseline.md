---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
packet_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-VFS-BASELINE-05
record_type: user_process_and_vfs_static_baseline
produced_by: "Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable"
date: "2026-10-02"
base_snapshot: "kernel=time a039d9803ade；workspace=master de3bb1df906a（开工时事实）；主线 agent/MYOS2-LEAD-002 0d066801371d（开工时）；被审执行头 de7c96ede546；MM 结果冻结 eb75b3100601。均为短 SHA"
status: final_for_user_vfs_baseline_05
acceptance_ceiling: PASS_PENDING_LOCAL
kernel_correctness_verdict: NOT_ISSUED
runtime_evidence: NOT_RUN
inputs_read: "19 号任务书、18 号约定、MM-04 审查回执全文；de7c96ede546 中 mm-baseline-04 的相关原件；master 上 DR-002/DR-003 两个对象的 ID 与端点名；time 源码见 facts-and-dependencies.yaml 各 anchors/queries 的 path"
open_questions: "见 §8 与 facts-and-dependencies.yaml 的 gaps；需要证据的条目见 deferred-findings.yaml（status: needs_evidence）"
---

# 用户程序与文件访问的静态基线（U01–U04）

## 0. 结论与边界

**一句话：** 六条主链都找到了实际入口，并且能从源码连起来讲清楚：启动时从 GPT 第 BOOT_FS_IDX 号 EFI 启动分区挂 FAT32 根，再以硬编码的 /boot/init 进入用户态；fork 走 sys_fork → kernel_clone → copy_process；exec 经唯一注册的 ELF 格式装载；open/read/close 经 FAT32_file_ops；文件映射的缺页经 FAT32_read 追到 ATA 请求的提交、等待、完成三点；退出经 do_exit 放掉文件表与 fs。所有结论都是静态的，**一项也没有运行**。

但有几处不能当作可靠基础：打开路径中两层函数缺少 return，打开成败取决于未定义值；FAT32 普通文件的 read() 把用户缓冲区的内容当作读目标，而且不推进文件位置；close 与 fput 不归还引用；fork 复制出的 file 对象在释放时以另一个 slab cache 的名义记账；退出后既没有父任务通知，也不回收 task_struct 与地址空间。这些都以带 qualified_id 的条目登记在 `deferred-findings.yaml` 中，等以后的专项处理；本文只说明它们在链上的位置。

**范围：** 主对象是 `sched.forkexec` 与 `fs.vfs`，各 9 个代表能力。FAT32 的 ->read 与 ATA 请求只作为第 5 条链的交界端点（2 个能力，object 为 boundary），各追不超过两层。mm、调度、entry、user.initramfs 只作端点，引用既有包的 ID，不重新盘点。数量：20 个能力、43 条边、90 条锚点、44 个检索、7 项词法结构检查、2 张值表、10 个链路断点、6 项未来核验候选（全部 NOT_RUN）。

## 1. 输入与旧 ID

- 开工绑定（`records/scope_start.json`）：19 号任务书经 raw URL 读取，与 git 对象逐字节一致；主线对象 0d066801371d；审查回执 record_id、被审头 de7c96ede546、冻结结果 eb75b3100601、returned_items = MR-01/MR-02、next_followup 都与本任务相符；执行分支包含 de7c96ede546；PR17 为 open 且是 Draft。
- 旧输入只取名字：DR-002 中 `sched.forkexec`、`fs.vfs` 两个条目的 node 名，以及 DR-003 中与两者相交的端点名。旧断言、成熟度和证据都不继承（facts 的 `old_inputs_index`，OLD-U01…OLD-U19）。新的细粒度 ID 一律标 `proposed_id`，不改公共词汇表。
- 旧条目 `sched.forkexec.obvious_missing`、`fs.vfs.obvious_missing` 只用了名字。本批按源码重新取证后，有些现象与旧描述相近（例如 wait4 为空），但这不意味着继承了旧断言。

## 2. 代表能力

| 能力（proposed_id） | 链 | 实现证据 | 正确性 | 主要依据 | 条目 |
|---|---|---|---|---|---|
| `sched.forkexec.first_user_launch` | C1 | 连通正文 | 静态疑点 | A-C1-01、A-C1-02、A-C1-03、A-C1-09… | UV-01 |
| `sched.forkexec.fork_entry` | C2 | 连通正文 | 未评估 | A-C2-03 | — |
| `sched.forkexec.copy_process_resources` | C2 | 连通正文 | 静态疑点 | A-C2-05、A-C2-07、A-C2-08、A-C2-09… | UV-14、UV-16 |
| `sched.forkexec.exec_entry` | C3 | 连通正文 | 静态疑点 | A-C3-01、A-C3-03、A-C3-04、A-C3-05 | UV-02 |
| `sched.forkexec.binfmt_dispatch` | C3 | 连通正文 | 静态疑点 | A-C3-06、A-C3-07、A-C3-09 | UV-03 |
| `sched.forkexec.exec_mm_switch` | C3 | 部分正文 | 静态疑点 | A-C3-10、A-C3-11、A-C3-12 | UV-02 |
| `sched.forkexec.exit_path` | C6 | 部分正文 | 静态疑点 | A-C6-03、A-C6-04、A-C6-07、A-C6-08… | UV-15 |
| `sched.forkexec.wait_reap` | C6 | 仅声明/空体 | 静态疑点 | A-C6-09、A-C6-10、A-C6-11 | UV-15 |
| `sched.forkexec.mm_refcount_release` | C2、C3、C6 | 部分正文 | 静态疑点 | A-C6-14 | UV-15 |
| `fs.vfs.root_switch` | C1 | 连通正文 | 静态疑点 | A-C1-02、A-C1-05、A-C1-06、A-C1-07 | UV-01 |
| `fs.vfs.path_open` | C3、C4 | 连通正文 | 静态疑点 | A-C4-01、A-C4-02、A-C4-03、A-C4-05… | UV-04、UV-07 |
| `fs.vfs.fd_table` | C2、C4、C6 | 连通正文 | 静态疑点 | A-C2-14、A-C2-15、A-C2-16、A-C4-12… | UV-09、UV-11 |
| `fs.vfs.file_lifecycle` | C4、C6 | 部分正文 | 静态疑点 | A-C4-25、A-C4-26、A-C4-28、A-C4-29… | UV-09、UV-10 |
| `fs.vfs.read_syscall` | C4 | 连通正文 | 静态疑点 | A-C4-14、A-C4-15、A-C4-16、A-C4-17 | UV-05、UV-06、UV-11 |
| `fs.vfs.kernel_read` | C3 | 连通正文 | 静态疑点 | A-C3-18、A-C4-18 | UV-06 |
| `fs.vfs.pagecache_read` | C4 | 连通正文 | 静态疑点 | A-C4-19、A-C4-20、A-C4-21、A-C4-22… | UV-05、UV-06、UV-07 |
| `fs.vfs.file_mmap_fault_read` | C5 | 连通正文 | 静态疑点 | A-C5-01、A-C5-02、A-C5-03 | UV-07、UV-08 |
| `fs.vfs.elf_loader` | C3 | 连通正文 | 静态疑点 | A-C3-13、A-C3-14、A-C3-15、A-C3-16… | UV-02 |
| `boundary.fs_fat.fat32_read` | C4、C5 | 连通正文 | 静态疑点 | A-C5-04、A-C5-05、A-C5-06、A-C5-07… | UV-12 |
| `boundary.block.ata_request` | C1、C5 | 连通正文 | 静态疑点 | A-C5-10、A-C5-11、A-C5-12、A-C5-14… | UV-12、UV-13 |

“实现证据”是本批对局部正文的判断，不是成熟度分数，也不能折算成旧 002 的 0–4。20 个能力只是代表样本，不等于这两个对象的全量完成度。

## 3. 六条主链

### C1 启动到首个用户程序

**入口：** `kernel_init` 中先 `myos_switch_to_root_disk()`，再 `kjmp_to_doexecve()`；之后只剩 `while (true)`。

A-C1-03（根盘切换之后跳到首个用户程序；其后只有 while (true)，第 304–306 行）
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	kjmp_to_doexecve();

	while (true);
```

**根盘：** 读 MBR，类型不是 GPT 保护分区就原地死循环；读 GPT 表项后，类型为 EFI 启动分区且下标为 BOOT_FS_IDX 的那一项以 FAT32 挂为根（经 `mount_fs` 的 `->read_super`，绑定为 `read_fat32_superblock`，Q18、Q07）；EXT4 类型分支为空。然后设置初始挂载与任务 root/pwd，并把 devtmpfs 挂到 /dev。

A-C1-05（非 GPT 保护分区类型时打印后原地 while (1)，第 94–98 行）
[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
	if (boot_sec->DPTE[0].type != 0xee &&
		boot_sec->DPTE[0].type != 0xef)
	{
		color_printk(RED, BLACK, "Read MBR failed!\n");
		while (1);
```

A-C1-06（只有 BOOT_FS_IDX 号且类型为 EFI 启动分区的项被 mount_fs("FAT32") 挂为根，第 125–126 行）
[VERIFIED mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk]
```c
				if (i == BOOT_FS_IDX)
					myos_root_sb = mount_fs("FAT32", gpt_pe, fat32_sb);
```

**程序名、参数与装载入口的来源：** 都是源码常量，没有 Linux 那样的 init 搜索顺序。程序名是 "/boot/init"，参数是两个测试串；`kernel_execve` 的返回值被丢弃，随后无条件跳到 `syscall_return_via_sysret`。

A-C1-09（首个用户程序路径是硬编码字符串，第 19–19 行）
[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	const char *initd_name = "/boot/init";
```

A-C1-10（借用 active_mm 作为 mm 后调用 kernel_execve，返回值被丢弃，第 32–33 行）
[VERIFIED mykernel/arch/x86_64/myos/arch_task.c::kjmp_to_doexecve]
```c
	curr->mm = curr->active_mm;
	kernel_execve(initd_name, argv, envp);
```

**用户态：** /boot/init 由 myinitramfs 的 `init` 目标经 CMake `install` 放到安装前缀下的 boot 目录（A-C1-14）。宿主脚本以 sudo 挂盘后安装，本批只读其文本、没有运行；`make_install.sh` 的文本选择静态链接（Q17）。init 程序 fork 之后，子进程 `execve("/boot/sh")`，父进程无限循环 `sched_yield()`，从不 wait（Q16）。

**失败边界：** MBR 不符时死循环；读盘、`init_mount` 的返回值不检查；找不到根分区时的后续行为未读（GAP-U03）；exec 失败时仍跳入用户态（LBU-09，UV-01）。

**覆盖限制：** 镜像内容未核（GAP-U02）；sysret 汇编与 pt_regs 来源未追（GAP-U04）。

### C2 创建 / fork

**入口：** 系统调用表中有 fork，vfork 一项被注释，没有 clone/clone3（Q04）；分派经 `do_syscall_64 → x64_sys_call`（Q20）。`sys_fork` 先空转 0x1000×0x1000 次（Q24），再以 `exit_signal = SIGCHLD`、其余 flags 为 0 调 `kernel_clone`。

**正常路径与复制/共享：** `kernel_clone → copy_process`（A-C2-03）。flags 为 0，所以 mm、文件表、fs 都走复制分支：

A-C2-09（CLONE_VM 共享 mm，否则 dup_mm 复制，第 690–694 行）
[VERIFIED mykernel/sched/forkexec/fork.c::copy_mm]
```c
	if (clone_flags & CLONE_VM) {
		mmget(oldmm);
		mm = oldmm;
	} else if ((mm = dup_mm(tsk, oldmm)) == NULL) {
			return -ENOMEM;
```

文件表复制时，每个打开的 file 被**按值复制成新对象**，而不是共享同一个 file；父表的空槽位在新表中不写入。

A-C2-15（只给父表非空槽位分配新 file 并按值复制；空槽位不写，第 54–58 行）
[VERIFIED mykernel/fs/vfs/file.c::dup_fd]
```c
		if (old_fp != NULL)
		{
			file_s *new_fp = kzalloc(sizeof(file_s), GFP_KERNEL);
			*new_fp = *old_fp;
			new_fds[i] = new_fp;
```

sighand 名义上会复制，但安装语句被注释，所有任务实际共用静态的 `init_sighand`（与 CORE-MM-BASELINE-04::RF-02 一致）：

A-C2-11（分配新 sighand，但安装语句被注释，第 758–759 行）
[VERIFIED mykernel/sched/forkexec/fork.c::copy_sighand]
```c
	sig = kmem_cache_alloc(sighand_cachep, GFP_KERNEL);
	// RCU_INIT_POINTER(tsk->sighand, sig);
```

成功后 `wake_up_new_task`（Q22）；入队位置属于调度，见 INTEGRATION-03（DRU-03）。

**失败与清理：** 标签链依次是 mmput（只减引用）、`__cleanup_sighand(p->sighand)`、exit_fs、exit_files，最后释放任务结构。其中 `__cleanup_sighand` 作用在未加引用的共享 `init_sighand` 上，计数 1 → 0 时会把静态对象交给 `kmem_cache_free`；`virt_to_slab` 对非 slab 页返回 NULL（Q43），随后的 `slab_free` 很可能在持锁时出错（UV-14，条件是 copy_sighand 之后的步骤失败）。另外，`dup_task_struct` 中一行被注释后，下一行分配语句成了 `if (node == NUMA_NO_NODE)` 的唯一语句；唯一调用点传入 NUMA_NO_NODE，目前不触发（UV-16，潜伏，Q44）。

A-C2-08（失败清理：对 p->sighand 调 __cleanup_sighand，第 1403–1404 行）
[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_cleanup_sighand:
	__cleanup_sighand(p->sighand);
```

**覆盖限制：** SMP 与并发可达性不自动成立：用户态没有 clone 入口，共享文件表与 fs 不会经用户态出现（DRU-06）。copy_thread、ret_from_fork 未追。页表复制与 COW 只引用 MM-04（MC-25、LB-09）。

### C3 exec / ELF 装载

**注册与分派：** `init_elf_binfmt` 以 `core_initcall` 登记，`register_binfmt` 的活动调用者只有这一处（Q01、Q02）。分派时对每个格式间接调用 `->load_binary`，绑定为 `load_elf_binary`（Q07）。“非 -ENOEXEC 即返回”的语句被注释，在只有一个格式时不显现（UV-03，潜伏）。

A-C3-06（间接调用每个已注册格式的 load_binary，第 1200–1200 行）
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		retval = fmt->load_binary(bprm);
```

**文件读取：** `do_open_execat` 经 `do_filp_open` 打开文件（进入 C4 的打开路径，因此同样受 UV-04 影响）；`prepare_binprm` 与 `elf_read` 经 `kernel_read` 读取，`kernel_read` 用 kvec 迭代器调 `->read_iter`，在这条路径上读目标是正确的（E31）。

**段映射：** 要求 `f_op->mmap` 存在；各 PT_LOAD 段经 `elf_load → elf_map → vm_mmap` 建文件私有映射。映射内部（选址、VMA 合并）直接引用 CORE-MM-BASELINE-04（MC-11、MC-13 等），本批不重查。段内容不在 exec 时读取，而是首次访问时经 C5 读入。

A-C3-16（段映射走文件 vm_mmap，第 347–347 行）
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
		map_addr = vm_mmap(filep, addr, size, prot, type, off);
```

**参数栈与 mm 切换：** `__bprm_mm_init` 建新 mm 与栈 VMA，参数复制到参数页；`begin_new_exec` 置不可返回点后调 `exec_mmap`，旧 mm 只做 mmput，而 `__mmput` 为空（LBU-08，与 MM-04 LB-03 同一断点）。close-on-exec 被注释（Q14）。

A-C3-10（begin_new_exec 设不可返回点，第 872–872 行）
[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	bprm->point_of_no_return = true;
```

**失败返回：** 越过不可返回点之后，失败只作为返回值上交，致命信号被注释（LBU-05，UV-02）：

A-C3-05（越过不可返回点后的致命信号被注释，第 1280–1281 行）
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// if (bprm->point_of_no_return && !fatal_signal_pending(current))
	// 	force_fatal_sig(SIGSEGV);
```

### C4 文件打开 / 读取 / 关闭

**对象与绑定：** fd 是 `files->fd_array` 的下标；file 持有 `f_path`（dentry 与挂载点）、`f_inode`、`f_mapping`、`f_op`、`f_pos`、`f_count`。`f_op` 在打开时取自 `inode->i_fop`，FAT32 inode 的 `i_fop` 在 `fat_fill_inode` 中绑定为 `FAT32_file_ops`（A-C4-11、Q06）；该对象同时提供 `read`（FAT32_read）、`read_iter`（generic_file_read_iter）和 `mmap`（generic_file_mmap）（Q07）。

A-C4-10（f_op 取自 inode->i_fop；->open 的返回值存入局部 error，第 53–55 行）
[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	f->f_op = inode->i_fop;
	if(f->f_op && f->f_op->open)
		error = f->f_op->open(f->f_path.dentry->d_inode, f);
```

**打开：** `sys_open` 以 dfd = 0 调 `do_sys_open`；`path_init` 只看首字符是否为 '/'，不使用 dfd。`do_sys_openat2` 先取空闲 fd，再 `do_filp_open`，成功后 `fd_install`。`path_openat` 用 `do_open` 的返回值判断成败：

A-C4-07（路径走完后 error 取 do_open 的返回值，第 727–728 行）
[VERIFIED mykernel/fs/vfs/namei.c::path_openat]
```c
	if (error == 0)
		error = do_open(nd, file, flags);
```

而 `do_open` 的整个函数体只是一次 `vfs_open` 调用，没有 return；`myos_do_dentry_open` 也没有 return（SC-01、SC-02）。所以打开的成败在 C 语义下未定义，实际取决于编译产物（UV-04，needs_evidence，候选 NVU-2）。

A-C4-08（do_open 的完整函数体：两行注释加 vfs_open 调用，无 return，第 703–706 行）
[VERIFIED mykernel/fs/vfs/namei.c::do_open]
```c
	// if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
	// 	return -ENOTDIR;

	vfs_open(&nd->path, file);
```

**引用获取与归还：** 打开时 `path_get` 取路径引用，`f_count` 初值 1。`filp_close` 只做 `fput`；`fput` 计数归零时只释放 file 对象，文件中没有 path_put、dput、iput（Q40）。`sys_close` 则绕开 fput，直接 kfree：

A-C4-25（close 直接 kfree file 并清槽位，第 58–59 行）
[VERIFIED mykernel/fs/syscall.c::sys_close]
```c
	kfree(fp);
	curr->files->fd_array[fd] = NULL;
```

**读取：** `sys_read → ksys_read`，用户给的 fd 不经上界检查就用作下标（UV-11）；`vfs_read` 有 `read_iter` 时优先走 `new_sync_read`，用用户缓冲区建 ITER_UBUF 迭代器：

A-C4-16（用户缓冲区建成 ITER_UBUF 后调 ->read_iter，第 85–88 行）
[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	iov_iter_ubuf(&iter, ITER_DEST, buf, len);

	// ret = call_read_iter(filp, &kiocb, &iter);
	ret = filp->f_op->read_iter(&kiocb, &iter);
```

FAT32 的 `read_iter` 落到 `simple_filemap_read`，它从 `iter->kvec` 取读目标与长度：

A-C4-21（读目标与长度一律取自 iter->kvec，第 210–213 行）
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
	void *bufp = iter->kvec->iov_base;
	loff_t
		start = iocb->ki_pos,
		end = start + iter->kvec->iov_len;
```

`kvec` 与 `ubuf` 是同一个联合体成员（A-C4-19、A-C4-20）。所以在 read() 路径上，读目标与长度取自用户缓冲区前 16 字节的内容（UV-05；类型布局推导，未运行，候选 NVU-1）。

**短读、EOF 与文件位置：** `simple_filemap_read` 不按 `i_size` 截短，也不更新 `ki_pos`（SC-04）；`new_sync_read` 把未变的 `ki_pos` 写回，`ksys_read` 再写回 `f_pos`，所以 `f_pos` 不前进。末页长度按下式计算：

A-C4-22（每页拷贝长度公式，第 229–232 行）
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		if (pgcache_pos + PAGE_SIZE < end)
			len = PAGE_SIZE - inpage_start;
		else
			len = end % PAGE_SIZE - inpage_start;
```

end 页对齐时，这个式子会得到 0 或负数（VT-FR01，规则算术）。表中“Σlen”是公式给出的和，不表示函数一定以该值返回：len 为负时，memcpy 以 size_t 计数逐字节拷贝（Q41），越过页面，函数不会正常返回（R3）。

| 行 | 起点 | 长度 | 各页 len（公式） | Σlen（公式） | 期望读到 |
|---|---|---|---|---|---|
| R1 | 0 | 256 | 256 | 256 | 256 |
| R2 | 0 | 4096 | 0 | 0 | 4096 |
| R3 | 100 | 3996 | -100 | -100 | 3996 |
| R4 | 4000 | 200 | 96, 104 | 200 | 200 |
| R5 | 0 | 8192 | 4096, 0 | 4096 | 8192 |

`kernel_read` 也经这条公式：end 页对齐且最后一页从页首开始时，该页长度为 0，得到短读，`elf_read` 把它判为 -EIO；起点落在最后一页页内时长度为负，见上（UV-06）。FAT32_read 本身按 `i_size` 截短（A-C5-05），但普通 read() 不走它，只有页缓存填页和缺页时才调用。

### C5 文件映射到读完成边界

**回调绑定：** `simple_mmap_region → call_mmap → file->f_op->mmap`（Q19）= `generic_file_mmap`，安装 `generic_file_vm_ops`；其 `fault` 为 `simple_filemap_fault`（Q07）。缺页时 `__do_fault` 间接调用 `vm_ops->fault`（Q39）。三项证据（调用点、绑定、接收方）齐全，记在边 E22、E33 的 `indirect` 字段中。

A-C5-01（文件 mmap 安装 generic_file_vm_ops，第 418–418 行）
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_mmap]
```c
	vma->vm_ops = &generic_file_vm_ops;
```

**缺页读：** 在 `page_array[pgoff]` 放一页清零的新页，同步调 `f_op->read` 读一页；返回值被丢弃，只要 vm_file 与 f_mapping 有效就返回 0（UV-08）。

A-C5-03（同步调 ->read，返回值丢弃，随后返回 0，第 132–135 行）
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_fault]
```c
	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
	vmf->page = *pgcache_ptr;

	return 0;
```

**后端第 1 层：FAT32_read。** 沿簇链逐簇调 `ROOTBLK_TRANSFER`。构建选项文本中 `-DROOTBLK_NVME` 被注释（A-C5-09），宏取 ATA 分支：

A-C5-08（非 ROOTBLK_NVME 分支：ROOTBLK_TRANSFER 展开为 ATA_master_ops.transfer，第 55–56 行）
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::ROOTBLK_TRANSFER]
```c
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				ATA_master_ops.transfer(MASTER, SLAVE, cmd, blk_idx, count, buffer)
```

这只是配置文本，不升级为“全系统唯一后端”；NVMe 分支未追（GAP-U07）。

**后端第 2 层：ATA 请求。** 提交点与等待点：

A-C5-10（请求挂到 IDEreq_lhdr 尾部并唤醒 ATArqd 线程（提交点），第 281–285 行）
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		spin_lock(&req_lock);
		list_header_add_to_tail(&IDEreq_lhdr, &node->req_list);
		spin_unlock(&req_lock);

		wake_up_process(thread);
```

A-C5-11（调用者在栈上 completion 上等待（等待点），第 286–286 行）
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_transfer]
```c
		wait_for_completion(&done);
```

守护线程从链表尾部取请求并发出 PIO 命令（Q29、A-C5-14）；中断处理按 `req_in_using` 回调读数据，计数归零时完成：

A-C5-15（中断处理按 req_in_using 回调 end_handler，计数归零时 end_request，第 320–324 行）
[VERIFIED mykernel/arch/x86_64/myos/ide.c::ATA_disk_handler]
```c
	blkbuf_node_s *node = req_in_using;
	node->end_handler((unsigned long)node);
	
	if (node->count == 0)
		end_request(node);
```

A-C5-16（完成点，第 155–155 行）
[VERIFIED mykernel/arch/x86_64/myos/ide.c::end_request]
```c
	complete(node->done);
```

到此本链闭合于提交、等待、完成三点。中断分发（register_irq → do_IRQ）与 PIO 细节未追（GAP-U07、GAP-U08）。等待原语的行为证据见 CORE-CHECK-01::V02/V03/V10/V11（有限宿主观察，消费时须一起读 recheck-02 审查，DRU-01）。

**失败边界：** 设备错误只打印；`ATA_disk_transfer` 对读写命令总是返回 -ENOERR（A-C5-12），所以 FAT32_read 的 -EIO 分支在 ATA 配置下不会进入；缺页者也看不到读失败（LBU-07）。

### C6 退出 / 回收

**入口：** `sys_exit → do_exit`；`sys_exit_group → do_group_exit → do_exit`（Q24）。

**do_exit 实际做的事：** exit_mm 被注释；exit_files、exit_fs 是活动的；exit_notify 把子任务挂到 1 号任务名下；最后 do_task_dead。

A-C6-03（exit_mm 被注释：退出不处理 mm，第 120–120 行）
[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_mm();
```

A-C6-04（活动的退出步骤：文件表与 fs，第 128–129 行）
[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	exit_files(tsk);
	exit_fs(tsk);
```

**四类区分：**

| 类别 | 内容 | 依据 |
|---|---|---|
| 函数空壳 | wait4（函数体为空，返回值未定义）；__mmput（函数体全为注释） | SC-03、SC-07 |
| 仅减引用 | mmput（归零后调用空的 __mmput）；fput（只释放 file 对象，不放路径引用） | A-C6-14、A-C4-28、Q40 |
| 活动释放 | put_files_struct 释放 files_struct 并对每个 file filp_close；exit_fs 在引用归零时 free_fs_struct（path_put root/pwd） | A-C4-31、A-C6-12、A-C6-04 |
| 真正归还资源 | 只有上一行中的 slab 对象；地址空间、task_struct、内核栈都不归还 | A-C6-03、A-C6-10、A-C6-11、Q12 |

**等待与通知：** 不设 ZOMBIE，也不通知父任务；`finish_task_switch` 的 TASK_DEAD 段被注释，`release_task` 只有声明（LBU-01、LBU-02，UV-15）。

A-C6-09（不进入 ZOMBIE，也不通知父任务，第 37–37 行）
[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	// tsk->exit_state = EXIT_ZOMBIE;
```

A-C6-10（切换后的 TASK_DEAD 处理整体被注释，第 856–856 行）
[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// if (unlikely(prev_state == TASK_DEAD)) {
```

MM 已知断点引用原 ID（CORE-MM-BASELINE-04::LB-03、::MC-16），本批只补上进程侧的连接证据（LBU-08），不另起新号。

## 4. 依赖与断点

**边：** 共 43 条，按类别为 build 1、call 34、config 1、data 3、init_order 4。方向沿用 MM 包：调用方 → 被调用方；读取方 → 提供方；初始化顺序为“后依赖先”。include 不算调用，GLOB 不算运行可达，词法上的 active 也不等于预处理或链接确认。

**间接调用：** 每条都给出调用点、对象或回调绑定、接收方三项证据：

| 边 | 调用点 | 绑定 | 接收方 |
|---|---|---|---|
| E08 | Q18 fs/vfs/myos_vfs.c:158 | Q07 fs/fat/myos_fat32.c:503 | Q06 fs/fat/myos_fat32.c:477 |
| E10 | Q20 arch/x86_64/entry/common.c:44 | Q04 arch/x86_64/include/asm/syscalls_64.h:31 | Q24 sched/syscall.c:55 |
| E19 | A-C3-06 | Q07 fs/vfs/binfmt_elf.c:85 | A-C3-13 |
| E22 | Q19 mm/vm_map/mmap.c:693 | Q07 fs/fat/myos_fat32.c:362 | A-C5-01 |
| E27 | A-C4-10 | A-C4-11 | Q07 fs/fat/myos_fat32.c:356 |
| E29 | A-C4-16 | Q07 fs/fat/myos_fat32.c:360 | A-C4-24 |
| E31 | A-C4-18 | Q07 fs/fat/myos_fat32.c:360 | A-C4-24 |
| E32 | Q25 mm/vm_map/filemap.c:222 | Q07 fs/fat/myos_fat32.c:357 | A-C5-05 |
| E33 | Q39 mm/fault/fault.c:901 | Q07 mm/vm_map/filemap.c:185 | A-C5-02 |
| E34 | A-C5-03 | Q07 fs/fat/myos_fat32.c:357 | A-C5-05 |
| E35 | A-C5-06 | A-C5-08 | A-C5-10 |

`ROOTBLK_TRANSFER` 是宏，E35 把它按宏定义加配置前提记为调用，另用 config 边 E36 记录配置前提。`syscall_return_via_sysret`（E07）只到端点，目标汇编未读。

**链路断点（facts 的 link_breaks）：**

| ID | 位置 | 内容 |
|---|---|---|
| LBU-01 | sched.forkexec.exit_path → sched.forkexec.wait_reap（父任务通知与 wait4） | exit_notify 不设 ZOMBIE、不通知父任务；wait4 的函数体为空。父任务没有获取子任务退出状态的活动路径。 |
| LBU-02 | TASK_DEAD → task_struct 与内核栈的释放 | finish_task_switch 的 TASK_DEAD 段被注释，release_task 只有声明；sched/ 中没有活动的 put_task_struct。 |
| LBU-03 | close → fput / ->release | sys_close 直接 kfree，不经 fput，不看引用计数；fs/vfs 中没有活动的 ->release 调用。 |
| LBU-04 | fput → path_put / dput / iput | myos_do_dentry_open 取路径引用；file_table.c 中没有 path_put、dput、iput，fput 归零只释放 file 对象。 |
| LBU-05 | exec 越过不可返回点后的失败 → 致命信号 | force_fatal_sig 被注释；失败只作为返回值上交。 |
| LBU-06 | read(FAT32 普通文件) → f_pos 前进与 EOF | simple_filemap_read 不更新 ki_pos，new_sync_read 把未变的 ki_pos 写回，f_pos 不前进；也不按 i_size 截短，因而没有 EOF。 |
| LBU-07 | 块设备错误 → 调用者 | IDE_read_handler 只打印错误；ATA_disk_transfer 总是返回 -ENOERR；FAT32_read 的 -EIO 分支在 ATA 配置下不进入；simple_filemap_fault 也丢弃 ->read 的结果。 |
| LBU-08 | 进程退出 / exec 换 mm → 地址空间拆除 | exit_mm 被注释；mmput 归零后 __mmput 为空。与 CORE-MM-BASELINE-04::LB-03 同一断点，本批补上进程侧的连接证据。 |
| LBU-09 | kernel_execve 失败 → 首个程序启动的错误处理 | kjmp_to_doexecve 丢弃返回值并无条件跳到 sysret。 |
| LBU-10 | vfork → 父任务等待 | 系统调用表中 vfork 被注释；wait_for_vfork_done/complete_vfork_done 只在注释中。 |

**依赖风险：** DRU-01（读盘依赖 completion，引用 CORE-CHECK-01 的 V 记录）、DRU-02（缺页中同步等待，引用 MM-04 DR-01/GAP-M05）、DRU-03（调度）、DRU-04（MM 回收断点）、DRU-05（fork 后的 TLB/COW）、DRU-06（并发前提）。详见 facts 的 `dependency_risks`。

**否定断言的范围：** 所有“没有”都附带检索范围、返回码与活动命中数（Q05、Q11、Q12、Q13、Q14、Q30、Q40），或者是对单个函数体的词法检查（SC-01…SC-07）。它们只说明所查范围内没有，不推广为全树结论。

## 5. U04：可用性（两层）

**第一层：各段的静态连接状态。** connected 表示已由源码连接；connected_with_concern 表示已连接但有登记的疑点；connected_under_config_text 表示以构建选项文本为前提；build_text_only 表示只有构建文本；endpoint 表示只到端点；broken 表示断开。

| 链 | 段 | 状态 | 依据 | 说明 |
|---|---|---|---|---|
| C1 | kernel_init → myos_switch_to_root_disk | connected | A-C1-02 | — |
| C1 | 根切换 → FAT32 ->read_super | connected | A-C1-06、Q18、Q07 | 间接调用三项证据齐全；登记过程未逐行读 |
| C1 | 根切换 → 读盘后端 | connected_under_config_text | Q37、A-C5-08、A-C5-09 | ATA 分支以构建选项文本为前提 |
| C1 | kernel_init → kjmp_to_doexecve → kernel_execve | connected | A-C1-03、A-C1-10 | — |
| C1 | kernel_execve 失败 → 错误处理 | broken | A-C1-10、A-C1-11 | LBU-09 |
| C1 | kjmp → 用户态（sysret） | endpoint | A-C1-11 | 汇编未读 |
| C1 | /boot/init 文件来源 | build_text_only | A-C1-14、Q17 | 镜像未核 |
| C2 | 系统调用表 → sys_fork | connected | Q04、Q20 | — |
| C2 | sys_fork → kernel_clone → copy_process | connected | Q03、A-C2-03 | — |
| C2 | copy_process → dup_fd（文件表复制） | connected | A-C2-10、A-C2-15 | — |
| C2 | copy_process → dup_mm → dup_mmap | connected | A-C2-09 | 内部属 MM-04 |
| C2 | copy_sighand → 子任务 sighand | broken | A-C2-11 | 安装语句被注释，任务共用 init_sighand |
| C2 | kernel_clone → wake_up_new_task | connected | Q22 | 入队位置属调度 |
| C2 | vfork → 父任务等待 | broken | Q04、Q05 | LBU-10；用户态无 vfork 入口 |
| C2 | 失败清理 | connected_with_concern | A-C2-07、A-C2-08 | UV-14 |
| C3 | execve/kernel_execve → bprm_execve | connected | A-C3-01、Q21 | — |
| C3 | do_open_execat → do_filp_open | connected_with_concern | A-C3-03 | UV-04：打开成败的返回值未定义 |
| C3 | search_binary_handler → load_elf_binary | connected | A-C3-06、Q07、A-C3-09 | — |
| C3 | load_elf_binary → begin_new_exec → exec_mmap | connected | A-C3-14、A-C3-11 | — |
| C3 | load_elf_binary → elf_map → vm_mmap | connected | A-C3-15、A-C3-16 | 映射内部属 MM-04 |
| C3 | elf_read → kernel_read → read_iter | connected | A-C3-18、A-C4-18 | — |
| C3 | 不可返回点后的失败 → 终止进程 | broken | A-C3-05 | LBU-05 |
| C3 | 旧 mm → 拆除 | broken | A-C3-12 | LBU-08，同 MM-04 LB-03 |
| C4 | sys_open → do_filp_open → f_op 绑定 | connected_with_concern | A-C4-01、A-C4-02、A-C4-10、A-C4-11 | UV-04 |
| C4 | open → fd_install | connected | A-C4-03 | — |
| C4 | sys_read → vfs_read → read_iter | connected | A-C4-15、A-C4-16、Q07 | — |
| C4 | read_iter → simple_filemap_read → FAT32_read | connected | A-C4-24、Q25、Q07 | — |
| C4 | 读目标与长度 | connected_with_concern | A-C4-19、A-C4-21、A-C4-22 | UV-05、UV-06 |
| C4 | 读 → f_pos 前进 / EOF | broken | A-C4-14、A-C4-23 | LBU-06 |
| C4 | close → 释放 file | connected_with_concern | A-C4-25 | 直接 kfree |
| C4 | close / fput → ->release 与路径引用归还 | broken | A-C4-28 | LBU-03、LBU-04 |
| C5 | mmap → f_op->mmap → vm_ops 安装 | connected | Q19、Q07、A-C5-01 | — |
| C5 | 缺页 → vm_ops->fault → simple_filemap_fault | connected | Q39、Q07 | — |
| C5 | simple_filemap_fault → f_op->read → FAT32_read | connected | A-C5-03、A-C4-11、Q07 | — |
| C5 | FAT32_read → ROOTBLK_TRANSFER → ATA_disk_transfer | connected_under_config_text | A-C5-06、A-C5-08、A-C5-09、Q07 | — |
| C5 | ATA 提交 → 等待 | connected | A-C5-10、A-C5-11 | — |
| C5 | 守护线程 → 控制器命令 | connected | Q29、A-C5-14 | — |
| C5 | 中断 → complete | connected | Q09、A-C5-15、A-C5-16 | 中断分发未追 |
| C5 | 设备错误 → 缺页者 | broken | A-C5-12、A-C5-17、A-C5-03 | LBU-07 |
| C6 | exit/exit_group → do_exit | connected | Q24 | — |
| C6 | do_exit → exit_files → put_files_struct → fput | connected_with_concern | A-C6-04、A-C6-12、A-C4-31 | UV-09 |
| C6 | do_exit → mm 释放 | broken | A-C6-03、A-C6-14 | LBU-08 |
| C6 | exit_notify → 改挂子任务 | connected_with_concern | A-C6-07、A-C6-08 | — |
| C6 | exit → 父任务通知 / wait4 | broken | A-C6-09 | LBU-01 |
| C6 | do_task_dead → 回收 task_struct | broken | A-C6-10、A-C6-11 | LBU-02 |

**第二层：现在能讲解什么，将来运行需要什么。**

| 链 | 现在能从源码讲解的范围 | 未来运行所需的前置 |
|---|---|---|
| C1 | 能讲清：首个程序名与参数的来源、根分区的选择规则（GPT、EFI 启动分区、BOOT_FS_IDX）、root/pwd 与 /dev 的建立顺序，以及 exec 失败时为什么仍会跳到用户态。 | 需要授权的完整内核构建与 QEMU，以及包含 /boot/init、/boot/sh 的根盘镜像；进入用户态还依赖调度前置条件（INTEGRATION-03）。 |
| C2 | 能讲清：fork 入口、各资源是复制还是共享、失败标签链的顺序，以及 sighand 实际共享这一现状。 | 观察父子返回值、文件表副本与失败路径，需要宿主夹具或 QEMU 授权；失败路径需要注入分配失败。 |
| C3 | 能讲清：格式注册与分派、ELF 校验与段映射、换 mm 的时点与不可返回点，以及读 ELF 头经过的 VFS 路径。 | open 返回值的实际取值要看构建产物（NVU-2）；装载成功与否需要 QEMU 与真实镜像。 |
| C4 | 能讲清：fd、file、dentry/inode、f_op 的关系与绑定位置，打开时的引用获取，以及 close/fput 不归还的部分；读路径中 read_iter 与 ->read 的分工。 | read 的实际行为（UV-05、UV-06）可先在宿主夹具中验证（NVU-1），无需 QEMU；涉及 /dev/console 的读写需要先追 devtmpfs。 |
| C5 | 能讲清：文件映射如何装上 vm_ops，缺页如何同步读一页，以及到 ATA 请求提交、等待、完成的两层后端。 | 需要构建产物确认 ROOTBLK_NVME 与链接保留；完成通知的可靠性依赖 completion 记录（DRU-01）；运行观察需要 QEMU。 |
| C6 | 能讲清：退出时实际释放的对象、只减引用的对象、完全缺失的回收（wait4、task_struct、mm）。 | 观察 wait4 返回值与资源占用需要 QEMU 与用户程序配合（修改用户程序需另行授权）。 |

## 6. 问题入库

本批发现集合非空，全部按 18 号约定写入 `deferred-findings.yaml`：16 项新条目（UV-01…UV-16，qualified_id 为 `CORE-USER-VFS-BASELINE-05::UV-xx`；其中 deferred_owner_not_ready 12 项、needs_evidence 4 项），2 项文档勘误（MR-01、MR-02），10 项旧条目状态变更，13 项交叉链接。所有条目 `owner_action_now: none`，`owner_decisions_requested_now: []`。

| ID | 标题 | 状态 |
|---|---|---|
| UV-01 | 首个程序启动链的失败边界 | deferred_owner_not_ready |
| UV-02 | exec 越过不可返回点后失败只返回错误 | deferred_owner_not_ready |
| UV-03 | 格式分派不提前返回（潜伏） | deferred_owner_not_ready |
| UV-04 | 打开路径两层函数缺少 return，成败取决于未定义值 | needs_evidence |
| UV-05 | FAT32 普通文件 read()：读目标取自用户缓冲区的内容 | deferred_owner_not_ready |
| UV-06 | 页缓存读的长度、位置与文件尾语义 | deferred_owner_not_ready |
| UV-07 | page_array 下标没有上界 | deferred_owner_not_ready |
| UV-08 | 文件缺页读的失败对缺页者不可见 | deferred_owner_not_ready |
| UV-09 | fork 的文件表复制与跨 cache 释放 | needs_evidence |
| UV-10 | close 与 fput 的引用语义 | deferred_owner_not_ready |
| UV-11 | fd 与读入口的边界检查 | deferred_owner_not_ready |
| UV-12 | FAT32_read 与 ATA 后端的局部错误处理 | deferred_owner_not_ready |
| UV-13 | ATA 请求完成链的局部前提 | needs_evidence |
| UV-14 | fork 失败清理对共享 init_sighand 减引用 | needs_evidence |
| UV-15 | 退出与回收的缺口 | deferred_owner_not_ready |
| UV-16 | dup_task_struct 中被注释行吞掉的 if 体（潜伏） | deferred_owner_not_ready |

## 7. 未来核验候选（全部 NOT_RUN）

| ID | 类型 | 目的 | 前置授权 |
|---|---|---|---|
| NVU-1 | host | 验证 simple_filemap_read 的读目标（ITER_UBUF 与 kvec 联合体）、长度公式（VT-FR01）与 f_pos 不前进（UV-05、UV-06） | 宿主夹具执行授权（当前 host_fixture_execution_authorized=false）；filemap.c、read_write.c 片段与 FAT32_read 的宿主桩 |
| NVU-2 | build_artifact | 确认 do_open/myos_do_dentry_open 缺少 return 时 path_openat 实际读到的 error 值（UV-04） | 完整内核构建授权（当前 full_kernel_build_authorized=false）；不需要运行 |
| NVU-3 | host | 观察 dup_fd 副本的来源 cache、空槽位初值，以及 fput 释放时 kmem_cache_free 收到的 cache（UV-09） | 宿主夹具执行授权；slub 分配释放的宿主桩 |
| NVU-4 | qemu | 最小启动观察：是否进入 /boot/init，fork 与 execve /boot/sh 是否完成（C1、C2、C3） | 完整内核构建与 QEMU 授权；带 /boot/init、/boot/sh 的根盘镜像；调度前置条件（INTEGRATION-03） |
| NVU-5 | qemu | 首个程序缺失或损坏时 kjmp_to_doexecve 的行为（UV-01、UV-02） | 完整内核构建与 QEMU 授权；可修改的测试镜像 |
| NVU-6 | qemu | 子进程退出后 wait4 的返回值与 task_struct、mm 的占用（UV-15） | 完整内核构建与 QEMU 授权；修改用户程序需另行授权 |

这些候选不是 Owner 当前的作业，本批没有启动任何宿主实验、构建或 QEMU。

## 8. 未覆盖与缺口

- **GAP-U01：** 构建产物未知：do_open、myos_do_dentry_open 返回值的实际取值依赖编译产物（UV-04）；Debug/Release 与 --gc-sections 的影响继承 CORE-MM-BASELINE-04::GAP-M01/GAP-M02
- **GAP-U02：** 镜像内容未核：/boot/init、/boot/sh 是否存在，是静态还是动态链接（Q17 文本为 static），ET_EXEC 还是 ET_DYN；与 CORE-MM-BASELINE-04::GAP-M06 同类
- **GAP-U03：** set_init_mount、set_init_taskfs、init_mount(devtmpfs) 的正文未逐行读；找不到根分区时的行为未知
- **GAP-U04：** syscall_return_via_sysret、ret_from_fork 与 pt_regs 内容（首个程序进入用户态与子任务返回时的寄存器来源）未追
- **GAP-U05：** devtmpfs 与 /dev/console 的 f_op 未追；init 用 freopen 打开 /dev/console，本批读路径只覆盖 FAT32 普通文件
- **GAP-U06：** 写路径（generic_file_write_iter、FAT32_write）、目录读取、dcache 回收、O_CREAT、namespace/mount 细节未覆盖
- **GAP-U07：** NVMe 后端（ROOTBLK_NVME 分支）与中断分发（register_irq → do_IRQ）、init_bdev_intr 调用时机未追
- **GAP-U08：** IDE_cmd_out 的 PIO 细节、LBA28 范围、单请求扇区数上限未核
- **GAP-U09：** copy_thread、kernel_thread 的 flags、pid 分配、wake_up_new_task 的入队位置未追（后者属 INTEGRATION-03）
- **GAP-U10：** 用户程序所用 musl 的系统调用选择（fork、exit_group、open 等）只按常识推断，未读 musl 源码

另外：devtmpfs 与 /dev/console 的读写、写路径、目录读取、dcache 回收、命名空间与挂载细节都不在本批范围内；`user.initramfs` 只读源码与构建文本。
