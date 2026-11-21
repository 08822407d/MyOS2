---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-SEEK-POSITION-114
record_type: independent_historical_documentation_closure
produced_by: GPT-6 Astra Pro
model_per_owner: Pro
conversation_display_name: MYOS2-A-C07 内核分析主线续建
date: 2026-11-20
phase: 2
read_channel: connector
source_pin_short12: a039d9803ade
master_direct_short12: e8e60cd19540
lead_before_publication_short12: 41b2c2f955c6
execution_pin_short12: a2dd370fcf15
full_local_P9: NOT_RUN
build_and_runtime: NOT_RUN
status: CLOSED_DOCUMENTATION_ONLY
R112_02: CLOSED_DOCUMENTATION_ONLY
R113_01: CLOSED_DOCUMENTATION_ONLY
composite_112_113: PASS_PENDING_LOCAL
inputs_read: [contract_162, reviews_113_and_D00, original_112_facts, original_113_facts_graph_and_findings, batch114_six, pinned_time_definitions]
self_check: {scalar_pairs: 6, return_outputs: 12, corrected_comparison_models: 3, new_review_quotes: 3}
---

# D00 独立核收 · 第112、113批

C07核收 R112-02、R113-01，均为 `CLOSED_DOCUMENTATION_ONLY`。分别合读旧验收、冻结原件和本次差量后，第112、113批均为 `PASS_PENDING_LOCAL`。此前已关闭的 R111-02、R112-01 沿用原裁定。第114批自身有独立裁定。[114验收](CORE-USER-FILE-SEEK-POSITION-114-review.md) · [前轮D00裁定](https://github.com/08822407d/MyOS2/blob/41b2c2f955c6/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMESPEC-ABI-BOUNDARIES-113-D00-111-112-review.md) · [113原裁定](https://github.com/08822407d/MyOS2/blob/41b2c2f955c6/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-TIMESPEC-ABI-BOUNDARIES-113-review.md) · [162号合同](https://github.com/08822407d/MyOS2/blob/41b2c2f955c6/agent-workspace/lead/MYOS2-LEAD-002/162-user-file-seek-position-contract.md)

## R112-02 · 六例回转与负门、零返回

对照113事实表 `D00.R112_02.roundtrip.models[original_model]` 与114的 `five_nonzero_models[model]` 和 `zero_model_unchanged`。先用整数把输入拆为整秒与非负余纳秒，再核两版字段和两个回转输出。旧五处 caveat 均能逐字定位。[113事实原件](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/facts-and-boundaries.yaml) · [facts-and-boundaries.yaml L28–205](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/facts-and-boundaries.yaml#L28-L205)

| 原例 | 输入 ns | 期望 (sec, nsec) | 经 timespec64_to_ktime 返回 | 经 timespec64_to_ns 返回 |
|---|---:|---|---:|---:|
| C01 | 0 | (0, 0) | 0 | 0 |
| C02 | 1 | (0, 1) | 1 | 1 |
| C03 | −1 | (−1, 999999999) | −1 | −1 |
| C04 | 1000000000 | (1, 0) | 1000000000 | 1000000000 |
| C05 | −1000000000 | (−1, 0) | −1000000000 | −1000000000 |
| C06 | −1000000001 | (−2, 999999999) | −1000000001 | −1000000001 |

C01沿初始化对直接返回，无除法。C02、C04进入正非零分支，局部 `s32 rem` 的地址进入要求 `u32 *` 的余数形参，实际编译及该写回仍未证。C03、C05、C06另需内层有符号表达式可表示。源码先对 nsec 取负，以上三例均在其定义域内。随后对 u64 商 q 计算外层 `-q-1`，q 为 0、1 时结果分别为 18446744073709551615、18446744073709551614。赋给有符号秒并得到表中负值使用目标转换约定。在这三个具体输入上的模型成立。[time_misc.c L50–69](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L50-L69) · [math64.h L41–45](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/math64.h#L41-L45)

复核113图114行、122行及114图的实际边。第58行判负，64–65行赋值，零输入沿初始化对至返回，只有非零路径写成员。更早的八条图修订保留113原账。[113原图](https://github.com/08822407d/MyOS2/blob/41697d76ae5e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-timespec-abi-boundaries-113/timespec-abi-boundaries-paths.dot#L114) · [file-seek-position-paths.dot](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/file-seek-position-paths.dot)

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
	if (likely(nsec > 0)) {
		ts.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
		ts.tv_nsec = rem;
	} else if (nsec < 0) {
```

[time_misc.c L55–58](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L55-L58)

[VERIFIED mykernel/time/misc/time_misc.c::ns_to_timespec64]
```c
		ts.tv_sec = -div_u64_rem(-nsec - 1, NSEC_PER_SEC, &rem) - 1;
		ts.tv_nsec = NSEC_PER_SEC - rem - 1;
```

[time_misc.c L64–65](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.c#L64-L65)

## R113-01 · 短路求值、比较出处与返回类型

| 原位置 | 独立核收 |
|---|---|
| 113图137/138 | 时钟回调正常返回后总会取其结果并求值 `!error`。只有左侧为真才求值右侧并调用 put_timespec64。114图已分开，原138的条件调用可继续消费。 |
| C04、C05 | 原输入按秒字段不同，equal 均为 false，compare 依次为 −1、+1。两个输出分别有 equal 体 E039/E055 与 compare 秒门 E040–041/E056–057 依据。旧 E 编号归113，新 E 编号归114。 |
| C06 | (0, 1000000000) 与 (1, 0) 在原字段下不相等且 compare 为 −1。把左侧规范化后相等，同秒差返回0；旧 E042、新 E058 实际覆盖此返回。 |
| TS113-CAND-07 | LP64 的 LONG_MAX 减 −1 在 long 运算中越界。2147483648 减 0 可用 long 表示，但超出32位返回 int 的值域，转换结果或信号取决于实现。规范纳秒的同秒差在 [−999999999,999999999] 内。秒不等时提前返回。 |

[syscall.c L44–49](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L44-L49) · [time_misc.h L44–63](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/misc/time_misc.h#L44-L63) · [facts-and-boundaries.yaml L209–361](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/facts-and-boundaries.yaml#L209-L361) · [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/a2dd370fcf15/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-file-seek-position-114/deferred-findings.yaml)

[VERIFIED mykernel/time/syscall.c::clock_gettime]
```c
	error = kc->clock_get_timespec(which_clock, &kernel_tp);

	if (!error && put_timespec64(&kernel_tp, tp))
		error = -EFAULT;
```

[syscall.c L44–47](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/time/syscall.c#L44-L47)

两项关闭范围限上述文档及有界数学解释。编译器对指针实参、极值输入及 ABI 的行为和实际运行仍待本地证据。冻结112/113/114作者文件、原始计数和原评论，当前裁定通过该追加回执表达。
