---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01
record_type: chat_execution_contract_and_scoped_writer_assignment
conversation_display_name: MYOS2-A-C03 内核分析主线续建
produced_by: ChatGPT普通对话；Owner本轮报告已选择gpt pro
model_per_owner: gpt pro
backend_identity: unknown_or_not_attestable
date: 2026-10-31
read_channel: connector
evidence_class: 后续执行授权与验收条件；不是修正成果或新的验收结论
authorization_ref: Owner要求后续执行迁移到独立ChatGPT对话、确认新对话已建立，并于本轮要求继续；工作令、18号、24号与25号约定
status: READY_FOR_OWNER_FORWARDING
executor: MYOS2-LEAD-002-CHAT-EXEC-01
executor_display_name: MYOS2-A-C03a 代码分析执行
execution_model_per_owner: GPT-6
execution_effort_per_owner: Extra
receiving_model_tier: next_tier_default
receiver_state: RECEIVED_READY_WAITING_FOR_TASK_REPORTED
receiver_evidence: 2026-10-31对话检索取得C03a先前接收答复摘要；非主线独立工具复测，不是GitHub回执文件
required_review_record: CORE-USER-RETURN-SIGNAL-09-REVIEW-001
returned_item: SR09-01
instruction_base_short12: 5c5773bdd7de
source_pin_short12: a039d9803ade
result_base_short12: 67b2458b4a6a
default_branch_observed_short12: e8e60cd19540
canonical_write_branch: claude/dazzling-cori-q0dnyt
canonical_pr: 17
allowed_new_file: agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09-doc-fix-01/SR09-01-disposition.md
existing_files_frozen: 67b2458b4a6a中的全部文件；包括09原件
writer_assignment: C03a唯一执行写入者，仅限本任务；完成后停写待C03验收
activation_gate: Owner将本任务入口发给C03a；C03a核对已有接收与写前条件后，同轮执行到交付
previous_executor_write_authority: 本主线撤回CC02在该执行谱系中的后续写入委派，不再并行派工；不等于确认本地进程已经停止
physical_old_process_state: unknown
external_execution_started: false
phase: 2
kernel_change_authorized: false
runtime_evidence: NOT_RUN
phase3_authorized: false
merge_authorized: false
acceptance_ceiling: PASS_PENDING_LOCAL
owner_decisions_requested_now: []
inputs_read:
  - 工作令的本轮可见段落及既有加载记录；18号全文；25号第91-142行；09回执第146-204行。
  - 09固定主说明第530-612行可见部分及第617-640行；未重跑09、未全量读取大型YAML。
  - PR16/17元数据、完整变更路径、heads直接引用；开放PR按创建时间排序每页一项的第3页为空。
  - Mnemosyne单一PR谱系guard第1-155行；仅定向复核，不声称本轮重载整套指导。
open_questions:
  - C03a尚未执行本任务，写工具实际调用成功与成果发布需由执行回传证明。
  - 旧本地工作树及未推送内容未核；不得猜测已保全，也不以此要求重做既有远端成果。
---

# C03a首个工作包：只处理SR09-01，然后交回主线

## 1. 操作入口与完成效果

执行模型为Owner指定的GPT-6＋Extra。沿用已建立的「MYOS2-A-C03a 代码分析执行」，不新建会话，不重做25号完整接收。其接收答复已报告只读GitHub和沙箱文本预检成功、写工具可见但未调用；这只是执行方报告，不能写成主线已独立复测。

目标只有一个：为09主说明中把有限检索过度概括为确定返回分支的表格行，交付可直接替代其读法的追加勘误，并核查同义摘要。不是修内核、不是重新审计09、不是寻找新的重启码生产者。

Owner启动句：

> 执行26号任务CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01。沿用已有接收和GPT-6＋Extra；按任务书完成定点修正、自查、原PR17提交和远端回读。只新增指定勘误文件，全部旧件冻结；完成后交回C03验收，不自行开始下一批。

发布任务文件或PR评论不自动唤醒新对话。Owner转发入口后，无需再为内部核查、写入或回读逐项请示；只有实际访问失败、输入冲突或不明并行写入才停下报告。

## 2. 本批唯一写入授权

在Owner已经授权执行迁移与主线继续派工的范围内，本任务把25号尚未授予的执行写区具体化：C03a复用PR17和现有分支，不改名、不新建分支、不新建PR。分支名中的claude只是历史名称；新文件署名为C03a，不冒充CC02。

从本任务发布起，CC02不再持有本主线委派的后续执行写入权；若其以后恢复，也必须先回主线重新分工，不能按旧23号任务继续写。此为职责授权的更替，不是对旧设备、进程、凭据或未推送文件的远程确认。C03a实际开始写前必须再次检查远端；看到不明变化就停写，不覆盖、不回退。

C03仍独占agent/MYOS2-LEAD-002上的lead记录，并负责最终验收；本轮不会替C03a写执行成果。C03a只可新增上方allowed_new_file这一件，并在PR17追加自己的交付评论；不得更新PR16/17标题、正文、状态或他人评论。67b2458b4a6a中的全部旧文件原样保留。

本授权仅持续到本工作包交付，之后等待主线验收，不包括自行选择下一研究题。主线原有RETURN不因文件发布或执行者自查而改变。

## 3. 固定输入与最小读取

先读本任务全文，再读取instruction_base_short12中的09回执全文（reviews/CORE-USER-RETURN-SIGNAL-09-review.md，重点§5）。18号与24/25号继续适用，已成功加载且版本一致的内容不重跑接收仪式；本任务只对本批范围、写区和单文件交付作具体授权。

旧材料固定从67b2458b4a6a读取，基目录为agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09/：

- user-return-signal.md：§0、D03.3–D03.5、衔接L1/L2，重点L1“08 §3.2各行接上③之后”的第一行“在表中，处理函数返回”。读取第一行全部单元格，不只改“可能命中的行”列而留下“没有投递handler时是分派值”的无条件概括。
- stdio-08-disposition.md：§0、§3.1、§3.3–3.6，作为同义摘要检查范围。
- facts-and-boundaries.yaml、deferred-findings.yaml：只定位与上述摘要直接相关的能力、RR行、SR08-02/ER-02及被实际引用条目，带上下文读取；不全量重读8229/1101行，不复制整表。
- MANIFEST.md只核交付身份；无需重读10661行queries、重跑rs_check或原批次核对。

源码固定a039d9803ade：先核mykernel/scripts/options_flags.cmake；再定点回读arch/x86_64/include/asm/syscall.h的syscall_get_nr/syscall_get_error，以及arch/x86_64/lock_IPC/signal/signal.c的handle_signal/arch_do_signal_or_restart。需要证明分派或建帧顺序时，仅补读arch/x86_64/entry/common.c、entry/common.c和signal_64.c的对应函数。必要常量回其真实定义；不扩展到全部系统调用返回值、外部libc、FPU或运行验证。

同义摘要检索可以在上述固定文档中使用“RR-07”“RR-14”“按名字”“字面量”“没有穷尽”等词，但这些查询只用于文档位置清查，不能声称完成了全树返回值可达性证明。默认分支搜索只可导航，判断必须回到固定对象。

## 4. 同轮完成A–D，不另停小步骤

**A 绑定。** 核对自己的25号接收结果、任务ID、09回执record_id及SR09-01；核PR17为open/Draft、执行分支首次写入前仍为67b2458b4a6a。主线正常追加任务导致lead前进不算源码漂移。time变化时仍可完成历史固定对象勘误，但必须明确不覆盖新源码；未知执行头变化按§6停写。

**B 处置。** 引用原表行及D03.5中未穷尽计算返回值的原句，给出accept/partial_accept/evidence_disagreement及理由。修正整行的相关单元格，保持表格可读；区分接口对orig_ax的门限、实际ax错误码、是否选到handler、建帧成功/失败和控制未到返回段。orig_ax判为-1只跳过重启switch，不跳过信号交付。命中四个重启值时按对应分支说明；其余值才可归入非重启分支。缺少有效return和未穷尽的计算结果都不作确定分类，也不能倒推“已发现新生产者”。

**C 一致性与自查。** 逐个登记同义摘要检查的位置、受影响ID、是否需要限定及理由。已完整保留条件的条目记“无需更正”，不为凑数改写；相关RR记录不因为被引用就自动判错。区分“数值恰好相同”与“帧从未被写”。保留既有08限定关闭、09其他保留成果、RS-01 needs_evidence和GAP-U04/U10边界，不重开已经裁定的五个组织问题。

**D 发布。** 只将本批单文件勘误发布到指定位置，按§6验证后回传PR17。若发现与本项直接相关的实质异议，保留候选及证据，标明尚待主线裁定；不伪造“无异议通过”，也不扩大成新一轮系统性分析。

## 5. 单文件交付与验收条件

仅新增：agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-return-signal-09-doc-fix-01/SR09-01-disposition.md。

这是09回执§5已经规定的单文件处置，不另造MANIFEST、图、脚本和大型事实库；该文件内合并清单、勘误、证据与核对小节，承接25号通用交付字段。无需修改公约、旧报告或旧YAML。

YAML头至少含task_id、track_id、followup_id、executor_identity、produced_by、model_per_owner、effort_per_owner、date、read_channel、base_snapshot、inputs_read、status、open_questions、target_review_record、target_item、supersedes_locations、affected_ids、runtime_evidence: NOT_RUN、mainline_acceptance: PENDING。公开只写工具返回的短提交号，完整Git身份仅供内部比较；不写个人路径、账户背景或凭据。

正文必须包含原位置与短引文、范围冲突说明、可直接采用的替代整行/附注、同义摘要检查表、至少覆盖门限和两个switch的定点源码证据、实际自查方式及未做事项。强制源码结论给真实路径/符号及1–5行逐字引文，所有本批强制引文均回源核对；未运行的检查写NOT_RUN，人工核对不可冒称脚本结果。

文档完成后以READY_FOR_MAINLINE_REVIEW回传，不自行把09改为PASS。主线验收重点为：原问题被准确定位；返回分类不再超出证据；同义摘要无遗漏的无条件结论（限实际检查范围）；引用和身份正确；diff仅指定单文件新增。主线最多按既有PASS_PENDING_LOCAL上限作有限接收，运行与全局正确性没有被证明。

## 6. 写前、写后与中断恢复

写前核实际default/master引用（准备时e8e60cd19540）、time、PR17执行头、开放PR与相关变更路径。主线本轮已核只有PR16/17开放，两者变更分别在lead/results；没有本勘误路径。执行方仍须在写前复查，不能把PR元数据里旧base_sha当最新master。若目标文件已存在，先按固定对象读取、确认是否本任务同一成果；不得覆盖或盲目再提交。

本批可以仅用GitHub连接器完成；可选在自己的临时沙箱用已有Python作字符串/YAML文本核对，不安装工具、不执行仓库脚本、不预处理/编译/运行C或ASM。沙箱不等于Owner本机，不要求建立本地clone或登录旧产品。工具可见与真实写入成功分开记录，首次写调用只用于这个正式成果，不另建探针文件。

提交时显式指定claude/dazzling-cori-q0dnyt；建议提交前缀为CORE-USER-RETURN-SIGNAL-09-DOC-FIX-01:。读取最新远端头后立即创建这一新文件；连接器未提供分支头原子比较参数时，如实保留竞态限制，不宣称持有GitHub锁或全局原子防并发。

写后从返回的固定提交完整读回文件，再按分支读回确认内容和blob身份一致；compare 67b2458b4a6a到本次结果头必须仅有指定文件A新增、无旧件修改/删除、无额外未知提交。确认PR17仍open/Draft、分支头与结果一致；若有不明漂移/越界，立即停写并报告，禁止用重置、删除或force修复。自己的写入只触及授权路径；不要把主线同期合法前进误报为自己越界。

远端回读和diff结果写在PR17追加评论中，不为了自引用结果提交而反复修改已发布文件。评论包含实际短提交、结果链接、完成范围、核对方式与剩余限制。评论权限不可用时，成果仍保留，向Owner回报准确提交让主线读取，不因此另开PR。

若创建调用超时或结果不明，先读取远端路径与历史查明是否成功；已成功则只补回读/回传。发现本任务现有成果后不再覆盖，任何后续修正交主线另行绑定。访问/写入确实失败时保全已完成材料并报告具体错误，不要求Owner搬运完整报告。

## 7. 禁止动作与回传

继续18号问题延期规则。禁止修改内核/用户程序/旧脚本/旧成果，禁止生成内核修复补丁、安装工具、挂盘、构建、QEMU、读取仓库外libc/镜像、直写master/time/lead、合并/关闭PR、删除/重命名分支、force-push。不得启动额外模型/API、复制历史大规模子代理配置或进入阶段3。

工作包内A–D连续推进，不逐项索取确认；交付后停止，回报“已提交PR17，短提交…，SR09-01待C03验收”，不自行续做10或其他任务。Owner只需把完成状态通知C03，正文由主线直接从GitHub读取。
