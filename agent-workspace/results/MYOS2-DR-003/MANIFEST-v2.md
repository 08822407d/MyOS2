---
task_id: MYOS2-DR-003
produced_by: GPT-5.6 Pro
date: 2026-09-01
base_snapshot: workspace master @ c7e68edb3dd76f4e4808dff12b5ca2021035e719; kernel source time @ a039d9803ade2a1613d620bda375e028530d5242
inputs_read:
  - agent-workspace/conventions.md
  - agent-workspace/results/MYOS2-DR-003/MANIFEST.md
  - agent-workspace/results/MYOS2-DR-003/FINAL-RESPONSE.md
status: final
supersedes: MANIFEST.md
open_questions:
  - 当前正式发布/测试使用的 CMake 构建类型、ARCH 参数与根块设备选择宏是什么？
  - ROOTBLK_TRANSFER 在目标机器上最终绑定 ATA 还是 NVMe，是否存在稳定的替代根文件系统配置？
  - kernel_init 中手工 do_initcalls() 与链接器 initcall section 机制未来是否计划并存，还是前者为临时过渡？
  - workqueue_init()、sched_init_smp() 等被注释路径是有意禁用、尚未移植完成，还是由其他私有路径替代？
  - 目标平台是否保证 HPET；若不保证，PIT/LAPIC/TSC-deadline 的降级策略在哪里完成？
---

# MYOS2-DR-003 交付清单 v2

本文件仅更新目录清单，以纳入 Owner 后续要求生成的可下载回复文件；原有依赖分析结论、证据等级、基线和限制均未改变。旧 `MANIFEST.md` 保留以满足“不覆盖修订”规则，本文件作为当前目录索引使用。

| 文件 | 内容 | 状态 | 后续消费方式 |
|---|---|---|---|
| `MANIFEST-v2.md` | 当前目录的完整清单、覆盖状态与消费说明 | final | 首先阅读 |
| `MANIFEST.md` | 初始五份任务交付物的原始清单 | superseded for inventory only | 审计初始交付状态 |
| `deps.yaml` | 102 个节点、123 条 build/init_order/functional 边 | final / static | 机器合并、过滤和配置依赖生成 |
| `deps.dot` | 与 YAML 同源的 Graphviz 总图 | final / static | Graphviz 渲染与人工浏览 |
| `init-sequence.md` | 从 `phys_startup_64` 到 boot idle 与 `/boot/init` 的实际时序 | final / static | 教学、启动故障定位、初始化重构 |
| `trim-analysis.md` | 当前硬核、条件必需、可选叶与 menuconfig 粒度建议 | final / preliminary | 配置系统设计输入，不作直接删除清单 |
| `FINAL-RESPONSE.md` | 对话中任务完成回复的完整 Markdown 归档，可直接下载 | final | 阅读、转存或作为任务完成记录 |

## 覆盖状态

- 原任务书要求的五份交付物均已完成。
- 后续新增 `FINAL-RESPONSE.md`，完整保存任务完成回复。
- 所有文件仍位于 `agent-workspace/results/MYOS2-DR-003/`，未修改内核源码、任务书、公约或其他任务目录。
- 静态分析限制和 `open_questions` 与初始清单一致；未新增构建、链接、QEMU、实机或设备 I/O 验证。

## 建议消费顺序

1. 读本文件确认当前完整文件集合。
2. 读 `init-sequence.md` 建立实际执行基线。
3. 用 `deps.yaml` 与 `deps.dot` 做机器处理和总览。
4. 用 `trim-analysis.md` 设计 profile、choice 与 menuconfig 粒度。
5. 用 `FINAL-RESPONSE.md` 获取对话完成摘要及 PR 信息。
