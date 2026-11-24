---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_mainline_independent_review
followup_id: CORE-USER-EXEC-ARGUMENT-LIMITS-143
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；实际后台与effort未独立认证）"
model_per_owner: "Owner本轮明确要求Pro独审及同轮后继设计"
date: 2026-11-24
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: c09ad0b19c2e
execution_pin_short12: 0e70d0a148b7
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PASS_PENDING_LOCAL
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "本记录不新增正式源码引文标记；被审143的91窗独立计量"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
acceptance_verdict: PASS_PENDING_LOCAL
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-22
execution_parent_short12: a911aa471305
author_receipt_id: 7129263893
new_return_items: []
D00: []
consumption_navigation_supplement: R143-N01
prior142_status: PASS_PENDING_LOCAL
R141_01: CLOSED_DOCUMENTATION_ONLY
combined_141: PASS_PENDING_LOCAL
prior140_status: PASS_PENDING_LOCAL
prior_D00_still_closed: [R138-03, R139-01, R139-02, R139-03, R139-04]
inputs_read:
  - "固定199/198、工作令和原项目规则；直接master公约及协议v2；142独审/检查点"
  - "143固定与活动六件、真实作者评论双读、提交/compare/四refs/PR/完整评论分页"
  - "证据专审独立取得13份固定正式源码，91窗逐字、归属、性质和支持关系"
  - "模型专审重新取得140 A1输入、exec与测长/宏/类型定义，核19模型、42表行及图"
  - "主线直接读配置，exec限额/复制/两caller、generic strnlen、strnlen_user、活动minmax及双canary"
review_limits:
  - "两个只读专审分工，C10汇总裁定；不把作者自检当独审，不认证其私有工具时序"
  - "仅连接器与纯文本/JSON/算术；完整本地P9及所有构建/运行均未做"
  - "全文与API Git Blob身份核对，未独立重算Git Blob SHA-1"
  - "GPT主线审阅GPT执行对话产出，不构成跨厂商模型或人类技术验收"
open_questions: ["实际编译/预处理、C对象有效性、GUP/页对象、真实用户访问/复制、清理及运行继续未证"]
---

# 第143批独立验收：PASS_PENDING_LOCAL

**第143批限定通过，无新增RETURN，后继144的D00为空。** 142的PASS_PENDING_LOCAL、R141-01文档关闭、141合并消费PASS_PENDING_LOCAL，以及140和五项更早文档关闭保持。主线在§5追加两项finding的标准来源导航；这不是内核修复或重新判闭141，也不改作者旧件。

## 1. 固定交付与远端身份

[199合同](https://github.com/08822407d/MyOS2/blob/c09ad0b19c2e/agent-workspace/lead/MYOS2-LEAD-002/199-user-exec-argument-limits-contract.md) · [正式作者#7129263893](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7129263893) · [固定六件](https://github.com/08822407d/MyOS2/tree/0e70d0a148b7/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-limits-143/)。

执行提交0e70d0a148b7唯一父a911aa471305；ahead=1、behind=0、一提交、六新增5675 LF、零其他修改或删除。固定/活动全文与API Blob逐件相同；UTF-8字节总数219350。短码均自连接器实际输出复制前十二位。

| 文件 | LF行数 | UTF-8字节 | API Blob短码 |
|---|---:|---:|---|
| MANIFEST.md | 351 | 24038 | e8460ef746b0 |
| exec-argument-limits.md | 151 | 20732 | efbf7746f05b |
| facts-and-boundaries.yaml | 4059 | 124395 | 65e3621e028b |
| exec-argument-limits-paths.dot | 173 | 19380 | febe867b1078 |
| deferred-findings.yaml | 124 | 4221 | 6b3db077c8c3 |
| evidence.md | 817 | 26584 | f54488881f1e |
| 合计 | 5675 | 219350 | 6/6 |

本轮预发布直接refs为lead=c09ad0b19c2e、exec=0e70d0a148b7、time=a039d9803ade、master=e8e60cd19540。作者真实ID双读正文相同。PR17无since完整分页100/100/78/0，共278唯一评论；十三个既有标记和本批作者共十四个标记各唯一，旧EXEC-21的141正式作者仍为0。开放PR2/0，仅16/17；PR16 Open/Ready，PR17原题Open/Draft，原head/base、未合并状态保持。

作者评论如实保留其发表时“后续G04待做”的状态；Owner短回件另报实际G04=PASS、STOP_WRITING。主线本轮直接核远端实物，不以该声明认证作者私有操作全过程，也不修改旧评论。

## 2. 三专题与19组模型

模型专审重新从源码推导A8/B4/C7，另从冻结140机器件A1取五串，而非只复述199期待表。635项字段/算术比较零差异；正文42个模型/阶段表行与facts逐字段相同，包括A8、B4、C_EQ/C_OVER两行和四组各7阶段。K0另按其独立零计数入口核。

### A：预算及类型边界

当前活动L0为6291456（0x600000）；rlim_cur的min为注释。活动max展开与int计数字段支持先完成int加法，再与sizeof(void*)运算。A_ZERO/A_SMALL/A_PTR_BELOW分别给argmin=0x200008/0x200020/0x7ffff8；A_PTR_EQUAL在limit<=ptr_size处拒绝，尚未执行减预算或set_limit。A_NEG在计数门拒绝；A_SET_EQ得到整数argmin=0，A_SET_LT保留A_old。

A_INT_SUM的数学和2147483648在所给32位有符号模型不可表示，后续ptr_size/正常errno/字段保持NOT_DERIVED；没有伪造定义良好的回绕结果，也没把乘法接收类型提前用于加法。整数预算和argmin=0均不证明实际映射或地址可用。

依据：[exec.c::bprm_stack_limits/set_limit](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L244)、[活动minmax](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/include/linux/kernel/minmax.h#L28)及E143-012至027。

### B：有界测长与终止测试

所选generic定义的四例r依次为131070、131071、131072、131072，len为r+1。短串最后count旧值2/1，递减后1/0；两长串耗尽M后再作旧值0的无符号后缀递减，发生定义良好的回绕，但&&短路不读索引M。与A_INT_SUM分开处理正确。

B_NEXT/B_UNTERM在长度门拒绝，bprm.p尚未扣减；返回M+1不表示完整串长或界限外NUL。条件定义门不等于实际预处理选择；strnlen_user活动体仅strnlen+1，被注释的访问/异常机制未被提升为当前用户故障恢复。

依据：[generic strnlen](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/string.c#L488)、[strnlen_user](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/lib/string/strnlen_user.c#L92)及E143-028至049。

### C：预扣p、字段时点和外返条件

两copy helper都先判长度、再预扣p、再直接比较p<argmin；没有虚构bprm_hit_stack_limit调用。C_OVER的-E2BIG发生时p已改变，本次取页/复制未到达；C_EQ仅通过预算门，不直接推出ret0。

C30/C29需求10/5/4/3/8，C29最后8字节预扣后拒绝，只保留22字节条件前缀，本次页/复制和exec调用跳过。C20/C19先19再补1；C20在给定补串正常返回后才argc=1，C19仍0。K0计数返回0先改retval为-EINVAL，再跳清理，尚未给bprm.argc赋值。

四条流水每行actual_copy_status=NOT_ESTABLISHED，前序返回及写入效果分别作为条件给定；“一个元素正常”没有冒充整个wrapper已返回0。C30/C20最后观察点在条件bprm_execve调用前；C29/C19/K0只记清理前字段，最终对外errno以free_bprm/putname正常返回为条件，不读取释放后bprm。GUP缺return、真实页/用户复制及清理问题没有被这些模型闭合。

依据：[两个copy helper](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L305)、[两个caller](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1291)及E143-036至088。C短预算是独立题设，不是A实际计算结果或142的建表入口。

## 3. 91窗、图及标准文档

证据专审独立取得13份正式固定源码，E143-001至091均为不同的1–5连续物理行窄窗，逐字相同，ID/path/symbol/range/kind与源Blob全部相合。正式标记仅evidence中91处，其余五件0。性质为活动函数63、宏10、类型4、结构成员3、局部声明2、纯注释3、条件门1、定义头2、构建3。

E143-029/031只用于签名类型，活动行为分别由030/034支持；028为条件定义门，023/032/033为注释，没有被用于证明活动操作。额外无正式窗口的mm_misc_const.h及类型入口有实际读取，PAGE_SIZE来自补读的真实page_const_arch.h/const.h，不误认auxv文件为页定义。正文93次E143引用均可定位91项索引。

MANIFEST标准根read_channel、startup_selfcheck_quote、branch_canary_quotes和self_check=91/91/0齐；主线直接重新核startup在master整句命中，两个canary在time命中、master不命中。六件消费表完整，D00空，旧129/73不混计。facts/deferred为可解析JSON。

68个节点id/kind/label/scenario和81条边from/to/kind/label与facts.graph逐项相同；无重复、悬空、孤立。这里只核文本和数据，不运行Graphviz，不把图中条件串联当作真实端到端执行。

## 4. 协议可达部分及限定

P9项1长标识扫描0；2及13全91窗、归属/支持关系及强制锚覆盖；3/4当前文件、源索引和物理范围可定位；5标准91/91/0闭合；6无新增外部技术结论或内部检索句柄；7/8金丝雀及直接master自检相合；11四种禁用运行措辞均0。9/10无实现代码或补丁、12非降级交付，均NA。完整本地P9继续NOT_RUN。

本轮独立核对结果与作者的91/91自检记录分开；不能从静态交付认证其工具全部操作时序。API Blob/全文相同不等于主线独立重算Git SHA-1。实际预处理、编译、用户对象/页、真实复制、清理和运行均未认证。

## 5. R143-N01：两项finding的标准来源消费导航

原deferred第98–115行使用source描述，而非逐项展开source_ref。共享头已经给time、固定短提交及实际输入路径，每项给唯一函数/场景，正文和source_index可唯一恢复来源；没有证据缺失、错配或技术扩大。18号要求可追溯内容，但未设P3/P6那样“键别名即退回”的机械门，本次不因此整批退回。

主线明确补下列规范导航，不能倒写成“作者原件本来就有逐项完整source_ref”；旧文件保持。本补注由主线负责，不要求144重交旧91窗或另造D00。

两项共同source_ref：base_snapshot=time（分支名），source_pin_short12=a039d9803ade；原记录为0e70d0a148b7固定的deferred.new_findings，并合读同提交evidence/source_index和facts。

| 原qualified_id后缀 | 真实源路径与符号 | 既有证据/模型selector |
|---|---|---|
| INT_SUM_BOUNDARY | mykernel/sched/forkexec/exec.c::bprm_stack_limits；mykernel/include/linux/fs/binfmts.h::linux_binprm；mykernel/include/linux/kernel/minmax.h::__cmp/max/max_t | E143-013、015–017、021–027；deferred.$.new_findings[0] → facts.$.A[5]（id=A_INT_SUM） |
| PREDEBIT_REJECT | mykernel/sched/forkexec/exec.c::copy_strings/copy_string_kernel/do_execveat_common/kernel_execve | E143-037–039、045–049、072–077、083–087；deferred.$.new_findings[1] → facts.$.C[1]/[3]/[5]（C_OVER/C29/C19） |

qualified_id均保留前缀CORE-USER-EXEC-ARGUMENT-LIMITS-143::。前者仍为条件算术证据缺口、needs_evidence；后者仍为局部状态观察、recorded_no_runtime_claim。没有新增已证运行缺陷、修复或Owner当前决策。144及以后要求每项直接写source_ref，或在每项明确继承公共快照/路径并给可解析selector，不靠读者猜测。

## 6. 后继

第144批按[200号合同](../200-user-brk-allocator-contract.md)转向仓内用户brk返回值及分配器消费，约三个关联专题、六件交付。143本身不需返工；主线只做144设计预检，不冒充其执行成果。

继续原C10a/EXEC-22、次一档GPT-6.2 Sol及Owner可用高推理强度，完成后交C10 Pro独审。双向通知依198保持一个简短取件块，详细材料在GitHub；C10本轮144正式发布成功后设计数从3到4，12/15未达到。

阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机与真实操作均NOT_RUN。仅原lead新增审查/合同/检查点及必要PR导航/正式评论，不改源码、执行分支、旧成果或旧评论，不新建分支/PR、不合并、强推或删分支。
