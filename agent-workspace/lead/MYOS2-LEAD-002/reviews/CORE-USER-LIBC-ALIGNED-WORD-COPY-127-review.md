---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_mainline_review"
phase: 2
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "8ad13b4fdfc7"
execution_pin_short12: "77a1e903d9af"
followup_id: "CORE-USER-LIBC-ALIGNED-WORD-COPY-127"
author_status: "READY_FOR_MAINLINE_REVIEW"
status: "RETURN"
open_returns: ["R127-01","R127-02"]
R126_01: "CLOSED_DOCUMENTATION_ONLY"
batch126_composite: "PASS_PENDING_LOCAL"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-memory-copy-fill126-reviewed-aligned-word-copy127.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/myshell/myshell.c"]
---

# 第127批独立验收

**C08裁定 `RETURN`。** 七个入口、三个循环样例和三个外层请求的整字来源、目的下标、字节与返回值通过固定源码复算。退回集中在两处可定位的文档问题。消费原件时用下表读取主报告，并以机器件的操作序列和固定源码读取跳转顺序。R126-01已独立关闭。[历史裁定](CORE-USER-LIBC-ALIGNED-WORD-COPY-127-D00-126-review.md) · [178号合同](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md) · [127作者交付 #7078015466](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078015466)

## R127-01 · 两张表的字段

主报告第75–81行的七行各有两个未展开的 `undefined`，且六个非零案例都把已经发生的入口预读取成横线。第95–97行的末写目的字三处也是 `undefined`。两表合计17处字面占位符。机器件有对应真值。下表的源、目的字序号相对各自的起始偏移128。[两张原表](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md#L71-L97) · [0至2字操作记录](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L494-L741) · [入口分派](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L32-L89)

| 案例字数 | 余数 | 入口预读 | 首个跳转目标 | 调整后源偏移/目的偏移/剩余字数 | 逻辑读/写字数 | 末写目的字 |
|---:|---:|---|---|---|---|---|
| 0 | 0 | 无 | 直接返回 | 无调整，128/128/0 | 0/0 | 无 |
| 1 | 1 | S0→a1 | do0 | 136/128/0 | 1/1 | D0 |
| 2 | 2 | S0→a0 | do1 | 80/72/8 | 2/2 | D1 |
| 7 | 7 | S0→a1 | do6 | 120/112/8 | 7/7 | D6 |
| 8 | 0 | S0→a0 | do7 | 128/120/8 | 8/8 | D7 |
| 9 | 1 | S0→a1 | do8 | 136/128/8 | 9/9 | D8 |
| 16 | 0 | S0→a0 | do7 | 128/120/16 | 16/16 | D15 |

B的三行末写分别是D1、D7、D15，值来自末次读取的a1。16字首轮写D0–D6，回边后写D7–D14，循环外写D15。C08从入口预取、八个标签的实际偏移和底部更新单独生成读写顺序，核对了A所有43次读和43次写的源码位置、有效地址、寄存变量、先前读来源及数值，以及全部43条图中对应的寄存数据边。以小端解释机器件十六进制整字，逐字节复制关系独立于该显示约定。[16字全序列](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L1534-L2003) · [循环与末写](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L92-L128)

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 1:
		a1 = ((op_t *)srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
```

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = a1;
}
```


## R127-02 · 流程图的入口预读与控制顺序

A组六个非零例把带“entry doN”的 CONTROL_GOTO 接到了入口预读节点。1字例图中 do0 标签出现在预读之前，而源码第83行预读、84–86行调整、87–88行判断并跳转，最后第127行才写D0。2字例同理，预读发生在第39行，然后才跳到do1。跨过游标调整、跳转或循环底部的边，原图却标作“next source statement”。16字的D6写到下一次读S8处既有这条顺序边又有回边，实际中间执行了119–122行。B组三个循环退出是条件为假后顺序落到do0；C组三条摘要中“helper entry doN”节点位于第一次读取之前，应改成函数入口/分派，或把目标标签移到预读之后。区分switch选支、源码goto、循环守卫与下一次访存即可，无需逐语句绘图。事实记录中16字的第二个GOTO按循环回边解读；该处源码是do/while条件。[1、2字入口](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L473-L507) · [16字与B组](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L658-L795) · [三个外层摘要](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L797-L895)

## 三个外层请求

源/目的分别是合法、互不重叠的384字节对象。选用通用头、8字节op_t、阈值16和空PAGE。按原源字节公式复算每个请求的完整字节串，核对helper每次源读和目的写、外层推进、尾部、原始指针返回。[WORD宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L119-L131) · [memcpy](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L28-L59) · [三请求](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L2436-L3359)

| 初始偏移／长度 | 前导字节／helper字数／尾字节 | helper写区间 | 最终外层目的游标／返回 |
|---|---|---|---|
| 128／16 | 0／2／0 | [128,144) | D+144／D+128 |
| 131／16 | 5／1／3 | [136,144) | D+147／D+131 |
| 128／129 | 0／16／1 | [128,256) | D+257／D+128 |

## 提交、引用与范围

执行头 `77a1e903d9af` 唯一父 `345d0f32374c`。一提交六件纯新增，5185行、221316 UTF-8字节。固定、活动全文与重算Git Blob逐件相符。两件机器表可解析，25个清单输入均在对应树中。直接master公约、time/master两处金丝雀吻合。结构化源码窗44个，全部核对符号、原行和1–5行连续正文；六件字面分布0/0/0/0/0/44。全图366节点、441边，其中127新增174节点、223边、3条直接调用、12条外层复制宏边，端点齐全。结构计数与控制语义分别核查。验收前PR17顶层评论100/100/42/0，作者标记唯一，按ID双读相同。[MANIFEST.md](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md) · [evidence.md](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md) · [作者评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078015466)

完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。实际sh所链接的库与生效宏仍未知。两项文档退回为OPEN，179号安排追加补正。原作者六件及评论冻结。
