---
task_id: MYOS2-DR-001
task_type: design_review
status: complete
produced_by: GPT-5.6 Pro
produced_at: 2026-09-01
base_snapshot:
  branch: time
  commit: a039d9803ade94c67918930525530d2a1b46e9f0
inputs:
  - make_install.sh
  - dbg-vmw.sh
  - dbg-qemu.sh.bak
  - scripts/*.sh
  - myloader/uefi/install_loader.sh
  - .vscode/*.json
  - .vscode-kdbg/*.sh
  - all CMakeLists.txt
artifacts:
  - 01-inventory.md
warnings:
  - “幂等性”按重复执行是否会扩大副作用进行静态判定，未进行设备实测。
open_questions:
  - 根 build/ 目录是否由仓库外工具生成？
---

# 01 · 构建与脚本现状盘点

## 1. 审计范围与计数

**代码事实：**

- 根目录正式 `.sh`：`make_install.sh`、`dbg-vmw.sh`。
- 根目录相邻遗留调试文件：`dbg-qemu.sh.bak`。
- `scripts/` 下 shell：8 份。
- 补充读取：`myloader/uefi/install_loader.sh`、`.vscode-kdbg/` 两份 QEMU 脚本、`.vscode/tasks.json`、`.vscode/launch.json`。
- CMake：内核树 80 份、initramfs 树 20 份、loader 顶层 1 份，共 101 份。内核数量按 `time` 的递归树和当前 `add_subdirectory()` 图交叉核对。

## 2. shell 入口清单

| 路径 | 角色 | 主要输入 | 主要输出/副作用 | 外部依赖 | 幂等性与失败行为 |
|---|---|---|---|---|---|
| `make_install.sh` | 聚合入口 | 环境变量、手工取消注释 | 默认只 source 文件，无实际动作 | Bash、`realpath` | 默认无操作；无 CLI；仅 `set -e` |
| `scripts/prepare_env.sh` | 推导仓库路径 | `MYOS2_ROOT` | 导出三个子工程路径 | Bash、`realpath` | 可重复；不验证目录 |
| `scripts/util_cmds.sh` | source/函数辅助 | 路径、函数名 | 动态 source/调用 | Bash、`eval` | 对空格和恶意输入脆弱 |
| `scripts/make_install_kernel.sh` | 构建/安装内核 | 源码、安装前缀、镜像 | 清空 build、生成 kernel、挂载/安装 | CMake、make、sudo、mount、qemu-nbd/kpartx/losetup | 构建前删除；失败可遗留挂载与映射 |
| `scripts/make_install_initranfs.sh` | 构建 musl/initramfs | 固定 musl 源路径 | musl 安装树、initramfs 产物 | configure、make、CMake | 修改外部源码树；路径不可移植 |
| `scripts/make_install_bootloader.sh` | 安装 EFI loader | 仓库内 EFI、设备/镜像 | 分区、格式化、挂载、复制 | CMake、sgdisk、mkfs.fat、sudo | 安装失败被 `|| true` 吞掉；可破坏目标 |
| `scripts/map_vdisk.sh` | 映射镜像 | VMDK 路径 | `/dev/nbd0` 映射、`/mnt` 挂载 | qemu-nbd、modprobe、partx、mount | 固定设备；检测逻辑混用 losetup/qemu-nbd |
| `scripts/part_vdisk.sh` | 分区/格式化镜像 | 文件尾硬编码 VMDK | 清分区表、建 GPT、mkfs | sgdisk、mkfs、qemu-nbd、sudo | 文件尾无条件执行；最高风险 |
| `scripts/phys_nvme_install.sh` | 复制到物理盘挂载点 | 固定 `/mnt`、`/mnt2` | `rm -rf /mnt2/*` 后复制 | rm、cp | 无 shebang/严格模式/挂载验证 |
| `dbg-vmw.sh` | 启动 VMware | `$HOME/vmware/...` | 启动 VMware | vmware | 固定个人路径；变量未引用 |
| `dbg-qemu.sh.bak` | 遗留 QEMU/GDB 启动 | 参数 `zbook/asus`、固定 VMX | 启动暂停 QEMU | QEMU、OVMF | 未传参数时测试表达式脆弱；文件为 `.bak` |
| `myloader/uefi/install_loader.sh` | EDK2 产物安装 | `/dev/dm-0`、EDK2 Debug 路径 | 挂载、复制 EFI、写个人目录 | mount、cp、sudo | 固定设备和个人路径；无 trap |
| `.vscode-kdbg/run-qemu-gdb-myos2.sh` | VS Code QEMU/GDB | 多个环境变量、固定磁盘路径 | QEMU、PID 文件、GDB 端口 | QEMU、OVMF、nc 或 `/dev/tcp` | 已有 `set -euo pipefail` 与就绪检测；磁盘仍硬编码 |
| `.vscode-kdbg/kill-qemu-myos2.sh` | 停止 QEMU | PID 文件 | TERM/KILL、删 PID 文件 | kill | 仅按 PID 文件；可能误杀 PID 重用后的无关进程 |

## 3. 构建数据流

### 3.1 内核

```text
mykernel/CMakeLists.txt
  ├─ include scripts/options_flags.cmake
  ├─ add_executable(kernel)
  ├─ include scripts/target_kbuild.cmake
  │    └─ asm-offset -> Python 生成 include/generated/asm-offsets.h
  ├─ GLOB_RECURSE 全部 *.c / *.S
  └─ include scripts/target_kernel.cmake
       ├─ target_sources(kernel, 全局通配结果)
       ├─ linker script: arch/${ARCH}/kernel.lds
       ├─ add_subdirectory 进入 15 个顶层子系统
       └─ install kernel -> boot/
```

**代码事实：** 目录级 CMake 大多只继续 `add_subdirectory()`；叶 CMake 为空。真正的源文件纳入由顶层 `GLOB_RECURSE` 决定，因此目录 CMake 当前不形成编译边界。

输入：全部 C/ASM 源、链接脚本、Python 生成器、主机 GCC 内部 include。  
输出：`mykernel/build/kernel`（按现行 shell 构建函数）、生成的 asm-offset 文件、复制出的 UAPI headers。  
隐含状态：生成文件写回源码树 `include/generated` 与 `arch/.../kbuild`。

### 3.2 initramfs

```text
scripts/make_install_initranfs.sh
  ├─ 在固定 $HOME/workspace/projects/musl-1.2.3 构建/安装 musl
  └─ cmake myinitramfs
       ├─ add_subdirectory(myinit) -> init
       ├─ add_subdirectory(myshell) -> sh
       └─ install init/sh -> boot/
```

**代码事实：** 顶层 CMake 在 `project()` 后设置个人路径中的 `musl-gcc`，含 `elseif ()`；`mylib/` 的 17 份后续 CMake 未被顶层接入。现行 shell 期望 `build/myinitramfs.gz`，但已读 CMake 中未找到负责创建该归档的规则。

### 3.3 loader

```text
myloader/CMakeLists.txt
  └─ install 仓库内 BOOTX64.EFI -> EFI/BOOT
```

`myloader/uefi/` 同时保留 EDK2 源与 `install_loader.sh`，后者从 EDK2 `DEBUG_GCC5` 目录取产物。顶层 CMake 不构建这套源代码，因此存在“预生成二进制安装”和“仓库外 EDK2 构建”两条路径。

## 4. CMake 结构分类

| 集合 | 数量 | 当前作用 |
|---|---:|---|
| `mykernel/CMakeLists.txt` + 3 个 helper `.cmake` | 1 + 3 | 实际配置、源扫描、生成文件、链接和安装 |
| 内核目录分发 CMake | 约 15 | 只递归进入下一层 |
| 内核空叶 CMake | 其余 | 不声明源、目标或选项 |
| initramfs 活跃 CMake | 3 | 顶层、`myinit`、`myshell` |
| initramfs 未接入 CMake | 17 | `mylib/myglibc` 实验/遗留静态库树 |
| loader CMake | 1 | 安装预生成 EFI |

## 5. 配置与硬编码矩阵

| 配置项 | 当前来源 | 是否有安全默认 | 问题 |
|---|---|---:|---|
| 仓库根 | 环境或脚本位置 | 有 | 不验证目录 |
| 构建目录 | 各子工程固定 `build/` | 有 | 与 VS Code 根 `build/` 不一致 |
| VMDK | 多处个人绝对路径 | 否 | 无法复现 |
| NBD | `/dev/nbd0` | 否 | 竞争与误操作 |
| 挂载点 | `/mnt`、`/mnt2` | 否 | 资源所有权不清 |
| 物理设备 | 注释或固定 `/dev/dm-0`、NVMe 示例 | 否 | 高破坏风险 |
| musl | 两套个人目录约定 | 否 | shell 与 CMake 还不一致 |
| OVMF | `myloader/OVMF_*.fd` | 部分 | 产物/依赖来源未说明 |
| QEMU 磁盘 | `$HOME/vmware/...` | 否 | VS Code 任务不可移植 |
| GDB 端口 | 8864 | 有 | 多实例冲突未检测 |
| 编译器 | 隐式主机 GCC / 后置 musl-gcc | 否 | 缺工具链契约 |

## 6. 文档与工具入口

- README 链接到 `master/user-guide/*.md`；`time` 当前根目录实际是 `documents/`，未见这些 `user-guide` 路径。
- 一键 QEMU **并非完全缺失**：VS Code 中已有 host/KVM/TCG 三套任务。
- 命令行可发现性仍然缺失：根目录只有 `.bak` QEMU 脚本，且 VS Code 脚本不负责构建或准备镜像。
- VS Code 调试符号路径是 `${workspaceFolder}/build/kernel`、`build/sh`；现行 shell 分别在子工程 `build/` 下构建，未见仓库内聚合步骤。
