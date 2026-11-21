---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89
record_type: bounded_worker_pool_bootstrap_static_analysis_report
produced_by: GPT-6（当前对话所示模型家族；后台不可独立认证）
model_per_owner: GPT-6＋Extra
date: 2026-11-15
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
lead_fixed_short12: ed9cd00830d9
execution_parent_short12: e9f1ad989ea9
status: AUTHOR_STATIC_READY_FOR_MAINLINE_REVIEW_AFTER_PUBLICATION
mainline_acceptance: PENDING
R88_01: CORRECTION_SUBMITTED_PENDING_C06_REVIEW
runtime_evidence: NOT_RUN
open_questions: [WQP89-U01, WQP89-U02, WQP89-U03, WQP89-U04, WQP89-U05, WQP89-U06, WQP89-U07, WQP89-U08, WQP89-U09]
---

# 第89批：工作队列线程池的静态存储、资源创建与manager进展

本批由原 **MYOS2-A-C06b / CHAT-EXEC-11** 在固定time源码下执行。只做源码静态合同和有限条件模型，未运行编译、仓库程序、内核、QEMU或实机。工作队列此前88批是 **RETURN（仅R88-01证据文档问题）**，不是88已通过；87限定PASS_PENDING_LOCAL和86的R86-01已闭合继续有效。89先追加D00，再完成A/B/C三个新增专题；不修改原88六件或正式评论6889090097，不给自己签88闭合或89通过。

## D00｜两条旧88证据边界的追加纠正

本批直接独立重新读取了原88固定提交`e9f1ad989ea9`的`evidence.md`和`facts-and-boundaries.yaml`，以及相关正文、问题台账、图、清单。原88有103个原始保留标签；原作者自检记103/103/0。**撤回**“这103条都在它们标出的真实单一定义内部”这一旧总括：字面能匹配源码和标签总数准确，不能免除单一定义边界/符号身份检查。

| 旧锚 | 原问题 | 89独立短锚 | 旧消费者的追加映射 |
|---|---|---|---|
| WQ88-E011 | 引文三行混入 `struct work_struct;` 前置声明、`work_s`别名和`work_func_t`三种定义，不能归为一个`work_func_t`定义 | **WQP89-E002**仅含`work_func_t`自身一行；**WQP89-E001**单独含`work_s`别名一行 | 源ledger、原证据账；矩阵WQ88-A-D01、A-D05；缺口WQ88-U01 |
| WQ88-E039 | 用不存在的`pool_flags`名称标记真实四个旗标枚举成员 | **WQP89-E003**：真实`enum worker_pool_flags`内部连续四行 | 源ledger、原证据账、`kick_decision`事实键；矩阵WQ88-B-Q06、B-K05；模型WQ88-M12；候选WQ88-F03及缺口WQ88-U05 |

这不是对旧88源码结论的整包重做。旧88六件、旧103/103/0、原回执、原正式评论均冻结；89有独立的120项新锚（其中三项为D00，旧88的103不作为89新增来源）；不能倒填旧报告或把103改成101。主线已经给出的R88-S01/S02/S03仍为必读消费限定：本批不将“返回true、入链、通知worker、执行回调”视为同一事件；不将固定main.c两个注释调用外推为全树禁用，也不把worker创建当回调实际到达。D00仅报 **CORRECTION_SUBMITTED_PENDING_C06_REVIEW**；是否关闭由原C06 Pro另行独立判断。

## G01/A｜每CPU静态池、双阶段入口与局部初始化

`percpu_area.c`分别声明`bh_worker_pools`和`cpu_worker_pools`，两者都是`worker_pool_s[NR_STD_WORKER_POOLS]`型perCPU数组。常量`NR_STD_WORKER_POOLS=2`；`for_each_bh_worker_pool`、`for_each_cpu_worker_pool`均取某CPU的数组[0]并以[2]为上界。这给出**每个被实际枚举的CPU拥有两类数组、每类两个pool对象**的链接模板结构；要变成合法真实CPU实例仍需86/87的模板拷贝、地址解码、对象范围和零初始化门。所读`for_each_possible_cpu`与`for_each_online_cpu`实际上都别名密集的`for_each_cpu`，不是根据名称就存在不同的真正CPU掩码循环。模型M01/M02/M03把N=0、1、2分别展开为早期池初始化调用尝试数0、4、8，未声称实际启动运行。

`workqueue_init_early`源序首先请求`pwq_cache=KMEM_CACHE(pool_workqueue,SLAB_PANIC)`并初始化`worker_pool_idr`，随后遍历每CPU的BH和普通两类pool。对每一类先把`i`设0，然后依次调用`init_cpu_worker_pool(pool,cpu,std_nice[i])`。BH路径在成功调用后追加`pool->flags |= POOL_BH`，普通类不做这一追加。初始化数组按`{0,HIGHPRI_NICE_LEVEL}`给定nice设置：同一个CPU中每类第0池nice0、第1池nice为HIGHPRI_NICE_LEVEL；并非直接保证线程优先级已生效。完成两类循环之后才出现system_wq和system_bh_wq的构造调用；前者历史PA87的perCPU PWQ资源/错误传播仍是独立门。

`init_worker_pool`确实写入`spin_lock_init`、`id=-1`、`cpu=-1`、`node=NUMA_NO_NODE`、`flags |= POOL_DISASSOCIATED`，并初始化`worklist`、`idle_list`、`workers`的链表头/计数，`ida_init`及`attrs=kzalloc(...)`。**它并没有在所读函数内显式重置`nr_running`、`nr_workers`和`manager`。**这些值若从合法初始零对象进入可以为零；但不能仅凭函数注释“newly zalloc'd”将这种前提普遍化。M04举出旧`flags=2,nr_running=3,nr_workers=7`的非零输入：`flags |= 4`得到6，后面三个未写字段仍保留给定旧值；此算例不是实际启动对象值。对未证有效的perCPU槽、IDA位图、attrs对象必须止证，不能继续沿下游字段进行运行模拟。

`init_cpu_worker_pool`先在`BUG_ON(init_worker_pool(pool))`检查内调用前者，然后才写CPU编号、`attrs->nice`、node0，最后`BUG_ON(worker_pool_assign_id(pool))`。`worker_pool_assign_id`调用`idr_alloc(worker_pool_idr,pool,0,WORK_OFFQ_POOL_NONE,GFP_KERNEL)`；非负ID写入pool.id并返回0，负数直接上交。两处BUG_ON在所读配置路径中展开为遇真调用BUG/panic的条件表达式，**不是普通记录错误后自然继续**。attrs申请NULL时已修改的pool元数据不会由本函数自动撤回；IDR的`idr_init_base`虽然给内部缓冲分配内存并标记initiated，却未在这条局部实现检查内部指针NULL。需要分别保留attrs/IDR/BUG的前序返回门。绝不以局部alloc函数名证明完整底层供应器的正确性。

`workqueue_init`的已读活动源码先以密集possible循环对BH每池调用`BUG_ON(!create_worker(pool))`，然后以“online”宏遍历普通池，先清`POOL_DISASSOCIATED`，再为每个普通池调用同一创建函数。若早期pool零初始化且相关调用正常返回，BH在进入后期时的标志为5（DISASSOCIATED4 | BH1）；普通池原为4，后期清位变0。**真实main.c的`start_kernel`中`workqueue_init_early()`调用和`kernel_init_freeable`中`workqueue_init()`调用均是注释。**只可说这两处固定启动入口不进行活动调用，不能外推为全库没有任何其它调用、也不能宣称正常启动已经建立了这些pool。

## G02/B｜worker ID、对象、线程、挂接、idle与失败

`create_worker(pool)`严格按以下源阶段：先`ida_alloc(&pool->worker_ida,GFP_KERNEL)`取int ID；若ID负直接返回NULL，不申请worker对象。成功后`alloc_worker(pool->node)`使用`kzalloc_node`请求worker对象；其直接宏把`__GFP_ZERO`传给忽略node参数的`kmalloc_node`，**不是每个字节零值已被执行程序证实**。对象非NULL时显式初始化`entry`和`node`的链节点、`scheduled`链表头，然后`flags=WORKER_PREP`。其`task`、`current_work`等若推断为零还需成功的清零分配。失败转`fail`标签调用`ida_free(id)`和`kfree(worker)`，不能从函数名推定IDA位已经被正确归还。

在所读`ida_alloc_range`实现中，函数遍历`bitmap[slot_idx]`，再对`bit_idx<sizeof(long)`逐位测试，遇0立即返回对应整数ID；这条函数**没有把已选择的位设置成1**。因此给定有效初始零位图、相同参数、两次串行调用之间没有任何外部写者，两次都可返回0。M09把位图“0→仍为0→再次得0”展开为具体中间状态；这是可能重复编号的源码候选，不是两个真实kthread已经成功同名运行。关联的`ida_free`实现采用`bitmap[word] &= (1 << bit)`，与释放一个位时通常需要的清位效果不同；M12以初始word11、bit1得word2，直观显示它可能清掉其它已置位而保留目标位。两处ID协议不一致应一并交Pro审核，不将函数注释中的通用Linux行为覆盖当前具体实现。`ida_init`同样在申请bitmap后没有判空就设置initiated。

普通池（`!(flags&POOL_BH)`）下，先格式化worker ID字符串，其中若pool.cpu>=0，会读`pool->attrs->nice`来决定后缀，再调用`kthread_create_on_node(simple_worker_thread,worker,pool->node,"%s",id_buf)`。所读`kthread_create_on_node`是到`__kthread_create_on_node`的变参包装，内部实际请求kthread创建、排入kthread_create_list、唤醒kthreadd并等待completion；不是调用后立即已运行worker回调。返回`IS_ERR(worker->task)`时按`PTR_ERR`分类并转入fail。只检查`IS_ERR`而未在该处额外判NULL，若假设其返回NULL，则所读`IS_ERR(NULL)`为false，之后`set_user_nice(worker->task,...) `的正常证明停在NULL任务参数处；**目前未证明kthread实会返回NULL**。`set_user_nice`调用没有在这条本地路径接收返回值，CPU绑定语句处于注释，不能由格式化名称认定真实绑定或调度完成。

BH池跳过整个kthread创建/nice设置分支；在成功且worker初始task=NULL的条件下，后面即使返回了非NULL worker，也没有新task对象。`worker_attach_to_pool`在有效对象/链表情况下会依据`POOL_DISASSOCIATED`对worker追加`WORKER_UNBOUND`，随后把`worker->node`加入`pool->workers`，**紧接着才写`worker->pool=pool`**。这是可见的发布顺序，不证明其他读取者必然观察到中间状态或发生竞态；相关mutex调用与亲和性调用已注释。之后`create_worker`加pool锁、`nr_workers++`、`worker_enter_idle`：后者添加`WORKER_IDLE`、写last_active和加入idle_list并增加表头count。在普通有效task时，执行`wake_up_process(task)`请求；BH假设task为NULL则不执行这一请求。表头count写入不单独证实链表/锁和中间函数都正常完成，更不证明任务已经获得CPU。

M16普通例以初态flags PREP8, nr_workers0, idle_count0，成功attach和idle后得到worker.flags=12、两个池计数各自递增1及一次wake请求；M17 BH例从`POOL_BH|DISASSOCIATED=5`出发，worker PREP8并加UNBOUND128和IDLE4，得到flags140、nr_workers1/idle_count1但新task仍为NULL。可分别作为条件性局部存储状态，不能混合为“系统运行了一个worker”。ID失败、worker元数据NULL、kthread错误指针、以及后续attach/list/lock/callee未返回，分别在矩阵中列了不同停止点。没有补丁或下级线程/调度全链研究。

## G03/C｜manager占用、创建重试与worker可工作门

在真实`workqueue.h`策略定义里，`need_more_worker`为`!list_header_is_empty(worklist) && nr_running==0`；此仓库的`list_header_is_empty`读**表头count==0**，不是只读anchor空链。 `may_start_working`是`idle_list.count!=0`；`need_to_create_worker = need_more_worker && !may_start_working`。因此在有效count/链不变量且worklist.count1、nr_running0、idle0时需求为true；若idle1或nr_running1则该需求为false。计数是源级字段读写，不能当作真实CPU运行计数实测值。

`manage_workers(worker)`先检查`POOL_MANAGER_ACTIVE`；若已经置位，立即返回false，不抢manager也不创建worker。未置位时先`flags|=MANAGER_ACTIVE`、`pool->manager=worker`，调用`maybe_create_worker`；**只有后者正常返回**，才置`manager=NULL`、清该位并返回true。这里的false只描述本次没有承接管理；true表示本次管理子流程曾执行到返回，不表示worker已可执行回调。调用者`simple_worker_thread`在`!may_start_working && manage_workers(worker)`为true时`goto recheck`，会重新判断而不是直接进入回调。

`maybe_create_worker`有一层do/while和一层while(true)：外层先请求`spin_unlock_irq`，在锁外内层反复尝试`create_worker(pool)`，如果其返回非NULL或重测`need_to_create_worker`为false则break；失败且需求仍存在还会再测一次，然后继续。源码中的mayday timer和CREATE_COOLDOWN相关调用均为注释；不能把注释当成活动退避。内层退出之后请求`spin_lock_irq`，再用`need_to_create_worker`判断是否继续外层循环。因此：一次成功创建可使idle计数增加而结束本次需要；如果重取锁前资源被消耗或需求重生，还可能再次循环。反之持续失败且需求稳定、每次调用正常返回时，在这个局部模型中找不到有限退出门。**无限失败假设不是实测死循环，任何锁等待、kthread completion等待或非法对象访问还可能更早停止正常证明。**

将88提交层对`pool->worklist`的有效非空、88 worker通知层的idle表与nr_running状态接回本包，仍须留下87的perCPU对象地址和88的work->data/PWQ/回调来源约束。在`simple_worker_thread`实际入口中，先检查need_more，之后遇到没有idle时才可能调用manager；已有manager返回false不是“可以安全运行”的充分证明。BH所谓伪worker非NULL只是数据对象，之前88的kick_pool BH分支仍有单独等待/不返回限定（保留WQ88-F03身份）。不研究完整调度/工作项执行、CPU热插拔、多核时序或真实线程竞争。

## 结论及下一步

D00纠正所需的新源码锚和消费者映射齐全；新增三个主题各自有定点反例。总体提供10组54行矩阵、26个明确输入/中间值的条件模型（A8/B9/C9）、7项WQP89条件候选和9项独立缺口。问题ID不是已复现的内核故障数，和旧PA87/WQ88身份有重叠的地方仍按原号保留。所有新短锚集中在本批evidence.md，共120条；该数量包括3个D00新定义，不重复算旧88的103条。

完整本地P9套件、内核预处理/编译/链接、仓库程序/脚本、Graphviz、QEMU、实机及并发实验均未运行，源代码行为与运行结果分账。只写本批限定六件，不修改旧文件、评论或PR元数据，不合并/强推，不启用额外模型或付费API。下一步在PR17正式提交、六件双回读及本人唯一评论核验后即STOP_WRITING；由原C06 Pro独立审核**R88-01追加纠正与第89批成果**并同轮安排后继。当前不自行签88闭合或89通过，也不启动90。

## 结构化矩阵的可读逐行导航（具体前提与证据见facts）

### pool_storage_and_initialization_matrix（6行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-A-ST01 | 每CPU静态池 | 每CPU模板各2个pool对象，BH/normal合计4个；不等于运行实例已初始化 | 86/87的模板副本、偏移及映射合法性 | WQP89-E004, WQP89-E005, WQP89-E008 |
| WQP89-A-ST02 | possible枚举 | 所读宏枚举整数0..N-1，不测试独立possible位图 | N与NR_CPUS/地址界限 | WQP89-E010 |
| WQP89-A-ST03 | BH内部索引 | 每CPU两个BH pool，各依次取得i=0/1 | per_cpu展开与内存映射 | WQP89-E006, WQP89-E008, WQP89-E043, WQP89-E044 |
| WQP89-A-ST04 | 普通内部索引 | 每CPU两个普通pool，各依次取得i=0/1 | per_cpu实例可写 | WQP89-E007, WQP89-E008, WQP89-E045 |
| WQP89-A-ST05 | pool部分赋值 | 未明确写nr_running/nr_workers/manager为0；其值依赖原对象来源 | 不能拿函数注释的新清零对象假定所有输入均清零 | WQP89-E011, WQP89-E012, WQP89-E013, WQP89-E014, WQP89-E015, WQP89-E016, WQP89-E031, WQP89-E032, WQP89-E033, WQP89-E034 |
| WQP89-A-ST06 | 归属/管理字段 | 三种状态字段的生产者不等同于pool初始化中显式写入 | 先验证对象存储和初始化先序 | WQP89-E013, WQP89-E014, WQP89-E015, WQP89-E016, WQP89-E031, WQP89-E035 |

### pool_id_attributes_and_failure_matrix（5行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-A-ID01 | 普通分配前置 | IDA bitmap请求不检查NULL，attrs单独检查NULL | bitmap kzalloc是否成功、后台供应器未审 | WQP89-E034, WQP89-E035, WQP89-E052 |
| WQP89-A-ID02 | attrs失败 | pool对象已经发生部分元数据写入；不进入该函数正常末尾 | BITMAP分配结果、调用者BUG_ON可能不返回 | WQP89-E031, WQP89-E032, WQP89-E033, WQP89-E034, WQP89-E035, WQP89-E028, WQP89-E037 |
| WQP89-A-ID03 | IDR初始化 | 赋值initiated=true不构成底层缓冲分配成功的证书 | 失败后调用idr_alloc可能先在NULL解引用停证 | WQP89-E041, WQP89-E022, WQP89-E023, WQP89-E024 |
| WQP89-A-ID04 | 池ID成功 | 正常调用者BUG_ON条件为false，池ID赋值有条件成立 | IDR存储缓冲正确、ID不冲突 | WQP89-E020, WQP89-E021, WQP89-E039, WQP89-E025, WQP89-E026 |
| WQP89-A-ID05 | 池ID失败 | 初始化路径要求BUG条件分支，不能继续声称后续池初始化成功 | BUG宏panic是否返回未证，按正常成功证明停止 | WQP89-E020, WQP89-E021, WQP89-E028, WQP89-E039 |

### bootstrap_entry_and_online_matrix（5行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-A-BOOT01 | 固定主启动入口 | 该位置没有活动调用，不能推全树不存在调用 | 其他调用者未穷尽、真实启动未跑 | WQP89-E050 |
| WQP89-A-BOOT02 | 固定后启动入口 | 该位置没有活动调用；局部函数定义可单独静态分析 | 不得宣称整个内核永不创建worker | WQP89-E051 |
| WQP89-A-BOOT03 | 早期系统队列 | 源码内先后关系成立，不保证slab/IDR/PWQ构造成功 | PA87构造错误传播条件未关闭 | WQP89-E040, WQP89-E041, WQP89-E042, WQP89-E043, WQP89-E045, WQP89-E046, WQP89-E047 |
| WQP89-A-BOOT04 | 后期BH工作线程 | 每遍历一个BH pool就请求一个worker对象，不等于有真实kthread | create_worker和BUG_ON先行返回 | WQP89-E048, WQP89-E006, WQP89-E028 |
| WQP89-A-BOOT05 | 后期普通工作线程 | 所读online同dense遍历；普通池标记清位先于创建请求 | 配置/CPU存在性、池对象和线程对象合法性 | WQP89-E049, WQP89-E010, WQP89-E003 |

### worker_resource_acquisition_matrix（6行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-B-RES01 | IDA成功编号 | 所读分配实现不设置对应bit；连续相同无外部修改时可返回同号 | 不等于真实多线程运行或用户程序创建成功 | WQP89-E061, WQP89-E077, WQP89-E078, WQP89-E079, WQP89-E082, WQP89-E083, WQP89-E084 |
| WQP89-B-RES02 | IDA失败编号 | 没有worker对象申请和attach，失误打印可有外部影响 | ida_alloc_range实际返回值前提 | WQP89-E060, WQP89-E061, WQP89-E062, WQP89-E077, WQP89-E080 |
| WQP89-B-RES03 | worker对象初始化 | 对象有三个本地显式初始化，task/current字段来源依赖清零 | kzalloc_node只传ZERO，低层清零有效未全面验证 | WQP89-E053, WQP89-E054, WQP89-E055, WQP89-E085, WQP89-E086 |
| WQP89-B-RES04 | 普通线程申请 | 检查错误指针且设置nice；不是直接启动回调 | kthread等待completion可能不返回、NULL非ERR_PTR | WQP89-E056, WQP89-E057, WQP89-E058, WQP89-E065, WQP89-E066, WQP89-E067, WQP89-E068, WQP89-E089, WQP89-E090, WQP89-E091, WQP89-E092 |
| WQP89-B-RES05 | BH伪worker | BH局部成功并不要求真实task产生；清零前提下task保持NULL | 是否在其它处模拟BH执行，已排除 | WQP89-E064, WQP89-E068, WQP89-E071, WQP89-E074 |
| WQP89-B-RES06 | IDA释放行为 | 此表达不同于清除所选位；不能凭函数名宣称已解除编号占用 | Bitmap状态/编号分配协议需分层审核 | WQP89-E081, WQP89-E084 |

### worker_attachment_publication_matrix（5行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-B-PUB01 | 分离池附着 | 普通早期或BH均可能进入分离逻辑 | 标志实际生产者及无并发修改条件 | WQP89-E074, WQP89-E003, WQP89-E032, WQP89-E044 |
| WQP89-B-PUB02 | 关联池附着 | 所读路径没有该位置实际CPU绑定调用 | 真实调度、其它绑定入口未审 | WQP89-E075, WQP89-E049 |
| WQP89-B-PUB03 | workers表插入 | 链表发布先于pool指针赋值，必须保留先行返回/同步门 | list校验/锁状态与并发未全面验证 | WQP89-E076, WQP89-E102, WQP89-E073 |
| WQP89-B-PUB04 | pool计数/idle | 计数和idle表发布发生在worker线程唤醒请求之前 | 池表一致性，WARN前置校验和低层lock操作 | WQP89-E069, WQP89-E070, WQP89-E099, WQP89-E100, WQP89-E101, WQP89-E102 |
| WQP89-B-PUB05 | task唤醒条件 | 记录的是唤醒请求/返回，不等于被调度或回调执行 | wake_up_process下级调度路径未审 | WQP89-E071, WQP89-E098 |

### worker_failure_and_cleanup_matrix（6行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-B-FAIL01 | ID返回负数 | 无本函数新对象需kfree/ida_free | 尚未分配worker ID成功 | WQP89-E062, WQP89-E077, WQP89-E080 |
| WQP89-B-FAIL02 | 对象NULL | worker尚未挂接，不代表IDA位图正确回收 | ida_free自身掩码语义与kfree NULL条件 | WQP89-E063, WQP89-E072, WQP89-E081 |
| WQP89-B-FAIL03 | kthread ERR_PTR | 创建路径在attach前退出；下级线程资源释放未全面审 | kthread_create_on_node错误/结果格式前提 | WQP89-E065, WQP89-E066, WQP89-E067, WQP89-E072, WQP89-E087, WQP89-E088, WQP89-E089 |
| WQP89-B-FAIL04 | kthread NULL | 正常路径在NULL任务有效性门停止证明；不声称实际返回NULL | 真实callee是否可返回NULL未确立 | WQP89-E065, WQP89-E068, WQP89-E089 |
| WQP89-B-FAIL05 | attach/idle不返回 | 不能沿正常返回行推算nr_workers/wakeup | 前序有效对象、列表与锁门 | WQP89-E069, WQP89-E070, WQP89-E076, WQP89-E099, WQP89-E102, WQP89-E117 |
| WQP89-B-FAIL06 | 线程已创建但后续异常 | 不代表实测泄漏，存储/线程所有权全链未核 | 下游不可证返回先停证 | WQP89-E069, WQP89-E071, WQP89-E072, WQP89-E090, WQP89-E097 |

### manager_ownership_and_return_matrix（4行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-C-MGR01 | 已有manager | 该分支没有调用maybe_create_worker, false不是worker可运行的证书 | 真实线程外部状态与锁所有权 | WQP89-E106, WQP89-E003 |
| WQP89-C-MGR02 | 取得manager | 有条件建立单个本地manager身份，不代表全系统互斥 | 所读函数外部并发/锁假设 | WQP89-E107, WQP89-E015 |
| WQP89-C-MGR03 | 管理成功回返 | true意为执行管理并可让调用者recheck，不等于已有可运行worker | 后续重新检查需求可能变化 | WQP89-E108, WQP89-E109, WQP89-E116 |
| WQP89-C-MGR04 | manager不返回 | 不能断言manager标志已清除 | create/锁/列表与调度接口返回缺口 | WQP89-E107, WQP89-E108, WQP89-E110, WQP89-E111, WQP89-E114 |

### retry_lock_and_progress_matrix（6行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-C-TRY01 | 进入重试 | 创建发生在源内释放锁请求之后 | 其它写者是否改变worklist/idle未知 | WQP89-E110, WQP89-E117, WQP89-E118 |
| WQP89-C-TRY02 | 创建成功 | 本次内循环退出，即使重新取锁后又发现需增建也可能重启外循环 | 合法创建对象/idle发布才是可用资源前提 | WQP89-E111, WQP89-E069, WQP89-E070, WQP89-E114 |
| WQP89-C-TRY03 | 创建失败且需求消失 | 外层还须重锁后再次评估 | need_more/idle字段与并发变化 | WQP89-E111, WQP89-E112, WQP89-E103, WQP89-E104, WQP89-E105 |
| WQP89-C-TRY04 | 失败且需求持续 | 在无限条件成功返回的失败序列中无法证有限结束 | 不宣称物理机已出现死循环 | WQP89-E111, WQP89-E112, WQP89-E105 |
| WQP89-C-TRY05 | 重锁后需求又生 | 再次申请资源；一次成功不承诺整个管理函数返回 | 锁外状态可能变化，真实并发未审 | WQP89-E113, WQP89-E114, WQP89-E105 |
| WQP89-C-TRY06 | 先行callee不返回 | 停止前序证明，不伪造下一状态 | 线程创建等待及底层锁机制 | WQP89-E110, WQP89-E111, WQP89-E113, WQP89-E108, WQP89-E116 |

### bootstrap_to_worker_readiness_matrix（5行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-C-RDY01 | worklist/idle政策 | 可能进入manager新建流程而非立即处理回调 | 完整worklist内容和PWQ有效性 | WQP89-E103, WQP89-E104, WQP89-E105, WQP89-E030 |
| WQP89-C-RDY02 | 已有idle worker | 创建器不必因这个状态新增worker | 实际可运行task/worker flag仍未证 | WQP89-E103, WQP89-E104, WQP89-E105 |
| WQP89-C-RDY03 | 已有running计数 | 管理不是该局部条件必需；不证明已有worker真的在CPU运行 | 计数来源和运行事件未观测 | WQP89-E103, WQP89-E105, WQP89-E013 |
| WQP89-C-RDY04 | 普通online清位 | 关联普通pool新建可有task/wake请求；不证明task已经执行回调 | late调用入口注释与kthread未审 | WQP89-E049, WQP89-E050, WQP89-E051, WQP89-E068, WQP89-E071 |
| WQP89-C-RDY05 | BH池建立 | 记录伪worker资源但不证明BH kick执行（88已记录相关门） | 88 WQ88-F03身份保留，不重新计根因 | WQP89-E044, WQP89-E048, WQP89-E064, WQP89-E070, WQP89-E071 |

### ABC_join_matrix（6行）

| 行 | 具体场景 | 有界结论 | 首个止证门 | 定位锚 |
|---|---|---|---|---|
| WQP89-ABC01 | 四池来源到worker | 理论每CPU4个池需分辨BH两个与普通两个的资源初态 | 86/87合法perCPU对象，88有效worklist输入 | WQP89-E004, WQP89-E005, WQP89-E006, WQP89-E007, WQP89-E008, WQP89-E044, WQP89-E045, WQP89-E048, WQP89-E049 |
| WQP89-ABC02 | pool初始化→标志 | later normal清DISASSOCIATED, BH仍DISASSOCIATED | 不是实际启动到达证明 | WQP89-E031, WQP89-E032, WQP89-E044, WQP89-E045, WQP89-E049 |
| WQP89-ABC03 | IDA到线程身份 | 同一数字ID可能复用，命名无法由函数名保证唯一 | 不推断两个线程均真实创建 | WQP89-E077, WQP89-E079, WQP89-E056, WQP89-E060, WQP89-E061 |
| WQP89-ABC04 | 创建发布层级 | 这不是实际获得CPU或回调运行，且BH无任务请求 | 对88的提交/回调门保持 | WQP89-E069, WQP89-E070, WQP89-E071, WQP89-E076, WQP89-E099, WQP89-E100 |
| WQP89-ABC05 | 空闲资源推动管理 | 返回true之后worker_thread recheck，重新求值而非成功运行 | 资源真实到位/先行返回条件 | WQP89-E103, WQP89-E105, WQP89-E107, WQP89-E111, WQP89-E113, WQP89-E114, WQP89-E108, WQP89-E109, WQP89-E116 |
| WQP89-ABC06 | 文档补正与技术分层 | 旧103标签数不变，89新120独立计数，C06待审 | 未修任何内核或旧成果 | WQP89-E001, WQP89-E002, WQP89-E003 |

## 26个模型的场景导航（真正输入和中间值保存在facts）

| 模型 | 对应真实源码场景 | 模拟输入及前提摘要 | 关键不能推断的事项 |
|---|---|---|---|
| WQP89-M01 | 零CPU枚举 | N=0；nr_cpu_ids=0, no signed loop overflow, early function hypothetically invoked | cache and IDR calls, later WQ allocations may fail or not return |
| WQP89-M02 | 单CPU清零模板 | N=1, perCPU pool instances zeroed；CPU0 memory points to four distinct valid pools, all callees return | not proof template/all CPU perCPU storage is fully copied |
| WQP89-M03 | 双CPU静态池 | N=2；valid perCPU arrays and for_each int bounds | not proof eight unique physical objects initialized or workers created |
| WQP89-M04 | 复用非零pool | pool.flags=2; nr_running=3; nr_workers=7; manager=nonNULL；valid object, all list/IDA/attrs helpers return; no outside writes | this nonzero input is hypothetical, static template startup object may be zero |
| WQP89-M05 | attrs申请失败 | pool valid, attrs kzalloc=NULL；spinlock/list/IDA init all return, ERROR=-ENOMEM | BUG/panic may not return, no actual kernel trap observed |
| WQP89-M06 | IDR取得ID | idr_alloc returns id=7；idr buffer valid, ret>=0 | IDR allocation consistency and complete cache preparation untested |
| WQP89-M07 | IDR拒绝 | idr_alloc returns -ENOSPC；attrs allocated and pool initialized through ID step | panic handler and full failure cleanup not audited |
| WQP89-M08 | late普通与BH pool标志 | early zero-init: BH flags5, normal flags4; N=1；early and later functions hypothetically invoked, all first calls return | main.c both call sites commented; zero-template/perCPU addresses unresolved |
| WQP89-M09 | IDA重复编号 | IDA bitmap[0]=0, sizeof(long)=8, min0；valid allocated array, two serial ida_alloc calls with no other writes | not two actual kernel threads; call-site success and lower allocation unknown |
| WQP89-M10 | IDA内存申请NULL | ida_init kzalloc returns NULL；ida_s object valid, no external writes | actual allocator failure/CPU fault not observed |
| WQP89-M11 | IDA所有低位占用 | on selected LP64 each bitmap word low 8 bits set (e.g. 0xff), all 4096 words valid；for loop does not find an unset bit among bit_idx 0..7 | no actual exhaustion or busy state measured |
| WQP89-M12 | IDA释放掩码 | valid bitmap word=0b1011; id=1；LP64 int shift valid, ida_free executes | intended IDA contract and real callers not fully audited |
| WQP89-M13 | worker元数据NULL | ida_alloc gives id3, kzalloc_node returns NULL；pool valid, ida_free and kfree(NULL) return | IDA bitmap semantics; lower kmalloc not executed |
| WQP89-M14 | kthread error pointer | id3 valid, worker allocated, ordinary pool, kthread returns ERR_PTR(-ENOMEM)；valid pool.attrs/task representation, formatter returns | thread resource ownership below kthread wrapper not fully audited |
| WQP89-M15 | kthread非ERR NULL | id3 valid, worker allocated, ordinary pool, kthread returns NULL；IS_ERR(NULL) false in chosen pointer representation | not demonstrated kthread returns NULL; cannot continue to attach |
| WQP89-M16 | 普通worker顺序成功 | pool.flags=0; nr_workers0; idle_list.count0; valid ordinary task nonNULL; id3；all allocations, list additions, locks, wake request return; no other writers | pointer/list/lock and caller initialization plus lower scheduler are conditional |
| WQP89-M17 | BH伪worker顺序成功 | pool.flags=POOL_BH\|POOL_DISASSOCIATED=5, nr_workers0, idle0; worker.task initially NULL；valid zeroed allocation, IDA accepted, lists/locks normal | not proof BH pseudo-worker actually runs/88 kick nonreturn resolved |
| WQP89-M18 | manager已有其他持有者 | pool.flags includes POOL_MANAGER_ACTIVE, idle_count0, demand=true；valid pool and worker, required lock context | existing manager actually progresses is outside function proof |
| WQP89-M19 | 单次创建使需求消失 | worklist.count1,nr_running0,idle_count0; manager flag0；valid list/fields; create_worker returns valid object, idle_count becomes1 and all locks return | worker must actually become idle under all valid list/return preconditions |
| WQP89-M20 | 反复失败且需求恒定 | worklist.count1,nr_running0,idle_count0 stays fixed；each create_worker returns NULL promptly; lock release normal; external state never changes | not real infinite execution; real resource outcome/concurrency unknown |
| WQP89-M21 | 失败后需求消失 | worklist.count1/nr_running0/idle0 initially; create_worker returns NULL；external change makes worklist.count0 before need_to_create second check | external state transition and lock semantics not observed |
| WQP89-M22 | 成功后被其它线程消耗 | first create_worker succeeds and idle_list goes 0->1, then becomes0 before relock; demand stays true；pool/workers and all source lock/calculations valid | concurrent state changes assumed, not measured |
| WQP89-M23 | kthread等待不返回 | ordinary create_worker enters __kthread_create_on_node, wait_for_completion never returns；prior allocations and completion setup valid | this is stop-of-proof, not evidence of actual deadlock |
| WQP89-M24 | nr_running使管理不触发 | worklist.count1; nr_running1; idle_count0；worklist valid, no concurrent changes | counter may not reflect actual thread execution |
| WQP89-M25 | idle worker绕过manager | worklist.count1; nr_running0; idle_count1；valid worker/pool and current worker in simple_worker_thread | 88 data/PWQ and callback returns remain open |
| WQP89-M26 | manager-active但idle=0 | worklist.count1,nr_running0,idle0, flag active and worker_thread proceeds；valid manager ref and worker/pool | existing manager and actual worklist validity unknown |

本页模型“发生/返回”只表示明确假定的有条件程序分支，不能从函数名、注释或模型输入冒称正常启动可达、资源供应、真实等待/唤醒、实际并发运行。所有计算均为独立有限整数/状态推演；未运行本仓库的任何执行文件或脚本。
