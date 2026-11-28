---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_executor_contract
followup_id: CORE-USER-EXEC-ARGUMENT-LIMITS-143
conversation_display_name: "MYOS2-A-C10 内核分析主线续建"
produced_by: "GPT-6 Astra Pro（当前系统标识；实际后台与effort未独立认证）"
model_per_owner: "Owner本轮明确要求Pro独审及同轮后继设计"
date: 2026-11-24
date_timezone: Asia/Singapore
phase: 2
base_snapshot: "time（分支名）"
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: e591efa48b3d
execution_pin_short12: a911aa471305
read_channel: connector
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
status: PUBLISHED_PENDING_EXECUTOR_START
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "本记录不新增正式源码引文标记；被审142新73窗与141补正旧129窗独立计量"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-22
executor_display_name: "MYOS2-A-C10a 代码分析执行"
executor_model_per_owner: "次一档GPT-6.2 Sol；沿用Owner界面可用Extra/高推理强度"
actual_batch143_receiver_state: NOT_OBSERVED
canonical_execution_branch: claude/dazzling-cori-q0dnyt
canonical_execution_PR: 17
allowed_directory: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-argument-limits-143/
allowed_files: [MANIFEST.md, exec-argument-limits.md, facts-and-boundaries.yaml, exec-argument-limits-paths.dot, deferred-findings.yaml, evidence.md]
D00: []
mainline_design_count: {conversation: C10, before_release: 2, after_successful_formal_release: 3, first_batch: 141, latest_batch: 143, threshold_12: NOT_REACHED, threshold_15: NOT_REACHED}
inputs_read:
  - "142固定六件与本轮独审；197原合同；141补正判闭"
  - "139的小向量基准、140对139判闭及A1字符串；23/24及70/71的限定边界"
  - "固定time exec.c限额/测长/复制/调用者；strnlen_user与generic strnlen；限额、类型及minmax定义"
  - "本轮Owner简洁双向转发指令与198；110/124/193/194及其余现行约束"
open_questions: ["143接收、执行及实际G04尚待发生；真实内核/用户访问与运行仍未证"]
---

# 199号合同 · 第143批 · exec参数预算、单串长度及拒绝时点

发给现有「MYOS2-A-C10a 代码分析执行」／EXEC-22，次一档GPT-6.2 Sol及Owner可用Extra/高推理强度。Owner转发本次简短正式发令即授权执行到交付、实际G04和停写，完成后交C10用Pro独审及同轮后继设计。

**先完整读取[198号简洁通信规则](198-repository-first-compact-relay.md)。** 任务细节和正式回执放GitHub，聊天只给简短取件通知；不再复制整份合同、统计表或长检查日志。此规则同时约束主线启动和你的最终回件。

[本轮142独审](reviews/CORE-USER-EXEC-STACK-PLACEMENT-142-review.md)为PASS_PENDING_LOCAL，R141-01已CLOSED_DOCUMENTATION_ONLY，141原件与142增补合并消费状态PASS_PENDING_LOCAL；140及五项历史D00保持。本批D00为空，不重做旧129窗或重发141/142作者评论。

## G00 · 接续、输入身份及写前核查

1. 读同次固定发布的本合同、198、142独审及[本轮检查点](checkpoints/2026-11-24-stack-placement142-reviewed-argument-limits143.md)，按Owner短通知入口取得本轮主线正式评论并用真实ID双读。该评论同时含142独审、D00_141_CLOSED、143发布三个主线标记。
2. 同一有效C10a对话可按79复用自己已完整读取且版本未变的原十三份项目约束；本轮新增198必须完整读。缺读或上下文缺失补齐，不继承其他身份的“已读”。[196原清单](https://github.com/08822407d/MyOS2/blob/5ab41ade828b/agent-workspace/lead/MYOS2-LEAD-002/196-executor-continuation/00-C09a-to-C10a-recovery-package.md)及[194来源导航](https://github.com/08822407d/MyOS2/blob/e591efa48b3d/agent-workspace/lead/MYOS2-LEAD-002/194-owner-continuity-and-copyable-return.md)保留。Meta-Agent当前有效采用/同版本共用全文、Mnemosyne接收命名按现行来源核是否变化；没有生效变化则准确记录复用，不导入其他项目写权。
3. 直接读四refs：lead应为本合同同次正式发布提交，exec=a911aa471305、time=a039d9803ade、master=e8e60cd19540。PR16 Open/Ready，原题PR17 Open/Draft，原head/base及未合并状态保持；直接master不能用PR base_sha替代。
4. 开放PR及PR17顶层评论无since完整分页至空；按本轮142独审的十个历史/当前标记，加本次主线三个标记检查唯一；用真实ID读142作者和本轮主线评论。142作者为[7126024963](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7126024963)，本轮主线实际ID由短通知入口给出，不猜测。
5. 核142固定提交唯一父08f6fee103dd、ahead=1/behind=0、六新增/零改删；固定/活动六件全文、LF、UTF-8字节及API Blob相同，总8008行/305139字节。仅消费本次需要的139/140模型和已判闭历史，不全历史重审。
6. 首次写前核目标143目录及本身份143正式作者标记不存在、exec真实expected_sha。若发现相符交付，按63只续缺项；未知并行写先定位保全，不覆盖、重复发布或强推。C10本合同正式发布后累计3批，12/15未到达。
7. MANIFEST记真实读取/复用、标准P3/P6、逐文件消费、模型可核信息及实际G00。完成G00即连续做A/B/C、自检、发布、实际G04和短回件，不为内部阶段停问。

## 共同模型、增量及范围

139已有小向量、充足限额、单页字符串的成功片段；23/24已有跨页、页引用与GUP缺return；70/71已有清理/提交门。本批只展开这些旧基准没有实例化的**总预算边缘、单串上限和最早拒绝/状态变化**，不重做完整exec、页表或硬件研究。

只读固定time。先读options_flags。给定x86-64类型模型：int为32位有符号，ulong/size_t/指针为8字节；PAGE_SIZE=4096。这些是采用的算例类型/布局，不是实际编译测量。地址计算、预算扣减、长度结果及其转换均可表示且无回绕；A_INT_SUM专门检查不可表示的有符号中间和。generic strnlen耗尽计数后的无符号终止递减另按B处理，不受地址/预算无回绕前提排除。

B/C明确采用string.c中#ifndef __HAVE_ARCH_STRNLEN下的generic strnlen定义作为局部模型；需要引用条件门并与配置面分开，不宣称实际预处理已选中。字符串对象及本次会读的字节有效、内容在片段期间不变；user取得的字符串指针不是ERR，必要读取正常，均为给定条件，不是实际用户访问证明。

所有地址只是题设数值。A是独立限额helper模型；C的短预算是另外给定的复制阶段模型，**不是A对该argc/envc实际计算出的剩余预算**，也不自动等同142建表入口p0。

## A · 总预算、指针预算与p阈值（8组）

先从活动定义取证：_STK_LIM=8*1024*1024；ARG_MAX=131072；MAX_ARG_STRLEN=PAGE_SIZE*32；MAX_ARG_STRINGS=0x7fffffff。bprm_stack_limits活动体给L0=max_t(ulong,_STK_LIM/4*3,ARG_MAX)=0x600000=6291456。rlim_cur参与min的语句是注释，不能把它当成当前活动限额。

按真实宏展开与字段类型分析：(max(argc,1)+envc)先是int加法，再与sizeof(void*)运算；不能因为接收变量ptr_size为ulong便说加法先在ulong完成。只引用当前活动__cmp/max/max_t，不把注释中的宏实现当活动代码。

以下六例均给入口bprm.p=P0=0x800000，先前argmin=A_old，A_old是独立输入、未指定数值。逐例列L0、计数检查、加法的类型/值、ptr_size、是否执行减预算、bprm_set_stack_limit是否到达、返回/未知和argmin是否改变：

| ID | argc / envc | 校核要点 |
|---|---|---|
| A_NEG | −1 / 0 | 计数门-E2BIG；ptr_size未计算，argmin保留A_old |
| A_ZERO | 0 / 0 | 有效指针数1；ptr_size=8，余6291448，argmin=0x200008 |
| A_SMALL | 2 / 2 | ptr_size=32，余6291424，argmin=0x200020 |
| A_PTR_BELOW | 1 / 786430 | 有效指针数786431，ptr_size=6291448，余8，argmin=0x7ffff8 |
| A_PTR_EQUAL | 1 / 786431 | ptr_size=L0，在limit<=ptr_size处-E2BIG；减预算及argmin赋值未到达 |
| A_INT_SUM | 2147483647 / 1 | 中间int和超出所给32位有符号无回绕模型；ptr_size、正常errno及后续argmin一律NOT_DERIVED，不虚构回绕结果 |

A_ZERO是直接helper输入，不能据此说kernel_execve会允许零argc到达此处。A_PTR_EQUAL需区分数学剩额0与源码根本没执行limit-=ptr_size；A_INT_SUM不能被包装成正常-E2BIG返回案例。

再给两例独立bprm_set_stack_limit：A_SET_EQ为limit=8、p=8，返回0且argmin=0；A_SET_LT为limit=8、p=7，返回-E2BIG且argmin保留A_old。两个例子均不访问实际地址0，也不证明该地址可用于后续复制。

指出这里仅按max(argc,1)+envc预留指针字节；argc槽、两种终止NULL、auxv、平台串及字符串需求不是这些指针数的同义项。不要把6 MiB算术预算直接当作已映射容量或实际可接受参数总量。

## B · 单串上限与长度检查（4组，两helper并列）

M=MAX_ARG_STRLEN=131072，索引从0算。源generic strnlen最多读索引0至M−1；固定strnlen_user活动体只有return strnlen(str,count)+1。copy_string_kernel也取strnlen(arg,M)+1。先按本模型取r，再列测得len、有效性及拒绝时点：

| ID | 给定内容 | r | 两helper的len | 长度门 |
|---|---|---:|---:|---|
| B_SHORT | 首NUL索引M−2 | M−2 | M−1 | 通过，后续预算/页操作另判 |
| B_EXACT | 首NUL索引M−1 | M−1 | M | 通过，后续预算/页操作另判 |
| B_NEXT | 首NUL索引M；前M字节均非零 | M | M+1 | -E2BIG，在bprm.p扣减前 |
| B_UNTERM | 前M字节均非零，之后未知 | M | M+1 | -E2BIG，在bprm.p扣减前 |

B_NEXT和B_UNTERM的可观察测长相同；函数没有读取索引M，不据返回值推断界限之外是否有NUL，也不把M+1写成真实完整字符串长度。无需真的生成或扫描十几万字节数据，只用给定布局和循环条件推导。

B_NEXT/B_UNTERM耗尽M次读取后，for条件仍执行一次count==0时的count--：size_t无符号计数发生定义良好的回绕，但旧值0使&&短路，不读索引M。r仍为M、len仍为M+1。这与A_INT_SUM的有符号中间和不可表示是不同情形，不得混成同一种溢出或漏掉终止测试。

r+1、size_t到long及int的转换在这些数字下均可表示。正常给定输入的len不为0；注释中的访问检查/异常返回、被注释的do_strnlen_user及if(!len)分支文本，均不能用来证明当前实现具有用户故障恢复。实际无效地址、并发变化、用户权限检查和异常处理不在本批证明范围。

## C · 预算拒绝时点、预扣p及后继消费（7组）

### C_EQ / C_OVER：先扣p，再判预算

两种copy helper并列分析；给T=0x70000000、bprm.p=T、argmin=T−16，测得len分别16/17且已通过B长度门。

- C_EQ：预扣后p=argmin，通过空间门；只裁到允许进入取页/复制片段，不自动写return0或复制成功。
- C_OVER：预扣后p=argmin−1，-E2BIG；本次get_arg_page/复制未到达，p已改变且该局部没有回滚。
- 源码在两helper里均直接比较p<argmin；不能画入一个不存在于该调用点的bprm_hit_stack_limit调用。
- 与B_NEXT/B_UNTERM对照：同样-E2BIG，长度失败时p不变，空间失败时p已预扣。以源码实际到达点区分。

### C30 / C29：既有五串，最后一串越界

采用冻结140 A1内容：filename="/bin/tool"为10字节，argv为"display"(8)、"-q"(3)，envp为"E=1"(4)、"F=xy"(5)，长度均含NUL。给复制开始p=T，两例独立给argmin=T−30/T−29。按调用者与逆序数组处理，需求次序为10、5、4、3、8。

模型明确分层：给定前序局部效果或返回值供后段消费，不认证GUP缺return之后的真实成功链。记录需求/预扣字节与取页/写入是否到达；若按题设给出“条件已写字节”，须单列actual_copy_status=NOT_ESTABLISHED，不能从p变化或返回0直接推出真实复制完成。

逐阶段列p、argmin、bprm.exec、剩余需求、返回/首个拒绝门、本次取页/复制与后继调用是否到达。C30最后p=T−30=argmin，通过预算门；能否进入bprm_execve仍以其他复制操作正常返回为条件。C29最后argv[0]之前的条件需求/写入前缀为22字节；最后8字节预扣后p=T−30低于argmin，-E2BIG，最后8字节的本次取页/复制和后续bprm_execve均SKIPPED。在filename条件正常完成并返回0之后，bprm.exec=T−10；后续失败不会在所读局部自动撤销该字段。

### C20 / C19 / K0：零argc补空串与早拒

C20/C19只用于do_execveat_common的零argc路径。仍取上述filename及两环境串，给p=T、argmin=T−20/T−19；在进入补空串之前，条件性需求为19字节。argv复制调用计数0，不进入字符串循环。

- C20：补空串长度1，预扣到T−20=argmin；仅在该复制按给定正常条件返回0后，bprm.argc才赋1，后续exec调用条件可达。
- C19：补空串预扣到T−20后越界，-E2BIG；这1字节取页/复制未到达，bprm.argc仍为已给定0，赋1及后续exec调用SKIPPED。
- K0：独立给定kernel_execve已经到达count_strings_kernel返回点且返回0。它先把retval改成-EINVAL并跳清理，bprm.argc赋值、env计数、限额、filename/env/argv复制与bprm_execve均SKIPPED；不能把“计数结果0”写成“该处已给bprm.argc赋0”。

C的多阶段成功效果全是限定模型。23/24的GUP/页对象缺口及70/71清理边界保持；失败现场字段只记录到清理前，最终向外返回同一errno还须清理调用正常返回条件，不能凭free_bprm名称推断已回滚或必然返回。实际字节写入、用户访问、页表和exec成功仍未证。

## 有界输入及强制证据

历史输入以本轮142裁定、[139独审](https://github.com/08822407d/MyOS2/blob/e591efa48b3d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGUMENT-VECTORS-139-review.md)的A/B/C与R139-03限定、[140历史判闭](https://github.com/08822407d/MyOS2/blob/e591efa48b3d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-ARGV-ENTRY-VIEW-140-D00-138-139-review.md)及冻结140 A1为限；原件在cde2d7f994b5的user-exec-argv-entry-view-140/。143只取相关字符串/计数和消费边界，不重新审核140全包。

[23](reviews/CORE-USER-EXEC-STACK-23-review.md)R23-S01/S02、[24](reviews/CORE-USER-ARG-PAGES-24-review.md)R24-S01/S02只用于页/取页/片段边界；[70](reviews/CORE-USER-EXEC-RESOURCE-OWNERSHIP-70-review.md)R70-S02、[71](reviews/CORE-USER-EXEC-COMMIT-GATES-71-review.md)R71-S02只用于清理与返回条件。原文和后续已生效关闭状态同时消费，不因旧RETURN重开已闭项目。

新源输入限以下文件相关定义及必要上下文，短码固定a039d9803ade：

- mykernel/sched/forkexec/exec.c：valid_arg_len、bprm_set_stack_limit、bprm_hit_stack_limit定义对照、bprm_stack_limits；copy_strings、copy_string_kernel、copy_strings_kernel；do_execveat_common/kernel_execve的计数/限额/复制/补空串/清理消费；get_arg_page仅承接23/24的边界。
- mykernel/lib/string/strnlen_user.c::strnlen_user及被注释代码性质；mykernel/lib/string/string.c::strnlen及其#ifndef条件门。
- mykernel/include/uapi/linux/binfmts.h、mykernel/include/uapi/linux/resource.h、mykernel/include/uapi/linux/limits.h：四个限额常量；mykernel/include/linux/fs/binfmts.h：计数、p/argmin/exec成员。
- mykernel/include/linux/kernel/minmax.h：当前活动__cmp/max/max_t；必要类型/页大小来源为mykernel/include/linux/kernel/types.h、mykernel/mm/misc/mm_misc_const.h。模型需额外依赖时只有限补读并记原因。
- mykernel/scripts/options_flags.cmake、mykernel/debug/panic.c及直接master对照；直接master公约/协议。不扩到完整uaccess、GUP重做、任意页操作、CPU或真实运行。

强制正式窄窗覆盖常量/类型/真实宏、L0与注释rlim门、负数/指针预算/减预算/set_limit、两种测长和generic循环及条件定义、len==0/valid_arg_len/预扣p/空间失败顺序、取页复制位置、数组逆序和错误传播、两个caller的计数拒绝/限额/filename赋exec/env/argv/补空串赋argc/exec调用/清理返回门。每窗1–5连续物理原文行，性质和支持范围明确，定义与使用分开。无新外部技术事实依赖；若使用外部结论按协议给原始出处。

## 六件、规模与验收

全部十三条协议v2适用。仅新增指定六件；MANIFEST实际使用read_channel、startup_selfcheck_quote、branch_canary_quotes、self_check三个整数及逐文件用途/覆盖/消费索引。新正式标记全包计数，交付前重开对应固定源逐条核对，n=m+k；不把初次生成当重核，不写近似替代键。D00为空，不复贴或重计141旧129/142旧73。

仅上述19组输入：A六个预算例＋两个set_limit例、B四例、C七例。同一输入两helper并列，不再乘成无必要的新场景。C五串流水每例最多八个观察点；不展开巨型字符串、不按每个字节造节点。不设字数/引文数/新缺陷配额。

正文、facts及DOT使用相同场景ID；明确区分SOURCE_FACT、给定题设、整数模型、独立片段、条件caller及NOT_DERIVED/NOT_REACHED/SKIPPED。图与facts.graph按节点ID及边from/to/kind/label逐项一致。deferred只记本批新增和旧项状态变更；旧缺口不重复报新漏洞，普通问题不转成Owner当前决策。

P9按清单号核连接器可达项1–8、11、13；9/10无实现代码/补丁、12非降级时记NA。完整本地P9与实际预处理/编译/运行保持NOT_RUN。P1长标识扫描0，P2支持关系/强制锚点、P6真实计数、金丝雀及直接master startup均须实做。

## 发布、实际G04、停写与短回件

原exec使用真实expected_sha、force=false、唯一父提交快进；恰六件新增，零其他修改/删除。提交后固定/活动六件逐件核全文、LF、UTF-8字节和API Git Blob，净差异与父提交正确。发正式评论前无since全分页查重，原Draft PR17仅一条本批正式作者顶层评论，标记：

MYOS2-LEAD-002-CHAT-EXEC-22 / CORE-USER-EXEC-ARGUMENT-LIMITS-143 / FORMAL_DELIVERY

正式作者评论在GitHub保留完整身份/合同/固定提交/六件/实际检查/模型信息/成果及未证边界。评论发表后按真实ID双读、无since分页至空查本次唯一及十三个历史/主线标记，重核四refs、开放PR集合、PR16/17原状态和元数据、固定/活动六件/净差异、142冻结原件和作者评论。63暂态失败先查效果、限次间隔恢复，不重复发评或强推。

实际完成才登记READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、实测G04及STOP_WRITING，不自行启动144。最终回复按198只给一个简短原生复制块，标题“转发给C10：第143批回执”，正文通常如下（填真实值/URL）：

第143批已交付；主线验收PENDING；G04=<实测结果>；<真实停写状态>。
正式作者回执：<完整GitHub评论URL，包含所有固定成果与详细记录>。
请MYOS2-A-C10使用Pro独立验收，并同轮安排后继工作包。

确有部分交付/阻断，再补一句最小必要状态及取件入口；不要把六件统计、A/B/C长摘要或检查日志搬到聊天。全部接收所需内容由该正式入口可达；保留真实未知与失败，不冒签。

结果先行、授权内连续推进、人工动作集中、问题先入库、约三专题六件、次一档执行/Pro独审、同轮后继、63/193/194及198持续继承并带入未来交接。阶段2；不改源码、旧成果/评论、主线目录或PR元数据，不新建分支/PR、不合并、强推、删分支；完整本地P9、MyOS2代码/脚本、预处理/编译链接、Graphviz、QEMU、实机及真实操作均NOT_RUN。
