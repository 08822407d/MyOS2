---
task_id: MYOS2-DR-001
task_type: design_review
status: complete
produced_by: GPT-5.6 Pro
produced_at: 2026-09-01
base_snapshot:
  branch: time
  commit: a039d9803ade94c67918930525530d2a1b46e9f0
workspace_snapshot:
  branch: master
  commit: c7e68edb3dd76f4e4808dff12b5ca2021035e719
inputs:
  - agent-workspace/conventions.md@master
  - agent-workspace/repo-map.md@master
  - agent-workspace/tasks/MYOS2-DR-001-build-and-scripts-audit.md@master
  - root shell scripts@time
  - scripts/*.sh@time
  - all CMakeLists.txt@time
  - .vscode and .vscode-kdbg debug entrypoints@time
artifacts:
  - 01-inventory.md
  - 02-problems.md
  - 03-target-structure.md
  - proposed/make_install.sh
  - proposed/dbg-qemu.sh
  - proposed/scripts/common.sh
  - proposed/scripts/prepare_env.sh
  - proposed/scripts/map_vdisk.sh
  - proposed/scripts/part_vdisk.sh
  - proposed/scripts/phys_nvme_install.sh
  - proposed/scripts/make_install_kernel.sh
  - proposed/scripts/make_install_initranfs.sh
  - proposed/scripts/make_install_bootloader.sh
  - proposed/cmake/MyOS2Common.cmake
  - proposed/myinitramfs/CMakeLists.txt
  - proposed/myloader/CMakeLists.txt
warnings:
  - 本审计为静态分析；未对任何磁盘、镜像或挂载点执行命令。
  - 未在真实 MyOS2 工具链上完成全量构建；proposed 中的 shell 文件仅做 Bash 语法校验。
  - proposed 文件位于隔离结果目录；它们不会改变 time 分支源码。
open_questions:
  - BOOTX64.EFI 的权威来源是仓库内预生成文件，还是 myloader/uefi 下的 EDK2 工程？
  - myinitramfs/mylib 是故意停用的实验树，还是遗漏了顶层 add_subdirectory？
  - 仓库根 build/kernel 与 build/sh 是否由未入库的外部聚合流程产生？
  - 物理盘安装应复制固定目录集合，还是同步完整根文件系统？
---

# MYOS2-DR-001 交付清单

## 结论摘要

**代码事实：** `time@a039d9803ade94c67918930525530d2a1b46e9f0` 的构建与安装能力分散在根脚本、`scripts/`、三套 CMake、loader 的 EDK2 辅助脚本，以及 VS Code 私有调试脚本中。危险操作与普通构建没有形成清晰权限边界；多处设备、挂载点、个人目录和镜像路径被写死。

**合理推断：** 当前流程主要围绕单一开发机逐步演化，能够支持作者本机调试，但难以让另一台主机在不阅读脚本实现的前提下安全复现。

**建议方案：** 先不重构内核源码目录；第一阶段只引入 fail-closed 配置、统一 CLI、资源所有权清理和 build/package/install/debug 分层。第二阶段再收敛 CMake 的全局 flags、递归 GLOB 和空目录控制面。

## 交付物用途

| 文件 | 用途 |
|---|---|
| `01-inventory.md` | 脚本、CMake、调试入口、输入输出和外部依赖的现状盘点 |
| `02-problems.md` | H/M/L 风险清单，含证据、影响与修复方向 |
| `03-target-structure.md` | 最小目标结构、配置优先级、迁移阶段和验收标准 |
| `proposed/` | 不触碰源码的可试用安全脚本与 CMake 迁移样例 |

## 使用 proposed 的边界

`bash proposed/make_install.sh help`、`bash proposed/dbg-qemu.sh --help` 可直接从结果目录运行。脚本通过 `git rev-parse` 或 `MYOS2_ROOT` 定位真实仓库根目录；所有设备破坏操作必须同时提供：

```text
MYOS2_ALLOW_DESTRUCTIVE=1
MYOS2_CONFIRM_TARGET=<与目标设备完全一致的路径>
```

未满足这两个条件时，脚本必须失败退出。
