---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-REVIEW-001
record_type: independent_bounded_static_acceptance_with_append_only_corrections
evidence_class: 六件全文、固定源码定义及远端净差异的可读审核；不是内核或硬件运行证明
conversation_display_name: MYOS2-A-C06 内核分析主线续建
produced_by: GPT-6 Astra Pro（模型自报；后台不可独立认证）
model_per_owner: Pro（Owner本轮明确要求）
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
contract118_short12: b129d706e350
reviewed_base_short12: 3f41eb242040
reviewed_head_short12: 18047d54b0a2
reviewed_delivery_comment: 6847954250
inputs_read:
  - agent-workspace/lead/MYOS2-LEAD-002/118-user-direct-map-table-install-contract.md
  - agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/MANIFEST.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/direct-map-table-install.md
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/facts-and-boundaries.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/direct-map-table-install-paths.dot
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/deferred-findings.yaml
  - agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-direct-map-table-install-83/evidence.md
status: PASS_PENDING_LOCAL
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_supplements: [R83-S01, R83-S02, R83-S03, R82-S01, R82-S02, R82-S03, R81-S01, R81-S02, R81-S03]
returned_items: []
new_D00_required: false
original_artifacts_modified: false
original_readable_definition_labels: 119
original_readable_raw_prefixes: 119
original_mechanical_scan_JSON_DOT_or_model_replay: NOT_RUN
self_check: {verified_claims: 10, quotes_reconfirmed: 10, downgraded_to_inferred: 0, scope: 本回执十条固定定义短锚逐字可读核对；不是原119条机械检查}
full_local_P9: NOT_RUN
runtime_evidence: NOT_RUN
next_followup: CORE-USER-EARLY-ROOT-HANDOFF-84
next_contract: ../120-user-early-root-handoff-contract.md
next_executor: MYOS2-A-C06b / MYOS2-LEAD-002-CHAT-EXEC-11
executor_rotation: OWNER_REQUESTED_NEW_CONVERSATION
phase: 2
owner_decisions_requested_now: []
open_questions: []
---

# 第83批按补充限定接收；下一批交新执行对话

第83批结论为PASS_PENDING_LOCAL，原六件须与R83-S01/S02/S03合读。主线直接追加路径、保护位和样例/图示纠正，不改作者原件与评论，不追认原支持关系全部正确。没有新增RETURN、没有D00，不要求重做83。Owner明确C06a上下文过大，84由新C06b/CHAT-EXEC-11接收并执行；本C06仍保有原主线职责。

## 1. 实际交付与核查范围

执行direct ref、PR17和正式评论6847954250均指向18047d54b0a2。compare 3f41eb242040至该头为ahead1/behind0、一个提交，严格六件added共5437行：MANIFEST166、正文66、facts3614、DOT147、deferred295、evidence1149；旧件零净改删。PR16/17保持Open/unmerged，17为Draft。停写以Owner交回和交付声明为据，不冒称监控另一对话后台。

六件及正式评论独立完整读取，MANIFEST及facts长响应截断均补齐。源码短锚A38+B36+C45=119，其他五件可读未见额外保留前缀；MANIFEST的119/119/0、14必备字段、公约整句、time/master金丝雀和read_channel齐备。全文可读未见完整40位十六进制串；这不是运行正则扫描的证明。

短锚字面及真实定义范围使用本轮新回源与本有效C06已直接取得的相同固定对象核对；没有借执行者119次二读声明替代本次判断。必要init写包装确已取得定义，不再是82规划时的定位缺口。C15等附带scope边界对闭括号的截取不精确，但相应短引文本身仍在所标真实函数定义内；不把辅助范围视为编译器边界，也不以空白行或签名前缀捏造失败。

11矩阵78行、35隔离条件样例、8候选记录和10缺口是交付内容统计。作者图56节点/77边及内容检查计数未由主线机械重算；113个内容支持锚与119个短锚不是同一计数栏目。矩阵、引文和模型互相复用，不能合计为更多独立实验。可读P9按下述纠正限定接收；完整本地P9、自动JSON/DOT解析及模型重放均NOT_RUN，不能升级PASS。

## 2. R83-S01：分支优先级和图中的真实调用者

facts.BE09/BE10把“none，或旧leaf未复用且mask仍允许该层大页”并列为新建大页入口，后半不适用于它们声明的稳定状态模型。已有非叶先下钻并continue；已有叶且允许该层大页先复用并continue；已有叶而不允许则进入降级。下面的PMD片段与PUD同构：

[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pmd_init]
```c
			if (page_size_mask & (1 << PG_LEVEL_2M)) {
				if (!after_bootmem)
					pages++;
				paddr_last = paddr_next;
				continue;
```

替代结论：在本批无并行改写、mask参数稳定且谓词求值合法的模型里，新建本层大页来自none分支且对应mask位允许；不能把已复用的旧叶再画成后续降级或新建。none按本树忽略erratum位的谓词判断，不必要求原始项值严格为零。真实并发、异常项或输入破坏不能偷渡进该稳定模型来挽救原表述。

DOT的B_pud_reuse→B_pud_old及PMD对应边应拆成“已有叶”的允许/不允许两个互斥分支，不是复用成功之后继续取旧prot。共有递归节点到父项发布的顺序只适用于新分配子表路径；复用已有子表的continue不发布那个新父项。阶段节点不是新的函数，alloc节点到递归callee只表示调用者的后续动作，不表示alloc_low_page自己调用建表。

父表链接与叶写接口必须分开：pud_populate_init经fill_pmd_to_pud_ent[_safe]到set_pud_ent[_safe]；pmd_populate_kernel_init经fill_pgtbl_to_pmd_ent_kernel[_safe]到set_pmd[_safe]；p4d同理。原图将父链接接到名为set_pud_init/set_pmd_init的节点，不能消费为实际调用这两个叶写包装。init布尔仍选择safe/ordinary，safe的告警返回后可继续store，不是拒绝覆盖的返回协议。未证明告警实现必返回时，只保留条件结论。

原AR06/BH02/CL06及部分图节点引用省略了前面的取子表指针、alloc或指针形成语句；应合读固定init.c相应完整函数，而不能只用被引片段的callee签名替代调用证据。所有分析停止节点表示证明边界，不是源码新增的错误处理。

## 3. R83-S02：过滤条件、非present编码与M22页框数值

原B36仅引到check_pgprot调用massage_pgprot，不能单独证明后者算法。BP06等“过滤支持位”的表述需追加present条件：

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::massage_pgprot]
```c
			pgprotval_t protval = pgprot_val(pgprot);

			if (protval & _PAGE_PRESENT)
				protval &= __supported_pte_mask;
```

非present时该函数保留protval，不是无条件与支持位掩码相交。PFN构造还在最终权限组合前做独立的反转：

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::__pte_needs_invert]
```c
		__pte_needs_invert(u64 val) {
			return val && !(val & _PAGE_PRESENT);
		}
```

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::protnone_mask]
```c
		protnone_mask(u64 val) {
			return __pte_needs_invert(val) ?  ~0ull : 0;
		}
```

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pmde]
```c
		pfn_pmde(ulong page_nr, pgprot_t pgprot) {
			phys_addr_t pfn = (phys_addr_t)page_nr << PAGE_SHIFT;
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PHYSICAL_PMD_PAGE_MASK;
			return __pmd(pfn | check_pgprot(pgprot));
```

M22的paddr=4096、mask允许2M而“物理页框取整为0”，须补送入构造器的prot含_PAGE_PRESENT，使protnone_mask为0，并声明所选类型/掩码及写入前提；此时PFN构造部分与2M掩码相交为0。未给prot时不能保证该数值，尤其非零且非present会走反转。最终entry的权限、软件编码解释、真实硬件映射仍另证，不把编码字段当实际可访问地址。

PUD/PMD的present辅助谓词在本树还认可PSE，PTE认可PRESENT或PROTNONE；它们与硬件present位、none及leaf分别评价。pte_clrhuge只清PSE，pte_pgprot再取PTE_FLAG_MASK；不得凭“继承保护位”承诺大页PAT和小页PAT等价。已有源码足以保留原TI83-F04/U04的属性/页框边界，未进行硬件或编译布局验证，不新造同一问题的独立根因。

## 4. R83-S03：同层索引、上层可达性和端点/计数分账

应比较同一层对VA和PA计算的索引，不能把PUD索引与PMD索引本身说成相等。PUD索引读取30–38位：

[VERIFIED mykernel/mm/fault/pgtable.h::ent_index_in_pud]
```c
		PREFIX_STATIC_INLINE
		ulong
		ent_index_in_pud(ulong vaddr) {
			return (vaddr >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
		}
```

在声明的64位加法模型下，偏移低39位为零足以保持PUD同层索引，低30位为零足以保持PMD，低21位为零足以保持PTE。原BH04仅列低30位不能单独推出PUD等价；本树实际PAGE_OFFSET=0xffff800000000000满足更强对齐条件，因此M05/M07具体低位计算可在各自地址域与合法性前提下保留。索引相符不证明__pa回译、phys_base、对象身份、可写性或活动CR3相符。

M21/M22应继续作为带明确条件的局部输入例，但不能扩大成“短区间或非大页对齐请求不可能来自82”。R82-S01已明确提升只改mask、不改MR端点：声明after_bootmem=0、全局mask4、memory为一条合法稳定的[0,2MiB)、请求[4096,8192)，可在提升后仍保留该短区间并带2M位；再声明父表有效、目标PMD为none及本回执S02的prot条件，即可研究对应下层请求。这里没有宣称真实启动取得这些输入，也不把向外覆盖可用RAM自动判为缺陷。

PTE先形成指针再执行范围判断；不能只靠C02中后置条件证明前面合法：

[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
	pte = pte_page + pte_index(paddr);
	i = pte_index(paddr);
```

已有项分支不会更新paddr_last：

[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		if (!pte_none(*pte)) {
			if (!after_bootmem)
				pages++;
			continue;
		}
```

[VERIFIED mykernel/arch/x86_64/mm/init.c::phys_pte_init]
```c
		set_pte_init(pte, pfn_pte(paddr >> PAGE_SHIFT, prot), init);
		paddr_last = (paddr & PAGE_MASK) + PAGE_SIZE;
```

因此补齐M32：在它的早期、slot0新建/slot1已有、请求[0,8192)且全部先行门合法的模型里，局部pages=2、新store=1，最终paddr_last=4096，而不是8192。全已有时可以保持初值end；混合时已有尾项不恢复end。端点可能大于或小于请求末端，既不是实际覆盖并集，也不是新写入字节数。

局部pages++位于新写调用之前；全局累计在本层循环结束后才调用：

[VERIFIED mykernel/arch/x86_64/mm/init.c::update_page_count]
```c
void update_page_count(int level, ulong pages) {
	/* Protect against CPA */
	spin_lock(&pgd_lock);
	direct_pages_count[level] += pages;
	spin_unlock(&pgd_lock);
```

CC07的“成功路径增加”只可表示所有前序正常返回的条件结果，不能倒推局部自增发生在写入成功之后。任何子层不返回时，不得继续认证本层全局计数、父项发布和上层返回。原图C_count4→C_return不表示计数值构成返回值；两账独立。尾后一指针与实际解引用仍分别判断，分析停止不替代源码缺失的NULL检查。

## 5. 来源、独立性与接续

本轮新回源为pgtable.h160–335及475至末尾、pgtable_macro.h全文、pgtable_arch.h220–750及915–935/1160–1180、page_const_arch全文、pgtable_const_arch1–145及245–287、init.c1–60；另读取110全文。核心init.c540–865、地址转换、init_mm根数组、memfault_api、宏别名以及公约/配置/金丝雀，复用本有效C06前轮直接取得且固定对象一致的源码，不继承C06a读取资格，不声称本轮又全读每个来源。

十条本回执短锚均已与固定源逐字可读核对；原119条没有机械全扫描或程序化重放。84设计预读另外取得head64.c和head_64.S全文、mm_const_arch.h，不能倒填为83原作者取证。详细读取、设计取舍和写前现场见本轮检查点。

TI83-F01–F08/U01–U10继续按原身份及条件保存；写包装的来源得到补足不等于所有接口/硬件缺口已闭合。旧DM82/PT81/BM80/MR79/PB78/PA77、MC/LB/GAP、MR-02明确收窄闭合及58/63撤回保持原限制。83接收没有授予内核修复、构建、运行或阶段3权限。

这是同族模型的独立会话审核，不是人工逐字审查或异族复核。新119交接与120合同由主线介导执行者10→11轮换；旧C06a停止后续业务写入，无需旧方再确认。新接收者必须独立接收、加载九规则及110、另行加载适用Mnemosyne指导，通过G00后才执行84。任务发布不等于新聊天已创建或启动；110继续进入每次主线交接固定必读清单。
