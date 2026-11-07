---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-FILE-CACHE-13-REVIEW-001
record_type: bounded_static_review_with_additive_consumption_corrections
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；沿用Owner最近报告的主线选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
reviewer: MYOS2-A-C03
human_technical_review_attested: false
date: 2026-10-31
read_channel: connector
evidence_class: 固定交付全文阅读、关键源码回读及远端增量核对；非运行认证
authorization_ref: Owner报告CORE-USER-FILE-CACHE-13完成；28号同轮验收和后继派发持续授权
base_snapshot: source=time；results=claude/dazzling-cori-q0dnyt；write=agent/MYOS2-LEAD-002
reviewed_pr: 17
reviewed_head_short12: 2d67bea2fe59
reviewed_base_short12: 16b77f23003f
source_pin_short12: a039d9803ade
contract_short12: 58b0bf268726
lead_before_this_turn_short12: 81aa97e73657
master_ref_observed_short12: e8e60cd19540
status: BOUNDED_ACCEPTED_WITH_MAINLINE_SUPPLEMENTS
acceptance_verdict: PASS_PENDING_LOCAL
acceptance_scope: 缓存容量来源、索引、填页顺序和错误消费及已核对象引用边界；必须合读本回执
returned_scope: []
consumption_corrections: [R13-S01, R13-S02, R13-S03]
errata_disposition: {FC13-ER01: ACCEPTED_BOUNDED_QUALIFICATION, FC13-ER02: ACCEPTED_BOUNDED_QUALIFICATION}
lifecycle_coverage: SELECTED_PATHS_ONLY_MUNMAP_EXIT_CLOSURE_PENDING_14
acceptance_ceiling: PASS_PENDING_LOCAL
runtime_evidence: NOT_RUN
kernel_correctness_verdict: NOT_ISSUED
reviewer_execution: false
reviewer_reproduced_88_quote_checks: false
reviewer_reproduced_70_narrative_checks: false
reviewer_parsed_yaml_or_dot: false
execution_branch_modified_by_mainline: false
phase3_authorized: false
merge_authorized: false
owner_decisions_requested_now: []
next_followup: CORE-USER-VMA-LIFETIME-14
next_task_ref: agent-workspace/lead/MYOS2-LEAD-002/32-user-vma-lifetime-contract.md
inputs_read:
  - 第13批最新交付评论及固定2d67bea2fe59六件全文，截断处用重叠分段补齐；执行compare、两PR状态、直接refs、开放PR第三项为空及PR16完整路径。
  - 固定time：namei.c 294–344；inode.c 70–172与319–365；fat/inode.c 150–270；open.c 1–130；file_table.c 98–138。
  - 固定time：filemap.c 115–346；fat/myos_fat32.c 113–168及352–367；fault/fault.c 1075–1135；buddy.h 161–221；buddy.c 615–644。
  - 固定16b77f23003f：05 deferred-findings.yaml 330–470，准确核对UV-07/08原句；既有05 C4/C5及12限定沿用前轮同源资料。
  - 固定time：mmap.c 1–235、400–640、650–805、1060–1310及arch/x86_64/myos/syscall.c 1–175，作C03边界补读和14设计预检，非14全链结果。
  - 31号任务、12回执、18/28号约定及固定options_flags.cmake沿用本对话先前完整读取的版本；未重新执行接收或整套指导刷新。
review_limits:
  - 六件全文读取不等于88条源码引文、70段正文引文和全部图边均完成独立机械认证；形式解析与本地闸门仍partial/pending_local。
  - 主线未运行命令、JS/Python、哈希、仓库检查器、预处理、编译、C/ASM、内核/QEMU或硬件实验。
  - 同厂模型角色分离复核，不是异族或人类技术验收；不证明完整并发、块I/O、页引用原语、VMA清理或全树回收。
  - RS-01、GAP-U04硬件/运行部分、GAP-U10及此前延期问题保持原边界。
---

# 第13批限定接受；补足消费条件；同轮进入VMA生命周期增量

## 1. 裁定、交付与中断恢复

第13批按PASS_PENDING_LOCAL接受，范围为已核缓存静态事实与本回执追加读法，不是全部生命周期或内核运行通过。无需要执行方单独返工的开放退回；历史原件全部保留。31号C03中的munmap/退出完整连接尚未由交付闭合，本回执不把MANIFEST的C00–C04完成声明扩大为该链已经证明，剩余范围明确进入14。

实际被审头2d67bea2fe59。compare相对16b77f23003f为9个后继提交、仅user-file-cache-13/六件A新增、合计2186行，无旧件净修改/删除。六件全文已读：MANIFEST 79、正文716、facts 1075、DOT 96、延期147、evidence 73行。PR17仍open/Draft，与交付评论的头一致。

执行方报告第五件提交后会话输出中断，随后从远端确认而未盲目重写；最终六件存在且净差异正确，未见本批重复成果目录。主线没有获取客户端故障日志，故不裁定此前卡顿的产品根因，也不把中断自动视为成果损坏。

## 2. 核心证据可用范围

### 容器与索引

已回读inode_init_always、fat_alloc_inode及__lookup_slow。i_mapping指向嵌入式i_data，打开时f_mapping复制该指针。所读slow lookup按当时i_size计算nrpages，正数才分配page指针数组；该段未检查kzalloc结果。不能把nrpages读为已填好页数，也不能从容量来源推出所有创建路径都有相同初始化。

[VERIFIED mykernel/fs/vfs/namei.c::__lookup_slow]
```c
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
		inode->i_mapping->page_array =
			kzalloc(inode->i_mapping->nrpages * sizeof(page_s *), GFP_KERNEL);
```

对ceil公式保留i_size非负、对齐及字节数运算不溢出的前提。已读read/fault/map_page直接使用页号，未见本地nrpages检查；无效指针和越界情形不推出固定errno或硬件崩溃。既有dentry命中绕过该次分配的事实仍限本报告所选入口，其他创建者未穷尽。

### 指针发布、数据完成及返回

[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
			*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
			vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
			filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &temp_pos);
```

simple_filemap_fault有相同的先存槽再读顺序；二者不消费read返回值。缓存命中仅凭非NULL进入复制或PTE赋值，不能把指针存在等同数据就绪。已读FAT32_file_ops.read赋值、FAT32_read真实签名/正文及generic_file_vm_ops；后端状态返回与上层缓存复制/缺页返回是不同接口。FAT32的EOF截短只在合法position、size/count运算及缓冲区前提下消费，不能保证任意越尾输入安全；块传输实际错误能力仍未测试。

do_cow_fault直接经__do_fault进入fault回调，既有槽可命中simple_filemap_fault的BUG_ON；读缺页的fast-map路径不同。该条件关系可保留，不宣称断言已在机器上触发。R13-S03进一步补齐该路径的源页归还操作。

### file引用及清理

已回读fput、iput、folio_put及__folio_put正文。file的计数与页的计数不同；所读fput归零仅释放file对象，iput的实质释放代码为注释，__folio_put也是空的活动主体。不能把函数名、注释中的Linux契约或PTE赋值解释成已完成缓存页回收。原05/08所区分的sys_close与filp_close/fput路径仍不可合并。

## 3. 历史候选FC13-ER01/02：准确原文与限定接受

已经独立读到05::UV-07.impact的“空文件时page_array为NULL”，以及UV-08.claim的“vm_file与f_mapping有效时返回0”及其已有BUG_ON说明。两项候选均接受限定，但不改变UV编号或延期状态。

- ER01替代子句：nrpages==0只证明该slow lookup不分配新数组；已读inode初始化未保证page_array为NULL。实际初值、其他初始化者或已有数组另有前提，不能断言必为NULL、必为非NULL或必然发生故障。
- ER02替代子句：只有file、mapping、数组、索引、空槽、分配结果和后续转换/回调都满足定义良好及返回前提，才走到源码末尾return 0。read返回值被丢弃仍成立，但“文件和mapping有效”不是充分条件，返回0也不是数据完整、硬件恢复或进程继续运行的证明。

UV-08.impact中的“照常映射…继续运行”也只按上述条件性风险消费，不认证实际发生。原报告已提到重复槽BUG_ON，不能把该项算作05从未记录的新发现。

## 4. 主线直接补足的三组记录

### R13-S01：长度变更的真实字段门

FC13-F04/S15/M15、X02及正文中“O_TRUNC后i_size变为0”必须使用真实门，而不是自动等同用户传入普通open flags：

[VERIFIED mykernel/fs/vfs/open.c::myos_do_dentry_open]
```c
	if(f->f_mode & O_TRUNC)
	{
		f->f_path.dentry->d_inode->i_size = 0;
	}
```

覆盖条件为“该函数实际被调用、f与路径有效且f->f_mode & O_TRUNC非零”。本轮未闭合用户flags到f_mode的全链，不承诺一次带O_TRUNC的open一定到达该赋值或成功返回。赋值所在函数没有数组调整的局部观察可保留；不得据此推出全树无truncate/invalidate。原A25只含赋值块，本段补足了真实门。

### R13-S02：E38是回调绑定，不是被调函数读取FOP表

FC13-E38以N13 generic_file_read_iter→N20 FAT32_file_ops表示binding，与此图其他“表持有函数目标”的方向不一致；函数正文也没有读取FAT32_file_ops。消费时用如下追加记录替代旧边，不同时保留为两个活动依赖：

```yaml
target: CORE-USER-FILE-CACHE-13::FC13-E38
from: FC13-N20
to: FC13-N13
kind: data
status: binding_FAT32
meaning: FAT32_file_ops.read_iter保存generic_file_read_iter目标；不是直接调用或运行数据流
anchors: [FC13-A30]
```

[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.read_iter		= generic_file_read_iter,
```

其余data边只按对象/目标依赖消费，写入仍看effects和源码。图的ID集合相同不能代替逐边语义核查；本次不认证未展开宏的机器级调用形态。

### R13-S03：源缓存页归还与私有cow页清理分开

原E44正确限定为私有预分配folio的错误清理，但所有权摘要不能只留下这一个put路径。已有A76对应的源页操作须显式合读：

[VERIFIED mykernel/mm/fault/fault.c::do_cow_fault]
```c
	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
	put_page(vmf->page);
```

在generic文件绑定及有效输入下，vmf->page来自缓存源页；vmf->cow_page是另分配的目标，二者不可合并。走过__do_fault错误/DONE_COW早退、复制并从finish_fault返回后，源页会调用put_page；这既不是清空page_array槽，也不等于私有folio的uncharge_out。若沿folio_put计数归零，当前__folio_put的实际释放调用仍为注释，不能签发物理释放或已发生UAF结论。

另一个需合读的源页归还点是do_read_fault：__do_fault未早退、finish_fault之后ret含ERROR/NOPAGE/RETRY时，调用folio_put(page_folio(vmf->page))。缓存命中没有get_page的原结论不受影响，但“本次缓存使用从不减少页引用”不成立为总括。后继所有权表须同时列源页put、私有页put和槽是否清除；不增加虚构的成对get，也不恢复尚未证明的页所有权保证。

## 5. C03剩余范围及下一批

第13批对fput/iput及页引用端点给出了有界证据，但未完成munmap/退出到缓存资源的端到端连接。主线补读mmap.c：__vm_munmap委托simple_do_vma_munmap；后者有分割/摘链代码；simple_unmap_region主体的页表清理是注释。此处不能仅按顶层函数未出现free就否定全部下层helper，也不能将源码返回0等同VMA、PTE、file与缓存页均已解除。

这些生命周期项作为显式pending进入CORE-USER-VMA-LIFETIME-14，专门核实际入口、区间分割/摘链、文件引用、页表清理边界和失败副作用；旧11–13不重做。本轮PASS_PENDING_LOCAL不是用“全部完成”掩盖这部分未闭合，而是按工作令允许的partial/pending范围接收可复用材料。

六项新发现保留为源码/条件/needs_evidence，四项旧证据追加保留原ID。R13-S01/S02/S03及ER01/02已由本回执给出足够明确的限定，无需另开小型文档返工。若14遇实质反证仍应记录，不以本裁定压过新证据。

本轮只在既有lead分支追加回执、32号任务和检查点，并同步PR导航；执行分支不写。保持阶段2、18号延期，PR17 Draft及两PR/分支，不修内核、不合并关闭、不删除、不强推、不运行或进入阶段3。全部88/70引文机械核验、形式解析和本地闸门未被主线独立重跑，最高结论仍为PASS_PENDING_LOCAL。
