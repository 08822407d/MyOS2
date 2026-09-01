# MYOS2-DR-004R · 调试桩与可观测性方案（004 复跑；原次连接器故障零交付）

```yaml
task_id: MYOS2-DR-004R
supersedes: MYOS2-DR-004（原次因 GitHub 连接器故障未取得任务书与源码，诚实零交付，未伪造——处置见 agent-workspace/results/MYOS2-DR-004/received/）
mode: 普通对话 Pro（GitHub 连接器）
priority: P0（第一波唯一缺口，且是 Owner 明示需求之一）
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-004R/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 复跑前须知（针对原次失败的加固）

原次故障：连接器把"读文件内容"请求错误路由为"读仓库元数据"，因而拿不到任何文件；该会话正确地选择零交付而非编造，此判断值得肯定。本次加固：

1. **开工自检**：先读 `agent-workspace/conventions.md` 并在回复中**逐字引用其 §1 第 2 条**（唯一可写区那句）。引用不出来就说明连接器又坏了——此时**立即停止**，报告"连接器读取失败"，不要继续。
2. **降级读法**：连接器读不到时，本仓库是 public 的，可直接用网页/浏览读取原始文件，例如
   `https://raw.githubusercontent.com/08822407d/MyOS2/master/agent-workspace/conventions.md`
   源码用 `https://raw.githubusercontent.com/08822407d/MyOS2/time/<path>`。任一通道成功即可开工，并在 MANIFEST 注明用了哪条通道。
3. **零交付优于假交付**：两条通道都失败时，照原次做法诚实报告，不猜内容。

## 背景

Owner 自述"野路子做内核"，运行中经常触发 bug 但定位困难，积累了不少悬案。现状：`mykernel/debug/` 仅 1 个文件 213 行；`lock_IPC/lockdep/` 目录存在但据 MYOS2-DR-007 抽样"实际上只有宏定义与头文件包含"（请自行核实）；printk 已有。Owner 要的是**基本调试桩**——让已实现部分自证清白、出错时快速缩小范围，不是完整 tracing 框架。

## 本次新增的现成输入（第一波已回收，务必消费）

- **`agent-workspace/results/MYOS2-DR-008/02-correctness-suspects.md`**：技术债台账已列出正确性嫌疑条目，并**专门为本任务备好了"建议给 MYOS2-DR-004 的调试桩"表**（按桩类别列出首批覆盖的 DR008-NNN 条目与最小观测量），以及来自 `bugs_record.md` 的历史 S1 线索（fork 用户栈内容错误、根盘 I/O 清了 IF、init.bin 之后 HPET 停摆导致不再切换任务、APIC ID 不连续）。**你的方案必须逐条回应该表**：每个桩类别对应到具体落点与代码。
- `agent-workspace/results/MYOS2-DR-003/init-sequence.md`：真实启动初始化时序——启动阶段检查点桩应据此设点。
- `agent-workspace/results/MYOS2-DR-002/completeness.yaml`：各子系统成熟度与风险点，用于判断哪些地方最需要自检。
- `agent-workspace/results/MYOS2-DR-001/`：外围脚本审计与 proposed/ 脚本（调试脚本改进须与之衔接，勿与其提案冲突）。

## 目标

1. **诊断现状**：现有 debug/printk/lockdep 提供什么；崩溃时 Owner 现在能看到什么（有无寄存器转储、栈回溯、panic 信息质量）。逐条给源码证据。
2. **调试桩体系设计**（按投入产出排序，给出"前十件快赢"清单）。候选面至少覆盖：
   - 启动阶段检查点（走到哪一步挂的；与 003 的 init 时序对齐）
   - 断言框架（BUG_ON/WARN_ON 等价物＋编译开关）
   - 内存类：页分配器一致性自检、kmalloc 毒化值、越界金丝雀、双重释放检测
   - 链表/IDR 等基础结构损坏检测（lib.list 是全内核地基）
   - 锁类：lockdep 完成度评估与最小可用集、中断上下文违规检测、IF 状态成对断言（直击 DR008-060 类历史 bug）
   - 调度/时钟活性观测：jiffies 前进、上下文切换计数、HPET/LAPIC 中断计数（直击 DR008-061）
   - panic 路径强化：寄存器＋栈回溯＋最近检查点回放
   - 串口日志规范（等级、子系统前缀、时间戳）
   - QEMU/GDB 辅助：gdb 脚本（打印 task 列表、遍历页表）、QEMU monitor 用法
3. **具体实现件**：每个"快赢"给可编译代码（优先新增头/源文件；对现有文件的改动用 unified diff），放 `proposed/` 并标明目标路径。
4. **回溯对照**：针对 008 台账中至少四条历史 S1 线索，说明"若当时有本方案的哪个桩，如何更快定位"。

## 交付物（放入 write_zone）

`MANIFEST.md`（必交，含 base_snapshot 与所用读取通道）＋`01-current-observability.md`＋`02-instrumentation-plan.md`（体系设计＋前十件快赢：做什么/放哪里/预计代码量/能抓住哪类 bug/对应 DR008 条目）＋`03-debt-crosswalk.md`（对 008 建议表的逐条回应）＋`proposed/…`。

## 边界

不直接改内核源码；尊重现有代码风格（先读几个文件感受）；不引入外部依赖；编译开关粗粒度 `CONFIG_DEBUG_*` 即可，须与未来 menuconfig 裁剪兼容（参考 003 的 trim-analysis 对 feature 边界的分析）。

## 验收判据

开工自检通过（逐字引用公约 §1.2）；前十件快赢每件有具体落点与代码；对 008 建议表七个桩类别逐条回应；至少四条历史 S1 线索有"当时若有此桩"的对照说明；base_snapshot 如实申报（内核源码应为 `time` 分支，完整 commit 必须复制粘贴、不得凭记忆书写）。
