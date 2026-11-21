---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SHELL-FILE-COMMANDS-120
record_type: bounded_shell_file_command_analysis_and_D00
produced_by: "GPT-6（后端与effort不可独立认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-18
date: 2026-11-21
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
D00_status: CORRECTION_SUBMITTED_PENDING_C08_REVIEW
owner_decisions_requested_now: []
open_questions: []
inputs_read: [agent-workspace/lead/MYOS2-LEAD-002/170-user-shell-file-commands-contract.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-review.md, agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-INPUT-DISPATCH-119-D00-117-118-review.md, myinitramfs/myshell/myshell.c, mykernel/fs/vfs/namei.c, myinitramfs/mylib/myglibc/io/sys/stat.h, myinitramfs/mylib/myglibc/bits/stat.h, myinitramfs/mylib/myglibc/syscall/syscall.c, mykernel/arch/x86_64/include/asm/syscalls_64.h]
---

# 第120批 · shell 文件命令的参数、返回与可见输出请求

作者 **MYOS2-A-C08a 代码分析执行 / CHAT-EXEC-18**。C08已独立把119定为RETURN，并发布170号合同；本件只追加纠正及调用方取证，不能自行关闭R117-02/R118-02/R119-01。固定time为`a039d9803ade`，170发令`9df81502ae54`，执行唯一父`9957ff1d927f`。**所有案例均是有界源码纸面模型；从未实际操作文件、目录、磁盘或shell。**

引用规则：本批新P2编号见本目录`evidence.md`的FC120-E001～E081，普通位置P01～P12；往期依据须同时阅读C08的119及57/58独立裁定，不把旧作者原件视为验收真值。

## D00 · 三项OPEN纠正，仍交C08复核

### R117-02：字面标签数量与合格P2资格分开

原文件顺序为 MANIFEST / 正文 / facts / DOT / deferred / evidence。下面三批都**只统计原有文本**，不修改旧件：

| 批次 | 六文件标签字面数 | 原报告窗总数 | 来源资格纠正 | 按已裁定保留的合格候选 |
|---|---|---:|---|---:|
| 117固定`52c51a3752d3` | 1/0/0/0/0/88 = 89 | 88 | 额外一次在MANIFEST:113说明，不是源码短窗 | 88项原窗，当前不冒充全部独立重审 |
| 118固定`cb96288abb8b` | 0/0/0/0/0/92 = 92 | 91 | E051/E052来自myshell文件作用域对象，转普通位置（原E050宏保留） | 89个候选 |
| 119固定`9957ff1d927f` | 0/0/0/0/0/46 = 46 | 46 | E020/E021/E022是全局命令表初始化，改为普通位置；E019是结构体定义保留资格 | 43个候选 |
| **120新六件** | **0/0/0/0/0/81 = 81** | **81** | 本批全部81条只摘独立函数/宏/结构体定义体；说明文字不额外出现协议字面前缀 | **81个新P2短窗** |

本批P6只认120六件最终字节和81/81/0，不把117–119的旧“原窗”混进新计数。历史C08重新分类属于本次补充消费规则，不替原件作未曾进行的独立全部机械闸门。R117-02状态`CORRECTION_SUBMITTED_PENDING_C08_REVIEW` / OPEN。

### R118-02：把clear→getcwd→kernel长度失败→空缓存真正接成一条条件路径

119旧DOT尽管修改了假直接调用，但在`b_getcwd`和`c_kernelgetcwd`之间必须**确证同一用户调用越过系统调用边界**，不能只留另一个`c_cdcall`摘要而缺少从清零开始的边。本批`shell-file-commands-paths.dot`中D00A/D00B/D00C三替代簇给出：

- cd成功的具体链：`d_cd → d_success → d_cachezero → d_getcwd → d_kernelgetcwd → d_lencheck → d_sizefail → d_no_usercopy → d_empty_cache → d_pwd_printf`，并从`d_empty_cache`进入`d_prompt_printf`。用户已清零、内核长度门失败且**完全没有执行copy_to_user**，才允许断言空缓存。内核`pwd`对象的变动不能自动推定用户`current_dir`内容。
- main首次初始化同理`d_main → d_mainzero → d_maingetcwd → d_kernelgetcwd`；这与cd的**两次不同调用现场**汇合在同一个内核函数定义节点，而非同一次实际调用。根据具体调用现场追一条完整路径。
- 成功分支必须额外假设`copy_to_user`完整写入了正确的含NUL字符串，才从`d_fullcopy`进入`d_valid_cache`；单看返回值不足以确定字节。
- `copy_to_user`失败分支进入`d_copyfail_unknown`：源代码没有证明全部无写入，故不得自动连`d_empty_cache`。内核无fd关联；`error=(long)buf`赋给int与复制结果分离，沿已闭合的R60-S01第3项。
- 旧A图`i_bs_case→i_uscore`只在count==0前提直走，count>0必经`buf[--count]=0`和第二次BS请求。旧CAT函数名也改为实际`cat_command→strcpy`、`cat_command→strcat`直接调用；原`c_catpath`只能是结果数据，不是可被直接调用的函数。

替代图每条边以DIRECT_CALL、SEQUENCE、DATA、CONTROL、MODEL、BOUNDARY、RETURN显式分类；跨簇路径能抽取端点，长度门失败不经过用户copy。R118-02保持OPEN，C08有权检查并关闭。

### R119-01：五行派发全部使用相同case_id

所有输入先有main中读行后的独立LF请求；下表的“第二请求”专指本次解析后会走的**实际分支**。`cd`可能后续失败打印，必须根据额外chdir模型才能确定，不能把尚未给定的条件输出当成已经请求。

| case_id | raw input | 逻辑词 | argc | find_called / 比较数 / find返回 | parse返回 | main本分支实际调用 | 间接目标和另一次可见请求 |
|---|---|---|---:|---|---:|---|---|
| D120-C01 | `"  cd  docs  "` | `cd docs` | 2 | true / 1 / 0 | 0 | `run_cmd(0,2,argv)` | 函数指针`cd_command`；成功时此函数无printf，失败条件另列 |
| D120-C02 | `"pwd"` | `pwd` | 1 | true / 3 / 2 | 2 | `run_cmd(2,1,argv)` | 函数指针`pwd_command`；请求缓存`current_dir`加LF |
| D120-C03 | `"nope"` | `nope` | 1 | true / 10 / −1 | −1 | `printf`错误行 | 不调用run_cmd，不存在间接目标；输出`Input Error, No Command Found!\n` |
| D120-C04 | `""` | 空行 | 0 | false / 0 / **无find返回** | −1 | `printf`错误行 | 不分配argv、不调用find_cmd或run_cmd；相同错误行 |
| D120-C05 | `"   "` | 全空格 | 0 | false / 0 / **无find返回** | −1 | `printf`错误行 | 仅前导空格跳过后返回−1；相同错误行 |

字段`find_return`在C04/C05必须为空值，不得用−1伪装函数实际调用；`parse_return=-1`仍存在。原119的`direct_call: main -> run_cmd`误填C03/C04/C05已被本表和facts的逐case纠正。raw input与逻辑等价命令单独存储，取消旧case_ids键与首尾空格输入无法对接的错误。跨文件及DOT均按同一D120-C01～05联结。R119-01仅提交纠正，仍OPEN。

## A · touch：调用creat但返回0并不等于创建成功

调用绑定是全局shell表索引4，run_cmd通过函数指针调用`touch_command`；通过`argc!=2`门后，参数来自`argv[1]`，直接指向parse_command从当前输入buf借出的路径起点。按用户态权限宏，owner/group/other各取read和write而没有execute：

`(0400|0200)|(0040|0020)|(0004|0002) = 0666`（十进制438）。其中0400/0200是`__S_IREAD/__S_IWRITE`，group和other由右移3及6生成。若程序所用头与宏符合固定源，则用户creat请求`(pathname=argv[1], mode=0666)`。内核do_creatat模式掩码为`S_IRWXUGO|S_ISVTX = 01777`；正常算术下`0666 & 01777 = 0666`，但不能从此推断FAT真正存储的权限或磁盘状态。

| case_id | 输入与argc | creat调用 | 指针/模式实参 | 给定内核/方法返回 | 局部err | 内建返回 | main与run_cmd可见反应 |
|---|---|---|---|---:|---|---:|---|
| A120-01 | `touch`,1 | 否 | 无 | 未调用 | 初值0 | −1 | main已进成功索引，run_cmd忽略−1；只有读行后LF |
| A120-02 | `touch note`,2 | 是 | `argv[1]`指向`"note"`，0666 | 0 | 0 | **0** | 无touch自身成功输出；run_cmd丢弃0 |
| A120-03 | `touch note`,2 | 是 | 同上 | −5（给定的回调返回） | −5 | **0** | 无touch自身失败输出；run_cmd同样丢弃0 |
| A120-04 | `touch note extra`,3 | 否 | 无 | 未调用 | 初值0 | −1 | 仍派发到touch，但不进入creat，返回值被忽略 |

活动系统调用路线：用户宏实例`SYSFUNC_DEF(creat)`→rax编号85→`syscall`公共桩→内核`__SYSCALL(__NR_creat,sys_creat)`的switch展开→生成的x64包装体→`MYOS_SYSCALL_DEFINE2(creat)`→`do_creatat(AT_FDCWD,getname(pathname),mode)`。这里入口、宏定义和调用实例**不同类证据**，见FC120-E070～E076、普通位置P02/P03/P04和源码`namei.c:932–935`。

内核`do_creatat`在有效getname/父目录对象和`filename_create`正常dentry前提下，先取`path.dentry->d_inode`，没有`dir->i_op->create`直接`return -EPERM`（提前绕过`done_path_create/putname`）；否则掩码后直接调`dir->i_op->create`，将**回调返回的error**作为系统调用返回来源。方法调用完成才到`done_path_create`与`putname`。当前读过的`creat/do_creatat`函数体没有`get_unused_fd_flags/fd_install/close`调用，故**不能证明POSIX creat所要求的新fd已经分配、安装、可用或之后关闭**；也不能用返回0直接替代fd。既有文件截断、mtime、FAT目录落盘与实际模式保存均未由本调用方取证证明，留57/58既有证据和新缺口。该内核实现不能据名称直接赋予完整POSIX creat语义。

## B · mkdir：0777请求、直接方法调用和三个提前出口

索引6的`mkdir_command`只在`argc==2`时调用用户`mkdir(argv[1],nmode)`，执行mode的`S_IRWXU|S_IRWXG|S_IRWXO = 0700|0070|0007 = 0777`（十进制511）。比touch多了`0100|0010|0001 = 0111`三类execute位。内核`do_mkdirat`使用`LOOKUP_DIRECTORY`并且**直接调用`dir->i_op->mkdir(dir,dentry,mode)`**，它没有先调用同文件的独立`vfs_mkdir`函数；两者存在相似掩码与回调，不等于同一调用链。

| case_id | argc及前提 | filename_create | i_op->mkdir是否调用 | 掩码后mode | 内核返回来源 | done_path_create / putname | shell返回 |
|---|---|---|---|---|---|---|---|
| B120-01 | 1，`mkdir` | 未调用 | 否 | 无 | syscall未调用 | 都未进入 | −1 |
| B120-02 | 2，`mkdir docs`、对象有效 | 正常dentry | 是，给定回调0 | 0777 | 回调0 | 两者均调用 | 0（忽略err） |
| B120-03 | 2，同上、回调−5 | 正常dentry | 是，给定−5 | 0777 | 回调−5 | 两者均调用 | 0（忽略err） |
| B120-04 | 2，filename_create返回ERR_PTR(−2) | 错误指针 | 否 | 未执行掩码 | PTR_ERR=−2 | **跳过done**，执行putname | 0（忽略err） |
| B120-05 | 2，正常dentry但`dir->i_op->mkdir=NULL` | 正常dentry | 否 | 未执行掩码 | 立即−EPERM | **两个都跳过** | 0（忽略err） |
| B120-06 | 3，`mkdir docs more` | 未调用 | 否 | 无 | syscall未调用 | 都未进入 | −1 |

以上B120-04是给定有效name和`filename_create`返回错误值的纸面分支；如果getname本身是ERR_PTR，实际`putname(name)`及上游使用会涉及另一项已有历史风险，**不能把错误指针当正常的可释放对象**。`// mode &= ~current_umask()`是注释，非执行。三个mode层次必须分开：用户传0777、`do_mkdirat`按01777掩后仍0777、**文件系统最终权限存储未知**。成功回调0仅代表此处收到0，不能认证目录已经在盘面持久存在。

## C · rm/rmdir：静态返回与用户可见错误字符串之间有缺口

删除命令索引5(`rm`)和7(`rmdir`)都先判断`argc==2`，否则立即−1且不在命令体中调用删除函数或printf。合法参数时，`rm_command`把`argv[1]`给`unlink`，`rmdir_command`把同等来源的`argv[1]`给`rmdir`，并把用户桩返回值存入局部`err`；不等于0则**在该命令体内**请求错误printf。run_cmd的void实现忽略两者的最终err，main在read_line后另有独立LF。

| case_id | 输入 / argc | 是否调内核路径 | 给定用户桩结果 | 命令体请求 | 内建返回 |
|---|---|---|---:|---|---:|
| C120-01 | `rm` /1 | 否 | 未调用 | 无自身printf | −1 |
| C120-02 | `rm note` /2 | unlink | 0 | 无自身printf | 0 |
| C120-03 | `rm note` /2 | unlink | −5 | `printf("rm: %s: %s\n",strerror(errno),"note")` | −5 |
| C120-04 | `rmdir` /1 | 否 | 未调用 | 无自身printf | −1 |
| C120-05 | `rmdir docs` /2 | rmdir | 0 | 无自身printf | 0 |
| C120-06 | `rmdir docs` /2 | rmdir | −5 | `printf("rmdir: %s: %s\n",strerror(errno),"docs")` | −5 |
| C120-07 | `rmdir docs extra` /3 | 否 | 未调用 | 无自身printf | −1 |

活动映射为`rmdir`用户SYSFUNC_DEF、rax84→活动switch→`MYOS_SYSCALL_DEFINE1(rmdir)`→`do_rmdir(AT_FDCWD,getname(pathname))`，另一条`unlink`为rax87→`MYOS_SYSCALL_DEFINE1(unlink)`→`do_unlinkat(AT_FDCWD,getname(pathname))`。二者在`namei.c`的父路径、末段类型、负dentry、方法可用性检查顺序不相同；本批最多接到所读方法入口。rmdir在`vfs_rmdir`调`dir->i_op->rmdir`，unlink在`vfs_unlink`调`dir->i_op->unlink`，真实FAT内部效果仍按57/58已审限界。

错误文字的真正缺口：用户桩`syscall.c:74–81`只保留`cmpq`与`jb`等活动指令，`movq %rax,__errno`和`orq $-1,%rax`两条处于注释；固定`errlist.c::__get_errlist`只描述传入错误号时的取表方式，**不能证明本次负返回已更新shell的errno，也未在本批看到由负值到某条具体英文错误文本的完整活动关系**。所以不能把给定−5直接说成必然显示某个固定字符串。

若**另外指定**模型输入`strerror(errno) → "EXAMPLE_ERROR"`，才能得出请求字节`"rm: EXAMPLE_ERROR: note\n"`或`"rmdir: EXAMPLE_ERROR: docs\n"`。它是人为预先设定的格式化输入，并非已证内核错误映射。另一方面，`printf.c`活动体只是向stdout的`write`发出请求，不能推断实机屏幕完整显示。

## 质量门及交回

本报告中的每个新P2锚都可通过`evidence.md`的FC120-E编号定位：参数门、mode宏及其八进制、有效系统调用路径、内核回调与返回、旧D00关键条件、time C配置都已覆盖。facts按相同case_id复算，DOT按真实节点/边区别静态绑定、函数指针调用、跨用户态内核边界、错误与数据。deferred分开记录旧三项仍OPEN、对象与ABI假设、errno、磁盘与写回等未证边界。

完整本地P9、预处理、编译、仓库程序、Graphviz、QEMU、实机、文件操作均NOT_RUN。新120只标`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，D00三项`CORRECTION_SUBMITTED_PENDING_C08_REVIEW`；由C08 Pro独立验收，执行对话不自签关闭、不写下一批。
