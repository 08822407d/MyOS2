---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-REALTIME-READOUT-ABI-107"
record_type: "independent_realtime_readout_abi_review"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-19"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "120f1597cfdd"
execution_pin_short12: "988632b3757e"
status: "RETURN"
full_local_P9: "NOT_RUN"
runtime_evidence: "NOT_RUN"
acceptance_ceiling: "PASS_PENDING_LOCAL"
R105_01: "CLOSED_DOCUMENTATION_ONLY"
R105_02: "CLOSED_DOCUMENTATION_ONLY"
R106_01: "CLOSED_DOCUMENTATION_ONLY"
R106_02: "OPEN_TWO_DIRECT_REFERENCES"
R107_01: "OPEN"
R107_02: "OPEN"
batch105_composite: "PASS_PENDING_LOCAL"
batch106_composite: "RETURN"
batch107: "RETURN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/153-user-realtime-readout-abi-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOOT-DELAY-ACCOUNTING-106-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-BOOT-DELAY-ACCOUNTING-106-D00-105-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107/realtime-readout-abi.md"]
owner_decisions_requested_now: []
open_questions: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":9,"quotes_reconfirmed":9,"downgraded_to_inferred":0,"scope":"本件独立源码短锚"}
---

# 第107批独立审核　两个墙钟接口的读数和输出

**C07 裁定 RETURN，限 R107-01 和 R107-02 两项文档补正。** 在固定 x86-64 源码所列正常条件下，两个接口汇入同一个墙钟读取函数。给定同一归一化样本，gettimeofday 写出整微秒，clock_gettime(CLOCK_REALTIME) 写出原纳秒字段。这部分可以复用。旧事项的逐项裁定另见 [D00 回执](CORE-USER-REALTIME-READOUT-ABI-107-D00-105-106-review.md)。

## 仓库交付与证据复算

审核起点四条直接引用为 lead 120f1597cfdd、exec 988632b3757e、time a039d9803ade、master e8e60cd19540。开放 PR 仅16、17，分别为 Open/Ready 和 Open/Draft，均以 master 为 base、均未合并。PR17 作者评论按 ID 和无 since 全分页回读一致，50＋50＋50＋48＝198 条中，正式作者标记只在 7004106478 出现一次。[作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7004106478) · [153固定合同](https://github.com/08822407d/MyOS2/blob/120f1597cfdd/agent-workspace/lead/MYOS2-LEAD-002/153-user-realtime-readout-abi-contract.md)

执行提交 988632b3757e 的唯一父为 11971296e4f3，compare 为 ahead 1、behind 0。六件共新增 3,273 行，旧文件修改、删除均为0。逐件比较固定提交、活动执行分支的全文与 blob，全部一致。[单步差异](https://github.com/08822407d/MyOS2/compare/11971296e4f3...988632b3757e) · [固定六件](https://github.com/08822407d/MyOS2/tree/988632b3757e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-realtime-readout-abi-107)

| 文件 | 新增行 | blob短SHA |
|---|---:|---|
| MANIFEST.md | 92 | bdb3e569b147 |
| deferred-findings.yaml | 256 | ea97c303ef39 |
| evidence.md | 997 | 021f808da702 |
| facts-and-boundaries.yaml | 1740 | a62831b81872 |
| realtime-readout-abi-paths.dot | 139 | d6d12c6dafbb |
| realtime-readout-abi.md | 49 | 5e7e182a28b0 |

从固定 time 的29份源码独立回取全部90处窗口。每个窗口均为连续1–5行，90个逐字相符，90个不同的路径与行段组合。六件原始验证词头共90，40位散列串0处，普通位置记录19项。事实矩阵各8、8、8、9、9、12、9行，共63行，纸面模型9项，候选和缺口各6项。DOT声明58个节点、75条边，未声明端点0。direct master 公约整句、两分支的编译选项和 panic 金丝雀已逐项回源。以上为远端文本核验，完整本地 P9 为 NOT_RUN。

## 可直接消费的结论

- 96 和 228 的编号、公共映射清单的两个实例、x64_sys_call 的 switch 及两参数包装在源码中相互对应。宏分别取 regs->di、regs->si，再转换成接口的两个形参。编译、链接和用户程序实际进入该路径均待证。[映射清单](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L50-L71) · [三次包含](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L10-L32)
- tv 有效且非空时，gettimeofday 直接调用 ktime_get_real_ts64。CLOCK_REALTIME=0 对应的数组项指向 clock_realtime，其 clock_get_timespec 字段指向 posix_get_realtime_timespec。clock_gettime 通过字段调用，回调直接取样并返回0。源码读取 xtime_sec 和 tkr_mono 的转换值，清零纳秒后归一化。[两个调用体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L12-L49) · [实时钟绑定](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/posix-timers.c#L89-L93) · [墙钟取样](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timekeeping/timekeeping.c#L73-L91)
- 假定字段可表示、用户输出对象可写且复制完整，同取秒12，纳秒为999、1000、999999999，gettimeofday 的微秒字段分别为0、1、999999，另一接口的纳秒字段依次为原值。两次独立调用的采样时刻可不同。tz 写块为注释。put_timespec64 填充两个 UAPI 字段，并按 sizeof(kts) 请求复制。两种布局各16字节是给定类型及对齐下的模型，时钟精度、可见内存和目标 ABI 均未实测。[字段写出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L19-L28) · [局部对象及复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L304-L312)

## R107-01 OPEN　补一个强制配置锚，修正三处宏定位

153把固定配置命令列为强制锚。90条中没有 options_flags.cmake。P05 写了 wrapper 选项的行号，MANIFEST 也写了金丝雀，但未以 P2 取这个选项。以 RV107-Q1 配 syscalls.h 第93–102行的条件导入及 P06，明确“选项文本、头文件条件、生成后链接”三层。[配置源](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L58-L70) · [导入条件](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/syscalls.h#L93-L102)

E043/E044/E045 的字面都正确，三个标签中的 “__SYSCALL declaration macro / table macro / switch macro” 是带用途的描述。源码里三次定义的标识符均为 __SYSCALL。用三个独立 ID 标出原名单行宏，另列各次 include/undef 的位置。数组初始化可直接引用 RV107-Q2，switch 已有 E046。P01/P02 保留为映射实例。t1-03～08 及 sysmap 发出的三条边可重用这组材料，不需重抄全部入口。

## R107-02 OPEN　限定归一化的整数域，把表和图接回直接来源

| 消费位置 | 独立核查 | 最小纠正 |
|---|---|---|
| t4-07、t7-05、M04；iterdiv 到两个结果字段 | __iter_div_u64_rem 用 u32 ret 计商。仅说明除数非零和 u64 加法合法，未确保数学商能装入 u32。原样例的商为1，结果仍为 (13,0)。 | 以 S 为合法无回绕的非负纳秒和、B=10^9。对完整数学归一化要求 floor(S/B) 可由 u32 表示，最终秒数可表示，再给余数域 0≤r<B。用 S=B×2^32 的纯数学界标说明计数宽度；无需实际迭代。引用 E078–080、RV107-Q5–Q7。 |
| t4-05、to_ns → sample_ns | E075/E076 跳过分支条件。普通乘加右移只适用于 delta≤clock->max_cycles 且所需算术有效的路径。 | 配 RV107-Q9 与原体内212–215、218行。简明标出超过阈值后的两个返回去向，其他细节沿用103已审边界。 |
| t2-06、t2-08、t5-09、t6-06、t6-11 | E057/P03 没有 tz 注释块；E046/E053 没有数组表项；E065/E072/E083 没有输出赋值；E060/E061 没有实时回调的成功返回；E088 仅使用 ENOERR。 | 依次补 E059；三处映射、包含及 RV107-Q2；E086；E065；RV107-Q8。已有证据只需交叉引用。 |
| tvnonnull → ktimeread、do_gt → putsec/putusec、ktimeread → sample_sec/sample_nsec | 判断节点被画作直接调用方；put_user 的定义是宏；E072 只取初始秒，E073 清零并调用归一化。 | 前者改作条件边，保留 do_gt 的带条件实际调用；两个 put_user 写作宏展开及短路先后；最终两个字段引用 E078 及对应商余依据。 |

上述退回限于作者文档的证据资格、适用域及引用关系。真实时钟取样、并发一致性、预处理、编译、内存与硬件仍为 NOT_RUN。作者停写，后继用新目录差量补正，107原件及90/90/0原账保全。

## C07 固定源码短证据

### RV107-Q1

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
```

同一条设置 C 编译标志的命令中的体系结构包装开关。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L65-L65)

### RV107-Q2

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::sys_call_table]
```c
const sys_call_ptr_t sys_call_table[] = {
#include <asm/syscalls_64.h>
};
```

数组定义实际包含公共映射清单。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L20-L22)

### RV107-Q3

[VERIFIED mykernel/time/syscall.c::gettimeofday]
```c
		ktime_get_real_ts64(&ts);
		if (put_user(ts.tv_sec, &tv->tv_sec) ||
				put_user(ts.tv_nsec / 1000, &tv->tv_usec))
			return -EFAULT;
```

宏实例的函数体中先读取墙钟，再依次请求秒和微秒写出。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L19-L22)

### RV107-Q4

[VERIFIED mykernel/time/timer/posix-timers.c::posix_get_realtime_timespec]
```c
	ktime_get_real_ts64(tp);
	return 0;
```

实时钟回调直接读同一墙钟并返回零。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timer/posix-timers.c#L30-L31)

### RV107-Q5

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
		u32 ret = 0;
		while (dividend >= divisor) {
```

商计数器的声明和循环条件。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/math64.h#L13-L14)

### RV107-Q6

[VERIFIED mykernel/include/linux/kernel/math64.h::__iter_div_u64_rem]
```c
			dividend -= divisor;
			ret++;
		}
		*remainder = dividend;
		return ret;
```

每次迭代减去除数、递增计数，最后分别交还余数和商。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/math64.h#L19-L23)

### RV107-Q7

[VERIFIED mykernel/time/misc/time_misc_const.h::NSEC_PER_SEC]
```c
	#define NSEC_PER_SEC			1000000000L
```

除数的实际固定宏定义。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_const.h#L32-L32)

### RV107-Q8

[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::ENOERR]
```c
	#define	ENOERR		0	/*  */
```

正常标量写宏所用的零值常量。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/uapi/asm-generic/errno-base.h#L5-L5)

### RV107-Q9

[VERIFIED mykernel/time/timekeeping/timekeeping.c::timekeeping_cycles_to_ns]
```c
	if (unlikely(delta > tkr->clock->max_cycles)) {
```

大周期差的分支条件。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/timekeeping/timekeeping.c#L206-L206)
