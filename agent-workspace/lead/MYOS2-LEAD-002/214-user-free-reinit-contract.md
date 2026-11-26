---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
conversation_display_name: "MYOS2-A-C11 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（本轮系统身份）"
model_per_owner: "Pro独立验收并同轮安排后继"
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: 2026-11-26
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 7ed4e9279b7d
execution_pin_short12: 6994f6b1aaec
read_channel: "GitHub connector；自制文本/整数/符号链核验"
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "仅本文件新增正式源码引文；本轮对作者160窗的独立复核另账。"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
mainline_design_count: {conversation: C11, before_release: 1, after_successful_153_release: 2, thresholds: [8, 10], reached: false}
previous_mainline_design_count: {conversation: C10, distinct_batches: 11}
evidence_class: "SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN"
record_type: phase2_executor_contract
followup_id: CORE-USER-FREE-REINIT-153
executor_display_name: "MYOS2-A-C11a 代码分析执行"
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-25
executor_model_per_owner: "次一档GPT-6.2 Sol＋Extra／高推理"
executor_start_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-free-reinit-153/
allowed_files: [MANIFEST.md, free-reinit.md, facts-and-boundaries.yaml, free-reinit-paths.dot, deferred-findings.yaml, evidence.md]
D00: [R152-01]
status: PREPARED_FOR_PUBLICATION_G04_PENDING
open_questions: ["152接收记账的旧历史未证项不得倒签；真实C/运行/模型档位未知保持。"]
---

# 214号合同 · 第153批 · 再次初始化后的free消费边界

**继续原「MYOS2-A-C11a 代码分析执行」／EXEC-25，使用GPT-6.2 Sol＋Extra／高推理，接收和执行无需Pro。** 按本合同完成D00及A/B/C、六件、自检、唯一作者回执、实际G04后STOP_WRITING，交C11 Pro独立验收并同轮后继，不自行进入154，不另等开工许可。

[152主线独审](reviews/CORE-USER-MALLOC-REINIT-152-review.md)裁定技术及160窗PASS_PENDING_LOCAL，整批仅R152-01接收记账RETURN_DOCUMENTATION。该项在本批规定的六件内补注，旧152及旧RETURN不改；准确技术模型可以消费。固定[152作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7185898830)、[作者六件](https://github.com/08822407d/MyOS2/tree/6994f6b1aaec/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-malloc-reinit-152)、[213旧合同](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/213-user-malloc-reinit-contract.md)与[212包](https://github.com/08822407d/MyOS2/blob/7ed4e9279b7d/agent-workspace/lead/MYOS2-LEAD-002/212-executor-continuation/00-C10c-to-C11a-recovery-package.md)只作各自限定范围的来源。

## 0. 业务范围与增量

三相关专题：A旧地址失配后的调用者首个对象门；B新地址命中后的空slab拒绝门；C独立复制中间快照后，释放新增低位使残留旧位被空slab门挡住。A2/B2/C1共5独立入口，合计6次顶层free，不再展开初始化/分配/补充递归。

152已经提供两例REINIT_RETURN及SPARSE OBJECT_UPDATE_DONE符号状态。后者只是malloc中间状态；152在311止步且没有可消费的malloc外返。本批C将该状态的完整受跟踪非局部字段**独立复制为一个新的GIVEN世界**，原malloc的控制栈、局部变量（包括ret_val）、未执行308及后继不迁入，不声称真实回调、重入、并发、原malloc完成或在311后继续。A/B也各从独立快照副本开始；它们不是接在152的最终311停点之后。

147已有独立search/低位free/空slab循环，151已有最后占用槽后的分类差异和地址识别。本批只新增它们对明确reinit派生状态的消费及拒绝门先后，不重新申报旧无清零、旧free==0迁链条件或NULL检查缺口为新的源码发现。152遗留条件、未知和原评论保持。

## D00 · R152-01接收记账补注（不得倒签）

152 MANIFEST未把212§4和完整启动词第4条要求的包ID/non_execution_source性质、到达后胜任及容量判断、规范刷新完成状态明确记齐。此项只补接收记账，不重做152技术模型/160窗，不修改旧152六件或旧作者评论，不发第二个152作者回执，也不新增第七件。

在本批MANIFEST内增加独立的152接收记账补注，分别列历史可核事实、旧时未记录/未认证项、本次153开始前实际完成的预检：

- 可公开核出：package_id=MYOS2-A-C10c-TO-C11a-152-20261126；来源为固定7ed4e9279b7d下212恢复包及启动词，entry_comment=7185473439；package_status=non_execution_source_transfer_artifact。源包提供接收材料，业务权限来自Owner/MyOS2工作令和正式合同，不产生跨仓写权。
- 历史接收当时的胜任/容量判断或refresh_completed，只有本会话确有可恢复当时记录才按证据引用；否则分别明确NOT_RECORDED或UNATTESTED，不由既有G00 PASS、后来六件发布或本次检查倒推旧值为true。不得由主线或新当前判断代签旧判断。
- 153正式动笔前独立作本次胜任与容量判断；复查当前身份、任务MYOS2-LEAD-002-CORE-CHECK-01/CORE-USER-FREE-REINIT-153、阶段2、原分支/PR与本批唯一六件写区、权限及下一安全步。记录本次真实规范刷新范围、新读/本人已读未变复用/缺失，实际完成后才写当前refresh_completed。task_local_only/current_task_preserved明确为本次作用域，不补签为旧时值。
- Meta-Agent当前建设head5cac46110877的§321仅收存Alaya意见/待合议，正式v1.4-r10.266及同版共用1.1-r9.260未变。本次读该新增现状并核实际refs；其余按79仅复用C11a本人同一会话已完整读且未变内容。不能把收存反馈当正式新采用。
- 本批deferred列R152-01的补注位置、历史缺口状态和本次实做证据，状态为SUBMITTED_FOR_MAINLINE_REVIEW、PENDING_MAINLINE_CLOSURE；由C11下一次独审判闭。旧RETURN_DOCUMENTATION历史保留，不宣称作者自行关闭。

这项D00不增加源码证据配额，历史152的160/160/0账保持冻结，与153新正式窗另计。只在准确模型上继续本批A/B/C；不得将本轮允许消费写成152整体已无条件通过。

## 1. 固定消费入口与共同GIVEN

源码固定time a039d9803ade；仅候选仓内myglibc，不证明真实sh链接。固定作者152提交6994f6b1aaec的六件与213合同，加本轮152主线独审合读。六件冻结字节应核，语义只消费下列选择器，不重算2×16轮或旧160证据全集：

|模型入口|152 facts选择器|已实核的delta前缀|来源性质|
|---|---|---:|---|
|R_S|variants.REINIT_SPARSE.stages[name=REINIT_RETURN]|1131|已完成初始化的符号后态，独立复制给A/B|
|R_F|variants.REINIT_FULL.stages[name=REINIT_RETURN]|1131|独立复制给B|
|U_S|variants.REINIT_SPARSE.stages[name=OBJECT_UPDATE_DONE]|1149|只复制字段；不同于REFILL_CALL_BOUNDARY的1150，尚无310临时count写|

以上数字由执行方针对实际固定facts.delta_schema、shared_entry、entry_override及stage重新定位，不能只信本表。用前缀逐项before/after重建所消费的状态，可由旧内容引用和明确覆盖构成紧凑新基态；不可把152整份520KB机器件重复铺在每个快照。

H=0x200000是用户分配器本地PAGE_SIZE。GIVEN byte8，int/unsigned32，pointer/size_t/unsigned long/long long/bitmap_t64，自然8对齐，BITMAP_UNITSIZE64；地址和void*差只作明确字节整数投影。真实C对象规则、ABI/标准、DEBUG、编译器、链接、映射、生命周期及生产可达继续UNKNOWN。新模型实际到达并消费的metadata/bitmap/headers和转换具足够大、活、正确类型及访问前提；A在search返回NULL后的page对象前提明确缺失，不能被此通用句偷补。

全部入口initiated=true保持；若DEBUG块存在，while(!initiated)条件假；若预处理排除则该运行项N/A。实际预处理UNKNOWN，不冒称本批已选择DEBUG开关。全部输入非NULL，337门假。无并发、无回绕和未说明别名，不改payload。A/B只执行各自一次free；C的两次free间不插入其他调用/写。

共同对象身份沿213：C_i/S_i/P_i静态对象；D及Qd/Dmap额外活对象。全局cache根[C0…C15]/16，used-page根[P15…P0]/16，free-page根空/count0；P_i.vaddr=S_i.virt_addr=0x60000000+(i+1)H，P_i.owner=S_i，S_i.owner=C_i。C9 O=0x4000、N128、base=S9、slab_total1；S9.colormap=cm_16KB[2]、S9.upage=P9；P9新地址0x61400000。D.owner=C9、D.free0/total128、Dmap两word全1、D.V=Qd.vaddr=0x54000000、D.upage=Qd、Qd.owner=D不变，但均脱离相应根。其残余链字段不必互逆，不由残余指针构造根成员。

|入口|C9 free/used/full成员及count|S9.free / CF / CU|S9两个word|
|---|---|---|---|
|R_S|[S9]/1；[]/0；[]/0|128 / 128 / 0|(1,0)|
|R_F|[S9]/1；[]/0；[]/0|128 / 128 / 0|(全1,全1)|
|U_S|[]/0；[S9]/1；[]/0|127 / 127 / 1|(3,0)|

其它15cache/静态bitmap、D/Qd全部具体字段按固定选择器保持；U_S的C9.free.count是0，不能把152下一步310的1带入。不存在备用R。旧P9地址为0x51400000；旧地址只是旧字段数值，不认证旧payload仍存在或已释放。

新批中无opaque helper返回假设：upage_search、bm_get_assigned_bit、bm_clear_bit均展开实际源路径；没有uslab_alloc/uslab_free/upage_alloc/upage_free/brk调用。未执行的辅助函数不得被GIVEN正常返回。来源152的brk frame属于旧模型前提，不在本批再次调用或新增证明。

## A · 旧地址失配到free的首个对象访问门（2独立入口）

两例分别复制R_S，不顺序调用，不遍历FULL变体作重复实例：

|ID|free输入整数|PAGE_ROUND_DOWN键|search结果|
|---|---|---|---|
|A_OLD_DYNAMIC|0x54000080|0x54000000|NULL；P15…P0共16比较/16推进|
|A_OLD_STATIC|0x51400080|0x51400000|NULL；P15…P0共16比较/16推进|

实际重放free 337→341及search151–169。search以anchor终止，不以count控制；所有已比较P_i.vaddr均为新区间，Qd没有成为候选；search不读取owner。将正常返回NULL赋给局部upgp后，下一句342要读upgp->uslab_p；此时缺少page对象前提，停在ANALYSIS_STOP_MISSING_PAGE_OBJECT_PREMISE。

此停点是推导边界，不是源码NULL分支、正常free返回、显式while循环或已观察崩溃。不要从此推出343/345/351/352等运行结论或任何释放写。局部upgp可记录NULL；uslp/uscgp、obj_idx、assigned返回及后续状态NOT_DERIVED。按截至停点实际读写集合确认metadata/bitmap/链字段零写；不是证明继续运行仍无副作用。

## B · 新地址命中但残留位图不能越过空slab门（2独立入口）

B_NEW_SPARSE复制R_S，B_NEW_FULL复制R_F；各free(0x61400000)。输入正好等于新P9/S9地址只是GIVEN地址关系，不先求或伪造351的obj_idx。

search均键0x61400000，P15…P9共7比较、6推进，匹配P9后break，不再为成功查找读取P9.next或其后候选。342读取P9.owner=S9，343读取S9.owner=C9；345实际读取free=128、total=128，相等为真，进入348的源while(1)。这是SOURCE_LOOP；无正常函数返回。

两种bitmap确实分别仍为(1,0)/(全1,全1)，但本次free不读取bitmap，不求槽商，不调用assigned/clear，不写free/CF/CU，不执行迁链/回收。不能由图中bit为1越过先行free==total门，也不能写成“assigned返回非零后拒绝”。A的对象前提停止与B的源码循环必须在正文、facts、DOT中不同标记。

## C · 克隆U_S后释放新增槽，再试残留槽（1独立序列）

入口为U_S的独立GIVEN复制世界；不存在仍在运行的原malloc栈。输入0x61404000由本合同给定，数值等于旧模型中的局部ret_val，但不是认领原malloc已经返回的指针。此入口未执行308/310，不接受free.count=1的REFILL_CALL_BOUNDARY替代。

固定顺序：free(0x61404000)条件性正常完成后，free(0x61400000)。第一调用的实际后态直接传给第二调用，不重置state。

### C1 · 第一次free新增bit1

1. 337门假；search键0x61400000，7比较/6推进返回P9；342/343得S9/C9。
2. 345的127==128为假；351字节差0x4000、O0x4000，商1余0。assigned使用word0、余数1、int模型mask2，unit3，unsigned返回2，352的!返回为假。仅这次真正求idx/调用assigned。
3. 358调用clear：1ULL<<1为2，64位补掩码0xfffffffffffffffd，word0从3→1，word1保持0；随后359 S9.free127→128，360 CF127→128，361 CU1→0。记录这四项源顺序写和各自前/后值，函数局部变量另列，不把自动局部赋值当metadata写。
4. 364判断128==1假；370判断128==0假，列表迁移均不进入；381读free.count0，>2假，回收体不进入。正常到391函数末尾，仅称给定源模型的void完成，不称实测释放成功。
5. C9仍used[S9]/1、free/full空/count0，slab_total1；S9 bitmap(1,0)仍有旧bit0。物理链、header count、D/Qd及所有其它缓存/bitmap无写。没有list_del_init等列表helper实际调用，不能为“最终空闲”补出插入free链。

### C2 · 第二次free残留bit0对应的地址

从C1后态输入0x61400000；search再7比较/6推进返回P9，owner读取同前，345判断128==128真，进入348 SOURCE_LOOP。旧bit0仍1只是状态观察，351没有求idx，assigned/clear未调用，零metadata写；不写“重复释放检测发现bit未清”或“成功清掉旧位”。本序列终点是第二次源码循环，不是第二次正常void返回。

### 域明确的阶段谓词

沿213定义CACHE_ROOT、CACHE_OWNER_GIVEN、BITMAP_ROOT、BITMAP_OWNER_GIVEN、CLASS_ROOT、OWNER_REGISTRATION、LENGTH_ROOT及SLAB_TOTAL_ROOT/OWNER，根成员按anchor实际遍历，不从count制造对象。给定活owner集合始终{S9,D}。

|状态|根CF/CU和|owner给定和|S9.free与bitmap空位|CLASS_ROOT|LENGTH_ROOT|
|---|---|---|---|---|---|
|R_S/B停点|128/0，CACHE_ROOT真|128/128，CACHE_OWNER_GIVEN假|128−127=1|真|真|
|R_F/B停点|128/0，真|128/128，假|128−0=128|真|真|
|U_S/C1前|127/1，真|127/129，假|127−126=1|真|真|
|C1完成及C2循环|128/0，真|128/128，假|128−127=1|假|真|

上述各状态BITMAP_ROOT与owner合取均假，D个体bitmap仍真；OWNER_REGISTRATION假（D0次），slab_total1对根1真、对owner2假。A截至342前不写metadata，保留R_S观察值；不能把它们说成342之后的真实运行后态。

## 2. 源输入、正式证据与交付结构

十二原根沿213读取：malloc.c、_malloc.h、utils.h、list.c、bitmap.c、两canary源及五份CMake。新正式窄窗全部从固定time独立回源，P6不复用旧160作为新账；不用行数或证据数配额。至少准确覆盖：

- malloc.c本地H/round宏；search151–169完整路径、首读anchor.next/每次推进/比较/成功break/NULL返回；free330–391完整范围，重点DEBUG在NULL前、341返回到342/343对象读取、345/348、351/352、358–361、364/370、381及391末行NONE。
- 213三个导入快照的来源需用固定152选择器并核具体字段。若新文档断言init建立某字段或malloc生成(3,0)，须相应回源174–221/249–304正式取证；否则明确是本批GIVEN快照消费，不把旧窗口改贴为新证据。C特别定位308/310/311未被导入。
- bitmap.c assigned4–12完整低位返回路径、clear19–22；utils.h bitmap类型/单位、container_of/List/List_hdr，_malloc.h实际page/slab/cache成员。idx只真正求1，N1547及高位signed移位外证UNKNOWN，不扩为0–127表。
- list.c用来核结构/遍历背景与未进入的迁链接口；不能把本批未调用helper记成实际调用。五份CMake按真实指令实例，只证明候选源码/目标关系，不认证真实sh链接、标准/DEBUG或编译器；两time canary及直接master反验和公约原句保持。

全部正式窗1–5连续物理行，CRLF/LF/NONE及围栏分隔按真实原字节可逆表示，实际定义/调用实例范围不能只字符串命中。evidence每窗应直接支持相邻断言，source_index/required_anchor_coverage对齐。新claimed source facts与GIVEN/整数模型/派生观察分开。

恰六新增目录user-free-reinit-153/：MANIFEST.md、free-reinit.md、facts-and-boundaries.yaml、free-reinit-paths.dot、deferred-findings.yaml、evidence.md。facts至少包括D00、snapshot_imports、given_clones、free_cases、ordered_delta、call_boundaries、read_write_sets、predicate_domains/observations、build_scope、source_index、required_anchor_coverage、graph。共享基态与variant/前缀引用可重建，不复制全部16cache到每节点。

DOT与facts.graph节点/边全部属性相同；snapshot_import是数据来源关系，不能画成旧malloc至新free的实际控制边。只有C1正常void完成→C2入口是一条controlled_next_top_level_call；A/B和C2停点无后继执行边。分析停止、SOURCE_LOOP及正常void完成严格分开；谓词是观察，不作为源级分支或“验证成功”门。

deferred只登记本批新组合观察与实际旧项状态变化，不把147/151/152的已知局部问题再编新源码缺陷；普通问题先入库，无Owner逐项技术选择。完整本地P9/预处理/编译链接/项目程序脚本/Graphviz/QEMU/实机NOT_RUN，真实模型/后台effort未独立认证。

## 3. G00、规范与单一写入范围

实际发令入口是本轮PR17唯一主线评论。先核其真实ID与实际固定lead提交，完整读本合同、同提交152独审和[检查点](checkpoints/2026-11-26-reinit152-reviewed-free153.md)，并完成上列D00当次判断部分。保持原任务MYOS2-LEAD-002-CORE-CHECK-01、阶段2、原exec与原Draft PR17，唯一写区为本合同frontmatter列出的user-free-reinit-153/六件。未授权修改源码、旧成果、主线文件、PR元数据或跨仓业务。

依[79](79-work-package-sizing-and-pro-review.md)只复用C11a本人同一会话已完整读且版本未变的十五项目规则、有效MA全文及同版共用十五条；不是复用C11或旧C10c的已读。读[63](63-github-write-retry-and-readback.md)、[198](198-repository-first-compact-relay.md)与[210](210-mainline-c11-recovery-and-context-thresholds.md)适用条款；212给出的固定读序及任务局部Mnemosyne命名/接收范围继续有效。新缺项实际补读；新对话则需完整独立接收，不沿用本同会话例外。

刷新MA master/建设及Mne master；本轮实核MA master b234c6c91a2e、建设5cac46110877、Mne e581eb9df6c4。MA现行入口仍指48采用→47 v1.4-r10.266及45同版共用1.1-r9.260十五条；建设检查点§321只是Alaya意见收存/等待合议，未改变当前有效规范。C11a本次须读该新增现状，记真实读取和自身未变内容复用，不重开外仓任务。新实际ref若变化，先核适用差异，不用旧短号覆盖或盲目停所有工作。

G00必须实做并入MANIFEST：

1. 直接lead等于本轮正式评论的实际发布提交；exec=6994f6b1aaec，time=a039d9803ade，直接master=e8e60cd19540；不用PR.base_sha代替master。原PR16 Open/Ready、PR17原题Open/Draft、原head/base master且未合并，开放PR无since全分页至空。
2. PR17顶层评论无since全分页至空，检查点列50历史标记及本次2新标记共52逐条唯一、实际ID正确。主线本轮新评论、152作者7185898830、前次发令7185473439各直接ID双读并与分页全文一致。分页页数以实时为准，不在满100页时提前结束。
3. 历史EXEC-23/149、EXEC-22/145、旧EXEC-24/152及旧EXEC-24/153作者0；本批EXEC-25/153作者入场0，旧EXEC-25/152作者仍唯一。153结果目录未占用。未知写入先定位保全，不能覆盖或冒用署名。
4. 152固定6994f6b1aaec、活动exec、API Git Blob六件全文核同，1868 LF/606992 UTF-8字节；唯一父07d636217117、一提交六新增零其他改删。原文件对象、评论冻结。语义只按本合同三个快照选择器消费，不重做152完整技术/160窗审查。
5. MANIFEST明确当前模型指定/可见系统身份/后台和effort UNATTESTED、实际胜任/容量判断、task_local_only/current_task_preserved及当前guidance_refresh完成状态；旧152记账补注与本次判断分列。真实阻断只停受影响部分并报告最小缺口，不把缺失改成已完成。

同一153包D00和技术工作连续完成，不先单独交D00再等待另一Pro回合。D00提交仅待主线判闭，作者不能自己将152总体写成PASS。

## 4. 六件、自检与发表后终检

六件在§2及frontmatter中固定，禁止第七件。MANIFEST除D00与当前接收判断外保留标准read_channel、startup_selfcheck_quote、branch_canary_quotes及self_check三个整数；facts必须能用固定共享基态/明确clone/有序delta唯一重建，给读写和未达边界；正文及图中的分析停、源循环、正常void完成互不混用。未到达351的例子不得为便利填上“实际obj_idx=0”。

P6新正式窗n=m+k；交付前真正重新打开固定源逐窗核文字、物理行、真实定义/命令实例及CRLF/LF/NONE。旧152160窗与本主线0新窗账不并入。连接器可达P9第1–8、11、13实做；9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。强制源锚不得以降级规避；无配额，不重复灌整套16cache或已冻结的长机器件。

写前fresh核当前refs、作者/目录0和未知并行写。只以真实6994f6b1aaec为唯一父、expected_sha匹配、force=false，在原exec一次提交六新增零其他改删；不新建分支/PR。先验证六件固定提交/活动分支/API Git Blob全文、LF/UTF-8字节、唯一父与净差异，再发表唯一作者回执。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-25 / CORE-USER-FREE-REINIT-153 / FORMAL_DELIVERY

正式评论列固定合同/提交/六件/真实D00补注、模型身份与限制、实际自检；发表时G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING。发表之后实际核新ID两次、所有顶层评论至空，52原标记＋新作者共53各唯一及真实ID；历史三负标记和旧EXEC-24/153为0；四ref（exec为本批实际新提交）、开放PR及两PR原状态/原题/元数据、六件三渠道全文/唯一父/净差异、旧152和所有旧文件/评论冻结。全部完成后才报告READY_FOR_MAINLINE_REVIEW、实际G04=PASS和STOP_WRITING，不启动154。

GitHub暂态异常不因一次失败直接停工；先核远端是否已写成，再按63号间隔重试，通常首次加三次、10/20/40秒或有效实质工作间隔。保持expected-head/force=false；未知并行写入不覆盖。记录真实write_retry_log及回读，没有实际重试则not_needed；工具重新发现/调用上限恢复与真实写重试分账，不能把工具缺失推断成没有权限，不重复评论或提交。

最终依198只给一个通常3–6行原生短复制块、一个真实作者回执URL，写152补正待判闭、153实际交付/G04/停写及交C11 Pro独审并同轮后继。完整正文和证据均留GitHub，不要求Owner拼接长报告或逐项选择技术处置。

## 5. 设计就绪与不变边界

本合同技术方向由只读设计分工提出，另一未参与候选实现的分工从固定源及其本轮独立152模型重新生成状态，135组比较零失败：5入口6free、60候选比较/56推进、2分析停/3源循环/1条件性void完成、4实际字段写一致。此为合同就绪核验，不预签未来153作者成果、D00关闭或实际G04。

C11本轮成功首次正式发布153后计2，8/10均未达；C10历史11不重置。后续交接继续传十五项目规则、有效MA全文/同版共用全文、Mnemosyne命名依据、实际断点与该计数。已有授权内持续推进、结果先行、必须人工动作集中、普通问题先入库、次一档执行/Pro独审及同轮后继保持。

阶段2。完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机NOT_RUN；自制文本/符号整数辅助不构成项目运行。真实C对象/指针、映射、生命周期、标准/ABI/DEBUG/编译器/链接、生产入口、原malloc返回、311 callee及N1547独立外证UNKNOWN。无源码/旧件修改、合并、强推、删除或跨仓写权；实际后台模型/Extra未独立认证，不操作用户聊天UI或自动切换型号。
