---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EARLY-ROOT-HANDOFF-84-REVIEW-001
record_type: independent_static_review_with_documentation_return
evidence_class: 六件全文、固定源码及远端净差异的可读审核；不是构建或启动证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（沿用Owner对本主线独立验收的明确选择）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract120_short12: 6b5aabc24e2a
reviewed_base_short12: 18047d54b0a2
reviewed_head_short12: 7d53b2c1ca36
reviewed_delivery_comment: 6849778226
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/120-user-early-root-handoff-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md
  - agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md
  - agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md
  - agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md
  - agent-workspace/tasks/00-gpt-task-protocol-v2.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/early-root-handoff.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/early-root-handoff-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/evidence.md
status: RETURN
acceptance_verdict: RETURN
returned_items: [R84-01]
return_scope: DOCUMENTATION_ONLY_NO_WHOLE_BATCH_REDO
technical_core: RETAINED_WITH_R84_S01_S02_S03_PENDING_DOCUMENTATION_CLOSURE
mandatory_supplements: [R84-S01, R84-S02, R84-S03, R83-S01, R83-S02, R83-S03, R82-S01, R82-S02, R82-S03, R81-S01, R81-S02, R81-S03]
new_D00_required: true
original_artifacts_modified: false
original_readable_label_count: 49
original_readable_raw_prefix_count: 49
self_check: {verified_claims: 8, quotes_reconfirmed: 8, downgraded_to_inferred: 0, scope: 本回执八条短锚逐字可读核对；不是原49条机械闸门}
original_mechanical_scan_JSON_DOT_or_model_replay: NOT_RUN
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-BOOT-BUILD-CONTRACT-85
next_contract: ../121-user-boot-build-contract.md
executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: false
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第84批审核完成：仅文档补正，不重做技术整包

第84批正式交付已成立，但独立验收裁定RETURN，仅R84-01。引用了真实源码，不等于引文都落在所标符号的定义内部；本次发现明确的P2/P9-2失败，不能以主线补充说明直接追认整包通过。A/B/C技术主干按本回执限定保留，补正在85的D00追加，原84六件和旧评论不改。当前不是PASS_PENDING_LOCAL，也不要求修内核或重做整批。

## 1. 交付、全文读取与可读门

direct exec、PR17和评论6849778226指向7d53b2c1ca36。compare 18047d54b0a2至该头为ahead1/behind0、一个提交，严格六件added共2754行：MANIFEST94、正文174、facts1595、DOT117、deferred237、evidence537；旧件零净修改删除。PR16/17均Open/unmerged，17仍Draft；lead为6b5aabc24e2a，master直接e8e60cd19540，time固定a039d9803ade。PR缓存base_sha不是master现状。

六件和正式评论均完整读取，正文、facts长响应的截断以重叠分段补齐。49个编号E01–E49与可读原始标签计数相符，其他五件未见额外保留前缀；MANIFEST的49/49/0、14必备字段、公约整句、time/master金丝雀和read_channel均存在。未见六件内完整40位十六进制串；不是正则全扫描证书。正式评论/工具返回中的Git对象值不混入六件的计数范围。

facts保留了新11的接收报告、九规则/110加载记录、指导刷新与G00账。本轮核对其已持久化内容和仓库入口，不冒称重新观察了旧聊天每次工具调用或后台时序。停写依据Owner本轮确认和正式交付，而不是监控另一对话。

可读P9-2失败，P9-13的必要调用/存储支持链还须补齐，详见R84-01；其余已读字段与交付边界未见新增硬失败。全文机械扫描、JSON/DOT解析、模型执行和完整本地P9均NOT_RUN。原49次二读是作者账，不能把它当主线无条件通过证明。技术内容为九张矩阵56行、28条件样例、五项候选和八项缺口；facts尾部technical_matrix_count=56实际混用了行数，须更正为九矩阵/56行，不能据栏目名说有56张矩阵。

## 2. R84-01：定义边界及必要支持链的追加补正

以下九项原文虽能在文件附近找到，却超出所标定义；它们不是九个内核缺陷。P2要求单一定义内部连续1–5行，不能以相邻上下文、标签附近或两个宏都真实存在代替。

| 原锚 | 明确问题 | D00必须追加的替代方式 |
|---|---|---|
| E06 / head_64.S::phys_base | 对齐指令、前置注释、EXPORT_SYMBOL并非同一个SYM_DATA定义内部 | 仅以真实SYM_DATA(phys_base, .quad 0x0)行支持初值；对齐和导出分别定点说明 |
| E07 / head_64.S::initial_code | 把前置注释、__REFDATA和.balign合入initial_code定义 | 保留单条SYM_DATA定义；另以__REFDATA真实宏及所在输入节上下文支持存储归属 |
| E09 / memfault_api.h::swapper_pg_dir | 前置init_top_pgt extern不是这个宏的一部分 | 只引实际define；声明、C数组定义和别名三者分开 |
| E12 / pgtable_const_arch.h::KERNEL_IMAGE_SIZE | 外层ifdef/else/endif与两条备选宏被合为一个定义 | 分别引用两个单行宏；条件选择由真实周边条件解释，不把条件指令纳入宏引文 |
| E13 / pgtable_const_arch.h::PMD_SHIFT | 连带了另一个PTRS_PER_PMD宏 | 移位和槽数各用自己的定义 |
| E18 / kernel.lds::SECTIONS | 最后ASSERT位于SECTIONS结束花括号之外 | 按真实顶层ASSERT语句的唯一位置/表达标识，不冒称仍在SECTIONS内或虚构函数 |
| E20 / page_const_arch.h::PAGE_SHIFT | 前置空白/注释和另一个PAGE_SIZE定义合入短锚 | 两个宏分别引用自己的定义，不以四行都位于文件中作为通过条件 |
| E22 / mm_type_declaration_arch.h::pgd_t | 同时引了独立的pgdval_t typedef | 仅以typedef p4d_t pgd_t行支持该类型别名；其余类型另锚 |
| E38 / mmzone_const_arch.h::MAX_PHYSMEM_BITS | 前置已注释旧定义不是活动宏定义内部 | 只引活动define，旧注释仅作为上下文，不赋予活动配置意义 |

例如别名的正确短锚只包含本定义：

[VERIFIED mykernel/mm/fault/memfault_api.h::swapper_pg_dir]
```c
	#define swapper_pg_dir init_top_pgt
```

输入节选择则有独立的真实宏：

[VERIFIED mykernel/include/linux/init/init.h::__REFDATA]
```c
	#define __REFDATA		.section	".ref.data", "aw"
```

D00还须补齐以下支持链，不能只把原编号改一个名字：实际__STARTUP与链接KEEP入口；setup_arch对myos_early_map_pgt_buf_into_directmap及init_mem_mapping的调用；提前映射函数的根项复制；AR04的va_text/va_end赋值；B-ID05/B22的pmd_entry初始权限表达。现有E27只给函数入口，E34只给PMD循环，E48只给early_alloc调用，E49是另一个函数的末尾，均不足以独自支持上述整个事实。

在85新正文/facts/evidence中列出旧锚→修正或拆分后的新锚→受影响矩阵/模型/图的映射，补清旧inputs_read缩写的仓库及根目录。追加撤回原“49条全部已在单一定义内部”的总括，不倒改原49/49/0或冒称历史机械检查已做；新85整包按其真实前缀另计。原“人工核对”不得被解释为Owner或人类逐字复核，应明确实际作者为AI可读核对。R84-01只有主线复核D00后才能关闭。

## 3. R84-S01：构建条件、对象和证明层级

可以保留early_top_pgt的511零槽加一项、两根对象不同、静态PMDS的256/512项条件计算和单项根复制。但“宏值”“配置选择”“最终输出节/ELF”和“实际机器执行”是四层证据；E12失去合规锚并不证明两个数值算错，也不能因为附近注释说8KiB或PIC就跳过实际定义。

E37的这段函数属于非汇编、非__pic__且非DEBUG分支，不是所有非PIC配置统一实现：

[VERIFIED mykernel/arch/x86_64/include/asm/asm.h::rip_rel_ptr]
```c
			static __always_inline __pure void
			*rip_rel_ptr(void *p) {
				asm("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
				return p;
			}
```

同一固定头的DEBUG分支是另一个宏。options_flags.cmake包含Debug/Release条件，且后继target/source属性是否覆盖尚未完整核定；仅由公共CFLAGS不能认证最终编译命令或机器码。F02/U01保留为构建与地址可达性的证据缺口，不直接断言编译器必产生某条绝对访存。

initial_code的.ref.data在所读kernel.lds中没有按名称显式收集，只能保留最终布局未知，不能推出一定丢失、不可访问或链接失败。level2_kernel_pgt的2048字节名义定义也不等于其后对齐填充和整张硬件表页的实际可访问范围已经取证；未被明确初始化的后半不能凭extern[512]补成有效叶。ER84-F03/F05及U02/U07维持条件，不增加独立已证故障计数。

## 4. R84-S02：索引、长度上界和条件模型

活动物理位门是：

[VERIFIED mykernel/arch/x86_64/mm/page/mmzone_const_arch.h::MAX_PHYSMEM_BITS]
```c
	#define MAX_PHYSMEM_BITS 46
```

在原模型声明的64位无回绕、起点检查已通过条件下，physaddr<2^46，故PGD索引floor(physaddr/2^39)为0至127，i+1最多128。原批已正确拒绝把i=511作为该成功前缀的越界见证，保留这个收窄。

B-ID03在正文表和facts中的“(physaddr>>30+0/1)%512”不可当C表达式或精确索引公式。替代为先算j=physaddr>>PUD_SHIFT，再分别取(j+0)%PTRS_PER_PUD和(j+1)%PTRS_PER_PUD：

[VERIFIED mykernel/arch/x86_64/kernel/head64.c::__startup_64]
```c
	i = physaddr >> PUD_SHIFT;
	pud[(i + 0) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
	pud[(i + 1) % PTRS_PER_PUD] = (pudval_t)pmd + pgtable_flags;
```

在明确增加_text-__START_KERNEL_map>=16MiB、_end-__START_KERNEL_map<=1GiB、_end>=_text、PMD_SIZE=2MiB且差值可表示的链接模型里，span<=1008MiB，所以ceil(span/2MiB)<=504。B19的N=512和B20的N=513都被这组前提排除，不能只说前者“可能”违反。它们仍可作为脱离该链接上界的纯槽位模型，不是已出现或同一构建下可达的输入；该不等式没有执行链接器或仓库代码。

B13第二片基址到2^46，只证明超出了该软件起点门使用的范围，不能等同超过CPU物理地址能力或所有表项编码能力；真实装载约束未核。F01的delta=2MiB例必须保留链接_text偏移16MiB、统一装载偏移、先行地址和存储合法、相关高叶未被其它动作修改等条件；不能仅由检查容许非零delta推断真实启动采用了它。

五项F记录保持条件候选/needs_evidence，八项U保持证明缺口；F02/F05与相关U互相重叠，不相加为更多根因。没有批准修复、真实装载实验或构建。

## 5. R84-S03：调用者、复制和活动根不能合并成一件事

setup_arch中的实际提前映射调用与其后init_mem_mapping调用应各自取证，而非由callee末尾反推调用者：

[VERIFIED mykernel/arch/x86_64/kernel/setup.c::setup_arch]
```c
	myos_early_map_pgt_buf_into_directmap();
```

提前映射函数在flush之后的单项赋值为：

[VERIFIED mykernel/arch/x86_64/mm/init.c::myos_early_map_pgt_buf_into_directmap]
```c
	*init_pgd = *early_pgd;
```

最终映射入口另有后继根切换请求：

[VERIFIED mykernel/arch/x86_64/mm/init.c::init_mem_mapping]
```c
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
```

这些不证明load_cr3包装已经执行、实际活动根正确或TLB生效，也不等于C入口复制高地址[511]时就切换了CR3。两根的一个值相同只有在该值为有效下级表指针时才能解释为共享子树，不能把任意相等值当已建立映射。

DOT的c_bss→c_clear、c_setup→c_alloc→c_direct→c_map以及其它共享节点链只读作各自真实调用者的局部源序，不能解释成clear_bss调用clear_page，或early_alloc调用提前映射。中间reserve_brk、e820等callee门仍存在；common_startup_64的条件分支也不能被一条到C入口的边代替。图不是完整CFG，停止证明不是源码自动拒绝或停机。

84保留myos_early_init_system等先行callee未知的处理方式。本轮为后续选题定点读到pre_init.c，只把它作为新增定位证据，不倒填成84作者已完成整链审计；80已有解析链不因此重开。旧PT81/DM82/TI83及更早限制继续，任何缺口关闭须另有明确证据和主线裁定。

## 6. 独立性、接续与未执行项

本轮新读范围与同一有效C06固定对象复用范围分别记录在检查点。49条原锚作可读来源/定义比对，八条本回执锚逐字可读回源；没有运行原件正则/JSON/DOT检查或模型程序。同族模型独立会话审核不是异族复核或Owner人工逐字审核。

下一85由现C06b/11按121执行，先G00，再D00及G01–G04；不新开对话、不恢复旧10/09、不重做83/84整批。D00纠正待本C06独立关闭，执行者不得自行签84或85通过。完整本地P9及全部预处理/汇编/编译/链接/仓库脚本/Graphviz/QEMU/实机仍NOT_RUN。110号继续显式进入以后每次主线交接固定清单。
