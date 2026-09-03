# 分析维度台账 v0.1（MYOS2-LEAD-001 · 阶段 0 交付件 2；只增不删）

```yaml
task_id: MYOS2-LEAD-001
track_id: MYOS2-LEAD-001
record_type: analysis_facets_register
version: 0.1
evidence_class: B
produced_by: claude-fable-5.1@claude-code-vscode（主导会话，UUID 3bfbb342-1b15-4461-a4c3-987f17aad103）
date: 2026-09-03
base_snapshot: 工作区 origin/master @ d231708c77e101dd38280ec0dc74e73d16a0446a；内核 time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read: [00-owner-work-order-verbatim.md §6, WAVE-1-REVIEW.md（提取件）, results/*/MANIFEST*.md（9 份）, tasks/（12 份）, WAVE-2-PLAN.md, repo-map.md, EXTRACTION-NOTE.md]
status: draft（v0.1；每次只增行或增列，改判定用新版本文件 supersedes）
open_questions:
  - 第 23~25 行是本会话新增的候选维度，是否采纳待 Owner（不答＝保留为候选）
  - "第一波覆盖"列中标 [REVIEW] 的判断来自 WAVE-1-REVIEW，未独立复核
```

## 0. 目的与口径（Owner 明令；不增不减）

①外围脚本整理优化（CMake/调试/打包安装）；②各子系统完成度＋"重要程度"图＋"依赖关系"图（含 menuconfig 式裁剪基础）；③基本调试桩快速定位 bug；④查表查手册类低价值工作由 agent 代劳（前提 agent 熟悉实现）；⑤未来以自制内核为蓝本建学习 agent 组。维度可超出五点（Owner 原话："不能仅限于它们，而是要考虑其他大型代码项目普遍关心的问题"），但每个维度必须注明服务于哪一条或标"通用关注"。

## 1. 台账

| # | 维度 | 服务目的 | 第一波覆盖（阶段 0 核对） | 现状与证据 | 下一步归属 |
|---|---|---|---|---|---|
| 1 | 构建系统与可复现性（CMake 结构、工具链钉住、产物链唯一性） | ① | 001（低可信、含编造 F1~F4）[REVIEW+VRF] | `mykernel/CMakeLists.txt` 用 GLOB_RECURSE、106 份 CMakeLists 中 70 份 0 字节 [REVIEW]；BOOTX64.EFI 双来源（预编译件 2024-04 19,200 B ＋ `myloader/uefi/` 源码）[VRF] | 001R；本地 L8；决策 2、3 |
| 2 | 配置管理（CONFIG 宏真相表、menuconfig 式裁剪可行性） | ② | 否——九份产出无一读 `options_flags.cmake`（003 的 inputs_read 列了它但结论未用）[REVIEW] | 生效宏 12 个；`-DROOTBLK_NVME` 被注释（`options_flags.cmake:71`）[VRF] | 本地 D（L2）；003R 的 config 边 |
| 3 | 子系统完成度矩阵 | ② | 002（53/53；medium；5 条证据路径不存在、4 处假 [VERIFIED]）[REVIEW] | implemented 5 / partial 43 / missing 5；成熟度 0~4 分布 5/13/30/5/0 [VRF MANIFEST] | 本地 C（L4）重锚；UP/SMP 分标（S8.5） |
| 4 | 依赖关系图与初始化序列 | ② | 003（结构好：102 节点/123 边；锚点坏 F5、rtc 误判 F6）[VRF] | 65 个行号引用多数失效 [VRF] | **003R（P0）** |
| 5 | 重要程度分级与学习路线 | ② | 007（53/53 T0~T3；tier 计数错 F13；三路线 A/B/C）[REVIEW+VRF] | 三套分级口径（002 risk_level / 007 tier / 008 severity）未统一 | 本地 L7；决策 7、12 |
| 6 | 调试桩、可观测性、断言/自检 | ③ | 否——004 零交付（诚实）；008 备有"给 004 的桩建议表" [VRF MANIFEST] | 现有 `mykernel/debug/` 仅 1 文件；打印走 framebuffer `color_printk`，无串口驱动 [VRF] | 004R（执行者待裁）；决策 5 |
| 7 | 正确性嫌疑与并发（自旋、唤醒链、调度状态机、lockdep-lite） | ③ | 008（32 条当前嫌疑，S1 28）；005 atomic-locks 卡；评审独立发现 3~6（两个 P0、唤醒链、msleep、永久自旋 32 处）[REVIEW] | `arch_atomic_add_test_negative` 汇编为 subl、`arch_spin_trylock` 无 CAS [REVIEW，子代理复核一致] | 主攻清单 1/4/6 Owner 亲手；011；本地 L5 |
| 8 | SMP bring-up 与 per-CPU 基础 | ③ | 否（005 apic 卡提及 AP trampoline 未闭环）[REVIEW] | `myos_APboot.S:124` 为 `jmp .` [VRF]；所有"可跑"只对单 BSP 成立 | 决策 11；S8.5 分标前提 |
| 9 | 内存安全与资源生命周期（分配器、页表、释放路径） | ③ | 008 部分 [REVIEW] | — | 通用关注；阶段 3 后立项 |
| 10 | 用户/内核边界与安全性（syscall 参数校验、权限） | ③ | 005 部分（syscall-context 卡：`sysretq` 无条件、IRQ ack 传常量 0、FS/GS/xstate 未完整保存）[VRF MANIFEST §4] | — | 通用关注；学习价值高；阶段 3 后立项 |
| 11 | 用户态运行时与自制 libc / initramfs | ② | 否（002 只判 user.initramfs 未闭环）[VRF MANIFEST] | `myinitramfs/CMakeLists.txt` 只 add_subdirectory(myinit)、(myshell)；mylib 下 17 份 CMakeLists 未接 [VRF] | 决策 1 |
| 12 | 测试与 CI（回归防线、QEMU 冒烟、契约测试） | 通用关注 | 006（低可信；含假 SHA F10；`run-qemu-smoke.sh` 实跑五场景通过）[REVIEW] | 无 `.github/`；无自动化测试 [REVIEW] | 010（已就绪）；本地 B、E（L6） |
| 13 | 查表资料包（cpuid、MSR、ACPI 表、中断向量…）绑定实际用到的位置 | ④ | 005（12 卡＋atlas；4 处错值 F7、假 [VERIFIED] F9）[REVIEW+VRF] | 34 条 OQ | **005R** |
| 14 | 可移植性/架构抽象（arch 层边界） | 通用关注 | 否 | `mykernel/arch/aarch64/` 仅 CMakeLists [VRF] | 决策 10 |
| 15 | 性能基线与测量方法 | 通用关注 | 否 | TSC 频率因 `x86_vendor` 恒 UNKNOWN 落到 3GHz 假值 [REVIEW，子代理复核一致]——测量前提不成立 | 与第 23 行合并处理后再建基线 |
| 16 | 文档与知识传承（教学蓝本、术语表、设计说明） | ⑤ | 009（零源码接触；无 MANIFEST 拆出，原件在 received/）[VRF] | `user-guide/` 五份入口文档存在 [VRF] | 本地 L10；阶段 5 |
| 17 | 依赖与许可卫生（musl、EDK2、第三方来源与许可证） | 通用关注 | 否 | 根目录 LICENSE 存在；`.vscode/c_cpp_properties.json` 引用 `~/projs/musl/build/include` [VRF] | 后续立项（公开仓尤需） |
| 18 | 发布/打包/部署脚本安全性（破坏性动作、干跑、参数校验、物理盘同步范围） | ① | 001 部分且判反（M-02）；评审 §7.4 实证三个危险脚本 [REVIEW] | `make_install.sh` 无参即 `rm -rf ./build/*`；`phys_nvme_install.sh` `rm -rf /mnt1/*`；`part_vdisk.sh` 末尾无条件 do_part [REVIEW] | 001R；决策 4；工作令 §4.5 禁令 |
| 19 | 技术债台账与已知问题 | 通用关注 | 008（69 条；行号越界 23 条 F12）[REVIEW] | 9 个 obsolete 头仍在活动依赖链 [VRF MANIFEST-v2] | 本地 C（L4） |
| 20 | 代码质量：编译警告、静态分析、UB、风格一致性 | 通用关注 | 否 | 被注释 return 2,244 处（未复核）[REVIEW] | 本地 L8（先开 `-Wreturn-type` 等零成本诊断） |
| 21 | 仓库与分支卫生（分支模型、历史主题分支去留、PR 残留） | 通用关注 | 部分（repo-map §4）[VRF] | 10 条历史主题分支；PR #1~#5 残留；Mnemosyne 遗留 worktree/分支 [VRF] | Owner 决定 |
| 22 | 存储栈与设备路径（ATA/NVMe、根盘策略） | ② | 部分（003 OQ、008 OQ-002、评审 4.2）[REVIEW] | 根块设备现为 ATA（`obsolete/device.h` #else 分支）[REVIEW，子代理复核一致] | 本地 L11；决策 9 |
| 23 | **新增候选**：时间与时钟源正确性（TSC 校准、HPET 假定、jiffies 推进、timer 到期→唤醒链） | ③ | 评审独立发现 5、8；005 timers 卡；003 核心结论 6 [REVIEW] | `timer.c` 到期路径 `// schedule()`；`msleep` 非零即死循环 [REVIEW，子代理复核一致] | 与主攻清单 5 合并（Owner 亲手，本地陪跑） |
| 24 | **新增候选**：引导链与镜像产物（UEFI loader → 内核 ELF → initramfs；GRUB2_BOOTUP_SUPPORT） | ①② | 002/003 部分（boot.uefi 节点；OQ）[VRF MANIFEST] | BOOTX64.EFI 与源码对应关系无哈希证明（008 OQ-004）[VRF] | 决策 2；003R |
| 25 | **新增候选**：分析产出自身的可信度治理（溯源、引文闸门、SHA 校验、降权清单） | 通用关注（元维度） | WAVE-1-REVIEW §3、§7.1；本轨道协议 v1 [VRF] | 第一波四份含编造 | 本地 A（引文闸门）；`tasks/00-gpt-task-protocol-v1.md` |

## 2. 使用说明

- 立项新任务时在本表加一列"任务号"或在"下一步归属"追加；不删行、不改既有判定（改判定出 v0.2 并 supersedes）。
- 未来 Meta-Agent 可把本表转成机器可读 YAML；行号即维度 ID（F-01…F-25），已在下游文件引用时保持不变。
