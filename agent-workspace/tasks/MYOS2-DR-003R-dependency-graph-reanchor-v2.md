# MYOS2-DR-003R · 依赖关系图与初始化序列重锚（003 复跑；任务书 v2）

```yaml
task_id: MYOS2-DR-003R
supersedes: agent-workspace/tasks/MYOS2-DR-003R-dependency-graph-reanchor.md（v1 任务书，保留不改）；产出 supersedes MYOS2-DR-003 的锚点与 rtc 判定（results/MYOS2-DR-003/ 保留不改）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支源码）
priority: P0（002/007 的交叉综合、menuconfig 式裁剪、004R 的启动检查点都以它的锚点为地基）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-003R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；与本任务书冲突时以协议为准）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.4、§3.5、§4.1、§4.2、§4.7；再按 §2 输入清单读 003 的指定文件
drafted_by: MYOS2-LEAD-001（v2：2026-09-05）
status: draft（发射由 Owner）
```

## 1. 为什么复跑

003 的**内容结构是好的**（102 节点/123 边、三层依赖、19 步初始化时序），但证据锚点不能用：65 个 `路径:行号` 引用抽 30 处实读，多数落在空行、注释行或无关行（例 `init/main.c:276` 是一行 `*/`）；`init-sequence.md` 引 `mm_init.c:430-650`，该文件只有 430 行；`setup_arch()` 实际在 `start_kernel` 的另一位置。`trim-analysis.md` 把 `drivers.rtc` 判为可裁剪叶，但 `timekeeping_init → read_persistent_clock64 → rtc` 在启动主链上。全文未读 `mykernel/scripts/options_flags.cmake`——所有"当前生效/可裁剪"判断缺配置面依据（根块设备实为 ATA）。

## 2. 输入清单（文件/节级；协议 P11：只取结构与函数名，不转抄行号）

| 输入 | 只读哪部分 | 用途 |
|---|---|---|
| `results/MYOS2-DR-003/deps.yaml`（21,956 B） | 全文（勘误对象） | 节点/边集合的起点 |
| `results/MYOS2-DR-003/init-sequence.md`（15,028 B） | "阶段 3"表格的函数名列（19 项）与各阶段标题；**不转抄任何 `:行号`** | 时序骨架 |
| `results/MYOS2-DR-003/trim-analysis.md`（17,388 B） | 硬核/条件必需/可选叶三张表 | 裁剪判定的勘误对象 |
| `results/MYOS2-DR-003/MANIFEST-v2.md` | 核心结论 1~7 | errata 的逐条对象 |
| `WAVE-1-REVIEW.md` | §3.4（行号失效）、§3.5（rtc 误判）、§4.1（12 个生效宏）、§4.2（根块设备 ATA）、§4.7（SMP 未上线） | 必须吸收的已证实事实 |
| `mykernel/scripts/options_flags.cmake` | 全文 | 协议 P4 前置；config 边来源 |

源码起点（按函数跳读）：`mykernel/init/main.c::start_kernel`、`::rest_init`、`::kernel_init`；`mykernel/arch/x86_64/kernel/head_64.S`、`head64.c`（`phys_startup_64` 起点）；`mykernel/time/timekeeping/timekeeping.c::timekeeping_init`；`mykernel/drivers/rtc/`（`read_persistent_clock64` 的实现）；`mykernel/arch/x86_64/include/obsolete/device.h`（`ROOTBLK_TRANSFER` 两个分支）；`mykernel/CMakeLists.txt`、`mykernel/scripts/target_kernel.cmake`（构建依赖）；`mykernel/fs/vfs/myos_vfs.c::myos_switch_to_root_disk`；`mykernel/arch/x86_64/kernel/myos_APboot.S`（AP 路径未闭环的证据）。

## 3. 强制 [VERIFIED] 锚点（不得降级、不得省略）

`mykernel/init/main.c::start_kernel` 中含 `setup_arch(NULL);`、`timekeeping_init();`、`rest_init();` 的三行（三条各一锚点）；`mykernel/time/timekeeping/timekeeping.c::timekeeping_init` 中含 `read_persistent_clock64(&wall_time);` 的行；`mykernel/arch/x86_64/include/obsolete/device.h` 的 `#ifdef ROOTBLK_NVME` 与 `#else` 分支各自的 `ROOTBLK_TRANSFER` 定义行；`mykernel/scripts/options_flags.cmake` 中被注释的 `-DROOTBLK_NVME` 行与含 `-DCONFIG_SLUB` 的行；`mykernel/CMakeLists.txt` 中含 `GLOB_RECURSE` 的行；`mykernel/scripts/target_kernel.cmake` 中任一 `add_subdirectory(` 行；`mykernel/arch/x86_64/kernel/myos_APboot.S` 中 `jmp .` 所在行。

## 4. 目标

1. **deps-v2.yaml / deps-v2.dot**：节点与边以 v1 集合为起点（102/123）；每条 `init_order` 边的证据改为 `[VERIFIED path::function]`＋引文；证据核不出的边降为 `confidence: low` 并在 errata 登记（不得静默保留）；新增 `kind: config` 边（≤20 条）：来自 `options_flags.cmake` 的宏对子系统的选择（至少 `ROOTBLK_*` → `drivers.ata`/`drivers.nvme`，`CONFIG_SLUB`，`CONFIG_HYPERVISOR_GUEST`/`CONFIG_KVM_GUEST`，`GRUB2_BOOTUP_SUPPORT`）。词汇表 ID 不改；新造节点只增。
2. **init-sequence-v2.md**：从 `phys_startup_64` 到 `/boot/init` 的每一步写 `函数名＋引文`，**不写行号**；被注释掉/未生效的路径单列一节（如 `sched_init_smp`、`workqueue_init`、AP bring-up），每条附引文证明其被注释或原地自旋。
3. **trim-analysis-v2.md**：撤回 `drivers.rtc` 可裁剪判断（附 `timekeeping_init` 调用链引文）；每个"可选叶"判断附配置面依据（哪条宏、哪段 `#if`）；每个叶子标"进入编译输入"还是"运行时可达"。
4. **errata.md**：对 v1 核心结论 1~7、每条被改动的边、被撤回的判定逐条写 保留（已重核）/ 保留（未重核，原因）/ 修正 / 撤回＋证据。

## 5. 交付物与 MANIFEST

`MANIFEST.md`（必交；`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`、`supersedes` 列表、覆盖表：多少条边重锚成功/降级）＋`errata.md`＋`deps-v2.yaml`＋`deps-v2.dot`＋`init-sequence-v2.md`＋`trim-analysis-v2.md`。降级交付按协议 P12（每条回复一个文件，顺序 MANIFEST → errata → deps-v2.yaml → init-sequence-v2 → trim-analysis-v2 → deps-v2.dot）。读不完的优先顺序：init_order 边 → functional 边 → build 边 → config 边。

## 6. 边界

不修改内核源码；不改 results/MYOS2-DR-003/；不写 40 位 SHA；不基于行号做任何断言；不转抄 v1 的行号与溯源字段；不读 bugs_record.md/todo.txt 当现状；不做 menuconfig 实现；不读 `received/` 原件。

## 7. 验收判据（本地闸门按协议 P9 执行；任一项不过整份退回）

1. MANIFEST 五字段齐全；`startup_selfcheck_quote` 命中；金丝雀两行 time 命中、master 不命中（P9-5、7、8）。
2. §3 强制锚点全部存在且引文 `grep -F` 100% 命中、位于符号定义体内（P9-2、13）；`[VERIFIED` 计数 = `self_check.verified_claims`。
3. deps-v2.yaml 所有 `init_order` 边有 `path::function` 证据；无证据的边 `confidence: low` 且出现在 errata；config 边 ≥ 4 条且每条引 `options_flags.cmake`。
4. init-sequence-v2 的每一步有引文；全文无 `:数字` 形式的行号引用（P9-4 之外的本任务专项正则 `\.c:[0-9]+`、`\.S:[0-9]+` 0 命中）。
5. `drivers.rtc` 判定已修正并附调用链引文；errata 覆盖 v1 核心结论 1~7。
6. 正则 `[0-9a-f]{40}` 0 命中；`supersedes` 所指路径存在（P9-1、3）。
