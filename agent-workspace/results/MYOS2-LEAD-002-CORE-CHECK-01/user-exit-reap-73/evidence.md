# 第73批固定源码短锚

固定源码：`time@a039d9803ade`，直接通过GitHub连接器读取；每个原样引文为同一真实函数或定义的连续1–5物理行。注释仅证明相应语句非活动；行号仅辅助，实际路径与符号为证据主键。完整本地P9未运行。

## A01

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE1(exit)]
```c
MYOS_SYSCALL_DEFINE1(exit, int, error_code)
```
固定物理行 68–68。

## A02

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE1(exit)]
```c
	do_exit(error_code);
```
固定物理行 72–72。

## A03

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE1(exit_group)]
```c
MYOS_SYSCALL_DEFINE1(exit_group, int, error_code)
```
固定物理行 133–133。

## A04

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE1(exit_group)]
```c
	do_group_exit((error_code & 0xff) << 8);
	/* NOTREACHED */
	return 0;
```
固定物理行 137–139。

## A05

[VERIFIED mykernel/sched/syscall.c::MYOS_SYSCALL_DEFINE4(wait4)]
```c
MYOS_SYSCALL_DEFINE4(wait4, pid_t, pid, int *, start_addr,
		int, options, void *, rusage)
{

}
```
固定物理行 76–80。

## A06

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
void __noreturn do_exit(long code)
{
	task_s *tsk = current;
	int group_dead;

```
固定物理行 68–72。

## A07

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// group_dead = atomic_dec_and_test(&tsk->signal->live);
```
固定物理行 95–95。

## A08

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	tsk->exit_code = code;
```
固定物理行 117–117。

## A09

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	exit_notify(tsk, group_dead);
```
固定物理行 153–153。

## A10

[VERIFIED mykernel/sched/forkexec/exit.c::do_group_exit]
```c
do_group_exit(int exit_code)
{
	signal_s *sig = current->signal;

```
固定物理行 194–197。

## A11

[VERIFIED mykernel/sched/forkexec/exit.c::do_group_exit]
```c
	// 		sig->flags = SIGNAL_GROUP_EXIT;
	// 		zap_other_threads(current);
```
固定物理行 213–214。

## A12

[VERIFIED mykernel/sched/forkexec/exit.c::do_group_exit]
```c
	do_exit(exit_code);
	/* NOTREACHED */
```
固定物理行 219–220。

## A13

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
long x64_sys_call(const pt_regs_s *regs, unsigned int nr)
{
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```
固定物理行 27–31。

## A14

[VERIFIED mykernel/arch/x86_64/include/asm/syscall_wrapper.h::__MYOS_SYSCALL_DEFINEx]
```c
	#define __MYOS_SYSCALL_DEFINEx(x, name, ...)						\
				long __se_sys##name(const pt_regs_s *regs);				\
				long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
				__SYS_STUBx(x64, sys##name)								\
```
固定物理行 241–244。

## A15

[VERIFIED mykernel/include/linux/kernel/syscalls.h::MYOS_SYSCALL_DEFINE1]
```c
	#define MYOS_SYSCALL_DEFINE1(name, ...)	__MYOS_SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
```
固定物理行 1384–1384。

## A16

[VERIFIED mykernel/sched/task/task_types.h::task_s]
```c
		int				exit_state;
		int				exit_code;
		int				exit_signal;
```
固定物理行 146–148。

## B01

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
static void exit_notify(task_s *tsk, int group_dead)
{
	while (tsk->children.count != 0)
```
固定物理行 15–17。

## B02

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	while (tsk->children.count != 0)
	{
		List_s * child_lp = list_header_remove_head(&tsk->children);
```
固定物理行 17–19。

## B03

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
		List_s * child_lp = list_header_remove_head(&tsk->children);
		while (child_lp == 0);
```
固定物理行 19–20。

## B04

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
		task_s *task_initd = find_task_by_pid_ns(1, PIDTYPE_PID);
		list_header_add_to_tail(&task_initd->children, child_lp);
```
固定物理行 22–23。

## B05

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	// tsk->exit_state = EXIT_ZOMBIE;
```
固定物理行 37–37。

## B06

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	// 	autoreap = do_notify_parent(tsk, sig);
```
固定物理行 43–43。

## B07

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	// 	release_task(p);
```
固定物理行 63–63。

## B08

[VERIFIED mykernel/sched/task/pid.c::find_task_by_pid_ns]
```c
task_s *find_task_by_pid_ns(pid_t nr, pid_ns_s *ns)
{
	// RCU_LOCKDEP_WARN(!rcu_read_lock_held(),
	// 		 "find_task_by_pid_ns() needs rcu_read_lock() protection");
	return pid_task(find_pid_ns(nr, ns), PIDTYPE_PID);
```
固定物理行 382–386。

## B09

[VERIFIED mykernel/sched/task/pid.c::find_pid_ns]
```c
	return idr_find(&ns->idr, nr);
```
固定物理行 306–306。

## B10

[VERIFIED mykernel/sched/task/pid_const.h::enum pid_type]
```c
	enum pid_type {
		PIDTYPE_PID,
		PIDTYPE_TGID,
		PIDTYPE_PGID,
		PIDTYPE_SID,
```
固定物理行 6–10。

## B11

[VERIFIED mykernel/lib/list/double_list.h::list_header_remove_head]
```c
		*list_header_remove_head(List_hdr_s *lhdr_p) {
			if (lhdr_p->count > 0) {
				List_s *ret_val = lhdr_p->anchor.next;
				list_del_init(ret_val);
				lhdr_p->count--;
```
固定物理行 652–656。

## B12

[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
```c
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```
固定物理行 664–667。

## B13

[VERIFIED mykernel/sched/task/task_types.h::task_s]
```c
		task_s __rcu	*real_parent;
		/* Recipient of SIGCHLD, wait4() reports: */
		task_s __rcu	*parent;
```
固定物理行 231–233。

## B14

[VERIFIED mykernel/sched/task/task_types.h::task_s]
```c
		List_hdr_s		children;
		List_s			sibling;
```
固定物理行 238–239。

## B15

[VERIFIED mykernel/sched/forkexec/exit.c::exit_notify]
```c
	// 	tsk->exit_state = EXIT_DEAD;
```
固定物理行 52–52。

## C01

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_signals(tsk);  /* sets PF_EXITING */
```
固定物理行 89–89。

## C02

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_mm();
```
固定物理行 120–120。

## C03

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	exit_files(tsk);
	exit_fs(tsk);
```
固定物理行 128–129。

## C04

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_task_namespaces(tsk);
```
固定物理行 132–132。

## C05

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	exit_thread(tsk);
```
固定物理行 134–134。

## C06

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	// exit_task_stack_account(tsk);
```
固定物理行 175–175。

## C07

[VERIFIED mykernel/sched/forkexec/exit.c::do_exit]
```c
	do_task_dead();
```
固定物理行 185–185。

## C08

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
void exit_files(task_s *tsk)
{
	files_struct_s * files = tsk->files;

```
固定物理行 98–101。

## C09

[VERIFIED mykernel/fs/vfs/file.c::exit_files]
```c
		tsk->files = NULL;
		task_unlock(tsk);
		put_files_struct(files);
```
固定物理行 104–106。

## C10

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
	if (atomic_dec_and_test(&files->refcount)) {
```
固定物理行 74–74。

## C11

[VERIFIED mykernel/fs/vfs/file.c::put_files_struct]
```c
				filp_close(file);
```
固定物理行 87–87。

## C12

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
void exit_fs(task_s *tsk)
{
	taskfs_s *fs = tsk->fs;

```
固定物理行 92–95。

## C13

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
		kill = !--fs->users;
```
固定物理行 102–102。

## C14

[VERIFIED mykernel/sched/misc/fs_struct.c::exit_fs]
```c
			free_fs_struct(fs);
```
固定物理行 107–107。

## C15

[VERIFIED mykernel/sched/misc/fs_struct.c::free_fs_struct]
```c
void free_fs_struct(taskfs_s *fs)
{
	path_put(&fs->root);
	path_put(&fs->pwd);
	kmem_cache_free(fs_cachep, fs);
```
固定物理行 85–89。

## C16

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::exit_thread]
```c
void exit_thread(task_s *tsk)
{
	// struct thread_struct *t = &tsk->thread;
	// struct fpu *fpu = &t->fpu;
```
固定物理行 38–41。

## C17

[VERIFIED mykernel/arch/x86_64/sched/context/process_arch.c::exit_thread]
```c
	// fpu__drop(fpu);
}
```
固定物理行 48–49。

## C18

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	set_special_state(TASK_DEAD);
```
固定物理行 1175–1175。

## C19

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::do_task_dead]
```c
	__schedule(SM_NONE);
	BUG();
```
固定物理行 1180–1181。

## C20

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// prev_state = READ_ONCE(prev->__state);
```
固定物理行 821–821。

## C21

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// if (unlikely(prev_state == TASK_DEAD)) {
	// 	if (prev->sched_class->task_dead)
	// 		prev->sched_class->task_dead(prev);

```
固定物理行 856–859。

## C22

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// 	put_task_stack(prev);
```
固定物理行 861–861。

## C23

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::finish_task_switch]
```c
	// 	put_task_struct_rcu_user(prev);
```
固定物理行 863–863。

## C24

[VERIFIED mykernel/sched/scheduler/scheduler.h::context_switch]
```c
			return finish_task_switch(prev);
```
固定物理行 214–214。

## C25

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::schedule_tail]
```c
	// finish_task_switch(prev);
	preempt_enable();
```
固定物理行 885–886。

