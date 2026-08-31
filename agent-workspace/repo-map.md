# MyOS2 仓库地图（预编译定向材料）

```yaml
task_id: bootstrap（非任务产出，由搭建方 claude-fable-5 本地扫描生成）
date: 2026-09-01
base_snapshot: time @ a039d980（本地最新开发版检出；master=63f0785c 是其祖先，落后 44 提交）
method: 对本地 time 分支检出做目录树扫描＋逐目录源码行数统计＋关键脚本抽读；未逐行核对源码——各条目默认为 [INFERRED]（目录名→内容推断），标注 [VERIFIED] 的除外
```

## §1 一句话背景

模仿 Linux 的学习/实验内核：C＋汇编，CMake 构建，UEFI 引导，x86_64 为主（aarch64 目录存在但规模未知），QEMU/VMware 调试，可安装到物理 NVMe 分区 [VERIFIED make_install.sh]。与 Linux 平铺式 kernel/ 不同，子系统按目录分类放置。

## §2 顶层布局

| 路径 | 内容 | 规模（.c/.h/.S 行数） |
|---|---|---|
| mykernel/ | 内核本体（下表细分） | ~150k |
| myloader/ | UEFI 引导器（含 ElfLib、OVMF 固件文件） | 小 |
| myinitramfs/ | 用户态：myinit、myshell、mylib（自制 libc：myglibc/csu/util） | 中 |
| scripts/ | 安装/建盘/环境脚本（make_install_kernel/initranfs/bootloader、map_vdisk、part_vdisk、phys_nvme_install、prepare_env、util_cmds） | — |
| documents/ | linux_src_notes（mm、vfs 笔记）＋related_docs（ATA 手册） | 历史材料 |
| user-guide/ | Build_Kernel、Installation、User-Application、Syscall-Table、kernel_flags | 入口文档 |
| 根目录 | make_install.sh（总装脚本）、dbg-vmw.sh、dbg-qemu.sh.bak、bugs_record.md、todo.txt、changelog.md | — |

## §3 mykernel/ 子系统细分（行数含头文件，time @ a039d980 快照实测；master 上缺 cpu/ 等新目录）

| 目录 | 行数 | 推断内容与备注 |
|---|---|---|
| arch/ | 33399 | x86_64 为主（含 kbuild/ 目录参与构建预处理 [VERIFIED CMakeLists]）；aarch64/ 存在 |
| include/ | 48651 | 全局头文件：linux/（仿 Linux 头）、klib/、uapi/、obsolete/（废弃头，技术债线索）、generated/ |
| mm/ | 13504 | early（启动期内存）、page_alloc、kmalloc、vm_map、fault、highmem、misc |
| lib/ | 11569 | digit、idr、list、printf、string |
| sched/ | 10063 | task、forkexec、runqueue、scheduler、misc |
| fs/ | 9732 | vfs＋fat（仅此二者，无 ext/proc/dev 文件系统目录） |
| drivers/ | 6507 | ata、base、char、pci、rtc |
| lock_IPC/ | 6444 | atomic、spinlock、lockdep（存在！）、semaphore、futex、signal |
| time/ | 4642 | systick、ktime、clocksource、timekeeping、timer、adjtime、misc |
| kactive/ | 2562 | softirq、workqueue、swait、completion（"kernel active"＝下半部/异步执行机制） |
| block/ | 2197 | 块层 |
| init/ | 637 | 内核初始化入口（637 行，3 文件——初始化序列分析的起点） |
| klib/ | 393 | 内核内部小工具库 |
| cpu/ | 370 | CPU 相关（与 arch 的分工待核） |
| debug/ | 213 | 仅 1 文件——调试设施薄弱 [VERIFIED 行数] |
| printk/ | 838 | 内核打印 |
| namespace/ | 170 | 命名空间（雏形） |
| entry/ | 74 | 系统调用/异常入口（薄，主体可能在 arch/） |
| device/ | 0 | **空壳目录**（CMake 引用但无源码 [VERIFIED 行数=0]） |
| scripts/ | — | CMake 辅助：options_flags.cmake、target_kbuild.cmake、target_kernel.cmake [VERIFIED] |

## §4 分支情况（重要）

**开发模式（Owner 口述，2026-09-01）**：两年来全部开发在本机进行；习惯是为每个机制/子系统/模块新建专门分支（但增改的代码并不限于该子系统的源码），基本达到要求后才合并回 master。因此**本机当前检出分支＝最新版本**。

当前状态：最新分支＝`time` @ a039d980（已同步 GitHub）；master（63f0785c）是它的祖先，落后 44 提交/324 文件（+13332/−5240），且没有 time 上新增的 mykernel/cpu/ 等内容 [VERIFIED git 实测]。**分析基线＝time 分支**，本地图即按 time 快照生成；agent-workspace/ 工作区只存在于 master。其余主题分支（libc、list、mmap、percpu、pid_ns、signal、slub、vma_list、workqueue、writeback）是既往工作轨迹，无需阅读。提交信息几乎全为 "backup"，**git 历史不可用作演进依据**。

## §5 构建与调试工作流实况（在 time 快照上抽读核实；master 版本或略有出入）

- 总装：根目录 make_install.sh → source scripts/ 下四个函数脚本 → `rm -rf build/*` 全量重建 → 内核/initramfs/引导器三件套 cmake 构建安装；含硬编码物理盘 by-id 路径（注释掉的 phys 安装）[VERIFIED]。
- 内核构建：mykernel/CMakeLists.txt 用 `file(GLOB_RECURSE)` 收全部 .c/.S 再剔除 kbuild 文件 [VERIFIED]——目录即模块，无编译开关粒度。
- 调试：dbg-vmw.sh 硬编码 $HOME/vmware 路径拉起 VMware；dbg-qemu.sh.bak（已弃用后缀）为 QEMU -s -S 等 gdb 接入模式，按机器名（zbook/asus）分支 [VERIFIED]。.vscode/.vscode-kdbg 存在于本地（master 快照未含），含 gdb 调试配置 [INFERRED]。

## §6 已知历史材料（可引用不可当现状）

bugs_record.md（历史 bug 记录）、todo.txt、changelog.md、documents/linux_src_notes/（Owner 读 Linux 源码的 mm/vfs 笔记——反映其学习轨迹）。
