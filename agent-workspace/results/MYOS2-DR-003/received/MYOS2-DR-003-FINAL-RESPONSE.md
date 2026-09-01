## MYOS2-DR-003 已完成

已在 `08822407d/MyOS2` 创建并核验 Pull Request：

**[PR #1 · MYOS2-DR-003: dependency graph, init sequence, and trim analysis](https://github.com/08822407d/MyOS2/pull/1)**

PR 当前为开放、非 Draft、可合并状态；目标分支为 `master`，来源分支为 `agent/MYOS2-DR-003`。fileciteturn95file0L2-L16

### 分支与快照

- 内核源码分析基线：`time @ a039d9803ade2a1613d620bda375e028530d5242`
- 工作区及任务书基线：`master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719`
- 工作分支：`agent/MYOS2-DR-003`
- PR Head：`2b86e0017b25fd95212d4dace6513e5346a97bef`
- 共 5 个提交、5 个新增文件、1134 行新增、0 行删除。fileciteturn95file0L28-L35

### 已交付文件

全部位于唯一许可目录 `agent-workspace/results/MYOS2-DR-003/`：

| 文件 | 内容 |
|---|---|
| `MANIFEST.md` | 快照、覆盖状态、限制、开放问题及后续消费顺序 |
| `deps.yaml` | 机器可读三层依赖数据，共 102 个节点、123 条边 |
| `deps.dot` | 与 YAML 同源的 Graphviz 总图，区分 build、init_order、functional 三种边 |
| `init-sequence.md` | 从 `phys_startup_64` 到 boot idle、根盘切换和 `/boot/init` 的实际初始化时序 |
| `trim-analysis.md` | 当前不可裁剪核、条件依赖、替代组、可选叶和 menuconfig 粒度建议 |

分支比较确认只新增上述五个文件，没有删除、修改内核源码或写入其他任务目录。GitHub 上五个 blob SHA 也与生成端逐文件一致。

### 主要分析结论

当前构建使用递归 GLOB 将几乎全部 `.c`、`.S` 纳入单一 `kernel` 目标，再依靠 section 粒度和 `--gc-sections` 回收不可达代码。因此当前尚不存在真正的目录级或 feature 级裁剪边界，构建输入与运行时可达性必须分开处理。

启动硬链已还原为：

`x86_64 入口与早期页表`
→ `setup_arch / memblock`
→ `page allocator / kmalloc`
→ `per-CPU / traps / scheduler`
→ `IRQ / softirq / time`
→ `PID、设备、VFS、信号`
→ `kernel_init + kthreadd + boot idle`
→ `driver_init + 手工 initcalls`
→ `PCI / ATA / NVMe`
→ `GPT + FAT32 根`
→ `/boot/init`
→ `SYSCALL/SYSRET`

当前 profile 中，HPET 是尚无已核实 fallback 的 timer provider；FAT32 由于承担根文件系统而不是普通可选叶；ATA 与 NVMe 应建模为根块设备替代组。已识别的可选或条件可选叶包括 RTC、AArch64、当前未生效的 initramfs、极简用户 ABI 下的 futex、未选择的存储/USB 后端及部分调试设施。

### 校验与限制

`deps.yaml` 已通过 YAML 解析，123 条边均包含 `from`、`to`、`kind`、`hardness` 和 `evidence`；`deps.dot` 已通过 Graphviz `dot -Tsvg` 解析。所有 `init_order` 边均有源码证据。

本任务完成的是 `time` 快照上的静态源码分析，未执行干净构建、链接 map 检查、QEMU/实机启动或设备 I/O 实测。ATA/NVMe 的实际根后端绑定、无 HPET 平台的降级路径，以及手工 initcall 与 linker-section initcall 的长期关系，均已如实写入各交付物的 `open_questions`。
