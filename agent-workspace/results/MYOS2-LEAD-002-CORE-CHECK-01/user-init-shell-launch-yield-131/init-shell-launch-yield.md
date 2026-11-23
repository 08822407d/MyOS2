---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-INIT-SHELL-LAUNCH-YIELD-131
record_type: phase2_executor_analysis_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: MYOS2-A-C08c 代码分析执行
produced_by: GPT-6 (self-reported; backend/effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time（分支名）
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第131批 · init启动shell、参数/环境、一次yield路径

**作者静态交付，非C09独立验收。** 本批只复核固定time源码及既有历史机器件；未编译、运行init/sh、加载ELF或实际调度。C09在#7089219709判定130为RETURN，183要求三个精确补正。原R129-01已由C09文档关闭，不重复日期四例。

## D00：R127-02——先129、再130、最后131，不可直接套旧128值

固定128的facts和DOT中三组B的iteration/footer均为旧文本；固定129已有这**六条新的已核标签**。130的九项标签选择器中，六条仍错误地用128旧标签匹配，严格重放会失配。此次把其`old_label`逐条重绑为129的`new_label`，并保持130提出的仅循环体写入/循环外do0分离的最终标签。其余三个B-do0使用原128标签能够精确匹配一次，保留130最终文本。

更重要的是130的DOT又把三组B-switch和C127_M16_U3的switch/goto/body六项退回128文字。本次逐一将该六项恢复为129已核完整标签。合计六条B选择器重绑、六条B/C继承标签恢复、三条do0无须重绑。对于一字C127_M16_U3路径，case1预读a1并调整长度，然后**直接goto循环外do0**写D[136,144)，helper返回void，不存在为此样本执行do8..do1的循环体。

机器件对128 facts与DOT、129 facts与DOT、130纠正及131最终投影逐节点给出ID、旧值、新值和顺序。最终六个历史簇恰81节点/91条有类型边且端点齐全，保留已有18条边替代、19条新增边、43对寄存来源（a0=20/a1=23）、三组C各四个宏位置/一次直接helper调用/一条原入参返回数据；不撤销先前已核的B16真回边。

## D00：R130-01——活动宏、有效短窗及P6机械账

130旧引文E130-014/015/016选到errno.h注释中的旧0x400000xx，而活动值实际是**ENOENT=0x02、EACCES=0x0d、ENOTEMPTY=0x42**。131新证据E131-019/020/021分别重新引用对应活动宏。E130-003包含run_cmd函数头；新E131-012只取体内实际函数指针调用。E130-017包含数组定义头；E131-022只取初始化器元素`[0] = "Success",`。旧E130-031仅支持printf写请求；E131-030复核write请求，E131-031独立引用`return rv;`。以上旧ID→新证据/替代范围详见facts和evidence。

130六件的**裸前缀**统计为MANIFEST 1、其余主报告/机器件/图/延后0、evidence 44，共45；带路径和符号的正式源短窗确为44。额外1次来自旧MANIFEST第24行P2标题，不是源码引文。原作者自报44/44/0与裸前缀机械总数不一致。131提供旧第24行可还原的原UTF-8 hex、替代行与130的*虚拟阅读版本*：删去非证据的前缀后，130为44个正式窗/44个裸前缀/44次核对，不修改旧六件。此外补入原130 MANIFEST漏写的结构化`startup_selfcheck_quote`、`branch_canary_quotes`、`self_check`，并单列`inputs_read`的审查来源重建，不能冒充130作者当时的内部新读取。本131自身新证据**48个正式定义短窗**，12个声明/配置/注释定位，自己的P6账独立为48/48/0。

## D00：R130-02——格式化数据和两个返回值不是同一条链

130五张ERR图的错误边是s1→write标作WRITE_REQUEST，以及write→ret标作RETURN_SOURCE；后者将`write`的返回值误当成`printf`及命令自己的返回来源。131对cd、ls、cat、rmdir、rm逐例改建独立来源：每个命令自己的失败守卫使其直接调用printf；两个%s取得的字符串仅是`vsnprintf`的格式化数据；`vsnprintf rv`同时决定写请求长度和`printf`返回值；`write`的返回值被丢弃；命令的返回另来自自己的常量或底层API结果；`run_cmd`最后丢弃命令返回。两份机器件一致保留五个旧错误边选择器对（共10条），不向历史文件写回。

| 失败路径（errno与文字按旧合同单独给定） | 请求字节 | printf自身返回 | 命令失败返回的真实源码来源 |
|---|---:|---:|---|
| cd /lost | 37 | 37 | 常量1 |
| ls，默认. | 25 | 25 | 常量-1 |
| cat note→/note | 37 | 37 | 常量-1（源码意外打印ls:） |
| rmdir /tmp/d，argc=2 | 35 | 35 | `err`，从rmdir返回-1赋入 |
| rm /log，argc=2 | 28 | 28 | `err`，从unlink返回-1赋入 |

`rmdir_command`及`rm_command`均先检查`argc!=2`，成立时直接返回−1，不调用底层删除或printf，也不产生所列诊断请求。合同给定的`ls`请求25、`write`实际仅返回2时，源码printf仍返回25、ls_command返回−1，run_cmd不消费。错误表0槽只能据数组下标`[0]`选取；`SUCCESS`是前批显示标签，不是所读活动errno宏。include/stdio.h的数组与__get_errlist是两条**活动extern声明**（E131-027/028），string/string.h中strerror声明则为**被注释的定位**（E131-059）。仍然无法把shell的strerror调用连接到该数组或证明最终sh链接，真实error文本、locale保持UNKNOWN。

## A：init的三个流、fork三态与生命周期

init的CMake目标为`init`，shell的为`sh`，顶层myinitramfs配置把二者安装到boot。固定initd.c的main依序调用`freopen("/dev/console","r",stdin)`、`freopen(...,"w",stdout)`、`freopen(...,"w",stderr)`，均丢弃返回值；随后请求打印`Welcome to MyOS2\n\n`，最后执行`return normal_boot(argc,argv)`。这些只是源码调用顺序，不能保证console重定向成功或屏显出现。

`normal_boot`**只有一次**`fork()`。给定`rv=0`时，进入子分支、调用`getpid()`并保存到未再使用的局部`pid`，之后调用`execve(prog_name,args,envs)`。若成功替换，调用不返回，随后运行何种映像需独立证明；若execve返回（例如失败），分支末尾就是非void的`normal_boot`函数末尾，未出现值返回。由于main使用normal_boot结果作为自身return表达式，此路径不能推定稳定返回值——按C语言使用该无值结果属未定义行为，不把失败自动归为正常退出。

给定`rv>0`（例如1234）时走else，保存另一次`getpid()`到未用的局部`pid`，执行`while(1){sched_yield();}`。给定`rv=-1`的fork失败也走**同一else**、同一getpid和循环，没有专门错误处理。在所读函数里**未见**wait、fork重试或重启子进程；不能把一次yield说成自动唤醒子进程。

## B：execve的filename、args与envs

在initd.c:24–35的有效对象范围，`prog_name`为局部字符数组`/boot/sh`（ASCII 8字节+NUL1），`filename`与`args[0]`是同一数组的地址。`args`与`envs`各有4个指针槽，前三个是有效字符串指针，末尾第4槽为NULL空指针，与各字符串终止NUL是**两种不同对象/语义**：

| 槽位 | args | envs |
|---|---|---|
| 0 | /boot/sh | userenv_test_1 |
| 1 | userarg_test_1 | userenv_test_2 |
| 2 | userarg_test_2 | userenv_test_3 |
| 3 | NULL | NULL |

五条测试字符串均为ASCII14字节，各另有1个终止NUL；prog_name是8+1。按7次参数字符串**引用**（filename、args三项、envs三项）相加，不含NUL=86、含NUL=93字节；按六种**不同字符串内容/目标数据**相加，不含NUL=78、含NUL=84字节。路径因两次引用只存一份prog_name对象；指针槽本身不计入字符串内容字节。main传入normal_boot的外层argc/argv并未作为execve的本地args/envs替身。

envs三项只是字面`userenv_test_1/2/3`，**均不含等号**；不能假定它们具有通常`NAME=value`环境变量的解释。用户态实际字符串地址、执行栈布置、动态链接、映像替换与进入sh的main均没有运行证明。

## C：一次sched_yield的调度源码路径

固定x86-64活动号`__NR_sched_yield=24`，syscalls_64.h列`__SYSCALL(__NR_sched_yield,sys_sched_yield)`，syscall_64.c的`x64_sys_call`使用活动switch宏建立一个候选映射。**真实用户态musl`sched_yield`到内核号和入口的动态链接仍是UNKNOWN**，以下是给定该入口成立时的内核源码展开。

`MYOS_SYSCALL_DEFINE0(sched_yield)`先取得`this_rq_lock_irq(&rf)`的rq，再通过`current->sched_class->yield_task(rq)`间接调用当前类回调，然后依次调用`preempt_disable()`、`sched_preempt_enable_no_resched()`、`schedule()`，通常函数返回0。夹在两条抢占操作之间的`rq_unlock_irq(rq,&rf)`是**注释，不是活动调用**；不在本包外推锁确已释放、IRQ恢复或抢占API实际语义。

若额外给定当前类是myos_rt，类表`.yield_task=yield_task_fair_myos`，该函数调用`resched_curr(rq)`。后者先查看rq->curr的need_resched标志：已设时立即返回，不重复置标；未设且rq属当前CPU时依次调用`set_tsk_need_resched(curr)`与`set_preempt_need_resched()`，然后返回。远CPU分支的发送IPI调用在所读函数中是注释，不能宣称已发送。无论这两种同CPU给定情况，外层sys_sched_yield源码仍有`schedule()`调用，但**不保证调到不同任务、唤醒子进程或实现公平性**。实际切换与运行进度还要其他源码和授权运行证据。

## 结果边界与交接

本批六件不修改源码、旧报告、任何PR元数据。补正R127-02、R130-01、R130-02均为CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW；第127与130批的既有RETURN在C09独判前保持有效。阶段2，不运行完整本地P9、预处理、编译链接、Graphviz、QEMU、仓内脚本、实机、真实进程、调度或文件操作。作者只交付到原exec，审查与下一合同由C09 Pro负责。
