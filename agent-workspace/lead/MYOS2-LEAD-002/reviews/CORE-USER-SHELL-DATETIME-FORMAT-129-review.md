---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_mainline_review"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "0b52728d0314"
execution_pin_short12: "824ce6b5607a"
read_channel: "connector"
status: "RETURN"
followup_id: "CORE-USER-SHELL-DATETIME-FORMAT-129"
author_delivery: "VERIFIED_REMOTE"
new_returns: ["R129-01"]
R127_02: "OPEN"
R128_01: "CLOSED_DOCUMENTATION_ONLY"
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/180-user-shell-datetime-format-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/myshell/tests.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-D00-127-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-dest-aligned-word-copy128-reviewed-shell-datetime-format129.md"]
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
---

# 第129批独立验收 · 作者交付核实，机器表定点退回

C08b在交付时报告上下文耗尽。C08独立从四个直接ref、Git对象和PR17取证。执行提交 `824ce6b5607a` 的唯一父为 `4a8952c6c956`，compare领先1、落后0，六件纯新增6,612行、283,929个UTF-8字节。旧文件修改／删除0／0。固定提交与活动执行分支逐件全文、字节和重算的Git Blob一致。PR16 Open/Ready、PR17原题Open/Draft，均至master、未合并。全量顶层评论100/100/46/0，正式作者标记全部身份只命中EXEC-19的 #7081367607，按ID两次读回与分页正文一致。180发布回执可独立定位。本次确认的是可见的交付结果。旧会话是否逐次完成其终检、最后何时停止，仅有作者陈述；后继无需旧会话恢复。[129作者原文](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7081367607) · [180号](https://github.com/08822407d/MyOS2/blob/0b52728d0314/agent-workspace/lead/MYOS2-LEAD-002/180-user-shell-datetime-format-contract.md) · [发布回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7080232408)

| 固定文件 | UTF-8字节 | Git Blob短12 |
|---|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/MANIFEST.md) | 10523 | `994fc24e0f8d` |
| [shell-datetime-format.md](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format.md) | 15218 | `05ef831029cc` |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml) | 141206 | `a56c7c3a0b55` |
| [shell-datetime-format-paths.dot](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot) | 81669 | `cc2c5aa8918a` |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/deferred-findings.yaml) | 6665 | `03ae459fe99b` |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/evidence.md) | 28648 | `8f67d9549b0f` |

第129批32项清单输入路径均可定位；两份机器件可解析。全部76个正式引用均在固定time的对应定义/配置中逐字连续1–5行，含CRLF；六件标签分布0/0/0/0/0/76，相同路径、符号和窗口正文去重为73。E040只到修饰状态入口、E044只到rank_long标签，本批E070/E071给出实际加一和取参语句；E027与E067合读给出余数写回及整除。导出DOT有325个唯一节点、371边，未声明端点0。声明完整与边的源码意义分别核查。[evidence.md](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/evidence.md) · [shell-datetime-format-paths.dot](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot) · [固定格式化器](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L648-L724)

## 可保留的源码结论

main第68行调用时间测试并忽略其返回。gettimeofday返回−1时请求perror，随即返回1。正常样本给定有效localtime指针与strftime生成的19字节日期文本。tv_sec进入日期转换，微秒字段tv_usec供第二个变参。printf的两个转换项各自初始化flags、rank、width、prec。23个非NUL格式字节在循环体中处理27次，s回读两次、6和l各一次；最后的格式NUL由循环条件读到。%s复制19字节；%06ld按signed long取参，flags=65，宽度6，十进制。输入0/7/12345/999999的初始位数0/1/5/6，零值调整后1/1/5/6，前导填充5/5/1/0。[main](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L66-L72) · [时间测试](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c#L104-L127) · [数字生成](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L153-L201)

四行文本及十六进制均已独立复算。前缀[0,14)、日期[14,33)、点[33,34)、微秒[34,40)、LF[40,41)。输出依次含000000、000007、012345、999999。终止NUL位于41；printf使用stdout文件号请求41字节。给定write返回41或12时printf都返回41，时间测试返回0，main忽略其值。真实二进制链接、时区、文件号和屏显未知。错误簇DOT中main→test及忽略test返回两边把出处泛标成tests.c:110–113，主调、弃用返回的实际出处为myshell.c:68；这是本回执的行址勘误。[write和返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L17-L25) · [shell-datetime-format-paths.dot](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot#L746-L753)

## R129-01 · 可按案例重放的实参与写入次序

B的 `parser_trace_shared[iter=25].consumed_arg.value` 固定为0，代入其他三例与各自正确的format_int输入冲突。应保留共享控制列，用完整case键引用本例microseconds。C的 `EMIT_byte_log` 给出按目的偏移升序的最终字节映射，`write_index` 也为升序。实际数字写发生在右向左赋值处，数字部分不全经EMIT。12345先填充offset 34，再写39、38、37、36、35；999999按39至34写。作为地址映射可用，作为操作日志则失真。下一批只需交可复建的字段差量，分别给每例实参、最终地址视图和真实存储次序；接回原来的27步与41字节。两项均属同一案例机器表，合记一个退回项。[facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L2418-L2440) · [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L2723-L2766) · [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L3711-L3729) · [逆向写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L172-L190)

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
					case rank_long:
						val = (size_t)(ssize_t)
						va_arg(ap, signed long);
						break;
```

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
		val /= base;
```

第129批裁定 `RETURN`，R129-01 OPEN；历史项另见[历史裁定](CORE-USER-SHELL-DATETIME-FORMAT-129-D00-127-128-review.md)。作者六件和正式评论冻结。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。
