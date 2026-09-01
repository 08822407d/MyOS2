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
  - 01-inventory.md
  - audited source files
artifacts:
  - 02-problems.md
warnings:
  - 严重度衡量“最坏可造成的损失 × 触发难度”，不是对开发者意图的评价。
open_questions:
  - 目标磁盘是否始终是可丢弃测试介质？
---

# 02 · 问题与风险排序

## 严重度定义

- **H / High：** 一次误调用即可破坏宿主数据、影响无关资源，或让失败被错误报告为成功。
- **M / Medium：** 阻碍复现、使构建结果不稳定、让调试入口失效，或显著提高维护成本。
- **L / Low：** 命名、可移植性、可读性或长期演进问题。

## High

### H-01 · `part_vdisk.sh` 在文件尾无条件执行破坏流程

**代码事实：** `scripts/part_vdisk.sh` 定义硬编码 `VDISK_VMDK=...`，随后直接调用 `part_vdisk $VDISK_VMDK`；流程包含 `sgdisk --clear`、建分区和 `mkfs`。

**影响：** 执行脚本甚至误 source 都可能重写目标镜像。目标来自个人绝对路径，调用者没有机会在执行前审阅解析后的真实目标。

**建议方案：** 仅在 `[[ ${BASH_SOURCE[0]} == "$0" ]]` 时进入 `main`；目标只接受 `MYOS2_TARGET_DISK`/显式参数；要求双重确认变量；先验证 block device、挂载状态和子分区。

### H-02 · 物理盘复制脚本先对固定目录执行递归删除

**代码事实：** `scripts/phys_nvme_install.sh` 首行操作之一是 `rm -vrf /mnt2/*`，无 shebang、严格模式、mountpoint 验证或确认。

**影响：** `/mnt2` 未挂载时会删除宿主目录内容；错误挂载时会清空错误文件系统；glob 对隐藏文件还产生不完整清理。

**建议方案：** 源/目标必须显式配置；以 `mountpoint`/`findmnt` 验证；确认字符串必须与规范化目标一致；用受控 `find -mindepth 1 -maxdepth 1` 或 `rsync --delete`，并记录计划。

### H-03 · 固定 `/dev/nbd0` 与全局 `losetup -D` 破坏资源隔离

**代码事实：** `map_vdisk.sh` 固定连接 `/dev/nbd0`；安装函数结束时调用 `losetup -D`。映射实际由 `qemu-nbd` 建立，检测却查询 `losetup -a`。

**影响：** 可能占用正在使用的 NBD；`losetup -D` 会分离本任务没有创建的所有 loop 设备；检测失配会重复映射。

**建议方案：** 查找空闲 NBD 或要求显式设备；只记录并释放本次创建的资源；禁止全局清理命令。

### H-04 · 特权 `eval` 形成命令注入面

**代码事实：** `part_vdisk.sh` 的 `makedir`、`mklink` 以 `eval sudo ... "$@"` 拼接命令；`util_cmds.sh` 也以 `eval` 调函数。

**影响：** 含空格、通配符或 shell 元字符的参数可能改变命令边界；在 sudo 路径中后果扩大。

**建议方案：** 始终使用数组和 `sudo -- command "$arg"`；函数调用用 `declare -F` 验证后直接 `"$function_name"`。

### H-05 · bootloader 安装失败被强制视为成功

**代码事实：** `install_boostloader()` 使用 `sudo cmake --install ... || true`。

**影响：** 后续卸载/退出可以正常返回，调用者得到“成功”假象，最终介质可能缺少 EFI loader。

**建议方案：** 不吞错误；若某个已知非致命条件可接受，应只匹配该条件并显式 warning。

### H-06 · 挂载与映射没有单一所有者和 EXIT trap

**代码事实：** `map_vdisk_once()` 自身把 p2 挂到 `/mnt`，调用者随后再次 mount；多处流程在中间命令失败时没有清理 trap。

**影响：** 双重挂载、busy mount、NBD 残留、后续运行误用旧映射。

**建议方案：** map 函数只映射并返回设备；上层在 subshell 中创建私有临时挂载点，注册 `trap cleanup EXIT`，严格逆序释放。

## Medium

### M-01 · 主机、个人目录、设备与挂载点硬编码

证据包括：

- `make_install.sh` 的 VMware VMDK 绝对路径；
- initramfs shell 的 `$HOME/workspace/projects/musl-1.2.3`；
- initramfs CMake 的 `/home/cheyh/projs/musl/build`；
- loader 安装的 `/dev/dm-0` 与 `~/projs/MyOS2/...`；
- QEMU 的 `$HOME/vmware/myos2test/...`；
- `/mnt`、`/mnt2`、`/dev/nbd0`。

**影响：** 新主机不能复现；错误路径可能指向真实设备；个人路径进入日志和文档。

**建议方案：** 环境变量优先；只为无破坏性的路径提供自动探测并打印 warning；设备类变量无默认，缺失即失败。

### M-02 · 根入口默认无操作，用户必须编辑脚本

**代码事实：** `make_install.sh` 的所有动作均为注释行。

**影响：** 无法自动化，无法发现支持的流程，也无法稳定传参。

**建议方案：** 增加 `build/package/install/qemu/partition/sync-physical/help` 子命令；无参数打印帮助并非静默成功。

### M-03 · build、package、install 与设备写入混在同一函数族

**影响：** 普通构建不必要地接触 sudo；难以在 CI 只执行无特权阶段；故障定位模糊。

**建议方案：** 构建输出到 build root；打包输出到 artifacts；安装到普通目录前缀；镜像/物理盘部署是显式高风险末级。

### M-04 · initramfs CMake 的编译器和条件语句不可靠

**代码事实：** `project()` 后设置 `CMAKE_C_COMPILER`；存在 `elseif ()`；`if (${LINK_TYPE} STREQUAL "static")` 在变量未定义时不稳；musl 路径写死。

**影响：** CMake 可能已完成编译器识别才切换编译器；配置分支可能解析失败；缓存重用会放大差异。

**建议方案：** 使用 toolchain file 或在 `project()` 前从 `MYOS2_MUSL_CC` 设置编译器；缓存变量有默认值；使用 `if(CMAKE_BUILD_TYPE STREQUAL ...)`。

### M-05 · musl 构建污染外部源码树，且 shell/CMake 路径约定冲突

**代码事实：** shell 把 install prefix 放进 `${MUSL_SRC_DIR}/build`；CMake 期待另一套个人路径和 `exec/bin/musl-gcc`。

**影响：** 依赖不可重建；清理源树会删除工具链；两条路径可能使用不同 musl。

**建议方案：** `MYOS2_DEPS_ROOT/musl-build` 与 `musl-prefix` 分离；记录版本/commit；只把 prefix 传给 initramfs。

### M-06 · loader 是两条未统一的产物链

**代码事实：** loader 顶层 CMake只安装仓库内 `BOOTX64.EFI`；`uefi/install_loader.sh` 从 EDK2 Debug 输出复制另一份。

**影响：** 无法从二进制反推源码版本；开发者可能测试与提交不同的 EFI。

**建议方案：** 明确一种权威模式：A）仓库只消费外部构建产物并记录 hash；或 B）提供 EDK2 可复现构建封装。未决定前，安装脚本必须打印所用 EFI 的绝对路径与 hash。

### M-07 · 内核同时维护“递归 GLOB”与“目录 CMake”两套控制面

**代码事实：** 顶层 `GLOB_RECURSE` 把全部 C/ASM 纳入 `kernel`；80 份目录 CMake 中大多数只分发或为空。

**影响：** 新增源码会自动编入而无需修改清单；空 CMake 给出虚假的模块边界；局部编译选项和依赖无法自然表达。

**建议方案：** 短期只给 GLOB 加 `CONFIGURE_DEPENDS`、排除 build/generated/test；中期按子系统转为 `target_sources()`，每迁移一个子树就移出全局 GLOB。

### M-08 · 全局 CMake flags 污染所有目标

**代码事实：** `options_flags.cmake` 修改 `CMAKE_C_FLAGS`、`CMAKE_ASM_FLAGS`、`CMAKE_EXE_LINKER_FLAGS`；`asm-offset` 与 `kernel` 被迫共享大量内核链接/编译语义。

**影响：** 辅助生成器难以使用不同选项；子目录无法覆盖；toolchain 检测与缓存难追踪。

**建议方案：** 使用 INTERFACE options target，并分别链接到 `kernel`、`asm-offset`；编译定义、include 和 link option 都改为 target scope。

### M-09 · 生成文件写回源码树

**代码事实：** asm-offset 规则输出到 `arch/.../kbuild/asm-offsets.s` 和 `include/generated/asm-offsets.h`。

**影响：** 并行 build 目录互相覆盖；源码树变脏；不同配置共享生成物。

**建议方案：** 输出到 `${CMAKE_CURRENT_BINARY_DIR}/generated`，把该目录加入 include；需要提交的快照另设显式命令。

### M-10 · QEMU 功能存在但被编辑器入口和个人磁盘路径锁定

**影响：** 非 VS Code 用户难发现；CI/终端无法直接复用；其他主机立即因磁盘路径失败。

**建议方案：** 把 `.vscode-kdbg` 的严格模式、PID 和就绪检测抽成根 `dbg-qemu.sh`；VS Code 任务只调用它并传环境。

### M-11 · 调试符号路径与现行构建目录不一致

**代码事实：** launch 配置使用根 `build/kernel`、`build/sh`；shell 构建使用 `mykernel/build`、`myinitramfs/build`。

**影响：** GDB 可能载入旧文件或直接找不到符号。

**建议方案：** 统一 `MYOS2_BUILD_ROOT`；所有配置引用同一 cache/env 值，或由 CMake 生成调试配置片段。

### M-12 · README 链接和现状失配

**代码事实：** README 指向 `master/user-guide/*.md`，而 `time` 根目录只有 `documents/`；README 未提及现有 QEMU 任务。

**影响：** 新参与者从第一入口得到 404/旧分支信息。

**建议方案：** 在后续独立 PR 修复 README，避免本任务越界改源文档。

### M-13 · 缺少依赖预检和版本记录

**影响：** 失败发生在破坏性步骤中途；不同 GCC/CMake/QEMU/OVMF 产生不可比较结果。

**建议方案：** `doctor` 子命令检查命令、版本、KVM、NBD、OVMF、musl、Python；把结果写入 artifact manifest。

### M-14 · 多数 shell 缺严格模式、引用与 local

**影响：** 空变量、空格和 glob 会改变命令；函数间全局变量相互覆盖。

**建议方案：** 可执行入口 `set -Eeuo pipefail`；函数参数立即保存为 `local`; 所有路径加引号；清理命令显式 `--`。

## Low

### L-01 · 命名错误扩散

`boostloader`、`initranfs`、`partion` 已进入函数名。建议保留兼容 alias，同时新增正确名称，在一个版本周期后移除旧名。

### L-02 · 直接调用 `make -j`

建议使用 `cmake --build <dir> --parallel <N>`，避免绑定 generator。

### L-03 · GNU/Linux 与 Bash 依赖未声明

`realpath`、间接变量展开、`/dev/tcp`、sgdisk、qemu-nbd 等均非 POSIX。无需强行 POSIX 化，但应在 README/doctor 中明确支持平台。

### L-04 · 仓库跟踪大型可执行镜像但缺少来源元数据

根目录和 loader 跟踪 ISO/VMDK/EFI。若有意保留，应附生成命令、源 commit、hash 和用途；否则考虑 release artifact 或 LFS。此项不在本任务写入范围。
