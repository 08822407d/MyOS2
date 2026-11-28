---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_errata_adjudication"
status: "CLOSED_DOCUMENTATION_ONLY"
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
lead_before_publication_short12: "18ab2e5dfb0f"
execution_pin_short12: "345d0f32374c"
batch125_composite: "PASS_PENDING_LOCAL"
batch126_review: "RETURN"
closed_documentation_only: ["R125-01","R125-02"]
open_returns: ["R126-01"]
adjudicated: ["R125-01","R125-02"]
original_125_receipt: "RETURN"
composite_125_verdict: "PASS_PENDING_LOCAL"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":2,"quotes_reconfirmed":2,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/string/strcat.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/mylib/myglibc/string/strncpy.c"]
---

# 第125批两项勘误的独立裁定

**R125-01、R125-02 均为 `CLOSED_DOCUMENTATION_ONLY`。** 第125批固定原件、原主线回执和126追加勘误合读为 `PASS_PENDING_LOCAL`。第126批新图件的 R126-01 单独退回。[125原主线裁定](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md) · [PR17 #7074685583](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7074685583) · [PR17 #7075488931](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7075488931) · [126独立验收](CORE-USER-LIBC-MEMORY-COPY-FILL-126-review.md)

## R125-01 · 从固定六件实际重建

C08对 `bd1372961eaa` 的六件逐字计数，找出MANIFEST第105行说明中的一次字面命中。按 `345d0f32374c` 机器勘误逐行替换MANIFEST的35、36、37、100、105行；前四处校对旧行，第105行以固定Blob、物理行和实际字面命中确定位置。按11个唯一证据ID替换章节标题及标签，保留原CMake短窗、源文件和行界，并应用该记录的普通定位说明。以下为在临时文本中真正执行重建后的统计。[五处清单替换](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml#L59-L100) · [逐ID分类及替换规则](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml#L101-L292) · [125原清单](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md#L98-L107)

| 核查对象 | 六件逐文件字面分布（MANIFEST、正文、facts、DOT、deferred、evidence） | 引文核对 |
|---|---|---|
| 125固定原件 | 1/0/0/0/0/34，共35 | 34个含路径、符号及引文的结构化窗口逐字命中。清单的额外一次为说明文字。 |
| 125组合阅读版本 | 0/0/0/0/0/23，共23 | E001–021为21个C函数窗，E030–031为2个time配置窗。复核23/23/0。 |
| 126自己的固定六件 | 0/0/0/0/0/37，共37 | 本批37/37/0，独立核算。 |

E022–029及E032–034共11个用户侧构建位置各有一条真实命令；降为普通定位后仍可追源。E025设置MUSL_PATH，E034才将其用于CMAKE_C_COMPILER。125作者评论保留原有记录。引用其中的“34”时，须区别结构化窗口数与全件字面计数。[125原构建窗口](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md#L279-L405) · [176合同](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md) · [计数规则](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/tasks/00-gpt-task-protocol-v2.md#L55-L67)

## R125-02 · 将每个零追到最后一次写入

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

[固定源码 263–267](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L263-L267)

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
STRCPY(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}
```

[固定源码 29–32](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L29-L32)

在三个给定对象上，按清零、复制cwd及条件赋值依次更新字节与最近写者。对旧facts的三个JSON路径分别核原值、替代值、守卫和写集。三个最终缓冲区、内外返回地址与open的三个实参也已与旧facts和本轮D00核对。[三个原字段与替代值](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml#L295-L447) · [根目录原字段](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml#L484-L495) · [实际open请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L267-L270)

| 125案例 | 斜杠步骤写集 | 此后首NUL与最近写者 | 原字段→组合字段 | open请求的路径 |
|---|---|---|---|---|
| C125-01 | 空 | F[1]，第一次strcpy内的memcpy | 1 → null | /readme |
| C125-02 | [4,5)写斜杠 | F[5]，memset | 5 → 5 | /bin/hi |
| C125-03 | [4,5)写斜杠 | F[5]，memset | 5 → 5 | /bin//hi |

根目录例只修改第一个案例的 `stages[2].new_NUL_from_prior_memset_offset` 阅读值。初始化者仍是memset；第一次复制写入同值也更新最近写者。另两例覆写旧NUL使F[5]成为第一个零，该字节此前只经历清零。至此，两项旧退回已满足177指定的独立关闭条件；完整本地P9与运行继续为 `NOT_RUN`。
