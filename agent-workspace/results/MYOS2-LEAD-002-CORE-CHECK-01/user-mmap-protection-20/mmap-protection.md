---
task_id: "CORE-USER-MMAP-PROTECTION-20"
track_id: "MYOS2-LEAD-002"
record_type: "bounded_static_mmap_permission_narrative"
execution_identity: "MYOS2-LEAD-002-CHAT-EXEC-02"
conversation_display_name: "MYOS2-A-C03b 代码分析执行续建"
produced_by: "GPT-6"
model_per_owner: "GPT-6"
effort_per_owner: "Extra（Owner选择；实际后端不可独立认证）"
date: "2026-11-01"
source_branch: "time"
source_pin_short12: "a039d9803ade"
contract_short12: "038217219faa"
initial_result_head_short12: "6166b3e2f5b9"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
mainline_acceptance: "PENDING"
runtime_evidence: "NOT_RUN"
---

# 第20批：mmap请求、访问检查、VMA标志与保护模板

## 核心结论：五层不是同一权限

MyOS2固定`time @ a039d9803ade`的mmap路径可以在静态层完整衔接 **六参数入口→字节off到页号pgoff→prot/flags归一化→文件模式检查→VMA字段/回调→vm_page_prot模板**。这些层的值不能互换；即使最末返回映射地址，也只是所读C控制流在相应前提下返回，**不是已建立PTE、CPU权限强制或用户程序成功访问的运行证明**。

有四个值得主线重点消化的源码事实：

- `do_mmap`对`PROT_READ`，只要没有file或其`path_noexec`为假，就会**额外置位`PROT_EXEC`**；并不执行被注释的`READ_IMPLIES_EXEC` personality检查（MP20-A037）。
- `vm_mmap_pgoff`的写锁包装实际调用存在，但所读`mmap_write_lock_killable`活动体只返回0，解锁接口的`up_write`也被注释。这证明**本选定包装没有真正取得所命名的mmap写锁**，不直接证明全系统没有其他同步（A021/A026–A029）。
- 同样是只读fd，私有`PROT_WRITE`映射只检查`FMODE_READ`而可继续进入回调；共享`PROT_WRITE`则先要求`FMODE_WRITE`。共享`PROT_READ`且fd无写模式时，代码把`VM_SHARED`清掉而保留`VM_MAYSHARE`（A061/A063/A064）。
- 新建VMA的`vm_page_prot`在文件回调**之前**计算；回调后如果更改了访问位，函数只核条件重合并及局部flags更新，末端`vma_set_page_prot`仍是注释。对所读FAT32→`generic_file_mmap`回调，实际只见设置`vm_ops`、不见修改访问位；所以保护失配仅是其他回调真实变位后的**条件风险**（A090/A091/A095–A102/A114–A117/A144）。

第19批回执R19-S01/S02/S03必须合读：`vm_page_prot`是上游输入，后续构造实际PTE还要通过`pfn_pte/check_pgprot/massage_pgprot`等步骤，不能把模板名字当最终PTE或原始RW必然保留。本批不是重新执行19匿名零页或18引用归还。

## P01 · syscall入口、六个实参和文件引用

固定系统调用表`syscalls_64.h`将`__NR_mmap`映射至`sys_mmap`；`syscall_64.c`的表与`x64_sys_call` switch以`__x64_##sym`接合。宏`MYOS_SYSCALL_DEFINE6(mmap)`经`__MYOS_SYSCALL_DEFINEx`生成`__x64_sys_mmap→__se_sys_mmap→__do_sys_mmap`的代码结构；六寄存器实参来源依序`di, si, dx, r10, r8, r9`，并映射为`addr, len, prot, flags, fd, off`（A001–A009）。这是**固定源码与宏文本的绑定**，未运行预处理、链接或系统调用二进制；第08批返回尾段的限定仍有效。

入口对**字节偏移**`off`执行`off & ~PAGE_MASK`检验，未页对齐直接`-EINVAL`；否则`pgoff = off >> PAGE_SHIFT`，单位从字节变为页号。若`MAP_ANONYMOUS`未置位则`fget(fd)`，定义良好且返回NULL时为`-EBADF`。置位匿名标志则本分支不访问fd、file保持NULL；另外匿名且`MAP_HUGETLB`的活动分支为`while(1)`，不走普通映射路径（A010–A014/A032–A034）。

在一个有效已占用的文件表槽上，所读`__simple_fget`按`fd<fd_count`取得`fd_array[fd]`并增加该`file_s`的`f_count`，不消费传入的mask；**若范围内slot意外为NULL**，就会在`atomic_long_inc(&retval->f_count)`处碰到无效对象风险，但未证明该输入实际可达。 `vm_mmap_pgoff`返回后，mmap入口对非NULL文件执行`fput`，这是**临时fd取用引用**；新建文件VMA后另有`vma->vm_file=get_file(file)`，又加一次引用，不能和入口临时引用混记。若原计数n、三次相关操作正常抵达且没有并发改动，新建路径可写作`n→n+1(fget)→n+2(get_file)→n+1(fput)`；合并路径不必有第二次get，错误清理另计（A012/A014–A019/A090/A091）。

`vm_mmap_pgoff`以已是页号的`pgoff`调用`do_mmap(..., vm_flags=0, pgoff)`。另一个`vm_mmap`包装接受**字节offset**、额外检查`offset+PAGE_ALIGN(len)`溢出和页内偏移，再右移；本mmap syscall直接走`vm_mmap_pgoff`而不经过这个额外检查，因此不能声称两入口检查完全相同（A020–A025）。`do_mmap`另有len为0、PAGE_ALIGN归零、pgoff+页数溢出、map_count以及选址失败的各自错误门，顺序也重要（A035/A036/A040–A042/A050–A052）。

写锁wrapper虽然有`if(mmap_write_lock_killable(mm)) return -EINTR`门，后者活动实现固定`return 0`，因此**在当前这个定义正常调用时该EINTR门不由锁函数本身产生**；`mmap_write_unlock`同样没有活动up_write。不能由调用点注释中的“已取得写锁”推定互斥（A021/A026–A029）。

## P02 · prot/flags归一化和文件访问检查

`do_mmap`首先处理`PROT_READ→PROT_EXEC`的增位，条件是`prot & PROT_READ`且`!(file && path_noexec(&file->f_path))`；若file为NULL，此第二条件恒真。若file非NULL，`path_noexec`并非桩函数，而是实际查询`path->mnt->mnt_flags & MNT_NOEXEC`或`path->mnt->mnt_sb->s_iflags & SB_I_NOEXEC`。只有请求包含显式`PROT_EXEC`或已被前置转成EXEC，才在后续noexec文件检查时出现`VM_EXEC`拒绝门（A037/A065/A150）。

随后可选`MAP_FIXED_NOREPLACE→MAP_FIXED`，再由`calc_vm_prot_bits`把有效`PROT_READ/WRITE/EXEC`映为`VM_READ/WRITE/EXEC`，`calc_vm_flag_bits`把`MAP_GROWSDOWN/LOCKED/SYNC`等转换；与`mm->def_flags`及`VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC`用OR组合（A038/A043–A049）。 `VM_MAYWRITE`是允许未来变更权限的上限标志，**不是当前VMA可写的`VM_WRITE`，更不是PTE的`_PAGE_RW`**。矩阵中默认模板演算都明确假设`mm->def_flags`没有额外设置低位当前访问权限，且没被回调改写；否则仅保留符号表达式，不能固定声称用户请求独占决定最终权限（A135–A137）。

`file_s`的`f_mode`、`f_flags`是独立字段。`alloc_empty_file`按打开flags写`f_flags`，以`OPEN_FMODE(flags)`生成`f_mode`，公式含`((flags+1)&O_ACCMODE)`：选定普通输入下`O_RDONLY=0→FMODE_READ=1`、`O_WRONLY=1→FMODE_WRITE=2`、`O_RDWR=2→READ|WRITE=3`。 `IS_APPEND(inode)`检查的是inode的`S_APPEND`，不是文件`O_APPEND`（A073/A074/A108/A109/A113/A145–A149）。

文件分支：`file_mmap_ok`先看最大映射长度和pgoff；再根据`LEGACY_MAP_MASK`及`file->f_op->fop_flags & FOP_MMAP_SYNC`得到可接受flags。普通文件`MAP_SHARED`先`flags &= LEGACY_MAP_MASK`，再落入验证分支，可能静默剥去额外位；`MAP_SHARED_VALIDATE`则对不支持位返回`-EOPNOTSUPP`。共享写请求先要求`FMODE_WRITE`，又有`IS_APPEND(inode) && (f_mode & FMODE_WRITE)`这一拒绝门，**后者并不额外要求请求含`PROT_WRITE`**；共享路径再要求`FMODE_READ`，并设置`VM_SHARED|VM_MAYSHARE`，无写模式时清`VM_SHARED|VM_MAYWRITE`（A053–A064/A112/A113/A151–A155）。

文件`MAP_PRIVATE`只要求`FMODE_READ`；没有在其本分支以`PROT_WRITE`直接要求`FMODE_WRITE`。noexec文件若已有`VM_EXEC`返回`-EPERM`，否则清`VM_MAYEXEC`；无`file->f_op->mmap`返回`-ENODEV`，不可接受的GROW标志和MAP_TYPE各有`-EINVAL`（A061/A064–A067/A150）。这些是**必要检查链**，不是承诺invalid file/f_op等所有指针均安全，也不是完整的filesystem或mprotect策略认证。

无文件的`MAP_SHARED`把pgoff置0并标`VM_SHARED|VM_MAYSHARE`；无文件`MAP_PRIVATE`改用地址推导`pgoff=addr>>PAGE_SHIFT`。其它无文件类型按代码走default错误分支，其中`MAP_SHARED_VALIDATE`的MAP_TYPE数值3不等于该分支的MAP_SHARED值1（A034/A068–A071）。

## P03 · 新建VMA、回调变更及最终模板

`simple_mmap_region`入口**先**调用`simple_munmap_vma_range`处理原先重叠区间、再尝试`simple_vma_merge`。因此先前VMA副作用与后来“新建失败、文件回调失败”不能简单视作一个全回滚事务；这是第14批已有VMA生命周期问题的新增消费证据，并须合读R14-S01关于brk字段正确回滚的独立例外，不重开14（A075/A087–A089/A103–A106）。

如果`simple_vma_merge`返回已有VMA，就`goto out`，此路径**不会**再给该已有VMA执行新建分支的`vm_get_page_prot`赋值。合并有`VM_SPECIAL`、基本flags相等（除SOFTDIRTY）、文件身份、close回调及前后相邻条件；命中后调用`__simple_vma_adjust`改变边界（A076–A083/A088）。未证明全体VMA及地址空间都正确合并，亦不能把合并路径描述成新建VMA。

对于不能合并且正常分配成功的新VMA，代码依次写`vm_start/end`、`vm_flags`、`vm_page_prot=vm_get_page_prot(vm_flags)`、`vm_pgoff`。若有file，再`vma->vm_file=get_file(file)`并执行`call_mmap(file,vma)`。实际`call_mmap`是具名函数，内部通过`file->f_op->mmap`做**间接回调**；已核FAT32文件操作表`.mmap=generic_file_mmap`，后者在此代表场景把`vma->vm_ops`设为`generic_file_vm_ops`并返回0，没有在这个函数体中更新`vm_page_prot`（A090/A091/A107/A114–A117）。不同文件回调的行为**没有**在本批全树穷尽。

回调后，若VMA成为共享可写候选，还调用`mapping_map_writable`；会检查回调是否改变起始地址并在`vm_flags`有变化时尝试再次合并，最后局部`vm_flags=vma->vm_flags`。**没有活动的`vma->vm_page_prot=vm_get_page_prot(vma->vm_flags)`刷新**，末端有注释`vma_set_page_prot(vma)`（A092–A102/A144）。若未知回调确实改了读/写/执行/共享低位且已到正常返回，新VMA的flags和保护模板可能不一致；对已读generic_file_mmap不能借此宣称它自己造成了失配。

无file且`VM_SHARED`调用`shmem_zero_setup`；无file且非shared才`vma_set_anonymous`将vm_ops设NULL。新建分支调用`simple_vma_link`，正常结束OR入`VM_SOFTDIRTY`并返回addr；文件回调失败有fput/free/unmap错误标签，却未因此证明先前全部VMA动作已回滚（A098–A106）。**成功返回地址、VMA结构存在、vm_ops就绪、保护模板生成、PTE实际安装、硬件权限生效都是独立事实**。

### 默认模板的代表值（假定权限未被mm->def_flags或回调改变）

| mmap请求 / 情况 | 有效当前VM访问标志 | x86-64保护模板 | PRESENT / USER / RW / NX |
|---|---|---|---|
| 匿名private `PROT_NONE` | 无 | `PAGE_NONE` | 0 / 0 / 0 / 未置NX |
| 匿名private `PROT_READ` | READ+EXEC | `PAGE_READONLY_EXEC` | 1 / 1 / 0 / 0 |
| 匿名private `PROT_WRITE` | WRITE | `PAGE_COPY` | 1 / 1 / 0 / 1 |
| 匿名private `PROT_READ|WRITE` | READ+WRITE+EXEC | `PAGE_COPY_EXEC` | 1 / 1 / 0 / 0 |
| 匿名private `PROT_EXEC` | EXEC | `PAGE_READONLY_EXEC` | 1 / 1 / 0 / 0 |
| 匿名shared `PROT_WRITE` | WRITE+SHARED | `PAGE_SHARED` | 1 / 1 / 1 / 1 |
| 文件shared `PROT_READ|WRITE` 且fd可读写、非noexec | READ+WRITE+EXEC+SHARED | `PAGE_SHARED_EXEC` | 1 / 1 / 1 / 0 |
| 文件private `PROT_READ` 且noexec、fd可读 | READ | `PAGE_READONLY` | 1 / 1 / 0 / 1 |

上表来自`vm_get_page_prot`查`protection_map[vm_flags & (VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]`及本树`PAGE_NONE/SHARED/COPY/READONLY`位定义（A118–A129）；`PRESENT`模板还受`__supported_pte_mask`条件筛选，架构PKEY追加位取决于配置。掩码在固定源码的选定定义**初值**为`~0`，但本批不认定所有时刻无其他修改；表中位值因而是未改变掩码等前提下的模板展开，不是实际PTE或CPU观察（A123–A130）。

## P04 · 返回、问题入库和验收限界

`do_mmap`中的`-EINVAL/-ENOMEM/-EOVERFLOW/-EEXIST/-EACCES/-EPERM/-ENODEV/-EOPNOTSUPP`各有不同的生产前提；`simple_mmap_region`新建/合并、回调与清理还改变对象及引用，不能从最后一个返回值倒推所有先行副作用。已核`mmap_write_lock_killable`和`mmap_write_unlock`的局部空活动体不等于全局同步不存在；`fget`对范围内空fd槽潜在的非法访问也要求真实文件表生产者证据。矩阵详细区分32个输入/分支及对应VM位、PGPROT和错误（详见`facts-and-boundaries.yaml`）。

新发现和旧问题追加在`deferred-findings.yaml`中有ID、风险前提、反证和待核条件。本批不采取修改源码、补丁、回滚执行、构建运行或硬件验证。对第19批的衔接**仅**是“生成的`vma->vm_page_prot`可能被缺PTE/COW消费”的限定接口；其零页、写页与引用正确性不在本批重算。最终状态由C03独立限定验收，执行者正式提交与回读后立即停写。
