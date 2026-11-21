接收与执行模型：次一档（默认GPT-6＋Extra）。请新建普通ChatGPT对话“MYOS2-A-C06a 代码分析执行”；主线独立验收仍由MYOS2-A-C06使用Pro完成。

# C06a / CHAT-EXEC-10接收并执行第79批

仓库：08822407d/MyOS2。
本启动词须由Owner以真实固定提交URL转发；下列L路径均取该URL中的同一固定提交，不能自行改用活动分支代替固定版本。L=agent-workspace/lead/MYOS2-LEAD-002/。

请将新对话用于“MYOS2-A-C06a 代码分析执行”，新身份为MYOS2-LEAD-002-CHAT-EXEC-10，继续任务轨道MYOS2-LEAD-002-CORE-CHECK-01。

旧C05c/CHAT-EXEC-09已在第78批正式交付后STOP_WRITING。Owner明确该对话上下文过长不能再使用；本次由C06主线根据正式交付和Owner指令介导执行者轮换，不要求旧C05c再发确认。不要沿用09身份，不把此次执行者接续误当主线写权交接。

## 固定入口

完整读取：
- L/113-executor-continuation/00-C05c-to-C06a-handoff-package.md。
- 本01启动词。
- L/114-user-memblock-region-lifecycle-contract.md及其指定材料。
- L/reviews/CORE-USER-PAGE-ALLOCATOR-LIFECYCLE-77-DOC-CLOSURE-review.md。
- L/reviews/CORE-USER-PAGE-BOOTSTRAP-HANDOFF-78-review.md。
- L/checkpoints/2026-11-12-bootstrap78-reviewed-memblock79-exec10-ready.md。

执行起始头03519ebb5d86；固定内核time@a039d9803ade；master直接头预期e8e60cd19540。主线发布头取本启动URL实际提交，不拿父ea5eb636ae89当新头。原执行分支claude/dazzling-cori-q0dnyt及PR17不变，主线仍agent/MYOS2-LEAD-002及PR16，不创建任何分支或PR。

receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - receive_explicit_handoff_and_form_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_owner_supplement_in_full
    - separately_load_applicable_Mnemosyne_guidance
    - pass_G00_then_execute_D00_and_G01_through_G04_continuously

## 接收顺序与执行授权

先按00交接包读取接收所需材料并形成真实mnemosyne_handoff_receive报告；此时未完成的规则/指导状态写pending。然后独立完整加载交接包列出的九份MyOS2约束，再完整加载110号Owner沟通与交付续记；随后单独完整加载本次适用Mnemosyne指导、形成刷新记录，确认MyOS2任务保留、不导入维护路线。不得继承C05c、C06或其它对话的“已读”声明，长响应截断必须分段补齐。

报告和规则加载是不同步骤，读取Mnemosyne接收程序不是已经加载其全部适用指导。只读接收阶段不写GitHub；临时接收/刷新记录在取得79写权后并入新MANIFEST/evidence，不能多交第七件或虚构临时路径。

全部接收和加载完成后，自行核合同G00的PR/direct refs、目标目录、本人marker、公约原句和两分支金丝雀。G00通过后即按本次Owner转发和114条件授权继续第79批，不另等旧C05c或Owner重复确认；新10只获得79新目录和本人交付评论的限定写权。遇真实缺件/访问/未知并行冲突只停受影响动作并报告最小缺项，不伪造通过、不覆盖未知推进。

先D00追加R78-01引文边界与证据索引纠正，再连续G01–G04三个区间专题，不逐专题等待批准。77的R77-01已由C06关闭，77组合PASS_PENDING_LOCAL；78仍RETURN，仅R78-01文档范围，合读R78-S01/S02/S03及指定历史限定。不重做77/78整批，不修改旧原件或评论，不把静态分析当真实运行。

## 唯一写区、回读和停止

只允许在原执行分支新增：
agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-memblock-region-lifecycle-79/

严格六件：MANIFEST.md、memblock-region-lifecycle.md、facts-and-boundaries.yaml、memblock-region-lifecycle-paths.dot、deferred-findings.yaml、evidence.md；另加10本人一次正式PR17顶层交付评论。主线目录、PR16、PR17元数据/Draft、内核、旧成果、旧评论、master/time均不可修改。

GitHub暂态按63号先核远端实际效果，再间隔安全重试；采用完整expected-head、force=false，复用已成功Git对象/评论，不重复创建；新固定提交及活动分支六件、净差异、四direct refs、PR状态和唯一评论全部回读。源码问题默认分类入库，文档问题准确追加纠正，不让Owner逐项决定修复。

全部完成后只报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING及R78-01纠正待C06，随后STOP_WRITING。通过PR17正式交回“MYOS2-A-C06 内核分析主线续建”进行Pro独立验收，不交旧C05/C05c、不自签78/79通过、不启动80。

不修内核、不出补丁、不预处理/编译/链接或运行仓库程序/旧夹具/Graphviz/QEMU/实机；不新建分支PR、不合并关闭强推删除、不用额外模型/API/子代理、不访问本机凭据或外部libc镜像、不进入阶段3或其它项目维护。仅按既有许可使用新建临时纯文本检查和有限模型，并如实标明非内核运行。
