---
task_id: MYOS2-LEAD-002
track_id: MYOS2-LEAD-002
record_id: CORE-USER-LAST-COMPONENT-32-REVIEW-001
record_type: independent_bounded_static_acceptance_with_mandatory_supplements
conversation_display_name: MYOS2-A-C04 内核分析主线续建
produced_by: unknown
product_surface: ChatGPT普通对话
model_per_owner: gpt pro（Owner最近报告）
backend_identity: unknown_or_not_attestable
date: 2026-11-03
read_channel: connector
reviewed_pr: 17
reviewed_base_short12: 45a065270e28
reviewed_head_short12: d6444512a213
source_pin_short12: a039d9803ade
contract55_short12: bf856f2ddb2d
review31_short12: bf856f2ddb2d
lead_before_publication_short12: bf856f2ddb2d
acceptance_verdict: PASS_PENDING_LOCAL
returned_scope: []
mandatory_supplements: [R32-S01, R32-S02]
runtime_evidence: NOT_RUN
full_local_P9_gate: NOT_RUN
executor_89_byte_checks_reproduced_by_reviewer: false
executor_JSON_DOT_mechanical_checks_reproduced_by_reviewer: false
kernel_correctness_verdict: NOT_ISSUED
execution_branch_modified_by_mainline: false
next_followup: CORE-USER-DENTRY-LOOKUP-33
next_executor: MYOS2-LEAD-002-CHAT-EXEC-04
owner_decisions_requested_now: []
---

# 第32批独立验收：末分量静态成果限定接收

## 1. 裁定及固定身份

依据55号任务书、31号主线回执R31-S01/R31-S02及第32批六件，裁定 PASS_PENDING_LOCAL、returned_scope=[]。原六件冻结，后继消费必须合读本回执R32-S01/S02；无需旧C03c再执行一批返工。30组合材料与31限定接收状态不变，普通源码问题仍按18号延期。

PR17实际head d6444512a213，open/Draft、未合并；交付评论issuecomment-6599612081声明READY_FOR_MAINLINE_REVIEW、STOP_WRITING。compare 45a065270e28→d6444512a213仅1提交，六件均在user-last-component-32/新增，分别MANIFEST153、正文103、facts887、DOT213、deferred253、evidence888行，共2497行，无旧件净修改、删除和写区外路径。执行方申报六件双ref全文读回及89个短引文自查；该部分不能冒充本主线机械重跑。

本轮主线实际完整读取六件、55号和执行交付评论，并按已加载31回执核R31-S01/S02；独立回核所选固定time源码及定义上下文、限量数值/字符串模型。成功取证来自GitHub连接器。没有编译、运行内核、执行仓库脚本、完整本地P9或全量字节扫描。

## 2. 确认的核心事实

固定源码time a039d9803ade。link_path_walk形成nd.last.name、len和last_type，局部name游标的前移不重新分配独立字符串。LAST_NORM/ROOT/DOT/DOTDOT按本仓枚举定义；源码ENOERR=0，所以return -ENOERR是整数0。前缀0不等于完整路径解析或权限通过。

do_filp_open先读 int flags=op->lookup_flags，传入path_openat；后者把同一flags传入open_last_lookups(nd,flags)。该函数参数虽叫open_flag，实际属于LOOKUP位域，而非op->open_flag或op->intent。O_CREAT=八进制00000100=0x40，LOOKUP_RCU=0x40，LOOKUP_CREATE=0x200。被注释的创建分支不会执行；在LAST_NORM且接收位0x40的helper条件下，dentry的唯一活动赋值块被跳过，后续却交给step_into。此条件未证明由所选成功的实际调用者产生，不能声称已经复现漏洞。

step_into在handle_mounts的负返回时执行ERR_PTR(err)；非负时先写nd->path，继而return ERR_PTR(-ENOERR)，在所选x86-64编码模型中是NULL。外层path_lookupat/path_openat的整数err/error由link_path_walk写入，末分量函数返回指针写入s：NULL自然退出；符合编码范围的非NULL ERR_PTR在下一轮link_path_walk转换成整数错误。普通非NULL续串只是类型可能性，本批所选活动出口没有证明其生产者。旧UV-04的do_open缺非void返回值仍阻断最终file状态推断，但缺return不等于永不返回。

## 3. R32-S01：输入生产者、错误范围、名字存储补充

1. 已读成功的build_open_flags由局部lookup_flags=0开始，所选FOLLOW/DIRECTORY/RESOLVE映射位的上界按实际位或为0x3f0003，与0x40交集为零。上界只是排除该位，不证明各种组合均可通过校验。所选do_open_execat初始化及AT选项只可形成0、1、0x4000、0x4001，同样不含0x40。结论只限已读正常生产者，不能说全仓所有调用都不可能生产0x40。
2. LC32-G04/T10/X10及LC32-F02的未定dentry条件，必须同时包含LAST_NORM、实际0x40位来源与定义良好的前提。OF30-F01中op部分字段未写时不能选择确定的比特分支。O_CREAT不应被表述成“只改变intent”：它还参与mode/open_flag处理，O_EXCL可改局部NOFOLLOW；核心观察是创建意图字段与传给末分量函数的lookup字段分离。
3. 错误指针转换只对所选IS_ERR_VALUE编码区间成立，非任意负整数都在其内。MAX_ERRNO=4095时，按选定64位编码模型，-1至-4095落入区间，-4096不落入。step_into负err产生ERR_PTR的事实，不单独保证外层下一轮IS_ERR一定能识别；不存在范围外实际生产者的证明。
4. nd.last.name是指向原字符串的借用位置，未为每个分量独立分配补NUL；但末分量若直接止于原NUL，本来就在该位置结束，中间分量或尾斜线可能不是NUL。空串/全斜线不写新的last.name/len，是否为零要合读set_nameidata先清零的有效上游前提。
5. 超长计数、无效指针、下层不返回均停在定义良好的已证前缀，不推导确定CPU、权限或文件创建后果。

## 4. R32-S02：标题/引文、真实门和图的合读映射

- LC32-A027/A028及O03/O05：游标按分量长度前进还必须引用link_path_walk的实际语句 name += nd->last.len；A028本身仅含跳斜线。
- O12和DOT的opOpen赋值，必须合读build_open_flags中的 op->open_flag = flags，不由A070的intent片段单独推出。
- P09的do_filp_open返回，须合读函数尾 restore_nameidata(); return filp;，A067只覆盖到restore的调用。
- G03的open_exec flags=0须合读open_exec中 do_open_execat(AT_FDCWD, filename, 0) 的实际调用；仅凭do_open_execat初始化不能推出0实参。
- DOT execopen→dofilp的“file opened under conditions”替换为“AT标志合法性检查通过后调用do_filp_open；返回后才检查file是否编码错误”，不能把调用结果写成前提。
- LC32-F03关于__lookup_slow错误分支的反证，必须读真实 kfree(dentry); return ERR_PTR(-ENOENT);，A047签名并不支持这个结论；其实际NULL返回可能性留第33批。
- LAST枚举数值0/1/2/3须合读匿名枚举全部连续定义；各单行标记仅证明该项存在。LC32-A070的“仅”按R32-S01收窄。
- path_lookupat的err在循环后还可能被complete_walk赋值；facts中O08的writer并非全函数唯一赋值源。所有DOT门图区分活动代码测试与为了安全推演额外要求的有效对象、op及正常返回前提。

上述补充对32原正文、facts、DOT、deferred和evidence同一消费位置生效，**不回写原件、不把全部89个旧片段自动认作机械复验通过**。

## 5. 方法、限定与后继

定点回源包括namei.c240–766、open.c67–180、dcache.h1–160、namei.h29–68、errno-base.h1–42；其它配套符号以已读同一固定time内容交叉核对。曾为33预检读取dcache.c的__d_lookup/__myos_d_alloc、string.c的strncmp和__lookup_slow，仅用于划界，不预签33成果。独立有限模型使用人工转录的10组字符串、O/LOOKUP位和5组错误编码情形；非仓库C运行。

没有原六件的字节哈希/regex自动匹配、JSON/DOT解析、完整本地P9、预处理/编译、QEMU/硬件、真实路径、目录项/文件系统回调运行、权限/锁/引用完整证明。执行方与主线虽角色不同，但属于同一模型家族，不宣称人类或异构技术验收。

后继发布56号CORE-USER-DENTRY-LOOKUP-33，交新CHAT-EXEC-04，在原PR17继续原分支，仅允许user-dentry-lookup-33/六件；C03c不再收到33委派，不要求其另写交接报告。由Owner转发新启动入口、接收与规则/远端预检合格后才启动；发布不等于自动跨对话投递或后台启动。

## 固定证据入口

55号 https://github.com/08822407d/MyOS2/blob/bf856f2ddb2d/agent-workspace/lead/MYOS2-LEAD-002/55-user-last-component-contract.md
31号回执 https://github.com/08822407d/MyOS2/blob/bf856f2ddb2d/agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-PATH-INIT-31-review.md
32结果 https://github.com/08822407d/MyOS2/tree/d6444512a213/agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-last-component-32
C03c交付 https://github.com/08822407d/MyOS2/pull/17#issuecomment-6599612081
