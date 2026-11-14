---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-PERCPU-BOOTSTRAP-86-REVIEW-001
record_type: independent_static_review_with_documentation_only_return
evidence_class: 六件全文、固定源码与远端净差异的可读审核；不是构建或CPU运行证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner对本主线独立验收的明确选择）
date: 2026-11-14
base_snapshot: time
read_channel: connector
external_read_channel: official_documentation_and_official_search_excerpt
source_pin_short12: a039d9803ade
contract122_short12: d2edb61330d9
reviewed_base_short12: 2489ea8e6963
reviewed_head_short12: 596fb3d7e1a5
reviewed_delivery_comment: 6870949847
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/122-user-percpu-bootstrap-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md
  - agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
  - agent-workspace/tasks/00-gpt-task-protocol-v2.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/percpu-bootstrap.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/percpu-bootstrap-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-percpu-bootstrap-86/evidence.md
status: RETURN
acceptance_verdict: RETURN
returned_items: [R86-01]
return_scope: DOCUMENTATION_ONLY_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_WITH_R86_S01_S02_S03_PENDING_DOCUMENTATION_CLOSURE
mandatory_supplements: [R86-S01, R86-S02, R86-S03, R85-S01, R85-S02, R85-S03, R84-S01, R84-S02, R84-S03]
new_D00_required: true
original_author_self_check: [77, 77, 0]
original_readable_definition_anchor_count: 77
original_readable_raw_prefix_count: 78
original_explanatory_literal_count: 1
original_artifacts_modified: false
self_check: {verified_claims: 8, quotes_reconfirmed: 8, downgraded_to_inferred: 0, scope: 本回执八条固定源码短锚，逐字可读核对；不是原六件机械扫描}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-PERCPU-ALLOCATION-87
next_contract: ../123-user-percpu-allocation-contract.md
executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第86批审核完成：仅自检文档补正，不重做技术整包

第86批已正式交付，但独立裁定RETURN，仅R86-01。77个定义锚之外，facts说明文字另含一次保留标签前缀，原清单仍写77/77/0，不符合122§6及协议P6/P9-5的全包原始前缀计数要求。技术主干按R86-S01/S02/S03保留；87开头追加纠正，随后直接做新专题。当前不是86已PASS_PENDING_LOCAL，不要求Owner逐项审查或修改内核。

## 1. 实际交付、读取和可读门

执行direct ref、PR17和正式评论6870949847均指向596fb3d7e1a5。相对2489ea8e6963为ahead1/behind0、一个提交，仅新增六件4080行：MANIFEST51、正文222、facts2378、DOT116、deferred365、evidence948；旧件零净修改删除。评论创建时间2026-11-14T03:26:47Z。初核lead d2edb61330d9，master直接e8e60cd19540，time固定a039d9803ade；PR16/17 Open/unmerged，17仍Draft。

六件及评论全文读到末尾。正文首段长响应从60行重叠补齐；facts331–660长响应以600–900重叠补齐，其余分段连续至EOF。证据索引E001–E077与真实源码的可读核对使用本轮新读和本有效C06此前亲自取得的同一固定对象；没有借用执行者的读取资格，也没有重演其历史工具日志。

十四个MANIFEST字段、公约整句、双分支金丝雀、read_channel和来源分账存在；25个源码入口含24个引文来源和1个声明上下文，超24软限的理由可见。十矩阵69行、29例（A12/B10/C7）、九候选九缺口是不同内容统计，不能相加为运行见证。六件可读未见完整40位十六进制串；工具返回与正式评论不混入该范围。

P9-5明确失败，故按00§5退回文档补正。其余已读P9字段、真实引文定义边界和新增写区未见同类硬失败；下述语义补充不追认原有过宽说法。完整机械P9、正则扫描、JSON/DOT解析、原模型重放、编译和内核运行仍NOT_RUN，不能签PASS。作者的77次重读是作者账，不等于主线机械复测。

## 2. R86-01：77个定义锚不等于全包原始前缀总数

| 原六件 | 可读原始前缀数 | 性质 |
|---|---:|---|
| MANIFEST.md | 0 | 清单三整数为77/77/0 |
| percpu-bootstrap.md | 0 | 引用证据ID，不另加定义标签 |
| facts-and-boundaries.yaml | 1 | source_ledger最后一个sections.h条目的extent说明中含裸标签字面量 |
| percpu-bootstrap-paths.dot | 0 | 图示与状态 |
| deferred-findings.yaml | 0 | 条件候选和缺口 |
| evidence.md | 77 | E001–E077定义短锚 |
| 合计 | 78 | 77个实际定义锚，加1个说明字面量 |

定位：facts.source_ledger中path为mykernel/include/linux/kernel/asm-generic/sections.h的条目，extent用英语说明不把extern声明当定义锚，其中仍写出了一个以左方括号紧接VERIFIED构成的标签。此处只是解释文字，不是第78条源码证明；协议却明确要求全包计数包含说明文字，因此不能排除该次出现后声称计数通过。本回执避免再复制该完整保留前缀作为说明例子。

87/D00须在新成果中追加原六件逐件计数、该具体位置、77定义锚/1说明字面量/78原始出现的区别，并撤回原“全包计数已一致”之类总括。原77/77/0、六件和旧评论保持冻结，不倒改为78/78/0，不把说明文字伪装成一条新核实源码事实，也不倒造历史机械检查。87新六件按其自身原始全文另计，连同D00新增锚一并核算。

D00状态只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW；只有主线独立回核后关闭R86-01。此项不是新增内核缺陷，不要求重做模板、偏移或GDT专题。

## 3. R86-S01：整数偏移、指针算术和真实访问链

普通per_cpu与当前CPU访问分别依赖offset数组和寄存器读取；原E049只证明per_cpu，不能单独支持this_cpu_ptr到arch_raw_cpu_ptr。补Q1：

[VERIFIED mykernel/arch/x86_64/include/asm/percpu.h::this_cpu_ptr]
```c
	#define this_cpu_ptr(ptr)	arch_raw_cpu_ptr(ptr)
```

Q2给出普通偏移相加的真实整数类型：

[VERIFIED mykernel/include/linux/compiler/compiler.h::RELOC_HIDE]
```c
# define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
	 __ptr = (unsigned long) (ptr);				\
	(typeof(ptr)) (__ptr + (off)); })
```

在size_t与ulong均为w位、索引运算合法且采用已读宏的条件模型中，D=(B-T) mod 2^w，U_i=(i*S) mod 2^w，O_i=(D+U_i) mod 2^w，最终整数地址A_i=(V+O_i) mod 2^w。若V=T+x，则A_i=(B+x+i*S) mod 2^w。原B-E03等的“no unsigned wrap”是其简化数学模型前提，不是源码整数计算必须满足的全局正确性条件；B<T时产生模意义偏移本身不证明错误。地址转回指针、映射、对象范围、对齐及读写是否合法仍须另外证明。

Q3与上面的双整数相减不同：

[VERIFIED mykernel/mm/misc/percpu.c::simple_pcpu_setup_first_chunk]
```c
	pcpu_reserved_chunk->base_addr =
		pcpu_reserved_chunk->data - (ulong)pcpu_base_addr;
```

这里左操作数是void指针，右操作数才转成ulong；不能静默改述为两个地址整数的相减，也不能仅由GNU的字节步长扩展认证跨对象运算、保存的编码值或后继访问合法。声明中的char数组与链接器提供的符号值同样不是本轮生成了真实C数组存储的证书。

适用外部语义（查阅日2026-11-14）：[EXTERNAL https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Unsigned-Overflow.html]，官方检索正文实际取得短句“Unsigned arithmetic in C ignores overflow”，并说明结果按类型位数取模；直接页面两次超时，未声称直开全文成功。[EXTERNAL https://gcc.gnu.org/onlinedocs/gcc-13.5.0/gcc/Pointer-Arith.html]，GCC13.5文档把void指针算术的目标大小按1处理。仅用于明确条件模型/扩展语义，不认证Owner实际编译器版本或生成的机器码。

## 4. R86-S02：算例可达性、失败前缀与数量

M01–M07的尺寸算术必须与真实模板可达性分开。若明确采用T和E按4096字节边界形成且E>=T的链接模型，则S应为4096的倍数，S=1/4097/8193只能是脱离该布局条件的算术例；S=0也不证明含有非空perCPU对象的实际镜像可以这样链接。没有link map并不等于可随意把任何整数当作符合脚本的成功输入。

M12的S=2GiB、C=4GiB可保留为宽度/转换的隔离算术，不能保留“全部大分配成功”作为固定启动路径见证。在原84/85声明的_text>=链接基址+16MiB、_end<=基址+1GiB，以及_text<=T<=E<=_end的模型内，S<=1008MiB，故该2GiB模板被排除。另有分配区间限制：

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_DMA32_PFN]
```c
	#define MAX_DMA32_PFN		(1UL << (32 - PAGE_SHIFT))
```

[VERIFIED mykernel/mm/early/memblock.c::memblock_alloc_range]
```c
	start = max_t(phys_addr_t, start, PAGE_SIZE);
	end = max(start, end);
```

[VERIFIED mykernel/mm/early/memblock.c::__memblock_find_range_bottom_up]
```c
		if (cand < this_end && this_end - cand >= size)
			return cand;
```

在已读DMA32包装、PAGE_SHIFT=12、正常有序区间且无算术异常的前提下，上界至多4GiB，而首个物理页被避开；完整4GiB请求不能放入这一受限区间，降低min_addr重试也没有取消上界与首页面排除。不能把M12的后续正常复制/发布当成源码实际可达。注意early helper在data申请前已经写free_bytes，不能反向说所有元数据赋值都在data失败之后。F08只保留原条件身份，不从这个孤立例子宣布实机损坏或全系统无风险。

M07的N=0虽给出零字节申请表达式，却没有因此取得S=4096的合法目标extent；“返回非空”也不自动提供所需空间。offset数组申请、reserved第二次创建及全部后继步骤须另加“前序复制等正常返回”的门，不能从静态线性排列宣称五次请求都已发生。M16本来就声明只评价循环，可按该范围保留。

M08中“execution stops”统一消费为“正常执行的证明在解引用处停止”，不是源码存在保护性停止分支，不断言实际trap及其后状态。元数据/数据失败、数组写入失败和未返回callee分别保留。五次是全部路径到达时的包装调用数量，不是包含memblock内部重试后的全部底层搜索次数。

for_each_possible_cpu的展开是局部int循环，不能由外层uint声明改其类型；0..N-1的完整枚举还需有符号递增可表示、循环体合法返回等前提。N=257样例在声明NR_CPUS=256时仅支持首个超界索引256的条件判断，不能跨过该点继续推定机器状态。稀疏集合模型只用来显示宏不查mask，不证明真实硬件拓扑。

roundup的非常量路径实际经fls_long、fls64到内联bsrq；本轮已定位相应定义，但未认证指令执行或最终代码生成。有限算例采用有效位扫描结果和合法移位宽度作为条件，不把函数名当成任意输入的正确性证明。原F/U不重新编号或重复计根因。

## 5. R86-S03：模板内容、先行启用、调用和图示边界

E070仅截到INVALID_SEG的零初始化，不足以单独证明GDT其它项非零。Q7补实际对象中的两个输入表达；其配置条件仍需与对象全文合读：

[VERIFIED mykernel/arch/x86_64/smp/percpu_area.c::gdt_page]
```c
	[GDT_ENTRY_KERNEL_CS]			= GDT_ENTRY_INIT(0xa09b, 0, 0xfffff),
	[GDT_ENTRY_KERNEL_DS]			= GDT_ENTRY_INIT(0xc093, 0, 0xfffff),
```

此锚只证明初始化输入，不能单独证明最终编码、实际GDT字节或LGDT成功。pcpu_hot与独立current_task仍是不同存储对象。单次memcpy与其它CPU的定点字段写入可以按所读路径保留，但不能据此宣称全库不存在其它复制或AP必然失败。

当前raw_cpu_read_long不消费this_cpu_off参数的结论保留；PC86-U07/F09不可扩成“完整启动源没有启用FSGSBASE”。本轮在早期源实际取得Q8：

[VERIFIED mykernel/arch/x86_64/kernel/head_64.S::startup_64]
```asm
	movq	%cr4,		%rax
	bts		$X86_CR4_FSGSBASE_BIT,	%rax	// enable fsgsbase instuctions
	movq	%rax,		%cr4
```

这只是先行设置请求。目标CPU是否支持、该路径是否到达和成功、后继是否改变CR4/GS、运行时读回都没有证明。保留相关缺口，不把调用包装中的局部无检查误报成全链无准备。refresh_segment_registers所读体含SS与远返回序列，没有因注释“无需写gs”就认证实际处理器状态不变。DEBUG的定义存在判断与实现门中的数值表达式!(DEBUG)也不得混称；DEBUG=0和未定义不是所有门下同一种情况。

图中creator/data节点供两次early helper使用，data→first只代表first那次，不是reserved也重写pcpu_first_chunk。点线只表示同一调用者的源码次序，不制造callee间调用；asm→O仅表示早期消费者依赖同名数组，不意味着后期发布已发生。CPU0 GDT/GS请求、内存偏移发布、寄存器真实值和current读取各自保留门槛。M20已声明假想reserved区间，不能把它与源码另行分配的reserved chunk混为同一块真实存储。

读取新旧账只认证原件中实际存有何种申报；本轮独立证据范围见检查点，不替作者历史全过程或后台模型作证。九F九U有共享条件，F09尤其是准备条件/缺口，不是新增已复现bug。84闭合和85限定接收不改变。

## 6. 接续和未执行项

86仅R86-01等待追加闭合；87按123由现C06b/11先G00，再D00及G01–G04，仍交本C06 Pro独立验收。不重做86，不新开执行对话，不恢复旧10/09，不要求Owner技术决策。87发布不等于已启动。

同族模型独立会话复核不是异族或Owner人工逐字审核。未执行正则/JSON/DOT检查器、模型程序、仓库脚本、CMake、预处理、汇编、编译、链接、Graphviz、QEMU、实机或硬件实验；无内核修复或补丁。110号继续显式进入以后每次主线交接的交接包、启动词和固定必读清单。
