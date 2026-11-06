---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-DEVTMPFS-SETUP-48
record_type: directly_reconfirmed_fixed_time_definitions
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-05
source_pin_short12: a039d9803ade
date: 2026-11-06
read_channel: connector
runtime_evidence: NOT_RUN
---

# 第48批：创建结果、线程err和完成通知的直接证据

引文均来自固定time@a039d9803ade的真实函数、单一宏、对象或结构体定义，每段1–5行。两轮独立GitHub连接器全文读取同一固定源码、对照真实行号与函数/宏归属；重叠短片段只帮助解释局部控制顺序，不等于同样数量的独立发现。宏中的注释说明或外层extern仅作上下文，不充当活动定义体。无预处理、编译或机器执行。

### DS48-A001 — 初始化入口和两个局部变量（409–413行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
int devtmpfs_init(void)
{
	char opts[] = "mode=0755";
	int err;
	INIT_LIST_HEADER_S(&internal_fs_type.fs_supers);
```

### DS48-A002 — 内部超块链表初始化与首次挂载调用（413–415行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	INIT_LIST_HEADER_S(&internal_fs_type.fs_supers);

	mnt = vfs_kern_mount(&internal_fs_type, 0, "devtmpfs", opts);
```

### DS48-A003 — 挂载错误指针判断与立即返回（415–419行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	mnt = vfs_kern_mount(&internal_fs_type, 0, "devtmpfs", opts);
	if (IS_ERR(mnt)) {
		pr_err("devtmpfs: unable to create devtmpfs %ld\n", PTR_ERR(mnt));
		return PTR_ERR(mnt);
	}
```

### DS48-A004 — 注册返回err赋值和直接早退（421–425行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	err = register_filesystem(&dev_fs_type);
	if (err) {
		pr_err("devtmpfs: unable to register devtmpfs type %d\n", err);
		return err;
	}
```

### DS48-A005 — 线程创建结果与非ERR等待门（427–430行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	if (!IS_ERR(thread)) {
		wait_for_completion(&setup_done);
	} else {
```

### DS48-A006 — 创建编码错误门对err和thread的赋值（430–433行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	} else {
		err = PTR_ERR(thread);
		thread = NULL;
	}
```

### DS48-A007 — 启动等待对象为setup_done（429–429行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
		wait_for_completion(&setup_done);
```

### DS48-A008 — 真正的while(err)空循环（435–435行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	while (err);
```

### DS48-A009 — 空循环之后的注销与错误返回（437–441行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	if (err) {
		pr_err("devtmpfs: unable to create devtmpfs %i\n", err);
		unregister_filesystem(&dev_fs_type);
		return err;
	}
```

### DS48-A010 — 跳过错误门后的正常报告与返回（443–444行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	pr_info("devtmpfs: initialized\n");
	return 0;
```

### DS48-A011 — 工作函数void指针参数与独立局部int err（384–388行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
static int devtmpfsd(void *p)
{
	// int err = devtmpfs_setup(&p);
	// {
		int err = 0;
```

### DS48-A012 — 工作函数内部init_mount错误门（390–392行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
```

### DS48-A013 — 两个目录切换的结果被丢弃（393–394行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
```

### DS48-A014 — 完成通知到工作循环和out返回（399–403行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
	complete(&setup_done);
	devtmpfs_work_loop();
out:
	return err;
}
```

### DS48-A015 — 工作函数out直接return局部err（401–402行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
out:
	return err;
```

### DS48-A016 — 工作循环返回类型及外层无限循环（355–357行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
static void devtmpfs_work_loop(void)
{
	while (1) {
```

### DS48-A017 — 请求级err字段和独立req.done通知（365–368行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
				req->err = handle(req->name, req->mode,
						req->uid, req->gid, req->dev);
				complete(&req->done);
				kfree(req);
```

### DS48-A018 — 循环尾部schedule没有break或return（373–376行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
		__set_current_state(TASK_INTERRUPTIBLE);
		spin_unlock(&req_lock);
		schedule();
	}
```

### DS48-A019 — 独立setup_done静态DECLARE_COMPLETION对象（344–344行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::setup_done]
```c
static __initdata DECLARE_COMPLETION(setup_done);
```

### DS48-A020 — 保存的静态task指针（41–41行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::thread]
```c
static task_s *thread;
```

### DS48-A021 — req结构内自身done与err而非setup_done（47–51行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::req_s]
```c
typedef struct req {
	req_s			*next;
	completion_s	done;
	int				err;
	const char		*name;
```

### DS48-A022 — req.done的运行时初始化请求（92–95行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
static int devtmpfs_submit_req(req_s *req, const char *tmp)
{
	init_completion(&req->done);

```

### DS48-A023 — 请求完成的等待对象是req.done（101–103行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	wake_up_process(thread);
	wait_for_completion(&req->done);

```

### DS48-A024 — 请求级结果来自req.err（104–106行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	kfree((void *)tmp);

	return req->err;
```

### DS48-A025 — create单一长宏参数和NUMA转交（16–18行）

[VERIFIED mykernel/sched/forkexec/forkexec_macro.h::kthread_create]
```c
	#define kthread_create(threadfn, data, namefmt, arg...)	\
				kthread_create_on_node(threadfn, data,		\
					NUMA_NO_NODE, namefmt, ##arg)
```

### DS48-A026 — run宏新建__k并传data给create（29–32行）

[VERIFIED mykernel/sched/forkexec/forkexec_macro.h::kthread_run]
```c
	#define kthread_run(threadfn, data, namefmt, ...)	({	\
				task_s *__k									\
					= kthread_create(threadfn, data,		\
						namefmt, ## __VA_ARGS__);			\
```

### DS48-A027 — run宏IS_ERR门下真实wake_up_process与返回__k（33–35行）

[VERIFIED mykernel/sched/forkexec/forkexec_macro.h::kthread_run]
```c
				if (!IS_ERR(__k)) wake_up_process(__k);		\
				__k;										\
			})
```

### DS48-A028 — 独立初始化宏done初值字面0（4–5行）

[VERIFIED mykernel/kactive/completion/completion_macro.h::COMPLETION_INITIALIZER]
```c
	#define COMPLETION_INITIALIZER(work)	\
			{ 0, __SWAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
```

### DS48-A029 — 独立静态对象声明宏引用初始化宏（15–16行）

[VERIFIED mykernel/kactive/completion/completion_macro.h::DECLARE_COMPLETION]
```c
	#define DECLARE_COMPLETION(work)	\
				completion_s work = COMPLETION_INITIALIZER(work)
```

### DS48-A030 — 固定time真实completion done与wait成员（19–22行）

[VERIFIED mykernel/kactive/completion/completion_types.h::completion]
```c
	struct completion {
		uint			done;
		swqueue_hdr_s	wait;
	};
```

### DS48-A031 — complete入口与done读取/更新门（32–36行）

[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
void complete(completion_s *x)
{
	ulong flags;
	spin_lock_irqsave(&x->wait.lock, flags);
	if (x->done != UINT_MAX)
```

### DS48-A032 — complete增加done与swake_up_locked调用（36–40行）

[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
	spin_unlock_irqrestore(&x->wait.lock, flags);
}
```

### DS48-A033 — void等待接口及下层调用未返回状态（53–55行）

[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
```

### DS48-A034 — 动态初始化helper独立函数片段（35–39行）

[VERIFIED mykernel/kactive/completion/completion.h::init_completion]
```c
		PREFIX_STATIC_INLINE
		void
		init_completion(completion_s *x) {
			x->done = 0;
			init_swait_queue_head(&x->wait);
```

### DS48-A035 — 编码错误区间单宏（15–15行）

[VERIFIED mykernel/include/linux/kernel/err.h::MAX_ERRNO]
```c
	#define MAX_ERRNO	4095
```

### DS48-A036 — 错误指针解码为long的实际函数（31–34行）

[VERIFIED mykernel/include/linux/kernel/err.h::PTR_ERR]
```c
		static inline long
		PTR_ERR(const void *ptr) {
			return (long) ptr;
		}
```

### DS48-A037 — 只按错误指针编码范围判断（36–39行）

[VERIFIED mykernel/include/linux/kernel/err.h::IS_ERR]
```c
		static inline bool
		IS_ERR(const void *ptr) {
			return IS_ERR_VALUE((size_t)ptr);
		}
```

### DS48-A038 — opts固定字符串和调用者局部err（411–412行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	char opts[] = "mode=0755";
	int err;
```

### DS48-A039 — 首次挂载IS_ERR门并解码（415–418行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	mnt = vfs_kern_mount(&internal_fs_type, 0, "devtmpfs", opts);
	if (IS_ERR(mnt)) {
		pr_err("devtmpfs: unable to create devtmpfs %ld\n", PTR_ERR(mnt));
		return PTR_ERR(mnt);
```

### DS48-A040 — 注册错误直接return err（422–424行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	if (err) {
		pr_err("devtmpfs: unable to register devtmpfs type %d\n", err);
		return err;
```

### DS48-A041 — 传&err且仅结果非ERR才调用wait（427–429行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	if (!IS_ERR(thread)) {
		wait_for_completion(&setup_done);
```

### DS48-A042 — 创建错误分支没有wait调用（431–433行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
		err = PTR_ERR(thread);
		thread = NULL;
	}
```

### DS48-A043 — while在if(err)注销分支之前（435–439行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	while (err);
	
	if (err) {
		pr_err("devtmpfs: unable to create devtmpfs %i\n", err);
		unregister_filesystem(&dev_fs_type);
```

### DS48-A044 — 只有跳出while才可进入unregister（437–440行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	if (err) {
		pr_err("devtmpfs: unable to create devtmpfs %i\n", err);
		unregister_filesystem(&dev_fs_type);
		return err;
```

### DS48-A045 — 独立局部err和init_mount写入（388–392行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		int err = 0;

		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
```

### DS48-A046 — 工作早错误直接goto out跳过切换（391–394行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		if (err)
			goto out;
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
```

### DS48-A047 — 目录切换与两行注释结果语境（393–397行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		init_chdir("/.."); /* will traverse into overmounted root */
		init_chroot(".");
	// out:
	// 	return err;
	// }
```

### DS48-A048 — 通知之后才调用工作循环（399–400行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
	complete(&setup_done);
	devtmpfs_work_loop();
```

### DS48-A049 — 外层while1及请求队列门（357–360行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
	while (1) {
		spin_lock(&req_lock);
		while (requests) {
			req_s *req = requests;
```

### DS48-A050 — 请求级completion和err两个成员（49–50行）

[VERIFIED mykernel/drivers/base/devtmpfs.c::req_s]
```c
	completion_s	done;
	int				err;
```

### DS48-A051 — 另一独立宏仅为区别请求/栈对象（18–18行）

[VERIFIED mykernel/kactive/completion/completion_macro.h::DECLARE_COMPLETION_ONSTACK]
```c
	#define DECLARE_COMPLETION_ONSTACK(work)	DECLARE_COMPLETION(work)
```

### DS48-A052 — 内部等待helper签名为long但不是wait_for_completion返回（70–72行）

[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
		PREFIX_STATIC_INLINE
		long
		wait_for_common(completion_s*x, long timeout, int state) {
```

## 定义、选择上下文及停止点

- devtmpfs_init完整活动体在devtmpfs.c409–445，devtmpfsd完整体384–403，devtmpfs_work_loop选定完整体355–377。**创建返回task指针**、调用者栈局部err、通过&err传递的指针值、工作函数void*p与独立局部err、setup_done.done事件是六种不同身份。所读devtmpfsd函数体从未以p读写调用者err。
- 单宏kthread_create与kthread_run分别在forkexec_macro.h 16–18及29–35；前者将threadfn/data/NUMA_NO_NODE/name转给kthread_create_on_node，后者将返回保存__k，只有!IS_ERR(__k)才调用wake_up_process，最终交回task_s*或错误指针。在本批**不打开kthread_create_on_node内部作为审计对象**；extern声明不能充当返回值生产者证据。运行线程、数据跨栈生命期与调度时序均未证。
- setup_done的使用点是一个static __initdata DECLARE_COMPLETION。COMPLETION_INITIALIZER单宏含done字面0及等待队列头初始化宏，DECLARE_COMPLETION再展开为completion_s类型声明，结合固定completion_s内uint done与swqueue_hdr_s wait。**这是静态宏定义层级推导，未运行预处理**，不把默认分支include/linux/kernel/completion.h替代固定time的kactive/completion真实树。
- 真实complete函数体（completion.c32–40）会有条件增加done、发出swake_up_locked及解锁；wait_for_completion定义（completion.c53–55）返回void，直接请求wait_for_common，**不返回int errno给调用方**。本批只核直接体和类型，不展开完整排队/调度/内存顺序。complete按当前体未从devtmpfsd获得工作err参数。
- 通过kactive_api/kactive_types转包含关系可定位本固定源码的completion.h/types.h；直接api头中的extern声明可说明签名，但不充当实际实现引用。本批只对真正completion.c的函数体或completion_*里的真宏/结构报VERIFIED。
- devtmpfs_submit_req::req.done与静态setup_done身份独立。req_s还有自己的req.err字段；工作循环把handle结果写req->err并complete(&req->done)，不是对setup_done发起setup通知；本批不分析handle/请求释放和队列进度协议。
- IS_ERR/PTR_ERR由固定err.h所选活动实现限定；!IS_ERR本身不认证返回指针非NULL或仍是活task，所有模型要显式有效对象前提。字符串注释所声称的线程返回ERROR条件不能代替当前真实结果通道。
- 在选定源码的while(err);分支，对**合法、稳定非零且无被认证其它写者**的局部err，后续if(err)/unregister/return不具备从该源循环正常流出的证明；不可将它描述为已经执行的清理或物理机器永恒自旋的实测。
- 每条VERIFIED只证明选定固定源片段对应的局部事实/符号。来源有效性、返回时点、其它写者、completion初始done实机值、调度、线程退出及真实内核结果均不得超出证据。

## 实际方法

直接源码引用52段，连接器第二轮确认52段，降级0；另定点回读kactive的包含路径和PR17旧47记录，不把后者计为本批新引文。完整P9、Graphviz、外部YAML解析、编译预处理、仓库代码/脚本/QEMU/硬件、请求工作循环实现、完整线程/调度/wait协议、实际初始化均NOT_RUN。
