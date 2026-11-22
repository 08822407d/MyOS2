---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "historical_correction_review"
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
status: "PARTIAL_CLOSURE"
followup_id: "CORE-USER-SHELL-DATETIME-FORMAT-129"
R127_01: "CLOSED_DOCUMENTATION_ONLY"
R127_02: "OPEN"
R128_01: "CLOSED_DOCUMENTATION_ONLY"
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/180-user-shell-datetime-format-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/myshell/tests.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-D00-127-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-dest-aligned-word-copy128-reviewed-shell-datetime-format129.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot"]
self_check: {"verified_claims":1,"quotes_reconfirmed":1,"downgraded_to_inferred":0}
---

# 第129批带回的两项历史补正 · C08分别裁定

## R128-01 文档关闭

把129机器件三份替代账本逐例套回128固定facts。每个目的位置恰覆盖一次；外层helper调用数均为1，helper内的字写作为子事件单列。原第二例5个前导、3个尾部单字节事件完整保留。128原A/B及C的源码/数值结论，连同本次经核的替代账本与图，组合为 `PASS_PENDING_LOCAL`。128携带的上一代R127-02仍独立跟踪。[facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L7650-L7697) · [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L1066-L1130) · [非对齐来源helper及调用方游标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L121-L131)

| 源／目的／长度 | 原账宽度和 | 新账写区（半开） | helper内字写数 |
|---|---:|---|---:|
| 129／128／16 | 32 | [128,144) 共16 | 2 |
| 130／131／16 | 16 | [131,147) 共16 | 1 |
| 131／128／41 | 201 | [128,169) 共41 | 5 |

18条旧边的端点、旧类型及说明命中固定128图；新类型和说明命中129替代图。新增3个原dstpp节点及12边。每例四个宏位置、一条MEMCPY拥有的直接helper调用；helper返回void之后，外层拿自己的游标、字节数推进。返回值来自原始dstpp。R128-01为 `CLOSED_DOCUMENTATION_ONLY`。[libc-dest-aligned-word-copy-paths.dot](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L904-L1085) · [shell-datetime-format-paths.dot](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot) · [原参数返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L51-L59)

## R127-02 部分成立，余项仍OPEN

以128历史版 `D00.R127_02.graph` 与同提交DOT为替代底本，以原127的ordered_trace作独立来源。已核43对源READ和目标WRITE、值、变量、站点、旧标签及旧边选择器；a0有20对、a1有23对。新映射修复86个字段。12项B/C标签均指中旧节点；B的case/预读/调整/goto、C127-M16-U3单字直接进入do0已写明。R127-01此前的关闭继续有效。[libc-dest-aligned-word-copy-paths.dot](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L17-L67) · [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L679-L758) · [127原始寄存来源](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L534-L740)

129差量明确保留原边类型、端点和其余旧边，实际重放后的三个簇各只有一条owner→prefix宏边，另有prefix→page、page→word、callerupdate→tail误标为宏，word→helpercall误标为直接C调用，tail→result误标为返回数据。原127的三个簇已有正确调用拥有者与原参来源。按 `C127_M16_A`、`C127_M16_U3`、`C127_M129_A` 精确补回源关系。每簇从owner出发连prefix/page/word/tail四处，位置memcpy.c:40/45/51/57；直接调用从owner到helpercall，memcopy.h:125。原dstpp从owner或入口实参流向result，memcpy.c:59。相邻阶段的边分别标先后或条件。B三例iteration仅记循环体写区，footer、外部do0归各自节点；特别是B127_16_iteration当前仍把两次footer及do0 D15写进自身。A的43对无需重做。127组合仍为 `RETURN`。[libc-dest-aligned-word-copy-paths.dot](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L389-L521) · [三个原C簇](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L797-L895) · [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L679-L758)

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
```

下一批在新文件登记可对固定旧件实际应用的差量，并读回序列化的facts及DOT逐簇计数。上述裁定仅覆盖文档。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。
