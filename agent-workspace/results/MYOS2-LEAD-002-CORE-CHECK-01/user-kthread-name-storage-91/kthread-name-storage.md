---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-KTHREAD-NAME-STORAGE-91
record_type: bounded_kthread_name_format_allocation_and_storage_report
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
executor_display_name: MYOS2-A-C06c 代码分析执行
produced_by: GPT-6 (visible consumer-model family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
date: 2026-11-16
base_snapshot: "time（分支名）"
read_channel: connector
status: AUTHOR_STATIC_READY_FOR_MAINLINE_REVIEW_AFTER_G04
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions: [KN91-U01, KN91-U02, KN91-U03, KN91-U04, KN91-U05, KN91-U06, KN91-U07, KN91-U08, KN91-U09]
---

# 第91批：worker名字格式化、动态全名与短名的静态生命周期

**交付边界。** 当前执行者MYOS2-A-C06c / CHAT-EXEC-12按主线合同129，在固定 `time@a039d9803ade` 源码上处理第90批两项文档纠正和A/B/C三专题。未构建、执行仓库程序、Graphviz、QEMU、实机或真实并发。名字产生、堆内存成功、任务对象发布、线程运行、业务回调与nice/CPU实际调度效果不能互相替代。固定第53、68批分别已覆盖create/done交接和首入口，不重做其主体。

## D00｜第90批原始清单和跨宏证据的定点纠正

### R90-01：原MANIFEST三个规范自检整数缺失

原90 `MANIFEST.md` 中 `self_check` 有 `evidence_anchor_tag_prefix_count_expected=59`、`evidence_anchor_literals_constructed_from_independently_fetched_fixed_source=59`、`evidence_anchor_constructor_mismatch_count=0`，**没有**协议v2 P6明确要求的 `verified_claims`、`quotes_reconfirmed`、`downgraded_to_inferred`。构造时找到引文、实际全包标签数、交付前独立重读及符号定义边界检查是不同的审查。PR评论或正文中的59/59/0不能替代MANIFEST本身的规范键。

本轮重新获取原90固定提交 `034cd9c52dbf` 六件原文，逐文件实际统计原始源码证据标签前缀：MANIFEST=0，主件=0，facts=0，DOT=0，deferred=0，evidence=59，合计**59**。本轮数字仅证明原标签的**字面出现次数**；不能倒造原90发布时的规范三整数、完整逐定义复核记录或实际机械P9，更不能以标签数证明所有引文合格。历史原作者构造计数59/59/0按原声明域冻结，**撤回“原59条均满足标注的单一定义边界”的总括**。原件和旧评论不改；91自身MANIFEST使用本批真实最终计数、真实交付前回源及规范三个整数，不将expected/constructor键冒充P6。

### R90-02：IDA90-E013须拆分三个宏定义

原 `IDA90-E013` 标签指向 `IDA_BITMAP_LONGS`，引文却包括前一行历史注释、`IDA_CHUNK_SIZE`、`IDA_BITMAP_LONGS`、`IDA_BITMAP_BITS` 三个独立宏。这不是合法的单一定义内引文。91从固定time新取四个各自的一行短锚：

| 原消费者所需事实 | 替代短锚 | 严格归属 |
|---|---|---|
| 原IDA90-E012数值32768 | KN91-E001 | `MYOS_IDR_BUF_SIZE=32768` |
| E013中chunk值 | KN91-E002 | `IDA_CHUNK_SIZE=MYOS_IDR_BUF_SIZE` |
| E013中longs计算 | KN91-E003 | `IDA_BITMAP_LONGS=IDA_CHUNK_SIZE/sizeof(long)` |
| E013中bits计算 | KN91-E004 | `IDA_BITMAP_BITS=IDA_BITMAP_LONGS*sizeof(long)*8` |

原常量末尾的“128 bytes”是历史注释，不能推翻活动替换式。受影响消费者映射：原90 `facts.source_ledger[mykernel/lib/idr/idr_const.h]`、`fact_to_evidence_ids.ida_units`；`ida_request_and_units_matrix` 的 `A-REQ01/A-REQ03`、`ida_scan_and_occupancy_matrix` 的 `A-SCAN03/A-SCAN04`、`ida_release_and_failure_matrix` 的 `A-REL02`；原 `IDA90-M04/M07/M08/M12` 各自的存储单位/word下标前提；原 `IDA90-F04/U01/U02`、主文G01/A容量段和DOT cluster_A容量/扫描节点。原E013仍存在于冻结文件，不能宣称删除；消费者改依据四个新独立短锚。该纠正不更改90的模型或旧根因，不意味着262144物理bit均能被实际IDA扫描。

**批次状态继续限定。** 89的R89-01/02已由C06另件关闭，组合PASS_PENDING_LOCAL，不重开。90仍RETURN，仅R90-01/02，作者D00状态 `CORRECTION_SUBMITTED_PENDING_C06_REVIEW`，待C06 Pro独立关闭。R90-S01/S02/S03仍有效：`idr_replace` 是先取旧值、写新值、再返回旧值；PID显式tid的slot7空与slot7忙/slot8空属于不同输入；旧90事实矩阵行ID不带IDA90前缀，但证据/模型ID保留它；LP64场景区分4096个long、32768个可扫描位置、262144物理bit；原M06/M07须令min=0，M08的实际结果是32760而非同轮返回32767。旧WQP89与PID11同根因和53已闭合误判保持原限定。

## G01/A｜worker实际格式、需求长度与缓冲容量

`mykernel/kactive/workqueue/workqueue.c::format_worker_id` 依据pool有效性和pool.cpu取三个格式，短锚KN91-E010..E013：

- pool非NULL且cpu≥0：`kworker/%d:%d%s`，整数分别来自`pool->cpu`和`worker->id`，最后`H`仅由`pool->attrs->nice<0`决定；这个字符**不是**调度优先级生效证明。
- pool非NULL且cpu<0：`kworker/u%d:%d`，整数来自`pool->id`及`worker->id`，并不读取nice来确定后缀。
- pool为NULL：常量 `kworker/dying`。这条回退分支的存在不证明实际 `create_worker(NULL)` 可安全到达。

`WQ_NAME_LEN=32` 和 `WORKER_ID_LEN=10+WQ_NAME_LEN=42` 各有自己的枚举内部证据（KN91-E081/E005）。`create_worker` 非BH路径定义了`char id_buf[42]`；随后调用`format_worker_id(id_buf,sizeof(id_buf),worker,pool)`，**未消费返回值**，再把此栈字符串以`"%s"`传入`kthread_create_on_node`（E014/E015）。格式化完成不等于申请了持久全名。

本包按有前提的32位int十进制模型：普通格式最坏`8+10+1+11+1=31`字符，unbound格式最坏`9+11+1+11=32`字符；dying字面为13字符。因此42字节缓冲可容纳三种格式在该模型中的名字及终止符。它不是在真实编译ABI/运行路径上验证过的长度结论。不能把“给格式接口任意更小size可能截断”的例子，升级为实际worker发生截断的发现。

`vsnprintf` 源体先检查size>INT_MAX，再设置`str=buf`、`end=buf+size`；每次 `format_decode` 后，文字路径即使只写了容量内的字符也推进逻辑`str`，`number`与`string`同样按需求推进；函数在size>0时于`str`或`end[-1]`设置NUL，返回`str-buf`（E019..E028）。这是**需求字符数**，不包含终止符，不等于实际写入字符数。`vscnprintf` 调用它后，若`i<size`返回i，否则在size>0时返回size-1，size0返回0；`scnprintf`为变参包装（E016..E018）。字符串消费须有可读、NUL终止或有效precision范围；`%d`须是适当的int实参和有定义的数值转写（E029..E036）。

在合法目的缓冲与字面`"kworker/dying"`需求13的条件模型里，size13写12字节加NUL并返回12，size14写完整13加NUL并返回13，size1仅写NUL并返回0。size0时`vscnprintf`最终返回0，但其内部依旧调用`vsnprintf`，后者会做指针加法及`str-buf`；**不能**因为size0没有输出字节就断言`vsnprintf(NULL,0,fmt,ap)`在所有C实现下都具有定义良好的指针语义。此处作为B专题的第一道止证门，不用源码“遵循C99”的注释替代核验。

## G02/B｜kvasprintf两遍计算、分配和输入稳定性

固定`mykernel/lib/printf/kasprintf.c::kvasprintf`的实际顺序（KN91-E037..E045）为：
1. `unsigned int first,second`；`va_copy(aq,ap)`，用 `first=vsnprintf(NULL,0,fmt,aq)` 计算首遍长度，随后`va_end(aq)`。
2. `p=kmalloc(first+1,gfp)`；若`p==NULL`直接返回NULL，不进行第二遍。
3. `second=vsnprintf(p,first+1,fmt,ap)`，源码中的`WARN(first!=second,...)`只是注释，当前没有活动比较、重分配或不一致报错，最终直接`return p`。

`va_copy`复制的是可变参数消费状态，而**不是**对`%s`所指字符数组做深拷贝。设两次format/va_list有效且每次格式化成功：`"abc"→"abc"`可得first3/alloc4/second3；`"abc"→"abcdefg"`可得first3/alloc4/second7，第二遍最多把三个字符和NUL放入原4字节分配，并不会因为需求长度变大自动重新分配；`"abcdefg"→"abc"`可得first7/alloc8/second3，字符串有效但额外字节不因本函数而全清零。两遍不一致缺少本地检查，不等于已经发生越界或真实并发竞态。

异常计数还需保留符号和转换门：第一遍`vsnprintf`返回类型是int，而`first`为unsigned；在另行假设首遍产生负数或超大值时，转换及`first+1`可能不满足所期待的正容量。未证明这些异常返回真实发生，也未全面审查`kmalloc(0)`结果，不能把抽象的溢出条件当实际故障。第一遍`NULL,0`的C指针计算问题也留在`KN91-U01`。本包只核实际`%s/%d`和相邻辅助函数，不审全套printf扩展、浮点、%p或所有类型变化。

### kthread名字分配失败≠task为NULL

`__kthread_create_on_node`先为请求结构`create`分配内存，失败直接返回`ERR_PTR(-ENOMEM)`；在请求有效后，将`kvasprintf`结果赋给`create->full_name`。如果此结果为NULL，明确设`task=ERR_PTR(-ENOMEM)`、跳到`free_create`释放请求结构，并把错误指针返回（E046..E050）。这是在排队之前的局部失败，不是“kthread包装返回NULL task”或“线程已创建但名称为空”的已证路径。名字成功、请求入队、唤醒kthreadd、completion返回、真正任务对象成立和业务回调开始必须逐段分别证明。53的done交接和68的线程首入口已经限定，本包不重做。

## G03/C｜全名、短名和释放的不同存储对象

| 对象 | 生产者/最后已见使用 | 所有权与止证门 |
|---|---|---|
| ① `id_buf` | `create_worker`栈上42字节缓冲，格式化后作为"%s"入参 | 不是堆全名，要求同步消费时栈仍有效 |
| ② `kvasprintf`所得`p` | `kmalloc(first+1)`产生独立名字字符串 | `create->full_name=p`只保存指针 |
| ③ `kthd_create_info_s create` | 另一次`kmalloc(sizeof(create))`产生，含name、done、result | creator的`kfree(create)`不自动释放其所指字符串 |
| ④ `kthread_s self` | `set_kthread_struct`经`kzalloc`并赋`task.worker_private` | 正常kthread路径的`self->full_name=create->full_name`为浅转移，非深拷贝 |
| ⑤ `task.comm` | `task_struct`内固定`char[32]`；fork在`args.name`非NULL时写入 | 由`strscpy_pad`限制，至多31字符+NUL |
| ⑥ `get_kthread_comm`目标buf | 由调用方给定空间与size | 选择私有全名或task.comm短名复制，不拥有来源的寿命 |

`create_kthread`用`kernel_thread(kthread,create,create->full_name,...)`把全名指针传入（KN91-E064）；`kernel_thread`在clone参数中赋`args.name=name`（E066）；`copy_process`调用`strscpy_pad(p->comm,args->name,sizeof(p->comm))`且忽略其返回（E067）。`TASK_COMM_LEN=32`，`task.comm`真是固定数组（E006/E070）。在合法、不重叠输入下，31字符加NUL恰好装入；32字符或更长则短名至多前31字符和NUL、内部strscpy可回`-E2BIG`，但`copy_process`不传播该拷贝结果。这是短名容量与返回值消费差异，并不说明私有堆`full_name`已缩短、任务必然创建失败或真实内存越界。

### 读回与补零

`get_kthread_comm`的返回类型是**void**。若`to_kthread(tsk)`得不到私有结构或私有`full_name`为NULL，调用`__get_task_comm`拷贝`task.comm`；否则直接`strscpy_pad(buf,kthread->full_name,buf_size)`，**不把返回的字节数或-E2BIG转给调用者**（E053..E055）。它不能与返回`char *buf`的`__get_task_comm`混成同一返回契约。`to_kthread`中的WARN不是自动拒绝/早退门，task/private实际有效性仍需验证。

`strscpy`对count0或过大返回-E2BIG；有效count>0时逐字节复制直至NUL，写满没有NUL时将最后一字节置NUL并返回-E2BIG（E073..E075）。`strscpy_pad`先调用它，若错误或恰好只剩终止位置就返回，否则以memset清余下字节（E071/E072）。条件例：dest长度5、src="A"，得到`['A',0,0,0,0]`和直接pad返回1；src="ABCDEFGH"得到`"ABCD"`+NUL、直接pad返回-E2BIG；通过void的get_kthread_comm调用时这些返回值不能从该接口取得。源/目标无效、缓冲重叠、无NUL前提时不能继续正常推演。

`__get_task_comm`加`task_lock`后复制`task.comm`并解锁，返回buf；`__set_task_comm`也加锁复制，但忽略pad返回（E076..E078）。固定`task.h`把`task_lock/unlock`分别映射到`spin_lock/unlock(&p->alloc_lock)`（E079/E080），证明这里只提出了锁调用，不能替代完整多核同步或私有`full_name`的引用寿命论证。尤其`get_kthread_comm`全名分支本局部并没有对应的`task_lock`动作。

### 成功指针转移、失败释放与回收调用边界

`kthread`中`xchg(&create->done,NULL)`若取得NULL，则有释放`create->full_name`及create、随后退出的分支（E061）；选定正常create路径中killable取消段被注释，不能宣布该路径实际发生。若done有效，则`self->full_name=create->full_name`，随后`create->result=current; complete(done)`，通知了请求者后它可能释放create；**这没有释放full_name，也不能证明业务threadfn已经运行**（E062/E063，R53-S01/S02，R68-S01/S02/S03）。

`create_kthread`中的`kernel_thread`若返回负PID，会按该失败分支`kfree(create->full_name)`，在done有效条件下`result=ERR_PTR(pid)`并`complete(done)`；名字释放、错误结果、请求交接是不同事件（E064/E065）。`free_kthread_struct`如果实际被调用且kthread有效，会先置`worker_private=NULL`，再`kfree(kthread->full_name)`及`kfree(kthread)`（E059/E060）。但是固定`fork.c::free_task`里对应的直接调用为**注释**（E069）；既未核实全部其它调用者，也没审退出全过程，故不能宣称“全名必释放”或“实际已经泄漏/UAF”。

第53批R53C-S01已经认定主线关于CO53-A03/B03空行的指控错误并撤回，原R53-01关闭、组合PASS_PENDING_LOCAL；本91不得恢复旧假例。第68批保留R68-S01/S02/S03关于首帧、done与退出的不完全性。原第89批R89-02已关闭且set_user_nice的活动体没有实际nice更新，名称中出现H不能升级为CPU优先级/亲和性生效证明。

## 结构化矩阵、有限模型与新问题

机器可消费的`facts-and-boundaries.yaml`包含**9组49行**的静态条件矩阵：`format_count_capacity_matrix`5、`worker_name_input_and_truncation_matrix`5、`two_pass_format_allocation_matrix`6、`varargs_and_input_stability_matrix`5、`name_failure_result_matrix`5、`name_object_ownership_matrix`7、`name_readback_and_padding_matrix`6、`failure_and_release_prefix_matrix`6、`ABC_join_matrix`4。每行有前置输入、可见源阶段、条件结论、首个止证门、对应KN91-E锚。

**22个有限模型**KN91-M01..M22均为纯文本数值和状态推演，含size0/1/13/14、CPU与unbound数字格式、极限int32假设、两遍字符串稳定/增长/缩短、NULL名字变ERR_PTR、短名31/32字符、pad截断补零、正常浅转移与fork失败清理、短名fallback，以及D00的LP64容量算例。有限模型不是已执行的本仓库单元测试，也不是线程调度或动态故障复现。

`deferred-findings.yaml`新增四个有条件候选KN91-F01..F04：两遍内容变化未检查、first unsigned/first+1边界、get_kthread_comm丢失截断返回、task.comm截断结果未向上传播；九个KN91-U01..U09缺口涉及NULL零尺寸指针算术、数字辅助器、kmalloc、输入稳定、旧53/68可达性、实际free调用、full_name同步、短名锁/对象，以及实际nice/CPU效果。未注册同根因新ID，不将这些条件直接称作4个真实bug。旧CO53、KT68、WQP89及90记录的qualified IDs继续有效。

## G04交付与停止条件

只允许新91专属目录的`MANIFEST.md`、`kthread-name-storage.md`、`facts-and-boundaries.yaml`、`kthread-name-storage-paths.dot`、`deferred-findings.yaml`、`evidence.md`六件，以及本人一次PR17顶层正式评论。91新源码短锚编号KN91-E001..E081；它们与旧90标签59分别计数，四宏各独立一锚。MANIFEST必须具有P6三个规范整数，最终以本批六件真实原始前缀、交付前重新回源和降级数为准；全部已知失格锚在发布前处理，不把构造器计数冒充真实规范字段。

仅在旧exec HEAD、91目录/旧新marker及PR16/17再次通过，且compare严格6件added、活动与固定文件blob一致、四direct refs无未知推进、唯一正式评论远端回读成功后，作者报告`READY_FOR_MAINLINE_REVIEW`，`mainline_acceptance=PENDING`，`STOP_WRITING`。完整本地P9、编译、仓库程序、Graphviz、QEMU、实机和真实并发`NOT_RUN`；没有内核补丁、额外模型、子代理或付费API。第90批两项纠正由原C06主线Pro审核闭合，本人不自签90通过、91验收或开启92。
