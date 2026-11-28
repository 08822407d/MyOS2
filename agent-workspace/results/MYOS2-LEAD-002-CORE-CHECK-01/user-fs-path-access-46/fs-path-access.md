---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FS-PATH-ACCESS-46
record_type: bounded_taskfs_root_pwd_direct_access_and_reference_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
produced_by: ChatGPT普通对话（Owner声明GPT-6＋Extra，实际后台身份不可独立认证）
date: 2026-11-06
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
contract_short12: 73454015b3e5
result_base_short12: 207c87cda0e6
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第46批｜taskfs root/pwd读写：路径值、调用与真实引用效果分离

## 1. 固定对象和归属

本轮只读固定time@a039d9803ade，审查`get_fs_root/get_fs_pwd`、`set_fs_root/set_fs_pwd`及直接的`path_get/path_put/dget/dput`定义。对象从内到外至少有：`taskfs_s`上下文与锁、其中两个**内嵌**`path_s root,pwd`、调用者独立持有的输入/输出`path_s`存储位置、`path_s`中的`vfsmount_s* mnt`和`dentry_s* dentry`指针值、被指向的挂载与目录项对象。复制一个path_s的两个指针不是深复制指向对象；调用以路径为参数的helper也不自动证明引用计数变化。直接证据FP46-A001、A040–A041；45已登记的FS45-F02必须合读R45-S01的活动dget/dput限定。

四层语义在下面分别登记：
1. **VALUE_COPY**：`*out=fs->root/pwd`或`fs->root/pwd=*path`，仅复制path_s值（mnt/dentry指针）。
2. **CALL_REQUEST**：调用`path_get/path_put`，是否发出由实际源码门和先后顺序决定。
3. **DIRECT_EFFECT**：path_get活动调用`dget(path->dentry)`，path_put活动调用`dput(path->dentry)`；dget仅返回原指针且其返回值被丢弃，dput活动体为空；mntget/mntput均为注释。已读这两层没有实际目录项或挂载引用计数增减。
4. **LIFETIME**：引用是否由其它操作持有、对象何时失效/被释放、锁/RCU/编译配置影响等仍**UNKNOWN/NOT_RUN**，不能把当前无操作的helper自动升级为全系统已发生的UAF/泄漏或已证明安全。

## 2. P01｜直接辅助体与dentry对象

固定`include/linux/fs/path.h::path_s`真实体为包含mnt/dentry两个指针的结构。固定`fs/vfs/namei.c`的`path_get(const path_s *path)`活动体仅包含`dget(path->dentry)`；同文件`path_put`活动体仅包含`dput(path->dentry)`。`// mntget(path->mnt)`与`// mntput(path->mnt)`是非活动注释，当前两个包装体根本没有读取path.mnt。两者声明/定义都是`void`，没有供外层消费的正常失败errno。证据FP46-A001、A012–A017、A056–A057。

进一步完整读取`include/linux/fs/dcache.h::dget`实际函数体和`fs/vfs/dcache.c::dput`：前者的`if(dentry)`和`lockref_get`都被注释，唯一活动语句是`return dentry;`，返回类型为`dentry_s*`；path_get只将其作为无接收变量的表达式调用，既无计数修改也不保留返回对象。`dput`为`void`函数，其中while循环、fast_dput、retain_dentry、dentry_kill和潜在释放流程均在注释里，**无任何活动函数体语句**。dentry_s定义中的`d_lockref`字段也被注释，并非可访问的活动结构成员；只能对该类型定义和这两个函数下结论，不得宣称全仓无其他引用机制。证据FP46-A002–A011、A058–A059。

输入分类非常关键：`path=NULL`时`path->dentry`字段读取在进入dget/dput之前已经失去合法输入前提；`path`本身有效但`path.dentry=NULL`时字段读取有定义，这两个选定直接helper仅接收NULL指针值（dget返回NULL；dput体空），**不会按函数名发生引用增减或析构**。对未初始化、陷阱值、失效指针的求值/调用及目标ABI不能一概保证安全，即便后续函数体没有解引用。有效非NULL目录项也只被值传给无操作helper，不能据此证明目录项永远存活。任何锁、调度及多线程真实副作用都不在本轮审计。

## 3. P02｜两个getter真实顺序、输出复制及别名模型

`sched/misc/fs_struct.h`两个getter的活动体结构一致：

| 函数 | 源码固定顺序 | 本地可认证效果 |
|---|---|---|
| get_fs_root(fs,root) | `spin_lock(&fs->lock)` → `*root=fs->root` → `path_get(root)` → `spin_unlock(&fs->lock)` | 有效fs、输出变量和锁正常时把当前root两个指针值复制到输出；path_get→dget只是返回值丢弃，不取得可见新引用 |
| get_fs_pwd(fs,pwd) | `spin_lock(&fs->lock)` → `*pwd=fs->pwd` → `path_get(pwd)` → `spin_unlock(&fs->lock)` | 对当前pwd同样按值复制并发出一次无实际计数变化的请求 |

证据FP46-A018–A025、A052–A053。两函数均`void`，没有对fs、输出、输出原有持有或dentry有效性作源码检查，也没有`path_put`输出旧值的步骤。若输出本来含另一条路径，确实被赋值覆盖；是否存在需要归还的旧引用、该责任属于谁或有无真实泄漏，必须有外部调用约定/有效持有证据，不能仅凭覆盖定性。

条件性模型：fs为空/无效时最早在`spin_lock(&fs->lock)`处失效；fs有效但输出NULL/不可写时先加锁、到`*out=`发生无效访问，不能把之后path_get和unlock说成必达。fs、输出均有效且内嵌dentry为NULL时，输出获得NULL值，path_get调用dget(NULL)只返回NULL，正常到解锁（如锁也正常）；不构成自动失败errno。

若输出别名`&fs->root`并调用get_fs_root，是路径值自赋值，仍有一次path_get请求但无活动引用增量；若输出为`&fs->pwd`而调用get_fs_root，在有效别名和锁前提下会把root值复制进pwd字段，可能改变内部pwd值。此是**通用helper的条件输入模型**，不是实际调用者使用这种别名的证据，故不登记为已发生的cwd覆盖缺陷。反向get_fs_pwd把值写进`&fs->root`同理。不同mnt而相同dentry的指针组合依旧照原值复制，不因dget只看dentry而得到mount引用。通过代码搜索在**默认分支检索**get_fs_root/get_fs_pwd时只返回定义自身，未定位独立调用点；未对固定time全仓做严格穷尽，不能断言本系统实际没有getter调用者。

函数体的可选编译绑定在`fs_struct.h`以DEBUG/FS_STRUCT_DEFINATION与PREFIX_STATIC_INLINE控制；已读所选宏上下文，但未运行预处理或实际编译链接。上述均是**选定源码定义体**的局部模型，不冒充所有目标编译配置已认证。

## 4. P03｜两个setter真实更新和旧路径请求

`sched/misc/fs_struct.c`的setter控制顺序一致，注意第一步位于锁外，而旧值归还请求也在解锁之后：

| 函数 | 活动顺序 | 有效前提下的局部后置状态 |
|---|---|---|
| set_fs_root(fs,path) | `path_get(path)` → `spin_lock(&fs->lock)` → `old_root=fs->root` → `fs->root=*path` → `spin_unlock` → `if(old_root.dentry) path_put(&old_root)` | root写入新mnt/dentry指针值；非NULL旧dentry才发出path_put请求；两端直接helper不做引用计数操作 |
| set_fs_pwd(fs,path) | `path_get(path)` → `spin_lock(&fs->lock)` → `old_pwd=fs->pwd` → `fs->pwd=*path` → `spin_unlock` → `if(old_pwd.dentry) path_put(&old_pwd)` | pwd更新同理，未独立检查旧mnt成员 |

证据FP46-A026–A039、A054–A055。二者`void`返回，不创建`-EFAULT/-ENOMEM`等错误码，也无本地失败回滚。条件路径必须保持真实顺序：如果输入path无效，最早在`path_get(path)`对`path->dentry`取值处停止，不能先认证已加锁/更新内部root/pwd；若path有效而fs无效，则路径helper调用可在锁前正常结束，但后续锁访问无效，不能据此认定已发布新值。若锁或复制不正常完成，也不补出解锁、旧路径归还或最终return。

旧值`dentry=NULL`时不进入path_put，即使旧`mnt`非NULL也一样，因为实际判断只读取`old_root.dentry/old_pwd.dentry`。非NULL旧dentry（无论旧mnt是否与新mnt相同）都会**请求**一次path_put；但当前请求下游dput没有活动动作，mntput也未执行，所以此处不意味着任何实测-1或析构。新旧path拥有相同dentry/mnt时，值更新为同样指针，仍发生一次path_get请求，并在旧dentry非NULL时一次path_put请求；不能把它们想当然简化成实际引用+1/-1相抵。不同mnt但同dentry亦如此。输入`&fs->root`传set_fs_root属于条件性自赋值，path_get在锁外读取旧内部字段；并发对该输入的合法性不在本批证明。输入`&fs->pwd`传set_fs_root会在锁内将pwd值复制给root，属于有效参数时的字段效果，不是已证实机用户调用。

**实际setter调用场景**：固定time`mykernel/fs/vfs/namespace.c::init_mount_tree`在`vfs_kern_mount`返回的mnt通过IS_ERR检查后的分支，设置局部`path_s root`的`root.mnt=mnt`、`root.dentry=mnt->mnt_root`，再**先**`set_fs_pwd(curr->fs,&root)`、**后**`set_fs_root(curr->fs,&root)`。即一个局部path值向两个fs内嵌字段分别发布，各触发一次path_get请求；在当前dget无操作体里并未因两次调用而获得两份真实目录项引用。需要curr->fs和mnt、mnt_root有效、各锁和调用正常返回；未展开实际mount系统、初始化最终状态或fs对象物理生命周期。证据FP46-A044–A049。

## 5. 历史关联、发现数与安全上限

本次没有强行新建与旧问题同义的FP46-Fxx：`FS45-F02`已登记path_get/path_put包装层未活动的mnt引用操作，而其R45-S01已经明示dget为仅return、dput体空。本批补全两组getter/setter的时间顺序、目录项实际结构和真实caller，属于对`CORE-USER-FS-CONTEXT-45::FS45-F02`的证据追加与`CORE-USER-VFS-BASELINE-05::UV-10`路径副作用段的**收窄解释**，不是新根因或对旧问题的关闭。UV-10的close绕过f_count问题依然是原任务号和状态。静态init_fs最终归还问题`FS45-F01`、31::PI31-F05及其主线限定也不由本批访问接口自动修复或关闭。若未来有真实getter输出别名生产者、完整路径所有权约定或独立运行证据，再由Owner限定新专项，不在此推断全仓内存/挂载引用安全。

本批仅纯静态读取和有限条件模型，**NOT_RUN**：完整chdir/chroot、mount/namespace、路径解析、dentry销毁、inode、锁/RCU/并发协议、SLAB、fork/exec/exit、真实用户程序、C/ASM/脚本、预处理/编译、QEMU/硬件、Graphviz及完整本地P9。没有内核修改、补丁、合并或阶段3授权。执行方申报READY_FOR_MAINLINE_REVIEW，`mainline_acceptance=PENDING`，上限仅PASS_PENDING_LOCAL。
