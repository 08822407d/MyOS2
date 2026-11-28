---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-YIELD-TASK-SELECTION-132
record_type: phase2_source_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: MYOS2-A-C08c 代码分析执行
produced_by: "GPT-6 self-reported; backend/effort UNATTESTED"
model_per_owner: "GPT-6＋Extra"
date: 2026-11-23
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第132批 · 一次 yield 选中谁、何时进入换任务分支、什么计数会变

**结论先行。** 四个严格给定的普通单CPU有效样本中：B1、B2返回当前任务P，不调用context_switch；B3由need_resched为真触发选择、B4由used_jiffies刚好等于time_slice触发选择，均从候选队列首部取Q，使先保存的prev=P与返回的next=Q不同，于是进入__schedule的换任务分支。B3/B4仅在所给源码与条件之下，才分别使rq.nr_switches由8到9、P.nivcsw由5到6；全部四例P.nvcsw维持4。**这是代码路径和数据的纸面演算，不是实际切换成功的证据。**

## G00 身份与裁定前提

原身份CHAT-EXEC-20、原会话C08c、原exec/Draft PR17。Owner本轮指派184号合同；现有模型自述GPT-6，Owner选择GPT-6＋Extra，后台实际型号及档位UNATTESTED。C09正式发布#7090769412，主线固定f07e60a5e8c0。前批作者#7090247003与C09裁定由GitHub按ID核读；G00直接refs：lead f07e60a5e8c0、exec 2089bfff6f4a、time a039d9803ade、直接master e8e60cd19540。PR16保持Open/Ready、PR17保持Open/Draft且原题，两者均指master、未合并；仅此两项开放PR。写前PR17无since顶层评论100/100/52/0到空共252，本批正式作者标记为0；132新目录原不存在。131单父7e52edee26b4，仅六个新增5,221行、185,935字节，固定/活动六件全文与Blob身份6/6相同。

time反向金丝雀：内核C参数有`-m64 -mcmodel=kernel -fno-pie -fno-pic`，panic.c调用`smp_processor_id()`；直接master分别是`-m64 -mcmodel=large -fPIE`、`raw_smp_processor_id()`。公约§1第2条限定唯一写区，只新增、不得动内核/任务/旧件。本会话既有Mnemosyne接收与指导刷新仍仅作行为指导，不重新冒充本批独立执行授权。

## D00 · 三项历史退回的追加纠正

### R130-02：五个printf诊断簇的取参者

冻结130的两条错误返回关系已由131修正；本轮在131的五个诊断簇按端点与类型各唯一命中三条仍不完整的关系：原`E131_{CASE}_printf -> E131_{CASE}_s0 [VARARG_FETCH0]`、`s0 -> s1 [FETCH_NEXT_VARARG_NOT_C_EVAL_ORDER]`、`request -> ignored_write [IGNORED_RESULT]`。修正前者为printf创建va_list，保留中者为**格式消费顺序而不是取参发起者**，把后者改成write调用结果被丢弃。新增三个实体节点`va_list`、`vsnprintf`、`write`和明确的调用/数据边：

- **发起者**：各命令失败分支直接调用`printf`；`printf`中`va_start`建立ap，并分别直接调用`vsnprintf(...,ap)`与`write(stdout->_fileno,buf,rv)`。
- **取参者**：只有`vsnprintf`在两次%s分支分别执行`va_arg(ap,const char*)`，第0与第1个取参事件都归其所有，并按格式字符串消费先后发生；不能推出调用者两实参表达式的求值顺序。
- **返回和弃用**：`vsnprintf`返回rv被赋给printf的局部rv，一方面作为write请求字节数，另一方面由printf直接返回；write自身返回值不保存；各命令失败值另由常量1、常量-1或rmdir/unlink的err产生，最终`run_cmd`丢弃命令结果。

| 样本 | 请求字节 | printf返回 | 命令返回 | 独立前提 |
|---|---:|---:|---:|---|
| cd /lost | 37 | 37 | 1 | chdir失败且给定文本 |
| ls 默认目录. | 25 | 25 | -1 | opendir失败且给定文本 |
| cat note，目录/ | 37 | 37 | -1 | open失败且保留源码错误前缀ls: |
| rmdir /tmp/d | 35 | 35 | -1 | argc=2且rmdir失败、err=-1 |
| rm /log | 28 | 28 | -1 | argc=2且unlink失败、err=-1 |

ls附加反例：**write实际只返回2**时，printf仍返回25，ls_command仍返回-1。若rmdir命令argc=1或rm命令argc=3，则不通过前置参数门，直接返回-1、不会调用底层删除/printf或请求这些输出。仓内错误表到shell的真实strerror/链接桥接依旧UNKNOWN。五组旧端点、替代和新边在facts与DOT逐项同源建模，不回写130/131原图。

### R131-01：一条跨定义引文、配置支持及130输入归属

旧E131-044的65–67行含`DEFINE_SCHED_CLASS(myos_rt)`定义头；以**myos_rt.c:66单行`.yield_task = yield_task_fair_myos,`**替换其正式P2证据，旧65行仅记为头部定位；本批新证据E132-001为真正表项。旧E131-053只有options_flags.cmake:41的`mcmodel`金丝雀，不能代替P4的活动配置证据；本批E132-002独立给出同一固定time配置第62–65行的`CONFIG_64BIT`、`CONFIG_PHYS_ADDR_T_64BIT`、`CONFIG_ZONE_DMA`、`CONFIG_ZONE_DMA32`、`CONFIG_SLUB`、`CONFIG_ARCH_HAS_SYSCALL_WRAPPER`等活动编译选项。其证明范围是**所读CMake源文本及所读函数**，不证明实际预处理/二进制配置，更不得据此作全仓无调用的否定断言。

从130固定evidence中44个带path::symbol的源窗独立去重出**11个路径**。131所造的旧`virtual_130_manifest_correction.inputs_read`只有8条：其中7条与旧证据同源；**遗漏**`stdio/errlist.c`、`string/memcpy.c`、`sysdeps/generic/memcopy.h`、`string/wordcopy.c`（均在myinitramfs/mylib/myglibc下）；而`myinitramfs/myinit/initd.c`没有旧130正式窗，属于131新专题，必须从旧源窗归属移出。新机器件把这11条列为`evidence_anchored_130`，其他来自合同、正文、评论的路径仅按其自己证据另列；不推测130作者不可证明的内在阅读史。

旧各计数**对象不能混用**：冻结130原件裸前缀45/正式窗44；若仅替换旧MANIFEST标题为44/44；若连同五个一对一替换以及一个新增printf return窗组合则45/45；冻结131原件仍为48/48，E044一对一虚拟替换仍有48个有效窗。本批132自有的新增正式短窗独立计算，旧文件冻结。

### R131-02：execve三根指针图、系统调用拥有者与A调用层级

新增`B_EXECVE`图簇的**三个实参根**`filename`、`args`、`envs`均来自initd.c:24–35的`execve(prog_name,args,envs)`。args与envs各四个指针槽（下标0–3），各自下标3保存NULL空指针；六个非空字符串目标各有一个尾部NUL字节节点，这不是空指针。filename与args[0]两条指针汇合到同一个`prog_name`字符数组`/boot/sh`，不重复分配对象；其余五个目标为两个`userarg_test_*`和三个`userenv_test_*`。按7个引用累计字符串内容为86字节，计每次引用的末尾NUL为93；按6个不同目标去重为78/84。指针自身地址、32/64位所占字节与实际用户栈布局不加入这些字符串数值，真实动态链接/进sh仍UNKNOWN。

A旧`main -> fork`合并调用边改成`main -> normal_boot -> fork`两个拥有者。C旧的rq→callback只传**rq实参数据**，系统调用体本身才通过`current->sched_class->yield_task(rq)`发起间接调用。给定当前类myos_rt，它的`.yield_task`才解析为`yield_task_fair_myos`，并直接调用`resched_curr`；与下面选任务时类遍历使用的`.pick_next_task`是两个不同的成员。内核系统调用体分别直接调用`preempt_disable`、`sched_preempt_enable_no_resched`及`schedule`，原preempt聚合节点→schedule只能表达源码先后，**不是preempt函数调用schedule**。调用终点通常返回常量0，异CPU远程通知及rq_unlock_irq均为注释边界。两载体明确区分调用、表项、实参、条件、先后与返回。

## A · 从schedule到类指针的一次非空pick

固定`sys_sched_yield`的直接调用现场在syscall.c:38。`schedule()`读取`current`，合同给定P为TASK_RUNNING，故`if (!task_is_running(tsk))`为假，不走`sched_submit_work`，再调用`__schedule_loop(SM_NONE)`。活动`SM_NONE=0`，循环是`do { preempt_disable(); __schedule(SM_NONE); sched_preempt_enable_no_resched(); } while(need_resched());`，无论初始标志如何，函数调用路径至少进入循环体一次；以后是否再迭代由循环底部**重新读取**的标志决定。

`__schedule`先以`smp_processor_id`和`cpu_rq`定位rq，再保存`prev=rq->curr`（合同初态P），随后设`switch_count=&prev->nivcsw`，读取prev状态，并通过`pick_next_task(rq,prev,&rf)`取next。这里`pick_next_task`在scheduler_core.c:976是`__pick_next_task`的**宏别名**。后者在scheduler.h:258遍历`for_each_class(class)`，逐类调用`class->pick_next_task(rq,prev)`，遇到非空p立即返回；否则到末尾`BUG()`。合同仅给定类遍历**已到myos_rt，且其回调返回非NULL**，不据此推定实际链接器对所有类的顺序。myos_rt.c:67的活动表项`.pick_next_task=pick_next_task_myos`确定此处转入下述函数。

同一sys_sched_yield中较早执行的`.yield_task`和这里类遍历的`.pick_next_task`调用点不同，不能互相替代。实际编译、链接、runqueue锁与抢占效果、系统调用桥接与循环次数均未由源码阅读证明。

## B · 四个独立合法场景的一次pick

固定共同前态：单CPU、队列结构有效，无并行改写或计数回绕，current=P非idle且TASK_RUNNING；R.vruntime=30、P.vruntime=10、time_slice=10、last_jiffies=100。B1–B4彼此**独立**，不是连续四轮调度。used_jiffies为jiffies−100；选择门为`(need_resched() || current==idle || used>=time_slice || current不RUNNING) && count>0`。短路意味着某些项目未求值，不能把所有分量都写作实际调用结果。

| 案例 | 队列初态 | need | jiffies/used | 满足选择门 | 选中/最终队列 | P键/last |
|---|---|---:|---:|---|---|---|
| B1 | 空,count0 | true | 103/3 | 否：右侧count>0为假 | P / 空 | 10 / 100 |
| B2 | Q,R,count2 | false | 103/3 | 否：非idle、3<10、RUNNING | P / Q,R | 10 / 100 |
| B3 | Q,R,count2 | true | 103/3 | 是：need为真 | Q / P,R | 13 / 103 |
| B4 | Q,R,count2 | false | 110/10 | 是：时间片等号`>=` | Q / P,R | 20 / 110 |

B1、B2仅保留初始化的`retval=current=P`，然后执行`rq->curr=P`并返回P；没有摘首、回插、计费、last_jiffies改写或队列计数改变。B1虽need为真，但空队列阻断；B2虽候选存在，但整个OR为假。

B3、B4都先从队首摘Q，`list_header_remove_head`调用`list_del_init(Q)`后**自身**执行count--（2→1）；此时余下R。P仍RUNNING且非idle，回插循环在读取R的比较键时使用**尚未计费的P.vruntime=10**，对R30测试`10>30`为假，直接`list_add_to_prev(P,R)`。该通用链表函数只链接P，并不负责队列count；回到`pick_next_task_myos`的第45行再**仅一次**count++（1→2），队列为P,R。随后才执行P.vruntime加used（B3加3得13、B4加10得20），retval从摘出的Q生成，last_jiffies写为当例103/110，最后`rq->curr=Q`并返回Q。不要把插前旧键与插后新键混作一次比较，也不要重复把count算到通用插入调用里。

这四例的排序/计费结论只在指定正常对象范围成立，02已有的W01–W08有限宿主见证及03全局入队/修复候选只引用范围，不重新运行、不将候选规范升为现行政策。

## C · prev/next如何触发两类切换计数

`__schedule`在选任务**以前**保存的`prev=P`不会随`pick_next_task_myos`对rq.curr的赋值自动改成Q。P在四例均RUNNING，活动`switch_count=&prev->nivcsw`在选取前建立；另一句`switch_count=&prev->nvcsw`在注释中。执行pick后，源码无条件调用`clear_tsk_need_resched(prev)`及`clear_preempt_need_resched()`，随后才判`if (likely(prev!=next))`：

| 案例 | prev/next | `prev!=next` | rq.nr_switches | P.nivcsw | P.nvcsw | context_switch |
|---|---|---:|---:|---:|---:|---|
| B1 | P/P | 假 | 8 | 5 | 4 | 不调用 |
| B2 | P/P | 假 | 8 | 5 | 4 | 不调用 |
| B3 | P/Q | 真 | 9 | 6 | 4 | 发起调用 |
| B4 | P/Q | 真 | 9 | 6 | 4 | 发起调用 |

真分支按所读代码`rq->nr_switches++`、`RCU_INIT_POINTER(rq->curr,next)`、`++*switch_count`、`context_switch(rq,prev,next,&rf)`，其中第二次对rq.curr的**RCU发布**与picker中直接`rq->curr=retval`不是同一个赋值点；前者仅B3/B4经过，后者四例均经过。此处`++*switch_count`写的是P.nivcsw，因而B3/B4是5→6而非P.nvcsw变化。假分支不执行这四项动作；两类末尾均存在`local_irq_enable()`调用现场，但真实切换后的到达时刻、锁的实际释放与恢复均UNKNOWN。其他如rq时钟与clock_update_flags的活动操作没有给前值，不假造其数值。

循环底部在`__schedule_loop`有`need_resched()`重测。B1/B2清P标志且给定无并行变化，正常返回路径没有新的标志来源；B3/B4选择了Q，但**Q在重测时的标志没有作为合同输入提供**，不能断言总共只有一轮，也不能据单次调度函数调用推断P何时恢复运行、是否实际进入另一程序或子任务被唤醒。

## 边界、使用方式与作者状态

机器件记录B1–B4每次读写的拥有者、数值、源位置、前后队列和C计数；DOT用相同ID/有类型端点可重建五个修复诊断簇、A/C调度链、三实参B图和四个样本。evidence新短窗与旧130/131短窗分账，MANIFEST保留G00/P2/P6/P9。R130-02、R131-01、R131-02仅达到`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，原131及130主线RETURN在C09独立复核前保持有效；R127-02/R130-01及其他历史文档闭合不被作者重开。

全过程阶段2。完整本地P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机、真实文件/进程/调度操作均为NOT_RUN；实际用户态musl到内核映射、类的实际链接顺序、CPU任务切换结果、唤醒和公平性均UNKNOWN。没有任何修复源码的授权、修改或补丁，六件仅为静态记录，交C09 Pro独立审查。
