---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_executor_contract
conversation_display_name: "MYOS2-A-C11 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份）"
model_per_owner: "继承Pro主线独审与同轮后继安排"
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 974a51a9aa96
execution_pin_short12: 07d636217117
read_channel: "GitHub connector；自制文本/整数辅助核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PUBLISHED_PENDING_NEW_EXECUTOR_START
followup_id: CORE-USER-MALLOC-REINIT-152
executor_display_name: "MYOS2-A-C11a 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-25
executor_model_per_owner: "次一档GPT-6.2 Sol＋Extra／高推理"
actual_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-reinit-152/
allowed_files: [MANIFEST.md, malloc-reinit.md, facts-and-boundaries.yaml, malloc-reinit-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "仅本文件新增正式源码引文；既有151的138窗独立复核另账。"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count: {conversation: C11, before_release: 0, after_successful_152_release: 1, thresholds: [8, 10], reached: false}
previous_mainline_design_count: {conversation: C10, distinct_batches: 11}
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
open_questions: ["真实运行、ABI/标准/DEBUG/链接/对象生命周期与后台档位未证；旧C10c发表后G04历史未证。"]
---

# 213号合同 · 第152批 · 再次初始化的根重建与残留状态消费

发给新「MYOS2-A-C11a 代码分析执行」／EXEC-25。先按[212恢复包](212-executor-continuation/00-C10c-to-C11a-recovery-package.md)与[完整启动词](212-executor-continuation/01-MYOS2-A-C11a-receiver-startup-prompt.md)独立接收和加载，再连续完成本合同。旧C10c的151实际已交付，主线[151独审](reviews/CORE-USER-MALLOC-STATE-CONSUMPTION-151-review.md)在R151-01消费补注下PASS_PENDING_LOCAL；本批D00=[]，不重做或改写151。

本批三个相关专题：A重新初始化对根与旧metadata的重建；B残留bitmap、根成员/owner集合及地址索引观察；C同一后态的一次malloc前缀。只有**两条独立受控序列REINIT_SPARSE、REINIT_FULL**，合计2次直接init_uslab＋2次顶层malloc；另外6个独立只读upage_search探针不插入执行序列。A/B/C不是六个重跑入口。

## G00 · 新身份实际接收后预检

按212完整执行十五项目规则、有效MA采用/全文及同版共用十五条、Mnemosyne任务局部接收命名；新对话不得继承旧方/主线已读。MANIFEST分别记execution_handoff_receive和guidance_refresh，不另造第七文件或接收评论。

直接lead为本轮正式评论所列实际发布提交；exec=07d636217117，time=a039d9803ade，直接master=e8e60cd19540。原PR16 Open/Ready、原题PR17 Open/Draft，原head/base master及未合并；开放PR及PR17顶层评论无since完整分页至空。按同提交检查点核46历史＋3本轮共49标记各唯一及实际ID，关键新评论/151作者/前次C11恢复双读。151固定/活动/API六件与单父净差异核同，旧成果/评论冻结；152目录和新EXEC-25/旧EXEC-24的本批作者均0，历史两负标记0。若出现未知写入先保全定位，不覆盖或强推。

## 1. 增量与历史消费边界

144已经两组16轮首次init/brk数值；145已选档、过大请求及初始化门先后；146已有静态bitmap容量与init不直接清零；149已有refill临时计数；150已有页描述符复用；151已有独立GIVEN满图/正free的消费。本批新量是**显式再次进入init后，旧活动态metadata仍保留owner却脱离根链，静态bitmap保留而字段重置，再从这一实际模型后态消费到refill调用边界**。不把旧无清零、旧临时count或旧scan-limit单独重报新缺陷。

旧材料只定点消费：144独审/200的B入口与brk整数边界；145/201初始化门和选档；146/203静态bitmap映射及未证；149/206的临时计数与未展开callee；151正文、deferred、本轮独审R151-01及有关source_index/模型块。旧151六件须冻结字节核同，但不要求重新逐语义遍历604KB facts、重跑7入口10调用或重核旧138窗全集。149旧文证继续合读150 D00与C10§3，旧RETURN和所有原件保持。

## 2. 共同给定、身份和调用frame

固定time下用户分配器局部H=PAGE_SIZE=CONST_2M=0x200000，不是内核页尺寸。GIVEN byte8，int/unsigned32，pointer/size_t/unsigned long/unsigned long long/bitmap_t64位，BITMAP_UNITSIZE=64，自然8对齐。metadata、静态数组/bitmap实际存储、header/list、所需转换均活、足够大、有效、互不混淆且无未说明别名；无并发、回绕。地址、加法、商及偏移只作整数投影，真实C指针操作、ABI、标准、映射、对象寿命与生产可达不由此证明。

对象字典对两例共享，i=0…15：

| 符号 | 真实对象及初态 |
|---|---|
| C_i | uslab_cache_groups[i]，obj_size O_i=32<<i，normal_base_slab_p=S_i |
| S_i | base_uslabs[i]，total N_i=H/O_i，owner=C_i，upage=P_i，colormap=base_colormaps[i] |
| P_i | base_upages[i]，owner=S_i，旧vaddr与S_i.virt_addr均为0x50000000+(i+1)H |
| D | 额外活slab，与全部静态对象不同；owner=C9，total128/free0，colormap=Dmap，独立Dmap[2]全1，virt_addr=0x54000000，upage=Qd |
| Qd | 额外活page，与全部P_i不同；vaddr=0x54000000，owner=D |
| Dmap | 两个64位word全1；不与任何静态bitmap重叠，整个序列保留 |

C9目标O9=16384、N9=128，静态bitmap对应cm_16KB[2]。指针表base_colormaps在ENTRY采用源码89–106的顺序绑定；所有静态bitmap按该绑定和实际数组容量给定；除S9变体外，其他15个数组所有word为0。D/Qd/Dmap全程存活，字段保持与实际已释放/泄漏是不同结论。

全局cache链[C0,…,C15]/count16；used页[Qd,P15,…,P0]/17；free页[]/0。initiated=true，brk_end=0x60000000。ENTRY时各已登记双向链的prev/next由上述成员次序与各自anchor唯一确定，count与实长一致。此合法链约束不强加到重建后的脱根D/Qd残余指针上；邻居可已重写，残余指针不保证互逆或仍构成合法登记链。其余15个cache仅free[S_i]/1，used/full空、slab_total1、CF=N_i/CU0，S_i.free=N_i。CF/CU是nsobj_free_count/nsobj_used_count的文档缩写。

两入口仅覆盖C9旧状态：

| ID | C9 used/free/full | S9.free与bitmap | C9 slab_total / CF / CU |
|---|---|---|---|
| REINIT_SPARSE | [S9]/1；[]/0；[D]/1 | 127；word(1,0) | 2 / 127 / 129 |
| REINIT_FULL | []/0；[]/0；[S9,D]/2 | 0；两word全1 | 2 / 0 / 256 |

入口字段、bitmap、链分类和所有给定owner成员的计数相合。D链字段在两例由其不同full链次序分别确定，不把两例D.prev混用。

**控制直接再次调用init_uslab为额外GIVEN。** 不声称initiated=true的正常malloc会自动进入；实际调用者与生产可达UNKNOWN。源码init内部没有initiated拒绝门，调用入口本身不自动清false。

每例17个opaque brk调用分别GIVEN正常返回：

- brk(0)返回B=0x60000000。
- 第i轮请求及返回W_i=B+(i+1)H。
- 每次调用从入口至正常返回，除给定返回值，不改变本批已指定metadata、headers/list、base_colormaps指针表的全部源映射绑定、静态及D bitmap载荷、initiated和此前已确定字段；本层源码在调用前/后自己的写另计。已知对象的类型、寿命与无别名保持。未跟踪内核/VMA/映射及调用中间行为UNKNOWN。
- 这是逐调用模型frame，不是brk实现天然无副作用、真实扩展已成功或真实C指针运算有效的证明。

## A · 根重建、旧节点与新关联

每例完整展开一次174–221的init_uslab，用一张可参数化16行表即可，不能只给最终count代替循环。

必记三个时点：ENTRY；GLOBAL_ROOTS_RESET（176查询及177–179三全局header初始化完成）；REINIT_RETURN（16轮完成、220赋true后正常到221尾）。另在16行表中注明i=9的C9三slab头重置与D脱根时点。

GLOBAL_ROOTS_RESET三个全局header均self-loop/count0；Qd从全局页根不可达，但对象字段未被清理。此时尚未执行逐cache的187–189，直接持有C9时其旧内部slab根仍保持；不能提前把C9所有子根写成空或把D写成已经由该子根摘除。后续到i=9重置内部三头，才消除D的该根可达性。新cache对象重新加入全局根的时间也须与子根重置区分。

16行各列i、O_i/N_i、brk实参/返回W_i、P_i身份、free页0→1→0、used页i→i+1；共同表可由两变体引用。每轮先INIT静态C_i/S_i/P_i节点再入新链，不能沿旧指针摘旧邻居；upage_alloc取回刚放free尾的同一P_i，16次均走复用分支。内部124/125的malloc/brk未到，外层17次brk仍计。

最终应独立核出：

- cache链[C0,…,C15]/16，used页[P15,…,P0]/16，free页[]/0。
- 每C_i仅free[S_i]/1，used/full空、slab_total1、base=S_i、CF=N_i/CU0。
- S_i.free/total=N_i，owner=C_i、upage=P_i、virt_addr=W_i；P_i.vaddr=W_i、owner=S_i。
- W0=0x60200000，W9=0x61400000，W15=brk_end=0x62000000；initiated全程原已true，220再次写true。
- D/Qd/Dmap各字段均保持，但D不在C9任何slab根、Qd不在全局page根。D.owner=C9、Qd.owner=D、D.upage=Qd仍存在，不等于仍登记或已释放。静态对象身份不变，其地址字段被重写。
- 静态bitmap载荷均未清零。证明须覆盖init全部实际调用、list助手、upage复用和每次brk frame，不能只以“搜不到memset”作结论。

分别给实际有序写与未写对象集；可用循环索引及模板描述重复同构写，但必须能唯一重建各字段前后值，特别D/Qd链字段。不要把旧失去根可达的对象写成实际已free、真实泄漏、UAF或内存破坏。

## B · bitmap残留、集合区别及只读地址探针

REINIT_RETURN：S9.free/CF128、CU0；SPARSE仍map(1,0)，前128位空127，free比实际位图空位多1；FULL仍全1、空0，free多128。其他15静态bitmap及Dmap保持。

**不追改151谓词定义。** 本批明确另定义下列观察域，只在指定完整时点评价，不当源码校验门：

| 观察 | 域与比较 |
|---|---|
| CACHE_ROOT | 从C9三header的anchor实际遍历成员求Σfree和Σ(total−free)，与CF/CU比较；不能用count虚构成员 |
| CACHE_OWNER_GIVEN | 从本合同持续活且owner=C9的给定集合{S9,D}求同两和，与CF/CU比较；D脱根不从该集合删除 |
| BITMAP_ROOT | 逐实际根成员比较free与前total位空位数 |
| BITMAP_OWNER_GIVEN | 对S9及D分别做同一比较，再列合取，不隐藏D自身仍一致 |
| CLASS_ROOT | free成员free=N、used成员0<free<N、full成员free=0；只评价根成员，D无根类别 |
| OWNER_REGISTRATION | 每个给定owner成员在三根恰出现一次；D出现0次则假，不替其假设full归类 |
| LENGTH_ROOT | 每个实际header.count与anchor遍历所得实长比较；给出具体失败header |
| SLAB_TOTAL_ROOT／OWNER | normal_slab_total分别对根成员数及owner给定成员数，不混成一种“总数” |

入口两变体上述相应比较均真。REINIT_RETURN时根集合{S9}，CACHE_ROOT真（128/0），OWNER集合仍{S9,D}、求和128/128，CACHE_OWNER_GIVEN假，CU少128；slab_total1对根1相合、对owner2不合。S9位图比较假、D自身真，两个BITMAP总体均假；CLASS_ROOT真、OWNER_REGISTRATION假、LENGTH_ROOT真。每组保留各自D旧链prev/next，不能由这些残余指针反推根成员。

每例在REINIT_RETURN快照做以下三个**独立只读观察**，不插入真实控制链、不继续free：

| 输入 | 对齐键 | 预期返回 | 候选vaddr比较／推进 |
|---|---|---|---|
| 0x54000080（旧动态） | 0x54000000 | NULL | 16比较，16推进至anchor |
| 0x51400080（旧P9） | 0x51400000 | NULL | 16比较，16推进至anchor |
| 0x61400080（新P9） | 0x61400000 | P9 | P15…P9共7比较，6推进后break |

search自身不读取owner；返回P9的owner=S9是快照字段。不读取P9.next或更后候选来完成成功探针。六探针必须标observation_only叶节点，与A/C实际执行边分开。旧对象继续存活与索引查不到并存，不证明真实堆空间遗失、释放或异常。

## C · 从同一后态一次malloc，止于311调用边界

每组从自己的REINIT_RETURN直接调用malloc(0x4000)，没有中间写入；观察探针只从快照派生。initiated=true不再init；size门通过，16档链9次推进/10次比较选择C9。used空/free1，282从free摘S9，286插used，free.count1→0、used0→1，S9.free仍128，CLASS_ROOT此时假。

| 场景 | scan | 296及对象更新 | 311前ret_val（仅局部） |
|---|---|---|---|
| REINIT_SPARSE | idx1；源码bm_unit求值2次，仅word0 | 真；offset0x4000，S.free127、CF127/CU1，map(1,0)→(3,0) | 0x61404000 |
| REINIT_FULL | idx128；源码求值128次，word0/1 | 假；offset/ret赋值、三计数、set都不执行，free128、CF128/CU0、图全1保持 | 初值NULL |

两例308均真，因free实际空；310临时count0→1、实长仍0，LENGTH_ROOT变假。**停于311 uslab_alloc(uslp)的调用边界，实参为选中的S9；callee未展开、new_uslp无可消费返回值，312–321及327未达，malloc正常外返UNKNOWN。** 不能把SPARSE局部非NULL或FULL局部NULL写成已返回结果；不把参数误写为源码读取normal_base_slab_p。本包无备用R，不套用151 B可绕过refill到327的结果。

截至此边界，S9仍used，full空；D/Qd/Dmap字段保持且脱根，页链及其他15cache/bitmap保持。SPARSE的free与bitmap空位差仍1，FULL仍128。CACHE_ROOT两例均真；SPARSE实际127/1、owner和127/129，FULL实际128/0、owner和128/128，OWNER仍少计128占用。SPARSE CLASS_ROOT在对象更新后恢复真，FULL保持假；OWNER_REGISTRATION均假，D自身bitmap比较真；根及owner合取bitmap比较均假。临时count改变不改变实际成员集合或slab_total1。

共同命名时点为ENTRY、GLOBAL_ROOTS_RESET、REINIT_RETURN、SLAB_SELECTED、SCAN_RETURN、OBJECT_UPDATE_DONE或SKIPPED、REFILL_CALL_BOUNDARY。各时点只评价该时点已完成的赋值，不要求每单字段写之间也有相同谓词。全局重置至循环中途按A的特定表消费，不把REINIT_RETURN的集合状态提前应用。未进入的callee副作用、递归、补充返回和malloc外返均不推导。

## 3. 源输入、强制锚与证据

只读固定time五个allocator相关完整根：myinitramfs/mylib/myglibc/malloc/malloc.c、同目录_malloc.h、myinitramfs/mylib/include/utils.h、myinitramfs/mylib/util/list.c、bitmap.c。读实际上下文后给1–5连续物理行正式窗，不截错真实定义/命令调用：

- malloc.c：本地常量/取整宏、initiated/brk_end/三根及静态数组、各bitmap数组与base_colormaps真实关联；init_uslab174–221完整源范围和所有实际写/调用，特别176–179、183–207、210–218、220。
- upage_alloc111–121与140的复用路径；124/125等仅作为未达动态分支定位，不扩展未发生调用。
- upage_search151–169、PAGE_ROUND_DOWN宏链；成功break与失败推进/返回分别取证。
- malloc249–311的init/size/选档/两链选择/scan/更新/308–311；312–321、327作为未达后继定位，uslab_alloc223–236只明确调用边界及未展开，不推其结果。
- bitmap.c bm_get_freebit_idx24–34、bm_set_bit14–17；本批不执行assigned高位查询，不重开N1547。
- list.c INIT_LIST_S10–14、list_insert_prev17–26、list_insert_next29–38、list_del_init40–45、INIT_LIST_HEADER_S73–77、add_head79–83、remove_head85–96、add_tail98–102，以及utils.h结构/类型/container_of和_malloc.h实际结构。
- 两time canary及直接master反验、公约原句；五份CMake：myinitramfs/CMakeLists.txt、mylib/CMakeLists.txt、myglibc/CMakeLists.txt、myglibc/malloc/CMakeLists.txt、mylib/util/CMakeLists.txt，逐实际指令/实例核来源及构建边界。根未直接列mylib不推真实sh链接；不读仓外musl、不声称真实ABI/标准/DEBUG或编译器已确定。

全部十二根的实际CRLF/LF/NONE身份逐窗记录、原文保留或声明可逆展示分隔；不得只核悄悄归一后的文字。正常源码返回、分析停点、独立观察、GIVEN frame、真实未知分别标清。所有强制锚均为正式源码证据，不用旧引用数替代本批精确支持。

## 4. 六件、结构与规模

恰新增agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-reinit-152/下六件：

| 文件 | 必须承担的内容 |
|---|---|
| MANIFEST.md | 新接收/规范刷新、G00、当前六件消费索引、标准read_channel/startup_selfcheck_quote/branch_canary_quotes/self_check三个整数、真实模型与未证 |
| malloc-reinit.md | A/B/C两序列、根与owner集合的差别、旧新地址探针、311边界及历史排重 |
| facts-and-boundaries.yaml | 共享对象基态、两个variant、16行init表、有序delta与可唯一重建快照、域明确谓词、探针、调用返回边界、读写集、build_scope/source_index/required_anchor_coverage/graph |
| malloc-reinit-paths.dot | 与facts.graph同一节点/边及全部属性；source_order与controlled_next_top_level_call和observation_only分清 |
| deferred-findings.yaml | 本批实质新增及旧项状态变化，标准source_ref与唯一场景/证据selector；普通问题先登记 |
| evidence.md | 本批所有新增正式窄窗与真实支持/行尾；交付前重开固定源逐窗复核 |

共享前提和参数化表可以引用；有序delta须保留前值、后值和实际源顺序，snapshot_ref可唯一重建。不得每观察点重复整套16cache和bitmap，也不得为省篇幅删掉实际对象门/停止点/未达赋值。图可引用16轮表而不画32套同构链；两个311停点无后继执行边，六观察节点仅观察叶。无行数、证据数或新缺陷数配额。

R151-01本轮由主线补注闭合，D00为空；旧138/137/96及主线11证据账不重贴或混计。P6新正式窗n=m+k，交付前真正重新打开各固定源，强制锚不得降级。连接器可达P9 1–8、11、13实做；9/10无代码补丁、12非降级可NA；完整本地P9未做就NOT_RUN。外部标准独立外证UNKNOWN保持。

## 5. 发布、实际G04与停写

只用原exec／Draft PR17，真实expected_sha、force=false、唯一父07d636217117，净一提交六新增零其他改删。先全件准备、自检、未知并行写排查；提交后固定/活动/API全文、LF/UTF-8字节及单父compare核同。原comment/旧151结果不得改署名、改写或重复发评。

唯一正式作者标记：

MYOS2-LEAD-002-CHAT-EXEC-25 / CORE-USER-MALLOC-REINIT-152 / FORMAL_DELIVERY

正式评论留完整新接收/规范/身份/模型/固定合同/提交/六件/真实自检/结果与限制。发表时G04只能PENDING；发表后实际新ID双读、全分页至空、49原标记＋本作者50各唯一、旧EXEC-24的152与两个历史负标记0、四ref、开放PR/两PR原状态/原题/head/base、六件和151冻结/旧评论核完，才报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、实际G04=PASS、STOP_WRITING。

63暂态异常先核远端效果再有限10/20/40秒或实质工作间隔重试，首次加三次，不重复提交/评论；未知并行写不覆盖，不将工具异常当缺仓权。未发生写重试按实记not_needed。

最终聊天依198，一个通常3–6行原生短复制块、一个真实正式作者URL，交C11 Pro独立验收并同轮后继，不自行开始153；受阻如实给状态和最小缺口。结果先行、人工动作集中、授权内连续推进、普通问题先入库与未来交接十五规则/MA全文/共用全文/命名/8/10要求持续继承。C11发布152后计1，提醒未达；不因新执行对话重置主线计数。

阶段2，完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机均NOT_RUN；不改源码、旧件、主线目录或PR元数据，不新建分支/PR、合并、强推、删除或跨仓写。实际生产入口、C指针/对象、映射、生命周期、真实运行及后台档位未知如实保留。
