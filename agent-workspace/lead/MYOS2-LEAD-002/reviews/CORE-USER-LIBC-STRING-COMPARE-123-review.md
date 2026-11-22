---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_bounded_batch_review"
status: "RETURN"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
read_channel: "connector"
base_snapshot: "time"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
execution_pin_short12: "16398b8b96c6"
lead_before_publication_short12: "561261755177"
batch122_composite: "PASS_PENDING_LOCAL"
batch123: "RETURN"
closed_documentation_only_this_round: ["R122-01"]
open_returns_this_round: ["R123-01"]
followup_id: "CORE-USER-LIBC-STRING-COMPARE-123"
new_return: "R123-01"
full_local_P9: "NOT_RUN"
preprocess_compile_link_Graphviz_QEMU_hardware_real_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":3,"quotes_reconfirmed":3,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/evidence.md","agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-STDIO-OPEN-FLAG-ABI-122-D00-120-121-review.md","myinitramfs/mylib/myglibc/string/strcmp.c","myinitramfs/mylib/myglibc/string/strncmp.c","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/string/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/open.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/libio/fileops.c","myinitramfs/mylib/myglibc/libio/iofopen.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
---

# 第123批独立验收 · 字符串比较和三个 shell 调用现场

**主线裁定 `RETURN`，唯一开放项为 R123-01。** 26个案例的精确整数、读取字节对数和 shell 分支均可在指定前提下使用。cd 和 ls 的实例图把比较后的控制转移标成函数直接调用，需追加纠正。历史 R122-01 的裁定另见[同提交的D00回执](CORE-USER-LIBC-STRING-COMPARE-123-D00-122-review.md)。[173号合同](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/lead/MYOS2-LEAD-002/173-user-libc-string-compare-contract.md) · [PR17 #7067185003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7067185003)

## 源码与复算

选择仓内默认 STRCMP/STRNCMP 定义并给定合法可读的字符串。C08独立按两份函数的退出条件取字节，再从 shell 的全局表重建遍历顺序。共核 A 六例、B 九例、查找六例、cd 两例、ls 三例。机器件合计122条已读字节对记录，逐对核下标、两字节与差值。六个查找场景共有38次 strcmp 调用。正文26行与机器案例对应。独立 mylibc 的源码收集、顶层 initramfs 的外部 musl 工具路径、sh 目标的实际链接结果各有不同证据；真实 sh 采用的函数实现仍为 UNKNOWN。[逐案例机器件](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml) · [仓内字符串库源清单](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/CMakeLists.txt#L1-L3) · [库目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/CMakeLists.txt#L1-L21) · [顶层配置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L38-L51) · [sh 目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L10)

| 所查现场 | 主线复算结果 |
|---|---|
| strcmp | 同名 pwd 读4对并返回0。rm/rmdir 读3对得 −100，反向得 +100。字节 0x80 与 0x7f 的差为 +1。 |
| strncmp | abcdX/abcdY 的 n=0、3、4、5 分别读0、3、4、5对，返回0、0、0、−1。abXd/abYd 给 n=4 只读3对。n=8 的双整组例读8对。 |
| find_cmd | 按 (调用次数，读取对数，索引) 记，pwd=(3,6,2)，pw=(10,12,−1)，rm=(6,8,5)，rmdir=(8,15,7)，rmd=(10,16,−1)，cd=(1,3,0)。 |
| cd_command | 比较实参为(".",path)。给 "." 得0，直接返回1；给 "./" 得−47，到 chdir 请求点。运行结果未知。 |
| ls_command | 给一条有效记录、下一次 readdir 返回空。在 "."、".."、".config" 三例，第二次比较分别为未调用、0、53；文件名输出次数0、0、1。循环结束另有一次换行请求。 |

上述精确整数来自仓内实现。shell 的分支只读取相等性。例如同一对 abcdX/abcdY 在 n=4 预算用完时返回0，不能据此认定完整名称相等。仓内 strcmp 的首字节为 NUL 和不同字节两种出口都返回差值。[strcmp 两出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strcmp.c#L29-L44) · [strncmp 四次检查和余数](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/string/strncmp.c#L31-L71) · [十项命令表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L56) · [find_cmd](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L144-L152)

[VERIFIED myinitramfs/myshell/myshell.c::find_cmd]
```c
	for(i = 0;i<sizeof(shell_internal_cmd)/sizeof(builtincmd_s);i++)
		if(!strcmp(cmd,shell_internal_cmd[i].name))
			return i;
	return -1;
```

[固定源码 148–151](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L148-L151)

## R123-01 OPEN · shell 实例图的调用者与返回值

DOT 实读8簇、244个唯一节点、241条有类型边，端点齐全。从33个案例入口遍历，每个均可达自己的终点，未跨案例。静态来源层的19条直接调用和2条间接调用成立。实例层须额外检查边的起点代表谁。第497–513行在 cd/ls 五例中有12条 DIRECT_CALL_INSTANCE，其中包含案例ID、strcmp 调用结果、跳过输出的状态及前一次 printf 作为起点。应给每例标出 cd_command 或 ls_command 的函数实例，再记录其实际发起的调用。[五个实例的原图](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L497-L513) · [原图节点含义](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L247-L271)

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	/////.
	if(!strcmp(".", path))
		return 1;

	i = chdir(path);
```

[固定源码 199–203](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L199-L203)

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
	}
	printf("\n");
```

[固定源码 236–240](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L236-L240)

| 现场 | 实际调用者和结果的去向 |
|---|---|
| C123-D01/D02 | case 给定 cd 实例。cd 在200行调用 strcmp。D01 零值触发 cd 的 return 1；D02 非零让 cd 到203行调用 chdir。比较器到 chdir 只有控制依赖。 |
| C123-L01/L02/L03 | case 给定 ls 实例。ls 在236行调用第一次 strcmp，条件成立才在237行调用第二次。238和240行的两个 printf 调用各属 ls，240另受下一次 readdir 返回空控制。L01 到不了第二次比较和名称输出，L02 到不了名称输出，L03 两个输出按先后发生。 |
| 六个 C123-F 终点 | 比较器向 find_cmd 返回差值；find_cmd 根据是否等零、是否走到表尾，自己返回索引或 −1。原图最后一次比较到 out 的 RETURN 须区分两层。例 pwd 的末次 strcmp 返回0，find_cmd 返回2；pw 的末次比较返回−2，find_cmd 最终返回−1。 |

追加一张小图和同一组 case_id 的机器表即可。调用边只从真实函数实例发出；分别检查每条必要/禁止的调用对、每个案例实际发生的调用以及未调用字段。第122批历史图的独立勘误已归入本轮D00回执。第123批整图须与本项的后继补正合读后再验收。[find 终点和两种返回](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot#L415-L442) · [find_cmd 两种返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L147-L152)

## 交付真实性及清单勘误

执行提交 `16398b8b96c6` 唯一父 `7d0e810ec4ff`，compare 前进1、落后0。六件全为新增，共4559行、164647个UTF-8字节。固定提交与活动分支全文相同，独立按 UTF-8 内容和 Git blob 头重算 SHA-1 并对照提交树、diff 与两次文件读取。[作者清单](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/MANIFEST.md) · [PR17 #7067185003](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7067185003)

| 文件 | 字节 | Git Blob短12 |
|---|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/MANIFEST.md) | 10666 | ee349bfd8764 |
| [libc-string-compare.md](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare.md) | 16271 | 6b7c2019872f |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/facts-and-boundaries.yaml) | 70912 | 8b2bb04aac16 |
| [libc-string-compare-paths.dot](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare-paths.dot) | 37157 | f74ab4dca583 |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/deferred-findings.yaml) | 7050 | 312f4c51be23 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/evidence.md) | 22591 | 75df8750b88f |

MANIFEST 第132行预写日志误列 e1272a3f9b69。主线据 Git 提交对象、compare、清单第18行和作者回执，把该行的拟用父更正为 **7d0e810ec4ff**。124的执行父为本次123的实际头 **16398b8b96c6**。原始日志保留供追溯。[原日志](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/MANIFEST.md#L128-L134) · [执行提交](https://github.com/08822407d/MyOS2/commit/16398b8b96c6)

P2有59个正式短窗，按给定物理行逐字对应8个固定源文件，均在所标定义或活动 CMake 命令内。P01–P15另计。六件前缀数0/0/0/0/0/59，P6为59/59/0。两份JSON兼容YAML可解析，37个清单项目输入路径有对应对象。直接master公约原句、time/master的两对金丝雀均回源比对，六件全文检查40位十六进制串及协议禁用的成功措辞均零命中。作者评论按ID两读一致，写前顶层分页100/100/33/0，作者全标记只出现一次。开放PR清单为16和17，分别Ready和Draft，均指向master且未合并。[原始证据](https://github.com/08822407d/MyOS2/blob/16398b8b96c6/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/evidence.md) · [协议v2](https://github.com/08822407d/MyOS2/blob/561261755177/agent-workspace/tasks/00-gpt-task-protocol-v2.md)

完整本地P9、预处理、编译链接、Graphviz、QEMU、实机和真实文件操作均为 `NOT_RUN`。
