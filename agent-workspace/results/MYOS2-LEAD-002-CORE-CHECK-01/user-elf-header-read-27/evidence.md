---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-ELF-HEADER-READ-27
record_type: fixed_source_quote_evidence
executor: MYOS2-LEAD-002-CHAT-EXEC-03
date: 2026-11-02
source_pin_short12: a039d9803ade
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第27批固定源码逐字证据

所有锚点来自本轮GitHub连接器打开的同一冻结`time`源码；1–5行原样引文，含空行/原CRLF时计入。证据的源代码存在与活动分支分析，不构成编译后执行、用户字节、磁盘页或ABI成功的认证；标题不是额外的源码引文。默认分支搜索仅用于导航，以下引文未取默认分支。

### EH27-A001 · 格式链表的静态声明
[VERIFIED mykernel/sched/forkexec/exec.c::formats]
```c
static DECLARE_LIST_HDR_S(formats);
```
固定源码行41–41；本次定点读取和逐行复核。

### EH27-A002 · 格式注册的真实链尾写入
[VERIFIED mykernel/sched/forkexec/exec.c::register_binfmt]
```c
void register_binfmt(linux_bfmt_s * fmt)
{
	// write_lock(&binfmt_lock);
	list_header_add_to_tail(&formats, &fmt->lh);
	// write_unlock(&binfmt_lock);
```
固定源码行44–48；本次定点读取和逐行复核。

### EH27-A003 · 文件打开返回检查与后继bprm分配语句
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c
	file = do_open_execat(fd, filename, flags);
	if (IS_ERR(file))
		return ERR_CAST(file);

	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
```
固定源码行1105–1109；本次定点读取和逐行复核。
### EH27-A004 · 文件句柄保存到bprm.file
[VERIFIED mykernel/sched/forkexec/exec.c::alloc_bprm]
```c

	bprm->file = file;

```
固定源码行1114–1116；本次定点读取和逐行复核。

### EH27-A005 · 首块pos=0、清零256字节及kernel_read真实调用
[VERIFIED mykernel/sched/forkexec/exec.c::prepare_binprm]
```c
prepare_binprm(linux_bprm_s *bprm) {
	loff_t pos = 0;

	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, bprm->buf, BINPRM_BUF_SIZE, &pos);
```
固定源码行1161–1165；本次定点读取和逐行复核。

### EH27-A006 · 分派本地retval类型与格式指针
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
search_binary_handler(linux_bprm_s *bprm) {
	mm_s *currmm = current->mm;

	// bool need_retry = IS_ENABLED(CONFIG_MODULES);
	linux_bfmt_s *fmt;
```
固定源码行1177–1181；本次定点读取和逐行复核。

### EH27-A007 · prepare返回只检查负值
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;
```
固定源码行1184–1186；本次定点读取和逐行复核。

### EH27-A008 · 负默认值重设与格式循环
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	retval = -ENOENT;
//  retry:
	// read_lock(&binfmt_lock);
	list_header_for_each_container(fmt, &formats, lh) {
		// if (!try_module_get(fmt->module))
```
固定源码行1192–1196；本次定点读取和逐行复核。

### EH27-A009 · 格式函数指针调用的实际语句
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// 	continue;
		// read_unlock(&binfmt_lock);

		retval = fmt->load_binary(bprm);

```
固定源码行1197–1201；本次定点读取和逐行复核。

### EH27-A010 · 非ENOEXEC提前结束逻辑处于注释
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
		// put_binfmt(fmt);
		// if (bprm->point_of_no_return || (retval != -ENOEXEC)) {
		// 	read_unlock(&binfmt_lock);
		// 	return retval;
		// }
```
固定源码行1203–1207；本次定点读取和逐行复核。

### EH27-A011 · 最终返回循环中保留的retval
[VERIFIED mykernel/sched/forkexec/exec.c::search_binary_handler]
```c
	// }

	return retval;
```
固定源码行1219–1221；本次定点读取和逐行复核。

### EH27-A012 · 真实search_binary_handler调用与负返回消费
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
		retval = search_binary_handler(bprm);
	// }
	if (retval < 0)
		goto out;
```
固定源码行1258–1261；本次定点读取和逐行复核。
### EH27-A013 · point_of_no_return失败后的致命信号是注释
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	//  */
	// if (bprm->point_of_no_return && !fatal_signal_pending(current))
	// 	force_fatal_sig(SIGSEGV);

```
固定源码行1279–1282；本次定点读取和逐行复核。

### EH27-A014 · 错误出口返回retval
[VERIFIED mykernel/sched/forkexec/exec.c::bprm_execve]
```c
	// sched_mm_cid_after_execve(current);
	current->fs->in_exec = 0;
	current->in_execve = 0;

	return retval;
```
固定源码行1283–1287；本次定点读取和逐行复核。

### EH27-A015 · exec调用后的bprm释放与retval返回
[VERIFIED mykernel/sched/forkexec/exec.c::do_execveat_common]
```c
	free_bprm(bprm);

out_ret:
	putname(filename);
	return retval;
```
固定源码行1367–1371；本次定点读取和逐行复核。
### EH27-A016 · 格式表的load_binary函数指针绑定
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_format]
```c
static linux_bfmt_s elf_format = {
	// .module			= THIS_MODULE,
	.load_binary	= load_elf_binary,
	.load_shlib		= load_elf_library,
	// .core_dump		= elf_core_dump,
```
固定源码行83–87；本次定点读取和逐行复核。

### EH27-A017 · initcall初始化格式节点并注册
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::init_elf_binfmt]
```c
int __init init_elf_binfmt(void)
{
	INIT_LIST_S(&elf_format.lh);
	register_binfmt(&elf_format);
	return 0;
```
固定源码行1215–1219；本次定点读取和逐行复核。

### EH27-A018 · 注册函数的返回和initcall声明
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::init_elf_binfmt]
```c
	return 0;
}
core_initcall(init_elf_binfmt);
```
固定源码行1219–1221；本次定点读取和逐行复核。

### EH27-A019 · 主程序头指向bprm.buf
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	int			executable_stack = EXSTACK_DEFAULT;
	elfhdr_s	*elf_ex = (elfhdr_s *)bprm->buf;
	elfhdr_s	*interp_elf_ex = NULL;
	mm_s		*mm;
```
固定源码行635–638；本次定点读取和逐行复核。

### EH27-A020 · retval初为ENOEXEC及ELF魔数判定
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	retval = -ENOEXEC;
	/* First of all, some simple consistency checks */
	if (memcmp(elf_ex->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;
```
固定源码行642–645；本次定点读取和逐行复核。

### EH27-A021 · ET_EXEC/ET_DYN与arch门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (elf_ex->e_type != ET_EXEC && elf_ex->e_type != ET_DYN)
		goto out;
	if (!elf_check_arch(elf_ex))
		goto out;
```
固定源码行647–650；本次定点读取和逐行复核。

### EH27-A022 · fdpic和f_op->mmap存在性门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	if (elf_check_fdpic(elf_ex))
		goto out;
	if (!bprm->file->f_op->mmap)
		goto out;
```
固定源码行651–654；本次定点读取和逐行复核。

### EH27-A023 · load_elf_phdrs的真实调用及NULL结果
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c

	elf_phdata = load_elf_phdrs(elf_ex, bprm->file);
	if (!elf_phdata)
		goto out;
```
固定源码行655–658；本次定点读取和逐行复核。

### EH27-A024 · 成功程序头指针被用于后续遍历
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	elf_ppnt = elf_phdata;
	for (i = 0; i < elf_ex->e_phnum; i++, elf_ppnt++) {
		char *elf_interpreter;
```
固定源码行660–662；本次定点读取和逐行复核。

### EH27-A025 · elf_read按值接收pos并调用kernel_read
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_read]
```c
static int
elf_read(file_s *file, void *buf, size_t len, loff_t pos) {
	ssize_t rv;
	rv = kernel_read(file, buf, len, &pos);
	if (unlikely(rv != len)) {
```
固定源码行427–431；本次定点读取和逐行复核。

### EH27-A026 · 读取长度不符时的原负错与EIO分流
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::elf_read]
```c
	rv = kernel_read(file, buf, len, &pos);
	if (unlikely(rv != len)) {
		return (rv < 0) ? rv : -EIO;
	}
	return 0;
```
固定源码行430–434；本次定点读取和逐行复核。

### EH27-A027 · phdr助手返回指针、retval初值和uint size
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
static elf_phdr_s
*load_elf_phdrs(const elfhdr_s *elf_ex, file_s *elf_file) {
	elf_phdr_s *elf_phdata = NULL;
	int retval = -1;
	uint size;
```
固定源码行466–470；本次定点读取和逐行复核。

### EH27-A028 · e_phentsize字段等于struct大小的门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	if (elf_ex->e_phentsize != sizeof(elf_phdr_s))
		goto out;
```
固定源码行476–477；本次定点读取和逐行复核。

### EH27-A029 · phnum与56字节乘积及两项上界门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	size = sizeof(elf_phdr_s) * elf_ex->e_phnum;
	if (size == 0 || size > 65536 || size > ELF_MIN_ALIGN)
		goto out;
```
固定源码行481–483；本次定点读取和逐行复核。

### EH27-A030 · kmalloc的失败门
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	elf_phdata = kmalloc(size, GFP_KERNEL);
	if (!elf_phdata)
		goto out;
```
固定源码行485–487；本次定点读取和逐行复核。

### EH27-A031 · 独立程序头文件读的调用
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	/* Read in the program headers */
	retval = elf_read(elf_file, elf_phdata, size, elf_ex->e_phoff);
```
固定源码行489–490；本次定点读取和逐行复核。

### EH27-A032 · 失败释放已分配程序头指针并置NULL
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
out:
	if (retval) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
```
固定源码行492–496；本次定点读取和逐行复核。

### EH27-A033 · 失败或成功的最终指针结果
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_phdrs]
```c
	if (retval) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
	return elf_phdata;
```
固定源码行493–497；本次定点读取和逐行复核。

### EH27-A034 · 解释器elf_read复用的单独输入
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		retval = elf_read(interpreter, interp_elf_ex,
				  sizeof(*interp_elf_ex), 0);
		if (retval < 0)
```
固定源码行713–715；本次定点读取和逐行复核。

### EH27-A035 · 解释器程序头表也调用同一助手
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
		interp_elf_phdata = load_elf_phdrs(interp_elf_ex,
						   interpreter);
		if (!interp_elf_phdata)
			goto out_free_dentry;
```
固定源码行757–760；本次定点读取和逐行复核。

### EH27-A036 · bprm的mm和栈指针
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
	typedef struct linux_binprm {
		vma_s			*vma;
		ulong			vma_pages;
		mm_s			*mm;
		ulong			p; /* current top of mem */
```
固定源码行17–21；本次定点读取和逐行复核。

### EH27-A037 · bprm包含实际file对象指针
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		file_s			*executable; /* Executable to pass to the interpreter */
		file_s			*interpreter;
		file_s			*file;
```
固定源码行43–45；本次定点读取和逐行复核。

### EH27-A038 · bprm内部首块固定长度字节数组
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binprm]
```c
		rlimit_s		rlim_stack; /* Saved RLIMIT_STACK used during exec. */

		char		buf[BINPRM_BUF_SIZE];
	} linux_bprm_s;
```
固定源码行59–62；本次定点读取和逐行复核。

### EH27-A039 · 格式对象的load_binary回调类型
[VERIFIED mykernel/include/linux/fs/binfmts.h::linux_binfmt]
```c
	typedef struct linux_binfmt {
		List_s		lh;
		// module_s	*module;
		int			(*load_binary)(linux_bprm_s *);
		int			(*load_shlib)(file_s *);
```
固定源码行94–98；本次定点读取和逐行复核。

### EH27-A040 · 首块缓冲长度宏为256
[VERIFIED mykernel/include/uapi/linux/binfmts.h::BINPRM_BUF_SIZE]
```c
	/* sizeof(linux_binprm->buf) */
	#define BINPRM_BUF_SIZE		256
```
固定源码行18–19；本次定点读取和逐行复核。

### EH27-A041 · ELF魔数常量定义
[VERIFIED mykernel/include/uapi/linux/elf.h::ELFMAG]
```c
	#define ELFMAG0			0x7f	/* EI_MAG */
	#define ELFMAG1			'E'
	#define ELFMAG2			'L'
	#define ELFMAG3			'F'
	#define ELFMAG			"\177ELF"
```
固定源码行355–359；本次定点读取和逐行复核。

### EH27-A042 · 魔数比较中的SELFMAG数值为4
[VERIFIED mykernel/include/uapi/linux/elf.h::SELFMAG]
```c
	#define ELFMAG			"\177ELF"
	#define SELFMAG			4
```
固定源码行359–360；本次定点读取和逐行复核。
### EH27-A043 · Elf64_Ehdr的识别字段、类型和机器字段
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
	typedef struct elf64_hdr
	{
		unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
		Elf64_Half	e_type;
		Elf64_Half	e_machine;
```
固定源码行232–236；本次定点读取和逐行复核。

### EH27-A044 · header版本、入口、phoff及section偏移
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
		Elf64_Word	e_version;
		Elf64_Addr	e_entry;	/* Entry point virtual address */
		Elf64_Off	e_phoff;	/* Program header table file offset */
		Elf64_Off	e_shoff;	/* Section header table file offset */
		Elf64_Word	e_flags;
```
固定源码行237–241；本次定点读取和逐行复核。

### EH27-A045 · header size、program header size/count与section信息
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_hdr]
```c
		Elf64_Half	e_ehsize;
		Elf64_Half	e_phentsize;
		Elf64_Half	e_phnum;
		Elf64_Half	e_shentsize;
		Elf64_Half	e_shnum;
```
固定源码行242–246；本次定点读取和逐行复核。

### EH27-A046 · program header类别和虚拟/文件偏移
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
	typedef struct elf64_phdr
	{
		Elf64_Word	p_type;
		Elf64_Word	p_flags;
		Elf64_Off	p_offset;	/* Segment file offset */
```
固定源码行268–272；本次定点读取和逐行复核。

### EH27-A047 · program header内存/文件长度及对齐字段
[VERIFIED mykernel/include/uapi/linux/elf.h::elf64_phdr]
```c
		Elf64_Addr	p_paddr;	/* Segment physical address */
		Elf64_Xword	p_filesz;	/* Segment size in file */
		Elf64_Xword	p_memsz;	/* Segment size in memory */
		Elf64_Xword	p_align;	/* Segment alignment, file & memory */
	} Elf64_Phdr;
```
固定源码行274–278；本次定点读取和逐行复核。

### EH27-A048 · ELF64 16/32/64位基本字段类型
[VERIFIED mykernel/include/uapi/linux/elf.h::Elf64_Half]
```c
	/* 64-bit ELF base types. */
	typedef __u64		Elf64_Addr;
	typedef __u16		Elf64_Half;
	typedef __s16		Elf64_SHalf;
	typedef __u64		Elf64_Off;
```
固定源码行15–19；本次定点读取和逐行复核。

### EH27-A049 · 可接受的两种主程序ELF类型常量
[VERIFIED mykernel/include/uapi/linux/elf.h::ET_EXEC]
```c
	/* These constants define the different elf file types */
	#define ET_NONE			0
	#define ET_REL			1
	#define ET_EXEC			2
	#define ET_DYN			3
```
固定源码行65–69；本次定点读取和逐行复核。

### EH27-A050 · 本架构校验只检查e_machine
[VERIFIED mykernel/arch/x86_64/include/asm/elf.h::elf_check_arch]
```c
	#define elf_check_arch(x) (				\
				(x)->e_machine == EM_X86_64	\
			)
```
固定源码行64–66；本次定点读取和逐行复核。

### EH27-A051 · 格式表的anchor初始化依赖
[VERIFIED mykernel/lib/list/double_list_macro.h::DECLARE_LIST_HDR_S]
```c
	#define DECLARE_LIST_S(name) \
				List_s name = LIST_INIT(name)
	#define DECLARE_LIST_HDR_S(name) \
				List_hdr_s name = LIST_HEADER_INIT(name)
```
固定源码行14–17；本次定点读取和逐行复核。

### EH27-A052 · 格式枚举首个entry宏及非空约定
[VERIFIED mykernel/lib/list/double_list_macro.h::list_first_entry]
```c
	 * Note, that list is expected to be not empty.
	 */
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)
```
固定源码行35–38；本次定点读取和逐行复核。

### EH27-A053 · 格式循环实际宏指针遍历
[VERIFIED mykernel/lib/list/double_list_macro.h::list_for_each_entry]
```c
	#define list_for_each_entry(pos, head, member)							\
				for (pos = list_first_entry(head, typeof(*pos), member);	\
					!list_entry_is_head(pos, head, member);					\
					pos = list_next_entry(pos, member))
```
固定源码行173–176；本次定点读取和逐行复核。

### EH27-A054 · 格式列表的容器遍历展开
[VERIFIED mykernel/lib/list/double_list_macro.h::list_header_for_each_container]
```c
	#define list_header_for_each_container(pos, header, member)	\
				list_for_each_entry(pos, &((header)->anchor), member)
```
固定源码行346–347；本次定点读取和逐行复核。

### EH27-A055 · 读区间验证函数活动立即返回0
[VERIFIED mykernel/fs/vfs/read_write.c::rw_verify_area]
```c
int rw_verify_area(int read_write, file_s *file, const loff_t *ppos, size_t count)
{
	return 0;

	if (likely((ssize_t) count < 0))
```
固定源码行49–53；本次定点读取和逐行复核。

### EH27-A056 · 内核KVEC目标指针与限长，非用户缓冲
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
ssize_t __kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	kvec_s iov = {
		.iov_base	= buf,
		.iov_len	= min_t(size_t, count, MAX_RW_COUNT),
```
固定源码行96–100；本次定点读取和逐行复核。
### EH27-A057 · 文件读模式FMODE_READ检查与局部-EINVAL返回
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	ssize_t ret;

	if (WARN_ON_ONCE(!(file->f_mode & FMODE_READ)))
		return -EINVAL;
```
固定源码行104–107；本次定点读取和逐行复核。
### EH27-A058 · 初始化kiocb及其局部文件位置
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;


	init_sync_kiocb(&kiocb, file);
```
固定源码行108–112；本次定点读取和逐行复核。

### EH27-A059 · 由pos构造ki_pos及ITER_KVEC
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	init_sync_kiocb(&kiocb, file);
	kiocb.ki_pos = pos ? *pos : 0;
	iov_iter_kvec(&iter, READ, &iov, 1, iov.iov_len);
	if (file->f_op->read_iter)
		ret = file->f_op->read_iter(&kiocb, &iter);
```
固定源码行112–116；本次定点读取和逐行复核。

### EH27-A060 · read_iter优先、read备用及缺失方法-EINVAL
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
		ret = file->f_op->read_iter(&kiocb, &iter);
	else if (file->f_op->read)
		ret = file->f_op->read(file, buf, count, pos);
	else
		ret = -EINVAL;
```
固定源码行116–120；本次定点读取和逐行复核。
### EH27-A061 · read_iter分支的pos更新代码为注释
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	// if (ret > 0) {
	// 	if (pos)
	// 		*pos = kiocb.ki_pos;
	// 	fsnotify_access(file);
	// 	add_rchar(current, ret);
```
固定源码行121–125；本次定点读取和逐行复核。

### EH27-A062 · __kernel_read直接返回读取方法结果
[VERIFIED mykernel/fs/vfs/read_write.c::__kernel_read]
```c
	// 	add_rchar(current, ret);
	// }
	// inc_syscr(current);
	return ret;
}
```
固定源码行125–129；本次定点读取和逐行复核。

### EH27-A063 · kernel_read真实rw_verify_area及调用链
[VERIFIED mykernel/fs/vfs/read_write.c::kernel_read]
```c
ssize_t kernel_read(file_s *file, void *buf, size_t count, loff_t *pos)
{
	ssize_t ret;

	ret = rw_verify_area(READ, file, pos, count);
```
固定源码行131–135；本次定点读取和逐行复核。

### EH27-A064 · 对比用户read另有FMODE_READ和access_ok门
[VERIFIED mykernel/fs/vfs/read_write.c::vfs_read]
```c
	if (!(file->f_mode & FMODE_READ))
		return -EBADF;
	// if (!(file->f_mode & FMODE_CAN_READ))
	// 	return -EINVAL;
	if (unlikely(!access_ok(buf, count)))
```
固定源码行147–151；本次定点读取和逐行复核。

### EH27-A065 · file_operations read/read_iter回调结构
[VERIFIED mykernel/include/linux/fs/fs.h::file_operations]
```c
		loff_t		(*llseek) (file_s *file, loff_t pos, int);
		ssize_t		(*read) (file_s *file, char *buf, size_t size, loff_t *pos);
		ssize_t		(*write) (file_s *file, const char *buf, size_t size, loff_t *pos);
		ssize_t		(*read_iter) (kiocb_s *, iov_iter_s *);
		ssize_t		(*write_iter) (kiocb_s *, iov_iter_s *);
```
固定源码行1862–1866；本次定点读取和逐行复核。

### EH27-A066 · 内核读接口校验的FMODE_READ位
[VERIFIED mykernel/include/linux/fs/fs.h::FMODE_READ]
```c
	/* file is open for reading */
	#define FMODE_READ			((__force fmode_t)0x1)
```
固定源码行60–61；本次定点读取和逐行复核。

### EH27-A067 · kiocb在所选源中初始化file指针
[VERIFIED mykernel/include/linux/fs/fs.h::init_sync_kiocb]
```c
	static inline void init_sync_kiocb(kiocb_s *kiocb, file_s *filp)
	{
		*kiocb = (kiocb_s) {
			.ki_filp = filp,
			// .ki_flags = iocb_flags(filp),
```
固定源码行2120–2124；本次定点读取和逐行复核。

### EH27-A068 · kernel_read kvec上限表达式
[VERIFIED mykernel/include/linux/fs/fs.h::MAX_RW_COUNT]
```c
	#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)
```
固定源码行2423–2423；本次定点读取和逐行复核。

### EH27-A069 · kvec保存目标地址及字节数
[VERIFIED mykernel/include/linux/kernel/uio.h::kvec]
```c
	typedef struct kvec {
		void    *iov_base; /* and that should *never* hold a userland pointer */
		size_t  iov_len;
	} kvec_s;
```
固定源码行15–18；本次定点读取和逐行复核。

### EH27-A070 · 内核方向迭代器创建入口
[VERIFIED mykernel/include/linux/kernel/uio.h::iov_iter_kvec]
```c
	static inline void
	iov_iter_kvec(iov_iter_s *i, uint direction,
			const kvec_s *kvec, ulong nr_segs, size_t count) {

		// WARN_ON(direction & ~(READ | WRITE));
```
固定源码行108–112；本次定点读取和逐行复核。

### EH27-A071 · ITER_KVEC绑定真实kvec和count
[VERIFIED mykernel/include/linux/kernel/uio.h::iov_iter_kvec]
```c
		*i = (iov_iter_s){
			.iter_type		= ITER_KVEC,
			// .copy_mc		= false,
			// .data_source	= direction,
			.kvec			= kvec,
```
固定源码行113–117；本次定点读取和逐行复核。

### EH27-A072 · 文件缓存读调用以iter.count决定零读
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
generic_file_read_iter(kiocb_s *iocb, iov_iter_s *iter)
{
	// size_t count = iov_iter_count(iter);
	size_t count = iter->count;
	ssize_t retval = 0;
```
固定源码行263–267；本次定点读取和逐行复核。

### EH27-A073 · 非零读经simple_filemap_read
[VERIFIED mykernel/mm/vm_map/filemap.c::generic_file_read_iter]
```c
	// 	if (iocb->ki_pos >= i_size_read(inode))
	// 		return retval;
	// }

	return simple_filemap_read(iocb, iter);
```
固定源码行301–305；本次定点读取和逐行复核。

### EH27-A074 · 页缓存read取得kvec目标及局部ki_pos
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
	void *bufp = iter->kvec->iov_base;
	loff_t
		start = iocb->ki_pos,
		end = start + iter->kvec->iov_len;
```
固定源码行210–213；本次定点读取和逐行复核。
### EH27-A075 · 缓存填页的read返回未检查
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		if (*pgcache_ptr == NULL) {
			*pgcache_ptr = alloc_page(GFP_USER | __GFP_ZERO);
			vaddr = (virt_addr_t)page_to_virt(*pgcache_ptr);
			filp->f_op->read(filp, (char *)vaddr, PAGE_SIZE, &temp_pos);
		}
```
固定源码行219–223；本次定点读取和逐行复核。

### EH27-A076 · 跨页读取最后一页实际len计算和else
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		loff_t len = 0;
		if (pgcache_pos + PAGE_SIZE < end)
			len = PAGE_SIZE - inpage_start;
		else
			len = end % PAGE_SIZE - inpage_start;
```
固定源码行228–232；本次定点读取和逐行复核。
### EH27-A077 · 按len拷贝及返回目标指针差
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		memcpy(bufp, (void *)(vaddr + inpage_start), len);
		bufp += len;
	}
	return bufp - iter->kvec->iov_base;
}
```
固定源码行234–238；本次定点读取和逐行复核。

### EH27-A078 · FAT32读与read_iter回调绑定
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
file_ops_s FAT32_file_ops = 
{
	.open			= FAT32_open,
	.read			= FAT32_read,
	.write			= FAT32_write,
```
固定源码行354–358；本次定点读取和逐行复核。

### EH27-A079 · FAT32 mmap和read_iter绑定
[VERIFIED mykernel/fs/fat/myos_fat32.c::FAT32_file_ops]
```c
	.read_iter		= generic_file_read_iter,
	.write_iter		= generic_file_write_iter,
	.mmap			= generic_file_mmap,
	.iterate_shared	= FAT32_getdents64,
```
固定源码行360–363；本次定点读取和逐行复核。

### EH27-A080 · x86-64固定页大小和掩码来源
[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::PAGE_SIZE]
```c
	/* PAGE_SHIFT determines the page size */
	#define PAGE_SHIFT				12
	#define PAGE_SIZE				(_AC(1,UL) << PAGE_SHIFT)
	#define PAGE_MASK				(~(PAGE_SIZE-1))
```
固定源码行8–11；本次定点读取和逐行复核。

### EH27-A081 · uint与ulong基本类型绑定
[VERIFIED mykernel/include/linux/kernel/types.h::uint]
```c
		/* sysv */
		typedef unsigned char		unchar;
		typedef unsigned short		ushort;
		typedef unsigned int		uint;
		typedef unsigned long		ulong;
```
固定源码行101–105；本次定点读取和逐行复核。

### EH27-A082 · 跨页读取inpage_start相对于页首的来源
[VERIFIED mykernel/mm/vm_map/filemap.c::simple_filemap_read]
```c
		loff_t inpage_start = max(pgcache_pos, start) % PAGE_SIZE;
		loff_t len = 0;
		if (pgcache_pos + PAGE_SIZE < end)
```
固定源码行227–229；本次定点读取和逐行复核。


### EH27-A083 · 真实ELF处理器末尾retval=0及统一return
[VERIFIED mykernel/fs/vfs/binfmt_elf.c::load_elf_binary]
```c
	finalize_exec(bprm);
	START_THREAD(elf_ex, regs, elf_entry, bprm->p);
	retval = 0;
out:
	return retval;
```
固定源码行1130–1134；本次定点读取和逐行复核。

### EH27-A084 · prepare_binprm返回类型为int
[VERIFIED mykernel/sched/forkexec/exec.c::prepare_binprm]
```c
static int
prepare_binprm(linux_bprm_s *bprm) {
	loff_t pos = 0;
```
固定源码行1160–1162；本次定点读取和逐行复核。

### EH27-A085 · ELF头及程序头别名绑定为64位版本
[VERIFIED mykernel/include/linux/kernel/elf.h::elfhdr]
```c
	#define elfhdr		elf64_hdr
	#define elf_phdr	elf64_phdr
	#define elf_shdr	elf64_shdr
	#define elf_note	elf64_note
	#define elf_addr_t	Elf64_Off
```
固定源码行43–47；本次定点读取和逐行复核。

### EH27-A086 · elfhdr_s和elf_phdr_s的结构体typedef来源
[VERIFIED mykernel/include/linux/kernel/elf.h::elfhdr_s]
```c
	typedef struct elfhdr		elfhdr_s;
	typedef struct elf_phdr		elf_phdr_s;
	typedef struct elf_shdr		elf_shdr_s;
```
固定源码行52–54；本次定点读取和逐行复核。


## 直接复核范围与非运行边界

- 精确源码源为time的以上15个必要定义文件，加上前置的`mykernel/scripts/options_flags.cmake`和`mykernel/debug/panic.c`金丝雀。H00按固定47任务、26主线R26-S01/S02/S03、历史05/13/25等适用限定消费，未以旧作者引文冒充本批重新读取。
- `prepare_binprm`的首块缓冲区为256字节；按值传递的ELF首块实际读取计数与先行清零分开。`load_elf_phdrs`的phnum、整数大小与文件offset必须逐接口看；`kernel_read`接口包装不能替代底层文件数据有效性证明。
- `list_for_each_entry`是否在空格式链上具备定义良好C对象语义尚不能仅凭最终`retval=-ENOENT`判为已安全处理；多处理器循环是直接函数模型，所选主注册仅elf_format。
- `generic_file_read_iter`内还存在05::UV-06的页边界长度与文件位置限制；本批只追加程序头读取的具体消费与纸面条件，不对其做运行重验。EOF、文件page_array大小、块设备结果、CPU异常/用户态真实恢复等均NOT_RUN。
- 本批未执行仓库脚本、C/ASM、预处理、编译、QEMU、硬件或真实ELF样本，亦无全仓根因去重/完整物理设备读证明；本地/云工具只做文本、有限算术与结构化数据一致性检查。
