---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ANON-ZERO-19-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-01
read_channel: connector
authorization_ref: Owner报告CORE-USER-ANON-ZERO-19完成；28号同轮验收及下一工作包持续授权
reviewed_pr: 17
reviewed_head_short12: 6166b3e2f5b9
reviewed_base_short12: 15a162f13089
contract_short12: d4bffc1a4816
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 13bfea01f2b0
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R19-S01, R19-S02, R19-S03]
acceptance_scope: 已读匿名缺PTE分派、选定VMA生产者、零页初始化文本、保护构造及匿名新页安装和返回的静态证据；须合读本回执
kernel_zero_page_or_permission_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_90_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-MMAP-PROTECTION-20
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/39-user-mmap-protection-contract.md
inputs_read:
  - PR17交付评论issuecomment-6558160523及固定6166b3e2f5b9六件；正文截断尾部另读76至末尾，evidence分1–290/291–580/581–855，facts分1–400/401–800/801–1200/1201–1615至末尾。
  - 固定time fault.c 770–900及1120–1390；vm_map.h 1–230；mmap.c 1–240、400–1000分段；util.c全文；pgprot.c全文；pgalloc.h全文。
  - 固定time pgtable_arch.h 640–715和850–1100；pgtable_const_arch.h 1–300请求的可见部分包含位常量、SOFT_DIRTY条件与PAGE模板，截断的后部不声称完整已读。
  - 固定time head64.c 1–200、head_64.S 400至末尾、kernel.lds全文、linkage.h 1–110、init/main.c 230–350、pgtable_macro_arch.h全文及pgtable.h 1–165/390至末尾；pgtable.h 540之后为空不解释成文件为空。
  - 同一固定对象的options_flags、init_zero_pfn、folio_prealloc、mempolicy、GFP组合、page转换、get/put与架构缺页返回等沿用本对话此前实际读取；不声称本轮重新通读全部依赖。
  - 12主线回执1–140及13回执70至末尾用于明确已核边界；38号、18回执及18/24/25/28/36规则沿用已加载版本。
  - 默认分支__PAGE_ALIGNED_BSS和vm_mmap_pgoff搜索仅导航，相关定义回固定time；mmap系统调用搜索尚仅作20入口导航，不认证其固定绑定已闭合。
  - PR16/17、直接refs、开放PR第三项为空、PR16完整变更路径及19增量compare。
review_limits:
  - 完整交付阅读及关键路径语义复核，不是执行方90条引文逐字检查、JSON/YAML解析或48条图边的独立机械复现。
  - 未执行命令、文本程序、仓库脚本、哈希、预处理、编译、C/ASM、内核/QEMU或硬件实验。
  - 未证明实际BSS字节、完整引导及所有缺页时序、所有pgprot生产者、全局锁/引用、CPU权限执行或TLB一致性。
  - 同厂分角色审查不等于异族模型或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10及普通延期项不关闭。
---

# 第19批限定接收：输入保护、局部PTE和真正发布分别理解

## 1. 交付与裁定

第19批按PASS_PENDING_LOCAL接收，无新的执行退回。R19-S01/S02/S03直接提供必要限定和替代证据，不要求C03b单独返工，不修改原六件、内核或用户程序。

交付评论与PR17头一致为6166b3e2f5b9。相对15a162f13089，GitHub compare为1个后继提交、仅user-anon-zero-19/六件A新增，共2919行，无历史文件净修改/删除。文件行数为MANIFEST 59、正文87、facts 1615、DOT 103、deferred 200、evidence 855。PR17保持open/Draft。

执行方报告90个锚点、47节点/48边、20场景、17项effects、4项新路径条件发现与4组历史追加，以及12次双重回读；这些数量与机械成功是执行者记录，不自动成为主线全量复验。主线已阅读六件并核下述关键源码，接受范围不包括全部图边形式性质或运行正确性。

## 2. 可保留的主链与前提

匿名判定的实际定义是!vma->vm_ops；vma_init清零后仍给vm_ops设置dummy表，因此新分配对象不自动满足匿名谓词。所选simple_mmap_region新VMA的无文件非共享分支明确调用vma_set_anonymous；既有VMA合并、文件回调和其他生产者不能默认与该新建路径相同。

固定do_anonymous_page先拒绝VM_SHARED，再检查pte_alloc。非WRITE路径以my_zero_pfn、vm_page_prot构造并增加SPECIAL；WRITE路径申请folio并按VM_WRITE条件增位。两者只有到达共同setpte标签且实参有效、调用正常完成才实际写PTE。源代码中的重检查、PTL获取、rmap/LRU操作不能从注释补成活动执行。

零页的PAGE_SIZE对象在head_64.S中由__PAGE_ALIGNED_BSS声明，链接脚本覆盖.bss*；head64.c的x86_64_start_kernel实际调用clear_bss。另已核kernel_init_freeable→do_pre_smp_initcalls→init_zero_pfn，ZERO_PAGE通过empty_zero_page的地址求page描述符，最终赋zero_pfn。支持的是源码级初始化及对象关系，不是实机全零、所有缺页均在初始化后或没有后续写者的保证。

匿名写分支传need_zero=true，但所读folio_prealloc活动体未消费它。固定GFP不含__GFP_ZERO、底层清零门及NULL转换问题依17/18限定继续成立；不能据此认定真实泄漏或唯一崩溃终态。新PTE安装不自动完成引用/rmap账；folio_put与物理入空闲链仍须分开。

## 3. R19-S01：可达条件不是发生概率

facts的M09称稳定PMD使第二次分配失败“unlikely”，M16称稳定路径下NULL“unlikely”。撤回概率措辞，改为下列明确前提：

[VERIFIED mykernel/mm/fault/pgalloc.h::pgtble_alloc]
```c
		pgtble_alloc(mm_s *mm, pmd_t *pmd_entp) {
			return (unlikely(pmd_none(*(pmd_entp))) && __pte_alloc(mm, pmd_entp));
		}
```

在同一有效PMD于此处仍非none时，左项为假，返回0且不调用__pte_alloc；不是“较少失败”。第二次非零出口须另有PMD状态变化、不同有效输入及实际分配失败等证据。源码中的unlikely是表达式包装，不是本轮测出的概率。

myos_handle_mm_fault在分派前已拒绝NULL pte_ptr：

[VERIFIED mykernel/mm/fault/fault.c::myos_handle_mm_fault]
```c
	if (unlikely(vmf.pte_ptr == NULL))
		return 0;
	vmf.orig_pte = ptep_get_lockless(vmf.pte_ptr);
```

在已读正常直接调用、定义良好且vmf.pte_ptr字段未被改写的前提下，后续匿名读/写的NULL检查不会命中。M10/M16及相关正文、X11/X14、旧问题追加只能作为局部备用分支消费，不能据其存在就报告正常调用已发生该失败。

另一个写者改变*pte_ptr的内容，不等于把栈上vmf.pte_ptr变量变成NULL；非NULL也不证明该指针仍有效。PTE值变化、指针字段变化、指向对象失效必须分列。这不否认局部return0未安装的源码出口，也不宣称已穷尽全树调用者或并发行为。IS_ERR出口仍须区分检查存在与错误指针生产者已证实。

## 4. R19-S02：保护位按实际构造顺序限定

pte_mkspecial只OR入SPECIAL，不主动清RW，这个局部事实保留。但M06/M18、X05、F03及正文中“输入vm_page_prot带RW就保留RW”不能跳过pfn_pte内部的保护处理：

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::pfn_pte]
```c
			pfn ^= protnone_mask(pgprot_val(pgprot));
			pfn &= PTE_PFN_MASK;
			return __pte(pfn | check_pgprot(pgprot));
```

[VERIFIED mykernel/arch/x86_64/mm/pgtable/pgtable_arch.h::massage_pgprot]
```c
			if (protval & _PAGE_PRESENT)
				protval &= __supported_pte_mask;

			return protval;
```

替代口径：传入保护先经check_pgprot/massage_pgprot；PRESENT输入受__supported_pte_mask筛选，之后pte_mkspecial才保留其输入PTE的位并增加SPECIAL。假设性的可写零页还须RW实际经过构造后仍存在、路径抵达、PTE有效及相应权限生效；不能只引用原始vm_page_prot。这里不反向宣称支持掩码必然清RW或该假设已经由真实生产者建立。

默认私有PAGE_COPY/PAGE_READONLY模板无RW，以及VM_WRITE为假不会主动清RW的结论仍保留。PAGE_NONE不主动变成硬件present；DIRTY/SOFT_DIRTY的宏值和配置分别处理。请求的PROT_*、归一化后的VM_*、vm_page_prot和最终PTE不是同一个状态：本批模板分析不能作为完整mmap权限强制执行的认证，下一批补其上游。

## 5. R19-S03：证据关联、赋值条件和零页对象

- AZ19-E45所述do_anonymous_page→folio_put调用，原anchors误含AZ19-A039（brk匿名赋值）。替换为[AZ19-A017, AZ19-A022, AZ19-A087]；DOT同ID合读，不把旧错引计为额外证据。正文SPECIAL段中A022/A023亦不是该权限操作依据，应使用A057/A058/A059，构造掩码再合读R19-S02。
- E36是接收alloc_anon_folio返回值、赋给局部folio变量。赋值发生的门是调用正常返回并抵达赋值，不仅限返回有效非NULL；随后的IS_ERR、NULL及合法对象使用门分别记录。不能把“赋值完成”画成“分配已验证成功”。
- M08的初始化前状态只表示“缺少本次路径已经完成zero_pfn赋值的证据时，my_zero_pfn读取当前全局值”。empty_zero_page的BSS归属不自动证明另一个整数zero_pfn的最终链接段、实际值或硬件已清零；二者是不同对象。

补充原锚点中仅按名字推导的段绑定：

[VERIFIED mykernel/include/linux/kernel/linkage.h::__PAGE_ALIGNED_BSS]
```c
	#define __PAGE_ALIGNED_BSS .section ".bss..page_aligned", "aw"
```

该宏与所读head_64.S、kernel.lds及clear_bss合读后，支持选定源码/链接契约下的零填充路径；没有执行预处理、链接或启动，不能给实际布局与内存内容认证。

四项新路径发现继续使用CORE-USER-ANON-ZERO-19::AZ19-Fxx。F01是匿名写路径对既有need_zero根因的扩展，F02为特定并发前提的覆盖风险，F03按本回执收窄，F04仍是映射持有证明缺口而非泄漏结论。四组历史追加保持原ID和状态，不将八条记录合计为八个新缺陷。引用到其他回执的related只作明确说明的关联，不凭编号构成因果证明。

## 6. 同轮下一步

发布CORE-USER-MMAP-PROTECTION-20：从真实mmap入口、prot/flags及文件访问模式，追到do_mmap归一化、VMA标志与vm_page_prot生产和回调后的变化。只核这条权限传递链，不重做19匿名缺页、17COW或18分配器，也不展开完整地址选择、VMA拆分、mprotect、exec、文件系统或硬件权限审计。

主线预检已见do_mmap存在PROT_READ相关的PROT_EXEC增位和file/no-file分支，simple_mmap_region包含新建/合并及文件回调路径；但真实系统调用绑定和整条生产链尚未完成审查，正是20要取证的内容，不在本回执提前认定其正确性或漏洞。

本轮仅新增本回执、39号任务及恢复检查点并同步PR导航；执行成果冻结。继续阶段2、18号延期、28号同轮派发和36号C03b分工，无内核修改、补丁、编译运行、合并关闭/删分支/强推或阶段3授权。Owner无需当前技术修复决定。
