---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
record_type: "independent_historical_correction_review"
status: "TWO_CLOSED_ONE_OPEN"
phase: 2
conversation_display_name: "MYOS2-A-C08 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（会话自报）"
model_per_owner: "Pro，后台与effort未独立认证"
date: "2026-11-22"
date_timezone: "Asia/Shanghai"
base_snapshot: "time"
read_channel: "connector"
source_pin_short12: "a039d9803ade"
master_direct_short12: "e8e60cd19540"
lead_before_publication_short12: "52ad1a8582e8"
execution_pin_short12: "e1272a3f9b69"
batch117_composite: "PASS_PENDING_LOCAL"
batch120: "RETURN"
batch121: "RETURN"
closed_documentation_only: ["R117-02","R120-01"]
open_returns: ["R120-02","R121-01","R121-02"]
followup_id: "CORE-USER-STDIO-FILE-OPEN-121"
full_local_P9: "NOT_RUN"
build_link_QEMU_hardware_file_operations: "NOT_RUN"
owner_decisions_requested_now: []
open_questions: []
self_check: {"verified_claims":0,"quotes_reconfirmed":0,"downgraded_to_inferred":0}
inputs_read: ["agent-workspace/lead/MYOS2-LEAD-002/171-user-stdio-file-open-contract.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/shell-cat-file-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/shell-cwd-display.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/shell-input-dispatch.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/MANIFEST.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/facts-and-boundaries.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands-paths.dot","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/shell-file-commands.md","mykernel/fs/vfs/read_write.c","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/drivers/char/mem.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/opendir.c","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/readdir64.c","mykernel/fs/vfs/readdir.c","mykernel/fs/fat/myos_fat32.c","myinitramfs/mylib/myglibc/syscall/syscall.c","mykernel/arch/x86_64/entry/syscall_64.c","myinitramfs/mylib/myglibc/string/strcat.c","myinitramfs/mylib/myglibc/string/strcpy.c","mykernel/fs/syscall.c","mykernel/mm/vm_map/filemap.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/stdio/vsnprintf.c","mykernel/fs/vfs/namei.c","myinitramfs/mylib/myglibc/stdio/errlist.c"]
---

# 第121批 D00 · 三项历史补正的独立裁定

| 编号 | C08结论 | 可消费的范围 |
|---|---|---|
| R117-02 | `CLOSED_DOCUMENTATION_ONLY` | 117原88窗/89次前缀。以下五个旧窗按C08补记调整；121的字面、单符号资格与E023题义分别计账。 |
| R120-01 | `CLOSED_DOCUMENTATION_ONLY` | 四个命令的C调用、仓内桩、内核映射、错误文字与实际链接来源已分层。 |
| R120-02 | `OPEN` | 两条真实早返和rm/rmdir分离成立；两条无方法的case_id仍能到达清理，正文表的返回值栏错位。 |

[171号关闭条件](https://github.com/08822407d/MyOS2/blob/52ad1a8582e8/agent-workspace/lead/MYOS2-LEAD-002/171-user-stdio-file-open-contract.md) · [上轮历史裁定](https://github.com/08822407d/MyOS2/blob/52ad1a8582e8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-FILE-COMMANDS-120-D00-117-119-review.md) · [121纠正正文](https://github.com/08822407d/MyOS2/blob/e1272a3f9b69/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open.md#L21-L71)

## R117-02 · 固定原件分账与117号补记

C08对四个历史提交的24个原件分别重数字面前缀，另对117的88窗及121的69窗逐段回读固定time。数字以各批最终六件自身为单位。下表中的候选数按符号归属筛选，不能单据此把引文标题或整批程序行为认证为真。

| 批次 | 原窗 | 六件字面次数 | 限定 |
|---|---:|---:|---|
| 117 | 88 | 89 | 额外一次在MANIFEST第113行。本回执实比88窗全部匹配，按原署名排除下述五窗后剩83个候选。 |
| 118 | 91 | 92 | evidence第3行多一次，CW118-E051/E052为文件作用域对象，沿用89个候选。 |
| 119 | 46 | 46 | IN119-E020–E022为全局命令表初始化，沿用43个候选。 |
| 120 | 81 | 81 | FC120-E072跨汇编标签，沿用80个合格原窗。 |
| 121 | 69 | 69 | 各在单一定义内，十二份源码逐字匹配。E023的“五参”纠正在121独立验收中。 |

117原报告中三个文件级初始化对象可以作为普通绑定位置使用：CT117-E015是zero_fops，E050是shell_internal_cmd，E070是FAT32_file_ops。它们的字段或条目不计作函数、宏或结构体类型定义内的短窗。CT117-E046的70–74行从外层asm跨到第二个标签；E048的79–81属于LABEL_SYSCALL_RET，旧署名为sysexit_ret_addr。121的E065–067已分别给71–73、74–76、79–81单标签替代引文。两处errno赋值及归一化在77–78行处于注释。117原稿“88个候选，资格待确认”在与本裁定组合消费时使用此补记，无需执行者重复审历史24件。[117原引文](https://github.com/08822407d/MyOS2/blob/52c51a3752d3/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cat-file-display-117/evidence.md) · [zero_fops](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/drivers/char/mem.c#L141-L145) · [内建表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L44-L56) · [FAT32文件操作表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/fat/myos_fat32.c#L354-L361) · [共同入口和两个返回标签](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L70-L82) · [121替代引文](https://github.com/08822407d/MyOS2/blob/e1272a3f9b69/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/evidence.md#L757-L789)

117的R117-01和R117-03在118历史裁定中已文档关闭。117已审 `cat` 专题与指定历史回执及本补记组合为 `PASS_PENDING_LOCAL`。118、119各自此前已获准的组合范围继续适用。[117原独立裁定](https://github.com/08822407d/MyOS2/blob/52ad1a8582e8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CAT-FILE-DISPLAY-117-review.md) · [118历史回执](https://github.com/08822407d/MyOS2/blob/52ad1a8582e8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-CWD-DISPLAY-118-D00-115-117-review.md) · [118原引文](https://github.com/08822407d/MyOS2/blob/cb96288abb8b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-cwd-display-118/evidence.md) · [119原引文](https://github.com/08822407d/MyOS2/blob/9957ff1d927f/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-input-dispatch-119/evidence.md) · [120原引文](https://github.com/08822407d/MyOS2/blob/9ea23faa4ab7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-file-commands-120/evidence.md)

## R120-01 · 调用来源和错误值

已核myshell的creat/mkdir/rmdir/unlink各直接调用；仓内syscall.c第53/54/55/57行是桩的宏调用点。内核头的行103/104/106/108分别给mkdir=83、rmdir=84、creat=85、unlink=87。另一个头的44/45/46/48进入分派switch，宏和调用实例各有来源。filename_create实际770–823。errlist第27–29行对给定负编号变号后取表，解释表项时要有可表示的有效索引。仓内printf第22行的write请求与最终显示之间尚有下游条件。脚本编译器选择、实际sh所链libc、errno、信号/重启对最终用户返回的影响均已限定。[系统调用号](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/unistd_64.h#L103-L108) · [分派实例](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/include/asm/syscalls_64.h#L44-L48) · [filename_create](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L770-L823) · [错误字符串索引](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/errlist.c#L23-L31) · [08号来源及返回限定](https://github.com/08822407d/MyOS2/blob/52ad1a8582e8/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SYSCALL-STDIO-08-review.md)

## R120-02 · 案例起点和分支起点的区别

源码可成立的两条路径都已经存在。touch的tc_missing → tc_minusEPERM → tc_library_return → tc_localerr → tc_return0；mkdir的mk_missing → mk_minusEPERM → mk_libreturn → mk_localerr → mk_return0。类型分别为RETURN、BOUNDARY_CONDITIONAL、DATA、SEQUENCE。从两个missing节点各自出发，到对应done和putname都不可达。rm与rmdir的参数、成功、错误输出及返回节点独立；双向跨至对方接口或printf均不可达。其格式请求参数次序均是strerror(errno)在先、路径名在后。[创建分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L916-L929) · [建目录分支](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/namei.c#L983-L996) · [删除命令](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/myshell/myshell.c#L349-L375) · [D00图的真实边](https://github.com/08822407d/MyOS2/blob/e1272a3f9b69/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open-paths.dot#L170-L239)

从合同要求的案例标识出发，D121_A01 → tc_checkmethod → tc_callback → tc_done → tc_putname，D121_B01 → mk_checkmethod → mk_callback → mk_done → mk_putname，都是当前DOT实际存在的边，类型均依次为MODEL、INDIRECT_CALL、SEQUENCE、SEQUENCE。两个案例的facts都指定method_exists=false。测试若从missing才开始，便漏掉模型入口接错的问题。正文第55–69行还有8列表头对9列数据；内核值与有条件的用户接口值应各占一列，内建0在第七列。参错行和删除命令的未调用/给定用户返回也需明确，不能给未执行的内核调用填实测返回。[13行表](https://github.com/08822407d/MyOS2/blob/e1272a3f9b69/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open.md#L55-L69) · [案例入口和回调](https://github.com/08822407d/MyOS2/blob/e1272a3f9b69/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-stdio-file-open-121/stdio-file-open-paths.dot#L180-L212)

122只需追加与这两条绑定及该表有关的替代图和表。同一案例起点同时验证到内建0、到回调/两项清理不可达。保留rm/rmdir双向隔离。120其余已审技术结论可按前述限定使用，整包仍为 `RETURN`。
