---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EXEC-RESOURCE-OWNERSHIP-70-REVIEW-001
record_type: independent_bounded_static_acceptance
conversation_display_name: MYOS2-A-C05 内核分析主线续建
produced_by: GPT-6（后端配置不可独立认证）
model_per_owner: gpt pro
date: 2026-11-10
base_snapshot: time
read_channel: connector
source_pin_short12: a039d9803ade
reviewed_base_short12: cf06a0d7f6da
reviewed_head_short12: 1058913beefe
reviewed_delivery_comment: 6766586536
contract101_short12: 8ff60d4d0ad2
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
subjects: {D00: ACCEPTED_R69_01_DOCUMENTATION, A: ACCEPTED_IN_SCOPE, B: ACCEPTED_WITH_SUPPLEMENTS, C: ACCEPTED_WITH_SUPPLEMENTS}
mandatory_supplements: [R70-S01, R70-S02, R70-S03, R69C-S01, R69-S01, R69-S02]
original_artifacts_modified: false
runtime_evidence: NOT_RUN
full_local_P9: NOT_RUN
original105_automatic_byte_definition_scan: NOT_RUN
original_six_file_automatic_prefix_scan: NOT_RUN
original_facts_deferred_full_JSON_parse: NOT_RUN
next_followup: CORE-USER-EXEC-COMMIT-GATES-71
next_task_ref: ../102-user-exec-commit-gates-contract.md
executor: MYOS2-A-C05b / MYOS2-LEAD-002-CHAT-EXEC-08
phase: 2
publication_note: 后置正式评论记录真实Git发布和读回，不把成文状态当作远端状态
---

# 第70批主线独立审查：PASS_PENDING_LOCAL

仅接收固定time源码的有界静态证据，A/B/C按R70-S01/S02/S03限定接收，无新增返工项。69的R69-01单独由闭合回执裁定，不因本轮发现普通内核风险而关闭或升级其他历史问题。

## 1. 固定交付、净差异与实际范围

已读C05b/CHAT-EXEC-08交付评论6766586536，固定提交1058913beefe六件：MANIFEST.md 126行、exec-resource-ownership.md 61行、facts-and-boundaries.yaml 734行、exec-resource-ownership-paths.dot 105行、deferred-findings.yaml 149行、evidence.md 1027行。比较cf06a0d7f6da→1058913beefe为ahead1/behind0、仅一提交、六件added共2202行，旧文件零净修改/删除；PR17 Open/Draft，原执行者STOP_WRITING。没有将执行者的声明当成独立运行认证。

主线读取六件原文和固定time的关键定义，证据标签D4+A24+B42+C35共105，未发现非证据文件额外前缀；属于可读核查而非执行原105标签自动字节/定义扫描。强制锚包括format注册/遍历、bprm.file及mm来源、begin_new_exec移交、free_bprm与fput、独立的D00四宏。原交付B33首次分段显示疑似空行，定点原文件、完整blob与固定源码重复回核为连续5行，因此不建立空行退回，不重开63/58同类误判。

## 2. 可保留的主要技术结论

A：静态formats对象通过DECLARE_LIST_HDR_S定义，启动代码有do_basic_setup→do_initcalls→init_elf_binfmt的源级调用，elf_format.load_binary绑定load_elf_binary，register_binfmt将节点追加，search_binary_handler迭代格式并调用fmt->load_binary。已观察到ELF注册调用不等于实机唯一handler、一定注册次数/顺序或已验证并发安全。相关局部锁与按返回值提前退出分支为注释；后一个影响旧IE69-F01，前一个只能形成需实际重叠操作的条件候选EO70-F02。

B：do_open_execat的file取得后由alloc_bprm写入bprm.file，filename/fdpath/interp有条件别名；bprm_mm_init取得新MM，begin_new_exec依次执行de_thread、unshare_files、set_mm_exe_file、exec_mmap，成功时bprm.mm置NULL。字段置NULL不是MM已被释放/CPU已装载。set_mm_exe_file对新非NULL文件调用get_file增加f_count，旧exe_file若非NULL再fput；解释器局部file与bprm.interpreter不能按同名混作同一对象。

C：free_bprm在bprm.file非NULL时先fput，随后do_close_execat对同一字段又执行fput，中间没有本地get_file。该源码链支持EO70-F01条件性重复归还风险，却不证明实测二次free/UAF。正初值1时第一次可能归零，必须在对象有效性未证处停止；初值2且两次访问都合法时方可讨论2→1→0。free_arg_pages所读体为空，不足以证明其它MM清理均不存在；ELF解释器正常与错误出口的fput不能合并成同一条必达路径。

## 3. R70-S01：MM来源引文与有效状态投影

原B14只有bprm_mm_init入口及mm=NULL的局部声明，不能单独证明已取得新mm。固定time同一真实定义的补锚为：

[VERIFIED mykernel/sched/forkexec/exec.c::bprm_mm_init]
~~~c
	bprm->mm = mm = mm_alloc();
	err = -ENOMEM;
	if (!mm)
		goto err;
~~~

B14须与补锚合读，不改旧70文件，也无需执行者单独返工。作者纯布尔模型mm_0_1（没有取得MM却完成有效转交）不符合当前正常生产链，不可当成可达实例。mm_1_0若谈free_bprm的mmput，还需对象尚有效且bprm.mm在清理时未被先前错误分支置NULL。曾分配与清理时持有是不同前提。

## 4. R70-S02：引用与清理不能用函数名补写

同一bprm.file两次fput的局部证据成立，但对象有效性限制如上。new exe_file的get与old exe_file的put必须合账：当old==new且同一file仍有效时，一次增量可能被旧引用递减抵消，不能机械宣称净加一；新鲜mm_init的NULL初值不代表所有路径旧值均NULL。mmput可读门：

[VERIFIED mykernel/sched/forkexec/fork.c::mmput]
~~~c
	if (atomic_dec_and_test(&mm->mm_users))
		__mmput(mm);
~~~

在固定__mmput中有关exit_mmap及set_mm_exe_file(NULL)的清理文本为注释，不能补造真实file引用或物理页归还：

[VERIFIED mykernel/sched/forkexec/fork.c::__mmput]
~~~c
	// exit_mmap(mm);
	// mm_put_huge_zero_page(mm);
	// set_mm_exe_file(mm, NULL);
~~~

exec_mmap有未处理的mmap_read_lock_killable返回；其后的执行必须保留锁、对象及callee返回的前提，不能直接推出实测锁失效。MM计数/回收缺口属于15已有身份，未重新生成同根因结论。

## 5. R70-S03：图、场景及旧证据边界

DOT文本身份独立核对为4618字节、105行、44节点、56边、未知端点0；Graphviz NOT_RUN。图的CALL_AFTER_RETURN要限定成功门；execmmap→nullmm是返回begin_new_exec后由调用者赋NULL，不是exec_mmap自身写字段；分析停止边不是内核真实错误分支。16个有限模型及old==new反例仅是静态假设算例，不是实际引用数。

EO70-F01/F02和八个EO70-Uxx继续needs_evidence；旧IE69及23/42/43/15原问题不改变身份。原facts/deferred全量JSON机械解析、原105自动逐字扫描、原六件自动P6、完整本地P9、预处理、编译、链接、仓库程序/旧夹具、QEMU/实机均NOT_RUN。

## 6. 后继

71在新的固定102号聚焦exec线程准入、文件表/任务状态变化、point_of_no_return之后的失败路径。现有C05b继续，Owner转发固定入口且G00通过后才可写原PR17新71目录。主线不改执行成果。阶段2，不批准修内核、运行、合并或阶段3。
