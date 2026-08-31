# MYOS2-DR-004 · 调试桩与可观测性方案

```yaml
task_id: MYOS2-DR-004
mode: 普通对话 Pro（GitHub 连接器）
priority: P0
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-004/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

Owner 自述"野路子做内核"，运行中经常触发 bug 但定位困难，积累了不少悬案（历史线索见根目录 bugs_record.md，仅供参考）。现状：mykernel/debug/ 仅一个文件 213 行；lock_IPC/lockdep/ 目录存在（完成度未知）；printk 已有。Owner 要的是**基本的调试桩**——确保已实现部分自证清白、出错时能快速缩小范围，不是完整 tracing 框架。

## 目标

1. **诊断现状**：现有 debug/printk/lockdep 能提供什么；崩溃时 Owner 现在能看到什么（有无寄存器转储、栈回溯、panic 信息质量）。
2. **调试桩体系设计**（按投入产出排序，给出"前十件快赢"清单）。候选面至少覆盖：
   - 启动阶段检查点（boot stage checkpoint：走到哪一步挂的）
   - 断言框架（BUG_ON/WARN_ON 等价物＋编译开关）
   - 内存类：页分配器状态一致性自检、kmalloc 毒化值（poison）、越界金丝雀、双重释放检测
   - 链表/IDR 等基础结构的损坏检测（lib.list 是全内核地基）
   - 锁类：lockdep 的完成度评估与最小可用集、中断上下文违规检测
   - panic 路径强化：寄存器＋栈回溯＋最近检查点回放
   - 串口日志规范（等级、子系统前缀、时间戳）
   - QEMU/GDB 辅助：gdb 脚本（打印 task 列表、页表遍历）、QEMU monitor 用法
3. **具体实现件**：每个"快赢"项给出可编译的代码（新增头文件/源文件形式优先，对现有文件的改动用 unified diff），放 `proposed/` 子目录，标明目标路径。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交）
- `01-current-observability.md` 现状诊断
- `02-instrumentation-plan.md` 体系设计＋前十件快赢（每件：做什么/放哪里/预计代码量/能抓住哪类 bug）
- `proposed/…` 实现件

## 边界

不直接改内核源码；方案须尊重现有代码风格（先读几个文件感受）；不引入外部依赖；编译开关设计须与未来 menuconfig 裁剪兼容（粗粒度 CONFIG_DEBUG_* 即可）。

## 验收判据

前十件快赢每件都有具体落点与代码；针对 bugs_record.md 中至少两类历史 bug 说明"若当时有本方案的哪个桩，如何更快定位"。
