---
task_id: MYOS2-DR-005
produced_by: "ChatGPT Pro / GPT-5.6 Pro"
date: 2026-09-01
base_snapshot: "time @ a039d9803ade2a1613d620bda375e028530d5242"
inputs_read:
  - "mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h"
  - "mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h"
  - "mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h"
  - "mykernel/arch/x86_64/include/asm/irqflags.h"
  - "mykernel/arch/x86_64/myos/apic.c"
  - "mykernel/arch/x86_64/pci/direct.c"
status: final
open_questions:
  - "通用 spinlock wrapper 是否在 UP/DEBUG 配置下替换 arch 实现，需要构建矩阵验证。"
  - "现有锁 API 对 acquire/release/full barrier 的正式契约未见文档。"
---

# 速查卡：原子指令、ticket spinlock 与内存顺序

## 1. x86 原子基础

| 指令/前缀 | 作用 | 关键语义 |
|---|---|---|
| `LOCK` | 使支持的读-改-写指令原子 | 对 cacheable memory 通常表现为 cache-line ownership；同时提供强排序 |
| `XCHG r/m,reg` | 交换 | 含内存操作数时隐含 lock 语义 |
| `CMPXCHG` | 比较累加器并条件写 | CAS 基础；失败也返回观察值 |
| `XADD` | 交换并相加 | ticket 分配/atomic fetch-add |
| `BTS/BTR/BTC` | 原子位设置/清除/翻转（配 LOCK） | 位锁/bitmap |
| `PAUSE` | 自旋提示 | 减少 pipeline/memory-order penalty |
| `MFENCE/LFENCE/SFENCE` | 显式屏障 | 用途不同；不能互换 |
| compiler barrier | 只限制编译器重排 | 不直接产生 CPU fence |

[EXTERNAL Intel SDM, Vol. 2 instruction reference; Vol. 3A §8.1–8.2]

x86 TSO 比许多架构强，但编译器仍可重排；锁 API 应明确 compiler 与 CPU 两层契约。[EXTERNAL Intel SDM 253668-092US, Vol. 3A, multiprocessor management]

## 2. MyOS2 atomic 实现

`atomic_arch.h` 以内联汇编提供 add/sub/inc/dec、return 变体、cmpxchg/xchg、test-and-bit 等操作。[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h]

### 已确认语义错误

`arch_atomic_add_test_negative(i, v)` 的函数名/注释表示“加 i 后测试负数”，但汇编执行 `subl i, v`。其别名 `arch_atomic_add_negative` 因而也是减法语义。[VERIFIED mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h]

修复前应加入：

```text
v=1, i=2 -> result=3, negative=false
v=-3, i=1 -> result=-2, negative=true
v=INT_MAX, i=1 -> 按二补码/接口约定处理 overflow
```

## 3. ticket spinlock 格式

MyOS2 使用 head/tail ticket：

```text
owner/head = 当前可进入的 ticket
next/tail  = 下一次发出的 ticket

lock:
  my = atomic_fetch_add(next, 1)
  while (owner != my)
      pause

unlock:
  owner++
```

[VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h] [VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h]

优点：FIFO 公平。缺点：所有 waiter 轮询同一 cache line，CPU 数增加时流量大；owner 停顿会阻塞后续全部 waiter。

### 已确认严重错误：trylock 未获取

当前 `arch_spin_trylock()` 只读取 head/tail 并在相等时返回 `true`，没有 CAS 或其他原子写入把 tail 增加。调用者看到成功后并未持有锁，多个 CPU 可同时“成功”。[VERIFIED mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h]

正确 ticket trylock 的基本形式：

```text
old = atomic_load(head, tail)
if (head != tail)
    return false
new = old with tail = tail + 1
return cmpxchg(lock, old, new) == old
```

必须处理 head/tail 位宽、溢出和 endian/布局，不应只比较。

## 4. acquire/release 语义

锁契约应是：

- successful lock/trylock：acquire；
- unlock：release；
- failed trylock：不获得保护；
- irqsave lock：还要保存/恢复本 CPU IF；
- raw lock 不允许睡眠；
- 递归获取非递归 spinlock 是死锁。

在 x86 上 locked RMW 足以提供强 acquire/full barrier；unlock 的普通 store 在 TSO 下可作为 release，但需要 compiler release barrier。[EXTERNAL Intel SDM 253668-092US, Vol. 3A §8.2]

## 5. IRQ 与 SMP

| 场景 | 最低要求 |
|---|---|
| 仅进程上下文，多 CPU | spin_lock |
| 同一数据也在本 CPU hardirq 使用 | spin_lock_irqsave |
| softirq 与进程共享 | spin_lock_bh 或等价禁 bottom half |
| NMI 也使用 | 普通 spinlock 通常不安全，需 NMI-safe 设计 |
| 单 CPU | 禁中断仍可能需要；不能把 SMP 锁简单删掉而忽略 IRQ reentry |

MyOS2 IRQ 汇编可在 C handler 前 `sti`，使“handler 默认不可嵌套”的假设不成立；驱动锁必须按实际重入语义审计。[VERIFIED mykernel/arch/x86_64/entry/entry_64.S]

## 6. 已见的无锁共享硬件窗口

### I/O APIC index/data

I/O APIC MMIO 常用一个 index 寄存器选择目标，再经 data 寄存器读写。两步操作必须作为整体加锁。MyOS2 `myos/apic.c` 未见锁；多个 CPU 或嵌套 IRQ 可把 selector 改成另一项。[VERIFIED mykernel/arch/x86_64/myos/apic.c]

### PCI CF8/CFC

CF8 selector + CFC data 同样是全局两步协议；`pci_config_lock` 被注释。[VERIFIED mykernel/arch/x86_64/pci/direct.c]

这些不是“CPU 单条指令不原子”的问题，而是多条设备协议事务缺少互斥。

## 7. 邻近指令/机制

| 能力 | 探测 | 用途 |
|---|---|---|
| CMPXCHG8B | CPUID.1 EDX.CX8 | 32 位下 64-bit CAS |
| CMPXCHG16B | CPUID.1 ECX[13] | 128-bit CAS；执行前必须门控 |
| MONITOR/MWAIT | CPUID.5 | 低功耗等待；内核策略复杂 |
| WAITPKG | CPUID.7 ECX[5] | UMONITOR/UMWAIT/TPAUSE |
| HLE/RTM | CPUID.7 EBX[4]/[11] | transactional lock elision；需 fallback/errata |
| split-lock detection | MSR/model capability | 诊断跨 cache-line locked access |
| queue spinlock | 软件算法 | 比 ticket lock 更适合高核数 |

MyOS2 当前无需急于引入高级算法；先修正 trylock/atomic 语义并建立 litmus/压力测试。

## 8. 最小测试

1. **trylock 双 CPU barrier 测试**：两 CPU 同时 trylock，恰好一个成功。
2. **互斥计数**：N CPU 各加共享 counter，最终精确。
3. **ticket 公平/回绕**：小位宽模拟 head/tail wrap。
4. **atomic add-negative 表驱动测试**：正/负/零/边界。
5. **message passing**：writer 在锁内写 data+flag，reader 获取锁后不得看到旧 data。
6. **IRQ reentry**：持锁时触发使用同锁的 IRQ，验证 irqsave 版本不会死锁。
7. **IOAPIC/PCI selector 压力**：并发写不同寄存器，读回不串项。
8. **alignment**：所有 locked object 自然对齐且不跨 cache line；debug build 做断言。
