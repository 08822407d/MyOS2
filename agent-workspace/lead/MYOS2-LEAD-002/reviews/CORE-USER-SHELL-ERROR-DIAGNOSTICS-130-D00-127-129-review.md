---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
date: "2026-11-23"
date_timezone: "Asia/Shanghai"
phase: 2
conversation_display_name: "MYOS2-A-C09 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前会话标识）"
model_per_owner: "Pro（Owner 本条发令）"
actual_backend_model: "UNATTESTED"
actual_backend_effort: "UNATTESTED"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_before_publication_short12: "bbbe03f75da1"
execution_pin_short12: "7e52edee26b4"
read_channel: "connector"
full_local_P9: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
record_type: "phase2_historical_correction_adjudication"
followup_id: "CORE-USER-SHELL-ERROR-DIAGNOSTICS-130"
status: "R127_02_OPEN_R129_01_CLOSED_DOCUMENTATION_ONLY"
batch127: "RETURN"
batch128: "PASS_PENDING_LOCAL"
batch129: "PASS_PENDING_LOCAL_COMBINED"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/181-user-shell-error-diagnostics-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-D00-127-128-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/myshell/myshell.c","myinitramfs/myshell/tests.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/mylib/myglibc/stdio/printf.c"]
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
---

# 两项历史补正的独立裁定

| 对象 | 本轮裁定 | 适用范围 |
|---|---|---|
| R127-02 | OPEN；127 组合 RETURN | 18 条替换边、19 条新增边和 43 对寄存来源通过。跨批标签仍有两组六项。 |
| R129-01 | CLOSED_DOCUMENTATION_ONLY；129 组合 PASS_PENDING_LOCAL | 冻结 129 原件与 130 指定的日期差量合读。四例能逐步重建。 |
| R127-01、R128-01 | 已文档关闭 | 128 自身专题与指定勘误组合为 PASS_PENDING_LOCAL。 |

原裁定记载各批交付时的事实，继续保留。此次关闭限被点名的文档问题。 [129 的历史审查](CORE-USER-SHELL-DATETIME-FORMAT-129-D00-127-128-review.md) · [129 的日期审查](CORE-USER-SHELL-DATETIME-FORMAT-129-review.md) · [181 的 D00 要求](../181-user-shell-error-diagnostics-contract.md)

## R127-02 · 哪一步丢掉了已核的标签

按合同依次取 128 固定 facts 与 DOT 的同一批节点/边、应用 129 的 12 项 B/C 标签，再应用 130 的选择器。独立核对 127 的顺序表和 129 的寄存来源，43 次读→写的站点、寄存器、值及次序相符，其中 a0 为 20 对、a1 为 23 对。128 两种载体都能定位 43 对，129 的 12 个历史标签在 128 各命中一次。 [128 机器图](https://github.com/08822407d/MyOS2/blob/4a8952c6c956/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/facts-and-boundaries.yaml) · [129 的十二项标签](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L679) · [129 对应的图](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format-paths.dot#L154)

130 的 18 个旧边选择器在两种载体均唯一命中。加 19 条边和三个 original_dstpp 节点后，六簇共 81 节点、91 条有类型边，端点齐全。三个 C 簇各 17/21 节点/边，B02、B08 各 10/9，B16 为 10/10。每个 C 簇的外层四次宏位置、一次直接 helper 调用、原始 dstpp 的返回数据均可核收；B16 footer 的真回边也成立。各 B 簇循环主体只写 D0、D0..6、D0..14，循环外 do0 各写 D1、D7、D15 一次。 [130 的选择器](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml#L394) · [130 的投影](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot#L44) · [外层调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L40) · [helper 调用位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h#L125)

| 待补的 12 个节点 | 实际对照 | 第 131 批的核收标准 |
|---|---|---|
| B127_02、08、16 各自的 iteration 与 footer，共 6 个 | 130 指定的 old_label 仍取自 128。套完 129 后未命中。9 个 B 选择器中仅 3 个 do0 的旧值仍匹配。 | 用 129 生效标签作旧值，逐项精确匹配一次，再应用 130 的主体/尾部新值。六个节点都要验证实际结果。 |
| B127_02、08、16 各自的 switch，共 3 个 | 130 DOT 127、148、169 行又写作 switch 选择 do1 或 do7。129 已给出 case 2/0/0、预取、调整、显式跳转的次序。 | 保留三个已核 switch 标签，验证不会随新差量回退。 |
| C127_M16_U3 的 switch、goto、body，共 3 个 | 130 DOT 52、55、56 行恢复了 128 文字。尤其 body 重列交替读写和 footer。 | 保留 129 已核的单字路径。预取 a1、调整 len、直接跳到循环外 do0，写 D[136,144)，helper 返回 void。 |

即使按 ID 强行替换第一组六个节点，下面六个遗失标签仍然存在。严格重放必须同时验证选择器的旧值与输出标签。131 只追加可应用的节点差量，把合成结果与 facts、DOT 两种表示逐项对比。 [原失配的回放说明](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml#L599) · [单字预取与跳转](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L82) · [循环外写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/wordcopy.c#L126)

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		dstp -= 0 * OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8; /* No-op.  */
```

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
do0:
	((op_t *)dstp)[0] = a1;
```

## R129-01 · 四组案例与存储顺序

用固定 tests.c:124 的格式、vsnprintf 的状态分支和 format_int 的填充/倒序赋值重建每例 27 次迭代，共 108 步。旧表的共享控制保留，iter=25 的 signed long 实参按完整 case_id 绑定。逐行比较格式位置、控制状态、指针与输出位置、取参结果；再以源码顺序生成存储事件，写回缓冲并比较旧、新最终偏移映射。 [129 共享解析表](https://github.com/08822407d/MyOS2/blob/824ce6b5607a/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/facts-and-boundaries.yaml#L2418) · [130 的分例差量](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml#L606) · [格式现场](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/tests.c#L124) · [数字写入](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L185)

| 完整 case_id | 第 25 步实参 | 填 0 次数 | 倒序写数字的偏移 | 六位字段 |
|---|---:|---:|---|---|
| DT129-000000 | 0 | 5 | 39 | 000000 |
| DT129-000007 | 7 | 5 | 39 | 000007 |
| DT129-012345 | 12345 | 1 | 39,38,37,36,35 | 012345 |
| DT129-999999 | 999999 | 0 | 39,38,37,36,35,34 | 999999 |

前 34 个字节为前缀、日期字符串和句点。四例共 164 条数据存储，序号、偏移、字符和机制逐项相符；再计每例独立的终止 NUL，共 168 次输出缓冲存储。每例 LF 在 40、NUL 在 41，vsnprintf 返回 41，printf 请求 41 字节并返回 41。四个日期图簇的尾部顺序与表一致。原错误簇的主调及弃用返回值处已在新正文和机器件勘误到 myshell.c:68；tests.c:110–113 属于被调函数的失败路径。日期取样、时区、运行库链接及实际 write 结果仍属未证。 [分例图](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot#L188) · [作者的地址勘误](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md#L21) · [实际主调](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L68)

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
		val /= base;
```

**裁定生效范围**为上述已有专题与指定文档补正。R129-01 已文档关闭。完整本地 P9、预处理、编译链接、仓库脚本、Graphviz、QEMU、实机及实际文件操作为 NOT_RUN。
