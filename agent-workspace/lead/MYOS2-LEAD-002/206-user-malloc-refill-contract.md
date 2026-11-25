---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；后台及effort未独立认证）"
model_per_owner: "Owner明确要求Pro独立验收及同轮后继"
date: 2026-11-25
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: aa4922a76378
execution_pin_short12: d2c99a1f285b
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
record_type: phase2_executor_contract
followup_id: CORE-USER-MALLOC-REFILL-149
status: PUBLISHED_PENDING_EXECUTOR_START
executor_display_name: "MYOS2-A-C10b 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-23
executor_model_per_owner: "GPT-6.2 Sol；Owner界面可用Extra／高推理强度"
actual_149_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-refill-149/
allowed_files: [MANIFEST.md, malloc-refill.md, facts-and-boundaries.yaml, malloc-refill-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
prior148: PASS_PENDING_LOCAL
N1547_author_independent_evidence: UNKNOWN_INHERITED
mainline_design_count: {conversation: C10, first_batch: 141, before_release: 8, after_successful_formal_release: 9, latest_batch: 149, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
self_check_scope: "本合同无新正式源码窗；A4/B3/C3为新任务给定，执行者独立回源"
inputs_read:
  - "agent-workspace/lead/MYOS2-LEAD-002/205-user-free-reclaim-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-FREE-RECLAIM-148-review.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/200-user-brk-allocator-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/201-user-malloc-slot-contract.md"
  - "agent-workspace/lead/MYOS2-LEAD-002/203-user-malloc-colormap-contract.md"
  - "myinitramfs/mylib/myglibc/malloc/malloc.c"
  - "myinitramfs/mylib/myglibc/malloc/_malloc.h"
  - "myinitramfs/mylib/include/utils.h"
  - "myinitramfs/mylib/util/list.c"
  - "myinitramfs/mylib/util/bitmap.c"
open_questions: ["完整补充递归、返回值对应真实对象、布局/编译/链接及运行未证"]
---

# 206号合同 · 第149批 · malloc补充预留、递归申请依赖与对象门

**继续现有MYOS2-A-C10b／EXEC-23，使用次一档GPT-6.2 Sol及Owner可用Extra／高推理强度。** CORE-USER-MALLOC-REFILL-149包含A4申请依赖、B3预留计数下的首次嵌套申请、C3独立调用响应，共三个相关专题、十个有界记录及两项结构布局辅助表，一套六件。D00为空；[148独审](reviews/CORE-USER-FREE-RECLAIM-148-review.md)为PASS_PENDING_LOCAL。

Owner转发真实正式发令即授权连续完成G00、分析、自检、六件提交、唯一作者评论、实际G04及停写。依[198号](198-repository-first-compact-relay.md)，详细内容留GitHub，最终聊天仅一个短复制块、一个正式作者URL。无需重启202交接或另等开工。

## G00 · 当前断点与规范

1. 按真实评论ID双读本轮148独审／149发令，取得同提交206、148独审与[检查点](checkpoints/2026-11-25-reclaim148-reviewed-refill149.md)。148作者[7162962611](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7162962611)双读；固定d2c99a1f285b六件及本轮裁定合读。作者中断原因UNKNOWN，不追认私有时序。
2. 依[202§3](https://github.com/08822407d/MyOS2/blob/7f68407aec9f/agent-workspace/lead/MYOS2-LEAD-002/202-executor-continuation/00-C10a-to-C10b-recovery-package.md)十四份项目规则和79，仅复用本C10b自己完整读过且未变的材料，遗漏或截断先补读；MANIFEST分列实际复用和本轮新读。保留README、公约、协议v2、工作令及18/24/28/63/79/110/124/193/194/198。
3. 刷新Meta-Agent与Mnemosyne当前refs及入口。主线本轮MA master=b234c6c91a2e、建设分支=5fec75f1c996；相对50806a75e050的一提交仅GDAL启动交接，未改变本项目适用规范。沿119597a75517的48采用决定、47有效v1.4-r10.266及45归档同版共用1.1-r9.260；Mnemosyne=e581eb9df6c4。按202§4只作任务局部接收/命名，不导入跨项目业务或写权。真实模型标识、后台/effort不可核范围和current_task_preserved分别记录。
4. 直接四refs：lead为本次发布提交，exec预期d2c99a1f285b、time=a039d9803ade、master=e8e60cd19540。PR16 Open/Ready、PR17原题Open/Draft，原head/base及未合并保持；不得以PR.base_sha替代直接master。
5. PR17顶层评论无since完整分页至空；本轮检查点所列32枚既有标记加本次148独审／149发布两枚，共34枚各唯一；旧EXEC-22的145作者0。开放PR分页至空仅16/17。148固定/活动/API Blob全文、LF及字节相合，单父12b3784893e5，一提交六新增；前序冻结成果及作者评论保持。
6. 首次写前核149目录与作者标记不存在，获取真实exec expected_sha。自身相符部分发布按63续缺项；未知并行变化保全定位，不覆盖、强推或重复提交/发评。未发生真实阻断就连续推进，普通问题先入deferred，不向Owner逐项问决策。

148六件长hex扫描为零；原作者评论两条URL使用全长提交串的格式偏差，主线已在独审§4记录并提供短12消费入口。旧评论冻结，不重发。**本批所有新文件及正式作者评论的提交/目录/blob链接都使用连接器原样复制的短12码，含链接目标；不是给URL新增P1豁免。**

C10已正式设计141–148共8批；149成功正式发布后为9。12/15提醒未到；改派、恢复、验收、补正不重复计数。147合读§4及146合读§4、combined145通过和全部既有文档关闭状态继承，N1547独立外证UNKNOWN保持；本批无重新取证该PDF的前置门。

## 增量与共同给定

144已有brk返回、两组16轮init及upage局部消费；145已有尺寸/槽选择并止于补充接口；146已有16档容量、动态请求及独立成功写入/发布；147/148已有free与回收边界。本批新增的是：申请尺寸之间的潜在依赖；outer临时count预留在首次嵌套malloc中的真实消费；三类调用响应需要的对象前提。不重做上述完整模型，不证明首次malloc或完整递归终止。

仍只读固定time的仓内myglibc候选实现。actual_linked_to_sh、实际标准/编译器/DEBUG/ABI/映射/运行UNKNOWN；不读取仓外musl填补语义。用户H=0x200000仅为malloc.c的PAGE_SIZE，不混成内核页尺寸。

给定8位byte；指针、size_t、unsigned long、unsigned long long及bitmap_t均8字节，BITMAP_UNITSIZE=64。A的结构布局另明确给定自然8字节对齐、按声明顺序、成员按其对齐放置、末尾按最大对齐补齐，无pack/额外布局属性；List_s为两指针。**布局是GIVEN算术模型，实际ABI未测。**

计数/算术可表示、无回绕、无并发；涉及的有效cache、列表节点、bitmap对象与合法转换按各例明确给定。B的预留count与物理空链不一致是被分析的瞬态，不以“列表有效”偷偷修正count。C显式NULL及B_NO_USED的缺失slab对象前提例外，不能套用所有对象都合法。地址仅作整数投影，不保证返回地址处存在可写、已建立类型的真实对象。

A/B/C分别独立，不用数值相同拼成完整运行。记录entry_kind、given_inputs、source_order、read/write_set、calls、conditions、counter/membership、endpoint及unknowns；区分源码分支、GIVEN调用响应、纯依赖边、缺少对象前提的分析停点。

## A · 元数据布局与潜在申请依赖（4记录）

先按实际结构成员和上述GIVEN列两项辅助布局：uslab_s各成员offset、大小、对齐与结构总尺寸；uPage_s同样列出。List_s及相关别名须有实际定义证据。预期模型sizeof(uslab_s)=64、sizeof(uPage_s)=32，uslab_list及upage_list偏移均0；执行者独立从字段复算，不作为ABI事实。

给initiated=true及16档合法升序cache链，O=32<<i、N=H/O。只选以下四档，将146相应容量项按固定来源消费；重新核申请表达式和选档，不重新展开16档容量表。

| ID | 外层O | i | N | colormap请求字节 | 该请求所选O |
|---|---:|---:|---:|---:|---:|
| A32 | 32 | 0 | 65536 | 8200 | 16384 |
| A64 | 64 | 1 | 32768 | 4104 | 8192 |
| A8K | 8192 | 8 | 256 | 40 | 64 |
| A16K | 16384 | 9 | 128 | 24 | 32 |

每行分别记录uslab_alloc首次malloc(sizeof(uslab_s))的64字节/64档依赖、colormap malloc的请求与所选档；仅当upage_alloc进入free-page count不大于0的动态分支时，另有malloc(sizeof(uPage_s))的32字节/32档依赖。正常合法count模型下该动态分支对应0；不扩展下溢情形。free-page复用分支不产生该malloc。

图可合并四行的共同调用依赖，但须有逐ID覆盖。标清源码调用位置、分支条件、参数来源、所选cache身份，区别64档自依赖、32/16K及64/8K相互依赖与真实运行递归。潜在有环不推出实际递归发生、无限递归或运行故障；这些依赖需要各自补充门、列表/bitmap和调用对象前提。A止于尺寸/依赖分析，不推导分配结果。

## B · outer预留count对首次嵌套malloc64的影响（3例）

三例独立从outer malloc的补充门判断前进入。outer已经选中64档C，local uslp=U，local ret_val=R=0x61000080（仅GIVEN局部值，尚未函数外返）；initiated=true、合法升序cache链、C.obj_size=64。C.normal_slab_total=1，free链物理为空且count=0，U.total=32768、U.virt_addr=V=0x61000000、U.uslabcache_p=C。U是唯一slab，owner/base关系本段不读；所有实际读写的元数据、链及512个bitmap word有效，除明确关系外互不别名。

| ID | used / full | U.free | bitmap word0 | word1…511 | C对象free/used |
|---|---|---:|---|---|---|
| B_MORE | [U] / [] | 2 | 0xfffffffffffffffc | 全1 | 2 / 32766 |
| B_LAST | [U] / [] | 1 | 0xfffffffffffffffe | 全1 | 1 / 32767 |
| B_NO_USED | [] / [U] | 0 | 全1 | 全1 | 0 / 32768 |

used/full counts起初与上表成员一致；free anchor合法self-loop。这些是新的补充前入口，不要求从首次初始化或145模型推得。bitmap采用word区间表达，不复制512行。

先记录outer门count<1为真、310的count0→1、物理free仍空，然后进入uslab_alloc(U)首次malloc64。此时outer停在调用中，312及以后未到达。嵌套malloc给initiated=true，不执行init，尺寸选择仍C。独立重放列表选择、bitmap和补充门，而非把给定返回跳过它们。

### B_MORE与B_LAST

两例used优先，分别核摘首、按摘取前U.free判断回used或入full的时点；扫描idx=0、offset=0、结果投影V；记录三对象计数、首word、所有列表count/实长及normal_slab_total。其余511word应按实际访问/写集合记录，不作机器访存次数认证。

嵌套malloc的补充门读到的free.count仍为预留1，故条件假；明确它并不代表存在备用节点。两例止于首次嵌套malloc把地址投影V交回uslab_alloc之后、INIT_LIST_S(&nslp->uslab_list)之前。不要跨到新metadata对象解引用：B未给V处有效uslab对象前提。outer R尚未返回，outer count回减/新slab发布均未到达。

### B_NO_USED

used为空而free.count为预留1，实际进入free选择分支。必须完整读list_header_remove_head：它按count选分支，取anchor.next；物理空环时该值是anchor自身，不是NULL。重放list_del_init的self-loop写及count1→0，核返回节点身份、环形状及malloc中等待NULL的条件；不能把此例写成remove_head返回NULL或源码显式停转。

仅作container_of偏移0的地址投影，不因此造出合法uslab对象。**最迟在对投影uslp执行后续成员/列表消费之前停止分析；不执行286的插入模型。** 给定对象只有free header/anchor，没有一个以该anchor为uslab_list的活uslab；所需类型、对象及转换前提缺失。图为ANALYSIS_STOP_MISSING_SLAB_OBJECT_PREMISE，不是源码if/return，也不是实际崩溃证明。bitmap、对象计数更新及函数返回不得从此点继续外推。

每例保留outer和nested两层局部变量、共享C身份及各自调用/返回状态；不把nested返回当outer完成。B_NO_USED的free.count已回0来自remove_head，不能记成outer312回减。

## C · uslab_alloc三类调用响应的独立对象门（3例）

C_META_NULL、C_MAP_NULL、C_PAGE_NULL都是独立GIVEN调用响应，不由B或真实allocator可达性推得。给活且合法cslp，相关两次total读取均128。每个注明有效的返回对象均足够大、活且类型/成员访问合法，D、cslp、M、P及所跟踪字段不发生未说明别名；对应调用前后保持这些对象条件。**对每一次未展开调用分别给定：从该调用前到其正常返回，对调用前已确定的所跟踪字段无其他净效应；指定返回值除外，本层随后显式写入另计。** 该前提不能仅由“调用返回”推出。未给定的全局/递归副作用仍UNKNOWN。

| ID | 首次metadata malloc | bitmap malloc | upage_alloc | 观察终点 |
|---|---|---|---|---|
| C_META_NULL | 返回NULL | 未到达 | 未到达 | 首个metadata成员初始化所需对象前提缺失 |
| C_MAP_NULL | 返回合法D | 返回NULL | 返回合法P，P.vaddr=W=0x70000000 | uslab_alloc条件性返回D |
| C_PAGE_NULL | 返回合法D | 返回合法M | 返回NULL | upage_p赋NULL后，读取其vaddr所需对象前提缺失 |

C_META_NULL不添源码NULL检查，不把缺少对象前提写成真实return NULL或已发生异常。C_PAGE_NULL分别记录向合法D.upage_p写NULL与随后需要page对象的读取；不能将前一存储也误判为对NULL对象的写。

C_MAP_NULL中存储colormap=NULL本身是合法指针字段赋值；在本层给定后续条件下可继续free/total赋128、upage_p=P、virt_addr=W、P.uslab_p=D并返回D。必须区分“本函数没有直接解引用bitmap”与“后续可以安全使用bitmap”；后者不成立为已证结论。本函数对D.uslabcache_p的直接写集合为无，其实际值UNKNOWN，不把未写解释成确定的未初始化字节或NULL。D链self-loop的保持须连同本节frame条件消费。

回看outer313对new_uslp的NULL检查只发生在uslab_alloc返回后，不能据此声称保护了callee更早的metadata/page访问。只引用这个源码时序，不虚构uslab_alloc在合法生产路径自然返回NULL来绕过内部对象门。C不展开caller发布增量，不重做146 C_PUBLISH；不推导真实空指针故障、恢复路径或补丁。

## 固定来源、强制证据与范围上限

历史最小消费为148固定六件/本轮独审、147与146/145有效裁定及本合同所需旧边界。146固定42066de2e04f的正文/facts相应四档和主线补注可索引，不复制旧90窗；144/145仅读已述范围排重，不重核旧完整模型。

固定time读malloc.c、_malloc.h、utils.h、util/list.c、util/bitmap.c；五份用户CMake文本及内核options_flags/panic仅核分支/构建边界。新正式锚须覆盖：

- H/CONST_1M、16档/32基数、sizeof(uslab_s)/sizeof(uPage_s)的实际调用位置、bitmap请求表达式及upage分支门。
- 两结构完整所用成员/别名、List/头/bitmap类型与单位、活动container_of；布局数值和对齐条件另列GIVEN。
- malloc的initiated/尺寸/选档、used/free优先、last-free预迁移、NULL等待、scan/条件返回值/计数/set、补充门及临时count、callee调用、312后检查/发布至return。源码循环与分析停点分别表示。
- list_header_remove_head完整体及list_del_init、INIT_LIST_S、list_insert_next、list_header_add_to_head实际助手体，特别是count门与anchor身份。
- bm_get_freebit_idx与bm_set_bit本批实际路径，64位word及1ULL表达式；不重做147 assigned/clear或N1547取证。
- uslab_alloc完整直接写/调用序列、三类响应后的首个所需对象前提、未直接写cache owner/bitmap载荷的有限范围。
- 活动配置、两canary和五份用户构建文本（含malloc/util的GLOB到target_sources），区分候选实现与真实sh链接。

正式窗按协议v2全部P1–P13，1–5连续物理行，路径/符号/性质及支持范围准确，保留CRLF/NONE可逆表示；强制点不能降级或漏掉。否定结论限完整所读函数/直接助手和明确write/call集合。无源码窗数、行数或问题数配额。

不扩展：完整init/补充/回收递归终止性、任意损坏链/坏指针全分类、真实ABI/编译器判定、内核分配对照、映射/生命周期/运行、修源码、恢复或清理实现。所有新普通条件观察按18用唯一qualified_id/source_ref/selector入deferred，继承的链接/载荷/外证未知不重报新缺陷。

## 六件、自检与验收

仅新增user-malloc-refill-149下MANIFEST.md、malloc-refill.md、facts-and-boundaries.yaml、malloc-refill-paths.dot、deferred-findings.yaml、evidence.md。

facts至少有given_model、layout_models、allocation_dependencies、nested_reservation_cases、call_result_boundaries、read_write_sets、object_access_boundaries、build_scope、source_index、required_anchor_coverage、graph及graph_coverage。两布局表为辅助，不虚增成完整运行场景；A4/B3/C3十记录可定位。用参数化/区间表达减少同形重复，正文、机器件和图逐字段相合。

DOT/facts.graph核节点id/kind/label/scenario、边from/to/kind/label全部字段。潜在dependency边、B实际有界调用/返回、C独立GIVEN响应、analysis_stop分开；A图有潜在环不等于B/C执行环。B_NO_USED没有NULL等待停转、没有成功投影对象后续边；C_MAP_NULL可以在追加对象/frame前提下返回，另外两C不画真实异常分支。

MANIFEST真实标准根含read_channel/base_snapshot/inputs_read/startup_selfcheck_quote/branch_canary_quotes及self_check三个整数，n=m+k。交付前重开所有正式固定来源逐窗核对，149新P6与旧证据消费分账。真实接收/规范复用刷新和六件用途/覆盖/后继消费齐全；UNKNOWN时点不追签。实际可达P9的1–8、11、13逐项核，9/10/12不适用时NA，完整本地P9仍NOT_RUN。

## 提交、回执、实际G04与停写

原exec以真实expected_sha、force=false单父快进，净差异恰六新增无其他改删。提交后核固定/活动/API Blob全文、LF/UTF-8字节、唯一父和净差异；发评前PR17无since分页至空查重。唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-23 / CORE-USER-MALLOC-REFILL-149 / FORMAL_DELIVERY

正式GitHub作者评论给固定合同/提交/六件、三专题及GIVEN/对象边界、真实自检、模型可核范围、未证与发表后G04待做状态；所有提交链接用短12。评论发表后按真实ID双读、PR17全分页至空核34历史/主线标记及本批作者唯一、旧EXEC-22的145作者0；开放PR至空仅16/17及原状态/head/base/PR17原题；四refs；六件固定/活动/API Blob及单父净差异；148冻结六件与原作者保持。实际完成才报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、G04=PASS及STOP_WRITING；不自行启动150。

暂态故障按63先核远端效果后有限间隔重试，通常首次加三次；自身相符部分只续缺项，不用重复提交/评论补救未知结果，不因单次故障判无权限。未知并行变化不覆盖；无故障记not_needed。

最终聊天按198只给一个通常3–6行原生复制块：149真实交付/主线待审/实际G04/停写、一条正式作者URL，请C10 Pro独审并同轮后继。详细统计/规则/技术摘要均留GitHub；受阻才补必要缺口。阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作NOT_RUN。源码、旧成果/评论、主线目录及PR元数据无执行方改权，不新建分支/PR、合并、强推或删分支。结果先行、人工动作集中、授权内连续推进、次一档执行/Pro独审、同轮后继及63/193/194/198在未来交接继续传递。
