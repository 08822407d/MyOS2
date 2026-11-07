---
task_id: CORE-USER-VMA-MERGE-22
track_id: MYOS2-LEAD-002
record_type: bounded_vma_merge_analysis
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected; backend not independently attested)
date: 2026-11-02
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: cffb519d089b
result_base_short12: 16baace7bd3c
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
phase: 2
---

# CORE-USER-VMA-MERGE-22 — VMA合并候选、对象身份与边界副作用



## 核心结论与先决



固定`time @ a039d9803ade`中，`simple_mmap_region`先调用`simple_munmap_vma_range`处理旧映射，并得到`prev`；随后第一次调用`simple_vma_merge(mm,prev,addr,addr+len,vm_flags,file,pgoff)`。若返回非NULL，局部`vma`就是**旧合并目标**并直接跳到`out`，不分配新的VMA、不调用file.mmap回调、不重算新VMA保护。若第一次NULL，才分配临时新VMA并可能在文件回调之后进行第二次合并。最终函数正常成功返回的是`addr`地址而非VMA指针，成功控制流并不认证实际PTE/TLB或CPU映射（VM22-A027–A041）。



要特别合读21主线R21-S01/S02/S03：选址或旧映射处理没有被独立证明能消除全部重叠，`for_each_vma_topdown`从NULL开始时实际帮助函数存在`&vma->list`的C语言前提。后面的矩阵仅描述给定有效对象/执行能推进时的**局部分支**，不能把“列表有序有效”当作初始NULL问题已解决。14主线R14-S01/S02保留页偏移、摘链和错误回滚根因，20主线R20-S02/S04保留回调后对象被替换及字段未自动刷新限定。



## G01 · 合并入口和相容规则



`simple_munmap_vma_range`实际先执行`*pprev=find_vma_prev_to_addr(mm,start)`，随后调用`simple_do_vma_munmap(mm,start,start+len)`。其前驱查询严格使用`vma->vm_end < addr`；在`prev`有效且两次调用之间边界未变化的前提下，传给首次merge的`prev->vm_end`只能小于`addr`。但merge_prev的活动判断要求`addr == prev->vm_end`。两式不能同时成立，因此**在这条稳定首次生产路径不能直接证明前驱合并可达**。这不是声称`simple_vma_merge`被其它调用者传入相邻prev时也不会合并；属于旧VL14-F01的merge消费新证据（VM22-A001/A019/A055/A090）。



候选`curr`通过`find_vma_intersection(mm, prev?prev->vm_end:0, end)`赋值；这个查询本身先找严格前驱、只检查其后一项，存在21/14已审的漏检和初始NULL前提。若`curr==NULL`或`end==curr->vm_end`，`next=vma_lookup(mm,end)`，否则`next=NULL`。所选`vma_lookup`直接依赖`simple_find_vma`点包含谓词`vm_start<=end<vm_end`，所以`next`可能是**包含请求end的对象**，并不保证`next->vm_start==end`。因此不能由变量名`next`或Linux式注释推出真正的相邻后继（VM22-A017/A018/A055–A060）。



合并在输入`vm_flags & VM_SPECIAL`非零时立即返回NULL。一般相容帮助函数`simple_is_mergeable_vma`检查候选的`(vm_flags ^ requested_flags) & ~VM_SOFTDIRTY`、`vm_file`指针等同性，以及`vm_ops!=NULL && vm_ops->close!=NULL`。`VM_SOFTDIRTY`宏可能取0，所谓“忽略SOFTDIRTY位”在该配置下也可能无实际屏蔽效果。它未在所选函数中比较`vm_page_prot`、`vm_ops`指针身份或`vm_private_data`，但缺少这些比较**本身不足以证明错误合并**，还须实际差异生产者及对应不兼容语义（VM22-A006–A008/A079/A080/A093）。



`simple_can_vma_merge_after(prev,...)`除上述相容检查外，要求`prev->vm_pgoff + vma_pages(prev) == request_pgoff`；`vma_pages`是`(vm_end-vm_start)>>PAGE_SHIFT`。`simple_can_vma_merge_before(next,...)`要求`next->vm_pgoff == request_pgoff + pglen`（`pglen=(end-addr)>>PAGE_SHIFT`），未直接检查`next->vm_start == end`。两套比较是页号条件，不是字节区间邻接或文件数据正确性的充分证明。计算还需无回绕、有效页单位和VMA对象前提（VM22-A009–A013/A061/A066/A081/A091/A092）。



## G02 · adjust_target何时有值、真实目标是谁



`simple_vma_merge`在栈上声明`curr,*next,*adjust_target`而未初始化三者；只对`merge_prev/merge_next`设false、`vma_start=addr`、`vma_end=end`。`curr`和`next`若通过前述实际赋值完成才有确定局部来源。若前驱有效且相邻、after helper为真，就赋`vma_start=prev->vm_start`、`vma_pgoff=prev->vm_pgoff`、`adjust_target=prev`并标记merge_prev。若`next`有效且before helper为真，赋`vma_end=next->vm_end`，但**仅在`!prev`时才写`adjust_target=next`**，而不是在`!merge_prev`时写。最后无任何合并标志时返回NULL（VM22-A013–A023）。



因此各分支必须分开：**仅前驱匹配**（对独立直接调用的假设输入）时目标明确为prev；**仅后继匹配且prev==NULL**时目标明确为next；**双向匹配**时已有`adjust_target=prev`，后继只扩张最终end；而**仅后继匹配且prev!=NULL**时两种赋值均未发生，随后`if (adjust_target == NULL)`先读取未初始化指针。即使源码接着有`adjust_target=curr`的备用句，也不能在这条C未定义或未闭合路径上保证它真会选择curr、调用adjust或返回。`while(prev==NULL && curr==NULL && next==NULL)`在已至少命中prev或next并到达此处时不能作为目标指针有效性证明（VM22-A014/A020–A025）。



一个有边界的纸面模型是：旧前驱在`addr`之前存在但并不相邻，下一VMA在`end`处或包含`end`，文件/标志/页偏移门允许next合并。首次merge取得的prev通常正是这种严格较早对象，因此代码可以走入“只有next而prev非NULL”的待核路径；但上游严格前驱查找从NULL迭代的语言语义、旧VMA解除后的对象身份和next实际生产仍未闭合，不能断言实机必然触发崩溃。反之，如果prev==NULL且next兼容，赋给目标的是原先的next VMA，而不是新的内存对象（VM22-A001/A017–A025/A055）。



在`curr != NULL`的假设情形下，源文本允许`if(adjust_target==NULL) adjust_target=curr`，但只有**此前对adjust_target的读取已经具有定义良好的初值**，才能分析是否真的走入该分支。不能用`curr`存在来修补变量未初始化，也不能凭`curr`来自交集查询推断它一定和请求相容。第一次合并返回旧对象后，上层会直接跳到out，新建分支的`vm_get_page_prot`/`get_file`和文件回调均不执行；原对象的`vm_file`、`vm_ops`、`vm_private_data`及`vm_page_prot`不被合并函数显式替换（VM22-A024/A025/A028–A031/A062–A066）。



## G03 · 被保留VMA的字段、摘链与引用



`__simple_vma_adjust(target,start,end)`实际先设`target->vm_start=start`，再执行`target->vm_pgoff += (start - target->vm_pgoff) >> PAGE_SHIFT`，最后设`target->vm_end=end`。`vm_start`是字节地址，`vm_pgoff`按结构定义是页号，因此公式把两种单位直接相减，且与旧`vm_start`的差值无关。局部变量`vma_pgoff`虽在merge_prev赋值，却没有进入该adjust接口或写回。旧VL14-F02根因在**合并路径**得到新证据，不计为新缺陷（VM22-A002/A003/A019/A020/A061/A066）。



纸面例子（非实际运行）：直接给merge helper传入prev=[0x10000,0x12000)、prev.pgoff=1，请求[0x12000,0x13000)、pgoff=3，页数2使after相容。目标起点仍是0x10000，但adjust公式把pgoff从1改成`1+((0x10000-1)>>12)=16`，而按无起点移动的连续文件映射逻辑应维持1。另有仅后继且prev==NULL的模型：next=[0x30000,0x32000)、pgoff10，请求[0x2f000,0x30000)、pgoff9，before条件成立；adjust把next.pgoff改成`10+((0x2f000-10)>>12)=56`，而向前扩一页的连续映射期望是9。两例都须满足有效对象、目标已初始化、64位ulong、PAGE_SHIFT=12，**第一例并不能据此证明普通首次mmap生产出的prev确实相邻**。这些计算只用于展示单位公式的条件性后果，不声称真实文件已经错页（VM22-A003/A010/A012/A019–A022/A081）。



设置目标start/pgoff/end之后，adjust循环调用`vma_next_vma(target)`；只要`next!=NULL && end>=next->vm_end`，就调用`__vma_unlink_from_list(mm,next)`并重新取下一VMA。对仍保留但被尾部部分覆盖的next，源码另执行`next->vm_pgoff += (end-next->vm_start)>>PAGE_SHIFT`与`next->vm_start=end`，未摘链。被完全覆盖的后继在这个循环里**没有逐个再检查与目标flags/file/close兼容**；若上游其实没有消除一个不相容的重叠VMA，可能把它从链表中摘掉。但上游旧映射处理的实际状态和函数有效性仍是前提，不能宣布真实吞掉了某个文件区间（VM22-A004/A005/A049/A050）。



`__vma_unlink_from_list`在可用对象并成功通过列表包含循环时执行`mm->map_count--`和`list_header_delete_node`；后者只有成员存在才`list_del_init`并减少`mm_mt.count`。`list_del_init`改双链并初始化节点，没有`vm_area_free`、`fput(vm_file)`或`vm_ops.close`。被摘后继是否有file引用、完整资源归还与其他回收者均未审全，这部分是旧VL14-F03的合并路径追加，**不是已运行证实的永久泄漏**。如果对象或链表检查不返回，就不能继续断言计数已变化或后继已经摘掉（VM22-A069–A075/A084/A085/A094）。



对保留的合并目标，所选函数只改边界和pgoff；上层到out时按宏执行`vma->vm_flags |= VM_SOFTDIRTY`，而宏可为0，不自动重算`vm_page_prot`，也不新建PTE。对被摘后继，本次源码没有配套释放。对部分裁切的后继，源文本修改start与pgoff并保留节点。物理页、文件引用净额、VMA的其他保护/回调状态不能因列表操作而自动闭合。



## 二次合并：临时VMA不是最终目标



第一次merge失败后，simple_mmap_region分配新VMA，设置vm_start/end、vm_flags、vm_page_prot、vm_pgoff；若file非NULL，则先`vma->vm_file=get_file(file)`、调用`call_mmap(file,vma)`。成功回调与其它映射门之后，只有`vm_flags != vma->vm_flags && prev != NULL`才用**回调后的**`vma->vm_start/end/flags/file/pgoff`再次调用simple_vma_merge。这里`prev`仍是先前得到的本地前驱指针，不能自动当作新VMA的真正相邻前驱（VM22-A029–A035/A082/A083）。



若第二次merge返回有效旧VMA，源码执行`fput(vma->vm_file)`、`vm_area_free(vma)`释放的是**临时新建对象**，然后`vma=merge`把局部指针换为旧对象，走`unmap_writable`，不再`simple_vma_link`。若回调更换`vma->vm_file`且原引用处理未另证，就不能对文件引用写确定的净增减；也不能把上面的临时对象释放算到adjust内部摘链的旧next身上。第二次返回NULL时，保留新对象并按后续选中路径入链。回调能改flags不代表必定有一次新的合并成功，且成功后的旧对象属性不等于临时新VMA原`vm_page_prot`（VM22-A034–A041/A082/A084/A087–A089）。



合并之前的旧区间处理可能有已登记的不完整分割、摘链与非事务回滚；后续`vm_area_alloc`或`call_mmap`返回错误时，上层确有专用清理分支，但不能把那些清理套回此前旧VMA状态。第14批R14-S01明确在sys_brk正常错误出口会恢复其brk字段，故本批只对VMA链表/引用副作用保留条件限制，不重复旧版错误结论（VM22-A001/A027/A029/A042–A045）。



## G04 · 状态矩阵、根因归属与自查边界



`facts-and-boundaries.yaml`覆盖首次与二次调用、无合并、仅前、仅后、双向、严格前驱/邻接、next包含end但非真正邻接、flags/file/close/pgoff不相容、curr/adjust_target未初始化、全摘/部分裁切、新VMA回调后对象替换、file引用及正常/错误/不能继续推演的情形；附对象身份账与有限算术演算。图仅用于定位直接调用、间接回调、数据消费与字段副作用，不代表运行证明。



`deferred-findings.yaml`只新增3条条件问题：后继合并下未初始化target、后继邻接没有被单独验证，以及相容性未覆盖部分字段的待证假设。旧VL14-F01/F02/F03/F05各追加合并路径证据，**不将旧根因重复计数**。所有历史文件与内核保持冻结；运行验证`NOT_RUN`，不生成修复补丁、不预处理、编译、C/ASM或QEMU。提交后停写交C03按第21批回执的纠正口径独立验收，绝不自行开始下一批。
