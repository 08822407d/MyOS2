---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-KTHREAD-NAME-STORAGE-91
record_type: bounded_static_source_evidence
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-12
produced_by: GPT-6 (current consumer-model family; backend UNATTESTED)
model_per_owner: GPT-6 + Extra
base_snapshot: "time（分支名）"
date: 2026-11-16
read_channel: connector
status: AUTHOR_STATIC_EVIDENCE_PENDING_PRO_REVIEW
runtime_evidence: NOT_RUN
---

# 第91批固定time独立短锚

以下所有短引文均取自同一个固定time源码对象中的一个真实宏、函数、结构或枚举定义；每条1至5个连续实际行，包含空行、制表符或注释时忠实保留。标题只界定源事实；任何函数名/注释不能代替前序对象、类型、并发、编译或运行证明。D00的四个独立宏各自为一锚。

### KN91-E001 | D00独立数组容量定义

[VERIFIED mykernel/lib/idr/idr_const.h::MYOS_IDR_BUF_SIZE]
```c
    #define MYOS_IDR_BUF_SIZE		32768
```

### KN91-E002 | D00独立CHUNK宏

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_CHUNK_SIZE]
```c
	#define IDA_CHUNK_SIZE			MYOS_IDR_BUF_SIZE	/* 128 bytes per chunk */
```

### KN91-E003 | D00独立LONGS宏

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_LONGS]
```c
	#define IDA_BITMAP_LONGS		(IDA_CHUNK_SIZE / sizeof(long))
```

### KN91-E004 | D00独立BITS宏

[VERIFIED mykernel/lib/idr/idr_const.h::IDA_BITMAP_BITS]
```c
	#define IDA_BITMAP_BITS 		(IDA_BITMAP_LONGS * sizeof(long) * 8)
```

### KN91-E005 | worker名字栈容量的枚举值

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_internal_consts]
```c
		WORKER_ID_LEN			= 10 + WQ_NAME_LEN,		/* "kworker/R-" + WQ_NAME_LEN */
```

### KN91-E006 | 短名称容量枚举值

[VERIFIED mykernel/sched/task/task_const.h::TASK_COMM_LEN]
```c
		TASK_COMM_LEN = 32,
```

### KN91-E007 | worker ID字段类型

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker]
```c
		int					id;				/* I: worker id */
```

### KN91-E008 | pool CPU、node与ID字段类型

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::worker_pool]
```c
		int				cpu;			/* I: the associated cpu */
		int				node;			/* I: the associated node ID */
		int				id;				/* I: pool ID */
		uint			flags;			/* L: flags */
```

### KN91-E009 | nice字段的整数类型

[VERIFIED mykernel/kactive/workqueue/workqueue_types.h::workqueue_attrs]
```c
		int nice;
```

### KN91-E010 | 格式化函数的CPU判定

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]
```c
	if (pool) {
		if (pool->cpu >= 0)
			return scnprintf(buf, size, "kworker/%d:%d%s",
					pool->cpu, worker->id,
					pool->attrs->nice < 0  ? "H" : "");
```

### KN91-E011 | CPU worker名字格式

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]
```c
			return scnprintf(buf, size, "kworker/%d:%d%s",
					pool->cpu, worker->id,
					pool->attrs->nice < 0  ? "H" : "");
```

### KN91-E012 | unbound worker名字格式

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]
```c
			return scnprintf(buf, size, "kworker/u%d:%d",
					pool->id, worker->id);
```

### KN91-E013 | pool缺失名字格式

[VERIFIED mykernel/kactive/workqueue/workqueue.c::format_worker_id]
```c
		return scnprintf(buf, size, "kworker/dying");
```

### KN91-E014 | 栈上id_buf容量

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
		char id_buf[WORKER_ID_LEN];
```

### KN91-E015 | 忽略名字格式化返回值再以%s传递

[VERIFIED mykernel/kactive/workqueue/workqueue.c::create_worker]
```c
		format_worker_id(id_buf, sizeof(id_buf), worker, pool);
		worker->task = kthread_create_on_node(simple_worker_thread,
							worker, pool->node, "%s", id_buf);
```

### KN91-E016 | scnprintf的变参包装

[VERIFIED mykernel/lib/printf/vsprintf.c::scnprintf]
```c
	va_start(args, fmt);
	i = vscnprintf(buf, size, fmt, args);
	va_end(args);

```

### KN91-E017 | 实际写入数的上界

[VERIFIED mykernel/lib/printf/vsprintf.c::vscnprintf]
```c
	i = vsnprintf(buf, size, fmt, args);

	if (likely(i < size))
		return i;
	if (size != 0)
```

### KN91-E018 | vscnprintf尺寸零分支

[VERIFIED mykernel/lib/printf/vsprintf.c::vscnprintf]
```c
	if (size != 0)
		return size - 1;
	return 0;
```

### KN91-E019 | vsnprintf拒绝超INT_MAX尺寸

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
	if (WARN_ON_ONCE(size > INT_MAX))
		return 0;
```

### KN91-E020 | vsnprintf无条件计算end指针

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
	str = buf;
	end = buf + size;
```

### KN91-E021 | format_decode真实调用

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
		int read = format_decode(fmt, &spec);

		fmt += read;
```

### KN91-E022 | 字面文本条件拷贝

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
```

### KN91-E023 | 未写长度仍推进str

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
			str += read;
			break;
```

### KN91-E024 | %s实际走string

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
		case FORMAT_TYPE_STR:
			str = string(str, end, va_arg(args, char *), spec);
```

### KN91-E025 | %d整数读取

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
			case FORMAT_TYPE_INT:
				num = (int) va_arg(args, int);
				break;
```

### KN91-E026 | number写入及需求长度

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
			str = number(str, end, num, spec);
		}
```

### KN91-E027 | 终止符位置控制

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
```

### KN91-E028 | 返回需求字符数的源码表达式

[VERIFIED mykernel/lib/printf/vsprintf.c::vsnprintf]
```c
	return str-buf;
```

### KN91-E029 | 格式串%s解析

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]
```c
	case 's':
		spec->type = FORMAT_TYPE_STR;
		return ++fmt - start;
```

### KN91-E030 | 格式串%d解析

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]
```c
	case 'd':
	case 'i':
		spec->flags |= SIGN;
```

### KN91-E031 | 字符串默认精度

[VERIFIED mykernel/lib/printf/vsprintf.c::format_decode]
```c
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (isdigit(*fmt)) {
```

### KN91-E032 | 字符串按NUL或精度读取

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]
```c
	while (lim--) {
		char c = *s++;
		if (!c)
			break;
		if (buf < end)
```

### KN91-E033 | 字符串缓冲条件写入

[VERIFIED mykernel/lib/printf/vsprintf.c::string_nocheck]
```c
		if (buf < end)
			*buf = c;
		++buf;
		++len;
```

### KN91-E034 | 字符串先过指针检查

[VERIFIED mykernel/lib/printf/vsprintf.c::string]
```c
	if (check_pointer(&buf, end, s, spec))
		return buf;

	return string_nocheck(buf, end, s, spec);
```

### KN91-E035 | 十进制数符号前提

[VERIFIED mykernel/lib/printf/vsprintf.c::number]
```c
	if (spec.flags & SIGN) {
		if ((signed long long)num < 0) {
			sign = '-';
			num = -(signed long long)num;
			field_width--;
```

### KN91-E036 | 数值字符条件写入

[VERIFIED mykernel/lib/printf/vsprintf.c::number]
```c
			if (buf < end)
				*buf = ' ';
			++buf;
```

### KN91-E037 | 两遍返回值为unsigned int

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	unsigned int first, second;
	char *p;
	va_list aq;

```

### KN91-E038 | 复制va_list的内建宏

[VERIFIED mykernel/include/linux/lib/stdarg.h::va_copy]
```c
	#define va_copy(d, s)	__builtin_va_copy(d, s)
```

### KN91-E039 | 起始va_list的内建宏

[VERIFIED mykernel/include/linux/lib/stdarg.h::va_start]
```c
	#define va_start(v, l)	__builtin_va_start(v, l)
```

### KN91-E040 | 结束va_list的内建宏

[VERIFIED mykernel/include/linux/lib/stdarg.h::va_end]
```c
	#define va_end(v)		__builtin_va_end(v)
```

### KN91-E041 | 首遍计长与va_end

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	va_copy(aq, ap);
	first = vsnprintf(NULL, 0, fmt, aq);
	va_end(aq);
```

### KN91-E042 | 零容量的第一遍调用

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	first = vsnprintf(NULL, 0, fmt, aq);
	va_end(aq);
```

### KN91-E043 | first加1分配和NULL门

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	p = kmalloc(first+1, gfp);
	if (!p)
		return NULL;
```

### KN91-E044 | 第二遍返回值及注释校验

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	second = vsnprintf(p, first+1, fmt, ap);
	// WARN(first != second, "different return values (%u and %u) from vsnprintf(\"%s\", ...)",
	// 	 first, second, fmt);
```

### KN91-E045 | 第二遍不一致也返回p

[VERIFIED mykernel/lib/printf/kasprintf.c::kvasprintf]
```c
	return p;
```

### KN91-E046 | create信息对象kmalloc

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
		kmalloc(sizeof(kthd_create_info_s), GFP_KERNEL);

	if (!create)
```

### KN91-E047 | 格式化NULL转ERR_PTR

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	create->full_name	= kvasprintf(GFP_KERNEL, namefmt, args);
	if (!create->full_name) {
		task = ERR_PTR(-ENOMEM);
		goto free_create;
	}
```

### KN91-E048 | 等待done后读取result并释放请求

[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
	task = create->result;
free_create:
	kfree(create);
	return task;
```

### KN91-E049 | 变参包装向内部传递

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]
```c
	va_start(args, namefmt);
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
	va_end(args);

```

### KN91-E050 | 公共包装返还任务指针

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_create_on_node]
```c
	return task;
```

### KN91-E051 | 创建请求中的full_name字段

[VERIFIED mykernel/sched/forkexec/kthread.c::kthd_create_info_s]
```c
	char			*full_name;
	int				(*threadfn)(void *data);
```

### KN91-E052 | 私有kthread中的full_name字段

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread_s]
```c
	char			*full_name;
```

### KN91-E053 | WARN后返回worker_private

[VERIFIED mykernel/sched/forkexec/kthread.c::to_kthread]
```c
	WARN_ON(!(k->flags & PF_KTHREAD));
	return k->worker_private;
```

### KN91-E054 | 优先全名缺失则短名回退

[VERIFIED mykernel/sched/forkexec/kthread.c::get_kthread_comm]
```c
	if (!kthread || !kthread->full_name) {
		__get_task_comm(buf, buf_size, tsk);
		return;
	}
```

### KN91-E055 | full_name复制值未返回

[VERIFIED mykernel/sched/forkexec/kthread.c::get_kthread_comm]
```c
	strscpy_pad(buf, kthread->full_name, buf_size);
```

### KN91-E056 | 创建私有结构前检查

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
	if (WARN_ON_ONCE(to_kthread(p)))
		return false;
```

### KN91-E057 | 私有结构堆分配

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
	kthread = kzalloc(sizeof(*kthread), GFP_KERNEL);
	if (!kthread)
		return false;
```

### KN91-E058 | 私有结构挂入task

[VERIFIED mykernel/sched/forkexec/kthread.c::set_kthread_struct]
```c
	p->worker_private = kthread;
	return true;
```

### KN91-E059 | 私有结构NULL释放门

[VERIFIED mykernel/sched/forkexec/kthread.c::free_kthread_struct]
```c
	if (!kthread)
		return;

	k->worker_private = NULL;
```

### KN91-E060 | 先释放full_name再释放私有结构

[VERIFIED mykernel/sched/forkexec/kthread.c::free_kthread_struct]
```c
	kfree(kthread->full_name);
	kfree(kthread);
```

### KN91-E061 | done缺失时释放名字及请求

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	if (!done) {
		kfree(create->full_name);
		kfree(create);
		kthread_exit(-EINTR);
	}
```

### KN91-E062 | full_name指针转移不是深拷贝

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	self->full_name = create->full_name;
	self->threadfn = threadfn;
	self->data = data;
```

### KN91-E063 | publish结果与complete

[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	create->result = current;
	/*
	 * Thread is going to call schedule(), do not preempt it,
	 * or the creator may spend more time in wait_task_inactive().
```

### KN91-E064 | 全名被作为kernel_thread名称传入

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
	pid = kernel_thread(kthread, create, create->full_name,
			CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
```

### KN91-E065 | kernel_thread失败释放名字

[VERIFIED mykernel/sched/forkexec/kthread.c::create_kthread]
```c
		kfree(create->full_name);
		if (!done) {
			kfree(create);
			return;
```

### KN91-E066 | kernel_thread clone传name指针

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_thread]
```c
		.name			= name,
		.kthread		= 1,
```

### KN91-E067 | task.comm的来源和定界拷贝

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	if (args->name)
		strscpy_pad(p->comm, args->name, sizeof(p->comm));
```

### KN91-E068 | 内核线程私有结构初始化

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
		if (!set_kthread_struct(p))
			goto bad_fork_cleanup_delayacct;
```

### KN91-E069 | free_task所读唯一调用被注释

[VERIFIED mykernel/sched/forkexec/fork.c::free_task]
```c
	// 	free_kthread_struct(tsk);
	free_task_struct(tsk);
```

### KN91-E070 | task.comm数组

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		char			comm[TASK_COMM_LEN];
```

### KN91-E071 | 错误或刚好容纳时返回

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	written = strscpy(dest, src, count);
	if (written < 0 || written == count - 1)
		return written;
```

### KN91-E072 | 额外补零及返回

[VERIFIED mykernel/lib/string/string_helpers.c::strscpy_pad]
```c
	memset(dest + written + 1, 0, count - written - 1);

	return written;
```

### KN91-E073 | 0大小及INT_MAX门

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		if (count == 0 || WARN_ON_ONCE(count > INT_MAX))
			return -E2BIG;
```

### KN91-E074 | 按字节读源写目标

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
			c = src[res];
			dest[res] = c;
			if (!c)
				return res;
			res++;
```

### KN91-E075 | 截断时尾部置NUL

[VERIFIED mykernel/lib/string/string.c::strscpy]
```c
		if (res)
			dest[res-1] = '\0';

```

### KN91-E076 | 读task.comm需锁及拷贝

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
	task_lock(tsk);
	/* Always NUL terminated and zero-padded */
	strscpy_pad(buf, tsk->comm, buf_size);
	task_unlock(tsk);
```

### KN91-E077 | 短名读接口返回buf

[VERIFIED mykernel/sched/forkexec/exec.c::__get_task_comm]
```c
	return buf;
```

### KN91-E078 | 任务短名写接口

[VERIFIED mykernel/sched/forkexec/exec.c::__set_task_comm]
```c
	task_lock(tsk);
	// trace_task_rename(tsk, buf);
	strscpy_pad(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
```

### KN91-E079 | task_lock绑定alloc_lock

[VERIFIED mykernel/sched/task/task.h::task_lock]
```c
			spin_lock(&p->alloc_lock);
```

### KN91-E080 | task_unlock释放alloc_lock

[VERIFIED mykernel/sched/task/task.h::task_unlock]
```c
			spin_unlock(&p->alloc_lock);
```

### KN91-E081 | 工作队列名字长度常量

[VERIFIED mykernel/kactive/workqueue/workqueue_const.h::wq_internal_consts]
```c
		WQ_NAME_LEN				= 32,
```


旧第90批冻结证据不改；此处短锚仅支持本91事实表指定条件的静态分析，协议本地全套P9与内核/仓库运行均NOT_RUN。
