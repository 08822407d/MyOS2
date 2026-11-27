---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
conversation_display_name: MYOS2-A-C11 内核分析主线续建
produced_by: GPT-6 Astra Pro（本轮系统身份）
actual_backend_model: UNATTESTED
actual_reasoning_effort: UNATTESTED
date: '2026-11-27'
date_timezone: Asia/Shanghai
phase: 2
base_snapshot: time（分支名）
source_pin_short12: a039d9803ade
direct_master_short12: e8e60cd19540
lead_before_publication_short12: c2bb3f55604f
execution_pin_short12: bf0a7ea26dcc
read_channel: GitHub connector；自写有限文本/字节/整数核验
startup_selfcheck_quote: 2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。
branch_canary_quotes:
- "\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\"
- "\tthis_cpu = smp_processor_id();"
self_check:
  verified_claims: 0
  quotes_reconfirmed: 0
  downgraded_to_inferred: 0
self_check_scope: 主线本轮不新增正式源码引文窗；复核作者122窗、图补注及设计READY分别另账。
full_local_P9: NOT_RUN
actual_G04: PENDING_POST_PUBLICATION
D00: []
R158_01: CLOSED_BY_MAINLINE_GRAPH_CONSUMPTION_ADDENDUM
R155_01: CLOSED_BY_156_CHECKPOINT_READ_SUPPLEMENT (retained)
R152_01: 153主线既有闭合保持；152历史RETURN/未证不倒签
owner_decisions_requested_now: []
mainline_design_count:
  conversation: C11
  first_batch: 152
  latest_batch: 159
  before_publication: 7
  after_successful_publication: 8
  thresholds:
  - 8
  - 10
  reminder_at_this_release: 8
  automatic_handoff: false
previous_mainline_design_count:
  conversation: C10
  distinct_batches: 11
record_type: independent_delivery_review
followup_id: CORE-USER-LIBC-FORMAT-CAPACITY-158
status: PASS_PENDING_LOCAL
consumption_qualification: WITH_MAINLINE_GRAPH_ADDENDUM
mainline_acceptance: PASS_PENDING_LOCAL_WITH_MAINLINE_ADDENDUM
author_identity: MYOS2-A-C11c / MYOS2-LEAD-002-CHAT-EXEC-27
author_commit_short12: bf0a7ea26dcc
author_comment_id: 7208914225
model_per_owner: 本轮独立验收与后继设计使用Pro；下一执行GPT-6.2 Sol＋Extra／高推理
---

# 第158批独立验收 · 用户库格式容量、指针与长度消费

**裁定：PASS_PENDING_LOCAL；消费资格为 WITH_MAINLINE_GRAPH_ADDENDUM。** 十二行数值、字节、局部帧、源读/目的写、首次停止与条件请求成立。原逐行图存在关系标签混用；本审§4给出有界、完整的图义补注，登记并当轮闭合 **R158-01=CLOSED_BY_MAINLINE_GRAPH_CONSUMPTION_ADDENDUM**。以后须以原六件与本审§4合读；不把原图改报为全部正确。后继 **D00=[]**，不要求作者重做158或回改旧件。

作者为C11c／EXEC-27，唯一提交[bf0a7ea26dcc](https://github.com/08822407d/MyOS2/commit/bf0a7ea26dcc)，[正式作者回执](https://github.com/08822407d/MyOS2/pull/17#issuecomment-7208914225)。所核六件固定于该提交；源码time=a039d9803ade、直接master=e8e60cd19540；当前修订220、221接收及158恢复检查点固定于lead=c2bb3f55604f。作者实际G04/STOP_WRITING是其回报，主线另完成下面独审；本次发布的实际G04须发表后另做，不预签。

## 1. 远端身份、唯一交付及原件冻结

| 对象 | 本轮独立观察 |
|---|---|
| 执行提交 | bf0a7ea26dcc，唯一父abbf15de6fbb；compare ahead1/behind0，1提交、6新增、零其它改删 |
| 执行树 | 完整2674项／2277非tree；父树2667项／2271非tree逐对象mode/type/SHA保持 |
| 主线写前树 | c2bb3f55604f，完整2077项／1842非tree |
| 原PR | 16 Open/Ready、17 Open/Draft，原head、base master、未合并，开放分页2/0；PR17原标题保持 |
| 评论 | 无since全分页100/100/100/12/0，共312唯一ID；恢复后的311旧评论正文及created_at/updated_at逐项不变 |
| 回执与标记 | 作者7208914225、改派7208211734各直接双读并与分页同文/同时间；73既有精确标记各1且ID正确，16既有负标记0；发布159另增3个错误旧席负标记 |
| root G00 | 2413项元数据/四refs/规范/完整树/六件三渠道/标记及2271旧对象检查通过；311旧评论精确冻结另核，不将数量代替内容 |

六件固定提交、活动exec及API Git Blob逐字一致，另独取base64复核。每个Git Blob均用实际UTF-8字节独算，六件末尾均LF：

| 文件 | Blob短12 | 字节 | LF | CRLF |
|---|---|---:|---:|---:|
| [MANIFEST.md](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/MANIFEST.md) | 2d98c20ea47a | 14597 | 91 | 0 |
| [libc-format-capacity.md](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/libc-format-capacity.md) | b263108ffc37 | 6046 | 50 | 0 |
| [facts-and-boundaries.yaml](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/facts-and-boundaries.yaml) | 65e5d2d620b8 | 229998 | 9931 | 0 |
| [libc-format-capacity-paths.dot](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/libc-format-capacity-paths.dot) | 34ffdfdd36fb | 31984 | 603 | 0 |
| [deferred-findings.yaml](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/deferred-findings.yaml) | 7e3fe2957db5 | 2842 | 82 | 0 |
| [evidence.md](https://github.com/08822407d/MyOS2/blob/bf0a7ea26dcc/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/evidence.md) | 46457f39976d | 36014 | 1399 | 258 |
| 合计 | — | 321481 | 12156 | 258 |

## 2. 独立方法与分账结果

技术审校先仅取得GIVEN-only输入与固定源码，fresh核27份源码，封存十二行有限模型和源笔记，才读取作者六件及合同预期；未以作者答案反建预期。后读时6个封存身份未变。文证审校另fresh取作者六件base64、27份源的文件/Blob渠道及18份历史输入，逐窗核原字节和真实定义。root回源核printf的rv赋值、write调用及return，另核图补注覆盖范围。三个角色均未执行项目代码。

| 检查账 | 实际结果 | 解释 |
|---|---|---|
| 158作者前独算 | 179／179／0 | 27源码Blob+1revision身份、23源锚、128有限断言 |
| 后读作者比较 | 414项：410通过、4失败 | 381值/字节/读写/帧/取参/边界条件通过；12图序列通过；14图语义有3失败；1映射约定失败；6封存身份通过 |
| 4项失败归属 | 同一图表示问题 | 三C尾边错误类型及99条相邻边的非控制标签；由§4补注解决，原失败账不改成0 |
| 正式P6复核 | 作者122／122／0；独审逐窗122通过 | 不把复核次数再加到作者P6，也不把作者12383模型断言当主线独算 |
| 文证/接收机械账 | 2645+61=2706项，0机械失败 | 原字节、身份、位置、节点/边属性和公开规范记录；图义观察另列，机械0失败不等于原图无误 |
| 主线图补注核验 | 305项，0失败 | 完整263边映射、275事件不变、首停无出边、20总图关系及三C的guard/rv来源；不重跑已成立的381项比较 |
| 主线新增正式P6 | 0／0／0 | 使用作者已核E066–E068及普通源定位，无新增正式引文窗 |

主线图助手初稿曾把所有原边标签都假定为目标kind，12个SOURCE_CALL目的实际映成LOCAL_CONTROL；核实际原件后纠正助手映射。该准备错误不算作者新缺陷，初稿与最终核验账分存。作者准备调用上限及草稿相邻引文问题也与正式写入分开：最终E116已指向types.h L130的long int定义；公开远端未见重复正式提交或作者评论，私有全过程不代签。

122正式窗连续FC158-E001–E122、每窗1–5物理行，合计414物理行=258 CRLF+155 LF+1 NONE；唯一NONE为E108、stdio/CMakeLists.txt L3。源码、代码块、符号/条件实例、行范围、Blob及source_index归属逐项核同，不归一化CRLF。六组普通定位拆成17个实际token/范围，另账通过。format_int L41–202、vsnprintf L533–877、printf L11–26、STRLEN L29–104及所用main/cd/test/getcwd/myos_prepend完整定义已读；声明/包含/CMake文本仍不等于实际编译、有效ABI或链接。

## 3. 十二行技术裁定

| 行 | 条件推导与界限 |
|---|---|
| A01_N0/N1/N4/N5 | 逻辑return均4；末q分别0/1/4/4。N0无目的写但仍读源；N1的A和N4的Z先写后被NUL覆盖；N5另写NUL。D物理8字节与n分开 |
| B01_N4/N7/N8 | helper返回6，外层return7。N4的helper q=5、外层q=6是不同帧；N7的!被NUL覆盖；N8保留!后另NUL |
| B02_NARROW | D[4]中候选D+5越过one-past，止于format_int L167的q+=ndigits之前；此前无目的写，不签随后的o更新/逆写/return/NUL/最终D |
| B02_WIDE | D[8]可形成D+5；相同n=4只控制后续存储门。数字实际写序3→2→1→0，最终D3再写NUL；helper和外层return均5 |
| C01_L4085/L4086/L4095 | vsnprintf rv与write整数请求4095/4096/4105；首NUL和完整读取都在给定current_dir对象内；buf末NUL位置4095 |

C只消费myshell.c L86的一次提示符，不合并L85独立换行。stdout的描述符是给定已初始化int F，不默认1。最大请求比4096对象多9字节只表示整数区间[4096,4105)，不声称构造该对象外指针、实际读出、显示或泄漏。格式化目的存储没有越出buf；只有write正常返回时可达printf末尾return rv。实际sh绑定、getcwd生产、有效ABI和真实I/O均未证。

## 4. R158-01：完整图义消费补注与当轮闭合

原图290节点/283边＝source overview的15节点/20边＋十二行275事件节点/263相邻seq边。没有缺失、重复、悬空或跨case相邻边，B02_NARROW首停无出边。facts.graph_mapping声明相邻顺序边应为LOCAL_CONTROL；实际99条用了目的事件类型：WRITE_EVENT70、RETURN_VALUE25、REQUEST_ARGUMENT3、ANALYSIS_STOP1。其余164条已为LOCAL_CONTROL，包括12个SOURCE_CALL目的。这个原件差异是本批技术文档问题，不归咎旧合同，也不重新打开R129-01。

**以下为覆盖全部十二行的正式合读规则：**

1. 原263条相邻seq边全部按 `relation_type=LOCAL_CONTROL` 读取；保留原端点与先后。99条非LOCAL原标签仅作为 `target_event_kind` 元数据，不据此推导调用、数据来源或已经发生的读取/输出。
2. 20条source overview关系保持原型。实际调用拥有者和返回数据源由该总图及facts的frame/from字段证明；WRITE_EVENT表示有实际store的事件，REQUEST_ARGUMENT只表示请求。原case边不是替代这些来源关系的证明。
3. 三条C尾边除统一控制类型外，显式施加 `guard=if write normally returns`；补明下表rv数据关系。新增的数据关系是本主线补注，不假称原case图已含它们。

| 原DOT精确位置与控制边 | 补注后的控制关系 | 独立返回数据关系及值 |
|---|---|---|
| L489：C01_L4085_26→C01_L4085_27 | LOCAL_CONTROL；write正常返回时继续 | C01_L4085_25.rv→C01_L4085_27；RETURN_VALUE=4095 |
| L544：C01_L4086_26→C01_L4086_27 | LOCAL_CONTROL；write正常返回时继续 | C01_L4086_25.rv→C01_L4086_27；RETURN_VALUE=4096 |
| L601：C01_L4095_27→C01_L4095_28 | LOCAL_CONTROL；write正常返回时继续 | C01_L4095_26.rv→C01_L4095_28；RETURN_VALUE=4105 |

三条返回数据关系同样以write正常返回为可达条件。write的实际返回值UNKNOWN且被忽略；printf返回的rv来自vsnprintf。依据为[printf完整定义](https://github.com/08822407d/MyOS2/blob/a039d9803ade/myinitramfs/mylib/myglibc/stdio/printf.c#L11-L26)中L18、L22、L25，分别已有正确的作者E066/E067/E068。facts三终事件分别为C01_L4085 seq27（L8094–8096）、C01_L4086 seq27（L8883–8885）、C01_L4095 seq28（L9708–9710），已明确条件与“rv; not write result”；原总图L30–31亦已分开rv返回数据和write正常继续。主线不用未知运行事实补足这项图义。

本补注经305项有界核对覆盖全部263边和三条返回来源，故 **R158-01当轮闭合**，不留给159补算、不改作者原六件。闭合方式沿用[151主线消费补注先例](https://github.com/08822407d/MyOS2/blob/c2bb3f55604f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MALLOC-STATE-CONSUMPTION-151-review.md)；不是将原4项失败抹去。159图契约明确分开relation_type与event_kind，避免继续混用。

## 5. 公开接收字段的语义归一

作者六件确实没有同名 `receiver_guidance_load` 键；不冒称该字面结构已存在。但其公开execution_handoff_receive/guidance_refresh包含包ID与non_execution_source_transfer_artifact性质、固定入口/授权、身份/任务/写区、真实接收日期及精确时刻UNKNOWN、当前胜任和容量判断、未复用前席、实际刷新完成、权限与下一步。十五项目规范恰1441 LF，MA采用48→47有效全文与45共用01–14/16十五全文声明及版本核同。建设检查点只声明适用范围248个唯一物理行，符合221本次范围，未冒称全2032行。

| 221要求的实质 | 作者公开对应与本轮归一 |
|---|---|
| project_guidance=required | project_rules十五项各full／this session；路径/Blob/LF逐项核同 |
| meta_agent_guidance=required | MA入口、48、47、45同版十五全文、model registry及适用检查点 |
| mnemosyne_guidance=no | mnemosyne.scope仅task-local receive/naming及application不导入外仓业务/写权；缺no字面值，但适用含义明确 |
| mnemosyne_receive_format=task_local_only | task_local_only=true与mnemosyne.scope |
| mnemosyne_naming=required | naming_section7真实L445–584及C11c／EXEC-27登记；不认证聊天UI改名 |
| refresh_completed | guidance_refresh.refresh_completed=true，另列实际本席读取 |
| task_local_only | true；current_task_preserved=true与原任务/阶段/权限保持 |

按[152原审§5](https://github.com/08822407d/MyOS2/blob/c2bb3f55604f/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-MALLOC-REINIT-152-review.md)采用的语义等效规则，本次不只因缺同名键退回；这里没有152当时缺包身份/性质/当前容量/刷新事实的实质空洞。公开归一不代签过去私有工具先后。159本轮MANIFEST显式记录profile及actual status，沿C11c自己的有效已读按79复用；这是当前记录要求，非158历史补正，D00仍空。

## 6. 历史限定与后继

18份historical_inputs固定身份与精确selector已核。158仅向 `CORE-USER-SHELL-DATETIME-FORMAT-129::DT129-VSNPRINTF-BOUNDARY` 追加限定证据；129的有效ABI、STDIO-LINK、PRINTF-WRITE和130的实际sh绑定/运行、08 GAP-U10仍保持旧未知或开放状态。124读取规则、117/118生产者边界和94/130–132内核实现不被本次条件模型替代。R155在156的闭合、155原RETURN/历史未证、R152在153的闭合均保持。

同轮发布[222／159](../222-user-libc-format-arg-consumption-contract.md)，仍交原C11c／EXEC-27，三相关专题、三场景十行六件：字符串精度与先行读取、动态宽度取参与首个算术门、字符NUL进入printf请求。定点排重与独立READY详见合同；159独立READY：先从GIVEN与固定源封存十行模型，195项（176有限断言、13源锚、6身份）0失败；后读合同158项（96语义、53源表、2声明、6封存、1候选身份）0失败。覆盖核心输入/源表/对象及图契约，不代签历史检索过程、远端G00/G04或未来作者P6。它不代签未来159作者的回源/P6/G04或交付验收。

主线仅新增本审、222及[检查点](../checkpoints/2026-11-27-format158-reviewed-args159.md)；旧1842主线非tree、全部2674执行tree和旧312评论须在实际发表后再核冻结。原16/17只追加本次简短导航并保留旧body逐字后缀，原17标题/状态不变。完整P9、项目程序/脚本、预处理、C/ASM、编译链接、Graphviz、QEMU/实机/真实I/O均NOT_RUN；真实ABI、运行、后台型号和推理档未独立认证。

159首次发布成功时，C11设计152–159计7→8；按210在当轮最终回复开头醒目提醒8批线，C10历史11保留；提醒不等于上下文测量、自动移权或停工。执行159完成实际G04后STOP_WRITING，回C11 Pro独审及同轮后继，不进入160。
