---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CAT-FILE-DISPLAY-117"
record_type: "independent_115_116_document_corrections_review"
status: "PARTIALLY_CLOSED_DOCUMENTATION_ONLY"
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
batch114_composite: "PASS_PENDING_LOCAL"
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":5,"quotes_reconfirmed":5,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/dir-stream-ls-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-dir-stream-ls-116/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/memdev-scalar-io-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/evidence.md","mykernel/scripts/options_flags.cmake","mykernel/fs/vfs/read_write.c","mykernel/drivers/char/mem.c","mykernel/fs/vfs/readdir.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/fs/fat/myos_fat32.c","myinitramfs/mylib/myglibc/dirent/dirent.h"]
---

# 第117批 D00 独立裁定 · 第115、116批的三个退回项

R116-02 文档闭合，R115-03 与 R116-01 继续 `OPEN`。第115、116批均维持 `RETURN`。本回执只核这三个原编号，第117批自身的裁定另见[同期独立验收](CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md)。[166号任务书](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/166-user-shell-cat-file-display-contract.md) · [115原补正裁定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-D00-115-review.md) · [116原裁定](https://github.com/08822407d/MyOS2/blob/b2f563a67814/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-DIR-STREAM-LS-116-review.md) · [117固定六件](https://github.com/08822407d/MyOS2/tree/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117) · [117作者交付 #7057046328](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057046328)

## R115-03 · 已有零设备返回路径，映射与位置入口仍缺

117图中的 `m_` 节点已经接通 ksys 到 VFS、设备表对 read_zero 的绑定、循环、返回与非负结果的外层位置写回。C08 沿条件重算如下，起始非流位置为9，假定调用到达选定标量方法。

| 请求字节 | 各轮减后余量 | 实际清零 | 返回 | 最终位置 |
|---:|---|---:|---:|---:|
| 0 | 无循环 | 0 | 0 | 9 |
| 4096 | 0 | 4096 | 0 | 9 |
| 4097 | 1，0 | 4097 | 4096 | 9 |

同步迭代读写分支保留了 queued 特殊门、局部位置写回条件。读写外层的非负且 ppos 有效门可回源。上述三个数值、既有 A9/B7/C6 以及已经闭合的 R115-01/02 继续按原条件消费。[清零与返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L82-L109) · [读位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L175-L196) · [写位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L319-L337)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

仍缺166指定的实图差量。首子图第5–6行的标识和标题是 `cluster_undefined`、`undefined`；facts.graph.subgraphs 与作者评论却写 R115。图从 m_sysread/m_syswrite 起步，缺 switch 使用的 read/write 映射、生成的入口以及两侧 `file_ppos` 和局部 pos 的形成。facts 的第一组 typed_repairs 指到 `DS116-P10`，它是 getdents64 的第67行；read/write 的实例是同表第4–5行。旧 MD115-P01 第11行只指 lseek。[117首子图](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot#L5-L51) · [原六组映射](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L77-L116) · [read/write映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L4-L11) · [活动 switch](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/entry/syscall_64.c#L26-L32)

**关闭条件**　在118实际 DOT 加入定名的 R115 子图，自 read/write 映射经 switch、对应入口到 ksys，补 file_ppos、局部 pos 捕获、回调及外层提交，保留限长、queued 门和零次/单页/跨页返回。源代码实例和定义分别标注。对照实际边列出三条取值路径，含4097两次循环。

## R116-01 · 尾记录模型成立，证据标签仍有错

独立按记录起点与上次记录长度演算。第一条时 prev_reclen=0，写入本条自身。两条时两次写的目标均为第1条。三条时三次写的目标依次为第1、第1、第2条。FAT actor 的 offset 为0，成功写入的单条尾值为0。两条和三条的最后一条分别只在已有确定、可读的 D2、D3 时才能给 DIR.filepos 赋模型值；退出段没有活动尾部补写。汇编宏的三层中转已补足，真实 closedir 函数可由 CT117-P19 定位。[actor 写目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L109-L123) · [初始化与退出](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L135-L168) · [FAT 实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L340) · [补正模型](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L253-L388)

[VERIFIED mykernel/fs/vfs/readdir.c::filldir64]
```c
	prev_reclen = buf->prev_reclen;
	dirent = buf->current_dir;
	prev = (void *)dirent - prev_reclen;

	prev->d_off = offset;
```
[readdir.c L109–113](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L109-L113)

仍有四处需在118的正式账中替换。CT117-E040 的42–43行仅判空，真正的 `return true` 在47行，E041才是49行的 false。E046 的70–74行同时含外层 asm 和下一个标签，应改取71–73。E047 的75–76行属于 sysexit_ret_addr；E048 的79–81行位于新标签 LABEL_SYSCALL_RET。核 errno 转换须合读77–81行的注释与活动出口。facts 的 CT117-P02 是 `close(int)` 原型，`closedir(DIR *)` 声明应用旧 DS116-P06 或本轮直接定位，函数体用 CT117-P19，调用者用 DS116-E058。表明旧P29的三处消费各对应什么证据。[117证据映射](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L367-L388) · [判空两出口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L39-L50) · [汇编三个标签](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L70-L82) · [closedir声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/dirent/dirent.h#L141-L147)

## R116-02 · 调用、过滤与有效 fd 数已文档闭合

新图有独立的 R116 子图。函数表提供绑定，run_cmd 间接调用 ls_command。opendir 调用 open 后再调用 __alloc_dir，后者调用 malloc 并按返回值决定结果。图221的条件边只表示名字非空可继续；调用者由图222的 opendir 出边给出。fd=-2 是 DIR 的存储值，此例有效 fd 数为0。fd=7而分配失败的分支保留一个有效 fd，本函数没有活动关闭。图230的存储依据应用 opendir.c:110，E025 仅覆盖其他字段。[条件和实际调用者](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot#L215-L240) · [分配结果与 fd 赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L100-L118)

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::__alloc_dir]
```c
	dirp->fd = fd;
```
[opendir.c L110–110](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L110-L110)

readdir 在缓存耗尽时限制长度并取数。内核有已产出记录才用请求减余量。缓存命中直接取记录。独立沿实际边核打开失败、缓存命中、正批次、零与负返回：点与双点跳过并继续；普通 alpha 请求输出后继续；只有返回 NULL 才跳出循环，请求换行、由 ls_command 调 closedir，最后返回1。有效三记录用后偏移24/48/80，第四次取0或−5时仍为80。[缓存、过滤及退出](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot#L239-L272) · [shell消费](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L229-L242)

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
		if(buf == NULL)
			break;
		if (strcmp(buf->d_name, ".") != 0 &&
			strcmp(buf->d_name, "..") != 0)
			printf("%s\t", buf->d_name);
```
[myshell.c L234–238](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L234-L238)

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	printf("\n");
	closedir(dir);
	return 1;
```
[myshell.c L240–242](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L240-L242)

**权威补记**　facts.D00.R116-02.six_corrections 第5句（400行）误把 skipped 名字也列为打印。以本段、同字段第6句、逐行 corrected_B_trace 和实际图的分支为准：两个点名不调用名字输出，普通名字请求 `%s\t`，循环结束才请求 `\n`。该项的实际图、两种 fd 情形、过滤表和退出路径均已交付并回源；本回执补正孤立句及证据位置后，R116-02 为 `CLOSED_DOCUMENTATION_ONLY`。[相邻两句及机器模型](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L393-L406)

完整本地P9、Graphviz、预处理、构建及运行均 `NOT_RUN`。
