---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
batch_id: "CORE-USER-FREE-REINIT-153"
conversation_display_name: "MYOS2-A-C11a 代码分析执行"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-25"
produced_by: "GPT-6.2（系统标识）"
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
lead_pin_short12: "55488278ab63"
execution_parent_short12: "6994f6b1aaec"
read_channel: "connector"
status: "AUTHOR_PENDING_MAINLINE_REVIEW"
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
full_local_P9: "NOT_RUN"
open_questions: ["真实生产入口、C对象/指针、ABI/标准/DEBUG/编译器/链接、映射/生命周期、实际运行、原malloc外返、311 callee、N1547独立外证及实际后台档位未证；R152-01待主线判闭。"]
record_type: "batch153_analysis"
inputs_read: ["myinitramfs/mylib/myglibc/malloc/malloc.c","myinitramfs/mylib/myglibc/malloc/_malloc.h","myinitramfs/mylib/include/utils.h","myinitramfs/mylib/util/list.c","myinitramfs/mylib/util/bitmap.c","mykernel/scripts/options_flags.cmake","mykernel/debug/panic.c","myinitramfs/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/mylib/myglibc/malloc/CMakeLists.txt","myinitramfs/mylib/util/CMakeLists.txt"]
D00: ["R152-01"]
---

# 第153批 · 再次初始化后的free消费边界

**有界结论：** 两个旧地址失配均只推至342页对象前提缺失；两个新地址即使命中也先因free==total进入348源码循环；独立U_S副本第一次free清新增bit1并条件性正常到尾，第二次针对残留bit0的地址仍被先行空slab门挡住。仅四项元数据写，不是实测释放或故障。R152-01的记账补注在MANIFEST，旧152的技术文证允许限定消费、整体RETURN历史冻结，待C11判闭。

## 1. 固定消费与共同GIVEN

源基线time a039d9803ade，旧152固定6994f6b1aaec的facts.given.shared_entry、variant.entry_override、ordered_delta及stage实际回读；按delta_schema逐条核before后赋after，不重跑旧分析。三个导入各有506个受跟踪非局部字段（位图全同word以区间压缩），前缀为：
|导入|固定选择器|核过前缀|S9.free/CF/CU|map words|C9 free/used/full成员和count|
|---|---|---:|---|---|---|
|R_S|variants.REINIT_SPARSE.stages[name=REINIT_RETURN]|1131|128/128/0|(1,0)|[S9]/1；[]/0；[]/0|
|R_F|variants.REINIT_FULL.stages[name=REINIT_RETURN]|1131|128/128/0|(全1,全1)|[S9]/1；[]/0；[]/0|
|U_S|variants.REINIT_SPARSE.stages[name=OBJECT_UPDATE_DONE]|1149|127/127/1|(3,0)|[]/0；[S9]/1；[]/0|

五个clone独立复制对应全量受跟踪非局部字段；C2才从本C1后态直接承接，不重新导入。U_S**不是**152的prefix1150（310临时count=1），不导入旧控制栈、旧局部ret_val、308/310/311及后继效应。输入0x61404000是本合同另给的数值，恰与旧局部值同值，不证原malloc曾正常外返，不声称真实回调/重入/并发。此处源E153-012–014只定位旧局部与调用边界，不将旧malloc操作计入本批。

共同局部H=0x200000，byte8、int/unsigned32、pointer/size_t/unsigned long/long long/bitmap_t64，自然8对齐；地址及void*差仅字节整数投影。给定活对象/转换/访问有效且无未说明别名、并发、回绕；A在342前明确缺page前提。旧brk frame仅旧模型假设，本批没有brk或新opaque helper返回。initiated=true；若DEBUG块编入334等待条件假，若排除则N/A，实际预处理未知。所有输入非NULL，337门假。

cache C0…C15/16，used页P15…P0/16，free页空/0；P_i.vaddr=S_i.virt_addr=0x60000000+(i+1)H，P_i.owner=S_i、S_i.owner=C_i。C9 O=0x4000/N=128、base=S9、slab_total1、colormap=cm_16KB[2]、upage=P9、新地址0x61400000。D/Qd/Dmap活且脱根，D.owner=C9、D.free0/total128、D.V=Qd.vaddr=0x54000000、D.upage=Qd、Qd.owner=D、Dmap两个全1保持；D.prev sparse/clone为C9.normal_slab_full.anchor，full为S9.uslab_list；D.next均full.anchor；Qd.prev旧used anchor、Qd.next=P15.upage_list。旧链字段不保证互逆，不能由其制造根成员。其它15cache及位图、页/链字段均按固定前缀导入。无备用R。

## 2. A · 旧地址失配后的分析停止

A_OLD_DYNAMIC和A_OLD_STATIC各复制R_S，一次free，分别给0x54000080和0x51400080；E153-001–011、015–019。341调用search按PAGE_ROUND_DOWN得到0x54000000/0x51400000；ret初始NULL，156读used anchor.next，157与anchor比较，160转换，161读候选vaddr。P15…P0的16次比较皆不等，16次158推进（最后至anchor），168正常返回NULL；不读owner或header.count，不访问Qd。free的局部upgp=NULL。下一句342需要读取upgp->uslab_p，无有效page对象前提，分析停止在该访问前：ANALYSIS_STOP_MISSING_PAGE_OBJECT_PREMISE。不是源码NULL拒绝分支、正常free返回、源码循环或真实崩溃观测。uslp/uscgp、obj_idx、assigned及释放后继不推导；实际截至边界metadata/bitmap/链零写。不能据此保证继续执行仍零副作用。

## 3. B · 新地址命中后的空slab先门

B_NEW_SPARSE复制R_S、B_NEW_FULL复制R_F，各free(0x61400000)。search键同地址，P15…P9共7比较、6推进；匹配后163保存P9、164 break，未读P9.next或更后候选，168正常返回。342读P9.uslab_p=S9、343读S9.uslabcache_p=C9；345实际读free=total=128，真入348 while(1)（SOURCE_LOOP_EMPTY_SLAB）。两位图仍分别(1,0)/(全1,全1)，仅文档状态观察，不是这次free的源位图读取。351未达、无obj_idx，352/358及全部后继计数/迁链/回收未达。没有“assigned非零之后拒绝”，无正常外返，实际截至loop零写。E153-018–022。

## 4. C · 独立克隆后两次free

C_CLONED_UPDATE从U_S独立复制非局部字段；第一次free(0x61404000)与第二次free(0x61400000)之间没有其他调用/写。不是继续原152的malloc。

C1：337门假，search键0x61400000，7比较6推进返回P9；342/343读owner S9/C9。345的127==128假；351字节差0x4000、O=0x4000，商1余0。352调用bm_get_assigned_bit，unit_idx=0、mod=1、int模型mask=2，word0=3，unsigned返回2（非布尔1），!返回假；E153-020–021、057–058。358调用bm_clear_bit，64位1ULL<<1=2，补掩码0xfffffffffffffffd，word0 3→1，word1保持0，正常void；然后359/360/361计数：
|实际写入源|字段|before→after|
|---|---|---|
|358 → bitmap.c:21|cm_16KB[0]|0x3→0x1|
|359|S9.free|127→128|
|360|C9.nsobj_free_count|127→128|
|361|C9.nsobj_used_count|1→0|

364更新后128==1假，370更新后128==0假；不进入列表helper；381 free.count0>2假，回收体未达；391正常到函数末尾（给定模型void，不是实测释放成功）。C9仍used[S9]/1、free/full空/count0、slab_total1；旧bit0仍1。根计数与owner和差128，位图空127与free128差1，分类变假。全局页/其他cache/位图/D/Qd无写；无uslab_alloc/uslab_free/upage_alloc/upage_free/brk或链表变更助手调用。E153-022–029、059。

C2：直接承接C1后态，free(0x61400000)，search仍P9（7/6），342/343读同owner；345读128==128真，348源码循环。残留bit0=1仅状态观察；351没有求槽商，assigned/clear未调用，零元数据写；不是重复释放检测、清旧位或第二次正常void完成。

## 5. 实际调用、观察域与停止点

|调用|输入|键|search正常返回|比较/推进|free终点|元数据写数|
|---|---|---|---|---|---|---:|
|A_OLD_DYNAMIC|0x54000080|0x54000000|NULL|16/16|ANALYSIS_STOP_MISSING_PAGE_OBJECT_PREMISE|0|
|A_OLD_STATIC|0x51400080|0x51400000|NULL|16/16|ANALYSIS_STOP_MISSING_PAGE_OBJECT_PREMISE|0|
|B_NEW_SPARSE|0x61400000|0x61400000|P9|7/6|SOURCE_LOOP_EMPTY_SLAB|0|
|B_NEW_FULL|0x61400000|0x61400000|P9|7/6|SOURCE_LOOP_EMPTY_SLAB|0|
|C_CLONED_UPDATE_C1|0x61404000|0x61400000|P9|7/6|VOID_FALLTHROUGH|4|
|C_CLONED_UPDATE_C2|0x61400000|0x61400000|P9|7/6|SOURCE_LOOP_EMPTY_SLAB|0|

合计5入口6调用、60/56、两分析停止、三源码循环、一条件性void完成、四源字段写。facts逐阶段保存locals、真实读集合（DEBUG条件读单列）、不写集合及四条有序delta；分析谓词中的bitmap/链遍历不混入源读取。未达351者obj_idx/assigned NOT_DERIVED。图的5条snapshot_import仅数据复制来源，唯一C1 VOID_FALLTHROUGH→C2 ENTRY为受控后继顶层调用，分析停止/源码循环叶无执行后继。无原malloc到新free的控制边。

C9根成员从三个header.anchor实际遍历、不是count；给定活owner集合始终{S9,D}。ENTRY与中途stage的完整域比较见facts，主要终点如下（T/F为文档比较，不是源码门）：
|调用|终点 CF/CU|ROOT和/比较|OWNER和/比较|S9 free−空位|CLASS_ROOT|LENGTH_ROOT|
|---|---|---|---|---:|---|---|
|A_OLD_DYNAMIC|128/0|128/0 T|128/128 F|1|T|T|
|A_OLD_STATIC|128/0|128/0 T|128/128 F|1|T|T|
|B_NEW_SPARSE|128/0|128/0 T|128/128 F|1|T|T|
|B_NEW_FULL|128/0|128/0 T|128/128 F|128|T|T|
|C_CLONED_UPDATE_C1|128/0|128/0 T|128/128 F|1|F|T|
|C_CLONED_UPDATE_C2|128/0|128/0 T|128/128 F|1|F|T|

R_S、R_F和U_S入口CLASS均真；U_S入口root和127/1、owner和127/129、S9空126差1，LENGTH真。所有指定完整状态CACHE_ROOT真、CACHE_OWNER_GIVEN假（owner CU多128）、BITMAP_ROOT/OWNER合取假、D自身位图真；OWNER_REGISTRATION假（D0次）；slab_total1对root1真对owner2假；51 header count均与anchor实长相符，C1更新后/完成及C2循环CLASS假。A谓词只截至342前，不是342后运行后态。

## 6. 历史排重、构建与未知

147已有search/低位free/空slab门，151已有最后占用槽释放后的分类差异和地址识别；152已有再次init后态与残留位图、311边界。新组合是明确再init派生状态进入free的首对象门/先行空slab门，以及独立U_S副本释放新增bit1后残留bit0再次被门挡住；不重复报旧无清零、旧free==0迁链条件或NULL检查缺口为新源码缺陷。旧152技术文证PASS_PENDING_LOCAL，R152-01记账待判闭、整体RETURN_DOCUMENTATION保留；R151-01合读补注及149/150历史不改。

五份CMake真实命令实例仅候选文本：myinitramfs根直接myinit/myshell，未直接mylib；mylib→myglibc/csu/util，myglibc建mylibc STATIC并malloc目录，malloc/util GLOB C/ASM、target_sources。实际sh链接、ABI/标准/DEBUG/编译器未知，不读仓外musl。低位只实际求idx1，N1547高位独立标准外证UNKNOWN。完整本地P9、预处理、编译链接、项目程序脚本、Graphviz、QEMU、实机NOT_RUN；真实C对象/指针、映射、寿命、生产入口、原malloc外返/311 callee和实际后台档位未证。
