接收模型：次一档（默认），建议GPT-6＋Extra。
执行位置：MyOS2辅助分析Project中新普通对话「MYOS2-A-C04b 代码分析执行」。

# C04b接收并执行第41批

@GitHub 请将本对话用于「MYOS2-A-C04b 代码分析执行」，任务内执行身份MYOS2-LEAD-002-CHAT-EXEC-05。
主线规划、独立验收与lead/PR16唯一写入者仍是「MYOS2-A-C04 内核分析主线续建」。
旧C04a停在第40批，不再接受第41批委派；本次不新建分支或PR。

以本启动URL中的真实固定提交为发布输入，完整读取同提交、相对agent-workspace/lead/MYOS2-LEAD-002/的：
1. 67-executor-continuation/00-C04a-to-C04b-handoff-package.md；包ID MYOS2-C04A-TO-C04B-20261105-01。
2. 66-user-fd-lookup-ref-contract.md。
3. reviews/CORE-USER-OPEN-COMPLETION-39-DOC-CLOSURE-review.md。
4. reviews/CORE-USER-FD-ALLOCATION-INSTALL-40-review.md，必须合读R40-S01/S02。
5. 63-github-write-retry-and-readback.md和本次检查点2026-11-05-closure39-fd40-reviewed-c04b-fd41-ready.md。

receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations: [receive_handoff, emit_receive_report, load_MyOS2_rules, separately_refresh_Mnemosyne_guidance, verify_GitHub, execute_batch41]

按Mnemosyne协议先形成接收报告，再分别完整加载项目约束和适用Mnemosyne指导，不继承旧C04a记忆、不导入其它项目维护任务。
模型档按Owner选择与实际可知信息记录，后台不可核则unknown；不擅自升级或调用额外付费服务。
材料、规则与现场核对通过后，同轮按66号G00–G04连续完成CORE-USER-FD-LOOKUP-REF-41，不为普通步骤另等确认。

固定源码time@a039d9803ade；首次执行写前预期PR17提交46f5327a8cdf。
沿用claude/dazzling-cori-q0dnyt/PR17，只在user-fd-lookup-ref-41/新增规定六件及本人评论；全部旧件冻结。
R39-01已由C04文档闭合；40为PASS_PENDING_LOCAL，不重做验收，不把静态接收当内核运行通过。

GitHub暂态异常先回读是否已经写成，再按63号间隔多次安全重试；写动作临时缺席也重新发现，不因一次异常直接判断无权限。
保持expected-head、force=false；复用已创建对象、避免重复评论，未知并行写入或真实安全限制不绕过。
不改内核、不编译运行、不写lead/PR16、不修改PR正文/Draft、不合并。
正式交付并远端回读后STOP_WRITING，交C04独立验收，不自行开始42。
