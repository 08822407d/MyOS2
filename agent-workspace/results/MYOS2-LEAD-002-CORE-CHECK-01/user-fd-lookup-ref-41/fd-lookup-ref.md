---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FD-LOOKUP-REF-41
record_type: bounded_fd_lookup_reference_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
contract_short12: b90b8a2c34d4
result_base_short12: 46f5327a8cdf
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第41批｜fd取回、对象有效性与引用归还：固定源码的局部契约

## G01 三组取回家族的输入、范围、出口

固定time的files_struct_s内嵌file_s* fd_array[NR_OPEN_DEFAULT]且NR_OPEN_DEFAULT=256。fd_count是另一个unsigned long字段；init_files静态值0，dup_fd对新files写256。这两个源头不等于所有运行任务fd_count恒等物理容量。旧R40-S01中“fd选择不等于预留”、索引类型转换与对象有效性限制继续有效。证据FG41-A006–A009。

| 函数 | 形参与转换 | 源码实际门 | 对槽、引用的实际动作 | 返回 |
|---|---|---|---|---|
| __simple_fget | uint fd, fmode_t mask | fd < files.fd_count；没有单独fd < 256门 | 门内读fd_array[fd]，立即atomic_long_inc(&retval->f_count)，不检查retval NULL，不使用mask | 合法file已增引用后返回；未入门正常NULL |
| fget | uint fd | 转调用__simple_fget(fd,FMODE_PATH) | 同上，不是轻量fdget | file_s* |
| myos_fdget_pos | int fd | 无物理/逻辑边界门 | 直接curr->files->fd_array[fd]，无f_count修改 | fd_s{file,flags=0} |
| __to_fd | ulong v | 无边界门 | 直接files->fd_array[v]，无f_count修改 | fd_s{file,flags=3} |
| fdget、fdget_raw | uint fd→ulong | 无新增门 | __to_fd(fd)，无f_count修改 | fd_s，flags=3 |
| fdget_pos | int fd→ulong | 无新增门 | __to_fd(fd)，无f_count修改 | fd_s，flags=3 |

证据：FG41-A001–A005、A010–A016、A025。file_s*本身与带file和flags的fd_s不是同一返回协议；flags=3也不代表取回了引用。fdget_pos直接以负int作入参时，向unsigned long转换按该类型宽度模运算，-1成为ULONG_MAX，再进行无边界的数组索引。对ksys_write的unsigned int→int入参若超出INT_MAX，其值是实现定义的，不能统一声称必然变成-1；ksys_read的uint→int同理。对下标不合法的模型，在首次数组访问处停止，不继续推导caller的-EBADF。

__simple_fget的逻辑范围内空槽更不同于轻量函数：retval为NULL时立刻对retval->f_count求成员地址并原子递增；这个访问已缺失有效对象前提，不会先正常return NULL供mmap的if(!file)判断。逻辑范围外则根本不读槽，retval保持NULL而正常返回。若fd_count>256且fd=256，逻辑门可能通过，但在数组读处先失效；有效非NULL指针也不能仅凭非NULL证明是合法file，ERR_PTR/悬空/未初始化槽的原子访问不可认证。mask虽出现在签名，活动体没有使用它：fget传的FMODE_PATH（0x4000）并不构成本层模式筛选；深层VM是否拒绝此类file仍未知（FG41-A001–A003、A021）。

## G02 借用/引用取得/归还标签分开

| API/对象 | 有效file上的局部f_count变化 | 归还行为 | 限制 |
|---|---|---|---|
| fget→__simple_fget | +1 | caller显式fput才配平 | 需要逻辑与物理下标都有效、真实file对象 |
| myos_fdget_pos | 0 | flags=0，fdput_pos→fdput不调用fput | 只证明借用，未认证并发安全 |
| fdget/fdget_raw/fdget_pos | 0 | flags=3且FDPUT_FPUT=1，调用fdput后-1 | **原US-01已记录**，非本批重新立项 |
| fdput | flags&1成立才调用fput | fput减计数；到0调用kmem_cache_free | 源码调用不等于实测物理回收 |
| fdput_pos | 只调用fdput | FDPUT_POS_UNLOCK=2的解锁语句被注释 | 不可由标签名臆断解锁 |
| fput_light | fput_needed真则调用fput | 本批未确认消费此helper的调用点 | 原语与调用者职责分开 |
| get_file | +1 | 返回同一file指针 | 不能说fd_install必须在本层增加引用 |

在有效对象、正初始计数n、原子操作正常且无其它写者的**有限计数模型**中：fget之后正常fput产生n→n+1→n；myos_fdget_pos+fdput_pos(flags=0)产生n→n；fdget家族+fdput(flags=3)产生n→n−1。n=1时最后一种使fput走缓存free调用；并非已证UAF或回收实际正确。R40-S01也要求安装不增/减计数与调用者转移已有引用严格区分。证据FG41-A011/A012、A017–A020、A023/A024、A039/A040。

历史去重：CORE-USER-SYSCALL-STDIO-08::US-01（flags3却无inc）以及US-02/03、CORE-USER-VFS-BASELINE-05::UV-09/10/11均保持原状态；close/fork/dup/exec全轨道不是本批范围。第13批区分file/f_mapping/page引用，第14批区分VMA自身持有的get_file，本轮仅取历史限定，不重新核验VM内部生命周期。

## G03 三个局部调用者及最早有效前缀

**A. mmap包装层**：MYOS_SYSCALL_DEFINE6(mmap)的fd形参是unsigned long，fget形参是uint。在x86-64 LP64数值模型（ulong64、uint32）下，隐式窄化令fget收到原fd模2^32。例如原fd=4294967298（2^32+2）转换为2；若offset页对齐、非匿名、逻辑/物理表内索引2持有效file，则包装层可把该file传给vm_mmap_pgoff，未从原64位fd主动拒绝。这是一个有前提的模型，不是实机映射成功认证。低32位超出fd_count时fget可正常返回NULL。

源码控制顺序为：off非对齐直接-EINVAL；否则非匿名则fget(fd)→if(!file)→-EBADF或继续；匿名且MAP_HUGETLB进入while(1)，不正常返回；其它匿名路径没有fget。非匿名合法file由fget局部+1，再调用opaque的vm_mmap_pgoff；被调**正常返回之后**，out_fput在file!=NULL时fput，局部临时引用配平，和retval的正负无关。匿名普通路径将NULL传给opaque vm_mmap_pgoff，out_fput不减引用。注释掉的ksys_mmap_pgoff不是实际调用。逻辑范围内空槽已在fget内失效，调用者的if(!file)无法弥补；逻辑范围外正常NULL才走-EBADF。被调如果不正常返回，不声称out_fput执行。证据FG41-A026–A030。

第13/14批中的文件VMA可能自己get_file，属vm_mmap_pgoff以下的独立持有与释放范畴；包装层+1/-1不是VMA全生命周期引用的总和。vm_mmap_pgoff保持opaque，本轮不展开do_mmap、回滚、VMA、页缓存/PTE、munmap或具体返回值。

**B. ksys_read**：uint fd经形参转换到myos_fdget_pos(int)，后者直接取槽、flags=0。若该访问正常得到NULL，caller保留-EBADF，不进入f.file真分支，也不调用fdput_pos；正常得到有效file后，file_ppos仅按FMODE_STREAM决定位置指针，调用opaque vfs_read，ret>=0且ppos非NULL时才写f_pos，但无论正常返回值正负均调用fdput_pos。因flags=0，不fput。vfs_read不正常返回则后继不可认证。无效下标在caller判空前停。证据FG41-A031–A034、A038。

**C. ksys_write**：unsigned int fd→fdget_pos(int)→__to_fd(ulong)，返回flags=3；正常得到NULL则-EBADF（无fdput）；有效file后file_ppos及opaque vfs_write正常返回时，仅ret>=0且ppos非NULL才更新f_pos，但不论ret正负均调用fdput_pos，最终经FDPUT_FPUT触发fput一次。这里没有配对本地增引用；是旧US-01的限定重证。无效下标、无效file或被调不正常返回各有更早停止点。证据FG41-A035–A037。注意代码之外没有可证统一“fdget等于fget”的别名关系。

| 模型输入/前提 | 索引/真实检查 | 首个终止点与局部归还 |
|---|---|---|
| fget fdCount内有效file | 逻辑门→物理槽有效→inc | 裸指针返回，引用+1 |
| fget fdCount内NULL | 逻辑门→槽NULL→inc | 在无效NULL成员访问处停止，非正常返回NULL |
| fget fd>=fdCount | 不访问槽 | 正常NULL；mmap非匿名caller→-EBADF |
| fget fd=256,fdCount=257 | 逻辑门通过，物理数组仅256 | 槽读越界，后续不可认证 |
| mmap非匿名、高32位非零 | ulong→uint窄化 | 按低32位槽分流，非原fd完整范围门 |
| mmap有效file且opaque正常返回 | fget +1→VM调用→fput -1 | 包装层净0；VM内部未知 |
| mmap普通匿名 | 不做fd索引 | 以NULL file调VM，正常返回不fput |
| mmap匿名HUGETLB | while(1) | 无已证正常返回 |
| read/write表内NULL | helper正常返回带标签结构，caller判空 | -EBADF；无fdput_pos调用 |
| read合法file | flags0、opaque read正常返回 | fdput_pos但不fput，净0 |
| write合法file，初值1 | flags3、opaque write正常返回 | fdput_pos→fput；可能走缓存free调用，净-1 |
| read/write非法索引或无效对象 | helper无本地索引门 | 在槽访问/对象使用的最早不合法位置停，不能补出-EBADF |

上表的“有效索引/对象”“opaque调用正常返回”“计数无其他写者”是**分析附加前提**，不是内核源码新增的检查门。errno路径、函数返回及编译行为均限所选C源语义，不认证实际用户程序。

## 新增延期与独立验收边界

本批只新立有条件的FG41-F01：fget在逻辑范围内空槽无NULL保护；FG41-F02：mmap的ulong→uint窄化可能别名；FG41-F03：fget的mask参数在本地无过滤。每项的反证、证据、未知及恢复门见deferred-findings.yaml，不将旧UV-11或US-01同义重报。

第39批文档闭合和第40批限定PASS_PENDING_LOCAL已由C04回执记录，仍必须合读R39-S01/S02、R40-S01/S02；本批执行者不复核或撤销其裁决。固定源静态范围内可解释各正常前缀，但没有编译、运行、实际fd/IO、VMA/页表、锁RCU/并发、页缓存、引用全生命周期、未知外部libc或付费模型认证。交付只是READY_FOR_MAINLINE_REVIEW，mainline_acceptance=PENDING，最高可由C04独立裁定PASS_PENDING_LOCAL。
