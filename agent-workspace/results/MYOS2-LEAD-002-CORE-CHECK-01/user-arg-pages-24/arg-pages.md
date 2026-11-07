---
task_id: CORE-USER-ARG-PAGES-24
track_id: MYOS2-LEAD-002
record_type: bounded_exec_argument_page_static_analysis
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected; backend not independently attestable)
date: 2026-11-02
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: 5b81217fc614
result_base_short12: 299a960562a9
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
phase: 2
---

# 第24批 — exec参数页的取得、装表、引用与字节复制



## 核心区分与历史消费



冻结`time @ a039d9803ade`的选定链是`copy_strings`或`copy_string_kernel`→`get_arg_page(bprm,pos,1)`→`myos_get_user_pages(bprm->mm,pos,1,&page)`→已有PTE查页或新分配/创建PTE→`memcpy_to_page`→`put_arg_page`。该链至少有四类彼此独立的结果：GUP返回的状态值、`pages[0]`是否被赋有效page、page上的`_refcount/_mapcount`、以及实际成功的PTE与字节复制。一个条件成立，不能替其余条件签发成功（AG24-A001–A005/A028–A033）。



第23批主线已按`PASS_PENDING_LOCAL`限定接收；本批强制消费R23-S01/S02/S03，特别是`get_arg_page`局部`ret`为**int**，GUP返回声明为**long**，且不能把初始栈VMA前的所有参数块混为同一个查询地址。ES23-F02（GUP返回/页槽缺口）与ES23-F03（新页固定PTE保护）属于旧根因，本批只追加精确调用/资源证据。第18批R18-S01/S02/S03约束分配初值、folio转换、减一测零与资源实际回收，不能把引用减至零当成物理页必然归还。第12批缺页和第21批从NULL迭代、15的exec mm身份沿用，不重新展开22/23完整主体。



## A01 · get_arg_page与GUP：状态、输出槽分账



`get_arg_page`声明局部`page_s *page`而未给它初始化值；`mm=bprm->mm`（此阶段的exec临时mm，不自动等于当时`current->mm`）。实际调用`ret=myos_get_user_pages(mm,pos,1,&page)`，中间的`mmap_read_lock/unlock`封装活动函数体并未执行down_read/up_read；完成调用后测试`if (ret <= 0) return NULL`，否则`return page`。这里`ret`是int，而被调原型/定义是long，意味着即便将来有定义的long返回值，还存在赋给int的转换；当前更先遇到的是GUP在完整函数尾**没有任何return表达式**，不能用未证的ABI寄存器残值或假设成功页数补齐函数契约（AG24-A001–A005/A028/A033/A041/A042/A044）。



GUP参数依次是`mm_s*`、`virt_addr_t start`（固定类型定义为u64）、`ulong nr_pages`和`page_s** pages`。循环使用int i计算`addr=start+i*PAGE_SIZE`。唯一确认的exec实际调用传`nr_pages=1`、`&page`，只考核索引0；nr_pages=0或多页仅作为该帮助函数直接调用的局部模型，不声称本批查到了其它真实生产者。当前固定PAGE_SIZE为4096；页号/PFN、虚拟字节地址、页内偏移、page描述符都是不同对象（AG24-A003/A028/A029/A037/A038/A074/A081）。



每页先`simple_find_vma(mm,addr)`做**点包含**判定。若无包含当前`addr`的VMA，直接continue，跳过`pages[i]`赋值；这表示输出槽**未写**，不是已经被设为NULL。若存在VMA，先调用`myos_get_one_page_from_mapping`；若它返回非NULL，`pg`直接采用该page描述符（没有在此分支调用get_page）。若返回NULL，`pg=alloc_page(GFP_USER)`；分配NULL后仍进入末尾`pages[i]=pg`，可写入NULL；分配有效页时则先调用`myos_creat_one_page_mapping`，后直接调用`get_page(pg)`，不检查映射返回值，最后写输出槽。三类输出（未写、NULL、有效指针）都不能消除GUP缺return带来的上层状态不可定性（AG24-A029–A033/A040）。



更重要的是`get_arg_page`读取ret后，只在`ret<=0`时返回NULL，源码没有单独验证本地page值是否有效。由于C非void函数无return的前提尚未闭合，不能构造确定的`ret>0`路径继续宣布真实复制成功、内核崩溃或固定错误码。下面的已有页/新页分支和拷贝结果均作为**定义良好对象和相应调用确实抵达时**的局部源码账保留。



## A02 · 现有页查询与新建PTE必须分开



`myos_get_one_page_from_mapping`先让retval=NULL，再从mm及字节地址求各级页表项：PGD→P4D后检查`p4d_none`，随后PUD/PMD/PTE各层检查`*_none`；遇到这些none门就返回当前NULL。若一路通过，执行`page_paddr=arch_pte_addr(*pte)`及`phys_to_page(page_paddr)`，最终返回page描述符。`arch_pte_addr`剥离PTE标志位，`phys_to_page`将物理地址转PFN再转描述符。**这不是获得额外引用**：所读函数没有get_page，也没有其它增加`_refcount`的活动语句；各级指针本身的有效性、PTE present状态与最终硬件映射并未认证（AG24-A053–A058/A074/A075）。



`myos_creat_one_page_mapping`另一个方向按顺序获得`pgd_offset`、`p4d_alloc`、`pud_alloc`、`pmd_alloc`、`pte_alloc_map`。前三级有针对刚取得的p4d/pud/pmd指针的NULL分支，正常抵达fail标签会`return ERR_PTR(-ENOMEM)`；这些门之间可能已分配或发布若干上层页表，没有局部成组回滚。P4D帮助函数在所读架构定义里把PGD指针转成P4D指针；PUD/PMD帮助函数分别依据上一级none测试与分配结果返回NULL或对应槽地址。不能把名字叫alloc就推断每层总能成功，也不穷尽所有无效父指针情况（AG24-A045–A048/A052/A059–A061）。



最关键的**PTE层检查对象错误**是：源码执行`pte=pte_alloc_map(mm,pmd,addr)`，紧接着却执行`if(!pmd) goto fail`，不是检查`!pte`。`pte_alloc_map`固定宏展开为`pgtble_alloc_map`，后者当`pgtble_alloc`返回非零时可给NULL。此前PMD分支已经过滤pmd==NULL；在此前对象稳定的局部模型中，即使PTE分配返回NULL，也不会从该错误门跳到正常`ERR_PTR(-ENOMEM)`。如果page非NULL，后面还会先`atomic_inc(&page->_mapcount)`，再把`*pte=arch_make_pte(...)`。一旦pte不是有效可写对象，**不能继续推导有定义的PTE写入、返回值或确切机器崩溃形式**。这不同于P4D/PUD/PMD失败正常返回错误指针（AG24-A049/A050/A062–A064）。



在`page!=NULL`且pte对象合法、代码确实抵达写入的条件下，所选构造写入的是`pgprot_val(PAGE_SHARED_EXEC) | _PAGE_PAT | page_to_phys(page)`。该固定模板当前含用户位、RW位且未置NX，**没有从栈VMA的`vm_page_prot`读取模板**。这只为旧ES23-F03增加PTE构造的实参来源和顺序；不是实机RWX结果，更不能声称缓存/TLB及所有软件掩码已验证。`page->_mapcount`增量也发生在写PTE语句之前，二者不构成自动原子事务。上层GUP没有接收/检查创建返回的pte或ERR_PTR，仍可能在帮助函数正常返回错误指针后执行get_page并写pages[i]（AG24-A031/A032/A050–A052/A076）。



## A03 · 已有page和新page的持有/归还账



页描述符的`_refcount`与`_mapcount`是独立字段/语义。已有页分支从PTE转换到page指针，没有本地`get_page`或映射`_mapcount`操作；如果后续`get_arg_page`真的返回它，`copy_strings`和`copy_string_kernel`都在有效page的拷贝语句之后调用一次`put_arg_page(page)`。后者直接进入`put_page→folio_put`，对相应folio的`_refcount`执行减一并测零，而不是向PTE借用归还另一个计数。若已有page处于合法k>0引用且无别的加减，局部算例是**k→k−1**；这是一项有条件的持有契约缺口，而不是已证明发生双重释放（AG24-A005/A014/A021/A058/A065–A069/A073）。



新page路径的局部数字不同。`alloc_page(GFP_USER)`宏为`alloc_pages(gfp,0)`；所选`alloc_pages`活动转发`__myos_alloc_pages`，伙伴路径`__rmqueue_smallest`在成功从free_area取得order0页、执行后续步骤时使该page的_refcount增加1；如果来自稳定的自由状态0，得到1。GUP在创建映射帮助函数**返回控制流之后**额外`get_page`，又增1；复制成功归还一次`put_page`会减1。其纸面轨迹因此是0→1→2→1，前提是没有其它所有权写者、分配/映射帮助函数都正常返回且GUP有定义地把对象交给调用者。`_mapcount`的+1是另一条、仅在映射构造抵达时成立的账，不等于新得到一份临时_refcount（AG24-A031/A032/A050/A065–A071/A073/A079/A080）。



反过来，PUD/PMD分配失败而创建帮助函数正常`ERR_PTR(-ENOMEM)`时，GUP不读取该错误，依旧执行`get_page(pg)`并可能将该page放入输出槽；此时不能当成成功安装PTE。若PTE分配得到NULL、下游误检pmd后访问无效pte，则连“随后get_page一定发生”的保证也不存在。`folio_put`在计数减至零时所调用的`__folio_put`在本树中是空活动主体；因此既不能把减至零当成已进入伙伴空闲池，也不能从某一个持有缺口推出物理页必然重分配（AG24-A049/A052/A066/A072/A078）。



同一物理page被多个参数块重复触及也必须按每块独立结算：第一次新建可有额外get与put配对，第二次若查到已有映射就走无get的借用分支、却再次put；但连续字符串的块是否真使用同一物理页、page对象身份与页表仍需实际生产证据。GUP只检查点包含而未获得新的保护锁证明：`mmap_read_lock`活动体没有down_read；本批不据此断言全系统没有外部同步。R18-S01对于引用初态、调用实际抵达和竞态限制继续有效。



## A03 · 字符串按页分块的字节复制与清理边界



`copy_strings`先取得源`char __user*`与字符串长度、预先保存`pos=bprm->p`，然后一次性`bprm->p-=len`并检查argmin；这一步发生在所有参数页分块取得之前。每轮用当前pos的页内余量和remaining len确定`bytes_to_copy`，再依次减pos、str和len；随后调用`get_arg_page(bprm,pos,1)`。只有拿到**有效**page并继续执行时，才调用`memcpy_to_page(page,offset_in_page(pos),str,bytes_to_copy)`，再调用`put_arg_page(page)`；不能把代码先减少剩余len理解为这块的字节已经真正复制。NULL页出口返回`-E2BIG`，此前的bprm->p与已复制块没有在本函数中成对撤销（AG24-A008–A016/A037）。



`copy_string_kernel`同样先减bprm->p，但使用`min_not_zero(offset_in_page(pos),PAGE_SIZE)`选本轮上界，随后pos/arg/len推进、获取page、执行memcpy_to_page和put_arg_page；`copy_strings_kernel`只是逐字符串调用该入口。一个条件纸面示例：pos初值0x2008，len=24，copy_strings先把bprm.p改成0x1ff0；第一块取8字节到pos=0x2000的page偏移0，第二块取16字节到pos=0x1ff0的page偏移4080。前提是这两个pos都存在合法VMA和有效page，且copy调用顺利完成；示例不是内核执行踪迹。`memcpy_to_page`实际以`page_to_virt(page)+offset`为目的地址直接调用`memcpy`，页内VM_BUG_ON检查在此函数被注释。它不能凭调用名证明用户源指针已完成所有uaccess容错、物理数据实际可读/已清零（AG24-A017–A023/A034–A039/A043）。



`GFP_USER`的固定组合不含`__GFP_ZERO`；所读伙伴分配`post_alloc_hook`只在该zero标志满足时执行页面清零。因而不能把分配一个新参数页本身当作“剩余字节必然为0”，但这不是已复现的信息泄露。`copy_strings`直接把用户源指针交给上述`memcpy`而非活动的copy_from_user逻辑；是否需要额外保护、访问错误与完整源字符串语义另属uaccess范围。本批只保留这个调用依赖，不扩大整个用户访问机制审计（AG24-A008/A014/A034–A036/A077/A082）。



选定`free_arg_pages(bprm)`函数体为空；`free_bprm`会调用它，并且只在`bprm->mm`非NULL时调用`mmput(bprm->mm)`。所以某个参数块失败前分配的页、引用、页表或已拷字节，并不能由这个空清理函数保证被立即撤销。`mmput`及exec替换当前mm后的全局生命周期以前序15限定消费，不能由本批函数内没有free就断定页永久泄漏或失败后用户态必然看到半初始化参数。成功调用返回0和真正完整参数可用必须分别证实（AG24-A006/A024/A027）。



## A04 · 场景和问题归属、静态上限



`facts-and-boundaries.yaml`涵盖只读已有页、VMA未命中、页分配NULL、新分配正常、P4D/PUD/PMD不同错误、PTE分配NULL错门、映射错误被忽略、page输出的未写/NULL/有效三态、不同_refcount/_mapcount、同页重复分块、user/kernel字符串分块和先行指针/字节副作用。DOT用独立call、macro_use/macro_binding、data与effect关系及真实锚点，所有数值示例均为带前提的纸面算术。



`deferred-findings.yaml`登记5条本批条件性候选：已有页借用后统一put、PTE分配NULL被错检、映射错误被GUP忽略、copy失败的局部非事务状态，以及用户源指针直接memcpy的待核依赖。23已有ES23-F02/F03及17/18的folio零引用回收根因只作3组证据追加，不冒充新的独立根因。没有进行编译、预处理、C/ASM执行、QEMU/硬件测试、实际argv字节或page引用监测；只申请`READY_FOR_MAINLINE_REVIEW`及`mainline_acceptance=PENDING`。提交及双重远端回读完成后，本执行者停写交C03验收，不自行进入下一批。
