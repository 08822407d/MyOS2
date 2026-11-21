---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WAIT-WAKE-CONSISTENCY-52
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-07
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
record_type: combined_wait_wake_A_B_C_report
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第52批 · A/B/C统一静态取证

## 总体完成边界
固定源码`time@a039d9803ade`。A、B、C均`completed_in_scope`，均仅静态源与纯JS有界模型；没有执行内核、源码脚本或编译。已继承主线R51-S01/S02、R50-S01/S02、R49C-S01和R48/R49限定。结论和未知要严格分离，详证统一见evidence.md，有限模型见facts-and-boundaries.yaml。

## A · swait带count头与普通List_s的摘除不变量
swait头的`task_list_hdr`为`List_hdr_s`，含普通双链`anchor`及独立`ulong count`，队列项只有`task`和普通`List_s task_list`。动态初始化调用INIT_LIST_HEADER_S，静态`LIST_HEADER_INIT`的anchor自指、count=0（A01–A03/A21）。本地有效不变量应为`count == 从anchor实际走到的待项数`，空链时`anchor.next==anchor`与count0一致。`__prepare_to_swait`在节点自指条件下调用`list_header_add_to_tail`，其确实count++（A04/A05）。
`swake_up_locked`先通过`list_header_is_empty`判空，该函数**只看count**，非空时`list_headr_first_container`从anchor.next按`task_list`回推`swqueue_s`，然后请求`try_to_wake_up`并直接`list_del_init`（A06–A09/A14）。在有效链和此前调用正常返回的模型中，普通del_init使节点重新自指，却**没有减少header.count**。若初始一待项count1，一次wake后链已空而count仍1；之后`__finish_swait`见节点自指会跳过`list_header_delete_node`，不能补齐count（A10–A13）。再次wake时count检查仍通过，但anchor.next已经是anchor，不能按真实waiter容器处理：**最早证明边界是容器前提失效，不能进一步虚构实际try_to_wake_up接收到的合法task或实机崩溃。**
`swake_up_all_locked`也只按count重复（A15），在已失配状态下不能凭宏名称证明循环会终止。两待项先后wake会使count仍2而实际链环先1后0；未唤醒节点的`__finish_swait`若仍真实在该头链中，`list_header_delete_node`会用`list_header_contains`按链查成员，再del_init与count--，这条路径与已被wake摘链的自指节点不同（A11/A12）。节点已在另一队列、删除方不持正确锁或对象寿命失效时立刻停止推论。
普通semaphore`wait_list`只是`List_s`，`__up`取首、普通list_del、置waiter.up=true；其错误出口亦普通list_del（A17/A18、51已验源码）。**不能要求普通List_s维护swait的独立count，也不能反过来用swait计数替代semaphore的waiter.up授予协议。**历史`CA-04::V02/V03/V11`已有有界宿主序列支持相同根因，本包新增的是counted helper的完整局部来源和completion/普通semaphore消费者合读，不另设同义WW52-F项。

## B · 有限timeout栈timer：登记、分离与同步删除条件
`schedule_timeout`的有限路径先设`timer.task=current`，`timer_setup_on_stack`映射`simple_init_timer_key`。初始化体明确写`entry.pprev=NULL`、`function=process_timeout`、处理合法flags并组合CPU号；`timer->expires`直到`__mod_timer`后段才被写，不能声称所有字段自setup起就被完整初始化（B01–B04/B15）。`proc_timer_s`的task与timer_list本体不可混为同一字段，`process_timeout`只在计时器被真实派发且对象有效时才向`task`发wake请求（B01/B02）。
本调用使用`__mod_timer(..., MOD_TIMER_NOTPENDING)`。它使`if (!(options & MOD_TIMER_NOTPENDING) && timer_pending(timer))`早期优化门直接为假，进入取得timer base锁、检查`function!=NULL`、forward timer-base的分支（B10/B11/B18）。下一步`detach_if_pending(timer,base,false)`：未pending为0、pending则按成员摘链并返回1；本次没有`MOD_TIMER_PENDING_ONLY`门（B07/B08/B12）。`idx`从UINT_MAX开始，只有刚被跳过的优化分支会赋预计算index；在合法正常本调用链中`idx!=UINT_MAX`为假，所以写`timer.expires`后选择`internal_add_timer`而非预先计算好的`enqueue_timer`分支。`internal_add_timer`实际通过calc_wheel_index再`enqueue_timer`，后者调用hlist_add_head和timer_set_idx；wheel的时钟推进/回调派发未审，不认证timer真的到期（B15/B16/B19–B21）。
锁基与迁移另受当前flags、base标志和`base->running_timer != timer`控制；不等base且不是running时，片段可解锁旧base、锁新base、改CPU/迁移标志。只认证源码门和先行函数正常返回条件，不签SMP迁移或锁内存序正确。`__mod_timer`返回int ret（可来自此前pending状态），但`schedule_timeout`直接忽略它，不能视为注册成功凭据（B13/B14/B17/B29）。
`del_timer_sync`是`timer_delete_sync`别名，其实际包装调用`__timer_delete_sync(timer,false)`；后者逐轮调用`__try_to_del_timer_sync`。当`base->running_timer == timer`时，初始`ret=-1`保留；否则`detach_if_pending(timer,base,true)`分别对不pending返回0、pending摘链清`entry.pprev`后返回1。外层对负值`cpu_relax`并重试直到ret非负；**它不是经源码直接证明的“总会完成”保证**。此调用传`shutdown=false`，故不因本调用主动把`timer.function`置NULL；即便返回正常也不能排除未审外部再登记，亦不能证明timer回调已执行/没有运行/栈对象安全（B22–B26）。
有限分支实际是`__mod_timer`、注释`// schedule();`、删除请求、空的`destroy_timer_on_stack`包装，正常返回仍为原正timeout（B27/B28、50既有结论）。任何步骤不正常返回就停在最早证据边界；`MAX_SCHEDULE_TIMEOUT`另走活动schedule，不适用此有限分支的无直接schedule断言。A的失配候选不作为B内部事实前提。

## C · 固定树实际调用点与49–51契约矩阵
从固定time顶层Git tree定位`mykernel`目录，递归树完整且未截断；**205/205个`.c`文件均以原Git blob全文按名称模式扫描，读取失败0**，再人工区分明显注释/定义/调用。该完整覆盖只限`.c`，不是全部632个`.h`、宏展开、汇编和目标编译链接。结果：所读`.c`文本中`down_timeout`仅自身定义、`msleep`仅自身定义，未发现其它外部直接调用；`schedule_timeout`在timer.c包装中直接调用，`semaphore.h`慢路直接调用，而`completion.h`把它作为函数指针传入。**不据此断言完整MyOS2中无人调用或真实运行可达性。**
实际外部completion调用者跨多个固定源可见：`devtmpfs.c`的`setup_done`与`req.done`两种不同事件；`kthread.c`的局部栈done、create->done；`init/main.c`的静态`kthreadd_done`；键盘的`getcode_done`；IDE、NVMe、XHCI请求中的`node->done`指向各局部done（各准确位置与blob见facts.C.direct_caller_sites）。这些都是源码中出现的直接调用，**不是已验证的构建选中/执行时序**。
合并后的接口区分：`complete`生产`completion.done`并请求swait wake、公开`wait_for_completion`是void且固定MAX，在`wait_for_common`内部把schedule_timeout作为action，不把long交C调用方。`down_timeout`消费普通semaphore count/授予flag并返回int(成功0/失败-ETIME)；公开不可中断state并不会经已读signal gate直接给-EINTR。`msleep`将毫秒转换结果作为timeout交给不可中断包装并循环赋回long值，若结果可表示的有限正数且callee正常返回，则源级每轮不递减；没有证据说明具体实机等待长度（A16–A19，C01–C06及R49/R50/R51）。
devtmpfs中的worker挂载早退不会走该worker的setup_done complete；另有req.done路径是worker写req.err后complete再kfree(req)，提交者在wait_for_completion后读req->err。**这只识别身份、读值与释放的源级相邻关系，不认证请求对象寿命安全，也不在本包判已发生UAF或死锁。**kthread创建方将局部done传入create，再经wait取得create->result，实际生产路径有complete(done)；是否真正调度、对象是否始终存活仍独立待证。错误码int、内部long/公开void、队列count、waiter.up和worker.err互不等价。

## 三专题交付身份与边界
A/B/C均`completed_in_scope`，无技术依赖冲突要求停任何主题。已确认可复用：历史CA-03/CA-04、R48–R51；本次新证：counted头helper与普通摘链分岔、MOD_TIMER_NOTPENDING精确登记/取消、fixed time `.c`调用点索引。需要降级的断言：全局FIFO、队列误取必然崩溃、timer回调必然触发/已取消、实际睡眠/阻塞、所有直接调用点已经完整枚举。运行/编译、完整头文件索引、并发对象寿命和实际跨CPU链均未知。`Graphviz/完整P9/内核C与脚本/QEMU/硬件`均NOT_RUN；提交后唯一交C04独立验收。
