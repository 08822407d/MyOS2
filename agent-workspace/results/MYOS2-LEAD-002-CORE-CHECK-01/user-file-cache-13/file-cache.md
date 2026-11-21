---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-FILE-CACHE-13
record_type: bounded_file_cache_readable_static_analysis
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-01
produced_by: "ChatGPT普通对话（Owner选GPT-6/Extra，backend不可独立认证）"
date: "2026-10-31"
read_channel: connector
source_pin: "time @ a039d9803ade"
execution_base: "16b77f23003f"
instruction_ref: "31-user-file-cache-contract.md @ 58b0bf268726"
prior_review: CORE-USER-MM-FAULT-12-REVIEW-001
prior_status: {SR11-01: CLOSED_IN_BOUNDED_REVIEW, fault12: PASS_PENDING_LOCAL_WITH_REVIEWER_SUPPLEMENTS}
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
runtime_evidence: NOT_RUN
open_questions:
  - "全路径分配、并发、页生命期、文件长度变更和真实磁盘读结果尚未闭合"
  - "本轮未运行内核/ASM/预处理/C/QEMU，不认证实际缓存数据或硬件可用性"
---

# 第13批：文件页缓存的容量、数据可用性和所有权

## 0. 执行结论

**在本批已读的 MyOS2 固定源码中，`page_array[index] != NULL`仅表明槽里有一个非空页指针，不等于已经读到了有效文件数据。** 普通`simple_filemap_read`和缺页回退`simple_filemap_fault`均先把`alloc_page(GFP_USER | __GFP_ZERO)`的结果写入槽，再调用`file->f_op->read`；都不消费该读取的`ssize_t`，也没有显式“加载中/读完/失败”状态。缓存命中的普通read直接`memcpy`，缺页快速映射直接从该指针构造PTE。缺少数据完成标记与引用保护**只说明本组局部接口尚不能证明安全**；并发读到中间状态或磁盘错误的实际发生均NOT_RUN。

其次，数组容量是在一条`__lookup_slow`路径中根据**当时的文件`i_size`**决定，并非每次open都分配。`nrpages`在此是指针数组的**槽位容量**，不是已有文件数据的页数，也不是已成功缓存的页数；实际消费点没有与`nrpages`比较的上界检查。零长度、复用已有dentry、文件长度变化和分配失败须分开讨论。

最后，`file.f_mapping`是指向`inode.i_data`的别名；`get_file/fput`管理file自身的`f_count`。已核缓存命中路径没有调用`get_page`，已核file最终释放路径也没有对`page_array`或其中页做逐项清理。不能由“有文件引用”“PTE已写入”“释放接口存在”推出页被保活、归还或回收正确。

第12批`R12-S01/S02`按主线`CORE-USER-MM-FAULT-12-REVIEW-001`**限定接收**：MF12-F01的稳定VMA成立前提应替换，MF12-E35的匿名PTE分配调用受`!VM_SHARED`门约束，MF12-F06的共享写保护需真实门引文。本批只接收12的文件VM回调端点，不用原先不准确字段证明新边，不返工11/12，也不重签其主线状态。

## 1. C01：容器从哪里来，容量表示什么

### 1.1 inode→mapping→file→VMA/PTE 最小对象关系

`inode_s`内部有嵌入式`i_data`对象；`inode_init_always`通过`mapping=&inode->i_data`，设置`mapping->host=inode`和`inode->i_mapping=mapping`。`myos_do_dentry_open`再将`inode->i_mapping`指针赋给`file.f_mapping`，并把`inode->i_fop`赋给`file.f_op`。同一inode生成的多个file可能共享此mapping，但具体inode/dentry复用仍需实际调用前提，不是已经证明任意两次open都别名同一对象。

**[VERIFIED `mykernel/fs/vfs/inode.c::inode_init_always` · FC13-A16，88–90行]**

```c
	static const file_ops_s no_open_fops = {.open = no_open};
	addr_spc_s *const mapping = &inode->i_data;

```

**[VERIFIED `mykernel/fs/vfs/inode.c::inode_init_always` · FC13-A17，125–127行]**

```c
	mapping->a_ops = &empty_aops;
	mapping->host = inode;
	mapping->flags = 0;
```

**[VERIFIED `mykernel/fs/vfs/inode.c::inode_init_always` · FC13-A18，136–138行]**

```c
	inode->i_private = NULL;
	inode->i_mapping = mapping;
	// INIT_HLIST_HEAD(&inode->i_dentry);	/* buggered by rcu freeing */
```

**[VERIFIED `mykernel/fs/vfs/open.c::myos_do_dentry_open` · FC13-A23，50–52行]**

```c
	f->f_inode = inode;
	f->f_mapping = inode->i_mapping;

```

**[VERIFIED `mykernel/include/linux/fs/fs.h::address_space` · FC13-A07，387–389行]**

```c
		inode_s				*host;
		page_s				**page_array;
		// xarray_s			i_pages;
```


FAT32普通文件的`fat_fill_inode`将目录项size写入`inode->i_size`，`i_fop`绑定`FAT32_file_ops`。`FAT32_file_ops`分别给出`.read=FAT32_read`、`.read_iter=generic_file_read_iter`、`.mmap=generic_file_mmap`；因此普通read_iter、缓存填页所调read和文件mmap回调是**三个不同的绑定**。

**[VERIFIED `mykernel/fs/fat/inode.c::fat_fill_inode` · FC13-A21，174–176行]**

```c
		// MSDOS_I(inode)->i_logstart = MSDOS_I(inode)->i_start;
		inode->i_size	= de->size;
		// inode->i_op = &fat_file_inode_operations;
```

**[VERIFIED `mykernel/fs/fat/inode.c::fat_fill_inode` · FC13-A22，178–180行]**

```c
		inode->i_op		= &vfat_dir_inode_operations;
		inode->i_fop	= &FAT32_file_ops;
		inode->i_mapping->a_ops = &fat_aops;
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_file_ops` · FC13-A29，356–358行]**

```c
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_file_ops` · FC13-A30，359–361行]**

```c
	.llseek			= FAT32_lseek,
	.read_iter		= generic_file_read_iter,
	.write_iter		= generic_file_write_iter,
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_file_ops` · FC13-A31，361–363行]**

```c
	.write_iter		= generic_file_write_iter,
	.mmap			= generic_file_mmap,
	.iterate_shared	= FAT32_getdents64,
```


### 1.2 仅在被核的slow lookup路径创建容量

`open_last_lookups`对不含O_CREAT的普通名字先`__d_lookup`；当其返回NULL才调用`__lookup_slow`。该函数在其inode lookup成功后执行：

**[VERIFIED `mykernel/fs/vfs/namei.c::open_last_lookups` · FC13-A01，673–675行]**

```c
	{
		dentry = __d_lookup(dir, &nd->last);
		if (dentry == NULL)
```

**[VERIFIED `mykernel/fs/vfs/namei.c::open_last_lookups` · FC13-A02，676–678行]**

```c
		{
			dentry = __lookup_slow(&nd->last, dir, nd->flags);
		}
```

**[VERIFIED `mykernel/fs/vfs/namei.c::__lookup_slow` · FC13-A03，333–335行]**

```c
	inode = dentry->d_inode;
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
```

**[VERIFIED `mykernel/fs/vfs/namei.c::__lookup_slow` · FC13-A04，335–337行]**

```c
	if (inode->i_mapping->nrpages > 0)
		inode->i_mapping->page_array =
			kzalloc(inode->i_mapping->nrpages * sizeof(page_s *), GFP_KERNEL);
```


`nrpages=ALIGN(i_size,PAGE_SIZE)>>PAGE_SHIFT`：仅当`i_size`为合法非负且对齐不溢出的前提下，等于`ceil(i_size/PAGE_SIZE)`。当`nrpages>0`，申请`nrpages*sizeof(page_s*)`个字节的清零指针数组；当`nrpages==0`，跳过申请。申请结果**没有检查**，接着继续链入dentry并返回它。既有`__d_lookup`命中则绕过本次slow路径，此外固定`__lookup_hash`也有另一种调用目录inode->lookup的代码，不能把此分配点升级为“所有inode第一次创建必会初始化数组”。

`inode_init_always`没有在已读活动行显式初始化`page_array/nrpages`，FAT32`fat_alloc_inode`直接`kmalloc`其组合对象，而不是先清零整个`vfs_inode`。因此**“零长度文件的page_array一定为NULL”缺少源文保证**；不是已经实测出某个特定非NULL值。旧05::UV-07对空文件位置的结论需要按该前提限定，本批另列文档候选而不改旧件。

**[VERIFIED `mykernel/fs/fat/inode.c::fat_alloc_inode` · FC13-A20，239–241行]**

```c
	msdos_inode_info_s *ei;
	ei = kmalloc(sizeof(msdos_inode_info_s), GFP_KERNEL);
	if (ei == NULL)
```

**[VERIFIED `mykernel/fs/vfs/namei.c::__lookup_slow` · FC13-A05，341–343行]**

```c

	return dentry;
}
```

**[VERIFIED `mykernel/fs/vfs/open.c::myos_do_dentry_open` · FC13-A25，58–60行]**

```c
	{
		f->f_path.dentry->d_inode->i_size = 0;
	}
```


### 1.3 两套索引公式和缺少上界门

普通读：
`start=iocb->ki_pos`，`end=start+iter->kvec->iov_len`，循环从`PAGE_ALIGN_DOWN(start)`至`pgcache_pos<end`，按`pgcache_pos>>PAGE_SHIFT`访问数组。这个读下标没有在`simple_filemap_read`里与`nrpages`比较；本批也未验证`ki_pos`负数或加法溢出有外部保护。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A55，212–214行]**

```c
		start = iocb->ki_pos,
		end = start + iter->kvec->iov_len;
	
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A56，217–219行]**

```c
		virt_addr_t vaddr = 0;
		page_s **pgcache_ptr = &(mapping->page_array[pgcache_pos >> PAGE_SHIFT]);
		if (*pgcache_ptr == NULL) {
```


缺页：
`myos_handle_mm_fault`先以`linear_page_index(vma,address)`得`vmf.pgoff`，具体公式是`((address-vma->vm_start)>>PAGE_SHIFT)+vma->vm_pgoff`。然后`simple_filemap_map_page`用传入`pgoff`查`mapping->page_array[pgoff]`，直接回退的`simple_filemap_fault`用`vmf->pgoff`索引相同数组。即使文件映射允许一个超出实际文件尾的页号，这些消费者里也没有按`nrpages`拦截。**容量、文件的有效字节长度、用户虚拟映射长度不能混作一项**；越界时只给语言层风险，不计算固定errno或实际崩溃。

**[VERIFIED `mykernel/mm/fault/fault.c::myos_handle_mm_fault` · FC13-A69，1318–1320行]**

```c
		.flags		= flags,
		.pgoff		= linear_page_index(vma, address),
		.gfp_mask	= __get_fault_gfp_mask(vma),
```

**[VERIFIED `mykernel/mm/page_alloc/buddy.h::linear_page_index` · FC13-A84，214–216行]**

```c
			// 	return linear_hugepage_index(vma, address);
			pgoff = (address - vma->vm_start) >> PAGE_SHIFT;
			pgoff += vma->vm_pgoff;
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A43，126–128行]**

```c
	loff_t pos = index << PAGE_SHIFT;
	page_s **pgcache_ptr = &mapping->page_array[index];
	BUG_ON(*pgcache_ptr != NULL);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_map_page` · FC13-A49，146–148行]**

```c

	page = mapping->page_array[pgoff];
	if (page != NULL) {
```


普通缓存读的末页复制长度仍按`end%PAGE_SIZE - inpage_start`计算；页对齐结束时可成为0或负值。这个问题已在旧05::UV-06登记，本批只核它与页槽索引/填充的衔接，不重做05值表或整个读迭代器审计。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A60，231–233行]**

```c
		else
			len = end % PAGE_SIZE - inpage_start;

```


## 2. C02：指针发布、填页、读取结果及三种消费者

### 2.1 普通 read_iter 的缓存路径

`generic_file_read_iter`在`iter->count==0`时直接返回0；非零才进入`simple_filemap_read`。读循环在`page_array[page_no]`为空时**先赋`alloc_page`结果，再将该页转换为内核地址、调用`filp->f_op->read`**。随后只是`BUG_ON(*pgcache_ptr == NULL)`、`page_to_virt`和`memcpy`，没有根据后端read的负值、零值、短读或完整读返回重新选择分支。实际返回值来自缓存复制的缓冲区指针差，不是后端read返回值。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_file_read_iter` · FC13-A63，268–270行]**

```c

	if (!count)
		return 0; /* skip atime */
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_file_read_iter` · FC13-A64，304–306行]**

```c

	return simple_filemap_read(iocb, iter);
}
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A57，219–221行]**

```c
		if (*pgcache_ptr == NULL) {
			*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
			vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A58，221–223行]**

```c
			vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
			filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &temp_pos);
		}
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A59，223–225行]**

```c
		}
		BUG_ON(*pgcache_ptr == NULL);
		vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A61，233–235行]**

```c

		memcpy(bufp, (void *)(vaddr + inpage_start), len);
		bufp += len;
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_read` · FC13-A62，236–238行]**

```c
	}
	return bufp - iter->kvec->iov_base;
}
```


从静态语句看，非空槽使此函数跳过后端读；**这只是指针复用判据**。若页刚被另一执行者放进槽但读尚未完成，当前所读函数没有完成状态可辨认；若前次read只返回短数据或失败，此函数也没有差错状态可查。这里不是已证明实际并发交叉发生，或已验证读出0字节/数据损坏。首个空槽分配若失败，代码先调用`page_to_virt(*pgcache_ptr)`，其后才有`BUG_ON`；不能把后置断言当成充分的空指针保护。

### 2.2 缺页快速cache-hit和fallback区别

`do_read_fault`先尝试`do_fault_around→vm_ops.map_single_page`；在**ret==0**时立即返回，否则进入`__do_fault`。对已核FAT32 generic映射，`generic_file_mmap`安装`generic_file_vm_ops`，其中`.map_single_page=simple_filemap_map_page`，`.fault=simple_filemap_fault`。这是调用点与绑定的组合，**不是任何文件VMA均有该函数指针目标**。

**[VERIFIED `mykernel/mm/fault/fault.c::do_read_fault` · FC13-A70，1087–1089行]**

```c
	// if (should_fault_around(vmf)) {
		ret = do_fault_around(vmf);
		if (ret == 0)
```

**[VERIFIED `mykernel/mm/fault/fault.c::do_read_fault` · FC13-A71，1088–1090行]**

```c
		ret = do_fault_around(vmf);
		if (ret == 0)
			return ret;
```

**[VERIFIED `mykernel/mm/fault/fault.c::do_fault_around` · FC13-A73，1069–1071行]**

```c
	// 		vmf->pgoff + to_pte - pte_off);
	ret = vmf->vma->vm_ops->map_single_page(vmf,
			vmf->pgoff + from_pte - pte_off);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_file_vm_ops` · FC13-A52，184–186行]**

```c
const vm_ops_s generic_file_vm_ops = {
	.fault				= simple_filemap_fault,
	.map_single_page	= simple_filemap_map_page,
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_file_mmap` · FC13-A68，417–419行]**

```c
	// file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	return 0;
```


快速map时`simple_filemap_map_page`仅检查`page_array[pgoff]`是否非NULL；命中则立即构造PTE并返回0，空槽返回`VM_FAULT_NOPAGE`。它**既没有验证数据填页完成，也没有通过`get_page`为PTE增加页引用**。这里的0只代表该分支的返回，不能证明后来硬件读取正确或完成12的异常恢复。返回NOPAGE会使`do_read_fault`走`__do_fault`，进一步间接调用`vma->vm_ops->fault`，仍须以具体generic绑定为前提。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_map_page` · FC13-A49，146–148行]**

```c

	page = mapping->page_array[pgoff];
	if (page != NULL) {
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_map_page` · FC13-A50，148–150行]**

```c
	if (page != NULL) {
		*vmf->pte_ptr = mk_pte(page,
			__pg(_PAGE_PRESENT | _PAGE_USER | _PAGE_PAT));
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_map_page` · FC13-A51，153–155行]**

```c
	else
		return VM_FAULT_NOPAGE;
}
```

**[VERIFIED `mykernel/mm/fault/fault.c::do_read_fault` · FC13-A72，1092–1094行]**

```c

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
```

**[VERIFIED `mykernel/mm/fault/fault.c::__do_fault` · FC13-A74，900–902行]**

```c

	ret = vma->vm_ops->fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
```


### 2.3 缺页fallback的顺序与重复槽

`simple_filemap_fault`先核`vmf->vma->vm_file`和`file->f_mapping`是否`IS_ERR_OR_NULL`，不检查`mapping->page_array`有效性或`index<nrpages`。它直接取得`&page_array[index]`，执行`BUG_ON(*slot!=NULL)`，然后在槽里写入新页指针，调用`file->f_op->read`，赋`vmf->page`，返回0。既没有`alloc_page`结果的活动检查，也没有读取`ssize_t`结果的判断。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A41，118–120行]**

```c
	file_s *file = vmf->vma->vm_file;
	if (IS_ERR_OR_NULL(file))
		return VM_FAULT_ERROR;
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A42，121–123行]**

```c
	addr_spc_s *mapping = file->f_mapping;
	if (IS_ERR_OR_NULL(mapping))
		return VM_FAULT_ERROR;
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A43，126–128行]**

```c
	loff_t pos = index << PAGE_SHIFT;
	page_s **pgcache_ptr = &mapping->page_array[index];
	BUG_ON(*pgcache_ptr != NULL);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A44，127–129行]**

```c
	page_s **pgcache_ptr = &mapping->page_array[index];
	BUG_ON(*pgcache_ptr != NULL);
	*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A46，131–133行]**

```c

	file->f_op->read(file, (char *)vaddr, PAGE_SIZE, &pos);
	vmf->page = *pgcache_ptr;
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A48，134–136行]**

```c

	return 0;
}
```


这表明快速map先命中时不会重新填页；但如果**别的合法上层调用**不经过快速命中检查而直接进入此fault，例如private file write fault经`do_cow_fault→__do_fault`，当缓存槽原本已有页时，才可能进入这条重复槽`BUG_ON`条件。此机制与之前“非空槽代表已准备好”是**两个分立的契约问题**。不假定`BUG_ON`的实际机器处理，也不将所有私有写缺页都判为必然失败。

**[VERIFIED `mykernel/mm/fault/fault.c::do_cow_fault` · FC13-A75，1115–1117行]**

```c

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::simple_filemap_fault` · FC13-A45，128–130行]**

```c
	BUG_ON(*pgcache_ptr != NULL);
	*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
	virt_addr_t vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
```

**[VERIFIED `mykernel/mm/fault/fault.c::__do_fault` · FC13-A74，900–902行]**

```c

	ret = vma->vm_ops->fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
```

**[VERIFIED `mykernel/mm/fault/fault.c::do_cow_fault` · FC13-A88，1124–1126行]**

```c

	ret |= finish_fault(vmf);
	// unlock_page(vmf->page);
```


### 2.4 FAT32实际后端与读结果

三项绑定证据是：`file.f_op`来自`inode.i_fop`（由FAT32 inode绑定），`FAT32_file_ops.read=FAT32_read`，后者的签名`ssize_t FAT32_read(file_s*,char*,size_t,loff_t*)`与调用形参匹配。FAT32_read根据`*position`和`inode->i_size`把`count`截短到文件尾，随后有条件通过`ROOTBLK_TRANSFER`读取簇，并按`buf`地址进行内存复制。它有首簇无效时的`-EFAULT`、块读接口返回非零时的`-EIO`及正常`count`/局部0路径；但**两种缓存填页调用均忽略该返回值**。

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A32，113–115行]**

```c

ssize_t FAT32_read(file_s *filp, char *buf, size_t count, loff_t *position)
{
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A34，128–130行]**

```c
	if(!cluster)
		return -EFAULT;
	for(i = 0; i < index; i++)
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A35，133–135行]**

```c
	if(*position + count > filp->f_path.dentry->d_inode->i_size)
		index = count = filp->f_path.dentry->d_inode->i_size - *position;
	else
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A36，141–143行]**

```c
		sector = fsbi->Data_firstsector + (cluster - 2) * fsbi->sector_per_cluster;
		if(ROOTBLK_TRANSFER(CMD_READ, sector,
				fsbi->sector_per_cluster, (unchar *)buffer))
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A37，145–147行]**

```c
			color_printk(RED, BLACK, "FAT32 FS(read) read disk ERROR!!!!!!!!!!\n");
			ret_val = -EIO;
			break;
```

**[VERIFIED `mykernel/fs/fat/myos_fat32.c::FAT32_read` · FC13-A40，164–166行]**

```c
	if(!index)
		ret_val = count;
	return ret_val;
```


这里不要把`alloc_page(...|__GFP_ZERO)`解释成磁盘尾部数据一定有效：它只是申请一页清零内存的意图，读失败或EOF短读后的残余字节并非从文件确认的内容。FAT32_read自身`kmalloc`的失败检查、设备传输返回真实性及后端同步属于05已记录的问题或本批未展开边界，不重核ATA/中断/完成队列。文件长度零时`count`可能被截至0，但该读函数的`do`循环仍出现在条件判断之后；是否可按有效首簇及配置走到返回0，保留对应前提。

### 2.5 写路径也是相同容器的消费者（限定）

`generic_perform_write`按相同`page_array[pgcache_pos>>PAGE_SHIFT]`找到页：空槽时也先分配、调用read填入旧数据，然后`memcpy`覆盖用户写入字节。其所读函数体内没有显示为缓存槽设置“脏/就绪/失败”状态或保证同步写回的活动接口。只记录这一同一容器的副作用，不重审`FAT32_write`、writeback策略或持久化保证。

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_perform_write` · FC13-A65，323–325行]**

```c
		virt_addr_t vaddr = 0;
		page_s **pgcache_ptr = &(mapping->page_array[pgcache_pos >> PAGE_SHIFT]);
		if (*pgcache_ptr == NULL) {
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_perform_write` · FC13-A66，327–329行]**

```c
			vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
			filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &temp_pos);
		}
```

**[VERIFIED `mykernel/mm/vm_map/filemap.c::generic_perform_write` · FC13-A67，339–341行]**

```c

		memcpy((void *)(vaddr + inpage_start), bufp, len);
		bufp += len;
```


## 3. C03：引用增减、释放与重复使用

### 3.1 所有权表（与facts中FC13-O01～O10逐项对应）

| 对象或持有方 | 在所读路径中的真实操作 | 不得假定 |
|---|---|---|
| `inode.i_data`/ `i_mapping` | mapping嵌于inode；i_mapping指向它，host反向指inode | 不存在独立mapping引用计数自动保护所有页 |
| `file.f_mapping` | 从inode复制指针；file本身有单独f_count | 增加file引用自动增加mapping/page引用 |
| `VMA.vm_file` | 文件mmap时`get_file(file)`，增加file.f_count | 这是对file的引用，不是对cache页的pin |
| `page_array`数组 | slow lookup中按nrpages `kzalloc`，非空槽存page_s指针 | 对每页有独立就绪/失败计数或自动释放 |
| cache-hit page_s | 普通读用`page_to_virt`，map_page可把PTE指向它 | `page_folio`/PTE指针转换自动调用`get_page` |
| private COW folio | `do_cow_fault`另行预分配cow页并可能在错误路径`folio_put` | cow页是page_array槽页，或其释放必清除cache槽 |
| file关闭 | `filp_close→fput`，计数为零则`kmem_cache_free(filp_cachep,file)` | 释放file的同时会清理inode.i_data/page_array或归还每页 |
| inode `iput` | 所读函数体对非NULL inode的清理核心为注释 | 因存在名称为iput的API就已实现inode/cache销毁 |

**[VERIFIED `mykernel/include/linux/fs/fs.h::get_file` · FC13-A12，905–907行]**

```c
	static inline file_s *get_file(file_s *f) {
		atomic_long_inc(&f->f_count);
		return f;
```

**[VERIFIED `mykernel/mm/vm_map/mmap.c::simple_mmap_region` · FC13-A78，691–693行]**

```c
	if (file) {
		vma->vm_file = get_file(file);
		error = call_mmap(file, vma);
```

**[VERIFIED `mykernel/fs/vfs/file_table.c::fput` · FC13-A27，109–111行]**

```c
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;
```

**[VERIFIED `mykernel/fs/vfs/file_table.c::fput` · FC13-A28，126–128行]**

```c

		kmem_cache_free(filp_cachep, file);
	}
```

**[VERIFIED `mykernel/mm/page_alloc/buddy.h::folio_put` · FC13-A83，180–182行]**

```c
			// Equal to do folio_ref_dec()
			if (atomic_dec_and_test(&folio->page._refcount))
				__folio_put(folio);
```


`get_page/put_page/folio_put`接口本身存在，但`simple_filemap_read`、`simple_filemap_map_page`及`simple_filemap_fault`所读活动段并未使用`get_page`作为缓存命中引用，`fput`也没有沿`page_array`作迭代归还。不能因`folio_put`存在就推断cache页会被正常释放；`__folio_put`在当前所读正文里真正释放页的调用还处于注释状态。这个证据是一组**局部生命周期断点**，不代表全仓所有异步回收/其它inode释放路径都被排除。

**[VERIFIED `mykernel/mm/page_alloc/buddy.h::get_page` · FC13-A81，186–188行]**

```c
		void
		get_page(page_s *page) {
			folio_get(page_folio(page));
```

**[VERIFIED `mykernel/mm/page_alloc/buddy.h::put_page` · FC13-A82，198–200行]**

```c
			// 	return;
			folio_put(page_folio(page));
		}
```

**[VERIFIED `mykernel/mm/page_alloc/buddy.c::__folio_put` · FC13-A85，638–640行]**

```c
	// else
	// 	__folio_put_small(folio);
}
```


应特别区分：`sys_close`在05另有不经f_count直接释放的旧问题，而这里追的是`filp_close`和`fput`。二者不自动相等；不复用旧05::UV-10的修复权限。

### 3.2 何时失去容量、引用或数据保证

文件后续被截短时，`myos_do_dentry_open`中的O_TRUNC条件可以写`i_size=0`，却没有在同一函数重算nrpages、重新分配或清空`page_array`。旧数组可能与新长度不一致，反过来文件增长也需要独立容量策略（当前未穷尽增长入口）。后者不是证明所有截短必触发错误；是静态未对齐的状态边界。

**[VERIFIED `mykernel/fs/vfs/open.c::myos_do_dentry_open` · FC13-A25，58–60行]**

```c
	{
		f->f_path.dentry->d_inode->i_size = 0;
	}
```

**[VERIFIED `mykernel/fs/vfs/namei.c::__lookup_slow` · FC13-A03，333–335行]**

```c
	inode = dentry->d_inode;
	inode->i_mapping->nrpages = ALIGN(inode->i_size, PAGE_SIZE) >> PAGE_SHIFT;
	if (inode->i_mapping->nrpages > 0)
```


`fput`只处理file对象本身；`iput`所读正文也没有对应page_array清理。用户解除映射/退出可能经过其它路径，不能把“未在本批选定释放路径里看到”写成“全系统永不释放”。具体PTE反向映射、folio/LRU引用与并发truncate机制均作为未核前提，阶段2不能验收实际释放正确性。

## 4. 有界输入/结果矩阵（实际分支，不凑笛卡尔积）

| 输入或状态（facts::M01–M16） | 已证的源码选择 | 关键未知 |
|---|---|---|
| zero-size + `iter->count=0` | `generic_file_read_iter`直接返回0 | 不代表空文件array已初始化 |
| zero-size + positive read/fault | slow路径无array分配，但读取端仍可能索引 | 指针有效性、实际结果未知 |
| 首/中/末页合法index且slot NULL | 发布新页指针，调用read后尝试消费 | 读回完整度、就绪与并发保护无证 |
| index >= nrpages | 三类消费者存在无上界的数组访问 | 不给具体崩溃/errno |
| dentry命中/首次slow路径 | 只有slow分支分配新array | 其它创建路径与重用未穷尽 |
| kzalloc/alloc_page失败 | 没有相应的当前局部成功校验 | 后续代码语言前提可能不成立 |
| FAT32后端全读、短读、0、负值 | 缓存填页忽略ssize_t，仍保留page指针 | 不宣称文件内容完整/必是0 |
| 普通缓存hit | 跳过后端read，直接memcpy | 页是否已经准备好与引用有效 |
| mmap缓存hit / miss | hit写PTE并返回0；miss返回NOPAGE后有条件fall back | 指针合法、真实VM绑定、硬件重试 |
| 直接fault且slot占用 | `BUG_ON`检查，不走缓存hit恢复 | 不宣称全输入触发或运行已复现 |
| end落在页边界 | `end%PAGE_SIZE - inpage_start`可以<=0 | 旧05::UV-06的转换/内存行为不重测 |
| O_TRUNC/最终file ref减少 | i_size变更、fput可释放file对象 | array/page重建与销毁链未闭合 |

## 5. 旧项、来源与主动反证

本轮普通缺陷/文档表达冲突分离登记在`deferred-findings.yaml`。05::UV-07（容量/越界）、05::UV-08（文件fault不消费读结果）、05::UV-06（页边界长度/读位置）、05::UV-10（file/fput）继续为原编号；本包只追加证据，不改其主线审查状态。将05关于“零文件的page_array必是NULL”与“file/mapping有效即可正常返回0”的过强消费列为文档候选，由C03裁定是否需追加限定，不改05原文。

**主动反证：** 非NULL槽不证明数据就绪；后端返回0不证明请求的整页读取成功；缓存hit未做index上界检查；private COW可绕过fast-map检查直接进fault；file引用不等于page引用；有`folio_put`不等于当前缓存槽被清理。上述证据均来自固定`time`的实际源码段和条件，编译宏、线程交错、硬盘故障、运行结果NOT_RUN。

本报告不预处理、不编译、不运行C/ASM/QEMU、不能认证实际CPU访问恢复、块I/O、文件缓存一致性或真实释放。阶段2交付状态`READY_FOR_MAINLINE_REVIEW`，C03验收上限`PASS_PENDING_LOCAL`。
