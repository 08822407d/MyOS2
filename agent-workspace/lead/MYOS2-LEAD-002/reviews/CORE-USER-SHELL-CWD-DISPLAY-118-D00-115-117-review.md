---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-CWD-DISPLAY-118"
record_type: "independent_historical_document_correction_review"
status: "FOUR_CLOSED_ONE_OPEN"
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
lead_before_publication_short12: "2b170b8072ed"
execution_pin_short12: "cb96288abb8b"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118: "RETURN"
R115_03: "CLOSED_DOCUMENTATION_ONLY"
R116_01: "CLOSED_DOCUMENTATION_ONLY"
R117_01: "CLOSED_DOCUMENTATION_ONLY"
R117_02: "OPEN"
R117_03: "CLOSED_DOCUMENTATION_ONLY"
R118_01: "OPEN"
R118_02: "OPEN"
closed_count: 4
open_count: 1
full_local_P9: "NOT_RUN"
build_and_runtime: "NOT_RUN"
open_questions: []
owner_decisions_requested_now: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","mykernel/fs/vfs/read_write.c","mykernel/arch/x86_64/entry/syscall_64.c","mykernel/arch/x86_64/include/asm/syscalls_64.h","myinitramfs/mylib/myglibc/syscall/syscall.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/d_path.c","myinitramfs/mylib/myglibc/dirent/dirent.h","mykernel/drivers/char/mem.c","mykernel/fs/vfs/readdir.c","mykernel/fs/fat/myos_fat32.c","mykernel/scripts/options_flags.cmake"]
---

# 五项历史补正独立裁定

原 C07c 在交付后报告上下文耗尽。以下逐项裁定来自已提交的文件、图边和固定源码。四项闭合，第117批计数项继续开放。[167号关闭条件](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/167-user-shell-cwd-display-contract.md) · [117独立验收](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md) · [上轮历史裁定](https://github.com/08822407d/MyOS2/blob/2b170b8072ed/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-D00-115-116-review.md) · [118固定六件](https://github.com/08822407d/MyOS2/tree/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118) · [118作者交付 #7057835701](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7057835701)

| 原编号 | 裁定 | 恢复使用的范围 |
|---|---|---|
| R115-03 | `CLOSED_DOCUMENTATION_ONLY` | read/write 入口、位置来源、三个零设备模型，须满足原条件。 |
| R116-01 | `CLOSED_DOCUMENTATION_ONLY` | 单/多尾记录条件、三处 closedir 引用及汇编标签。 |
| R117-01 | `CLOSED_DOCUMENTATION_ONLY` | 七行报告全部八列与两个原始数据字段相符。 |
| R117-02 | `OPEN` | 117旧账及映射位置已修。118新六件正式前缀与自检仍相差1。 |
| R117-03 | `CLOSED_DOCUMENTATION_ONLY` | CAT图的直接调用、顺序、方法绑定与四条路径，合读下述两处图例补记。 |

### R115-03 · 映射、位置和读零循环

实际图首簇名为 R115。映射表第4/5行经活动 switch、生成的 x64 桩、se/do 包装到 ksys_read/ksys_write。图中两侧都出现真实 fd 获取、file_ppos 与非流局部 pos 节点，以及分别属于 VFS 的验证/限长、属于 ksys 的有条件位置写回。图的表绑定指到唯一 read_zero。同步迭代的 queued 门在正常局部位置写回之前；标量零设备不走迭代支路。[R115真实图边](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot#L183-L246) · [read/write 映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L4-L5) · [包装与入口](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscall_wrapper.h#L241-L251) · [读端局部位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L175-L196) · [写端局部位置](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L319-L337)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[options_flags.cmake L64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

独立从循环计数复算：0字节不进循环，清零0返0；4096字节减后余0，清零4096返0；4097字节的两轮余量是1、0，清零4097返4096。初始非流位置9，回调未改偏移，三例外层均同值提交9。与旧115数值和本次 facts、表、R115子图一致。第115批原件合读追加材料后为 `PASS_PENDING_LOCAL`。[零设备实现](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L82-L109) · [三个模型](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md#L7-L18)

### R116-01 · 尾条、公开声明和三个汇编标签

对长度序列24、24、32用当前地址减上次记录长度复算。一次成功时写第1条自身；两次写第1/1条；三次写第1/1/2条。FAT给 actor 的 offset 为0，直接存储成功时一条记录的尾值为0。后两例仍须另有已初始化尾值 D2/D3 方可读入用户 filepos。[前条赋值](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L109-L123) · [最后记录处理](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/readdir.c#L155-L168) · [FAT实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L340)

核 CW118-E031/032/033 的判空、47行 true 和49行 false，E038/039/040 分别只取 LABEL_SYSCALL、sysexit_ret_addr、LABEL_SYSCALL_RET。77–78行注释的 errno 变换未被当作活动动作。旧116报告116行、facts 840行、deferred 147行中的悬空P29，按命题用 P22（函数体）、P21（closedir 声明）和 DS116-E058（ls 调用）替代。证据归属可逐处复核。R116-02 既有闭合继续有效，第116批组合 `PASS_PENDING_LOCAL`。[补正索引](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml#L177-L268) · [汇编标签](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L70-L82) · [活动声明](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/dirent/dirent.h#L141-L147) · [ls收尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L240-L242)

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c::invalid_name]
```c
		// __set_errno(ENOENT);
		return true;
	}
	return false;
```
[opendir.c L46–49](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c#L46-L49)

### R117-01 · 七行八列

从固定117 B.models 的预清零长度和 read 提供的字节复建数组，求首次0，再取前缀并追加10（LF）。首NUL依次为3/1/2/2/0/0/0，写请求和 printf 返回均为4/2/3/3/1/1/1。独立比较117的 B.first_NUL_index 与 C.first_NUL_at（分别在 models 和 output_cases 的行对象内）、118 facts 及正文八列表，七行全部相等。负返回按原模型另设缓冲未被修改。[117原机器数据](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml#L1323-L1460) · [118实表](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md#L31-L44)

### R117-02 · 历史账正确，本次自检仍差一次

重新计117六件，正式前缀1/0/0/0/0/88，共89，其中88个引文窗，多出一次在旧 MANIFEST 第113行。核本次 P01/P02/P03 对应 read/write/open/close 第4–7行、lseek 第11行及 getcwd/chdir 第41–42行。旧错已定位。167另要求用118最终六件核P6；实际分文件为 **0/0/0/0/0/92**。evidence 第3行的说明也含同一前缀，其余91处是源码短窗。MANIFEST 的91/91/0与正式全件计数不符。保留原编号；119追加118原账、标明那一个额外位置，并直接按待发布的119六件重算，不把说明行升格为引文。[多出的一处](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md#L1-L8) · [118自检](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md#L33-L39) · [真实映射](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L4-L11)

### R117-03 · CAT实际调用者

沿图核正常、短读、open失败、迭代计数0四例。cat拥有两次分配、清零、复制及 read/printf/close 的直接调用边。malloc→memset→strcpy 的顺序边与调用者分开。read 映射经生成的包装进入 ksys_read→vfs_read→new_sync_read，后者经 read_iter 调 FAT 表所绑定的 generic_file_read_iter；正计数至 simple_filemap_read 入口为止。调用方给定的短读模型可到格式化与 close，读满及实际显示仍未验证。[CAT真实图](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot#L247-L309) · [VFS选择](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L154-L165) · [间接调用](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L83-L92)

[VERIFIED mykernel/fs/vfs/read_write.c::new_sync_read]
```c
	ret = filp->f_op->read_iter(&kiocb, &iter);
	BUG_ON(ret == -EIOCBQUEUED);
	if (ppos)
		*ppos = kiocb.ki_pos;
	return ret;
```
[read_write.c L88–92](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/read_write.c#L88-L92)

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```
[myshell.c L263–267](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L263-L267)

图例补记：CAT表项在 myshell.c:49，实际绑定用 CW118-P07 或 CT117-E050，图247引用的 CW118-E053 止于48。图256的 t_separator 含条件写斜杠与 strcat，直接调用是 cat→strcat；strcpy 属前一 t_strcpy 节点。图283和401描述同一次间接调用，不能相加。实际调用链满足原退回项的四个路径条件。第117批因 R117-02 继续 `RETURN`。完整本地P9、编译及运行 `NOT_RUN`。
