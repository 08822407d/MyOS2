---
task_id: CORE-USER-EXEC-STACK-23
track_id: MYOS2-LEAD-002
record_type: bounded_exec_user_stack_narrative
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected; backend not independently attested)
date: 2026-11-02
source_branch: time
source_pin_short12: a039d9803ade
task_contract_short12: c9be5ac03550
result_base_short12: b32f3ebc9095
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
phase: 2
---

# CORE-USER-EXEC-STACK-23：exec用户栈的生产、准备、扩展与缺页消费



## 总体结论：五种不同的“栈已就绪”命题



固定time源码提供的是分阶段接口链，而不是一笔原子、完整的栈安装事务。第一层由`bprm_mm_init`在**临时bprm->mm**中创建、入链一个单页VMA；第二层参数复制通过get_arg_page/GUP尝试取得实际page并拷入字节；第三层`begin_new_exec→exec_mmap`把新mm装到`current->mm`；第四层`setup_arg_pages`计算用户栈指针、局部权限与VMA向下扩展；第五层运行期缺页可从VMA定位到`myos_handle_mm_fault`，但不保证能找到栈起点以下的目标，也不由返回0直接证明PTE或CPU初始RSP有效。内核任务栈、bprm->p、VMA的[start,end)、真正物理页面、PTE保护和最后用户寄存器必须分别核算。所有运行验证`NOT_RUN`，本报告仅申请`READY_FOR_MAINLINE_REVIEW`，主线验收`PENDING`。



## S01 — 单页临时栈VMA的创建与发布



`alloc_bprm`实际调用`bprm_mm_init`；后者调用`mm_alloc`设置`bprm->mm`，并在成功分配后进入`__bprm_mm_init`。固定`mm_alloc→mm_init`初始化mm的`mm_mt`列表头、`mm_users/mm_refcount`及`map_count=0`等必要字段；此时正常调用者的`current->mm`仍为旧地址空间，**不是**刚分配的`bprm->mm`。若mm_alloc失败，没有新VMA；若__bprm_mm_init失败，外层清`bprm->mm`并调用`mmdrop`，不能借这个调用推出全仓内存完全回收（ES23-A005–A009/A043/A081–A083/A115）。



`__bprm_mm_init`中的`vm_area_alloc(mm)`分配VMA并执行`vma_init`：memset清零、初始化list、置`vm_mm=mm`、先赋非NULL dummy `vm_ops`；随后`vma_set_anonymous`将其`vm_ops=NULL`。所以原来被清零的`vm_file`、`anon_vma`等是按此生产者得到的0，而不是随机未知。`vm_page_prot`、起止、flags和页号则须追后续明确赋值，不可把默认清零说成已经初始化最终栈语义（ES23-A009/A010/A046/A084–A086/A103）。



成功越过写锁包装的选定门后，初始`vma->vm_end=STACK_TOP_MAX`、`vma->vm_start=vm_end-PAGE_SIZE`、`vma->vm_flags=VM_SOFTDIRTY | VM_STACK_FLAGS | VM_STACK_INCOMPLETE_SETUP`，随即`vma->vm_page_prot=vm_get_page_prot(vma->vm_flags)`。`VM_STACK_FLAGS`按所读默认宏有`VM_GROWSDOWN`、`VM_READ/WRITE/EXEC`（因`TASK_EXEC=VM_EXEC`）、`VM_MAY*`及`VM_ACCOUNT`；临时标志实际为`VM_RAND_READ|VM_SEQ_READ`，SOFTDIRTY随配置可为0。在该默认组合下，私有VMA查表模板可选到`PAGE_COPY_EXEC`（RW初始为0、NX未置，后续仍可能受supported-mask筛选）；这只是pgprot模板，不是已建立的参数PTE（ES23-A011/A089–A094/A109–A112）。



此后实际调用`insert_vm_struct(mm,vma)`：先通过`find_vma_prev`求前驱；当VMA已是匿名（`!vm_ops`）且对象有效，令`vm_pgoff=vm_start>>PAGE_SHIFT`，再调用`simple_vma_link→__vma_link_to_list`入链并更新`map_count`及相应列表头计数。只有插入正常返回0，才令`mm->stack_vm=mm->total_vm=1`、解除锁包装并把`bprm->p=vm_end-sizeof(void*)`。因此单页VMA入链与一个实际数据页可用、复制过参数、用户CPU堆栈指针已被设置，都不是同一件事。失败标签会清`bprm->vma`并调用`vm_area_free`；但所读`simple_vma_link`正常返回0，不能凭存在插入失败标签认定该非零返回在固定正常路径有实际生产者（ES23-A012–A014/A046–A051）。



锁条件必须按代码限定：`mmap_write_lock_killable`当前活动体直接返回0，down_write_killable为注释；对应unlock未执行up_write。因此本链不能把包装函数名当成已经拥有互斥锁或实际可产生-EINTR。对于先行分配、写字段、入链和出错清理，只能按各行被抵达的时间与对象前提计账（ES23-A010/A014/A095/A096）。



## 参数字节不是初始VMA本身



在`begin_new_exec`切换mm之前，`do_execveat_common`经`bprm_stack_limits`等准备，调用`copy_string_kernel`、`copy_strings`复制环境和argv；这些操作通过`bprm->p`不断向下推进，再在每段实际调用`get_arg_page(bprm,pos,1)`取得页面。`get_arg_page`使用`bprm->mm`而非旧`current->mm`，调用`mmap_read_lock`与`myos_get_user_pages`，以其`long ret<=0`判断失败。若取得有效page并继续，`copy_strings`才会执行`memcpy_to_page`，不能仅凭字节指针变动断言复制已完成（ES23-A001–A004/A015–A018/A044/A045/A099/A100）。



固定`myos_get_user_pages`有两个实际边界。其一，它只对被`simple_find_vma`包含的参数地址继续；若addr不落在任一VMA中便`continue`，并未给对应`pages[i]`赋值。初始VMA仅一个PAGE_SIZE，而`copy_strings`在`setup_arg_pages`调用扩栈以前执行；向下跨出单页的参数地址能否得到合法页面并未闭合。其二，该函数声明`long`、遍历中可能查页或分配并创建页映射、写`pages[i]`，但在所读完整活动函数末尾**没有返回值语句**。因此上层`ret<=0`并没有源码级可验证的确定结果；不以C未定义返回行为推断某个固定错误码、必然复制成功或某个硬件崩溃（ES23-A077–A080/A088/A099）。



若有效VMA和page确实让GUP进入新建映射，`myos_creat_one_page_mapping`直接以`PAGE_SHARED_EXEC | _PAGE_PAT | page_addr`写`*pte`，并未读取栈VMA的`vm_page_prot`；此选定宏带RW且不含NX，和私有栈默认的PAGE_COPY_EXEC并非相同初始位组合。GUP对该帮助函数的返回值未作成功门检查。这里支持的是静态**权限模板不一致**及调用前提，不是已经运行证明用户RWX页面、安全突破、可用PTE或有效物理内容（ES23-A079/A105–A112）。



## S02 — ELF调用时序、地址对齐和栈执行策略



真实`load_elf_binary`从程序头的`PT_GNU_STACK`和`PF_X`确定`executable_stack=EXSTACK_ENABLE_X`或`EXSTACK_DISABLE_X`，否则沿初值`EXSTACK_DEFAULT`。接着在成功前提下先调用`begin_new_exec(bprm)`；其内`exec_mmap(bprm->mm)`置`tsk->active_mm=mm`与`tsk->mm=mm`，并在返回后清`bprm->mm=NULL`。随后ELF活动次序是`setup_new_exec(bprm)`、`setup_arg_pages(bprm,randomize_stack_top(STACK_TOP),executable_stack)`。因此`setup_arg_pages`中的`mm=current->mm`在这条有效的成功链上是**新mm**，`bprm->vma`仍指向此前创建在此mm中的栈VMA。此身份转换以`exec_mmap`正常返回为前提；完整调度、CR3/TLB或线程资源管理依第15批旧限定，不重新审查（ES23-A034–A040/A070–A074）。



`randomize_stack_top`选定代码仅返回输入；`arch_align_stack(sp)`当前返回`sp &~0xf`，不执行注释里的随机扣除。x86-64固定宏下`STACK_TOP=STACK_TOP_MAX=TASK_SIZE_MAX=(1UL<<47)-PAGE_SIZE=0x7ffffffff000`，本来就按16字节对齐；而临时VMA的`vm_end=STACK_TOP_MAX`。所以这个**已选ELF调用源**的`stack_shift=vm_end-stack_top`在纸面上等于0，初始`bprm->p=0x7fffffffeff8`再经过字符串复制的移动。若某其它有效调用产生非零stack_shift，源码却先减少`bprm->p`、可选`loader`及`exec`，把新`bprm->p`写`mm->arg_start`，然后才检查锁；最低栈顶地址校验和`shift_arg_pages`页表/页面搬迁都为注释。不能把指针数值平移等同于已有页面被搬迁，也不能对默认shift=0路径编造搬迁失败（ES23-A021–A023/A029/A073/A075/A076/A093/A101/A114）。



`setup_arg_pages`在写锁包装正常返回0之后令**局部**`vm_flags=VM_STACK_FLAGS`；EXSTACK_ENABLE_X执行局部OR VM_EXEC，EXSTACK_DISABLE_X执行局部清VM_EXEC，然后再OR `mm->def_flags`与VM_STACK_INCOMPLETE_SETUP。如果def_flags有EXEC，局部清除可被再次加回；但所选mm_init的默认来源又有自己的掩码前提，不可假设任意真实值。最关键的是`mprotect_fixup`、TLB更新和`shift_arg_pages`均被注释，活动的`vm_flags_clear(vma,VM_STACK_INCOMPLETE_SETUP)`仅在**VMA对象**清临时标志，不将局部vm_flags其它位赋回VMA，亦没有刷新`vm_page_prot`或已建PTE。`pr_warn_once`只检查局部VM_EXEC，日志内容不能认证VMA已被赋可执行权限或禁执行权限成功（ES23-A024–A030/A087/A098/A104）。



默认路径继续令局部`stack_expand=32*PAGE_SIZE`，虽然读了`stack_size=vm_end-vm_start`，但利用`bprm->rlim_stack`收窄扩张范围的计算为注释。`stack_base=vm_end-stack_expand`后，源码先写`current->mm->start_stack=bprm->p`，才调用`expand_stack(vma,stack_base)`；若ret非零则映射为`-EFAULT`，正常解锁并返回。局部`rlim_stack`未由此活动计算取得有效限制；不能由这个变量的名字证明已落实资源上限。即使后续扩张失败，`start_stack`和此前的`bprm->p/mm->arg_start`也没有在所读`setup_arg_pages`里配对恢复；只称条件性部分效果，不能脱离有效对象声称实际文件执行失败导致错误CPU寄存器（ES23-A030–A033/A102）。



## S03 — expand_stack直接扩展及缺页衔接



`expand_stack(vma,address)`先令`mm=vma->vm_mm`、`error=-ENOERR=0`，再按PAGE_MASK对齐地址。若`address<mmap_min_addr`直接返回-EPERM；选定`mmap_min_addr`初始化为PAGE_SIZE。有关stack_guard_gap（常量存在）、前驱VMA冲突、`anon_vma_prepare`、匿名锁、资源增长与统计的操作均为注释。若`address>=vm_start`，无需扩展却仍返回0。若`address<vm_start`，才计算`size=vm_end-address`与`grow=(old_vm_start-address)>>PAGE_SHIFT`，并且只有`grow<=vma->vm_pgoff`才在`mm->page_table_lock`包围下先写`vm_start=address`、再写`vm_pgoff-=grow`。若grow太大，源码未设置错误，直接以0返回，不能说成功扩大VMA或建立新PTE（ES23-A052–A062/A113/A116）。



在选定x86常量和一个PAGE_SIZE初始VMA且对象有效前提下，最初`vm_end=0x7ffffffff000`、`vm_start=0x7fffffffe000`、`vm_pgoff=34359738366`；`stack_base=0x7ffffffdf000`时向下增长31页，满足`grow<=vm_pgoff`，若锁语句等正常执行，VMA元数据范围会成为32页且pgoff减到34359738335。但最初`mm->stack_vm=mm->total_vm=1`，本批所读`expand_stack`活动体未更新两个计数，也没有把32个页面实际分配好；这是对已读选定路径的局部账差，不是实机内存占用曲线。写`vm_start`与写`vm_pgoff`发生在不同语句，之后的锁/循环/返回失败不会自动撤销已经进行的字段写入（ES23-A011–A013/A032/A050/A058–A062）。



两种调用者必须分离。exec主动准备在`setup_arg_pages`中直接调用`expand_stack`，不依靠运行期页故障发现“尚未包含”的栈地址；运行期`do_user_addr_fault`却先`simple_find_vma(current->mm,address)`，找不到立即`myos_bad_area`，找到并确认`vm_start<=address`就跳到缺页处理。稳定VMA与点包含谓词下，**栈起点以下的地址不会得到后继VMA返回值**，因而达不到后面的VM_GROWSDOWN/expand_stack分支；除非证明其它lookup语义或并发变化，不能说实际栈外缺页会自动触发扩栈。此点复用MF12-F01及R12-S01，连同初始NULL迭代器的第21批限制，不重复登记为新根因（ES23-A063–A067/A088）。



当栈地址已在VMA内、所有选择条件有效，运行期可调用`myos_handle_mm_fault(vma,address,flags)`；但实际fault==ENOERR(0)或非零的消费只按12主线旧界限理解：源码只调用一次，非零进入`while(fault!=ENOERR)`，没有在循环里重算；0也不证明PTE安装、页面内容、CPU指令重试已完成。访问错误校验与读锁接口在所选架构缺页主体仍有注释门，相关条件风险已由MF12-F02/F03登记。本批不重新分析匿名零页、COW、缓存、页回收、内核栈或系统调用返回汇编（ES23-A065–A069/A113）。



最终ELF调用附近有`finalize_exec(bprm)`和`START_THREAD(...,bprm->p)`，只能证明传入用户栈指针的一个接口位置；这不是对实际寄存器RSP、用户态第一条指令或CPU页表权限的直接认证。栈VMA入链、元数据扩张、虚拟地址查找、真正物理页、PTE位和用户寄存器彼此独立，不能以一个函数返回0替代全部证明（ES23-A074）。



## S04 — 条件矩阵、延期去重与未做事项



`facts-and-boundaries.yaml`保留临时bprm.mm与current.mm身份切换、VMA字段的原始/准备/扩张快照、文件回调未参与的局限、初始分配及入链失败、参数页越界与未返回值、EXSTACK默认/启用/禁用、局部vm_flags与VMA实际写回、stack_shift=0及非零条件、扩栈`grow<=pgoff`与无扩大返回0、guard/rlim注释门、启动调用与缺页调用及返回状态，共35行场景和四组纯纸面演算。调用、字段数据消费和真实effect由DOT及结构账分列。



`deferred-findings.yaml`将5个新的**条件性**待核主题与MF12-F01/F02/F03的3条历史追加分开：栈执行策略未写回、参数GUP不返回值/未命中缺写、新参数PTE固定保护、扩栈无扩张却返回0及计数/限额边界、非零shift搬迁不执行和先行字段变化。对12既有的缺页扩栈不可达、权限/同步门及非零fault只追加当前exec路径证据，绝不重计为三个新缺陷。没有进行预处理/编译/运行C或ASM、内核/QEMU/硬件、实际argv内容比对、RSP读取、TLB或并发/性能实验，不修内核、不改历史分析。正式提交与远端回读后停写交C03独立验收，不自行开始下一批。
