---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "phase2_independent_historical_correction_review"
followup_id: "CORE-USER-INIT-SHELL-LAUNCH-YIELD-131"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（沿用 Owner 对 C09 正式验收的选择）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "5a22d13876bc"
execution_pin_short12: "2089bfff6f4a"
read_channel: "connector"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/183-user-init-shell-launch-yield-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/myinit/initd.c","myinitramfs/CMakeLists.txt","myinitramfs/myinit/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/sched/syscall.c","mykernel/sched/scheduler/scheduler_types.h","mykernel/sched/scheduler/myos_rt.c","mykernel/sched/scheduler/scheduler_core.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h","myinitramfs/mylib/myglibc/stdio/errlist_internal.c","myinitramfs/mylib/myglibc/stdio/errlist.c","myinitramfs/mylib/myglibc/include/stdio.h","myinitramfs/mylib/myglibc/string/string.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
status: "PARTIAL_CLOSURE"
R127_02: "CLOSED_DOCUMENTATION_ONLY"
R130_01: "CLOSED_DOCUMENTATION_ONLY"
R130_02: "OPEN"
combined_127: "PASS_PENDING_LOCAL"
combined_130: "RETURN"
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
---

# 第 131 批 D00 · 三项历史补正的独立裁定

| 项目 | 主线裁定 | 对应冻结批与本次差量的组合 |
|---|---|---|
| R127-02 | CLOSED_DOCUMENTATION_ONLY | 127 与指定补正合读为 PASS_PENDING_LOCAL。 |
| R130-01 | CLOSED_DOCUMENTATION_ONLY | 183 要求的五替换、一追加、三字段和旧 P6 计数已交齐。131 新写的输入重建另记 R131-01。 |
| R130-02 | OPEN | 五例输出与两个返回来源核收；五份机器图仍误记取参拥有者。130 组合维持 RETURN。 |
| 128 / 129 | 历史裁定沿用 | R127-01、R128-01、R129-01 已文档闭合，128、129 各自的组合为 PASS_PENDING_LOCAL。 |

历史原件及原始 RETURN 留档。这里的关闭只针对列名的文档问题，完整本地 P9 为 NOT_RUN。 [130 历史裁定](CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md) · [130 主审](CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md) · [本批主审](CORE-USER-INIT-SHELL-LAUNCH-YIELD-131-review.md)

## R127-02 · 两份底本、四层顺序

从 128 固定 facts 的历史模型与同批 DOT 分别取六簇；两份底本均先套 129 的十二个旧→新标签，与 129 DOT 对照。套入 130 的 18 条旧边替换和 19 条新增边；按 131 的选择器，以 129 后实际生效值为匹配对象，重绑三组 B 的 iteration/footer，共六条。三组 B 的 switch 与 C127_M16_U3 的 switch/goto/body 六条保持 129 的标签。另三条 do0 沿用已核结果。各目标按 ID、旧值、边类型唯一匹配。逐 ID 和逐类型边比对 131.final_view、总机器图及 DOT，结果相同。[128 原图](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L354) · [129 选择器](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L679) · [131 重放](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L23)

| 投影 | 复核结果 |
|---|---|
| B02、B08、B16 | 10/9、10/9、10/10（节点/边）。B16 仅一条真实回边。 |
| 三个 C 簇 | 各 17/21。每簇均为四个宏位置、一次 helper 直接调用、一条原参返回数据边。 |
| 合计 | 81 个节点、91 条有类型边，端点缺失 0。43 个读→写的寄存器来源与 127 原轨迹相合，a0 为 20、a1 为 23。 |

一字样本 case 1 预读 a1，减少 len 后分支命中 do0，唯一目标区间 D[136,144)，循环体执行零次。B16 第一轮写 D0–6，第二轮写 D7–14，循环外 do0 写 D15。与固定 wordcopy.c 的分支、回边和循环外写位置相合，128/129 的已核差量未倒退。[case 1](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L82) · [循环尾与 do0](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L119) · [最终历史图](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L147)

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
do0:
	((op_t *)dstp)[0] = a1;
```

## R130-01 · 已补的旧要求及 131 新增的归属问题

| 原 130 窗 | 131 新证据 | 独核内容 |
|---|---|---|
| E003 | E012 | run_cmd 函数体内调用命令函数指针。 |
| E014、015、016 | E019–021 | 固定 errno.h 三个活动宏分别为 0x02、0x0d、0x42。 |
| E017 | E022 | 初始化器内部的字面零槽和 Success 字符串。 |
| E031 | E030、031 | 原写请求语义继续有效；printf 自己的 return rv 单独追加。 |

原 130 六件裸前缀 45，44 个正式窗另加 MANIFEST 第 24 行标题一次；旧 44/44/0 的声明不能代表原件上的裸计数。131 附的第 24 行 UTF-8 十六进制可还原旧整行。只把标题换成新句，保留原 44 个证据窗的虚拟读法，得到裸/正式 44/44。若构造包含所有修正且追加 return 为新窗的视图，五个一换一再加一得到 45/45。两者都不能回写为旧六件的实测。新增引文归 131 自身 P6。startup_selfcheck_quote、branch_canary_quotes、self_check 三项均已补记。[五替换、一追加与虚拟清单](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L1258) · [旧 MANIFEST](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md#L24)

131 虚拟清单所列 8 路径中，initd.c 无法由旧 130 的六件、181 合同或 130 主审证实。独立从旧 evidence 的 44 个正式标签抽出 11 种路径，对照虚拟清单共有 7 种，遗漏 stdio/errlist.c、string/memcpy.c、sysdeps/generic/memcopy.h、string/wordcopy.c（均在 myinitramfs/mylib/myglibc/ 下）。这是 131 新增回顾材料的来源归属问题，列 R131-01。下批可用“有旧正式窗的文件”和“其他由旧合同或正文记录的输入”逐一列据；作者当时未留下证据的内部阅读状态保留 UNKNOWN。[131 重建清单](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L1345) · [130 固定 evidence](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md)

## R130-02 · 已纠正的返回，以及五份仍待纠正的调用关系

旧五组 s1→write、write→ret 共十个选择器分别唯一命中。131 已分出两次字符串消费、格式化长度、write 请求及其被弃返回、printf 返回与命令自身返回，run_cmd 的命令函数指针调用丢弃命令结果。按固定格式将给定字符串拼回，包含 LF，末尾 NUL 不计入 write。[命令函数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188) · [五例值与旧边](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L1370)

| 失败样本 | 行内容（另加 LF） | write 请求 / printf 返回 | 命令返回源 |
|---|---|---:|---|
| cd /lost | cd: No such file or directory: /lost | 37 / 37 | 常量 1 |
| ls 默认目录 | ls: Permission denied: . | 25 / 25 | 常量 −1 |
| cat note | ls: No such file or directory: /note | 37 / 37 | 常量 −1；固定源码写了 ls 前缀 |
| rmdir /tmp/d | rmdir: Directory not empty: /tmp/d | 35 / 35 | rmdir 结果 err，所给失败为 −1 |
| rm /log | rm: Permission denied: /log | 28 / 28 | unlink 结果 err，所给失败为 −1 |

ls 的 write 若仅返回 2，printf 仍返回 25，命令返回 −1。rmdir/rm 的 argc≠2 分支直接返回 −1；源码中的删除和打印在其后。字面 [0]、两个活动 extern 和一处注释声明分别核实。真实 strerror 到数组、locale 与最终 sh 的链接状态 UNKNOWN。[两个参数门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L349) · [请求与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L17)

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
```

五例 DOT 第 207/239/271/303/335 行均从 printf 节点以 VARARG_FETCH0 指向首个 %s；facts.graph.edges 中五个同类型关系也如此。固定 printf.c:17–19 由 printf 建立并传递 va_list，具体 va_arg 在 vsnprintf.c:784 的 %s 分支，同一位置按格式先后执行两次。五张图也缺 printf→vsnprintf 及 printf→write 的调用，已有 rv→request 仅表达长度数据。补正限这五实例，两载体按端点、类型、拥有者交叉检查，保留各例数值及弃用关系。 [184 合同](../184-user-yield-task-selection-contract.md) · [首个错误边](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/init-shell-launch-yield-paths.dot#L207) · [机器件对应边](https://github.com/08822407d/MyOS2/blob/2089bfff6f4a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-init-shell-launch-yield-131/facts-and-boundaries.yaml#L3285)

完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机及真实进程、调度、文件操作均为 NOT_RUN。
