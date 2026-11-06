---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-SEMAPHORE-TIMEOUT-51
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6 (Owner-reported, backend unverified)"
date: 2026-11-06
base_snapshot: time（分支名）@a039d9803ade（短号）
read_channel: connector
record_type: source_quote_ledger
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第51批全部固定源码引文
来源均为time@a039d9803ade本轮重新读取的单一定义体或宏/结构/typedef。每条连续1–5行，支持范围仅限其标题，DEBUG extern不充当函数实现；实际定义范围、符号和行区间单列。
## SM51-A001 · sema_t typedef
[VERIFIED mykernel/lock_IPC/lock_ipc_type_declaration.h::sema_t]
```c
	typedef struct semaphore sema_t;
```
evidence_metadata: {"id":"SM51-A001","actual_definition_kind":"typedef","enclosing_symbol":"sema_t","definition_range":"20-20","quote_range":"20-20","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A002 · lock,count,ordinary List_s wait_list
[VERIFIED mykernel/lock_IPC/semaphore/semaphore_types.h::struct semaphore]
```c
	struct semaphore {
		spinlock_t	lock;
		uint		count;
		List_s		wait_list;
	};
```
evidence_metadata: {"id":"SM51-A002","actual_definition_kind":"struct","enclosing_symbol":"struct semaphore","definition_range":"7-11","quote_range":"7-11","quote_supports_claim":true,"consumer_positions":["wait"]}

## SM51-A003 · waiter list/task/up fields
[VERIFIED mykernel/lock_IPC/semaphore/semaphore_types.h::struct semaphore_waiter]
```c
	struct semaphore_waiter {
		List_s  list;
		task_s  *task;
		bool    up;
	};
```
evidence_metadata: {"id":"SM51-A003","actual_definition_kind":"struct","enclosing_symbol":"struct semaphore_waiter","definition_range":"13-17","quote_range":"13-17","quote_supports_claim":true,"consumer_positions":["wait","up"]}

## SM51-A004 · static semaphore initializer
[VERIFIED mykernel/lock_IPC/semaphore/semaphore_macro.h::__SEMAPHORE_INITIALIZER]
```c
	#define __SEMAPHORE_INITIALIZER(name, n)	{					\
				.lock		= __SPIN_LOCK_UNLOCKED((name).lock),	\
				.count		= n,									\
				.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
			}
```
evidence_metadata: {"id":"SM51-A004","actual_definition_kind":"macro","enclosing_symbol":"__SEMAPHORE_INITIALIZER","definition_range":"4-8","quote_range":"4-8","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A005 · declaration initializer macro
[VERIFIED mykernel/lock_IPC/semaphore/semaphore_macro.h::DEFINE_SEMAPHORE]
```c
	#define DEFINE_SEMAPHORE(_name, _n)	\
				sema_t _name = __SEMAPHORE_INITIALIZER(_name, _n)
```
evidence_metadata: {"id":"SM51-A005","actual_definition_kind":"macro","enclosing_symbol":"DEFINE_SEMAPHORE","definition_range":"16-17","quote_range":"16-17","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A006 · dynamic semaphore initializer
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::sema_init]
```c
		sema_init(sema_t *sem, int val) {
			// static struct lock_class_key __key;
			*sem = (sema_t) __SEMAPHORE_INITIALIZER(*sem, val);
			// lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
```
evidence_metadata: {"id":"SM51-A006","actual_definition_kind":"function","enclosing_symbol":"sema_init","definition_range":"28-34","quote_range":"30-33","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A007 · public int result initialized zero
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_timeout]
```c
int __sched down_timeout(sema_t *sem, long timeout)
{
	ulong flags;
	int result = 0;
```
evidence_metadata: {"id":"SM51-A007","actual_definition_kind":"function","enclosing_symbol":"down_timeout","definition_range":"135-151","quote_range":"135-138","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A008 · count fast branch versus __down_timeout slow
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_timeout]
```c
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_timeout(sem, timeout);
```
evidence_metadata: {"id":"SM51-A008","actual_definition_kind":"function","enclosing_symbol":"down_timeout","definition_range":"135-151","quote_range":"142-146","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A009 · outer unlock and int result return
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_timeout]
```c
	spin_unlock_irqrestore(&sem->lock, flags);

	return result;
```
evidence_metadata: {"id":"SM51-A009","actual_definition_kind":"function","enclosing_symbol":"down_timeout","definition_range":"135-151","quote_range":"148-150","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A010 · fixed TASK_UNINTERRUPTIBLE helper argument
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__down_timeout]
```c
static noinline int __sched
__down_timeout(sema_t *sem, long timeout) {
	return __down_common(sem,
			TASK_UNINTERRUPTIBLE, timeout);
}
```
evidence_metadata: {"id":"SM51-A010","actual_definition_kind":"function","enclosing_symbol":"__down_timeout","definition_range":"197-201","quote_range":"197-201","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A011 · int wrapper ret declaration
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::__down_common]
```c
		int __sched
		__down_common(sema_t *sem, long state, long timeout) {
			int ret;
```
evidence_metadata: {"id":"SM51-A011","actual_definition_kind":"function","enclosing_symbol":"__down_common","definition_range":"72-82","quote_range":"73-75","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A012 · wrapper forwards helper return
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::__down_common]
```c
			ret = ___down_common(sem, state, timeout);
			// trace_contention_end(sem, ret);

			return ret;
```
evidence_metadata: {"id":"SM51-A012","actual_definition_kind":"function","enclosing_symbol":"__down_common","definition_range":"72-82","quote_range":"78-81","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A013 · actual helper signature and local waiter
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
		int __sched
		___down_common(sema_t *sem, long state, long timeout) {
			struct semaphore_waiter waiter;
```
evidence_metadata: {"id":"SM51-A013","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"42-44","quote_supports_claim":true,"consumer_positions":["wait"]}

## SM51-A014 · stack waiter appended task/up setup
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
			list_add_tail(&waiter.list, &sem->wait_list);
			waiter.task = current;
			waiter.up = false;
```
evidence_metadata: {"id":"SM51-A014","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"46-48","quote_supports_claim":true,"consumer_positions":["wait","up"]}

## SM51-A015 · signal then timeout gate
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
			for (;;) {
				if (signal_pending_state(state, current))
					goto interrupted;
				if (unlikely(timeout <= 0))
					goto timed_out;
```
evidence_metadata: {"id":"SM51-A015","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"50-54","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A016 · state unlock schedule assign relock grant test
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
				__set_current_state(state);
				spin_unlock_irq(&sem->lock);
				timeout = schedule_timeout(timeout);
				spin_lock_irq(&sem->lock);
				if (waiter.up)
```
evidence_metadata: {"id":"SM51-A016","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"55-59","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A017 · grant true returns zero
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
				if (waiter.up)
					return 0;
			}
```
evidence_metadata: {"id":"SM51-A017","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"59-61","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A018 · timeout removes waiter returns -ETIME
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
		timed_out:
			list_del(&waiter.list);
			return -ETIME;
```
evidence_metadata: {"id":"SM51-A018","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"63-65","quote_supports_claim":true,"consumer_positions":["wait"]}

## SM51-A019 · signal removes waiter returns -EINTR
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
		interrupted:
			list_del(&waiter.list);
			return -EINTR;
```
evidence_metadata: {"id":"SM51-A019","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"67-69","quote_supports_claim":true,"consumer_positions":["wait"]}

## SM51-A020 · up public void
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::up]
```c
void __sched up(sema_t *sem)
{
	ulong flags;
```
evidence_metadata: {"id":"SM51-A020","actual_definition_kind":"function","enclosing_symbol":"up","definition_range":"161-173","quote_range":"161-163","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A021 · up empty count++ versus __up
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::up]
```c
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(list_empty(&sem->wait_list)))
		sem->count++;
	else
		__up(sem);
```
evidence_metadata: {"id":"SM51-A021","actual_definition_kind":"function","enclosing_symbol":"up","definition_range":"161-173","quote_range":"166-170","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A022 · up unlock
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::up]
```c
	// raw_spin_unlock_irqrestore(&sem->lock, flags);
	spin_unlock_irqrestore(&sem->lock, flags);
}
```
evidence_metadata: {"id":"SM51-A022","actual_definition_kind":"function","enclosing_symbol":"up","definition_range":"161-173","quote_range":"171-173","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A023 · first waiter selected from ordinary list
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__up]
```c
static noinline void __sched
__up(sema_t *sem) {
	struct semaphore_waiter *waiter =
			list_first_entry(&sem->wait_list,
				struct semaphore_waiter, list);
```
evidence_metadata: {"id":"SM51-A023","actual_definition_kind":"function","enclosing_symbol":"__up","definition_range":"203-211","quote_range":"203-207","quote_supports_claim":true,"consumer_positions":["wait"]}

## SM51-A024 · __up unlinks then up=true then wake request
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__up]
```c
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
}
```
evidence_metadata: {"id":"SM51-A024","actual_definition_kind":"function","enclosing_symbol":"__up","definition_range":"203-211","quote_range":"208-211","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A025 · down public void
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down]
```c
void __sched down(sema_t *sem)
{
	ulong flags;
```
evidence_metadata: {"id":"SM51-A025","actual_definition_kind":"function","enclosing_symbol":"down","definition_range":"22-35","quote_range":"22-24","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A026 · down count versus __down
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down]
```c
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		__down(sem);
```
evidence_metadata: {"id":"SM51-A026","actual_definition_kind":"function","enclosing_symbol":"down","definition_range":"22-35","quote_range":"28-32","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A027 · plain down passes MAX and uninterruptible
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__down]
```c
static noinline void __sched
__down(sema_t *sem) {
	__down_common(sem, TASK_UNINTERRUPTIBLE,
		MAX_SCHEDULE_TIMEOUT);
}
```
evidence_metadata: {"id":"SM51-A027","actual_definition_kind":"function","enclosing_symbol":"__down","definition_range":"179-183","quote_range":"179-183","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A028 · interruptible int signature
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_interruptible]
```c
int __sched down_interruptible(sema_t *sem)
{
	ulong flags;
	int result = 0;
```
evidence_metadata: {"id":"SM51-A028","actual_definition_kind":"function","enclosing_symbol":"down_interruptible","definition_range":"47-63","quote_range":"47-50","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A029 · interruptible entry count fast gate
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_interruptible]
```c
	spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_interruptible(sem);
```
evidence_metadata: {"id":"SM51-A029","actual_definition_kind":"function","enclosing_symbol":"down_interruptible","definition_range":"47-63","quote_range":"54-58","quote_supports_claim":true,"consumer_positions":["entry","up"]}

## SM51-A030 · interruptible result return
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_interruptible]
```c
	spin_unlock_irqrestore(&sem->lock, flags);

	return result;
```
evidence_metadata: {"id":"SM51-A030","actual_definition_kind":"function","enclosing_symbol":"down_interruptible","definition_range":"47-63","quote_range":"60-62","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A031 · interruptible helper receives TASK_INTERRUPTIBLE and MAX
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__down_interruptible]
```c
static noinline int __sched
__down_interruptible(sema_t *sem) {
	return __down_common(sem, TASK_INTERRUPTIBLE,
			MAX_SCHEDULE_TIMEOUT);
}
```
evidence_metadata: {"id":"SM51-A031","actual_definition_kind":"function","enclosing_symbol":"__down_interruptible","definition_range":"185-189","quote_range":"185-189","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A032 · list_add_tail alias to list_add_to_prev
[VERIFIED mykernel/lib/list/double_list.h::list_add_tail]
```c
		#define list_add_tail list_add_to_prev
```
evidence_metadata: {"id":"SM51-A032","actual_definition_kind":"macro","enclosing_symbol":"list_add_tail","definition_range":"128-128","quote_range":"128-128","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A033 · list_empty alias
[VERIFIED mykernel/lib/list/double_list.h::list_empty]
```c
		#define list_empty list_is_empty_entry
```
evidence_metadata: {"id":"SM51-A033","actual_definition_kind":"macro","enclosing_symbol":"list_empty","definition_range":"133-133","quote_range":"133-133","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A034 · list_first_entry picks head next
[VERIFIED mykernel/lib/list/double_list_macro.h::list_first_entry]
```c
	#define list_first_entry(ptr, type, member) \
				list_entry((ptr)->next, type, member)
```
evidence_metadata: {"id":"SM51-A034","actual_definition_kind":"macro","enclosing_symbol":"list_first_entry","definition_range":"37-38","quote_range":"37-38","quote_supports_claim":true,"consumer_positions":["entry"]}

## SM51-A035 · tail insertion before head
[VERIFIED mykernel/lib/list/double_list.h::list_add_to_prev]
```c
		PREFIX_STATIC_INLINE
		void
		list_add_to_prev(List_s *new, List_s *head) {
			__list_add_between(new, head->prev, head);
		}
```
evidence_metadata: {"id":"SM51-A035","actual_definition_kind":"function","enclosing_symbol":"list_add_to_prev","definition_range":"219-223","quote_range":"219-223","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A036 · list_del invokes unlink and poisons
[VERIFIED mykernel/lib/list/double_list.h::list_del]
```c
		list_del(List_s *entry) {
			__list_del_entry(entry);
			entry->next = LIST_POISON1;
			entry->prev = LIST_POISON2;
		}
```
evidence_metadata: {"id":"SM51-A036","actual_definition_kind":"function","enclosing_symbol":"list_del","definition_range":"267-273","quote_range":"269-273","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A037 · list empty tests self next
[VERIFIED mykernel/lib/list/double_list.h::list_is_empty_entry]
```c
		bool
		list_is_empty_entry(const List_s *head) {
			return READ_ONCE(head->next) == head;
		}
```
evidence_metadata: {"id":"SM51-A037","actual_definition_kind":"function","enclosing_symbol":"list_is_empty_entry","definition_range":"428-432","quote_range":"429-432","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A038 · uint state and fast mask check
[VERIFIED mykernel/lock_IPC/signal/signal.h::signal_pending_state]
```c
		PREFIX_STATIC_INLINE
		int
		signal_pending_state(uint state, task_s *p) {
			if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
				return 0;
```
evidence_metadata: {"id":"SM51-A038","actual_definition_kind":"function","enclosing_symbol":"signal_pending_state","definition_range":"498-507","quote_range":"498-502","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A039 · remaining pending signal branch
[VERIFIED mykernel/lock_IPC/signal/signal.h::signal_pending_state]
```c
			if (!signal_pending(p))
				return 0;

			return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
		}
```
evidence_metadata: {"id":"SM51-A039","actual_definition_kind":"function","enclosing_symbol":"signal_pending_state","definition_range":"498-507","quote_range":"503-507","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A040 · MAX active schedule
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
		schedule();
		goto out;
```
evidence_metadata: {"id":"SM51-A040","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"774-775","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A041 · finite timer sequence, schedule commented
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer.task = current;
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```
evidence_metadata: {"id":"SM51-A041","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"795-799","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A042 · schedule_timeout return input or 0
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
 out:
	return timeout < 0 ? 0 : timeout;
```
evidence_metadata: {"id":"SM51-A042","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"804-805","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A043 · EINTR definition
[VERIFIED mykernel/include/uapi/asm-generic/errno-base.h::EINTR]
```c
	#define	EINTR		4	/* Interrupted system call */
```
evidence_metadata: {"id":"SM51-A043","actual_definition_kind":"macro","enclosing_symbol":"EINTR","definition_range":"10-10","quote_range":"10-10","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A044 · ETIME definition
[VERIFIED mykernel/include/uapi/asm-generic/errno.h::ETIME]
```c
	#define	ETIME			62		/* Timer expired */
```
evidence_metadata: {"id":"SM51-A044","actual_definition_kind":"macro","enclosing_symbol":"ETIME","definition_range":"45-45","quote_range":"45-45","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A045 · TASK_UNINTERRUPTIBLE macro definition
[VERIFIED mykernel/sched/task/task_const.h::TASK_UNINTERRUPTIBLE]
```c
	#define TASK_UNINTERRUPTIBLE	0x00000002
```
evidence_metadata: {"id":"SM51-A045","actual_definition_kind":"macro","enclosing_symbol":"TASK_UNINTERRUPTIBLE","definition_range":"28-28","quote_range":"28-28","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A046 · TASK_INTERRUPTIBLE macro definition
[VERIFIED mykernel/sched/task/task_const.h::TASK_INTERRUPTIBLE]
```c
	#define TASK_INTERRUPTIBLE		0x00000001
```
evidence_metadata: {"id":"SM51-A046","actual_definition_kind":"macro","enclosing_symbol":"TASK_INTERRUPTIBLE","definition_range":"27-27","quote_range":"27-27","quote_supports_claim":true,"consumer_positions":["up"]}

## SM51-A047 · TASK_WAKEKILL macro definition
[VERIFIED mykernel/sched/task/task_const.h::TASK_WAKEKILL]
```c
	#define TASK_WAKEKILL			0x00000100
```
evidence_metadata: {"id":"SM51-A047","actual_definition_kind":"macro","enclosing_symbol":"TASK_WAKEKILL","definition_range":"38-38","quote_range":"38-38","quote_supports_claim":true,"consumer_positions":[]}

## SM51-A048 · MAX_SCHEDULE_TIMEOUT macro definition
[VERIFIED mykernel/sched/task/task_const.h::MAX_SCHEDULE_TIMEOUT]
```c
	#define MAX_SCHEDULE_TIMEOUT	LONG_MAX
```
evidence_metadata: {"id":"SM51-A048","actual_definition_kind":"macro","enclosing_symbol":"MAX_SCHEDULE_TIMEOUT","definition_range":"86-86","quote_range":"86-86","quote_supports_claim":true,"consumer_positions":[]}

## 限界
否定性结论必须合读完整函数；例如有限schedule_timeout正常返回原输入，凭本体759–806活动路径，不仅凭return一行。引用不证明实际唤醒/调度、宏链接或锁安全。
