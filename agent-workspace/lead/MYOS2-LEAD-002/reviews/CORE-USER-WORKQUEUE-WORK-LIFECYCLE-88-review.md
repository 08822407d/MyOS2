---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88-REVIEW-001
record_type: independent_static_review_with_documentation_only_return
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner正式验收安排）
date: 2026-11-15
base_snapshot: time
read_channel: connector
evidence_class: 六件全文、固定源码和远端净差异的可读独立复核；非机械全检或运行认证
source_pin_short12: a039d9803ade
contract125_short12: cf2377d7c580
reviewed_base_short12: bc611665b38f
reviewed_head_short12: e9f1ad989ea9
reviewed_delivery_comment: 6889090097
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/125-user-workqueue-work-lifecycle-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-ALLOCATION-87-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-DOC-CLOSURE-review.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/workqueue-work-lifecycle.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/workqueue-work-lifecycle-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-workqueue-work-lifecycle-88/evidence.md
status: RETURN
acceptance_verdict: RETURN
returned_items: [R88-01]
return_scope: DOCUMENTATION_ONLY_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_WITH_R88_S01_S02_S03_PENDING_DOCUMENTATION_CLOSURE
mandatory_supplements: [R88-S01, R88-S02, R88-S03, R87-S01, R87-S02, R87-S03, R86-S01, R86-S02, R86-S03]
new_D00_required: true
original_author_self_check: [103, 103, 0]
original_readable_raw_prefixes: 103
original_artifacts_modified: false
self_check: {verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, scope: 本件十条固定源码短锚的可读回核，不是原103条机械扫描}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-WORKQUEUE-POOL-BOOTSTRAP-89
next_contract: ../126-user-workqueue-pool-bootstrap-contract.md
executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 88审核完成：仅证据文档补正，不重做技术整包

独立裁定RETURN，仅R88-01：E011引文跨出所标类型定义，E039枚举名标错。源码片段真实存在不等于满足P2的单一定义锚定。技术主干按R88-S01/S02/S03保留；89先追加定点纠正，再继续新专题。此时不能称88已PASS_PENDING_LOCAL，也不要求Owner逐项审源码或修内核。

## 1. 实际交付与可读闸门

正式评论6889090097创建/更新时间均为2026-11-15T04:20:41Z；执行direct ref与PR17均为e9f1ad989ea9。compare父bc611665b38f至该头为ahead1/behind0、一个提交，严格新增六件5321行：MANIFEST58、正文216、facts3259、DOT105、deferred359、evidence1324；旧件零净修改删除。Owner恢复核验不构成本轮重复执行或重复发布的理由。

初核lead cf2377d7c580、master直接e8e60cd19540、time a039d9803ade；PR16/17 Open/unmerged，17仍Draft。两PR导航仍是上轮发布88时的状态，不能用其中“88未启动”覆盖已取得的后置交付事实。

六件及正式评论完整读取，正文和facts长响应缺段通过重叠分段补齐。103个原始标签均在evidence，其余五件可读未见额外同前缀；原三整数103/103/0与标签出现数量相符，但不证明103项都在所标定义内部。MANIFEST十四字段、公约整句、双分支金丝雀、read_channel存在；固定公约和canary按本有效C06此前实际读取的同一对象合读。六件可读未见40位十六进制串，工具JSON和正式评论不混入该范围。

| P9范围 | 本轮结论 |
|---|---|
| 1/3/4/5/7/8/11 | 可读范围未见新增硬失败；路径、数量、字段及未运行说明与所读原件对应 |
| 2/13 | FAIL：E011定义边界、E039符号身份不满足125§6及P2；强制近邻支持另见S02 |
| 6 | 本轮不新增外部标准/硬件结论；原数值模型按声明前提消费，不认证实际编译环境 |
| 9/10/12 | 无代码交付、补丁、降级件，N/A；不是对内核全树符号或可构建性的认证 |
| 完整机械P9及运行 | NOT_RUN；不可签PASS |

取证结合本轮新读与本C06自己此前亲自读到的固定time完整相关函数、宏和结构，不继承执行者读账。作者17源入口（15有锚、2上下文）、十矩阵62行、26条件模型、七候选九缺口属于不同统计维度，不相加为运行实验或已确认故障。103次作者回源申报不等于本轮重演了其全部工具过程。

## 2. R88-01：单一定义边界与真实符号名

| 原锚 | 原问题 | 89/D00必须追加的对应纠正 |
|---|---|---|
| WQ88-E011 | 标work_func_t，但三行中前两行是work_struct声明及另一个work_s typedef | 只以work_func_t自身的一行作替代引文；需要work_s别名时另锚，不串入同一定义 |
| WQ88-E039 | 标workqueue_const.h::pool_flags；真实外层枚举为worker_pool_flags | 使用真实枚举名及其内部1–5行；同步说明facts锚表、模型/矩阵/候选的消费映射 |

Q1是E011可用的单定义内部原文，不包含前两条声明：

[VERIFIED mykernel/kactive/kactive_type_declaration.h::work_func_t]
```c
    typedef void (*work_func_t)(work_s *work);
```

Q2所在定义为worker_pool_flags；不得另造pool_flags定义：

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::worker_pool_flags]
```c
		POOL_BH					= 1 << 0,		/* is a BH pool */
		POOL_MANAGER_ACTIVE		= 1 << 1,		/* being managed */
		POOL_DISASSOCIATED		= 1 << 2,		/* cpu can't serve workers */
		POOL_BH_DRAINING		= 1 << 3,		/* draining after CPU offline */
```

89/D00须独立回查原件，在89新六件中建立旧锚→新锚→受影响事实/矩阵/模型/候选的映射，撤回原“103条全部位于真实单一定义内部”的总括。保留原103/103/0和作者字面匹配日志，不把历史改成101/101/0，不把本轮替代引文倒填回88；新89按其自身原文（含D00）重新计数。执行者只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW，不自签R88-01闭合。

本退回不否定回调类型、实际pool位值或整个提交/回调分析，也不要求重跑88。主线本件给出可用限定并不自动代替执行者的追加纠正交付。

## 3. R88-S01：返回值、条件模型与列表计数

普通返回true、入链、发出唤醒请求、任务获得CPU、实际回调是不同事件。原主要分层判断保留。E005标题提void但引文从函数名开始，补Q3避免靠标题补返回类型：

[VERIFIED mykernel/kactive/workqueue/workqueue.c::__simple_queue_work]
```c
static void
__simple_queue_work(int cpu, workqueue_s *wq, work_s*work) {
```

M01仅给entry/pending/queue_count/cpu及“调用均返回”，不能独立推得kick=false；需明确再选nr_running非零或无idle等分支前提。可把M13的有效列表、插入后count1、nr_running1、first_idle_worker正常返回作为同类例。woken=false、callback=false只能表示所列局部调用未直接发生该事件，不能证明其它CPU/已有worker没有执行；未建立全局轨迹时应消费为NOT_PROVEN而不是全局否定。

M04的pending=1是给定输入；当前提交路径没有活动的置位/清位，并非连输入也未知。若声明没有其它写者，可保留局部值不变；有并发则值另证。M08的count0是独立kick输入，不是向原空表合法插入后立即仍count0的见证。M10步骤顺序应先first_idle_worker，再need_more_worker；相同真假结果不使错误源序成为真实调用顺序。

C-T06初态写count=1/2，结论却把处理一个后都说成空表：准确是1→0、2→1；第二种还需再正常移除剩余项且无新增才能到空。PWQ读取和strscpy在删除前，回调在删除、kick及解锁之后，不能把回调成功当成删除之前的源序门。M17的一项终止例须保留合法删除、先行kick返回、回调正常返回、无新入队和worker/pool存活条件。

list_empty仅检查next是否指回自身，不能单独证明prev、表头count和全链一致；把它称为“已链接/脱离”要有有效双向循环链前提。M15等计数转移需实际contains成功，不能只凭delete函数返回了就认定删中。Q4补齐自链接语义的近邻：

[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
```

其下级验证和初始化都需合法返回；不能由这四行跨过损坏链对象。DOT的work_count>0、正文“非空”在有效非负count不变量下相容；真实谓词是count!=0，不应对任意异常负数也替换为大于零。nr_running是int，增减模型需在可表示范围内，不是实际运行线程数测量。

正文26例场景列全部N/A是导航缺失，不表示26种输入均不适用；场景须以facts.finite_models对应inputs/preconditions为准，不把无输入的摘要行单独当完整模型。该导航问题由本限定指出，不另造整批返工。

## 4. R88-S02：CPU近邻、PWQ解码与回调门

原U03仍保留实际CPU/GS/对象条件，但“最近定义未展开”不等于没有可读实现。主线补Q5/Q6；这是主线新增的有界核验，不冒称88作者已经交付此支持链：

[VERIFIED mykernel/include/linux/smp/smp.h::smp_processor_id]
```c
	#define smp_processor_id()	__this_cpu_read(pcpu_hot.cpu_number)
```

[VERIFIED mykernel/arch/x86_64/include/asm/smp.h::__this_cpu_read]
```c
		#define __this_cpu_read(pcp)	\
					(*this_cpu_ptr(&(pcp)))
```

结合本轮全文读取的asm/percpu.h，this_cpu_ptr经arch_raw_cpu_ptr使用rdgsbase得到的基值与符号地址相加后解引用；不是直接读取APIC-ID，也不是由函数名保证CPU编号有效。沿R87-S02区分GS值G与offset数组O_i，G=O_i仍需另证。M05显式CPU1例还需CPU1不等于当前声明的哨兵并且索引有效，不由nr_cpu_ids=2独自确定NR_CPUS常量。

atomic_long_read→arch_atomic_long_read→arch_atomic64_read的实际宏/定义已核，其最后读取counter；不由atomic名字额外推导PWQ生命周期、发布顺序或完整内存屏障。WARN两个宏是在所读CONFIG_BUG门内的直接条件表达式；配置面有该定义请求，实际编译命令和处理器行为未认证。单独丢弃WARN表达式的结果不等于if检查后返回，也不证明有打印。

PWQ掩码模型须指定CONFIG_DEBUG_OBJECTS_WORK，而非把它与Debug构建名混为一谈。Q7提供位数来源：

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::work_bits]
```c
		WORK_STRUCT_COLOR_SHIFT		= WORK_STRUCT_FLAG_BITS,
		WORK_STRUCT_COLOR_BITS		= 4,
```

在四个flag位、四个color位、适用ulong宽度的模型里，shift8及M19/M20掩码计算可保留。M20从0x123404解出0x123400只是数值假设；继续到回调还须work.func可调用、worker/PWQ/WQ/pool有效、scheduled成员真实、strscpy/删除/必要标志操作/kick/解锁均合法返回。不能仅凭假设PWQ地址有效就宣布所有前序门通过。

process_one_work里的kick调用本身是无条件语句，是否发通知由其内部条件决定；facts.state_layers的“conditionally kicks”据此限定。正确PWQ还不单独证明pwq->pool与worker->pool一致：该函数用后者的锁、前者的wq元数据，两者关系仍属调用前提，不新增实测竞态结论。

Q8补worker_clr_flags在计数测试前确实执行的状态写入：

[VERIFIED mykernel/kactive/workqueue/workqueue.h::worker_clr_flags]
```c
			worker->flags &= ~flags;
```

M23/M24在既定mask、无其它修改、有效计数前提下可保留。M21除了回调返回，还需后继锁和标志辅助函数返回且worker/pool仍可访问；回调可能释放work时，不能笼统宣称本函数后段再次读取work，也不能据局部未再读就认证任意销毁/重入安全。

## 5. R88-S03：实际启动入口、图与历史消费

本轮在固定time实际读取main.c，所读两个入口的工作队列调用均为注释：

[VERIFIED mykernel/init/main.c::start_kernel]
```c
	// workqueue_init_early();
```

[VERIFIED mykernel/init/main.c::kernel_init_freeable]
```c
	// workqueue_init();
```

Q9/Q10只支持这两个固定位置不发起对应调用，不证明全树、所有配置或Owner机器永远没有其它调用者。88的有效输入条件模型仍可研究局部契约，但不能被解释为当前正常启动已经初始化并运行工作队列。89把启动门及pool/worker生产者列为具体专题，不要求解除注释或运行实验。

DOT中的first→assign是simple_worker_thread在取得宏结果后的调用，不是宏自身调用simple_assign_work；decode→current是get_work_pwq返回到process_one_work后的接续，不是callee在写worker字段。图中共享节点、点线与蓝线须按实际调用帧消费；post→scheduled表示回到外层循环，不能自动保证下次容器指向有效work。const注释、WARN名、初始化函数名和CPU编号声明不是现场证据。

七F九U继续按原身份保留。F02和F07涉及pending协议两端，可能共享前提，不能相加为两个独立已确认根因；F03的BH分支源内不返回与运行死锁复现分开，F04哨兵容器与实际崩溃分开，F05局部NULL缺口不扩大为全库无初始化。R86-01闭合及87限定接收均不改变；不关闭PA87/PC86源码风险。

## 6. 后续安排与独立性

89按126继续现C06b/11，先G00，再D00及G01–G04，次一档执行；完成后仍交本C06一轮Pro审核纠正及新成果。只追加新文件，不改原88/评论/103三整数，不自行签88闭合或89通过，不启动90。

本轮只作可读源审查，没有运行正则/JSON/DOT检查程序、有限模型程序、完整本地P9、仓库脚本、预处理、编译、链接、Graphviz、QEMU、实机或真实并发。上述十锚不是十次运行实验。同族独立主线对话审查不构成异族或Owner人工逐字复核，也不为作者历史工具过程或后台模型作证。110及124继续显式进入以后主线交接固定清单。
