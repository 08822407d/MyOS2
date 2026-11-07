---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-COMPLETION-OWNERSHIP-53
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "无法直接读取实际后台作者；Owner选定GPT-6/Extra不等于已认证后台身份"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
date: 2026-11-07
base_snapshot: "time@a039d9803ade"
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: combined_ownership_analysis_and_R52_external_correction
---
# 第53批：R52-01补充、devtmpfs/kthread/keyboard/IDE对象寿命
## O00｜R52-01外置文档补充，52原裁定仍RETURN
原52 MANIFEST缺少`produced_by`和`branch_canary_quotes`字段。本批MANIFEST专设`prior_batch52_manifest_supplement`，`supersedes_fields`仅为这两字段，produced_by=unknown且说明后台无法独立认证；金丝雀从固定time重新核原样tab和反斜杠。原52六件/评论不改。
旧52证据WW52-C14属于kthread真实164–215行而非旧记164–222，原203–205行仍合法；旧WW52-C16属于kernel_init真实270–309行而非旧270–365，原275–277亦合法。CO53-R5201..06分别短引文核起点/旧片段/函数尾，不重新验旧52其余65条。原消费者C.kthread_producer和C.init_consumer保留。52 deferred UV-01拼写`deferred_not_ready`仅为缩写误差，Owner既有正式状态`deferred_owner_not_ready`，不准做状态变更。修正映射跨MANIFEST/本文/facts/evidence/deferred/交付评论。本执行者仅申请`R52-01_DOCUMENTATION_CLOSURE_REQUESTED`，不自签CLOSED/PASS。

## O01｜A devtmpfs请求和通知后的读/释放
req_s包含next、内嵌done、err以及name/mode/uid/gid/dev。`devtmpfs_create_node`用kzalloc产生heap req且立即写字段；若分配返回NULL，首次req->mode解引用即成停止点；成功分配后若dev_name返回NULL，则本函数直接返回-ENOMEM而未在本路径kfree(req)，只能作局部遗漏释放候选。name指针来自dev_name，不等于本函数分配的name存储，真实寿命未审。tmp明确NULL，固定kfree对NULL早退。
`devtmpfs_delete_node`的请求却是未零初始化的自动局部struct req，只写mode=0和dev；对name赋值的旧语句被注释，uid/gid也未写。submit_req只对内嵌done初始化、在req_lock下保存next并发布requests队首，发wake请求，调用void wait_for_completion，正常返回后kfree(tmp=NULL)而后取req->err。
worker在req_lock下取下全局requests批、设全局NULL，然后每项先取next，再将`handle(req->name,req->mode,req->uid,req->gid,req->dev)`结果写入err，接着complete、kfree(req)、转next。delete路径**最早的潜在无效求值**是worker读取name/uid/gid未初始化值作为实参，随后handle(mode0)转到int类型的handle_remove，该函数当前没有活动return。不能从这些未定义/未正常返回前提跳到晚一步称已真实执行kfree(stack)；但源代码的统一kfree对自动对象构成需证的独立储存协议冲突。
对于create heap req，worker次序是写err→complete→kfree，creator次序是wait正常返回→kfree(NULL tmp)→读req.err。即使建立匹配通知先于wait返回的条件边，free与read仍有两种次序。JS枚举4个拓扑序列中3个free先read、1个read先free，仅证明两种有界序列可能，不代表概率或CPU已发生UAF；若真实对象在free后失效且caller随后读，就是CO53-F01候选。通知不单独转交对象所有权。

## O02｜B kthread创建、通知与对象最后使用
创建方kmalloc独立create信息，栈上DECLARE_COMPLETION_ONSTACK(done)并设create->done=&done；另一指针full_name由kvasprintf创建。create分配失败直接ERR_PTR(-ENOMEM)，命名失败在入队前goto free_create后kfree(create)。正常在kthread_create_lock下入队并请求唤醒kthreadd。kthreadd摘队回推create，调用create_kthread后仅重新使用自身队列锁，不在这段尾部再次解引用create。
create_kthread调用kernel_thread(kthread,create,create->full_name,...)。pid<0失败分支xchg(&create->done,NULL)，请求kfree(full_name)；若done为空也kfree(create)，非空则create->result=ERR_PTR(pid)并complete(done)。该NULL分支不应凭已注释的killable取消代码宣称必定可达：所选creator真实代码只是wait_for_completion，不执行取消xchg。
成功新线程kthread早期从create读取threadfn/data，xchg得到原done指针并把create->done设NULL；done非NULL时self->full_name=create->full_name（指针传递、非深拷贝），create->result=current，在complete(done)前写成。通知后仍会schedule_preempt_disabled/运行threadfn，但**已读kthread定义体通知后不再访问create**；creator若正常从wait返回，会先读取create->result再kfree(create)。不能用“producer函数未完全退出”否定这段已完成的create成员最后访问，也不能忽略kernel_thread等未审框架可能的访问。full_name后续由free_kthread_struct释放，和create对象不是同一释放责任。
xchg宏在固定source为xchg→arch_xchg→__xchg_op交换并返回旧值的路径；未经预处理编译、缓存/原子架构验证，不把它升格为已证运行同步。纯偏序允许creator已读/释放create，而producer仍在其后续schedule调用；它不是内核实测。

## O03｜C 静态keyboard与IDE一次性完成对象
`keyboard.c`文件作用域`DECLARE_COMPLETION(getcode_done)`是复用的同名对象；handler在kbdbuf_lock下写扫描码、增加p_kb->count，解锁后complete(&getcode_done)。kbd_get_scancode的do-while每轮都wait_for_completion(&getcode_done)，再查p_kb->count是否大于零；读取侧在锁中减少buffer count。kbd_parse_scancode调用此函数，扩展扫描码还可能多次调用。`completion.done`、`swait header.count`、`kbd_inbuf_s.count`为不同对象字段，不能互证。读到的init_keyboard只初始化buffer count，未见活动reinit_completion(getcode_done)；**不声明全仓无别处复位**。通知发生在等待前时可走done快路；等待真正入队后通知则须保留CA-04的swait stale count限制。无IRQ运行证明。
`IDE_make_request`单独kzalloc node及IDE_Params_s，写控制器/磁盘、请求状态及end_handler；ATA_disk_transfer为每次合格命令声明栈done，发布node->done=&done及node->task=current，把node入队并唤醒daemon，随后wait。等待若正常返回，按序kfree(node.DevSpecParams)、kfree(node)，返回固定源码常量，而非从IRQ传回完整I/O错误码。
daemon将队列节点保存为req_in_using并调用IDE_cmd_out；ATA_disk_handler经req_in_using获得node、调用node->end_handler，count==0才end_request；end_request使用node->done发complete后仅锁住req_lock并清req_in_using，没有在该函数尾部再次解引用node。这不同于devtmpfs worker的complete后kfree(req)。有限偏序允许consumer释放node发生在producer清全局指针之前，但不能证明其它中断/daemon在这段交错中不访问node，亦不能当作已证安全。硬件、队列和生存期留OPAQUE；NVMe/XHCI仅沿用52命名调用点，不重审整驱动。

## O04｜候选与边界
新增CO53-F01/F02/F03均是独立来源的**条件源码候选**：heap通知后结果读取、delete stack/uninitialized字段协议、create分配/早退。WW52-U04仍needs_evidence，CA-04仍历史SUPPORTED_IN_SCOPE；不将来源清单升级为已观测UAF/非法free/崩溃或内核修复授权。A/B/C均completed_in_scope，53待C04独立验收、52仅申请文档闭合。实际内核C/ASM/脚本、预处理编译、QEMU硬件、Graphviz、完整P9及并发都NOT_RUN。
