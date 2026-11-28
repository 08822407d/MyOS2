---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-MALLOC-REINIT-152"
conversation_display_name: "MYOS2-A-C11a 代码分析执行"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-25"
produced_by: "GPT-6.2（系统标识）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理"
actual_client_model: "UNATTESTED"
actual_backend_model: "UNATTESTED"
actual_reasoning_effort: "UNATTESTED"
date: "2026-11-26"
date_timezone: "Asia/Shanghai"
phase: 2
base_snapshot: "time@a039d9803ade"
source_pin_short12: "a039d9803ade"
direct_master_short12: "e8e60cd19540"
lead_pin_short12: "7ed4e9279b7d"
execution_parent_short12: "07d636217117"
read_channel: "GitHub connector；自制文本/整数辅助核验"
status: "AUTHOR_PREPUBLICATION"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
full_local_P9: "NOT_RUN"
open_questions: ["实际生产入口、C指针/对象/映射/寿命、真实ABI/标准/DEBUG/编译器/链接/运行及311 callee未证；N1547外部标准独立外证UNKNOWN；后台档位未证。"]
record_type: "TECHNICAL_ANALYSIS"
inputs_read: ["myinitramfs/mylib/myglibc/malloc/malloc.c","myinitramfs/mylib/myglibc/malloc/_malloc.h","myinitramfs/mylib/include/utils.h","myinitramfs/mylib/util/list.c","myinitramfs/mylib/util/bitmap.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/mylib/myglibc/malloc/CMakeLists.txt","myinitramfs/mylib/util/CMakeLists.txt"]
D00: []
---

# 第152批 · 再次初始化的根重建与残留状态消费

**结论（GIVEN条件下）**：两次独立再初始化均让活D/Qd脱离登记根而保持owner、地址、位图和残余链字段；静态对象仍为原对象，地址被重写，计数复位而bitmap保留。后续两个malloc分别产生局部非NULL/NULL，但均停在311调用边界，不能认定函数返回。此为静态条件模型，不是生产缺陷实测。

## A · 根重建
共同H=0x200000为用户分配器局部页粒度；i档O=32<<i、N=H/O。全部对象、header、链表、bitmap及转换合法，byte8、int/unsigned32、pointer/size_t/long/long long/bitmap_t64、自然8对齐、无未说明别名、并发或回绕。地址仅整数投影。C_i/S_i/P_i为固定静态对象；D和Qd额外存活，Dmap独立两个全1 word。ENTRY cache C0…C15/16；used页Qd,P15…P0/17；free页空。initiated=true、brk_end=0x60000000。其它15cache各free[S_i]，CF=N/CU0。C9 sparse为used[S9],full[D]、S9.free127、map(1,0)、CF127/CU129；full为full[S9,D]、S9.free0、全1、CF0/CU256；两例total2。

直接进入init是额外GIVEN，非正常malloc入口推导。176 brk(0)返回B；177–179三header重置count0/self-loop。GLOBAL_ROOTS_RESET Qd脱根，但C9内部三根未重置，D仍在旧full根；C9还未重新加入全局。每轮184自环C节点、185尾插全局，再187–189重置内部header；192自环S节点，194–196计算O/N/局部cm_size（按sizeof(bitmap_t)=8，未用于清零），198尾插free，199–207更新base/CF/CU/total/colormap/owner/total/free。211自环P节点，212–213给定brk正常返回W并赋brk_end与P.vaddr，214尾插free页，216 upage_alloc复用刚入的P（114真、116 remove、117等待假、119 container、120头插used、140正常返回），216–218绑定页、地址和owner。每次INIT节点先自环再insert，helper内部delete只操作当前新链/自环，不沿残存旧链摘旧邻居。220再写true、221正常void返回。源E152-035–038、043–052、089–101。

|i|O|N|brk实参|brk返回|P|free页count|used页count|事件|
|---:|---:|---:|---|---|---|---|---|---|
|0|32|65536|0x60200000|0x60200000|P0|0→1→0|0→1|—|
|1|64|32768|0x60400000|0x60400000|P1|0→1→0|1→2|—|
|2|128|16384|0x60600000|0x60600000|P2|0→1→0|2→3|—|
|3|256|8192|0x60800000|0x60800000|P3|0→1→0|3→4|—|
|4|512|4096|0x60a00000|0x60a00000|P4|0→1→0|4→5|—|
|5|1024|2048|0x60c00000|0x60c00000|P5|0→1→0|5→6|—|
|6|2048|1024|0x60e00000|0x60e00000|P6|0→1→0|6→7|—|
|7|4096|512|0x61000000|0x61000000|P7|0→1→0|7→8|—|
|8|8192|256|0x61200000|0x61200000|P8|0→1→0|8→9|—|
|9|16384|128|0x61400000|0x61400000|P9|0→1→0|9→10|185 C9重新入全局；187–189重置三内部根；189后D脱根（非释放）|
|10|32768|64|0x61600000|0x61600000|P10|0→1→0|10→11|—|
|11|65536|32|0x61800000|0x61800000|P11|0→1→0|11→12|—|
|12|131072|16|0x61a00000|0x61a00000|P12|0→1→0|12→13|—|
|13|262144|8|0x61c00000|0x61c00000|P13|0→1→0|13→14|—|
|14|524288|4|0x61e00000|0x61e00000|P14|0→1→0|14→15|—|
|15|1048576|2|0x62000000|0x62000000|P15|0→1→0|15→16|—|

每例17次brk（176一次+213十六次）；16次复用，124/125动态malloc/brk未达。每个opaque调用GIVEN独立frame：指定metadata、headers/list、base_colormaps全部绑定、静态及D bitmap载荷、initiated及已确定字段、对象有效性保持；内核/VMA/中间行为UNKNOWN。本层源赋值另计。所有实际源/助手写入逐条前后值在facts.variants.*.ordered_delta，A各1131条（包括同值写），每次调用及返回另列；快照按前缀可重建。局部变量单独记录，链式赋值的内层值→外层值顺序不认证真实机器存储时序。

REINIT_RETURN cache C0…C15/16，used页P15…P0/16，free页空；每cache仅free[S_i]/1、used/full空、total1、base=S_i、CF=N/CU0；S.free=total=N、owner=C、upage=P、virt_addr=W_i；P.vaddr=W_i、owner=S_i。W0=0x60200000,W9=0x61400000,W15=brk_end=0x62000000。i9才清C9三头使D脱根；D.prev sparse=C9.full.anchor、full=S9.uslab_list；D.next两例C9.full.anchor；Qd.prev=used_upage_lhdr.anchor、Qd.next=P15.upage_list，均全程未写。邻居已重写，残余指针不保证互逆。init全部实际helper仅写metadata/链表，brk frame保护全部图与表，204仅赋指针；位图载荷全程保持。未证明释放、泄漏、UAF或实际内存破坏。

## B · 根、给定owner集合、独立探针
C9三根实际遍历是ROOT，持续给定owner={S9,D}不是从根生成。ENTRY全谓词真；REINIT_RETURN根S9 CF/CU128/0，owner和128/128，CU少计128，total1对根1真对owner2假。D自身free0/空0仍真；S9 sparse空127差1、full空0差128，两个BITMAP总体假。分类此时真，D不在任何根不赋类别；LENGTH真、OWNER_REGISTRATION假。谓词只评价指定完整时点，GLOBAL_ROOTS_RESET/i9中途不提前套用后态。

每例从同一REINIT_RETURN快照独立派生三探针，非控制序列插入：PAGE_ROUND_DOWN取键；旧0x54000080→0x54000000 NULL/16比较16推进；旧0x51400080→0x51400000 NULL/16/16；新0x61400080→0x61400000 P9/7/6，候选P15…P9，break不读P9.next或后续。search本身无owner读取；P9.owner=S9是快照字段；无写、无free、正常返回。E152-003–006,039–042。六叶无执行边，旧地址找不到不等于真实堆空间丢失。

## C · 同一后态malloc前缀
两例malloc(0x4000) initiated=true跳init，255过大门假，262–264十比较九推进到C9。269 used=0，280 free=1，282 remove_head非NULL，283等待假，285 container得S9，286加入used；free count1→0 used0→1，free字段128，分类假。295 scan：
- SPARSE idx1，word0源码求值2次，296真；298 offset0x4000，299 ret_val局部0x61404000，301/302/303 free/CF127、CU1，304 OR使(1,0)→(3,0)，空126差1，分类恢复真。
- FULL idx128，源码bm_unit求值128次、word0/1，296假；offset/ret_val赋值、三个计数及set均未达，free/CF128 CU0，全1，初值局部NULL、分类仍假。
308两例真；310临时count0→1，真实free链仍self-loop/0成员，LENGTH仅C9.normal_slab_free假。311参数是选中的uslp=S9，停止在调用边界，不展开callee，无new_uslp可消费返回；312–321及327未达，malloc正常外返UNKNOWN。无备用R；不能套151 B的正常返回。页链、其它15档、D/Qd/Dmap保持，slab_total1不随临时count改变。E152-056–071、102–105。

|例|时点|CF/CU|ROOT和/比较|OWNER和/比较|S9 free-空|BITMAP root/owner|CLASS|REG|LENGTH|TOTAL root/owner|
|---|---|---|---|---|---:|---|---|---|---|---|
|SPARSE|ENTRY|127/129|127/129 T|127/129 T|0|T/T|T|T|T|T/T|
|SPARSE|REINIT_RETURN|128/0|128/0 T|128/128 F|1|F/F|T|F|T|T/F|
|SPARSE|SLAB_SELECTED|128/0|128/0 T|128/128 F|1|F/F|F|F|T|T/F|
|SPARSE|SCAN_RETURN|128/0|128/0 T|128/128 F|1|F/F|F|F|T|T/F|
|SPARSE|OBJECT_UPDATE_DONE|127/1|127/1 T|127/129 F|1|F/F|T|F|T|T/F|
|SPARSE|REFILL_CALL_BOUNDARY|127/1|127/1 T|127/129 F|1|F/F|T|F|F|T/F|
|FULL|ENTRY|0/256|0/256 T|0/256 T|0|T/T|T|T|T|T/T|
|FULL|REINIT_RETURN|128/0|128/0 T|128/128 F|128|F/F|T|F|T|T/F|
|FULL|SLAB_SELECTED|128/0|128/0 T|128/128 F|128|F/F|F|F|T|T/F|
|FULL|SCAN_RETURN|128/0|128/0 T|128/128 F|128|F/F|F|F|T|T/F|
|FULL|OBJECT_UPDATE_SKIPPED|128/0|128/0 T|128/128 F|128|F/F|F|F|T|T/F|
|FULL|REFILL_CALL_BOUNDARY|128/0|128/0 T|128/128 F|128|F/F|F|F|F|T/F|

D位图个体比较始终真；实际header逐个从anchor遍历（不从count造成员），最后唯一失败C9.normal_slab_free。facts阶段保存实长与count、有序delta、实际字段读写集合，观察不混入执行写集。图同facts.graph。

## 历史排重与边界
144/200 B首次init、brk整数投影；145/201选档/门；146/203位图容量与无清零；149/206临时count与未展开callee；150页复用；151正文/deferred、有关source_index/模型块与R151-01独审。旧149文证合读150 D00和C10独审§3，RETURN历史不改；151六件仅冻结字节核及定点消费，不重审/重跑。 新量仅是明确再次init后的根/owner差异和残留位图如何从该后态消费到311；不重复申报146无清零、149临时count、151扫描上限为新独立缺陷。D00=[]、R151-01由C11补注闭合，旧成果/评论不改。

五份CMake原命令实例仅证明文本候选：myinitramfs根未直接列mylib，mylib列myglibc、csu、util，myglibc static目标及malloc/util文件候选；不推实际sh链接、ABI/标准/DEBUG、编译器。不读仓外musl。N1547外部标准独立外证UNKNOWN。未作完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机运行。实际生产入口/真实C对象、指针、映射、寿命与311后果未知。
