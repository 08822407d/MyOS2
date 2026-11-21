---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FS-CONTEXT-CWD-60
record_type: bounded_static_fs_context_cwd_contract
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-07
conversation_display_name: MYOS2-A-C05a 代码分析执行
model_per_owner: GPT-6
reasoning_effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-08
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: 2b3aec6f71e1
fixed_contract90_short12: 94777b770c43
read_channel: GitHub_connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
subjects: {A: completed_in_scope, B: completed_in_scope, C: completed_in_scope}
prior_return_items: []
phase: 2
runtime_evidence: NOT_RUN
---

# 第60批：taskfs路径更新、getcwd逆向构造与复制共享退出

本文件只陈述固定time上的有界源级事实。对caller实参求值、callee正常返回、有效指针、整数范围、实际写用户缓冲、被调用的锁和引用操作、最终物理释放分别建模；失去任何前提后，停止在最早不合法表达式，不把后面的源码标签、errno或注释补作已执行。A/B/C证据索引见evidence.md，结构化反例见facts-and-boundaries.yaml。本批无D00返工。

## G00历史消费：58 B25有效，59已限定接收

完整读取90号及固定94777b770c43的58 DOC-CLOSURE和59主线回执，按R58C-S01将原58的B25认定为真实连续5行、原审查的空行/6行指控正式撤回。B24跨独立宏问题已由59 D01/D02更正，R58-01由C05关闭，58组合材料PASS_PENDING_LOCAL。59的新A/B/C已获PASS_PENDING_LOCAL，需合读R59-S01/S02；59 D03只是重复确认原本合格的B25，旧59中“两处真实缺陷”的叙述不应继续使用。原58共87个标签的准确读法：B24一项纠正、B25一项经独立重验有效、其余85项历史保留未全部重验。第57批和56及52/53既有裁定不重开。本批没有D00，也不修改任何历史文件或评论。

为避免将既有根因当新发现，额外查阅R31-S01/S02、R32-S01/S02、R43-S01/S02、R45-S01/S02及R46C-S01、R47-S01/S02，按真实身份链接旧FS45-F01/F02、IP47-F01、FT43-F01及PI31-F05；相应批次的限定接收不代表来源问题修复。历史status保存在facts.prior_findings_links；本批FC60条目只记录新增chdir/getcwd消费者或明确的跨边界推论。

## A / G01：chdir、root/pwd来源与更新

### A1. 用户chdir与内核初始化路径是两种写入方式

open.c的用户chdir宏调用体以LOOKUP_FOLLOW|LOOKUP_DIRECTORY调用user_path_at。真实包装先经user_path_at_empty，再经过getname、filename_lookup和path_lookupat；在该查找链有效、err最终为0时才把nd.path的两指针值复制到输出path，随后清空局部nd.path字段。若查找返回非零，chdir走out返回当前error，此活动体未改pwd；若正常返回0，直接执行curr->fs->pwd = path。此赋值把vfsmount与dentry成员按值写入当前taskfs，无活动set_fs_pwd调用，没有独立旧pwd保存、锁或path_put(&old_pwd)。它不改变root。lookup flags的名字、被注释的retry与path_put和被注释的权限检查均不能替代真实类型门、权限决定或资源归还。无效current/fs或不合法输出路径会使后继推导更早停止。证据A01–A05/A21/A30–A34；FC60-F01。

init.c::init_chdir则在kern_path成功返回0后调用set_fs_pwd，随后path_put(&path)；init_chroot对相同LOOKUP_FOLLOW|LOOKUP_DIRECTORY成功结果调用set_fs_root并path_put，权限/能力和security相关检查是注释。init_chroot只改root成员，没有在本函数内同时更新pwd。错误返回则在setter之前退出，不能当作已经修改cwd/root；两者是内核调用，不自动代表用户chroot syscall注册或权限机制。此前devtmpfsd未使用两次返回值的IP47-F01保持原状态，不在此重复编号。证据A06–A12。

### A2. setter、path_get/path_put只是请求，不是引用的真实增减

set_fs_root与set_fs_pwd各自先在fs->lock外对输入path调用path_get，然后锁内保存旧path_s、按值写新path_s，解锁后仅在旧dentry非NULL时请求path_put(old)。源码中的seqcount begin/end均被注释。若旧path.dentry=NULL但old.mnt!=NULL，仍跳过old的path_put；如果新旧路径相同，赋相等指针值不证明任何真实引用对消。在固定定义里path_get只调用dget而mntget被注释；dget直接返回参数，增引用注释；path_put只调用dput而mntput被注释；dput活动体为空。因而所选调用链没有实际dentry/mount计数增减。此前45/46已记录这一点，仍须保留其全局对象所有权/释放缺口，不能新签明确泄漏、引用配平或UAF。一个有效path_s的dentry成员为NULL不同于传入NULL path_s指针：后者在path_get读取path->dentry前就失去合法性，前者在所读dget中不因NULL dentry本身被解引用。证据A13–A26，旧FS45-F02/R46C-S01。

### A3. 初始root/pwd与本次消费者

myos_vfs.c::set_init_taskfs从current->fs取同一taskfs_s对象，将pwd/root.dentry均设为myos_root_sb->s_root，将两个mnt均设为&myos_root_mnt.mnt，最后有*curr->fs=*taskfs_p的自对象结构赋值；这不额外分配两个path对象或取得引用，所有指针及之前的root superblock/mount生产还需有效。path_init中的实际绝对路径起点选择current->fs->root，非绝对起点选择pwd，未以LOOKUP_DIRECTORY一位就独立执行类型验证。有限矩阵区分查找失败、有效同/异目标、旧dentry为NULL、setter入参path=NULL和根/pwd不同时更新。有效CLONE_FS共享taskfs时，用户chdir的直接赋值也修改两任务共同指向的fs对象；这是同址成员赋值事实，不认证实际并发或观察到竞态。证据A27–A36、C04–C09。

## B / G02：getcwd真实逆向拼接、溢出与返回

### B1. buffer写法和真正的主调用链

d_path.c完整活动体里prpndbuf_s是char* buf和int len。尽管同文件有DECLARE_BUFFER、extract_string、prepend_char等定义，getcwd实际创建局部b后设置b.len=PATH_MAX(4096)、bufhead=kzalloc(b.len)、b.buf=bufhead+PATH_MAX，随后myos_prepend(&b,"",1)请求写入末尾NUL。当前函数不检查kzalloc返回NULL；如果分配失败，最早合法性缺口是对bufhead进行PATH_MAX指针偏移，不能跨过该表达式捏造安全-ENOMEM返回、后续memcpy或kfree成功。extract_string/prepend_char未在本getcwd活动体中被调用，只能作局部helper对照，不画成必经步骤。证据B01–B07/B22–B25；FC60-F02。

在所选成功分配和有效数据条件下，myos_prepend_path把当前pwd、root交给__prepend_path，后者以dentry以及mount两项都等于root为终止门。普通dentry沿READ_ONCE(d_parent)向上行走并调用prepend_name；在挂载根遇有实际parent的mount时改用mnt_mountpoint和mnt_parent继续，无parent时返回2（detached），当dentry与自己的parent相等时返回3（escaped）。名称追加是反向在p->buf之前预留1字节斜线加dlen，再复制名字；p->len是int，但dlen由size_t的qstr.len窄化为uint32_t。它会在名字中途遇NUL时提前停止复制，仍以声明dlen预留空间；数值与源、目标缓冲、链表拓扑有效才有后继。极端大长度、dlen+1 unsigned溢出及窄化只作为helper-only类型模型，不宣称真实用户路径产生该值。READ_ONCE与注释的RCU/rename_lock不能认证并发安全，循环终止还需实际有限合法父链。证据B08–B17/B31–B33；FC60-F05。

myos_prepend_path中确有restart标签，但没有活动goto restart、seqcount验证或重新执行的循环。error==3时两次相同b=*p重置局部缓冲状态；如果没有追加目录名（b.len==p.len），会请求前置根斜线；getcwd对myos_prepend_path返回正值(2或3)再前置13字节"(unreachable)"，没有据正返回直接转成ENOENT。error=2和3的真正在实际挂载中的生产仍需对象条件，不自动证明root外路径可见。证据B18–B21/B25。myos_prepend的返回true不一定说明已复制：它在进入时p.len非负就先减namelen，减后变负时跳过memcpy，但无条件return true；因此该布尔不能单独作为缓冲有效证明。不过getcwd稍后仍有长度上界门，不能由这个helper局部问题自动宣布越界复制成功（B05–B07；FC60-F04）。

### B2. 数值与内存完整性逐层分开

一般普通有效单一mount模型里，每个非根目录项消耗1斜线+d_name.len字节，另加最开始的一个尾NUL。root本身先追加一个斜线；例子：pwd=root时结果应为"/"加NUL、源需求2字节；pwd=/a为3字节；/a/b为5字节；/usr/bin/app为13字节。假设仅一段名字长4094、无早NUL且全部对象有效，则该helper的需求4096刚好用满PATH_MAX；4095则使prepend_name的剩余长度变-1、该次复制跳过，之后的上界门在有效有限算术下产生-ENAMETOOLONG。两者是helper-only，实际FAT名字/路径上游界限未证明。前缀"(unreachable)/"和NUL共15字节只在无先前路径片段的detached或escaped样例成立，不能套所有挂载拓扑。有限例详见facts.cwd_buffer_return_traces。证据B08–B30。

getcwd接着计算unsigned len=PATH_MAX-b.len，依次测试len>PATH_MAX→-ENAMETOOLONG、len>用户给定size→-ERANGE、copy_to_user非零余数→-EFAULT。有效root长2时size0/1→-ERANGE，size2可进入copy_to_user。copy_to_user的所读内联体通过access_ok(to,n)后请求raw_copy_to_user并返回余数，不能因此认证真实用户地址、复制已完成或真实页故障机制。对b.len过分负或p->len算术溢出的情形要在更早非法整数处停止，不能用JS任意精度强行产生最终errno。成功/负返回各自正常结束时会请求kfree(bufhead)，不能据函数名确定物理释放。证据B24–B29/B34–B35。

**成功返回契约特别不同于长度。** 源代码是当copy_to_user返回0时执行error=(long)buf，把用户提供的缓冲指针转long后赋入局部int error，再return error；并非把刚计算的len赋给error。即使在选定LP64二补数截低32位模型中，某个有效用户指针的低32位恰为0或负整数，也只能说明该模型下的结果可能与2字节路径长度不一致；不能跳过实际指针→long→int实现定义、MYOS_SYSCALL_DEFINE宏展开、真实ABI和用户缓冲有效性。现有Linux/libc约定与源代码上方的注释都不是已经执行的行为。证据B26–B29；FC60-F03。

### B3. 共享fs下的读取/写入归属

getcwd直接依次读取current->fs->pwd及root到两个局部path_s，没有调用fs_struct.h中带锁且会请求path_get的get_fs_pwd/get_fs_root；同时用户chdir也直接写pwd，而初始化setter才单独锁住自己。仅据这些活动源事实可记录“未在这些路径中建立统一读写锁/快照协议”，不能宣布具体SMP竞态发生或所有其它调用者均不加锁。NULL fs、非法dentry/mount、循环不终止、拷贝失效或源根不一致均是可先于合法输出发生的条件。证据B22/B23、A01–A04、C32/C33；FC60-F06。

## C / G03：CLONE_FS共享、复制与exit_fs

### C1. taskfs_s来源与共享分支

taskfs_s是struct fs_struct，实际字段为int users、spinlock_t lock、int umask、int in_exec以及内嵌的两个path_s root和pwd；与files_struct_s和file_s不同。CLONE_FS为0x200，而CLONE_FILES为0x400，旧43文件表按值复制及复制失败根因不属于本taskfs工作。init_task静态.fs指向init_fs，init_fs显式users=1、umask=0022，静态未显式成员按其存储期规则初始化；并不能由此认证实机初始路径或真实最后持有者。证据A35/A36、C01/C15/C26–C30。

fork.c::dup_task_struct在所选有效node及分配前缀中真实执行*tsk=*orig，因此在copy_process随后把p传给copy_fs时，tsk->fs已从orig按值带入；不是copy_fs的注释单独创造指针。必须合读R43-S01：node!=NUMA_NO_NODE时，源中分配语句受if约束，不能无条件认证成功复制。copy_fs取得current->fs，在CLONE_FS分支先检查fs->in_exec，非零直接-EAGAIN且没有users++；为零才对普通int users++并返回0，本体不重写tsk->fs。共享指针同址依赖orig/current相符、无前序修改和有效对象；此处增计数没有实际fs->lock活动保护，不能推全系统竞态。fs=NULL时先在in_exec解引用失去对象合法性，不安全产生-EAGAIN或其他errno。证据C02–C09/C31；FC60-U06/U07。

### C2. 非共享分支及浅复制的所有权

没有CLONE_FS时，copy_fs调用copy_fs_struct(fs)，把其返回值存入tsk->fs，然后NULL→-ENOMEM。copy_fs_struct先从fs_cachep分配新taskfs；分配正常NULL时不进入if(fs)的初始化块，正常返回NULL，外层可以按源码报-ENOMEM；但如果成功分配而old=NULL/非法，在读old->umask处更早失效，不能把这种情况也压成分配失败。正常新对象写users=1、in_exec=0、初始化自己的lock，先于对old->lock加锁读取old->umask，之后在旧锁内分别按值复制root与pwd、对各自调用path_get，再解锁返回。这里新taskfs对象独立；root/pwd仍只是复制两指针值，不深复制mount/dentry。path_get当前指向无活动引用计数的dget且mntget是注释，不能凭名字算作深持有。umask写在旧锁之前，不认证所有字段一致性快照或实测数据竞争。证据C09–C14及A21–A26，旧FS45-F02。

### C3. exit_fs与分支错误收尾

exit_fs先读取tsk->fs，仅非NULL时任务加锁、fs加锁，在锁内将tsk->fs=NULL，计算kill=!--fs->users，随后解锁并在kill为真时请求free_fs_struct。该函数按顺序path_put(root)、path_put(pwd)、kmem_cache_free(fs_cachep,fs)。这是三次调用请求的序列，不是已观察到dentry/mount计数递减或真实slab成功释放。合法无其它计数写者的有限例：users1→0使kill为真；users2→1不free；users0→-1尽管int有限也不代表合法拥有者；INT_MIN再减1或共享INT_MAX再加1则超有符号范围，不能继续推确定出口。静态init_fs在最后归还时可能送入动态fs_cachep释放接口的源对象不匹配，已有FS45-F01(status=needs_evidence)；本批只引用旧ID，绝不冒称实际静态对象被释放或新增同义根因。证据C15–C20/C25/C26。

copy_process先copy_files，再copy_fs；copy_fs正常失败跳bad_fork_cleanup_files并跳过exit_fs(p)，与已成功copy_fs后后续失败经bad_fork_cleanup_fs→exit_fs(p)不同。实际do_exit选定片段中先exit_files(tsk)，再exit_fs(tsk)，不把这些顺序扩展成整套fork/exec、task引用/线程或完整退出正确性认证。CLONE_FS在in_exec拒绝时没有新增users持有，虽然tsk->fs在dup_task_struct前序拷入旧值，后续失败标签跳过exit_fs，不能把未取得的持有又错误归还。对任意非正常调用/无效对象须在更早位置停止。证据C21–C24/C31，旧R45-S01/S02及R43-S01/S02。

## 有界结论、验收入口与NOT_RUN

第60批A/B/C均为completed_in_scope，指固定time选定源码层的三组分析、反例和记录，非系统正确性或运行测试。新FC60-F01..F06为六项有前提的条件候选，FC60-U01..U08为八项证据缺口；没有将旧FS45-F01/F02、IP47-F01、FT43-F01、PI31-F05或R58C-S01重新登记为独立新根因。

六件成果包括本正文、MANIFEST、JSON兼容YAML事实与63例有限模型、关系DOT、延期记录和104条1–5行直接源码锚。图标CALL/RETURN/DATA/LOCAL_CONTROL/ANALYSIS_STOP，仅为选定源级关系，不是完整CFG；分析STOP不是CPU指令、异常处理或正常源码return。固定time的源码原文、短blob与所用定义见evidence与source_ledger。

全部运行层均NOT_RUN：仓库C/ASM/脚本/旧夹具、预处理/编译/链接、完整本地P9、Graphviz、QEMU/硬件、真实路径/用户缓冲、POSIX ABI/SMP/引用/物理释放。执行者仅提交READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING，交MYOS2-A-C05独立验收（最高PASS_PENDING_LOCAL）；不自签通过、不修改旧件、不自行启动61。
