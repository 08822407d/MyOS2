---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/init/main.c"
  - "mykernel/arch/x86_64/kernel/time.c"
  - "mykernel/arch/x86_64/kernel/hpet.c"
  - "mykernel/arch/x86_64/kernel/tsc.c"
  - "mykernel/arch/x86_64/kernel/kvmclock.c"
  - "mykernel/arch/x86_64/kernel/rtc.c"
  - "mykernel/arch/x86_64/myos/LVT_timer.c"
  - "mykernel/time/clocksource/clocksource.c"
  - "mykernel/time/timekeeping/timekeeping.c"
  - "mykernel/time/systick/systick.c"
status: final
open_questions:
  - "HPET main-counter 到纳秒的 mult/shift 计算需用已知 period_fs 向量做单元测试。"
  - "HPET 中断路由固定值与目标芯片组/虚拟机 IOAPIC pin 的对应关系需从 ACPI/HPET 表验证。"
  - "LAPIC timer 的标称频率和 divisor 当前是否经过校准，需运行时测量。"
---

# 速查卡：TSC、HPET、LAPIC Timer、PIT、RTC 与 clocksource

## 1. 当前角色图

| 机制 | 产生中断 | 提供连续 cycle | 进入 timekeeper | 进入 sched_clock | MyOS2 状态 |
|---|---:|---:|---:|---:|---|
| jiffies | 间接由 HPET tick 推进 | 以 jiffies 计数 | **是：启动默认源** | 否 | active |
| HPET main counter | timer0 可周期中断 | 是 | 注册候选，但当前选择函数未通知 timekeeper | 否 | partial |
| TSC | 本身不产生普通 tick | 是 | 未注册且被标 unstable | **是** | partial |
| LAPIC timer | 是，每 CPU | current count | 否 | 否 | partial/debug |
| PIT/i8253 | 可 | channel counter | 否 | 否 | stub/commented |
| RTC/CMOS | 可有周期/闹钟，但当前只读时钟 | 否 | 只初始化 wall time | 否 | active boot source |
| KVM clock | 可提供 pvclock | 是 | 注册代码被注释 | sched_clock 代码被注释 | stub |

[VERIFIED mykernel/time/timekeeping/timekeeping.c] [VERIFIED mykernel/arch/x86_64/kernel/hpet.c] [VERIFIED mykernel/arch/x86_64/kernel/tsc.c] [VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c]

## 2. 实际启动顺序

```text
timekeeping_init()
  ├─ read_persistent_clock64()       # CMOS wall clock
  ├─ clocksource_default_clock()     # jiffies
  └─ tk_setup_internals(jiffies)

late_time_init()
  ├─ hpet_time_init()
  │   ├─ HPET 映射/周期 timer0/IRQ
  │   └─ clocksource_register_hz(hpet)
  └─ tsc_init()
      ├─ tsc_early_init()/频率比例
      ├─ sched_clock_register(native_sched_clock)
      └─ mark_tsc_unstable()
```

[VERIFIED mykernel/init/main.c] [VERIFIED mykernel/arch/x86_64/kernel/time.c] [VERIFIED mykernel/time/timekeeping/timekeeping.c]

`clocksource_register_hz()` 将 HPET 插入 rating 排序列表，但 `__clocksource_select()` 中调用 `timekeeping_notify()` 和更新当前源的代码被注释。因此“HPET 注册成功”不等于 timekeeper 已切换；当前可证明的连续时基仍是 jiffies，jiffies 由 HPET timer0 中断推进。[VERIFIED mykernel/time/clocksource/clocksource.c] [VERIFIED mykernel/arch/x86_64/kernel/hpet.c]

## 3. TSC

### 3.1 CPUID.15H

| 寄存器 | 字段 |
|---|---|
| EAX | denominator |
| EBX | numerator |
| ECX | nominal crystal/reference frequency in Hz（若枚举） |
| EDX | reserved |

当 EAX、EBX、ECX 均非零：

```text
TSC_frequency_hz = ECX * EBX / EAX
```

[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.15H]

### 3.2 CPUID.16H

| 寄存器 | 15:0 |
|---|---|
| EAX | processor base frequency, MHz |
| EBX | maximum frequency, MHz |
| ECX | bus/reference frequency, MHz |

这些值是标称信息，不保证等于瞬时核心频率，也不直接证明 TSC 恒定。[EXTERNAL Intel SDM 253665-092US, Vol. 1, CPUID.16H]

### 3.3 invariant TSC

`CPUID.80000007H:EDX[8]` 通常表示 invariant TSC：TSC 以恒定速率运行，与 P/C-state 频率变化解耦；是否在深 idle/平台迁移/虚拟化中保持同步还需平台与 hypervisor 保证。[EXTERNAL Intel SDM 253665-092US, Vol. 1, extended CPUID tables]

### 3.4 MyOS2 当前行为

- 优先尝试 CPUID/MSR 信息，失败回退固定 `3 GHz`；
- 建立 per-CPU cycle→ns 比例；
- 注册 `native_sched_clock()`；
- 没有把 TSC 注册为 clocksource；
- 最终调用 unstable 标记；
- leaf 0x15 的字段解释/量纲与 SDM 公式不一致，正常平台也可能落回 3 GHz。

[VERIFIED mykernel/arch/x86_64/kernel/tsc.c]

## 4. HPET register map

| offset | 寄存器 |
|---:|---|
| `0x000` | General Capabilities and ID |
| `0x010` | General Configuration |
| `0x020` | General Interrupt Status |
| `0x0F0` | Main Counter Value |
| `0x100 + 0x20N` | Timer N Configuration/Capabilities |
| `0x108 + 0x20N` | Timer N Comparator |
| `0x110 + 0x20N` | Timer N FSB Interrupt Route |

[EXTERNAL IA-PC HPET Specification Rev. 1.0a]

### 4.1 General Capabilities/ID

| 位 | 含义 |
|---:|---|
| 7:0 | revision ID |
| 12:8 | number of timers minus 1 |
| 13 | main counter is 64-bit capable |
| 15 | legacy replacement routing capable |
| 31:16 | PCI vendor ID |
| 63:32 | counter period in femtoseconds |

### 4.2 General Configuration

| 位 | 名称 | 含义 |
|---:|---|---|
| 0 | ENABLE_CNF | main counter/timers enable |
| 1 | LEG_RT_CNF | legacy replacement route |
| 63:2 | reserved | 保持 0 |

### 4.3 Timer N Configuration/Capabilities

| 位 | 字段 |
|---:|---|
| 1 | interrupt type：0 edge，1 level |
| 2 | interrupt enable |
| 3 | periodic type |
| 4 | periodic capable（只读） |
| 5 | 64-bit capable（只读） |
| 6 | value-set：下一次 comparator write 是绝对/周期建立语义 |
| 8 | 32-bit mode |
| 13:9 | I/O APIC route |
| 14 | FSB interrupt enable |
| 15 | FSB interrupt capable（只读） |
| 31:16 | reserved |
| 63:32 | route capability bitmap（只读） |

[EXTERNAL IA-PC HPET Specification Rev. 1.0a]

### 4.4 MyOS2 当前 HPET

- 固定映射物理地址 `0xFED00000`；
- 读取 period，建立 clocksource，rating 250；
- timer0 配置为周期模式并注册 custom IRQ；
- handler 增加 jiffies 并调用 `do_timer()`；
- `hpet_time_init()` 未严格传播 `hpet_enable()` 失败，随后初始化可能继续；
- 固定路由/地址未由 ACPI HPET 表确认；
- comparator 周期和 mult/shift 需用测试向量核对。

[VERIFIED mykernel/arch/x86_64/kernel/hpet.c]

## 5. LAPIC timer

关键 local APIC timer 寄存器：

| 寄存器 | 作用 |
|---|---|
| LVT Timer | vector、mask、one-shot/periodic/TSC-deadline |
| Initial Count | 启动倒计数 |
| Current Count | 当前倒计数 |
| Divide Configuration | 输入时钟 divisor |

MyOS2 设置周期 LVT timer、vector `0xEE`，handler 增加局部计数并在屏幕显示；没有调用 `do_timer()`、hrtimer/timer wheel 或 scheduler tick。[VERIFIED mykernel/arch/x86_64/myos/LVT_timer.c]

邻近可用：
- one-shot clockevent；
- TSC-deadline mode（CPUID.1:ECX[24]）；
- per-CPU calibration；
- tick broadcast/NO_HZ；
- timer migration。

[EXTERNAL Intel SDM 253668-092US, Vol. 3A, Local APIC timer]

## 6. PIT/i8253

PIT 经典端口：

| 端口 | 用途 |
|---:|---|
| `0x40` | channel 0 data |
| `0x41` | channel 1 data |
| `0x42` | channel 2 data |
| `0x43` | mode/command |

MyOS2 保留 `init_pit_timer()` 痕迹，但相关调用/函数体被注释；所读快照中未见 PIT 作为校准源或 tick 源。[VERIFIED mykernel/arch/x86_64/kernel/time.c] [VERIFIED mykernel/arch/x86_64/myos/pre_init.c]

## 7. RTC/CMOS

`read_persistent_clock64()` 通过 MC146818/CMOS 读取 calendar，转换成秒，作为 timekeeping 初始化 wall time；持续的 `ktime_get()` 不直接轮询 CMOS。[VERIFIED mykernel/arch/x86_64/kernel/rtc.c]

CMOS 访问必须处理 update-in-progress、BCD/12h 模式和一致性快照；这些细节由 `mc146818_get_time()` 下层承担，卡片不把 `rtc.c` 的两次端口访问误写成完整协议。[VERIFIED mykernel/arch/x86_64/kernel/rtc.c] [INFERRED]

## 8. KVM clock

`kvmclock_init()` 能探测 KVM signature/features 并选新旧 system-time/wall-clock MSR；但 per-CPU pvclock 页面、stable bit、TSC 校准覆盖、sched_clock、wallclock 与 clocksource 注册全部注释，且所读快照中未见调用者。因此状态为 `stub`。[VERIFIED mykernel/arch/x86_64/kernel/kvmclock.c]

## 9. clocksource 与 clockevent 不可混淆

| 概念 | 要求 | 例子 |
|---|---|---|
| clocksource | 单调可读、足够宽、可换算 ns | TSC、HPET main counter、jiffies |
| clockevent | 在未来某时刻产生事件，可 one-shot/periodic | LAPIC timer、HPET comparator、PIT |
| sched_clock | 快速调度时间戳，允许与 timekeeping 分离 | MyOS2 的 TSC |
| persistent clock | 断电保持的墙钟初值 | CMOS RTC |

MyOS2 当前同一 HPET 同时具备 main-counter clocksource 候选和 comparator tick，但只证明后者推进了系统 jiffies；前者尚未成为活动 timekeeper。[VERIFIED mykernel/arch/x86_64/kernel/hpet.c] [VERIFIED mykernel/time/clocksource/clocksource.c]

## 10. 最小测试

1. 用模拟 CPUID 0x15/0x16 输入验证 Hz 公式、溢出和零字段回退。
2. 用 HPET period 10 ns、100 ns 等已知值验证 mult/shift 和 1 秒累计误差。
3. 启动后打印 `timekeeper.clock->name`，不能仅打印 clocksource list。
4. 对比 `ktime_get()`、jiffies、TSC sched_clock、HPET raw counter 的 10 秒漂移。
5. 屏蔽 HPET IRQ，确认 jiffies 停止；这可证明当前 tick 依赖。
6. 多 CPU 比较 TSC 偏移并跨 vCPU migration。
7. HPET 初始化失败时确保系统降级而不是访问未映射寄存器。
