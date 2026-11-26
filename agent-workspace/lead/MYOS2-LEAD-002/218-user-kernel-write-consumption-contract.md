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
lead_before_publication_short12: 61acd9920c6f
execution_pin_short12: def3b82d9191
self_check_scope: 本文件新增正式源码窄窗0/0/0；155作者101正式窗的主线复核另账。
mainline_design_count:
  conversation: C11
  before_release: 4
  after_successful_156_release: 5
  thresholds:
  - 8
  - 10
  reached: false
status: PREPARED_FOR_PUBLICATION_G04_PENDING
record_type: phase2_executor_contract
followup_id: CORE-USER-KERNEL-WRITE-CONSUMPTION-156
executor_display_name: MYOS2-A-C11b 代码分析执行
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-26
executor_model_per_owner: GPT-6.2 Sol＋Extra／高推理；接收执行无需Pro
executor_start_state: NOT_OBSERVED_FOR_156
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-kernel-write-consumption-156/
allowed_files:
- MANIFEST.md
- kernel-write-consumption.md
- facts-and-boundaries.yaml
- kernel-write-consumption-paths.dot
- deferred-findings.yaml
- evidence.md
D00:
- R155-01
execution_parent_short12: def3b82d9191
model_scenarios: 7
open_questions:
- 155历史检查点全文接收未留证；本批先实际补读和新记账，待C11判闭；实际运行/后台档位未认证。
---

# 218号合同 · 第156批 · kernel_write准入、方法与位置消费

**继续原「MYOS2-A-C11b 代码分析执行」／EXEC-26，使用GPT-6.2 Sol＋Extra／高推理，接收与执行无需Pro。** 本合同授权先完成D00的实际补读与当前补记，再连续完成三相关专题、七场景、恰六新增、自检、唯一作者回执与发表后实际G04；随后STOP_WRITING，回C11 Pro独审及同轮后继，不另等开工、不进入157。

[155主线独审](reviews/CORE-USER-RW-POSITION-CONSUMPTION-155-review.md)裁定技术/源码文证PASS_PENDING_LOCAL，整体RETURN_DOCUMENTATION，仅余R155-01。冻结[155作者7195286787](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7195286787)及[def3b82d9191六件](https://github.com/08822407d/MyOS2/tree/def3b82d9191/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-rw-position-consumption-155)。本合同D00=[R155-01]；R152-01沿153闭合，152历史RETURN与NOT_RECORDED不变。

## D00 · R155-01检查点全文读取与当前补记

216恢复包§4要求新席“自己依次完整读”，其中包括MA建设检查点。155 MANIFEST.guidance_refresh.ma.read[4].range只记录L1–100及L1971–2033，却签guidance_refresh.refresh_completed=true；问题为FULL_READ_NOT_EVIDENCED，不是已证私有读取未发生。本项只补该范围/当前记录，不重做155七例或101窗、不改155旧件/回执、不另发155作者、不加第七件。

1. fresh核MA master与建设ref，固定本次实际checkpoint版本。准备时建设06b783861368、路径reviews/META-AGENT-FABLE5-REVIEW-AND-DESIGN-001/09-continuation/15-checkpoint-2026-10-01.md、blob54ac6e5fc66c，386310字节、2032 LF/2032物理行，最后有LF。155所记2033为换行拆分末空段，不当真实物理行。
2. 实际补足**自己本会话**的全文读取。若blob未变，已真实读过的1–100和1971–2032可按79复用；本次独立读取缺口101–1970共1870行，分段到尾后合为2032行完整覆盖。若版本变化，则完整读取本次新固定版本并核差异，不盲套旧行号。其他已完整读且版本未变的十五项目规则、MA有效全文/同版共用十五条与Mne材料可按79复用，不重读所有旧成果，不借主线/前任/另一对话已读代签。
3. 在156 MANIFEST内新增独立R155-01补记，精确引155固定MANIFEST上述selector、216原要求、本轮155独审与实际入口；保留原range和原true为历史原文。historical_full_read_before_155记NOT_ESTABLISHED_FROM_PUBLISHED_RECORD，不由本次补读或既有交付反推旧值。当前补记记录实际ref/blob、物理行/行尾、本人旧段复用依据、本次新读范围/完成日期与未知时刻、适用差异/冲突/缺项；当前全部真实读完才签current_refresh_completed=true。
4. 分列本次156 execution_handoff_receive与guidance_refresh：同一C11b/EXEC-26、任务/阶段/原分支PR/本批六件写区、Owner与00§19及218授权、当前胜任/容量定性判断、下一安全步、task_local_only/current_task_preserved；指定模型/可见身份与实际后台/effort未知分列。当前容量不是余量保证。检查点各历史段只核适用和后续决定，不接MA/Alaya/Mne自身业务或外仓写权。
5. 156 deferred写R155-01补记selector、历史缺口和本次实做出处，状态SUBMITTED_FOR_MAINLINE_REVIEW、PENDING_MAINLINE_CLOSURE，由C11下一次独审判闭。155原RETURN_DOCUMENTATION及历史缺口保留；作者不可自行签CLOSED，也不可把当前true倒填155。补读/补记实际完成后按已授权范围连续做A/B/C，不等Owner或主线另发开工。

这是接收记录补正，不新增源码引文配额；155的101/101/0与156新账分开。当前有效MA仍为48采用→47 v1.4-r10.266及45同版共用1.1-r9.260十五条；本轮§322为待议反馈而非新采用。若fresh发现新内容，按真实适用效力处理，不能用旧短号覆盖现状或把讨论自动当规则。

## 1. 为什么接这一段，以及旧范围怎样消费

本批沿已有标量写分析读取同一read_write.c内的kernel_write→__kernel_write→__kernel_write_iter，核三组有直接关系的消费：A相同方法表在vfs_write和kernel_write中实际选谁；B构造KVEC与模式门、方法调用前停止点的先后；C显式pos别名及零/排队哨兵数值返回怎样被kernel_write尾部消费。共A2/B3/C2七个独立局部场景。

kernel_write是此处实际存在的内核接口，不是本批新发现的用户系统调用或已证高频生产调用者。本批补已审标量写层的相邻内部接口，不声称用户write会改走kernel_write，也不虚构真实上层消费者。A仅从vfs_write局部入口比较，不从fd取得开始，不到ksys外层位置提交/归还，不重做fd/uaccess/系统调用ABI。B/C均从kernel_write局部入口开始。

历史精确消费：

- 115原facts（452444a1c4b4，blob2246b85a8792）的A.write_call_chain、A.write_VFS、A.iter_detail与九个A.models，以及164合同A，覆盖ksys_write→vfs_write→legacy或new_sync_write；R115-03约束同步助手的-EIOCBQUEUED哨兵。不能把本批A的VFS legacy优先、模式位和截断再报新根因。
- 155固定def3b82d9191、217合同的A_WRITE_ZERO/B_WRITE_NEG只走write=NULL的new_sync_write；C两例是kernel_read的显式别名。本批不改这些七例，不重算101窗；新增的是kernel_write自己的方法调用、前置初始化与返回消费，不能把旧零/负返回位置规律重新包装成已证新漏洞。
- 27及R27-S01/S03已说明kernel_read的KVEC、未存储方向、min截断/legacy原count和无ki_pos回写；本批复用对象概念，但从kernel_write实际源码另证其不同的正返回写回。
- 62及R62-S01的opaque回调字段副作用与返回/位置独立给定限制继续适用。05/08的代表VFS/用户stdio基线不构成本内部接口七场景的既有完整证明；这里只对已查相关范围定点排重，不宣称全仓穷尽检索。

旧条目及历史补正不改；deferred只登记新增接口组合观察或明确旧ID的evidence_append。没有从条件模型证明生产回调行为、实际IO、POSIX合规或运行错误。

## 2. 共用GIVEN与对象边界

### 2.1 数值、文件和长度

明确选定byte8、int/unsigned32、long/ulong/size_t/ssize_t/loff_t及指针64模型；所有出现的有符号值可表示，无并发及隐藏写者。F是有效活file，F.f_op是有效且保持的操作表；本批所列位置对象、helper栈对象及调用目标在实际需要时有效。F初始f_pos=10，mode及方法表保持。七例相互重置。

M=MAX_RW_COUNT=(INT_MAX & PAGE_MASK)=0x7ffff000=2147479552；N=M+0x20=0x7ffff020=2147479584，count一律取N。固定PAGE_SHIFT12、PAGE_SIZE4096，按选定数值模型推导M。活动min_t走__cmp三目宏，可重复求值选中操作数；本批count与M稳定且无副作用，不把它称为通用单次求值。B是仅传递的缓冲指针符号，所有回调GIVEN均不访问缓冲，不分配/复制2GB内容，不认证真实大缓冲或用户地址范围。

WRITE=0x2，CAN_WRITE=0x40000，二者都有为0x40002。本稿中的WRITE/CAN_WRITE简写只指FMODE_WRITE/FMODE_CAN_WRITE；kernel_write调用rw_verify_area的WRITE方向常量另按kernel.h的WRITE=1，不能混作同一个标志。

WARN_ON_ONCE采用固定文本CONFIG_BUG分支下的(condition)，因此B首模式失败按该文本得到-EBADF；实际预处理未知，不从宏名补出打印、计数或警告副作用。errno真实定义EBADF=9、EINVAL=22、EIOCBQUEUED=529分别回源。既有new_sync_write含BUG_ON，但本批A的VFS例选legacy，六个kernel_write例均不经过new_sync_write，不把其哨兵门迁入__kernel_write_iter。

### 2.2 位置、参数和历史快照

A与B有独立可读写位置对象Z，Z初始10，P=&Z，Z与F.f_pos不同且不别名。C明示P=&F.f_pos，不存在独立Z。P指针变量本身保持；*P随其所指对象变化。

K指__kernel_write_iter内kiocb.ki_pos；init_sync_kiocb按选定C初始化模型先令ki_filp=F、K=0，后续__kernel_write_iter自己的语句才令K=*P=10。K、iter与iov分别属于其helper栈；返回后只留最后一次有效观察的历史快照，不能在kernel_write返回后读取已失效局部对象。B两早退中kiocb虽已声明，未初始化；不得读K或填成0。KVEC/iov仍由更外层__kernel_write先构造。

构造出的选定活动字段：iov_base=B、iov_len=M；iter_type=ITER_KVEC、kvec=&iov、nr_segs=1、iov_offset=0、count=M。iov_iter_kvec收到ITER_SOURCE=1实参，但data_source成员及其初始化为注释；不虚构方向存储或iter消耗。只讨论指定活动成员，不给未选联合体视图补值。

rw_verify_area以实际首句return0推导，不把它当作GIVEN的完整权限/长度检查。A_VFS_BOTH的access_ok明确给定true且保持受跟踪状态，不展开其内部。kernel_write链没有该access_ok调用；这不等于任何指针安全证明。file_start_write/file_end_write是注释，不构造实际锁定/解锁效果。

### 2.3 两类回调GIVEN

WL为类型正确且正常返回的legacy write目标，只在A_VFS_BOTH执行；给定有序作用为：F.f_pos 10→40，随后*P（Z）10→30，返回2。缓冲不访问，F其它字段/方法表及P保持。

WI为类型正确的write_iter目标，按所列r正常返回；有序作用为：经ki_filp指向F，写F.f_pos 10→90；再写当前K 10→70；返回r。保持iter/iov所有所跟踪活动字段、F其它字段、P指针及缓冲内容。A_KERNEL_BOTH中Z不由该回调直接写；C中*P因别名F随第一步成为90，不另计一次独立写。

上述均为GIVEN_CALLBACK_EFFECT，不能列成仓库真实驱动语句或IO字节证据。返回值与给定位置写入独立，不以r计算位置增量。r=-529只是给定回调正常返回的数值；不模拟异步排队、completion、稍后再次访问kiocb或真实IO生命周期。

## A · 相同双方法表的两个接口（2例）

### A_VFS_BOTH

入口vfs_write(F,B,N,P)，F.f_mode=0x40002，F.f_op.write=WL、write_iter=WI同时有效，P=&Z。先模式门，再GIVEN access_ok，再实际rw_verify_area=0，count由N截为M；write非NULL，所以只调用WL(F,B,M,P)，不进入new_sync_write，不创建kiocb/iter，不检查-EIOCBQUEUED。

消费WL给定作用后，vfs_write正常局部返回2。终点F=40、Z=*P=30；K/iter/iov不属于本例。没有ksys外层再提交，不能把结果替换成F30；不执行fdput。这是旧VFS单点作为比较控制例，增量须与下面kernel接口联合表达。

### A_KERNEL_BOTH

入口kernel_write(F,B,N,P)，其余对象/双方法表与A_VFS_BOTH相同，WI r=3。rw_verify_area先返回0；__kernel_write构造长度M的iov及KVEC，再进入__kernel_write_iter，两模式门均通过，init_sync_kiocb后K从0读P成为10。

同时装有write与write_iter并未触发注释中的拒绝门；实际只调用WI(&kiocb,&iter)。WI收到count=M的KVEC，不收到N形式的legacy长度实参。回调后F=90、K=70、Z仍10；ret>0真、pos非NULL真，源码*pos=kiocb.ki_pos使Z10→70。__kernel_write_iter、__kernel_write、kernel_write正常局部返回3。

终点F90、Z=*P70；K70与KVEC/iov是返回前历史快照。WL没有执行。不能把注释里的“two methods unsupported”当活动校验，也不能从vfs_write的legacy优先迁移出kernel_write fallback。

## B · 初始化、模式门和缺失方法（3例）

三例均入口kernel_write(F,B,N,P)，P=&Z，F10/Z10，write=WL；下述选择的write_iter为NULL。只核实际门与到达次序，不执行NULL调用。

### B_NO_WRITE

F.f_mode=0x40000，仅CAN_WRITE。rw_verify_area=0；__kernel_write仍先构造长度M的iov和KVEC。进入__kernel_write_iter后，WARN_ON_ONCE(!(mode&WRITE))为真，按所选宏文本直接返回-EBADF=-9，后续正常透传至kernel_write局部返回-9。

第二模式门、init_sync_kiocb、*P读取和write_iter字段调用均未到达；F10、Z10。kiocb未初始化，K=NOT_INITIALIZED_AND_NOT_READ；这不是K等于0。保留iov/iter返回前历史快照，不能因最终权限失败说此前从未构造迭代器。

### B_NO_CAN_WRITE

F.f_mode=0x2，仅WRITE。同样先构造iov/KVEC；首门为假，第二门!(mode&CAN_WRITE)为真，返回-EINVAL=-22并正常透传。init_sync_kiocb及*P读取、方法调用未达；F10、Z10、K未初始化且未读。

### B_MISSING_ITER

F.f_mode=0x40002，两门都通过；write=WL有效但write_iter=NULL。构造iov/KVEC，初始化kiocb，K先0后由*P读为10。实际下一动作是read_write.c239的write_iter间接调用；注释中的方法完整性检查不会产生-EINVAL或legacy fallback。

**在该NULL间接调用动作之前停止。** 由明确给定且有效的方法表知道目标为NULL；不执行调用，不补回调返回、尾部*P赋值、任何helper正常返回或kernel_write结果。终点source_outcome=ANALYSIS_STOP_BEFORE_NULL_INDIRECT_CALL，kernel_write_return=NOT_DERIVED，F10、Z10、K10，调用栈和其对象在分析停止点仍有效。不要把停止点画成实际CPU返回、panic或已发生崩溃；所有ret>0/pos尾部门未到达。

## C · 显式位置别名及两种非正返回（2例）

两例入口kernel_write(F,B,N,P)，F.f_mode=0x40002，write=NULL、write_iter=WI有效，P=&F.f_pos，F初10。rw_verify_area、KVEC构造、模式门和K初始化均按A_KERNEL_BOTH到达，K调用前10；不存在Z或ksys局部副本。

### C_ALIAS_ZERO

WI给定先F10→90，再K10→70，返回0。__kernel_write_iter的ret>0为假，嵌套pos门和*pos赋值均不执行；没有ksys外层ret>=0提交。kernel_write正常局部返回0，F=*P=90，K最后历史快照70。只新增此kernel接口的消费，不重报155已证明的VFS/ksys零返回规律。

### C_ALIAS_QUEUED_VALUE

WI按同样有序GIVEN写F90、K70，正常返回-EIOCBQUEUED=-529。当前__kernel_write_iter无BUG_ON或其它该值门；ret>0为假，跳过pos门/赋值，最终kernel_write正常局部返回-529，F=*P=90、K历史70。

这里“正常局部返回-529”是指定源码/GIVEN模型的数值消费，不证明真实异步排队或支持同步接口接收排队任务，不外推用户errno或syscall返回。不能借new_sync_write的BUG_ON推定本例panic，也不能因为负返回就把F复原为10。没有给出后续回调使用kiocb的任何授权或模型。

## 3. 强制源码、结构化交付与边界

每个新正式窗从固定time自行定位，连续1–5物理行、真实定义/调用实例，LF/CRLF/NONE单列；注释用于证明活动缺席时按协议列普通位置。不得复用155的101窗或旧115/27窗充当本批新引文。

| 来源 | 必须核验的实际范围 |
|---|---|
| mykernel/fs/vfs/read_write.c | rw_verify_area49–72；new_sync_write202–217只作哨兵归属对照；__kernel_write_iter219–248、__kernel_write250–261、kernel_write271–285、vfs_write287–317。完整尾部证明所选位置门/正常返回；注释中的unsupported与file_start/end不能活动化。 |
| mykernel/include/linux/fs/fs.h | FMODE_WRITE/CAN_WRITE真实值；file位置/模式/方法字段；write/write_iter实际签名；kiocb实际两字段；init_sync_kiocb复合初始化与MAX_RW_COUNT。 |
| mykernel/include/linux/kernel/uio.h | kvec/iter活动成员与联合体、ITER_KVEC/ITER_SOURCE、iov_iter_kvec赋值，direction没有存进注释data_source。 |
| mykernel/include/linux/kernel/kernel.h | READ/WRITE常量，和FMODE位分开；kernel_write调用rw_verify_area的WRITE实参。 |
| mykernel/include/linux/kernel/minmax.h；limits.h；mykernel/arch/x86_64/mm/page/page_const_arch.h | min_t→__cmp活动链、INT_MAX/PAGE_MASK与M/N数值；重复求值在此稳定输入域不影响结果。 |
| mykernel/include/linux/lib/errno.h；mykernel/include/uapi/linux/errno.h；mykernel/include/uapi/asm-generic/errno.h及errno-base.h | -529、-9、-22的实际定义及包含导航；不以系统Linux常识补数值。 |
| mykernel/include/linux/kernel/asm-generic/bug.h；mykernel/include/linux/debug/bug.h；options_flags.cmake等实际配置入口 | WARN_ON_ONCE所选条件分支、new_sync_write所引用BUG_ON的归属对照，配置文本不等于预处理认证。 |
| mykernel/include/linux/kernel/types.h；mykernel/include/uapi/linux/posix_types.h；mykernel/include/uapi/asm-generic/posix_types.h | 本批size_t/ssize_t/loff_t等类型链，所选64/32位模型和实际ABI未证分列。 |
| mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c | 双canary与规范核对；直接master反验、公约及具体G00要求由主线管理段给出。 |

facts至少包含historical_scope、numerical_model、object_identity_and_aliases、given_callbacks、seven_scenarios、actual_call_arguments、initialization_and_mode_order、source_writes/given_writes、actual_reads/not_reached、terminal_boundaries、source_index/required_anchor_coverage、graph。A/B共享基态可以用明确delta重建，不反复复制大状态；错误码/停止/未知不同字段，不用统一数字代替NOT_DERIVED。

正文、facts与DOT同一场景/语义；DOT全部节点/边属性与facts.graph一致。图允许共享源码骨架，无须按每个字段造巨图。GIVEN副作用、SOURCE控制/调用、历史比较三类边分开；B_MISSING_ITER分析停止无执行后继；各正常返回终点只到所选局部接口，不接用户态。A_VFS_BOTH不虚构kernel层局部对象；两早退不把未初始化K作为可读值。

deferred只收实际新增限定观察、旧事实追加及主线管理段指定D00；不冒称实际驱动违反规则、已发生NULL崩溃、数据损坏或新漏洞。既有问题先入库，修复和真实运行不属于本批。可自写有限纯文本/整数模型；不执行仓库程序、预处理、构建、链接、Graphviz、QEMU或真实IO。

正式六件按页首；执行者完成唯一提交/作者回执与实际G04后STOP_WRITING，交C11 Pro独审及同轮后继，不进入157。R155-01按本合同D00段补正，作者不自行判闭。


## 4. 本轮入口、规范复用和权限

以本轮PR17唯一主线正式评论为真实入口，取实际ID及最终固定lead提交，完整读本合同、同提交[155独审](reviews/CORE-USER-RW-POSITION-CONSUMPTION-155-review.md)及[检查点](checkpoints/2026-11-26-rw155-reviewed-kernel-write156.md)。216恢复包及完整启动词继续作身份/读序依据，R155-01按上述D00补足。新任务不继承155控制栈，也不需要新开执行对话。

保持MYOS2-LEAD-002-CORE-CHECK-01、阶段2、C11b／EXEC-26、claude/dazzling-cori-q0dnyt及原Draft PR17；唯一写区为页首user-kernel-write-consumption-156六件。无源码、旧件、主线文件、PR元数据或外仓写权。Owner当前同轮后继授权与00§19/本合同共同成立，不重演早期建分支/阶段0等待。

按79只复用**本人同一C11b会话**实际完整读过且未变的十五项目规则、有效MA全文及45十五条、Mne任务局部接收/命名；R155检查点的不完整首次读取没有复用例外，须按D00真实补全。本轮fresh MA master b234c6c91a2e、建设06b783861368，入口881fd8c1076e、检查点54ac6e5fc66c；Mne master e581eb9df6c4。新适用差异真实读取并留证；待议§322不自动改变有效采用。

保留18结果先行/问题先入库、24职责、28同轮后继、63先核效果再限次重试、124次一档执行/Pro验收、198仓库正文和单一短入口、210的8/10提醒。未知后台型号不冒签，也不因此重复索取Owner许可；真正权限/材料/容量阻断才保全并报告最小缺口。

## 5. fresh G00与旧成果冻结

1. 直接lead等于本轮评论真实发布提交，exec=def3b82d9191，time=a039d9803ade，直接master=e8e60cd19540；不以PR.base_sha代替master。原PR16 Open/Ready、PR17原标题Open/Draft，原head/base master且未合并；开放PR无since完整到空。
2. PR17顶层评论无since全分页到空，满100必须继续。检查点61历史标记加本轮独审155/发令156两枚共63，逐条字面恰一次、实际ID正确，不靠总数。当前入口、155作者7195286787、前次发令7194265302各直接ID双读并与分页同文；准备时305评论，加本轮入口预计306，以fresh实际页数为准。
3. 检查点所列十枚错误作者负标记均0：旧EXEC-23/149、EXEC-22/145、EXEC-24/152/153/154/155/156、EXEC-25/155/156、EXEC-26/154。本批EXEC-26/156入场0，EXEC-26/155与EXEC-25/152/153/154各1；核任意作者的156占用和整个结果目录，未知并行写不覆盖。
4. 155固定def3b82d9191、活动exec及实际API Git Blob六件全文一致，17341 LF/541883字节；唯一父b57bd74931dc，一提交六新增零其它改删，原2253非tree对象保持。只核冻结与本轮已审选择器，不重算162阶段或101窗；旧成果/评论/作者及未证历史不改。
5. 两time canary、直接master反验和公约原句实际核，保留read_channel与self_check。本次真实权限/材料/胜任/容量、规范刷新和R155补正明确；只有实际完成的读取才记完成。G00进入后先D00实际补齐，再连续A/B/C；不得把主线尚未判闭写成作者已关闭。

## 6. 六件、证据、唯一提交与发表后G04

恰六新增：MANIFEST.md、kernel-write-consumption.md、facts-and-boundaries.yaml、kernel-write-consumption-paths.dot、deferred-findings.yaml、evidence.md。R155-01补记与接收/刷新放MANIFEST，处置索引放deferred；不另发接收评论或第七件。保留read_channel、startup_selfcheck_quote、branch_canary_quotes及self_check三整数。允许共享基态和有序差量，明确selector并能完整重建，避免把同一大状态重复铺进每阶段；不以简写省掉关键未初始化/未达/历史对象边界。

正式新证据每窗1–5连续物理行，按固定实际源的原字节、CRLF/LF/NONE、定义/调用/命令实例重开核，相邻断言由相应窗支持；P6 n=m+k。位置性证据与正式窗分账，155旧101及主线0不并入新账。连接器可达P9第1–8、11、13实做，9/10无代码变更、12非降级可NA；完整本地P9未做即NOT_RUN。强制锚不能用降级逃避，不设字数/窗口/字段/新问题数量配额。

写前fresh四refs、原PR状态、作者/目录未占用及未知并行写。只以真实def3b82d9191为唯一父，expected_sha匹配、force=false，原exec一次提交六新增零其它改删。先回读六件固定提交/活动exec/API Git Blob全文、LF/UTF-8字节、唯一父、compare净差异和全部旧对象冻结，再发唯一作者正式评论。

唯一作者标记：

MYOS2-LEAD-002-CHAT-EXEC-26 / CORE-USER-KERNEL-WRITE-CONSUMPTION-156 / FORMAL_DELIVERY

正式作者列真实固定合同/提交/六件、R155-01已实际补正及PENDING_MAINLINE_CLOSURE、七场景/关键停止边界、Owner指定/可见身份/未知后台档位、实际P6与自检。发表时actual_G04=PENDING_POST_PUBLICATION、mainline_acceptance=PENDING；不可先签主线验收、R155闭合或发表后终检。

发表后实际核新评论ID两次并与全分页全文一致；全评论到空，63原标记加唯一作者共64，逐条字面一次/实际ID正确，十历史负标记0。核四refs（exec为真实156提交）、开放PR到空、两原PR状态/原标题/元数据、六件三渠道全文、唯一父/一提交六新增零其它改删及全部旧成果/评论冻结。通过后才报READY_FOR_MAINLINE_REVIEW、实际G04=PASS、STOP_WRITING，不启动157。

GitHub暂态按63：先核远端效果，避免重复写入，再通常首次加三次、10/20/40秒或实质工作间隔限次重试。expected-head/force=false保持；参数校验未发出、工具上限恢复、真正写重试与读回暂态分账，如实write_retry_log，不把单次错误当权限丧失。旧评论/文件不为补正而覆盖。

按198最终只给一个通常3–6行原生短复制块和一个真实作者评论URL，报156六件/唯一提交/P6、R155补正已提交待主线判闭、实际G04/停写及必要未知；交C11 Pro独审并同轮后继。正文/证据/详细处置留GitHub，不要求Owner拼接长报告或逐项裁决普通技术问题。

## 7. 技术合同就绪与计数

156由设计分工提出，另一未参与设计的技术分工先读20份固定源码、62处精确源锚及实际errno包含链，按明确GIVEN独建七场景模型后比较候选；133项检查、0失败，READY，未读或执行设计者model156。已核双方法分派、KVEC先构造后模式拒绝、NULL调用前停止、正返回位置写回及本接口无-529 BUG_ON；这只证明合同就绪，不预签156未来交付、R155补正闭合或真实G04。

该就绪审只核本合同GIVEN/源码/预期和边界，不代替156未来作者交付、R155-01补正核收或实际G04。A_VFS_BOTH只有vfs局部返回；六个kernel场景没有用户syscall认证；B_MISSING_ITER只停NULL调用前，C的-529只按给定正常数值返回消费，不推定真实异步排队或崩溃。

C11本次成功首次正式发布156后计4→5，8/10均未达；155验收与补正登记不另计，C10历史11不变。未来交接带十五项目规则、有效MA全文/同版共用全文、Mnemosyne命名依据、实际断点/身份/计数及口述约定。C11b当前会话继续，不能据一次交付保证未来容量。

阶段2。禁止源码/旧件/主线/PR元数据改动、新分支/PR、合并、强推、删除及跨仓写；仅自写有限纯文本/整数辅助。完整本地P9、项目程序/脚本、预处理、编译链接、Graphviz、QEMU、实机和真实IO NOT_RUN。真实C/ABI、配置、生产绑定/调用者、数据/映射/生命周期及实际后台模型和Extra未独立认证。没有聊天UI或自动选模操作。
