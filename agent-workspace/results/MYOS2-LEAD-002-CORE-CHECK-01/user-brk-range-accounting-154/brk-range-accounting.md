---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-BRK-RANGE-ACCOUNTING-154"
conversation_display_name: "MYOS2-A-C11a 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-25"
produced_by: "GPT-6.2（系统标识；界面实际名称不可独立读取）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理（Owner指定）"
actual_client_selection: "UNATTESTED"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "d02e263970ef"
execution_parent_short12: "c9f4ba8d3226"
read_channel: "GitHub connector；自制文本/整数/链表有限模型"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
full_local_P9: "NOT_RUN"
D00: []
open_questions: ["真实C转换、ABI、预处理、生产可达、物理映射/回收、对象寿命、实际运行、用户返回寄存器与实际后台档位未证"]
record_type: "batch154_analysis"
inputs_read: ["mykernel/arch/x86_64/myos/syscall.c","mykernel/mm/vm_map/mmap.c","mykernel/mm/vm_map/util.c","mykernel/lib/list/double_list.h","mykernel/lib/list/double_list_macro.h","mykernel/lib/list/double_list_types.h","mykernel/lib/list/double_list_const.h","mykernel/include/linux/kernel/asm-generic/bug.h","mykernel/include/linux/debug/bug.h","mykernel/include/linux/compiler/myos_debug_option.h","mykernel/mm/vm_map/vm_map.h","mykernel/mm/vm_map/vm_map_macro.h","mykernel/mm/misc/mm_misc_types.h","mykernel/mm/vm_map/vm_map_types.h","mykernel/arch/x86_64/mm/page/page_const_arch.h","mykernel/mm/page_alloc/buddy_const.h","mykernel/include/linux/kernel/align.h","mykernel/include/uapi/linux/const.h","mykernel/include/linux/kernel/types.h","mykernel/include/linux/kernel/err.h","mykernel/include/uapi/asm-generic/errno-base.h","mykernel/mm/vm_map/vm_map_const.h","mykernel/include/uapi/asm-generic/mman-common.h","mykernel/include/uapi/asm-generic/mman.h","mykernel/arch/x86_64/include/asm/uapi_mman.h","mykernel/include/uapi/linux/mman.h","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c"]
---

# 第154批 · brk边界请求、增长页数账与缩小消费

**有界结论。** A_CROSS_TASK的长度检查通过且MAP_FIXED提前返回旧对齐地址，停在清理调用前，不能推出真实越界映射；A_ALIGN_WRAP由模64位长度门拒绝，按给定转换模型回旧break。B新VMA和给定合并对象均使total/data增加3，锁定位决定locked增量，列表对象数分别增1和保持；分配失败只证明后清理观察帧之后账本保持。C两个独立数据克隆：整段摘链后数值页账仍保留3页增量，首split失败先写后恢复break而列表保持。所有结论均限于固定time源码及合同GIVEN。

## 1. 数值、类型、历史约束

K=4096、PAGE_SHIFT=12、M=2^64、U=0xffffffffffffffff、T=TASK_SIZE=(1<<47)−K=0x00007ffffffff000。ulong/unsigned long/size_t 64位、int32位为GIVEN；PAGE_ALIGN(x)=((x+4095) mod M)&~4095，而非无界ceil。源码宏链E154-146–159、类型160–162、错误码163–167：IS_ERR_VALUE通过(size_t)(void*)投影，给定保64位模式，门为>=0xfffffffffffff001；do_brk_flags int接口仅对本包编码错误按低32位补码投影，不认证实际C实现/ABI。

默认文本VM_DATA_FLAGS_EXEC=0x77，VM_ACCOUNT=0x00100000，F0=0x00100077，FL=0x00102077（VM_LOCKED=0x2000），外部flags=0。MAP_FIXED=0x10属于mmap flags命名空间，与VM标志分离；__get_unmapped_area实际pgoff=0、vm_flags=0，不是do_brk_flags本地pgoff/flags。SD符号保留：CONFIG_MEM_SOFT_DIRTY两文本支SD=0或0x08000000，未独立认证最终预处理。F0|SD分别0x00100077或0x08100077；FL|SD分别0x00102077或0x08102077；即使SD=0仍有OR赋值。实际配置、ABI未知（E154-168–191）。

旧14 R14-S01字段恢复非VMA事务恢复；21 R21-S01/S02 MAP_FIXED早退、初始NULL对象表达式与严格前驱；22 R22-S01/S02合并目标与先行字段/计数阶段；26 R26-S01/S02旧零长/回绕和simple_vma_link正常仅0；144及R144-N01候选用户原始返回/别名文证限定。固定独审路径/回执及选择器见facts.historical_scope。19只作VMA/页表/零页/匿名限定，75不证明用户包装或运行。旧144低地址、init_uslab/upage、用户包装生成链及旧26 vm_brk_flags不重做；本批仅给定sys_brk局部返回消费，不签用户errno、寄存器或真实sh链接。

GIVEN合法已初始化mm/VMA/list对象、正确相邻关系、无并发。地址只字段值；真实访问对象、物理页、生产可达仍UNKNOWN。vm_area_alloc、vm_area_creat_dup、vm_get_page_prot的给定正常返回明确保持全部受跟踪mm/既有VMA字段、链指针和计数；分配器内部/未跟踪状态不认证。其他先行helper的后返回帧状态直接给定，不推出先行无副作用。旧mm_next_vma/mm_prev_vma从NULL求&vma->list的缺口仍在（E154-030–035、124–134），不能把查询GIVEN返回或已返回合并对象拼成闭合生产链。

## 2. A 两个独立边界请求

共用入口mm.start_brk=0x400000，origbrk=mm.brk=0x00007fffffffd000，def_flags=0，页数与列表对象为有效任意GIVEN。不继承B/C。

|场景|request|newbrk|oldbrk|len模64位|pgoff|
|---|---|---|---|---|---|
|A_CROSS_TASK|0x00007ffffffff001|0x0000800000000000|0x00007fffffffd000|0x3000|0x7fffffffd|
|A_ALIGN_WRAP|0xffffffffffffffff|0|0x00007fffffffd000|0xffff800000003000|0x7fffffffd|

两例49的request<min假；62/63对齐；64 old==new假；70按原request<=mm.brk比较而非对齐值，两例假；103调用do_brk_flags(oldbrk,len,0)。1198本地pgoff，1203 flags & ~VM_EXEC=0、拒绝门假，1205 flags=F0；1207实参(NULL,oldbrk,len,0,MAP_FIXED,0)。E154-001–018、044–051、066–076。

A_CROSS_TASK：953 len>T假，957 MAP_FIXED真，958返回addr=0x00007fffffffd000。1208 IS_ERR_VALUE假（不在末4095编码区）。抵达1212即停在simple_munmap_vma_range(mm,addr,len,&prev)调用之前，ANALYSIS_STOP_BEFORE_UNEXPANDED_CALLEE；其callee没有执行，不知prev、清理/合并/创建或后续返回。区间算术末端T+K>T仅输入关系，不是建立VMA或超界访问。960–984非fixed回调、后置IS_ERR_VALUE、TASK_SIZE-len、offset_in_page检查均未达，不说检查通过。增长账未达，mm.brk仍orig，无非局部写，syscall_return=NOT_DERIVED；非源码循环、正常返回或实测崩溃。

A_ALIGN_WRAP：请求U对齐至0，仍按原请求走增长；953 len>T真，954返ulong -ENOMEM=0xfffffffffffffff4，957固定地址门未达；1208 IS_ERR_VALUE真，1209给定int投影-12；sys_brk103的<0真，104 goto out，122写mm.brk=orig（实际一次同值写），124返回orig。清理、merge、alloc、新字段/link和尾部计数全部未达，不能称0长度增长成功。返回不是-12，真实系统调用和用户包装均未认证。

## 3. B 三个独立增长尾部观察帧

X=0x60000000，L=0x3000=3K，pgoff=0x60000；共同局部入口mm.brk=X，(total,data,locked)=(100,40,7)，map_count/mm_mt.count=4/4。给定caller暂停于103，orig/old=X、req/new=X+L，callee有序实参(X,L,0)，不证明机器上已建立该帧。B_NEW/B_ALLOC_FAIL为1226前：给定前序清理正常0、merge正常NULL、prev=P；前序状态直接给定，内部效果未知。

|对象|给定范围（半开）|
|---|---|
|Q0|[0x5fff8000,0x5fff9000)|
|Q1|[0x5fffa000,0x5fffb000)|
|P|[0x5fffc000,0x5fffd000)|
|R|[0x60004000,0x60005000)|
|N（新）|[0x60000000,0x60003000)|

B_NEW旧环anchor→Q0→Q1→P→R→anchor，prev互逆。vm_area_alloc给定正常返回新N（不同身份、vm_mm=mm，vm_file=NULL，其余初始受写字段有效但值未指定）。1227非NULL过门，1232 vma_set_anonymous写N.vm_ops=NULL；1233–1237写start=X、end=X+L、pgoff=0x60000、flags=F0、page_prot=PROT0（vm_get_page_prot给定不透明返回，保持受跟踪状态）。E154-019–023、066–076、123。

1238 simple_vma_link先180 __vma_link_to_list，util17 INIT_LIST_S先N.next=N再N.prev=N；prev=P非NULL，util22 contains依anchor遍历Q0/Q1/P共3比较2推进，命中P，while条件假。24 list_add_to_next(N,P)传入P.next=R；相邻检查prev/next非NULL、R.prev=P/P.next=R、N不同，__list_add_valid正常true；四写依193–196：R.prev=N、N.next=R、N.prev=P、P.next=N。此时环5个对象而header count/map_count仍4/4，随后util25 header4→5，util27 map4→5，分别记录瞬间5/4与5/5（顺序为header/map）。181 vma_link_file读N.vm_file=NULL，156门假，文件映射锁/引用分支不进入，184正常返回0；不造负返回。E154-037–043、077–081、085–111、123、135–145。

尾1241 total100→103；1242 data40→43；1243 flags&VM_LOCKED假，locked仍7；1245 N.vm_flags OR SD（SD=0同值仍赋值）；1246 return0。给定caller恢复103<0假，106 mm.brk=X+L，119返回X+L。B_NEW_DONE是完整受跟踪数据快照，不带栈；列表[Q0,Q1,P,N,R]、5/5、103/43/7、N范围上表、N.vm_ops/file=NULL、pgoff与PROT0、flags=F0|SD；Q0/Q1/P/R旧范围和未指定字段的符号值保持。

B_MERGE_LOCKED从1219已正常返回有效M、尚未消费1220起，给定def_flags=VM_LOCKED、flags=FL、M.vm_mm=mm、M.vm_flags=FL；M范围/pgprot/是否覆盖请求及实际成员关系未知。1220 nonNULL→1221 goto out；1226–1238均未达。1241/1242各加3，1243真、1244 locked7→10，1245 M.flags OR SD，1246 return0；给定caller106写并119返回X+L。map_count/header count仍4/4，不认证合并过程或真实锁了3物理页。

B_ALLOC_FAIL给定1226正常NULL；1227真、1229 return -12。1232–1246均未达；入口给定保持条件下页账100/40/7、4/4、旧四节点不变；caller103<0真，122同值写X并124返回X。仅后清理局部不变，清理前状态未知，不说整次增长失败无作用。

## 4. C 两个独立B_NEW_DONE数据克隆

每例复制B_NEW_DONE全部明确字段，R范围仍[0x60004000,0x60005000)，不偷接B旧栈、回调或另一路C；新caller min前提start_brk<=X，orig=X+L，按各请求对齐，old=X+L，new分别X/X+K；min、same-page门假，原请求<=入口brk真。82先写请求，再83 simple_do_vma_munmap(mm,new,old)，第三参为end而非len。查询正常返回是GIVEN且进入指定观察点状态除brk先写外为原克隆；不认证初始NULL遍历可达。

C_REMOVE_WHOLE：请求X，实参(mm,0x60000000,0x60003000)。GIVEN find_vma_intersection=N、首split未执行（start==N.vm_start）、prev=P、simple_find_vma(mm,end)=NULL、error=-ENOERR=0，入口1148前。1148 last!=NULL假，短路不读last.vm_start；1156 vma_next_vma(P)从P.list.next=N，转换非anchor到N（E154-024–029、132），N.vm_end=end，比较<=真；1157 N!=last，所选CONFIG_BUG且未覆写BUG_ON的活动宏false不入BUG（E154-116–119；实际配置未知）。1158 __vma_unlink_from_list，util33 contains N：Q0/Q1/P/N共4比较3推进，正常true；util34 map5→4先写。util35 list_header_delete_node再contains N同4/3，list_del_init经__list_del_entry_valid：P/R非NULL/非毒值、P.next=N/R.prev=N，正常true；236 R.prev=P、237 P.next=R；INIT_LIST_HEAD别名→INIT_LIST_S，150 N.next=N、151 N.prev=N；686 header count5→4，687返回节点N。物理摘链先于header计数，map先于嵌套删除。N仍有效自环、范围及其他字段保持，不声称对象已释放。下一次1156 vma_next_vma(P)=R，R.vm_end>end为真（即<=门假），退出循环，1161 error0。caller83条件假，86→success_unlocked，119返回X；brk保持X，不执行out。

限定片段无total/data/locked减量，终值103/43/7、4/4、四节点；对B_NEW入口100/40/7仍多3/3/0而N已摘链。simple_unmap_region未调用，不把注释当执行；不能签真实页表残留、物理泄漏、全局统计错误、运行损坏。E154-057–065、080–115。

C_FIRST_SPLIT_FAIL：请求X+K=0x60001000，实参(mm,0x60001000,0x60003000)，GIVEN find_vma_intersection=N，观察点1139之前，brk已先写请求。start>N.vm_start真；1140 __split_vma(mm,N,X+K,1)，1094 vm_area_creat_dup给定正常NULL保持受跟踪状态，1095真→1096 -ENOMEM。1098–1115 file/open/边界修改/link未达；1141 error!=ENOERR真→start_split_failed→end→1161 return -12。1144 prev、1147 last、尾split、摘链均未达。caller83非零真，84 goto out，122恢复X+L、124返回X+L，非-12。显式序列brk X+L→X+K→X+L；账103/43/7、5/5、N范围/环原样，不能把本例局部无变化推广到旧14首split已经成功的其它失败。E154-052–065、001–018。

## 5. 七场景汇总

|场景|局部callee/停止|sys_brk局部返回|终break|total/data/locked|map/header|写数|
|---|---|---|---|---|---|---:|
|A_CROSS_TASK|清理前分析停|NOT_DERIVED|0x00007fffffffd000|给定不变|给定不变|0|
|A_ALIGN_WRAP|do_brk_flags -12|0x00007fffffffd000|同入口|给定不变|给定不变|1|
|B_NEW|do_brk_flags 0|0x60003000|0x60003000|103/43/7|5/5|18|
|B_MERGE_LOCKED|do_brk_flags 0|0x60003000|0x60003000|103/43/10|4/4|5|
|B_ALLOC_FAIL|do_brk_flags -12|0x60000000|0x60000000|100/40/7|4/4|1|
|C_REMOVE_WHOLE|munmap 0|0x60000000|0x60000000|103/43/7|4/4|7|
|C_FIRST_SPLIT_FAIL|munmap -12|0x60003000|0x60003000|103/43/7|5/5|2|

写数指所选前缀/观察帧显式非局部字段赋值（包括同值或条件同值），不含局部变量或给定新N对象建立。34总写；facts提供按顺序字段before/after、GIVEN初值、每场景阶段表、实际/未达分支与边界、独立克隆和完整B_NEW_DONE。物理链、header计数、map_count、页数账、break字段和实际映射分开；计数中间时点不是原子事务。DOT与facts全属性相同，5个B/C GIVEN帧不伪造前序查询/merge的生产链，B_NEW_DONE→C两条snapshot_import是数据关系，不是执行后继，分析停无后继。

本批仅条件性局部关系观察（deferred），继承R14/R21/R22/R26/R144及R19/R75范围，不新立旧根因或宣称缺陷已经修复。191新窗与153/144等旧证据分账。实际C转换/ABI/编译配置/映射/寿命/真实生产入口、运行未证；预处理、编译链接、项目程序脚本、Graphviz、QEMU、实机、真实syscall及完整本地P9 NOT_RUN。
