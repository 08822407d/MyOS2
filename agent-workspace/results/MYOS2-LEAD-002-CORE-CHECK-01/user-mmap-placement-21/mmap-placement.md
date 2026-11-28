---
task_id: CORE-USER-MMAP-PLACEMENT-21
track_id: MYOS2-LEAD-002
record_type: bounded_static_mmap_placement_narrative
execution_identity: MYOS2-LEAD-002-CHAT-EXEC-02
conversation_display_name: MYOS2-A-C03b 代码分析执行续建
produced_by: GPT-6
model_per_owner: GPT-6
effort_per_owner: Extra (Owner selected)
date: 2026-11-02
source_branch: time
source_pin_short12: a039d9803ade
instruction_short12: 84ca207f5cc2
result_base_short12: 0e139c6ca86d
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第21批 · mmap地址候选、空洞与整段区间的静态分析

## 总结与消费限定

在固定`time @ a039d9803ade`中，mmap地址的“起点不在已有VMA”“整个半开区间空闲”“边界及页对齐合法”“候选实际返回”“VMA/PTE已安全建立”是不同事实。本批只核地址选择及进入simple_mmap_region之前和刚进入时的控制流；不恢复19/20的权限与分配链。第20批主线R20-S01/S02/S03/S04及第14批VMA撤销/非事务回滚、第12批simple_find_vma包含谓词仍强制限定。当前`runtime_evidence=NOT_RUN`。

## A01 入口、回调身份、x86地址常量

`do_mmap`在len=0时返回-EINVAL；将MAP_FIXED_NOREPLACE局部增MAP_FIXED，只有非fixed时才`round_hint_to_min(addr)`。后者清PAGE_MASK以下位，并在非零提示低于mmap_min_addr时提高到PAGE_ALIGN(mmap_min_addr)。随后PAGE_ALIGN(len)，检查结果为零、pgoff+页数无符号回绕和映射数量，才调用`__get_unmapped_area(file,addr,len,pgoff,flags,vm_flags)`。回调返回编码错误则do_mmap直接退出；NOREPLACE还有后续交叉查询。所有早退均受对象和算术有效性限制（AP21-A001–A011/A080/A081）。

`__get_unmapped_area`先做len>TASK_SIZE门。对于MAP_FIXED，它立刻返回addr，跳过随后用来检查非fixed候选的上界与offset_in_page对齐分支；不能把正常非fixed的后验门当作fixed也执行。非fixed时get_area起初取current->mm->get_unmapped_area，有文件且file->f_op->get_unmapped_area非空则覆盖此指针。无文件共享映射只活动清pgoff=0，shmem专用get_area绑定是注释；之后经函数指针动态调用并按IS_ERR_VALUE、addr>TASK_SIZE-len及offset_in_page进行选定后验校验，正常仅return起点addr（AP21-A018–A026/A082–A084）。文件回调与mm缺省目标不能混称为一次直接调用。

固定arch_pick_mmap_layout把mm->get_unmapped_area绑定为simple_get_unmapped_area，mmap_base设为TASK_UNMAPPED_BASE；setup_new_exec实际调用arch_pick_mmap_layout，随后另赋mm->task_size=TASK_SIZE。此调用链真实存在，但不保证所有mm在使用前都执行，也未审完整fork/exec复制继承。旧arch_get_unmapped_area分支及随机化基址调用为注释（AP21-A070–A075）。

x86-64宏给PAGE_SHIFT=12、PAGE_SIZE=4096、TASK_SIZE=TASK_SIZE_MAX=DEFAULT_MAP_WINDOW=(1UL<<47)-PAGE_SIZE，即0x7ffffffff000。TASK_UNMAPPED_BASE=PAGE_ALIGN(TASK_SIZE_LOW/3)=0x2aaaaaaab000（仅在64位ulong和选定ALIGN宏条件下的纸面结果）。于是`TASK_SIZE_MAX-DEFAULT_MAP_WINDOW=0`；simple_get对超DEFAULT窗口的提示执行该加法在此固定表达式下不扩大搜索上界。数字只用于静态边界演算，不是实机内存布局认证（AP21-A076–A081/A015）。

## A02 提示地址与Top-down

`simple_get_unmapped_area`先将info.length设为len、align_mask=0、align_offset=pgoff<<PAGE_SHIFT、low_limit=PAGE_SIZE、high_limit=mm->mmap_base。非零hint先按PAGE_MASK清低位，再只调用`simple_find_vma(mm,addr)`判断起点是否落在某个已有VMA；若不在，直接返回addr。该谓词本身只检查`vm_start<=addr<vm_end`，不检查`[addr,addr+len)`是否延伸到下一VMA。例：已有区间[0x3000,0x5000)，请求[0x2000,0x4000)，起点空闲而全段重叠，快速返回仍可能生效；只要上界/对齐满足，__get没有额外全区间空洞检查。不能据此断言实际已经覆盖，因后续还有旧VMA处理（AP21-A012–A014/A055–A056/A059/A009）。

提示未快速返回或为零时，selected callback只调用`simple_find_vm_unmapped_area_topdown`。它从gap=~0、tmp=NULL开始，经for_each_vma_topdown访问VMA；在有效有序链和对象条件下，用当前VMA的vm_end、后继的vm_start及low/high_limit形成tmp_gap与tmp_gap_end，仅当unsigned差达到info.length才将候选写入gap/gap_end并break，另有低边界跳出条件。跨度相减要求端点顺序有效；发生无符号回绕不能当成找到真实空洞（AP21-A027–A032）。

这里存在两个不可混合的出口问题。第一：若找到候选且执行正常到达尾部，`if (gap = ~0)`是赋值语句，非比较；~0在ulong上为非零，于是旧gap被覆盖为`(info.high_limit-info.length)&PAGE_MASK`，并不是返回刚刚扫描到的候选。这个结果不保证不与VMA相交。第二：若遍历自然耗尽，`tmp==NULL`分支调用`vma_next_vma(tmp)`；该帮助函数对NULL有活动的while(vma==NULL)，而mm_prev_vma/mm_next_vma还在vma可能为NULL时计算&vma->list。不能保证这一分支抵达fallback，亦不能由未编译C代码推断特定硬件死锁、崩溃或优化结果（AP21-A033–A036/A049–A054/A063–A065）。

同文件Bottom-up实现可以单独做静态判断：若某实际调用者使用它并且代码抵达末尾，`if (gap = 0)`先把gap置零、条件为假，最终返回0，覆盖此前候选；自然耗尽时也有对NULL的vma_next_vma调用。但**默认simple_get没有调用Bottom-up**，因此本批不能把其局部缺陷称为默认mmap已经执行的路径（AP21-A037–A045/A017/A072）。

info具有flags、length、low/high_limit、align_mask/offset和start_gap；selected simple_get只赋某些字段，Top-down实际使用长度/界限与原始VMA边界，并未消费align字段或调用已有vm_start_gap/vm_end_gap保护帮助函数。对默认align_mask=0的情形，不能据此构造已经发生的非零额外对齐违约；对栈保护间距也必须另证VMA标志与真实访问策略（AP21-A013/A016/A067–A069/A088–A091）。

## A03 fixed、NOREPLACE、区间与返回

MAP_FIXED及MAP_FIXED_NOREPLACE在do_mmap后均含MAP_FIXED，因此__get都从fixed early-return直接返回addr；前者没有在所读do_mmap选址部分附加NOREPLACE交叉查询，后者有`find_vma_intersection(mm,addr,addr+len)`，非NULL时-EEXIST。此时addr+len的unsigned溢出尚无本表达式的完整校验：若end回绕小于start，助手源码`while(start>end)`可以构成不返回路径（前提为控制流实际到达）；不能无条件给出固定错误码（AP21-A002/A007/A019/A020/A060）。

半开区间的底层交集谓词正确表达为：不满足`vma->vm_end<=start`且不满足`vma->vm_start>=end`才重叠；贴邻不相交。但find_vma_intersection先用find_vma_prev_to_addr找**严格满足vm_end<start**的前驱，只有该前驱非NULL才去vma_next_vma并测试其后继。这使覆盖第一VMA、此前没有严格前驱的请求在定义良好迭代前提下返回NULL，形成NOREPLACE门的条件性漏检；若有合适严格前驱且下一VMA相交，该门确实能返回-EEXIST。首尾/贴邻时不能假设全部VMA都被检查。源码里mm前后迭代的NULL成员表达式限制也继续存在（AP21-A057–A065）。

`do_mmap`把通过候选交给simple_mmap_region，后者首先调用simple_munmap_vma_range。这个帮助函数先写`*pprev=find_vma_prev_to_addr(mm,start)`，再进入`simple_do_vma_munmap(mm,start,start+len)`，下游又消费同一个find_vma_intersection；成功分支才继续尝试合并或新建VMA。选址“返回一个值”与旧映射处理、清理副作用、完整映射事务是分离的，必须合读第14批回执，不能把范围候选当成全系统安全空洞（AP21-A008/A009/A085–A087/A092）。

## 代表场景与证据等级

| 场景和前提 | 条件性静态出口 | 不可直接得出的结论 |

|---|---|---|

| 非fixed hint起点空闲但尾部碰VMA | 快速返回hint，后验可能通过 | 整段没有冲突 |

| Top-down识别候选且抵达末尾 | 赋值if(gap=~0)覆盖候选 | 返回被扫描到的空洞 |

| Top-down遍历自然耗尽 | NULL实参交给vma_next_vma | 必然正常返回 |

| 仅定义的Bottom-up找到候选且抵达末尾 | if(gap=0)改写为0并返回 | 默认mmap用了Bottom-up |

| MAP_FIXED且addr不对齐/区间越界，len有效 | __get直接返回原addr | 后验上界与对齐已经检验 |

| NOREPLACE碰首VMA但无严格前驱 | find_vma_intersection可返NULL | -EEXIST总能拦截 |

| 文件覆盖get_area回调 | __get调用文件槽，后查编码错误/上界/对齐 | 文件回调保证全区间空闲 |

| 非fixed callback返回0 | 0可经过边界门后作为候选起点 | 必然成功映射或必然errno |

完整30个场景见facts-and-boundaries.yaml，额外包含空表、多个VMA、长度与上界、文件回调及堆栈保护约束。未知对象、C未定义或不可达分支不冒充已执行测试。

## 延期、证据和停止条件

deferred-findings.yaml登记七条条件性新事项：hint只查起点、Top-down赋值覆盖、NULL迭代、NOREPLACE严格前驱漏检、fixed提早返回、Bottom-up仅定义处的赋值错误、guard/alignment未消费；向第14批旧VMA恢复边界追加一项证据，历史ID和状态不改。P00–P04仅做静态报告，未运行源码、预处理、C/ASM、内核、QEMU、硬件或真实并发，所有CPU权限/错误码落地和完整地址空间事务均非已验证。完成本批六件交付后执行者停止写入，交C03独立验收，不自行后继。
