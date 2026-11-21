---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-SHELL-FILE-COMMANDS-120"
record_type: "independent_phase2_mainline_review"
status: "RETURN"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-21"
date_timezone: "Asia/Shanghai"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "9df81502ae54"
execution_pin_short12: "9ea23faa4ab7"
batch115: "PASS_PENDING_LOCAL"
batch116: "PASS_PENDING_LOCAL"
batch117: "RETURN"
batch118_composite: "PASS_PENDING_LOCAL"
batch119_composite: "PASS_PENDING_LOCAL"
batch120: "RETURN"
R117_02: "OPEN"
R118_01: "CLOSED_DOCUMENTATION_ONLY"
R118_02: "CLOSED_DOCUMENTATION_ONLY"
R119_01: "CLOSED_DOCUMENTATION_ONLY"
R120_01: "OPEN"
R120_02: "OPEN"
author_delivery: "CONFIRMED_STOP_WRITING"
review_new: ["R120-01","R120-02"]
historical_open: ["R117-02"]
author_windows_actual: 81
eligible_as_submitted: 80
new_file_cases_checked: 17
full_local_P9: "NOT_RUN"
build_link_QEMU_hardware_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":4,"quotes_reconfirmed":4,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-21-shell-input119-reviewed-file-commands120.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SYSCALL-STDIO-08-review.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md","myinitramfs/myshell/myshell.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/include/asm/unistd_64.h","mykernel/arch/x86_64/include/asm/syscalls_64.h","mykernel/scripts/options_flags.cmake","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/mylib/myglibc/io/sys/stat.h","myinitramfs/mylib/myglibc/bits/stat.h","mykernel/include/linux/kernel/stat.h","mykernel/include/uapi/linux/stat.h","myinitramfs/mylib/myglibc/stdio/errlist.c","myinitramfs/mylib/myglibc/stdio/printf.c"]
---

# 第120批独立验收 · RETURN

六件已提交，四个文件命令的17个条件案例通过源码复算。整包退回两项文档问题：应补足从构建目标到库实现的证据边界，且图的失败出口和命令分支须能逐条复核。三项历史补正的单项裁定见同批回执。[170号合同](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md) · [120作者正式交付](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7062612922) · [120固定六件](https://github.com/08822407d/MyOS2/tree/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120) · [历史D00裁定](CORE-USER-SHELL-FILE-COMMANDS-120-D00-117-119-review.md)

## 交付身份与完整性

直接读取 lead `9df81502ae54`、exec `9ea23faa4ab7`、time `a039d9803ade`、master `e8e60cd19540`。Open PR清单为16（Ready）、17（原题Draft），base均为master，均未合并。执行提交唯一父 `9957ff1d927f`，compare单提交六件全新增，2400新增行、0旧件改删。对固定/活动正文重算UTF-8字节及含对象头的Git SHA-1，逐一核对提交树、compare及按文件读取的Blob。

| 文件 | 行 | 字节 | 重算Blob（短12） |
|---|---:|---:|---|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/MANIFEST.md) | 145 | 10179 | `8c91c0376497` |
| [shell-file-commands.md](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md) | 125 | 14756 | `d9b2db31fd99` |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml) | 630 | 16230 | `9918fc4e1fbe` |
| [shell-file-commands-paths.dot](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot) | 319 | 17055 | `ab2ce58c36d8` |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/deferred-findings.yaml) | 153 | 7180 | `867d8d3bfd10` |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md) | 1028 | 28397 | `5b2d39b82c2d` |

合计93797字节。作者评论 #7062612922 按ID双读，正文与分页项相同；100/100/27/0共227条，C08a/CHAT-EXEC-18的120正式标记唯一。四条金丝雀、公约原句、MANIFEST列出的项目路径均核对。两份机器表可解析，延期表含三个纠正、七项来源或运行缺口。六件无40位提交串。81个短窗均逐字命中固定源码（12个文件），其中一窗跨汇编标签，合格数见D00。[固定MANIFEST](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/MANIFEST.md) · [C08发布170](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7062355498)

## 四个文件命令的可用结论

十项内建表中touch、rm、mkdir、rmdir的索引分别为4、5、6、7；main成功派发时通过run_cmd间接调用相应函数，内建的返回数无人接收。正常cmd参数模型不把屏幕或磁盘效果当成已验证。mode从用户宏的0400/0200/0100及右移关系复算，内核掩码为01777；0666=438、0777=511、差0111。[内建表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L56) · [用户模式位](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/io/sys/stat.h#L164-L186) · [基础位](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/bits/stat.h#L82-L84)

| 专题 | 独立模型和结果 | 消费前提 |
|---|---|---|
| touch / creat（4例） | argc=1/3均返−1；argc=2时向creat给argv[1]和0666。给定用户调用返回0或−5，touch都返0、没有自身报错行。 | 内核do_creatat正常方法分支返回回调状态；在所读体未见分配新fd的调用。单凭0无法推出fd或落盘。 |
| mkdir（6例） | argc=1/3提前退出；有效对象和可用方法时，回调0/−5都经过done_path_create、putname，内建仍返0。filename_create给定ERR_PTR(−2)只经putname；方法为空直接−EPERM，两处清理均绕过。 | 错误指针例假定输入name有效。掩码后0777为交给回调的值。 |
| rm / rmdir（7例） | argc≠2提前退出；给定调用返回0时无自身错误行；给定−5时，将strerror(errno)和路径依次交给该命令的printf，内建返回−5。 | 返回值、errno、错误文字是三个分别待证的值。给定示例字符串可复算两个格式请求，用户真实可见输出未测。 |

[VERIFIED myinitramfs/myshell/myshell.c::touch_command]
```c
	nmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	err = creat(argv[1], nmode);

	return 0;
```
[固定源码 331–334](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L331-L334)

[VERIFIED mykernel/fs/vfs/namei.c::do_mkdirat]
```c
		if (!dir->i_op->mkdir)
			return -EPERM;
		mode &= (S_IRWXUGO|S_ISVTX);
		error = dir->i_op->mkdir(dir, dentry, mode);
	done_path_create(&path, dentry);
```
[固定源码 985–989](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L985-L989)

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);
```
[固定源码 369–372](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L369-L372)

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
```
[固定源码 64–66](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/scripts/options_flags.cmake#L64-L66)

纸面计算逐行对照facts及报告，A4、B6、C7例的分支、模式、返回及清理结果成立。B120-06报告输入 `mkdir docs more`、facts输入 `mkdir docs extra`，第三个词无消费，均为argc=3；后续按同一ID引用时以facts的raw_input为准。`do_mkdirat`直接调用目录方法指针；同文件另有vfs_mkdir定义。给定回调或桩返回的数值只用于其适用层。[120正文](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md#L69-L119) · [120机器表](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml) · [创建/建目录入口及清理](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L899-L1002)

## R120-01 OPEN · 用户库来源及证据位置

报告第82、115行把命令调用、仓内myglibc桩、内核映射连作一条“活动”路线。固定快照能分别证明shell直接调用的接口、仓内桩的实现、内核的条件分派。myinitramfs/CMakeLists.txt先project，后设置仓库外musl工具链与头路径；顶层只加入myinit、myshell，sh目标在其子目录创建。现有构建文本不能独立认定交付的 /boot/sh 选用了仓内桩；亦不能据此断言已链接某一实际musl二进制或它会选择哪一个syscall。这个限制已由08正式裁定。四条边、`strerror(errno)`和printf→write须分别标明来源。未走本次负返回→errno映射并不排除另一库实现有自己的返回约定。[initramfs CMake](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/CMakeLists.txt#L31-L51) · [sh目标](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/CMakeLists.txt#L1-L9) · [08回执§6](https://github.com/08822407d/MyOS2/blob/9df81502ae54/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SYSCALL-STDIO-08-review.md)

P03把系统调用号写成源文件行号。真正的mkdir/rmdir/creat/unlink号为83/84/85/87，在unistd_64.h **103/104/106/108** 行。映射表行44/45/46/48与上述宏编号分列。P10所称filename_create完整体应为770–823，825已开始另一个函数。E079只到if条件，要证明__get_errlist如何翻转负号并索引表，须补28–29行。四个命令的结果表按给定接口返回成立；从内核局部状态跨到用户观察还要有实际库、正常返回且没有信号/重启改写等条件。[普通位置原表](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md#L1010-L1022) · [编号宏](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/unistd_64.h#L103-L108) · [内核映射实例](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L44-L48) · [filename_create结尾](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L817-L826) · [errlist返回表达式](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/errlist.c#L23-L31)

## R120-02 OPEN · 分支可达性

DOT独立提取六簇、129个不同节点、164条有类型边，所有端点存在。按邻接逐段核了18条路径；另外查错命令可达性和提前返回的后继。实测 `a_missing` 和 `b_missing` 都无法到 `a_usererr` / `b_usererr`，但源码在方法缺失时立即返−EPERM，合法argc的内建仍会在返回后给0。两个出口必须能避开清理点并到达用户侧的有条件返回。[A/B图](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot#L219-L274) · [creat提前返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L916-L929) · [mkdir提前返回](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L983-L996)

共享r_argc与r_error令 `r_rm→r_argc→r_rmdircall` 和反方向均可达，也能从rm走到rmdir格式串，反之亦然。实际两条直接调用与格式各有其主。应保留命令身份至成功/报错及返回；用边类型/分支条件既证明正确输出可达，也证明交叉目标不可达。D00旧图的端点勘误和119的索引0见单独回执，不要求重做已闭合历史分析。[删除命令图](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot#L275-L317) · [删除命令调用与格式](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L349-L375)

<details><summary>18条独立重查的实际DOT路径（相邻节点均有直接的图边）</summary>

```
d_cd d_success d_cachezero d_getcwd d_kernelgetcwd d_prepend d_lencheck d_sizefail d_no_usercopy d_empty_cache d_pwdprintf
d_cd d_success d_cachezero d_getcwd d_kernelgetcwd d_prepend d_lencheck d_sizefail d_no_usercopy d_empty_cache d_promptprintf
d_cd d_success d_cachezero d_getcwd d_kernelgetcwd d_prepend d_lencheck d_usercopy d_fullcopy d_valid_cache d_pwdprintf
d_cd d_success d_cachezero d_getcwd d_kernelgetcwd d_prepend d_lencheck d_usercopy d_copyfail d_partial_unknown d_promptprintf
d_main d_mainzero d_maingetcwd d_kernelgetcwd d_prepend d_lencheck d_sizefail d_no_usercopy d_empty_cache d_promptprintf
i_bs i_count0 i_underscore
i_bs i_countpos i_decrement i_secondbs i_underscore
d_cat d_strcpy
d_cat d_strcat
c120_01 p_good p_run p_ptrcall p_cd
c120_02 p_good p_run p_ptrcall p_pwd
c120_03 p_bad p_error
c120_04 p_early p_bad p_error
c120_05 p_early p_bad p_error
a_docreat a_filename a_iop a_mask a_callback a_done a_putname a_kret a_usererr a_return
b_domkdir b_filename b_badfilename b_putname b_kret b_usererr b_return
r_kunlink r_dounlink r_vfsunlink r_iopunlink
r_krmdir r_dormdir r_vfsrmdir r_ioprmdir
```
</details>

完整本地P9、Graphviz、预处理/编译/链接、QEMU、实机、真实文件操作均 `NOT_RUN`。本回执为阶段2文档及源码裁定。[121任务书](../171-user-stdio-file-open-contract.md)
