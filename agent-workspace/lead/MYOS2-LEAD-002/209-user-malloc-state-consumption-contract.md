---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；实际后台及effort未独立认证）"
model_per_owner: "Pro独立验收与同轮后继设计"
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: dc12f97a7a91
execution_pin_short12: cca59f60e032
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
record_type: phase2_executor_contract
followup_id: CORE-USER-MALLOC-STATE-CONSUMPTION-151
status: EXECUTION_PENDING
executor_display_name: "MYOS2-A-C10c 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-24
executor_model_per_owner: "GPT-6.2 Sol；Extra／高推理强度"
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-state-consumption-151/
allowed_files: [MANIFEST.md, malloc-state-consumption.md, facts-and-boundaries.yaml, malloc-state-consumption-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
prior150: PASS_PENDING_LOCAL
prior149: PASS_PENDING_LOCAL_WITH_150_D00_AND_MAINLINE_REVIEW150_SECTION3
R149_01: CLOSED
D150_O04: CLOSED_BY_MAINLINE_ADDENDUM
R149_02: CLOSED_BY_MAINLINE_ADDENDUM
N1547_author_independent_evidence: UNKNOWN_INHERITED
mainline_design_count: {conversation: C10, first_batch: 141, latest_batch: 151, before_release: 10, after_successful_formal_release: 11, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "设计给定与取证要求，不冒充执行成果；新作者须独立回源复算"
inputs_read:
  - "150固定六件、208、同轮150独审及149合读补注"
  - "145/146/147/148/149指定已审范围及固定time的五份allocator源码、配置/canary与五份CMake"
  - "14项项目规则、Meta-Agent有效采用及Mnemosyne命名的当前入口"
open_questions: ["151尚未执行；给定状态生产可达性、实际语言对象/指针运算、ABI/链接/映射/完整递归和运行未证"]
---

# 209号合同 · 第151批 · 分配与释放的跨调用状态、满位图后继及地址识别

交现有「MYOS2-A-C10c 代码分析执行」／EXEC-24，继续GPT-6.2 Sol＋Owner可用Extra／高推理。Owner转发本轮真实主线正式评论即授权连续完成G00、A/B/C、六件、自检、唯一作者回执、实际G04及停写，不另等开工，不自行启动152。

[150独审](reviews/CORE-USER-UPAGE-LIFECYCLE-150-review.md)已判150 PASS_PENDING_LOCAL；R149-01四项关闭。D150-O04的E149-090和本轮主线另发现R149-02的095/096，由该审§3准确新窗/消费映射闭合，149须合读150 D00与该补注；旧RETURN、旧原件/评论及96/137计数保留。**本批D00=[]，不再做这些旧文证补正。** GitHub存正文，双向聊天遵守[198](198-repository-first-compact-relay.md)。

## G00 · 现有C10c续接与实际预检

1. 按Owner入口真实ID双读本轮正式评论，取同提交209、[150独审](reviews/CORE-USER-UPAGE-LIFECYCLE-150-review.md)与[检查点](checkpoints/2026-11-26-upage150-reviewed-state151.md)。本次三新主线标记分别为150 INDEPENDENT_REVIEW、150 D00_149_CLOSED和151 TASK_RELEASE；评论实际ID由入口取得，不猜号。
2. 同一有效C10c只能按79复用自己已经完整读过且版本未变的规范；缺失/截断/变更项补全，不继承其他对话已读。[207§3十四项清单](https://github.com/08822407d/MyOS2/blob/95d47a823b80/agent-workspace/lead/MYOS2-LEAD-002/207-executor-continuation/00-C10b-to-C10c-recovery-package.md#3-新方独立全文读取的十四份项目规则)含工作令、README、公约、协议v2、18/24/28/63/79/110/124/193/194/198；工作令固定95d47a823b80，余13项1174c16c4540未变。207仅作规范/接收来源，不重新执行其中149业务或旧目录不存在门。
3. 刷新Meta-Agent和Mnemosyne当前refs/入口。准备时MA master=b234c6c91a2e、建设分支meta-agent-fable5-review-and-design-001-batch-03=5fec75f1c996；48采用/47有效v1.4-r10.266及45归档十五条1.1-r9.260全文仍固定119597a75517，入口Blob881fd8c1076e、检查点70a5bda0be6e。§316候选/318待反馈/319暂缓/320 GDAL启动没有新的本项目采用。Mne master=e581eb9df6c4，README04e450fc22f7、命名§7 Blob2d930f07381e未变。按194与207§4辨明真实后续采用，未变全文只复用本会话自己的实际已读；不得以摘要代替缺失全文，不导入外仓任务/写权。
4. MANIFEST记guidance_refresh新读/复用/缺失、current_task_preserved=true、task_local_only=true；当前任务151，身份C10c/EXEC-24。Owner指定Sol/Extra、系统可核标识和不可认证后台/effort分开，不自动切换模型，不将名称/版本当实际后端认证。
5. 直接核四refs：lead取本合同同次正式发令给出的真实固定提交；exec=cca59f60e032、time=a039d9803ade、master=e8e60cd19540。开放PR无since分页至空仅16/17，原PR16 Open/Ready、PR17原题Open/Draft，原head/base及未合并保持；PR.base_sha不是直接master。
6. PR17顶层评论无since完整分页至空；检查点40枚旧标记加本次3枚共43枚各唯一。150作者#7179601188与150发令#7167992035真实ID双读，149作者#7167216690保持。旧EXEC-23的149作者、旧EXEC-22的145作者及本批作者为0。所有评论和旧原件冻结。
7. 冻结150六件：固定cca59f60e032/活动exec/API Blob全文、LF、UTF-8字节与真实Git Blob相合，9419 LF/323103字节；唯一父9aa7de497eb0、ahead1/behind0、一提交六新增。核149的合读规则，不重跑旧137/96窗或旧模型。
8. 第151批目录及作者标记首次写前不存在，读取真实expected_sha。自身已有且相符的部分交付按63只续缺项；未知并行写保全定位，不覆盖或重复提交。无真实阻断后连续完成本合同；普通问题按18入库，不要求Owner技术选择。C10正式发布151后11批，12/15未到。

## 增量、共同边界及比较谓词

145已有malloc选档/迁链与helper扫描边界，146有位图/动态slab载荷和caller发布，147有free低位与单次最后占用槽，148有回收，149/150有依赖/对象门及页描述符复用。本批把既有局部操作组成明确有限的跨调用序列，另核“满bitmap但正free字段”的后继和地址识别的首匹配/槽商；不重做旧完整模型集，也不把重复已知差异报成新源码缺陷。

固定time仓内myglibc候选实现。H=0x200000仅malloc.c的PAGE_SIZE，不混成内核页大小。GIVEN 8位byte，int/unsigned32；pointer/size_t/unsigned long/unsigned long long/bitmap_t64，BITMAP_UNITSIZE=64，沿既有自然8对齐模型。实际ABI/标准/DEBUG/编译器、sh实际链接均UNKNOWN，仓外musl不读。

所有本例读取和写入的metadata、bitmap、header、循环链及container_of转换有足够大、存活且合法的对象；未说明的metadata/bitmap/header别名不存在。initiated=true使malloc初始化门假、若包含DEBUG则free等待门假。无并发、无回绕；顶层调用之间不插入其他操作，各函数只按明确源路径和实际助手推导，不GIVEN一个不透明的free/uslab_alloc正常返回。

地址、obj_p−virt_addr及商/余数明确按给定字节差的整数投影记录。此数值模型不证明void*算术的实际C实现、语言对象同一性/生命周期或payload映射。整个包不读写payload；内部偏移和重复键均为独立GIVEN，不从144拒绝brk或真实递归推导生产可达，不作外部标准裁定。free的assigned仅触及bits0/1/3，N1547外证UNKNOWN继续。

分别记录以下对照谓词，不用一个“状态一致”代替：
- V_BITMAP：S.free是否等于N减去bitmap前N位的置位数，尾容量不混算。
- V_CACHE：C的CF/CU是否等于该C全部给定slab的free与N−free之和；CF=nsobj_free_count，CU=nsobj_used_count。
- V_LENGTH：各header.count是否等于其物理成员数。
- V_CLASS：free链成员free=N、used链成员0<free<N、full链成员free=0。这是合同定义的比较谓词，不冒称源码保证的入口不变量，也不按注释改写实现。

A入口四项都成立；B明确让V_BITMAP不成立，其他入口关系按给定独立核；C两cache内部四项成立，重复vaddr只作另一个地址键关系。谓词评价是分析观察，不能画成源码校验/回滚分支。

## A · 两条跨调用低位槽序列

两例独立，给合法16档升序cache链：uslab_cache_groups[i].obj_size=32×2^i，i=0…15，实际链/转换可用。所有malloc请求size=0x4000，选C=第9索引档，O=16384、N=128。只需核本请求实际选择路径，不重列16档分析表。

C恰有S/R两个slab，normal_slab_total=2、base=S；S.virt_addr=V=0x60000000，R.virt_addr=0x60400000，各有合法对应page descriptor P_S/P_R，used_upage=[P_S,P_R]/count2、free_upage=[]/count0，vaddr/owner/反向关系对应且全段不被页操作修改。S/R.total=128；R始终为备用free slab，R.free=128，两个bitmap word均0。对象和bitmap彼此互异，除正常链指针连接外无未说明别名。

每次malloc到308时free链保留R使count=1，补充门假；每次free到381时free.count=1，回收门假。须由实际前后态核此条件，不能先假定helper无副作用。整个A无uslab_alloc/uslab_free/upage_alloc/upage_free递归；R字段保持范围与其链节点可能被助手写入的范围分开列明，不泛称“R完全无写”。

### A_LAST_SLOT_CYCLE

入口used=[S]/1、free=[R]/1、full=[]/0；S.free=1；bitmap word0=0xfffffffffffffff7（仅bit3空），word1=0xffffffffffffffff；CF=129、CU=127。顺序固定为p1=malloc(0x4000)→free(p1)→p2=malloc(0x4000)，无插入操作。每次后态必须直接传给下一次，不重新给任意初态。

须独立复算三次调用边界及必要阶段：第一次malloc在scan前以旧S.free==1把S迁full，扫描idx3、offset0xc000，p1整数投影0x6000c000；更新后S.free=0、两word全1、CF/CU=128/128、used空/free[R]/full[S]。free以assigned低位mask8接受、清bit3，更新后S.free=1/CF129/CU127，再以新free==1从full迁used。第二次malloc再取idx3，数值返回与p1相同，最终回到第一次malloc结束状态。

分别核“malloc的旧free判定时点”与“free的更新后free判定时点”；地址数值相同不等于实际语言对象同一或生命周期证明。这里只扫描到idx3，不扩成所有128槽的表。

### A_LAST_OCCUPIED_REUSE

入口used=[S]/1、free=[R]/1、full=[]/0；S.free=127，bitmap=(1,0)，CF=255、CU=1。顺序固定free(V)→p=malloc(0x4000)。

free清bit0后S.free=128、bitmap=(0,0)、CF/CU=256/0；源码更新后else-if为free==0，故S仍used。此点V_BITMAP/V_CACHE/V_LENGTH保持，V_CLASS不成立。随后malloc仍优先used选S，非R；旧free==1假，先摘首再加回used，scan得idx0并返回V，S.free=127、bitmap=(1,0)、CF/CU=255/1，V_CLASS恢复。须从源码完整核到返回，不能把恢复直接GIVEN。

增量是该有限后继如何消费最后槽状态；147已有的单次分类差异只引用，不重复申报新缺陷，也不由本例恢复推出所有分类差异无害。两条序列共5次顶层调用，不计为5个独立模型。

## B · 满bitmap与正free字段的两个明确不一致入口

两例重新给独立入口，沿A的合法档表、O/N/size/V/R和page/owner关系；S两个bitmap word全1（128位均占用），备用R仍两个word0/free128。此时S.free与bitmap不相合是显式GIVEN，未说明来源及生产可达，不意味着读取对象不合法。

| ID | 入口used / free / full | S.free | CF / CU | 必核scan前迁移 |
|---|---|---:|---|---|
| B_USED_ONE_FULLMAP | [S]/1、[R]/1、[]/0 | 1 | 129 / 127 | 摘S并按旧free==1迁full。 |
| B_FREE_FULLMAP | []/0、[S,R]/2、[]/0 | 128 | 256 / 0 | free摘S、头插used；free余[R]/1。 |

两例分别调用一次malloc(0x4000)。实际helper scan均返回limit=128；循环内bm_unit源码求值128次，只涉及word0/1，不称为128次实测访存。obj_idx<total假，ret_val仍初始NULL；门内offset/对象地址、三计数及set均未执行。S.free、CF/CU、bitmap保持，而链迁移已经发生；无源码回滚，不还原入口链态。

因R仍在free且count1，308补充门假，最后327条件返回NULL。须明确所读路径没有再次选择R，虽其bitmap全0；不要把“扫描耗尽”“局部ret_val为NULL”“最终函数返回NULL”混成同一步。291/316的显式无限循环本两例未进入。终态分别used空/full[S]与used[S]/full空；逐项评价四谓词，只核这一次调用，不继续B的第二次malloc，不编造恢复实现。

## C · 对齐键首匹配与槽内偏移（3个独立入口）

V=0x62000000。两套metadata/bitmap/header互异不重叠，同V只给地址字段关系，不假设两个不相交的实际payload映射各自拥有V。

S32/C32/Q32：Q32.vaddr=V、Q32.uslab_p=S32，S32.virt_addr=V、upage_p=Q32、uslabcache_p=C32；C32.obj_size=O32=32、S32.total=N32=65536、S32.free=65534；bitmap word0=0x88(bits3/7)，其余1023word0。C32恰有S32，used[S32]/1，free/full空/count0，slab_total1、base=S32，CF/CU=65534/2。

T64/C64/Q64：Q64.vaddr=V、owner=T64，T64.virt_addr=V、upage_p=Q64、cacheowner=C64；C64.obj_size=O64=64、T64.total=N64=32768、T64.free=32766；bitmap word0=0x82(bits1/7)，其余511word0。C64恰有T64，used[T64]/1，free/full空/count0，slab_total1、base=T64，CF/CU=32766/2。

三个入口各自重置到对应初态；单键例只需要第一套，双键例给两套。每例仅调用一次free；未选owner/cache不由函数路径改写，page队列不迁移，free页链物理空/count0。cache.free.count0使381门假；无需递归或opaque调用frame。

| ID | used_upage顺序/count | obj_p整数投影 | 须独立复算的选择与商/余数 |
|---|---|---|---|
| C_INTERIOR_SINGLE | [Q32]/1 | V+0x61 | 对齐键V，首Q32；97/32=3余1，assigned=8。 |
| C_DUP32_FIRST | [Q32,Q64]/2 | V+0x60 | 首Q32；96/32=3余0，assigned=8。 |
| C_DUP64_FIRST | [Q64,Q32]/2 | V+0x60 | 首Q64；96/64=1余32，assigned=2。 |

每例核PAGE_ROUND_DOWN宏链、used首节点、相等即break和实际返回描述符，再分别读取slab/cache归属；不能直接从obj_p值推所属cache。双键两例只读首候选就break，不继续用第二节点作候选；这是两个独立队列初态，不是源程序主动交换顺序。

前两例clear0x88→0x80，S32/C32.free=65535、C32.used=1；第三例clear0x82→0x80，T64/C64.free=32767、C64.used=1，另一套保持。需核更新后迁链门及回收门均假、正常到free尾部；不只停在整数商。此路径没有另验“字节差对O的余数必须0”，不得补出源码不存在的对齐检查。

内部偏移例只说明给定字节差/有效metadata前提下消费商对应位；不宣称真实用户非法free事件、标准UB裁定或修复。重复键只说明给定链序决定首匹配归属，不能从相同数值地址推出真实双映射、双重分配或生产到达。

## 输入、范围上限与强制新证据

历史仅读必要对应段：
- 150六件的D00、模型/对象/实际链接边界及本轮独审；149只消费合读更正规则，不重做旧96窗。
- 主线[145独审](reviews/CORE-USER-MALLOC-SLOT-145-review.md)、[146独审](reviews/CORE-USER-MALLOC-COLORMAP-146-review.md)、[147独审](reviews/CORE-USER-FREE-SLOT-147-review.md)、[148独审](reviews/CORE-USER-FREE-RECLAIM-148-review.md)中选档/扫描、载荷、低位free/最后槽、回收门及有效合读注记；不重开旧关闭项。
- 原201/203/204/205合同仅需定位上述已审范围；不得批量重做16档/16轮、旧模型全集或完整递归。

固定time：malloc.c完整malloc/free/upage_search、所需常量/global；_malloc.h的page/slab/cache/档常量；utils.h的实际类型、container_of与bitmap宏；list.c的初始化/删除/摘首/插头及实际直接助手；bitmap.c四函数完整。阅读init_uslab/upage/uslab仅为实际选择/避免递归扩展所需的身份边界，不开启新全递归专题。

重新核mykernel/scripts/options_flags.cmake和panic.c、直接master双canary/公约反验。五份CMake仍为myinitramfs根、mylib、myglibc、malloc、util；准确区分命令调用实例。允许明示复用本会话自己已读且固定未变的构建背景，所有本批新增正式窗仍须交付前重新打开源逐窗核对；不得把E149旧无效标签作为支持，也不复制旧窗凑P6。

强制正式窄窗覆盖：malloc初始化/大小门与实际选档、used优先/旧free预迁链、free备选分支、scan/limit返回、<total门及三计数/set、308与327；free DEBUG/NULL、search/两个owner读取、351商、assigned/clear/计数、更新后迁链和381；search取整/首节点/匹配/break/返回；实际List及helper写、结构/字段/基数、四bitmap函数字面量/返回类型；配置/canary和本批使用的构建命令归属。每窗1–5连续物理行，malloc.c原CRLF/末行NONE及其他真实行尾可逆保存；路径/符号/调用范围/支持断言精确。

A2/B2/C3共7独立入口，其中A两条有限控制序列共5顶层调用。三专题共用六件，无模型/字数/引文/新缺陷配额。只覆盖给定两word或用区间表示其余word，不展开65536/32768位大表。不扩展损坏状态全分类、真实错误来源、完整heap/allocator递归、外部标准、任意高位assigned、内核VMA/映射、payload访问、修复或运行。

## 六件、自检与验收

本任务适用协议v2全部P1–P13。唯一六件见头部，不加第七件。facts至少有D00=[]、given_models、state_predicates、allocation_free_sequences、bitmap_mismatch_cases、address_identity_cases、read_write_sets、call_return_boundaries、build_scope、source_index、required_anchor_coverage、graph/graph_coverage。

正文/facts/图字段一致；A顶层调用之间的控制组合、源码边、观察谓词及B/C独立入口分开。DOT/facts.graph节点id/kind/label/scenario、边from/to/kind/label全部一致，无悬空；不能画入回滚、验证谓词、重选R或主动交换页链的虚构源分支。V_CACHE/V_BITMAP/V_LENGTH/V_CLASS逐时点评价，不能以仅有合计代替链成员与bitmap内容。

MANIFEST标准根包括read_channel/base_snapshot/inputs_read/startup_selfcheck_quote/branch_canary_quotes、self_check三整数n=m+k、六件用途/覆盖/后继消费索引、规范新读/复用与可核模型/未知后台。交付前重开本批每一正式固定源逐窗核字节、实际符号/实例范围及断言支持；新P6与旧137/96、主线11窗分账。

P9实际可达1–8、11、13逐项核；9/10无代码/补丁、12非降级时NA。全长40位串（包括链接目标）零命中；每窗路径/范围/符号/原字节与强制锚核实，禁止只核字符串命中。完整本地P9保持NOT_RUN。新普通观察按18分类记录，不升级实测故障，不重复把147既有差异记为本批新缺陷。

## 提交、实际G04与短回传

原exec取得真实expected_sha、force=false、唯一父快进，净差异恰六新增；不修改旧文件/评论、lead或PR元数据，不建分支/PR、合并、强推或删除。写后核固定/活动/API Blob全文、LF/UTF-8字节、真实Git Blob、唯一父/compare六新增；发评前无since全分页查重。

本批唯一正式作者标记：

MYOS2-LEAD-002-CHAT-EXEC-24 / CORE-USER-MALLOC-STATE-CONSUMPTION-151 / FORMAL_DELIVERY

完整GitHub作者回执包含真实身份、固定209/主线独审入口、提交/六件、A2/B2/C3关键前提与未证范围、D00为空、实际自检/模型限制及发表后G04尚待做状态。短SHA均复制连接器实际输出前12位，所有URL目标也用短12。

发表评论后按真实ID双读、PR17无since全分页至空，43枚历史/主线标记各唯一、本批作者恰1、两项历史负标记仍0；直接四refs、开放PR至空仅16/17及原状态/head/base/未合并/PR17原题；151六件固定/活动/API Blob、唯一父及六新增；150冻结六件和原作者保持，149有效合读限定不改。全部实际完成才报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、G04=PASS、STOP_WRITING；不自行开始152。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次。保持expected-head/force=false；未知并行写入不覆盖。记录实际重试和回读结果，不能把工具缺失推断成没有仓库权限。无故障记not_needed；未知效果不以重复提交/评论补救。

最终聊天按198只给一个通常3–6行原生standard writing block、唯一五位ID：151实际交付/主线待审/实际G04/停写，一个真实正式作者URL，请C10 Pro独审并同轮后继。完整统计、证据、规则及技术摘要留GitHub；必要阻断才加最小人工动作，块外不留需拼接条件。

持续继承结果先行、人工动作集中、问题先入库、已有授权内连续推进、约三关联专题/六件、次一档执行/Pro独审、同轮后继、63/193/194/198及未来交接必传。阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作NOT_RUN。

