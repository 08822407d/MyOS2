---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-TIMESPEC-ABI-BOUNDARIES-113"
record_type: "independent_batch_review"
status: "RETURN"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro"
conversation_display_name: "MYOS2-A-C07 内核分析主线续建"
date: "2026-11-20"
date_timezone: "Asia/Singapore"
phase: 2
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "c58d1ad8f222"
execution_pin_short12: "41697d76ae5e"
R110_01: "CLOSED_DOCUMENTATION_ONLY"
R110_02: "CLOSED_DOCUMENTATION_ONLY"
batch110: "PASS_PENDING_LOCAL"
R111_01: "CLOSED_DOCUMENTATION_ONLY"
R111_02: "CLOSED_DOCUMENTATION_ONLY"
batch111_composite: "PASS_PENDING_LOCAL"
R112_01: "CLOSED_DOCUMENTATION_ONLY"
R112_02: "OPEN"
batch112: "RETURN"
R113_01: "OPEN"
batch113: "RETURN"
full_local_P9: "NOT_RUN"
runtime: "NOT_RUN"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/161-user-timespec-abi-boundaries-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-D00-110-111-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-KTIME-DEADLINE-ARITHMETIC-112-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-20-ktime112-reviewed-timespec113.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/evidence.md","mykernel/scripts/options_flags.cmake","mykernel/time/timer/timer_const.h","mykernel/time/misc/time_misc_const.h","mykernel/time/timer/timer.c","mykernel/time/timer/timer.h","mykernel/time/misc/time_misc.c","mykernel/time/syscall.c","mykernel/time/misc/time_misc.h","mykernel/include/linux/kernel/math64.h","mykernel/time/ktime/ktime.h","mykernel/time/timer/hrtimer.c","mykernel/time/ktime/ktime_macro.h","mykernel/lib/list/hash_list.h","mykernel/include/uapi/linux/time_types.h","mykernel/include/uapi/asm-generic/posix_types.h","mykernel/time/misc/time_misc_types.h","mykernel/time/time_type_declaration.h"]
open_questions: []
owner_decisions_requested_now: []
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: {"time_options":"\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","master_options":"\t-m64 -mcmodel=large -fPIE \\","time_panic":"\tthis_cpu = smp_processor_id();","master_panic":"\tthis_cpu = raw_smp_processor_id();"}
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0}
---

# 第113批主线独立验收　用户态时间结构

**裁定 RETURN，限文档映射。** 用户态结构复制的七个抽象分支、三种校验器的十二组输入、比较与规范化的九组样例均核收。有界数值与返回路径可以消费。R113-01只要求A/C的图和局部证据指向对应原句。历史三项另见[本轮D00裁定](CORE-USER-TIMESPEC-ABI-BOUNDARIES-113-D00-111-112-review.md)。[161合同](https://github.com/08822407d/MyOS2/blob/c58d1ad8f222/agent-workspace/lead/MYOS2-LEAD-002/161-user-timespec-abi-boundaries-contract.md) · [113作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7033514346) · [固定六件](https://github.com/08822407d/MyOS2/tree/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113)

## Git与证据身份

执行 41697d76ae5e 的唯一父为 f909700f17ab。compare 为1个提交、ahead 1、behind 0；仅六新增3297行，旧改删0/0。固定与活动执行分支逐件全文、Git blob一致。112目录的六个blob亦与其冻结树一致。正式作者评论按ID两读、顶层无since分页50/50/50/50/11/0，211条中本批全部作者的正式标记仅一个，属于EXEC-17。161主线发布标记一次。主线发新件前，四条直接引用为 lead=c58d1ad8f222、exec=41697d76ae5e、time=a039d9803ade、master=e8e60cd19540；仅PR16/17开放，分别Ready与原题Draft，均base master、未合并。[执行差异](https://github.com/08822407d/MyOS2/compare/f909700f17ab...41697d76ae5e) · [161发布回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7030199541)

| 文件 | 新增行 | 固定blob短值 |
|---|---:|---|
| MANIFEST.md | 116 | 31eac87b16f9 |
| deferred-findings.yaml | 210 | 8f4888a060b9 |
| evidence.md | 629 | 71e6071671a6 |
| facts-and-boundaries.yaml | 2059 | b33325c667b1 |
| timespec-abi-boundaries-paths.dot | 162 | 02049494d8fa |
| timespec-abi-boundaries.md | 121 | de28e2d03171 |

60个新标签逐一从13个固定源文件按起止行重新取文。逐字匹配60，窗口皆为1–5连续行。分别核CMake选项体、独立宏、普通函数及系统调用宏体的归属，单定义合格60。facts索引的ID、符号、路径、整数行窗均与新引文相符。26个普通位置单独核对。图含72个声明节点、66条带标签边，端点都有声明；边的语义另审。候选与缺口各8。六件无40位十六进制串；清单公约原句、双分支两对金丝雀已回源。完整本地P9、预处理/编译/链接、Graphviz、内核及实机未运行。[固定引文账](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/evidence.md#L1) · [作者清单与检查范围](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/MANIFEST.md#L1)

## A　复制报告、字段写入和调用者

在普通可表示字段、有效局部对象和可正常返回的复制调用前提下，A01/A02在 get_timespec64 只看局部 int ret。非零则返回 -EFAULT，随后的两次目标字段赋值均未发生；为零则先写秒、后写纳秒，返回0。复制大小取 sizeof(kts)。285–287行的compat掩码均处在注释内。核 A03/A04，put_timespec64先构造本地双字段对象，请求一次输出复制，按其返回零或非零分别返回0或-EFAULT。两方向请求都未实测布局、复制字节或实际用户地址状态。[输入复制完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L273-L293) · [输出复制完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L304-L313)

A05–A07回到 clock_gettime。kc为空时立即-EINVAL。在可调用的读取回调正常返回后，每次均计算 `!error`。旧error非零直接保留，零才求值第二操作数并调用put；若put报告非零则改为-EFAULT。冻结A表与正文准确记录了返回值。[系统调用完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L33-L50) · [A的七个模型](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L844)

## B　三种校验器的边界

从单宏独立计算：N=1000000000，H=(2^63−1)/N向零取整=9223372036，预留30×365×24×3600=946080000，S=H−预留=8277292036，后两门以秒为单位。先看秒非负与 `(ulong)tv_nsec < N`；basic失败时另两个函数均提前返回。以64位ulong和样例可表示域重算12行共36个布尔值和24个第二门可达标志，全部一致。表中顺序为 basic/strict/settod。[三个完整谓词](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L89-L121) · [秒上限宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_const.h#L44-L58) · [十二行原表](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L1012)

| 输入 | 三项结果 | 第二门 |
|---|---|---|
| (0,0)、(0,N−1)、(1,7) | 真／真／真 | 均求值 |
| (0,−1)、(0,N)、(−1,N−1) | 假／假／假 | 均跳过 |
| (S−1,0) | 真／真／真 | 均求值 |
| (S,0)、(S+1,0)、(H−1,0) | 真／真／假 | 均求值 |
| (H,0)、(H+1,0) | 真／假／假 | 均求值 |

## C　字段比较和规范化

重算C01–C05，compare依次为0、+7、−7、−1、+1，equal依次为真、假、假、假、假。两秒不同直接返回±1，同秒返回有符号 long 纳秒差并转 int；规范纳秒差的范围是 [−999999999,999999999]。C06 原始 (0,N) 比 (1,0) 小，左侧经进位后相等，compare由−1变0。C07 的 (0,−1) 借位后为 (−1,N−1)，前者纳秒越界，后者秒为负，两次basic均假。C08/C09在中间算术可表示时分别得到 (1,0)、(0,N−1)。get/put、equal/compare 各自完整体没有隐式调用规范化；add/sub 的实际调用可定位。[相等、比较与两处实际调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L44-L84) · [进位、借位与最终写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L22-L41) · [C的模型与来源](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L1305)

**R113-01 OPEN**，只修以下可定位的消费者，无需重写 A/B/C。

- 图第137行从读取回调返回连向 `if !error` 的边标 CONTROL_ONLY_ON_SUCCESS。源码的真假判断会在回调返回后求值；只有条件右侧 put 调用受成功门控制。保留现图第138行的有条件调用，拆清返回值进入判断和通过判断后的求值。[113图](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries-paths.dot#L135) · [两个操作数与最后返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L44-L49)
- facts 的 C04、C05 都报告 equal=false，但各行只引 compare 的秒门 E040/E041，补 equal 体 E039。C06 报规范后 compare=0，其本行缺同秒差返回 E042。三行沿用原值、另列每个输出的直接出处。[C04–C06](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml#L1413) · [两个独立定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L44-L63)
- CAND-07 的未证描述把非规范输入归为 “overflow long/int”。应区分 long 减法本身越界，以及 long 差已可表示、转为返回 int 时超出 int 值域的目标转换语义；等秒之前不会执行该减法。可用 LP64 的两个有界宽度说明，编译和实际结果留待验证。事实表比较段已有区分，以此统一条目。[原候选](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/deferred-findings.yaml#L120) · [返回类型和减法操作数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L55-L63) · [纳秒字段](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc_types.h#L7-L10)

历史 R112-02 的两条图边与五处前提由D00回执单独跟踪。双方验收均限静态文档，没有内核变更。作者当时真实的 READY_FOR_MAINLINE_REVIEW 与本轮主线 RETURN 分别保留。

## 主线独立源码短引文

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```c
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
```

固定配置中的平台和CPU数选项。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L60-L61)

[VERIFIED mykernel/time/misc/time_misc.c::get_timespec64]
```c
	ret = copy_from_user(&kts, uts, sizeof(kts));
	if (ret)
		return -EFAULT;
```

输入层观察复制报告，非零立即返回。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L279-L281)

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	error = kc->clock_get_timespec(which_clock, &kernel_tp);

	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;
```

回调结果进入两操作数的短路判断。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L44-L47)

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_valid_settod]
```c
			if (!timespec64_valid(ts))
				return false;
			/* Disallow values which cause overflow issues vs. CLOCK_REALTIME */
			if ((ulonglong)ts->tv_sec >= TIME_SETTOD_SEC_MAX)
				return false;
```

基本判断提前返回，随后是含等号的设时门。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L115-L119)

[VERIFIED mykernel/time/misc/time_misc.h::timespec64_compare]
```c
			if (lhs->tv_sec > rhs->tv_sec)
				return 1;
			return lhs->tv_nsec - rhs->tv_nsec;
```

秒较大与同秒返回不同量。 [固定源码](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L60-L62)
