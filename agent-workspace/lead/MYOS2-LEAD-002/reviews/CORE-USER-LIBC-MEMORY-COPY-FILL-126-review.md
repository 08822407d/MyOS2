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
lead_before_publication_short12: "18ab2e5dfb0f"
execution_pin_short12: "345d0f32374c"
batch125_composite: "PASS_PENDING_LOCAL"
batch126_review: "RETURN"
closed_documentation_only: ["R125-01","R125-02"]
open_returns: ["R126-01"]
followup_id: "CORE-USER-LIBC-MEMORY-COPY-FILL-126"
author_status: "READY_FOR_MAINLINE_REVIEW"
return_scope: "R126-01: typed case graph, labels and associated graph counts"
assigned_to_successor: "CORE-USER-LIBC-ALIGNED-WORD-COPY-127"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-STRING-COPY-APPEND-125-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-22-copy-append125-reviewed-memory-copy-fill126.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/conventions.md","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/string/memset.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/strcpy.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/mylib/myglibc/string/strncpy.c","myinitramfs/myshell/myshell.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md"]
---

# 第126批独立验收 · 算术复核与图件定点退回

**C08裁定 `RETURN`，本批新增一项文档退回 R126-01。** 四个复制、六个填充、三个路径案例的字节、游标、写区和返回结果可在原合同的对象及库定义前提下采用。图中表示宏位置、访问范围和返回值来源的若干边须追加更正。两项第125批历史返修已分别裁定为 `CLOSED_DOCUMENTATION_ONLY`。[177号合同](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/lead/MYOS2-LEAD-002/177-user-libc-memory-copy-fill-contract.md) · [PR17 #7075488931](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7075488931) · [历史裁定](CORE-USER-LIBC-MEMORY-COPY-FILL-126-D00-125-review.md)

## 可采用的静态结果

| 专题 | C08从固定源码、给定对象和初值独立复算 |
|---|---|
| A · memcpy | 长度0、3、15对应字节循环0、3、15次；既定源/目的起点为(1,2)、(4,3)、(2,5)，末偏移为(1,2)、(7,6)、(17,20)。长度16两端从偏移8开始，在空页宏、字宽8及helper按约返回条件下传入2个字；外层末偏移(24,24)，返回D+8。 |
| B · memset | 按“对齐前导字节、64字节组数、其余8字节字数、尾字节”记账。对齐长度0/7/8/9/64分别为(0,0,0,0)、(0,0,0,7)、(0,0,1,0)、(0,0,1,1)、(0,1,0,0)。D+3开始、长度9为(5,0,0,4)，末游标D+12，返回D+3。给定c=0x123，各字节为0x23。 |
| C · cat_command | 三例分配9/8/9字节，写宽总和18/17/19，写过的不同位置有9/8/9个。每例有两次以同值复写的字节，均计入写宽。按字节记录写者、次数和首NUL，与125的十二个完整缓冲区快照一一相符。第270行参数分别含 /readme、/bin/hi、/bin//hi。 |

源和目的为合法、不重叠对象，F容量和对齐是案例输入。C08核对了四组完整40字节目的对象、六组88字节目的对象和十二次路径缓冲区状态。16字节例沿用177明确允许的整字helper正常返回前提。[A/B/C原报告](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md#L88-L149) · [机器案例](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml#L448-L2078) · [返回原始目的指针](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L54-L60) · [路径请求](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L270)

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		len -= (-dstp) % OPSIZ;
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

[固定源码 39–40](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L39-L40)

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
		((byte *)dstp)[0] = c;
		dstp += 1;
		len -= 1;
	}
```

[固定源码 78–81](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L78-L81)

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	return dstpp;
}
```

[固定源码 83–84](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L83-L84)

## R126-01 · 给后继的修正范围

| 对象 | 原图可观测差异 | 更正后的判据 |
|---|---|---|
| A126-M16 | 守卫直接连到页宏。第40行的前导字节宏位置漏画，只画了第57行的零长度尾宏。原图12条宏边计数属实，按这组案例中选取的复制宏位置还少一处。 | 将第40行和第57行画成独立位置。顺序为前导BYTE、空PAGE、WORD（其中有2字helper请求）、尾BYTE。前导和尾的字节循环均为零。宏在文本中的调用位置、预处理及案例实际读写分别说明。 |
| A126-M00 | 第245–247行有从入口可达的READ、WRITE和游标更新路径，数量写作0。 | 0字节的宏请求可达；本例内存读、内存写和游标递增均不发生。用守卫/零轮路径或明确的空区间汇总表达，保持返回D+2。 |
| B组六例 | 将最后局部dstp节点到返回值画为RETURN_DATA。例D+3/9中，D+12与实际返回D+3有明确数值差。另有10个U3节点在S09和S09-U3两个簇内重复声明。 | 最后游标到返回动作只表达先后；返回数据来自原始dstpp。按完整case_id归簇，一份声明属于一个案例。全部六例包括零长均按同一来源解释。 |
| C组三例 | 清零、第一次复制、末次复制共有9个标签使用闭右括号。C125-02把容量8的清零写成[0,8]、追加写成[5,8]，与其标注宽度8、3冲突。第二次求长的数据标签误写argc。 | 按机器表使用半开区间，求长对象为argv[1]。三组依次是[0,9)、[0,2)、[1,8)；[0,8)、[0,5)、[5,8)；[0,9)、[0,5)、[5,9)。可选斜杠另按原守卫登记。 |

原图文本有194次案例节点声明、184个唯一节点、211条有类型边。40条直接函数调用边逐一核过拥有者，39条来自三个cat实例及其库函数，1条是MEMCPY按通用头选择的helper调用。13个案例入口皆可达指定终点，根目录斜杠写不可达，另两例可达。上述计数只描述作者交付的图。新视图须分别注明其计数、语义和适用范围。[两处重复归簇](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot#L100-L148) · [九个写区标签](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot#L164-L224) · [短复制和阈值分支边](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot#L241-L282) · [填充返回边](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot#L283-L341) · [前导与尾部两个位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memcpy.c#L35-L59)

**两处表述按源码收窄。** 正文“8整字”读为一次8字节整字写，大小9时另有一次尾字节写。E023标题“中间四字”实际引到了下标1、2、3、4、5五条赋值；E022–024连读可还原一组全部八次赋值。这两处均不影响已复算的数值。[C组表](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md#L128-L138) · [E023引文](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md#L284-L296) · [八次赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/memset.c#L47-L62)

## 远端实物与引用核查

固定执行提交 `345d0f32374c` 唯一父 `bd1372961eaa`，compare前进1、落后0，六件纯新增3578行、164917 UTF-8字节。按Git对象头和全文重算六个Blob，均与固定提交、活动分支、树及差异清单相符。两件机器可读文件均可按JSON解析，MANIFEST列出的29个输入路径逐项核对。直接master公约原句和time/master配置、panic金丝雀均吻合。

| 文件 | 字节 | Git Blob短12 |
|---|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/MANIFEST.md) | 9955 | e0c02ff13640 |
| [libc-memory-copy-fill.md](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md) | 17357 | 4061ddcadc99 |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/facts-and-boundaries.yaml) | 79053 | d401cc30623a |
| [libc-memory-copy-fill-paths.dot](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill-paths.dot) | 35491 | 32ca343ebf22 |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/deferred-findings.yaml) | 6612 | f2a1c4fb3e52 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md) | 16449 | f41626a8f176 |

126的P6字面前缀分布为0/0/0/0/0/37，结构化引文37个，全部与8份固定源文件的符号、行界和逐字内容相符。P6为37/37/0。用户侧构建仅作来源定位。审查前PR17顶层无since分页100/100/40/0，共240条，126正式作者标记恰好一次，按ID两读相同。四直接ref为lead `18ab2e5dfb0f`、exec `345d0f32374c`、time `a039d9803ade`、master `e8e60cd19540`；PR16 Open/Ready、PR17原题Open/Draft，均至master、未合并。实际sh链接、真实宏和open结果未知。完整本地P9、预处理、编译链接、Graphviz、QEMU、实机及真实文件操作均为 `NOT_RUN`。[126证据](https://github.com/08822407d/MyOS2/blob/345d0f32374c/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/evidence.md) · [协议v2](https://github.com/08822407d/MyOS2/blob/18ab2e5dfb0f/agent-workspace/tasks/00-gpt-task-protocol-v2.md) · [PR17 #7075488931](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7075488931)
