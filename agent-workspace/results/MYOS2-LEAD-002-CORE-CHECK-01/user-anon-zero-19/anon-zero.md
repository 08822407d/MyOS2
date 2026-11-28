---
task_id: "CORE-USER-ANON-ZERO-19"
track_id: "MYOS2-LEAD-002"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra（Owner选择；实际后端未独立认证）"
date: "2026-11-01"
source_pin_short12: "a039d9803ade"
source_branch: "time"
required_review_record: "CORE-USER-PAGE-REF-LIFETIME-18-REVIEW-001"
task_contract_short12: "d4bffc1a4816"
result_base_short12: "15a162f13089"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
phase: 2
record_type: "bounded_static_anonymous_missing_PTE"
---

# CORE-USER-ANON-ZERO-19：匿名地址第一次缺页时真正建立了什么

## 最重要的结论

固定 `time @ a039d9803ade` 的匿名缺PTE路径真实存在。若VMA的 `vm_ops == NULL`、原PTE满足 `pte_none`、相关指针与权限前提有效，则 `do_anonymous_page` 在**非写入缺页**时用一个全局零页的PFN构造带SPECIAL标志的PTE；在**写入缺页**时则分配自己的folio，按VMA保护构造新PTE，最终调用 `set_pte_at` 写入（AZ19-A001/A002/A010/A014/A016/A020）。

**不能直接认定完整的匿名零页机制已正确。** 关键局限有三项：匿名写新页虽然请求 `need_zero=true`，实际预分配器却没有按它清零；两条匿名路径的PTE二次检查均被注释；`pte_mkspecial`只添加SPECIAL，不能替代检查RW位。三者都是条件性静态边界，并不是已测得的数据泄露、CPU错误或权限绕过。

本批没有重做17号已有PTE的 `wp_page_copy`、18号普通物理页分配器/引用全链，也没有运行内核。第18批主线回执的R18-S01/S02/S03、第17批R17-S01/S02/S03、第16批R16-S01/S02及第12批缺页返回限定均继续适用。

## Z01 · 第一次缺PTE与“匿名”的真实定义

调用方向：架构 `do_user_addr_fault`从错误码生成 `FAULT_FLAG_WRITE`，调用 `myos_handle_mm_fault`；后者保留 `vma`、地址对齐、flags，申请/定位页表并取得 `pte_ptr`，保存 `orig_pte`。只有该槽按软件 `pte_none(orig_pte)`为真，`handle_pte_fault`才分派至 `do_pte_missing`，它调用 `vma_is_anonymous(vma)`选择匿名路径（AZ19-A001–A005/A041/A043）。

本树 `vma_is_anonymous` **只检查 `!vma->vm_ops`**，而不是检查 `vm_file==NULL`、某个VM_ANON flag或系统调用名。新 `vma_init` 先将VMA清零、再主动给 `vm_ops`设置非NULL的dummy表，所以“刚分配VMA”也不自动等于匿名。所选无文件非共享 `simple_mmap_region` 调用 `vma_set_anonymous`，`do_brk_flags`也有这一赋值，并把 `vm_page_prot`设为 `vm_get_page_prot(flags)`。无文件共享映射则有单独的 `shmem_zero_setup` 分支（AZ19-A031–A040）。

因此矩阵同时保留“匿名谓词为假走 `do_fault`”和“匿名谓词为真且VM_SHARED时，本函数返回SIGBUS”两类情况；后者不是宣称正常MAP_SHARED匿名路径必会触发该拒绝。有效VMA/MM/PTE、权限检查与实际生产者仍是条件。

`myos_handle_mm_fault`在进入匿名分支**之前**已经调用一次 `pte_alloc`；`do_anonymous_page`又调用它。真正宏 `pte_alloc→pgtble_alloc`只在PMD为none时尝试分配。因此稳定且PMD已有PTE表时，不应把第二个调用理解成必定分配第二份表；若第二次返回非零，源码才直接返回 `VM_FAULT_OOM`。另外，外层 `pte_ptr==NULL`直接返回0，不能据此认证已经安装任何映射（AZ19-A003/A006/A009/A044/A045）。

原PTE的none不等于硬件Present位为0的唯一状态：`pte_is_none`有掩码，`pte_is_present`软件谓词接受 `_PAGE_PRESENT|_PAGE_PROTNONE`。这些前置语义受R16-S01限制，不把已有只读PTE误算成第一次缺席PTE（AZ19-A066/A067）。

## Z02 · 读缺页：零页从哪里来、PTE有哪些位

零页不是“物理PFN编号0”。固定代码中 `ZERO_PAGE(vaddr)` 实际返回 `virt_to_page(empty_zero_page)`；`empty_zero_page`在 `head_64.S` 以页对齐BSS段占用 `PAGE_SIZE`，链接脚本将 `.bss*` 放入 `__bss_start`—`__bss_stop`。 `x86_64_start_kernel`的活动C语句调用 `clear_bss`，后者 `memset`上述范围为0。另有 `kernel_init_freeable→do_pre_smp_initcalls→init_zero_pfn`，它执行 `zero_pfn=page_to_pfn(ZERO_PAGE(0))`（AZ19-A026–A028/A069/A072–A079）。

这条链**可证明有源码级零填充和PFN赋值操作**，但并不能证明真实机器已经执行、每个可达缺页都发生在赋值后、PFN实际数字多少、内存后来没有被其它写者改动或每次读取都得到零。 `my_zero_pfn(addr)` 只是返回 `zero_pfn`当前值，参数addr没有参与选择（AZ19-A071）。

读分支需要 `!(vmf->flags & FAULT_FLAG_WRITE)`。它计算局部 `entry=pte_mkspecial(pfn_pte(my_zero_pfn(address), vma->vm_page_prot))`，在 `pte_ptr`非NULL时直接 `goto setpte`。没有在本读分支为零页调用`folio_get`，也不增加零页数据引用计数；这只能说明**本选定分支**没有局部取得引用，不能证明其它零页使用者的全局引用协议（AZ19-A010–A013）。

SPECIAL与只读是**两个不同的PTE属性**。 `pfn_pte`从PFN及传入pgprot构造条目；`pte_mkspecial`最终是按位OR `_PAGE_SPECIAL`，并不清除 `_PAGE_RW`（AZ19-A022/A023/A057–A059）。所选VMA生产者 `vm_get_page_prot`按VM_READ/WRITE/EXEC/SHARED选择保护表；私有 `VM_READ`映射使用PAGE_READONLY，私有 `VM_WRITE`或READ|WRITE使用PAGE_COPY，二者在固定常量中都没有RW。因此在**这些正常私有初态且保护未改变**的前提下，读缺页生成的零页PTE为不可写（AZ19-A048/A051/A053）。若输入的 `vm_page_prot`本来已有RW，SPECIAL不会自动清除它——但当前材料**没有证明**一个非共享匿名读缺页会合法抵达这种异常权限初态，所以不把它叫作实测“可写零页”漏洞。

还须分清 `VM_NONE` 的特殊情况：PAGE_NONE模板没有硬件PRESENT；读分支并不主动OR入PRESENT，仅添加SPECIAL。若这种访问真的通过上游到达这里，不能由 `return 0`推论新条目已满足CPU访问要求；架构访问错误门在所选代码里处于注释，实际效果仍取决于其它权限及硬件前提（AZ19-A042/A054/A066/A067）。

## Z03 · 写缺页：独立新页、清零、权限及引用

当 `FAULT_FLAG_WRITE`为真且前面的VM_SHARED与pte_alloc门已越过，`do_anonymous_page`调用 `alloc_anon_folio(vmf)`，后者把 `need_zero=true`传给 `folio_prealloc`。但其活动主体直接 `return folio_alloc(GFP_HIGHUSER_MOVABLE,0)`：参数中的need_zero没有参与清零，原有零分配处理只存在注释中（AZ19-A023–A025）。

继续沿现有17/18限定接口，`folio_alloc`别名指向 `folio_alloc_noprof`，后者忽略自己的gfp/order，直接做 `page_folio(alloc_page(GFP_HIGHUSER_MOVABLE))`，而该GFP没有 `__GFP_ZERO`。已读 `post_alloc_hook`中按请求位进行的 `clear_page`只在 `__GFP_ZERO`存在时活动。故**即使写分支PTE正常安装，也没有本链已经清零新PAGE_SIZE数据的证明**；如果实际分配页包含可观察的旧内容且没有其它机制初始化，存在条件性暴露风险，绝不声明已发生泄漏（AZ19-A080–A086）。

对有效新folio，`mk_pte(&folio->page, vma->vm_page_prot)`建立局部PTE；只有 `vma->vm_flags & VM_WRITE`为真时才执行`pte_mkdirty`、`pte_mkwrite`，这两者都是增加标志位而不是重建全部权限。**VM_WRITE为假表示该分支不主动增加RW，不表示会主动清掉原entry里已有RW**。普通私有保护模板初始RW为0是另一条来源事实；不能混用。这也遵守17回执R17-S01的权限增位限定（AZ19-A016/A053/A061/A062/A065）。

成功路径只见 `set_pte_at→set_pte→WRITE_ONCE`发布新PTE；`folio_ref_add`、 `folio_add_new_anon_rmap`和LRU操作在这一函数内都是注释，`__folio_mark_uptodate`也未活动（AZ19-A015/A019/A020/A063/A064）。18已经证明所选buddy成功出队会增加页引用；但**本批不能因此判定新PTE所需的mapping/rmap/page生命周期账已配平**。本次正常安装后也没有把新folio立即put回去，不应把局部“持有”与“已释放物理页”相混。

失败与不安装出口逐条区别：

- **VM_SHARED门**：在二次pte_alloc和新folio分配之前返回 `VM_FAULT_SIGBUS`。
- **二次pte_alloc失败**：在读/写分流前返回 `VM_FAULT_OOM`，真实生产者还需PMD/分配条件。
- **IS_ERR(folio)**：检查为真则返回0而不安装PTE；但本批所读`alloc_anon_folio→folio_prealloc→folio_alloc_noprof`没有活动的ERR_PTR返回生产者，不能把注释的EAGAIN当已实现重试。
- **底层alloc_page失败**：`folio_alloc_noprof`先对返回值调用 `page_folio`，而`_compound_head`会解引用page字段。因此外层的 `if(!folio) goto oom`虽存在，仍不能保证NULL能安全抵达；不凭C未定义行为预言唯一硬件终态。
- **写分支pte_ptr NULL**：已获得有效folio时跳到release，`folio_put`后返回初值0，**没有安装新PTE**；18回执说明`__folio_put`当前活动主体为空，不据此说物理页已经重入空闲链。
- **读分支pte_ptr NULL**：直接到unlock返回0，也没有安装PTE。
- **PTE在原快照后变化**：读和写的`vmf_pte_changed`二次核验均为注释，只要指针仍有效并走到setpte，本函数就可能覆盖中间变化；这是有并发前提的静态风险，不是已复现竞态。

相关源码锚点为AZ19-A008–A022/A080–A090。上层 `do_user_addr_fault`对非零fault的 `while(fault!=ENOERR)` 不会重新调用处理器，故错误返回与0返回都不可自行推断为系统已正确重试或用户指令恢复（AZ19-A043/A046）。

## 已读零页后再写：只交接给第17批，不重算COW

在默认非共享私有VM_READ或VM_WRITE保护模板下，首次**读**缺PTE安装共享零页条目时，RW位为0。此后若发生写访问且产生WRITE缺页，`handle_pte_fault`只有在**现存PTE非none、当前值等于快照、WRITE置位、PTE不可写、对象有效**这些门均成立时才走`do_wp_page`。第17批已经研究它的`wp_page_copy`及零PFN分支；本批只提供生产侧身份和PTE权限接口，不声称后续私有复制一定正确，也不重复其旧问题（AZ19-A001/A030/A053/A058/A070）。

## 事实矩阵与后续限制

`facts-and-boundaries.yaml`列出20个条件场景，分别标记对象身份、有效前提、页表写入、引用、返回和unknown，17项effect把局部变量赋值与实际PTE WRITE_ONCE分开。`anon-zero-paths.dot`按相同ID表示活动call、data消费方→提供方、单独effect；历史R17-S02要求的调用与参数依赖区分已保留。

普通问题只在`deferred-findings.yaml`登记，区分新匿名路径的条件缺口与旧17问题的证据追加，不改旧文件/ID、不要求Owner决定修复。最主要的待核是匿名写新页清零、无二次PTE重检的条件竞态、特殊零页PTE在异常初态中可能继承RW，以及成功PTE映射的引用/rmap归属。所有运行、完整启动、真实数据内容、全局引用及锁协议仍为NOT_RUN/needs_evidence；本执行者本批提交后停写交C03验收。
