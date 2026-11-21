---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-TASK-STORAGE-LIFETIME-74-REVIEW-001
record_type: independent_bounded_review_with_documentation_return
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台配置未独立认证）
model_per_owner: gpt pro
date: 2026-11-12
read_channel: connector
source_pin_short12: a039d9803ade
contract106_short12: d46bab628653
reviewed_base_short12: 8bc7583e45c9
reviewed_head_short12: 6f792023d592
reviewed_delivery_comment: 6812545638
acceptance_verdict: RETURN_DOCUMENTATION
returned_scope: [R74-01]
prior_R73_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_RECEIPT
subjects: {D00: ACCEPTED_DOCUMENTATION_ONLY, A: TECHNICAL_CORE_RETAINED, B: TECHNICAL_CORE_RETAINED_WITH_LIMITS, C: TECHNICAL_CORE_RETAINED_WITH_LIMITS}
mandatory_supplements: [R74-S01, R74-S02, R74-S03, R73C-S01]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original74_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
original_DOT_hash_and_parser: NOT_RUN
reviewer_models_executed: false
self_check: {verified_claims: 5, quotes_reconfirmed: 5, downgraded_to_inferred: 0, scope: 本回执五条直接引文；人工核对，非原74条机械重验}
next_followup: CORE-USER-ALLOCATOR-ENTRY-75
next_task_ref: ../107-user-allocator-entry-contract.md
executor: MYOS2-A-C05c / MYOS2-LEAD-002-CHAT-EXEC-09
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
---

# 第74批独立审查：一项引文归属纠正，技术主体保留

RETURN_DOCUMENTATION，唯一R74-01。不是交付遗漏或内核修复要求；不重做74。73的R73-01另行闭合，73组合材料PASS_PENDING_LOCAL，不受本批新问题影响。75以D00追加处理R74-01，再完成三个新技术专题。

## 1. 固定交付和方法

正式交付6812545638对应C05c/CHAT-EXEC-09及6f792023d592，执行分支HEAD一致。独立compare自8bc7583e45c9：ahead1/behind0、一个提交、严格六件added，共2078行，旧件零净改删。六件全文已读：MANIFEST130、正文97、facts910、evidence667、DOT109、deferred165；长件连续分段补齐。PR16/17开放未合并，17仍Draft。执行者STOP_WRITING是交付声明，不当作监控另一聊天物理进程的证据。

D5+A24+B22+C23共74个标签已阅读全文，强制结论对照固定源及本有效C05此前取得的同pin定义。没有重放作者74条自动字节/定义扫描、32例模型、50节点52边自动统计或完整本地P9；可读审查与执行方自检声明分列。

## 2. R74-01：B12引用了另一个宏的定义

B12标签是mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE，引文却为THREAD_SIZE_ORDER的定义。两者是同文件内两个独立宏，不能用相邻关系代替定义身份。固定源码分别是：

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE_ORDER]
```c
	#define THREAD_SIZE_ORDER		3
```

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE]
```c
	#define THREAD_SIZE				(PAGE_SIZE << THREAD_SIZE_ORDER)
```

这不是源码不存在、宏数值被改动或空行争议；问题是P2/P9-2要求的标签—定义不相符。B12单段也不足以单独支持栈尺寸完整表达式。按协议保留文档退回，不把可推导的数值正确当作引文已经合格。

75 D00须：将旧B12重锚到THREAD_SIZE_ORDER；另给THREAD_SIZE、PAGE_SIZE、PAGE_SHIFT独立短锚，若使用_AC生成的C字面量则定点核其定义；以旧→新映射补正文G02、facts.stack_lifetime_matrix、M14、图K48和相关消费。32768只作为所选宏组合的有界整数模型，不当作编译或实际分配证明。

同步追加限定原MANIFEST、正文、facts、evidence开头及评论6812545638中74/74全部定义合格的总括。74保留为历史标签数，B12不满足定义门；其它73标签没有在本轮全量机械重新认证，不能倒算为全部正确。旧六件、旧评论不改；纠正合入75六件，不多交一包，由C05返回后独立关闭R74-01。

## 3. R74-S01：复制、错误门与实际消费者

任务结构与内核栈由不同调用取得；先结构赋值、后新栈发布、再usage重设的顺序成立。非匹配node的未初始化tsk属于UV-16。匹配node且task分配NULL不先得到可回收子task；栈分配失败时尚未覆盖继承的stack，局部只释放task结构，不应凭该继承指针追加栈释放。

dup中的整结构赋值之后紧接着给err确定的零值：

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		err = 0;
```

在这段正常、无未定义行为的活动源序列里，紧邻的if(err)不是已证可达的另一个失败生产者。destroy_caller_matrix的“or err test branch”只可保留为结构分支说明；当前真正可解释的free_tsk路径须回到栈取得失败等真实生产点，不能自由赋予err非零。

INIT_LIST_S结果被整体复制覆盖的源事实可以保留；不等于新节点已插入原父链、已产生实际列表破坏或每次fork必坏。TSL74-F01仍需中间消费者与时序，后续插入是否重写节点及并发协议分别核。usage被复制/重设不自动取得外部引用，静态init_task初值不代表任意current或所有运行时task。

## 4. R74-S02：状态检查、持有者和释放入口

release_task_stack使用的是完整数值相等检查，而非测试某一位是否出现：

[VERIFIED mykernel/sched/forkexec/fork.c::release_task_stack]
```c
	if (WARN_ON(READ_ONCE(tsk->__state) != TASK_DEAD))
		return;  /* Better to leak the stack than to free prematurely */
```

例如含额外位的值不能仅凭含0x80就越过该门。READ_ONCE的所选宏包含类型检查和volatile读，不负责给task/stack取得引用，也不是从读状态到kfree的锁定协议。options_flags.cmake含CONFIG_BUG，linux/debug/bug.h实际包含generic bug.h；该选定WARN_ON定义返回条件本身，不据名字认证日志或故障恢复。没有做预处理/最终二进制选择认证。

原74把kfree内部留作边界是合理的，不能因此扣为缺交。为75划界，主线另读固定slub.h，发现明确的NULL早退：

[VERIFIED mykernel/mm/kmalloc/slub.h::kfree]
```c
			if (unlikely(object == NULL))
				return;
```

因此，在同一仍有效task、第一次调用正常返回并已将stack置NULL、第二次仍满足状态门且字段未被改写的限定模型中，所选第二次kfree(NULL)不进入folio/释放分派；不能据此证明任意重复put安全。task已经释放时会先在状态读取处失效；栈有其它使用者或静态/悬空地址时也不能借NULL分支豁免。此为主线补充，不改旧74，不签75已完成或物理回收正确。

get_pid/put_pid针对pid对象，不能替代task.usage；put_pid在减量前先读取numbers[level].ns，须有正确的对象/索引。普通n→n+1→n模型还要求实际原语处于对应非饱和有效区间及无并行变化。PID对象仍活着不证明其关联task仍活着。创建失败的state写入→栈归还→task归还与正常退出是不同调用链。

## 5. R74-S03：图示与范围

K46→K8静态init_task来源只适用于orig确为该对象的场景；其它current不能套静态usage=2。K24→K31是copy_process失败调用者的顺序，不是所有put_task_stack都会调用delayed_free_task。K12→K13含callee返回及中间有效语句；分配nonNULL不能替代对象有效。K2→K15不能将恒零err门当真实失败生产者。K5的停止点在未初始化值被读取，不是node不匹配本身执行了内核终止。

K40→K41仍须实际switch和对应C路径恢复；正常退出更早的group_dead边界继续生效。ANALYSIS_STOP、数据边及注释不当完整CPU CFG。图和32个有限场景是资料形态，不是运行或形式化证明。

本轮新读固定源码：page_const_arch.h、generic bug.h、linux/debug/bug.h、generic rwonce.h全文；init_task.c1–140；pid.c100–125、pid.h1–85；time/master options_flags.cmake全文和panic65–85。fork.c分配/dup/失败尾、task_struct/alias、PID查询体、退出与finish相关体复用本有效C05已直接读取的同一固定Git对象，不借执行者已读声明。73回执、106及其检查点按本会话完整正文消费；协议v2本轮完整刷新，00/18/24/28/63/79及适用指导沿本有效主线记录，不导入其它项目维护。

为75另读slub.h全文、slab_common.c1–310、slub.c150–350、slub_const.h1–150；04回执已显示的§1–4、04正文mm.kmalloc/§3.1相关片段及05 deferred索引只作范围设计和历史入口，不追认旧整包全部结论。默认分支搜索仅定位，技术判断均回固定time。TSL74一候选八缺口、UV/PID等状态保持；完整本地P9和所有内核构建运行NOT_RUN。
