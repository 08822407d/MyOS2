---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_correction_review"
date: "2026-11-22"
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
lead_before_publication_short12: "a0687ad53a3b"
execution_pin_short12: "4a8952c6c956"
read_channel: "connector"
followup_id: "CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128"
status: "PARTIAL_CLOSURE"
R127_01: "CLOSED_DOCUMENTATION_ONLY"
R127_02: "OPEN"
batch127_composite: "RETURN"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/179-user-libc-dest-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-D00-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-aligned-word-copy127-reviewed-dest-aligned-word-copy128.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
---

# 第127批两项历史补正的独立裁定

**R127-01关闭为 `CLOSED_DOCUMENTATION_ONLY`。R127-02仍为 `OPEN`，第127批组合裁定仍为 `RETURN`。** 本次分别以127固定六件和128固定六件为输入，核表行、源码访问及替代图的实际节点和边。[127原审](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md) · [128作者补正](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078730513) · [128独立审查](CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-review.md)

## R127-01 · 10行原位重建

C08对127主报告第75–81、95–97行逐行验证旧正文，套用128机器表的10条替代行，再和128可读报告逐行比对。原17处未展开值在组合报告中降为0。分别按127操作序列的余数、首读变量、末游标、读写数和末写位置复核八列A表与七列B表。[127原表](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md#L71-L97) · [128替代表](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md#L36-L63) · [逐行变换](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L24-L296)

| 输入字数 | 0 | 1 | 2 | 7 | 8 | 9 | 16 |
|---|---:|---:|---:|---:|---:|---:|---:|
| 入口余数 | 0 | 1 | 2 | 7 | 0 | 1 | 0 |
| 首读S0至 | 无 | a1 | a0 | a1 | a0 | a1 | a0 |
| 逻辑读数＝写数 | 0 | 1 | 2 | 7 | 8 | 9 | 16 |
| 末写目的字 | 无 | D0 | D1 | D6 | D7 | D8 | D15 |

B表的2、8、16字末写分别为D1、D7、D15，寄存变量均为a1。S0按小端显示为 `0xa090807060504030`。此次关闭只覆盖表格，原件及当时作者评论继续保留。[127机器事实](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L494-L741) · [余数0和1](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L75-L89)

## R127-02 · 已成立的路径与剩余缺口

128历史替代视图有13簇、224个节点、254条有类型边。C08把机器件的全部节点ID、标签及全部边的两端、类型、说明与DOT对齐，两种载体相符。A组非零例已有先预读后调整再显式goto的顺序，A00直接返回。16字的首轮末写之后，真分支经游标和字数更新、循环判断回到do8，末轮假分支到循环外do0。43条源读→目的写的端点配对也全部正确。[1、2字](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L17-L67) · [16字](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L248-L353) · [替代图索引](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L297-L365)

**尚有43个写节点标成“from undefined”，43条对应数据边标成“stored in undefined”。** 相同的86处也出现在机器件替代图的标签/说明中。它们是实际输出字段，区别于历史表格为说明修正而保留的旧字符串。从每条边起点的真实READ取寄存变量，并按该案例中旧facts的WRITE.from_reg及固定赋值交叉核对，应为a0的有20对，a1的有23对。1字例的唯一结果来自a1；2字例的D0来自a0，D1来自a1；16字例D15来自a1。[缺失标签样本](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L22-L65) · [原始读写与寄存来源](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L534-L740)

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	do1:
		a1 = ((op_t *)srcp)[7];
		((op_t *)dstp)[7] = a0;
```

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	/* This is the right position for do0.  Please don't move
	   it into the loop.  */
do0:
	((op_t *)dstp)[0] = a1;
}
```


三个B簇的switch标签写成选do标签，实际先按余数选case。iteration摘要重复包含入口预读和do0末写，图上又单列了它们；将其限于循环体可使每步只读一次。历史C的单字M16-U3直接goto do0，只做一次末写，通用body摘要中的交替循环和footer不适用于它。C组三个替代簇还应保留原有MEMCPY拥有的helper调用、四个宏位置和原始dstpp返回数据关系，宏间先后另标。三个旧C原图已有这些来源，可按比较结果定点恢复。历史图余下部分继续使用，180号要求交可重建的差量。[B、C替代摘要](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L354-L521) · [127原C图](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L797-L895) · [外层各操作与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L35-L59)

R126-01此前的文档关闭继续有效。全部裁定限于静态阅读，完整本地P9与运行均为 `NOT_RUN`。
