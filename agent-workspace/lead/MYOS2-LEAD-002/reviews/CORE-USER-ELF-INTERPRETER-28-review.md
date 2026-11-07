---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ELF-INTERPRETER-28-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
evidence_class: 固定交付全文阅读、关键源码独立语义复核和GitHub增量核对；不是运行或全量机械认证
authorization_ref: Owner报告28在427604c818a5交付并要求按48号独立验收；28号同轮验收及后继安排持续约定
reviewed_pr: 17
reviewed_head_short12: 427604c818a5
reviewed_base_short12: f98ee06bbce8
contract_short12: 5e5fb5e4e460
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-03
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: ec208b47a2e4
master_ref_observed_short12: e8e60cd19540
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R28-S01, R28-S02, R28-S03]
acceptance_scope: PT_INTERP取得、解释器校验/偏移/入口和局部清理的条件性静态成果；原件须合读本回执
kernel_interpreter_or_pathname_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_89_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-PATHNAME-COPY-29
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/49-user-pathname-copy-contract.md
inputs_read:
  - PR17最新交付评论issuecomment-6576578349、两PR元数据、直接heads、开放PR第三项为空及f98ee06bbce8到427604c818a5的compare。
  - 48号固定任务全文分1–80/81–130；27回执R27-S01/S02/S03、25/26回执与18/28/45约定沿用本对话已有固定材料，不声称本轮全部重载。
  - 固定427604c818a5六件全文：MANIFEST；正文1–60/61–115；facts1–300/301–600/601–920；deferred首次长响应可见主体后以200–310补齐；DOT长响应后以140–190补齐；evidence1–240/241–480/481–720/721–980。
  - 本轮先核固定time options_flags.cmake全文；binfmt_elf.c538–820、821–1150、315–537、65–95，包含完整load_elf_interp和load_elf_binary及共享助手。
  - 本轮固定time namei.c1–180、exec.c550–620、err.h1–100至末尾、file_table.c90–140至末尾、uapi/linux/elf.h9–24。
  - 完整ELF字段布局、基础类型、页面宏、create_elf_tables及AT_BASE/AT_ENTRY和历史mm替换限定沿用本对话此前同一固定源码/回执；不冒称本轮重做其底层证明。
  - 29范围预检：固定time arch/x86_64/myos/utils.c全文、open.c1–285、exec.c1290–1480至末尾及namei.c上述路径名函数。默认分支getname/strncpy_from_user搜索只定位。
review_limits:
  - 未机械复跑89段引文、108图边、19资源场景、10装载模型或全部5算例；执行方Code Mode检查不改记为主线复现。
  - 未运行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU、硬件或真实ELF样本。
  - 未认证实际文件字节、完整路径权限/文件引用、VMA回滚、异常恢复、全配置ABI及动态链接器执行。
  - 未做全仓根因去重；同厂分角色复核不是异族或人类技术验收。
---

# 第28批限定接收：装载偏移、最终入口与清理调用不互相替代

## 1. 裁定及已核交付

按48号I00–I04以PASS_PENDING_LOCAL限定接收，returned_scope=[]。本回执直接补正原报告，旧六件不改，不另派小型文档返工。接受的是有界静态材料，不是原89条锚点/108条图边全量正确或内核运行通过。

交付评论与实际PR17头均为427604c818a5；compare f98ee06bbce8→该头恰1提交、仅user-elf-interpreter-28/六件A新增2558行：MANIFEST103、正文107、facts897、DOT183、deferred301、evidence967。无旧件净修改/删除。主线完整阅读六件及48号，并独立回核核心装载和直接路径/引用接口。

保留主体：第一个遇到的PT_INTERP一旦处理失败便退出，不会跳过坏项寻找下一个；成功完成其解释器头读取后break。路径末字节NUL、打开结果和ELF内容有效性分别核算。本地interpreter与bprm->file、bprm->interpreter、路径buffer和两组头表不同。解释器magic/架构/phdr检查与晚到的类型/mmap门不同；helper返回装载调整量，AT_ENTRY仍使用主程序入口。局部kfree/fput不证明整体映射已回滚。

EI28-F01的分配失败缺口由实际源码支持：__myos_getname可返回ERR_PTR(-ENOMEM)，getname_kernel在有效长度分支直接写name->len，未先判断该错误指针。这一条件事实不证明分配失败已经发生或某种硬件后果。F02–F05仍是带反证/历史重叠的候选，不统称五个独立新漏洞。

## 2. R28-S01：按两个时点分开错误编码和入口范围

需要补充facts.EI28-M15及I03/图的返回分类，不能把helper返回的所有BAD_ADDR都直接折叠为调用者-EINVAL。原矩阵的“may”只是部分分支，不是穷尽分类。源码的实际顺序是：

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (!IS_ERR_VALUE(elf_entry)) {
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (BAD_ADDR(elf_entry)) {
			retval = IS_ERR_VALUE(elf_entry) ?
					(int)elf_entry : -EINVAL;
			goto out_free_dentry;
```

设helper原返回R，解释器头入口E。若R在IS_ERR_VALUE编码区，跳过加法，随后对R走BAD_ADDR/错误转换；若不在编码区，则先记录interp_load_addr=R，再按所选ulong宽度计算F=R+E，随后检查的是F，不是原R。

因此，非编码BAD_ADDR数值若从helper的map_addr早退口返回，调用者也会先作上述加法；需要分别列F仍越界、F进入错误编码区、以及发生模回绕后F落入数值允许区等条件模型。**本轮未证明下层能实际生产这种输入，也未证明全链可达；这是消费矩阵补全，不是新确认的可利用内核漏洞。** 高位ulong也可能用于表达模意义下的装载调整量，不能反过来提出“凡高位偏移都应按无效用户地址拒绝”的无依据结论。

同样，interp_load_addr被赋值不等于解释器已经正确装载；该赋值只受!IS_ERR_VALUE(R)控制。最终BAD_ADDR通过只证明数值门，不能证明F落在已映射、可执行的PT_LOAD内。EI28-X03中~0UL转int得到-1限所选目标转换语义，不当作所有C实现的保证，也不是子函数生产了-ELIBBAD。

load_addr_set在首个ET_DYN的map_addr通过BAD_ADDR后设置，但发生在随后p_filesz/p_memsz/TASK_SIZE校验之前。原“首段成功”应明确为此处数值门通过；后置失败时不能推出已经进入第二段。total_size=0同样要求elf_load实际返回；不返回路径没有后置赋值。首次elf_load的total_size是跨度参数，不等于已经建立持久的整段地址保留。

## 3. R28-S02：路径模型、所有权及条件调用的精确读法

交付评论中“首个成功读取的PT_INTERP”应按正文T03/T04理解为**第一个遇到的PT_INTERP**：失败离开整个装载流程，成功读完头则停止该轮遍历。不能理解为尝试多个候选直到找到成功者。

getname_kernel先分配filename对象，再调用strlen并检查长度，然后写name->len。对本批稳定、尾NUL有效且p_filesz<=4096的路径模型，strlen最多4095，不能把通用len>PAGE_SIZE分支当作该PT_INTERP路径自然可达的错误出口。字符串为空、内部提前NUL与最终非空有效路径是不同条件；末尾NUL不保证首字节非NUL。更广输入的长度边界留29，不在本轮扩审全部路径解析。

[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	size_t len = strlen((void *)k_filename);
	if (len > PAGE_SIZE)
		return ERR_PTR(-ENAMETOOLONG);
	name->len = len;
```

临时路径的kfree位于open_exec调用返回之后，返回文件或ERR_PTR均可抵达；不以“打开文件成功”为必要条件。open_exec若在getname_kernel无效成员访问处无法正常继续，不能仍宣称已完成其后的putname或临时路径释放。M06保留“实际正常返回错误指针”的前提。

正常解释器入口分支调用fput和两次kfree后没有活动的解释器对象再次解引用；随后失败的create_elf_tables走out，不再次经过这些释放标签。该局部结论保留。facts.M18中的already-freed应消费为“已发起规定释放调用”，不是已证物理回收。fput只在计数测试成立时进入kmem_cache_free，文件全局引用、路径和映射资源未闭合。

图须按这些规则合读：loader→kfree的pathname关系覆盖open_exec正常返回错误；localpath(PATH_MAX)→path_length只是p_filesz比较的约束，不是PATH_MAX生成或改写p_filesz；loader→argpages还要求begin_new_exec返回0；interp_loader→make_prot仅在PT_LOAD分支，不能解释为非PT_LOAD也调用；getname→openexec的filename/error结果边要求getname正常返回。后续AT_BASE/AT_ENTRY先写内核辅助向量的既有边界沿用25，不能凭该图认定用户栈副本已写入或START_THREAD已执行。

## 4. R28-S03：补真正符号与关键语句，不用标题充当证据

**EI28-A081的标签和标题是PTR_ERR，但代码实际是ERR_PTR。** 二者转换方向相反；此条不能作为PTR_ERR的逐字符号证据。替代为本轮固定err.h真实定义：

[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```

A059标题包含请求flags/MAY_EXEC，原片段只到初始化器开头，补实际字段：

[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
		.open_flag = O_LARGEFILE | O_RDONLY | __FMODE_EXEC,
		.acc_mode = MAY_EXEC,
		.intent = LOOKUP_OPEN,
		.lookup_flags = LOOKUP_FOLLOW,
```

A053未显示不完整读取的错误分流，A054没有数量门，A055没有真实elf_read调用。虽然27已有相应限定，本批不能把这些不含语句的单个标题视为直接证明。补同一固定对象：

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_read]
```c
	if (unlikely(rv != len)) {
		return (rv < 0) ? rv : -EIO;
	}
	return 0;
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	size = sizeof(elf_phdr_s) * elf_ex->e_phnum;
	if (size == 0 || size > 65536 || size > ELF_MIN_ALIGN)
		goto out;
```
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	retval = elf_read(elf_file, elf_phdata, size, elf_ex->e_phoff);
```

A074标Elf64_Xword但片段未含该typedef，补本轮实际读取的类型：

[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Xword]
```c
	typedef __u64		Elf64_Xword;
```

A077未含p_filesz/p_vaddr/p_memsz，A076未含e_entry/e_phoff；这些字段须合读此前同一固定完整结构，不能只凭片段标题或相邻行号认定证据闭合。A022未显示setup_arg_pages调用后半和返回门，本轮完整load_elf_binary已补核。A037/030分别可由本包已有A088/A087补足AT_ENTRY和第二个kfree，不再重复造锚点。

facts.source_evidence旧范围写A001..A088，但对象引用包括A089；有效证据目录应包含A089。facts.history_edges有11个字符串（含UV-06），deferred.historical_evidence_only有10个对象，二者不是同一个集合；不把“10组历史追溯”推广为每一份清单恰同数、同项。这只是索引消费限定，不改旧根因状态。

以上纠正覆盖正文、facts、DOT、deferred及交付摘要同义表述。执行方89/89文本匹配不等于89/89符号归属与语义已独立验证；本轮没有机械重跑全部检查。

## 5. 后继与停止线

C03c已完成25–28四个工作包，24件、8447净新增行。继续现有执行对话，无需重新接收；规模数字不是上下文token余额。

下一批CORE-USER-PATHNAME-COPY-29接续getname/getname_kernel/putname的输入、长度、复制、错误指针与局部生命周期，连接所选open/exec调用者；不重做ELF、完整VFS路径遍历或权限审计。主线只预检了有限源码以限定范围，尚未执行29。

本轮只写lead回执、49号任务和检查点及PR导航/审查评论；PR17文件冻结、保持Draft。阶段2、18延期、28号同轮安排和45角色分离继续有效。无内核/用户程序修改、补丁、构建运行、合并关闭、删改名/强推或阶段3。RS-01、GAP-U04硬件/运行部分、GAP-U10及历史VMA/GUP/参数页/25局部vma缺口均保留。
