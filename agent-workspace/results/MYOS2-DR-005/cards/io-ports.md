---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/insns/io_arch.h"
  - "mykernel/arch/x86_64/kernel/rtc.c"
  - "mykernel/arch/x86_64/kernel/myos_i8259.c"
  - "mykernel/arch/x86_64/pci/direct.c"
  - "mykernel/arch/x86_64/myos/keyboard.c"
  - "mykernel/arch/x86_64/myos/ide.c"
  - "mykernel/arch/x86_64/include/obsolete/ide.h"
  - "mykernel/arch/x86_64/myos/pre_init.c"
  - "mykernel/arch/x86_64/processor/processor_types_arch.h"
status: final
open_questions:
  - "legacy ATA 控制寄存器的 exact macro 展开值应由预处理输出纳入后续自动文档。"
  - "PCI CF8/CFC 并发访问锁被注释后，在多 CPU/中断环境下的实际竞态尚未复现。"
---

# 速查卡：端口 I/O 与 legacy 设备

## 1. IN/OUT 指令语义

x86 port I/O 使用独立 16 位 I/O address space：

- `inb/inw/inl` 从 port 读 8/16/32 位；
- `outb/outw/outl` 向 port 写；
- `ins*/outs*` 结合 `(E)SI/(E)DI`、`RCX` 和 `DF` 批量搬运；
- 执行权限由 CPL、RFLAGS.IOPL 和当前 TSS I/O bitmap 共同决定；
- 端口 I/O 通常具有比普通内存更强的设备顺序语义，但不能代替所有 MMIO memory barrier。

[VERIFIED mykernel/arch/x86_64/insns/io_arch.h] [EXTERNAL Intel SDM, Vol. 2, IN/OUT/INS/OUTS instruction reference]

MyOS2 的 TSS 包含 I/O bitmap 存储，但初始化/任务切换被注释；因此没有证据表明用户任务可被按端口授权。[VERIFIED mykernel/arch/x86_64/processor/processor_types_arch.h] [VERIFIED mykernel/arch/x86_64/kernel/cpu/common.c]

## 2. 活动端口清单

| 设备/机制 | 端口 | MyOS2 源码 | 状态/注意 |
|---|---|---|---|
| CMOS/RTC index | `0x70` | `rtc_cmos_read/write` | 写 bit7 还涉及 NMI mask，调用层需避免误改 |
| CMOS/RTC data | `0x71` | `rtc_cmos_read/write` | 读 calendar 寄存器 |
| 8259 master command | `0x20` | `init_i8259/i8259_eoi` | 初始化后全部 mask |
| 8259 master data/mask | `0x21` | 同上 | ICW/OCW |
| 8259 slave command | `0xA0` | 同上 | 级联 PIC |
| 8259 slave data/mask | `0xA1` | 同上 | ICW/OCW |
| PS/2 data | `0x60` | keyboard init/handler | handler 直接 `inb(0x60)` |
| PS/2 status/command | `0x64` | keyboard controller setup | 写命令前 busy wait |
| PCI CONFIG_ADDRESS | `0xCF8` | `pci_conf1_read/write` | mechanism #1 |
| PCI CONFIG_DATA | `0xCFC–0xCFF` | 同上 | 按 reg 低位选择 8/16/32 位 |
| ATA primary data/task-file | 通常 `0x1F0–0x1F7` | `IDE_PIO_*` 宏、`insw/outsw` | legacy PIO |
| ATA primary control | 通常 `0x3F6` | controller 宏 | alternate status/device control |
| ATA secondary data/task-file | 通常 `0x170–0x177` | controller 宏 | legacy PIO |
| ATA secondary control | 通常 `0x376` | controller 宏 | legacy PIO |
| PIT channel 0–2/control | `0x40–0x43` | 仅注释/保留接口 | `stub` |

[VERIFIED mykernel/arch/x86_64/kernel/rtc.c] [VERIFIED mykernel/arch/x86_64/kernel/myos_i8259.c] [VERIFIED mykernel/arch/x86_64/pci/direct.c] [VERIFIED mykernel/arch/x86_64/myos/keyboard.c] [VERIFIED mykernel/arch/x86_64/myos/ide.c] [EXTERNAL PCI Local Bus Configuration Mechanism #1] [EXTERNAL ATA legacy task-file I/O map]

“通常”标注的 ATA 地址来自标准兼容布局；MyOS2 代码通过 `IDE_PIO_*` 宏选择 controller，后续生成式文档应读取宏展开而不是手工复制地址。[VERIFIED mykernel/arch/x86_64/include/obsolete/ide.h] [INFERRED]

## 3. PCI 配置机制 #1

`CONFIG_ADDRESS` 32 位格式：

| 位 | 字段 |
|---:|---|
| 31 | enable |
| 30:24 | reserved |
| 23:16 | bus |
| 15:11 | device |
| 10:8 | function |
| 7:2 | register number（DWORD 对齐） |
| 1:0 | 0 |

MyOS2 宏还把 `reg[11:8]` 放入高位，以尝试访问 4 KiB extended config；但传统 CF8/CFC 机制对扩展配置空间的支持依平台，现代通用方案是 PCIe ECAM/MCFG。[VERIFIED mykernel/arch/x86_64/pci/direct.c] [EXTERNAL PCI/PCIe configuration-space specifications]

当前读写流程：

```text
outl(address, 0xCF8)
in/out at 0xCFC + byte offset
```

源码中的 `pci_config_lock` 获取/释放被注释。由于 CF8 是全局 selector，两个 CPU 的 selector/data 操作可交叉，读写错误 BDF/register。[VERIFIED mykernel/arch/x86_64/pci/direct.c] [INFERRED]

## 4. 8259A

初始化需要 ICW1–4：

1. master/slave 进入初始化；
2. 设置 vector base；
3. 声明级联关系（slave 接 master IRQ2）；
4. 设置 8086 模式；
5. 写 OCW1 mask。

MyOS2 完成上述重编程后把两片 mask 全置 1，随后主要使用 APIC/IOAPIC；仍保留 mask/unmask/EOI 函数。[VERIFIED mykernel/arch/x86_64/kernel/myos_i8259.c]

EOI 顺序：slave IRQ 通常先向 slave 发 EOI，再向 master 发 EOI。当前 `i8259_eoi()` 先 master 后 slave，若该路径重新成为活动控制器，需对照 8259 级联协议修正/测试。[VERIFIED mykernel/arch/x86_64/kernel/myos_i8259.c] [EXTERNAL Intel 8259A-compatible PIC protocol]

## 5. PS/2 controller

典型状态位（读 `0x64`）：

| 位 | 含义 |
|---:|---|
| 0 | output buffer full |
| 1 | input buffer full |
| 2 | system flag |
| 3 | command/data |
| 4 | keyboard inhibit/implementation-specific |
| 5 | auxiliary data |
| 6 | timeout error |
| 7 | parity error |

写 command port 前等待 input buffer clear；读 data port 前等待 output buffer full。MyOS2 使用 wait helper 初始化 controller，IRQ handler 从 0x60 取扫描码并推入 buffer。[VERIFIED mykernel/arch/x86_64/myos/keyboard.c] [EXTERNAL i8042-compatible controller documentation]

风险：

- wait helper 是否有超时需测试；
- IRQ handler 里屏幕调试输出会显著拉长中断；
- parser thread 创建被注释，完成量/缓冲消费语义需验证；
- 多字节 E0/E1 扫描码要防中途丢字节。

## 6. ATA PIO task-file

常用寄存器（base+offset）：

| offset | read | write |
|---:|---|---|
| 0 | data | data |
| 1 | error | features |
| 2 | sector count | sector count |
| 3 | LBA low | LBA low |
| 4 | LBA mid | LBA mid |
| 5 | LBA high | LBA high |
| 6 | device/head | device/head |
| 7 | status | command |

常见 status bits：

| 位 | 名称 | 含义 |
|---:|---|---|
| 0 | ERR | error |
| 3 | DRQ | data request |
| 5 | DF | device fault |
| 6 | DRDY | ready |
| 7 | BSY | busy |

MyOS2 实装 LBA28、部分 LBA48、IDENTIFY、PIO `insw/outsw` 和 IRQ completion。[VERIFIED mykernel/arch/x86_64/myos/ide.c]

确定风险：

1. 多个 `while (BSY/DRQ)` 无超时，设备缺失/故障可永久卡 CPU；
2. error/status 的读序、400 ns delay、flush-cache 等协议未完整体现；
3. request 锁与 IRQ 完成需在 SMP 下验证；
4. 现代 SATA 控制器若工作在 AHCI 模式，不提供 legacy task-file 兼容端口。

## 7. CMOS/RTC

`0x70` 的低 7 位选择 CMOS index，最高位通常控制 NMI mask；写 index 时若不保留预期 NMI 状态，可能意外屏蔽/开启 NMI。[EXTERNAL MC146818-compatible RTC]

读取时间需：

1. 检查 update-in-progress；
2. 获取一致快照；
3. 按 Status B 判断 BCD/binary、12/24 小时；
4. 处理 century/年份策略。

MyOS2 的 `rtc.c` 调用通用 `mc146818_get_time()`，本卡不假定这些步骤全部在该文件内可见。[VERIFIED mykernel/arch/x86_64/kernel/rtc.c]

## 8. 相邻可用机制

- PCIe ECAM（ACPI MCFG）替代 CF8/CFC；
- AHCI/NVMe MMIO 队列替代 legacy ATA PIO；
- MSI/MSI-X 替代固定 IOAPIC pin；
- 任务级 TSS I/O bitmap 与 `ioperm`/`iopl` 风格接口；
- 端口资源 reservation，阻止驱动冲突；
- 超时统一 helper、设备 reset 与 error recovery；
- QEMU `isa-debug-exit`/debugcon 可作为测试专用端口，但不应硬编码到生产路径。

## 9. 最小测试

| 测试 | 判据 |
|---|---|
| 双 CPU 并发 PCI config read | BDF/register 不串线 |
| 不存在的 PS/2/ATA 设备 | 超时返回，不永久忙等 |
| ATA 注入 ERR/DF | request 正确失败并唤醒等待者 |
| 8259 slave IRQ | EOI 顺序和 ISR 位正确 |
| 用户执行 IN/OUT | 无授权时稳定 #GP |
| RTC 跨秒读取 | 不产生撕裂时间 |
| CF8/CFC 与 ECAM 对照 | 同一设备 header 一致 |
