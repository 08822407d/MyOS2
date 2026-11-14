---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FD-CONTROL-61-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: unknown
model_per_owner: gpt pro（沿用Owner最近明确报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
contract91_short12: 4de8f7b487d3
reviewed_base_short12: 1a6ef1c7a5aa
reviewed_head_short12: 7f37c7b1e445
reviewed_delivery_comment: 6722542260
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R61-S01, R61-S02, R61-S03]
subjects: {A: ACCEPTED_IN_SCOPE_WITH_LIMITS, B: ACCEPTED_IN_SCOPE_WITH_LIMITS, C: ACCEPTED_IN_SCOPE_WITH_LIMITS}
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original97_automatic_byte_definition_scan: NOT_RUN
original_facts_deferred_JSON_parse: NOT_RUN
reviewer_finite_model_cases: 78
graph_blob_short12: 3c2b0c518f48
graph_bytes: 6306
graph_nodes: 68
graph_edges: 79
graph_undefined_endpoints: 0
graph_text_blob_match: true
self_check: {verified_claims: 3, quotes_reconfirmed: 3, downgraded_to_inferred: 0, scope: 本回执三段定义内短引文人工回源，非原97条全量机械重放}
next_followup: CORE-USER-WRITEV-CONSUMPTION-62
next_task_ref: ../92-user-writev-consumption-contract.md
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
phase: 2
owner_decisions_requested_now: []
---

# 第61批独立验收：fcntl分派、fd别名和文件定位

## 1. 裁定及远端交付

依据91号合同、61固定六件与正式评论，裁定PASS_PENDING_LOCAL，A/B/C限定接收，returned_scope=[]。原成果须合读R61-S01/S02/S03；本回执直接纠正逻辑范围措辞、图的计数方向和遗漏的旧根因关联，不另派微型返工包，不把原图和七条记录无条件签成完全准确或全新独立缺陷。
compare 1a6ef1c7a5aa→7f37c7b1e445：ahead1/behind0，一个提交，严格仅user-fd-control-61/六件A新增3233行，分别MANIFEST81、正文105、facts1548、evidence971、DOT161、deferred367；旧件零净改删，无内核或越界写区。
PR16/直接lead为4de8f7b487d3，PR17/直接执行为7f37c7b1e445；两PR均Open/unmerged，17 Draft，已知两项开放且第三项分页为空。direct time=a039d9803ade、master=e8e60cd19540；PR返回的base_sha不代替direct master。
交付评论6722542260申报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及STOP_WRITING。本次核固定工件，不认证另一对话的隐藏模型、所有历史读取和作者机械自检过程。56–60、57/58闭合及58 B25主线误判撤回保持。

## 2. 核心得到固定源码支持

A：FMODE_PATH的准入名单与do_fcntl活动实现不同。GETFD/SETFD通过前者，但对应case在后者为注释；正常default保留-EINVAL。非白名单PATH命令在外层保留-EBADF，但正常out1仍fdput。F_GETFL来自file状态字段：
[VERIFIED mykernel/fs/vfs/fcntl.c::do_fcntl]
```c
	case F_GETFL:
		err = filp->f_flags;
		break;
```
fdget_raw直接借槽并返回flags3，不增加file计数；安全NULL槽早退不fdput，非法索引或非NULL非法对象则不能借后置判空获得安全errno。do_fcntl的argi转换即使随后不使用仍是源码表达式，不认证所有目标实现的越界转换值。

B：f_dupfd的合法成功分支确实先增加引用再安装同一file指针：
[VERIFIED mykernel/fs/vfs/file.c::f_dupfd]
```c
	if (err >= 0) {
		get_file(file);
		fd_install(err, file);
	}
```
正常无其它写者时，外层fcntl的额外fdput使n→n+1→n，但新槽仍成为该file的别名。失败早退没有get_file而外层仍可能fput，须分开。O_CLOEXEC实参没有在所读alloc/install链中变成每fd存储。dup/dup2活动宏体没有复制或返回语句；实际ALERT宏只含条件打印，不凭注释补出功能或确定返回值。

C：lseek先以fd_count作逻辑门，再以SEEK_MAX=4拒绝whence，随后才读槽及方法。有效file缺方法时返回初始0，不代表定位到了0。FAT方法的局部门为：
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_lseek]
```c
	if(pos < 0 || pos > filp->f_path.dentry->d_inode->i_size)
		return -EOVERFLOW;

	filp->f_pos = pos;
```
SET/CUR/END在合法算术域内计算pos；DATA=3可过外层但FAT默认拒绝，HOLE=4由外层拒绝。CUR/END加法先于范围检查，溢出不能由后置EOVERFLOW补救。负回调结果经过ulong再由long返回的转换，与编译后的用户ABI结论分开。

## 3. R61-S01：数值、对象和副作用的强制限定

1. DC61-F05的“物理索引范围比较”更正为“以fd_count为上界的逻辑比较”；固定数组容量仍是独立的NR_OPEN_DEFAULT=256。正文和facts已作此区分，本补充覆盖该候选措辞，不新增或假设物理边界检查。
2. lseek门有先后：fd逻辑拒绝在whence拒绝之前，两者又在实际槽读取之前。即使fd超过物理容量，若逻辑门放行而whence=4，仍可先在whence门返回；只有实际到达非法下标读取才停止。合法NULL槽的fcntl早退与lseek的NULL成员访问不是同一路径。
3. alloc_fd的start→int初始化、int i与uint end比较、ulong nofile→uint end、uint fd→int return分别保留。空/满区间不能补EMFILE；end>256不排除表内提前命中。按所选低32位模型，nofile=2^32可变成end0，但这不是实测rlimit或ABI。
4. BUG_ON不是容量门；所选宏若求值condition，读fps[fd]先要求合法索引，若不求值则不能把该读取硬说成最早访问。后继slot存储仍需合法对象。屏障、锁调用、找到空槽、预留和安装是不同事件。
5. 引用数事件模型仅限同一有效file、可表示正计数、正常调用及无其它写者。计数归零只确认释放请求，不能继续证明物理对象仍存活或已发生UAF；也不能把f_dupfd单独的+1抹去。原槽未由此链清除是局部源码事实，不是对任何失效之后机器状态的保证。
6. “dup_fd深复制files表”只可理解为所选代码新分配表和各非空槽file对象并按值复制成员；不是深复制f_path、映射或mount/dentry，也不是引用所有权自然配平。沿R43-S01/S02消费。
7. i_size与f_pos所选字段为loff_t，pos为long，off_t沿__kernel_off_t/__kernel_long_t；generic typedef受条件选择。这里的LP64、int32及二补数回转只是显式模型，未预处理认证所有绑定。转换链本身不是已证漏洞；DC61-F07继续needs_evidence。
8. 不带回调的方法缺席路径、FAT失败不写f_pos路径和成功写入路径分开；两个槽共用位置需要实际同一且仍有效的file。初始无效对象、回调不返回或先行非法运算之后，不补安全返回、清理或已观测到的副作用。

## 4. R61-S02：图的计数方向及证据声明

原DOT边fput→inc指向标签“file.f_count +1”的节点，不能按该标签解释成fput加引用。正确消费为fput→file.f_count减1；get_file→加1保留。原图冻结，本回执给出必须合读的语义纠正；这不是原图已经有一个独立减计数节点。
CALL边是潜在调用关系，不是无条件轨迹：fcntl→whitelist仅FMODE_PATH；fcntl→fdput仅对应有效非NULL、正常到达out1的路径；f_dupfd→get_file/fd_install须合法非负返回；lseek→FAT须方法真实绑定。inactive是注释信息；noreturn节点意为没有活动返回值来源，不是__noreturn属性或一定不返回；dir59→alias只是比较关系，不是59制造了fd别名。ANALYSIS_STOP不是CPU指令或源码errno。
DOT全文转录重建6306字节，完整Git blob身份与远端3c2b0c518f48一致；68唯一节点79边0未定义端点。结构匹配不豁免上述语义纠正，Graphviz未运行。
97标签全文阅读，关键签名、命令、宏、类型和强制组按原始源码核查；作者97/97、17源项、63个JS模型仍按作者口径记录，本主线未全量自动扫描97段的字节/定义归属，也未机械解析原facts/deferred全文。
主线独立78例自建Python模型通过：命令17、arg转换10、扫描11、limit与扫描组合4、引用8、FAT定位15、入口门顺序8、返回转换5。不是作者63例重放。完整P9、Graphviz、原97全量机械闸门、仓库C/ASM/脚本/旧夹具、预处理/编译/链接、QEMU/硬件及真实用户fd均NOT_RUN。

## 5. R61-S03：补齐旧根因关联，不重编号或改旧状态

本轮直接回读旧原条目发现下列关联应补入组合消费；61原record ID及needs_evidence状态保留为局部细化记录，不删除、不覆盖、不等同新根因。
- DC61-F01的GETFD/SETFD活动case缺席、DC61-F03的CLOEXEC标志未保存，均关联CORE-USER-SYSCALL-STDIO-08::US-03，原source_field=status、source_value=deferred_owner_not_ready。
- f_dupfd的rlimit与物理表长差异还关联CORE-USER-SYSCALL-STDIO-08::US-02，原status=deferred_owner_not_ready；不能只列UV-11后就将该具体消费者算成新发现。
- DC61-F04的dup/dup2空壳与无返回语句关联CORE-USER-BOUNDARIES-07::UB-10，原status=deferred_owner_not_ready。
- DC61-F02沿08::US-01的成功复制消费者关系保持；UV-09/10/11、FG41、CL42、FT43、DE59既有状态不改变。

US-03旧影响栏对“所有fd跨exec保留”的无条件概括，不由当前局部读取重新背书；UB-10也不据旧影响栏认证编译后确定返回或用户实际调用。保留旧ID/状态，不等于照单采纳其全部历史措辞。本轮只是定点去重，不声称穷尽历史。七条DC61条件记录不能称为七个独立新漏洞；九项缺口仍在。后继只需准确消费，不要求另交61勘误包。

## 6. 实际输入、复用及后继

六件及评论全文读取：正文1–65/66–105；facts1–360、351–650、651–950、951–1250、1251–1548（重叠补尾）；evidence1–330/331–660/661–971；deferred1–195/196–367；MANIFEST和DOT全文。
本轮新回源：syscalls.h1380至末尾、syscall_wrapper.h229至末尾、file_table.c100至末尾、fs.h500–680及70–110、fdtable.h1–85、task.h85–132、types.h1–115、posix_types.h1–105、asm-generic/fcntl.h55–193、linux/fcntl.h1–25、resource.h24–38、linux/fs.h37–53、myos_fat32.c278–367。
syscall.c选定完整入口、fcntl.c全文、file.c的alloc/install/f_dupfd、file.h与get_file同pin原文沿本有效C05已读材料复用；91及60回执、41全部限定和双金丝雀亦沿同pin已读原文使用，不以作者引文代替原始文件。40/42/43回执相关全部S01/S02本轮定点重读；旧08 US-01/02/03、07 UB-10从原deferred重新定位，08回执仅新读前100行，其余限定沿已有记录，不声称本轮全文重读。
28/79和协议v2本轮完整再读；九项目约束、63及适用Mnemosyne按同会话此前实际已读固定范围复用，未全量刷新指导或导入其它项目维护。
后继92号组合writev向量来源/准入、回调累计与部分失败、位置提交及限定FAT消费者三个专题。已预读read_write.c175至末尾、FAT32_write完整体和uapi/linux/uio.h；只作划界，不代执行62或自动跨对话投递。
