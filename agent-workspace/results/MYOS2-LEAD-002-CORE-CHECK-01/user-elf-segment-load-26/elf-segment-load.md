---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-SEGMENT-LOAD-26
record_type: bounded_elf_ptload_mapping_zero_tail_analysis
produced_by: "ChatGPT普通聊天；Owner选择GPT-6＋Extra"
date: 2026-11-02
base_snapshot: "time @ a039d9803ade（短提交号）"
result_base_short12: 897c3a935aa8
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# ELF PT_LOAD文件映射、清尾与匿名续接：固定源码静态分析

## E00｜固定对象、历史限定与范围

本批对 `mykernel/fs/vfs/binfmt_elf.c` 的所选主程序 `PT_LOAD` 循环及其 `elf_load → elf_map → vm_mmap`、`padzero → clear_user`、`vm_brk_flags → do_brk_flags` 有界展开。分析目标是**文件字节/虚拟地址范围如何换算、程序检查的真实先后、错误值怎样消费、先行作用是否回滚**，而不是宣布真实ELF成功加载。固定源码 `time @ a039d9803ade`，任务指令46号固定 `e5ea70e40bd4`，结果冻结前头 `897c3a935aa8`。

已读取25主线R25-S01/S02/S03及05、07相关回执，并定点合读13、14、19、20、21的限定：文件页缓存对象/数据生产不能从VMA存在推出；unmap实际摘链与页表/物理回收是不同阶段；用户页与VMA迭代的初始NULL问题未消除；所选映射权限不直接认证CPU PTE；fixed/NOREPLACE搜索及分段释放不能被当作绝对成功保证。25的局部vma未初始化、23/24参数页/GUP问题仍然保留，不阻止单独对**已获得有效ELF头、有效VMA对象和返回约定**做条件化静态算术。没有重做旧批，也没有用旧报告代替本轮固定源码引文。

本文件的关键断言引用同批 `evidence.md` 中 `EL26-A001` 等编号；所有直接源码标签、path::symbol与1–5行原文均保存在该件。报告正文中的条件推导不升级为源代码事实或实际运行轨迹。

## E01｜主程序PT_LOAD实参与先后顺序

**真实入口与过滤。** `load_elf_binary`先检查ELF magic、e_type为`ET_EXEC/ET_DYN`、架构条件与文件`f_op->mmap`，然后用`load_elf_phdrs`取得程序头；后者在读取前核e_phentsize、程序头数量造成的总大小，绝不等于全部ELF结构/映射输入已通过。中途先发生`begin_new_exec`与`setup_arg_pages`，然后遍历头表，只处理`p_type==PT_LOAD`的段；不针对`PT_DYNAMIC`或解释器重新审计。源锚点：EL26-A001–A005、A008、A013、A017。

| 实参/字段 | 本批所选来源 | 类型与阶段 |
|---|---|---|
| `p_vaddr`, `p_offset` | 当前`elf_ppnt`的ELF64字段 | `Elf64_Addr/Elf64_Off`即__u64；虚拟地址/文件字节偏移 |
| `p_filesz`, `p_memsz` | 当前程序头 | `Elf64_Xword`，字节长度；不等于物理页面数 |
| `p_flags` | 当前程序头 | PF_R/PF_W/PF_X按位，经make_prot到prot，可能再经arch_elf_adjust_prot |
| `vaddr` | `elf_ppnt->p_vaddr` | 先为相对主程序的虚拟地址，再用于`load_bias + vaddr` |
| `elf_flags` / `type` | 起始`MAP_PRIVATE` | 后续段加MAP_FIXED，ET_EXEC首段MAP_FIXED_NOREPLACE，ET_DYN首段按解释器/对齐策略调整 |
| `total_size` | 每一PT_LOAD循环开始为0 | ET_DYN首段用`total_mapping_size`的所有PT_LOAD跨度作整体预留；后续段仍0 |
| `elf_prot` / `prot` | `make_prot(p_flags, &arch_state, !!interpreter, false)` | PF_*到PROT_*的请求，不自动等于最终VMA或CPU权限 |
| `load_bias` / `addr` | ET_EXEC默认0；ET_DYN首段先依据解释器存在、ELF_ET_DYN_BASE及对齐决定 | `elf_load(bprm->file, load_bias + vaddr, ...)`传入字节地址，返回`ulong` |

`total_mapping_size`在程序头数组中取各PT_LOAD的最小页下取整地址与最大`p_vaddr+p_memsz`尾界，若存在PT_LOAD则取两者差，否则返回0。对零跨度有`-EINVAL`门；但该计算含64位加减且未做每项无溢出的证明，故“整个跨度一定大于每段文件长度”只能在合法`memsz>=filesz`、地址排序与无回绕等前提下成立。权限`make_prot`仅建立请求位，`do_mmap`还会按其本地配置修改；此处不替代20号权限评审。源锚点：EL26-A039–A044、A070、A077–A082。

**关键顺序：** 第一次ET_DYN调用前可能计算total_size和alignment；无解释器且alignment大于一页的分支，**先试调用一次**`elf_load(..., addr=0, total_size)`获取候选load_bias，成功后再调用`vm_munmap(load_bias,total_size)`，其返回未被消费；随后用对齐后load_bias进行主`elf_load`调用。该探测不是“没有副作用的纯查询”，也不是每种ET_DYN都执行的逻辑。普通ET_EXEC、ET_DYN有解释器、ET_DYN无解释器且小对齐的分支需分开消费（EL26-A006–A014）。

真实主程序调用后先判断`BAD_ADDR(error)`，对编码负错误以`IS_ERR_VALUE`配合`PTR_ERR`转为`retval`，其他不在任务地址内的值给`-EINVAL`。只有未触发该门后才更新`first_pt_load`和某些`load_bias`数据，然后对程序头计算后置`BAD_ADDR(k)`、`p_filesz>p_memsz`、`p_memsz>TASK_SIZE`、`TASK_SIZE-p_memsz<k`检查。**后置文件大小/地址门不能被前移成之前的`elf_load`已遵守的前提**。`phdr_addr`只在当前PT_LOAD文件范围包含`e_phoff`起点时写入；并未在此验证整个程序头表均位于映射中（合读R25-S02）。成功段随后的`start/end_code/data`、`elf_brk`更新同样属于后续阶段，不能据它们推定前一步映射已经清零或被裁剪。EL26-A014–A019、A085–A086。

## E02｜elf_map：字节区间、预留、裁剪与返回

设 `P=ELF_MIN_ALIGN=PAGE_SIZE=4096`、`d=p_vaddr&(P-1)`（字节数）且没有无符号回绕。代码先求`size_raw=p_filesz+d`（字节）、`off=p_offset-d`（文件字节偏移）、`addr_aligned=addr&~(P-1)`，再以`size=PAGE_ALIGN(size_raw)`交给映射接口（EL26-A020–A026、A081）。

正常文件区间的条件模型为：
`file[off, off+size)` → `user[map_addr, map_addr+size)`，其中页对齐的前缀可能覆盖文件中位于实际`p_offset`之前的字节，实际ELF段字节从`map_addr+d`起；该区间是**期望的VMA/文件映射规格**，不是已经把文件内容搬入物理页的证明。若`p_offset<d`，`off`在所选无符号类型中下溢，实际`vm_mmap`在活动路径包含偏移溢出与`offset_in_page`门，不能假设其自动纠正文件偏移。宏的`+P-1`和其他算术也须无回绕。没有独立验证整个ELF输入已满足页偏移同余。

`if (!size) return addr`发生在任何`vm_mmap`前。对`p_filesz=0,d=0`的**直接elf_map输入**确实成立；若`p_filesz=0,d!=0`则size仍可能为一页，不成立。但实际`elf_load`在`p_filesz=0`时**完全不调用elf_map**，改走匿名分支，故这两个直接elf_map条件不是所选主调用的真实零文件生产路线。正文件长度但算术回绕使size=0的情形也仅是需另证输入的帮助函数模型。EL26-A025、A026、A032、A035。

当`total_size==0`，`elf_map`调用`vm_mmap(filep,addr,size,prot,type,off)`并直接保存其返回；当`total_size!=0`，先将total_size页向上取整，再调用`vm_mmap(filep,addr,total_size,prot,type,off)`，且只在`!BAD_ADDR(map_addr)`时**发起**`vm_munmap(map_addr+size,total_size-size)`。后者返回int但本调用者未读取/判断，最后仍返回原`map_addr`。因此需要明确三种状态：整体映射调用了、解除剩余段调用了、解除成功/页表与引用释放了，彼此不等价。若`total_size<size`，减法按ulong回绕可能生成巨大长度；当前主程序的合法单段/总跨度可以提供`total_size>=size`的条件证明，但后置`p_filesz<=p_memsz`检查尚未执行，异常直接输入不得自行认定该前提。EL26-A027、A028、A039–A041、A059–A062。

`vm_mmap`的真实直接桥接是`offset`字节检查→`vm_mmap_pgoff(...,offset>>PAGE_SHIFT)`→`do_mmap`→`simple_mmap_region`。固定`mmap_write_lock_killable`实际返回0，`mmap_write_unlock`无活动解锁，故本层错误不能被称为已证锁竞争-EINTR。所选`do_mmap`会拒绝某些长度、偏移、地址、文件读权限及缺失的mmap方法，并可能先处理旧VMA、合并或创建文件VMA；内层`simple_vma_link`的返回在已读路径未消费。即使`vm_mmap`返回满足地址门，也不能等同文件字节已经加载、裁剪完成、PTE有效或并发安全。13号文件页缓存的`generic_file_mmap`安装`vm_ops`，页缺失时再调用read且有其独立错误/数据时序问题。只复用该限定，不重做页缓存审计。EL26-A045–A058、A083–A084。

当`type`包含`MAP_FIXED_NOREPLACE`且`PTR_ERR(map_addr)==-EEXIST`时，`elf_map`调用`pr_info`打印信息，**没有改变map_addr，也没有重试/解除其他映射**。编码负值由实际`BAD_ADDR`及`IS_ERR_VALUE`关系消费，不将`-EEXIST`日志误判为成功返回。MAP_FIXED和NOREPLACE的真实地址冲突限制仍沿用R21-S01/S02。EL26-A029–A030、A022、A075–A076。

### 纸面字节账（仅合法输入和无回绕，非运行）
设`p_vaddr=0x401234`、`p_offset=0x1234`、`p_filesz=0x1800`（6144字节）、`p_memsz=0x3000`（12288字节）、`addr=0x401234`；则`d=0x234`（564），`addr_aligned=0x401000`，`off=0x1000`，`size_raw=0x1a34`，`size=0x2000`。条件返回`map_addr=0x401000`时，原文件数据对应的目标区间起于`0x401234`，结束于`0x402a34`。文件VMA预留到`0x403000`，其中末页部分尾区的单独清零由E03负责。以上地址计算没有证明实际文件字节已读入页面。

### 条件下溢算例（与合法生产区分）
设直接给`elf_map`的`total_size=0x1000`，而对齐后的`size=0x3000`；则`total_size-size`在64位ulong纸面模型中为`0xffffffffffffe000`。对于首次ET_DYN的某种违反`p_filesz<=p_memsz`的头部组合，总跨度可能只有一页而文件映射需三页；**实际主程序确在映射后才检查这个比例**，但从这个算例不得推出`vm_mmap`必成功、`vm_munmap`必执行完成或物理内存如何变化。故列为条件风险而不是漏洞复现。EL26-A017、A025、A027、A040–A041。

## E03｜elf_load：有/无文件字节的变量生产、清尾和匿名扩展

### 分支对象的赋值与使用
`elf_load`先声明`ulong zero_start,zero_end,map_addr`，不初始化。其后**只在`p_filesz!=0`时**调用`elf_map`，若`BAD_ADDR`则立刻把其返回值原样作为ulong返回；否则当`p_memsz>p_filesz`时，才给`zero_start/zero_end`赋`map_addr+p_filesz+d`与`map_addr+p_memsz+d`。在`p_filesz=0`分支则明确给`map_addr=zero_start=ELF_PAGESTART(addr)`和`zero_end=zero_start+d+p_memsz`。末尾仅当`p_memsz>p_filesz`使用zero_start/zero_end；因此`p_filesz!=0且p_memsz<=p_filesz`分支中的尚未赋值变量**并没有活动读取**，不能误报“声明未初始化即UB”。关键区别是之前的25号局部vma确实在赋值缺失处被读，而本函数这两个变量的读取有门。EL26-A031–A038。

当`p_filesz>0,p_memsz=p_filesz`：可进入文件映射而不清尾/不匿名扩展；当`p_memsz<p_filesz`：映射仍可能先调用，后置头检查才报-EINVAL；当`p_memsz>p_filesz`：文件映射成功条件下，先尝试尾页清零，再请求整页匿名扩展。对于`p_filesz=0,p_memsz>0`：跳过文件映射和padzero，直接将包含起始非页对齐字节的**整页向上取整范围**交给`vm_brk_flags`；不是跳过起始页。若两者都为0则未必执行任何映射/扩展，局部返回页下取整addr，但不能由此证明全局没有相邻映射。所有p_filesz/p_memsz地址和对齐运算均保留有效对象、无溢出与实际映射生产者前提。

### padzero并不等于全部BSS已清零
`padzero(elf_bss)`以`nbyte=ELF_PAGEOFFSET(elf_bss)`取得文件内容末端在页内的偏移；仅当nbyte非0时，算`P-nbyte`并调用`clear_user((void __user*)elf_bss,nbyte)`，若其返回非0则本函数`-EFAULT`，否则0。没有尾部字节需求（地址刚好页对齐）时直接返回0，无清零调用。此处`elf_bss=map_addr+p_filesz+d`的页尾与匿名整页区间应分开；清尾的尝试范围为`[zero_start, ceil_P(zero_start))`。EL26-A020、A023–A024、A033–A034。

**条件的求值顺序：** `if (padzero(zero_start) && (prot & PROT_WRITE)) return -EFAULT;`中，`padzero`先被调用；只有其返回非零时才求`prot&PROT_WRITE`。所以“不可写就不尝试清零”是错误读法。若padzero非零且`prot`包含WRITE，则`elf_load`此处返回`-EFAULT`（强制转换成ulong编码），不会在此路径调用后面的vm_brk_flags；若不可写，**忽略这个错误并继续**，不能据其返回成功认为未清部分已为零。对于页对齐的zero_start，padzero原体直接返回0，继续匿名门，实际未触发clear_user。EL26-A034、A079、A082。

`clear_user`先做`__access_ok(to,n)`，通过才调用`__clear_user`，否则返回n。实际`__clear_user`含stac、ALTERNATIVE/rep stos与异常表宏，返回剩余长度`size`。这些源码只建立“尝试清理与返回值消费”的接口边界，不能自动签发CPU权限、实际页面已存在、异常表正确装配或所有故障都可靠转换为剩余字节。13/19/20对缓存页、匿名页零化与PTE权限的历史限制继续有效。EL26-A071–A074。

### 匿名扩展、实际vm_brk_flags消费者和局部副作用
当`p_memsz>p_filesz`时，`elf_load`将zero_start、zero_end**分别向上页对齐**，向`vm_brk_flags(zero_start,zero_end-zero_start, prot&PROT_EXEC?VM_EXEC:0)`传入**字节长度**和一个VMA标志位，不传PTE值、不按物理页数调用。对前述纸面输入，`zero_start=0x402a34`、`zero_end=0x404234`；尾页清零长度`0x5cc=1484`字节，匿名扩展区间为`[0x403000,0x405000)`，请求长`0x2000`字节，末端比程序头内存尾多出对齐余量。无映射/字节内容实测。EL26-A033、A037–A038、A063–A064。

`vm_brk_flags`先`PAGE_ALIGN(request)`，若结果比原值小报-ENOMEM，若为0则直接返回0；非零才调用`do_brk_flags`。例如`p_filesz>0`且`memsz>filesz`但新增字节全在已映射尾页内，`zero_start/zero_end`上取整可相等，此时`vm_brk_flags(...,0,...)`直接返回0，**没有发生匿名VMA创建**；此前padzero仍受独立清零契约约束。对`p_filesz=0,p_memsz>0`，zero_start已页下对齐，再上对齐不移除起始页，zero_end向上对齐后将包括这页，不能误称“丢失第一个部分页”。EL26-A035、A037、A063。

`do_brk_flags`仅允许来参VM_EXEC之外无其他位，随后OR`VM_DATA_DEFAULT_FLAGS|VM_ACCOUNT|mm.def_flags`；本批已读的默认`VM_DATA_FLAGS_EXEC`包含VM_EXEC，即使`prot&PROT_EXEC`是假，仍不能根据那个三元条件断言最终新VMA必不带执行位。之后它可能先处理重叠旧VMA、合并既有匿名VMA或分配/填写新VMA并调用`simple_vma_link`；后者的返回在当前函数中被丢弃。最后有`mm.total_vm/data_vm`等计数更新及return0。VMA合并/新建、PTE/物理页存在、数据零填充和硬件权限互不等价。原13/14/19/20/21限制仍适用，不能“因函数名叫brk”补造零页面。EL26-A065–A070。

`vm_brk_flags`非零错误由`elf_load`写入`map_addr`（int负值转ulong后仍要由外层BAD_ADDR/IS_ERR_VALUE判断），之后原样返回；**此前成功的文件映射和可写性允许的部分清零不会被该函数自动撤销**。但没有完整全局cleanup审计，不能因此宣告永久泄漏或永不回收。下游`load_elf_binary`的错误分流主要释放程序头/解释器对象，不等于本层对旧VMA/PTE的实际回滚，且受14号已核生命周期断点限制。EL26-A017、A019、A031–A038、A064、A075–A076。

### 返回与前缀作用矩阵
| 局部条件 | 先行操作/依赖 | 本层返回、消费及确定性限制 |
|---|---|---|
| `elf_map`的对齐size==0 | 已计算size、off、addr | 直接返回addr，未调用vm_mmap；须区分实际p_filesz=0绕过elf_map |
| `vm_mmap`返回编码错误 | 可能有下层错误前缀 | elf_load的BAD_ADDR先退出；外层按IS_ERR_VALUE或-EINVAL消费 |
| `total_size>0`且整体映射返回可用地址 | vm_mmap先发生 | 无论裁剪是否成功，elf_map未检查vm_munmap返回，仍返回原地址 |
| `MAP_FIXED_NOREPLACE`且返回-EEXIST | 下层拒绝后可能记录pr_info | 原错误地址不变，不能视为冲突自动解决 |
| `p_filesz>0,p_memsz=p_filesz` | 有文件映射门 | 不读取zero_start/zero_end，不清尾或匿名续接 |
| `p_filesz>0,p_memsz<p_filesz` | 可能先文件映射 | 本层可返回映射地址；外层后置长度门报-EINVAL，没有本层统一撤销 |
| `p_filesz>0,p_memsz>p_filesz`且尾已页对齐 | 文件映射已作为前缀 | padzero返回0无clear_user；匿名扩展可能是0字节（无需新VMA） |
| `padzero`清理非零且prot可写 | 文件映射前缀可能已有 | elf_load直接-EFAULT；后继vm_brk_flags不调用 |
| `padzero`清理非零但prot不可写 | 已尝试clear_user，未保证全清 | 本层忽略该错误并继续扩展；不得称尾部已经全零 |
| `p_filesz=0,p_memsz>0` | elf_map和padzero均未调用 | 向匿名扩展传入页下取整起点及向上取整尾界；成功仍只认证接口返回 |
| `vm_brk_flags`请求长度0 | 先有前缀（若是文件段） | 包装直接返回0，没有do_brk_flags或新VMA |
| `vm_brk_flags`返回非0 | 文件映射和尾部局部写入可能存在 | int错误覆盖ulong map_addr，上交给外层；先行效果不假定回滚 |
| 主程序`elf_load`成功但后置程序头校验失败 | 可能已建文件VMA、裁剪/匿名扩展 | 外层retval=-EINVAL，goto清理对象；不是文件VMA/PTE已撤销证明 |
| 所有局部门返回正常 | 只可认定各源码分支/接口完成条件 | 无实际磁盘内容、物理页、内存零值、CPU权限、用户启动认证 |

注意：`vm_munmap`调用发生与其返回/摘链完成、`simple_vma_link`调用与新VMA有效发布、`clear_user`返回信号与实际CPU恢复、`vm_brk_flags`成功与物理页面已准备，分别是不同断言。这是本批图/结构化事实的主要分层。

## E04｜发现登记、验收与停止线

本批将`EL26-F01..F05`作为**条件性待核记录**：包括整体裁剪返回未消费、后置头检查与先行映射、不可写尾页错误忽略、下溢边界以及匿名VMA link返回未消费。它们既不等于5次实测失败，也不宣称已完成对14/20/21等全部历史问题去重；相关历史全限定ID在deferred中保留。尤其`p_filesz=0`路径确实从页下取整起点扩展，**不**登记虚构的“首个部分页必然丢失”。

本批仅阶段2静态；没有预处理、编译、运行C/ASM、内核/QEMU、真实磁盘/CPU或外部libc验证，没有修补内核。输出的布局、数值、风险和条件分支要由主线C03独立审查。状态`READY_FOR_MAINLINE_REVIEW`、`mainline_acceptance=PENDING`；主线在其可读/本地边界内最高可考虑`PASS_PENDING_LOCAL`，执行方不代签。正式交付后停写，不自行选择第27批。
