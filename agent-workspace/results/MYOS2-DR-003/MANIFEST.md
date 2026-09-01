---
task_id: MYOS2-DR-003
produced_by: ChatGPT Pro / GPT-5.6 Pro
date: '2026-09-01'
generated_at: '2026-09-01T00:56:24+08:00'
agent_id: openai-gpt-5.6-pro
base_snapshot: workspace master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719; kernel source time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
- agent-workspace/conventions.md
- agent-workspace/repo-map.md
- agent-workspace/tasks/MYOS2-DR-003-dependency-graph.md
- mykernel/CMakeLists.txt
- mykernel/scripts/options_flags.cmake
- mykernel/scripts/target_kbuild.cmake
- mykernel/scripts/target_kernel.cmake
- mykernel/arch/x86_64/kernel.lds
- mykernel/arch/x86_64/kernel/head_64.S
- mykernel/arch/x86_64/kernel/head64.c
- mykernel/arch/x86_64/myos/pre_init.c
- mykernel/arch/x86_64/kernel/setup.c
- mykernel/arch/x86_64/smp/setup_percpu.c
- mykernel/arch/x86_64/kernel/traps.c
- mykernel/mm/misc/mm_init.c
- mykernel/sched/scheduler/scheduler_core.c
- mykernel/arch/x86_64/kernel/irqinit.c
- mykernel/arch/x86_64/kernel/time.c
- mykernel/arch/x86_64/kernel/hpet.c
- mykernel/time/timekeeping/timekeeping.c
- mykernel/init/main.c
- mykernel/fs/vfs/init.c
- mykernel/fs/vfs/myos_vfs.c
- mykernel/arch/x86_64/entry/entry_64.S
- mykernel/arch/x86_64/entry/common.c
- mykernel/entry/common.c
- mykernel/arch/x86_64/myos/arch_task.c
content_type: manifest_markdown
source_scope: MYOS2-DR-003 五份交付物的索引、覆盖状态与消费说明
status: final
analysis_coverage: static_source_only
limitations:
- 仅对 time 快照做静态源码分析，未在连接器环境中执行干净构建、链接、启动或设备 I/O 实测。
- 递归 GLOB 会把大量源码加入 kernel 目标；本文区分“进入构建输入”与“运行时实际可达”，但未生成链接 map 验证 --gc-sections 的最终保留集合。
- 动态注册、函数指针、宏展开与条件编译的闭包仅覆盖启动主链和关键功能链，不能视为逐符号完备图。
- 部分 Linux 通用框架代码被注释或保留但未进入当前调用路径；未把“源码存在”自动视为“当前生效”。
open_questions:
- 当前正式发布/测试使用的 CMake 构建类型、ARCH 参数与根块设备选择宏是什么？
- ROOTBLK_TRANSFER 在目标机器上最终绑定 ATA 还是 NVMe，是否存在稳定的替代根文件系统配置？
- kernel_init 中手工 do_initcalls() 与链接器 initcall section 机制未来是否计划并存，还是前者为临时过渡？
- workqueue_init()、sched_init_smp() 等被注释路径是有意禁用、尚未移植完成，还是由其他私有路径替代？
- 目标平台是否保证 HPET；若不保证，PIT/LAPIC/TSC-deadline 的降级策略在哪里完成？
---

# MYOS2-DR-003 交付清单

## 交付状态

本目录完成了任务书要求的五份文件。分析基于 `time @ a039d9803ade2a1613d620bda375e028530d5242`，工作区与任务书来自 `master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719`。产出为**最终静态分析稿**；“final”表示文件内容已完成并互相一致，不表示已进行真实构建、链接、启动或设备测试。

| 文件 | 内容 | 覆盖状态 | 后续消费方式 |
|---|---|---|---|
| `MANIFEST.md` | 本清单、快照、方法、已完成/未完成事项 | final | 先读；确认基线与限制 |
| `deps.yaml` | 102 个节点、123 条 build/init_order/functional 边；含 hardness、证据与置信度 | final / static | 机器合并、过滤 `kind`、生成配置依赖 |
| `deps.dot` | 与 YAML 同源的 Graphviz 总图；三类边分别为虚线/实线/点线 | final / static | 用 Graphviz 渲染；边方向均为依赖方指向被依赖方 |
| `init-sequence.md` | 从 `phys_startup_64` 到 boot idle 与 `/boot/init` 的实际时序、注册点、被注释路径 | final / static | 教学阅读、启动故障定位、初始化重构 |
| `trim-analysis.md` | 当前硬核、条件必需、可选叶、menuconfig 粒度、替代组与验证门 | final / preliminary | 配置系统设计输入；不能直接当删除清单 |

## 核心结论

1. [VERIFIED `mykernel/CMakeLists.txt:26-35`] 当前构建递归收集几乎全部 `.c`/`.S` 到单一 `kernel` 目标；[VERIFIED `mykernel/scripts/options_flags.cmake:31-82`] 最终镜像还依赖 section 粒度与 `--gc-sections`。因此“进入编译输入”和“运行时保留/可达”是两种不同依赖。
2. [VERIFIED `mykernel/init/main.c:112-177`] `start_kernel()` 的硬骨架为早期内存/per-CPU/异常 → 通用内存 → 调度 → IRQ/softirq/time → PID/设备/VFS/信号 → `rest_init()`。
3. [VERIFIED `mykernel/init/main.c:76-123`] `rest_init()` 先创建 PID 1，再创建 kthreadd；PID 1 等待 completion，boot task 完成首次调度后进入 idle。
4. [VERIFIED `mykernel/init/main.c:193-293`] PID 1 使用固定手工 initcall 列表，随后无条件建立 NVMe/ATA/XHCI 请求线程、扫描 PCI、初始化存储、切换根盘并执行用户态。
5. [VERIFIED `mykernel/fs/vfs/myos_vfs.c:55-154`] 当前根路径是 GPT + `BOOT_FS_IDX == 0` + FAT32，并挂载 devtmpfs；[VERIFIED `mykernel/arch/x86_64/myos/arch_task.c:7-45`] 首进程固定为 `/boot/init`。
6. [VERIFIED `mykernel/arch/x86_64/kernel/time.c:29-59`] 当前 timer 路径假定 HPET；在没有替代 provider 前，HPET 对当前 profile 不是可选叶。
7. 可选叶至少包括 `drivers.rtc`、`arch.aarch64`、当前未生效的 `user.initramfs`，以及在极简用户 ABI 下可能可选的 `lock.futex`；`fs.fat` 只有在提供另一根文件系统后才可降为可选。

## 事实与推断纪律

- 所有 `init_order` 边都有 `[VERIFIED path:line]` 证据。
- `functional` 边若仅由语义推出，明确标为 `[INFERRED]` 并降低 confidence。
- “未见”只限定于所读 `time` 快照与本任务范围，不断言其他历史分支或未读文件中不存在替代实现。
- 被注释的 Linux 通用框架不计入当前生效主链；源码存在不等于运行时生效。

## 机器可读约定

`deps.yaml` 的边方向统一如下：

- `build`：构建产物/源集合 → 构建所依赖的目标、配置或子系统。
- `init_order`：后执行/后可用节点 → 必须先执行/先可用节点。
- `functional`：能力 → 提供该能力所依赖的子系统或能力。
- `hard`：当前快照/当前配置下缺失会阻断起点。
- `soft`：可降级、仅某配置需要，或仅确认了实际顺序而未证明不可交换。

所有节点 ID 均使用 `agent-workspace/conventions.md` §3 的共享词汇表前缀。

## 生成时一致性检查

- `deps.yaml` 已在生成端通过 YAML 解析，节点 ID 唯一，所有边端点均能解析到节点。
- `deps.dot` 已在生成端通过 Graphviz `dot -Tsvg` 语法检查。
- `deps.yaml` 与 `deps.dot` 从同一节点/边集合生成：102 个节点、123 条边。
- Markdown 文件均带 YAML 头，包含任务号、生产者、日期、基线、输入、状态、限制与开放问题。
- 未修改内核源码、任务书、公约或其他任务结果目录。

## 明确未完成

- 未执行 CMake 构建，未产生/检查 linker map。
- 未在 QEMU、虚拟机或实机启动。
- 未验证 ATA、NVMe、XHCI、PCI、HPET 的实际硬件行为。
- 未闭包所有动态注册、宏展开、函数指针、系统调用表与设备回调。
- 未计算逐符号链接 SCC，也未实现任何 menuconfig/Kconfig 机制。
- 未确认 `/boot/init` 的实际用户态依赖，因此 futex、信号、TTY、devtmpfs 等只能做分级判断。

## 建议消费顺序

1. 先读 `init-sequence.md`，建立“实际执行”基线。
2. 再按 `deps.yaml` 的 `kind=init_order` 验证初始化重构；这些边证据最硬。
3. 用 `kind=functional` 规划学习顺序和 feature 约束，优先处理 `hard + confidence=high`。
4. 用 `trim-analysis.md` 设计 profile/choice，而不是直接移除目录。
5. 首次实现配置机制后，回填真实 build/link/boot 结果，新增版本文件而不覆盖本稿。
