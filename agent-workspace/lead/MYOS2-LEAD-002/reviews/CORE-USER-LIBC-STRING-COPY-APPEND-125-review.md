---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_bounded_batch_review"
status: "RETURN"
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
lead_before_publication_short12: "cbbb416dae74"
batch125_author_short12: "bd1372961eaa"
batch125_mainline: "RETURN"
batch125_D00_at_author_delivery: "NONE"
open_returns: ["R125-01","R125-02"]
followup_id: "CORE-USER-LIBC-STRING-COPY-APPEND-125"
author_status: "READY_FOR_MAINLINE_REVIEW"
reviewer_status: "RETURN"
correction_destination: "CORE-USER-LIBC-MEMORY-COPY-FILL-126"
D00_issued_for_next: ["R125-01","R125-02"]
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-D00-123-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-length-copy124-reviewed-copy-append125.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/mylib/myglibc/string/strncpy.c","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/facts-and-boundaries.yaml"]
---

# 第125批独立验收 · 两项文档补正

**C08裁定 `RETURN`。** 九个案例的复制区间、返回地址、最终路径及调用归属通过静态复核。R125-01涉及协议v2按字面前缀计数的硬性验收项，R125-02涉及根目录例一个机器字段的写入来源。两项均为 `OPEN`，由第126批按177号任务书追加补正。125作者当时收到的D00为空。[176号原合同](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/176-user-libc-string-copy-append-contract.md) · [PR17 #7074685583](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7074685583) · [177号后继合同](../177-user-libc-memory-copy-fill-contract.md)

## 两项退回与消费范围

| 编号 | 固定原件及独立结果 | 后继验收依据 |
|---|---|---|
| R125-01 | MANIFEST第35–37行登记34个标签，逐件分布列MANIFEST=0。按协议匹配六件文件的所有字面前缀，实际为 **1/0/0/0/0/34，合计35**。多出的一次在MANIFEST第105行的中文说明中。34个具有路径、符号和引文块的标签均在evidence，逐字回源34/34。 | 保留原件实际35次及34个有效格式引文。追加勘误给出第105行的无前缀替换，并按176号将11个用户侧构建窗口改列普通定位。若只去掉说明中的误命中，计数为34；完成证据分类后，替代视图正式引文应为21个C函数窗加2个内核配置窗，逐件复算23/23/0。126自己的六件另计。 |
| R125-02 | C125-01的 `after_optional_separator.new_NUL_from_prior_memset_offset=1` 与本例的操作顺序不符。先清零整个F，第一次strcpy又把根目录的终止零写在F[1]，随后守卫为假，本步写集为空。此时首NUL仍在1，最后写它的是第一次strcpy中的memcpy。 | 补出可解析的案例、更正字段和写入者。根目录例应无“本步新出现的、来自先前清零的NUL”（该字段取null或等义有类型表示）。普通目录两例中F[4]被改成斜杠，F[5]上次写入确实来自memset。保留两例作对照。 |

原件里的程序案例可以按下述条件引用。引用125的P6合规状态、根目录NUL来源或整批通过状态时，以本回执的退回为准。R123-01维持此前的文档关闭。[清单自报](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md#L34-L37) · [清单额外命中](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md#L103-L106) · [协议v2 P6](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/tasks/00-gpt-task-protocol-v2.md#L55-L67) · [P9第5项](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/tasks/00-gpt-task-protocol-v2.md#L77-L85) · [争议字段](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml#L481-L495) · [正文对照](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md#L79-L89) · [124历史裁定](https://github.com/08822407d/MyOS2/blob/cbbb416dae74/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-LENGTH-COPY-124-D00-123-review.md)

## 从固定源码重算

[VERIFIED myinitramfs/mylib/myglibc/string/strcpy.c::STRCPY]
```c
STRCPY(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}
```

[固定源码 29–32](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcpy.c#L29-L32)

[VERIFIED myinitramfs/mylib/myglibc/string/strcat.c::STRCAT]
```c
STRCAT(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
```

[固定源码 28–32](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcat.c#L28-L32)

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

[固定源码 263–267](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L263-L267)

C08以独立的字节数组计算，先写入调用给定的初始对象和哨兵，再逐次比较范围、字节和返回指针。源与目的对象各自有效且不重叠，使用仓内所选定义及8字节整字模型。A组三例的复制量为1、4、3，目的写区依次为[0,1)、[0,4)、[2,5)，返回D、D、D+2。B组内层返回D、D+2、D+2，外层均返回D。B125-02把一个零写回原本为零的D[2]，最终值相同，这次复制确实发生。[A/B正文与实参](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md#L36-L63) · [memcpy末尾和返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L54-L60)

| 案例 | 请求容量 | 斜杠判断后首零及最后写者 | 追加写区 | 传给open的路径 | 最终NUL后剩余字节 |
|---|---:|---|---|---|---:|
| C125-01：/ 和 readme | 9 | F[1]，第一次strcpy的memcpy | [1,8) | /readme | 1 |
| C125-02：/bin 和 hi | 8 | F[5]，memset | [5,8) | /bin/hi | 0 |
| C125-03：/bin 和 /hi | 9 | F[5]，memset | [5,9) | /bin//hi | 0 |

三例共十二个缓冲区快照及其首NUL位置、写区均与正文和机器表吻合。每例shell直接发起7次调用（其中两次strlen），两次strcpy及一次strcat实例在所选定义下合计再发起6次调用。求长共5次，memcpy共2次。C08独立生成每个调用应有的拥有者并对照图中57条直接调用边，A/B/C三组分别为6/12/39。图有120个唯一节点、196条有类型边，端点均已定义。九例终点均可达，三个斜杠存储节点从本例入口的可达性依次为假、真、真。最终路径只说明第270行传给open的参数。[C组正文及嵌套调用](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md#L64-L107) · [三组案例图](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot) · [open实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L264-L270)

## 文件、引文及发布证据

原执行提交 `bd1372961eaa` 以 `00c0228c52b8` 为唯一父，前进1、落后0，六件纯新增2277行，共103228个UTF-8字节。以下每件均以正文和Git对象头重算Blob，对照固定读取、活动分支、提交树和compare。机器件均按JSON兼容的YAML解析，MANIFEST的32条结构化输入在相应树中逐项找到。四条分支金丝雀及直接master公约原句回源吻合。[PR17 #7074685583](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7074685583) · [固定清单](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md)

| 固定文件 | UTF-8字节 | Git Blob短12 |
|---|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md) | 9544 | fd77b1eb9695 |
| [libc-string-copy-append.md](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md) | 11130 | 3a6e3812ccac |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml) | 33916 | b9abb3ccc3ca |
| [libc-string-copy-append-paths.dot](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot) | 31442 | a31ccfb037cd |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/deferred-findings.yaml) | 3738 | fccf1ff20595 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md) | 13458 | 389c3003121a |

34个结构化引文分布于11个源文件，每窗1至5行，逐字和起止行均相符。E001–021为21个C函数定义范围内的窗口，E022–029和E032–034为11个用户侧构建命令，E030–031为2个time内核C配置窗口。构建命令的证据限于目标、目录或配置，例如E025只设置MUSL_PATH，E034才把该路径用于编译器变量。176号明确要求用户库构建来源另列普通定位，11个用户侧构建标签纳入R125-01一并补正。构建标签后的通用“调用者和返回表达式”说明应按各命令本身收窄解读。头文件声明单独定位。[构建证据](https://github.com/08822407d/MyOS2/blob/bd1372961eaa/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md#L279-L405) · [用户侧工具和目录](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L40-L48) · [sh构建目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)

审查前PR17无since分页100/100/38/0，共238条。作者正式标记唯一，按ID两读相同。四个直接ref为lead `cbbb416dae74`、exec `bd1372961eaa`、time `a039d9803ade`、master `e8e60cd19540`；PR16 Open/Ready、PR17原题Open/Draft，均以master为base、未合并。实际sh所链libc、有效宏、真实文件解析和open结果仍未知。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。
