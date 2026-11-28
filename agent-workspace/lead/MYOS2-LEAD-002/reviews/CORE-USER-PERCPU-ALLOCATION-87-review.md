---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PERCPU-ALLOCATION-87-REVIEW-001
record_type: independent_bounded_static_acceptance_with_append_only_supplements
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner对正式验收的选择）
date: 2026-11-15
base_snapshot: time
read_channel: connector
external_read_channel: official_GCC_documentation
evidence_class: 六件全文、固定源码和远端净差异的可读复核；非构建、机械全检或运行认证
source_pin_short12: a039d9803ade
contract123_short12: fc14432f2a56
reviewed_base_short12: 596fb3d7e1a5
reviewed_head_short12: bc611665b38f
reviewed_delivery_comment: 6872044008
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/123-user-percpu-allocation-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PERCPU-BOOTSTRAP-86-review.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/percpu-allocation.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/percpu-allocation-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-allocation-87/evidence.md
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R87-S01, R87-S02, R87-S03, R86-S01, R86-S02, R86-S03]
returned_items: []
new_D00_required: false
R86_01: CLOSED_DOCUMENTATION_ONLY_BY_SEPARATE_SAME_ROUND_RECEIPT
batch86_composite: PASS_PENDING_LOCAL
original_artifacts_modified: false
original_author_self_check: [102, 102, 0]
original_readable_definition_labels: 102
original_readable_raw_prefixes: 102
self_check: {verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, scope: 本件十条固定源码短锚的可读回核，不是原102条机械全扫描}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-WORKQUEUE-WORK-LIFECYCLE-88
next_contract: ../125-user-workqueue-work-lifecycle-contract.md
executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 87按补充限定接收；86文档项另件闭合，无新增返工

本批PASS_PENDING_LOCAL，必须合读R87-S01/S02/S03；不是原报告每一句都无误，也不是内核正确性或运行通过。主线直接追加元数据写入、指针视图、模型前提和图示边界的纠正，不改87原件。86的R86-01由同轮独立DOC-CLOSURE回执关闭，不与87自身验收混为一项。

## 1. 实际交付、可读验收与未执行项

正式评论6872044008创建于2026-11-14T04:56:47Z，执行direct ref及PR17均为bc611665b38f。相对596fb3d7e1a5为ahead1/behind0、一个提交，仅六件added5736行：MANIFEST51、正文242、facts3654、DOT115、deferred422、evidence1252；旧件零净修改删除。初核lead fc14432f2a56、master直接e8e60cd19540、time a039d9803ade；PR16/17均Open/unmerged，17仍Draft。

六件和正式评论独立全文读到末尾；正文及facts长响应按重叠区段补齐。102个编号定义锚集中于evidence；其它五件包括D00可读未见新增保留前缀，102/102/0与该范围相符。清单十四字段、公约整句、双分支金丝雀和read_channel齐备。公约和两分支金丝雀本轮回到固定原文比对；六件可读未见完整40位十六进制串，不把工具JSON或正式评论混入六件计数。

source_ledger列25个源入口，其中19个引文来源、6个上下文来源；超过24软限一项不是自动越权或质量加分。十矩阵72行、28条件模型（A10/B8/C10）、九候选九缺口是不同记录维度，不相加为独立实验。声明路径、实际锚、正文/模型消费结合本C06自己的新读和同一固定对象复用核对，不继承11的读取资格。

P9-1/2/3/4/5/7/8/13按可读范围及本回执补充消费，未发现已知需RETURN的原始计数、定义边界或写区硬失败；外部语义按S02的实际版本范围，P9-11未运行状态明确；9/10/12无代码件、补丁或降级交付，N/A。完整本地机械P9、正则扫描、JSON/DOT解析、Graphviz、原模型重放均NOT_RUN，不能签PASS。作者“102次重读”仍是作者日志，不是本轮重演或后台认证。

## 2. R87-S01：元数据写入、容量算例和失败前缀

A-CM02的止证字段写“no actual memory store by this line”，对应free_offset赋值，若解释成这条源码不写内存就是错误。Q1：

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_alloc_area]
```c
	chunk->free_offset = start + size;
	chunk->free_bytes -= size;
	return start;
```

准确消费为：合法执行时修改chunk元数据；这里没有对新分配payload作初始化写入，本轮也没有观测任何真实执行。不能把“未运行”与“语句语义不写内存”混同。A-CM06返回表达式同样只说明本步构造返回值，不否定整个分配器此前已写元数据和链表。

在初始F=C-x、对齐后start=x+g、g>=0、q>=0且所有int运算可表示的模型中，提交后报告F'=F-q，名义单位尾余量为C-(start+q)，二者差g；连续填充可累积该差。M02→M03→M04的C16384、请求(1,1)、(16380,4)、(3,1)算术可保留，第三次接受off16384、new_offset16387。但超出名义单CPU容量C不必然等于超出整个N*C数据块，更不证明随后真实写出了边界。

原“physical_tail_bytes”“physical backing numeric”规范化为名义单位剩余字节/后备内核虚拟地址数值，不是物理地址、已测物理内存或实际可访问范围。

Q2表明对齐参数先按x的类型转换，不能把输入align一律当size_t无符号运算：

[VERIFIED mykernel/include/uapi/linux/const.h::__ALIGN_KERNEL]
```c
	#define __ALIGN_KERNEL(x, a)			__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
```

M07/M08是故意违反正的2次幂对齐前提的隔离算例；M09停于未认证的size_t→int转换，M10停于声明的有符号加法范围，不能继续套任意精度结果。C16/C32等小容量只用于fit/commit局部状态；在既定非负S、PAGE_SIZE4096、无溢出的启动尺寸模型下C>=8192，不把这些小值称成真实启动块。M18的first_chunk应解释为列表中首个候选的抽象状态，不是pcpu_first_chunk的真实初值。

M12/M13给出的精确need=8还需初始偏移已按8对齐；未给出偏移时不能认证该精确值。M12在正常非负填充条件下need>=8仍足以支持free4被拒绝，路线结论不受影响。M17持续扩块不仅需要元数据非空，还需每次data及相对编码有效、列表操作正常返回、请求不变、旧列表有限稳定且先行算术有效。data为NULL时正常证明可能先在指针运算处停止；不能笼统说一定持续到内存耗尽。

Q3给出列表插入的真实先行等待条件：

[VERIFIED mykernel/lib/list/double_list.h::__list_add_valid]
```c
			while ((prev == NULL) || (next == NULL) ||
					(next->prev != prev) || (prev->next != next) ||
					(new == prev || new == next));
```

新块入链和count增加须以该依赖正常返回及合法对象为门；不能把尾部计数语句单独当成功凭据。空循环、返回NULL与停止正常证明是三种不同状态，不追加运行死锁结论。

## 3. R87-S02：补齐当前CPU视图，保留类型和链接前提

87的主要工作队列消费者实际使用per_cpu_ptr；原包没有单独展开this_cpu_ptr对照。所需近邻定义已在86强制补充与本C06固定源读取中取得，本回执明确补齐，不声称这是87作者已独立完成的新表。

Q4、Q5分别是实际宏，不以相邻per_cpu定义替代：

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::this_cpu_ptr]
```c
	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
```

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::raw_cpu_read_long]
```c
	#define raw_cpu_read_long(pcp)	((ulong)rdgsbase())
```

Q6是当前CPU指针的合成；它没有读取传入名字所指的this_cpu_off内存值：

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::arch_raw_cpu_ptr]
```c
	#define arch_raw_cpu_ptr(_ptr)	({							\
				ulong tcp_ptr__ =								\
						raw_cpu_read_long(this_cpu_off);		\
				tcp_ptr__ += (__force ulong)(_ptr);				\
				(typeof(*(_ptr)) __kernel __force *)tcp_ptr__;	\
```

在声明ulong为w位并采用相应指针数值模型时，per_cpu_ptr返回数值(P+O_i) mod 2^w；this_cpu_ptr返回(P+G) mod 2^w，G是实际rdgsbase结果。只有另证G=O_i时两式才一致。offset数组已写入不证明GS寄存器取该值；不扩成硬件/调度安全认证。

first/reserved/new三类块的数值代入及原PA87-F05保留条件身份：由O_i=B-T+i*S，P=off或data-B+off得到相应-T项。void指针减整数、NULL基址加偏移、ulong模加和合法对象访问必须分开；GNU字节步长扩展本身不使所有跨对象或NULL运算合法。旧PC86-F05的单位步长问题与本批返回编码问题有交集但不自动合成或拆成额外已证根因。

Q7、Q8防止把脚本中的零常量误当实际perCPU输出节起点：

[VERIFIED mykernel/arch/x86_64/kernel.lds::_PCPU_BASE]
```ld
_PCPU_BASE			= 0x0;
```

[VERIFIED mykernel/arch/x86_64/kernel.lds::.data..percpu]
```ld
	.data..percpu : AT(__per_cpu_load - __START_KERNEL_map)
	{
		__per_cpu_start = .;
```

这个输出节声明没有把_PCPU_BASE写成其起始地址。M23的T=0仍是隔离假设，不能由Q7推出当前链接结果等于零；M19–M22的低正地址同样不是本内核实际高地址布局。是否构建、实际符号值及后备存储都未测。Q8只登记脚本文本，不声称链接器实际执行成功。

本轮实际读取的外部定义（2026-11-15）：
- [EXTERNAL https://gcc.gnu.org/onlinedocs/gcc-13.5.0/gcc/Pointer-Arith.html]，GCC13.5 §6.24，原句“In GNU C, addition and subtraction operations are supported on pointers to void and on pointers to functions.” 用于字节步长扩展的条件解释，不认证具体指针合法性或目标编译器。
- [EXTERNAL https://gcc.gnu.org/onlinedocs/gcc-13.4.0/gcc/Integers-implementation.html]，GCC13.4 §4.5给出超范围有符号转换按目标位宽归约的实现约定，短原文“no signal is raised.” 因此“目标未认证所以不计算”不等于“任何实现都无法确定”；本包不能冒称已采用该版本。
- [EXTERNAL https://gcc.gnu.org/onlinedocs/gcc-13.4.0/gcc/Code-Gen-Options.html]，GCC13.4 §3.17的-fwrapv原句“This option instructs the compiler to assume that signed arithmetic overflow of addition, subtraction and multiplication wraps around using twos-complement representation.” 用于说明选项前提；实际编译命令未取得，不无条件套用回绕或陷阱行为。

13.5的后两页直开失败后读取13.4对应具体页；不把不同版本拼成目标环境证书，不使用检索中第三方/PDF摘要作依据。原C99/C11式语言前提与GNU扩展、最终命令各自限定。

## 4. R87-S03：分配供应器、失败分类、图和历史来源

Q9仅证明传递清零标志，不单凭包装名保证全部路径和生命周期已清零：

[VERIFIED mykernel/mm/kmalloc/slub.h::kzalloc]
```c
			return kmalloc(size, flags | __GFP_ZERO);
```

最近kmem_cache_alloc/free定义已回源，未重做整个slab/buddy审计。PA87-U03继续保留；已有75/76/81分层限定不能因kzalloc这个名称而被覆盖。M25对所有槽位有效且已初始化的条件明确保留；单个NULL检查不证明其余未写槽均可安全读取。

Q10确认外层没有在此调用点接收内层返回值：

[VERIFIED mykernel/kactive/workqueue/workqueue.c::simple_alloc_workqueue]
```c
	simple_alloc_and_link_pwqs(wq);
```

结合已读完整函数，PA87-F07/M24的局部错误传播断点保留；仍须前序访问、格式化和列表调用正常返回，不能越过无效槽地址宣称发布或真实损坏。F08保留所读清理未解除既有PWQ链、perCPU释放行为仍为注释的范围，不断言所有销毁路径都不释放。F09在UNBOUND分支只保留未初始化ret的止证，不能捏造确定返回值；已见events/事件BH调用的flags不触发该分支。

F06的reserved不足不回退是源码路径事实，是否违反接口预期尚未证明，继续作为策略待核而不是已证内核bug。failure_prefixes中FP01/FP02的not_proven字段夹有局部可由源码支持的路线/无新块结论；按其条件消费，不因字段名把全部内容说成未知或全局保证。

DOT中commit到area是callee返回到调用者area_found的条件控制接合，不是同一函数连续语句；growth到fail也是调用者检查创建返回值后的分支。共享节点不表示同一块对象/同一栈帧；点线和蓝线不能覆盖这些层次。图未包含当前CPU宏支路，按S02补充消费，不当完整CFG。

102锚的作者回读日志、19+6来源分类和PREPARED/READY记录只是原件所含申报，不认证后台模型或所有历史操作。正式交付由实际提交与评论承接，STOP_WRITING据Owner及作者报告。九F九U共享条件，不相加为18个已复现故障；不重开84闭合项或改写旧ID。

## 5. 后继与边界

86计数文档闭合和87限定接收均完成；下一88无D00，按125继续现C06b/11，以次一档完成三个关联工作项专题，再交本C06一轮Pro独立验收和后继设计。124记录Owner的模型选择及醒目提醒，不改变权限或自动切换模型。

本轮未运行本地检查程序、模型、仓库脚本、预处理、编译、链接、Graphviz、QEMU或实机；未改内核、原成果或原评论。主线补充是有界交叉核验，不冒充作者新增执行成果或另一次独立第三方审核。110及124继续显式进入未来主线交接固定必读清单。
