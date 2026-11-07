---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FAT-ALLOCATION-GEOMETRY-56
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "Owner-selected GPT-6+Extra does not independently attest backend author"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
date: 2026-11-07
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: G01_G02_G03_combined_analysis
---
# 第56批簇链与容量／单位有限静态分析
## G00 主线前提
time@a039d9803ade；53/54/55均PASS_PENDING_LOCAL，55无返工项。R53C-S01已撤回的CO53-A03/B03空行误判不重述为事实。R55-S01/S02限定55错误与结果消费的边界，本批只新增簇号状态、expand循环与几何单位；R35/R38保留旧路径来源和ERR_PTR边界。下列全部是有效对象、整数可定义、callee正常返回且按明示数据假设的静态条件序列。
## G01 A 簇链推进
读路径按position/bytes_per_cluster先通过FAT getter跳簇，之后每一轮按Data_firstsector+(cluster-2)*sector_per_cluster求sector，只有循环末index还非零才直接`cluster=FAT32_read_FAT_Entry`。示例BPS512/SPC8/DataFirst1000/FAT[2]=3/count8192的合法二簇读，block sector依次1000和1008。do尾部只判断新cluster真值，next=0时可提前退出而留旧ret_val，next为非零EOF或坏值却可能继续用它计算sector；这属于无源码合法性门的STOP，不能推断实际访问坏盘。
写路径初始化current cluster、next_cluster、sector0、flags0。已有簇时flags0才计算sector，写成功后若index还正只将getter结果放入next_cluster。`if(next_cluster>=0x0ffffff8)`为假且next是普通有效簇3时，**没有赋`cluster=next_cluster`**：二簇2→3的两轮写请求可都带sector1000（AG56-F01），与旧BI55-F02的flags1 stale-sector不同。EOF门为真则finder候选、新FAT旧尾链接/新簇EOF请求、`cluster=next_cluster`、`flags=1`，但下轮flags1跳过重算sector，继续使用旧sector。初始无簇时找到候选后会先写finode.first_cluster并请求write_inode和FAT标记，第一次数据WRITE则使用sector初值0，属于旧BI55-F02，不重复建新缺陷。候选0返回-ENOSPC（以正常清理为前提），候选1及BAD/0/1普通next均不受完整合法簇检查；先行无效即STOP。以上“找到候选／发布首簇／请求标记／提交数据”是不同事件，不推断真实介质已完成保留。
## G02 B iobuf扩容和回滚
`FAT32_iobuf_init`求max_nr=64MiB/bufsize，count>0才分配buffers/clusters/flags三个数组；count0可形成空数组来源，且get_cluster_chain的ERR_PTR不受仅NULL门保护是R35旧问题。链节点被依次追加到tail，却从tail移除填入clusters[i]，因此有效[2,3]示例生成数组[3,2]，clusters[0]不自动就是起始簇。expand参数`size_t nr`，本地`int tmp_nr=nr_clus`，循环条件`nr>=0`对无符号值恒真；从nr1减到0也会再次调用finder，若再成功分配则0--回绕SIZE_MAX。expand没有数组非NULL、tmp_nr<max_nr_clus或先预留候选的门，稳定FAT数据与无其它写者时可反复返回同一簇号。
在有效数组/容量下，nr1、finder[5,0]追加1项后第二次探测0，可提交nr_clus2；finder[5,5,0]多追加1项后nr回绕，下一次退出进入nr>0失败回滚，释放两个buf并清clusters/flags，却不清buffers指针槽，nr_clus保持原值。nr0仍可尝试追加，数组从count0构造而缺失时会先在buffers[0]失去合法前提；nr_clus==max_nr_clus时也会先越界，不可编造后续rollback已发生。expand内部未调用FAT setter完成持久预留，回滚也未撤销任何真实FAT写。`fat_add_entries`遇end直接调用expand(1)但**不消费int返回**，接着调用next；异常游标/ERR_PTR须在首次无效数据处STOP。AG56-F02专指扩容计数、容量和回滚，BI55-F04仍指NEWCLUS=3的release状态，二者不重复。
## G03 C 几何字段到传输单位
`read_fat32_superblock`在有效DPTE、BPB、alloc_super及fsbi前提下，将GPT u64 StartingLBA、EndingLBA及BPB的u16 BytesPerSec/u8 SecPerClus/u32 FATSz32/u8 NumFATs填进u64 fsbi：簇字节数=BPS×SPC、Data_firstsector=StartingLBA+RsvdSecCnt+FATSz32×NumFATs，FAT1等分别赋值。alloc_super只测试NULL（历史R38还允许ERR_PTR路径），fsbi kzalloc无单独NULL门；这些分配前提失效时停止，不能签已完成超级块绑定。u8×u16先按int计算，极限65535×255=16,711,425仍在int有效域；u32 FATSz32×u8 NumFATs在u32中可先回绕，再加到u64起始LBA，极端0x80000000×2得到u32零。该示例不代表真实BPB值。
`FAT32_clus_to_blknr`在合法簇条件下计算`((sector_t)clus-2)*SPC+DataFirst`，sector_t按所读类型是u64。文件簇数据buffer=BPS*SPC字节，而ROOTBLK count=SPC；只有下层单count单位也是BPS字节时才有名义一致性。IDE软件一次读256个u16、buffer按512字节推进；NVMe软件将node.count写入Dword12，真实NLB/硬件字节意义未验证。对于BPS512/SPC8且软件单位512，缓冲4096与count8名义匹配；BPS4096/SPC8而用IDE软件512单位，buffer32768却名义4096；BPS512/SPC8而假设NVMe单位4096，buffer4096却名义请求32768。都是条件单位模型，不能据此称实际DMA越界。
FAT getter用PAGE_SIZE=4096栈数组、清4096，`fat_entry>>7`使LBA逐1增加、`fat_entry&127`只访问128个u32即512字节，却通过SECT_PER_PG请求名义整页：ATA常量8，NVMe宏为4096/bootdisk_LBAsize（软件初值512，非构建/运行证明）。在512B LBA的假设里，相邻4KB读取窗口LBA只差1，名义重叠7个512B单位；finder/setter仅清512并扫首128项却也请求整页名义大小。iobuf release count=bufsize/BPS、FSInfo PAGE_SIZE读count=SECT_PER_PG、newdir按BPS分配buffer写count1，详facts.transfer_unit_matrix。真实磁盘单位、BPB可信度、NVMe NLB语义及硬件结果均OPAQUE，AG56-F03/F04仅条件来源，不是已验证磁盘损坏。
## G04 状态和未执行
A/B/C均completed_in_scope。AG56-F01..F04依不同根因列在deferred，保持旧BI55-F01..F05、IT35、R38、CA-03/04、UV-01规范状态。全部源码只读，未修改、预处理、编译、链接或执行仓库C/ASM/脚本/旧夹具；未跑QEMU/硬件/Graphviz/完整P9，也无实际磁盘和额外API。完成六件后只交C04独立验收，不自签PASS、不启动第57批。
