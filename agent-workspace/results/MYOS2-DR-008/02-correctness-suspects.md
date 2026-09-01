---
task_id: MYOS2-DR-008
produced_by: GPT-5.6 Pro
date: '2026-09-01'
base_snapshot: time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
- agent-workspace/conventions.md
- agent-workspace/repo-map.md
- agent-workspace/tasks/MYOS2-DR-008-tech-debt-register.md
- .gitignore
- todo.txt
- bugs_record.md
- changelog.md
- make_install.sh
- dbg-vmw.sh
- dbg-qemu.sh.bak
- .vscode/c_cpp_properties.json
- .vscode-kdbg/run-qemu-gdb-myos2.sh
- mykernel/CMakeLists.txt
- mykernel/scripts/options_flags.cmake
- mykernel/scripts/target_kbuild.cmake
- mykernel/scripts/asm-offset_copy_asm.py
- mykernel/block/CMakeLists.txt
- mykernel/block/part_core.c
- mykernel/block/part_efi.c
- mykernel/cpu/vmware.c
- mykernel/init/main.c
- mykernel/arch/x86_64/entry/common.c
- mykernel/arch/x86_64/myos/interrupt.c
- mykernel/arch/x86_64/mm/fault.c
- mykernel/arch/x86_64/lib/delay.c
- mykernel/arch/x86_64/smp/setup_percpu.c
- mykernel/mm/vm_map/filemap.c
- mykernel/mm/page_alloc/mempolicy.c
- mykernel/fs/vfs/fs_context.c
- mykernel/fs/vfs/myos_vfs.c
- mykernel/sched/scheduler/scheduler_core.c
- mykernel/sched/forkexec/fork.c
- mykernel/sched/forkexec/exit.c
- mykernel/sched/forkexec/kthread.c
- mykernel/lock_IPC/futex/futex.c
- mykernel/lock_IPC/syscall.c
- mykernel/lock_IPC/signal/signal.c
- mykernel/time/adjtime/adjtime.c
- mykernel/time/adjtime/adjtime.h
- mykernel/time/adjtime/adjtime_api.h
- mykernel/include/linux/kernel/threads.h
- myloader/CMakeLists.txt
- myloader/uefi/install_loader.sh
- myinitramfs/CMakeLists.txt
- myinitramfs/myinit/CMakeLists.txt
- myinitramfs/myshell/CMakeLists.txt
- scripts/map_vdisk.sh
- scripts/part_vdisk.sh
- scripts/phys_nvme_install.sh
- mykernel/include/obsolete/glo.h
- mykernel/include/obsolete/ktypes.h
- mykernel/include/obsolete/printk.h
- mykernel/include/obsolete/proto.h
- mykernel/arch/x86_64/include/obsolete/arch_proto.h
- mykernel/arch/x86_64/include/obsolete/device.h
- mykernel/arch/x86_64/include/obsolete/ide.h
- mykernel/arch/x86_64/include/obsolete/keyboard.h
- mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h
status: final
open_questions:
- 'OQ-001: 顶层递归 glob 静态上会纳入 part_core.c/part_efi.c，但本会话无法执行完整构建，尚未用最终链接映射再次证明所有配置均包含它们。'
- 'OQ-002: 旧 mykernel/fs/vfs/myos_vfs.c 根盘 GPT 路径与新 mykernel/block/part_* 路径谁是当前唯一权威实现，所读快照未给明确所有权说明。'
- 'OQ-003: 调度器其他文件是否存在可补偿 enqueue/TTWU/set_task_cpu 缺失语义的自定义机制，已读调用链中未见，但未做运行时不变量追踪。'
- 'OQ-004: 仓库中的 myloader/BOOTX64.EFI 是否由 time@a039d980 的 uefi 源码生成，缺少可重复构建记录与哈希证明。'
- 'OQ-005: bugs_record.md 的四条历史故障在 time@a039d980 上是否仍可复现，未执行启动/硬件测试。'
- 'OQ-006: 9 个仍被活动源码依赖的 obsolete 头应按何种顺序迁移，需结合编译依赖图和回归覆盖决定。'
- 'OQ-007: 因连接器环境无法进行完整编译、链接和静态分析，仓库中可能仍有未被关键词/定向阅读捕获的非 void 跌落、未初始化变量或死循环。'
---
# MYOS2-DR-008 正确性隐患专表

## 使用说明

[VERIFIED debt-register.yaml] 当前快照正确性隐患共 **32** 条，其中 S1 28 条、S2 4 条；另列历史材料 4 条，均未核现状。

[INFERRED] 本表用于选择调试桩和回归用例，不代表所有条目已在运行时复现。优先级规则是：先处理可由用户/设备入口触达的永久自旋、未定义返回、未初始化指针与破坏性写入，再处理资源/边界和可移植性问题。

证据等级：`high` 表示源码控制流和活动注册/调用关系可直接确认；`medium` 表示源码事实明确，但最终运行影响依赖配置或调用场景；`low` 仅用于历史材料。

## S1 · 当前快照

| ID | subsystem | symbol/位置 | 静态失败模式 | 证据 | 回归保护 |
|---|---|---|---|---|---|
| DR008-001 | `lock.futex` | `do_futex` | 任一进入该系统调用实现的线程都会永久占用执行上下文；若发生在关键内核路径，影响可扩大。 | [mykernel/lock_IPC/futex/futex.c:L6-L56](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/lock_IPC/futex/futex.c#L6-L56) · high | 先加最小 futex syscall 回归：未知/未支持操作必须快速返回错误，不得自旋；再恢复或显式禁用实现。 |
| DR008-002 | `lock.futex` | `__do_sys_futex` | 合法的超时 futex 请求不能返回，用户态同步库可能挂死。 | [mykernel/lock_IPC/syscall.c:L105-L136](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/lock_IPC/syscall.c#L105-L136) · high | 回归覆盖 FUTEX_WAIT timeout：返回受支持结果或 -ENOSYS/-EOPNOTSUPP，禁止无限循环。 |
| DR008-003 | `entry` | `__x64_sys_ni_syscall` | 用户态可通过未实现或越界系统调用使当前 CPU 长时间停留在内核态。 | [mykernel/arch/x86_64/entry/common.c:L18-L50](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/entry/common.c#L18-L50) · high | 建立 syscall gap/out-of-range 回归，期望 ax=-ENOSYS 并正常返回用户态。 |
| DR008-004 | `block` | `check_partition` | 调用者可能获得不确定返回值并继续解引用或误判分区状态。 | [mykernel/block/part_core.c:L120-L187](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/block/part_core.c#L120-L187) · high | 先用编译器 -Wreturn-type 设为错误，并加空盘/GPT 盘探测回归。 |
| DR008-005 | `block` | `blk_add_partitions` | bdev_disk_changed() 消费未定义返回值，可能错误重试、误报成功或破坏后续控制流。 | [mykernel/block/part_core.c:L190-L248](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/block/part_core.c#L190-L248) · high | 先加空分区表/有效 GPT/读错误三类回归，再恢复完整收尾。 |
| DR008-006 | `block` | `last_lba/read_lba` | GPT 校验得到的 LBA 与读取字节数不确定，后续边界检查失去意义。 | [mykernel/block/part_efi.c:L100-L155](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/block/part_efi.c#L100-L155) · high | 将缺失返回视为构建错误；以已知磁盘容量和短读场景覆盖。 |
| DR008-007 | `block` | `find_valid_gpt` | 一旦读盘实现恢复或被替换，可能向任意地址写入；当前函数本身还存在未定义返回。 | [mykernel/block/part_efi.c:L165-L275](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/block/part_efi.c#L165-L275) · high | 先写保护 MBR/损坏主 GPT/备用 GPT 回归；再恢复分配、清理与所有返回路径。 |
| DR008-008 | `block` | `efi_partition` | GPT 探测结果可能随机，且成功分支泄漏分配对象。 | [mykernel/block/part_efi.c:L295-L345](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/block/part_efi.c#L295-L345) · high | 回归验证有效 GPT 返回 1、无 GPT 返回 0、I/O 错误返回负值及资源释放。 |
| DR008-009 | `sched.forkexec` | `dup_task_struct` | 指定节点创建任务时可产生随机指针解引用、崩溃或内存破坏。 | [mykernel/sched/forkexec/fork.c:L260-L295](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/forkexec/fork.c#L260-L295) · high | 启用 -Wmaybe-uninitialized/-Wmisleading-indentation，并覆盖 NUMA_NO_NODE 与具体 node 两条路径。 |
| DR008-010 | `sched.scheduler` | `enqueue_task/dequeue_task` | 任务运行队列状态与调用者判断可能分叉，睡眠/唤醒及调度选择失真。 | [mykernel/sched/scheduler/scheduler_core.c:L28-L108](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L28-L108) · high | 先建立 runnable→sleep→wake 的队列不变量测试，再恢复单一队列所有权。 |
| DR008-011 | `sched.scheduler` | `try_to_wake_up` | 睡眠任务可能未入队，调用者又被告知未唤醒；信号和 kthread 创建路径均依赖此接口。 | [mykernel/sched/scheduler/scheduler_core.c:L318-L510](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L318-L510) · high | 回归覆盖当前任务、同 CPU 睡眠任务、远端 CPU 睡眠任务及重复唤醒。 |
| DR008-012 | `sched.scheduler` | `set_task_cpu` | task_cpu(p) 与实际队列归属可能不一致，并存在并发链表损坏风险。 | [mykernel/sched/scheduler/scheduler_core.c:L116-L180](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L116-L180) · high | 先断言 task_cpu、队列归属和 rq 锁一致，再恢复受锁保护的迁移。 |
| DR008-013 | `sched.scheduler` | `kick_process/resched_curr` | 远端运行任务可能不能及时进入内核处理信号或重新调度。 | [mykernel/sched/scheduler/scheduler_core.c:L183-L215](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L183-L215) · high | 用双 CPU 测试远端 signal/resched IPI 到达和 need_resched 清除。 |
| DR008-014 | `sched.scheduler` | `wake_up_new_task` | 新任务全部堆积在 CPU0，且任务 CPU 元数据可能与队列归属不一致。 | [mykernel/sched/scheduler/scheduler_core.c:L650-L705](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L650-L705) · high | 回归检查多核 fork 后的队列分布、CPU 字段和可运行性。 |
| DR008-015 | `mm.vm_map` | `filemap_page_mkwrite` | 共享可写文件映射触发回调时返回值未定义，写保护/脏页状态不可预测。 | [mykernel/mm/vm_map/filemap.c:L150-L205](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/mm/vm_map/filemap.c#L150-L205) · high | 回归覆盖 MAP_SHARED 写故障、脏页标记和错误返回；将 -Wreturn-type 设为错误。 |
| DR008-016 | `mm.vm_map` | `simple_filemap_read/generic_perform_write` | 页对齐结尾的最后整页会被漏读或漏写，返回字节数也会短一页。 | [mykernel/mm/vm_map/filemap.c:L205-L365](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/mm/vm_map/filemap.c#L205-L365) · high | 参数化测试 0、1、PAGE_SIZE-1、PAGE_SIZE、PAGE_SIZE+1 与非零起始偏移。 |
| DR008-017 | `arch.x86_64` | `hwint_irq_handler` | 需要按 IRQ/向量确认的控制器可能无法清除非零中断源，造成丢中断或中断风暴。 | [mykernel/arch/x86_64/myos/interrupt.c:L155-L180](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/myos/interrupt.c#L155-L180) · high | 为至少两个非零 IRQ 注入中断并记录 ack 参数与 EOI 状态。 |
| DR008-018 | `fs.vfs` | `myos_switch_to_root_disk` | 畸形或较大的 GPT 头可导致越界读/写和越界遍历。 | [mykernel/fs/vfs/myos_vfs.c:L75-L155](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/fs/vfs/myos_vfs.c#L75-L155) · high | 使用规范上限和溢出检查；回归覆盖最大条目数、无零终止条目和短读。 |
| DR008-019 | `mm.page_alloc` | `folio_alloc_noprof` | 请求高阶 folio 或不同分区/权限标志的调用者会收到语义不匹配的内存。 | [mykernel/mm/page_alloc/mempolicy.c:L105-L122](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/mm/page_alloc/mempolicy.c#L105-L122) · high | 回归验证 order 0/1/2 的页数、对齐和传入 GFP 标志传播。 |
| DR008-020 | `fs.vfs` | `legacy_init_fs_context/put_fs_context` | 每次 legacy 文件系统上下文销毁都会泄漏至少一个 legacy_fs_ctx_s，并可能泄漏其中数据。 | [mykernel/fs/vfs/fs_context.c:L85-L175](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/fs/vfs/fs_context.c#L85-L175) · high | 建立 mount-context 成功/失败注入的分配计数测试，再恢复 free 回调。 |
| DR008-021 | `sched.forkexec` | `do_exit` | 退出通知逻辑接收随机组终止状态；未来恢复相关分支时会造成不确定行为。 | [mykernel/sched/forkexec/exit.c:L70-L155](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/forkexec/exit.c#L70-L155) · high | 启用未初始化变量诊断，并覆盖单线程退出与最后一个线程退出。 |
| DR008-022 | `sched.forkexec` | `do_exit/do_group_exit` | 反复创建/退出进程可能泄漏地址空间与对象，线程组语义也不完整。 | [mykernel/sched/forkexec/exit.c:L70-L180](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/forkexec/exit.c#L70-L180) · high | 按资源类别建立退出后计数基线，逐项恢复并验证幂等清理。 |
| DR008-023 | `mm.fault` | `do_user_addr_fault` | OOM、权限、总线错误或任何可恢复/不可恢复错误都会卡死当前 CPU，而不是向进程返回信号/错误。 | [mykernel/arch/x86_64/mm/fault.c:L250-L330](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/mm/fault.c#L250-L330) · high | 故障注入覆盖 OOM、权限拒绝、无 VMA、COW；每条路径必须终止或重试时重新求值。 |
| DR008-024 | `mm.fault` | `myos_bad_area/do_kern_addr_fault` | 用户可触发的坏地址可能占住 CPU，内核可修复异常也无法走 fixup。 | [mykernel/arch/x86_64/mm/fault.c:L90-L155](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/mm/fault.c#L90-L155) · high | 分别测试用户 SIGSEGV、内核 uaccess fixup 与真正内核崩溃策略。 |
| DR008-025 | `arch.x86_64` | `exception_handler` | 对不可恢复 fault，CPU 可能反复执行同一指令并形成异常循环，而不是终止任务或 panic。 | [mykernel/arch/x86_64/myos/interrupt.c:L120-L165](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/myos/interrupt.c#L120-L165) · high | 为 #UD/#DE/#NM 等异常建立明确策略和回归，禁止静默返回。 |
| DR008-026 | `sched.scheduler` | `finish_task_switch/schedule_tail` | on_cpu/锁/死亡任务生命周期不变量可能无法完成，新任务首切换路径尤为敏感。 | [mykernel/sched/scheduler/scheduler_core.c:L735-L895](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/sched/scheduler/scheduler_core.c#L735-L895) · high | 在上下文切换回归中断言 rq 锁、preempt_count、prev->on_cpu、死亡栈释放。 |
| DR008-031 | `boot.uefi` | `do_part` | 误执行或路径指向错误介质时会重建 GPT/格式化分区，造成数据丢失。 | [scripts/part_vdisk.sh:L1-L95](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/scripts/part_vdisk.sh#L1-L95) · high | 改为显式 --device、类型/容量校验、默认 dry-run、双重确认和测试镜像。 |
| DR008-032 | `boot.uefi` | `install_to_physdisk` | 传错设备或中途失败可删除真实文件系统内容，并留下挂载状态。 | [scripts/phys_nvme_install.sh:L1-L12](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/scripts/phys_nvme_install.sh#L1-L12) · high | 验证块设备 UUID/分区类型、默认拒绝非测试介质、使用临时挂载点和 trap。 |

## S2 · 当前快照

| ID | subsystem | symbol/位置 | 静态失败模式 | 证据 | 回归保护 |
|---|---|---|---|---|---|
| DR008-027 | `ipc.signal` | `__sigqueue_alloc` | 正常正值限制下队列上限不会被实际执行，可导致未受控的 signal queue 分配。 | [mykernel/lock_IPC/signal/signal.c:L15-L55](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/lock_IPC/signal/signal.c#L15-L55) · high | 按进程设置低 RLIMIT_SIGPENDING，连续排队实时信号并验证拒绝与计数回收。 |
| DR008-028 | `time.systick` | `loops_per_jiffy` | 不同 CPU/虚拟机频率下 busy-wait 延时可能大幅偏离请求值，影响设备初始化与超时。 | [mykernel/init/main.c:L55-L70](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/init/main.c#L55-L70) · high | 在 QEMU/VMware/物理机校准并测量 udelay/mdelay 误差，建立允许区间。 |
| DR008-029 | `arch.x86_64` | `myos_delay_full_u32` | 任一零参数调用会不可恢复挂起。 | [mykernel/arch/x86_64/lib/delay.c:L205-L220](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/arch/x86_64/lib/delay.c#L205-L220) · high | 明确零值契约并加单元测试；错误应返回或 BUG，而非静默自旋。 |
| DR008-030 | `init` | `asm-offset_copy_asm.py` | 编译器临时文件命名或目录变化会产生无诊断语义的 IndexError，中断内核构建。 | [mykernel/scripts/asm-offset_copy_asm.py:L15-L33](https://github.com/08822407d/MyOS2/blob/a039d9803ade2a1613d620bda375e028530d5242/mykernel/scripts/asm-offset_copy_asm.py#L15-L33) · high | 检查 exactly-one match，输出候选列表和明确退出码；加入缺失/多匹配测试。 |

## 历史正确性线索（现状未核）

[VERIFIED bugs_record.md] 下列条目来自历史故障记录，不作为 `time@a039d980` 当前 bug 的证明；只有在重新复现后才能转为 `open` 当前条目。

| ID | severity | subsystem | 历史线索 | 重新验证入口 |
|---|---|---|---|---|
| DR008-058 | S1 | `sched.forkexec` | fork() user stack contents frequently wrong | 建立 fork 前后寄存器/用户栈字节级对照回归，并关联当前缺陷 DR008-009。 |
| DR008-060 | S1 | `arch.x86_64` | disk read in myos_switch_to_root_disk cleared IF | 在每次根盘 I/O 前后记录 IF/preempt/irq nesting，并断言成对恢复。 |
| DR008-061 | S1 | `time.systick` | HPET stopped after init.bin; scheduler stopped switching | 启动 init 后持续采集 HPET/LAPIC IRQ、jiffies、need_resched 和上下文切换计数。 |
| DR008-059 | S2 | `cpu` | firmware ProcessorId differs from initial APIC ID and continuity is not guaranteed | 在稀疏 APIC ID 拓扑下验证 logical CPU↔APIC ID 映射。 |

## 建议给 MYOS2-DR-004 的调试桩

| 桩类别 | 首批覆盖 ID | 最小观测量 |
|---|---|---|
| 永久自旋/系统调用出口 | DR008-001, DR008-002, DR008-003 | syscall nr、task pid、CPU、irq/preempt 状态、返回码或 watchdog 超时 |
| 调度队列不变量 | DR008-010..014, DR008-026 | task state、on_rq、task_cpu、所属 rq、need_resched、IPI 计数 |
| 分区/GPT | DR008-004..008, DR008-018 | LBA、读取长度、GPT 条目数/大小、分配/释放计数、返回值 |
| 页错误/文件映射 | DR008-015, DR008-016, DR008-023, DR008-024 | fault code、VMA、vm_fault_t、页索引、I/O 长度、循环次数 |
| fork/exit | DR008-009, DR008-021, DR008-022 | node、tsk 指针、用户栈摘要、group_dead、mm/文件/栈引用计数 |
| IRQ | DR008-017, DR008-025 | vector、descriptor、ack 参数、EOI、重复触发次数 |
| 磁盘脚本护栏 | DR008-031, DR008-032 | 目标设备 canonical path、UUID、容量、dry-run 计划、确认 token |

## 建议给 MYOS2-DR-006 的回归用例

1. `syscall_unsupported_returns_enosys`：未知 syscall 和未支持 futex 必须有界返回。
2. `partition_probe_return_contract`：无表、有效 GPT、损坏主表、备用表、短读分别验证返回值与释放。
3. `sched_sleep_wake_cross_cpu`：同 CPU/跨 CPU 睡眠唤醒，检查队列、CPU 字段、IPI 和返回值。
4. `fork_node_matrix`：`NUMA_NO_NODE` 与具体节点均能分配有效任务结构。
5. `filemap_page_boundary_matrix`：端点覆盖 `PAGE_SIZE-1/PAGE_SIZE/PAGE_SIZE+1` 和非零偏移。
6. `fault_error_terminates`：无 VMA、权限、OOM、COW 等路径不得在不变条件上永久自旋。
7. `irq_ack_vector_identity`：多个非零向量验证 handler/ack 的参数一致性。
8. `exit_resource_baseline`：循环 fork/exit 后 mm、栈、文件、fs_context 分配计数回到基线。
9. `deployment_scripts_default_safe`：无参数必须拒绝，dry-run 不写盘，测试镜像外目标默认拒绝。

## 停止条件

只有同时满足“源码契约明确、对应测试在至少一种受支持平台通过、失败注入不再触发原模式、台账记录修复提交”时，条目才可从 `open` 关闭；仅删除注释或让编译通过不足以关闭。
