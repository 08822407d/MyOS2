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
# MANIFEST · MYOS2-DR-008

## 文件清单

| 文件 | 状态 | 内容 | 后续消费方式 |
|---|---|---|---|
| `debt-register.yaml` | final | 69 条机器可读台账；含证据、永久链接、分类、子系统、严重度、状态、来源类型与置信度 | 以 `id` 为稳定引用；过滤 `source_kind=current` 获得当前快照条目 |
| `01-summary.md` | final | 统计、热点图、问题簇、obsolete/.bak 盘点、历史材料与治理顺序 | 作为维护者阅读入口；统计必须与 YAML 一致 |
| `02-correctness-suspects.md` | final | 按 S1→S2 排序的当前正确性嫌疑、历史故障线索、调试桩与回归建议 | MYOS2-DR-004/006 优先按 ID 选题 |
| `MANIFEST.md` | final | 覆盖状态、未做事项、开放问题与消费约定 | 判断本任务边界和可信度 |

## 覆盖状态

### 已完成

- [VERIFIED agent-workspace/conventions.md] 已读取并遵守写入公约、共享 subsystem 词汇和事实标注纪律。
- [VERIFIED agent-workspace/repo-map.md] 已按实际 `mykernel/` 布局而非旧 `src/` 提示扫描。
- [VERIFIED agent-workspace/tasks/MYOS2-DR-008-tech-debt-register.md] 已交付任务书规定的四个文件。
- [VERIFIED debt-register.yaml] 已扫描 TODO/FIXME/XXX/未实现、永久自旋、非 `void` 跌落、大段注释代码、魔数/宿主路径、构建边界和破坏性脚本。
- [VERIFIED debt-register.yaml] 已合并 `todo.txt`、`bugs_record.md`、`changelog.md`，历史条目统一标记“历史材料，现状未核/无法判定”。
- [VERIFIED debt-register.yaml] 已逐文件盘点 1 个 `.bak` 与 9 个 `obsolete` 头，并区分未见活动引用与仍被生产源码依赖。
- [VERIFIED debt-register.yaml] 已把第三方/上游注释噪声排除在正式计数之外，除非存在 MyOS2 活动调用、注册或构建关系。
- [VERIFIED 01-summary.md] 统计由 YAML 条目生成并经机器校验：69 总条目、60 当前快照、9 历史未核。

### 明确未做

- 未修改任何内核、引导器、用户态或脚本源码；本任务只登记不修复。
- 未读取 `time` 之外的历史主题分支。
- 未执行完整编译、最终链接映射、QEMU/VMware/物理机启动、单元/集成测试或硬件故障注入。
- 未声称关键词扫描证明“零遗漏”；连接器环境不能提供本地仓库归档，机器辅助仅用于条目/统计一致性，不用于编译诊断。
- 未把无活动证据的 Linux/glibc 上游 TODO/FIXME 计入项目债务。

## 质量与一致性检查

- `debt-register.yaml` 可由 YAML 解析器加载。
- 每条均含任务书要求的 `id/quote/path/line/category/subsystem/severity/status/notes`，并额外含 `permalink/source_kind/confidence`。
- `category` 仅使用：`正确性隐患/未完成功能/废弃物/文档欠账/风格`。
- `severity` 仅使用：`S1/S2/S3`；`status` 仅使用：`open/可能已修/无法判定`。
- 当前源码事实、推断影响和历史材料分别使用 `[VERIFIED]`、`[INFERRED]` 与“历史材料，现状未核”标记。
- `02-correctness-suspects.md` 当前条目按 S1 后 S2 排序；历史正确性线索独立分区。

## 交付快照

- 分析源码：`time@a039d9803ade2a1613d620bda375e028530d5242`。
- 工作区基点：`master@c7e68edb3dd76f4e4808dff12b5ca2021035e719`（创建分支时已再次核对 Git ref）。
- 目标分支：`agent/MYOS2-DR-008`。
- 唯一写入目录：`agent-workspace/results/MYOS2-DR-008/`。

## 后续 agent 消费约定

1. **MYOS2-DR-004（调试桩）**：从 `02-correctness-suspects.md` 的 S1 开始，优先选择 DR008-001..003、004..008、010..017、023..026；桩输出必须带 debt ID。
2. **MYOS2-DR-006（回归用例）**：按同一 debt ID 命名测试或在测试元数据中引用；先证明原模式会失败，再修复。
3. **构建治理任务**：消费 DR008-030、033、034、040..045；先让诊断和源集合可重复，再处理大规模移植。
4. **接口/归档治理任务**：消费 DR008-048..057；活动 `obsolete` 头不可直接删除，应先迁移所有引用。
5. **历史核验任务**：消费 DR008-058..066；复现成功后新增当前快照条目，不覆盖历史记录。

## 开放问题

- OQ-001: 顶层递归 glob 静态上会纳入 part_core.c/part_efi.c，但本会话无法执行完整构建，尚未用最终链接映射再次证明所有配置均包含它们。
- OQ-002: 旧 mykernel/fs/vfs/myos_vfs.c 根盘 GPT 路径与新 mykernel/block/part_* 路径谁是当前唯一权威实现，所读快照未给明确所有权说明。
- OQ-003: 调度器其他文件是否存在可补偿 enqueue/TTWU/set_task_cpu 缺失语义的自定义机制，已读调用链中未见，但未做运行时不变量追踪。
- OQ-004: 仓库中的 myloader/BOOTX64.EFI 是否由 time@a039d980 的 uefi 源码生成，缺少可重复构建记录与哈希证明。
- OQ-005: bugs_record.md 的四条历史故障在 time@a039d980 上是否仍可复现，未执行启动/硬件测试。
- OQ-006: 9 个仍被活动源码依赖的 obsolete 头应按何种顺序迁移，需结合编译依赖图和回归覆盖决定。
- OQ-007: 因连接器环境无法进行完整编译、链接和静态分析，仓库中可能仍有未被关键词/定向阅读捕获的非 void 跌落、未初始化变量或死循环。

## 完成判定

本任务状态为 `final`，完成级别为 `best_effort_static_scan`：任务书规定交付物、统计一致性、历史材料分区和废弃物盘点均已完成；运行时复现与全编译验证明确留给后续任务。
