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
supersedes: MANIFEST.md
open_questions:
- 'OQ-001: 顶层递归 glob 静态上会纳入 part_core.c/part_efi.c，但本会话无法执行完整构建，尚未用最终链接映射再次证明所有配置均包含它们。'
- 'OQ-002: 旧 mykernel/fs/vfs/myos_vfs.c 根盘 GPT 路径与新 mykernel/block/part_* 路径谁是当前唯一权威实现，所读快照未给明确所有权说明。'
- 'OQ-003: 调度器其他文件是否存在可补偿 enqueue/TTWU/set_task_cpu 缺失语义的自定义机制，已读调用链中未见，但未做运行时不变量追踪。'
- 'OQ-004: 仓库中的 myloader/BOOTX64.EFI 是否由 time@a039d980 的 uefi 源码生成，缺少可重复构建记录与哈希证明。'
- 'OQ-005: bugs_record.md 的四条历史故障在 time@a039d980 上是否仍可复现，未执行启动/硬件测试。'
- 'OQ-006: 9 个仍被活动源码依赖的 obsolete 头应按何种顺序迁移，需结合编译依赖图和回归覆盖决定。'
- 'OQ-007: 因连接器环境无法进行完整编译、链接和静态分析，仓库中可能仍有未被关键词/定向阅读捕获的非 void 跌落、未初始化变量或死循环。'
---
# MANIFEST · MYOS2-DR-008 · v2

[VERIFIED MANIFEST.md] 本文件是权威清单，取代首版 `MANIFEST.md`。首版在机器台账尚未最终压缩定稿时描述了未实际保留的扩展字段，并把存在一处热点图换行缺陷的 `01-summary.md` 列为权威摘要；本修订只纠正交付关系和字段说明，不改变分析统计与结论。

## 权威交付物

| 文件 | 状态 | 内容与约定 |
|---|---|---|
| `debt-register.yaml` | final | 69 条机器可读行式台账。每条均含任务书规定的 `id/quote/path/line/category/subsystem/severity/status/notes`；以 `id` 为稳定引用。 |
| `01-summary-v2.md` | final | 统计、子系统热点、问题簇、obsolete/.bak 盘点、历史材料与治理顺序；取代 `01-summary.md`。 |
| `02-correctness-suspects.md` | final | 32 条当前正确性嫌疑按 S1→S2 排序，另列 4 条历史正确性线索，并给出调试桩与回归建议。 |
| `MANIFEST-v2.md` | final | 本权威清单；取代 `MANIFEST.md`。 |

## 保留但不再权威的首版文件

| 文件 | 原因 |
|---|---|
| `01-summary.md` | 热点图中的 `boot.uefi` 与 `sched.forkexec` 两行在连接器写入时被合并；正文统计和结论未受影响。 |
| `MANIFEST.md` | 对最终压缩台账的字段说明不准确，且未指向摘要修订版。 |

## 覆盖状态

### 已完成

- [VERIFIED agent-workspace/conventions.md] 遵守从 `master` 建专用分支、仅在规定结果目录新增文件、提交前缀、事实标签与修订不覆盖规则。
- [VERIFIED agent-workspace/repo-map.md] 按 `time` 快照的实际 `mykernel/`、`myloader/`、`myinitramfs/` 与 `scripts/` 布局扫描；未把任务书中的旧路径提示伪装为当前文件。
- [VERIFIED debt-register.yaml] 登记 69 条：60 条当前快照、9 条历史未核；类别为正确性隐患 36、未完成功能 15、废弃物 10、文档欠账 3、风格 5。
- [VERIFIED debt-register.yaml] 严重度为 S1 31、S2 36、S3 2；状态为 open 60、可能已修 0、无法判定 9。
- [VERIFIED debt-register.yaml] 当前快照正确性隐患 32 条，其中 S1 28、S2 4。
- [VERIFIED debt-register.yaml] 已逐文件盘点 1 个 `.bak` 和 9 个 `obsolete` 头；9 个 obsolete 头仍处于活动依赖链，不能直接按死文件删除。
- [VERIFIED bugs_record.md] [VERIFIED todo.txt] 历史故障与计划只作为“历史材料，现状未核”，未据此断言当前状态。
- [VERIFIED 01-summary-v2.md] 已把上游/第三方注释噪声与 MyOS2 活动调用、注册、构建关系分开。
- [VERIFIED 02-correctness-suspects.md] 已为后续调试桩和回归用例提供稳定 debt ID、失败模式、证据区间和测试入口。

### 明确未做

- 未修改任何内核、引导器、用户态或部署脚本源码；本任务只登记，不修复。
- 未读取 `time` 以外的历史主题分支。
- 未执行完整编译、最终链接映射、QEMU/VMware/物理机启动、单元/集成测试或硬件故障注入。
- 未声称关键词和定向阅读可证明零遗漏；编译与运行时无法验证的部分均保留在 `open_questions`。
- 未把缺少活动证据的 Linux/glibc 上游 TODO/FIXME 计入项目债务。

## 质量与一致性规则

- `category` 只使用：`正确性隐患/未完成功能/废弃物/文档欠账/风格`。
- `severity` 只使用：`S1/S2/S3`。
- `status` 只使用：`open/可能已修/无法判定`。
- 当前源码事实使用 `[VERIFIED path]`；影响推断在 Markdown 中使用 `[INFERRED]`；历史记录明确写“历史材料，现状未核”。
- `01-summary-v2.md` 的统计必须与 `debt-register.yaml` 一致；`02-correctness-suspects.md` 的当前条目必须先 S1 后 S2，历史线索独立分区。
- 后续修复不得删除或改写现有 debt ID；应在新产物中关联修复提交、回归用例和关闭依据。

## 快照与写入边界

- 分析源码：`time@a039d9803ade2a1613d620bda375e028530d5242`。
- 工作区/建分支基点：`master@c7e68edb3dd76f4e4808dff12b5ca2021035e719`。
- 交付分支：`agent/MYOS2-DR-008`。
- 唯一写入目录：`agent-workspace/results/MYOS2-DR-008/`。

## 后续消费约定

1. **MYOS2-DR-004（调试桩）**：从 `02-correctness-suspects.md` 的 S1 开始，优先 DR008-001..003、004..008、010..017、023..026；输出必须带 debt ID。
2. **MYOS2-DR-006（回归用例）**：测试名称或元数据引用同一 ID；先证明原失败模式，再修复。
3. **构建治理**：消费 DR008-030、033、034、040..045，先显式化源集合和诊断，再推进大规模移植。
4. **接口/归档治理**：消费 DR008-048..057；活动 obsolete 头先迁移引用，再删除或归档。
5. **历史核验**：消费 DR008-058..066；复现成功后新增当前快照记录，不覆盖历史条目。

## 开放问题

- OQ-001: 顶层递归 glob 静态上会纳入 part_core.c/part_efi.c，但本会话无法执行完整构建，尚未用最终链接映射再次证明所有配置均包含它们。
- OQ-002: 旧 mykernel/fs/vfs/myos_vfs.c 根盘 GPT 路径与新 mykernel/block/part_* 路径谁是当前唯一权威实现，所读快照未给明确所有权说明。
- OQ-003: 调度器其他文件是否存在可补偿 enqueue/TTWU/set_task_cpu 缺失语义的自定义机制，已读调用链中未见，但未做运行时不变量追踪。
- OQ-004: 仓库中的 myloader/BOOTX64.EFI 是否由 time@a039d980 的 uefi 源码生成，缺少可重复构建记录与哈希证明。
- OQ-005: bugs_record.md 的四条历史故障在 time@a039d980 上是否仍可复现，未执行启动/硬件测试。
- OQ-006: 9 个仍被活动源码依赖的 obsolete 头应按何种顺序迁移，需结合编译依赖图和回归覆盖决定。
- OQ-007: 因连接器环境无法进行完整编译、链接和静态分析，仓库中可能仍有未被关键词/定向阅读捕获的非 void 跌落、未初始化变量或死循环。

## 完成判定

本任务状态为 `final`，完成级别为 `best_effort_static_scan`。任务书规定的四类交付均已给出；因遵守“不覆盖既有结论”的公约，摘要与 MANIFEST 的格式/字段修订以新增 `-v2` 文件呈现，权威入口为本文件。
