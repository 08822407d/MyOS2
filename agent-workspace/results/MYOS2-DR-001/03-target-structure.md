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
  - 02-problems.md
artifacts:
  - 03-target-structure.md
  - proposed/
warnings:
  - 目标结构强调渐进迁移；不建议一次性移动内核源码目录。
open_questions:
  - 是否接受将所有 build 输出统一到仓库根 out/？
---

# 03 · 最小目标结构与迁移方案

## 1. 设计原则

1. **默认无破坏。** build、package、目录 install 永不接触块设备。
2. **配置先显式后推导。** 环境变量优先；仅安全路径可自动探测并 warning；设备/删除目标缺失即 fail。
3. **资源有唯一所有者。** 创建 NBD、挂载、PID 的函数负责精确清理自己创建的对象。
4. **每层只有一个入口。** 根 CLI 是人和 CI 的入口；VS Code 只调用根 CLI。
5. **保持兼容。** 旧拼写函数保留 alias，不再新增调用。
6. **先安全后整洁。** 第一批不移动源码、不改内核逻辑，只封住危险边界。

## 2. 建议目录

```text
make_install.sh                 # 唯一 CLI
dbg-qemu.sh                     # 可独立使用，亦由 CLI/VS Code 调用
scripts/
  common.sh                     # 日志、依赖检查、sudo、确认、路径验证
  prepare_env.sh                # 仓库与 build/artifact 路径
  make_install_kernel.sh        # build/install-to-prefix
  make_install_initramfs.sh     # dependency/build/package/install-to-prefix
  make_install_bootloader.sh    # resolve/package/install-to-prefix
  map_vdisk.sh                  # 只 map/unmap，不 mount
  part_vdisk.sh                 # 唯一分区/格式化入口，双确认
  phys_nvme_install.sh          # 唯一物理介质同步入口，双确认
cmake/
  MyOS2Common.cmake             # target-scoped 通用选项与工具检查
out/                            # 默认 build root，不提交
artifacts/                      # kernel/initramfs/EFI/image manifest
```

`proposed/` 已按这些目标路径镜像提供参考实现。

## 3. 配置解析契约

### 3.1 通用顺序

```text
1. 环境变量或显式 CLI 参数
2. 仓库内可证明安全的自动探测，并打印 WARNING
3. 无安全默认时立即失败，打印缺失变量和示例
```

### 3.2 可安全自动推导

| 变量 | 自动推导 |
|---|---|
| `MYOS2_ROOT` | `git rev-parse --show-toplevel`；否则脚本相对位置 |
| `MYOS2_BUILD_ROOT` | `${MYOS2_ROOT}/out/build`，warning |
| `MYOS2_ARTIFACT_ROOT` | `${MYOS2_ROOT}/out/artifacts`，warning |
| `MYOS2_JOBS` | `nproc` 或 1 |
| `MYOS2_GDB_PORT` | 8864 |
| `MYOS2_QEMU_ACCEL` | 有 `/dev/kvm` 则 kvm，否则 tcg，warning |

### 3.3 必须显式提供，禁止默认

- `MYOS2_TARGET_DISK`
- `MYOS2_SOURCE_ROOT` / `MYOS2_DEST_ROOT`（物理同步）
- `MYOS2_MUSL_SRC`（要从源码构建时）
- 找不到仓库内或系统 OVMF 时的 `MYOS2_OVMF_CODE` / `MYOS2_OVMF_VARS`
- 无仓库镜像可回退时的 `MYOS2_DISK_IMAGE`

### 3.4 破坏性双确认

```bash
export MYOS2_TARGET_DISK=/dev/nbd3
export MYOS2_ALLOW_DESTRUCTIVE=1
export MYOS2_CONFIRM_TARGET=/dev/nbd3
```

必须对目标执行 `realpath`/块设备规范化后再比较；任一不一致即失败。脚本还必须拒绝已挂载目标及其子分区。

## 4. 命令分层

```text
./make_install.sh doctor
./make_install.sh build kernel|initramfs|bootloader|all
./make_install.sh package initramfs|all
./make_install.sh install kernel|initramfs|bootloader|all <directory-prefix>
./make_install.sh qemu [--gdb-wait]
./make_install.sh partition
./make_install.sh sync-physical
```

- `build`：无 sudo。
- `package`：无 sudo；只写 `out/`。
- `install`：默认目录前缀；目标可写时无 sudo。
- `partition`/`sync-physical`：显式高风险命令，双确认。
- `qemu`：只读配置；默认不重写镜像分区。

## 5. CMake 收敛路径

### 阶段 A：不改变源码归属

- 把 `CMAKE_*_FLAGS` 改为 INTERFACE options target。
- GLOB 增加 `CONFIGURE_DEPENDS`，显式排除 build、generated、tests。
- 编译器由 toolchain file 或 `-DCMAKE_C_COMPILER` 在第一次 configure 时指定。
- asm-offset 输出移到 build tree。
- 删除/停止调用空叶 CMake，但暂不移动源码。

### 阶段 B：逐子系统显式源清单

每次只迁移一个顶层子系统：

```cmake
add_library(myos2_mm OBJECT)
target_sources(myos2_mm PRIVATE
  page_alloc/foo.c
  vm_map/bar.c
)
target_link_libraries(myos2_mm PRIVATE myos2_kernel_options)
target_sources(kernel PRIVATE $<TARGET_OBJECTS:myos2_mm>)
```

验收后，从全局 GLOB 排除该子树。这样不会要求一次性编辑 80 份 CMake。

### 阶段 C：依赖和产物可追溯

- toolchain manifest：编译器路径、版本、musl commit。
- artifact manifest：source commit、CMake cache 摘要、EFI/kernel/initramfs SHA-256。
- QEMU/VS Code 只消费 manifest 中的路径。

## 6. 迁移批次

### P0：立即封堵（建议首个源代码 PR）

- 删除 `part_vdisk.sh` 文件尾无条件调用。
- 删除全部 `eval` 和 `losetup -D`。
- 给物理同步加 mountpoint 与双确认。
- bootloader 安装不再吞错。
- map 与 mount 分离，统一 EXIT trap。

### P1：统一入口

- 引入根 CLI 和 `common.sh`。
- build root / artifacts root 统一。
- 现有函数名保留 alias。
- VS Code tasks 改为调用 `./make_install.sh qemu`。

### P2：可复现依赖

- musl out-of-tree build。
- loader 产物来源决策并写 hash。
- OVMF 路径探测与版本记录。
- `doctor`。

### P3：CMake 收敛

- target-scoped flags。
- generated 文件移出源码树。
- 按子系统替换 GLOB/空 CMake。

### P4：文档与 CI

- README 指向真实路径和 `time`/主开发分支策略。
- CI 至少执行 shell 语法、CMake configure、无特权 build。
- 危险脚本只做 dry-run 测试，绝不接入真实设备 runner。

## 7. 验收标准

- 无参数执行只显示帮助，不写磁盘。
- `build all` 不调用 sudo。
- 每个 destructive 命令在缺任一确认变量时非零退出。
- 脚本退出后不存在新增挂载、NBD 或 PID 文件。
- 移动仓库目录后无需编辑脚本。
- 新增含空格的工作区路径仍可构建。
- VS Code 与命令行使用同一 kernel/sh 路径。
- CMake 第二次 configure 不依赖源码树中的旧 generated 文件。
