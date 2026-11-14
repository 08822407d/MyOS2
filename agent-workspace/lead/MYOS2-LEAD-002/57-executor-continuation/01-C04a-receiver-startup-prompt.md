接收模型：次一档（默认）；建议GPT-6＋Extra。
执行位置：MyOS2辅助分析Project中的新ChatGPT普通对话「MYOS2-A-C04a 代码分析执行」。

# C04a接收并执行第33批：启动指令

@GitHub 请将本对话用于 MYOS2-A-C04a 代码分析执行，身份 MYOS2-LEAD-002-CHAT-EXEC-04。主线唯一验收/规划者仍是 MYOS2-A-C04 内核分析主线续建。

请把**本启动指令URL中的固定提交**作为本轮任务输入固定ref，完整读取同一提交：
1. 57-executor-continuation/00-C03c-to-C04a-handoff-package.md（包ID MYOS2-C03C-TO-C04A-20261103-01）；
2. 56-user-dentry-lookup-contract.md；
3. reviews/CORE-USER-LAST-COMPONENT-32-review.md（必须合读R32-S01/S02）；
4. 该提交的检查点。

接收模型：次一档（默认）；Owner建议GPT-6＋Extra。
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations: [接收包, Mnemosyne接收报告, 加载MyOS2完整规则, 独立刷新适用Mnemosyne指导, 核实GitHub现场, 执行第33批]

先按Mnemosyne协议形成接收报告，再分别加载项目规则和适用指导，不继承旧C03c聊天记忆。材料和现场预检通过之后，在同一轮按照56号D00–D04连续完成，不因普通内部步骤等待确认。

固定源码time a039d9803ade；首次执行写前预期PR17提交 d6444512a213。沿用PR17及claude/dazzling-cori-q0dnyt分支；**只可在user-dentry-lookup-33/新增任务指定六件及本人进度/交付评论**。旧C03c不获第33批委派；不新建分支PR，不写PR16/lead，不修改旧件、内核、PR标题正文或Draft，不编译运行、不合并。

遇输入/规则/权限/竞争写入阻断就停相关动作如实说明；无写工具时只完成安全只读工作并报告ANALYSIS_PREPARED_WRITE_BLOCKED，不伪造提交。正常交付后STOP_WRITING，交C04独立验收，不自行开始第34批。
