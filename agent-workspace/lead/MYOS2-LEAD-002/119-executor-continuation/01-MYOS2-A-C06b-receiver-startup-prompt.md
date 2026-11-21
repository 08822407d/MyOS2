接收与执行模型：次一档（默认GPT-6＋Extra）。请在本MyOS2项目中新建普通对话“MYOS2-A-C06b 代码分析执行”；主线独立验收仍由本MYOS2-A-C06使用Pro。

# C06b / CHAT-EXEC-11接收并执行第84批

仓库：08822407d/MyOS2。任务轨道：MYOS2-LEAD-002，执行任务号MYOS2-LEAD-002-CORE-CHECK-01。
新对话名称：MYOS2-A-C06b 代码分析执行。
新身份：MYOS2-LEAD-002-CHAT-EXEC-11。
工作包：CORE-USER-EARLY-ROOT-HANDOFF-84。

Owner已说明旧C06a上下文过大，下一批在新对话开展。旧C06a/CHAT-EXEC-10在83交付后STOP_WRITING，本次由C06主线介导执行者轮换；无需旧10另行确认，不沿用10身份或已读声明。旧C05c/09继续停用。主线仍为MYOS2-A-C06，不转移lead/PR16写权。

## 固定材料

L=agent-workspace/lead/MYOS2-LEAD-002/。下面所有L文件必须取Owner转发本启动词URL中的同一实际固定发布提交，不改用活动分支，不拿父b129d706e350当发布头。

完整读取本01及：
- L/119-executor-continuation/00-C06a-to-C06b-handoff-package.md。
- L/120-user-early-root-handoff-contract.md及其中全部必读材料。
- L/110-owner-communication-and-delivery-continuity.md。
- L/reviews/CORE-USER-DIRECT-MAP-TABLE-INSTALL-83-review.md。
- L/checkpoints/2026-11-13-table-install83-reviewed-early-root84-exec11-ready.md。
- L/reviews/CORE-USER-EARLY-PGTABLE-STORAGE-81-review.md和L/reviews/CORE-USER-DIRECT-MAP-RANGE-82-review.md。

83原六件取执行固定18047d54b0a2，正式评论6847954250；必须独立全文读取，不继承旧C06a的已读声明。源码仅time@a039d9803ade，master直接头预期e8e60cd19540。原执行分支claude/dazzling-cori-q0dnyt、PR17以及主线agent/MYOS2-LEAD-002、PR16均不变，不新建分支或PR。

receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  task_local_only: true
  ordered_operations:
    - read_explicit_handoff_materials_and_form_real_receive_report
    - independently_read_nine_MyOS2_constraints_in_full
    - independently_read_110_owner_supplement_in_full
    - separately_load_current_applicable_Mnemosyne_guidance
    - pass_G00_then_execute_G01_through_G04_continuously

## 按顺序接收并执行

先按00交接包读取接收材料与Mnemosyne接收程序、核现场，形成真实mnemosyne_handoff_receive报告。未加载的九规则/指导此时写pending，不能提前声称完成。

再独立全文读取00包列出的九份MyOS2项目约束，包括00工作令§12、README、公约、协议v2全部P1–P13、18/24/28/63/79；然后全文110号Owner协作续记。长响应截断必须补齐。

随后单独完整加载本次适用Mnemosyne指导并形成真实刷新记录，保留当前84任务，不导入Mnemosyne或其它项目维护路线。主线观察版本为8b58dd7b07c1；若实际master变化，如实记录并按当前适用指导加载，不擅改MyOS2固定合同。原对话临时日志无法取得不构成本新对话已读证明。

接收、规则和指导加载完毕前只读，不写GitHub。真实接收/刷新记录先保留临时材料；取得84限定写权后并入本批六件，不额外交第七件或虚构sandbox路径。

完整核120的G00：PR16/17、四direct refs、开放PR、目标目录及本人marker、公约原句、time/master金丝雀。执行起点须18047d54b0a2；主线须本119/120实际发布头。已有结果或未知并行变化先查归属，不覆盖或重复交付。

全部门通过后，按本次Owner转发与120条件授权连续执行G01–G04，不逐专题等待批准，也不在G00正常完成后再次等Owner发令。本批无D00，不重做83、81或82，不重开已闭合文档事项；合读R83-S01/S02/S03、R82-S01/S02/S03、R81-S01/S02/S03及实际依赖的历史限定。

完成三个新专题：启动静态页表的存储与覆盖；地址修正与临时恒等映射；C入口清零、根项复制及后续交接。只做固定源码有界取证，不证明完整启动或硬件实际成功。

## 限定写入、回读与停止

唯一新增区：
agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-early-root-handoff-84/

严格六件：MANIFEST.md、early-root-handoff.md、facts-and-boundaries.yaml、early-root-handoff-paths.dot、deferred-findings.yaml、evidence.md；另加11本人一次正式PR17交付评论。发布前按120核字段、原文公约/金丝雀、三整数计数、真实定义内短锚及事实/模型/图的语义支持。

不改旧成果、旧评论、PR元数据/Draft/base、主线或内核；不出补丁，不预处理/汇编/编译/链接或运行仓库代码/脚本/旧夹具/Graphviz/QEMU/实机；不新建分支PR，不合并关闭强推删除，不启额外模型/API/子代理，不跨项目维护。

GitHub暂态按63号先核远端效果，再间隔安全重试；完整expected-head、force=false、单父提交，复用成功对象，未知推进不覆盖。新固定/活动六件全文与blob、compare、四direct refs、PR状态及本人正式评论唯一性全部远端回读。

完成后仅报READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，随后STOP_WRITING，经PR17正式交回“MYOS2-A-C06 内核分析主线续建”作Pro独立验收。不自签84，不启动85，不交旧C06a/C05c，不恢复旧10/09写权。

110号必须继续显式进入以后每次主线交接包、启动词和固定必读清单。本轮是执行者接续，不是主线接管；任务发布不等于新对话已自动创建或启动。
