# MYOS2-DR-001R · 外围工程审计复跑（001 含假 SHA 与成段虚构；任务书 v2，勘误优先）

```yaml
task_id: MYOS2-DR-001R
supersedes: agent-workspace/tasks/MYOS2-DR-001R-build-and-scripts-audit-rerun.md（v1 任务书，保留不改）；产出 supersedes MYOS2-DR-001（results/MYOS2-DR-001/ 保留不改）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支的根目录脚本、scripts/、三套 CMakeLists）
priority: P1（Owner 目的①；受 Owner 决策 2/3/4 制约，建议决策后发射）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-001R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v2.md（全部十三条适用，硬性；与本任务书冲突时以协议为准）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.1、§3.2、§7.4；再按 §2 输入清单读 001 的指定文件
drafted_by: MYOS2-LEAD-001（v2：2026-09-05）
status: draft（发射由 Owner）
```

## 1. 为什么复跑

001 是九份里编造最重的一份：全部 19 个交付文件（含 14 个 proposed/ 脚本头部）写了同一个不存在的 40 位 commit（前 12 位真、后 28 位编造）；问题清单 H-03（`/dev/nbd0`、`qemu-nbd`、`losetup -D`）与 H-05（`install_boostloader() || true`）整条虚构，真实实现是 `losetup --show -f`＋`kpartx`、函数真名 `make_install_bootloader`；M-02 判反且危险（称 `make_install.sh` 默认无操作，实际无参即 `rm -rf ./build/*` 并挂盘安装）；MANIFEST 称 proposed 脚本可直接运行，实测报错退出；抽查 24 条 14 条被证伪。但其 H/M/L 分类框架与目标结构方向仍可用，所以本任务是**逐条勘误＋只补两件改进件**，不整包重写。

## 2. 输入清单（文件级；协议 P11）

| 输入 | 只读哪部分 | 提示 |
|---|---|---|
| `results/MYOS2-DR-001/02-problems.md`（10,790 B） | 全文（勘误对象） | 其"代码事实"段落多处虚构，逐条重核 |
| `results/MYOS2-DR-001/MANIFEST.md` | 只取 artifacts 与 warnings 段 | 文件头 `commit:` 是编造值，不得转抄 |
| `results/MYOS2-DR-001/proposed/scripts/common.sh` | 全文 | 双确认原语思路可借鉴，须重写并去掉假 SHA 头 |
| `WAVE-1-REVIEW.md` | §3.1、§3.2、§7.4 | 已证实的证伪与危险脚本清单 |
| time 分支脚本（**只读，绝不运行**）：根目录 `make_install.sh`、`dbg-vmw.sh`、`dbg-qemu.sh.bak`；`scripts/{make_install_bootloader,make_install_initranfs,make_install_kernel,map_vdisk,part_vdisk,phys_nvme_install,prepare_env,util_cmds}.sh`；`.vscode-kdbg/run-qemu-gdb-myos2.sh`、`.vscode-kdbg/kill-qemu-myos2.sh` | 全文（都很短） | 现状事实的唯一来源 |
| CMake：`mykernel/CMakeLists.txt`、`mykernel/scripts/{options_flags,target_kbuild,target_kernel}.cmake`、`myloader/CMakeLists.txt`、`myinitramfs/CMakeLists.txt`、`myloader/uefi/install_loader.sh` | 全文 | 三套构建链 |

不读 `01-inventory.md`、`03-target-structure.md`、其余 proposed/ 文件（不在勘误范围）；不读 `.vscode/` 之外的 IDE 配置。

## 3. 强制 [VERIFIED] 锚点（不得降级、不得省略；脚本顶层语句写作 `path::(top-level)`，函数内语句写作 `path::function`）

`make_install.sh::(top-level)` 含 `rm -rf ./build/*` 的行；`make_install.sh::(top-level)` 含 `/dev/disk/by-id/` 的行；`scripts/phys_nvme_install.sh` 含 `rm -rf /mnt1/*` 的行（写明所在函数）；`scripts/part_vdisk.sh::(top-level)` 文件末尾未注释的 `do_part $MYOS_VMDK_NVME0` 行；`scripts/part_vdisk.sh::(top-level)` 含 `.vmdk` 的变量赋值行；`scripts/map_vdisk.sh` 含 `losetup --show -f` 与 `kpartx -av` 的两行（写明所在函数）；`scripts/util_cmds.sh` 全文（一行）；`scripts/make_install_bootloader.sh` 中定义 `make_install_bootloader` 函数的行；`mykernel/CMakeLists.txt` 含 `GLOB_RECURSE` 的行；`myloader/CMakeLists.txt` 含 `install(PROGRAMS BOOTX64.EFI` 的行；`myinitramfs/CMakeLists.txt` 的两行 `add_subdirectory(`。

## 4. 目标

1. **errata.md（主件）**：对 `02-problems.md` 的每一条 H-xx / M-xx / L-xx 逐条判定 保留（已重核）/ 修正 / 撤回，每条附协议 P2 引文。已知必须撤回：H-03、H-05；必须修正：H-04（`util_cmds.sh` 只有一行）、M-02（反向：默认即破坏性）、M-11、M-12。
2. **01-inventory-v2.md**：§2 列出的每个脚本与三套 CMakeLists 各一段"它实际做什么、依赖什么环境"，每段至少一条引文；**四个危险点必须逐字引用**（`make_install.sh` 无参路径的 `rm -rf`；`phys_nvme_install.sh` 的 `rm -rf /mnt1/*`；`part_vdisk.sh` 末尾无条件的分区/格式化调用；硬编码的 VMDK 与 by-id 设备路径）。
3. **02-problems-v2.md**：按危害排序，每条：引文证据 / 影响 / 修复方向 / 是否需要 Owner 决策（对应 WAVE-2-PLAN §3 决策 2「BOOTX64.EFI 来源」、3「输出目录 out/」、4「物理盘同步范围」）。条目上限 30。
4. **改进件（只交两件）**：`proposed-v2/scripts/common.sh`（fail-closed 双确认原语：`MYOS2_ALLOW_DESTRUCTIVE=1`＋`MYOS2_CONFIRM_TARGET=<与目标完全一致的路径>`，`--dry-run` 默认开）与 `proposed-v2/make_install.sh`（薄封装，默认 dry-run 打印将执行的动作，破坏性动作经 common.sh 双确认）。两件首部标 `UNTESTED — generated without execution`（协议 P10）；MANIFEST 列 `symbols_referenced`（脚本调用的每个外部命令名）。

## 5. 交付物与 MANIFEST

`MANIFEST.md`（必交；`base_snapshot: time（分支名）`、`read_channel`、`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`、`symbols_referenced`、`supersedes`、"未运行任何脚本"声明）＋`errata.md`＋`01-inventory-v2.md`＋`02-problems-v2.md`＋`proposed-v2/scripts/common.sh`＋`proposed-v2/make_install.sh`。降级交付按协议 P12（顺序 MANIFEST → errata → 01 → 02 → proposed-v2）。读不完的优先顺序：errata 的 H 条目 → 四个危险点 → M 条目 → L 条目 → 改进件。

## 6. 边界

绝不运行任何脚本；不修改内核本体与原脚本；不改 results/MYOS2-DR-001/；不写 40 位 SHA；不声称任何脚本可运行；不重写 `03-target-structure.md`（方向性提案待 Owner 决策后另案）；不读 bugs_record.md/todo.txt 当现状；不转抄 001 任何文件头字段。

## 7. 验收判据（本地闸门按协议 P9 执行；任一项不过整份退回）

1. MANIFEST 六字段齐全；`startup_selfcheck_quote` 命中；金丝雀两行 time 命中、master 不命中（P9-5、7、8）。
2. §3 强制锚点全部存在、引文 100% 命中（P9-2、13）；`[VERIFIED` 计数 = `self_check.verified_claims`。
3. errata 覆盖 `02-problems.md` 的**全部** H/M/L 条目；H-03/H-05 撤回、M-02 修正为"默认即破坏性"。
4. 两件改进件存在、首部含 `UNTESTED`、默认 dry-run、双确认；`symbols_referenced` 列出的命令名在两件脚本中确实出现（P9-9、11）。
5. 正则 `[0-9a-f]{40}` 0 命中；`supersedes` 路径存在（P9-1、3）。
