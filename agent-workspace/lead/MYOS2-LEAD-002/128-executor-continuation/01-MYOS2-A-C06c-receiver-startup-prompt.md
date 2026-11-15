接收与执行模型：次一档，沿用GPT-6＋Extra。执行位置：本MyOS2项目中新开的普通ChatGPT对话“MYOS2-A-C06c 代码分析执行”。新身份：MYOS2-LEAD-002-CHAT-EXEC-12。正式验收仍交原C06主线使用Pro。

# C06c / CHAT-EXEC-12接收并执行第90批

仓库08822407d/MyOS2；轨道MYOS2-LEAD-002；执行任务号MYOS2-LEAD-002-CORE-CHECK-01；工作包CORE-USER-ID-ALLOCATION-LIFECYCLE-90。

Owner已明确旧C06b上下文已满、不能继续，本次要求在新对话开展90。旧C06b/11停用，不索取其新确认，不沿用11身份、临时进度或已读资格；旧10/09继续停用。C06主线未换届，lead/PR16写权不移交给本执行对话。

本次意图为RUN_AFTER_GATE_G00：先接收和独立加载，通过G00后连续执行D00、G01–G04，不只接收后等待，不逐专题等批准。尚未满足门时只读。

receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - read_explicit_package_and_form_real_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_and_124_in_full
    - separately_load_applicable_Mnemosyne_guidance
    - pass_fresh_G00_then_execute_D00_through_G04_continuously

## 一、固定入口和替代关系

L=agent-workspace/lead/MYOS2-LEAD-002/。先完整读取本01、同目录00-C06b-to-C06c-handoff-package.md和02-batch90-executor-amendment-and-checkpoint.md，三件均取Owner转发本01链接中的真实固定发布提交。

既有工作材料固定取MyOS2@07ed0fed054c：L/127-user-id-allocation-lifecycle-contract.md及本00列出的全部必读材料，尤其110、124、89-review、88-review及88-DOC-CLOSURE、87/86限定回执和旧检查点。127技术范围及质量门继续有效；其中原C06b/11、不新开不轮换、旧会话复用及旧11交付marker被本02明确替代，不因冲突停在旧指令。

89原六件固定42897a06feb1，必须独立全文读取；PR17作者评论6893704757、主线评论6894330789及本128后置发布评论分清。第11批历史去重材料按00及127的固定2c410132e42d范围读取，保留PID11-F01/F02，不重审全部PID生命周期。其它历史仅按真实依赖定点读取，未读不得记为已核。

## 二、按序接收与加载

先按00读取接收程序、指定材料和现场，形成真实mnemosyne_handoff_receive报告；未读的九规则、110/124或指导此时记pending，不提前宣布G00完成。接收报告与指导刷新是不同操作。

随后独立全文读取00列出的九份MyOS2约束：00工作令含§12、agent-workspace/README、公约、协议v2全部P1–P13、18、24、28、63、79；再独立全文110和124。截断分段补齐，不继承C06b、C06主线或任何旧执行者已读声明。

再单独按Mnemosyne的load命令加载适用指导并形成真实刷新记录。准备时观察其master为ee65de8a255d；实际版本变化须记录并使用适用固定版本。只刷新本90任务行为约束，不导入Mnemosyne/Alaya维护路线，不新增外仓写入。接收/加载记录先临时保全；G00通过后并入90六件，不另交第七件。

## 三、G00后连续执行

源码仅time@a039d9803ade；执行起点42897a06feb1；master直接头预期e8e60cd19540；lead以本128真实发布提交为准。沿用claude/dazzling-cori-q0dnyt和PR17，主线agent/MYOS2-LEAD-002和PR16不变。不新建分支/PR。

重新核PR16/17、四direct refs、开放PR、90目录及所有执行者的90正式marker，包括旧11与新12；核公约原句、time/master金丝雀，配置判断前全文读取options_flags.cmake。发现未知写入先查归属，不覆盖、不自动变基、不重复交付；只有确为本人成功动作才转补回读。

G00通过后立即连续D00、G01–G04。R88-01已闭合，88组合PASS_PENDING_LOCAL；89仍RETURN，仅R89-01/02，主体合读R89-S01/S02/S03；R88/R87/R86限定继续有效，不重开闭合项。

D00新增两项纠正：分别锚定for_each_possible_cpu、for_each_online_cpu和必要共同下级，映射原WQP89-E010全部消费者；实际读取set_user_nice完整实现及create_worker相邻控制流，纠正NULL任务模型M15及相关矩阵、F05、U08等读法。保留原120/120/0与旧原件，不能把假设NULL返回说成实际可达或在空操作函数中必然解引用失败。只报CORRECTION_SUBMITTED_PENDING_C06_REVIEW。

之后完成127的三个专题：IDA范围/容量单位/占用回收；IDR范围/NULL槽/发布/游标；查找替换移除遍历及工作池/PID有界消费者。旧WQP89和PID11根因只追加原身份，不重复编号，不扩大为完整工作队列、PID、kthread或调度器审计。

## 四、写入与交回

唯一新增区：agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-id-allocation-lifecycle-90/。
严格六件：MANIFEST.md、id-allocation-lifecycle.md、facts-and-boundaries.yaml、id-allocation-lifecycle-paths.dot、deferred-findings.yaml、evidence.md；另加CHAT-EXEC-12本人一次正式PR17顶层交付评论。
新marker：MYOS2-LEAD-002-CHAT-EXEC-12 / CORE-USER-ID-ALLOCATION-LIFECYCLE-90 / FORMAL_DELIVERY。

发布前按127全部P1–P13和02检查字段、公约、金丝雀、标签总数、真实符号/定义边界及类型/调用帧/模型前提/语义支持；未做的机械检查明确NOT_RUN，已知错误在本批发布前纠正。不改旧文件、旧评论、PR元数据或内核，不出补丁，不运行仓库程序、构建、Graphviz、QEMU、实机或真实并发，不启额外模型/子代理/API，不合并关闭强推删除或跨项目维护。

GitHub暂态按63先核远端是否成功，再间隔安全重试，通常首次加三次；完整expected-head、force=false、单父提交，成功对象及评论不重复。固定六件全文、活动六件内容或blob身份、compare严格六件added、四refs、PR状态和评论内容/唯一性全部回读。

完成报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING、R89-01/02纠正待C06，随后STOP_WRITING，经PR17交回原“MYOS2-A-C06 内核分析主线续建”。提醒Owner正式验收消息选择Pro，同轮审核补正、90新成果及后继。不自签89闭合或90通过，不启动91，不交回旧C06b。110和124继续显式进入后续主线交接材料。
