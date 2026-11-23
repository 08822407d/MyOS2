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
record_type: "phase2_independent_mainline_review"
followup_id: "CORE-USER-SHELL-ERROR-DIAGNOSTICS-130"
status: "RETURN"
open_returns: ["R130-01","R130-02"]
historical_dispositions_in: "CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md"
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/181-user-shell-error-diagnostics-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md","myinitramfs/myshell/myshell.c","myinitramfs/myshell/tests.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/mylib/myglibc/string/wordcopy.c","myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h","myinitramfs/mylib/myglibc/string/memcpy.c","myinitramfs/mylib/myglibc/stdio/errlist.c","myinitramfs/mylib/myglibc/stdio/errlist_internal.c","myinitramfs/mylib/myglibc/include/stdio.h","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h","myinitramfs/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/mylib/myglibc/stdio/CMakeLists.txt","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0}
---

# 第 130 批独立验收 · 五条 shell 错误诊断

**C09 裁定 RETURN，限 R130-01、R130-02 两项文档补正。** 五个给定样本的输出字节、请求长度和命令返回值成立。三个活动错误码缺少合格的作者引文，五张诊断图把写请求画成了返回值的来源。两项均可随第 131 批追加。 [181 号合同](../181-user-shell-error-diagnostics-contract.md) · [130 作者回执 #7088362162](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088362162) · [两项历史补正裁定](CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md)

## 六件交付与作者身份

执行者为 MYOS2-A-C08c／CHAT-EXEC-20。固定提交 7e52edee26b4 的唯一父为 824ce6b5607a。GitHub 比较为领先 1、落后 0，共 1 个提交，只有合同指定目录内六项新增，共 3,947 行。按固定提交及活动执行分支分别读全文，比对 UTF-8，并从内容重算 Git Blob；以下六项均与远端一致。六件合计 **121,585 字节**。PR17 的 249 条顶层评论中作者全身份正式标记恰在 #7088362162；按 ID 两次读取的正文与分页相同。作者在评论中更正了 MANIFEST 第 32 行所回顾的旧哈希，129 的 evidence.md 实读短号为 8f67d9549b0f。此处采纳勘误。 [提交比较](https://github.com/08822407d/MyOS2/compare/824ce6b5607a...7e52edee26b4) · [130 作者回执 #7088362162](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7088362162)

| 文件 | 新增行 | UTF-8 字节 | 核算的 Blob 前 12 位 |
|---|---:|---:|---|
| MANIFEST.md | 42 | 5,414 | 2260d3de3b1e |
| shell-error-diagnostics.md | 57 | 5,890 | b321a5329b48 |
| facts-and-boundaries.yaml | 3,025 | 75,669 | dd01054c3b2b |
| shell-error-diagnostics-paths.dot | 278 | 23,925 | f27ed095e5e7 |
| deferred-findings.yaml | 39 | 1,425 | 4d1095dfbe8f |
| evidence.md | 506 | 9,262 | da6b1e97f096 |

## 五项条件结论

C09 从各命令自己的格式字面量、数组中对应的错误文本及合同给定的路径重新拼出完整 ASCII 与十六进制，再核对两次取参、LF、NUL、请求和返回。下表五个加数依次为命令前缀、说明、冒号加空格、路径和 LF。给定缓冲充足，LF 位于请求末字节，输出 NUL 位于紧随的偏移，两份源字符串各自的结尾 NUL 均未复制。真实 sh 选用的 libc、实际 errno、locale 和输出设备仍待证。 [命令实现](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L188) · [字符串转换](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783) · [写请求和返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L17)

| 命令与本例输入 | 失败守卫及输出（行尾另加 LF） | 字节加数 | LF / NUL 偏移 | printf / 命令返回 |
|---|---|---|---|---|
| cd /lost | chdir 返回 −1；cd: No such file or directory: /lost | 4+25+2+5+1=37 | 36 / 37 | 37 / 1 |
| ls，默认路径 . | opendir 返回 NULL；ls: Permission denied: . | 4+17+2+1+1=25 | 24 / 25 | 25 / −1 |
| cat note，当前目录 / | open 返回 −1；ls: No such file or directory: /note | 4+25+2+5+1=37 | 36 / 37 | 37 / −1 |
| rmdir /tmp/d | argc=2，rmdir 返回 −1；rmdir: Directory not empty: /tmp/d | 7+19+2+6+1=35 | 34 / 35 | 35 / −1 |
| rm /log | argc=2，unlink 返回 −1；rm: Permission denied: /log | 4+17+2+4+1=28 | 27 / 28 | 28 / −1 |

cat 的目录长度是 1，于是直接追加 note，传入第二个转换项的五字节是 2f 6e 6f 74 65，源码前缀确为 ls。候选格式化器先后从 va_list 取两个 const char *，各按 strlen 给出的长度逐字节复制。这个顺序只描述格式化器；调用方计算实参表达式的先后未经限定。给定 ls 的底层 write 返回 25 或 2，两次 printf 均返回 25；ls_command 均返回 −1，run_cmd 忽略它。 [cat 构造路径和报错](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260) · [run_cmd](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139) · [格式化器复制](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L805)

在 0、±2、±13、±66 七个给定值上，__get_errlist 的负值分支先取相反数，再按下标访问数组。0 对应字面下标，文本长度分别为 Success 7、No such file or directory 25、Permission denied 17、Directory not empty 19；七例的独立核算与机器件的 ASCII、hex 和长度相同。只在给定有效下标和三个条件表项成立时使用这些结果。 [数组开头](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/errlist_internal.c#L23) · [取下标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/errlist.c#L27)

顶层配置指定 musl-gcc、musl 头文件目录并加入 myinit 和 myshell；myshell 声明 sh。另一棵 mylib/myglibc 配置声明 mylibc 静态目标，经 stdio 子目录收集源码。已读配置和头文件能支持上述独立事实；sh 实际链接到哪个实现、strerror 是否调用 __get_errlist 尚无证明。 [顶层配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L40) · [sh 目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L4) · [stdio 源归集](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/CMakeLists.txt#L1)

## R130-01 · 活动定义、短引文与自检账

逐一比对 11 个固定源码路径，44 个带路径、符号和行窗的块均为原文件中连续 1–5 行；保留了原制表符及 CR。此结果只核文本。E130-014/015/016 在 errno.h 的 235/246/300 行引用了带双斜线的旧值 0x40000002、0x4000000d、0x40000042。实际活动定义在 424/435/489 行。E130-003 带入 run_cmd 的函数签名，E130-017 带入数组声明头。替代点分别是 myshell.c:141 与 errlist_internal.c:25。E130-031 覆盖写请求，但其窗口止于 printf.c:24，需另列第 25 行的返回。第 131 批用新增引文替代相应旧证据。 [旧引文账](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/evidence.md#L175) · [活动宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h#L424) · [run_cmd 体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L141)

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOENT]
```c
	#define ENOENT			0x02
```

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::EACCES]
```c
	#define EACCES			0x0d
```

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOTEMPTY]
```c
	#define ENOTEMPTY		0x42
```

两份机器件均可按 JSON 解析。六件顺序按上表，协议 P6 的原始前缀计数是 **1/0/0/0/0/44，共 45**。其中 MANIFEST 第 24 行引用了 P2 标题。正式带路径的窗口仍为 44。MANIFEST 将二者都按 44 记，并以正文代替 startup_selfcheck_quote、branch_canary_quotes、self_check 三个机器字段；read_channel 有值。实际读到的 time 编译标志和 panic 语句与直接 master 区分成功；原公约句也在正文中。补正应列旧计数及其出处，并给新的、可解析的字段和两行逐字核对用的金丝雀。131 自己的引文单独计数。原六件 40 位十六进制扫描为 0；完整本地 P9 仍为 NOT_RUN。 [作者 MANIFEST](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/MANIFEST.md#L24) · [P2/P3/P6/P9](../../../tasks/00-gpt-task-protocol-v2.md)

## R130-02 · 返回值的来源及命令的前置条件

五个 ERR 簇都把第二个 %s 节点指向 write 请求，又把 write 节点以 RETURN_SOURCE 指向含 printf 和命令两个返回值的合并节点。可在原 DOT 的 205–206、222–223、239–240、256–257、273–274 行复现。独立扫描整图得 137 个声明节点、135 条有类型的边，端点均有声明；其中五诊断簇 40 节点、35 边。结构检查不能签收这些边的语义。第 131 批按五个案例分别补齐直接调用者、数据来源、弃用及顺序的关系。 [五个诊断簇](https://github.com/08822407d/MyOS2/blob/7e52edee26b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics-paths.dot#L192)

- printf 发起 write；vsnprintf 的 rv 同时给请求长度和 printf 返回。write 的返回值被丢弃。
- cd 返回常量 1，ls/cat 失败时各返回常量 −1。rmdir/rm 在 argc≠2 时直接返回 −1；通过参数门后将底层结果保存到 err，只在 err≠0 时打印，最后返回原 err。run_cmd 只弃用命令返回。诊断表达式的两个字符串是格式化数据。
- B 表中输入 0 的 `symbol: "SUCCESS"` 应标为显示标签或空符号，源里用于选取该槽的写法是 [0]。include/stdio.h:184、186 是数组和 __get_errlist 的活动声明，string/string.h:416 的 strerror 声明为注释。分别记作声明/注释定位，保留实际链接 UNKNOWN。 [两个参数门](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L349) · [活动声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/include/stdio.h#L183) · [带注释的声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/string.h#L416)

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	write(stdout->_fileno, buf, rv);
```

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	return rv;
```

本轮不需要 Owner 裁决源码修复。R130-01、R130-02 均为 OPEN，结果只作合同内静态取证。完整本地 P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机和真实文件操作均为 NOT_RUN。
