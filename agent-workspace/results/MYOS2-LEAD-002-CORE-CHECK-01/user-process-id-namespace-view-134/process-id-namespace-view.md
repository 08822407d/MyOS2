---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-PROCESS-ID-NAMESPACE-VIEW-134
record_type: phase2_process_id_namespace_source_report
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
# 第134批 · getpid/getppid的命名空间公开编号

**静态结论：同一task的线程编号、线程组编号以及不同观察者命名空间中的可见编号，是三个不同维度。** 在186合同的有效普通对象上，getpid读取当前P的线程组对象G而得20；getppid读取P.real_parent指向的父任务线程组对象GP而得7；作为内核helper的task_pid_vnr(P)选线程对象T而得22。C3与C4都能得到零，前者不满足命名空间层级，后者通过层级判断却不满足槽内命名空间指针相等。均为给定对象演算，绝非真实进程或真实系统调用测量。

## G00 固定输入、身份及限制

原执行身份「MYOS2-A-C08c 代码分析执行」/MYOS2-LEAD-002-CHAT-EXEC-20，Owner本次以186合同、C09正式回执#7093714156启动。Owner选GPT-6＋Extra，模型自述GPT-6，后端实际型号/effort为UNATTESTED；读取通道GitHub connector。同一原会话既有MyOS2约束/公约及先前固定来源的Mnemosyne行为指导按授权范围沿用，但C09原主线审查独立性不能转嫁给作者。

写前四直接refs：lead 3251b8f68697、exec 5e979654dc92、time a039d9803ade、直接master e8e60cd19540。C09正式发布提交3251b8f68697的唯一父为f73f55546245，四件新增359行。开放PR仅16、17（下一页为空）；PR16 Open/Ready，原题PR17 Open/Draft，均base master、未合并。PR17无since顶层评论100/100/56/0共256；133作者#7093437139和C09 133主审/186发布#7093714156依ID核对且均唯一，本134正式作者标记0。新134目录写前不存在；C08→C09接管签认#7088796995已核。

133执行提交5e979654dc92唯一父1021b4b7740c，compare仅6件纯新增5181行、177567字节。133六件从固定与活动原执行分支各取全文，按Git Blob头及UTF-8独立重算哈希，6/6一致；evidence的正确短十二位为a536ca7e8ea1。公约§1第二条已自直接master读取，固定time的两个金丝雀分别为`-m64 -mcmodel=kernel -fno-pie -fno-pic`、`this_cpu=smp_processor_id()`；直接master相应为`-m64 -mcmodel=large -fPIE`、`raw_smp_processor_id()`。固定源码本批只取time，不拿master冒充。

C09已关闭R132-01、R132-02、R132-03（文档关闭，132与修正合读PASS_PENDING_LOCAL）；133独审原裁定RETURN，本批仅追加三项开放差量R133-01/02/03。不得作者自签关闭旧RETURN。

## D00 · 只追加三项原件勘误

### R133-01｜冻结旧图选择器、虚拟模型、实际新图的ID不相同

第133批报告第35行称四组loop以“相同ID”改好，事实是旧132图的`S132_*`只在132自身出现，而133两载体的实际模型改用了`D133_*`。严格核四个ID各自在对应固定facts、DOT中命中一次，133图里旧ID均命中零次：

| 案例 | 132旧ID（facts行／DOT行） | 133实际ID（facts行／DOT行） | 两侧各自命中 |
|---|---|---|---|
| B1 | S132_B1_loop（2595／404） | D133_B1_loop（2253／19） | 各1/1；旧名在133为0 |
| B2 | S132_B2_loop（2650／428） | D133_B2_loop（2338／54） | 各1/1；旧名在133为0 |
| B3 | S132_B3_loop（2705／452） | D133_B3_loop（2468／98） | 各1/1；旧名在133为0 |
| B4 | S132_B4_loop（2760／476） | D133_B4_loop（2598／151） | 各1/1；旧名在133为0 |

B1/B2若控制正常回到P的循环底且无新置标，可以有条件地认为不需下一轮；B3/B4只是选中Q，尚不知哪个保存上下文何时恢复、恢复时哪个need_resched标志被读。新机器件同时包含旧选择器→新实际ID的**说明性映射边**，不把这个映射冒充运行时的调用边。

旧132的一条`S132_class_member → S132_picker`，类型`INDIRECT_CALL_INSTANCE`，在旧两载体均唯一。133 D00.virtual模型把同名旧端点临时改写为`STATIC_INITIALIZER_TARGET_ONLY`，但**实际133图**既没有这对旧ID，也不是一个节点承载所有关系。其可执行含义拆解为：

| 133真实节点/边 | 类型 | 源码所有者 |
|---|---|---|
| D133_CLASS_member → D133_CLASS_callee | STATIC_INITIALIZER_TARGET_ONLY | myos_rt.c:67，静态表项把函数指针指向pick_next_task_myos |
| D133_CLASS_owner → D133_CLASS_call | INDIRECT_CALL_INSTANCE | scheduler.h:264，__pick_next_task通过成员指针调用 |
| D133_CLASS_call → D133_CLASS_callee | TARGET_RESOLUTION_IF_MYOS_RT | 给定遍历至myos_rt时目标解析 |
| D133_CLASS_member → D133_CLASS_call | FUNCTION_POINTER_VALUE | 表成员值不是主动的调用者 |
| D133_CLASS_argument → D133_CLASS_call | RQ_AND_PREV_DATA | rq、prev来自调用现场 |

上述实际五条边在133固定facts及DOT均各命中1。空p继续类遍历、非空p立即返回；全部类遍历耗尽才进入末尾BUG位置，未由本文推断任何实机BUG必达。

### R133-02｜判断节点不能自身又是早返又能直通 +=

冻结133的D133_A_negative标签写“delta<0 early return”，却仍有`D133_A_negative → D133_A_add [CONDITION_NONNEGATIVE]`，因而视觉上把已早返的节点连到加赋值，违背固定scheduler_core.c:24–27。本批以一个**中立判断节点**`D134_CLOCK_GUARD`替代该复合语义，分开两条路：条件真→`D134_CLOCK_RETURN`（跳过`rq.clock += delta`）；条件假→`D134_CLOCK_ADD`（执行该加赋值）。两条路径回到`__schedule`的`D134_CLOCK_OVERWRITE`，依第1067行把flags覆盖为4。实际DOT和机器件以新的有类型边表示真假、返回和汇合，旧错误边只作冻结选择器。

C.counters的旧冗余`first_clock_write`不能把两个函数混合成每例均实际写clock。三个样本逐一还原：

| 样本 | __schedule:1065 写flags（左移） | update_rq_clock:27 写clock | __schedule:1067 写flags（覆盖） | clock终值 |
|---|---|---|---|---:|
| C1 | 是 | 是，delta+12 | 是 | 1012 |
| C2 | 是 | 是，delta+12 | 是 | 1012 |
| C3 | 是 | **否，delta−4早返** | 是 | 1000 |

上表区分三个不同字段写点及拥有者，与133固定`clock.control`、`source_write_owners`和三例数值逐项一致。局部时钟函数的返回**不会中断**调用者继续做flags覆盖、后续picker选择；实机时钟、编译后机器写指令等并未运行。

### R133-03｜三个单行宏定义，前后账各有归属

冻结133 E133-015用`RQCF_REQ_SKIP`一个符号标记了scheduler_const.h:46–48三行，其实每行是一个独立定义。第134批新增证据`E134-001`核第46行`RQCF_REQ_SKIP=0x01`，`E134-002`核47行`RQCF_ACT_SKIP=0x02`，`E134-003`核48行`RQCF_UPDATED=0x04`；机器件、正文及图中的宏实体逐一交叉对应。

计数对象分开：133冻结六件原样正式35/裸35、降级0、另7个定位；若仅构造旧件“替换E133-015的1条为三条”的完整虚拟修订视图，则正式及裸均为`35−1＋3=37`，不是旧件实测，也不能据此改写133原六件；134这三个窗分别计入**134自身**新的正式引文账。

以上R133-01、R133-02、R133-03暂均为`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，待C09独立判定。

## A · init中的两个读取点、x86-64编号与两个系统调用体

固定initd.c的`normal_boot`第30行调用一次fork并保存rv。rv为0时走子分支：第33行`pid_t pid = getpid()`，局部pid在所读段内不再消费，紧接着第35行请求`execve(prog_name,args,envs)`；第34行打印仅为注释。rv为正或−1都进入else：第38行再次有**另一条分支的**`pid_t pid=getpid()`，局部pid也未被活动语句使用；第39行打印为注释，随后第40–42行无限循环请求`sched_yield()`。两处getpid各自属于被选中的分支，并非同一个运行进程必然先后都执行。该initd.c全文没有getppid调用，其在本批是对照系统调用而非init真实调用点。fork、exec和系统调用桥接的真实执行结果均UNKNOWN。

固定x86-64编号和登记位置：`__NR_getpid=39`（unistd_64.h:50）→`__SYSCALL(__NR_getpid,sys_getpid)`（syscalls_64.h:29）；`__NR_getppid=110`（unistd_64.h:135）→`__SYSCALL(__NR_getppid,sys_getppid)`（syscalls_64.h:57）。同一表头在syscall_64.c其实有**三个**源码纳入语境，不能少记第一个声明阶段：第10–11行将`__SYSCALL`定义为生成extern原型；第19–22行定义为`[nr]=__x64_##sym`，初始化`sys_call_table[]`，这是表项/地址数据而非调用；第26–30行定义为`case nr:return __x64_##sym(regs)`，在`x64_sys_call`的switch中形成按给定编号有条件调用的源级分支。第31行还有不命中编号时的默认入口。实际预处理、链接与用户库是否进入此分支未检验；文件注释说该表不再用于直接系统调用，不能把静态初始化器误作运行时调用点。

内核实现`MYOS_SYSCALL_DEFINE0(getpid)`在固定syscall.c:45–48，活动体只`return task_tgid_vnr(current)`。与名称容易混淆的是此处不是`task_pid_vnr`：选择的是**线程组TGID**。另一个`MYOS_SYSCALL_DEFINE0(getppid)`在syscall.c:115–124，先以`current->real_parent`作目标执行`task_tgid_vnr`，赋本地pid再返回；RCU读锁/解锁语句在第119及121行均被注释，不得据此认证并发读锁生效。

## B · 槽位、目标与观察者严格分离

PIDTYPE枚举中`PIDTYPE_PID=0`、`PIDTYPE_TGID=1`，另有PGID/SID但本次不查询。固定`task_tgid_vnr(task)`是`__task_pid_nr_ns(task,PIDTYPE_TGID,NULL)`，固定`task_pid_vnr(task)`是`__task_pid_nr_ns(task,PIDTYPE_PID,NULL)`；它们不仅名字不同，**type决定了两个不同的PID对象指针槽**：`task_pid_ptr`遇PIDTYPE_PID返回`&task->thread_pid`，其他类型返回`&task->signal->pids[type]`，所以本次TGID取`signal->pids[PIDTYPE_TGID]`。

`__task_pid_nr_ns`先令nr=0；若显式ns不为空，则原样使用该指针；若为空，调用`task_active_pid_ns(current)`，其活动宏为`ns_of_pid(task_pid(current))`。`task_pid(current)`先取`current->thread_pid`，`ns_of_pid`再取该pid_s的`numbers[pid->level].ns`。在本题P的thread_pid为T、T.level=1、T.numbers[1].ns=N1，故缺省**观察者**为N1。即使目标换成`current->real_parent`，这段缺省ns仍从**current=P**推得N1，而不从父任务重新选观察者。

`__task_pid_nr_ns`借活动的`rcu_dereference(*task_pid_ptr(task,type))`获取目标PID对象，转给`pid_nr_ns(pid,ns)`；后者也是先初始化`pid_t nr=0`。执行`if(pid && ns->level<=pid->level)`，只有成功才实际求`&pid->numbers[ns->level]`，随后`if(upid->ns==ns)`为真才读`upid->nr`并覆盖nr，最后返回nr。一个零值既可能来自level门失败，也可能来自索引已取但指针不相等；数值相同的层级并不构成命名空间对象同一性。

所读`upid_s`包含`nr`和`pid_ns_s* ns`；`pid_s`含`level`、柔性数组`numbers[]`及引用/任务链；`pid_namespace`含`level`、`parent`与IDR；P的task结构含`thread_pid`、`real_parent`和`signal`，signal内有`pids[PIDTYPE_MAX]`。本报告按指定的有效对象、足够长的numbers数组和关系稳定作推演，不验证真实PID分配、线程组生命周期或命名空间创建可行性。pid.c中的`rcu_dereference`是活动语句，但其相邻`rcu_read_lock/unlock`均为注释；getppid调用体的两处读锁也为注释。不能据此断言并发与对象释放时序安全。

## C · 四组有效输入、十个独立查询

N0为level0；N1、M1均为level1，父命名空间都指向N0，但**N1与M1不是同一指针**。current始终P，P.thread_pid=T，P.signal的TGID对象是G，默认父任务的TGID对象是GP，C3替换后的父任务TGID是GP0。

| pid_s对象 | level | numbers[0] | numbers[1] |
|---|---:|---|---|
| T（P的线程PID） | 1 | (N0, 5102) | (N1, 22) |
| G（P的线程组TGID） | 1 | (N0, 5100) | (N1, 20) |
| GP（默认父任务TGID） | 1 | (N0, 4100) | (N1, 7) |
| GP0（C3替代父TGID） | 0 | (N0, 4100) | 不存在；不读取 |

以下每行在机器件中有唯一ID，列出了真实被选槽、观察者来源、层级门、索引与相等门。符号“未评估”表示该表达式因为前一个条件短路而**确实未执行**，不只是结果未知。

| ID（组） | 入口/目标与type | 观察者ns | level门 | 实际索引 | upid.ns==ns | 读nr? | 返回 |
|---|---|---|---|---|---|---|---:|
| C1-GETPID | getpid，P/G，TGID | 缺省N1 | 1≤1 真 | G[1] | N1=N1真 | 是 | **20** |
| C1-GETPPID | getppid，父/GP，TGID | 缺省N1 | 1≤1 真 | GP[1] | N1=N1真 | 是 | **7** |
| C1-PIDVNR | task_pid_vnr(P)，P/T，PID | 缺省N1 | 1≤1 真 | T[1] | N1=N1真 | 是 | **22** |
| C2-N0 | 显式TGID，P/G | N0 | 0≤1 真 | G[0] | N0=N0真 | 是 | **5100** |
| C2-N1 | 显式TGID，P/G | N1 | 1≤1 真 | G[1] | N1=N1真 | 是 | **20** |
| C3-GETPPID | 替代父/GP0，TGID | 缺省N1 | **1≤0 假** | **不读取** | 未评估 | 否 | **0** |
| C3-N0 | 显式TGID，替代父/GP0 | N0 | 0≤0 真 | GP0[0] | N0=N0真 | 是 | **4100** |
| C3-GETPID | getpid，P/G，TGID | 缺省N1 | 1≤1 真 | G[1] | N1=N1真 | 是 | **20** |
| C4-M1 | 显式TGID，P/G | M1 | 1≤1 真 | G[1] | **N1≠M1假** | 否 | **0** |
| C4-N1 | 显式TGID，P/G | N1 | 1≤1 真 | G[1] | N1=N1真 | 是 | **20** |

**C1**说明在同一个观察者N1下，getpid取G的20，getppid取GP的7，而task_pid_vnr取T的22，后者仅为内核helper对照，并非init新增调用。**C2**不换目标G，只把显式ns由N1换成N0，于是同一个PID对象对不同观察者分别给20和5100。**C3**仅换real_parent指向的父任务：父的GP0.level=0、小于观察者N1.level=1，故getppid取数组前就停在level门、保留初始nr=0；同一个GP0从显式N0查则为4100，getpid仍取G返回20。**C4**命名空间M1与N1同为level1，但实际G.numbers[1].ns=N1，M1不等于它，虽然数组已经读取，`upid->nr`分支却跳过而返回0；显式N1查询仍是20。C3和C4的零不是同一失败原因，也不可据此推断哪个进程实际存在或不受谁可见。

## 机器件、证据账及运行边界

facts使用稳定ID `PID134_C1_GETPID` 等十条查询，各包含入口、目标task、type、槽、pid_s、显式或current缺省ns、level门是否通过、索引是否执行、指针比较是否执行/结果、nr读取布尔及返回。DOT以相同稳定ID显示推导步骤，另把D00旧→新说明映射、三条时钟控制边、三个宏各自证据窗、三个宏语境、PID结构对象和观察者关系作带类型投影。两种机器载体可分别核到节点和边，未运行Graphviz。

旧133证据仍冻结35正式、7定位，虚拟替代为37；本134新增证据和定位自立P6账。原133/132制品不覆盖，文档闭合须由C09独审决定。真实用户态musl到内核桥接、PID命名空间分配与创建、RCU并发保障、进程退出/回收、实际返回值、链接/编译及运行均UNKNOWN。**阶段2完整本地P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机、真实时钟/进程/调度/文件操作全部NOT_RUN。** 本执行不修改一行内核代码，不提供运行试验或修复补丁。
