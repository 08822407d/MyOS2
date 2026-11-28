---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-WP-COW-17-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线模型选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-01
read_channel: connector
authorization_ref: Owner报告CORE-USER-WP-COW-17已交付；28号同轮验收及下一工作包持续授权
reviewed_pr: 17
reviewed_head_short12: b7f89a7bc5c0
reviewed_base_short12: 38a23958a355
contract_short12: c1493507a818
receiver_binding_short12: 662271a558b6
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
reviewed_executor_display_name: MYOS2-A-C03b 代码分析执行续建
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 662271a558b6
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R17-S01, R17-S02, R17-S03]
acceptance_scope: 已核写保护分派、数据准备、PTE替换条件及新旧folio引用的选定静态路径；必须合读本回执
page_lifetime_coverage: LOCAL_REFERENCE_OPERATIONS_ONLY_CONTINUE_18
kernel_reference_or_isolation_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_53_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-PAGE-REF-LIFETIME-18
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/37-user-page-ref-lifetime-contract.md
inputs_read:
  - PR17交付评论issuecomment-6556950130及固定b7f89a7bc5c0六件全文；facts分四段、evidence分三段读至末尾。
  - 固定time fault.c 92–210、422–770、1240–1420；arch/x86_64/mm/fault.c 1–435。
  - 固定time mempolicy.c全文；buddy.c 1–780（已抵文件末尾）、buddy.h 1–240、buddy_macro.h全文、buddy_const.h 1–220和225–355。
  - 固定time highmem_macro.h全文、highmem.h 1–160、page_arch.h 1–150（已抵末尾）、include/linux/kernel/asm-generic/page.h全文；page-flags.h 1–160及page-flags_macro.h 1–70。
  - 固定time pgtable.h 145–168、pgtable_arch.h 200–278和620–850、options_flags.cmake全文；已有同源pgtable-generic.c、pgtable_macro.h、PRESENT/PROTNONE及SOFT_DIRTY常量文本沿用本对话前轮实际读取。
  - 35/36固定任务与16回执、18/24/25/28工作约定沿用本对话已加载版本；04回执定向重读至§5可见部分用于避免重复全MM基线，不采用其中已限定的绝对结论。
  - 默认分支folio_alloc、mk_pte、do_user_addr_fault搜索仅导航；实际结论回固定源码。两PR元数据、直接heads、PR16完整变更路径和开放PR第3项为空、交付增量compare。
review_limits:
  - 完整材料阅读与定点源码语义复核，不是执行方53/53机械引文检查或JSON/YAML/DOT解析的独立复现。
  - 本轮未执行命令、脚本、解析器、哈希、预处理、C/ASM、构建、内核、QEMU或硬件；不认证所有编译绑定和全图形式性质。
  - 同厂分角色复核不是异族模型或人类技术验收；不证明实际数据、全局引用平衡、并发、TLB或物理页可复用。
  - 接收/指南加载及写入租约是执行方自报；主线验证了远端内容和单提交净差异，不认证聊天内部状态或全局锁。
  - RS-01、GAP-U04硬件/运行部分、GAP-U10及所有普通延期问题不关闭。
---

# 第17批限定接收；读写权限、图关系和资源状态不能由名字代替证据

## 1. 裁定和交付完整性

以PASS_PENDING_LOCAL接受第17批，R17-S01/S02/S03为强制合读补充，无新的执行退回。主线直接给出替代读法，不另派小型返工，不修改原六件或任何内核代码。

被审头b7f89a7bc5c0，执行者C03b / CHAT-EXEC-02，与36号改派相符。相对38a23958a355，远端compare为一个后继提交，恰好user-wp-cow-17/六件A新增、1754行，旧件无净修改/删除。六件行数为MANIFEST 39、正文62、facts 922、DOT 68、deferred 151、evidence 512。PR17仍open/Draft；主线未代写执行成果。

执行者报告53条引文、28节点/35边、12情景/12效果、9条问题记录及12次双重回读。主线已完整读取六件并回源核关键路径，但没有重新运行其全部检查；数量和格式自查不等于技术正确性。其首次接收和工具“租约更新成功”仅保留为自报，不由一次compare推导存在全局原子锁或其他服务授权。

## 2. 可保留的关键事实及前提

### 数据准备与返回值

已核handle_pte_fault的orig非none、pte_same、WRITE及当前不可写门，myos_handle_mm_fault通过pte_offset_map_nolock取指针和快照。不能把non-none直接读成硬件present，也不能把nolock返回锁地址读成已经加锁。do_wp_page的SHARED/MAYSHARE分支为活动while，私有分支在有效旧folio时get，然后进入wp_page_copy。

wp_page_copy用orig_pte派生PFN判断zero；vm_normal_page则由所读PTE经PFN上界判断和pfn_to_page转换，二者不是同一个谓词。各时点的PTE/页身份稳定仍是条件，不以两次快照比较替代并发锁。

[VERIFIED mykernel/mm/page_alloc/mempolicy.c::folio_alloc_noprof]
```c
folio_s *folio_alloc_noprof(gfp_t gfp, uint order)
{
	return page_folio(alloc_page(GFP_HIGHUSER_MOVABLE));
```

已核folio_prealloc未使用need_zero，实际固定GFP_HIGHUSER_MOVABLE；按本树宏展开，该组合不带__GFP_ZERO。post_alloc_hook所见清零受__GFP_ZERO门，不能据此保证零PFN路径的新数据已经清零，也不能反向断言物理内存实测非零。alloc_page返回NULL后page_folio会读取page字段，故外层OOM判断不构成可靠失败处理证明；同样不预言实际故障形式。

非zero PFN、非NULL且有效源页的路径到copy_user_highpage→copy_user_page→copy_page的PAGE_SIZE memcpy接口；src为空的备用复制/清零被注释，活动返回0。复制助手正常返回集合不等于硬件可访问性证明。PTE复核失败可在已经分配/复制后不安装新PTE并返回0；不能把0当作本次完成替换。

### 引用操作与物理回收

已核folio_get→folio_ref_inc→page_ref_inc和folio_put→atomic_dec_and_test。成功set_pte_at之后，new_folio局部变量改指old_folio，两个尾部put因而都指旧对象；在有效稳定对象、两次put都抵达且原语符合所读接口的前提下，临时get一次与尾部put两次是局部+1−2。初始引用、映射持有与并发计数不能由该算式补出。

[VERIFIED mykernel/mm/fault/fault.c::wp_page_copy]
```c
		/* Free the old page.. */
		new_folio = old_folio;
		page_copied = 1;
```

已核__folio_put当前主体没有活动物理归还调用；另一接口__free_pages却进入__free_pages_ok。这是两条不同释放路径，不能写成全内核永不回收，也不能把folio计数归零等同进入空闲链。实际页的初始化、回用和这些接口之间的契约由18继续取证。

[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	fault = myos_handle_mm_fault(vma, address, flags);
	while (fault != ENOERR);
```

上层该循环没有再次调用处理器；非零结果不因此成为重试，0也不证明故障指令已经在CPU上成功恢复。原报告关于返回消费的方向保留，补上这段独立源证。

## 3. R17-S01：权限变换只增位，不是自动清权限

facts的W17-X05写“RW iff VM_WRITE”过强。替代为：普通非UNSHARE路径先从新页和vm_page_prot构造entry，再调用pte_mkdirty和maybe_mkwrite；VM_WRITE为真时主动设置RW，为假时保留传入entry原有RW，而非清除它。只有另证初始entry不可写，才能进一步建立相应的“仅当”结论。

[VERIFIED mykernel/mm/fault/pgtable.h::maybe_mkwrite]
```c
		maybe_mkwrite(pte_t pte, vma_s *vma) {
			if (likely(vma->vm_flags & VM_WRITE))
				pte = pte_mkwrite(pte);
			return pte;
		}
```

pte_mkwrite是OR入_PAGE_RW；pte_mkdirty是OR入_PAGE_DIRTY及配置相关_PAGE_SOFT_DIRTY，不是覆盖全部保护位。正文“softdirty”受该宏可能为0的编译条件约束；不根据options文件未显式列出某宏，就认证所有外部编译选项。

W17-F07的“UNSHARE skips protections”替代为“跳过本分支的dirty/maybe_mkwrite增位步骤，仍继承所构造entry的保护；随后有BUG_ON(unshare && pte_write(entry))”。它不主动清RW，也不等于完全绕过权限。S10、X05、E27/E28及正文同义句一并受限。UNSHARE的真实调用来源仍未穷尽，不作已发生的故障判断。

## 4. R17-S02：图边恢复实际对象来源和调用层次

以下覆盖facts与DOT同ID关系；不将旧箭头和替代关系同时计为两条证据。data继续使用消费方→提供方；输出赋值/修改用effect。N13须区分“new_folio变量”与“初次分配的物理新页”，成功重绑定后不再同义。

| 原ID | 替代关系/消费口径 | 依据 |
|---|---|---|
| E03 | N02→N04 effect：do_wp_page把vm_normal_page返回值写到vmf.page；不是vm_normal_page内部写该字段 | A04、A07/A08 |
| E04 | N07→N04 data：wp_page_copy按vmf.page取得旧对象，经page_folio赋old_folio；不得把字段值当函数调用方 | A53及page_folio宏 |
| E09 | N07→N08 data：调用方从orig_pte提取PFN后传给is_zero_pfn；不是is_zero_pfn自行读取orig_pte结构 | A09/A51 |
| E14 | N07→N13 effect：wp_page_copy接收folio_prealloc结果；物理page必须经过page_folio转换，不能把__myos_alloc_pages直接标为返回folio | A09/A11及mempolicy.c |
| E17 | N14→N04 data：复制助手的src实参来自vmf.page；按调用绑定理解，不是助手重新读取该字段 | A17/A18 |
| E18 | N14→N13 data：复制助手使用初次新folio.page的目标地址；只限变量尚未重绑定时，不代表读取目标旧内容 | A17 |
| E24 | N07→N08 data：wp_page_copy提供保存的orig_pte值给pte_same | A23/A24 |
| E25 | N07→N24 data：调用方先读取pte_ptr指向的当前项，再按值调用pte_same；pte_same不自行解引用pte_ptr | A23/A24 |

E12的folio_alloc→folio_alloc_noprof别名在highmem_macro.h已补证。E13应读作明确压缩的间接路径，而非直接调用：folio_alloc_noprof→alloc_page宏→alloc_pages→__myos_alloc_pages。已回读完整宏和mempolicy.c，所有结果计数要保留这层区别。E15的引用增加实际位于__rmqueue_smallest，只在该子路径抵达时成立。

[VERIFIED mykernel/mm/highmem/highmem_macro.h::alloc_page]
```c
	#define alloc_page(gfp_mask)	alloc_pages(gfp_mask, 0)
```

[VERIFIED mykernel/mm/page_alloc/mempolicy.c::alloc_pages]
```c
	page = __myos_alloc_pages(gfp, order);

	return page;
```

这些修正足以消除当前消费歧义，不要求执行方重绘整批或新增第七件；不宣称其他边都已形式认证。

## 5. R17-S03：证据关联与风险强度

W17-A43题为zero特殊处理注释，但所截段落没有is_zero_pfn调用；引用相等不能证明该标题。以同一固定源码如下内容替代其zero过滤证据，相关S02及正文消费同时修正：

[VERIFIED mykernel/mm/fault/fault.c::vm_normal_page]
```c
	// if (is_zero_pfn(pfn))
	// 	return NULL;
```

A46只有说明性注释，不单独证明flush调用被禁用；关于具体ptep_clear_flush的断言应引用已有A28和所读完整wp_page_copy。原本所述局部同步缺口仍可保留，不能上升为全系统不存在锁/失效。

9条deferred记录统一用CORE-USER-WP-COW-17::W17-Fxx引用，它们不是9个已复现或互相独立的新bug。F01/F02须有分配及安装实际抵达、内容未被其他机制初始化等前提；F03为映射引用未闭合，不认定双重释放；F04为已选路径同步证据缺口；F05不认证可靠OOM；F06是既有共享WP停点的追加证据；F07按R17-S01改读；F08“返回0且未安装”本身也可能对应其他写者已处理，不能单独判错；F09只是已读__folio_put端点，不排除其他free API。

原报告把fork映射引用未闭合统称R16-S02，后继应定位到16回执§2复制链事实及§4图/目标初态限定，避免把一个更正编号当整套引用协议。旧问题不重新计数，不自行改旧状态或关闭风险。

## 6. 下一工作包

同轮发布CORE-USER-PAGE-REF-LIFETIME-18：限定普通order-0物理页，从所选初始入池、分配出队、page/folio引用操作，到直接free和空闲链再可用边界。重点区分“计数变了”“归零”“调用释放接口”“进入空闲结构”，不重跑17、不展开全MM、全内存启动或所有buddy阶的正确性证明。

04基线的MR-02已明确有限初值不支持PageBuddy全局恒假/永不合并等绝对句；18必须继承该限制并按需读取05后继处置，不重开调度选择或全树基线任务。主线写回仅新增本回执、37号及检查点，随后更新PR导航；C03b只在新工作包授予的独立新目录写入。继续阶段2、18号延期、28号同轮派发与36号角色分工，无代码修复、运行、合并或阶段3授权。
