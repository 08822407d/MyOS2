---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
record_type: phase2_mainline_independent_review
followup_id: CORE-USER-EXEC-STACK-PLACEMENT-142
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
status: PASS_PENDING_LOCAL
self_check: {verified_claims: 0, quotes_reconfirmed: 0, downgraded_to_inferred: 0}
self_check_scope: "本记录不新增正式源码引文标记；被审142新73窗与141补正旧129窗独立计量"
full_local_P9: NOT_RUN
owner_decisions_requested_now: []
acceptance_verdict: PASS_PENDING_LOCAL
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-22
execution_parent_short12: 08f6fee103dd
author_receipt_id: 7126024963
new_return_items: []
D00_closed: [R141-01]
D00_closure_kind: CLOSED_DOCUMENTATION_ONLY
combined_141: PASS_PENDING_LOCAL
prior140_status: PASS_PENDING_LOCAL
prior_D00_still_closed: [R138-03, R139-01, R139-02, R139-03, R139-04]
inputs_read:
  - "e591efa48b3d固定197合同、141独审、检查点；142固定/活动六件及作者评论"
  - "本轮只读复核组回源核73新窗及141旧129窗，分别13/11份固定源码；复核完整D00映射"
  - "主线直接读binfmt_elf建表/调用片段、exec计数/复制/限额/调用者及长度实现，另查配置与金丝雀"
  - "140 A1/A2/C_ENTRY与141 A0/Afd固定机器模型由模型复核组重新组合"
  - "110/193/194及工作令相关原话；协议v2与直接master公约；其余已完整加载规范身份未变"
review_limits:
  - "证据/D00、数值/阶段/图由两组只读分工交叉核对，最终裁定由C10作出；不把作者自检当独审"
  - "仅连接器、纯文本/JSON与算术；完整本地P9、实际预处理/编译/运行均未做"
  - "API Git Blob与固定/活动全文身份核对，未独立重算Git Blob SHA-1"
  - "作者私有工具操作全程未取得，不反推认证其发表后的全部内部时序"
open_questions: ["既有C对象/指针、vma、真实映射/用户访问、CRT/CPU状态及运行边界保留"]
---

# 第142批独立验收与R141-01判闭

**142裁定PASS_PENDING_LOCAL，无新增RETURN项。R141-01关闭为CLOSED_DOCUMENTATION_ONLY；141原件与142增补合并消费状态为PASS_PENDING_LOCAL。** 原[141的RETURN_DOCUMENTATION裁定](https://github.com/08822407d/MyOS2/blob/e591efa48b3d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-EXEC-AUXV-PAYLOAD-141-review.md)保持为历史记录，本文件追加后续判闭，不覆盖旧文件或旧评论。

140的PASS_PENDING_LOCAL及R138-03、R139-01至04五项历史文档判闭保持。此次关闭只针对AI文档格式/自检记录/消费索引，不是内核问题修复，也不是本地或运行验证通过。

## 1. 固定交付与本轮直接远端核查

[197合同](https://github.com/08822407d/MyOS2/blob/e591efa48b3d/agent-workspace/lead/MYOS2-LEAD-002/197-user-exec-stack-placement-contract.md) · [142作者#7126024963](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7126024963) · [固定成果目录](https://github.com/08822407d/MyOS2/tree/a911aa471305/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-exec-stack-placement-142/) · [固定提交a911aa471305](https://github.com/08822407d/MyOS2/commit/a911aa471305)。

执行提交唯一父08f6fee103dd；ahead=1、behind=0，恰一个提交、六件新增、8008行，零其他修改/删除。本轮取得固定与活动六件，逐件全文、LF数量、UTF-8字节及API Blob相同。

| 文件 | LF行数 | UTF-8字节 | Blob短码 |
|---|---:|---:|---|
| MANIFEST.md | 517 | 34741 | 77fbb4537cca |
| exec-stack-placement.md | 288 | 32204 | 027e02fc55b7 |
| facts-and-boundaries.yaml | 6216 | 190993 | 09721aa960df |
| exec-stack-placement-paths.dot | 220 | 20797 | eb62d9f302d0 |
| deferred-findings.yaml | 128 | 4205 | 682cb85ab8e0 |
| evidence.md | 639 | 22199 | c7ee796026e6 |
| 合计 | 8008 | 305139 | 六件相合 |

本轮预发布直接refs：lead=e591efa48b3d、exec=a911aa471305、time=a039d9803ade、master=e8e60cd19540。短码均从实际连接器输出复制前十二位。PR16 Open/Ready，PR17原题Open/Draft，原head/base及未合并状态相合；开放PR分页2/0，仅16/17。

142作者评论真实ID双读正文相同；PR17顶层评论无since分页100/100/76/0，共276个唯一ID。140作者/独审/五项D00关闭、141原发布/改派/恢复包准备/作者/独审、142发布/作者共十个指定标记各唯一；旧EXEC-21的141作者标记为0。作者正式评论保留其发表时G04待做状态，Owner另转发后续G04=PASS、STOP_WRITING；主线按本轮真实远端状态独立复核，不要求改旧评论，也不认证作者私有操作全过程。

## 2. D00：R141-01逐项关闭

原退回依据为协议P3/P6/P9及公约§1.5：141 MANIFEST缺标准字段、实际自检三整数和当前六件消费索引。142补正位置为MANIFEST的D00段及旧件索引、facts.D00.R141-01，以及deferred的D00提交记录。

| 原缺项/关闭条件 | 本轮实物与独立复核 | 裁定 |
|---|---|---|
| 读取通道与来源字段 | D00使用read_channel=connector、base_snapshot=time（分支名），固定短提交独立列出；旧字段到新字段逐项映射 | 满足 |
| startup与canary标准键 | startup_selfcheck_quote、branch_canary_quotes实际存在；启动整句在直接master命中，两canary在time命中且直接master不命中 | 满足 |
| 旧141自检三整数与真实范围 | 旧六件正式标记分布0/0/0/0/0/129；D00记录129/129/0、等式及RECONFIRMATION_DURING_BATCH_142_ONLY | 满足 |
| 逐条复核可追溯 | 129条reconfirmation与旧source_index的ID/path/symbol/物理范围/kind逐项一致，old_selector和源Blob正确；本轮独立重新取得11源并核129窗全部逐字相合 | 满足 |
| 当前重核不追认旧时序 | 补正明确只描述142期间的重新核对；没有声称141发表前已经完成未记录的P6 | 满足 |
| 141旧六件用途/覆盖/消费 | 六件均有purpose、coverage、consume、原路径和固定ref，MANIFEST与facts对应 | 满足 |
| 旧件冻结与判闭权限 | 142提交仅六新增；142固定树中141六件Blob与原固定对象相同；作者只提交documentation_disposition_submitted，未自行判闭 | 满足 |
| 新旧计数分账 | 旧129仅属于D00，142根self_check及新正式标记均为73；没有旧窗重复贴入新证据并混算 | 满足 |

MANIFEST补正对象与facts的共有字段一致；MANIFEST省去129条数组、改用明确selector和count，不缺少索引。当前142六件也有独立逐文件消费表，根read_channel、startup_selfcheck_quote、branch_canary_quotes及self_check均用标准键。

因此R141-01没有剩余关闭条件。后继消费141时应合读：冻结141六件＋原141独审的技术消费限定＋142中的D00增补＋本次判闭。原141的C1模板R条件及图中L为最终bias的既有澄清继续有效；关闭文档项不取消那些限定。

## 3. A/B：地址、区间与关键槽

模型组从冻结140的A1/A2/C_ENTRY及141的A0/Afd原记录重算，再与142合同、正文、机器件及图标签比对；不是只照合同期待表核数。

| 模型 | 平台P | 最终S | auxv起点Q | 排他上界E | 六区间字节数（低到高） | 总跨度 |
|---|---|---|---|---|---|---:|
| R1 | 0x6fffffd9 | 0x6ffffed0 | 0x6fffff08 | 0x6fffffd8 | 56/208/1/7/2/30 | 304 |
| R2 | 0x6fffffd9 | 0x6ffffec0 | 0x6ffffef0 | 0x6fffffd0 | 48/224/9/7/12/20 | 320 |
| R3 | 0x6fffff99 | 0x6ffffe80 | 0x6ffffec8 | 0x6fffff98 | 72/208/1/7/3/93 | 384 |

全部地址、U偏移、mod8/mod16与源公式相合；18个主半开区间连续、无重叠遗漏，22个前缀槽和9对选定auxv的字序/地址/值正确。三例33行地址表、65行区间/字符串/槽表、42行阶段表，共140行正文表记录与机器件逐字段相同。

R2的非空argv[0]指针指向单NUL，与终止空指针不同；R3两份/boot/sh内容相同但地址不同。两个padding区间、既有字符串区、平台串和saved_auxv的内核清零/用户复制范围保持分层，没有把局部未专门清零升级成已证泄露或真实残留。

整数投影中S为16字节对齐、槽为8字节对齐，而P/sp_aux/raw_bottom均mod8=1；该数值结论不证明C对象/指针转换及运算合法。新的U与入口p0是合同给定，不倒推真实栈映射。

依据固定[STACK宏及建表计算](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/binfmt_elf.c#L110)、[arch_align_stack](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/arch/x86_64/sched/context/process_arch.c#L168)及142 evidence/source_index。

## 4. C：42个观察阶段与caller消费

三个14阶段对象的local p/sp、bprm.p/exec/counts、u_platform、mm字符串边界及start_stack全部相合：

- T02对齐后sp仍为入口p0；T03平台分配后local p=P，bprm.p仍为p0。
- T05只先改bprm.p为S；T06才重置sp。T07写argc时local p仍为P，T08才重置p及mm.arg_end。
- T09 argv循环结束时局部argc=-1，mm.arg_end仍为p0；T10经过终止槽及字段赋值后更新边界。
- T11 envp循环结束为p=F、sp=Q−8，mm.env_end仍是env_start；T12终止槽和字段更新完成才为sp=Q、env_end=F。
- T13 auxv复制不推进sp到E。T14将已出作用域局部变量明确记为OUT_OF_SCOPE，独立caller模型中的mm.start_stack与START_THREAD栈实参均取S；bprm计数不受局部后缀递减写回。
- 所读finalize_exec定义体只有注释，没有活动语句修改bprm.p；这仍不等于实际CPU SP或CRT已绑定。

源码依据为[binfmt_elf.c::create_elf_tables](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/binfmt_elf.c#L274)、[表返回门及start_stack](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/binfmt_elf.c#L1075)、[最终栈实参](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/fs/vfs/binfmt_elf.c#L1130)、[上游字段平移](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L478)及[finalize_exec](https://github.com/08822407d/MyOS2/blob/a039d9803ade/mykernel/sched/forkexec/exec.c#L1055)。vma门前、门后及caller是不同条件片段，不拼成真实端到端运行。

## 5. 证据、图及协议可达核查

新正式标记仅evidence中73处；E142-001至073及source_index无缺号/重复。13份固定源的73个不同物理窗口均为1–5连续原文行，路径、符号、性质、源Blob及支持范围相合。性质为活动函数30、宏20、局部声明2、条件门1、注释与活动门混合1、定义头1、纯注释1、类型别名1、类型定义5、结构成员8、构建声明3。E142-036只支持签名类型，活动对齐另由038支持，未把定义头或注释当作执行证据。

process_arch.c有173个LF而有174个物理行，末行无LF；E142-038的173–174是合法范围。交付统计的LF行数与证据的物理行定位分开，不能以LF数误判末行越界。

95个唯一图节点的id/kind/label/scenario、100条唯一边的from/to/kind/label均与facts逐项一致；无重复、悬空或孤立。T06→T07明确独立门后模型，T13→T14为条件caller模型；上游连接仅说明字段来源，不证明实际平移成功。

按协议P9清单号：1六件长标识扫描0；2/13新73窗、旧129窗与强制锚点已核；3/4路径及附带范围可定位；5新73/73/0与旧129/129/0分别闭合；6无无出处外部技术结论；7/8实际canary/startup及标准字段相合；11无确定性运行禁用措辞。9/10无实现代码或补丁，12非降级交付，均不适用。完整本地P9仍NOT_RUN。

## 6. 后继与沟通调整

第143批按[199合同](../199-user-exec-argument-limits-contract.md)转向参数预算、单串长度上限和拒绝时点。139的小向量/充足预算、23/24的页引用/跨页以及70/71清理边界作为已核基础，不重做旧专题；新工作只展开限额边缘输入及条件性后继消费。

Owner指出本主线恢复后双向转发过长。本轮按[198号](../198-repository-first-compact-relay.md)落实：详细合同/证据/回执仍在GitHub，聊天仅一个简短取件块。142遵照当时197/194的回件不能因后出的简洁要求被追溯退回；今后两方向立即按新约定。

阶段2；完整本地P9、预处理、编译链接、MyOS2程序/脚本、Graphviz、QEMU、实机及真实操作均NOT_RUN。未初始化vma、GUP/页对象、真实映射/用户访问、C指针合法性、CRT/CPU状态等原有缺口继续保留。无源码修改、执行分支写入、旧件/旧正式评论改写或阶段3授权。
