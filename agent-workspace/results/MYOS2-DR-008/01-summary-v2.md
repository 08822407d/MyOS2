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
supersedes: 01-summary.md
open_questions:
- 'OQ-001: 顶层递归 glob 静态上会纳入 part_core.c/part_efi.c，但本会话无法执行完整构建，尚未用最终链接映射再次证明所有配置均包含它们。'
- 'OQ-002: 旧 mykernel/fs/vfs/myos_vfs.c 根盘 GPT 路径与新 mykernel/block/part_* 路径谁是当前唯一权威实现，所读快照未给明确所有权说明。'
- 'OQ-003: 调度器其他文件是否存在可补偿 enqueue/TTWU/set_task_cpu 缺失语义的自定义机制，已读调用链中未见，但未做运行时不变量追踪。'
- 'OQ-004: 仓库中的 myloader/BOOTX64.EFI 是否由 time@a039d980 的 uefi 源码生成，缺少可重复构建记录与哈希证明。'
- 'OQ-005: bugs_record.md 的四条历史故障在 time@a039d980 上是否仍可复现，未执行启动/硬件测试。'
- 'OQ-006: 9 个仍被活动源码依赖的 obsolete 头应按何种顺序迁移，需结合编译依赖图和回归覆盖决定。'
- 'OQ-007: 因连接器环境无法进行完整编译、链接和静态分析，仓库中可能仍有未被关键词/定向阅读捕获的非 void 跌落、未初始化变量或死循环。'
---
# MYOS2-DR-008 技术债与已知问题摘要

[VERIFIED 01-summary.md] 本修订只修复热点图中 `boot.uefi` 与 `sched.forkexec` 两行在写入时被合并的格式缺陷；统计、证据与结论不变。

## 结论

[VERIFIED debt-register.yaml] 本次登记 **69** 条：其中 **60** 条来自 `time@a039d980` 的当前快照证据，**9** 条仅来自历史材料并统一标记为“无法判定”。当前快照条目中，正确性隐患 32 条、未完成功能 10 条、废弃物 10 条、文档欠账 3 条、风格 5 条。

[VERIFIED mykernel/sched/scheduler/scheduler_core.c] [VERIFIED mykernel/block/part_core.c] [VERIFIED mykernel/block/part_efi.c] 债务并非平均分布：调度、分区/GPT、x86_64 异常与中断、构建/部署边界最集中。

[INFERRED] 最先需要保护的不是大规模重构，而是能把永久自旋、非 `void` 跌落、未初始化指针、队列不变量和破坏性脚本变成可重复失败的回归用例。

## 扫描口径与完成度

- [VERIFIED agent-workspace/conventions.md] 分析基线固定为 `time@a039d9803ade2a1613d620bda375e028530d5242`；工作区文件读取自 `master`。
- [VERIFIED debt-register.yaml] 扫描覆盖标记词、永久自旋、非 `void` 返回、临时注释代码、构建 glob、硬编码宿主路径、历史清单、`.bak` 与 `obsolete/`。
- [VERIFIED debt-register.yaml] 上游/第三方源码原有 TODO/FIXME 在没有 MyOS2 活动调用或构建关系时不计入条目。
- [VERIFIED debt-register.yaml] 同一迁移根因只有在形成独立运行时失败模式或独立治理对象时才拆条。
- [INFERRED] 连接器环境不能执行完整编译、链接映射、启动或硬件测试，因此结论是“静态证据台账”，不是运行时复现报告；遗漏风险已写入 `open_questions`。

## 统计

### 全部条目

| 维度 | 值 | 数量 |
|---|---:|---:|
| category | 正确性隐患 | 36 |
| category | 未完成功能 | 15 |
| category | 废弃物 | 10 |
| category | 文档欠账 | 3 |
| category | 风格 | 5 |
| severity | S1 | 31 |
| severity | S2 | 36 |
| severity | S3 | 2 |
| status | open | 60 |
| status | 可能已修 | 0 |
| status | 无法判定 | 9 |

### 当前快照严重度

| 严重度 | 数量 | 解释 |
|---|---:|---|
| S1 | 28 | 可导致运行错误、未定义行为、永久自旋或破坏性写入的静态嫌疑 |
| S2 | 30 | 阻碍开发、迁移、构建复现或资源治理 |
| S3 | 2 | 主要为观感/归档噪声 |

> S1 是任务书规定的台账分级，不等于已在真实硬件上复现。每条是否复现见 `02-correctness-suspects.md` 的证据等级。

## 子系统热点图（当前快照）

```text
init                  8  ████████
sched.scheduler       7  ███████
block                 6  ██████
arch.x86_64           5  █████
boot.uefi             5  █████
sched.forkexec        3  ███
mm.vm_map             3  ███
fs.vfs                3  ███
lock.futex            2  ██
mm.fault              2  ██
time.adjtime          2  ██
debug                 2  ██
device                2  ██
entry                 1  █
mm.page_alloc         1  █
```

| 排名 | subsystem | 条目数 | 主要债务形态 |
|---:|---|---:|---|
| 1 | `init` | 8 | 递归 glob、诊断/配置、构建与文档治理 |
| 2 | `sched.scheduler` | 7 | enqueue/dequeue、TTWU、CPU 迁移、远端重调度、switch 收尾 |
| 3 | `block` | 6 | 分区探测/GPT 半移植、缺失返回和未初始化指针 |
| 4 | `arch.x86_64` | 5 | 系统调用、异常处理、IRQ ack、延时与 obsolete 接口 |
| 5 | `boot.uefi` | 5 | 预编译启动器、宿主路径和破坏性磁盘脚本 |
| 6 | `sched.forkexec` | 3 | fork 控制流、退出生命周期 |
| 7 | `mm.vm_map` | 3 | page_mkwrite、页对齐 I/O、写回语义 |
| 8 | `fs.vfs` | 3 | GPT 旧路径边界、fs_context 生命周期 |
| 9 | `lock.futex` | 2 | 活动 syscall 永久自旋 |
| 10 | `mm.fault` | 2 | 坏地址/错误返回永久自旋 |
| 11 | `time.adjtime` | 2 | API 声明与实现断裂 |
| 12 | `debug` | 2 | 历史 bug 与备份调试脚本 |
| 13 | `device` | 2 | 活动代码依赖 obsolete 设备接口 |
| 14 | `entry` | 1 | 未实现系统调用处理 |
| 15 | `mm.page_alloc` | 1 | folio 分配忽略 order/GFP |

## 最高优先级问题簇

### 1. 可由活动入口触达的永久自旋

- [VERIFIED mykernel/lock_IPC/futex/futex.c] `do_futex()` 入口无条件 `while (1);`。
- [VERIFIED mykernel/lock_IPC/syscall.c] 带 timeout 的 futex 在解析前永久自旋。
- [VERIFIED mykernel/arch/x86_64/entry/common.c] 未实现系统调用处理器永久自旋。
- [VERIFIED mykernel/arch/x86_64/mm/fault.c] 用户页错误返回非 `ENOERR` 后不重新求值而永久自旋；坏地址和内核地址错误也直接自旋。

### 2. 分区/GPT 路径的未定义行为链

- [VERIFIED mykernel/block/part_core.c] `check_partition()` 与 `blk_add_partitions()` 均存在非 `void` 跌落。
- [VERIFIED mykernel/block/part_efi.c] `last_lba()`、`read_lba()`、`find_valid_gpt()`、`efi_partition()` 的关键返回被注释；`legacymbr` 未初始化即传入读盘函数。
- [VERIFIED mykernel/fs/vfs/myos_vfs.c] 旧根盘 GPT 路径固定读取 5 页，却按磁盘声明条目数复制和遍历，缺少容量/上界验证。

### 3. 调度唤醒与队列不变量

- [VERIFIED mykernel/sched/scheduler/scheduler_core.c] `enqueue_task()` 为空，`dequeue_task()` 无返回；`try_to_wake_up()` 不入队且恒返回 0。
- [VERIFIED mykernel/sched/scheduler/scheduler_core.c] `set_task_cpu()` 不更新任务 CPU 字段，`wake_up_new_task()` 固定 CPU0，远端 IPI 路径被注释。
- [VERIFIED mykernel/lock_IPC/signal/signal.c] [VERIFIED mykernel/sched/forkexec/kthread.c] 信号唤醒与 kthreadd 唤醒均依赖上述接口，说明影响面跨模块。

### 4. 文件映射、进程和中断

- [VERIFIED mykernel/mm/vm_map/filemap.c] `filemap_page_mkwrite()` 无返回却注册为回调；页对齐结尾会把最后一页长度算成 0。
- [VERIFIED mykernel/sched/forkexec/fork.c] `dup_task_struct()` 因注释改变 if 归属，在指定 NUMA 节点时使用未初始化 `tsk`。
- [VERIFIED mykernel/arch/x86_64/myos/interrupt.c] IRQ handler 始终向控制器 `ack(0)`。
- [VERIFIED mykernel/sched/forkexec/exit.c] `group_dead` 未初始化，且大部分退出生命周期清理尚未接通。

## `obsolete/` 与 `.bak` 盘点

[VERIFIED debt-register.yaml] 共登记 10 个显式历史资产；除 `dbg-qemu.sh.bak` 外，9 个 `obsolete` 头在所读快照中都仍处于活动依赖链，不能按“死文件”直接删除。

| ID | 路径 | 当前引用判断 | severity |
|---|---|---|---|
| DR008-048 | `dbg-qemu.sh.bak` | 所读快照中未见活动引用 | S3 |
| DR008-049 | `mykernel/include/obsolete/glo.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-050 | `mykernel/include/obsolete/ktypes.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-051 | `mykernel/include/obsolete/printk.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-052 | `mykernel/include/obsolete/proto.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-053 | `mykernel/arch/x86_64/include/obsolete/arch_proto.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-054 | `mykernel/arch/x86_64/include/obsolete/device.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-055 | `mykernel/arch/x86_64/include/obsolete/ide.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-056 | `mykernel/arch/x86_64/include/obsolete/keyboard.h` | 仍被活动源码直接或传递包含 | S2 |
| DR008-057 | `mykernel/arch/x86_64/include/obsolete/myos_irq_vectors.h` | 仍被活动源码直接或传递包含 | S2 |

## 历史材料

[VERIFIED debt-register.yaml] 历史材料共 9 条，全部为 `status: 无法判定`；它们不计作当前快照已验证 bug。

| ID | 出处 | 主题 | severity |
|---|---|---|---|
| DR008-058 | `bugs_record.md:L1-L2` | fork() user stack contents frequently wrong | S1 |
| DR008-059 | `bugs_record.md:L4-L5` | firmware ProcessorId differs from initial APIC ID and continuity is not guaranteed | S2 |
| DR008-060 | `bugs_record.md:L7-L8` | disk read in myos_switch_to_root_disk cleared IF | S1 |
| DR008-061 | `bugs_record.md:L10-L12` | HPET stopped after init.bin; scheduler stopped switching | S1 |
| DR008-062 | `todo.txt:L1-L6` | upgrade ELF argv/env setup to AUX_VEC; port complete uaccess API | S2 |
| DR008-063 | `todo.txt:L7-L12` | simplify mmap and add file-mmap/page cache; add libata partition scan framework | S2 |
| DR008-064 | `todo.txt:L13-L17` | re-enable AP-core/idle and implement runtime percpu allocation | S2 |
| DR008-065 | `todo.txt:L18-L27` | split kernel stack/task_struct; upgrade memblock and exception entry | S2 |
| DR008-066 | `todo.txt:L28-L31` | implement FAT32 create/mkdir/rmdir and VFS file flags | S2 |

## 建议的治理顺序

1. **先保护行为**：MYOS2-DR-004/006 优先消费 `02-correctness-suspects.md` 的 S1 当前快照条目，先写能失败的最小测试与诊断桩。
2. **再封闭构建边界**：把递归 glob 改成显式源清单，启用 `-Wreturn-type`、未初始化变量和误导缩进诊断，防止半移植文件静默进入内核。
3. **统一所有权**：分区选择旧 VFS 路径或新 block 路径；调度选择单一队列模型；时间 API 明确归属。
4. **最后清理历史资产**：逐个迁移活动 `obsolete` 头；无法证明与源码对应的预编译 EFI 和 `.bak` 文件转入可追溯归档或删除。
5. **结构化关闭**：每个债务 ID 绑定修复提交、回归用例和关闭状态；历史清单只保留索引，不再作为现状真相源。

## 可消费接口

- `debt-register.yaml`：机器可读全集；按 `id`、`category`、`subsystem`、`severity`、`status` 查询。
- `02-correctness-suspects.md`：供 MYOS2-DR-004（调试桩）和 MYOS2-DR-006（回归用例）直接选题。
- `MANIFEST-v2.md`：覆盖范围、修订关系、明确未做事项及后续使用说明。
