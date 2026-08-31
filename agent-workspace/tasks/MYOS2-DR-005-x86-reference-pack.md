# MYOS2-DR-005 · x86 查表资料包（绑定源码位置的速查卡）

```yaml
task_id: MYOS2-DR-005
mode: 普通对话 Pro（GitHub 连接器；可开浏览核对官方手册）
priority: P1
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-005/
prerequisites: 先读 agent-workspace/conventions.md 与 agent-workspace/repo-map.md
```

## 背景

Owner 认为纯查表查手册（SDM/ABI 文档）耗时耗力且对自身理论水平增值低，这类工作应由 agent 代劳。但代劳的前提是资料**贴着 MyOS2 的实现给**——通用手册摘抄没有价值，要的是"你的代码在 mykernel/arch/x86_64/xxx.c 用到了这个位域，它的完整定义和相邻可用项如下"。

## 目标

1. **用途普查**：扫 mykernel/arch/x86_64/（含 kbuild/）与 cpu/、entry/，列出代码实际触碰的 x86 机制清单：cpuid 叶、MSR、控制寄存器位、页表项格式、GDT/IDT/TSS、APIC（本地/IO）、时钟源（TSC/HPET/PIT）、端口 IO 设备等，逐项带源码位置 `[VERIFIED]`。
2. **速查卡**：对普查中的每个主题做一张卡：
   - 代码当前用到的位/叶/寄存器的**完整**定义表 `[EXTERNAL SDM 卷章]`
   - 同主题下代码**尚未用到但邻近可用**的项（如已查 cpuid 0x1 则附 0x7 的扩展特性表）——这是 Owner 未来扩展时的现成弹药
   - 已知陷阱（如某 MSR 需先 cpuid 探测、TSC 不变性判定）
3. **cpuid 全景表**：单独一份 cpuid 叶位域大表（0x0~0x1F 常用叶＋扩展叶 0x80000000~），标注 MyOS2 已用/未用。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交）
- `00-usage-census.md` 用途普查（机制→源码位置清单）
- `cards/<主题>.md` 速查卡（paging、msr、cpuid、apic、interrupts-exceptions、timers、segmentation、io-ports…按普查结果定）
- `cpuid-atlas.md` cpuid 全景表

## 边界

只做 x86_64（aarch64 另案）；资料以 Intel SDM 口径为准、AMD 差异单独标注；不确定的位域宁缺勿错——错误的手册数据比没有更糟，无把握就标注"待核对 SDM 卷X章Y"。

## 验收判据

普查清单每项有源码路径；每张卡明确区分"代码已用"与"邻近可用"；无凭记忆杜撰的位域（抽查可对上手册）。
