---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ID-ALLOCATION-LIFECYCLE-90
record_type: bounded_id_allocation_lifecycle_static_analysis_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_display_name: MYOS2-A-C06c 代码分析执行
produced_by: ChatGPT GPT-6 family (consumer backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-15
base_snapshot: time
source_pin_short12: a039d9803ade
execution_parent_short12: 42897a06feb1
mainline_pin_short12: ffe6d8b314c8
contract_pin_short12: 07ed0fed054c
read_channel: GitHub_connector
status: AUTHOR_STATIC_READY_FOR_MAINLINE_REVIEW_AFTER_PUBLICATION
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [IDA90-U01, IDA90-U02, IDA90-U03, IDA90-U04, IDA90-U05, IDA90-U06, IDA90-U07, IDA90-U08, IDA90-U09, IDA90-U10]
---

# 第90批｜IDA编号范围、IDR对象登记与限定消费者的静态接口核查

**执行边界。** 新会话 `MYOS2-A-C06c 代码分析执行`／`MYOS2-LEAD-002-CHAT-EXEC-12` 独立接收128三件并执行127技术合同。第90批不是89整体复审，也不是PID11整包重做；固定源码只来自 `time@a039d9803ade`。本件属于阶段2的静态作者分析；没有构建、执行仓库脚本、Graphviz、QEMU、实机或真实并发验证。所有“能够返回/发生”的条件句，仅对逐项列出的有效对象、范围、ABI与下级返回前提负责。

**历史裁定不变。** R88-01已关闭，88组合 `PASS_PENDING_LOCAL`。89为 `RETURN`，退回仅 `R89-01/R89-02`，其余主体按 `R89-S01/S02/S03` 有限定保留。本批D00只递交追加证据，状态 `CORRECTION_SUBMITTED_PENDING_C06_REVIEW`；90成果最高待C06 Pro审核，不由作者自签PASS或关闭任何旧编号。

## G00｜新会话接收与写前检查

接收顺序为固定128交接包／01接收指令／02替换令和127 → Mnemosyne真实早期接收报告（未读事项标pending）→ 九份MyOS2约束独立全文（含协议v2 P1–P13）→ 110 Owner续记、124模型分工续记分别全文 → Mnemosyne固定指导独立刷新及记录 → 全新G00。旧C06b、C06主线、早期执行者的“已读”资格一概未复用。128仅替换身份、接收方式、激活和交付marker，不改变127技术合同、D00、质量门及禁止事项；119为旧84模板，不启用。

实际G00核得：direct lead `ffe6d8b314c8`、exec `42897a06feb1`、master `e8e60cd19540`、time `a039d9803ade`；只有开放PR16与PR17，PR17保持Draft并指向现有执行分支；第90批目录和第90批正式交付均未出现。固定 `options_flags.cmake` 与双分支 `panic.c` 的金丝雀匹配，公约`1(2)完整读取。此处 `G00=PASS` 仅为接收/只读现场/限定写权门，不是协议P9完整机械检查或源码正确性证书；P9完整本地执行仍为 `NOT_RUN`。

## D00｜对第89批两处退回的独立追加纠正

### R89-01｜拆解错误复合宏短锚

旧 `WQP89-E010` 在 `for_each_possible_cpu` 的身份下合引了相邻的 `for_each_online_cpu` 行。它们虽在同一个头文件、字面正确且挨在一起，却是**两个独立宏定义**，不能说两行落在前一宏的单一定义内。本批独立读取固定 `mykernel/include/linux/kernel/cpumask.h`，重新建立：

| 原位置 | 本批独立短锚 | 真实定义及对消费者的修正 |
|---|---|---|
| `WQP89-E010` 合引 possible/online | `IDA90-E002` | `for_each_possible_cpu(cpu)` 自身一行，转发到 `for_each_cpu((cpu))` |
| 同一个旧锚的第二个宏 | `IDA90-E003` | `for_each_online_cpu(cpu)` 自身一行，也转发到 `for_each_cpu((cpu))` |
| 二者真正共同的低层语义 | `IDA90-E001` | `for_each_cpu` 是密集整数循环 `cpu=0..nr_cpu_ids-1`，没有活动位图筛选 |

**旧消费者逐项映射：** `89/evidence.md::WQP89-E010`；`89/facts.evidence_anchor_ledger[WQP89-E010]` 和 `source_ledger[cpumask.h]`、`fact_to_evidence_ids`；矩阵 `WQP89-A-ST02`、`WQP89-A-BOOT04`、`WQP89-A-BOOT05`；有限模型 `WQP89-M01/M02/M03/M08` 的CPU枚举前提；`89/workqueue-pool-bootstrap.md` G01/A叙述；`89/workqueue-pool-bootstrap-paths.dot` cluster_A的possible/online节点与关联边。受影响的不是全部89结论，而是这些位置以失格短锚支持“两种遍历”的消费方式。两种宏各自存在、都转交同一个低层循环的语义仍然成立；不得据名称外推真正不同的CPU掩码。

保留89作者原始120标签的 `120/120/0` **字面自检历史**，但明确**撤回**“120条均已通过所标真实单一定义边界审查”的总括。不能静默把120改成119，不能回填89旧六件或修改评论。新短锚独立计数。

### R89-02｜纠正假设NULL任务在set_user_nice的停止位置

89 `WQP89-M15`、`WQP89-B-FAIL04` 及 `WQP89-F05` 将“假设 `kthread_create_on_node` 返回NULL，`IS_ERR(NULL)` 为false”后传入 `set_user_nice(NULL,attrs.nice)` 当作定位完成的非法任务解引用或立即停止点。这一说法没有读最近callee的完整活动体。

本批实际全文查看 `mykernel/sched/runqueue/runqueue.c::set_user_nice`：函数只声明 `queued/running/rq/old_prio` 局部量，对任务 `p` 的检查、调度和优先级赋值**均为注释**，没有活动的 `p->...` 访问。新锚 `IDA90-E004/E005` 给出函数首尾；`IDA90-E006/E007/E008/E009/E010/E059` 对应调用、错误检查、挂接、计数、idle、条件唤醒和 `IS_ERR`。

仅在如下明确隔离假设中，后续条件轨迹可继续：

1. 普通（非BH）池、有效pool/worker、合法 `pool->attrs`；worker经适当清零初始化；假设kthread接口真的返回 `NULL`，并且前序格式化与其余下级函数均正常返回。
2. `IS_ERR(NULL)` 为false，不进入错误指针的 `fail` 标签。`set_user_nice(NULL,pool->attrs->nice)` 中 `pool->attrs->nice` 必须先是合法可求值表达式；函数活动体没有使用这个NULL任务指针。
3. 返回到 `create_worker` 后，继续调用 `worker_attach_to_pool`：合法的情况下先将节点加入 `pool->workers`，再写 `worker->pool=pool`；随后请求pool锁，`nr_workers` 从0变1，`worker_enter_idle` 可令 `idle_list.count` 从0变1。
4. `if(worker->task)` 为false，跳过 `wake_up_process`；若所有前置与callee都正常，可 `return worker`（非NULL）。此时**没有因此证明产生了可运行线程、线程被唤醒或处理工作回调**。

这条有限轨迹在结构化事实文件中记为 `IDA90-M30`。它**不能证明**真实kthread接口会返回NULL，也不解除旧89的对象/列表/锁/调度器未知前提。

**旧消费者映射：** `89/facts.finite_models[WQP89-M15]` 中“normal proof stops on invalid task argument”须替换为上述有条件继续读法；`89/matrices.worker_failure_and_cleanup_matrix[WQP89-B-FAIL04]` 的“cannot continue to attach”失效；`worker_resource_acquisition_matrix[WQP89-B-RES04]` 的NULL风险限定应转为未验证真实callee返回；`facts.state_proof_gates` 相应停止条件；`89/deferred-findings.new_candidates[WQP89-F05]` 保持旧ID但更正停止点；`new_gaps[WQP89-U08]` 仍是实际nice/CPU绑定及BH执行缺口；`89/workqueue-pool-bootstrap.md` G02/B叙述和`DOT cluster_B`条件边需要按此新证据消费。旧89六件与评论不修改，也不自行把R89-02签为CLOSED。

**R89-S01/S02/S03和第88批限定继续有效：** 直接IDA失败在 `create_worker` 对象申请前 `return NULL`，不是走后来 `fail:ida_free/kfree`；只有发生后续失败才进入该标签。有关WARN的位置要按真实源序，在相应函数内部检查先后，不能以图中的总结节点替代源码。返回true、入链、唤醒请求与回调执行仍是不同事件。

## G01/A｜IDA请求范围、存储容量、搜索与归还

### 容量单位不是同一个概念

`idr_const.h` 设 `MYOS_IDR_BUF_SIZE=32768`、`IDA_CHUNK_SIZE=32768`；`IDA_BITMAP_LONGS=32768 / sizeof(long)`。`ida_init` 分配 `IDA_BITMAP_LONGS * sizeof(long)` **字节** 并写 `initiated=true`，没核查kzalloc是否成功。以明确的LP64模型 `sizeof(long)=8` 计算：申请32768字节、4096个 `ulong`、理论位数262144。但 `ida_alloc_range` 的活动内循环是 `bit_idx < sizeof(long)`，每个word只检查 **8个bit**，不是64个；每个返回ID的公式为 `slot_idx * sizeof(long) + bit_idx`，所读函数实际最多枚举编号0..32767。

因此 `IDA_BITMAP_BITS=262144` 这个宏所描述的位图总位数，并非这条活动分配扫描可以访问的逻辑编号个数。这里不能把前者照搬成IDA实际支持的最大ID，也不能把上述模型自动标为未经实测机器的ABI事实。辅助短锚为 `IDA90-E011..E014`、`E018..E021`。

### 请求语义、占用与释放的分离

- `ida_alloc(ida,gfp)` 传 `(min=0,max=~0)`；`ida_alloc_min` 将所给 `min` 原样传入；`ida_alloc_max` 将所给 `max` 原样传入。wrapper与其文档不能代替 `ida_alloc_range` 活动体。（`IDA90-E015..E017`）
- `ida_alloc_range` 用 `min` 初始化 `slot_idx`，即**word数组下标**，而不是数字ID的精确下界；没有任何活动语句消费 `max`。所以LP64条件下 `min=5`、word5空可返回40；`max=0`，bit0忙bit1空可返回1。（`IDA90-E018..E020`，新候选 `IDA90-F01/F02`）
- 外层 `if (slot != ~1ULL)` 的字面判断不是“有空bit就扫描”的正确等价物；word的64位值为 `~1ULL`（只留bit0为0）时，反而会**跳过这个word**。（`IDA90-E019`，新 `IDA90-F03`）
- 找到一个空位立即 `return`，没有在位图标记占用；同一位图两次静态请求可返回同一数字。这是第89批已经存在的 `WQP89-F01`，本批只增加范围/消费者证据，不重复登记。（`IDA90-E020`）
- `ida_free(id)` 实际执行 `bitmap[id / sizeof(long)] &= (1 << (id % sizeof(long)))`；例如word二进制1011、id1得到0010，保留被释放bit并清掉其它bit，这是旧 `WQP89-F02`。ID超出可访问word的输入先在下标合法性门停止，不预言机器结果。（`IDA90-E022`）
- 位图空间申请NULL时 `initiated` 仍写true，后续数组读取前必须止证。这继承 `WQP89-F03` 的同根因，不另登记。扫描结束返回数值 `-1`，不等同于源码实际返回 `-ENOSPC`；对底层资源/线程行为仍不下结论。

由此区分四个不同事件：**识别bit空闲、给调用方返回数字、把位标记为占用、以后释放该占用**。当前代码对中间两项存在明确静态语义差异；从函数名“alloc”或“free”不能越过源码。

## G02/B｜IDR数组、范围、NULL槽和循环游标

`idr_init_base` 申请32768个指针元素，写 `idr_base=base`、`idr_next=0`、`initiated=true`；不检查实际分配NULL。`struct idr` 的 `idr_rt` 是 `void **`，`idr_base/idr_next` 是 `uint`。与历史注释中的radix tree不同，活动实现直接访问数组；源码中 `idr_base` 在实际 `idr_alloc_u32`、`idr_find`、`idr_replace`、`idr_remove` 没有作为数组索引偏移。（`IDA90-E023..E025/E028/E038..E040`）

### 范围和返回值

`idr_alloc_u32` 实际做的是：从 `uint id=*nextid` 赋给 `int slot_idx`，循环条件 `slot_idx<32768`，访问 `idr_rt[slot_idx]`；一旦槽是NULL，就写 `ptr`、写 `*nextid=slot_idx`、返回状态0；找不到返回 `-ENOSPC`。它接收但**没有实际使用** `max`，也没有利用 `base` 修正索引。对 `nextid>INT_MAX` 的有符号转换不作无限精度推断，遇到实现定义转换与下标合法性则止证。（`IDA90-E028..E031`）

`idr_alloc(start,end)` 在 `start<0` 时返回 `-EINVAL`；其它情况传递 `start` 和 `end>0?end-1:32768` 作为“max”，但底层不看max。假定 `start=5,end=6`，slot5非NULL、slot6=NULL，则可以返回6——超过注释中“end排他”的范围。`start=end=8` 且slot8空，也没有前置的“空区间”拒绝。此为新的本地范围候选 `IDA90-F05`，**不是**NULL预约旧 `PID11-F01` 的重复登记。`base=500,start=0` 的隔离例也可得到slot0，反映不同的偏移语义候选 `IDA90-F06`。

### 循环游标的实际控制流

`idr_alloc_cyclic` 首先读 `idr_next`；若 `(int)id < start` 才把id提升至start，调用相同的 `idr_alloc_u32`。若失败恰为 `-ENOSPC` 且当时 `id>start`，重置为start后**再试一次**。成功后 `idr_next=id+1`，失败时不执行这个更新。（`IDA90-E035..E037`）

例如cursor7、slot7已占、slot8空，得到8且next=9。cursor已到32768、start1时，首次因起点不在有效存储区而失败，可重试得到1。**但是**cursor在所请求end之外而仍在数组之内时，因为max被忽略，可在end之外成功，不能保证先回绕。游标的变化可能降低同一数值的短期碰撞机会，但不能把写NULL的空槽自动变成排他保留；必须与已有 `PID11-F01` 合读。

### NULL预约与对象发布

`ptr=NULL` 时，`idr_alloc_u32` 将空槽重新写为NULL，即便已返回成功也不能从槽内容区分“保留”与“未分配”。PID路径在取得号码后，**另一个阶段**用 `idr_replace(ptr=pid)` 发布PID对象。对非NULL payload 的成功插入，要区分指针写入的源码语句和实际可被并发读取的内存可见性；本任务没有进行完整锁/RCU审查。无论何种payload，`idr_alloc_u32` 均未消费max，范围问题独立存在。

## G03/C｜查找/替换/删除/遍历与两个限定消费者

### 直接数组API

| API | 固定time活动行为 | 有界结论/止证 |
|---|---|---|
| `idr_find(id)` | `return idr_rt[id]` | 合法槽可返回对象或NULL；越容量直接索引的风险已有 `CORE-USER-PID-LIFETIME-11::PID11-F02`，不重开 |
| `idr_replace(ptr,id)` | 读旧值、直接写ptr、返回旧值 | 不校验旧槽是已分配或原值匹配；返回NULL不是成功保留证书；与`PID11-F01` 合读 |
| `idr_remove(id)` | 读旧值、改NULL、返回旧值 | 重复移除第二次NULL；索引无上界检查的同根因按既有PID11限定 |
| `idr_for_each(fn,data)` | 旧radix循环注释，活动体直接 `return 0` | 对非空表亦不调用fn；“返回0”不是遍历了全部对象；新 `IDA90-F07` |
| `idr_get_next_ul(nextid)` | 设局部entry=NULL；历史扫描全注释；直接返回entry | 对非空表也返回NULL，未活动更新 `*nextid`；新 `IDA90-F08` |
| `idr_get_next(int*nextid)` | 调用前者，检查 `id>INT_MAX`，否则回写id并返回entry | 非负可表示输入通常回写相同id且返回NULL；负值转换的WARN条件需单独限制 |

定位：`IDA90-E038..E045`。这些是**本固定源码**的活动体，不是Linux历史注释所描述的抽象API。不能因为文档写了RCU和radix迭代，就把注释算法当作当前实际执行。

### 消费者一：worker池与worker编号

`worker_pool_assign_id` 用 `idr_alloc(&worker_pool_idr,pool,0,WORK_OFFQ_POOL_NONE,GFP_KERNEL)`；收到非负数字后写 `pool->id`、返回0，负值上交。该调用用非NULL池指针作为payload，但仍须遵守当前IDR范围实现和合法的内存对象前提。（`IDA90-E046/E047`）

`create_worker` 则走IDA，不是IDR：首先调用 `ida_alloc(&pool->worker_ida)`，若负数便直接返回NULL；只有取得ID后新建对象/线程的后续失败才转 `fail` 调用 `ida_free(id)` 和 `kfree`。旧89的未置占用、释放AND、条件NULL任务与发布时序保留原ID；不再分析89 manager工作队列整包，不把“已有编号”视作“线程已真正执行回调”。（`IDA90-E048/E049`）

### 消费者二：PID数字到PID对象

`alloc_pid` 对显式 `set_tid` 申请使用 `idr_alloc(&tmp->idr,NULL,tid,tid+1,GFP_ATOMIC)`；自动路径按游标选择 `pid_min=1` 或 `RESERVED_PIDS=300`，调用 `idr_alloc_cyclic(&tmp->idr,NULL,pid_min,pid_max,GFP_ATOMIC)`。随后设置 `pid->numbers[i]` 等前缀，在其它检查/初始化之后执行 `idr_replace(&upid->ns->idr,pid,upid->nr)`，才把PID对象指针放入映射。`find_pid_ns` 简单转发 `idr_find`；失败清理的 `out_free` 与 `free_pid` 有各自 `idr_remove` 消费。（`IDA90-E050..E057`）

因此**数字申请 → NULL槽写入 → PID对象发布 → 按数字裸查找 → 任务关联/引用管理 → 后续清理**不能合并为同一步。旧 `PID11-F01`（NULL不排他）和 `PID11-F02`（IDR查找上界）都在本批追加了条件证据，但没有重新编号或解除原11的任务生命周期限定。不能由这几处函数定义证明完整fork/exit成功或PID最终安全复用。

## 可读矩阵与有中间值的有限模型

对应完整结构化数据在 `facts-and-boundaries.yaml`。表中行数反映本批实际逐行的静态消费模型，而非动态测试数量。

| 结构化矩阵组 | 行数 | 代表行与主要证据 |
|---|---:|---|
| `ida_request_and_units_matrix` | 5 | `A-REQ01/03/04`，`IDA90-E011..E020` |
| `ida_scan_and_occupancy_matrix` | 5 | `A-SCAN01/02/03`，`E018..E021` |
| `ida_release_and_failure_matrix` | 5 | `A-REL01/02/04`，`E011/E022/E048/E049` |
| `idr_storage_and_range_matrix` | 6 | `B-STO01/03/04`，`E023/E029/E033` |
| `idr_slot_publication_matrix` | 5 | `B-PUB01/04/05`，`E030/E039` |
| `idr_cursor_and_retry_matrix` | 5 | `B-CUR01/02/05`，`E035..E037` |
| `idr_lookup_replace_remove_matrix` | 5 | `C-LRR01/02/04`，`E038..E040` |
| `idr_iteration_and_output_matrix` | 5 | `C-IT01/03/04`，`E041..E045` |
| `consumer_reservation_publication_matrix` | 6 | `C-CON01/03/05/06`，`E046..E055` |
| `ABC_join_matrix` | 4 | `ABC01..04`，跨A/B/C并保持旧编号 |
| **总计** | **51** | 10组；不是51项运行PASS |

### 三个带中间值的核心演算

**IDA请求范围（`IDA90-M03`）：** LP64且 `min=5,max=5,bitmap[5]=0` → `slot_idx=5` → `bit_idx=0` → 返回 `5×8+0=40`。这不涉及执行底层分配器，也不证实用户程序能到达这个输入；仅显示活动源代码把min当word下标。

**IDR游标回绕（`IDA90-M20`）：** 给定cursor32768、start1、slot1为空、存储合法 → 首次从32768扫描结果 `-ENOSPC` → 因 `id>start` 重试从1 → 返回1 → `idr_next=2`。若cursor12、end5且slot12为空，则 `IDA90-M22` 反而直接返回12，没有自动因end回绕，两个模型要合读。

**D00假设NULL任务（`IDA90-M30`）：** 给定有效worker/pool/attrs、普通池、kthread假设返回NULL、全部下级调用正常返回 → `IS_ERR(NULL)=false` → `set_user_nice` 不访问p → attach → `nr_workers:0→1` → `idle.count:0→1` → 跳过wake → `return worker`。该条件轨迹不能代替真实kthread返回值或可运行worker证据。

### 30个有限模型导航

| 模型 | 场景 | 关键条件性结果 |
|---|---|---|
| `IDA90-M01` | IDA零起点 | word0=0，取得0且word不变 |
| `IDA90-M02` | IDA min1 | LP64首word=1，取得8 |
| `IDA90-M03` | IDA min5 | LP64取得40 |
| `IDA90-M04` | IDA min4096 | 扫描不进入，返回-1 |
| `IDA90-M05` | IDA max0 | bit0忙bit1空，仍取得1 |
| `IDA90-M06` | 特殊word跳过 | `~1ULL` 下bit0空仍跳到word1 |
| `IDA90-M07` | 低8位已占 | 高56位未检，转下一word |
| `IDA90-M08` | 最后word | 得到32760 |
| `IDA90-M09` | 全扫描位忙 | 返回-1，不等价所有bit满 |
| `IDA90-M10` | 重复申请 | 两次0；已有WQP89-F01 |
| `IDA90-M11` | 释放AND | 1011→0010；已有WQP89-F02 |
| `IDA90-M12` | 释放越界 | 下标4096处停证 |
| `IDA90-M13` | 初始化NULL | initiated仍true；已有WQP89-F03 |
| `IDA90-M14` | base500 | 仍可取得0 |
| `IDA90-M15` | end=6 | 可能返回6超界 |
| `IDA90-M16` | start=end | 空区间也可取得8 |
| `IDA90-M17` | NULL重复保留 | 数字8两次；已有PID11-F01 |
| `IDA90-M18` | 非NULL顺延 | slot8忙则取得9 |
| `IDA90-M19` | cursor向前 | 取得8，next=9 |
| `IDA90-M20` | cursor回绕 | 首轮-ENOSPC后取得1，next=2 |
| `IDA90-M21` | 两次失败 | 返回-ENOSPC，cursor不改 |
| `IDA90-M22` | cursor超end | 得12，next=13 |
| `IDA90-M23` | 替换空槽 | 写P且返回旧NULL |
| `IDA90-M24` | 替换已有 | A→B，返回A |
| `IDA90-M25` | 重复remove | B→NULL→NULL |
| `IDA90-M26` | find超容量 | 32768索引前止证；PID11-F02 |
| `IDA90-M27` | for_each非空 | 仍return0，callback 0次 |
| `IDA90-M28` | get_next非空 | 仍NULL，nextid2不变 |
| `IDA90-M29` | PID预约/发布 | NULL写槽后独立replace为pid |
| `IDA90-M30` | D00假设NULL线程 | 可条件走到idle和return，跳过wake |

**解释边界：** 30个模型均是给定小整数与受限内存状态的纯文本演算，模型中引用的“已有问题”只增强旧项；结构化文件记录每个模型的中间值、相关短锚和第一个不能再推导的门，不能把模型的前提误记为真实机器初态。`facts-and-boundaries.yaml` 与正文保留同一10组/51行/30模型视图。

## 发现与延期的独立账目

新候选 `IDA90-F01..F08` 共8条：IDA的min坐标、max未消费、`~1ULL` 特殊word跳过、位图bits/sizeof计量差异；IDR的max/end未限制、base偏移未处理、`idr_for_each` 不执行遍历、`idr_get_next` 不取得下一个元素。这些是有条件的源码语义候选，尚未经Pro独立裁定，更不是8个实际运行故障。

新证据缺口 `IDA90-U01..U10` 共10条，分别约束ABI/转换、底层内存申请、旧注释算法、锁和竞争、IDA既有问题、base消费者、迭代消费者、PID全生命周期、kthread NULL真实可达、运行验证。与89/11历史问题的精确 qualified ID 去重关系见 `deferred-findings.yaml`；没有增加旧根因的新编号，没有新的修复代码、实验或闭合声明。

## G04与独立验收的停止边界

固定新目录只包含六件：本主件、`MANIFEST.md`、`facts-and-boundaries.yaml`、`id-allocation-lifecycle-paths.dot`、`deferred-findings.yaml` 和 `evidence.md`。`evidence.md` 包含59个本批单一定义短锚（包含D00），旧89的120和旧88的103仍分别保持历史自检数字，不进入本批新锚计数。文档里引用的老ID仅指向旧冻结对象而非修改对象。

**已实际完成的检查**应以MANIFEST及正式PR17交付评论中的后置远端记录为准；未执行的全部本地P9/运行/Graphviz/编译/内核/真实并发检查必须记 `NOT_RUN`。远端树、比较、文件blob、四direct refs、评论唯一性经核验才可宣称正式交付。作者上限：`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`；R89-01/02纠正交C06主线Pro审，不自行闭合89或通过90，不启动91。
