---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-BLOCK-IO-CONSUMPTION-55
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: unknown
produced_by_reason: "Backend author not attested; Owner specified GPT-6+Extra"
model_per_owner: GPT-6
effort_per_owner: Extra
read_channel: GitHub_connector
source_pin_short12: a039d9803ade
date: 2026-11-07
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: combined_block_io_analysis
---
# 第55批：块返回值怎样进入文件字节、iobuf与FAT表项
## B00共同限定
time@a039d9803ade固定源码。本轮按83、R53C-S01、R54-S01/S02、79、63和R35/R38消费。53文档已经闭合，53/54均PASS_PENDING_LOCAL，不返工、不传播已撤回的A03/B03空行指控。54驱动正常有效返回long0只能说明软件返回通道，不能证明磁盘数据正确；以下非零设备返回值都是有限条件假设，未进行真机失效注入。
## A：ROOTBLK和FAT32_read/write
ROOTBLK_TRANSFER经`#ifdef ROOTBLK_NVME`选择NVMe_ops.transfer(0,0,cmd,LBA,count,buf)、NVM_CMD_READ/WRITE及SECT_PER_PG=SZ_4K/bootdisk_LBAsize；else选择ATA_master_ops.transfer(MASTER,SLAVE,...)、ATA_CMD_READ/WRITE、SECT_PER_PG=8。transfer类型为long。固定options_flags.cmake里-DROOTBLK_NVME在注释中，但不签真实编译宏选择。
FAT32_read返回ssize_t，取得first_cluster、几何与position，先kmalloc一个cluster大小buffer但没有NULL门。first_cluster0时直接-EFAULT；其它路径按index追FAT项并依据i_size调整count，size_t→int index及loff_t/u64运算需有效值域。do-while意味着count0也可能执行一次块读。若ROOTBLK CMD_READ返回非零，设置-EIO并break；返回0后计算length、copy_to_user或memcpy。copy_to_user返回未复制余数cplen，但函数没有核它，照样按length更新index、buf和*position。先成功一段再错误时position已推进，最终却可能返回-EIO；index归零返回count并不保证用户真实收到了count字节。
FAT32_write同样在flags0时读取旧块并对非零结果-EIO，写块返回非零也-EIO；copy_from_user返回余数cplen未核便继续写入/推进。新文件first_cluster0时找簇并设flags1，write_inode/FAT setter结果都不检查；`if(!flags)`控制的sector计算被跳过，而sector入口为0，因此在有效新簇和先前函数正常返回条件下，首次CMD_WRITE收到sector0。后续扩簇flags继续置1而非重新算sector；这是源码候选，不宣称盘0扇区实际被写坏。zero count仍可进入do块读/写。部分完成后新-EIO与position推进可不一致；更新i_size时write_inode结果仍被忽略。分配、偏移、簇及缓冲大小失效时止于首次问题，不补称后续copy/free/return必发生。
## B：iobuf缓存发布、脏标记和释放
iobuf_init先以bytes_per_cluster计算bufsize及MAX_FILE_SIZE/bufsize（零除数提前STOP），调用get_cluster_chain只查NULL，旧R35已表明ERR_PTR不能由此门拦住。正常有效路径分配buffers/clusters/flags，把链尾逐项填入clusters[i]，不无证认为clusters[0]就是文件逻辑第一簇。readent以off算bufidx/bufoff，cache miss时kmalloc，然后用clusters[0]映射块并ROOTBLK读，但**丢弃long结果**便把buffer发布到buffers[bufidx]；cache hit直接返回原缓存地址。读取失败但缓冲未改/部分改写的影响只能作有前提模型。
iobuf_write仅验证起始off处于整体bufend范围，缺缓存时块读也丢弃返回；随后`buffers[bufidx][bufoff+i]`逐字写size个字节并OR DIRTY，未检查size<=bufsize-bufoff。如果越过单个buffer，止于首次非法store，不能拿尾部-ENOERR证明安全。DIRTY=1、DELETE=2、NEWCLUS=3，3又等于1|2；expand赋flags3再OR DIRTY仍3，而release只用`flags==DIRTY`和`flags==DELETE`相等测试，3均不满足。release每次循环重置clus_end_change=false；同轮DELETE设true后`flags!=DELETE`为假，下一轮更不会记住true，因此EOF写门不能借跨轮状态成立。
release仅flags恰为DIRTY时发ROOTBLK CMD_WRITE，恰为DELETE时发FAT_ENT_FREE setter，二者都丢弃结果；之后释放单缓冲、clusters、flags与iobuf，完整函数未见释放buffers数组指针。它返回void，资源释放与数据持久化是不同事。若写失败而callee正常返回，后续free仍可能继续；若callee不返回，不能自动补后续清理。
## C：FAT表项和直接消费
FAT32_read_FAT_Entry返回u32，先将PAGE_SIZE栈数组全部清零，忽略ROOTBLK读取结果，再按fat_entry&0x7f取28位值。假设错误读取未改变缓冲，则值0来自预置0，不是受验证的磁盘FAT_ENT_FREE。FAT32_find_available_cluster每轮只清前512字节，忽略块读取值后扫128个u32，失败未改写可在i0/j0得到0或在i1/j0得到128，不能把所选数字当可靠空簇。
FAT32_write_FAT_Entry返回uint64_t，PAGE_SIZE缓冲只清前512，忽略预读返回并合并高4/低28位，按NumFATs为每份发ROOTBLK CMD_WRITE却不核任一结果，最终固定return1。NumFATs=0什么都没写仍1；多副本一份写失败的条件序列仍是1，不认证实际副本已分叉。get_cluster_chain将getter的u32解释为下一簇，遇BAD精确值可ERR_PTR(-EIO)，其它越出有效范围终止，不能反推出块读成功。iobuf_release、文件write、目录FAT32_alloc_new_dir忽略setter结果；后者检查直接块写非零-EIO但FAT表结果仍忽略；fat_add_entries忽略iobuf_write且最终return0，纵使局部error曾置负。
## B04整包状态与未执行
文件ssize_t、块long、iobuf指针/int/void、FAT getter/finder u32、setter u64与目录值分别见facts.interface_matrix；图只表示被所读源码支持的当前对象条件关系。BI55-F01–F05均是和RH54/IT35去重后的有前提源码候选，非已观察的UAF、越界、丢数据或盘损坏。CA-03/04保持SUPPORTED_IN_SCOPE、UV-01=deferred_owner_not_ready；没有新验收或修复。内核C/ASM/脚本、编译预处理、旧夹具、QEMU硬件、真实I/O/usercopy、Graphviz及完整P9一律NOT_RUN。A/B/C completed_in_scope，交C04独立验收，不自行56。
