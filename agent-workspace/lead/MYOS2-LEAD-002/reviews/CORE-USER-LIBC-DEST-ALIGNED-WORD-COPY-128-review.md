---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_mainline_review"
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
status: "RETURN"
author_status: "READY_FOR_MAINLINE_REVIEW"
R127_01: "CLOSED_DOCUMENTATION_ONLY"
R127_02: "OPEN"
new_returns: ["R128-01"]
open_returns: ["R127-02","R128-01"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/179-user-libc-dest-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-ALIGNED-WORD-COPY-127-D00-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-aligned-word-copy127-reviewed-dest-aligned-word-copy128.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
---

# 第128批独立验收

**C08裁定 `RETURN`。** 小端64位、独立且容量充足的两个384字节对象、8字节整字和空PAGE分支等合同前提下，13个案例的复制值成立。C组机器件有两例重复登记同一调用，三例图上的返回数据来源也需要纠正。新增R128-01仅限C组记录。历史两项单独裁定，R127-01关闭，R127-02保留OPEN。[179号合同](https://github.com/08822407d/MyOS2/blob/a0687ad53a3b/agent-workspace/lead/MYOS2-LEAD-002/179-user-libc-dest-aligned-word-copy-contract.md) · [128作者交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078730513) · [历史裁定](CORE-USER-LIBC-DEST-ALIGNED-WORD-COPY-128-D00-127-review.md)

## 逐字节与逐整字复算

从固定函数的四个余数入口分别设置两个预读变量、源和目的局部游标及剩余字数，从八字节源字的值计算64位移位和按位或。每次写回再与原源字节的直接切片比较，独立核对数据来源图中的68条低字/高字边及其端点。A/B/C合计46次完整源字读取、34次整字目的写；C组另有前导和尾部的字节操作。[固定helper](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L139-L214) · [字宽与MERGE](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L63-L74) · [单字实例](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L3288-L3519)

| 专题 | C08复核结果 |
|---|---|
| A · 单字 | S+129、131、135的位移分别为8/56、24/40、56/8。两次加载均读[128,136)、[136,144)，各向D[128,136)写一次。输出首末字节分别为81–88、83–8a、87–8e（十六进制）。 |
| B · 四字展开 | 0、1、2、3、4、5、8字的源读/目的写分别为0/0、2/1、3/2、4/3、5/4、6/5、9/8。正数字数首跳依次do0、do1、do2、do3、do4、do3。8字第一轮写3字，底部len=4；第二轮写4字，len=0；循环外以a2/a3写D[184,192)。 |
| C · 外层 | 三次请求各选一次目的对齐helper。前导字节／整字数／尾字节依次为0/2/0、5/1/3、0/5/1，完整源字加载并集为[128,152)、[128,144)、[128,176)。三个目的写区依次为[128,144)、[131,147)、[128,169)，返回D+128、D+131、D+128。 |

用三份初始全0xCC的目的对象按实际分段重建，三个请求区的16、16、41字节均等于各自源切片；区外哨兵均保持。第二例的整字阶段源为S+135、目的为D+136。B组逐写记录的机器键为 `dest_word_stores`，报告“events/stores”中的后者按此正式键读取。[专题表](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md#L76-L148) · [八字操作账](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L6392-L7416) · [外层调用及返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L35-L59)

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_DEST_ALIGNED]
```c
		srcp += 4 * OPSIZ;
		dstp += 4 * OPSIZ;
		len -= 4;
	} while (len != 0);
```

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	/* There are just a few bytes to copy.  Use byte memory operations.  */
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```


## R128-01 · 三个外层调用的事件与数据来源

原facts的 `C_outer_memcpy.cases[].byte_copy_trace` 将整段helper调用按它写出的字数重复登记。第一个案例有两行相同的16字节整段，第三个有五行相同的40字节整段。下表按数组中 `width` 或 `byte_width` 实际求和；对照同件的stages、helper逐字写区与源码，三者可还原真实分段。[第一个账本](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L7650-L7697) · [第二个账本](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L7876-L7966) · [第三个账本](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml#L8361-L8455) · [一次helper调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L121-L131)

| 源/目的/请求字节 | 原账helper调用条目 | 原账宽度和 | 正确调用数／整字写数／总字节 |
|---|---:|---:|---|
| 129/128/16 | 2 | 32 | 1／2／16 |
| 130/131/16 | 1 | 16 | 1／1／16 |
| 131/128/41 | 5 | 201 | 1／5／41 |

保持第二例可作对照。第一、三例用一次整段调用，加上既有的独立逐字子表；或逐字登记互不重叠写区，另列一次调用。任一单层写账可直接求和为请求字节数。

三例DOT各自有一条正确的MEMCPY→helper直接调用边和四条从MEMCPY出发的宏位置边。另有沿前导、PAGE、WORD及尾部串接的四条边也标为MACRO_EXPANSION；这些边表达各阶段的到达顺序。三处helper返回void→外层游标更新的CALLER_DATA应为先后关系，更新值来自调用者持有的游标和字节数；三处尾BYTE→函数返回的RETURN_DATA也应为先后关系，返回值来源为入口dstpp。核对 `returnvoid`、`callerupdate`、`tail`、`result` 节点及其数据来源。加载与写入的细节子图按数据依赖阅读。[前两例图及类型](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L904-L1007) · [第三例图](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot#L1008-L1085) · [外层游标更新](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L124-L130) · [返回原指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L56-L59)

## 交付身份与证据范围

执行头 `4a8952c6c956` 唯一父 `77a1e903d9af`，比较领先1、落后0，六件纯新增10419行、415857个UTF-8字节。固定与活动六件全文、字节及独立重算的Git Blob均一致。清单列出的25个输入路径可定位，公约与两套分支金丝雀已对照。两份机器件可解析。39个正式窗口覆及五份固定来源，逐一核对路径、定义、连续1–5行正文与行号；六件的字面标签数为0/0/0/0/0/39。新图225节点295边，加历史子图共449节点549边，全部端点存在。PR17审查前顶层分页100/100/44/0，作者标记唯一，按ID双读一致。作者所述422恢复按作者记录引用，已独立核实恢复后的提交及净差。[MANIFEST.md](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/MANIFEST.md) · [evidence.md](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/evidence.md) · [作者正式评论](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7078730513)

完整本地P9、预处理、编译链接、Graphviz、QEMU、实机与真实文件操作均为 `NOT_RUN`。实际sh链接的实现及生效宏仍未知。
