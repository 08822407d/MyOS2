---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-TASK-COMM-FORK-EXEC-138
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: phase2_executor_static_analysis_report
date: 2026-11-23
produced_by: "GPT-6（模型族可识别，实际后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第138批 · 任务名的结构复制、内核字节复制、exec改名和有界读

本批只对固定time源码与合同给定的有效对象进行静态分析。不改136/137冻结六件，不写旧评论，不签独立验收。D00三项均由本作者标 `CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，交C09的Pro复审。

## D00 · 三项历史文档退回

### R136-04余项：纠正真实节点/类型，而不是仅重写说明

137新件的 `facts.D00.R136-04.new_graph_node` 曾自述：节点`D137_R13604_C1_GE_TEST`的kind为`PREDICATE_GE_FALSE`、false边kind为`FALSE_SKIP_COUNT_AND_PICKER_BODY`。但是在同批 **真正的 facts.graph / DOT** 中该节点的kind为`condition`、label为`3 >= 10 false (not 3<10)`，实图边一是`D137_R13604_C1_GE_TEST → D137_R13604_C1_SKIP`，kind=`FALSE_3_GE_10`、label空；边二为`D137_R13604_C1_SKIP → D137_R13604_C1_OUT`，kind=`SHORT_CIRCUIT_COUNT`、label空。声明的kind/false边kind在实际旧图**没有对应项**，不能把元数据误当作实图。

新的 `D138_GE → D138_STATE → D138_OR → D138_SKIP → D138_P` 五节点和四条有类型边，按真实源码分开了：先算**3≥10是假**；接着必须对`P.__state != TASK_RUNNING`求值、结果为假；因此整个OR为假；`&&`右侧的队列count完全不求值；picker体跳过，最后`rq.curr=P`并返回P。旧136 C1/C2三时点、返回P/K和K两种后态完整保留在facts对应字段，不把新布尔链造作实际CPU事件。依据 E138-036/037。

### R137-01：RQ时钟的调用不等于值一定变化

137 `facts.C.clk_vs_jiffies` 及正式作者回执的无条件“rq.clock已更新”描述过强。固定源码 `__schedule` 先左移 `rq->clock_update_flags`，然后调用 `update_rq_clock(rq)`，调用返回后还会把flags写为`RQCF_UPDATED`；这三个语句在给定继续到选择器的路径中到达，但对`rq.clock`值是否变化，必须取决于 `local_clock()` 的采样。以下三例各自从rq.clock=1000开始，互不承接：

| local_clock | delta | delta为负 | 早返 | 执行 +=delta | rq.clock末值 | 数值变化 |
|---:|---:|---|---|---|---:|---|
| 999 | -1 | 真 | 是 | 否 | 1000 | 否 |
| 1000 | 0 | 假 | 否 | 是 | 1000 | 否 |
| 1001 | 1 | 假 | 否 | 是 | 1001 | 是 |

负差提前返回不执行时钟加法；零差执行加法但值仍1000；正差执行并变为1001。三例的调用者flags前后写点均不被函数的普通早返跳过，但不给flags的原值和具体数值。原137的C1/C2**未给**local_clock或rq.clock，因此原两例不能补写上述任一时钟值。picker自有的`jiffies=106`、`last_jiffies=100`、used=6仍是独立输入，K片4选Q、J片9继续J两项结果不变。依据 E138-032至E138-036。

### R137-02：73项全覆盖、成员活动行与案例实参

原137 `evidence.md`有73个E137编号，本轮按固定time源码独立物理行复读**73/73匹配**；原 `facts-and-boundaries.yaml` 实际只出现52个不同编号，缺21个。新机器件 `D00.R137-02.old_window_crosswalk` **逐项列满73个**，标记旧facts是否有编号及真实缺项，`new_47_window_index`独立列出本批47个正式锚点，不能把新旧计数合并。原缺项为：

`E137-038, E137-039, E137-041, E137-042, E137-043, E137-044, E137-045, E137-046, E137-047, E137-049, E137-050, E137-051, E137-055, E137-058, E137-059, E137-065, E137-066, E137-067, E137-068, E137-069, E137-070`

冻结E137-048混有独立注释，本批把实际两个成员分别锚到`E138-038`（run_list第43行）与`E138-039`（time_slice第46行）；注释只用`L138-E048-COMMENT`定位。E137-049则以`E138-040`仅锚活动的195–196行；旧注释仅作`L138-E049-COMMENT`。

旧E137-028/031/035是**通用**摘首、当前任务回插、前节点插入。实际对象由调用样本决定：136 C2取**K**、插**P**于**Q**前；137 C1取**Q**、插**K**于**R**前；137 C2因外门假，均**不调用**。旧E137-033对应 `E138-044` 的retval赋值与last写，真正的return在旧E137-034对应的 `E138-045`。旧137作者评论把固定/活动一致性写成“提交前”，本次把它限定为**分支快进之后**的实际回读，不擅自编辑历史评论。以上映射与原ID/符号/行/处置均在机器件和证据正文留存。

## A · 内核 strscpy 与 strscpy_pad 的分工

所选 `strscpy` 定义在count=0时直接返回`-E2BIG`，不读src或写dest。count正数时，逐字节读`src[res]`写`dest[res]`，遇NUL返回当前字符数；若容量先耗尽，则将刚写入的**最后一个字节**再覆盖为NUL并返回`-E2BIG`。外层`strscpy_pad`先调用内层，内层负数或恰为`count-1`时直接传播返回而不补零；否则从已写NUL后继续memset直至本次count边界。以下目的数组每次独立初始化为8个0x5A，输入ASCII/NUL终止，绝无重叠。

| 样本 | 源 | count | 源读取下标 | 内层写下标 | 末字节改写下标 | 外层补零下标 | 内层／外层返回 | 最终8字节hex |
|---|---|---:|---|---|---|---|---|---|
| A0 | ab | 0 | 无 | 无 | 无 | 无 | -E2BIG / -E2BIG | 5A 5A 5A 5A 5A 5A 5A 5A |
| A1 | ab | 1 | 0 | 0 | 0 | 无 | -E2BIG / -E2BIG | 00 5A 5A 5A 5A 5A 5A 5A |
| A2 | ab | 3 | 0,1,2 | 0,1,2 | 无 | 无 | 2 / 2 | 61 62 00 5A 5A 5A 5A 5A |
| A3 | ab | 5 | 0,1,2 | 0,1,2 | 无 | 3,4 | 2 / 2 | 61 62 00 00 00 5A 5A 5A |
| A4 | abcdef | 4 | 0,1,2,3 | 0,1,2,3 | 3 | 无 | -E2BIG / -E2BIG | 61 62 63 00 5A 5A 5A 5A |

A0 无源、目标的本次读写；A1在下标0先写'a'随后覆盖零；A2在下标2写原有NUL，恰好装下无额外memset；A3在下标2读到NUL后另补下标3、4；A4读取并写入abcd（0–3）后覆写原'd'（3）为NUL，保留abc。count之外的全部0x5A维持初值。证据 E138-010至017，声明位置L138-STRSCPY-DECL、L138-PAD-DECL及源码条件单独列于evidence，不将注释的word-at-a-time循环算作实际操作。

## B · fork复制完整32字节；有name时才重写

内核`TASK_COMM_LEN=32`且`task_struct.comm`是**内嵌**`char[32]`。父P原始字节是：0–3=`init`，4=NUL，5–31=27个`0x5A`。选定普通fork的`kclone_args_s`只显式指定exit_signal，故`args.name=NULL`；`dup_task_struct`用`*tsk=*orig`按值复制整个数组到独立的K存储，`copy_process`的`if(args->name)`为假，**不经过命名拷贝**。K仍保留NUL后27个0x5A，P原数组不变。

对照J是已给`kernel_thread(...,name="io",...)`且走到同一name门，参数`.name=name`非空；`copy_process`**直接**调用`strscpy_pad(J.comm,"io",32)`，不是调用`__set_task_comm`。J末值下标0='i'、1='o'、2=NUL、3–31为29个零；两层strscpy返回2但copy_process不把其当返回值。P/K/J的内嵌数组相互独立。

单独用初值8个0x5A的buf调用`__get_task_comm(buf,8,K)`，它以K为锁目标，`task_lock(K)`取得K.alloc_lock，内部strscpy先写`init\0`返回4，外层补下标5、6、7为零，解锁并返回**同一个buf指针**。buf八字节为`69 6E 69 74 00 00 00 00`，K的32字节原数组没有被改动。证据 E138-001至009、018/019、030/031。

## C · execve使用的是路径字符串，而不是注释里的basename

C1/C2为相互独立的K样本，均以B1 K原数组`init\0` +27个0x5A开始，P/J不变。题设保证普通execve经过`do_execve(getname(filename),argv,envp)`并以`AT_FDCWD`到达`alloc_bprm`，故`bprm->filename=filename->name`，最初`bprm->interp=bprm->filename`指向同一源串；`begin_new_exec`令`me=current=K`，实际调用`__set_task_comm(me,bprm->filename)`。给定的argv[0]=display-name与此命名实参不同。源码注释“excluding path”不构成活动basename操作。setter是void，内部两层复制的数值不会经setter返回；加锁和解锁围住K.comm复制。

**C1**：源`/opt/demo/worker`长度16。setter内外层均返回16，K.comm[0–15]为完整路径、[16]=NUL、[17–31]=15个零。独立8字节读者buf初值0x5A，实际得到`2F 6F 70 74 2F 64 65 00`（`/opt/de\0`）。读者内层返回`-E2BIG`，外层`__get_task_comm`仍返回buf指针，不返回这个整数；K.comm原数组不变。

**C2**：源`/opt/demo/abcdefghijklmnopqrstuvwxyz012345`长度42。setter经容量32复制源前32字节后把下标31覆盖NUL；内外`strscpy_pad`均为`-E2BIG`，void setter不传播该值。最终K.comm下标0–30严格为31字符`/opt/demo/abcdefghijklmnopqrstu`、下标31=NUL。独立32字节getter复制这31字节及NUL，内部返回31，因written==count−1不做memset，外层返回buf原指针。32字节buf与K.comm内容一致。

**C3**：给定`do_execveat_common`入口的filename为`ERR_PTR(-ENOENT)`。函数开头`IS_ERR(filename)`为真并`PTR_ERR`返回`-ENOENT`，没有到`alloc_bprm`、`begin_new_exec`或setter；K仍为`init\0`及27个0x5A。随后独立的8字节getter与B相同，内部返回4、buf内容`69 6E 69 74 00 00 00 00`、外层返回buf指针。

两种方向的锁均由`task_lock(tsk)`与`task_unlock(tsk)`围住各自`strscpy_pad`，对应的是K.alloc_lock；B2 copy_process 的直接name分支不经过setter。无法从这些局部语句推出完整exec成功、实际并发保证、物理任务恢复或用户程序观察。依据 E138-018至031及L138-COMM-COMMENT。

## 证据和边界

本批47条正式源码窗在同一`evidence.md`，全部逐字来自固定time实际符号体、1–5行；facts.D00.R137-02.new_47_window_index给47个唯一ID对应的路径、行与窄断言。事实图有56节点、42条有向类型边，与DOT从同一机器数组生成，未运行Graphviz。四组样本仅是独立给定输入的推演，无完整本地P9、预处理、编译链接、仓库脚本、QEMU或实机运行。最终由C09使用Pro独立复核本批三个D00和A/B/C，同轮签发后继；本执行身份G04后STOP_WRITING。
