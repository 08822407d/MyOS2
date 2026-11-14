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
record_type: literal_fixed_source_evidence
---
# 第55批证据账本（真实CRLF、tab、空行保留）
每段为time固定源真实连续1–5行。超过5行的原连续范围自动拆为不同编号，须合读同consumer，不删除中间行。单证的文字逐字、真实函数/宏/类型归属、命题支持和消费者映射分别自核；不是代码运行或GitHub主线签收。
## BI55-A01 · long transfer signature and block params
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::blkdev_ops_s]
```c
		long (* transfer)(unsigned controller, unsigned disk,
						long cmd, unsigned long blk_idx,
						long count, unsigned char * buffer);
	} blkdev_ops_s;
```
source_claim_metadata: {"id":"BI55-A01","source_blob_short12":"a97a1162420f","actual_definition_kind":"typedef","enclosing_symbol":"blkdev_ops_s","definition_range":"32-41","quote_range":"38-41","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.macro.type"}

## BI55-A02 · NVMe conditional transfer(0,0,...)
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::ROOTBLK_TRANSFER]
```c
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				NVMe_ops.transfer(0, 0, cmd, blk_idx, count, buffer)
```
source_claim_metadata: {"id":"BI55-A02","source_blob_short12":"a97a1162420f","actual_definition_kind":"macro","enclosing_symbol":"ROOTBLK_TRANSFER","definition_range":"48-49","quote_range":"48-49","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.macro.nv"}

## BI55-A03 · ATA else transfer(MASTER,SLAVE,...)
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::ROOTBLK_TRANSFER]
```c
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				ATA_master_ops.transfer(MASTER, SLAVE, cmd, blk_idx, count, buffer)
```
source_claim_metadata: {"id":"BI55-A03","source_blob_short12":"a97a1162420f","actual_definition_kind":"macro","enclosing_symbol":"ROOTBLK_TRANSFER","definition_range":"55-56","quote_range":"55-56","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.macro.ata"}

## BI55-A04 · NVMe sector number expression
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::SECT_PER_PG]
```c
	#define SECT_PER_PG		(SZ_4K / bootdisk_LBAsize)
```
source_claim_metadata: {"id":"BI55-A04","source_blob_short12":"a97a1162420f","actual_definition_kind":"macro","enclosing_symbol":"SECT_PER_PG","definition_range":"52-52","quote_range":"52-52","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.macro.nv_sectors"}

## BI55-A05 · ATA eight sectors per page
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::SECT_PER_PG]
```c
	#define SECT_PER_PG		8
```
source_claim_metadata: {"id":"BI55-A05","source_blob_short12":"a97a1162420f","actual_definition_kind":"macro","enclosing_symbol":"SECT_PER_PG","definition_range":"59-59","quote_range":"59-59","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.macro.ata_sectors"}

## BI55-A06 · part 1/3 of read ssize_t entry, cluster/position and buffer
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
ssize_t FAT32_read(file_s *filp, char *buf, size_t count, loff_t *position)
{
	FAT32_inode_info_s * finode = filp->f_path.dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->f_path.dentry->d_inode->i_sb->private_sb_info;

```
source_claim_metadata: {"id":"BI55-A06","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"114-118","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.initial"}

## BI55-A07 · part 2/3 of read ssize_t entry, cluster/position and buffer
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	ulong cluster = finode->first_cluster;
	ulong sector = 0;
	int i, length = 0;
	long ret_val = 0;
	ulong cplen;
```
source_claim_metadata: {"id":"BI55-A07","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"119-123","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.initial"}

## BI55-A08 · part 3/3 of read ssize_t entry, cluster/position and buffer
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);
```
source_claim_metadata: {"id":"BI55-A08","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"124-126","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.initial"}

## BI55-A09 · part 1/2 of read invalid first cluster and i_size adjustment
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	if(!cluster)
		return -EFAULT;
	for(i = 0; i < index; i++)
		cluster = FAT32_read_FAT_Entry(fsbi, cluster);

```
source_claim_metadata: {"id":"BI55-A09","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"128-132","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.count"}

## BI55-A10 · part 2/2 of read invalid first cluster and i_size adjustment
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	if(*position + count > filp->f_path.dentry->d_inode->i_size)
		index = count = filp->f_path.dentry->d_inode->i_size - *position;
	else
		index = count;
```
source_claim_metadata: {"id":"BI55-A10","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"133-136","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.count"}

## BI55-A11 · part 1/3 of read block return check and -EIO error
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	do
	{
		memset(buffer, 0, fsbi->bytes_per_cluster);
		sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
		if(ROOTBLK_TRANSFER(CMD_READ, sector,
```
source_claim_metadata: {"id":"BI55-A11","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"138-142","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.io_gate"}

## BI55-A12 · part 2/3 of read block return check and -EIO error
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
				fsbi->sector_per_cluster, (unchar *)buffer))
		{
			color_printk(RED, BLACK, "FAT32 FS(read) read disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
```
source_claim_metadata: {"id":"BI55-A12","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"143-147","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.io_gate"}

## BI55-A13 · part 3/3 of read block return check and -EIO error
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		}
```
source_claim_metadata: {"id":"BI55-A13","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"148-148","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.io_gate"}

## BI55-A14 · part 1/4 of read copy_to_user residue ignored, position updates and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < TASK_SIZE_MAX)
			cplen = copy_to_user(buf, buffer + offset, length);
		else
```
source_claim_metadata: {"id":"BI55-A14","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"150-154","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.progress"}

## BI55-A15 · part 2/4 of read copy_to_user residue ignored, position updates and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
			memcpy(buf, buffer + offset, length);

		index -= length;
		buf += length;
		offset -= offset;
```
source_claim_metadata: {"id":"BI55-A15","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"155-159","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.progress"}

## BI55-A16 · part 3/4 of read copy_to_user residue ignored, position updates and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		*position += length;
	} while(index && (cluster = FAT32_read_FAT_Entry(fsbi, cluster)));

	kfree(buffer);
	if(!index)
```
source_claim_metadata: {"id":"BI55-A16","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"160-164","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.progress"}

## BI55-A17 · part 4/4 of read copy_to_user residue ignored, position updates and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		ret_val = count;
	return ret_val;
```
source_claim_metadata: {"id":"BI55-A17","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"165-166","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.read.progress"}

## BI55-A18 · part 1/5 of write ssize_t entry, sector0/flags and new-cluster selection
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
ssize_t FAT32_write(file_s *filp, const char *buf, size_t count, loff_t *position)
{
	FAT32_inode_info_s * finode = filp->f_path.dentry->d_inode->private_idx_info;
	FAT32_SBinfo_s * fsbi = filp->f_path.dentry->d_inode->i_sb->private_sb_info;

```
source_claim_metadata: {"id":"BI55-A18","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"169-173","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.new"}

## BI55-A19 · part 2/5 of write ssize_t entry, sector0/flags and new-cluster selection
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	u32 cluster = finode->first_cluster;
	u32 next_cluster = 0;
	unsigned long sector = 0;
	int i,length = 0;
	long ret_val = 0;
```
source_claim_metadata: {"id":"BI55-A19","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"174-178","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.new"}

## BI55-A20 · part 3/5 of write ssize_t entry, sector0/flags and new-cluster selection
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	long flags = 0;
	ulong cplen;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);
```
source_claim_metadata: {"id":"BI55-A20","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"179-183","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.new"}

## BI55-A21 · part 4/5 of write ssize_t entry, sector0/flags and new-cluster selection
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c

	if(!cluster)
	{
		cluster = FAT32_find_available_cluster(fsbi);
		flags = 1;
```
source_claim_metadata: {"id":"BI55-A21","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"184-188","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.new"}

## BI55-A22 · part 5/5 of write ssize_t entry, sector0/flags and new-cluster selection
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	}
```
source_claim_metadata: {"id":"BI55-A22","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"189-189","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.new"}

## BI55-A23 · part 1/3 of write ENOSPC and unchecked inode/FAT metadata
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(!cluster)
	{
		kfree(buffer);
		return -ENOSPC;
	}
```
source_claim_metadata: {"id":"BI55-A23","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"194-198","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.metadata"}

## BI55-A24 · part 2/3 of write ENOSPC and unchecked inode/FAT metadata
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c

	if(flags)
	{
		finode->first_cluster = cluster;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
```
source_claim_metadata: {"id":"BI55-A24","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"199-203","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.metadata"}

## BI55-A25 · part 3/3 of write ENOSPC and unchecked inode/FAT metadata
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
```
source_claim_metadata: {"id":"BI55-A25","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"204-204","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.metadata"}

## BI55-A26 · part 1/3 of write flags0-sector computation and old-data read -EIO
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	do
	{
		if(!flags)
		{
			memset(buffer, 0, fsbi->bytes_per_cluster);
```
source_claim_metadata: {"id":"BI55-A26","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"209-213","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.preread"}

## BI55-A27 · part 2/3 of write flags0-sector computation and old-data read -EIO
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
			if(ROOTBLK_TRANSFER(CMD_READ, sector,
					fsbi->sector_per_cluster, (unchar *)buffer))
			{
				color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
```
source_claim_metadata: {"id":"BI55-A27","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"214-218","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.preread"}

## BI55-A28 · part 3/3 of write flags0-sector computation and old-data read -EIO
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
				ret_val = -EIO;
				break;
			}
```
source_claim_metadata: {"id":"BI55-A28","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"219-221","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.preread"}

## BI55-A29 · part 1/4 of write copy_from_user residue, CMD_WRITE gate and progress
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		length = index <= fsbi->bytes_per_cluster - offset ? index : fsbi->bytes_per_cluster - offset;

		if((unsigned long)buf < TASK_SIZE_MAX)
			cplen = copy_from_user(buffer + offset, (void *)buf, length);
		else
```
source_claim_metadata: {"id":"BI55-A29","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"224-228","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.write"}

## BI55-A30 · part 2/4 of write copy_from_user residue, CMD_WRITE gate and progress
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			memcpy(buffer + offset, buf, length);

		if(ROOTBLK_TRANSFER(CMD_WRITE, sector,
				fsbi->sector_per_cluster, (unchar *)buffer))
		{
```
source_claim_metadata: {"id":"BI55-A30","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"229-233","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.write"}

## BI55-A31 · part 3/4 of write copy_from_user residue, CMD_WRITE gate and progress
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			color_printk(RED, BLACK, "FAT32 FS(write) write disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
		}

```
source_claim_metadata: {"id":"BI55-A31","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"234-238","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.write"}

## BI55-A32 · part 4/4 of write copy_from_user residue, CMD_WRITE gate and progress
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		index -= length;
		buf += length;
		offset -= offset;
		*position += length;
```
source_claim_metadata: {"id":"BI55-A32","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"239-242","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.write"}

## BI55-A33 · part 1/3 of write allocates next cluster, ignores FAT setter and sets flags1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(next_cluster >= 0x0ffffff8)
		{
			next_cluster = FAT32_find_available_cluster(fsbi);
			if(!next_cluster)
			{
```
source_claim_metadata: {"id":"BI55-A33","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"249-253","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.extend"}

## BI55-A34 · part 2/3 of write allocates next cluster, ignores FAT setter and sets flags1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
				kfree(buffer);
				return -ENOSPC;
			}			
				
			FAT32_write_FAT_Entry(fsbi, cluster, next_cluster);
```
source_claim_metadata: {"id":"BI55-A34","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"254-258","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.extend"}

## BI55-A35 · part 3/3 of write allocates next cluster, ignores FAT setter and sets flags1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			FAT32_write_FAT_Entry(fsbi, next_cluster, 0x0ffffff8);
			cluster = next_cluster;
			flags = 1;
		}
```
source_claim_metadata: {"id":"BI55-A35","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"259-262","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.extend"}

## BI55-A36 · part 1/2 of write size update, unchecked write_inode and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(*position > filp->f_path.dentry->d_inode->i_size)
	{
		filp->f_path.dentry->d_inode->i_size = *position;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
	}
```
source_claim_metadata: {"id":"BI55-A36","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"266-270","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.return"}

## BI55-A37 · part 2/2 of write size update, unchecked write_inode and count return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c

	kfree(buffer);
	if(!index)
		ret_val = count;
	return ret_val;
```
source_claim_metadata: {"id":"BI55-A37","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"271-275","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.write.return"}

## BI55-A38 · part 1/2 of copy_from_user raw result/residue and zero padding
[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
			res = raw_copy_from_user(to, from, n);
		// 	instrument_copy_from_user_after(to, from, n, res);
		// }
		if (unlikely(res))
			memset(to + (n - res), 0, res);
```
source_claim_metadata: {"id":"BI55-A38","source_blob_short12":"ab39e43ad2f7","actual_definition_kind":"function","enclosing_symbol":"_inline_copy_from_user","definition_range":"149-167","quote_range":"161-165","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.usercopy.from"}

## BI55-A39 · part 2/2 of copy_from_user raw result/residue and zero padding
[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_from_user]
```c
		return res;
```
source_claim_metadata: {"id":"BI55-A39","source_blob_short12":"ab39e43ad2f7","actual_definition_kind":"function","enclosing_symbol":"_inline_copy_from_user","definition_range":"149-167","quote_range":"166-166","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.usercopy.from"}

## BI55-A40 · copy_to_user raw result is uncopied amount
[VERIFIED mykernel/include/linux/kernel/uaccess.h::_inline_copy_to_user]
```c
		if (access_ok(to, n)) {
			instrument_copy_to_user(to, from, n);
			n = raw_copy_to_user(to, from, n);
		}
		return n;
```
source_claim_metadata: {"id":"BI55-A40","source_blob_short12":"ab39e43ad2f7","actual_definition_kind":"function","enclosing_symbol":"_inline_copy_to_user","definition_range":"171-181","quote_range":"176-180","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"A.usercopy.to"}

## BI55-B01 · part 1/2 of cluster/sector/FAT geometry
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
		u64	sector_per_cluster;
		u64	bytes_per_cluster;
		u64	bytes_per_sector;

		u64	Data_firstsector;
```
source_claim_metadata: {"id":"BI55-B01","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"typedef","enclosing_symbol":"FAT32_SBinfo_s","definition_range":"86-104","quote_range":"91-95","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.geometry"}

## BI55-B02 · part 2/2 of cluster/sector/FAT geometry
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
		u64	FAT1_firstsector;
		u64	sector_per_FAT;
		u64	NumFATs;
```
source_claim_metadata: {"id":"BI55-B02","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"typedef","enclosing_symbol":"FAT32_SBinfo_s","definition_range":"86-104","quote_range":"96-98","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.geometry"}

## BI55-B03 · part 1/2 of iobuf arrays and nr_clus/bufsize
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IO_BUFFER]
```c
	typedef struct FAT32_IO_BUFFER
	{
		int		nr_clus;
		int		max_nr_clus;
		size_t	bufsize;
```
source_claim_metadata: {"id":"BI55-B03","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"typedef","enclosing_symbol":"FAT32_IO_BUFFER","definition_range":"155-171","quote_range":"155-159","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.type"}

## BI55-B04 · part 2/2 of iobuf arrays and nr_clus/bufsize
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IO_BUFFER]
```c

		u32		*clusters;
		u32		*flags;
		char	**buffers;
```
source_claim_metadata: {"id":"BI55-B04","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"typedef","enclosing_symbol":"FAT32_IO_BUFFER","definition_range":"155-171","quote_range":"160-163","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.type"}

## BI55-B05 · DIRTY=1
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IOBUF_DIRTY]
```c
	#define FAT32_IOBUF_DIRTY		1
```
source_claim_metadata: {"id":"BI55-B05","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"macro","enclosing_symbol":"FAT32_IOBUF_DIRTY","definition_range":"145-145","quote_range":"145-145","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.flags"}

## BI55-B06 · DELETE=2
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IOBUF_DELETE]
```c
	#define FAT32_IOBUF_DELETE		2
```
source_claim_metadata: {"id":"BI55-B06","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"macro","enclosing_symbol":"FAT32_IOBUF_DELETE","definition_range":"146-146","quote_range":"146-146","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.flags"}

## BI55-B07 · NEWCLUS=3
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IOBUF_NEWCLUS]
```c
	#define FAT32_IOBUF_NEWCLUS		3
```
source_claim_metadata: {"id":"BI55-B07","source_blob_short12":"f03f3c0c1ec7","actual_definition_kind":"macro","enclosing_symbol":"FAT32_IOBUF_NEWCLUS","definition_range":"147-147","quote_range":"147-147","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.flags"}

## BI55-B08 · part 1/3 of init max_nr division and weak chain error gate
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
FAT32_iobuf_s *FAT32_iobuf_init(inode_s *dir)
{
	int error = 0;
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;
```
source_claim_metadata: {"id":"BI55-B08","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"165-169","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init"}

## BI55-B09 · part 2/3 of init max_nr division and weak chain error gate
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	size_t bufsize = fsbi->bytes_per_cluster;
	size_t max_nr = FAT32_MAX_FILE_SIZE / bufsize;

	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
```
source_claim_metadata: {"id":"BI55-B09","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"170-174","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init"}

## BI55-B10 · part 3/3 of init max_nr division and weak chain error gate
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		return ERR_PTR(-ENOMEM);

	int count = clus_lhdrp->count;
	FAT32_iobuf_s *iobuf = kzalloc(sizeof(FAT32_iobuf_s), GFP_KERNEL);
	if (iobuf == NULL)
```
source_claim_metadata: {"id":"BI55-B10","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"175-179","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init"}

## BI55-B11 · part 1/5 of init alloc buffers/clusters/flags arrays
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->nr_clus = count;
	iobuf->max_nr_clus = max_nr;
	iobuf->bufsize = bufsize;
	if (count > 0)
	{
```
source_claim_metadata: {"id":"BI55-B11","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"184-188","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.arrays"}

## BI55-B12 · part 2/5 of init alloc buffers/clusters/flags arrays
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->buffers = kzalloc(max_nr * sizeof(char *), GFP_KERNEL);
		if (iobuf->buffers == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_bufps_fail;
```
source_claim_metadata: {"id":"BI55-B12","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"189-193","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.arrays"}

## BI55-B13 · part 3/5 of init alloc buffers/clusters/flags arrays
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		}
		iobuf->clusters = kzalloc(max_nr * sizeof(*iobuf->clusters), GFP_KERNEL);
		if (iobuf->clusters == NULL)
		{
			error = -ENOMEM;
```
source_claim_metadata: {"id":"BI55-B13","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"194-198","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.arrays"}

## BI55-B14 · part 4/5 of init alloc buffers/clusters/flags arrays
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
			goto alloc_iobuf_clusters_fail;
		}
		iobuf->flags = kzalloc(max_nr * sizeof(*iobuf->flags), GFP_KERNEL);
		if (iobuf->flags == NULL)
		{
```
source_claim_metadata: {"id":"BI55-B14","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"199-203","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.arrays"}

## BI55-B15 · part 5/5 of init alloc buffers/clusters/flags arrays
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
			error = -ENOMEM;
			goto alloc_iobuf_flags_fail;
		}
```
source_claim_metadata: {"id":"BI55-B15","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"204-206","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.arrays"}

## BI55-B16 · part 1/3 of init list-tail extraction into clusters
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	int i = 0;
	while (clus_lhdrp->count > 0)
	{
		List_s *lp = list_header_remove_tail(clus_lhdrp);
		clus_list_s *clus_lp = container_of(lp, clus_list_s, list);
```
source_claim_metadata: {"id":"BI55-B16","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"209-213","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.order"}

## BI55-B17 · part 2/3 of init list-tail extraction into clusters
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->clusters[i] = clus_lp->cluster;
		iobuf->flags[i] = 0;
		kfree(clus_lp);
		i++;
	}
```
source_claim_metadata: {"id":"BI55-B17","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"214-218","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.order"}

## BI55-B18 · part 3/3 of init list-tail extraction into clusters
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->fsbi = fsbi;
	iobuf->iter_cursor = -1;
	iobuf->iter_init = FAT32_iobuf_iter_init;
	iobuf->next = FAT32_iobuf_iter_next;
	return iobuf;
```
source_claim_metadata: {"id":"BI55-B18","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"219-223","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.init.order"}

## BI55-B19 · part 1/5 of readent bounds/cache miss, ignored ROOTBLK return, published buffer
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
const msdos_dirent_s *FAT32_iobuf_readent(FAT32_iobuf_s *iobuf, loff_t off)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
		return ERR_PTR(-ENOENT);
```
source_claim_metadata: {"id":"BI55-B19","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_readent","definition_range":"64-84","quote_range":"64-68","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.readent"}

## BI55-B20 · part 2/5 of readent bounds/cache miss, ignored ROOTBLK return, published buffer
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
```
source_claim_metadata: {"id":"BI55-B20","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_readent","definition_range":"64-84","quote_range":"69-73","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.readent"}

## BI55-B21 · part 3/5 of readent bounds/cache miss, ignored ROOTBLK return, published buffer
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
		if (buf == NULL)
			return ERR_PTR(-ENOMEM);

		ROOTBLK_TRANSFER(CMD_READ,
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
```
source_claim_metadata: {"id":"BI55-B21","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_readent","definition_range":"64-84","quote_range":"74-78","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.readent"}

## BI55-B22 · part 4/5 of readent bounds/cache miss, ignored ROOTBLK return, published buffer
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
	}

	return (msdos_dirent_s *)&(iobuf->buffers[bufidx][bufoff]);
```
source_claim_metadata: {"id":"BI55-B22","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_readent","definition_range":"64-84","quote_range":"79-83","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.readent"}

## BI55-B23 · part 5/5 of readent bounds/cache miss, ignored ROOTBLK return, published buffer
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_readent]
```c
}
```
source_claim_metadata: {"id":"BI55-B23","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_readent","definition_range":"64-84","quote_range":"84-84","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.readent"}

## BI55-B24 · part 1/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
int FAT32_iobuf_write(FAT32_iobuf_s *iobuf, loff_t off,
		char *content, size_t size)
{
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	if (off >= bufend || off < 0)
```
source_claim_metadata: {"id":"BI55-B24","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"86-90","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B25 · part 2/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
		return -ENOENT;
	int	bufidx = off / iobuf->bufsize;
	int	bufoff = off % iobuf->bufsize;
	if (iobuf->buffers[bufidx] == NULL)
	{
```
source_claim_metadata: {"id":"BI55-B25","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"91-95","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B26 · part 3/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			return -ENOMEM;

		ROOTBLK_TRANSFER(CMD_READ,
```
source_claim_metadata: {"id":"BI55-B26","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"96-100","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B27 · part 4/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
				FAT32_clus_to_blknr(iobuf->fsbi, iobuf->clusters[0]),
				iobuf->fsbi->sector_per_cluster, buf);
		iobuf->buffers[bufidx] = buf;
	}

```
source_claim_metadata: {"id":"BI55-B27","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"101-105","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B28 · part 5/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
	for (size_t i = 0; i < size; i++)
	{
		iobuf->buffers[bufidx][bufoff + i] = content[i];
		iobuf->flags[bufidx] |= FAT32_IOBUF_DIRTY;
	}
```
source_claim_metadata: {"id":"BI55-B28","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"106-110","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B29 · part 6/6 of write only offset bounds and size loop, DIRTY OR and 0 result
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_write]
```c
	
	return -ENOERR;
}
```
source_claim_metadata: {"id":"BI55-B29","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_write","definition_range":"86-113","quote_range":"111-113","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.write"}

## BI55-B30 · part 1/2 of expand sets NEWCLUS flag
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			break;
		
		iobuf->buffers[tmp_nr] = buf;
```
source_claim_metadata: {"id":"BI55-B30","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"134-138","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.expand"}

## BI55-B31 · part 2/2 of expand sets NEWCLUS flag
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		iobuf->clusters[tmp_nr] = cluster;
		iobuf->flags[tmp_nr] = FAT32_IOBUF_NEWCLUS;
		tmp_nr++;
		nr--;
	}
```
source_claim_metadata: {"id":"BI55-B31","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"139-143","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.expand"}

## BI55-B32 · part 1/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
void FAT32_iobuf_release(FAT32_iobuf_s *iobuf)
{
	if (iobuf->nr_clus > 0)
	{
		for (int i = iobuf->nr_clus - 1; i >= 0; i--)
```
source_claim_metadata: {"id":"BI55-B32","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"236-240","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B33 · part 2/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
		{
			bool clus_end_change = false;
			u32 cluster = iobuf->clusters[i];
			u32 flags = iobuf->flags[i];
			char *bufp = iobuf->buffers[i];
```
source_claim_metadata: {"id":"BI55-B33","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"241-245","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B34 · part 3/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			if (bufp != NULL)
			{
				if (flags == FAT32_IOBUF_DIRTY)
					ROOTBLK_TRANSFER(CMD_WRITE,
						FAT32_clus_to_blknr(iobuf->fsbi, cluster),
```
source_claim_metadata: {"id":"BI55-B34","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"246-250","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B35 · part 4/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
						iobuf->bufsize / iobuf->fsbi->bytes_per_sector, bufp);
				kfree(iobuf->buffers[i]);
			}
			if (flags == FAT32_IOBUF_DELETE)
			{
```
source_claim_metadata: {"id":"BI55-B35","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"251-255","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B36 · part 5/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
				clus_end_change = true;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_FREE);
			}
			if (flags != FAT32_IOBUF_DELETE && clus_end_change)
			{
```
source_claim_metadata: {"id":"BI55-B36","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"256-260","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B37 · part 6/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
				clus_end_change = false;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_EOF);
			}
		}
		kfree(iobuf->clusters);
```
source_claim_metadata: {"id":"BI55-B37","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"261-265","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B38 · part 7/7 of release per-loop flag, dirty/data/FAT writes ignored, free objects
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
		kfree(iobuf->flags);
	}
	kfree(iobuf);
}
```
source_claim_metadata: {"id":"BI55-B38","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"266-269","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.release"}

## BI55-B39 · chain ERR_PTR allocation boundary
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
	unsigned long cluster = finode->first_cluster;
	List_hdr_s *clus_lhdrp = kmalloc(sizeof(List_hdr_s), GFP_KERNEL);
	if (clus_lhdrp == NULL)
		return ERR_PTR(-ENOMEM);
	INIT_LIST_HEADER_S(clus_lhdrp);
```
source_claim_metadata: {"id":"BI55-B39","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"25-29","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"B.history"}

## BI55-C01 · part 1/2 of u32 FAT getter initializes full page and ignores read return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read_FAT_Entry]
```c
u32 FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry)
{
	uint32_t buf[PAGE_SIZE / sizeof(uint32_t)];
	memset(buf, 0, PAGE_SIZE);
	ROOTBLK_TRANSFER(CMD_READ,
```
source_claim_metadata: {"id":"BI55-C01","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read_FAT_Entry","definition_range":"35-43","quote_range":"35-39","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.readfat"}

## BI55-C02 · part 2/2 of u32 FAT getter initializes full page and ignores read return
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read_FAT_Entry]
```c
		fsbi->FAT1_firstsector + (fat_entry >> 7),
		SECT_PER_PG, (unchar *)buf);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
}
```
source_claim_metadata: {"id":"BI55-C02","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_read_FAT_Entry","definition_range":"35-43","quote_range":"40-43","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.readfat"}

## BI55-C03 · part 1/5 of find free only clears512, scans128, ignores block returns
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
u32 FAT32_find_available_cluster(FAT32_SBinfo_s * fsbi)
{
	int i, j;
	int fat_entry;
	ulong sector_per_fat = fsbi->sector_per_FAT;
```
source_claim_metadata: {"id":"BI55-C03","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"65-69","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.find"}

## BI55-C04 · part 2/5 of find free only clears512, scans128, ignores block returns
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
	uint32_t buf[PAGE_SIZE / sizeof(uint32_t)];

//	fsbi->fat_fsinfo->FSI_Free_Count & fsbi->fat_fsinfo->FSI_Nxt_Free not exactly,so unuse

	for(i = 0; i < sector_per_fat; i++)
```
source_claim_metadata: {"id":"BI55-C04","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"70-74","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.find"}

## BI55-C05 · part 3/5 of find free only clears512, scans128, ignores block returns
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
	{
		memset(buf, 0, 512);
		ROOTBLK_TRANSFER(CMD_READ,
			fsbi->FAT1_firstsector + i,
			SECT_PER_PG, (unchar *)buf);
```
source_claim_metadata: {"id":"BI55-C05","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"75-79","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.find"}

## BI55-C06 · part 4/5 of find free only clears512, scans128, ignores block returns
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c

		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
				return (i << 7) + j;
```
source_claim_metadata: {"id":"BI55-C06","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"80-84","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.find"}

## BI55-C07 · part 5/5 of find free only clears512, scans128, ignores block returns
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
		}
	}
	return 0;
}
```
source_claim_metadata: {"id":"BI55-C07","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"85-88","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.find"}

## BI55-C08 · part 1/4 of u64 FAT setter ignores read and repeated writes, return1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry, u32 value)
{
	uint32_t buf[PAGE_SIZE / sizeof(uint32_t)];
	int i;

```
source_claim_metadata: {"id":"BI55-C08","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"90-94","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.writefat"}

## BI55-C09 · part 2/4 of u64 FAT setter ignores read and repeated writes, return1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
	memset(buf,0,512);
	ROOTBLK_TRANSFER(CMD_READ,
		fsbi->FAT1_firstsector + (fat_entry >> 7),
		SECT_PER_PG, (unchar *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);
```
source_claim_metadata: {"id":"BI55-C09","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"95-99","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.writefat"}

## BI55-C10 · part 3/4 of u64 FAT setter ignores read and repeated writes, return1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c

	for(i = 0; i < fsbi->NumFATs; i++)
		ROOTBLK_TRANSFER(CMD_WRITE,
			fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
			SECT_PER_PG, (unchar *)buf);
```
source_claim_metadata: {"id":"BI55-C10","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"100-104","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.writefat"}

## BI55-C11 · part 4/4 of u64 FAT setter ignores read and repeated writes, return1
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
	return 1;	
}
```
source_claim_metadata: {"id":"BI55-C11","source_blob_short12":"447307aaa4b1","actual_definition_kind":"function","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"105-106","line_ending":"CRLF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.writefat"}

## BI55-C12 · valid cluster lower bound
[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_START_ENT]
```c
	#define FAT_START_ENT		2
```
source_claim_metadata: {"id":"BI55-C12","source_blob_short12":"bab4f2fcbc97","actual_definition_kind":"macro","enclosing_symbol":"FAT_START_ENT","definition_range":"61-61","quote_range":"61-61","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.range"}

## BI55-C13 · valid cluster upper bound
[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::MAX_FAT32]
```c
	#define MAX_FAT32			0x0FFFFFF6
```
source_claim_metadata: {"id":"BI55-C13","source_blob_short12":"bab4f2fcbc97","actual_definition_kind":"macro","enclosing_symbol":"MAX_FAT32","definition_range":"66-66","quote_range":"66-66","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.range"}

## BI55-C14 · bad cluster sentinel
[VERIFIED mykernel/include/uapi/linux/msdos_fs.h::FAT_ENT_BAD]
```c
	#define FAT_ENT_BAD			(BAD_FAT32)
```
source_claim_metadata: {"id":"BI55-C14","source_blob_short12":"bab4f2fcbc97","actual_definition_kind":"macro","enclosing_symbol":"FAT_ENT_BAD","definition_range":"79-79","quote_range":"79-79","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.bad"}

## BI55-C15 · part 1/5 of chain consumes FAT next result/range/BAD error
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
	if (cluster > 0)
	{
		do
		{
			clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s), GFP_KERNEL);
```
source_claim_metadata: {"id":"BI55-C15","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"31-35","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.chain"}

## BI55-C16 · part 2/5 of chain consumes FAT next result/range/BAD error
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			if (clus_sp == NULL)
			{
				free_cluster_chain(clus_lhdrp);
				return ERR_PTR(-ENOMEM);
			}
```
source_claim_metadata: {"id":"BI55-C16","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"36-40","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.chain"}

## BI55-C17 · part 3/5 of chain consumes FAT next result/range/BAD error
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			INIT_LIST_S(&clus_sp->list);

			clus_sp->cluster = cluster;
			list_header_add_to_tail(clus_lhdrp, &clus_sp->list);
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);
```
source_claim_metadata: {"id":"BI55-C17","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"41-45","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.chain"}

## BI55-C18 · part 4/5 of chain consumes FAT next result/range/BAD error
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
		} while (cluster <= MAX_FAT32 && cluster >= FAT_START_ENT);

		if (cluster == FAT_ENT_BAD)
		{
			free_cluster_chain(clus_lhdrp);
```
source_claim_metadata: {"id":"BI55-C18","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"46-50","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.chain"}

## BI55-C19 · part 5/5 of chain consumes FAT next result/range/BAD error
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			clus_lhdrp = ERR_PTR(-EIO);
		}
	}
	return clus_lhdrp;
```
source_claim_metadata: {"id":"BI55-C19","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"51-54","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.chain"}

## BI55-C20 · part 1/2 of iobuf release discards FAT setter results
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			if (flags == FAT32_IOBUF_DELETE)
			{
				clus_end_change = true;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_FREE);
			}
```
source_claim_metadata: {"id":"BI55-C20","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"254-258","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.release"}

## BI55-C21 · part 2/2 of iobuf release discards FAT setter results
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
			if (flags != FAT32_IOBUF_DELETE && clus_end_change)
			{
				clus_end_change = false;
				FAT32_write_FAT_Entry(iobuf->fsbi, cluster, FAT_ENT_EOF);
			}
```
source_claim_metadata: {"id":"BI55-C21","source_blob_short12":"f577b7f65786","actual_definition_kind":"function","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"259-263","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.release"}

## BI55-C22 · part 1/2 of new dir checks block write, ignores FAT setter
[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	sector = FAT32_clus_to_blknr(fsbi, cluster);
	if(ROOTBLK_TRANSFER(CMD_WRITE, sector, 1, (unchar *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
```
source_claim_metadata: {"id":"BI55-C22","source_blob_short12":"a6ef65d326d2","actual_definition_kind":"function","enclosing_symbol":"FAT32_alloc_new_dir","definition_range":"119-155","quote_range":"146-150","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.newdir"}

## BI55-C23 · part 2/2 of new dir checks block write, ignores FAT setter
[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	}

	FAT32_write_FAT_Entry(fsbi, cluster, FAT_ENT_EOF);
	return cluster;
}
```
source_claim_metadata: {"id":"BI55-C23","source_blob_short12":"a6ef65d326d2","actual_definition_kind":"function","enclosing_symbol":"FAT32_alloc_new_dir","definition_range":"119-155","quote_range":"151-155","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.newdir"}

## BI55-C24 · part 1/3 of directory ignores iobuf_write and returns0
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	if (emtpy_count == 0 && empty_off == -1)
	{
		error = -ENOENT;
		goto not_found;
	}
```
source_claim_metadata: {"id":"BI55-C24","source_blob_short12":"a6ef65d326d2","actual_definition_kind":"function","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"193-197","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.dir"}

## BI55-C25 · part 2/3 of directory ignores iobuf_write and returns0
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	else
	{
		FAT32_iobuf_write(iobuf, empty_off, slots,
				nr_slots * sizeof(msdos_dirent_s));
	}
```
source_claim_metadata: {"id":"BI55-C25","source_blob_short12":"a6ef65d326d2","actual_definition_kind":"function","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"198-202","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.dir"}

## BI55-C26 · part 3/3 of directory ignores iobuf_write and returns0
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c

not_found:
	FAT32_iobuf_release(iobuf);
	return 0;
}
```
source_claim_metadata: {"id":"BI55-C26","source_blob_short12":"a6ef65d326d2","actual_definition_kind":"function","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"203-207","line_ending":"LF","exact_text_including_blank_lines":true,"inside_real_definition":true,"claim_supported_in_segment_or_group":true,"consumer":"C.dir"}

## 历史限定
R35-S01/S02中的get_cluster_chain ERR_PTR与clusters[0]下标身份不重复编号；R38-S01/S02的两套FAT超级块/对象来源不等同；R54-S01/S02驱动long值不等于硬件成功。R53C-S01已撤回的A03/B03空行误判不再传播。

## EOF未终止换行的两处结构注释
固定源`myos_fat32_iobuf.c`末尾269行与`myos_dir.c`末尾207行均无终止LF。为让Markdown代码围栏独立成行，文档在两处末尾源码`}`之后加一个**围栏分隔换行**；该分隔符不是原源码的一部分，不纳入source raw quote字节认证。两个代码片段的内容、tab与最终`}`均来自固定Git blob，其余CRLF/LF引用保留原样。
