---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FD-CLOSE-42
record_type: bounded_fd_close_and_file_table_teardown_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner指定GPT-6＋Extra；后台身份不认证）
date: 2026-11-05
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
contract_short12: d7d7477c3a15
result_base_short12: ee0882d46bbf
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第42批｜关闭单个fd、file引用归还与files_struct退出清理

## 1. 总结与证据界限

固定time@a039d9803ade包含**三条不同的局部机制**：系统调用close直接调用kfree(file)，正常继续后清fd槽并返回0；filp_close调用fput，后者依file.f_count减至零才请求释放；exit_files先将tsk->files设为NULL，再由put_files_struct对files_struct.refcount减一，仅在计数归零时遍历文件槽和释放表。三者不能被统称为一种“close自动释放全部资源”语义。证据CL42-S001–S004、S011–S014、S019–S024。

本包只对所读函数有效源控制和附加前提作分层论证，不证明物理回收、真实并发、VMA/页缓存/路径回收、真实用户open/close成功或Linux兼容。旧CORE-USER-VFS-BASELINE-05::UV-09/UV-10/UV-11及08::US-01保持原编号和既有状态；R41-S01/S02、R40-S01/S02继续约束有效对象与计数推演，尤其不把“非NULL”直接解释为“有效file”。

## 2. C01：close的实际索引检查与返还

在mykernel/fs/syscall.c的MYOS_SYSCALL_DEFINE1(close, unsigned int, fd)中，活动顺序为：
1. 取得current作为curr，初始化局部fp=NULL；
2. `if(fd < 0 || fd >= curr->files->fd_count) return -EBADF;`；
3. `fp=curr->files->fd_array[fd];`；
4. 调用`kfree(fp)`；
5. `curr->files->fd_array[fd]=NULL;`；
6. 正常返回0。

fd声明为unsigned int，因此`fd<0`在标准非负取值模型中恒假；真正执行的上界比较是与unsigned long类型`fd_count`比较，**没有单独的物理256槽容量门**。字段数组实际长度取自NR_OPEN_DEFAULT=256。fd_count若为257且请求fd=256，则逻辑门可通过，但最早失去合法下标的是第3步槽读取；后续kfree、清槽和返回0均不可认证。相反fd>=fd_count可在任何数组访问前正常返回-EBADF。源码没有`if(!fp)`的空槽检查，也没有检查fp是否合法、已分配或是ERR_PTR。证据CL42-S001/S002、S005/S007。

对合法物理索引、有效files对象且槽NULL，定点读取的mykernel/mm/kmalloc/slub.h::kfree定义在NULL输入上立即return；如果这个条件编译定义被实际选中、调用正常结束且files仍有效，close随后把槽（仍为NULL）赋NULL并返回0。**这是受kfree定义选择条件约束的模型，不是所有配置/对象都已证安全的正常errno语义。** 源码kfree的实际定义受`#if defined(SLUB_DEFINATION) || !(DEBUG)`控制，未预处理确认；其最初门只检测NULL，不检测ERR_PTR、悬空或未知值。非NULL路径选择folio相关操作及可能的`kmem_cache_free`调用，后续分配器内部和真实释放未核。证据CL42-S008–S010。

有效文件且`f_count=1`或2时，close两种模型都**不读取或更新f_count**，而直接将文件指针传给kfree；这与下一节fput的计数路径不同。若同一file另有真实引用/另一槽持有，源码没有在kfree前按计数避免回收请求。早期UV-10已记录该根因，本包只补充清槽前后事件及异常前缀，不另立同义问题。只有`kfree`正常返回、表对象及索引仍有效，才可继续陈述清槽、返回0；若更早发生非法下标或非法对象访问，不能按顺序推断后续已执行。注释里的`fp->f_op->close`和`close_fd`不是活动调用，close不通过filp_close/fput。证据CL42-S002–S004。

### close最小输入矩阵

| 输入条件 | 最早事件或停止点 | 最后可证的局部结果 |
|---|---|---|
| 有效files，fd>=fd_count | 真实逻辑上界门 | -EBADF，不访问数组 |
| fd<fd_count且fd<256，槽NULL | 读NULL，再进入所选kfree的NULL早退 | kfree正常返回后赋NULL、return0（条件性） |
| 界内有效file，f_count=1 | 无引用计数门，向kfree交指针 | 正常返回后清槽并return0；实际物理回收未证 |
| 界内有效file，f_count=2或另有持有者 | 同上，不读取f_count | 与历史UV-10同一缺口；其它持有者是否实际悬空未运行 |
| fd_count=257，fd=256 | 逻辑门过，数组下标256越界 | 停于取槽，不继续认定errno/清槽 |
| files为NULL或非有效对象 | 比较中先访问files->fd_count | 无正常返回保证 |
| 界内槽含ERR_PTR/失效/未知地址 | kfree不能按NULL分支证明安全 | 只能证明尝试kfree；不冒称清槽或最终0 |
| kfree不正常返回或中间表被失效 | 尚未走到赋NULL | 不宣称清槽已完成 |

## 3. C02：filp_close、fput的计数与来源

mykernel/fs/vfs/open.c::filp_close(file_s *filp)声明返回int，先赋局部retval=0，活动体调用`fput(filp)`，最后返回retval。文件计数为0时提前返回、文件操作表flush、通知/锁清理等仅在注释内，不能复原为可执行步骤。它既没有清某个fd槽，也不直接处理files_struct.refcount。证据CL42-S011/S012。

mykernel/fs/vfs/file_table.c::fput是void，只有`atomic_long_dec_and_test(&file->f_count)`为真才进入`kmem_cache_free(filp_cachep,file)`源码调用。分开记录四层：①调用fput；②有效原子计数减至零；③执行缓存释放函数调用；④是否物理释放/何时再分配。前3层有直接源码和正常返回前提，第4层**NOT_RUN**。前提为有效file、初值1或2、无其它计数操作、计数未溢出和相关原子操作正常；无效/零初值/重复释放后的file不延伸推算。证据CL42-S013/S014。

文件来源也不是单一：`__alloc_file`经`kmem_cache_zalloc(filp_cachep,...)`，成功时给f_count设1；`files_init`创建该缓存；历史`dup_fd`则为非NULL父槽执行`kzalloc(sizeof(file_s))`、按值复制file并保存到新槽，因此该副本不自动成为filp_cachep分配对象（旧UV-09）。files_struct另由`files_cachep`表示，`dup_fd`经kmem_cache_alloc(files_cachep,...)获得表并写独立的refcount=1、fd_count=256。两种对象的缓存、两个引用计数不能混同。证据CL42-S015–S018、S027–S030、S034/S035。

`f_dupfd`可在有效操作下先`get_file(file)`、后`fd_install`形成两个槽共享同一file的来源例；增加引用与把指针安装入槽不是同一事件，但此处的实际用户入口与各类旧问题另有独立限定。别名模型只需要一个已存在的有效对象和清楚的初始计数，不据此宣布真实fork/dup正确。证据CL42-S031/S033。

| 有效文件初值及动作 | file.f_count局部变化 | 是否到达缓存free请求 | 不能推出 |
|---|---|---|---|
| n=1，filp_close→fput正常执行 | 1→0 | 进入kmem_cache_free(filp_cachep,file) | 物理回收完成、路径/页已回收 |
| n=2，filp_close→fput正常执行 | 2→1 | 否 | “关闭失败”或另一个持有者必定安全 |
| n=1，sys_close直接kfree | 没有本地计数操作 | 调用kfree；是否进入内部free依对象/编译条件 | 一定经fput或f_count归零 |
| n=2，sys_close直接kfree | 没有本地计数操作 | 同上 | 不会影响其他持有者 |
| 无效对象、零/溢出计数 | 缺少有效模型前提 | 不继续推演 | 负计数/硬件异常确定结论 |

## 4. C03：退出文件表的局部顺序

`exit_files(task_s *tsk)`先按值读取`files=tsk->files`，仅在files非NULL时进入：`task_lock(tsk)`→`tsk->files=NULL`→`task_unlock(tsk)`→`put_files_struct(files)`。NULL时整个分支跳过；局部没有遍历/释放。固定`do_exit`的一个直接调用点已定位，但没有展开整个调度/任务退出链。有效tsk、锁操作正常、保存的files对象在这段内有效是必要条件，不能由非NULL推导所有引用寿命正确。证据CL42-S023/S024/S039。

`put_files_struct(files)`首先`atomic_dec_and_test(&files->refcount)`。若原值2而正常减到1，**不进入**文件槽遍历，也不发出files_cachep的释放请求。只有假定原值1并正常减到0，才初始化`unsigned int i=0`，对每个`i<files->fd_count`读取`files->fd_array[i]`，仅在非NULL时调用`filp_close(file)`；这个返回值没有被消费，遍历时并不把槽写NULL。循环结束后请求`kmem_cache_free(files_cachep, files)`。因此退出任务清空tsk->files和释放最后一个共享表对象并非同一事件；整个表在最后持有者路径请求释放，不是活表上的普通单fd关闭。证据CL42-S019–S022。

当fd_count>256且前256次迭代均能够正常完成时，到i=256有物理越界；如果更早遇到不合法file或callee不正常返回，则以前一停止点为界。i是unsigned int而fd_count是unsigned long，比较也不是容量门。表内两个非NULL槽为**同一有效file**且进入第一槽前file.f_count=2，无其它写者、两个filp_close均正常时可建2→1→0模型；源码分别调用两次filp_close，第二次才请求文件缓存free，之后可能请求表缓存free。若初值只有1，则第一次可到1→0、请求缓存free；第二次仍会读取同一个槽指针，**无法认证其为仍有效file，也不得继续算第二次确定减计数或实际UAF**。正常的f_dupfd路径包含get_file是这个风险模型的反证，不能把人为不足的引用初值说成已发生。旧UV-09、UV-10及US-01可能影响别名/计数初态，全部不因本批模型而关闭。

额外独立候选：`init_files`是源码定义的全局静态文件表对象，refcount初值1、fd_count=0；`put_files_struct`并没有对该特殊地址设置跳过缓存free的条件。因此**如果**某次exit_files实际交给它这个全局对象且真的是最后一份files_struct引用，则零计数分支在零槽遍历后仍请求`kmem_cache_free(files_cachep,&init_files)`。静态对象并非由files_cachep的`kmem_cache_alloc`创建；这是新的、独立于UV-10的条件性来源/归还契约缺口（CL42-F01），**没有追证实机任务引用init_files并最后归还，不能写成已经发生的非法free或物理损坏**。证据CL42-S025/S026与S019/S022/S034/S035。普通问题登记，Owner本轮无修复操作。

### exit/put_files_struct有限模型与停止点

| 表/槽条件 | 实际控制与计数动作 | 最后可证状态 |
|---|---|---|
| tsk->files=NULL | exit_files的if(files)不入 | 不调用put_files_struct |
| files.refcount=2，有效表 | tsk->files置NULL；put使2→1 | 不遍历槽、不请求表缓存free |
| files.refcount=1，fd_count=0 | put使1→0，循环零次 | 请求释放files表；若是静态init_files须独立核地址来源 |
| files.refcount=1，fd_count=3、全NULL | put使1→0，三次按槽判NULL | 不调用filp_close，最后请求表缓存free |
| files.refcount=1，槽0有效file计数1 | put最后引用、读槽、filp_close→fput 1→0 | 请求file缓存free，然后请求表缓存free（均须正常返回） |
| files.refcount=1，槽0/1别名file、初值2 | 两次filp_close，f_count 2→1→0 | 第二次请求file缓存free，最后请求表缓存free |
| files.refcount=1，槽0/1别名file、初值1 | 第一次fput 1→0 | 请求file缓存free；第二次仍尝试取槽，后续对象有效性未知，停止 |
| fd_count=257且前256槽处理全部正常 | i从0至256，读取第257槽之前已越过内嵌数组 | fd_array[256]处停止，无确定最终表缓存free |
| 非NULL槽为失效对象或filp_close不正常返回 | 先进入callee；未有有效对象/正常返回保证 | 停止于该callee/对象边界，不能补出整个循环完成 |
| files.refcount无有效正值或相关对象失效 | 计数操作前提不满足 | 不预测返回、非法free的真实机后果 |

## 5. 本批发现、反证和不可越过的旧限制

本批只登记一个确切新候选CL42-F01（静态init_files在“最后引用”路径的缓存释放来源条件）；其余是对历史UV-09、UV-10、UV-11、US-01、FG41-F01/F02/F03的局部证据追加或条件模型。R41-S01中的有效对象、opaque调用与局部计数限制持续适用；R41-S02中类型别名应分别读单定义，图里的源码NULL门不应脑补有效对象门。38/39/40先前对象/路径或安装缺口也没有被新close结论修复。

本轮不修改内核、用户程序或旧成果，不给补丁，不编译运行，不执行仓库脚本，不访问Owner本机或外部libc/镜像，不演示实际文件描述符泄漏/崩溃，不证明缓存/文件/表生命周期闭环。最高只申请主线的PASS_PENDING_LOCAL，当前由执行方申报READY_FOR_MAINLINE_REVIEW / mainline_acceptance=PENDING。
