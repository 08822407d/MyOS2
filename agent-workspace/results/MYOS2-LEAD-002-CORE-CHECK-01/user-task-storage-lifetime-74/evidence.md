# CORE-USER-TASK-STORAGE-LIFETIME-74｜原样固定源码证据

固定源码time@a039d9803ade，所有引文位于真实单一定义内、1–5行连续物理行。注释只作非活动意图证据，宏/结构单独锚，未执行仓库程序。D01–D05为73 R73-01的追加新锚，不覆盖旧73或重新认证旧56条。

## D01｜原73 A16状态成员重锚

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		int				exit_state;
		int				exit_code;
		int				exit_signal;
```
固定源物理行146–148。

## D02｜原73 B13父指针成员重锚

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		task_s __rcu	*real_parent;
		/* Recipient of SIGCHLD, wait4() reports: */
		task_s __rcu	*parent;
```
固定源物理行231–233。

## D03｜原73 B14子节点成员重锚

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		List_hdr_s		children;
		List_s			sibling;
```
固定源物理行238–239。

## D04｜别名独立定义

[VERIFIED mykernel/sched/sched_type_declaration.h::task_s]
```c
	typedef struct task_struct task_s;
```
固定源物理行57–57。

## D05｜真实参数为exit_code

[VERIFIED mykernel/sched/forkexec/exit.c::do_group_exit]
```c
	do_exit(exit_code);
	/* NOTREACHED */
```
固定源物理行219–220。

## A01｜task结构申请

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_task_struct_node]
```c
*alloc_task_struct_node(int node) {
	return kzalloc(sizeof(task_s), GFP_KERNEL);
}
```
固定源物理行68–70。

## A02｜task结构kfree封装

[VERIFIED mykernel/sched/forkexec/fork.c::free_task_struct]
```c
free_task_struct(task_s *tsk) {
	kfree(tsk);
}
```
固定源物理行73–75。

## A03｜原task与新指针声明

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
static task_s *dup_task_struct(task_s *orig, int node)
{
	task_s *tsk;
	int err;
```
固定源物理行273–276。

## A04｜node条件门

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (node == NUMA_NO_NODE)
		// node = tsk_fork_get_node(orig);
	tsk = alloc_task_struct_node(node);
```
固定源物理行278–280。

## A05｜NULL结果判断

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (!tsk)
		return NULL;
```
固定源物理行281–282。

## A06｜链节点初始化随后被整体复制覆盖

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	INIT_LIST_S(&tsk->tasks);
	// int __weak arch_dup_task_struct(task_s *dst, task_s *src)
	// {
		*tsk = *orig;
```
固定源物理行285–288。

## A07｜整结构赋值

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		*tsk = *orig;
		err = 0;
```
固定源物理行288–289。

## A08｜新栈申请失败分流

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	err = alloc_thread_stack_node(tsk, node);
	if (err)
		goto free_tsk;
```
固定源物理行294–296。

## A09｜新task usage重设

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	refcount_set(&tsk->usage, 1);
```
固定源物理行333–333。

## A10｜worker_private再初始化

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	tsk->worker_private = NULL;
```
固定源物理行340–340。

## A11｜成功交出新指针

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	return tsk;
```
固定源物理行351–351。

## A12｜局部失败释放task

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
free_tsk:
	free_task_struct(tsk);
	return NULL;
```
固定源物理行355–357。

## A13｜接收task及NULL分支

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p = dup_task_struct(current, node);
	if (!p)
		goto fork_out;
```
固定源物理行959–961。

## A14｜后续初始化children及sibling

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	INIT_LIST_HEADER_S(&p->children);
	INIT_LIST_S(&p->sibling);
```
固定源物理行1018–1019。

## A15｜可见node生产者

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	p = copy_process(NULL, trace, NUMA_NO_NODE, args);
```
固定源物理行1485–1485。

## A16｜独立stack与usage成员

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		void			*stack;
		refcount_t		usage;
```
固定源物理行32–33。

## A17｜task链节点成员

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		List_s			tasks;	// 全局task_s链表的节点
```
固定源物理行133–133。

## A18｜原task栈及usage静态初值

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.stack				= init_stack,
	.usage				= REFCOUNT_INIT(2),
```
固定源物理行77–78。

## A19｜原task链节点来源

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.tasks					= LIST_INIT(init_task.tasks),
```
固定源物理行102–102。

## A20｜结构复制后的旗标写入

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	p->flags &= ~PF_KTHREAD;
	if (args->kthread)
		p->flags |= PF_KTHREAD;
```
固定源物理行962–964。

## A21｜正常返回跳过错误标签

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	return p;
```
固定源物理行1372–1372。

## A22｜复制后的局部错误码

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
		err = 0;
```
固定源物理行289–289。

## A23｜错误码判断

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (err)
		goto free_tsk;
```
固定源物理行291–292。

## A24｜task状态独立成员

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		uint			__state;
```
固定源物理行19–19。

## B01｜栈申请函数

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
alloc_thread_stack_node(task_s *tsk, int node) {
	vma_s *vm;
	void *stack;
	int i;
```
固定源物理行79–82。

## B02｜kmalloc及失败判断

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	stack = kmalloc(THREAD_SIZE, GFP_KERNEL);
	if (!stack)
		return -ENOMEM;
```
固定源物理行94–96。

## B03｜成功后发布新stack字段

[VERIFIED mykernel/sched/forkexec/fork.c::alloc_thread_stack_node]
```c
	tsk->stack = stack;
	return 0;
```
固定源物理行110–111。

## B04｜非DEAD状态门返回

[VERIFIED mykernel/sched/forkexec/fork.c::release_task_stack]
```c
	if (WARN_ON(READ_ONCE(tsk->__state) != TASK_DEAD))
		return;  /* Better to leak the stack than to free prematurely */
```
固定源物理行118–119。

## B05｜栈释放请求再设NULL

[VERIFIED mykernel/sched/forkexec/fork.c::release_task_stack]
```c
	kfree(tsk->stack);
	tsk->stack = NULL;
```
固定源物理行123–124。

## B06｜引用减量门注释

[VERIFIED mykernel/sched/forkexec/fork.c::put_task_stack]
```c
	// if (refcount_dec_and_test(&tsk->stack_refcount))
		release_task_stack(tsk);
```
固定源物理行130–131。

## B07｜结构销毁入口

[VERIFIED mykernel/sched/forkexec/fork.c::free_task]
```c
	free_task_struct(tsk);
```
固定源物理行149–149。

## B08｜栈计数断言注释

[VERIFIED mykernel/sched/forkexec/fork.c::free_task]
```c
	// WARN_ON_ONCE(refcount_read(&tsk->stack_refcount) != 0);
	// rt_mutex_debug_task_free(tsk);
```
固定源物理行143–144。

## B09｜task指针kfree

[VERIFIED mykernel/sched/forkexec/fork.c::free_task_struct]
```c
	kfree(tsk);
```
固定源物理行74–74。

## B10｜失败设置DEAD、put栈、销毁

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	WRITE_ONCE(p->__state, TASK_DEAD);
	put_task_stack(p);
	delayed_free_task(p);
```
固定源物理行1425–1427。

## B11｜delayed_free实际立即调用

[VERIFIED mykernel/sched/forkexec/fork.c::delayed_free_task]
```c
	free_task(tsk);
```
固定源物理行856–856。

## B12｜THREAD_SIZE常量宏

[VERIFIED mykernel/arch/x86_64/mm/page/page_const_arch.h::THREAD_SIZE]
```c
	#define THREAD_SIZE_ORDER		3
```
固定源物理行13–13。

## B13｜TASK_DEAD常量宏

[VERIFIED mykernel/sched/task/task_const.h::TASK_DEAD]
```c
	#define TASK_DEAD				0x00000080
```
固定源物理行37–37。

## B14｜READ_ONCE宏

[VERIFIED mykernel/include/linux/kernel/asm-generic/rwonce.h::READ_ONCE]
```c
	#	define READ_ONCE(x) ({							\
					compiletime_assert_rwonce_type(x);	\
					__READ_ONCE(x);						\
				})
```
固定源物理行49–52。

## B15｜WARN_ON所读活动定义

[VERIFIED mykernel/include/linux/kernel/asm-generic/bug.h::WARN_ON]
```c
	#		define WARN_ON(condition) (condition)
```
固定源物理行130–130。

## B16｜栈refcount成员被注释

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		// refcount_t stack_refcount;
```
固定源物理行698–698。

## B17｜init_task静态栈

[VERIFIED mykernel/init/init_task.c::init_task]
```c
	.stack				= init_stack,
```
固定源物理行77–77。

## B18｜直接取task栈辅助体

[VERIFIED mykernel/sched/misc/sched_misc.h::task_stack_page]
```c
			return task->stack;
```
固定源物理行29–29。

## B19｜错误门/前置条件

[VERIFIED mykernel/sched/forkexec/fork.c::dup_task_struct]
```c
	if (err)
		goto free_tsk;
```
固定源物理行291–292。

## B20｜释放后字段清空

[VERIFIED mykernel/sched/forkexec/fork.c::release_task_stack]
```c
	tsk->stack = NULL;
```
固定源物理行124–124。

## B21｜put helper内活动release

[VERIFIED mykernel/sched/forkexec/fork.c::put_task_stack]
```c
void put_task_stack(task_s *tsk)
{
	// if (refcount_dec_and_test(&tsk->stack_refcount))
		release_task_stack(tsk);
```
固定源物理行128–131。

## B22｜free task本体

[VERIFIED mykernel/sched/forkexec/fork.c::free_task]
```c
void free_task(task_s *tsk)
{
	// release_user_cpus_ptr(tsk);
```
固定源物理行134–136。

## C01｜task裸查询

[VERIFIED mykernel/sched/task/pid.c::find_get_task_by_vpid]
```c
	task = find_task_by_vpid(nr);
	// if (task)
	// 	get_task_struct(task);
```
固定源物理行399–401。

## C02｜task引用增量注释

[VERIFIED mykernel/sched/task/pid.c::find_get_task_by_vpid]
```c
	// 	get_task_struct(task);
```
固定源物理行401–401。

## C03｜直接返回裸task

[VERIFIED mykernel/sched/task/pid.c::find_get_task_by_vpid]
```c
	return task;
```
固定源物理行404–404。

## C04｜PID转task，引用增量注释

[VERIFIED mykernel/sched/task/pid.c::get_pid_task]
```c
	result = pid_task(pid, type);
	// if (result)
	// 	get_task_struct(result);
	// rcu_read_unlock();
```
固定源物理行421–424。

## C05｜get task注释

[VERIFIED mykernel/sched/task/pid.c::get_pid_task]
```c
	// 	get_task_struct(result);
```
固定源物理行423–423。

## C06｜实际取得pid引用

[VERIFIED mykernel/sched/task/pid.c::get_task_pid]
```c
	pid = get_pid(rcu_dereference(*task_pid_ptr(task, type)));
	// rcu_read_unlock();
	return pid;
```
固定源物理行411–413。

## C07｜get_pid增加pid计数

[VERIFIED mykernel/sched/task/pid.h::get_pid]
```c
			if (pid) refcount_inc(&pid->count);
			return pid;
```
固定源物理行62–63。

## C08｜put pid按PID计数减少

[VERIFIED mykernel/sched/task/pid.c::put_pid]
```c
	if (refcount_dec_and_test(&pid->count)) {
		kmem_cache_free(ns->pid_cachep, pid);
		put_pid_ns(ns);
```
固定源物理行119–121。

## C09｜pid_task通过列表取得裸指针

[VERIFIED mykernel/sched/task/pid.c::pid_task]
```c
			result = hlist_entry(first, task_s, pid_links[(type)]);
	}
```
固定源物理行373–374。

## C10｜kernel_clone取得PID

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	pid_struct = get_task_pid(p, PIDTYPE_PID);
	nr = pid_vnr(pid_struct);
```
固定源物理行1497–1498。

## C11｜kernel_clone归还PID

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
	put_pid(pid_struct);
```
固定源物理行1527–1527。

## C12｜vfork task get为注释

[VERIFIED mykernel/sched/forkexec/fork.c::kernel_clone]
```c
		// get_task_struct(p);
```
固定源物理行1506–1506。

## C13｜失败path设置dead、put栈、free task

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
bad_fork_free:
	WRITE_ONCE(p->__state, TASK_DEAD);
	put_task_stack(p);
	delayed_free_task(p);
```
固定源物理行1424–1427。

## C14｜成功不落入失败标签

[VERIFIED mykernel/sched/forkexec/fork.c::copy_process]
```c
	return p;
```
固定源物理行1372–1372。

## C15｜正常死亡任务释放注释

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// 	put_task_stack(prev);

	// 	put_task_struct_rcu_user(prev);
```
固定源物理行861–863。

## C16｜task put注释

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// 	put_task_struct_rcu_user(prev);
```
固定源物理行863–863。

## C17｜正常退出状态改写

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	set_special_state(TASK_DEAD);
```
固定源物理行1175–1175。

## C18｜正常退出调度请求

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	__schedule(SM_NONE);
	BUG();
```
固定源物理行1180–1181。

## C19｜BUG后备循环

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	for (;;)
		cpu_relax();
```
固定源物理行1184–1185。

## C20｜退出主体末尾请求

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	do_task_dead();
```
固定源物理行185–185。

## C21｜switch完成后实际C恢复前提

[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch]
```c
			return finish_task_switch(prev);
```
固定源物理行214–214。

## C22｜delayed_free并非主动异步延迟

[VERIFIED mykernel/sched/forkexec/fork.c::delayed_free_task]
```c
	free_task(tsk);
```
固定源物理行856–856。

## C23｜usage与pid.count两类计数

[VERIFIED mykernel/sched/task/task_types.h::task_struct]
```c
		refcount_t		usage;
```
固定源物理行33–33。
