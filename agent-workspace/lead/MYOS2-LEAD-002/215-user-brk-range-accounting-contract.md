---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
model_per_owner: Pro独立验收并同轮安排后继
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 55488278ab63
execution_pin_short12: c9f4ba8d3226
read_channel: GitHub connector；自制文本/整数/符号链核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 本合同新增正式源码窄窗0/0/0；153作者114窗的主线复核另账，不转作154新证据。
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count:
  conversation: C11
  before_release: 2
  after_successful_154_release: 3
  thresholds:
  - 8
  - 10
  reached: false
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
record_type: phase2_executor_contract
followup_id: CORE-USER-BRK-RANGE-ACCOUNTING-154
executor_display_name: MYOS2-A-C11a 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-25
executor_model_per_owner: 次一档GPT-6.2 Sol＋Extra／高推理
executor_start_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-brk-range-accounting-154/
allowed_files:
- MANIFEST.md
- brk-range-accounting.md
- facts-and-boundaries.yaml
- brk-range-accounting-paths.dot
- deferred-findings.yaml
- evidence.md
D00: []
status: PREPARED_FOR_PUBLICATION_G04_PENDING
open_questions:
- 给定观察帧不认证真实可达；C转换/ABI、预处理、映射、运行及实际后台档位未证。
execution_parent_short12: c9f4ba8d3226
model_scenarios: 7
---

# 215号合同 · 第154批 · brk边界请求、增长页数账与缩小消费

**继续原「MYOS2-A-C11a 代码分析执行」／EXEC-25，使用GPT-6.2 Sol＋Extra／高推理，接收和执行无需Pro。** 本合同授权连续完成A/B/C七个有界场景、恰六新增、自检、唯一作者回执、实际G04后STOP_WRITING，回C11 Pro独立验收及同轮后继；不自行进入155，不另等开工许可。

[153主线独审](reviews/CORE-USER-FREE-REINIT-153-review.md)裁定153 PASS_PENDING_LOCAL，并判闭R152-01=CLOSED_BY_153_RECEIPT_ADDENDUM。152原RETURN记录、六件及160/160/0保留，合读本次独审§5后当前整体PASS_PENDING_LOCAL；旧历史缺失判断NOT_RECORDED、后台UNATTESTED不倒签。**154 D00=[]。** 冻结[153作者#7191089498](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7191089498)及[六件c9f4ba8d3226](https://github.com/08822407d/MyOS2/tree/c9f4ba8d3226/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-free-reinit-153)，不再改写153或重做114窗。本包技术从固定time源码另建有界模型，不承接153的allocator控制栈。

## 1. 增量和历史消费边界

200号/第144批已经覆盖候选用户brk原始返回、sys_brk的查询/同页/普通增长/缩小成功与失败、init_uslab的16轮以及动态upage消费。本批不重新执行这些低地址、无回绕模型，不扩展malloc/free/bitmap，也不另造未经发现的sbrk入口。

本批只做三个关联专题：

- A：在明确64位无符号模型下，两个边界请求怎样传入do_brk_flags和MAP_FIXED选择门；区分早退可推定返回与遇到未展开helper后的分析停止。
- B：在已明确给定的局部返回边界，比较新建VMA、已返回合并对象及新VMA分配失败时，增长尾部三个页数账与map_count、列表count的消费。
- C：克隆B_NEW完成快照作为数据输入，在给定的缩小观察帧继续推演整段摘链与首split分配失败，分别记录break字段、链表对象数和页数账。

必须合读旧14的R14-S01（字段恢复不等于VMA事务恢复）、21的R21-S01/S02（MAP_FIXED早退、初始NULL迭代/严格前驱）、22的R22-S01/S02（合并目标与分阶段副作用）、26的R26-S01/S02（vm_brk_flags自身零长/对齐回绕、simple_vma_link正常仅返回0）、144及R144-N01。75是内核kmalloc入口，不为本包提供用户库或运行证明；19只作VMA/页表/零页与匿名性质的历史限定。

A是21既有地址门事实在brk调用者的新增输入消费；B/C是既有局部增长与摘链源码的计数关系。不得把旧根因重新登记成新独立漏洞。实际内核错误、运行损坏、物理映射/回收和修复一概没有由这些模型证实。

## 2. 共用数值与对象模型

给定ulong/unsigned long/size_t是64位无符号整数；int为32位；本包出现的负errno可在int内表示。无符号运算按模2^64。IS_ERR_VALUE中的整数→void*→size_t转换在本模型明示投影为保持64位模式；这不是C整数/指针转换的实际认证。仅当把ulong编码错误返回值从do_brk_flags的int返回接口投影时，明确采用低32位二进制补码有符号解释；这是本包给定转换模型，不作任意C实现/ABI认证。

M=2^64，U=M-1，K=PAGE_SIZE=4096，PAGE_SHIFT=12。按固定宏链，T=TASK_SIZE=(1<<47)-K=0x00007ffffffff000。PAGE_ALIGN(x)的数值模型是((x+K-1) mod M)&~(K-1)，而非无限精度数学ceil后不回绕。

VM_DATA_DEFAULT_FLAGS沿本包明确选用的固定文本默认分支为VM_DATA_FLAGS_EXEC=0x77；F0=0x77|VM_ACCOUNT=0x00100077；FL=F0|VM_LOCKED=0x00102077。外部flags参数均为0，锁定位只由给定mm.def_flags带入。VM_SOFTDIRTY另记符号SD：明确登记所选源码条件分支，不以未经预处理的构建文本认证最终配置；若无法独立闭合最终宏选择，保留SD∈{0,0x08000000}。页数账结论不依赖SD。

每个被操作mm/VMA/list节点合法、身份互不混淆，相关字段已初始化、可读写；选定链表相邻关系正确、计数匹配，无并发修改；只有实际进入且全部所需检查、读写正常完成的直接列表操作，才登记其返回后的效果。整数地址仅作字段值，不证明真实可访问内存、用户地址合法性或物理页存在。

观察帧是明确给定的局部入口，不是对旧函数可达性的证明。特别是旧21的mm_next_vma/mm_prev_vma以NULL开始所涉语言/对象缺口仍在。不得把“查找返回N”“合并已返回M”拼成已经闭合的真实调用链。未展开helper的中间步骤、VMA修改和副作用保持UNKNOWN；若题设给了后返回状态，只从该状态继续。图将GIVEN入口/返回边界与源码调用边分开。

本包继续调用的vm_area_alloc、vm_area_creat_dup、vm_get_page_prot采用明确的有限保持条件：在其给定正常返回时，全部受跟踪mm字段、既有VMA字段、链表指针和计数保持调用前值；只给定返回值与B_NEW的新N身份/所列字段，不认证helper内部过程或未跟踪的分配器状态。这个保持条件属于GIVEN，不得写成“正常返回/NULL本身证明无副作用”。后返回观察帧中的其他先行helper不采用这个保持推论，其进入观察帧的状态直接给定。

## A · sys_brk边界请求到固定地址门（2个独立入口）

共用入口：mm.start_brk=0x400000，origbrk=mm.brk=T-0x2000=0x00007fffffffd000；mm.def_flags=0；页数账和链表对象有效且任意给定。两例各从sys_brk主体入口独立重置。不要继承B/C的mm或列表。

### A_CROSS_TASK

请求brk=T+1=0x00007ffffffff001。精确列出min比较、newbrk、oldbrk、同页门、增长门、传给do_brk_flags的addr/len/flags、pgoff、局部flags、__get_unmapped_area的全部实参和每个已到达检查。

预期：newbrk=T+K=0x0000800000000000，oldbrk=T-0x2000，len=0x3000，pgoff=oldbrk>>12=0x7fffffffd。__get_unmapped_area的len>T为假，MAP_FIXED为真，返回原addr；该值不在IS_ERR_VALUE的末4095编码区，因此do_brk_flags抵达simple_munmap_vma_range调用边界。

**停在执行该调用之前。** 不给它成功返回，不推定merge、VMA创建、页数账、sys_brk成功返回或任何页表效果；source_outcome=ANALYSIS_STOP_BEFORE_UNEXPANDED_CALLEE，syscall_return=NOT_DERIVED。此时mm.brk仍为origbrk；这个限定前缀没有增长尾部的页数增量。可比较输入区间末端addr+len=T+K>T，不能称为已建立越界VMA或实际超界访问。

不进入非fixed回调、TASK_SIZE-len或offset_in_page后置门。它们是被MAP_FIXED提前返回越过的源码检查，不是本例执行后通过的检查。只追加21的brk消费证据。

### A_ALIGN_WRAP

请求brk=U=0xffffffffffffffff。按上述模模型得newbrk=0，oldbrk仍T-0x2000；请求值大于入口mm.brk，故不会因newbrk较小而误选缩小分支。增长长度是(newbrk-oldbrk) mod M=0xffff800000003000。

__get_unmapped_area先命中len>T，返回ulong编码的-ENOMEM=0xfffffffffffffff4；MAP_FIXED的return addr没有执行。do_brk_flags的IS_ERR_VALUE为真；按本包明确给定的int转换投影得到-12，sys_brk的<0门进入out，写回并返回origbrk。

simple_munmap_vma_range、merge、allocation、增长页数账均未到达。不要把A_CROSS_TASK的未知后续移植到本例，也不要把虚构的“扩展0字节成功”当作PAGE_ALIGN回绕结果。源代码有一次out字段写回，尽管写入值等于入口值，仍区分“值没变”和“未发生赋值”。

## B · 增长尾部的页数账（3个局部观察帧）

令X=0x60000000，L=0x3000=3K，pgoff=X>>12=0x60000。三帧相互独立，入口均是所列helper已经按题设正常返回后的状态，不从函数最初入口倒推出清理/合并的真实作用。

三帧的页数账入口：(total_vm,data_vm,locked_vm)=(100,40,7)。这里不要求locked_vm为0；已有7只表示给定字段值，不能从本次分支反推既有页面状态。map_count=mm_mt.count=4；mm.brk=X。保留一个sys_brk增长调用者帧：origbrk=X、请求X+L、oldbrk=X、newbrk=X+L，其do_brk_flags调用暂停等待返回。调用者是否可在机器上建立这样的帧保持未证；本包只分析指定返回被调用者如何消费。

### B_NEW

入口定在do_brk_flags第1226行vm_area_alloc调用之前：addr=X、len=L、pgoff=0x60000、flags=F0；先行simple_munmap_vma_range正常返回0且simple_vma_merge正常返回NULL是给定边界，其内部不展开。当前旧四节点恰为Q0、Q1、P、R，按此顺序构成有效列表，prev=P，P之后为R。给定Q0=[X-0x8000,X-0x7000)、Q1=[X-0x6000,X-0x5000)、P=[X-0x4000,X-0x3000)、R=[X+0x4000,X+0x5000)；四节点均在请求范围外。这个精确R范围进入B_NEW_DONE供C原样克隆。

给定vm_area_alloc正常返回新有效对象N，N.vm_mm指向本mm、N.vm_file=NULL；对象与旧四节点不同。给定vm_get_page_prot(F0)正常返回不透明值PROT0，不展开权限/PTE构造。随后按源码推演vma_set_anonymous、vm_start/end/pgoff/flags/page_prot赋值、simple_vma_link及本包所需直接列表效果；N在P和R之间入链。simple_vma_link必须消费当前活动仅return0的实现，不设负返回。

逐阶段记：N.vm_ops=NULL，范围[X,X+L)，pgoff=0x60000，page_prot=PROT0；实际插入后先mm_mt.count4→5、再map_count4→5，不能当作一个原子写入。随后simple_vma_link实际调用vma_link_file；N.vm_file=NULL使其file条件为假，不进入文件映射/引用分支，之后simple_vma_link返回0。到out后total100→103、data40→43、locked仍7，N.vm_flags从F0再OR SD；do_brk_flags局部返回0。此后给定调用者恢复执行才有mm.brk=X+L及返回X+L。

命名完整数据快照B_NEW_DONE，用于C的独立克隆；该名字不意味着真实运行或可达性认证。这个快照的元数据包含既有四节点+N，不带其他未明栈帧或回调执行状态。

### B_MERGE_LOCKED

入口定在第1219行simple_vma_merge已经正常返回有效对象M，尚未执行if(vma)的消费位置。给定mm.def_flags=VM_LOCKED，局部flags=FL，vma=M；其返回前可能发生的合并字段/链表变化不在本包推导，入口计数直接采用本节共用值。给定M.vm_flags=FL，M.vm_mm=mm；M范围/pgprot及是否真正覆盖请求均UNKNOWN，不补造合并过程。

源码非NULL门goto out，vm_area_alloc、新VMA字段赋值、simple_vma_link均未到达。仅在此观察帧后，map_count/mm_mt.count仍4/4；total/data各加3成为103/43，locked7→10，M.vm_flags OR SD；局部返回0，给定sys_brk caller随后写请求并返回X+L。

这个结果只证明尾部使用len>>12，而非由新建对象数决定增量；不把“merge返回非NULL”写成已证明真实生产路径、正确合并或已锁定三物理页。既有22的严格prev、目标缺赋值限制继续适用。

### B_ALLOC_FAIL

入口与B_NEW相同，但给定vm_area_alloc正常返回NULL。第1227门命中，return -ENOMEM；新VMA字段、link及out账本均未到达。局部观察帧之后的页数账仍100/40/7，map_count/mm_mt.count仍4/4。给定sys_brk caller进入out，写回并返回X。

上述保持仅比较本节明确给定的后清理入口，不能称为“整个do_brk_flags失败没有先行作用”。本例没有给出清理前的VMA/计数状态，不把UNKNOWN补成零作用。

## C · 从B_NEW_DONE克隆的缩小消费（2个独立观察帧）

C两例各自克隆B_NEW_DONE的全部明确数据：mm.brk=X+L、页数账103/43/7、map_count=mm_mt.count=5、列表[Q0,Q1,P,N,R]、N=[X,X+L)、N.vm_ops=NULL、N.vm_file=NULL，N前驱P、后继R；Q0/Q1/P/R的范围按B_NEW原样复制，不在C隐式覆写。两例不连续执行。元数据克隆不是恢复B中的待返回调用栈；各自明确另给下述新的sys_brk缩小调用者/被调观察帧。

共用min前提：mm.start_brk≤X；无并发和外部写者。sys_brk已经依对应请求完成其源码前缀，并先写mm.brk=request，再暂停等待simple_do_vma_munmap(mm,newbrk,oldbrk)返回；该前缀的字段赋值为源码顺序，但到达内部观察点的查询正常返回仍是明确给定边界。

### C_REMOVE_WHOLE

请求X；caller保存origbrk=X+L，simple_do_vma_munmap有序实参(mm,X,X+L)。内部观察点定在第1148行之前，即find_vma_intersection已按题设返回N、首split未执行、prev=P、simple_find_vma(mm,X+L)已按题设正常返回NULL、局部error=0。这个error与函数初始-ENOERR且未执行split的源码关系一致。查找过程不展开；并明确观察点的字段/链表状态仍为上面克隆值，只有caller已把mm.brk置X。

从此处继续：last=NULL，所以尾split未执行；vma_next_vma(P)返回N，N.vm_end≤end为真，N!=last，调用__vma_unlink_from_list移除N。map_count5→4在嵌套删除前；mm_mt.count5→4在list_del_init正常完成之后。直接列表助手依给定有效相邻关系正常完成，N的链节点自环；不宣称N对象已释放。下一次vma_next_vma(P)返回R，R.vm_end>end，退出循环。error为入口-ENOERR=0，因此simple_do_vma_munmap返回0，caller走成功返回X，mm.brk保持X。

该选定片段及直接列表助手没有total_vm/data_vm/locked_vm减量：终值仍103/43/7。与B_NEW观察帧入口的100/40/7相比，3页增量仍留在数值账，而N已从所选列表摘下。只称“此限定模型中的字段/列表与页数账关系”；不签全系统计数错误、物理泄漏、实际页表残留或运行损坏。simple_unmap_region没有在这里被调用；不把其注释当作已执行清理。

### C_FIRST_SPLIT_FAIL

请求X+K；caller保存origbrk=X+L，实参(mm,X+K,X+L)。内部观察点定在第1139行之前：find_vma_intersection已按题设返回N，尚未调用__split_vma；当前计数/列表保持克隆值，mm.brk已先写X+K。

start>N.vm_start为真；__split_vma(mm,N,X+K,1)进入，并给定其vm_area_creat_dup正常返回NULL。按所选源码实际return -ENOMEM；不得假设一个当前simple_vma_link无法生产的正常负返回。split中的file/open/边界修改/link均未到达；simple_do_vma_munmap走start_split_failed→end并返回-12。prev赋值、last查询、摘链循环均未到达。

sys_brk恢复mm.brk=origbrk=X+L并返回该值；不是-12。计数仍103/43/7、map_count/mm_mt.count仍5/5，N范围和列表关系保持。字段过程X+L→X+K→X+L必须显式列出；不能仅凭终值相同抹掉中间赋值，也不把该首split失败的局部无变化推广到旧14中第一split已成功的其他失败路径。

## 3. 强制证据与来源范围

正式执行者必须从固定time重新取得本批实际使用的定义/调用与必要直接助手，按现行协议给1–5连续物理行窄窗，保留原对象字符与行尾。下面行号仅为候选导航，不替代执行者实际重新定位和逐字重核。

| 源文件 | 本批必要范围与作用 |
|---|---|
| mykernel/arch/x86_64/myos/syscall.c | brk完整39–125上下文；min/对齐/请求值分支、缩小先写/增长后写、out恢复和返回 |
| mykernel/mm/vm_map/mmap.c | __get_unmapped_area 945–984；do_brk_flags 1193–1246；simple_do_vma_munmap 1124–1167；__split_vma 1092–1115；simple_vma_link 172–185、vma_link_file 153–169的NULL文件门；vma_prev/next 70–90 |
| mykernel/mm/vm_map/util.c | __vma_link_to_list与__vma_unlink_from_list 15–36，两个计数先后 |
| mykernel/lib/list/double_list.h | 实际新增/摘除、membership与直接指针助手/检查；尤其INIT_LIST_S/别名、list_del_init、list_header_delete_node、list_add_to_next；只为本包选定有效列表模型，不扩全部列表库 |
| mykernel/lib/list/double_list_macro.h、double_list_types.h、double_list_const.h | list_header_foreach→list_for_each与实际链节点/计数/毒值；直接定义/使用分开；已有四节点与N可有界核membership |
| mykernel/include/linux/kernel/asm-generic/bug.h、mykernel/include/linux/debug/bug.h、mykernel/include/linux/compiler/myos_debug_option.h | 本包所选BUG_ON(false)与list定义分支；与配置文本合读，不认证实际预处理 |
| mykernel/mm/vm_map/vm_map.h | vma_set_anonymous 160–164；查询函数和初始NULL历史停止边界只作上下文核对，禁止绕过旧21签端到端 |
| mykernel/mm/misc/mm_misc_types.h | brk/start_brk、total/data/locked、def_flags、map_count/mm_mt的真实成员/类型 |
| mykernel/mm/vm_map/vm_map_types.h | VMA范围、mm、flags、pgoff、vm_ops/file/page_prot字段 |
| mykernel/arch/x86_64/mm/page/page_const_arch.h | PAGE_SHIFT/SIZE/MASK；__VIRTUAL_MASK_SHIFT、task_size_max/TASK_SIZE宏链 |
| mykernel/mm/page_alloc/buddy_const.h；mykernel/include/linux/kernel/align.h；mykernel/include/uapi/linux/const.h | PAGE_ALIGN→ALIGN→__ALIGN_KERNEL→__ALIGN_KERNEL_MASK与常量后缀链 |
| mykernel/include/linux/kernel/types.h；mykernel/include/linux/kernel/err.h；mykernel/include/uapi/asm-generic/errno-base.h | ulong与需要的类型、MAX_ERRNO/IS_ERR_VALUE真实表达式、ENOERR/ENOMEM |
| mykernel/mm/vm_map/vm_map_const.h | 默认数据flags分支、VM_ACCOUNT/VM_LOCKED/VM_SOFTDIRTY条件定义 |
| mykernel/include/uapi/asm-generic/mman-common.h；mykernel/include/uapi/asm-generic/mman.h；mykernel/arch/x86_64/include/asm/uapi_mman.h；mykernel/include/uapi/linux/mman.h | MAP_FIXED真实定义及本包使用的包含导航；不能把VM_LOCKED/VM_EXEC与MAP_FIXED放入同一个标志命名空间 |
| mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c | 现行固定配置/双canary规范；不能将文本当作实际预处理或运行认证 |

用户活动brk包装和编号/生成链可直接按144的已审限定作为条件接口导航，本包不重复造一次完整包装/汇编返回证据。最终消费结果写“给定sys_brk caller的局部返回”，不声称用户寄存器/errno或真实sh链接已认证。vm_brk_flags只有历史排重，不新增其零长/回绕专题。vm_get_page_prot保留不透明返回边界，不扩权限/缺页；vm_area_alloc/creat_dup只读NULL/非NULL接口和本包边界，禁止扩kmalloc/SLUB。

## 4. 结构化交付、验收和停止

七个场景ID固定为A_CROSS_TASK、A_ALIGN_WRAP、B_NEW、B_MERGE_LOCKED、B_ALLOC_FAIL、C_REMOVE_WHOLE、C_FIRST_SPLIT_FAIL。不是七个真实syscall运行记录。A2从完整局部入口投影，B3和C2为指定观察帧；逐项记录entry_kind、given_frame、source_prefix、callee_boundary、values_written、normal_return_or_stop、not_reached、unknown_effects、historical_scope_ref。

facts至少包含numerical_model、A边界矩阵、B账本阶段表、B_NEW_DONE快照、C克隆/调用者帧、计数与对象身份表、source_index和graph。正文与facts表值一致；DOT与facts.graph的节点/边约定字段一致。链表对象数、页数账、break数值和实际映射四类对象分别记录；不把ANALYSIS_STOP画成CPU return或循环。

图可用共享源码骨架加七个场景/观察点，不按每次指针写造大图。对B_MERGE_LOCKED不得画一条已经闭合的真实merge生产调用链；对A_CROSS_TASK必须在simple_munmap_vma_range前停止；对C两入口不得把给定查找返回画成已证明越过旧NULL迭代问题。

无字数、引文数、模型字段数或新问题数量配额。deferred允许仅有旧条目的evidence_append或本批限定关系观察，必须给原根因ID/回执和明确selector，不以“发现重复计数/越界/泄漏”作为已证运行结论。D00=[]；R152-01已由本轮153独审判闭，本批不安排旧文证返工。

完整本地P9、真实模型/effort独立认证、MyOS2程序/脚本、预处理、编译链接、Graphviz、QEMU、实机、真实系统调用均NOT_RUN。可自写局部纯整数/文本一致性脚本，不调用项目程序或构建脚本；正式作者必须区分自写有限模型成功与项目程序运行。只新增指定六件；无源码改动、旧成果改写、PR元数据变更或进入155授权。


## 5. G00、当前接收与唯一写区

实际入口为本轮PR17唯一主线正式评论。先取真实评论ID、固定lead提交，完整读本合同、同提交153独审及[检查点](checkpoints/2026-11-26-free153-reviewed-brk154.md)。保持任务MYOS2-LEAD-002-CORE-CHECK-01、阶段2、原claude/dazzling-cori-q0dnyt、原Draft PR17；唯一写区为本合同frontmatter列出的user-brk-range-accounting-154/六件。无源码、旧成果、主线文件、PR元数据或跨仓业务写权。

按[79](79-work-package-sizing-and-pro-review.md)只复用C11a本人同一会话实际完整读过且版本未变的十五项目规则、有效MA全文及同版共用十五条；不借用C11/旧C10c的已读。按[63](63-github-write-retry-and-readback.md)、[198](198-repository-first-compact-relay.md)、[210](210-mainline-c11-recovery-and-context-thresholds.md)及212恢复包固定读序补必要新读；Mnemosyne命名/接收范围仅任务局部。新会话必须完整独立接收，不能沿用本同会话例外。

本轮主线实核MA master b234c6c91a2e、入口881fd8c1076e，建设06b783861368、检查点54ac6e5fc66c，新增§322是Alaya反馈原始背景与收件流程的分析、待合议，未改变48采用→47 v1.4-r10.266及45同版共用1.1-r9.260十五条。Mne master e581eb9df6c4未变。C11a须刷新实际refs、读取新适用差异；若又有变更按其真实内容处理，不能用旧短号覆盖现状，也不将待议反馈当正式采用。

MANIFEST分别登记execution_handoff_receive及guidance_refresh：当前身份/显示名、任务/阶段、真实入口和合同版本、接收日期/未知时刻、授权、写区、缺项及下一安全动作；当前任务胜任与容量的真实定性判断；task_local_only/current_task_preserved；本次新读/本人未变已读复用的出处版本，实际完成后才写当前refresh_completed。实时剩余容量、实际客户端/后台/effort不可认证则UNATTESTED；不能从上一批成功推导本轮必然足够，也不因无法认证后台而伪造阻断。只有实际材料或容量阻断才保全并报告最小缺口。

G00实际完成并入MANIFEST：

1. 直接lead等于本轮正式评论的实际发布提交；exec=c9f4ba8d3226，time=a039d9803ade，直接master=e8e60cd19540，不用PR.base_sha代替master。PR16原head/base master Open/Ready，PR17原标题/原head/base master Open/Draft，均未合并；开放PR无since全分页至空。
2. PR17顶层评论无since全分页至空；检查点53历史标记及本次3新标记共56逐条字面恰一次、实际ID正确。当前正式入口、153作者7191089498、前次发令7186791511各直接ID双读并与分页全文一致。页数以实时结果为准，满100须继续读。
3. 历史EXEC-23/149、EXEC-22/145、旧EXEC-24/152、旧EXEC-24/153及旧EXEC-24/154作者标记均0；本批EXEC-25/154作者入场0，EXEC-25/152及153始终各1。本结果目录未占用；未知并行写入先定位保全，不覆盖或借用作者身份。
4. 153固定c9f4ba8d3226、活动exec、API Git Blob六件全文核同，19971 LF/648723 UTF-8字节；唯一父6994f6b1aaec、一提交六新增零其它改删，旧文件和评论冻结。本批只核旧六件冻结，不重审114窗/三进口/六free。两time canary、直接master反验和公约原句实核，保持read_channel及自检字段。
5. 本轮真实接收、权限/材料/胜任/容量、规范刷新记账明确。154无D00补正；R152-01按主线闭合状态消费，旧152当时NOT_RECORDED不更改。全部材料具足后按三专题连续完成，不把每个场景拆成独立Pro回合或另等Owner技术裁定。

## 6. 六件、自检、唯一回执与发表后G04

恰六新增：MANIFEST.md、brk-range-accounting.md、facts-and-boundaries.yaml、brk-range-accounting-paths.dot、deferred-findings.yaml、evidence.md。不得加第七件。MANIFEST保留read_channel、startup_selfcheck_quote、branch_canary_quotes及self_check三整数；facts按§4重建给定局部状态、实际源码写入/同值写、返回消费、未达与UNKNOWN；不要把GIVEN返回与已独立证明的callee可达性混为一体。

正式证据1–5连续物理行，按真实字节/CRLF/LF/NONE与实际定义或命令实例重开复核，相邻断言确由对应窗支撑；P6 n=m+k。旧153的114和主线0窗不得并入新账。连接器可达P9第1–8、11、13实做，9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。强制源锚不能靠降级回避；无字数/证据数/字段数配额，不重灌旧allocator机器件。

写前fresh核四refs、原PR状态、作者/目录0及未知并行写。只以实际c9f4ba8d3226为唯一父、expected_sha匹配、force=false，在原exec一次提交六新增零其它改删。先回读六件固定提交/活动分支/API Git Blob全文，核LF/UTF-8字节、唯一父、compare净差异和旧对象冻结，再发唯一作者正式评论。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-25 / CORE-USER-BRK-RANGE-ACCOUNTING-154 / FORMAL_DELIVERY

正式评论列真实固定合同/提交/六件、D00=[]、七局部场景及边界、当前模型指定/可见身份与限制、实际自检。发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING；不能先签最终终检。

发表后实际核新评论ID两次并与全分页全文一致；所有顶层评论至空，56原标记＋唯一作者共57各字面一次/实际ID正确、五历史负标记0；四refs（exec为实际154新提交）、开放PR到空、两PR原状态/标题/元数据；六件固定/活动/API全文、唯一父/一提交六新增零其它改删，153/152及所有旧文件/评论冻结。完成后才向Owner报READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING；不启动155。

GitHub暂态错误遵守63：先核远端写入效果，避免重复提交/评论，再按通常首次加三次、10/20/40秒或有效实质工作间隔限次重试；保留expected-head、force=false。未知并行写不覆盖。write_retry_log如实记录，未发生写重试则not_needed；工具发现/调用上限恢复与实际写重试分账，不把单次暂态失败推成权限已失。

最终按198用一个通常3–6行原生短复制块、一个真实作者回执URL，报154六件、唯一提交/P6、实际G04/停写及必要未知，交C11 Pro独审并同轮后继。完整正文、证据、范围与处置留GitHub，不要求Owner拼接长报告或逐项作技术选择。

## 7. 合同就绪及不变边界

154合同由设计分工提出，另一未参与候选构造的分工先从固定源码独建两A前缀、三B后返回帧及两C独立数据克隆模型，再与候选输出比较；97项针对核验、0失败。已核64位对齐/错误码投影、明确helper保持、四旧节点与新N身份、插入/摘除两个计数的不同写序及caller恢复；七例均在其GIVEN边界内成立。此项只证明合同设计就绪，不预签154未来交付、真实可达、运行或实际G04。

C11本次成功首次正式发布154后计3，8/10均未达；C10历史11不变。未来交接须带十五项目规则、有效MA全文/同版共用全文、Mnemosyne命名依据、实际断点与计数。结果先行、问题先入库、授权内持续推进、约三相关专题六件、次一档执行/Pro独审及同轮后继继续适用。

阶段2；不改源码、旧件、主线或PR元数据，不新建分支/PR、合并、强推、删除或跨仓写。完整本地P9、预处理、编译链接、项目程序/脚本、Graphviz、QEMU、实机、真实系统调用NOT_RUN；真实C对象/指针/整数转换/ABI、构建配置、生产可达、映射/寿命/回收、实际后台模型及Extra档位未独立认证。不操作聊天UI或宣称已自动切换模型。
