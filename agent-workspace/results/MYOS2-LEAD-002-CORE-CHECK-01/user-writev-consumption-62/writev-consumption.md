---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-WRITEV-CONSUMPTION-62
record_type: bounded_static_vector_write_consumer_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
executor_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 7f37c7b1e445
fixed_contract92_short12: 8647a2225055
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
runtime_evidence: NOT_RUN
---

# 第62批：writev向量来源、逐段累计和位置/FAT副作用

本文仅审固定time源码。必须区分回调被请求调用、回调正常返回的数值、真实已复制字节、传入的局部position变化、inode/FAT/ROOTBLK副作用请求以及最终file.f_pos写回；不能用其中一个替代另一个。任何非法指针、数组、整数、对象寿命或不返回路径，都在其首次失去合法前提处停止。正文所有有限序列是假设不透明回调正常返回的自建模型，并非运行用户writev、FAT块设备或旧仓库脚本。直接源锚在evidence.md，结构化事实在facts-and-boundaries.yaml。

## G00与历史限定

本轮完整读取92号合同、61主线审查回执R61-S01/S02/S03、63/79规则。61为PASS_PENDING_LOCAL，无D00。本批精确消费：R61-S01把61旧DC61-F05所谓物理fd边界改为fd_count逻辑门，不可混称；R61-S02中fput对file.f_count执行递减，而get_file才是增加；R61-S03补US-02/03、UB-10给61原发现，不能借本批重编号。58 B25原证据有效且审查误判正式撤回（R58C-S01），56–60裁定不再重新验收。

强制历史实际合读：41回执R41-S01/S02、55回执R55-S01/S02、56回执R56-S01/S02、08回执SR08-01/02及09后继闭合状态。07::UB-01与08::US-01、08向UB-01的evidence_appends、55::BI55-F01/F02、56::AG56-F01逐项从原deferred定位；前二条status=deferred_owner_not_ready，BI55字段为disposition=source_only_conditional，AG56字段为status=source_conditional_only。旧source_field/source_value与qualified_id不改，新增仅消费者关系。本批无完整旧问题库去重。

G00 direct time=a039d9803ade、master=e8e60cd19540、lead/PR16=8647a2225055、exec/PR17=7f37c7b1e445；两PR均Open、未合并、17 Draft，已知开放PR仅16/17，新62目录首次404，双金丝雀匹配。提交前重新核HEAD与写区。不跨仓、不建分支/PR、不改旧评论或PR元数据。

## A / G01：系统调用入口、iovec生产和准入对照

### A1. fdget_pos与有效用户向量是不同前提

syscall.c唯一writev宏调用体参数为ulong fd、指向iov_s的vec、ulong vlen，执行do_writev(fd,vec,vlen,0)。do_writev形参fd仍是ulong，交给fdget_pos(int)时可能出现超int范围的实现定义转换，接着fdget_pos把int传给__to_fd(ulong)，直接索引current->files->fd_array[v]并返回flags3。这个选定借用过程没有本地逻辑fd_count或物理256槽边界门，也没有file.f_count增加。仅在下标合法且槽确实为NULL时，后续do_writev默认-EBADF且跳过fdput_pos构成正常分支。非法下标、current/files无效或非NULL非法file须在更早对象读取处停止，不能用后置if(f.file)补出安全EBADF。引用失配属于旧US-01，物理边界属旧UV-11；本批不重新立项。证据A01–A07/A18–A25、R41-S01/S02。

uapi/linux/uio.h定义每个iov_s的iov_base地址和iov_len长度；UIO_FASTIOV=8、UIO_MAXIOV=1024真实存在。vfs_writev虽声明iovstack、iov、iter、tot_len，实际import_iovec、iov_iter_count、总长预检、rw_verify_area、write_iter选择与释放都在注释中。活动体仅检查FMODE_WRITE，然后按int i对ulong vlen循环，直接将vec[i].iov_base和iov_len作为file->f_op->write的实参。既未复制用户iovec数组本体，也没有活动检查vec地址、元素数或base可访问性。iov_base与iov_len在同一调用表达式中被求值，不能把其中一个声明成普适先读。无效vec/元素时报告最早非法字段或函数指针求值，不虚构EFAULT或后续fdput。证据A08–A17、B01–B12；WV62-F01。

有界分支区分：
- vlen0且有效可写file，循环0次，vec=NULL也不读vec[0]或file.f_op->write；仍经过fd获取、file_ppos、模式门，并在有效正常路径执行fdput_pos。
- vlen1而vec=NULL，合法对象前缀结束后首次取vec[0]字段便没有可靠内存前提，不补成功回调或errno。
- vlen2但仅第一条iovec可读，第一回调正常返回才会试读第二条；第二条非法则不能推完整累计/外层位置提交。
- iov_base=NULL而iovec本体合法，代码仍传给不透明write方法，方法是否拒绝及复制行为不由vfs_writev保证。
- vlen1而iov_len0，仍请求一次write调用；不同于vlen本身为0。
- file.f_op非NULL但write=NULL、write_iter存在，vlen>=1时仍尝试无效write调用，不能借标量的write_iter回退补安全；vlen0时不读取该方法。

### A2. 与scalar的活动门分别成立

| 条件/操作 | 标量vfs_write | 向量vfs_writev |
|---|---|---|
| FMODE_WRITE | 活动门，未置位-EBADF | 活动门，未置位-EBADF |
| FMODE_CAN_WRITE | 活动门，未置位-EINVAL | 对应语句注释 |
| access_ok | 活动地检查标量buf/count | 未调用对vec或逐项iov_base的总检 |
| rw_verify_area | 调用，所读callee活动首句return0 | 调用语句注释 |
| MAX_RW_COUNT | 对标量count裁剪 | 无活动向量总长门 |
| write方法 | 先write，缺席时可选write_iter | 仅逐项直呼write |

07::UB-01已经记录FMODE_CAN_WRITE的scalar/vector不对称；本轮增量是确切用户向量访问及回调选择消费者，不重复命名旧根因。模式位实际由哪个open生产、完整安全权限、真实用户地址均未证。证据A26–A36、B20；WV62-F02。

## B / G02：逐段返回与有符号累计

vfs_writev中ssize_t ret初始0，活动for体只有ret += file->f_op->write(file,vec[i].iov_base,vec[i].iov_len,pos)。该循环不按负返回、零返回、短写break，也不核单项返回是否超过请求长度；每项继续仍要求前面数组字段可读取、write方法有效且回调正常返回。已有08::evidence_appends[target UB-01]记录直接求和事实，本批补充序列及位置消费者，不重报旧发现。证据B01–B11。

| 纯抽象回调结果 | 算术总ret | 被调用控制流的限定结论 |
|---|---:|---|
| [] | 0 | 无底层write回调 |
| [3,0] | 3 | 第二次返回0仍完成逐段循环 |
| [3,-5] | -2 | 第二次错误不回滚第一段请求 |
| [8,-5] | 3 | 总正不证明第二段成功 |
| [5,-5] | 0 | 总零不表示未写任何一段 |
| [-5,8] | 3 | 负返回后仍请求下一段 |
| [-5,-2] | -7 | 两负值照样相加 |
| [2,-5,7] | 4 | 错误后仍能出现最终正总值 |
| 请求长度[8,5]，返回[3,5] | 8 | 第一段短写后仍尝试下一段 |

各项只是可反驳的正常返回整数模型，不能说明真实FAT/TTY会产生这些输入，也不表示数值等于真实复制量或持久写盘量。若方法不返回、非法向量或第一次符号溢出，必须在那里停止；不能为凑模型强行得到后续总和。vfs_writev的int i与ulong vlen比较还存在范围差异：在明确LP64/int32模型中，若假设超巨量iovec均可访问且前面回调正常，i加到INT_MAX之后再递增不能继续按有定义的有符号运算推演；真实数组越界可能先发生。同理ssize_t累计超过其可表示范围要在该次加法停止，不能用JS无限精度延伸。rwf_t flags形参当前未被活动循环读取，带flags的iter代码是注释。证据B01–B20；WV62-F03/F08。

## C / G03：局部position、符号提交及FAT32消费

### C1. 三种位置状态

do_writev对有效file先调用file_ppos。FMODE_STREAM时得到NULL；非流时得到&file.f_pos、再把原值复制到局部loff_t pos，后续ppos指向局部pos。各段回调共享该指针；整个vfs_writev正常返回后，只有**累计ret>=0且ppos非NULL**，do_writev才执行file.f_pos=pos，然后无论结果符号在正常退出中均请求fdput_pos，最后返回ret。fdput_pos调用fdput，flags3的FDPUT_FPUT使fput递减file.f_count；这是旧US-01，不可按fdget_pos的名称想象相应增引用或把物理释放当已完成。证据C01–C10、A18–A23及R61-S02。

假设非流文件初始f_pos10，所有回调与局部position合法且无并发/直接file.f_pos改写：
- 回调返回[3,-5]，各自局部位置增量[3,0]，累计-2；本层不写回局部13，但第一段数据或metadata请求不因此撤销。
- 返回[5,-5]、局部增量[5,0]，累计0；本层反而写回15，虽然第二回调负返回。
- 返回[8,-5]、增量[8,0]，累计3；本层提交18，不保证实际复制字节正好3。
- 不透明回调也可能直接修改file.f_pos，故本层缺少写回不自动等于整个系统位置未改变；当前所读FAT写主要更新传入position指针。

若FMODE_STREAM有效，ppos为NULL：vlen0不执行write回调，外层也不提交位置；vlen>=1且选中FAT32_write时，该方法先读取filp对象的inode/private字段，再用*position作除法与取余，NULL position在这里失去合法性。其它可能允许NULL位置的write不能用来说明FAT安全；对象前缀坏则停止更早。证据C01/C03–C06/C15–C18；WV62-F04/F05。

### C2. FAT32_write的零count仍能执行一次do循环

FAT32_file_ops真实绑定write=FAT32_write，但任何具体file是否绑定该操作表需独立成立。FAT32_write先读取FAT inode/private_sbi、first_cluster、初始化sector=0与flags=0、读取*position计算int index及long offset，再分配cluster-sized buffer。position、inode、缓冲、bytes_per_cluster非0与整数转换是前置门；若早期NULL/除0/分配失败导致无效访问，不得补后续安全错误。

初始cluster0时，正常候选查找可用簇并设置flags1；若找到有效簇则在数据循环前赋first_cluster，调用write_inode和FAT32_write_FAT_Entry。之后index=count发生size_t到int转换，并进入do...while(index)，所以在正常index0模型中**count0仍尝试执行一次循环**。已有簇flags0可以先ROOTBLK READ、length0复制、再ROOTBLK WRITE；新簇flags1跳过sector计算及READ，却可能把初始sector0送入WRITE，且之前已经发出metadata/FAT请求。结果0不是无副作用证明；这属于R55-S01和BI55-F02既有内核来源在writev零长度段上的新增消费者，不重新编号。vlen0没有回调，与有一项iov_len0不能混为一谈。证据C14–C27/C39；WV62-F06。

### C3. 复制余数、块错误及大小更新

每轮FAT写将copy_from_user的返回保存在cplen，但所选方法没有用它决定是否继续ROOTBLK WRITE。块WRITE非零时局部ret_val=-EIO且break，该轮尚未执行index-=length、buf+=length、offset清零或*position+=length；若某轮WRITE正常返回则可先执行这四个推进步骤。普通后继簇未把next_cluster赋回cluster的问题沿AG56-F01；flags1扩簇不重算sector的边界沿BI55-F02；不在本批再查完整链/驱动。

即使后续某轮失败，循环之后仍先检查*position>inode.i_size并可能更新i_size、请求write_inode。到最后若index==0，又会用ret_val=count覆盖此前可能的局部-EIO；count0分支尤其不能简单说“块错误最终必-EIO”。在第一轮失败且index>0时也不能凭后续大小检查想象该轮已经推进位置。物理数据/metadata真实效果和copy余数的含义仍是opaque/NOT_RUN；R55-S01/S02、R56-S01/S02必须继续合读。证据C28–C42；WV62-F07。

### C4. 消费矩阵

| 有效模型分支 | 汇总结果 | 外层file.f_pos与副作用区别 |
|---|---:|---|
| vlen0，非流可写file | 0 | 原位置按值再次写回；仍可调用fdput |
| [3,0]且局部增量[3,0] | 3 | 写回原位置+3 |
| [3,-5]且局部增量[3,0] | -2 | 本层不提交；不回滚前段请求 |
| [5,-5]且局部增量[5,0] | 0 | 本层写回原位置+5 |
| [-5,8]且局部增量[0,8] | 3 | 本层写回原位置+8 |
| stream加一项且绑定FAT | 无合法汇总 | 在FAT的*position处须停止 |
| FAT段长度0且选定几何有效 | 回调可报0 | 仍可能有块I/O和元数据请求 |
| 无效vec、缺write或符号溢出 | 无认证汇总 | 不补后续fdput、错误或介质状态 |

源级的对象数值与介质副作用不等于规范验证。FAT目录枚举第59批使用独立ctx.pos==-1哨兵/游标，不能套用writev中局部position提交规则。数组及用户地址的真实映射、file_op选择、SMP、fd引用/释放及FAT块设备均未运行。

## 独立验收入口与NOT_RUN

A/B/C均completed_in_scope；WV62-F01–F08为8条有前提的新增消费者/扩展候选（F03/F04/F06/F07明确联系旧UB-01、US-01、BI55与AG56），WV62-U01–U09为9项证据缺口。没有将原问题同义重号或向Owner提出即时修复要求。

六件中本正文配有固定time源码短锚98段（A36/B20/C42）、11条源码ledger、63个自建有限数学/状态模型和独立DOT；证据切片数量不等于独立缺陷数量。完整本地P9、全树校验、仓库C/ASM/脚本/旧夹具、预处理/编译/链接、Graphviz、QEMU/硬件、真实用户writev/目录/磁盘、POSIX及引用/并发测试均NOT_RUN。执行者仅READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，交MYOS2-A-C05独立接收（最高PASS_PENDING_LOCAL），不自签通过、不自行启动63。
