---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-FAT-SUPERBLOCK-BINDING-38
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-04
date: 2026-11-04
read_channel: connector
base_snapshot: time
source_pin_short12: a039d9803ade
result_base_short12: bcb7ecb66a06
contract62_short12: 928d1d521e4e
mandatory_supplements: [R36C-S01, R37-S01, R37-S02]
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
---

# 第38批：FAT注册、超级块操作表及根/子inode来源

## B00｜固定输入与安全边界

已完整读62号、同固定提交928d1d521e4e的36文档闭合与37验收回执，以及发布恢复检查点。R36-01/R36-02已由C04作CLOSED_DOCUMENTATION_ONLY，36组合材料与37均为PASS_PENDING_LOCAL；本执行者只消费裁定，不重做验收、不修改历史RETURN或旧文件。R36C-S01的图解释条件及R37-S01/S02的路径绑定、NULL正常失败、定义与调用区别均继续适用。旧37字段矩阵和分配前提、34的CB34-F05/R34限定定点合读；13回执只用于防止扩大为完整页缓存审计。

初次现场：PR17为原claude/dazzling-cori-q0dnyt，HEAD bcb7ecb66a06，open/Draft/unmerged；PR16/lead为928d1d521e4e，157个变更路径全在主线区，与本目录无交集。开放PR检索只有16、17；目标user-fat-superblock-binding-38不存在；本人37交付评论6632057217已回读。direct branches核time a039d9803ade、master e8e60cd19540，配置及panic金丝雀与master不同。PR接口的base_sha不是本批源码来源；只认固定time及直接分支身份。任何真正的结果头移动或同名成果出现，都须暂停对应写动作核归属。

沿用原C04a/CHAT-EXEC-04及已经完成的MyOS2/Mnemosyne接收。新读与继承输入在MANIFEST分栏，不虚称本轮刷新全部guard。只许新六件与本人评论；不编译运行，不审完整挂载、设备驱动、分配器、权限或生命周期。下文所有路径都以对象可访问、整数/尺寸合法、所需被调函数正常返回为条件；源码未见赋值只限完整所选体，不推出全仓绝无其它写者。

## B01｜注册、名称匹配与两种接口

为避免混淆，本文用A指FAT32_fs_type/read_fat32_superblock/FAT32_sb_ops，用B指vfat_fs_type/vfat_fill_super/fat_fill_super/fat_sops。

A的已读register_diskfs活动体只调用init_fat32_fs；init_vfat_fs那行是注释。init_fat32_fs调用register_filesystem(&FAT32_fs_type)，其对象名称为"FAT32"、read_super=read_fat32_superblock、next=NULL。实际注册链头是filesystems.c的file_systems，不是myos_vfs.c另一个叫filesystem的对象。register_filesystem先检查next及重名条件，成功才把对象链接进去；init_fat32_fs不消费其整数返回。因此这是**注册尝试和静态分派来源**，不是已发生注册成功或实机启动证明（A002、A009–A015）。

myos_switch_to_root_disk在先前分配/读取、MBR类型门及分区遍历都有效时，匹配EFIBOOT_PART_GUID_LOW/HIGH，再以i==BOOT_FS_IDX调用mount_fs("FAT32",gpt_pe,fat32_sb)。当前宏BOOT_FS_IDX=0，不能把它解释成任意匹配分区均被选为根；EXT4分支未在所读体给myos_root_sb赋值。磁盘内容与宏实际I/O保持不透明（A001/A003/A004）。本批不追全启动调用图，也不认证这些函数实际执行的先后。

mount_fs遍历file_systems，strcmp名字相等后**调用p->read_super(DPTE,buf)**，不是调用p->mount；无匹配返回0即NULL。只有注册链实际包含所选A节点、名字相同且函数指针有效时，才能把这次间接调用定到read_fat32_superblock。已注册同名对象、注册失败或名字未命中分别保留条件（A007/A008）。循环结束后的set_init_mount/set_init_taskfs没有本地根指针有效性门；此前没有得到有效根时，不能越过myos_root_sb->s_root访问继续声明挂载完成（A005/A006）。这不是在本轮观测到启动崩溃。

B的vfat_fs_type名称是"vfat"，绑定的是.mount=vfat_mount，另有init_vfat_fs定义；该注册函数不在当前register_diskfs活动调用中。vfat_mount把vfat_fill_super交给mount_bdev，vfat_fill_super调用fat_fill_super(sb,data,1,setup)。mount_bdev内部是否及何时调用回调不在本批；表存在、回调被传入不等于成功挂载（A016–A020）。fs_type_s真实结构将read_super与mount分为不同成员，返回类型分别为super_block_s*与dentry_s*（A021/A022）。在另行注册vfat且手工调用mount_fs("vfat")的helper-only模型中，本分派器仍试图调用其初始化列表中未指定的read_super槽；按所选C聚合初始化规则该槽为NULL，不会自动改调.mount。这不是启动代码中的实际调用。

## B02｜s_op、两类私有字段与分配门

### A超级块

alloc_super用kzalloc(sizeof(super_block_s))申请对象，分配NULL时真实返回ERR_PTR(-ENOMEM)，不是注释所说NULL。普通成功路径初始化s_inodes、flags/count/maxbytes及默认操作表后返回对象（A029–A032）。其调用者read_fat32_superblock却只测sbp==NULL，然后写sbp->s_op=&FAT32_sb_ops。若alloc_super按已读失败分支返回编码错误指针，该NULL门为假，最早不能获得有效性保证的是随后的s_op存储；**不得声称这个实际错误已被安全传回mount_fs**。NULL分支源码确实存在，但当前alloc_super定义良好完成出口没有正常NULL生产者（A030/A033/A034，SB38-F01）。

完整FAT32_sb_ops是文件作用域对象，成员初始化只列write_super、put_super、write_inode，没有指定alloc_inode。结合真实super_ops_s字段定义与所选C聚合初始化模型，未指定的函数指针成员为NULL；在没有后续写者改变该表的条件下，sb->s_op->alloc_inode可以判定为NULL。**这与堆对象某字段没有显式写入、初值未知不是同一种情况**（A023/A025）。不把表名"FAT32"补成Linux习惯的分配回调。

read_fat32_superblock把private_sb_info另行分配为FAT32_SBinfo_s，随后立即通过fsbi写分区及BPB派生字段；这个分配没有本地NULL门。其后FSInfo请求、缓冲分配和memcpy都需有效前提，不能直接跳到根构造。所选完整体未给s_fs_info建立msdos_sb_info_s来源；只有在kzalloc底层清零语义等成立时才可讨论其初始零值，而不是由名字推断它含有另一类型实例（A034/A035）。

### B超级块

fat_fill_super先申请msdos_sb_info_s，NULL明确返回整数-ENOMEM；成功写sb->s_fs_info=sbi，再写s_op=&fat_sops，然后调用setup。setup通过MSDOS_SB(sb)->dir_ops写表，而MSDOS_SB真实定义只返回s_fs_info。fat_sops完整表明确绑定alloc_inode=fat_alloc_inode（A020/A024/A028/A042/A043）。private_sb_info与s_fs_info在super_block_s中是**独立字段**；前者的FAT32_SBinfo_s与后者的msdos_sb_info_s不是同一结构，不能自动交换、等同或断言任意运行时两个值必不相等（A026/A027）。该fat_fill_super体没有为private_sb_info建立FAT32_SBinfo_s实例，入口前已有值仍需另证。

B也不是本轮证明成功的挂载替代方案：完整所选fat_fill_super尾部的d_make_root、s_root发布及return0均为注释。即便前面的new_inode和fat_read_root正常非负完成，活动控制会落入out_invalid，将error设-EINVAL，随后清空s_fs_info、kfree(sbi)、返回error。root分配NULL或其它检查失败则走各自原错误出口，不能一律称-EINVAL。这个局部尾部事实用于限制B的完成等级，不递归mount_bdev、盘读、释放或挂载全链（A044–A047，SB38-F05）。

## B03｜根和子inode不能套同一分配模型

### A根：直接分配而非new_inode

read_fat32_superblock申请根dentry，初始化child/subdirs、d_sb、d_parent、d_op，另申请2字节名字写入"/"及NUL。之后直接kmalloc(sizeof(inode_s))作为根d_inode，写i_op=&vfat_dir_inode_operations、i_fop=&FAT32_file_ops、i_size=0、i_mode=S_IFDIR及i_sb=sbp。i_blocks还依赖簇/扇区数值运算和非零除数；**i_size=0不能直接推出这一表达式的i_blocks=0**。该根构造没有调用new_inode、inode_init_always或inode_init_once，完整活动体也没有给根i_mapping/i_data作直接初始化（A036–A039）。这只形成根mapping来源缺口，不证明具体未知位模式、实际页缓存故障或整个内核缺少初始化（SB38-F03；13回执继续划界）。

根private_idx_info又是独立kmalloc(sizeof(FAT32_inode_info_s))的缓冲，赋root cluster和选定位置/日期字段。它不把根inode变成外层msdos_inode_info_s的内嵌成员。每次分配都要求实际成功；任一更早字段或读请求先失去有效性时，不补出return sbp（A040/A041）。根i_mapping缺来源也不等于FAT32_lookup入口必先读取它；该入口直接需要的是父i_sb和其private_sb_info等，后继具体消费者需分别证明。

### A子：备用inode缓存与MSDOS_I的包含对象缺口

根的i_op表把lookup绑定FAT32_lookup。只要该父对象、查找的前置迭代/解析和实际调用都成立，FAT32_lookup以**parent_inode->i_sb**传给fat_build_inode，后者调用new_inode，再由alloc_inode读取这同一个sb->s_op（A048–A053）。A表未被改变时，alloc_inode成员为NULL，因而走备用kmem_cache_alloc(inode_cachep,GFP_KERNEL)。所选inode_init的缓存对象大小为sizeof(inode_s)，最后构造器参数NULL；不能无证据补成fat_alloc_inode分配的外层对象（A055）。

备用分配NULL是正常失败：alloc_inode/new_inode返回NULL，fat_build_inode产生ERR_PTR(-ENOMEM)，不会进入fat_fill_inode；本图不把它画成异常停机。普通有效指针成功时，inode_init_always在alloc_inode内部执行，明确写i_size=0、i_mapping=&inode->i_data，随后new_inode进行列表初始化/发布并返回（A054/A056–A060）。这些字段默认值可以条件性保留，不能认证其它mapping成员、锁或一次性初始化。

关键是其下一消费者：fat_fill_inode首先写i_mode=0，紧接着执行**MSDOS_I(inode)->i_pos=0**，然后才读取de属性。MSDOS_I通过container_of逆推msdos_inode_info_s，而真实该外层结构包含vfs_inode（A061–A063）。A的备用分配只建立独立inode_s对象来源，**没有建立这个外层对象存在的前提**；地址计算结果不是有效包含对象的证据。因此本批的可证前缀最多到这里的包含关系要求，不能跨过它宣称已完整填充并返回A子inode、已绑定dentry或已成功打开文件（SB38-F02）。额外allocator行为和实机内存后果未审。

### B的对照对象

若另一个实际sb采用fat_sops，fat_alloc_inode才会kmalloc(sizeof(msdos_inode_info_s))，NULL正常返回；成功只写五个外层字段并返回&ei->vfs_inode，真实包含关系来源成立（A064–A066）。B中root_inode=new_inode(sb)也使用这个回调。它只说明条件构造来源，既不能替代A表的NULL槽，也不能抹去B尾部返回错误/未发布根的事实。inode->private_idx_info仍是另一个字段，不等于ei。第37批有关有效fat_sops路径的字段结论保留其原条件；CB34-F05和NI37候选只新增路径区分，不自动结案。

## B04｜结果、证据和停止条件

facts以7条路线、14行字段来源、6类返回契约、12个有限条件案例记录上述链；12案例不是实际运行重放。本轮另用8个明确的纯Python有限分派/NULL/状态模型作一致性检查，不执行仓库C。新SB38-F01至F06为有前提、反证和恢复条件的延期候选，不等于六项已复现漏洞或六个独立根因。

evidence有66段、10份源文件的直接引文，每段一个真实定义内1–5行，没有重复摘录冒计独立事实，也没有把声明、EXPORT或条件编译门当定义。程序检查的对象是连接器文本所形成的本地规范化副本，不宣称重新下载整个仓库并对全部原字节认证；精确方法与未执行项见MANIFEST。DOT的结构/端点与关键控制顺序分开检查。它只表示选定路径，数据/静态绑定/额外有效性前提用虚线，不是额外源码检查或CPU停止指令。

六件仅写user-fat-superblock-binding-38/。不修改内核/用户程序/旧成果/主线/PR元数据，不出补丁、不开新分支或PR；不执行预处理、编译、QEMU/硬件、真实磁盘、完整挂载/驱动/allocator/权限/引用回收或额外付费模型API。完整本地P9和runtime均NOT_RUN。新提交须用expected-head和非强推，比较只能六件A新增，固定提交与活动分支六件全文/blob均回读；本人评论仅申请READY_FOR_MAINLINE_REVIEW、mainline_acceptance=PENDING。正式交付后STOP_WRITING，交C04独立验收，不开始39。
