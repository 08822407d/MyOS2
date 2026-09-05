# MYOS2-DR-003R · 依赖关系图与初始化序列重锚（003 复跑：行号锚点腐烂、rtc 误判）

```yaml
task_id: MYOS2-DR-003R
supersedes: MYOS2-DR-003（原目录 results/MYOS2-DR-003/ 保留不改；本任务产出 -v2 文件并写 supersedes）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支源码）
priority: P0（002/007 的交叉综合、menuconfig 式裁剪、004R 的启动检查点都以它的锚点为地基）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-003R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v1.md（全部十条适用，硬性）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.4 与 §4.1~4.2；再读 results/MYOS2-DR-003/{MANIFEST-v2.md,deps.yaml,init-sequence.md,trim-analysis.md}（只读，作为待勘误对象）
drafted_by: MYOS2-LEAD-001（2026-09-03）
status: draft（发射由 Owner）
```

## 为什么复跑

第一波 003 的**内容结构是好的**（102 节点/123 边、三层依赖、初始化时序），但它的证据锚点不能用：
- 65 个 `路径:行号` 引用里，抽 30 处实读，多数落在空行、注释行或无关行（例 `init/main.c:276` 是一行 `*/`）；`init-sequence.md` 引 `mm_init.c:430-650`，该文件只有 430 行；`setup_arch()` 实际在 `init/main.c` 第 138 行而非所引区间的起点。
- `trim-analysis.md` 把 `drivers.rtc` 判为可裁剪叶，但 `timekeeping_init → read_persistent_clock64 → rtc` 是启动主链的一部分。
- 全文未读 `mykernel/scripts/options_flags.cmake`（实际生效的 CONFIG 宏只有 12 个，`-DROOTBLK_NVME` 被注释、根块设备走 ATA）——所有"当前生效/可裁剪"判断都缺配置面依据。

## 目标

1. **deps-v2.yaml / deps-v2.dot**：节点与边沿用 v1 的集合为起点；
   - 每条 `init_order` 边的证据改为 `[VERIFIED path::function]`＋逐字引文（协议 P2）；证据核不出来的边降为 `confidence: low` 并在 errata 登记，不得静默保留；
   - 新增 `kind: config` 边：来自 `options_flags.cmake` 的宏对子系统的选择（至少：`ROOTBLK_*` → `drivers.ata` / `drivers.nvme`，`CONFIG_SLUB`、`CONFIG_HYPERVISOR_GUEST`/`CONFIG_KVM_GUEST`、`GRUB2_BOOTUP_SUPPORT` 各自影响哪些节点）；
   - 保持词汇表 ID（conventions.md §3）；新造节点只增不改。
2. **init-sequence-v2.md**：从 `phys_startup_64` 到 `/boot/init` 的每一步都写 `函数名＋引文`，**不写行号**；被注释掉的路径单列一节（"源码存在但未生效"），每条附引文证明它被注释。
3. **trim-analysis-v2.md**：
   - 撤回 `drivers.rtc` 可裁剪判断，附 `timekeeping_init` 调用链引文；
   - 每个"可选叶"判断附配置面依据（哪条宏、哪段 `#if`）；
   - 明确区分"进入编译输入"与"运行时可达"（v1 已提出，本次给每个叶子标其所属）。
4. **errata.md**：对 v1 的核心结论 1~7、每条被改动的边、被撤回的判定逐条写 保留/修正/撤回＋证据。

## 输入（务必消费）

- `agent-workspace/WAVE-1-REVIEW.md` §4.1（options_flags.cmake 的 12 个生效宏）、§4.2（根块设备是 ATA）、§3.4（行号降级纪律）；
- `mykernel/scripts/options_flags.cmake`（协议 P4 的前置）；
- `results/MYOS2-DR-003/` 全部文件（待勘误对象）。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含 `read_channel`、`self_check`、`supersedes`）＋`errata.md`＋`deps-v2.yaml`＋`deps-v2.dot`＋`init-sequence-v2.md`＋`trim-analysis-v2.md`。

## 边界

不修改内核源码、不改 results/MYOS2-DR-003/、不改公约与任务书；不写 40 位 SHA（协议 P1）；不基于行号做任何断言；不读 bugs_record.md/todo.txt 当现状；不做 menuconfig 实现。

## 验收判据（本地闸门按协议 P9 执行）

- 所有 `init_order` 边与 init-sequence-v2 的每一步均为 `path::function`＋引文，引文本地 `grep -F` **100% 命中**；
- `options_flags.cmake` 已读且被引用（至少 12 个生效宏逐条出现在 config 边或 trim-analysis-v2）；
- `drivers.rtc` 判定已修正并附调用链引文；
- errata 覆盖 v1 核心结论 1~7；
- MANIFEST 的 `self_check` 数字自洽（`quotes_reconfirmed + downgraded_to_inferred = verified_claims`）；
- 全文无 40 位 SHA；`base_snapshot: time（分支名）`。
