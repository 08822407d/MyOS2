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
record_type: bounded_source_quote_ledger
---
# AG56 固定源码证据：A/B/C分组
从time@a039d9803ade Git Blob逐字截取，每段1–5连续源码行，保留Tab、空行和CRLF。长范围拆分为同consumer的多段，不删除中间行。事实/定义/命题/消费者各自验证；引文不能替代真实编译或主线独立审查。
## AG56-A01 · read current cluster, index and offset，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	ulong cluster = finode->first_cluster;
	ulong sector = 0;
	int i, length = 0;
	long ret_val = 0;
	ulong cplen;
```
quote_metadata: {"id":"AG56-A01","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"119-123","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.119-125"}

## AG56-A02 · read current cluster, index and offset，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
```
quote_metadata: {"id":"AG56-A02","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"124-125","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.119-125"}

## AG56-A03 · read skip FAT chain then reassign byte count，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	if(!cluster)
		return -EFAULT;
	for(i = 0; i < index; i++)
		cluster = FAT32_read_FAT_Entry(fsbi, cluster);

```
quote_metadata: {"id":"AG56-A03","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"128-132","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.128-136"}

## AG56-A04 · read skip FAT chain then reassign byte count，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
	if(*position + count > filp->f_path.dentry->d_inode->i_size)
		index = count = filp->f_path.dentry->d_inode->i_size - *position;
	else
		index = count;
```
quote_metadata: {"id":"AG56-A04","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"133-136","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.128-136"}

## AG56-A05 · read map current cluster to sector and transfer count
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		memset(buffer, 0, fsbi->bytes_per_cluster);
		sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
		if(ROOTBLK_TRANSFER(CMD_READ, sector,
				fsbi->sector_per_cluster, (unchar *)buffer))
```
quote_metadata: {"id":"AG56-A05","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"140-143","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.140-143"}

## AG56-A06 · read do-tail assigns next cluster from getter
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read]
```c
		index -= length;
		buf += length;
		offset -= offset;
		*position += length;
	} while(index && (cluster = FAT32_read_FAT_Entry(fsbi, cluster)));
```
quote_metadata: {"id":"AG56-A06","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read","definition_range":"114-167","quote_range":"157-161","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_read.157-161"}

## AG56-A07 · write cluster/next_cluster/sector0/flags0 initial state，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	u32 cluster = finode->first_cluster;
	u32 next_cluster = 0;
	unsigned long sector = 0;
	int i,length = 0;
	long ret_val = 0;
```
quote_metadata: {"id":"AG56-A07","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"174-178","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.174-183"}

## AG56-A08 · write cluster/next_cluster/sector0/flags0 initial state，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	long flags = 0;
	ulong cplen;
	int index = *position / fsbi->bytes_per_cluster;
	long offset = *position % fsbi->bytes_per_cluster;
	char * buffer = (char *)kmalloc(fsbi->bytes_per_cluster, GFP_KERNEL);
```
quote_metadata: {"id":"AG56-A08","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"179-183","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.174-183"}

## AG56-A09 · new candidate and old chain skip，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(!cluster)
	{
		cluster = FAT32_find_available_cluster(fsbi);
		flags = 1;
	}
```
quote_metadata: {"id":"AG56-A09","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"185-189","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.185-192"}

## AG56-A10 · new candidate and old chain skip，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	else
		for(i = 0; i < index; i++)
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);
```
quote_metadata: {"id":"AG56-A10","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"190-192","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.185-192"}

## AG56-A11 · zero candidate failure, first cluster publication and FAT mark，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	if(!cluster)
	{
		kfree(buffer);
		return -ENOSPC;
	}
```
quote_metadata: {"id":"AG56-A11","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"194-198","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.194-205"}

## AG56-A12 · zero candidate failure, first cluster publication and FAT mark，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c

	if(flags)
	{
		finode->first_cluster = cluster;
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
```
quote_metadata: {"id":"AG56-A12","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"199-203","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.194-205"}

## AG56-A13 · zero candidate failure, first cluster publication and FAT mark，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		FAT32_write_FAT_Entry(fsbi, cluster, 0x0ffffff8);
	}
```
quote_metadata: {"id":"AG56-A13","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"204-205","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.194-205"}

## AG56-A14 · sector calculated only under flags0，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	do
	{
		if(!flags)
		{
			memset(buffer, 0, fsbi->bytes_per_cluster);
```
quote_metadata: {"id":"AG56-A14","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"209-213","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.209-215"}

## AG56-A15 · sector calculated only under flags0，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
			if(ROOTBLK_TRANSFER(CMD_READ, sector,
```
quote_metadata: {"id":"AG56-A15","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"214-215","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.209-215"}

## AG56-A16 · data write uses current sector
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(ROOTBLK_TRANSFER(CMD_WRITE, sector,
				fsbi->sector_per_cluster, (unchar *)buffer))
```
quote_metadata: {"id":"AG56-A16","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"231-232","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.231-232"}

## AG56-A17 · next_cluster read and EOF-only branch，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		index -= length;
		buf += length;
		offset -= offset;
		*position += length;

```
quote_metadata: {"id":"AG56-A17","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"239-243","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.239-249"}

## AG56-A18 · next_cluster read and EOF-only branch，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(index)
			next_cluster = FAT32_read_FAT_Entry(fsbi, cluster);
		else
			break;

```
quote_metadata: {"id":"AG56-A18","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"244-248","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.239-249"}

## AG56-A19 · next_cluster read and EOF-only branch，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(next_cluster >= 0x0ffffff8)
```
quote_metadata: {"id":"AG56-A19","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"249-249","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.239-249"}

## AG56-A20 · EOF extension requests FAT links then sets cluster and flags1，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		if(next_cluster >= 0x0ffffff8)
		{
			next_cluster = FAT32_find_available_cluster(fsbi);
			if(!next_cluster)
			{
```
quote_metadata: {"id":"AG56-A20","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"249-253","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.249-262"}

## AG56-A21 · EOF extension requests FAT links then sets cluster and flags1，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
				kfree(buffer);
				return -ENOSPC;
			}			
				
			FAT32_write_FAT_Entry(fsbi, cluster, next_cluster);
```
quote_metadata: {"id":"AG56-A21","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"254-258","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.249-262"}

## AG56-A22 · EOF extension requests FAT links then sets cluster and flags1，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
			FAT32_write_FAT_Entry(fsbi, next_cluster, 0x0ffffff8);
			cluster = next_cluster;
			flags = 1;
		}
```
quote_metadata: {"id":"AG56-A22","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"259-262","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.249-262"}

## AG56-A23 · loop exit and inode size update，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
	}while(index);

	if(*position > filp->f_path.dentry->d_inode->i_size)
	{
		filp->f_path.dentry->d_inode->i_size = *position;
```
quote_metadata: {"id":"AG56-A23","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"264-268","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.264-270"}

## AG56-A24 · loop exit and inode size update，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write]
```c
		filp->f_path.dentry->d_inode->i_sb->s_op->write_inode(filp->f_path.dentry->d_inode);
	}
```
quote_metadata: {"id":"AG56-A24","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write","definition_range":"169-276","quote_range":"269-270","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"A.FAT32_write.264-270"}

## AG56-B01 · size_t nr/int tmp_nr and unsigned >=0 loop，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
int FAT32_iobuf_expand(FAT32_iobuf_s *iobuf, size_t nr)
{
	u32 cluster = 0;
	int tmp_nr = iobuf->nr_clus;
	while (nr >= 0 &&
```
quote_metadata: {"id":"AG56-B01","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"127-131","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.127-132"}

## AG56-B02 · size_t nr/int tmp_nr and unsigned >=0 loop，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		(cluster = FAT32_find_available_cluster(iobuf->fsbi)) > 1)
```
quote_metadata: {"id":"AG56-B02","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"132-132","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.127-132"}

## AG56-B03 · allocate buffer, store 3 arrays then tmp_nr++/nr--，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		char *buf = kmalloc(iobuf->bufsize, GFP_KERNEL);
		if (buf == NULL)
			break;
		
		iobuf->buffers[tmp_nr] = buf;
```
quote_metadata: {"id":"AG56-B03","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"134-138","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.134-143"}

## AG56-B04 · allocate buffer, store 3 arrays then tmp_nr++/nr--，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		iobuf->clusters[tmp_nr] = cluster;
		iobuf->flags[tmp_nr] = FAT32_IOBUF_NEWCLUS;
		tmp_nr++;
		nr--;
	}
```
quote_metadata: {"id":"AG56-B04","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"139-143","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.134-143"}

## AG56-B05 · remaining nr>0 triggers buffer rollback and -EIO，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
	if (nr > 0)
	{
		while (tmp_nr > iobuf->nr_clus)
		{
			tmp_nr--;
```
quote_metadata: {"id":"AG56-B05","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"145-149","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.145-155"}

## AG56-B06 · remaining nr>0 triggers buffer rollback and -EIO，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
			kfree(iobuf->buffers[tmp_nr]);
			iobuf->clusters[tmp_nr] = 0;
			iobuf->flags[tmp_nr] = 0;
		}

```
quote_metadata: {"id":"AG56-B06","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"150-154","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.145-155"}

## AG56-B07 · remaining nr>0 triggers buffer rollback and -EIO，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
		return -EIO;
```
quote_metadata: {"id":"AG56-B07","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"155-155","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.145-155"}

## AG56-B08 · success commits nr_clus and resets end，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
	else
	{
		iobuf->nr_clus = tmp_nr;
		iobuf->iter_reach_end = false;
		return 0;
```
quote_metadata: {"id":"AG56-B08","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"157-161","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.157-162"}

## AG56-B09 · success commits nr_clus and resets end，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_expand]
```c
	}
```
quote_metadata: {"id":"AG56-B09","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_expand","definition_range":"127-163","quote_range":"162-162","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_expand.157-162"}

## AG56-B10 · init bufsize/MAX_FILE_SIZE and chain NULL-only gate，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
FAT32_iobuf_s *FAT32_iobuf_init(inode_s *dir)
{
	int error = 0;
	FAT32_inode_info_s * finode = dir->private_idx_info;
	FAT32_SBinfo_s * fsbi = dir->i_sb->private_sb_info;
```
quote_metadata: {"id":"AG56-B10","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"165-169","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.165-179"}

## AG56-B11 · init bufsize/MAX_FILE_SIZE and chain NULL-only gate，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	size_t bufsize = fsbi->bytes_per_cluster;
	size_t max_nr = FAT32_MAX_FILE_SIZE / bufsize;

	List_hdr_s *clus_lhdrp = get_cluster_chain(dir);
	if (clus_lhdrp == NULL)
```
quote_metadata: {"id":"AG56-B11","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"170-174","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.165-179"}

## AG56-B12 · init bufsize/MAX_FILE_SIZE and chain NULL-only gate，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		return ERR_PTR(-ENOMEM);

	int count = clus_lhdrp->count;
	FAT32_iobuf_s *iobuf = kzalloc(sizeof(FAT32_iobuf_s), GFP_KERNEL);
	if (iobuf == NULL)
```
quote_metadata: {"id":"AG56-B12","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"175-179","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.165-179"}

## AG56-B13 · arrays allocated only if count>0 at capacity max_nr，连贯片段1/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->nr_clus = count;
	iobuf->max_nr_clus = max_nr;
	iobuf->bufsize = bufsize;
	if (count > 0)
	{
```
quote_metadata: {"id":"AG56-B13","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"184-188","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.184-202"}

## AG56-B14 · arrays allocated only if count>0 at capacity max_nr，连贯片段2/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->buffers = kzalloc(max_nr * sizeof(char *), GFP_KERNEL);
		if (iobuf->buffers == NULL)
		{
			error = -ENOMEM;
			goto alloc_iobuf_bufps_fail;
```
quote_metadata: {"id":"AG56-B14","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"189-193","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.184-202"}

## AG56-B15 · arrays allocated only if count>0 at capacity max_nr，连贯片段3/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		}
		iobuf->clusters = kzalloc(max_nr * sizeof(*iobuf->clusters), GFP_KERNEL);
		if (iobuf->clusters == NULL)
		{
			error = -ENOMEM;
```
quote_metadata: {"id":"AG56-B15","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"194-198","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.184-202"}

## AG56-B16 · arrays allocated only if count>0 at capacity max_nr，连贯片段4/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
			goto alloc_iobuf_clusters_fail;
		}
		iobuf->flags = kzalloc(max_nr * sizeof(*iobuf->flags), GFP_KERNEL);
		if (iobuf->flags == NULL)
```
quote_metadata: {"id":"AG56-B16","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"199-202","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.184-202"}

## AG56-B17 · chain tail popped to clusters[i], init returns iobuf，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	int i = 0;
	while (clus_lhdrp->count > 0)
	{
		List_s *lp = list_header_remove_tail(clus_lhdrp);
		clus_list_s *clus_lp = container_of(lp, clus_list_s, list);
```
quote_metadata: {"id":"AG56-B17","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"209-213","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.209-223"}

## AG56-B18 · chain tail popped to clusters[i], init returns iobuf，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
		iobuf->clusters[i] = clus_lp->cluster;
		iobuf->flags[i] = 0;
		kfree(clus_lp);
		i++;
	}
```
quote_metadata: {"id":"AG56-B18","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"214-218","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.209-223"}

## AG56-B19 · chain tail popped to clusters[i], init returns iobuf，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_init]
```c
	iobuf->fsbi = fsbi;
	iobuf->iter_cursor = -1;
	iobuf->iter_init = FAT32_iobuf_iter_init;
	iobuf->next = FAT32_iobuf_iter_next;
	return iobuf;
```
quote_metadata: {"id":"AG56-B19","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_init","definition_range":"165-234","quote_range":"219-223","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_init.209-223"}

## AG56-B20 · append chain nodes and derive next FAT entry，连贯片段1/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
	if (cluster > 0)
	{
		do
		{
			clus_list_s *clus_sp = kmalloc(sizeof(clus_list_s), GFP_KERNEL);
```
quote_metadata: {"id":"AG56-B20","blob_short12":"f577b7f65786","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"31-35","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.get_cluster_chain.31-46"}

## AG56-B21 · append chain nodes and derive next FAT entry，连贯片段2/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			if (clus_sp == NULL)
			{
				free_cluster_chain(clus_lhdrp);
				return ERR_PTR(-ENOMEM);
			}
```
quote_metadata: {"id":"AG56-B21","blob_short12":"f577b7f65786","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"36-40","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.get_cluster_chain.31-46"}

## AG56-B22 · append chain nodes and derive next FAT entry，连贯片段3/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
			INIT_LIST_S(&clus_sp->list);

			clus_sp->cluster = cluster;
			list_header_add_to_tail(clus_lhdrp, &clus_sp->list);
			cluster = FAT32_read_FAT_Entry(fsbi, cluster);
```
quote_metadata: {"id":"AG56-B22","blob_short12":"f577b7f65786","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"41-45","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.get_cluster_chain.31-46"}

## AG56-B23 · append chain nodes and derive next FAT entry，连贯片段4/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::get_cluster_chain]
```c
		} while (cluster <= MAX_FAT32 && cluster >= FAT_START_ENT);
```
quote_metadata: {"id":"AG56-B23","blob_short12":"f577b7f65786","enclosing_symbol":"get_cluster_chain","definition_range":"21-55","quote_range":"46-46","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.get_cluster_chain.31-46"}

## AG56-B24 · next advances cursor and reach_end，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
	loff_t	bufend = iobuf->nr_clus * iobuf->bufsize;
	const msdos_dirent_s *ret_val = FAT32_iobuf_readent(iobuf, iobuf->iter_cursor);

	iobuf->iter_cursor += sizeof(msdos_dirent_s);
	if (iobuf->iter_cursor >= bufend)
```
quote_metadata: {"id":"AG56-B24","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_iter_next","definition_range":"115-125","quote_range":"117-121","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_iter_next.117-124"}

## AG56-B25 · next advances cursor and reach_end，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_iter_next]
```c
		iobuf->iter_reach_end = true;

	return ret_val;
```
quote_metadata: {"id":"AG56-B25","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_iter_next","definition_range":"115-125","quote_range":"122-124","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_iter_next.117-124"}

## AG56-B26 · caller expand(1) result ignored then next，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
	FAT32_iobuf_s *iobuf = FAT32_iobuf_init(dir);
	iobuf->iter_init(iobuf);

	do
	{
```
quote_metadata: {"id":"AG56-B26","blob_short12":"a6ef65d326d2","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"163-167","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.fat_add_entries.163-173"}

## AG56-B27 · caller expand(1) result ignored then next，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		de = iobuf->next(iobuf);
		if (iobuf->iter_reach_end)
		{
			FAT32_iobuf_expand(iobuf, 1);
			de = iobuf->next(iobuf);
```
quote_metadata: {"id":"AG56-B27","blob_short12":"a6ef65d326d2","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"168-172","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.fat_add_entries.163-173"}

## AG56-B28 · caller expand(1) result ignored then next，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_dir.c::fat_add_entries]
```c
		}
```
quote_metadata: {"id":"AG56-B28","blob_short12":"a6ef65d326d2","enclosing_symbol":"fat_add_entries","definition_range":"157-207","quote_range":"173-173","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.fat_add_entries.163-173"}

## AG56-B29 · int nr_clus and max_nr_clus, three arrays，连贯片段1/2须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c
	typedef struct FAT32_IO_BUFFER
	{
		int		nr_clus;
		int		max_nr_clus;
		size_t	bufsize;
```
quote_metadata: {"id":"AG56-B29","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_iobuf_s","definition_range":"155-171","quote_range":"155-159","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_s.155-163"}

## AG56-B30 · int nr_clus and max_nr_clus, three arrays，连贯片段2/2须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_iobuf_s]
```c

		u32		*clusters;
		u32		*flags;
		char	**buffers;
```
quote_metadata: {"id":"AG56-B30","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_iobuf_s","definition_range":"155-171","quote_range":"160-163","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_s.155-163"}

## AG56-B31 · size limit macro 64MiB
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_MAX_FILE_SIZE]
```c
	#define FAT32_MAX_FILE_SIZE		(SZ_1M * 64)
```
quote_metadata: {"id":"AG56-B31","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_MAX_FILE_SIZE","definition_range":"143-143","quote_range":"143-143","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_MAX_FILE_SIZE.143-143"}

## AG56-B32 · NEWCLUS flag is 3
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_IOBUF_NEWCLUS]
```c
	#define FAT32_IOBUF_NEWCLUS		3
```
quote_metadata: {"id":"AG56-B32","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_IOBUF_NEWCLUS","definition_range":"147-147","quote_range":"147-147","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_IOBUF_NEWCLUS.147-147"}

## AG56-B33 · release DIRTY-equality writeback only
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
				if (flags == FAT32_IOBUF_DIRTY)
					ROOTBLK_TRANSFER(CMD_WRITE,
						FAT32_clus_to_blknr(iobuf->fsbi, cluster),
						iobuf->bufsize / iobuf->fsbi->bytes_per_sector, bufp);
				kfree(iobuf->buffers[i]);
```
quote_metadata: {"id":"AG56-B33","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"248-252","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_iobuf_release.248-252"}

## AG56-B34 · finder returns zero candidate or0 but does not reserve，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
				return (i << 7) + j;
		}
```
quote_metadata: {"id":"AG56-B34","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"81-85","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_find_available_cluster.81-87"}

## AG56-B35 · finder returns zero candidate or0 but does not reserve，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
	}
	return 0;
```
quote_metadata: {"id":"AG56-B35","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"86-87","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"B.FAT32_find_available_cluster.81-87"}

## AG56-C01 · GPT start/end LBA u64，连贯片段1/2须合读
[VERIFIED mykernel/include/linux/fs/GPT.h::GPT_PE_s]
```c
typedef struct GPT_Partition_Entry
{
	u64		PartitionTypeGUID[2];
	u64		UniquePartitionGUID[2];

```
quote_metadata: {"id":"AG56-C01","blob_short12":"c06f8c9ab51a","enclosing_symbol":"GPT_PE_s","definition_range":"43-52","quote_range":"43-47","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.GPT_PE_s.43-52"}

## AG56-C02 · GPT start/end LBA u64，连贯片段2/2须合读
[VERIFIED mykernel/include/linux/fs/GPT.h::GPT_PE_s]
```c
	u64		StartingLBA;
	u64		EndingLBA;
	u64		Attributes;
	char	PartitionName[72];
} GPT_PE_s;
```
quote_metadata: {"id":"AG56-C02","blob_short12":"c06f8c9ab51a","enclosing_symbol":"GPT_PE_s","definition_range":"43-52","quote_range":"48-52","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.GPT_PE_s.43-52"}

## AG56-C03 · BPB u16 bytesPerSec/u8 sectorsPerCluster/u8 NumFATs
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_BS_s]
```c
		u16	BPB_BytesPerSec;
		u8	BPB_SecPerClus;
		u16	BPB_RsvdSecCnt;
		u8	BPB_NumFATs;
```
quote_metadata: {"id":"AG56-C03","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_BS_s","definition_range":"26-61","quote_range":"30-33","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_BS_s.30-33"}

## AG56-C04 · BPB FATSz32 u32，连贯片段1/2须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_BS_s]
```c
		u32	BPB_FATSz32;
		u16	BPB_ExtFlags;
		u16	BPB_FSVer;
		u32	BPB_RootClus;
		u16	BPB_FSInfo;
```
quote_metadata: {"id":"AG56-C04","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_BS_s","definition_range":"26-61","quote_range":"43-47","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_BS_s.43-48"}

## AG56-C05 · BPB FATSz32 u32，连贯片段2/2须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_BS_s]
```c
		u16	BPB_BkBootSec;
```
quote_metadata: {"id":"AG56-C05","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_BS_s","definition_range":"26-61","quote_range":"48-48","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_BS_s.43-48"}

## AG56-C06 · fsbi geometry targets u64，连贯片段1/3须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
	typedef struct FAT32_SBinfo
	{
		u64	start_sector;
		u64	sector_count;

```
quote_metadata: {"id":"AG56-C06","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_SBinfo_s","definition_range":"86-104","quote_range":"86-90","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_SBinfo_s.86-98"}

## AG56-C07 · fsbi geometry targets u64，连贯片段2/3须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
		u64	sector_per_cluster;
		u64	bytes_per_cluster;
		u64	bytes_per_sector;

		u64	Data_firstsector;
```
quote_metadata: {"id":"AG56-C07","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_SBinfo_s","definition_range":"86-104","quote_range":"91-95","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_SBinfo_s.86-98"}

## AG56-C08 · fsbi geometry targets u64，连贯片段3/3须合读
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_SBinfo_s]
```c
		u64	FAT1_firstsector;
		u64	sector_per_FAT;
		u64	NumFATs;
```
quote_metadata: {"id":"AG56-C08","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_SBinfo_s","definition_range":"86-104","quote_range":"96-98","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_SBinfo_s.86-98"}

## AG56-C09 · alloc_super/check NULL and fsbi allocation，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp = alloc_super(&FAT32_fs_type, 0);
	if (sbp == NULL)
		return ERR_PTR(-ENOMEM);

	sbp->s_op = &FAT32_sb_ops;
```
quote_metadata: {"id":"AG56-C09","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"429-433","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.429-434"}

## AG56-C10 · alloc_super/check NULL and fsbi allocation，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	sbp->private_sb_info = kzalloc(sizeof(FAT32_SBinfo_s), GFP_KERNEL);
```
quote_metadata: {"id":"AG56-C10","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"434-434","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.429-434"}

## AG56-C11 · producer of cluster and FAT geometry，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	fbs = (FAT32_BS_s *)buf;
	fsbi = sbp->private_sb_info;	
	fsbi->start_sector = DPTE->StartingLBA;
	fsbi->sector_count = DPTE->EndingLBA - DPTE->StartingLBA;
	fsbi->sector_per_cluster = fbs->BPB_SecPerClus;
```
quote_metadata: {"id":"AG56-C11","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"437-441","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.437-450"}

## AG56-C12 · producer of cluster and FAT geometry，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	fsbi->bytes_per_cluster = fbs->BPB_SecPerClus * fbs->BPB_BytesPerSec;
	fsbi->bytes_per_sector = fbs->BPB_BytesPerSec;
	fsbi->Data_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt +
								fbs->BPB_FATSz32 * fbs->BPB_NumFATs;
	fsbi->FAT1_firstsector = DPTE->StartingLBA + fbs->BPB_RsvdSecCnt;
```
quote_metadata: {"id":"AG56-C12","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"442-446","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.437-450"}

## AG56-C13 · producer of cluster and FAT geometry，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	fsbi->sector_per_FAT = fbs->BPB_FATSz32;
	fsbi->NumFATs = fbs->BPB_NumFATs;
	fsbi->fsinfo_sector_infat = fbs->BPB_FSInfo;
	fsbi->bootsector_bk_infat = fbs->BPB_BkBootSec;	
```
quote_metadata: {"id":"AG56-C13","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"447-450","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.437-450"}

## AG56-C14 · FSInfo PAGE_SIZE allocation/read and metadata copy，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	void *fsinfo_buf = kzalloc(PAGE_SIZE, GFP_KERNEL);
	ROOTBLK_TRANSFER(CMD_READ,
		DPTE->StartingLBA + fbs->BPB_FSInfo,
		SECT_PER_PG, fsinfo_buf);
	fsbi->fat_fsinfo = kzalloc(sizeof(FAT32_FSinfo_s), GFP_KERNEL);
```
quote_metadata: {"id":"AG56-C14","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"453-457","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.453-459"}

## AG56-C15 · FSInfo PAGE_SIZE allocation/read and metadata copy，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::read_fat32_superblock]
```c
	memcpy(fsbi->fat_fsinfo, fsinfo_buf, sizeof(FAT32_FSinfo_s));
	kfree(fsinfo_buf);
```
quote_metadata: {"id":"AG56-C15","blob_short12":"447307aaa4b1","enclosing_symbol":"read_fat32_superblock","definition_range":"421-497","quote_range":"458-459","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.read_fat32_superblock.453-459"}

## AG56-C16 · cluster LBA formula
[VERIFIED mykernel/include/linux/fs/fat32.h::FAT32_clus_to_blknr]
```c
	static inline sector_t FAT32_clus_to_blknr(FAT32_SBinfo_s *fsbi, int clus)
	{
		return ((sector_t)clus - FAT_START_ENT) * fsbi->sector_per_cluster
			+ fsbi->Data_firstsector;
```
quote_metadata: {"id":"AG56-C16","blob_short12":"f03f3c0c1ec7","enclosing_symbol":"FAT32_clus_to_blknr","definition_range":"137-141","quote_range":"137-140","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_clus_to_blknr.137-140"}

## AG56-C17 · PAGE_SIZE用PAGE_SHIFT作为移位量的真实单一宏定义
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
```
quote_metadata: {"id":"AG56-C17","blob_short12":"b10e280f26f0","enclosing_symbol":"PAGE_SIZE","definition_range":"10-10","quote_range":"10-10","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.PAGE_SIZE.10"}

## AG56-C18 · SZ_4K constant
[VERIFIED mykernel/include/linux/kernel/sizes.h::SZ_4K]
```c
	#define SZ_4K			0x00001000
```
quote_metadata: {"id":"AG56-C18","blob_short12":"be89dc5c0d3f","enclosing_symbol":"SZ_4K","definition_range":"23-23","quote_range":"23-23","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.SZ_4K.23-23"}

## AG56-C19 · SZ_64M constant
[VERIFIED mykernel/include/linux/kernel/sizes.h::SZ_64M]
```c
	#define SZ_64M			0x04000000
```
quote_metadata: {"id":"AG56-C19","blob_short12":"be89dc5c0d3f","enclosing_symbol":"SZ_64M","definition_range":"38-38","quote_range":"38-38","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.SZ_64M.38-38"}

## AG56-C20 · ROOTBLK_NVME条件成立时的单一ROOTBLK_TRANSFER宏体
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::ROOTBLK_TRANSFER]
```c
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				NVMe_ops.transfer(0, 0, cmd, blk_idx, count, buffer)
```
quote_metadata: {"id":"AG56-C20","blob_short12":"a97a1162420f","enclosing_symbol":"ROOTBLK_TRANSFER","definition_range":"48-49","quote_range":"48-49","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.ROOTBLK_TRANSFER.NVMe"}

## AG56-C21 · NVMe分支SECT_PER_PG按SZ_4K/bootdisk_LBAsize计算的单一宏
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::SECT_PER_PG]
```c
	#define SECT_PER_PG		(SZ_4K / bootdisk_LBAsize)
```
quote_metadata: {"id":"AG56-C21","blob_short12":"a97a1162420f","enclosing_symbol":"SECT_PER_PG","definition_range":"52-52","quote_range":"52-52","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.SECT_PER_PG.NVMe"}

## AG56-C22 · else分支ATA ROOTBLK_TRANSFER宏体（条件见CONTEXT）
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::ROOTBLK_TRANSFER]
```c
	#define ROOTBLK_TRANSFER(cmd, blk_idx, count, buffer)	\
				ATA_master_ops.transfer(MASTER, SLAVE, cmd, blk_idx, count, buffer)
```
quote_metadata: {"id":"AG56-C22","blob_short12":"a97a1162420f","enclosing_symbol":"ROOTBLK_TRANSFER","definition_range":"55-56","quote_range":"55-56","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.ROOTBLK_TRANSFER.ATA"}

## AG56-C23 · ATA分支SECT_PER_PG固定为8的单一宏
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::SECT_PER_PG]
```c
	#define SECT_PER_PG		8
```
quote_metadata: {"id":"AG56-C23","blob_short12":"a97a1162420f","enclosing_symbol":"SECT_PER_PG","definition_range":"59-59","quote_range":"59-59","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.SECT_PER_PG.ATA"}

## AG56-C24 · long transfer argument types
[VERIFIED mykernel/arch/x86_64/include/obsolete/device.h::blkdev_ops_s]
```c
		long (* transfer)(unsigned controller, unsigned disk,
						long cmd, unsigned long blk_idx,
						long count, unsigned char * buffer);
	} blkdev_ops_s;
```
quote_metadata: {"id":"AG56-C24","blob_short12":"a97a1162420f","enclosing_symbol":"blkdev_ops_s","definition_range":"32-41","quote_range":"38-41","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.blkdev_ops_s.38-41"}

## AG56-C25 · NVMe software initial LBA size512
[VERIFIED mykernel/arch/x86_64/myos/MineOS_NVMe.c::bootdisk_LBAsize]
```c
int bootdisk_LBAsize = 512;	// default block size
```
quote_metadata: {"id":"AG56-C25","blob_short12":"6602a0c42fcf","enclosing_symbol":"bootdisk_LBAsize","definition_range":"44-44","quote_range":"44-44","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.bootdisk_LBAsize.44-44"}

## AG56-C26 · Dword12 receives node.count; hardware interpretation untested
[VERIFIED mykernel/arch/x86_64/myos/MineOS_NVMe.c::NVMe_cmd_out]
```c
			sqptr->PRP_SGL_Entry1 = (ulong)virt_to_phys(node->buffer);
			sqptr->Dword10 = node->LBA & 0xffffffff;		//SLBA-lower = 0
			sqptr->Dword11 = (node->LBA >> 32) & 0xffffffff;	//SLBA-upper = 0
			sqptr->Dword12 = node->count;	//LR=0,FUA=0,PRINFO=0,NLB=1
```
quote_metadata: {"id":"AG56-C26","blob_short12":"6602a0c42fcf","enclosing_symbol":"NVMe_cmd_out","definition_range":"89-136","quote_range":"121-124","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.NVMe_cmd_out.121-124"}

## AG56-C27 · IDE software advances buffer by 512 per count
[VERIFIED mykernel/arch/x86_64/myos/ide.c::IDE_read_handler]
```c
		insw(IDE_PIO_DATA(IdeParam->ATA_controller), (uint16_t *)node->buffer, 256);

	node->count--;
	if (node->count)
		node->buffer += 512;
```
quote_metadata: {"id":"AG56-C27","blob_short12":"39c36c8cc4f9","enclosing_symbol":"IDE_read_handler","definition_range":"161-177","quote_range":"172-176","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.IDE_read_handler.172-176"}

## AG56-C28 · sector_t u64
[VERIFIED mykernel/include/linux/kernel/types.h::sector_t]
```c
		typedef u64		sector_t;
```
quote_metadata: {"id":"AG56-C28","blob_short12":"ef531baa8317","enclosing_symbol":"sector_t","definition_range":"143-143","quote_range":"143-143","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.sector_t.143-143"}

## AG56-C29 · FAT getter PAGE_SIZE and >>7/&0x7f index，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read_FAT_Entry]
```c
u32 FAT32_read_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry)
{
	uint32_t buf[PAGE_SIZE / sizeof(uint32_t)];
	memset(buf, 0, PAGE_SIZE);
	ROOTBLK_TRANSFER(CMD_READ,
```
quote_metadata: {"id":"AG56-C29","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read_FAT_Entry","definition_range":"35-43","quote_range":"35-39","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_read_FAT_Entry.35-42"}

## AG56-C30 · FAT getter PAGE_SIZE and >>7/&0x7f index，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_read_FAT_Entry]
```c
		fsbi->FAT1_firstsector + (fat_entry >> 7),
		SECT_PER_PG, (unchar *)buf);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
```
quote_metadata: {"id":"AG56-C30","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_read_FAT_Entry","definition_range":"35-43","quote_range":"40-42","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_read_FAT_Entry.35-42"}

## AG56-C31 · FAT scan clears first512 and indexes128，连贯片段1/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
	for(i = 0; i < sector_per_fat; i++)
	{
		memset(buf, 0, 512);
		ROOTBLK_TRANSFER(CMD_READ,
			fsbi->FAT1_firstsector + i,
```
quote_metadata: {"id":"AG56-C31","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"74-78","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_find_available_cluster.74-84"}

## AG56-C32 · FAT scan clears first512 and indexes128，连贯片段2/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
			SECT_PER_PG, (unchar *)buf);

		for(j = 0; j < 128; j++)
		{
			if((buf[j] & 0x0fffffff) == 0)
```
quote_metadata: {"id":"AG56-C32","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"79-83","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_find_available_cluster.74-84"}

## AG56-C33 · FAT scan clears first512 and indexes128，连贯片段3/3须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_find_available_cluster]
```c
				return (i << 7) + j;
```
quote_metadata: {"id":"AG56-C33","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_find_available_cluster","definition_range":"65-88","quote_range":"84-84","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_find_available_cluster.74-84"}

## AG56-C34 · FAT setter first512 and count SECT_PER_PG，连贯片段1/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
uint64_t FAT32_write_FAT_Entry(FAT32_SBinfo_s * fsbi, u32 fat_entry, u32 value)
{
	uint32_t buf[PAGE_SIZE / sizeof(uint32_t)];
	int i;

```
quote_metadata: {"id":"AG56-C34","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"90-94","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_write_FAT_Entry.90-105"}

## AG56-C35 · FAT setter first512 and count SECT_PER_PG，连贯片段2/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
	memset(buf,0,512);
	ROOTBLK_TRANSFER(CMD_READ,
		fsbi->FAT1_firstsector + (fat_entry >> 7),
		SECT_PER_PG, (unchar *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);
```
quote_metadata: {"id":"AG56-C35","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"95-99","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_write_FAT_Entry.90-105"}

## AG56-C36 · FAT setter first512 and count SECT_PER_PG，连贯片段3/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c

	for(i = 0; i < fsbi->NumFATs; i++)
		ROOTBLK_TRANSFER(CMD_WRITE,
			fsbi->FAT1_firstsector + fsbi->sector_per_FAT * i + (fat_entry >> 7),
			SECT_PER_PG, (unchar *)buf);
```
quote_metadata: {"id":"AG56-C36","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"100-104","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_write_FAT_Entry.90-105"}

## AG56-C37 · FAT setter first512 and count SECT_PER_PG，连贯片段4/4须合读
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_write_FAT_Entry]
```c
	return 1;	
```
quote_metadata: {"id":"AG56-C37","blob_short12":"447307aaa4b1","enclosing_symbol":"FAT32_write_FAT_Entry","definition_range":"90-106","quote_range":"105-105","source_line_ending":"CRLF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_write_FAT_Entry.90-105"}

## AG56-C38 · release count=bufsize/bytes_per_sector
[VERIFIED mykernel/fs/fat/myos_fat32_iobuf.c::FAT32_iobuf_release]
```c
				if (flags == FAT32_IOBUF_DIRTY)
					ROOTBLK_TRANSFER(CMD_WRITE,
						FAT32_clus_to_blknr(iobuf->fsbi, cluster),
						iobuf->bufsize / iobuf->fsbi->bytes_per_sector, bufp);
				kfree(iobuf->buffers[i]);
```
quote_metadata: {"id":"AG56-C38","blob_short12":"f577b7f65786","enclosing_symbol":"FAT32_iobuf_release","definition_range":"236-269","quote_range":"248-252","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_iobuf_release.248-252"}

## AG56-C39 · newdir buffer bytes_per_sector，连贯片段1/2须合读
[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	FAT32_SBinfo_s *fsbi = (FAT32_SBinfo_s *)dir->i_sb->private_sb_info;
	FAT32_inode_info_s *finode = (FAT32_inode_info_s *)dir->private_idx_info;
	char *buf;
	size_t bufsize = fsbi->bytes_per_sector;
	msdos_dirent_s *de;
```
quote_metadata: {"id":"AG56-C39","blob_short12":"a6ef65d326d2","enclosing_symbol":"FAT32_alloc_new_dir","definition_range":"119-155","quote_range":"123-127","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_alloc_new_dir.123-130"}

## AG56-C40 · newdir buffer bytes_per_sector，连贯片段2/2须合读
[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c

	buf = kzalloc(bufsize, GFP_KERNEL);
	if (buf == NULL)
```
quote_metadata: {"id":"AG56-C40","blob_short12":"a6ef65d326d2","enclosing_symbol":"FAT32_alloc_new_dir","definition_range":"119-155","quote_range":"128-130","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_alloc_new_dir.123-130"}

## AG56-C41 · newdir writes one sector
[VERIFIED mykernel/fs/fat/myos_dir.c::FAT32_alloc_new_dir]
```c
	sector = FAT32_clus_to_blknr(fsbi, cluster);
	if(ROOTBLK_TRANSFER(CMD_WRITE, sector, 1, (unchar *)buf))
	{
		color_printk(RED, BLACK, "FAT32 FS(write) read disk ERROR!!!!!!!!!!\n");
		return -EIO;
```
quote_metadata: {"id":"AG56-C41","blob_short12":"a6ef65d326d2","enclosing_symbol":"FAT32_alloc_new_dir","definition_range":"119-155","quote_range":"146-150","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.FAT32_alloc_new_dir.146-150"}

## AG56-C42 · NVMe WRITE命令给PRP地址/LBA和Dword12软件count赋值，硬件字节含义未核
[VERIFIED mykernel/arch/x86_64/myos/MineOS_NVMe.c::NVMe_cmd_out]
```c
			sqptr->PRP_SGL_Entry1 = (ulong)virt_to_phys(node->buffer);
			sqptr->Dword10 = node->LBA & 0xffffffff;		//SLBA-lower = 0
			sqptr->Dword11 = (node->LBA >> 32) & 0xffffffff;	//SLBA-upper = 0
			sqptr->Dword12 = node->count;	//LR=0,FUA=0,PRINFO=0,DTYPE=0,NLB=1
```
quote_metadata: {"id":"AG56-C42","blob_short12":"6602a0c42fcf","enclosing_symbol":"NVMe_cmd_out","definition_range":"89-136","quote_range":"103-106","source_line_ending":"LF","exact_text":true,"inside_real_definition":true,"claim_supported_in_piece_or_consumer_group":true,"consumer":"C.NVMe_cmd_out.WRITE"}

## 证据限制
CONTEXT（不计入VERIFIED）：obsolete/device.h的`#ifdef ROOTBLK_NVME`（47行）与`#else`（54行）选择不同宏；options_flags.cmake第71行`# -DROOTBLK_NVME`是CMake注释、并非已执行的构建选项，不属于C函数/宏定义体证据。`PAGE_SHIFT=12`来自固定page_const_arch.h第9行的另一宏，单一PAGE_SIZE引文C17只证明它对PAGE_SHIFT的依赖。上述跨定义条件需合读固定源全文，不扩张任一短引文的定义边界。
R35的clusters[0]/ERR_PTR、R38的两FAT对象体系、R55的long结果/void返回均按既有回执消费。无效对象、簇、容量或单位首先STOP，不能凭同类函数名称证明真实物理块语义。
