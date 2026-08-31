# MYOS2-DR-001 · 外围工程审计：构建系统与自动化脚本

```yaml
task_id: MYOS2-DR-001
mode: 普通对话 Pro（GitHub 连接器）
priority: P0
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-001/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

MyOS2 的构建/调试/打包脚本是多年随手积累的：根目录 make_install.sh 总装，scripts/ 下函数式子脚本，CMake 三套（mykernel/myloader/myinitramfs），调试靠 dbg-vmw.sh 与已弃用的 dbg-qemu.sh.bak。已知问题样例：硬编码物理盘和 $HOME 路径、全量 `rm -rf build/*` 重建、GLOB_RECURSE 收源码无模块开关。Owner 希望整理和优化这套外围工程。

## 目标

1. **现状盘点**：逐一说明每个脚本/每份 CMakeLists 做什么、依赖什么环境（工具链、固件文件、盘路径）、彼此调用关系。
2. **问题清单**：脆弱点（硬编码、非幂等、误删风险、机器绑定）、缺失能力（增量构建、一键 QEMU 调试、环境自检）。按危害排序。
3. **目标结构提案**：给出整理后的脚本/CMake 组织方案（如统一 tools/ 或维持现布局的最小改法），含迁移步骤；兼顾未来 menuconfig 式裁剪对构建系统的要求（与 MYOS2-DR-003 的依赖图衔接，此处只需预留接口不需实现）。
4. **可直接采用的改进件**：重写版脚本/CMake 片段，作为**新文件**放入你的 results 目录（如 `proposed/` 子目录镜像目标路径），不得直接改原件。

## 建议步骤

读 make_install.sh → scripts/*.sh → mykernel/scripts/*.cmake → 三份顶层 CMakeLists → user-guide/Build_Kernel.md 与 Installation.md（对照文档与实物的偏差也是发现）。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交，格式见公约 §1.5）
- `01-inventory.md` 现状盘点（含调用关系图，mermaid 或 DOT）
- `02-problems.md` 问题清单（排序＋危害说明）
- `03-target-structure.md` 目标结构与迁移步骤
- `proposed/…` 改进后的脚本/CMake 新文件（每份文件头注释标明目标路径与替代对象）

## 边界

- 不改动 write_zone 之外任何文件；不建议推倒重来（Owner 要渐进整理）；物理盘安装路径这类机器特定配置应提为配置文件/环境变量而非删除。

## 验收判据

盘点覆盖根目录与 scripts/ 全部 .sh 及全部 CMakeLists；每个问题给出复现/触发场景；proposed/ 中脚本可被 Owner 直接试用（自带用法注释）。
