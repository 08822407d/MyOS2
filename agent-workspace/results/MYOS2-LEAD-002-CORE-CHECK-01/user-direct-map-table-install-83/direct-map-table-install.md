---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DIRECT-MAP-TABLE-INSTALL-83
record_type: bounded_static_direct_map_table_install_narrative
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-10
produced_by: GPT-6 (model self-report, backend unknown)
model_per_owner: GPT-6 + Extra (Owner-directed)
date: 2026-11-13
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
lead_pin_short12: b129d706e350
execution_start_short12: 3f41eb242040
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第83批：根项、PUD/PMD大页转换、PTE落表和计数

本批只消费固定`time@a039d9803ade`的真实建表链，不运行内核、不修代码。上批82已经由C06裁定`PASS_PENDING_LOCAL`，必须合读R82-S01/S02/S03；第81批R81-S01/S02/S03、79/80及其历史限定保持。**无D00、不重审81/82，不重新关闭任何旧代码候选或文档。**119条新锚(A38/B36/C45)以11份源码的单一定义内1–5原始行登记在evidence.md，facts用独立事实键明确内容及条件；一个锚不等于完整运行行为。

## G01/A｜根项地址域、已有/新子表与父项发布

上层`kernel_physical_mapping_init`宏把物理字节起终点、候选掩码与prot传入`__kernel_physical_mapping_init(...,init=true)`；`kernel_physical_mapping_change`则以`PAGE_KERNEL`和`init=false`调用同一下层。这是**init写模式的参数差异**，不是`after_bootmem`阶段标志的替代（证据A01/A02/A03；事实键：mapping_macro_init_true,change_macro_init_false,root_signature）。

根函数先设`paddr_last=paddr_end`，将物理字节地址用`__va`构造`vaddr/vaddr_end`，而循环的根槽来自`pgd_offset_k(vaddr)`，间隔用`(vaddr & PGDIR_MASK)+PGDIR_SIZE`求下一根界。所读常量给出PGDIR_SHIFT=39即512GiB；这些是**虚拟**根槽和边界，不是页表已经装入CR3的证明（证据A04/A05/A34/A35/A31；事实键：root_initial_addr_conversion,root_loop_and_boundary,root_shift,root_size,physical_to_virtual_macro）。实际`init_mm.pgd`通过`swapper_pg_dir`指向`init_top_pgt`数组，`pgd_entp_in_mm`按`pgd_index`加偏移；`phys_p4d_init`是直接折叠到`phys_pud_init`的宏，`p4d_offset`亦只是把根槽转成p4d_t指针，并不自动分配独立P4D页（证据A26/A27/A28/A11/A10/A15；事实键：init_mm_root_pointer,swapper_root_alias,init_root_array,pgd_in_mm_pointer,folded_p4d_macro,folded_p4d_cast）。

已有根分支的条件是`pgd_val(*pgd)`**非零**，不是显式调用`pgd_present`或`pgd_none`的合取。它借`pgd_page_vaddr`（别名`p4d_page_vaddr`）按页框掩码并`__va`回到子表虚址，递归到PUD后直接continue；如果非零值并非有效非叶表项，**证明必须在第一次未证的页指针访问停止**，不能从非零宣告合法内存（证据A06/A23/A24/A25；事实键：root_reuse_existing_nonzero,pgd_nonzero_alias,root_existing_vaddr_alias,p4d_pointed_page_virtual）。新根分支调用`alloc_low_page`，先经折叠P4D/PUD递归建立子表，再持`init_mm.page_table_lock`调用`p4d_populate_init`发布父槽；页源/清零/失败的前提沿81的PT81限定，锁范围只按函数内文本声明，不认证多核同步（证据A07/A08/A38；事实键：root_alloc_before_child,root_publish_after_recursion,one_page_alloc_wrapper）。

PUD层以`pud_index(__va(paddr))`选槽，PMD层以`pmd_index(paddr)`选槽；所读`PAGE_OFFSET=0xffff800000000000`的低于30位部分全零，在**声明64位、不回绕、选中四级常量**下两者对应的低30位索引与物理低位一致，但不能把`__va`和`__pa`无条件当成逆函数。尤其物理字节`paddr_start/end`、虚拟`vaddr/vaddr_end`、PFN以及子表`pud_t*/pmd_t*/pte_t*`绝非同一个单位（证据A19/A20/A30/A33/A32；事实键：pud_index_formula,pmd_index_formula,page_offset_base_constant,physical_retranslation_arithmetic,virtual_to_physical_macro）。在声明模型中物理[512GiB-2MiB,512GiB+2MiB)跨两个PGD槽；真实地址/表页合法性不由模型证明。空/反向/回绕输入也只在最早非法门止步。根`paddr_last`最初就是`end`，经合法递归才可能被后续返回覆盖，结尾`sync_global_pgds`注释行不作活动同步证据（证据A09；事实键：root_return_and_sync_comment）。

G01交付table_identity_and_address_matrix、root_and_child_selection_matrix和publication_order_matrix。若分配失败、已有表页失效或子递归不返回，**不能继续画成父项已发布**。

## G02/B｜PUD/PMD已有大页、递归或新叶的选择

PUD用`pud_none`判断空，再对非空项用`pud_leaf`区分已有表页或大页；PMD同理。`pud_leaf/pmd_leaf`实际测试`_PAGE_PSE`，none谓词将`_PAGE_KNL_ERRATUM_MASK`作为特殊忽略位，这些谓词不是同一个条件，也不等同于当前硬件present认证（证据B20/B22/B26/B24/B21/B23/B27；事实键：pud_none_alias,pud_leaf_alias,pud_leaf_tests_pse,pud_none_ignores_erratum,pmd_none_alias,pmd_leaf_alias,pmd_leaf_tests_pse）。

**非空但非叶项先递归：** 对已有PUD表页，使用`pmd_offset(pud,0)`取得PMD表基址，再调用`phys_pmd_init`；PMD已有非叶项则经`pmd_page_vaddr`访问PTE表并调用`phys_pte_init`。即使请求mask允许1GiB/2MiB，也不会在这些分支先把非叶表转换成更大的叶。旧子表存储和映射有效性仍需证明（证据B04/B13/A21；事实键：pud_existing_child_recursion,pmd_existing_child_call,pmd_offset_real）。

**已有huge叶可复用，也可被降级：** 若当前已有1GiB PUD叶且`page_size_mask`含1G位，或已有2MiB PMD叶且含2M位，则保留已有叶并将`paddr_last`设为本层`paddr_next`；`pages++`只在`!after_bootmem`时发生（证据B05/C11/B14/C08；事实键：pud_large_reuse_or_old_prot,pud_existing_leaf_count_early,pmd_large_reuse_vs_downgrade,pmd_existing_leaf_count_early）。若所需大页候选不允许，源码将旧PUD/PMD表项强转为`pte_t`，用`pte_clrhuge`清PSE，再用`pte_pgprot`取旧标志作为下一层prot，然后申请小一级表页，**先递归填充后发布**新父表项（证据B08/B17/B28/B30；事实键：pud_alloc_child_and_recursion,pmd_alloc_child_after_oldleaf,remove_pse_from_pte_cast,pte_to_pgprot_macro）。这不是把旧物理页框原样抄进所有子叶：实际新叶使用当前请求的`paddr`换成PFN再构造，而非用旧巨大叶中的物理页框字段，只有旧映射与请求物理区间一致并且类型/掩码合法时才能论证等价。注释所说的属性保留和Intel/Xen说明不构成本包外部认证。

**新huge叶：** PUD允许1G候选时走`set_pud_init(pud,pfn_pud(paddr>>PAGE_SHIFT,prot_sethuge(prot)),init)`；PMD允许2M时走`set_pmd_init`和`pfn_pmd`。这两种构造函数都对物理页框使用各自的1GiB/2MiB掩码，再合并`check_pgprot`过滤的prot；`prot_sethuge`把PSE位置1且对PAT做WARN语句，不能因WARN或函数名认定停止或权限完全一致（证据B06/B15/B31/B32/B35/B34/B19/B36；事实键：pud_new_large_leaf_choice,pmd_new_large_choice,pud_leaf_frame_mask_and_flags,pmd_leaf_frame_mask_and_flags,physical_1g_frame_mask,physical_2m_frame_mask,prot_add_pse_warning,page_protection_supported_bits）。各层`paddr_next`由本层对齐后的物理范围上界计算，可能**超过传入的精确`paddr_end`**；上游82正常分段通常可提供对齐条件，独立助手收到异常或短区间输入的模型不可冒充正常实际启动。原页表叶是否释放、旧PFN及PAT/NX等位与新表完全一致，不在所读局部已证明范围。

G02的三矩阵（existing_entry_transition、huge_to_child_mapping、protection_and_frame）及35组模型中的B部分，明确每层入口`init`、`after_bootmem`、mask、已有项种类、物理字节单位与callee返回。复用、重写和子层降级必须分别记账。

## G03/C｜4KiB PTE与写入包装、返回及计数

`phys_pte_init`的`paddr_last`先设为**传入的paddr_end**，索引和`pte=pte_page+pte_index(paddr)`在前，循环从该索引一直遍历至`PTRS_PER_PTE=512`。每轮先计算`(paddr & PAGE_MASK)+PAGE_SIZE`，然后才用`if(paddr>=paddr_end) continue`过滤范围（证据C01/C02/C41/C44；事实键：pte_signature_and_last_default,pte_start_slot_and_next,pte_table_512_slots,pte_index_formula）。因此地址/指针形成必须先合法，后置range判断不能代替它。正常索引511处最后一轮更新可能形成尾后一指针，但只凭这种**地址形成**不能称已越界访问；若末页为部分页且实际被写入，`paddr_last`采用下一个4KiB边界，可能大于精确`paddr_end`。这些均为下层直接助手的隔离输入，不证明82规范化调用真的提供过异常端点。

PTE已有非none项时直接复用、没有新叶store；仅`!after_bootmem`时`pages++`。PTE为空时，无论早期/晚期都`pages++`，使用`pfn_pte(paddr>>PAGE_SHIFT,prot)`及`set_pte_init(...,init)`写入，再把last更新为下一页边界。最后`update_page_count(PG_LEVEL_4K,pages)`并返回last（证据C03/C04/C05/C28；事实键：pte_existing_count_or_skip,pte_new_leaf_write,pte_new_last_count_return,pte_pfn_frame_mask_and_flags）。若区间全部复用，函数可能返回初始end而无新写；不能据last证明这些项是本次创建的。

**写入包装确实有实现。** 固定time中的`set_pte_init`、`set_pmd_init`、`set_pud_init`及`p4d_populate_init`、`pud_populate_init`、`pmd_populate_kernel_init`全部找到实际定义；`init=true`选择*_safe实现，false选择普通实现（证据C14/C15/C16/C17/C18/C19；事实键：pte_init_bool_wrapper,pmd_init_bool_wrapper,pud_init_bool_wrapper,p4d_populate_bool_wrapper,pud_populate_bool_wrapper,pmd_populate_kernel_bool_wrapper）。安全路径并非遇到冲突即返回失败：本树`set_pte_safe`、`set_pmd_ent_safe`、`set_pud_ent_safe`和`set_p4d_ent_safe`是在`WARN_ON_ONCE`后继续调用普通store；普通实现最终用`WRITE_ONCE`赋值（证据C20/C21/C22/C23/C24/C25/C26/C27；事实键：pte_warn_then_write,pmd_warn_then_write,pud_warn_then_write,p4d_warn_then_write,pte_write_once,pmd_write_once,pud_write_once,p4d_write_once）。P4D/PUD/PMD populate-safe别名到真实fill_*_safe，后者通过`__pa(child)|_PAGE_TABLE`构造父槽；地址合法、告警行为及并发安全另立前提（证据C29/C30/C31/C32/C33/C34；事实键：p4d_safe_parent_writer,pud_safe_parent_writer,pmd_safe_parent_writer,p4d_populate_safe_alias,pud_populate_safe_alias,pmd_populate_safe_alias）。不能将`init`当作`after_bootmem`，也不能因名字含safe就说会拒绝覆盖。

**四层返回值不同于计数器。** PUD与PMD层初值亦为各自`paddr_end`；大页复用/新建后返回本层next边界、下钻后取子层最终返回；根层取最后一次合法递归的返回，最终上层`init_memory_mapping`逐MR覆盖ret再右移PAGE_SHIFT（证据B01/B10/B07/B16/A09/C45；事实键：pud_signature_initial_index,pmd_signature_initial_slot,pud_large_write_return,pmd_new_large_write_return,root_return_and_sync_comment,upper_return_consumption）。相等、空区间、某层全部复用或只有部分新建必须分开；末端值不代表整个请求已建立映射。

本树`direct_pages_count[PG_LEVEL_NUM]`由`update_page_count`在`pgd_lock`范围内累加；PTE/PMD/PUD均有各自更新入口。早期`after_bootmem=0`时已有非空叶复用仍计入该层pages，晚期复用不计；**新建叶不论阶段都计入**。已有子表递归只让下层另行计数；降级旧大页时，在本链局部所读更新点没有看到自动扣掉旧层先前计数。这使它不应被直接称为全局去重的“现存映射总页数”，但不推出全树无其它计数写者、实际内核已经发生重复统计或必定溢出（证据C06/C07/C03/C08/C09/C11/C12；事实键：per_level_count_array,counter_lock_and_add,pte_existing_count_or_skip,pmd_existing_leaf_count_early,pmd_new_leaf_counter,pud_existing_leaf_count_early,pud_new_leaf_counter）。

G03交付五张矩阵（pte_loop_and_store、init_write_interface、return_endpoint、page_count、cross_layer_install），所有样例明确`after_bootmem`、`init`、`page_size_mask`、当前项类型、物理字节区间、callee返回和最早未证点；不把条件性模型当实际用户程序、内核或硬件输出。8条候选TI83-F01–F08、10条缺口TI83-U01–U10都保持needs_evidence，旧DM82/PT81/MR79/BM80/PB78/PA77及MC/LB/GAP/05/MR-02、58/63历史限定不变。

## 证据质量和交接

本批119条固定源码短锚（A38/B36/C45），11份直接源码；11张矩阵78行、35个有明确阶段输入的隔离条件模型及静态关系图56节点/77条类型边，不相加为独立的实机见证。事实键绑定`path::symbol`、真实定义内原文、证据ID；正文引用按事实键校验，图中共享节点及边按CALL、MACRO、CALLER_SEQUENCE、DATA、CONDITION、ANALYSIS_STOP、HYPOTHETICAL_SEQUENCE区分，**不是完整CFG或运行结果**。

完整本地P9、预处理、编译/链接、Graphviz、QEMU、实机、TLB、并发和硬件行为均为NOT_RUN。本执行者只在原执行分支/PR17新增本83规定的六件与本人一次顶层正式交付评论。六件固定/活动全文读回、旧→新compare、四direct refs、PR16/17及评论唯一性都核完才STOP_WRITING。完成时交原**MYOS2-A-C06 内核分析主线续建（Pro）**独立验收；`READY_FOR_MAINLINE_REVIEW`且`mainline_acceptance=PENDING`，不自签83、不启动84，不要求Owner逐项判断技术缺口。
