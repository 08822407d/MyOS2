---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-DIR-STREAM-LS-116"
record_type: "independent_directory_stream_ls_review"
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
lead_before_publication_short12: "e96b1079ff0d"
execution_pin_short12: "145d6781c75e"
batch115: "RETURN"
R115_01: "CLOSED_DOCUMENTATION_ONLY"
R115_02: "CLOSED_DOCUMENTATION_ONLY"
R115_03: "OPEN"
batch116: "RETURN"
R116_01: "OPEN"
R116_02: "OPEN"
author_status: "READY_FOR_MAINLINE_REVIEW"
author_write_state: "STOP_WRITING"
conditional_candidates: 8
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MEMDEV-SCALAR-IO-115-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/evidence.md","mykernel/scripts/options_flags.cmake","mykernel/include/linux/kernel/limits.h","mykernel/arch/x86_64/mm/page/page_const_arch.h","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/fs/vfs/read_write.c","mykernel/drivers/char/mem.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/dirstream.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/include/linux/kernel/dirent.h","mykernel/fs/vfs/readdir.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/dirent.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c","mykernel/fs/fat/myos_fat32.c"]
---

# 第116批独立验收 · shell 如何消费目录流

**裁定 `RETURN`。** 三条记录的消费、`ls` 的过滤与结束分支、`closedir` 活动函数的限制均可有条件使用。R116-01、R116-02 交第117批追加补正。[固定165](https://github.com/08822407d/MyOS2/blob/e96b1079ff0d/agent-workspace/lead/MYOS2-LEAD-002/165-user-dir-stream-ls-contract.md) · [116固定六件](https://github.com/08822407d/MyOS2/tree/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116) · [116作者交付 #7056533291](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056533291) · [115补正裁定](CORE-USER-DIR-STREAM-LS-116-D00-115-review.md)

## 交付与独立复算

以执行父 `452444a1c4b4` 对 `145d6781c75e` 复核，单父、领先一提交、六件新增2693行，旧件零改删。固定与活动六件按全文及 Git blob 逐件一致。PR17顶层评论从第一页查至空，100/100/19/0，116作者标记唯一，评论按ID两读与分页正文相同。开放列表仅PR16 Ready 与原题PR17 Draft，均指向 master 且未合并。[116作者交付 #7056533291](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056533291) · [165发布 #7056268192](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7056268192)

固定源码15个文件，62个连续1–5行的窗口均逐字命中。六件正式前缀计数为0/0/0/0/0/62（依 MANIFEST、报告、facts、图、deferred、evidence 排列）。62项证据账可定位，27个普通位置在行界内，另有三处消费者引用未登记的P29。两份结构化文件可解析，8个条件候选；DOT 46个声明节点、47条边，端点均有声明。复核两快照的编译和 panic 金丝雀及 direct master 公约原句。完整本地P9、预处理、构建、Graphviz、QEMU与实机均 `NOT_RUN`。

| readdir 调用 | 新增取数调用 | 返回记录及起点 | 用后 offset / size | shell 动作 |
|---|---:|---|---|---|
| 1 | 1，取80字节 | `.` @ 0，长24 | 24 / 80 | 跳过，继续 |
| 2 | 0 | `..` @ 24，长24 | 48 / 80 | 跳过，继续 |
| 3 | 0 | `alpha` @ 48，长32 | 80 / 80 | 请求输出名字和制表符，继续 |
| 4 | 1，取数返0或模型值−5 | NULL | 80 / 80 | 请求换行，调用 closedir，ls_command 返回1 |

独立用 `8 × ceil((19 + 名字节数 + 1) / 8)` 复算。给定三个完整有效条目、读到的尾字段已有确定值 D3，指针均指向可再填充的同一 DIR 缓冲。第四次保留用户 filepos=D3，内核 file.f_pos 另记。已缓存且 offset=24 时，下一次前进到48。打开返回 NULL 时，shell 走错误显示并返回−1。成功打开的路径中，`closedir.c` 完整体未见活动 close、free 或显式 return，调用者忽略返回，`run_cmd` 也不接收内建命令返回值。[消费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c#L39-L80) · [显示与收尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L224-L242) · [closedir 完整体](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/closedir.c#L28-L51)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	printf("\n");
	closedir(dir);
	return 1;
```
[myshell.c L240–242](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L240-L242)

## R116-01 · 补全尾字段边界，修复证据指向

报告95行、facts.B.syscall.tail_d_off 和 B.batch_model.d_off_status、候选 DS116-CAND-07 对三条记录保留 D3 前提合理，遗漏165要求的单条对照。getdents64 初始化回调上下文时其余成员为零。首次成功填入时 prev_reclen=0，prev 指向当前记录，写入首条自身的 d_off。若仅填一条，这也是尾条。若填入至少两条，后续成功 actor 写前条，最后一条自身缺少该写入，退出时的补写又在注释内。FAT 给 actor 的 offset 为0，仅在相应存储成功且字段有效的模型中赋数值。三条模型需要已知可读 D3 方可把它交给 DIR.filepos。[初始化及退出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L135-L165) · [记录存储](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L109-L123) · [FAT actor 实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L340)

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
```
[readdir.c L109–113](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L109-L113)

报告116行、facts 840行、deferred 147行引用的 `DS116-P29` 在证据索引中缺席。P01定位完整 closedir 函数，P06定位公开声明，E058支持实际调用和忽略返回。facts 中的源路径补齐仓根。汇编 E045 跨到下一个局部标签，E046 的比较和跳转要与 P22 完整退出段合读。补上 `_SYSFUNC_DEF_` 中转定义，分别注明 wrapper 实例、LABEL_SYSCALL 和 sysexit_ret_addr 的范围及活动指令。判定 invalid_name 返回时指向含两个 return 的完整助手。关闭时逐项校验更正后的消费ID、标签与源码位置。[原报告](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md#L89-L116) · [证据账](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/evidence.md) · [生成宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L13-L27) · [共享汇编](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L70-L82)

## R116-02 · 图的调用者、消费分支与所持资源

图56把表绑定写成调用，实际是 run_cmd 间接调用。61把 open 写成 __alloc_dir 的调用者，63–64的 malloc 结果由 __alloc_dir 判断。72–74要画出 readdir 先取并限制 maxread，再调用 getdents64，数值作为实参。76的返回字节数受 prev_reclen 条件限制，无已产出条目时另走 error。90使返回指针可绕开过滤与显示直接回循环，93把换行节点写成 closedir 的直接调用者。按真实调用、数据依赖、判定和先后顺序分别画边，覆盖打开失败、缓存与耗尽、两个点名、普通名字、取数返0或负数。[需补正的边](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot#L56-L95)

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```
[myshell.c L139–142](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L139-L142)

报告82行和 facts A03 把 `fd_held` 记为−2。负值是保存在 DIR.fd 的 open 结果，此例持有的有效 fd 数为零。A04 则已获得 fd 7，分配失败后在这两个函数中未关闭。将存储值、持有的有效 fd 和清理分列。报告99–100行的“跳过/停止”拆成跳过并继续，NULL 才退出枚举。关闭时从更正后的实际 DOT 逐例走到下一轮或函数返回，并对照替代表和 facts。[A与B原表](https://github.com/08822407d/MyOS2/blob/145d6781c75e/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md#L78-L104) · [打开和分配](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L62-L121)

两项退回只指第116批。旧 R115-03 沿用原编号，R115-01/02 已文档闭合。
