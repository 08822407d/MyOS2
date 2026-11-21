---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-ELF-HEADER-READ-27-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
evidence_class: 固定六件全文阅读、关键源码独立回核与远端增量核对；非运行或全量机械认证
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；C03主线
model_per_owner: gpt pro（沿用Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-02
read_channel: connector
authorization_ref: Owner报告27在f98ee06bbce8完成并要求按47号独立验收；28号同轮验收及后继安排持续约定
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: f98ee06bbce8
reviewed_base_short12: 487936efec09
contract_short12: e1fc364983c0
reviewed_executor: MYOS2-LEAD-002-CHAT-EXEC-03
source_pin_short12: a039d9803ade
lead_before_this_turn_short12: 24bf81b0af94
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MANDATORY_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
consumption_corrections: [R27-S01, R27-S02, R27-S03]
acceptance_scope: 首块/程序头读取、格式分派和kernel_read直接接口的限定静态资料；原件须合读本回执
kernel_elf_read_problems_closed: false
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
next_followup: CORE-USER-ELF-INTERPRETER-28
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/48-user-elf-interpreter-contract.md
inputs_read:
  - 最新交付评论issuecomment-6574280669、两PR元数据、直接heads、开放PR第三项为空及487936efec09到f98ee06bbce8增量compare。
  - 固定47号任务全文分1–80/81–130；26主线回执和18/28/45约定沿用本对话此前实际完整读取的固定版本，不冒称本轮重载全部指导。
  - 固定f98ee06bbce8六件全文：MANIFEST；正文1–60/61–120；facts1–250/251–500/501–750；deferred1–270；DOT1–180；evidence1–230/231–460/461–690/691–910。
  - 本轮先核固定time options_flags.cmake全文；exec.c1090–1295、35–60；binfmt_elf.c427–500、610–780、1210–1235；read_write.c49–142；filemap.c200–310。
  - 本轮固定time uio.h1–150至末尾、binfmts.h1–112、fs.h2115–2135、uapi/linux/elf.h232–280、types.h1–115、asm-generic/posix_types.h1–120至末尾、page_const_arch.h1–22。
  - 本轮固定time double_list_macro.h1–180、asm-generic/bug.h1–200、myos_fat32.c350–366。相关类型/ELF常量、elf_format绑定和循环包装末段沿用此前同源实际读取，不声称全部引文独立机械复现。
  - 为28范围预检，固定time binfmt_elf.c539–612及上述610–780的PT_INTERP路径；exec.c570–730含open_exec，额外返回的exec_mmap/de_thread片段不作为新批成果。
  - __kernel_loff_t、WARN_ON_ONCE和open_exec的默认分支搜索仅定位，结论均回固定time。
review_limits:
  - 未机械复跑86引文、96图边、全部结构/场景/四算例或执行方双分支回读；Code Mode JS自查保持执行者归属。
  - 未运行命令、文本程序、解析器、哈希、仓库脚本、预处理、编译、C/ASM、内核/QEMU、硬件或真实ELF样本。
  - 未认证完整编译配置/ABI、空哨兵的全部C对象语义、实际文件EOF、缓存字节、底层I/O、异常恢复及全链exec。
  - 未做全仓根因去重；同厂分角色审查不是异族或人类技术验收。
open_questions:
  - 旧VMA/GUP/参数页和25局部vma等缺口仍未关闭；RS-01、GAP-U04硬件/运行部分和GAP-U10保持。
  - 四项候选是输入/诊断/验证覆盖的条件记录，不等于四个已证独立漏洞。
---

# 第27批限定接收：返回计数、内容、位置和迭代状态分别证明

## 1. 验收结果与交付范围

按47号H00–H04有限验收为PASS_PENDING_LOCAL，returned_scope=[]。主线在本回执直接补正，不改PR17旧件，不另派小型文档返工。该结论不是原报告每条图边无误、全部源码正确或内核运行通过。

交付评论及实际PR17头一致为f98ee06bbce8。compare从487936efec09到该头恰1个后继提交、user-elf-header-read-27/恰六件A新增2280行：MANIFEST113、正文109、facts735、DOT173、deferred257、evidence893；没有历史净修改/删除。六件及47号全文已阅读，关键函数和生产/消费关系已独立回核。

主体判断保留：首块先清零256字节，只在读取负返回时阻止格式分派；非负短读还须经过ELF字段和独立程序头读取的门。所选64位布局下程序头为56字节、最多73项4088字节。elf_read要求返回计数恰等于请求，helper失败变NULL，主程序load_elf_binary此时保留-ENOEXEC。rw_verify_area当前立即返回0；__kernel_read优先read_iter，位置回写被注释。固定FAT32方法表支持所选间接调用绑定，不认证所有文件对象都使用该方法表。

EH27-F01–F04保留为条件候选：F01短读本身不必然是缺陷；F02错误来源折叠是事实，是否违背所选接口要求仍需判定；F03/F04是范围/字段校验边界，不能由缺少某检查直接推导可利用性。UV-03、UV-06/07/08继续使用旧身份；本轮未重做全仓去重。

## 2. R27-S01：补齐迭代器消费状态和真正的赋值责任

47号要求分别证明返回值、迭代器消耗、目标字节和位置。原H03已区分ki_pos、*pos及file.f_pos，但对迭代器进度应明确补账：在所选generic_file_read_iter→simple_filemap_read函数体中，复制使用局部bufp；未见对iter->count、iov_offset、nr_segs、kvec指针或kvec.iov_len的活动推进。初始化为一个KVEC段，不等于实际读后已经消耗该段。给定正常完成、有效对象且无额外间接改写的局部模型，返回256不能自动推出iter.count已减至0；这不是本批新增的全局读迭代器审计。

[VERIFIED mykernel/include/linux/kernel/uio.h::iov_iter_kvec]
```c
			.nr_segs		= nr_segs,
			.iov_offset		= 0,
			.count			= count,
```
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		memcpy(bufp, (void *)(vaddr + inpage_start), len);
		bufp += len;
	}
	return bufp - iter->kvec->iov_base;
```

iov_iter_kvec的direction形参在当前活动初始化中没有写入data_source；相应字段和赋值均为注释。因此“READ方向传参”只能证明调用实参，不证明对象保存了完整方向状态。当前KVEC数据选取仍有实际证据，不能反向声称由于此简化所有本批读取必然失败。

DOT中kiocbinit→kiocb的“initialize file, then assign ki_pos from *pos”要拆开：init_sync_kiocb以复合字面量初始化结构，指定ki_filp；从pos读取并覆盖ki_pos的是__kernel_read自己的后继语句。初始化结构与按文件偏移赋值不是同一个调用者动作。

[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	init_sync_kiocb(&kiocb, file);
	kiocb.ki_pos = pos ? *pos : 0;
	iov_iter_kvec(&iter, READ, &iov, 1, iov.iov_len);
```

FMODE_READ门在所读CONFIG_BUG实现下可落实：WARN_ON_ONCE活动宏仅为(condition)，所以条件成立时进入-EINVAL分支；不附加“已经输出一次警告”的效果。合法file/f_op仍是前提。read_iter优先和legacy read接收原始count的区别保留；MAX_RW_COUNT未截短本批256/4088请求，不推广至任意请求。

## 3. R27-S02：页尾算例的类型、分配门与作用范围

X03及UV-06追加的数学区间正确：pos=64、72×56=4032，end=4096，所选末页表达式对应0−64。但它不能直接被标为不带类型前提的实际loff_t值。

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```
[VERIFIED mykernel/include/uapi/asm-generic/posix_types.h::__kernel_loff_t]
```c
	typedef long long			__kernel_loff_t;
```

在所选64位unsigned long与long long模型下，end % PAGE_SIZE及随后减inpage_start按通常算术转换形成无符号中间值0xffffffffffffffc0，再赋给loff_t。原len=-64保留为采用相应有符号转换语义的目标工具链条件读法，不当成仅凭数学减法就得到的跨实现保证；后续传到size_t的巨大长度仍不是实际memcpy已完成的证据。没有编译/执行验证。此补充约束正文、facts.X03、deferred历史追加和同义摘要，不另编号重复计UV-06根因。

size=sizeof(elf_phdr_s)*e_phnum的73/74界限可保留，依据本轮完整读到的Elf64_Phdr字段及既有固定类型/页定义；A046/A047的片段中间漏掉p_vaddr，不能只把片段的字段相加算结构大小。完整固定结构含两项32位和六项64位字段，所选通常布局为56字节；无额外packing或构建注入的认证未做。

facts.P03的“kmalloc then read attempted”必须写成：数值门通过后调用kmalloc，只有它返回非NULL且控制流继续，才调用elf_read。O08的“allocated uninitialized”收窄为helper没有显式清零此buffer；未核分配器内容不能证明所有实际分配字节都非零或处于某种特定状态。无效对象、分配不返回、kfree不返回等不能拼成正常失败返回路径。

elf_read的ssize_t/size_t比较须放在本批正且不超过4088的长度域；正常负errno与该无符号长度不相等，随后rv<0分流成立。不要将此结论推广到任意size_t极值和异常返回。主程序NULL→-ENOEXEC已核；解释器同一helper的调用点之前设有-ELIBBAD，不得直接复制主程序错误归类，下一批另核该支。

格式表空列表的保留项不等于已发现新的NULL迭代错误：当前宏以list_entry_is_head比较成员地址后才进入循环体。仅凭list_first_entry的“expected not empty”注释不能宣布外层for必有未定义行为。原空/多handler模型继续保留自己的对象与注册前提，本轮没有形式化认证所有哨兵操作，也未证第二个真实注册者。

## 4. R27-S03：补真实调用锚点并修正图中对象身份

A063仅展示kernel_read调用rw_verify_area，没有包含实际__kernel_read调用；A071标题包含count但引文截在.kvec。补关键语句，不用标题代替证据：

[VERIFIED mykernel/fs/vfs/read_write.c::kernel_read]
```c
	ret = rw_verify_area(READ, file, pos, count);
	if (ret)
		return ret;
	return __kernel_read(file, buf, count, pos);
```

count/iov_offset/nr_segs已在R27-S01补原句。A006标题包含retval类型而片段未含int retval；实际类型来自本轮完整search_binary_handler，不能以该标题单独认证。A072未含count==0早返，补对应真实门：

[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	if (!count)
		return 0; /* skip atime */
```

图的必要替代读法：

| 原边/节点 | 修正与消费限定 |
|---|---|
| search→formats标data | search读取formats，数据来源是formats；可改为formats→search数据边，或保留方向但标read/access关系。 |
| lastRet→searchRet标write | 返回值是赋值的数据来源，实际执行retval赋值的是search_binary_handler；不把数值节点当写入动作主体。 |
| kiocbinit→kiocb | 按R27-S01拆初始化与__kernel_read的ki_pos赋值；节点同时代表结构及单字段时要标清。 |
| readBytes→buf标initial or program-header destination | buf原节点仅是bprm.buf[256]，elf_phdata是另一个分配对象；两条读取调用分别连各自目标，不能将程序头读画成再次写首块。 |
| fopIter→generic标indirect_call | 是__kernel_read在该调用点通过file->f_op->read_iter调用；fopIter若作为调用点分组必须明示，不另造中间函数。 |
| simple→pageRead | 仅缓存槽为空且分配/转换等前提成立时通过filp->f_op->read调用；FAT32_read绑定也须合读所选方法表。 |
| noMethod→readReturn | -EINVAL由__kernel_read在两方法均缺失时产生，应保留实际生产者及分支。 |
| phdata→phdrLoop | 目标数组只在elf_read返回0且helper正常交付时可由后继消费；非NULL本身不是完整读或数据正确的充分证明。 |

上述直接补正适用于正文/结构表/图的同义摘要；96边端点检查是执行者的有限机械记录，不是主线判定所有边语义已无误。未机械重跑的86条引文不升级为全部独立复验；本轮新增短引文均从实际返回的固定源码手工对照。

## 5. 后继与停止线

C03c完成25–27三包，共18件5889净新增行；继续现有对话，无需重新接收。这是交付规模记录，不是上下文token余额测量。

同轮后继CORE-USER-ELF-INTERPRETER-28核PT_INTERP路径读取、解释器文件/头取得、load_elf_interp偏移与错误消费、入口交接及局部释放。选择它是为了接续27明确留下的解释器调用差异；不重做25栈表、26共享段帮助函数或27完整读取机制，也不扩展用户态动态链接/重定位和全文件系统。

主线只做了28范围预检，未执行28工作包。继续阶段2、旧件冻结、18延期、28号同轮派发和45角色分离。C03只写lead与PR导航/审查评论；无源码修改、补丁、构建运行、合并关闭PR、删改名/强推或阶段3授权。
