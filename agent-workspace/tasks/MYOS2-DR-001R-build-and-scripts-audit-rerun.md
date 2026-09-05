# MYOS2-DR-001R · 外围工程审计复跑（001 含假 SHA 与成段虚构，整体降权；本次以勘误为主）

```yaml
task_id: MYOS2-DR-001R
supersedes: MYOS2-DR-001（原目录 results/MYOS2-DR-001/ 保留不改）
mode: 普通对话 Pro（GitHub 连接器；读 time 分支的根目录脚本、scripts/、三套 CMakeLists）
priority: P1（Owner 目的①；但受 Owner 决策 2/3/4 制约，建议在决策后发射）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-001R/
protocol: agent-workspace/tasks/00-gpt-task-protocol-v1.md（全部十条适用，硬性）
prerequisites: 先读 conventions.md、上述协议、WAVE-1-REVIEW.md §3.1 与 §7.4；再读 results/MYOS2-DR-001/{MANIFEST.md,01-inventory.md,02-problems.md,03-target-structure.md}（只读，待勘误对象）
drafted_by: MYOS2-LEAD-001（2026-09-03）
status: draft（发射由 Owner）
```

## 为什么复跑

第一波 001 是九份里编造最重的一份：
- 全部 19 个交付文件（含 14 个 proposed/ 脚本头部）写了同一个不存在的 40 位 commit SHA（前 12 位真、后 28 位编造）；
- 问题清单 H-03（`/dev/nbd0`、`qemu-nbd`、`losetup -D`）与 H-05（`install_boostloader() || true`）整条虚构——仓库里没有这些内容，真实实现是 `losetup --show -f`＋`kpartx`，函数真名 `make_install_bootloader`；
- M-02 判反且危险：称 `make_install.sh` 默认无操作，实际无参运行即 `rm -rf ./build/*` 并挂盘安装；
- MANIFEST 称 proposed 脚本"可直接运行"，实测报错退出；
- 抽查 24 条中 14 条被证伪。

但 001 的**问题分类框架（H/M/L）和目标结构提案的方向**仍有价值，所以本任务不是从零重做，而是**逐条勘误＋只补最有用的两件改进件**。

## 目标

1. **errata.md（主件）**：对 `results/MYOS2-DR-001/02-problems.md` 的每一条 H-xx / M-xx / L-xx 逐条判定 **保留 / 修正 / 撤回**，每条附协议 P2 格式的引文证据（脚本文件路径＋函数名/关键行的逐字引文）。已知必须撤回：H-03、H-05；必须修正：H-04（`util_cmds.sh` 只有一行）、M-02（反向：默认即破坏性）、M-11、M-12。
2. **01-inventory-v2.md**：每个脚本（根目录 `make_install.sh`、`dbg-vmw.sh`、`dbg-qemu.sh.bak`、`scripts/` 下八个）和三套 CMakeLists 各一段"它实际做什么、依赖什么环境"，每段至少一条逐字引文。**四个危险点必须逐字引用**：`make_install.sh` 无参路径的 `rm -rf`；`scripts/phys_nvme_install.sh` 的 `rm -rf /mnt1/*`；`scripts/part_vdisk.sh` 末尾无条件调用的分区/格式化函数；硬编码的 VMDK/by-id 设备路径。
3. **02-problems-v2.md**：按危害排序的问题清单，每条：引文证据 / 影响 / 修复方向 / 是否需要 Owner 决策（对应 WAVE-2-PLAN §3 决策 2「BOOTX64.EFI 来源」、3「输出目录 out/」、4「物理盘同步范围」）。
4. **改进件（只交两件，其余不交）**：
   - `proposed-v2/scripts/common.sh`：fail-closed 双确认原语（`MYOS2_ALLOW_DESTRUCTIVE=1` ＋ `MYOS2_CONFIRM_TARGET=<与目标完全一致的路径>`）＋ `--dry-run` 默认开；
   - `proposed-v2/make_install.sh`：薄封装，默认 dry-run 打印将执行的动作，任何破坏性动作经 common.sh 双确认；
   - 两件头部标 `# UNTESTED — generated without execution`（协议 P10）。不再整包重写全部脚本。

## 输入（务必消费）

- `agent-workspace/WAVE-1-REVIEW.md` §3.1（001 的证伪清单）、§7.4（三个危险脚本的实证）；
- time 分支：根目录 `make_install.sh`、`dbg-vmw.sh`、`dbg-qemu.sh.bak`；`scripts/*.sh`；`mykernel/CMakeLists.txt`、`mykernel/scripts/*.cmake`、`myloader/CMakeLists.txt`、`myinitramfs/CMakeLists.txt`；
- `results/MYOS2-DR-001/`（待勘误对象；其 `proposed/common.sh` 的双确认思路可借鉴，但须重写并去掉假 SHA）。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交；含 `read_channel`、`self_check`、`supersedes`、"未运行任何脚本"声明）＋`errata.md`＋`01-inventory-v2.md`＋`02-problems-v2.md`＋`proposed-v2/scripts/common.sh`＋`proposed-v2/make_install.sh`。

## 边界

不修改内核本体与原脚本；不改 results/MYOS2-DR-001/；不写 40 位 SHA；不声称任何脚本可运行；不重写 `03-target-structure.md`（方向性提案保留，待 Owner 决策后另案）；不读 bugs_record.md/todo.txt 当现状。

## 验收判据（本地闸门按协议 P9 执行）

- errata 覆盖 001 `02-problems.md` 的**全部** H/M/L 条目，H-03/H-05 撤回、M-02 修正为"默认即破坏性"；
- 四个危险点逐字引用且本地 `grep -F` 命中；
- 全部 [VERIFIED] 引文闸门 100% 命中；
- 两件改进件含 UNTESTED 标记、默认 dry-run、双确认；
- MANIFEST `self_check` 自洽；全文无 40 位 SHA；`base_snapshot: time（分支名）`。
