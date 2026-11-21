---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CAT-FILE-DISPLAY-117"
record_type: "independent_shell_cat_review"
status: "RETURN"
produced_by: "GPT-6 Astra Pro"
model_per_owner: "Pro（Owner选择）"
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
phase: 2
read_channel: "connector"
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "b2f563a67814"
execution_pin_short12: "52c51a3752d3"
batch115: "RETURN"
batch116: "RETURN"
batch117: "RETURN"
R115_01: "CLOSED_DOCUMENTATION_ONLY"
R115_02: "CLOSED_DOCUMENTATION_ONLY"
R115_03: "OPEN"
R116_01: "OPEN"
R116_02: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "OPEN"
R117_02: "OPEN"
R117_03: "OPEN"
author_status: "READY_FOR_MAINLINE_REVIEW"
author_write_state: "STOP_WRITING"
source_windows_verbatim: 88
full_prefix_occurrences: 89
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","mykernel/fs/vfs/read_write.c","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/drivers/char/mem.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c","mykernel/fs/vfs/readdir.c","mykernel/fs/fat/myos_fat32.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/entry/syscall_64.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strcpy.c","mykernel/fs/syscall.c","mykernel/mm/vm_map/filemap.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/mylib/myglibc/posix/unistd.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/include/asm/unistd_64.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c","mykernel/fs/vfs/file.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h","mykernel/include/linux/lib/errno.h","mykernel/scripts/options_flags.cmake","agent-workspace/tasks/00-gpt-task-protocol-v2.md"]
---

# 第117批独立验收 · shell 的 cat

**主线裁定 `RETURN`。** A的三种路径、B/C的七个读取与输出模型、三条资源路径按列明前提成立。报告未给出七个首NUL位置，正式证据计数有误，CAT图把若干先后动作画成直接调用。R117-01、02、03 交118追加。旧项另见[115/116补正裁定](CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md)。[固定166](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md) · [117固定六件](https://github.com/08822407d/MyOS2/tree/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117) · [117作者交付 #7057046328](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057046328)

## 远端事实与检查范围

执行提交 `52c51a3752d3` 唯一父 `145d6781c75e`，compare 为领先1、落后0、一个提交、六新增4135行、旧件零改删。固定与执行活动头六件全文及Git blob 6/6相同。PR17按ID两读 #7057046328，与无since分页100/100/21/0的同条正文相同，作者标记唯一。开放列表仅PR16 Ready、原题PR17 Draft，均以 master 为 base 且未合并。[117作者交付 #7057046328](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057046328) · [166发布 #7056768574](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056768574)

按证据账逐条切固定源码原始物理行，保留原行尾，88/88与所引窗口一致，均为1–5行，来自16份源码。88项 ledger 的路径、符号文本和行号与 evidence 对齐；35个普通位置均在文件行界内。两份结构化文件可解析，8条候选、8处缺口。两组分支金丝雀、公约原句、MANIFEST输入路径均可回查，六件未见40位十六进制串。实际 DOT 153个唯一节点、193条边，全部端点有声明。C08 另外检查十组有向路径及关键边的调用者和条件；连通、定义内归属、证据支持分别判断。完整本地P9、预处理、Graphviz、编译、链接、QEMU和实机 `NOT_RUN`。

## 可消费的源码结论

限定路径字符串有效且分配成功。`/` + `note`、`/docs` + `note`、`/docs` + `/note` 分别请求7、11、12字节，拼成 `/note`、`/docs/note`、`/docs//note`。打开失败使用 `ls:` 前缀，依次传入 strerror(errno)、文件名，返回−1；具体错误文字未证。普通成功时两次 seek 后一次读，第二次 seek 的返回、read 保存到 len 的值均不决定后续 `%s` 的输出长度。[路径与打开](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L260-L275) · [两次定位、一次读及收尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L276-L286)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	i = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	buf = malloc(i + 1);
	memset(buf, 0, i + 1);
	len = read(fd, buf, i);
```
[myshell.c L276–280](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L276-L280)

在稳定小文件、两个 seek 成功、缓冲有效、给定 read 写入前缀的条件下，从预清零区重建每例缓冲，按首NUL截取，追加一个 LF。七例独立复算与两份机器字段逐项相同。B07另设负返回没有修改缓冲。

| 模型 | 文件长度 | 给定 read 返回 | 首NUL下标（从0起） | 请求字节（转义） | 请求长度 |
|---|---:|---:|---:|---|---:|
| B01 | 3 | 3 | 3 | `ABC\n` | 4 |
| B02 | 3 | 3 | 1 | `A\n` | 2 |
| B03 | 4 | 2 | 2 | `AB\n` | 3 |
| B04 | 2 | 2 | 2 | `A\n\n` | 3 |
| B05 | 0 | 0 | 0 | `\n` | 1 |
| B06 | 4 | 0 | 0 | `\n` | 1 |
| B07 | 3 | -5 | 0 | `\n` | 1 |

printf 使用 vsnprintf 返回的 rv 作为 stdout 写请求长度并返回 rv，未消费 write 结果。cat 在所读正常及 read 负返回分支都调用 close 后返回1；函数体未见 free 两次分配的内存。open 失败时只已分配 filename。不能由调用推出关闭成功或终端已显示。[printf](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L11-L26) · [首NUL与输出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/vsnprintf.c#L783-L809) · [cat完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L252-L286)

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
	return rv;
```
[printf.c L21–25](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L21-L25)

## 退回项及关闭条件

**R117-01 · 报告字段缺值。** 报告74–80行的首NUL列七次为 `undefined`。源值同时在 facts.B.models[].first_NUL_index 与 C.output_cases[].first_NUL_at。118追加完整八列表，依B01–07逐项写入3、1、2、2、0、0、0，用实际缓冲字节独立求首个0，与请求字节、长度、printf返回及两处机器字段核相等。[原报告表](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md#L72-L80) · [机器字段](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L1323-L1460)

**R117-02 · 引文与自检计数。** 六件按任务书顺序有1/0/0/0/0/88次正式前缀，共89次。第89处在 MANIFEST:113 的计数说明，头部35行只报88。保留88个逐字窗与89次原前缀两个原数，不把说明行当新证据。另补 CT117-P05 的真实范围：syscalls_64.h:8–17 覆盖 lseek，但 read/write/open/close 实例在4–7行。P05行界有效不足以证明 read 分派。新包逐件核最终工件及标签、定义归属、普通位置语义；涉及旧 E040/E046/E048 的处置归 R116-01。[自检前缀](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md#L113) · [普通位置](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md#L1021-L1035) · [调用号映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L4-L11)

**R117-03 · CAT 图的真实调用者。** 图291–292及312行把 malloc 到 memset、memset 到 strcpy 的程序顺序标作直接调用，实际三者均由 cat 调用。图320把 vfs_read 到 generic_file_read_iter 标成间接调用。源码先从 vfs_read 直接进入 new_sync_read，再由后者经 filp->f_op->read_iter 调用；FAT32_file_ops 提供方法绑定。图316另将用户入口、switch、生成的系统调用体和 ksys_read 压成单边。118在实际 DOT 补三类边：调用主体，执行先后，数据/绑定。给 read 分派标出真实映射/入口或具名的已核摘要边；回调返回值、给定写入前缀和已证运行效果分别标明。正常、短读、open失败和迭代零计数路径应可逐边回源。[CAT图](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot#L287-L327) · [VFS选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L154-L165) · [同步助手](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L83-L92) · [FAT方法表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L354-L364) · [read系统调用体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/syscall.c#L11-L14)

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	// ret = call_read_iter(filp, &kiocb, &iter);
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
```
[read_write.c L87–91](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L87-L91)

该裁定只影响报告、索引和图的上述位置。七个输出是格式化/写请求模型，真实 shell、文件和终端行为仍待运行证据。
