---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89-REVIEW-001
record_type: independent_bounded_static_review_with_targeted_return
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收安排）
date: 2026-11-15
base_snapshot: time
read_channel: connector
evidence_class: 六件全文及固定源码的独立可读复核，非运行或完整机械验收
source_pin_short12: a039d9803ade
contract126_short12: ed9cd00830d9
reviewed_base_short12: e9f1ad989ea9
reviewed_head_short12: 42897a06feb1
reviewed_delivery_comment: 6893704757
inputs_read: [126及88-review的本有效C06固定对象, 110/124及适用项目规则, 89六件全文及正式评论, 固定time近邻源，详见同轮检查点]
status: RETURN
acceptance_verdict: RETURN
returned_items: [R89-01, R89-02]
return_scope: TARGETED_EVIDENCE_AND_LOCAL_ANALYSIS_CORRECTION_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_EXCEPT_RETURNED_CONSUMERS_WITH_R89_S01_S02_S03
mandatory_supplements: [R89-S01, R89-S02, R89-S03, R88-S01, R88-S02, R88-S03, R87-S01, R87-S02, R87-S03]
R88_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_SAME_ROUND_RECEIPT
batch88_composite: PASS_PENDING_LOCAL
original_artifacts_modified: false
original_author_self_check: [120, 120, 0]
original_readable_raw_prefixes: 120
self_check: {verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, scope: 本件十条固定源码短锚的可读回核，不是原120项机械全检}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-ID-ALLOCATION-LIFECYCLE-90
next_contract: ../127-user-id-allocation-lifecycle-contract.md
executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 89审核完成：两项定点补正；88文档项另件关闭

89为RETURN，仅R89-01跨宏证据和R89-02近邻实现遗漏/NULL任务模型停止点纠正。第二项涉及局部分析，不只是排版。其余技术主干按R89-S01/S02/S03保留，不重做工作队列整包。90先追加纠正，再研究编号接口；普通源码风险仍只分类收存，不授权修内核。

## 1. 交付与验收范围

正式评论6893704757创建/更新时间均为2026-11-15T11:34:58Z。PR17与exec直接头为42897a06feb1；相对e9f1ad989ea9为ahead1/behind0、一个提交、六件added5661行：MANIFEST53、正文213、facts3361、DOT121、deferred379、evidence1534，旧件零净改删。初核lead ed9cd00830d9、master直接e8e60cd19540、time a039d9803ade；PR16/17均Open/unmerged，17仍Draft。

六件和评论本轮独立全文读取，长响应补齐到末尾。原120个编号锚均位于evidence，其他五件可读未见额外原始前缀；120/120/0与出现数量对应，但不能认证边界合格。十四字段、公约原句、双分支金丝雀和read_channel齐备；六件可读未见40位十六进制串，不冒称正则零命中已执行。20个源入口、十矩阵54行、26条件例、七F九U是不同维度，不合计为运行实证。

P9-2/13存在已知FAIL：E010跨定义，合同要求的set_user_nice最近实现未取得而影响模型。P9-1/3/4/5/7/8/11按可读范围核对，局部语义须按本回执；6未新增外部标准认证，数值例只在声明前提下解释；9/10/12无代码/补丁/降级交付，N/A。完整本地P9、原件机械扫描/解析和模型程序均未完成，不能签PASS。

原88的E011/E039由89/D00正确替换并映射，另件关闭R88-01，88组合PASS_PENDING_LOCAL；这一闭合与89自身裁定分开，不重开R86-01等旧文档事项。

## 2. R89-01：E010必须按两个真实宏分别锚定

WQP89-E010标for_each_possible_cpu，却同时引用下一行for_each_online_cpu。两行各是完整单行宏，不是同一个宏体，字面均存在也不能通过P2。

Q1：
[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_possible_cpu]
```c
	#define for_each_possible_cpu(cpu)	for_each_cpu((cpu))
```

Q2：
[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_online_cpu]
```c
	#define for_each_online_cpu(cpu)	for_each_cpu((cpu))
```

Q3是共同下级，不能由宏名推导真实CPU集合：
[VERIFIED mykernel/include/linux/kernel/cpumask.h::for_each_cpu]
```c
	#define for_each_cpu(cpu) \
				for (int cpu = 0; cpu < nr_cpu_ids; cpu++)
```

90/D00回查原E010和全部消费，新增两个独立锚及必要下级支持，映射原facts锚表、CPU枚举矩阵、M01–M03/M08等实际消费者。撤回原“120条均在所标单一定义内部”的总括；保留原120/120/0、字面回读历史、六件及评论不动，不改成119/119/0或倒造过去的语义核验。新90连同D00独立计数。

## 3. R89-02：NULL任务不能在未核的set_user_nice处结束推导

原M15、相应NULL任务矩阵/文字和WQP89-F05把IS_ERR(NULL)未拦截后的证明停在set_user_nice，未取得合同要求的最近实现。主线已在固定time完整读取mykernel/sched/runqueue/runqueue.c：函数仅有未使用局部声明，所有实际优先级和任务访问语句均被注释。不能将未知实现继续保留为已定位的NULL任务解引用点。

Q4为实际定义开头，不是外部原型：
[VERIFIED mykernel/sched/runqueue/runqueue.c::set_user_nice]
```c
void set_user_nice(task_s *p, long nice)
{
	bool queued, running;
	rq_s *rq;
	int old_prio;
```

Q5为其末尾；“无活动调度语句”的判断依据是本轮完整函数阅读，不是只凭两行注释证明全部函数：
[VERIFIED mykernel/sched/runqueue/runqueue.c::set_user_nice]
```c
	// p->sched_class->prio_changed(rq, p, old_prio);
}
```

Q6说明进入该空操作实现前，仍须合法求值pool/attrs/nice；不能因不访问p就忽略实参表达式：
[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
		set_user_nice(worker->task, pool->attrs->nice);
```

在明确假设kthread包装返回NULL、IS_ERR按所读错误指针约定判断为假、worker/pool/attrs及后续列表/锁调用均合法正常返回且无其它写者的局部模型中，set_user_nice不会因p为NULL而解引用它。源码可以继续挂接、增加nr_workers、进入idle；Q7随后跳过唤醒并在后续返回worker：

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
	if (worker->task)
		wake_up_process(worker->task);
	spin_unlock_irq(&pool->lock);
```

这仅补齐假设路径，不证明kthread真实可返回NULL，不证明线程存在或已运行，也不将普通worker无task的假设等同BH设计分支。实际kthread结果生产/完成协议仍为U05等未证门，不在本轮重审全线程系统。

90/D00必须新增实际set_user_nice证据、纠正M15和所有同义消费者，给旧→新映射；WQP89-F05保留旧ID但收窄为需要另证NULL返回可达性的调用方契约候选，撤回任何将set_user_nice当作确定非法访问点或必然故障的读法。U08的nice效果据此限定：本固定函数不执行调度优先级更新；name格式化使用nice不等于优先级已生效。其它“正确线程对象”轨迹不因此失效。

## 4. R89-S01：初始化、资源前缀和图示

池初始化关于显式写字段、依赖先前清零及部分失败前缀的主体成立。M04非零旧池反例违反init_worker_pool所述newly-zalloc输入前提，只说明函数不做完整重置，不凭此签正常启动缺陷。flags的OR与nr_running/nr_workers/manager保持旧值分别判断；静态模板零值不自动证明真实perCPU实例合法。

possible/online在所读宏里同为稠密循环，M01–M03只在稳定N、可表示索引、对应实例有效且初始化调用返回时成立。N=0是隔离边界输入，不认证真实启动零CPU。M06“ret=7”应指worker_pool_assign_id内的局部idr返回；该函数成功向调用方返回0，不是7。BUG_ON/错误值、panic调用和已观察到的停止不能混为一层。

Q8给出ID申请失败的直接出口，不得让DOT的统一fail节点把它画成已取得资源后的归还：
[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
	if (id < 0) {
		pr_err_once("workqueue: Failed to allocate a worker ID: %pe\n",
			    ERR_PTR(id));
		return NULL;
	}
```

Q9才是worker分配或线程错误指针等后续失败进入的清理段：
[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
fail:
	ida_free(&pool->worker_ida, id);
	kfree(worker);
	return NULL;
```

日志、ida_free和kfree都须合法返回才到相应return；调用了归还不等于编号位图确已正确释放。M13/M14按此保留，不能以局部kfree证明所有线程资源回收。

B-PUB04的“WARN前置校验”不准确：worker_enter_idle先设标志、加入idle列表，再执行以下丢弃返回值的表达式。它不是插入前的拒绝门，也不认证日志输出：
[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_enter_idle]
```c
			WARN_ON_ONCE(pool->nr_workers ==
				pool->idle_list.count && pool->nr_running);
```

DOT的alloc_worker到线程创建、init_worker_pool到分配pool ID是返回到各自调用者后的接续，不是两个被调函数彼此直接调用。挂接列表先于worker->pool赋值的源序可保留，但没有并发观察者/锁证据不推实际竞态。

## 5. R89-S02：IDA模型、错误指针与既有编号

已完整回读idr.c及idr.h/const/types。IDA分配返回前未置占用位、ida_free执行按目标位与运算，支持WQP89-F01/F02的条件局部结论。M09以零位图、包装min=0及无其它写者为前提可重复选0；它不是两个真实worker已重号。M12在声明sizeof(long)=8、目标word值11、id=1时得到2；这个数值不是“只清目标位”的正确释放结果，也不代表运行复现。

字节数、每字位数、实际只扫描的位数及bitmap长度必须分开。M11的4096字取决于32768字节除以8的模型，不由常量旁历史注释“128 bytes”认证。E077把ida_alloc称为macro是种类描述错误，它是实际inline函数；调用路线不变。min/max、slot上界、移位类型与IDR游标的其余边界留给90，不把本89局部两例升级成整个编号器已审完。

IS_ERR/PTR_ERR按固定err.h实际包装及目标指针/整数前提解释，不把NULL、错误指针和正常对象合为一类。kzalloc_node忽略node的包装事实不等于已审全部供应器清零；BH的task初值仍需真实分配返回和初始化前提。

F03的分配失败后initiated置位保留条件身份；未证NULL分配可达时不报实测故障。F06是复用前提问题，F07是发布源序问题；与各U相互依赖，不把七F九U说成十六个独立根因。90须保留原qualified_id。

## 6. R89-S03：管理进展、阶段和历史去重

manage_workers已有manager时返回false不等于创建失败；取得管理身份后，只有maybe_create_worker及后继状态写入返回才清理身份并返回true。成功创建、已有idle和需求消失都可能使重试结束，但必须分别核内层检查与重新取锁后的外层检查。

M20持续失败且需求不变只证明所读源没有活动cooldown及有限退出保证；create_worker内部可能等待、打印或分配，不能仅据未调用msleep认定CPU必然紧密忙等。M21需求在内层消失，还须在外层复查时仍为假才能结束。M25/M26的idle_count必须给在worker_leave_idle之后的决策点；不能把包含当前worker的入口计数直接带入后续may_start_working。

M16/M17等固定计数结果以没有其它worker插入/离开和所有callee合法返回为门；唤醒请求不保证新线程已运行。源码两个workqueue启动调用是注释，模型可分析假设有效输入，但不声称当前正常启动进入了它们。

下一90为编号分配/登记的增量专题。已定点合读旧11-review及2c410132e42d的11清单/延期原件：NULL占号与无界查询已有CORE-USER-PID-LIFETIME-11::PID11-F01/F02，不能在90另造同根因。旧11接口文档退回的历史文本不用于反向改写后来状态；本轮不重审或重开11。90的PID片段只验证编号接口消费，不重做任务引用、kill、exit或namespace全生命周期。

本轮按126独立审核，未运行仓库程序、构建、内核或真实并发。临时Python原文镜像探测在DNS阶段失败，未取得可扫描输入；不把失败探测说成机械检查完成。上述十锚可读回核，完整P9仍NOT_RUN。90继续现C06b/11以次一档执行D00后G01–G04，交本C06下一轮Pro一并复核，不为两项补正单独增加Pro回合。
