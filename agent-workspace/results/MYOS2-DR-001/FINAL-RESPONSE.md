## 已完成

已在从 `master` 创建的分支 `agent/MYOS2-DR-001` 上完成任务，并向 `master` 提交 PR：

**[PR #2 · MYOS2-DR-001 构建与自动化脚本审计及安全化提案](https://github.com/08822407d/MyOS2/pull/2)**

PR 当前处于 open、非 draft、可合并状态；head 为 `7b01ff72665edccb86f63e98e192cf8ab80ba5cf`。共新增 17 个文件、1640 行，零删除。fileciteturn180file0L2-L16 fileciteturn180file0L31-L35

- 分析源码基线：`time@a039d9803ade94c67918930525530d2a1b46e9f0`
- 工作区与 PR 基线：`master@c7e68edb3dd76f4e4808dff12b5ca2021035e719`
- 唯一写入目录：`agent-workspace/results/MYOS2-DR-001/`
- [查看完整结果目录](https://github.com/08822407d/MyOS2/tree/agent/MYOS2-DR-001/agent-workspace/results/MYOS2-DR-001)

## 交付物

核心文档包括：

- `MANIFEST.md`
- `01-inventory.md`
- `02-problems.md`
- `03-target-structure.md`

`proposed/` 下另有 13 份改进件，覆盖统一根 CLI、QEMU/GDB 启动、环境解析、NBD 映射、分区与物理盘同步安全门、kernel/initramfs/loader 构建安装流程，以及 CMake 公共模块和两个子工程样例。

## 主要结论

最高风险集中在设备破坏边界：

1. `scripts/part_vdisk.sh` 在文件末尾无条件调用分区、格式化流程。
2. `scripts/phys_nvme_install.sh` 未验证挂载点便执行 `rm -rf /mnt2/*`。
3. 镜像流程固定使用 `/dev/nbd0`，并调用会影响无关资源的全局 `losetup -D`。
4. 映射、挂载和调用者之间没有单一资源所有者及可靠的 `EXIT` 清理。
5. bootloader 安装用 `|| true` 吞掉失败，可能把不完整介质报告为成功。
6. 特权路径中存在 `eval`，扩大了参数解析错误和命令注入风险。

构建系统方面，`time` 分支共核对到 101 份 `CMakeLists.txt`：内核 80 份、initramfs 20 份、loader 1 份。内核的大量目录 CMake 为空或只负责继续递归，实际源码仍由顶层 `GLOB_RECURSE` 整体纳入；initramfs 存在个人 musl 路径、在 `project()` 后切换编译器、空 `elseif ()`，以及整套未接入顶层的 `mylib` 子树。

QEMU 一键能力并非完全缺失：VS Code 已有 host、KVM、TCG 三套任务。实际缺口是这些能力隐藏在编辑器配置中，根目录只有过时的 `.bak` 入口，磁盘镜像仍固定在个人 `$HOME/vmware/...` 路径，而且 GDB 使用的根 `build/kernel`、`build/sh` 与现行子工程 build 目录不一致。

早期关于“四个被 source 的脚本不存在”的误判已经剔除；这些脚本在 `time` 分支均存在，没有写入最终问题表。

## proposed 的安全边界

新提案将破坏性操作改成 fail-closed。除显式目标外，还必须同时满足：

```bash
MYOS2_ALLOW_DESTRUCTIVE=1
MYOS2_CONFIRM_TARGET=<规范化后与实际目标完全一致>
```

映射函数只负责 map/unmap，不再隐式挂载；调用者使用私有临时挂载点，并通过 `EXIT` trap 只清理本次创建的资源。普通 `build`、`package` 和目录 `install` 与设备部署已经分层。

## 验证

- 10 份 proposed shell 全部通过 `bash -n`。
- 根 CLI 的 `help`、`doctor` 和 QEMU 的 `--help` 已在最小环境中执行。
- `MyOS2Common.cmake` 已通过 CMake script-mode 解析。
- loader 与 initramfs CMake 提案已通过最小工程 configure。
- 未执行任何磁盘映射、挂载、分区、格式化或物理盘复制命令。
