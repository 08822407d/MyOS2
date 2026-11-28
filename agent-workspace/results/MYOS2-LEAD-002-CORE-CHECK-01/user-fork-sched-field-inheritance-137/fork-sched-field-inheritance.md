---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-SCHED-FIELD-INHERITANCE-137
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-21
record_type: phase2_executor_analysis
date: 2026-11-23
produced_by: "GPT-6（文本模型族，实际后端未认证）"
model_per_owner: "GPT-6＋Extra（Owner指定）"
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---

# 第137批 · 普通fork后字段值来源、分时继承和调度计数

本件仅静态重放189合同与固定time源码。136的四项缺陷只在本新目录追加补正，原件、原评论和RETURN全部保留。作者不负责独立验收；完成后交原C09使用Pro。

## D00 · 136四项定点文档补正

**R136-01（启动与身份）**：冻结136 MANIFEST（129和162–165）没有在自身YAML中放入`startup_selfcheck_quote`、`branch_canary_quotes`或指导中的`task_local_only`。本批 MANIFEST直接载入从direct master公约读取的§1.2完整原句、time的两条金丝雀并在master反验不命中；本批独立G00记录保留。188/00给出的`task_local_only=true`仅是MyOS2本执行任务借用通用指导，不引入Mnemosyne自身任务。原135作者C08c／CHAT-EXEC-20、136补正与137作者C09a／CHAT-EXEC-21分别署名。既有136交接/刷新不在本批冒充重新办理。

**R136-02（证据纠偏）**：将 E136-001…038 各自原符号、物理行与新证据编号一一列于facts.D00.R136-02及本批evidence.md；旧38段的文本命中不等于38个行为正式证据。E136-014重锚为仅活动 INIT_LIST_S（新 E137-014），注释另列；E136-022、023退为纯注释定位，不计新正式窗。新证据另外包括 CONFIG_BUG（CMake E137-073）、受其条件包围的BUG_ON宏（E137-072）、列表头文件的`LIST_DEFINATION || !DEBUG`条件及double_list.c定义入口（独立定位）。没有预处理、编译或对运行时链接决断。D00子帧E137-011；A TASK_NEW/TASK_RUNNING宏E137-018/019；B四指针写E137-025；C回插/返回/自连E137-031/033/034/035/036，新C外层引用E137-056/057/060/061/062/063/064/071。机器件列所有新增源窗的真实ID。

**R136-03（合法链验证与B未知输入）**：136的`B.base.need_resched`及四个B插入后态给了false，但188未给B该初值；本批B统一写“初值未指定、wake片段没有此字段写点、后态沿用未知输入”，不把 C 中另行给的false搬到B。对四个有效(new,prev,next)=(K,H,Q)/(J,H,K)/(J,H,Q)/(K,H,J)检查六个无效谓词：prev空、next空、next.prev不等prev、prev.next不等next、new等prev、new等next。每个均逐项求值、值false；while条件初次为false，循环0次，函数返回true；外层`if(!valid)`为false跳过return，四次指针写+count++正常继续；对应E137-052/053/054和E137-025/023。四组正逆链尾均为R：

| new/prev/next | 源序四写 | 正序 | 逆序 | count |
|---|---|---|---|---|
| (K,H,Q) | Q.prev=K；K.next=Q；K.prev=H；H.next=K | H→K→Q→R→H | H→R→Q→K→H | 2→3 |
| (J,H,K) | K.prev=J；J.next=K；J.prev=H；H.next=J | H→J→K→Q→R→H | H→R→Q→K→J→H | 3→4 |
| (J,H,Q) | Q.prev=J；J.next=Q；J.prev=H；H.next=J | H→J→Q→R→H | H→R→Q→J→H | 2→3 |
| (K,H,J) | J.prev=K；K.next=J；K.prev=H；H.next=K | H→K→J→Q→R→H | H→R→Q→J→K→H | 3→4 |

**R136-04（真实比较方向与完整状态）**：旧节点`C136_103`标签误写used3<time_slice10；实际表达式是`used_jiffies >= rt->time_slice`，103样本应记**3>=10=false**，外OR为false，**不会求值**后半的队列count。C1由入K前 H-Q-R-H,count2,K=TASK_NEW自连；入K后H-K-Q-R-H,count3,K=TASK_RUNNING；picker后队列、P键3、last100与rq.curr=P不变，K仍TASK_RUNNING。C2从同样前两状态起，110时入picker取K并重自连，回插P时旧键比较3>Q20为false，while的anchor比较右项**没有求值**；P回插之后计费到13，last110、rq.curr=K，末队列H-P-Q-R-H,count3,K仍TASK_RUNNING。原回值、旧节点来源和假真条件在新图保留映射，不代表物理执行。

## A · 完整任务结构复制后，哪些字段重写

`dup_task_struct`内`*tsk = *orig` 是**独立子存储上的结构值复制**，同时复制嵌入标量和指针值，而不是共享父的rt.time_slice标量。P的rt.run_list在给定前态自连，故刚复制时子rt.run_list的next/prev **指向P的节点**，并非子节点自连；以后`INIT_LIST_S`才自连，唤醒插头后才与H和原首节点相连。copy_process把utime/stime/gtime清0，__sched_fork把se.vruntime清0，sched_fork写TASK_NEW/on_cpu=0，wake写TASK_RUNNING并入CPU0队列。rt.time_slice、nvcsw、nivcsw在这些函数指定活动语句范围内没有后续清零，保留复制时父值；不声称全局无其它写者。

| 字段 | 复制来源 | 随后活动覆写 | 唤醒后来源 |
|---|---|---|---|
| rt.time_slice | P.rt.time_slice | NO_ACTIVE_WRITE | COPIED_AT_FORK |
| se.vruntime | P.se.vruntime | __sched_fork sets 0 | 0 |
| utime | P.utime | copy_process zero | 0 |
| stime | P.stime | copy_process zero | 0 |
| gtime | P.gtime | copy_process zero | 0 |
| nvcsw | P.nvcsw | NO_ACTIVE_WRITE | COPIED_AT_FORK |
| nivcsw | P.nivcsw | NO_ACTIVE_WRITE | COPIED_AT_FORK |
| __state | P.__state | sched_fork:TASK_NEW; wake_up_new_task:TASK_RUNNING | TASK_RUNNING |
| on_cpu | P.on_cpu | sched_fork sets 0 | 0 |
| rt.run_list.next | P.rt.run_list.next pointer ADDRESS | __sched_fork self; wake linked | LINKED_NEIGHBOR |
| rt.run_list.prev | P.rt.run_list.prev pointer ADDRESS | __sched_fork self; wake linked | LINKED_NEIGHBOR |

正式证据：E137-037复制、040 CPU时间、013/014 调度初始化、015/016/017状态与链、018/019两状态宏、048调度实体字段、046切换数成员。这里只说明选定字段，不把复制指针误当成新进程整体资源已经隔离。

## B · K和J读取父值的不同时间点

给定P初始切片4、vruntime23、utime/stime/gtime=17/8/2、nvcsw/nivcsw=7/11、TASK_RUNNING/on_cpu=1，自链，rq.curr=P，队列H-Q-R-H,count2。K在P切片4时复制，J在P被给定改成9后复制；随后P又被直接给定改为12。两次更改只是题设的外部赋值，**不是**推导出来的系统调用或调度操作。

|阶段|P切片|K切片/链形|J切片/链形|CPU0队列|count/curr|
|---|---:|---|---|---|---|
| B0 | 4 | 未创建 | 未创建 | H→Q→R→H | 2 / P |
| B1 | 4 | 4 / copied-pointer-to-P | 未创建 | H→Q→R→H | 2 / P |
| B2 | 4 | 4 / self-linked | 未创建 | H→Q→R→H | 2 / P |
| B3 | 4 | 4 / linked | 未创建 | H→K→Q→R→H | 3 / P |
| B4 | 9 | 4 / linked | 未创建 | H→K→Q→R→H | 3 / P |
| B5 | 9 | 4 / linked | 9 / copied-pointer-to-P | H→K→Q→R→H | 3 / P |
| B6 | 9 | 4 / linked | 9 / self-linked | H→K→Q→R→H | 3 / P |
| B7 | 9 | 4 / linked | 9 / linked | H→J→K→Q→R→H | 4 / P |
| B8 | 12 | 4 / linked | 9 / linked | H→J→K→Q→R→H | 4 / P |

子K、J独立保留切片4与9，不跟随父后续变12；两子vruntime和三项CPU时间从父拷贝值重设为0；切换计数仍7/11。结构复制瞬间链指针还是复制父节点的地址，初始化后子节点自连，入队后K先挂H与Q、J再插在H与K之间，同时K.prev从H变J。末队列H-J-K-Q-R-H,count4，尾R，rq.curr=P。每个阶段完整P/K/J字段及前后驱详列机器时间轴；物理首次执行和真实时间未知。

## C · 相同经过6 ticks时当前任务的切片决定外层分支

与B实际时间顺序无关，合同给了**两个独立**的`__schedule(SM_NONE)`场景。C1 current=prev=rq.curr=K，C2相应为J，二者当前不在H-Q-R-H的队列（count2），当前状态TASK_RUNNING、非idle、need_resched=false、vruntime0，nvcsw/nivcsw=7/11。Q/R键20/40，jiffies106、last100，used=6，rq.nr_switches=30。前述copy使K.rt.time_slice=4而J.rt.time_slice=9。本次myos_rt选择器的`rt=&current->rt`，所以每次比较的是**当前孩子**的切片。

`__schedule`在调用picker之前另执行`update_rq_clock`，它从`local_clock()`减rq.clock计算增量；这不是本题jiffies106与last100的相减，不给rq.clock虚构数值。随后`switch_count=&prev->nivcsw`；那段把选择指针改成nvcsw的条件代码被注释，本次不能称“自愿切换计数已递增”。

| 项 | C1=K(切片4) | C2=J(切片9) |
|---|---|---|
| used及切片比较 |6；6>=4=true|6；6>=9=false|
| need_resched、idle |false、false|false、false|
| OR状态项 |前一项已真，跳过|求值为false|
| AND count>0 |求值true（count2）|前一OR假，跳过|
| 入队前摘首 |取Q，count2→1，自连Q，余H-R-H|不摘任何节点|
| 当前重入队与旧键比较 |K旧键0>R40=false；anchor右项未求值；K插在R前count1→2|不发生|
| 计费及last |K键0→6；last100→106|J键0不变；last仍100|
| picker返回 / 写rq.curr |Q / Q|J / J|
| 末队列 |H-K-R-H,count2|H-Q-R-H,count2|
| `prev != next` |K!=Q=true|J==J=false|
| rq.nr_switches |30→31|30维持|
| prev.nivcsw / nvcsw |K 11→12 / 7不变|J 11不变 / 7不变|
| 外层RCU_INIT_POINTER |调用，额外写rq.curr=Q|不调用|
| context_switch 调用点 |**到达**，止于此|**跳过**|

两个样本在picker末尾均执行`rq->curr = retval`，但只有K例进一步执行外层`RCU_INIT_POINTER(rq->curr,next)`及两个切换计数；P和另一子任务在这两个指定片段无写入。结论只能到达`context_switch`的调用边界，不能据此证明真实切换/运行指令或返回用户态。支持窗：E137-026/027、029–036、055/056/057/060/061/062/063/064、067/071。

## 消费与禁令

四项R136由作者标`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，原136仍RETURN，原135历史补正已C09定为CLOSED_DOCUMENTATION_ONLY，组合PASS_PENDING_LOCAL。本件只限定普通fork字段与两给定样本；全部rt.time_slice/on_rq写者、实际CPU执行、并发和全局配置状态仍开放。完整本地P9、预处理、编译链接、仓库脚本、Graphviz、QEMU、实机、真实进程/PID/调度/时钟/文件操作均NOT_RUN。作者交回原C09，使用Pro独审且同轮排下一批，本执行会话G04后STOP_WRITING。
