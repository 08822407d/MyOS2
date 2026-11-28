---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-INTERPRETER-28
record_type: fixed_source_direct_quote_register
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-02
read_channel: connector
source_ref: time
source_pin_short12: a039d9803ade
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第28批 · 固定time直接引文证据

以下每一锚点都从固定 `time@a039d9803ade` 的实际文件与定义中取1–5行**逐字源码**，空行计入行数。引文只证明已核源码文本的语句与顺序；标题/注释不代替活动生产者、调用、返回或资源释放，不认证运行成功、实际文件/物理页/CPU、底层异常恢复。所有默认分支检索仅用于导航。

### EI28-A001 · 主程序解释器本地指针独立于load_bias
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
load_elf_binary(linux_bprm_s *bprm) {
	file_s		*interpreter = NULL; /* to shut gcc up */
	ulong		load_addr,
				load_bias = 0,
				phdr_addr = 0;
```
固定源第613–617行（行号供复核定位，均含在当前范围）。

### EI28-A002 · 主程序和解释器程序头指针变量
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	elf_phdr_s	*elf_ppnt,
				*elf_phdata,
				*interp_elf_phdata = NULL;
	elf_phdr_s	*elf_property_phdata = NULL;
```
固定源第620–623行（行号供复核定位，均含在当前范围）。

### EI28-A003 · 最终入口与解释器装载基址的本地变量
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	int			retval, i;
	ulong		elf_entry;
	ulong		e_entry;
	ulong		interp_load_addr = 0;
```
固定源第626–629行（行号供复核定位，均含在当前范围）。

### EI28-A004 · 主程序buf头和解释器头指针分离
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	ulong		reloc_func_desc __maybe_unused = 0;
	int			executable_stack = EXSTACK_DEFAULT;
	elfhdr_s	*elf_ex = (elfhdr_s *)bprm->buf;
	elfhdr_s	*interp_elf_ex = NULL;
```
固定源第634–637行（行号供复核定位，均含在当前范围）。

### EI28-A005 · 主程序头表读取先于PT_INTERP
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c

	elf_phdata = load_elf_phdrs(elf_ex, bprm->file);
	if (!elf_phdata)
		goto out;
```
固定源第655–658行（行号供复核定位，均含在当前范围）。

### EI28-A006 · 第一轮程序头遍历及GNU_PROPERTY继续
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	elf_ppnt = elf_phdata;
	for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++) {
		char *elf_interpreter;

		if (elf_ppnt->p_type == PT_GNU_PROPERTY) {
```
固定源第660–664行（行号供复核定位，均含在当前范围）。

### EI28-A007 · PT_INTERP过滤条件
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (elf_ppnt->p_type != PT_INTERP)
			continue;
```
固定源第669–670行（行号供复核定位，均含在当前范围）。

### EI28-A008 · 解释器路径大小上下界与-ENOEXEC
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = -ENOEXEC;
		if (elf_ppnt->p_filesz > PATH_MAX || elf_ppnt->p_filesz < 2)
			goto out_free_ph;
```
固定源第676–678行（行号供复核定位，均含在当前范围）。

### EI28-A009 · 路径kmalloc与-ENOMEM门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = -ENOMEM;
		elf_interpreter = kmalloc(elf_ppnt->p_filesz, GFP_KERNEL);
		if (!elf_interpreter)
			goto out_free_ph;
```
固定源第680–683行（行号供复核定位，均含在当前范围）。

### EI28-A010 · 解释器路径读取及短读错误跳转
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = elf_read(bprm->file, elf_interpreter, elf_ppnt->p_filesz,
					elf_ppnt->p_offset);
		if (retval < 0)
			goto out_free_interp;
```
固定源第685–688行（行号供复核定位，均含在当前范围）。

### EI28-A011 · 路径末字节NUL检验
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		/* make sure path is NULL terminated */
		retval = -ENOEXEC;
		if (elf_interpreter[elf_ppnt->p_filesz - 1] != '\0')
			goto out_free_interp;
```
固定源第689–692行（行号供复核定位，均含在当前范围）。

### EI28-A012 · open_exec、路径释放与错误指针门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		interpreter = open_exec(elf_interpreter);
		kfree(elf_interpreter);
		retval = PTR_ERR(interpreter);
		if (IS_ERR(interpreter))
			goto out_free_ph;
```
固定源第694–698行（行号供复核定位，均含在当前范围）。

### EI28-A013 · 解释器ELF头单独kmalloc和失败出口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		interp_elf_ex = kmalloc(sizeof(*interp_elf_ex), GFP_KERNEL);
		if (!interp_elf_ex) {
			retval = -ENOMEM;
			goto out_free_file;
		}
```
固定源第706–710行（行号供复核定位，均含在当前范围）。

### EI28-A014 · 解释器ELF头固定64字节完整读取
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		/* Get the exec headers */
		retval = elf_read(interpreter, interp_elf_ex,
				  sizeof(*interp_elf_ex), 0);
		if (retval < 0)
			goto out_free_dentry;
```
固定源第712–716行（行号供复核定位，均含在当前范围）。

### EI28-A015 · 首个通过读取的PT_INTERP后break及失败释放
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		break;

out_free_interp:
		kfree(elf_interpreter);
		goto out_free_ph;
```
固定源第718–722行（行号供复核定位，均含在当前范围）。

### EI28-A016 · 首轮解释器选择后重启程序头扫描
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	elf_ppnt = elf_phdata;
	for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++) {
		switch (elf_ppnt->p_type) {
```
固定源第725–727行（行号供复核定位，均含在当前范围）。

### EI28-A017 · 解释器magic校验时默认-ELIBBAD
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* Some simple consistency checks for the interpreter */
	if (interpreter) {
		retval = -ELIBBAD;
		/* Not an ELF interpreter */
		if (memcmp(interp_elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
```
固定源第745–749行（行号供复核定位，均含在当前范围）。

### EI28-A018 · 解释器架构和fdpic校验
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		/* Verify the interpreter has a valid arch */
		if (!elf_check_arch(interp_elf_ex) ||
			elf_check_fdpic(interp_elf_ex))
			goto out_free_dentry;
```
固定源第751–754行（行号供复核定位，均含在当前范围）。

### EI28-A019 · 解释器程序头助手与NULL错误归类
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		/* Load the interpreter program headers */
		interp_elf_phdata = load_elf_phdrs(interp_elf_ex,
						   interpreter);
		if (!interp_elf_phdata)
			goto out_free_dentry;
```
固定源第756–760行（行号供复核定位，均含在当前范围）。

### EI28-A020 · 属性解析为注释不作活动验证
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	/* x86-64 has no elf_arch_state */
	// retval = parse_elf_properties(interpreter ?: bprm->file,
	// 				elf_property_phdata, &arch_state);
	// if (retval)
	// 	goto out_free_dentry;
```
固定源第781–785行（行号供复核定位，均含在当前范围）。

### EI28-A021 · 解释器头已核之后begin_new_exec门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c

	/* Flush all traces of the currently running executable */
	retval = begin_new_exec(bprm);
	if (retval)
		goto out_free_dentry;
```
固定源第797–801行（行号供复核定位，均含在当前范围）。

### EI28-A022 · setup_arg_pages发生在实际映射之前
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	setup_new_exec(bprm);

	/* Do this so that we can load the interpreter, if need be.  We will
	   change some of these later */
	retval = setup_arg_pages(bprm,
```
固定源第813–817行（行号供复核定位，均含在当前范围）。

### EI28-A023 · 主程序ET_DYN且有解释器的load_bias
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			if (interpreter) {
				load_bias = ELF_ET_DYN_BASE;
				// if (current->flags & PF_RANDOMIZE)
				// 	load_bias += arch_mmap_rnd();
				alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);
```
固定源第920–924行（行号供复核定位，均含在当前范围）。

### EI28-A024 · 主程序PT_LOAD实际映射和错误门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		error = elf_load(bprm->file, load_bias + vaddr,
					elf_ppnt, elf_prot, elf_flags, total_size);
		if (BAD_ADDR(error)) {
			retval = IS_ERR_VALUE((void *)error) ?
				PTR_ERR((void*)error) : -EINVAL;
```
固定源第969–973行（行号供复核定位，均含在当前范围）。

### EI28-A025 · 主程序e_entry与phdr_addr先加load_bias
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	e_entry = elf_ex->e_entry + load_bias;
	phdr_addr += load_bias;
	elf_brk += load_bias;
```
固定源第1025–1027行（行号供复核定位，均含在当前范围）。

### EI28-A026 · 主程序brk字段设置后调用load_elf_interp
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	current->mm->start_brk = current->mm->brk = ELF_PAGEALIGN(elf_brk);

	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
```
固定源第1033–1037行（行号供复核定位，均含在当前范围）。

### EI28-A027 · 解释器装载调用与IS_ERR_VALUE判别
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		elf_entry = load_elf_interp(interp_elf_ex, interpreter,
						load_bias, interp_elf_phdata, &arch_state);
		if (!IS_ERR_VALUE(elf_entry)) {
			/*
			 * load_elf_interp() returns relocation
```
固定源第1036–1040行（行号供复核定位，均含在当前范围）。

### EI28-A028 · 解释器偏移赋interp_load_addr及最终入口求和
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
			 * adjustment
			 */
			interp_load_addr = elf_entry;
			elf_entry += interp_elf_ex->e_entry;
		}
```
固定源第1041–1045行（行号供复核定位，均含在当前范围）。

### EI28-A029 · 最终入口BAD_ADDR及错误编码转int
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		if (BAD_ADDR(elf_entry)) {
			retval = IS_ERR_VALUE(elf_entry) ?
					(int)elf_entry : -EINVAL;
			goto out_free_dentry;
```
固定源第1046–1049行（行号供复核定位，均含在当前范围）。

### EI28-A030 · 解释器成功分支fput/kfree三件
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		reloc_func_desc = interp_load_addr;

		fput(interpreter);

		kfree(interp_elf_ex);
```
固定源第1051–1055行（行号供复核定位，均含在当前范围）。

### EI28-A031 · 无解释器分支采用主程序e_entry
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	} else {
		elf_entry = e_entry;
		if (BAD_ADDR(elf_entry)) {
			retval = -EINVAL;
			goto out_free_dentry;
```
固定源第1057–1061行（行号供复核定位，均含在当前范围）。

### EI28-A032 · 主程序头表局部释放及set_binfmt
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	kfree(elf_phdata);

	set_binfmt(&elf_format);
```
固定源第1065–1067行（行号供复核定位，均含在当前范围）。

### EI28-A033 · 栈表接收interp_load_addr与主程序e_entry
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = create_elf_tables(bprm, elf_ex,
				interp_load_addr, e_entry, phdr_addr);
	if (retval < 0)
		goto out;
```
固定源第1075–1078行（行号供复核定位，均含在当前范围）。

### EI28-A034 · START_THREAD最终入口及源码成功return
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```
固定源第1130–1134行（行号供复核定位，均含在当前范围）。

### EI28-A035 · 解释器头及程序头错误出口释放
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
out_free_dentry:
	kfree(interp_elf_ex);
	kfree(interp_elf_phdata);
out_free_file:
	if (interpreter)
```
固定源第1137–1141行（行号供复核定位，均含在当前范围）。

### EI28-A036 · 解释器file条件fput与主程序表kfree
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
out_free_file:
	if (interpreter)
		fput(interpreter);
out_free_ph:
	kfree(elf_phdata);
```
固定源第1140–1144行（行号供复核定位，均含在当前范围）。

### EI28-A037 · AT_BASE与AT_ENTRY分别取不同数值
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_PHDR, phdr_addr);
	NEW_AUX_ENT(AT_PHENT, sizeof(struct elf_phdr));
	NEW_AUX_ENT(AT_PHNUM, exec->e_phnum);
	NEW_AUX_ENT(AT_BASE, interp_load_addr);
	if (bprm->interp_flags & BINPRM_FLAGS_PRESERVE_ARGV0)
```
固定源第212–216行（行号供复核定位，均含在当前范围）。

### EI28-A038 · 解释器装载函数签名和初始状态
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
static ulong
load_elf_interp(elfhdr_s *interp_elf_ex, file_s *interpreter, ulong no_base,
		elf_phdr_s *interp_elf_phdata, arch_elf_state_s *arch_state) {
	elf_phdr_s *eppnt;
	ulong load_addr = 0;
```
固定源第538–542行（行号供复核定位，均含在当前范围）。

### EI28-A039 · 解释器e_type/架构/mmap晚到校验
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	/* First of all, some simple consistency checks */
	if (interp_elf_ex->e_type != ET_EXEC &&
			interp_elf_ex->e_type != ET_DYN)
		goto out;
	if (!elf_check_arch(interp_elf_ex) ||
```
固定源第548–552行（行号供复核定位，均含在当前范围）。

### EI28-A040 · 总跨度为零时- EINVAL编码返回
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	total_size = total_mapping_size(interp_elf_phdata,
						interp_elf_ex->e_phnum);
	if (!total_size) {
		error = -EINVAL;
		goto out;
```
固定源第558–562行（行号供复核定位，均含在当前范围）。

### EI28-A041 · PT_LOAD筛选与MAP_PRIVATE初始标志
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	eppnt = interp_elf_phdata;
	for (i = 0; i < interp_elf_ex->e_phnum; i++, eppnt++) {
		if (eppnt->p_type == PT_LOAD) {
			int elf_type = MAP_PRIVATE;
			int elf_prot = make_prot(eppnt->p_flags,
```
固定源第565–569行（行号供复核定位，均含在当前范围）。

### EI28-A042 · ET_EXEC或后续段固定及no_base负vaddr
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
			ulong vaddr = 0;
			ulong k, map_addr;

			vaddr = eppnt->p_vaddr;
			if (interp_elf_ex->e_type == ET_EXEC || load_addr_set)
```
固定源第571–575行（行号供复核定位，均含在当前范围）。

### EI28-A043 · 共享elf_load调用和total_size消费时间
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
			map_addr = elf_load(interpreter, load_addr + vaddr,
							eppnt, elf_prot, elf_type, total_size);
			total_size = 0;
			error = map_addr;
			if (BAD_ADDR(map_addr))
```
固定源第580–584行（行号供复核定位，均含在当前范围）。

### EI28-A044 · ET_DYN首段成功后load_addr调整和状态位
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
			if (!load_addr_set && interp_elf_ex->e_type == ET_DYN) {
				load_addr = map_addr - ELF_PAGESTART(vaddr);
				load_addr_set = 1;
			}
```
固定源第587–590行（行号供复核定位，均含在当前范围）。

### EI28-A045 · 解释器PT_LOAD后置大小检查与-ENOMEM
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
			k = load_addr + eppnt->p_vaddr;
			if (BAD_ADDR(k) || eppnt->p_filesz > eppnt->p_memsz ||
					eppnt->p_memsz > TASK_SIZE || TASK_SIZE - eppnt->p_memsz < k) {
				error = -ENOMEM;
				goto out;
```
固定源第597–601行（行号供复核定位，均含在当前范围）。

### EI28-A046 · 解释器正常返回load_addr或错误数值
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
		}
	}

	error = load_addr;
out:
```
固定源第603–607行（行号供复核定位，均含在当前范围）。

### EI28-A047 · 总跨度最小最大值初始化
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::total_mapping_size]
```c
static ulong
total_mapping_size(const elf_phdr_s *phdr, int nr) {
	elf_addr_t min_addr = -1;
	elf_addr_t max_addr = 0;
	bool pt_load = false;
```
固定源第411–415行（行号供复核定位，均含在当前范围）。

### EI28-A048 · 总跨度按PT_LOAD内存边界生成
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::total_mapping_size]
```c
	for (int i = 0; i < nr; i++) {
		if (phdr[i].p_type == PT_LOAD) {
			min_addr = min(min_addr, ELF_PAGESTART(phdr[i].p_vaddr));
			max_addr = max(max_addr, phdr[i].p_vaddr + phdr[i].p_memsz);
			pt_load = true;
```
固定源第417–421行（行号供复核定位，均含在当前范围）。

### EI28-A049 · 文件长度为正才调用elf_map
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
	if (eppnt->p_filesz) {
		map_addr = elf_map(filep, addr, eppnt, prot, type, total_size);
		if (BAD_ADDR(map_addr))
			return map_addr;
```
固定源第370–373行（行号供复核定位，均含在当前范围）。

### EI28-A050 · 文件长度为零的独立匿名路线
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
	} else {
		map_addr = zero_start = ELF_PAGESTART(addr);
		zero_end = zero_start + ELF_PAGEOFFSET(eppnt->p_vaddr) +
			eppnt->p_memsz;
	}
```
固定源第387–391行（行号供复核定位，均含在当前范围）。

### EI28-A051 · 匿名扩展调用及结果保存
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_load]
```c
		zero_start = ELF_PAGEALIGN(zero_start);
		zero_end = ELF_PAGEALIGN(zero_end);

		error = vm_brk_flags(zero_start, zero_end - zero_start,
						prot & PROT_EXEC ? VM_EXEC : 0);
```
固定源第400–404行（行号供复核定位，均含在当前范围）。

### EI28-A052 · 整体文件映射与未消费裁剪结果
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_map]
```c
	if (total_size) {
		total_size = ELF_PAGEALIGN(total_size);
		map_addr = vm_mmap(filep, addr, total_size, prot, type, off);
		if (!BAD_ADDR(map_addr))
			vm_munmap(map_addr+size, total_size-size);
```
固定源第341–345行（行号供复核定位，均含在当前范围）。

### EI28-A053 · 读取长度不符与错误来源
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_read]
```c
static int
elf_read(file_s *file, void *buf, size_t len, loff_t pos) {
	ssize_t rv;
	rv = kernel_read(file, buf, len, &pos);
	if (unlikely(rv != len)) {
```
固定源第427–431行（行号供复核定位，均含在当前范围）。

### EI28-A054 · 解释器phdr表的长度和数量门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	if (elf_ex->e_phentsize != sizeof(elf_phdr_s))
		goto out;

	/* Sanity check the number of program headers... */
	/* ...and their total size. */
```
固定源第476–480行（行号供复核定位，均含在当前范围）。

### EI28-A055 · 解释器phdr分配后单独读取
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	elf_phdata = kmalloc(size, GFP_KERNEL);
	if (!elf_phdata)
		goto out;

	/* Read in the program headers */
```
固定源第485–489行（行号供复核定位，均含在当前范围）。

### EI28-A056 · phdr失败返回NULL及清理调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
out:
	if (retval) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
```
固定源第492–496行（行号供复核定位，均含在当前范围）。

### EI28-A057 · TASK_SIZE的BAD_ADDR数值分类
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::BAD_ADDR]
```c
#define BAD_ADDR(x) (unlikely((unsigned long)(x) >= TASK_SIZE))
```
固定源第91–91行（行号供复核定位，均含在当前范围）。

### EI28-A058 · 页起点与对齐宏定义
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::ELF_PAGESTART]
```c
#define ELF_PAGESTART(_v)	((_v) & ~(unsigned long)(ELF_MIN_ALIGN - 1))
#define ELF_PAGEOFFSET(_v)	((_v) & (ELF_MIN_ALIGN - 1))
#define ELF_PAGEALIGN(_v)	(((_v) + ELF_MIN_ALIGN - 1) & ~(ELF_MIN_ALIGN - 1))
```
固定源第79–81行（行号供复核定位，均含在当前范围）。

### EI28-A059 · do_open_execat实际open flags与MAY_EXEC
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
static file_s
*do_open_execat(int fd, filename_s *name, int flags) {
	file_s *file;
	int err;
	open_flags_s open_exec_flags = {
```
固定源第554–558行（行号供复核定位，均含在当前范围）。

### EI28-A060 · open flags限定与no_follow/empty门
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	if ((flags & ~(AT_SYMLINK_NOFOLLOW | AT_EMPTY_PATH)) != 0)
		return ERR_PTR(-EINVAL);
	if (flags & AT_SYMLINK_NOFOLLOW)
		open_exec_flags.lookup_flags &= ~LOOKUP_FOLLOW;
	if (flags & AT_EMPTY_PATH)
```
固定源第565–569行（行号供复核定位，均含在当前范围）。

### EI28-A061 · do_filp_open真实调用和IS_ERR检查
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	file = do_filp_open(fd, name, &open_exec_flags);
	if (IS_ERR(file))
		goto out;
```
固定源第572–574行（行号供复核定位，均含在当前范围）。

### EI28-A062 · 二次文件模式/noexec检查为注释
[VERIFIED mykernel/sched/forkexec/exec.c::do_open_execat]
```c
	// err = -EACCES;
	// if (WARN_ON_ONCE(!S_ISREG(file_inode(file)->i_mode) ||
	// 		path_noexec(&file->f_path)))
	// 	goto exit;

```
固定源第581–585行（行号供复核定位，均含在当前范围）。

### EI28-A063 · getname_kernel返回与ERR_CAST初值
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
file_s *open_exec(const char *name)
{
	filename_s *filename = getname_kernel(name);
	file_s *f = ERR_CAST(filename);

```
固定源第604–608行（行号供复核定位，均含在当前范围）。

### EI28-A064 · 成功filename时打开再putname
[VERIFIED mykernel/sched/forkexec/exec.c::open_exec]
```c
	if (!IS_ERR(filename)) {
		f = do_open_execat(AT_FDCWD, filename, 0);
		putname(filename);
	}
	return f;
```
固定源第609–613行（行号供复核定位，均含在当前范围）。

### EI28-A065 · point_of_no_return发生在解释器映射之前
[VERIFIED mykernel/sched/forkexec/exec.c::begin_new_exec]
```c
	/*
	 * Ensure all future errors are fatal.
	 */
	bprm->point_of_no_return = true;

```
固定源第869–873行（行号供复核定位，均含在当前范围）。

### EI28-A066 · filename_s分配与ERR_PTR(-ENOMEM)
[VERIFIED mykernel/fs/vfs/namei.c::__myos_getname]
```c
static inline filename_s *__myos_getname()
{
	filename_s *name = kmalloc(sizeof(filename_s), GFP_KERNEL);
	if (name == NULL)
		return ERR_PTR(-ENOMEM);
```
固定源第58–62行（行号供复核定位，均含在当前范围）。

### EI28-A067 · getname_kernel没有先核分配错误并计算路径长度
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
// struct filename *getname_kernel(const char __user *filename)
filename_s *getname_kernel(const char *k_filename)
{
	filename_s *name = __myos_getname();

```
固定源第103–107行（行号供复核定位，均含在当前范围）。

### EI28-A068 · getname_kernel在name.len赋值和name.name分配
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	name->len = len;

	name->name = kzalloc(len + 1, GFP_KERNEL);
	if (name->name == NULL)
	{
```
固定源第111–115行（行号供复核定位，均含在当前范围）。

### EI28-A069 · 内核路径按len复制并在异常时释放
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	char * dest = strncpy((void *)name->name, (void *)k_filename, len);
	if (dest != name->name)
	{
		kfree((void *)name->name);
		kfree(name);
```
固定源第120–124行（行号供复核定位，均含在当前范围）。

### EI28-A070 · getname_kernel成功返回filename_s
[VERIFIED mykernel/fs/vfs/namei.c::getname_kernel]
```c
	}

	return name;
}
```
固定源第126–129行（行号供复核定位，均含在当前范围）。

### EI28-A071 · putname实际释放filename字段和对象
[VERIFIED mykernel/fs/vfs/namei.c::putname]
```c
void putname(filename_s *name)
{
	kfree((void *)name->name);
	kfree((void *)name);
```
固定源第133–136行（行号供复核定位，均含在当前范围）。

### EI28-A072 · fput在计数归零时进入释放候选
[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
void fput(file_s *file)
{
	if (atomic_long_dec_and_test(&file->f_count)) {
		task_s *task = current;

```
固定源第108–112行（行号供复核定位，均含在当前范围）。

### EI28-A073 · fput局部kmem_cache_free但不能认证全引用
[VERIFIED mykernel/fs/vfs/file_table.c::fput]
```c
		// if (llist_add(&file->f_u.fu_llist, &delayed_fput_list))
		// 	schedule_delayed_work(&delayed_fput_work, 1);

		kmem_cache_free(filp_cachep, file);
	}
```
固定源第124–128行（行号供复核定位，均含在当前范围）。

### EI28-A074 · ELF64偏移与p_filesz类型为64位
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Xword]
```c
	/* 64-bit ELF base types. */
	typedef __u64		Elf64_Addr;
	typedef __u16		Elf64_Half;
	typedef __s16		Elf64_SHalf;
	typedef __u64		Elf64_Off;
```
固定源第15–19行（行号供复核定位，均含在当前范围）。

### EI28-A075 · 程序头类型PT_INTERP值3
[VERIFIED mykernel/include/uapi/linux/elf.h::PT_INTERP]
```c
	/* These constants are for the segment types stored in the image headers */
	#define PT_NULL			0
	#define PT_LOAD			1
	#define PT_DYNAMIC		2
	#define PT_INTERP		3
```
固定源第25–29行（行号供复核定位，均含在当前范围）。

### EI28-A076 · ELF64解释器头的entry和phoff字段
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
	typedef struct elf64_hdr
	{
		unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
		Elf64_Half	e_type;
		Elf64_Half	e_machine;
```
固定源第232–236行（行号供复核定位，均含在当前范围）。

### EI28-A077 · program header p_filesz/p_vaddr/p_memsz类型
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
	typedef struct elf64_phdr
	{
		Elf64_Word	p_type;
		Elf64_Word	p_flags;
		Elf64_Off	p_offset;	/* Segment file offset */
```
固定源第268–272行（行号供复核定位，均含在当前范围）。

### EI28-A078 · 解释器路径最大值为4096含NUL
[VERIFIED mykernel/include/uapi/linux/limits.h::PATH_MAX]
```c
	#define MAX_CANON		255		/* size of the canonical input queue */
	#define MAX_INPUT		255		/* size of the type-ahead buffer */
	#define NAME_MAX		255		/* # chars in a file name */
	#define PATH_MAX		4096	/* # chars in a path name including nul */
	#define PIPE_BUF		4096	/* # bytes in atomic write to a pipe */
```
固定源第10–14行（行号供复核定位，均含在当前范围）。

### EI28-A079 · 主/解释器头结构体别名为elf64
[VERIFIED mykernel/include/linux/kernel/elf.h::elfhdr_s]
```c
	extern Elf64_Dyn	_DYNAMIC [];
	#define elfhdr		elf64_hdr
	#define elf_phdr	elf64_phdr
	#define elf_shdr	elf64_shdr
	#define elf_note	elf64_note
```
固定源第42–46行（行号供复核定位，均含在当前范围）。

### EI28-A080 · 编码错误区间最大errno4095
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR_VALUE]
```c
	#define MAX_ERRNO	4095

	#ifndef __ASSEMBLY__
	#	define IS_ERR_VALUE(x) (								\
					(size_t)(void *)(x) >= (size_t)-MAX_ERRNO	\
```
固定源第15–19行（行号供复核定位，均含在当前范围）。

### EI28-A081 · PTR_ERR把指针数值转换为long
[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline void *
		ERR_PTR(long error) {
			return (void *) error;
		}

```
固定源第26–30行（行号供复核定位，均含在当前范围）。

### EI28-A082 · IS_ERR复用错误值区间
[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```
固定源第36–39行（行号供复核定位，均含在当前范围）。


### EI28-A083 · error的真实初值为~0UL
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	ulong load_addr = 0;
	int load_addr_set = 0;
	ulong error = ~0UL;
	ulong total_size;
	int i;
```
固定源第542–546行（行号供复核定位，均含在当前范围）。

### EI28-A084 · 晚到架构与文件mmap方法门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
	if (!elf_check_arch(interp_elf_ex) ||
			elf_check_fdpic(interp_elf_ex))
		goto out;
	if (!interpreter->f_op->mmap)
		goto out;
```
固定源第552–556行（行号供复核定位，均含在当前范围）。

### EI28-A085 · no_base/ET_DYN首段的- vaddr及MAP_FIXED判定
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c
			if (interp_elf_ex->e_type == ET_EXEC || load_addr_set)
				elf_type |= MAP_FIXED;
			else if (no_base && interp_elf_ex->e_type == ET_DYN)
				load_addr = -vaddr;
```
固定源第575–578行（行号供复核定位，均含在当前范围）。

### EI28-A086 · 正常返回load_addr与错误返回error的共同出口
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_interp]
```c

	error = load_addr;
out:
	return error;
}
```
固定源第605–609行（行号供复核定位，均含在当前范围）。

### EI28-A087 · 解释器成功分支fput和两个内存buffer kfree
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		fput(interpreter);

		kfree(interp_elf_ex);
		kfree(interp_elf_phdata);
	} else {
```
固定源第1053–1057行（行号供复核定位，均含在当前范围）。

### EI28-A088 · AT_BASE与AT_ENTRY分别写入不同入口信息
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::create_elf_tables]
```c
	NEW_AUX_ENT(AT_BASE, interp_load_addr);
	if (bprm->interp_flags & BINPRM_FLAGS_PRESERVE_ARGV0)
		flags |= AT_FLAGS_PRESERVE_ARGV0;
	NEW_AUX_ENT(AT_FLAGS, flags);
	NEW_AUX_ENT(AT_ENTRY, e_entry);
```
固定源第215–219行（行号供复核定位，均含在当前范围）。

### EI28-A089 · bprm->interpreter字段与bprm->file是两个独立成员
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		file_s			*executable; /* Executable to pass to the interpreter */
		file_s			*interpreter;
		file_s			*file;
```
固定源第43–45行（行号供复核定位，均含在当前范围）。

## 检查方法与未做项

本轮由GitHub连接器分别读取固定time的9个相应文件，另读取配置金丝雀、48号固定任务及27号主线回执。对引用做重复取证/逐字匹配和1–5行限制检查；重要的负面结论以所读完整函数限定，不从注释断言已执行。静态引文、结构/数值算例及真实调用消费为不同证据层。

共享 `elf_read/load_elf_phdrs` 来自第27批有界契约；共享 `elf_load/elf_map` 来自第26批有界契约；`create_elf_tables`及`START_THREAD`来自第25批及R25。路径解析、文件引用及MM生命周期只在本次直接接口止步，不独立认证全部下游回收。R27-S01/S02/S03必须合读；第27批源码证据/图的既有纠正不被原样复现为新的完整分析。

**未做**：C/ASM、仓库脚本、预处理/编译、内核/QEMU/硬件、真实ELF样本、物理文件读取、外部动态链接器/libc、全部路径权限、并发/引用、全仓根因去重及人工/异族模型验收。NOT_RUN不是失败，也不是成功。
