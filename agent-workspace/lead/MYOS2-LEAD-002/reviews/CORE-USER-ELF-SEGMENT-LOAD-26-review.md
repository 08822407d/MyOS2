---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ELF-SEGMENT-LOAD-26-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定六件全文阅读、关键源码独立回核及GitHub增量比较；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告26已完成；28号验收及下一工作包同轮持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 487936efec09
reviewed_base_short12: 897c3a935aa8
contract_short12: e5ea70e40bd4
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-03
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: e65be30ea6f4
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R26-S01, R26-S02, R26-S03]
partially_rejected_claim: EL26-F05中当前simple_vma_link正常返回非零错误被忽略的假设；按R26-S02收窄
acceptance_scope: 主程序PT_LOAD到所选映射、尾页清理、匿名扩展及错误消费的条件性静态资料；须合读本回执
kernel_load_or_cleanup_problems_closed: false
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_all_86_quote_checks: false
reviewer_parsed_json_yaml_or_dot: false
human_technical_review_attested: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-ELF-HEADER-READ-27
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/47-user-elf-header-read-contract.md
inputs_read:
  - 最新交付评论issuecomment-6572916239、PR16/17、直接heads、开放PR第三项为空及897c3a935aa8到487936efec09的compare。
  - 46号固定任务1–130长响应及109–130补尾；25回执和18/28/45边界沿用本对话此前已完整读取版本。
  - 固定487936efec09六件全文：MANIFEST117行、正文1–62/63–125、facts1–220/221–440/441–610、deferred1–250、DOT1–160、evidence1–240/241–480/481–720/721–930。
  - 本轮先核固定time options_flags.cmake全文；binfmt_elf.c90–110、315–505、515–535、790–1025、1130–1150。
  - 本轮固定time mmap.c1–235、465–650、650–805、1090–1285；util.c60–120；vm_map_const.h188–242。
  - uaccess_64.h、err.h、ELF/页类型和宏、mmap_lock及相关文件缓存绑定沿用本对话此前实际读取的同一固定对象；不声称本轮全部重读或新做全链证明。
  - 为27范围预检，固定time exec.c1000–1245及read_write.c1–180；后者250–420是定位时额外返回的写接口片段，未据此启动写路径分析。prepare_binprm/kernel_read默认分支搜索仅导航。
  - PR16路径边界沿用此前完整列表及799a39b36e47到e65be30ea6f4已核增量，当前直接ref仍同一固定头；未本轮重列全部历史文件。
review_limits:
  - 全文阅读加有界语义复核，不是86条引文、83边、全部结构/算例及双分支回读检查的机械复现。
  - 未运行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU、硬件或实际ELF样本。
  - 未认证全配置、程序头实际输入可达性、全部VMA链表/页表释放、用户字节及CPU权限/异常恢复。
  - 未完成全仓延期问题去重；同厂分角色复核不构成异族或人类技术验收。
open_questions:
  - RS-01、GAP-U04硬件/运行部分、GAP-U10及既有VMA/GUP/参数页缺口仍保留。
  - C03c本轮自查工具和检查成功以其材料为自报证据，不视为主线访问其沙箱或独立环境认证。
---

# 第26批限定接收：先行作用、区间计算和错误生产者分别证明

## 1. 交付与裁定

本批以PASS_PENDING_LOCAL限定接收，returned_scope=[]；原件须合读本回执三组补充。对EL26-F05的一项条件判断明确不采纳，直接给替代读法，不把问题全部留给下一轮，不要求执行者单独重交小型勘误。接收主体不表示原86个标签/83条边或五条候选均无条件正确，更不是内核修复或运行PASS。

执行头及交付评论一致为487936efec09。compare 897c3a935aa8→该头恰1个后继提交，仅user-elf-segment-load-26/六件A新增2126行：MANIFEST117、正文115、facts594、DOT149、deferred242、evidence909。历史旧件无净修改/删除。主线完整读六件，独立回核主装载循环、elf_map/elf_load/padzero及关键mmap/munmap/brk接口。执行者86段引文检查、53节点/83边、14场景和4算例仍是其自查记录，不冒称主线机械复现。

可保留主体：主循环的文件大小/部分地址检查在elf_load之后；整体映射后裁剪返回未被消费；padzero先于PROT_WRITE求值且不可写时可能忽略其非零结果；filesz=0绕过elf_map/padzero，匿名起点不会仅因上取整而遗漏首页；VMA描述、实际页内容和硬件权限分层。上述结论保持实际到达、对象有效和算术条件，不证明某一异常输入已在机器上触发。

## 2. R26-S01：零长度扩展、裁剪回绕与真正的区间

### M07的两个条件不能无条件并列

正文返回矩阵和facts.effects_matrix::EL26-M07把文件尾已页对齐与“brk可能为零长度”并列。必须改为：这里的尾指zero_start，即文件有效字节之后的第一个地址，不是最后一个文件字节。对filesz>0、memsz>filesz、地址/对齐无回绕的模型，若zero_start已页对齐，则zero_end>zero_start，ceil(zero_end)-zero_start至少为一页。padzero虽然不调用clear_user，匿名请求却不是零。

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
		zero_start = ELF_PAGEALIGN(zero_start);
		zero_end = ELF_PAGEALIGN(zero_end);

		error = vm_brk_flags(zero_start, zero_end - zero_start,
						prot & PROT_EXEC ? VM_EXEC : 0);
```

零长度请求的EL26-X04/C06/M11可保留：文件尾未页对齐，新增内存字节仍在同一尾页内，使两端上取整相等。memsz==filesz则根本不进入扩展调用，不能混成调用了vm_brk_flags(...,0)。给条件模型时分别登记“未调用”“调用且长度0”“正长度调用”。

### 巨大无符号长度不能直接解释为实际巨大解除范围

EL26-X03/F04的64位差值0x1000-0x3000=0xffffffffffffe000成立，但必须继续接上真实消费者。__vm_munmap不保留长度形式，而是直接计算start+len传给simple_do_vma_munmap：

[VERIFIED mykernel/mm/vm_map/mmap.c::__vm_munmap]
```c
	ret = simple_do_vma_munmap(mm, start, start + len);
```

例如另给map_addr=0x400000、size=0x3000、已对齐total_size=0x1000：裁剪start=0x403000，传入len是上述模值，start+len按64位无符号回绕为0x401000，得到end<start的逆序区间。不能直接认定下层将解除接近全地址空间；也不能据逆序就断言安全拒绝、无作用或必然死循环。下层交集/遍历的历史限制仍在。这里是纸面消费链，不是运行记录。

合法输入下total_size>=size的说明，须比较elf_map内部上取整之后的total_size，且各PT_LOAD地址/大小与对齐运算无回绕。此数值关系与首段是否位于整个跨度最低地址、整体映射是否成功是不同条件。filesz=0的elf_load还会忽略total_size文件预留路线；即使它用于ET_DYN探测，也不能把一次调用/地址返回解释成已建立总体文件预留。

## 3. R26-S02：当前simple_vma_link不能生产正常非零错误

EL26-F05中“simple_vma_link正常返回非零或未完成预期插入”不可合并为一个导致后续计数继续执行的前提。本轮完整核到其固定函数体：旧预分配失败返回是注释，活动流程调用__vma_link_to_list和vma_link_file后只返回0。

[VERIFIED mykernel/mm/vm_map/mmap.c::simple_vma_link]
```c
	__vma_link_to_list(mm, vma, prev);
	vma_link_file(vma);

	// validate_mm(mm);
	return 0;
```

**替代处置：** “do_brk_flags未检查该int返回值”是源码事实；“当前函数正常返回非零错误被吞掉”没有活动生产者，本回执不采纳其作为本固定实现的错误传播缺陷。候选保留原ID但收窄为接口/完成性待核，不能计作一个已证独立内核错误。若下层调用不返回或先发生无效对象行为，不能接着声称调用者必已计数并return0；若正常返回0，也不自动认证全局链表、引用或硬件映射正确。

同一限定覆盖正文E02/E03摘要、EL26-C07、DOT的linkret/ignored_result关系和F05的premises/possible_impact。与之不同，__split_vma等路径确有-ENOMEM文本生产者，不能把本项的常量0结论推广至vm_munmap或整个映射链。

EL26-M06/F02的“后置-EINVAL”还要求elf_load先返回且通过外层BAD_ADDR门；如果此前已报其他错误或不返回，不能强行走到后置门。M13及A019标题的“header cleanup only”收窄为所选错误出口显式kfree头部缓冲并按条件fput解释器文件，而不是只释放头部或已经审计了全局清理：

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
out_free_file:
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
```

当前这些出口未直接实现段映射的全量回滚，不等于外层free_bprm/mm生命周期永不清理。已有映射、部分清零、解除尝试和错误返回是分别带前提的事实；无定义完整执行前提时，不把“先行源码”升级为确定机器终态。

## 4. R26-S03：补证据缺口，纠正图的直接/折叠及实参关系

EL26-A023的代码块实际为6行（含空行），不满足自报全部1–5行；其辅助行号99–103也未包含完整显示片段。主线替代使用下列5行，不改旧件。该局部格式修正不表示其余86段已被主线机械全量验证：

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::padzero]
```c
	nbyte = ELF_PAGEOFFSET(elf_bss);
	if (nbyte) {
		nbyte = ELF_MIN_ALIGN - nbyte;
		if (clear_user((void __user *) elf_bss, nbyte))
			return -EFAULT;
```

A034仅到if语句而未展示提前返回，必须合读实际return -EFAULT；A038到赋值和大括号，最终return map_addr由本轮完整elf_load补核。A003不含e_phentsize比较，不能凭标题/摘要认证字段尺寸门；补真实源句：

[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	if (elf_ex->e_phentsize != sizeof(elf_phdr_s))
		goto out;
```

A070所引VM_DATA_FLAGS_EXEC定义还不足以单独证明VM_DATA_DEFAULT_FLAGS绑定。补所选兜底宏，保留arch可覆盖与配置范围，不当全构建认证：

[VERIFIED mykernel/mm/vm_map/vm_map_const.h::VM_DATA_DEFAULT_FLAGS]
```c
	#ifndef VM_DATA_DEFAULT_FLAGS		/* arch can override this */
	#define VM_DATA_DEFAULT_FLAGS  VM_DATA_FLAGS_EXEC
	#endif
```

图与同义摘要按以下替代读法消费：

| 原关系 | 本轮必要限定 |
|---|---|
| map→vmm无条件call，maplen→vmm长度 | elf_map先有size非零门；total_size非零时映射长度是对齐total_size，否则是size。不能把裁剪起点所用size当两分支共同映射长度。 |
| totalcalc→total的max(min)标签 | 实际是max(p_vaddr+p_memsz)减min(floor(p_vaddr))；保留PT_LOAD存在和算术前提。 |
| loader→probe→load两条call | probe是同一函数内的一处调用点，不是另一个会调用elf_load的函数；标为调用点分组或直接loader→elf_load条件调用。 |
| region→binding→generic_file_mmap | 必须标明经call_mmap包装和真实f_op绑定的折叠链；不是所有文件都自动绑定generic_file_mmap。 |
| filevma→cache | generic_file_mmap写vm_ops，再由对应fault字段绑定帮助函数；新建VMA时不因此立即读取文件页。历史13绑定只按已核范围复用。 |
| brkerr只从do_brk_flags来 | vm_brk_flags自己还可因对齐回绕返回-ENOMEM、零长度返回0；应分包装返回与其内部调用结果，elf_load消费的是包装返回。 |
| clearres只从rawclear来 | __access_ok拒绝时clear_user直接返回原n，不调用__clear_user；条件返回生产者也须入账。 |
| linkret→dobrk标ignored_result | 只表示未消费，不暗示当前可生产非零。必须合读R26-S02。 |

清尾底层的stac/ALTERNATIVE/异常表是活动接口文本，不认证真实剩余字节恢复或硬件权限；BAD_ADDR通过也只是数值门，非有效映射证明。图中数据、调用、返回和写入数量一致不能替代这些语义条件。

## 5. 后继和停止边界

本轮不修内核。五项原延期记录保留原件；F05部分说法已被本回执收窄，不把五项统称五个新漏洞，亦不宣布全仓去重完毕。R23/R24/R25及更早VMA/GUP等限制保持。

C03c现累计正式完成25、26两包，12件、3609净新增行。继续现有对话，无需新接收；这些统计不等于上下文token测量。

下一工作包CORE-USER-ELF-HEADER-READ-27选择向上核输入：prepare_binprm首块读取、格式分派、load_elf_phdrs/elf_read的完整长度门和kernel_read直接接口。相比重跑26映射或展开整个文件系统，此边界能厘清进入段装载前哪些字节真正经过检查。主线只进行了固定源范围预检，未形成27执行成果；具体见47号。

本主线只新增lead回执、47任务和检查点，并同步PR导航及审查评论；不改执行分支。保持阶段2、原两PR及PR17 Draft；无源码修改、补丁、预处理/编译/运行、合并关闭、删改名/强推分支或阶段3授权。
