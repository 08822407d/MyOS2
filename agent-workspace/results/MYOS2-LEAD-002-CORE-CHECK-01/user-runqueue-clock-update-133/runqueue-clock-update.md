---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-RUNQUEUE-CLOCK-UPDATE-133
record_type: phase2_executor_source_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
produced_by: "GPT-6 self-reported; actual backend/effort UNATTESTED"
model_per_owner: "GPT-6＋Extra"
date: 2026-11-23
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第133批 · 运行队列纳秒时钟与选任务tick计费

**本批为静态源码分析及合同给定数值的纸面计算，不是测量。** 结论先行：在所读函数里，`__schedule`每轮按顺序左移`clock_update_flags`、直接调用`update_rq_clock`、覆盖标志为4；时钟函数调用一次`local_clock()`并把`u64`时钟差赋给`s64 delta`，负差早返、非负差才通过`rq.clock += delta`。选任务回调`pick_next_task_myos`却单独用`jiffies-last_jiffies`判断切换门和计费；不能把rq.clock的纳秒增量冒充P.vruntime的tick增量。

## G00与裁定继承

执行者沿用原「MYOS2-A-C08c 代码分析执行」和`MYOS2-LEAD-002-CHAT-EXEC-20`。Owner转发C09固定[185合同]及正式发布#7091306717，使本批获得独立授权；C08→C09主线接管依据182/03及#7088796995。Owner指定GPT-6＋Extra，模型自述GPT-6，实际后台子型号/档位不能独立认证，均记UNATTESTED。同一对话此前从Mnemosyne已批准固定来源读取的行为指导和MyOS2项目工作令/协议在适用范围内沿用，不把跨项目指导当执行授权。

直接G00四ref首读为lead`f73f55546245`、exec`1021b4b7740c`、time`a039d9803ade`、直接master`e8e60cd19540`；C09发布提交唯一父f07e60a5e8c0、四件lead新增360行；原PR16 Open/Ready、原题PR17 Open/Draft，base都为master且未合并，开放PR仅2且后页空。PR17顶层评论无since共254条（100/100/54/0），132作者正式#7091027479与C09 132独审/185发布#7091306717经ID核对各一，133本人正式标记初始为0，新133目录写前不存在。132唯一父2089bfff6f4a、只六新增5910行/208467字节，固定/活动六件全文、Git Blob和UTF-8 6/6相同。公约§1第2条由直接master读取，time C旗含`-m64 -mcmodel=kernel -fno-pie -fno-pic`而master含`-m64 -mcmodel=large -fPIE`；time panic用`smp_processor_id()`而master用`raw_smp_processor_id()`。固定源码未使用master替代。

C09已经独立文档关闭R130-02、R131-01、R131-02，130/131原件与相关差量合读为PASS_PENDING_LOCAL。132本体裁定RETURN，但B1–B4的40个终态字段已核收，本批仅追加其三项文档差量R132-01、R132-02、R132-03，不改旧成果。

## D00 · 三项最小追加修正

**R132-01（四案例事件序、短路和重测）**：冻结132每例events中的`switch_count=&prev->nivcsw`列在picker已返回之后，B1/B2旧序号7、B3/B4旧序号13；且旧事件把1116的函数调用发起与收到返回混为一个动作。本次按每条唯一旧事件号重建新顺序：1034先`prev=P`；1069绑定`switch_count=&P.nivcsw`；1078读`prev_state`；1116发起picker调用；进入被调用函数各个实际事件；回到1116完成`next=`返回指针；1118及1119依次清标志；1124比较prev与next；之后按分支计数/发布/调用，最后函数尾。1065–1067的时钟三步实在1034与1069之间，单独在下面A专题展开。修订表为四例分别给旧事件序号、新严格递增序号、选取条件、原十个终态字段，读者可逐行回放。

计数判断`&& count>0`的表达式是否**真的求值**也分账：B1的OR为true，实读count0得false；B3、B4实读count2得true；B2虽然**输入**count=2且数学比较为真，整个OR先为false，右侧`count>0`被短路，**实际没有求值**，不能记录成“读到true”。

对应四个旧`S132_B*_loop`节点也已以相同ID定点更正：B1/B2停留在P，若正常回到`__schedule_loop`尾且没有新置标，P此前在1118被清标，因此重测为假、无需第二轮；B3/B4返回Q只说明选择指针与进入不同任务分支，**不能证明哪个保存的执行上下文在何时来到循环底部**，该时刻执行者的标志与后续次数仍UNKNOWN。此项同时列入修正图与deferred，不能只用“Q的标志未给定”代替真实上下文不确定性。

**R132-02（间接调用发起者）**：132原机器图和DOT里唯一`S132_class_member -> S132_picker [INDIRECT_CALL_INSTANCE]`是错误的调用归属，`A.source_chain`也把`myos_rt_sched_class`写作caller。此次严格以该旧端点/旧类型唯一匹配一次，将表项→目标改为`STATIC_INITIALIZER_TARGET_ONLY`。真正间接调用拥有者是`__pick_next_task`内`p=class->pick_next_task(rq,prev)`（scheduler.h:264），另加由此发向目标的`INDIRECT_CALL_INSTANCE`、rq与prev数据关系和给定myos_rt表项解析关系。返回非空p立即return；若该次返回NULL则类遍历继续，所有类遍历耗尽才到函数末尾BUG位置，不能把一个NULL说成立刻BUG。较早`sys_sched_yield`的yield_task是**另一个成员与调用现场**，旧历史闭合不受影响。

**R132-03（旧图统计范围）**：用冻结130 DOT及131、132 facts/DOT分别按分组重算：130五个ERR诊断簇为40节点/35边；131同五簇为70/80；131五簇再加A_INIT、C_YIELD才是91/99；132经修订的五诊断簇为85/115。此前132的`D00.R130_02.old_file`指五簇，却把`old_graph_baseline=91/99`和`selected_131_baseline=91/99`作为五簇数，归属失配。本次机器件把“同五簇版本演进”和“扩大到七簇的范围”分开，列出固定文件及分簇计数；本批确实读取130固定DOT，132旧执行者未记载的内部读取史保持UNKNOWN，不反推它必定未读。

以上三项均为`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，须由C09用Pro复判后才可改变原RETURN。

## A｜具体哪个调用读取时钟、生产者的两种返回

在固定scheduler_core.c:1032–1078，`__schedule`用当前CPU定位rq与prev后，依次执行以下**三个活动**动作：

1. `rq->clock_update_flags <<= 1`（第1065行）；
2. `update_rq_clock(rq)`（第1066行），被调内用`delta=local_clock()-rq->clock`，负值早返，否则`rq->clock += delta`；
3. 无论被调函数是正常结束还是因delta为负提前返回，调用者第1067行均执行`rq->clock_update_flags=RQCF_UPDATED`。

头文件给出`RQCF_REQ_SKIP=1`、`RQCF_ACT_SKIP=2`、`RQCF_UPDATED=4`；附近注释谈及skip优化，但**当前所读`update_rq_clock`活动函数体内没有以这些标志跳过采样的分支**。队列类型定义将`clock_update_flags`设为`uint`，`clock`、`clock_task`和`clock_pelt`均为`u64`。此处只显式写flags与clock，不写clock_task或clock_pelt；不能据此否定仓库其他函数的更新。

`local_clock()`在固定clock.h定义体直接返回`native_sched_clock()`；其函数声明见asm/timer.h。`native_sched_clock`内`__use_tsc`为true则读`rdtsc()`并调用`cycles_2_ns(tsc_now)`返回纳秒值；为false则返回`(jiffies_64-INITIAL_JIFFIES)*(1000000000/HZ)`，其中`1000000000/HZ`是整数除法，再乘用整型jiffies差。两条只追到返回表达式：实际启动状态、CPU计数器、HZ运行配置和TSC换算结果均未测量，也不从函数名称认证跨CPU单调性。旧105启动偏移和校频研究仅用于范围参考，不重复执行。

类型边界尤其重要：`local_clock()`与`rq->clock`都是`u64`，减法先在无符号64位模域进行，再赋给`s64 delta`。当差的无符号值超过有符号范围时，转成`s64`不属于全体C实现都必须遵守的负数映射；B3/C3**只依185合同明确指定的低64位二补码转换**得到−4。

## B｜连续三次采样和零标志对照

初态：`rq.clock=1000 ns`，`clock_update_flags=REQ_SKIP=1`，`clock_task=700`，`clock_pelt=600`。三次取样依序为1012、1012、1008纳秒；每次后态作为下一次前态且正常返回至调用者。

| 次序 | flags入→左移→覆盖 | local_clock / 减数clock | u64减法与s64 delta | 是否执行 clock+=delta | clock终值 |
|---|---|---|---|---|---:|
| B1 | 1→2→4 | 1012−1000 | 12 → +12 | 是，加12 | 1012 |
| B2 | 4→8→4 | 1012−1012 | 0 → 0 | **是，执行+=0** | 1012 |
| B3 | 4→8→4 | 1008−1012 | 2^64−4 → −4 | 否，负差早返 | 1012 |

三次均确实经过一次`local_clock()`调用，B2虽然最终clock没变，但源码`+=0`分支仍然**到达并执行**；不保证编译后存在一条实际机器存储指令。B3负差`u64=18446744073709551612`经本合同转换后为−4，`delta<0`为真，函数直接return；外层flags覆盖仍发生。三次clock_task均仍700、clock_pelt均600，所选语句对两字段的写点为0。

独立对照B0把初始flags改为0，其他同第一次：flags 0→0→4，`local_clock`仍调用一次、delta仍+12，clock也到1012。这说明**在所读这三个语句和被调函数范围内**，带REQ_SKIP的1并未阻止此次时钟采样；不扩展为所有调用点或全部调度路径均无skip效果。

## C｜相同纳秒与不同tick，或相同tick与负纳秒差

每例独立初态：clock=1000ns，flags=1，P普通且RUNNING、非idle，队列Q,R，P旧vruntime10、R30，time_slice10、last_jiffies100、need=false；开头三个切换计数为rq.nr_switches8、P.nivcsw5、P.nvcsw4，均单CPU且队列结构有效、无并发修改。C先处理上面A的纳秒时钟片段，再由picker独立读取给定`jiffies`。

| 样本 | local ns / tick | delta与rq.clock后态 | picker used_jiffies | next、最终队列 | P.vruntime / last | 三切换计数 |
|---|---|---|---:|---|---|---|
| C1 | 1012 / 103 | +12、1012 | 3 | P；Q,R | 10 / 100 | 8/5/4 |
| C2 | 1012 / 110 | +12、1012 | 10 | Q；P,R | 20 / 110 | 9/6/4 |
| C3 | 996 / 110 | −4、1000 | 10 | Q；P,R | 20 / 110 | 9/6/4 |

三例最终flags都是4。C1的OR选择门为假，右侧count判断实际短路（输入count=2仍是真的），没有摘首/回插/任务计费；rq.clock虽然前进12ns，却不代表任务已到时间片。C2与C3的used_jiffies都等于10，触发`>=time_slice`等号门：摘Q，count由2减到1；P按旧键10插回R30之前，再使count恢复2；之后P键加10到20、last_tick改为110、返回Q。原prev=P，因next=Q，__schedule随后按源码令nr_switches8→9、P.nivcsw5→6，P.nvcsw仍4，并请求context_switch；真实切换结果未观测。

**成对隔离：** C1与C2有同样纳秒采样与rq.clock，却因为tick103对110而决定不同picker结果；C2与C3有同样tick110及任务计费，却因为采样1012对996而令rq.clock分别为1012和1000。C3即便被`update_rq_clock`以负差提前返回，调用者仍能继续到picker并选Q。纳秒时钟写点是`update_rq_clock`及调用者标志；tick计费写点属于`pick_next_task_myos`，不同任务分支的切换计数由`__schedule`控制。真实硬件上两类计时输入可能有关联，不把合同人为给定的数值当成独立物理计时证明。

## 交付边界

这批在旧exec分支新目录追加六件：MANIFEST、正文、可解析facts、同源DOT、deferred与新增证据。D00每例记录原有序号→新事件ID和严格顺序、40个旧终态字段、B2短路与实际执行上下文未知；A/B/C记录来源、u64/s64操作数、标志中间态、所有决定性写入及不写字段。图上通过同一机器ID重放，旧130–132图和评论一律冻结。

状态为作者`READY_FOR_MAINLINE_REVIEW`、主线`PENDING`，三项D00仍须由C09独审。**阶段2所有运行禁令严格保持：完整本地P9、预处理、编译链接、仓库程序与脚本、Graphviz、QEMU、实机、真实时钟/进程/调度/文件操作均NOT_RUN。** 仅按固定文本、给定整数和明确转换规则完成纸面演算，不提供或应用任何内核修复。
