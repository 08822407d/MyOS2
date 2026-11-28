接收模型：次一档（默认），GPT-6＋Extra。
执行位置：MyOS2辅助分析Project内新ChatGPT普通对话「MYOS2-A-C04c 代码分析执行」。

# C04c接收并执行第49批：固定启动指令

@GitHub 请将本对话用于「MYOS2-A-C04c 代码分析执行」，执行身份MYOS2-LEAD-002-CHAT-EXEC-06。
主线规划、独立验收及lead/PR16唯一写入者仍为「MYOS2-A-C04 内核分析主线续建」。不接管主线、不导入其它项目任务。

本启动指令URL中的真实固定提交是本轮发布输入。完整读取同提交的以下路径（相对agent-workspace/lead/MYOS2-LEAD-002/）：
1. 76-executor-continuation/00-C04b-to-C04c-handoff-package.md，包ID MYOS2-C04B-TO-C04C-20261106-01；
2. 75-user-completion-wait-contract.md；
3. reviews/CORE-USER-DEVTMPFS-SETUP-48-review.md，必须合读R48-S01/S02；
4. 63-github-write-retry-and-readback.md；
5. checkpoints/2026-11-06-devsetup48-accepted-c04c-completion49-ready.md。

```yaml
receiver_guidance_load:
  project_guidance: required
  mnemosyne_guidance: yes
  ordered_operations:
    - 接收固定交接包并形成Mnemosyne接收报告
    - 完整加载MyOS2工作令及本次适用项目约束
    - 单独刷新适用Mnemosyne指导并保持任务49不变
    - 核对固定输入与GitHub实际状态
    - 连续执行第49批W00至W04
```

先按Mnemosyne receive协议形成接收报告，再按交接包分别加载项目规则与适用指导；新对话不能继承旧C04b的已读声明。
固定包、规则和现场预检通过后，同轮连续完成CORE-USER-COMPLETION-WAIT-49，不为普通内部步骤或单纯接收完成再等第二条Owner指令。
第48批已由C04限定接收，无另行返工；第46文档闭合与47验收不重复执行。只消费其规定的历史限定。

固定内核time@a039d9803ade；首次执行写前预期PR17提交d23aa99d2c3a。
沿用原PR17及claude/dazzling-cori-q0dnyt；只在user-completion-wait-49/新增75号规定六件及本人新交付评论。
C04b不再获第49批委派；不新建分支/PR、不写lead/PR16、不改旧件/旧评论/PR元数据，不修内核、不编译运行、不合并。

GitHub暂态异常按63号先确认远端，再有间隔多次安全重试；工具暂缺也多轮正规发现，不因一次故障推断无权限。
保持expected-head、force=false，复用对象并查重评论；未知并行写入不覆盖，不绕过真实认证或安全限制。记录真实尝试与回读，未执行不得声称已执行。
真实关键输入、规则或竞争写入阻断时停相关动作并如实保全；模型能力不足时明确降级候选或请求主线升级，不隐式调用付费模型或扩大范围。

正常交付后STOP_WRITING，交「MYOS2-A-C04 内核分析主线续建」独立验收，不自行开始第50批。
本指令发布不等于新对话已经接收或任务已经运行；权限来自Owner实际转发与MyOS2固定任务书。
