---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_correction_review"
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
status: "CLOSED_DOCUMENTATION_ONLY"
return_id: "R126-01"
batch126_composite: "PASS_PENDING_LOCAL"
batch127_review: "RETURN"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/178-user-libc-aligned-word-copy-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-memory-copy-fill126-reviewed-aligned-word-copy127.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/myshell/myshell.c"]
---

# R126-01独立裁定 · 第126批图件补正成立

**R126-01关闭为 `CLOSED_DOCUMENTATION_ONLY`。第126批固定六件、127的D00替代视图与此前C08两处文字勘误组合为 `PASS_PENDING_LOCAL`。** 原126独立回执仍记录当时的RETURN。127新增专题的表格和流程图另有裁定。[127独立回执](CORE-USER-LIBC-ALIGNED-WORD-COPY-127-review.md) · [126原审与文字勘误](https://github.com/08822407d/MyOS2/blob/8ad13b4fdfc7/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md) · [126原图](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot) · [补正映射](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml#L21-L478)

## 从旧图重建

以126固定DOT为底，按完整case_id归簇，去除S09簇中10个属于S09-U3的重复声明。按机器件的旧端点、类型、标签逐一定位10条删边，加入17条边、8个新节点，分别执行其余节点改名、边重分类及标签替换。比较节点ID、标签、有效形状和带重数的全边集合，所得历史视图与127的D00子图一致。原图三个open终点继承默认box，新图显式写box。[D00子图](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L5-L462) · [126机器事实](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml)

| 阅读版本 | 节点声明／唯一 | 有类型边 | C调用 | 所选复制宏位置 |
|---|---|---:|---:|---:|
| 冻结126原图 | 194／184 | 211 | 40 | 12 |
| D00历史视图 | 192／192 | 218 | 40 | 13 |

- M00的字节宏请求到达零轮分支；从入口遍历不能到达旧空区间读、写节点。两个游标分别留在S+1、D+2，返回原D+2。M16顺序为第40行前导BYTE、45行空PAGE、51行WORD及两字helper、57行尾BYTE；两处BYTE各零轮。四个A案例各1、1、1、4处，三条cat路径各两处，合计13。[复制位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L35-L59)
- 六个memset实例的末游标到返回处均为顺序边。返回数据各自来自保存的原始dstpp。S09-U3末游标D+12，返回D+3；零长例无目的写，返回D+0。S09与S09-U3各有11个归属自己的节点。[填充案例声明](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L65-L148) · [入口与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L22-L84)
- 三组清零、第一次复制、第二次复制的区间依次为 [0,9)、[0,2)、[1,8)；[0,8)、[0,5)、[5,8)；[0,9)、[0,5)、[5,9)。对应宽度分别为9/2/7、8/5/3、9/5/4。三条第二次复制的长度边现在取自argv[1]的求长加一。斜杠条件及操作沿用原表。[三个路径子图](https://github.com/08822407d/MyOS2/blob/77a1e903d9af/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot#L149-L229) · [cat_command](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L270)

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	return dstpp;
}
```

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```


126旧正文“8整字”按一次8字节整字写消费。E023引到下标1–5共五条赋值，整组八次赋值由相邻窗口合看。这两处已由126原审给出明确读法，本裁定纳入组合版本。[旧正文](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md#L128-L138) · [E023](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md#L284-L296) · [整组赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L47-L62)

关闭范围为文档语义。完整本地P9与运行均为 `NOT_RUN`。
