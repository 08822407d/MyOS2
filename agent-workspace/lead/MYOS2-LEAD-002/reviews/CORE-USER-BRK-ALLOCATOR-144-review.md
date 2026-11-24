---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_mainline_independent_review
followup_id: CORE-USER-BRK-ALLOCATOR-144
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；实际后台与effort未独立认证）"
model_per_owner: "Owner本轮明确要求Pro独审及同轮后继设计"
date: 2026-11-24
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: 5d6e550be123
execution_pin_short12: af3fde16ba9b
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PASS_PENDING_LOCAL
self_check: {verified_claims: 1, quotes_reconfirmed: 1, downgraded_to_inferred: 0}
self_check_scope: "本审R144-N01新增1条正式源码窄窗；144原118窗独立复核，不重贴或混计"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
acceptance_verdict: PASS_PENDING_LOCAL
mandatory_mainline_addendum: R144-N01
addendum_status: CLOSED_BY_THIS_MAINLINE_EVIDENCE
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-22
execution_parent_short12: 0e70d0a148b7
author_receipt_id: 7130505162
new_return_items: []
D00_next: []
prior143: PASS_PENDING_LOCAL
prior142: PASS_PENDING_LOCAL
R141_01: CLOSED_DOCUMENTATION_ONLY
combined141: PASS_PENDING_LOCAL
prior140: PASS_PENDING_LOCAL
prior_D00_still_closed: [R138-03, R139-01, R139-02, R139-03, R139-04]
inputs_read:
  - "200合同、198及原有效项目规则；当前四refs/两PR/提交/compare/完整评论分页"
  - "144固定与活动六件、真实作者评论双读、143正式裁定与144发令"
  - "证据专审取得27份正式固定源并核118窗；模型专审取得26份相关源并核11组及图表"
  - "主线直接回源options/panic及master反验、brk、候选用户包装、malloc/结构/list/bitmap及构建文本"
review_limits:
  - "两个只读专审分工，C10汇总并裁定；不把作者自检当独审，不认证作者私有工具时序"
  - "全文/API Git Blob身份核对，未独立重算Git SHA-1"
  - "同族GPT独立会话审阅，不构成跨厂商模型或人类技术验收"
open_questions: ["真实sh链接、预处理/编译、C指针/对象、VMA/页与实际堆操作仍未证"]
---

# 第144批独立验收：PASS_PENDING_LOCAL

**144原六件合读本审§5的R144-N01补注，裁定PASS_PENDING_LOCAL；无新增RETURN，145的D00为空。** 本补注限定一个窄窗说明的支持范围，并补一条实际别名证据；不改旧六件或评论，不要求执行者重交118窗。143、142、141合并消费及140状态、R141-01与五项更早文档关闭保持。

## 1. 固定交付与远端实物

[200合同](https://github.com/08822407d/MyOS2/blob/5d6e550be123/agent-workspace/lead/MYOS2-LEAD-002/200-user-brk-allocator-contract.md) · [正式作者#7130505162](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7130505162) · [144固定六件](https://github.com/08822407d/MyOS2/tree/af3fde16ba9b/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-brk-allocator-144/)。

提交af3fde16ba9b唯一父0e70d0a148b7；ahead=1/behind=0，一提交、恰六件新增、7794 LF、零其他修改或删除。固定/活动全文和API Blob逐件相同，合计289891 UTF-8字节。短SHA均自实际连接器输出复制前12位。

| 文件 | LF | UTF-8字节 | API Blob短码 |
|---|---:|---:|---|
| MANIFEST.md | 369 | 27917 | c3a76c6bb73e |
| brk-allocator.md | 153 | 26046 | c328fb5b6f1b |
| facts-and-boundaries.yaml | 5734 | 169042 | 43f76b4f9918 |
| brk-allocator-paths.dot | 114 | 15088 | 424d30bc0c12 |
| deferred-findings.yaml | 331 | 9348 | 1ac24a0372d3 |
| evidence.md | 1093 | 42450 | 4ce27cdeb51a |
| 合计 | 7794 | 289891 | 6/6 |

直接refs：lead=5d6e550be123、exec=af3fde16ba9b、time=a039d9803ade、master=e8e60cd19540。作者真实ID双读正文相同；PR17无since完整分页100/100/80/0，共280个唯一评论；144 MANIFEST.G00所列十六个历史/主线标记加144作者，共十七个各唯一。开放PR2/0仅16/17；PR16 Open/Ready、PR17原题Open/Draft，原head/base与未合并状态保持。

作者评论保留发表时后续G04待做的真实时点；Owner短回件另报实际G04=PASS和STOP_WRITING。主线直接核远端实物，不据此认证另一聊天私有操作全程或实际后台停写进程。

## 2. 三专题、11组及图表独审

模型专审从固定源码重算A6/B2/C3：B共32轮、C共16阶段，1895项字段/算术对照零差异；正文57行785格与机器件相同（A6＋B32＋C汇总3＋C阶段16）。DOT的40节点/50边按全部约定字段与facts.graph相合，重复、悬空、孤立均0；没有运行Graphviz。

### A：局部brk返回与候选包装

查询0低于min，直接out返回0x401234，对齐未执行；同页请求0x401ff0与旧值均对齐0x402000，直接赋请求。缩小0x400ff0先写字段，再调用simple_do_vma_munmap(mm,0x401000,0x402000)，第三参为end；给定负返回后out恢复并返回入口值。增长0x403001调用do_brk_flags(0x402000,0x2000,0)，第二参为len；给定0后才更新字段，负返回则返回旧值。

R14-S01字段恢复不等于VMA事务恢复，R26-S02的simple_vma_link正常返回0限制保持；没有将假造的非零simple_vma_link返回当作拒绝来源。依据[内核brk](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/myos/syscall.c#L39)、两helper接口与E144-007至022。

所选用户活动声明为void *brk(void *)；编号12、活动switch及MYOS生成链仅作C绑定。用户三层生成宏和共用返回尾中，errno/-1转换为注释，另一__brk转换主体也为注释。给定正常传出的原始数值未被误改为libc式0/-1；非NULL旧break未被写成分配成功。依据[候选用户包装](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/syscall/syscall.c#L13)、E144-032至074及声明/构建相关窗口。

### B：完整初始化与拒绝轮统计

B_OK两break最终0x42000000，16次接受、16节点/16地址。B_REFUSE_I1仅第二次扩展给定负返回，返回旧0x40200000，链式赋值同时保存旧值；第三次再次请求0x40400000，最终0x41e00000，15次接受、16节点/15地址。

两组各查询1＋扩展16，共17个给定brk调用。每轮先把本轮base_upage放free尾，再从非空分支取出同一节点加入used头，free 0→1→0、used i→i+1；动态malloc/brk支路未到。obj_size/obj_nr从32/65536至1048576/2，最终cache16/free0/used16、used顺序15…0；每档normal_free1、used/full0、total1，所有给定效果完成后才initiated=true。

拒绝组的尝试长度和32MiB与去重请求跨度30MiB分开，不把拒绝轮算成真实扩展；接受轮保存值等于请求区间排他上界，拒绝轮保存下界。重复数值不证明真实对象别名、页重复占用或内存破坏。依据[init_uslab](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/malloc/malloc.c#L174)、[实际列表](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/util/list.c#L73)及E144-075至118相关项。

### C：两种消费与观察入口

C_REUSE返回同一Q、used头插，两个break和Q.vaddr保持，Q.uslab_p未读写。动态两例从给定malloc已正常返回有效M之后开始；拒绝值0x50000000非NULL仍过门，独立+=H使用户值0x50200000、内核字段0x50000000，M.vaddr保存旧值。两例均used尾插并返回M，free(M)未到，M.uslab_p未被赋值。

4/6/6阶段区分brk返回、非NULL门、独立加量与存值、入链及返回；没有从后继入口反推递归malloc真实可达性。依据[upage_alloc](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/malloc/malloc.c#L111)、E144-080至085和列表/成员证据。

## 3. 118窗、标准文档及支持范围

证据专审独立读取27份正式固定源码，118个不同物理窗口均为1–5行，ID连续唯一；路径、符号、性质、范围、源Blob及逐行行尾元数据相合。118窗逐字还原均相同，0逐字不一致。正式前缀只在evidence中118次，其他五件0；MANIFEST标准根P3/P6=118/118/0，六件消费索引齐。

malloc.c原对象44db9ea9d7f7含390 LF/390 CRLF，末行无换行另计物理行；21窗共69条CRLF物理行按记录可逆还原后逐字相合。证据专审另取真实Git Blob API全文，与fetch_file原字符相同；不等于独立重算SHA-1。主线也直接取得同一原始源文本，未将无声明换行归一化当作原字节相同。

正文128次证据ID引用均可定位，涉及117个不同ID。除§5的E144-098说明短语外，其余支持关系及强制锚范围相合；E144-098本身的引文与结构/成员标签真实，不能把这个支持范围补正写成原118条说明全部原样无瑕疵。

两项新deferred为INIT_RETURN_ADDRESS_CONSUMPTION和DYNAMIC_BREAK_DIVERGENCE，逐项source_ref及6个模型selector可定位；继续是条件局部数值观察、recorded_no_runtime_claim，不签新增已证运行缺陷。源码事实、给定前提和未知效果没有混写。

## 4. 可达闸门与未证界限

P9-1长标识扫描0；2/13的逐字/归属/强制锚按§3及§5合读；3/4路径/物理范围可定位；5原根118/118/0闭合；6无新增外部结论或内部检索句柄；7/8主线直接核time双canary命中、直接master不命中，startup整句命中；11四项禁用运行措辞0。9/10无实现代码或补丁，12非降级交付，均NA。完整本地P9仍NOT_RUN。

K=4096与用户本地H=2MiB分开。顶层构建只进入myinit/myshell并指向仓外musl，独立mylib文本不能证明当前sh采用该库，actual_linked_to_sh=UNKNOWN。用户void*加法只作给定字节地址投影；真实CPU返回、C对象/指针、VMA/页及实际堆操作均未证。作者发布前重开源码时序与本次独立复核分开，不倒签其私有操作过程。

## 5. R144-N01：限定原窗口说明并补别名证据

原定位：固定af3fde16ba9b的evidence.md第887–896行、facts.source_index[97]（E144-098 / upage_members）。其窗口为_malloc.h第19–23行，正式标签uPage、kind=struct_member，支持结构头和upage_list/vaddr/uslab_p三个成员；说明中额外的“and alias uPage_s”未被原窗口包含。别名实际在同一定义下一行24，正文没有据此使用错误类型或布局。

**替代消费：原E144-098只支持结构/成员；别名由以下主线新增ML144-001支持。** 这不是把原窗口悄悄扩到六行，不追认作者原说明已完整受原窗支持。旧六件/旧评论保持，来源与修正关系留在本审。

ML144-001；time=a039d9803ade，源Blob短码2dea1f8fc05c；真实路径myinitramfs/mylib/myglibc/malloc/_malloc.h，uPage定义内第22–24行；逐字LF原文：

[VERIFIED myinitramfs/mylib/myglibc/malloc/_malloc.h::uPage]
```c
		void	*vaddr;
		uslab_s *uslab_p;
	} uPage_s;
```

该尾部直接给出uPage_s别名。主线在交付前重新打开固定源核此3行，独立新计1/1/0；不加入作者118计数，也不要求执行者在145重复重核它们。本补注闭合这处说明的证据缺口，R144-N01=CLOSED_BY_THIS_MAINLINE_EVIDENCE；原技术模型、图表及新增条件观察不受影响，new_return_items仍为空。

## 6. 后继与规范持续性

第145批按[201号合同](../201-user-malloc-slot-contract.md)转向malloc档位选择、位图找槽与对象消费三个关联专题；不重做brk初始化或全malloc/free。144冻结原件必须与本审§5合读，145 D00空。

继续C10a/EXEC-22、次一档GPT-6.2 Sol及Owner可用高推理强度，交C10 Pro独审。按198，完整材料在GitHub、双向聊天一个短取件块。C10正式设计从4增至5仅在145正式发布成功后登记，12/15均未达到。

本轮Meta-Agent master及有效入口未变；建设ref前移50806a75e050的五份记录只登记/暂缓待办和旧评审处理时机，没有新采用决定，继续原有效全文。Mnemosyne ref及批准命名未变；当前C10/C10a名称和原任务号保持，不导入跨项目写权。

阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作NOT_RUN。仅新增主线独审/合同/检查点并维护必要PR导航/正式评论，不改源码、执行成果或旧正式评论，不建分支/PR、合并、强推或删分支。
