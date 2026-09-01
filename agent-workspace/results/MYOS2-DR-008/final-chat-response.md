## MYOS2-DR-008 已完成

已在 `08822407d/MyOS2` 创建并提交 **[PR #4：MYOS2-DR-008 技术债与已知问题台账](https://github.com/08822407d/MyOS2/pull/4)**。PR 当前为 `open`、非草稿、**可合并**，从 `agent/MYOS2-DR-008` 指向 `master`；共新增 6 个文件、1057 行，删除 0 行。fileciteturn154file0L2-L16 fileciteturn154file0L28-L35

- **源码分析快照：** `time@a039d9803ade2a1613d620bda375e028530d5242`
- **建分支基点：** `master@c7e68edb3dd76f4e4808dff12b5ca2021035e719`
- **交付分支头：** `053a271cde9d5538f63e09beb07eabc012614496`
- **实际变更范围：** 六个文件全部位于 `agent-workspace/results/MYOS2-DR-008/`，未修改其他目录。

## 权威交付物

1. **[debt-register.yaml](https://github.com/08822407d/MyOS2/blob/053a271cde9d5538f63e09beb07eabc012614496/agent-workspace/results/MYOS2-DR-008/debt-register.yaml)**  
   69 条机器可读行式台账，每条均包含任务书要求的 `id/quote/path/line/category/subsystem/severity/status/notes`。

2. **[01-summary-v2.md](https://github.com/08822407d/MyOS2/blob/053a271cde9d5538f63e09beb07eabc012614496/agent-workspace/results/MYOS2-DR-008/01-summary-v2.md)**  
   统计、热点图、问题簇、废弃物盘点、历史材料和治理顺序。热点图已确认逐行正确。fileciteturn143file0L1-L7

3. **[02-correctness-suspects.md](https://github.com/08822407d/MyOS2/blob/053a271cde9d5538f63e09beb07eabc012614496/agent-workspace/results/MYOS2-DR-008/02-correctness-suspects.md)**  
   32 条当前正确性嫌疑按 S1→S2 排列，另列 4 条历史正确性线索，并提供 MYOS2-DR-004 调试桩和 MYOS2-DR-006 回归用例入口。

4. **[MANIFEST-v2.md](https://github.com/08822407d/MyOS2/blob/053a271cde9d5538f63e09beb07eabc012614496/agent-workspace/results/MYOS2-DR-008/MANIFEST-v2.md)**  
   权威交付清单、覆盖状态、限制、开放问题和后续消费约定。fileciteturn144file0L1-L7

首版 `01-summary.md` 在连接器写入时出现一处热点图换行合并；根据公约不能覆盖既有结论，因此新增 `01-summary-v2.md`。首版 `MANIFEST.md` 同样由 `MANIFEST-v2.md` 取代，两份首版文件仅为保留的历史版本。

## 台账结果

共登记 **69 条**：

| 维度 | 统计 |
|---|---|
| 来源 | 当前快照 60；历史材料且现状未核 9 |
| 类别 | 正确性隐患 36；未完成功能 15；废弃物 10；文档欠账 3；风格 5 |
| 严重度 | S1 31；S2 36；S3 2 |
| 状态 | `open` 60；`无法判定` 9；`可能已修` 0 |
| 当前正确性嫌疑 | 32，其中 S1 28、S2 4 |

最高优先级问题集中在：

- futex、未实现系统调用和页错误路径中的永久自旋；
- `part_core.c`、`part_efi.c` 中非 `void` 跌落、未初始化指针及 GPT 半移植路径；
- `try_to_wake_up()`、`enqueue_task()`、`dequeue_task()`、`set_task_cpu()` 和远端重调度 IPI 的队列不变量缺口；
- `filemap_page_mkwrite()` 缺失返回及页对齐末页漏读/漏写；
- `dup_task_struct()` 的未初始化 `tsk`、退出生命周期清理缺失；
- IRQ 控制器始终收到 `ack(0)`；
- 会重建 GPT、格式化分区或清空挂载目录的部署脚本缺少安全护栏。

另外逐文件盘点了 **1 个 `.bak` 文件和 9 个 `obsolete` 头文件**。9 个 `obsolete` 头仍被活动源码直接或传递依赖，因此未建议直接删除。

## 校验与边界

- PR 的六个变更路径已逐项核对，全部在唯一许可目录内。
- `debt-register.yaml` 已解析为 69 条记录，并验证字段和值域。
- 台账、摘要修订版和正确性专表的本地校验版本与 GitHub blob SHA 完全一致。
- PR 最终复核结果为 `mergeable: true`。fileciteturn154file0L2-L16
- 未执行完整编译、最终链接映射、QEMU/VMware/物理机启动或运行时故障注入；这些未验证事项均已写入各文件的 `open_questions`，没有把静态嫌疑表述成已复现故障。
