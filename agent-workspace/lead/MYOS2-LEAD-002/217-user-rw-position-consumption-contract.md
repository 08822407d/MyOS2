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
read_channel: GitHub connector；自制文本/整数/符号链核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
evidence_class: SOURCE_FACT / GIVEN / ARITHMETIC_MODEL / OBSERVATION / UNKNOWN
lead_before_publication_short12: d02e263970ef
execution_pin_short12: b57bd74931dc
self_check_scope: 本合同新增正式源码窄窗0/0/0；154作者191窗的主线复核另账，不转作155新证据。
mainline_design_count:
  conversation: C11
  before_release: 3
  after_successful_155_release: 4
  thresholds:
  - 8
  - 10
  reached: false
D00: []
status: PREPARED_FOR_PUBLICATION_G04_PENDING
record_type: phase2_executor_contract
followup_id: CORE-USER-RW-POSITION-CONSUMPTION-155
executor_display_name: MYOS2-A-C11b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-26
executor_model_per_owner: 次一档GPT-6.2 Sol＋Extra／高推理
executor_start_state: NOT_OBSERVED
execution_handoff_package: MYOS2-A-C11a-TO-C11b-155-20261126
reception_guidance_status: NEW_CONVERSATION_REQUIRES_INDEPENDENT_FULL_READ
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rw-position-consumption-155/
allowed_files:
- MANIFEST.md
- rw-position-consumption.md
- facts-and-boundaries.yaml
- rw-position-consumption-paths.dot
- deferred-findings.yaml
- evidence.md
execution_parent_short12: b57bd74931dc
model_scenarios: 7
historical_115_pin_short12: 452444a1c4b4
historical_115_facts_blob_short12: 2246b85a8792d
open_questions:
- 回调直接写是GIVEN，不认证生产驱动、真实IO、C/ABI或实际后台档位；新C11b尚未实际接收。
---

# 217号合同 · 第155批 · 标量读写的位置消费与别名

**新开「MYOS2-A-C11b 代码分析执行」／EXEC-26，选择GPT-6.2 Sol＋Extra／高推理；接收与执行无需Pro。** C11a已由Owner报告上下文耗尽。本轮同时发布[216恢复包](216-executor-continuation/00-C11a-to-C11b-recovery-package.md)及[完整启动词](216-executor-continuation/01-MYOS2-A-C11b-receiver-startup-prompt.md)；新席自行完整接收并加载规范，按本合同连续完成三相关专题、七有限场景、六新增、自检、唯一作者回执、实际G04后STOP_WRITING，交C11 Pro独审并同轮后继。不另等开工，不进入156。

[154主线独审](reviews/CORE-USER-BRK-RANGE-ACCOUNTING-154-review.md)裁定PASS_PENDING_LOCAL、D00=[]。冻结[154作者#7192993361](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7192993361)及[b57bd74931dc六件](https://github.com/08822407d/MyOS2/tree/b57bd74931dc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-brk-range-accounting-154)。旧C11a/EXEC-25的作者归属不变；新席不重做154或重审191窗。R152-01承继153独审的闭合状态，152原RETURN、历史NOT_RECORDED及未证后台档位不倒签。**155 D00=[]。**

本批从固定time源码另建标量读写的位置消费模型，不承接154的VMA状态/控制栈。旧115精确原件：[facts@452444a1c4b4](https://github.com/08822407d/MyOS2/blob/452444a1c4b4/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memdev-scalar-io-115/facts-and-boundaries.yaml)，历史排重只消费下文明确选择器。

## 1. 增量与旧材料的精确消费

本批三相关专题：A同样零返回时，迭代读/写怎样覆盖回调对file.f_pos的直接赋值；B负返回及stream分支怎样保留该直接赋值，且保留的局部位置不相同；C kernel_read显式位置参数与file.f_pos别名时，iter与legacy接收器的差别。共A2/B3/C2七个独立有限场景，不是七次真实系统调用。

必须合读旧115及其主线R115-03、27及R27-S01/S03、62及R62-S01，承继08/41的fd/归还限制、63的用户访问限制和117的实际调用者补注。本批不重做fd索引、引用计数、用户复制、VFS准入表、设备注册、文件缓存/页填充或全部迭代器审计。

固定115原件为452444a1c4b4（固定提交及facts blob短号见页首）下user-memdev-scalar-io-115/：其facts.A.position_rule.stream明确回调可直接改file但not modeled；A.models[0..8].premise九例都排除回调直接f_pos修改和并发。原正文A同样声明“没有回调直接改file字段”。其A.iter_detail、对应独审R115-03已给出new_sync_read/new_sync_write的位置条件与排队哨兵门；本批不将这些已知单点登记为新根因。27已给kernel_read迭代分支不回写ki_pos及legacy分支保留原count；62已有向量回调直接字段写与外层提交的限定。本批增量是**在同一明确直接字段写入条件下，逐层组合标量迭代助手、外层提交门以及kernel_read别名参数**，不得冒称证明真实FAT或设备回调确实采用这些写入。

旧115九例、旧27/62证据和既有补正均保持，不重新复制成长机器件或重算旧证据账；只定点核上述选择器/原范围、按本批实际使用源码建立新窗。未完成的完整本地P9、真实返回/ABI和实际运行限制不改变。

## 2. 共同GIVEN、字段身份与停止边界

### 2.1 数值和对象

明确模型：byte8，int/unsigned32，long/ulong/size_t/ssize_t/loff_t及指针64；本批所有有符号位置、返回和加减均可表示。没有并发、隐藏别名或额外写者。F为一个活且正确类型的file对象，F.f_op指向所列有效方法表；所有实际访问的局部对象、方法目标和字段有效。F初始f_pos=10，且f_mode/f_op在每例中保持。七例各自独立重置，不按顺序操作同一个真实file。

buf是仅传参的有效指针符号B；A/B count=8。C count=N=MAX_RW_COUNT+0x20=0x7ffff020=2147479584；固定PAGE_SHIFT12、PAGE_SIZE4096及32位INT_MAX给M=MAX_RW_COUNT=0x7ffff000=2147479552。C不分配或复制约2GB内容，模型只传递长度和指针；回调按下面GIVEN不读写缓冲。无实际内存访问范围/大分配或用户复制认证。

CONFIG_BUG/WARN选择只用固定源码文本所选分支建模，实际预处理未知；所有回调返回仅0、-5或3，均不等于-EIOCBQUEUED=-529，故A/B所选new_sync_read或new_sync_write的BUG_ON条件为假。C的__kernel_read没有该BUG_ON，不给C添此哨兵；C的FMODE_READ存在，所选WARN_ON_ONCE条件为假。不展开panic，不把-529算作本批普通负返回。

### 2.2 入口与未展开边界

A/B从ksys_read或ksys_write中**fd取得调用已经按题设正常返回之后**的局部观察帧开始，f.file=F；该时点F.f_pos=10及下表字段直接给定。fd.flags不在所选前缀中读取，不为便利制造其数值或重审两个fd助手等价性。保留源getter名称分别为myos_fdget_pos与fdget_pos；未推导它们的内部读取/引用动作。

A/B之后真实消费file非NULL门、file_ppos、局部pos、vfs方法选择、同步助手和外层提交条件，**统一在执行fdput_pos调用之前停止**（read_write.c读侧194行、写侧334行）。这是ANALYSIS_STOP_BEFORE_FDPUT_POS，不是源码return、不证明归还完成或ksys最终已返回。可以记录到此已取得的局部ret；不能写成用户系统调用返回。F在这个边界前保持有效；不越过可能的释放请求继续观测对象。

A/B中VFS权限位按下表给定，因此模式门通过。access_ok只采用本批明确GIVEN的true且不改受跟踪状态；不展开其配置分派或重复63。rw_verify_area必须按固定活动函数第一条return0推导，不能伪造成GIVEN的完整范围检查。count8没有触及M截断。所选回调及其他本包正常完成的直接初始化器不改F的模式、方法表或本包未列字段。

C从kernel_read(F,B,N,pos)局部函数入口开始，**pos明确等于&F.f_pos**，不存在另一独立位置对象Z。按源码经过rw_verify_area→__kernel_read，实际正常返回只认证此局部kernel_read的3，不扩展任何上层真实调用者、系统调用寄存器或数据到达。C没有fdget/fdput。

### 2.3 三种位置接收器与回调GIVEN

字段须分开命名：F.f_pos；A/B非流时ksys栈内L=pos；同步助手的K=kiocb.ki_pos；C参数P=pos指向F.f_pos。非流A/B的ppos实际改指&L，**不等于&F.f_pos**；stream时ppos=NULL，L未赋值，后继不得读它。C的P与F.f_pos才是本批显式别名。

K、iter以及C的iov均是其所属helper/__kernel_read的栈内对象。下文汇总出现的K/iter/iov值，均指所属函数返回之前最后观测的**历史快照**，不能在fdput_pos前或kernel_read返回后假定这些局部对象仍然存活，更不能通过已失效地址再读取。A/B终点仍存活的L和F值与这些历史快照分栏；C返回结果同样与__kernel_read局部历史快照分栏。

所有迭代回调采用同一有序GIVEN：

1. 经kiocb.ki_filp指向同一F，直接写F.f_pos：10→90。
2. 写当前kiocb.ki_pos为70；不得偷改A/B的L或C的指针变量P本身。C的*P因P=&F.f_pos随第一步的直接F写自动变为90，这不算另一独立写入，也不违背P指针保持。
3. 按各例给定r正常返回；保持iter的全部活动字段、F其它字段、缓冲内容及其他受跟踪对象不变。

这两次写是**GIVEN_CALLBACK_EFFECT**，不是在仓库找到的真实驱动语句，不计作本批已证源码写；其返回值与位置赋值独立，不用r计算位置增量。不假设回调完成实际IO、复制字节、文件长度变化或推进iter.count。

C_LEGACY_ALIAS的legacy回调另给严格顺序：先F.f_pos=90，再*P=70，正常返回3；P确实为&F.f_pos，所以第二步写同一对象90→70，不能按两个独立位置保存。保持其它受跟踪字段/缓冲。此顺序属于本例GIVEN，不由C实参求值顺序或真实FS实现推导。

## A · 零返回时两种同步助手与外层覆盖（2例）

### A_READ_ZERO

入口ksys_read，F.f_mode=FMODE_READ；方法表read_iter=RI、read=RL均有效。RI是本例所选迭代GIVEN回调，RL存在但不执行。r=0，非stream。

file_ppos先返回&F.f_pos，ksys保存L=10再令ppos=&L。vfs_read按read_iter优先进入new_sync_read；真实初始化kiocb.ki_filp=F，K先由复合初始化为0，再由ppos读得10；UBUF迭代器count8。回调先F10→90、再K10→70并返回0；BUG_ON(false)后读侧只需ppos非NULL，因此L10→70。vfs返回0，ksys的ret>=0&&ppos真，F90→70。在fdput_pos前停。

结果：局部ret=0、L=70、K=70、F.f_pos=70，外层提交覆盖了回调直接写90；不是回调报告0就能保证文件位置未变。逐步说明源写与GIVEN写的不同来源。

### A_WRITE_ZERO

入口ksys_write，F.f_mode=FMODE_WRITE|FMODE_CAN_WRITE=0x40002；方法表write=NULL、write_iter=WI有效，r=0，非stream。不能给write非NULL后仍虚构进入new_sync_write；其活动优先序与read相反。

L初10、ppos=&L、K初10，UBUF count8；同样给定回调先F10→90、K10→70，r=0。BUG_ON(false)后，new_sync_write的ret>0&&ppos为假，L保持10；ksys的ret>=0&&ppos真，F90→10。在fdput_pos前停。

结果：局部ret0、L10、K70、F10。helper没有向L复制新位置，但外层仍会把旧L写回F；这是实做的90→10覆盖，不能概括成“零返回时没有位置赋值”。与A_READ_ZERO比较的是同一回调GIVEN下的源码消费差别。

## B · 负返回及stream保留直接字段写（3例）

### B_READ_NEG

入口/方法/模式与A_READ_ZERO相同，独立重置F10，回调r=-5，仍先F90后K70。BUG_ON条件假；读helper仍将L10→70。外层ret>=0假，短路跳过其ppos条件/赋值，F保持回调写入的90；停fdput_pos前。结果局部ret=-5、L70、K70、F90。

### B_WRITE_NEG

入口/方法/模式与A_WRITE_ZERO相同，r=-5、同一GIVEN。write helper的ret>0为假，L保持10；外层ret>=0亦假，F保持90。结果局部ret=-5、L10、K70、F90；停fdput_pos前。两负返回外层都没有提交，不等于整个路径把file位置恢复为入口10，也不表示两个局部L相同。

### B_READ_STREAM

入口ksys_read，F.f_mode=FMODE_READ|FMODE_STREAM=0x200001，read_iter=RI/read=RL，r=3。file_ppos返回NULL，没有读取F.f_pos来初始化L，也没有把ppos改指栈内L。new_sync_read初始化K后以ppos为空选择0；回调给定F10→90、K0→70，正常返回3。

BUG_ON条件假，read helper的ppos门为假，不读写L；外层ret>=0真、第二项ppos假，跳过F赋值。停fdput_pos前；局部ret3、L=NOT_INITIALIZED_AND_NOT_READ、K70、F90、ppos=NULL。这里保留90来自明确回调GIVEN；不能称为stream保证F.f_pos永远不变。

## C · kernel_read位置参数的显式别名与方法消费（2例）

两例各自F.f_mode=FMODE_READ、F10、count=N=0x7ffff020、P=&F.f_pos。rw_verify_area立即0；__kernel_read先构造iov.len=min(N,M)=M，再过读模式门、init_sync_kiocb、K=*P=10及KVEC迭代器。初始化器实参READ不自动证明存在data_source字段；所读该字段/赋值仍为注释。

### C_KERNEL_ITER_ALIAS

read_iter=RI、read=RL均有效，故实际走iter；同一迭代GIVEN先F10→90、K10→70并返回3，保持iter。回调收到iter.count=M、kvec.iov_len=M、nr_segs1、offset0；没有收到原始N作为它的迭代长度。

__kernel_read尾部回写*pos的代码为注释，不执行；kernel_read因此正常局部返回3，F.f_pos=90，*P也为90（同一对象），K70。不能写成“pos未更新所以*P仍10”：没有该源码赋值，仍可因P别名F而观察到回调直接字段变化。旧27“不回写ki_pos”事实继续成立。

### C_KERNEL_LEGACY_ALIAS

read_iter=NULL、read=RL有效。前面的iov/KVEC初始化仍实际发生，K10不被本例回调改动，但这个迭代器没有被传给legacy回调。实际调用实参是(F,B,N,P)，count仍原始N，而不是iov.len=M。

legacy GIVEN先F10→90，再*P=70；P与F.f_pos别名，因此真实模型顺序为F10→90→70，返回3。__kernel_read无后继位置回写；kernel_read局部返回3，F=*P=70，K仍10，未使用的iter.count及iov.len仍M。

两C场景只比较明确调用接口/别名/给定回调写入后的字段；不把N与M差别新报成已发生的越界IO，也不声称实际文件系统按给定顺序改位或copy满某长度。

## 3. 必要源码锚及紧凑交付

所有正式新窗从固定time原文独立定位，1–5连续物理行，真实定义/调用实例和LF/CRLF/NONE分别核，不复用115/27旧窗充当本批证据。不设窗口数量配额。

| 来源 | 必要核验范围 |
|---|---|
| mykernel/fs/vfs/read_write.c | rw_verify_area 49–70；new_sync_read 75–93；__kernel_read 96–129及kernel_read131–140；vfs_read142–171；file_ppos175–178、ksys_read180–197；new_sync_write201–217；vfs_write287–317、ksys_write319–339。按实际原文重新定位；未选路径只作方法门/未达证据，不自动扩大执行场景。 |
| mykernel/include/linux/fs/fs.h | FMODE_READ/WRITE/CAN_WRITE/STREAM；kiocb_s实际两字段；file.f_pos/f_mode/f_op；read/read_iter/write/write_iter真实函数签名；init_sync_kiocb复合初始化；MAX_RW_COUNT。不可把被注释ki_flags/ki_complete当活动字段。 |
| mykernel/include/linux/kernel/uio.h | kvec/实际iov_iter字段与联合体；ITER_KVEC/ITER_UBUF、ITER_SOURCE/DEST；iov_iter_ubuf/kvec实际初始化、offset/count/nr_segs、未存储direction。仅跟踪选定活动成员，不展开整个迭代器库或复制器。 |
| mykernel/include/linux/kernel/kernel.h | READ/WRITE活动定义51–52行；C的iov_iter_kvec实参READ按该定义取0，仅是传入实参，不虚构存储进注释字段。 |
| mykernel/include/linux/fs/file.h | fd_s、fdget_pos及fdput_pos的实际接口，定位停止点背景；不运行/展开fd取槽与fput引用生命周期。 |
| mykernel/include/linux/lib/errno.h；mykernel/include/linux/kernel/asm-generic/bug.h；mykernel/include/linux/debug/bug.h | EIOCBQUEUED=529与所选BUG_ON/WARN_ON_ONCE实际定义，判定这些输入未进入异常体；宏条件构建文本不等于实际预处理。 |
| mykernel/include/linux/kernel/limits.h；mykernel/arch/x86_64/mm/page/page_const_arch.h；mykernel/include/linux/kernel/minmax.h | INT_MAX/PAGE_MASK、min_t活动定义→__cmp分派与M/N数值域。活动__cmp以条件表达式重复使用选中操作数，不能称通用单次求值；本例count与MAX_RW_COUNT稳定且无副作用，所以数值M不受重复求值影响。只数值模型，不真实大内存IO。 |
| mykernel/include/linux/kernel/types.h；mykernel/include/uapi/linux/posix_types.h；mykernel/include/uapi/asm-generic/posix_types.h | ssize_t/size_t/loff_t/ulong等本包选定类型链，选定64/32位模型与实际ABI未证分列。 |
| mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c、mykernel/include/linux/compiler/myos_debug_option.h | 固定配置/双canary和定义选择；两直接master反验、公约与本项目P9规范由管理段要求。 |

facts至少包含historical_scope、numerical_model、object_identity_and_aliases、given_frames、callback_effects、seven_scenarios、stage_values、source_writes、given_writes、actual_reads/not_reached、callback_arguments、terminal_boundaries、source_index/required_anchor_coverage、graph。只保存有意义的阶段字段，不复制未使用的fs.h全部对象或旧九例。

来源性质至少分SOURCE_FACT、GIVEN_CALLBACK_EFFECT、DERIVED_CONDITIONAL、UNKNOWN。按明确顺序分开保存回调直接写F与ki_pos/*P、helper对L的源码赋值及外层对F赋值，不能把GIVEN写数冒充真实仓库源写数量。初始化复合字面量的零填充按本包选定C对象初始化规则建模，不认证实际内存布局；语句级源码顺序不等于机器存储时序或原子性。

DOT与facts.graph全部节点/边属性相同。七场景独立；GIVEN返回/副作用边和源码控制/赋值边分开，回调目标只是本模型有类型的间接调用，不冒称某真实驱动已绑定。A/B五个fdput前分析停无执行后继；C两个正常局部返回不连接用户态。无需按每字段写造庞大图，允许共享源码节点并明确场景守卫和对象别名。

六件按frontmatter指定；deferred仅登记本批新增组合观察、旧115/27/62源事实追加及实际独审遗留项（本合同D00=[]）。不得把“零返回改变位置”“负返回保留直接写”泛化为已证真实FS错误、POSIX不合规、数据损坏或新独立漏洞。后继若要证明生产回调确实写F/ki_pos，须另获明确范围并读其具体实现。

本合同授权新执行任务；不修改内核/用户代码、旧结果或PR元数据，不执行仓库脚本/预处理/编译/链接/Graphviz/QEMU/实机。可自写有限文本/整数符号辅助，本批模型成功不代表MyOS2运行成功。完成后按正式管理段唯一作者回执、实际G04及STOP_WRITING，不进入156。


## 4. 新席接收、规范全文与唯一写区

实际入口为本轮PR17唯一主线正式评论。先取真实评论ID及最终固定lead提交，完整读同提交本合同、[154独审](reviews/CORE-USER-BRK-RANGE-ACCOUNTING-154-review.md)、[检查点](checkpoints/2026-11-26-c11a-to-c11b-brk154-reviewed-rw155.md)、216恢复包及完整启动词。Owner已授权本轮恢复和后继；[00第19条](00-work-order.md)登记原话与边界。恢复包本身是non_execution_source_transfer_artifact，执行权限来自Owner、工作令与217；不以外仓材料另生业务或写权。

新席为MYOS2-A-C11b／MYOS2-LEAD-002-CHAT-EXEC-26，保持任务MYOS2-LEAD-002-CORE-CHECK-01、阶段2、原claude/dazzling-cori-q0dnyt及原Draft PR17。唯一写区/六文件在页首。无源码、旧成果、主线文件、PR元数据或外仓业务写权。

按216§3逐项独立全文读十五项目规则，含当前00第19条、有效P1–P13、18/24/28/63/79/110/124/193/194/198及210。按216§4独立读Meta-Agent现行入口、48采用决定、47有效v1.4-r10.266全文、45同版共用1.1-r9.260中01–14及16全部十五条、最新建设检查点与模型登记；独立读Mnemosyne任务局部接收/命名的四材料。输出截断分段到尾；摘要/标题/前任或主线已读声明不能代替。79自身同会话未变复用只在本新会话本人完整读过后才适用，不复用C11a/C11的已读。

本轮MA master b234c6c91a2e、建设06b783861368，入口881fd8c1076e、检查点54ac6e5fc66c；§322的Alaya反馈原始背景和收件流程分析待合议，未改变48采用→47及45同版十五条。Mne master e581eb9df6c4。新席fresh核真实refs和适用差异；不把待议反馈当正式采用、不用旧短号覆盖新状态，也不接走Mne/Alaya active-context/todo。

MANIFEST把execution_handoff_receive与guidance_refresh分列：package_id=MYOS2-A-C11a-TO-C11b-155-20261126、package_status=non_execution_source_transfer_artifact、真实固定入口/授权、接收日期/未知时刻、新名/EXEC-26、任务/阶段/分支/写区、实际新读的路径/版本/范围、缺项/冲突、当前胜任与容量的真实定性判断、下一安全动作、task_local_only/current_task_preserved；实际读全后才写当前refresh_completed=true。Owner指定档位、可见身份、实际客户端/后台/effort分别记账，无法认证保持UNATTESTED；不以旧C11a成功代替本次判断、不预保证足以完成未来批次。只有实际材料/权限/容量阻断才保全并报告最小缺口，不因未知后台档位反复索取Owner确认。

## 5. fresh G00与冻结基线

真实完成后入MANIFEST，不把本合同准备记录照抄成新席已检：

1. 直接lead等于本轮正式评论的最终发布提交；exec=b57bd74931dc，time=a039d9803ade，直接master=e8e60cd19540，不用PR.base_sha替代。原PR16 Open/Ready、PR17原标题Open/Draft，原head/base master、未合并；开放PR无since完整到空，未知并行变化先定位保全。
2. PR17顶层评论无since全分页到空，满100必须继续。检查点57历史精确标记加本轮3新标记共60，各字面恰一次且实际ID正确；不能只核总数。本轮正式入口、154作者7192993361、前次154发令7192512724各直接ID双读并与分页全文一致。准备时303评论加本轮入口预计304，实际页数以新方读数为准。
3. 历史错误作者EXEC-23/149、EXEC-22/145、EXEC-24/152、EXEC-24/153、EXEC-24/154继续为0；EXEC-24/155、EXEC-25/155、EXEC-26/154也为0。本批EXEC-26/155作者入场为0，EXEC-25/152、153、154各1。核全体作者的155占用和整个结果目录未占用，不通过换身份绕过冲突。
4. 154固定b57bd74931dc、活动exec、实际API Git Blob六件全文核同，11227 LF/376430 UTF-8字节；唯一父c9f4ba8d3226、一个提交六新增零其它改删，旧2247非tree对象及旧评论冻结。此处核冻结，不重建154模型/191窗。两time canary、直接master反验及公约原句按规范实际核，保留read_channel及自检字段。
5. 实际接收、完整规范刷新、权限/材料/胜任/容量判断明确。155 D00=[]；R152-01闭合及152历史未知按既有裁定消费。全部材料具足后连续做本包，不把七例拆七次Pro回合或另等Owner技术裁定。

## 6. 六件、自检、唯一作者与发表后G04

恰六新增：MANIFEST.md、rw-position-consumption.md、facts-and-boundaries.yaml、rw-position-consumption-paths.dot、deferred-findings.yaml、evidence.md。接收与刷新留MANIFEST，不另发接收评论或第七件。保留read_channel、startup_selfcheck_quote、branch_canary_quotes及self_check三整数；来源类别、GIVEN写与源码写、短路未读/未达、helper历史快照和实际终点分栏。A/B五例停止前不接源码执行后继，C只画kernel_read正常局部返回。

正式证据每窗1–5连续物理行，按固定源真实字节、CRLF/LF/NONE与实际定义/调用/命令实例重开核验；相邻断言需由对应窗支撑，P6 n=m+k。旧154的191、旧115/27/62证据及主线0窗不并入本批账。连接器可达P9第1–8、11、13实做，9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。强制源锚不能靠降级回避，不设字数/窗口/字段/新发现数量配额。

写前fresh四refs、原PR状态、作者/目录未占用和未知并行写。只以实际b57bd74931dc为唯一父，expected_sha匹配、force=false，在原exec一次提交六新增零其它改删。先回读六件固定提交/活动exec/API Git Blob全文，核LF/UTF-8字节、唯一父、compare净差异与全部旧对象冻结，再发唯一作者正式评论。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-26 / CORE-USER-RW-POSITION-CONSUMPTION-155 / FORMAL_DELIVERY

正式评论列实际固定合同/提交/六件、D00=[]、七例给定边界、所选模型/可见身份与未证后台档位、实际自检。发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING，不能预签发表后检查或C11验收。

发表后真实完成新评论ID双读并与全分页同文；全评论到空，60旧标记＋唯一作者共61字面各一次且实际ID正确，上述八错误作者标记为0；四refs（exec改为实际155新提交）、开放PR到空、两原PR状态/标题/元数据；六件三渠道全文、唯一父/一提交六新增零其它改删、154及全部旧文件/评论冻结。通过后才报READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING；不启动156。

GitHub暂态错误按63：先核远端效果、避免重复提交/评论，再通常首次加三次、10/20/40秒或有效实质工作间隔限次重试。保持expected-head和force=false，未知并行写不覆盖。write_retry_log如实记录，没发生写重试则not_needed；工具发现/调用上限恢复与写重试分账，不把单次暂态失败当权限丧失。

终回按198用一个通常3–6行原生短复制块和一个真实作者回执URL，报155六件/唯一提交/P6、实际G04/停写和必要未知，请C11 Pro独立验收并同轮后继。全文/证据/详细处置留GitHub，不让Owner搬长报告、拼接材料或逐项裁决普通技术问题。

## 7. 合同独立就绪及计数

155技术合同另由设计分工准备，另一分工从17份固定源码、78处精确源锚独建七场景位置/别名模型后再比候选，未读或执行设计者model155；183项检查、0失败，READY。发布前已校正min_t活动__cmp可重复求值、helper局部对象只留返回前历史快照、P保持与*P别名随动、BUG_ON仅A/B四处文字。该检查只证明合同条件与预期就绪，不预签155未来交付或实际G04。

同一回调GIVEN下七例终点F.f_pos依次为70/10/90/90/90/90/70；A/B五例仅有fdput_pos前局部ret，C两例仅有kernel_read局部返回3。C截断M=0x7ffff000、原count N=M+0x20；稳定操作数不受活动min_t重复求值影响。helper栈对象在返回后只保留历史快照，不读取失效栈；C参数P不变但*P随F别名变化。独审建立的字段计数仅其核验口径，不强加交付配额。

C11本次成功首次正式发布155后计3→4，8/10均未达；154验收和换席恢复不另计，C10历史11不变。未来交接须携带十五项目规则、有效MA全文/同版共用全文、Mnemosyne命名依据、实际断点/身份/计数与口述约定。结果先行、问题先入库、已有授权持续推进、约三相关专题六件、次一档执行/Pro独审及同轮后继继续适用。

阶段2。无源码/旧件/主线/PR元数据修改、新分支/PR、合并、强推、删除或跨仓写权。完整本地P9、项目程序/脚本、预处理、编译链接、Graphviz、QEMU、实机、真实IO NOT_RUN；给定C/ABI模型、真实生产回调及数据效果、实际后台模型/Extra档位未独立认证。可自写有限文本/整数模型，不执行项目程序。主线只登记新C11b，未操作聊天UI或预签新席接收/启动。
