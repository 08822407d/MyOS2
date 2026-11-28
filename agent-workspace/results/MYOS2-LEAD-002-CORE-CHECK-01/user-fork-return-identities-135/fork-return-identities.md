---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FORK-RETURN-IDENTITIES-135
record_type: phase2_fork_identity_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: "MYOS2-A-C08c 代码分析执行"
produced_by: "GPT-6 self-reported; backend model/effort UNATTESTED"
model_per_owner: "GPT-6＋Extra"
date: 2026-11-23
phase: 2
base_snapshot: "time（分支名）"
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第135批 · 一次普通fork的父子返回值、公开编号与寄存器帧

**结论先行：父方kernel_clone以子任务的PIDTYPE_PID对象在父方当前命名空间计算数字nr；子方copy_thread把新子任务的寄存器保存帧ax写成0。这是两个不同所有者的不同数据来源。** 给定C2的N1观察者，K的全局标量pid/tgid均为5200，父方kernel_clone却返回30，子方保存帧ax为0；在真实用户态返回工作有效的附加条件下，子方之后getpid为30、getppid为20，父方自己的getpid为20。源码事实与条件观察必须分账，未运行任何内核代码。

## G00 · 作者席位、冻结源与写入权限

原「MYOS2-A-C08c 代码分析执行」／CHAT-EXEC-20身份，Owner转发[187号合同]与C09正式发布PR17评论#7094597734。执行档Owner指定GPT-6＋Extra，模型自述GPT-6，后台实际子模型及推理强度无法认证，均UNATTESTED。C08→C09旧主线写权交接固定182/03及#7088796995已按来源核，C09保有lead/PR16和PR17主线验收/导航写权；本会话只执行作者本批。原C08c先前已读取并接受的MyOS2及Mnemosyne行为规范在可追溯范围内继续适用，本轮新读文件另列，不将C09其他对话的内部动作冒充本作者观察。

G00首启四条直接refs分别lead `676e4169f14c`、exec `f0d46c448367`、time `a039d9803ade`、直接master `e8e60cd19540`。C09本次lead固定提交唯一父`3251b8f68697`、仅四文档纯新增355行；C09最新裁定：134原件`RETURN`、十查询200个给定字段均核收；133的R133-02、R133-03已文档关闭，R133-01尚有一处虚拟间接调用旧源映射；132原件与差量合读PASS_PENDING_LOCAL。只有本187授权的R133-01余项及R134-01/02/03/04可在新135目录追加，不回写133/134。

原PR16 Open/Ready、原题PR17 Open/Draft、均base master未合并；开放PR仅两条，分页翻空。PR17全部顶层无since页100/100/58/0共258，本135作者正式标记写前0，134作者#7094240400及C09 134独审/187发布#7094597734各唯一并按ID核正文。135新目录写前404。134作者固定`f0d46c448367`单父`5e979654dc92`，compare仅六个新增4480行、170040字节；固定/原活动exec六件各自全文、独立Git Blob SHA-1及UTF-8均6/6一致，134证据Blob前12为`66dfb0a00a20`。

直接master公约§1第2条在MANIFEST精确列出。time内核CMake金丝雀为`-m64 -mcmodel=kernel -fno-pie -fno-pic`，master相应为`-m64 -mcmodel=large -fPIE`；time panic.c为`this_cpu=smp_processor_id()`而master为`raw_smp_processor_id()`。新引文只用固定time的活动源码定义，不把master当内核分析基线。

## D00 · 五项有限历史补正

### R133-01仅余的虚拟间接调用

133固定facts中`D00.R132_02.correct.indirect_call`旧虚拟三元组为`S132_class_iter → S132_picker / INDIRECT_CALL_INSTANCE`，owner记`__pick_next_task`、行scheduler.h:264。此元组在旧D00模型作为一条**描述性差量**，不是真实133 DOT里的一条边；134整包未出现该旧源。135把它显式映射到133真实两载体中均各唯一的两步：

| 133实际边 | 类型 | 所有者/语义 |
|---|---|---|
| D133_CLASS_owner → D133_CLASS_call | INDIRECT_CALL_INSTANCE | `__pick_next_task`在类遍历内以rq/prev发起函数指针调用 |
| D133_CLASS_call → D133_CLASS_callee | TARGET_RESOLUTION_IF_MYOS_RT | 给定遍历到myos_rt类，目标为pick_next_task_myos |

前次已核的四个旧loop选择器、静态成员目标、函数指针值和rq/prev数据边只作既定依据，不重新开项。新增机器D00.R133_01保留旧虚拟来源、实际两边和两载体命中数；DOT以单独映射节点说明语义，不伪造旧边在新实际图里的出现。

### R134-01时钟图四节点、四边的虚拟/真实对应

134机器`D00.R133_02.replacement_graph`声明了四个虚拟全大写ID，但134实际图是另外四个ID。每对实际134 facts/DOT中各命中1；对应虚拟拼写在实际图中都是0：

| 134虚拟ID | 134实际ID |
|---|---|
| D134_CLOCK_GUARD | PID134_CLOCK_guard |
| D134_CLOCK_RETURN | PID134_CLOCK_early |
| D134_CLOCK_ADD | PID134_CLOCK_add |
| D134_CLOCK_OVERWRITE | PID134_CLOCK_caller |

四条虚拟边按该表替换端点后：guard→early为`CONDITION_NEGATIVE_TRUE`，guard→add为`CONDITION_NONNEGATIVE_FALSE`；early→caller真实类型为`RETURN_TO_CALLER_THEN_FLAGS_OVERWRITE`（不是旧`RETURNS_TO_CALLER_THEN_FLAGS_WRITE`），add→caller真实类型为`NORMAL_RETURN_THEN_FLAGS_OVERWRITE`（不是旧`NORMAL_RETURN_THEN_FLAGS_WRITE`）。各实边在134固定facts与DOT均独立命中1，报告与清单的旧拼写仅是虚拟视图。此前负差早返和三例flags/clock写点已由C09核收，不在这项中重改源码结论。

### R134-02十条查询的目标/观察者来源、求值顺序与跨载体ID

冻结134的十条`PID134_*_target → PID134_*_observer [EVALUATION_ORDER]`各在facts/DOT命中1，但该类型会误导读者以为总是先执行目标PID槽读取，再计算默认ns。真实代码的**语句顺序**是：调用者传入目标task、PID type和可为空的ns实参；`__task_pid_nr_ns`在pid.c:473先实际求值`if(!ns)`；仅真分支在474决定缺省`task_active_pid_ns(current)`；随后第475行才进入目标`task_pid_ptr(task,type)`和`rcu_dereference`，并把PID对象与ns传给`pid_nr_ns`。本次不强加C语言对同一调用中不同实参表达式的求值顺序。

五条缺省ns的调用均为**current=P → task_pid(P)直接返回P.thread_pid=T → ns_of_pid(T)读取T.numbers[T.level=1].ns=N1**。不是“task_pid(P)先返回P再对其thread_pid求一次”，也不是“task_pid(P)->thread_pid=T”（该写法混淆对象）。其余五条显式ns只传N0、N1或M1，`if(!ns)`仍实际求值但为假，默认链根本未调用；显式ns作为数据传给下层。即使目标为real_parent，默认观察者仍为current=P。下层先测试对象非空与ns.level≤pid.level，再决定是否索引numbers、比较指针，只有相等才读nr；C3-GETPPID_PARENT0因层级门失败返回零，C4-M1_G因ns指针不等返回零，短路点不同。

下表的每个`Q135_*`是**135实际机器和DOT中的根节点**；左列为134真实完整查询主键，括号为134正文简写。134原每条的20个已核字段均原样保留，共200/200：

| 134完整ID（正文简称） | 135新图根ID | 返回 |
|---|---|---:|
| PID134_C1_GETPID（C1-GETPID） | Q135_C1_GETPID_entry | 20 |
| PID134_C1_GETPPID（C1-GETPPID） | Q135_C1_GETPPID_entry | 7 |
| PID134_C1_PIDVNR（C1-PIDVNR） | Q135_C1_PIDVNR_entry | 22 |
| PID134_C2_N0_TGID（C2-N0） | Q135_C2_N0_TGID_entry | 5100 |
| PID134_C2_N1_TGID（C2-N1） | Q135_C2_N1_TGID_entry | 20 |
| PID134_C3_GETPPID_PARENT0（C3-GETPPID） | Q135_C3_GETPPID_PARENT0_entry | 0 |
| PID134_C3_N0_GP0（C3-N0） | Q135_C3_N0_GP0_entry | 4100 |
| PID134_C3_GETPID（C3-GETPID） | Q135_C3_GETPID_entry | 20 |
| PID134_C4_M1_G（C4-M1） | Q135_C4_M1_G_entry | 0 |
| PID134_C4_N1_G（C4-N1） | Q135_C4_N1_G_entry | 20 |

每个135查询子图把entry、目标实参、type、ns实参、`if(!ns)`、缺省/显式分支、resolved ns、slot、pid_s、level、条件index/pointer/nr以及返回单独建节点和关系；真实字段访问通过条件边表示，旧错误`EVALUATION_ORDER`不再充当运行顺序。facts存旧ID→根/阶段ID、老边选择器、实际新边、完整200字段及每个零的理由；DOT与之同源。

### R134-03两层x86-64系统调用包装

134固定图的`PID134_A_switch → PID134_A_getpid_body / CONDITIONAL_DISPATCH_39`和`...getppid_body / CONDITIONAL_DISPATCH_110`各唯一，但那是**合成可达性**而非switch直接调用最终查询体。受固定`CONFIG_ARCH_HAS_SYSCALL_WRAPPER`条件及相关宏定义的源码**条件**约束，正确的分层为：

| 源级阶段 | getpid | getppid |
|---|---|---|
| switch按编号的直接调用 | `__x64_sys_getpid(regs)`，nr39 | `__x64_sys_getppid(regs)`，nr110 |
| x64存根的直接转发 | `__se_sys_getpid(regs)` | `__se_sys_getppid(regs)` |
| __se的直接转发 | `__do_sys_getpid()` | `__do_sys_getppid()` |
| 真正查询体 | `task_tgid_vnr(current)` | `task_tgid_vnr(current->real_parent)` |

`MYOS_SYSCALL_DEFINE0`通过`__MYOS_SYSCALL_DEFINEx(0,_name)`生成命名层，`__SYS_STUBx(x64,sys_name)`的体调用__se，__se的体用`__ARCH_MAP(0,...)`传零个内核参数给__do。最后一行是__do函数**定义体**，不是又一次函数调用。固定`syscall_64.c`还在第11行以登记头生成extern原型，第21行生成静态sys_call_table数组，第30行生成switch案例：前两者均不能冒充实际直接调用。实际预处理、符号链接及用户态libc到此桥接未知，禁止宣称已运行。

### R134-04纠正E134-021并给47条窄断言

旧E134-021在一个正式窗内包含pid.h:124签名与125行返回，本次以新证据E135-001的**pid.h:125一行**`return task->thread_pid;`作为一对一虚拟替代；旧134文件不改。134的E134-001至047，每个在facts.D00.R134_04中分别配有源文件、原符号、1–5行窗及**独立具体窄断言**，不重新抄47个旧源码窗。例如E134-012只证明`int pid`局部声明、E134-026只证明按type选PID对象槽、E134-031单独证明先处理缺省ns后在下一语句解析slot、E134-045只证明`task_pid_nr`返回标量而非公开编号。没有把签名、注释、静态表或宏登记冒充实际函数调用。

冻结134仍为47裸/47正式/0降级、另16定位；仅旧件的**一窗换一窗虚拟完整修订视图**也为47。135自己新增正式短窗另立P6，不与旧134计数混算。

所有五项D00均暂列`CORRECTION_SUBMITTED_PENDING_MAINLINE_REVIEW`，由C09 Pro独立判闭。R133-02/03历史关闭及132组合PASS_PENDING_LOCAL不重开；133和134原主线RETURN仍保持。

## A · 普通fork从用户调用到父方可见编号

固定initd.c:30仅发起一次`fork()`并保存rv；rv=0子分支调用getpid并保存未使用的局部pid，随后请求execve；rv非零（包括正值和给定-1）else分支调用另一次getpid、保存未使用的局部，然后进入无限sched_yield循环。两条打印语句都是注释。不能推断同一执行上下文连续经过两支，也不能把getpid局部值充当fork分支依据（分支只检查rv）。

固定`MYOS_SYSCALL_DEFINE0(fork)`在sched/syscall.c:51–65建立`kclone_args_s args={.exit_signal=SIGCHLD}`并直接`return kernel_clone(&args)`；该初始化列在普通C聚合规则下令其余未明示成员为零，故本样本flags=0、fn=NULL、kthread=0、stack=0，而非自动附加CLONE_THREAD。函数前面的嵌套计数循环为活动源码，但不产生fork编号。真正将用户侧fork请求送达此内核体的libc/ABI桥接仍UNKNOWN。

正常`kernel_clone`在fork.c:1485调用`copy_process(NULL,...,args)`；错误指针先在1488用IS_ERR拦截，成功才继续。copy_process开始处先执行`retval=-ENOMEM`、`dup_task_struct(current,node)`；给定成功后清PF_KTHREAD，普通kthread=0不会重新设置。copy_thread在fork.c:1143被调用，之后才有alloc_pid；在给定PID结构有效时，第1206行`p->pid=pid_nr(pid_struct)`读取**numbers[0]全局标量**，无CLONE_THREAD的第1212行`p->tgid=p->pid`。没有CLONE_PARENT/THREAD，第1275行`p->real_parent=current=P`。

这里有一个必须保持的独立条件：`thread_group_leader(p)`返回`p->exit_signal>=0`，并**不直接判断args.exit_signal**。copy_process中`p->exit_signal=args->exit_signal`是注释，所以SIGCHLD入参不能证明新p字段的值。C1/C2已经明确给定“非零PID且组长谓词真”。在这两个前提下，第1314行`init_task_pid(p,PIDTYPE_PID,pid_struct)`使`K.thread_pid=KPID`；第1316行`init_task_pid(p,PIDTYPE_TGID,pid_struct)`使`K.signal.pids[TGID]=同一个KPID`。成功之后copy_process于1372返回任务指针K，尚非公开整数返回。

kernel_clone先`get_task_pid(K,PIDTYPE_PID)`，它通过`task_pid_ptr(K,PIDTYPE_PID)`得到K.thread_pid槽，并经`get_pid`增加临时引用；之后`nr=pid_vnr(KPID)`以**当前父任务P的活动命名空间**读取子PID对象的可见编号。这个`nr`是独立局部整数，后续源码请求`wake_up_new_task(K)`，再`put_pid(KPID)`减少临时引用，最后`return nr`。给定KPID原count2且片段中无其它变化，轨迹为2→3→2，不触发减至零的释放分支。唤醒调用在nr已得到之后，不能推出子进程真的抢先运行或已经执行getpid。父方fork取的是**子线程PID的可见编号**，与父方自己的getpid读取**父线程组TGID**是两个来源。

## B · 子任务的ax=0与公开进程编号无关

fork.c:1143成功调用`copy_thread(K,args)`。固定process_arch.c:92使用`task_pt_regs(K)`定位**子任务K**的寄存器保存区，随后合成fork_frame、初始返回地址ret_from_fork_asm和K.thread.sp。给定PF_KTHREAD=false、fn=NULL、stack=0：跳过内核线程清帧分支，先执行`*childregs=*current_pt_regs()`将P当前保存帧拷给K。题设P拷贝前ax=57，则K子帧在拷完后暂有ax=57；紧接第135行明确执行`childregs->ax=0`，把K子帧ax改为0。sp=0使可选sp覆盖分支跳过，fn=NULL使另一条用户函数栈/IP清零路径跳过，函数正常返回源级ret=0。**该寄存器零不影响K.pid、K.tgid或任何pid_s号码。**

`ret_from_fork`在process_arch.c:69–77还存在另一处`regs->ax=0`，但它位于`if(unlikely(fn))`的非空fn分支内，本普通fn=NULL的样本不执行该写点。旧67批首帧研究的上下文切换、汇编返回与SYSRET条件不重新推演；这里仅区分两个源码拥有者。只有在内核新任务被正确调度、返回工作、ABI和库桥接全部如预期时，用户侧子fork才会观测零，真实先后与CPU执行均未知。

## C · 两个独立成功前态和一个指定失败出口

所有样本都以合法对象和源级正常调用作为**假设输入**，不是实机测试。父P拥有分开的线程PID对象与线程组TGID对象，K的PID/TGID槽在给定组长前提下共用KPID。P与K在每个成功样本都处于相同活动ns，父帧复制前ax=57，get_task_pid前KPID临时引用count2。对C1/C2独立推演，不把一次fork后的状态带入下一例。

| 字段 | C1：N0 | C2：N1 |
|---|---:|---:|
| P线程PID对象 numbers | N0:5102 | N0:5102，N1:22 |
| P组TGID对象 numbers | N0:5100 | N0:5100，N1:20 |
| K共用PID/TGID对象 numbers | N0:5200 | N0:5200，N1:30 |
| K.pid、K.tgid标量 | 5200、5200 | 5200、5200 |
| K.thread_pid、K.signal.pids[TGID] | 同一KPID | 同一KPID |
| K.real_parent | P | P |
| P调用kernel_clone最终nr | **5200** | **30** |
| K子寄存器帧复制前后ax | P源57 → K复制57 → K置0 | P源57 → K复制57 → K置0 |
| KPID临时refcount | 2→3→2 | 2→3→2 |
| K后续getpid（假设可执行） | 5200 | 30 |
| K后续getppid（假设可执行） | 5100 | 20 |
| P后续getpid（假设可执行） | 5100 | 20 |
| 显式N0查K的TGID helper | 同为5200 | **5200** |

**C1父方**通过pid_vnr在N0读取KPID[0]=5200，子K若进入用户态，getpid从K.signal.pids[TGID]=KPID按K当前N0取5200；getppid用real_parent=P的组PID对象按**观察者K的N0**取5100；父P自己的getpid也按P当前N0从P组对象取5100。**C2父方**在N1读取KPID[1]=30，即使K.pid和K.tgid标量都是5200；子K的getpid在N1读TGID的同一KPID[1]=30，getppid从P的组对象N1槽读20，父自己的getpid同样20。单独用显式N0查询K的TGID仍得5200。父方返回30、子方fork保存帧ax=0、子方getpid得30，是三个由不同位置决定的数据，不可混称“fork返回子PID，所以子也返回子PID”。

两次成功均在kernel_clone:1497取得一次引用，1498计算父方公开nr，1516发起唤醒调用，1527归还引用，1529返回此前的nr。给定原引用2，新增临时引用3，再归还至2，不表示子对象在该点被释放。两例均在copy_thread早于PID分配时就已把K子帧ax置0；`wake_up_new_task`是否让K真的开始执行未观测，父子真实先后不排序。

**C3指定提前返回**：P继续沿用C2的N1可见对象，但是`dup_task_struct(current,node)`在fork.c:959按题设返回NULL。copy_process之前已经设`retval=-ENOMEM`，第960–961行的`if(!p)`立即转到`fork_out`；该标签在第1428–1432行执行所示spin_lock_irq与spin_unlock_irq源码调用，再`return ERR_PTR(retval)`，因此返回`ERR_PTR(-ENOMEM)`。kernel_clone在第1488–1489行检测IS_ERR为真并直接`return PTR_ERR(p)`，返回符号值`-ENOMEM`，sys_fork再传播该内核返回。

这个指定失败分支根本没有到达K的copy_thread、childregs的ax复制或置0、PID对象分配、K.pid/tgid赋值、get_task_pid、pid_vnr、wake_up_new_task、put_pid或成功的return nr；**不存在本次成功创建并走子分支的K**。用户态fork是否把内核错误转换成`-1`及errno何值，不由所读源码认证。如果另外单独给定“用户已看到rv=-1”，那么init的`rv!=0` else条件成立，当前仍为P，在N1下该分支getpid的**给定公开结果**是20，然后循环请求sched_yield；这是额外条件推论，不能倒写成已证的实际错误处理。

## 证据、机器件与运行边界

新facts采用稳定跨载体ID，D00含五项冻结选择器、134十条旧查询到新查询子图的主键、200字段逐键保持及47条旧正式窗的独立窄断言；A含普通fork调用和返回的源级所有者，B区分两处ax=0，C1/C2各有19步明示事件及条件查询，C3有6步错误链和未到达清单。DOT同源投影，分离调用、数据、条件、真正直接调用及虚拟旧映射；仅文本结构核对，Graphviz未运行。新证据第一个正式窗E135-001为pid.h:125，全部新短窗各列一条来源符号和窄断言，旧件冻结、不混P6。

**阶段2、运行禁令不变：完整本地P9、预处理、编译链接、仓库程序或脚本、Graphviz、QEMU、实机、真实进程/PID/命名空间/时钟/调度/文件操作均NOT_RUN。** 真实类/系统调用链接、PID对象并发安全、父子退出、ABI错误转换和子进程实际调度均未知。本包仅静态源码及合同指定数值模型，无内核源码修改、补丁、PR元数据改动或合并权限。作者仅交`READY_FOR_MAINLINE_REVIEW`，主线`PENDING`，交原C09用Pro独立验收五项勘误及新专题。
