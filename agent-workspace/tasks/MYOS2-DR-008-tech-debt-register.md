# MYOS2-DR-008 · 技术债与已知问题台账

```yaml
task_id: MYOS2-DR-008
mode: 普通对话 Pro（GitHub 连接器）
priority: P2
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-008/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

多年个人项目积累了散落各处的债务线索：源码中的 TODO/FIXME/XXX/hack 注释、根目录 bugs_record.md 与 todo.txt、include/obsolete/ 废弃头目录、.bak 后缀文件、注释掉的大段代码。把它们收进一本台账，后续任何工作（学习路线、调试桩、重构）都能查询"这里有没有已知的坑"。

## 目标

1. **收割**：全仓扫描 TODO/FIXME/XXX/HACK/WORKAROUND/临时注释掉的代码块/魔法数字注记；合并 bugs_record.md、todo.txt、changelog.md 中仍可能有效的条目；盘点 include/obsolete/ 与 .bak 文件的内容与被引用情况。
2. **分类分级**：每条：类别（正确性隐患/未完成功能/废弃物/文档欠账/风格）、疑似影响子系统（词汇表 ID）、严重度（S1 会导致运行错误 / S2 阻碍开发 / S3 观感）、状态（open/可能已修/无法判定）。
3. **交叉线索**：正确性隐患类条目单独汇总成表，供 MYOS2-DR-004（调试桩）与 MYOS2-DR-006（回归用例）取材。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交）
- `debt-register.yaml`（行式台账：id/quote/path/line/category/subsystem/severity/status/notes）
- `01-summary.md`（统计与热点图：债务在哪些子系统扎堆）
- `02-correctness-suspects.md`（正确性隐患专表）

## 边界

只登记不修复；历史文件中的条目标注"历史材料，现状未核"；无法定位到源码的传闻性条目单独分区存放。

## 验收判据

台账每条可回溯（路径＋行号或历史文件出处）；统计与明细一致；正确性专表按严重度排序。
