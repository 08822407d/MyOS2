---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-EXEC-STACK-23-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定交付全文阅读、关键源码回核和远端增量比较；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告CORE-USER-EXEC-STACK-23完成；28号同轮验收和派发持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 299a960562a9
reviewed_base_short12: b32f3ebc9095
contract_short12: c9be5ac03550
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-02
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 50ca9e6b2c31
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R23-S01, R23-S02, R23-S03]
acceptance_scope: 所选exec栈VMA创建、准备、扩展及参数页/缺页接口的条件性静态材料；原件须合读本回执
kernel_stack_or_argument_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_116_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-ARG-PAGES-24
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/43-user-arg-pages-contract.md
inputs_read:
  - PR17交付评论issuecomment-6568589042、两PR元数据、直接heads、开放PR第三项为空及b32f3ebc9095到299a960562a9的compare。
  - 固定299a960562a9六件全文。正文1–85/86–140；evidence1–310/311–620/621–930/931–1160；facts1–460/461–900/901–1310/1311–1750，长响应截断后以1680–1735和1720–1980/1981–2180补齐；DOT长响应后以138–160补尾；MANIFEST和deferred完整返回。
  - 本轮固定time exec.c 1–190、250–550、615–710、1080–1230及1230–1440；gup.c全文；binfmt_elf.c 680–910；fork.c180–490。
  - 本轮固定time mmap.c1250–1370、vm_map.h1–160、vm_map_const.h1–260可见主体至VM_INIT_DEF_MASK、mmap_lock.h全文、pgprot.c全文、process_arch.c1–230至末尾。
  - 本轮固定time mm/fault/fault.c1–230及1100–1490至末尾，含两个myos单页映射帮助函数；1500–1780空返回只说明越过末尾，不算已读源码。
  - 同一固定time的expand_stack全文、do_user_addr_fault点包含/扩栈/返回段、util.c列表与randomize_stack_top、PAGE/STACK常量、pgtable常量以及begin_new_exec已见调用沿用本对话前次实际读取；不申报本轮全量重读。
  - 42号任务和22回执全文已在本对话前次发布并读回；本轮重读28号。18/24/25/36、12/15/19/20/21既有约束按已读固定材料合用，不声称重载全部指导。
  - 默认分支myos_creat_one_page_mapping搜索只作导航；结论回time固定对象。新24的取证范围仅作设计预检，不宣告已执行。
review_limits:
  - 未机械复跑116段引文、74条边、35场景及4组算例；未解析JSON/YAML/DOT或认证全部图关系。
  - 未执行命令、文本程序、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU、硬件或实际argv/RSP测试。
  - 未闭合初始NULL迭代、GUP缺失返回值、全部生产时序、全局同步、引用净额、页内容或硬件权限。
  - 同厂分角色复核不是异族或人类技术验收；RS-01、GAP-U04硬件/运行部分、GAP-U10和普通延期项保持。
---

# 第23批限定接收；栈区域、参数页与返回值不能互相替代

## 1. 裁定和交付身份

按PASS_PENDING_LOCAL接收，必须合读R23-S01/S02/S03。主要链和问题边界可以复用；以下报告局部错误由主线给出替代口径，不改执行原件，不要求单独返工，也不声称所有原始标注都正确。

交付评论及执行头均为299a960562a9；compare b32f3ebc9095→该头为1个后继提交，仅user-exec-stack-23/六件A新增3893行。MANIFEST55、正文126、facts2168、DOT154、deferred250、evidence1140行已完整阅读，无旧件净修改/删除。执行方116条逐字检查和12次双重回读保留为其记录，主线没有冒称再次机械执行。

可保留的核心事实：临时栈VMA在bprm.mm创建，参数复制发生在所选mm替换之前；setup_arg_pages在ELF成功分支中晚于begin_new_exec/setup_new_exec；局部EXSTACK策略不等于实际VMA/PTE权限写回；expand_stack的返回值不自动证明扩张或物理页就绪；GUP声明long但没有活动return，选定新页映射又使用固定PAGE_SHARED_EXEC模板。完整调用可达性仍受未定义/未闭合的前序接口限制。

## 2. R23-S01：分支前提、生产不变量和地址阶段

M25/M26/M28、正文S03及no-grow纸面例必须补：对象有效、地址先经PAGE_MASK，且对齐后的地址已通过address>=mmap_min_addr门。仅有address>=vm_start或grow>vm_pgoff不足以推出返回0；最低地址门在这些比较之前，可能先返回-EPERM。字段写入还要实际越过spin_lock调用；函数正常返回不是前缀写入发生的必要条件。

[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
	address &= PAGE_MASK;
	if (address < mmap_min_addr)
		return -EPERM;
```

再限定ES23-F04的生产路径：insert_vm_struct对该匿名栈设置pgoff=vm_start>>PAGE_SHIFT。设页大小P，页对齐的旧起点S满足pgoff=S/P，则对页对齐的非负A<S，grow=(S-A)/P<=S/P。成功扩展后新pgoff=S/P-grow=A/P，关系仍保持。因此，在这一初始生产与仅经正常expand_stack更新、无其他写者/回绕/无效对象的模型中，grow>pgoff不是仅把请求再向下移动就能触发的分支。该helper的缺省零返回事实保留，但必须另证改变pgoff关系的生产者，不能把直接函数的任意参数模型当作已选exec正常路径。

[VERIFIED mykernel/mm/vm_map/mmap.c::insert_vm_struct]
```c
	if (vma_is_anonymous(vma)) {
		BUG_ON(vma->anon_vma);
		vma->vm_pgoff = vma->vm_start >> PAGE_SHIFT;
	}
```

M10须区分bprm.p一次减掉整个字符串长度，与每次get_arg_page真正收到的pos：整体终点落到栈VMA以下，并不证明本次循环的每个分块都未命中。只对当前被查询地址没有任何覆盖VMA的情形，才能说GUP执行continue并跳过pages[i]赋值。

M32及正文“栈起点以下lookup为NULL”须补“也不被其他VMA包含”。若低地址恰在另一VMA内，lookup可能返回那个对象，而不是原栈的后继；稳定点包含条件仍会绕过栈增长门，但不能无条件写成myos_bad_area。MF12-F01关于不能用现有点包含查询发现待向下扩展的原栈VMA的核心结论保留，初始NULL迭代前提也不解除。

M21/M22的loader变化保留if(bprm->loader)；局部变量和错误分支模型不能默默取消这一门。缺失搬迁只对非零shift且确需搬迁的条件讨论；所选固定栈顶与未变VMA末端支持shift=0，不能据被注释的shift_arg_pages推定默认路径已经丢失搬迁。

## 3. R23-S02：返回类型、宏来源、计数和身份

### 返回值与页输出是独立契约

正文“long ret<=0”不准确：GUP声明long，而get_arg_page保存结果的是int ret。E31/M11及F02合读该转换和实际消费类型，不能用被调签名替代调用者局部类型。

[VERIFIED mykernel/sched/forkexec/exec.c::get_arg_page]
```c
	page_s *page;
	vma_s *vma = bprm->vma;
	mm_s *mm = bprm->mm;
	int ret;
```

GUP末尾没有return且其结果被使用，使这条消费缺少定义良好的返回值依据；不能推定为某个随机整数、固定负errno或必然成功。pages[i]是否写过又是单独事实。有效指针、获得新引用、映射帮助函数成功、数据已复制均不能从“返回了控制流”推出。M12/X19须保留有效pte与实际抵达写入门；GUP并未检查myos_creat_one_page_mapping的返回值，分配page成功不等于映射已成功。

### 默认可执行位的来源

正文S01、M07和A090标题把默认可执行位归因于TASK_EXEC，因果链应改为当前未被覆盖的VM_STACK_DEFAULT_FLAGS→VM_DATA_DEFAULT_FLAGS→VM_DATA_FLAGS_EXEC；后者直接含VM_EXEC。TASK_EXEC确实另有定义，但它属于另一组合VM_DATA_FLAGS_TSK_EXEC，不能因为最终位值相同就替换实际宏依赖。PAGE_COPY_EXEC的选表结论在该默认链下保留；完整预处理配置与硬件结果仍未认证。

[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_DATA_DEFAULT_FLAGS]
```c
	#ifndef VM_DATA_DEFAULT_FLAGS		/* arch can override this */
	#define VM_DATA_DEFAULT_FLAGS  VM_DATA_FLAGS_EXEC
	#endif
```
[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_STACK_DEFAULT_FLAGS]
```c
	#ifndef VM_STACK_DEFAULT_FLAGS		/* arch can override this */
	#define VM_STACK_DEFAULT_FLAGS VM_DATA_DEFAULT_FLAGS
	#endif
```

M18仅是给定其他生产者的局部OR模型。当前mm_init使用VM_INIT_DEF_MASK，而固定宏为VM_NOHUGEPAGE，不包含VM_EXEC；在此生产链且无其他修改时，不能宣称mm.def_flags会把局部已清除的EXEC重新加回。EXSTACK没有整体写回VMA的F01独立成立，不需要借用这一未证生产者。

ES23-F04.claim中“setup_arg_pages在初始插入成功时设置stack_vm/total_vm为1”归属错误；真实写入者是__bprm_mm_init。M29/X09及其正文已有正确的初始阶段口径，应以此为准。expand_stack所读活动体未更新这些计数；这只是特定阶段的赋值/局部差异，不证明所有时刻全局total_vm都等于1。

X02的“linked to new mm identity”只能表示vma_init写vm_mm。对象身份关联不是已经加入mm_mt列表，后者需要insert_vm_struct及真实链接动作；也不等于分配/安装了参数PTE。

## 4. R23-S03：补足调用证据，修正图中的关系类型

以下引文片段原文存在，但与部分标题/边所需事实不完全相符。原件不覆盖；后续以本节补文和限定消费，不把116个标签等同116项已独立语义通过。

E01/A043：A043仅含返回与清理标签，缺alloc_bprm中的真正调用，补：
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	retval = bprm_mm_init(bprm);
	if (retval == 0)
		return bprm;
```

M06/E08/E09/A084的清零依据应包含真正memset，而不是只凭后续vm_mm与dummy赋值：
[VERIFIED mykernel/mm/vm_map/vm_map.h::vma_init]
```c
			static const vm_ops_s dummy_vm_ops = {};
			memset(vma, 0, sizeof(*vma));
			INIT_LIST_S(&vma->list);
```

E21/A051：原片段只有错误分支和return，不含调用；补：
[VERIFIED mykernel/mm/vm_map/mmap.c::insert_vm_struct]
```c
	if (simple_vma_link(mm, vma, prev)) {
		// if (vma->vm_flags & VM_ACCOUNT)
		// 	vm_unacct_memory(charged);
		return -ENOMEM;
	}
```

E23/X08的map_count增量不能仅用simple_vma_link的调用链片段证明；实际写入在util.c::__vma_link_to_list，仍须按链接调用与计数各自实际抵达、正常返回的条件消费，不自动认证链表结构。E30/A002的mmap_read_lock调用补为exec.c::get_arg_page中的单行mmap_read_lock(mm)，原A002本身只含注释和GUP调用。

E44/A073所需的setup_new_exec真实调用来自binfmt_elf.c::load_elf_binary中的setup_new_exec(bprm)，位于begin_new_exec非零早退之后、setup_arg_pages之前；原A073只覆盖后一个调用。A071应连读case PT_GNU_STACK，不能仅凭PF_X分支的截取证明程序头类别。A045遗漏env复制句，实际顺序以do_execveat_common中copy_strings(bprm->envc, envp, bprm)及之后的argv调用为准。

M31/E71及A066标题所称跳过grow检查，还必须包含以下被原截取省略的门：
[VERIFIED mykernel/arch/x86_64/mm/fault.c::do_user_addr_fault]
```c
	if (likely(vma->vm_start <= address))
		goto lock_mmap;
```

E64的数据提供者为expand_stack入口处mm=vma->vm_mm，而原A054没有覆盖该赋值。E67的终点N66是mm.page_table_lock字段，不能标成对字段的函数call；按锁操作使用该对象的数据关系消费，真实调用是spin_lock及spin_unlock。A060没有锁入语句，补源中的：
[VERIFIED mykernel/mm/vm_map/mmap.c::expand_stack]
```c
				spin_lock(&mm->page_table_lock);
```

E46不应将ELF的PT_GNU_STACK/PF_X→EXSTACK策略推导画为读取VM_EXEC宏：它们是不同阶段的标志体系。将该边解释为ELF程序头数据到局部executable_stack策略的依赖；VM_EXEC由setup_arg_pages后续局部策略操作消费。E74为START_THREAD宏调用位置，不是函数指针绑定；未展开该宏就不签发寄存器写入结论。A036标题提activate_mm但片段未含调用，可合读exec_mmap真实activate_mm(active_mm, mm)语句，仍不认证CR3/TLB效果。

以上限定同样适用于DOT同ID和同义摘要。未逐条复验的其他关系保留有限状态，不因端点集合一致自动通过。

## 5. 后继范围与权限

五条本批条件记录及MF12-F01/F02/F03三组旧证据追加保持原身份/延期状态；不等于八个新实测故障，也没有完成全仓历史根因去重。报告修正不关闭内核风险。

同轮发布CORE-USER-ARG-PAGES-24：定向核get_arg_page→myos_get_user_pages→单页查询/创建映射→参数复制/put_arg_page的接口与对象链。重点区分已有page与新page、返回状态和输出槽、借用指针与持有引用、映射失败与已分配对象、实际拷贝调用和先行指针推进。23已证缺return/固定PTE模板只作既有根因输入；18页引用、12缺页、15 mm清理和23栈元数据不重做。

主线本轮读到的GUP和映射帮助函数仅为验收及24范围预检，未完成24的引用/复制分支分析。继续C03b、原PR17、新批限定六件；主线独占lead和验收。无内核/用户程序修改、补丁、编译运行、合并关闭、删分支、强推或阶段3授权。
