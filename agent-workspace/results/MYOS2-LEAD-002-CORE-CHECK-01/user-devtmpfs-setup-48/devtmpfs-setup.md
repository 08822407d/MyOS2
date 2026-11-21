---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-DEVTMPFS-SETUP-48
record_type: bounded_devtmpfs_creation_error_and_completion_handoff_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner声明GPT-6＋Extra，真实backend未独立证明）
date: 2026-11-06
source_pin_short12: a039d9803ade
contract_short12: b63fa979e409
result_base_short12: 6f8408772ea7
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第48批｜devtmpfs创建结果、错误传递身份与setup_done握手

## 1. 结论分层与第46/47批来源

固定源码`time@a039d9803ade`中至少存在五类彼此不同的状态：① `vfs_kern_mount`返回的挂载指针/编码错误；② `kthread_run`返回的task_s指针/编码错误；③ `devtmpfs_init`栈上`int err`；④ `devtmpfsd(void *p)`自身栈上`int err`及其函数`int`返回；⑤静态`completion_s setup_done`的完成事件。线程创建、线程执行、完成通知、等待函数返回、工作函数的int返回、调用方最终返回属于不同控制/数据通道，不能被同一个“初始化成功”概念替代。直接证据DS48-A001–A015、A019、A025–A033。

主线已在固定b63fa979e409将`R46-01`裁定`CLOSED_DOCUMENTATION_ONLY`，第46批组合材料限定`PASS_PENDING_LOCAL`；第47批亦已`PASS_PENDING_LOCAL`且`R47-S01/S02`是本批强制消费。本批不重做前者闭合或后者验收，不修改任何旧件。既有`CORE-USER-INIT-PATH-SWITCH-47::IP47-F01`专指工作函数忽略init_chdir/init_chroot返回。本批只分析线程创建/等待/跨栈err通道，对相同根因不重新编号。

## 2. D01：devtmpfs_init的实际时序与返回条件

`devtmpfs_init(void)`的活动顺序从`char opts[]="mode=0755"; int err;`及`INIT_LIST_HEADER_S(&internal_fs_type.fs_supers)`开始，之后如下：

| 来源操作和真实门 | 局部状态与合法路径 | 不可推论 |
|---|---|---|
| `mnt=vfs_kern_mount(&internal_fs_type,0,"devtmpfs",opts)`；`if(IS_ERR(mnt))` | 编码错误指针分支打印并`return PTR_ERR(mnt)`；不调用register/kthread/wait | 非ERR不等于非NULL、有效挂载或已经成功初始化文件系统 |
| `err=register_filesystem(&dev_fs_type)`；`if(err)` | 正常非零时打印并`return err`，不会在本函数此支调用后面的kthread或unregister | 不审是否存在此前部分注册副作用，不能判实际泄漏 |
| `thread=kthread_run(devtmpfsd,&err,"kdevtmpfs")` | 将调用方**栈变量err的地址**作为线程数据指针传入创建宏；宏返回的是task_s*或编码错误指针 | 不等于函数返回int启动结果，不等于工作函数已经执行或完成 |
| `if(!IS_ERR(thread)) wait_for_completion(&setup_done)` | 当结果落在非编码错误分支，源码尝试等待setup_done；要求thread是有效task及完整等待协议有定义才能推演进一步行为 | IS_ERR为假并不另外检查NULL/活性；等待不是错误码回传 |
| `else {err=PTR_ERR(thread);thread=NULL;}` | 编码错误分支不执行wait，将**创建错误码**赋给调用方err并清本地静态thread指针 | 不代表devtmpfsd已经运行并向&err回写错误 |
| `while(err);` | 位于后续`if(err)`注销分支**之前**；在该点若err为合法稳定非零且无有效其它写者，则源码层面没有正常离开这个空循环的条件变化 | 不能宣称if(err)中已执行unregister/return，亦不签真实机器永远自旋的运行结果 |
| `if(err){...unregister_filesystem(...);return err;}`及后续`pr_info; return0` | 只有先正常越过while才会测试if(err)；若err为稳定0，则跳过错误分支，正常走报告成功与return0（被调打印正常前提） | 本地正常0不认证工作线程目录切换成功、挂载对象存活或completion/调度正确 |

见DS48-A001–A010、A035–A044。成功与错误指针使用的是`IS_ERR`及`PTR_ERR`的编码域，不是对任意非NULL/非法地址的有效性检测（DS48-A035–A037）。本函数的`err`在进入kthread_run之前由注册调用正常返回写入，`register_filesystem`成功时为0；创建编码错误会在`else`重新写成错误码。对于稳定非零的创建错误，源码中的`while(err);`拦在后续if(err)分支之前；如存在其它未审但真实能修改err的写者，只能作为不同有条件模型，不能把可能性表述为已发生。这个独立候选登记为`DS48-F02`。

## 3. D02：kthread_run和两个不同栈帧的err

固定`sched/forkexec/forkexec_macro.h`的`kthread_create(threadfn,data,namefmt,...)`定义将threadfn、data和NUMA_NO_NODE透传给`kthread_create_on_node`；`kthread_run`的单个GNU statement-expression宏先执行`__k=kthread_create(...)`，然后`if(!IS_ERR(__k)) wake_up_process(__k)`，最后表达式交回`__k`。源注释中描述的唤醒接口不能替代实际活动语句；这里实际调用的是`wake_up_process`。没有读取下层创建/调度具体实现来认证成功线程已经运行，亦不将kthread_create的extern声明当函数实现。见DS48-A025–A027、A041。

调用处传`&err`属于`devtmpfs_init`这一调用方栈帧；`devtmpfsd(void *p)`的参数`p`可持有该地址**值**，但它的完整活动体并不读取、解引用或写入`p`。原本的`// int err = devtmpfs_setup(&p);`是注释而非活动行为。工作函数随后另外声明`int err=0`，将自己的`init_mount`结果写入**工作栈的局部err**，在非零时`goto out`，最后`return err`。所以“把调用方&err传进创建宏”与“工作函数有实际写回调用方err”之间缺少直接连通边；两个同名err不是一个内存单元。工作函数的int返回亦不是创建调用的task指针值，不经此代码直接存到调用方err。见DS48-A011/A012/A014/A015/A026/A041/A045/A046。

如果线程的init_mount正常返回非零，`devtmpfsd`有一个明确**没有调用setup_done通知**的早退源路径；它能否实际到达、线程是否调度并正常退出，要有更多证据。若init_mount正常返回0，则工作函数依序调用init_chdir和init_chroot（其返回被丢弃，沿用`IP47-F01`），而在两个调用都正常返回后才进入setup_done的complete。这种前置条件不表示任一次目录切换真实成功。上述未接线错误身份/状态通道按`DS48-F03`作独立候选，不和IP47的两次目录返回忽略同义重立。

## 4. D03：setup_done初始化、实际通知与等待事件

固定time中真实头文件位于`mykernel/kactive/completion/`，不是默认分支索引展示的`mykernel/include/linux/kernel/completion.h`：后一个路径在固定time查询返回未找到；经固定树确认前者的`completion_macro.h`、`completion_types.h`、`completion_api.h`及`completion.c`。实际包含入口`mykernel/kactive/kactive_types.h`及`kactive_api.h`已只读核实，未做预处理。

`static __initdata DECLARE_COMPLETION(setup_done)`是静态对象宏使用点（DS48-A019）。`DECLARE_COMPLETION(work)`单宏会声明`completion_s work = COMPLETION_INITIALIZER(work)`；`COMPLETION_INITIALIZER`单宏文本含done初值**0**和等待队列头初始化宏，而`struct completion`实际字段为`uint done`和`swqueue_hdr_s wait`（DS48-A028–A030）。**这只是在假定该宏/静态对象定义于固定目标配置有效时的源码初始化模型**，不是已运行的预处理结果、真实等待者初始状态或调度证明。

直接`completion.c::complete(completion_s *x)`在当前定义体中锁内条件性增加`done`并调用`swake_up_locked`；它接收的仅是completion_s对象地址，没有错误码形参。`wait_for_completion(completion_s *x)`真实返回类型是`void`，活动体调用`wait_for_common(...,MAX_SCHEDULE_TIMEOUT,TASK_UNINTERRUPTIBLE)`但不把内部返回值作为外层整数结果（DS48-A031–A033）。不会从此接口给`devtmpfs_init.err`自动赋线程错误码。这里仅认证两个直接函数体与调用关系，不追wait队列、锁/内存屏障、等待者唤醒和调度的完整实现。

| 线程源路径 | 本批直接setup_done事件 | 调用方在有效创建结果下的观察 |
|---|---|---|
| 创建结果为编码错误指针 | 未有合法工作线程可认为已执行；调用方**不进入wait** | 赋err=PTR_ERR、thread=NULL，之后直接遇到while(err) |
| 创建结果为有效task，但worker init_mount正常返回非零 | 该worker源分支goto out并返回本地err，**跳过complete(&setup_done)** | 调用方已进入wait_for_completion；在宏初始done=0、没有其它通知者且等待机制依赖该通知的前提下，存在无法由此worker满足的通知条件 |
| worker init_mount正常返回0且两个目录函数正常返回（即使它们返回非零int） | 执行`complete(&setup_done)`调用，再进入workloop的源码路径 | 即使通知/等待正常完成，也**不携带目录切换errno、工作函数err或完整挂载成功凭据** |
| worker/创建/路径中途不正常返回 | 后面complete事件无保证 | 不把源码后继位置当事件已发生，也不签必然死锁或wait正常结束 |

由于静态done初始化按宏可得0，而源码中devtmpfsd的非零init_mount早退分支在complete之前，**存在条件性握手缺口**`DS48-F01`；这不等于真正的内核死锁已经复现，其他未知通知者、完整等待语义及真实错误生产者未审。

`setup_done`与`req_s.done`是两个独立对象：devtmpfs_submit_req先对**每个req.done**请求init_completion，之后单独等待req.done并读取req.err；devtmpfs_work_loop对**每个req.err**写入handle结果、通知req.done。不能把请求完成事件充作初始化setup_done通知，更不能把req.err赋值当作devtmpfs_init.err赋值。见DS48-A017/A021–A024/A050。未审handle、req释放与请求队列完整执行条件。

## 5. 正常路径与异常路径的实际停止点

正常“启动已被调用并返回一个task_s指针”的路径，在task有效、工作函数实际开始运行、init_mount与两次路径函数都正常返回、complete通知及wait正常交接、调用方err保持0等前提下，`devtmpfs_init`可跳过while及注销并正常`return0`。这只证明**调用方源码路径**；工作函数的`devtmpfs_work_loop`含`while(1)`且选定完整体没有活动break/return，通知发生后它仍进入循环。不能要求工作函数正常return0才允许调用方在通知后完成，也不能将请求循环的完成通知替代setup完成。

不同错误路径必须单独停止：
- 内部vfs_kern_mount正常返回IS_ERR编码：直接`return PTR_ERR(mnt)`，不进入注册/线程创建/等待。
- register_filesystem正常返回非零：直接return该int，不进入kthread_run/等待。
- kthread_run返回IS_ERR编码：赋调用方err=PTR_ERR(thread)、thread=NULL，不wait；在合法稳定非零模型中停在`while(err)`，不能声称已经执行后续if(err)注销/return。
- kthread_run返回非ERR但为NULL/失效指针：源码进入非ERR分支，并尝试等待setup_done；**没有充分证据认证线程真实存在**，不能补出worker通知或正常返回。
- worker的init_mount正常非零：worker跳过setup_done通知并返回自己的err；调用方独立的err不会由这个return直接变化。若已等待且没有其它通知事件，存在条件性等待无通知情形，不宣称真实机器无限阻塞。
- worker两次路径切换正常非零：47已证它们的int返回未被捕获；如果两个都正常返回，worker仍调用complete（按有效对象前提），因此“已通知”也不足以证明路径成功。

`IP47-F01`旧候选限定在工作函数内部的两次目录切换结果被忽略；本批`DS48-F01`是更早的setup_done条件性缺席，`DS48-F02`是**调用者本身**while(err)对错误清理/返回的控制屏障，`DS48-F03`是两个不同栈帧err与p的状态回传契约空缺。三者的源操作/时间点/前提不同，均仅作为普通待证候选，不因当前源观察而升格为实测内核故障。

## 6. 证据等级与边界

直接固定time文本不认证任意机器层面线程执行、等待唤醒、真正挂载成功、workloop返回或内存生命周期。图中的两个执行上下文必须分开，不绘成devtmpfs_init同步调用devtmpfsd的普通call/return；只有kthread_run宏的指针/创建数据转发关系与setup_done跨上下文通知边有条件联系。**create返回task**、**worker返回int**、**complete事件**、**wait返回void**及**caller最终return int**都不可合并。

R46C-S01与R47-S01/S02保持对有限对象、宏上下文、正确路径输出、原第47批缺少的早退匹配边及workloop无已证退出的强制限制。所有旧qualified_id保持原状态，无内核修改、无编译/预处理/构建/QEMU/实机，不扩大为完整completion/调度/挂载/线程生命周期审计。本执行者只申报READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，最高由C04独立裁定PASS_PENDING_LOCAL。
