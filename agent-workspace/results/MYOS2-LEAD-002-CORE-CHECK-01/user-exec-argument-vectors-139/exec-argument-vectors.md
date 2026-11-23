---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-EXEC-ARGUMENT-VECTORS-139
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: phase2_executor_static_analysis_report
date: 2026-11-23
produced_by: "GPT-6（模型族，后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第139批 · exec文件名、argv指针计数、参数字节与空向量政策

本批所有对象均为191合同给定的独立合法静态场景。没有执行MyOS2，也不改变原136/137/138冻结件。四项D00由本作者提交补正，均为`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`；原C09使用Pro独立判闭，原138的RETURN保留。

## D00 · 第138批四项定点补正

### R138-01 · 截断覆零属于内层，而非外层

冻结138的A1/A4图实物按 `INPUT→INNER(已经标 -E2BIG)→PAD(才写rewrite)→RESULT` 排列，文字后态虽正确，因果次序却错。新图 `D139_A1_*` 与 `D139_A4_*` 分别明确：
- A1先在 `strscpy` 内层写`dest[0]='a'`，容量耗尽后同一内层执行`dest[0]=NUL`，**然后**返回`-E2BIG`；外层`strscpy_pad`因负数直接返回，**不做memset**。
- A4先在下标0–3写入`abcd`，再由内层把下标3的`d`覆为NUL，**然后**内层负返回，外层直接传播。
- A3对照源串`ab\0`在下标0–2写完，内层返回2；外层判`2<0`假、`2==count-1(4)`假，**随后才memset下标3、4**并返回2。

原A0–A4两层10个返回值、40个输出字节均逐个沿用并写入机器件；旧节点/九条边的真实label与kind也在`D00["R138-01"]`保留。固定源窗`E139-077/078/079/080`直接支持“读写、覆零、返回、包装判断”源序。

### R138-02 · 两次独立fork，不是K创建J；C3无setter

冻结138的三条B边将`B138_P→B138_K→B138_J→B138_GET`误画为一条依赖链，旧C.shared_premises还把C1/C2的bprm、setter与其锁套到C3。本批的新图中`D139_P→D139_K`、`D139_P→D139_J`分别为两次**独立**父结构复制；J只经自己的name门改为`io\0`后29个零；`D139_K→D139_KGET`明确getter取K的数组，不通过J。P、K、J及读者buf是不同对象。

新图的C1/C2各自包含filename→bprm→setter锁K.alloc_lock→K.comm写入→解锁→另一次getter重新取得K.alloc_lock→精确读者buf。C3图只从`ERR_PTR(-ENOENT)`入口早返`-ENOENT`，**不进入bprm、setter、setter锁**；随后另有一次对原K的独立getter。三例分别为：
| 旧C案例 | getter容量 | 写入buf的准确结果 | 内部复制返回 | getter本身返回 |
|---|---:|---|---|---|
| C1 | 8 | `2F 6F 70 74 2F 64 65 00` | `-E2BIG` | 原buf指针 |
| C2 | 32 | 下标0–30为`/opt/demo/abcdefghijklmnopqrstu`，31为`00` | `31` | 原buf指针 |
| C3 | 8 | `69 6E 69 74 00 00 00 00` | `4` | 原buf指针 |

资料来自`D00["R138-02"]`与冻结138机器实例；新图的C3没有被偷偷连接到C1/C2的setter。源锚`E139-081–086`支持限定写、读与早返。

### R138-03 · 修复证据表真实键与全部47项定义身份

旧138正文第41行以及evidence第17、552行曾用`D00.R137_02.old_window_crosswalk`或`R137_02`，事实上该键不在旧机器件中。真实可解析路径是 `D00["R137-02"].all_73_old_windows`（73项）与同级`D00["R137-02"].new_47_window_index`（47项）。本批`D00["R138-03"].old_47_crosswalk`按旧ID逐条列全47个源位置／原标题／引文文本／处置；本批的`new_92_index`则逐条列出自身92个窗口的文件、符号、首尾行、角色及正文锚，完全不混淆两批。旧47条为**46个不同物理区间**（E138-037/045共窗）；本批新92条为**91个不同物理区间**。

E138-010改锚`E139-088`中真正活动的`max=count; res=0`，并不声称对入参count赋初值。E138-027采用`E139-023`的完整`MYOS_SYSCALL_DEFINE3(execve,...)`参数定义及实际`return do_execve(...)`，不推测宏展开。旧E138-006/010/014/018/034中独立注释降为定位；E138-040活动行里的原行内文字逐字保留，不能由注释多推行为。涉及配置的判定仅限固定`mykernel/scripts/options_flags.cmake`的`E139-087`活动选项窗；没有预处理，不宣称全仓其它配置宏不存在或最终二进制采用何种优化分支。**源窗逐字存在和旧标题能否获其支持分别判断。**

### R138-04 · flags初值未知，但末值有固定宏

固定`__schedule`源序是先`rq->clock_update_flags <<= 1`，调用`update_rq_clock`，函数返回后**无条件**`rq->clock_update_flags = RQCF_UPDATED`；同一time宏`RQCF_UPDATED=0x04`。因此三个阶段为：初态`F`（数值未给）、左移后`F<<1`（数值未给）、调用返回后`0x04`（数值明确）。不因`F`未知就给末值也填UNKNOWN。

原R137-01在C09已`CLOSED_DOCUMENTATION_ONLY`，本批不重开。给定rq.clock=1000时local_clock为999/1000/1001，仍是负差不加、零差加零、正差加一；picker独立用106−100=6，K时间片4选择Q、J时间片9继续J，均不因本次flags补正而改写。证据`E139-075/076`。

## A · 四种向量：用户get_user与内核直接数组索引

四组向量各自从合法独立输入开始，max足够，用户槽访问不失败。用户版先`if(argv != NULL)`再逐次`get_user_arg_ptr(argv,i)`，得到非空**指针**才计数，遇NULL槽跳出；内核版在非NULL向量上直接测试`argv[i]`，遇NULL结束。**计数阶段只读取指针槽，不读取指针指向的字符串字节。** 空字符串指针不是NULL，因此A2计数为1。

| 样本 | argv对象 | 用户计数阶段读取的槽下标（get_user次数） | user count | 内核数组直接读取下标 | kernel count | 后续复制阶段再次读取的非空槽 |
|---|---|---|---:|---|---:|---|
| A0 | argv=NULL | 无（0次） | 0 | 无 | 0 | 无 |
| A1 | argv[0]=NULL | 0（1次） | 0 | 0 | 0 | 无 |
| A2 | argv[0] nonnull pointer to NUL | 0,1（2次） | 1 | 0,1 | 1 | 0 |
| A3 | two nonempty strings and NULL | 0,1,2（3次） | 2 | 0,1,2 | 2 | 1,0 |

特别注意：A0传的是**整个argv=NULL**，所以根本不尝试读取槽；A1是有效数组而其首槽NULL，读取槽0后立刻结束；A2首槽指向`"\0"`，读取0、1，计数1，在**后续复制阶段**才通过`strnlen_user`或`copy_string_kernel`读取那个NUL字节；A3读取0、1、2并计数2，实际复制又按1→0逆序重新读槽。计数的4组用户槽调用数为0、1、2、3，内核数组直接判槽同样0、1、2、3。源锚E139-001至010及补充E139-091。

## B · 一次普通execve的五段实际字节与T游标

给定filename=`/bin/tool`，argv=`{"display","-q",NULL}`、envp=`{"E=1","F=xy",NULL}`，AT_FDCWD；所有字符为ASCII且都带终止零。64位指针宽按题设是8字节。初始游标`T=vma->vm_end-sizeof(void*)`。`filename->name`和`bprm->filename`在本路径别名指向原字符串；`bprm.exec`是**把文件名字节复制入新栈后的地址数值**，而不是原文件名指针，更不是argv[0]。

**预算与p移动分开**：固定`_STK_LIM=8×1024×1024=8,388,608`；四分之三为6,291,456；`ARG_MAX=131,072`，`max_t`后仍6,291,456。此例`argc=2`、`envc=2`，指针预算`(max(2,1)+2)×8=32`字节。扣减后字符串限额6,291,424字节，`argmin=T−6,291,424`。**扣限额本身不扣bprm.p**，这一步p仍为T，随后字符串复制才依实际含NUL长度逐次下移。

### 按时间先后复制

| 步骤 | 来源对象 | 函数 | 含NUL长度 | 进入时p | 离开时p | 新写入半开区间 | 本次字节 |
|---|---|---|---:|---|---|---|---|
| B1_FILENAME | bprm->filename alias filename->name | copy_string_kernel | 10 | T | T-10 | [T-10, T0) | `undefined` |
| B2_ENV_F | envp[1] | copy_strings (user reverse) + strnlen_user | 5 | T-10 | T-15 | [T-15, T-10) | `undefined` |
| B3_ENV_E | envp[0] | copy_strings (user reverse) + strnlen_user | 4 | T-15 | T-19 | [T-19, T-15) | `undefined` |
| B4_ARG_Q | argv[1] | copy_strings (user reverse) + strnlen_user | 3 | T-19 | T-22 | [T-22, T-19) | `undefined` |
| B5_ARG_DISPLAY | argv[0] | copy_strings (user reverse) + strnlen_user | 8 | T-22 | T-30 | [T-30, T-22) | `undefined` |

文件名先写完10字节并保存`bprm.exec=T-10`，然后按envp[1]、envp[0]、argv[1]、argv[0]逆序。每个用户字符串由`get_user_arg_ptr`再次取槽、`strnlen_user`测得**含NUL长度**，再根据`bprm.p-=len`与`memcpy_to_page`写入；文件名走`copy_string_kernel`，用`strnlen(...)+1`。在所有长度、页取得及复制都成功的给定前提下，最终p=T−30。

### 按新栈地址从低到高

| 半开地址区间 | 字符串 | 具体十六进制字节 |
|---|---|---|
| [T-30, T-22) | display + NUL | 64 69 73 70 6C 61 79 00 |
| [T-22, T-19) | -q + NUL | 2D 71 00 |
| [T-19, T-15) | E=1 + NUL | 45 3D 31 00 |
| [T-15, T-10) | F=xy + NUL | 46 3D 78 79 00 |
| [T-10, T0) | /bin/tool + NUL | 2F 62 69 6E 2F 74 6F 6F 6C 00 |

因此完整`[T−30,T)`的30字节为：
`64 69 73 70 6C 61 79 00 2D 71 00 45 3D 31 00 46 3D 78 79 00 2F 62 69 6E 2F 74 6F 6F 6C 00`

**三个身份绝不混同**：原`filename->name`是来源；`bprm.exec=T-10`定位新栈`/bin/tool\0`；`argv[0]`在复制后位于`T-30`，内容为`display\0`。到`bprm_execve`调用点为止，不能替未来`setup_arg_pages`可能施加的stack_shift填绝对地址；后续`AT_EXECFN`确实引用`bprm.exec`，但不是已经构造完整ELF栈指针表。关键源锚E139-011至058。

## C · 用户侧合成空argv[0]；kernel_execve零计数拒绝

六例全部独立从各自新建bprm的T开始，filename仍为`/bin/tool`，envp均为`{"E=1","F=xy",NULL}`。用户三例从`do_execveat_common`进入；内核三例从`kernel_execve`进入，另外给定当前`PF_KTHREAD`位不置、名称取得与bprm分配成功。

| 样本 | 计数初值 | pr_warn_once调用点 | bprm.argc首次写入 → 末值 | envc写入 | 指针预算 | bprm.exec | 末p | 合成空串 | 下一步 |
|---|---:|---|---|---|---|---|---|---|---|
| C-U0 | 0 | 到达 | 0 → 1 | 2 | 24 | T-10 | T-20 | 合成空串 | bprm_execve() call reached |
| C-U1 | 0 | 到达 | 0 → 1 | 2 | 24 | T-10 | T-20 | 合成空串 | bprm_execve() call reached |
| C-U2 | 1 | 跳过 | 1 → 1 | 2 | 24 | T-10 | T-20 | 未合成 | bprm_execve() call reached |
| C-K0 | 0 | 跳过 | SKIPPED → SKIPPED | SKIPPED | SKIPPED | SKIPPED | T | 未合成 | retval=-EINVAL; goto out_free -> out_ret -> return -EINVAL |
| C-K1 | 0 | 跳过 | SKIPPED → SKIPPED | SKIPPED | SKIPPED | SKIPPED | T | 未合成 | retval=-EINVAL; goto out_free -> out_ret -> return -EINVAL |
| C-K2 | 1 | 跳过 | 1 → 1 | 2 | 24 | T-10 | T-20 | 未合成 | bprm_execve() call reached |

U0（argv整体NULL）和U1（argv数组首槽NULL）计数都是0，故两者均到达`pr_warn_once`调用点；不据调用点宣称日志已真正输出。两例`copy_strings(argc=0)`没有复制参数，先保存文件名10字节、复制环境9字节，之后`copy_string_kernel("")`**额外写入一个NUL字节**，才把`argc=0`改写为1；p由T−19到T−20。

U2 首槽不是NULL而是**指向空串**，故计数1、没有警告调用；普通用户`copy_strings(1)`重新读argv[0]并以`strnlen_user`获得含NUL长度1，复制现成字符串的一字节NUL到T−20；**合成分支完全跳过**。用户三个样本最终`argc=1`、`envc=2`、`bprm.exec=T−10`、p=T−20，指针预算均24字节，`argmin=T−6,291,432`，但字节来源不同。

K0（整个argv NULL）和K1（首槽NULL）在`count_strings_kernel`返回0后立即将`retval=-EINVAL`并跳`out_free`。因此它们**不再调用envp计数、不赋bprm.argc/envc、不作指针预算、不复制文件名/环境/参数、不保存bprm.exec、不调用bprm_execve**。在给定初始p=T且中间无额外写入时，p仍T；通常清理调用正常返回下经`free_bprm`、`putname`走到函数返回`-EINVAL`。所有未到达字段标`SKIPPED`，不虚构最终值。

K2 首槽为合法`"\0"`指针，内核直接`argv[0]`计1，env计2，预算24；`copy_strings_kernel(1)`从原数组取该空串，用`copy_string_kernel`复制一字节NUL，**不是用户版的合成分支**。结果与U2同为p=T−20、`argc=1`、`envc=2`、`bprm.exec=T−10`，止于`bprm_execve`调用边界。上述全部值与source-dependent条件见facts.C.cases，源锚E139-059至074。

## 交付、适用范围与禁止操作

本批四项D00均待原C09独立验收。机器件对两种计数路径、五段字节、六个C样本使用独立case ID与真实对象归属；同源DOT包含120节点和103条具有ID/from/to/kind/label的唯一边，绝不把不同样本串成真实任务链。正式源码引文92条，来自91处物理区间，全部1–5行；原138的47条、46处物理区间另作为旧覆盖表复验，不加入新正式计数。

**运行禁令一律保持**：完整本地P9、预处理、编译、链接、仓库程序/脚本、Graphviz、QEMU、实机以及真实文件、进程、PID、调度、时钟操作均NOT_RUN。这里止于源级字符串入栈和bprm_execve调用边界，不证明最终用户栈布局、exec完成、进程实际运行或用户态输出。完成正式交付与发评后G04远端只读核验，作者即STOP_WRITING，交原C09用Pro独审并同轮发布后继。
