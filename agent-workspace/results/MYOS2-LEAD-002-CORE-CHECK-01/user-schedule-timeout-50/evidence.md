---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SCHEDULE-TIMEOUT-50
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
produced_by: "GPT-6（Owner所选，后台不可独立证明）"
model_per_owner: GPT-6
effort_per_owner: Extra
date: 2026-11-06
base_snapshot: "time（分支名），短号a039d9803ade"
read_channel: connector
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
record_type: bounded_source_anchors_and_R49_corrections
---
# R49新增修正证据7条、T50新增证据31条
全部源为固定time，按真实定义体范围截取1–5行。`actual_definition_kind`、`enclosing_symbol`、`quote_range`和`consumer_positions`逐条供主线复查；标签在函数体或单宏/typedef内，而不在extern声明中。旧49的36/36仍只作历史作者自报。
## C49-A01 · 独立的typedef，不混前向声明
[VERIFIED mykernel/kactive/kactive_type_declaration.h::completion_s]
```c
	typedef struct completion completion_s;
```
evidence_metadata: {"id":"C49-A01","actual_definition_kind":"typedef","enclosing_symbol":"completion_s","definition_range":"15-15","quote_range":"15-15","quote_supports_claim":true,"consumer_positions":["E01","W01"]}

## C49-A02 · wrapper真实long与定义头
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
		long
		wait_for_common(completion_s*x, long timeout, int state) {
```
evidence_metadata: {"id":"C49-A02","actual_definition_kind":"function","enclosing_symbol":"wait_for_common","definition_range":"70-80","quote_range":"71-72","quote_supports_claim":true,"consumer_positions":["E12","W02","T05","DOT.wrapper"]}

## C49-A03 · wrapper锁/callee schedule_timeout/解锁/return
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
			spin_lock_irq(&x->wait.lock);
			timeout = do_wait_for_common(x, schedule_timeout, timeout, state);
			spin_unlock_irq(&x->wait.lock);

			return timeout;
```
evidence_metadata: {"id":"C49-A03","actual_definition_kind":"function","enclosing_symbol":"wait_for_common","definition_range":"70-80","quote_range":"75-79","quote_supports_claim":true,"consumer_positions":["E12","W02","T05/T07","matrix.entry/public-return","DOT.lock/wunlock"]}

## C49-A04 · 实际do_wait的初始!done和局部wait
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
			if (!x->done) {
				DECLARE_SWAITQUEUE(wait);
```
evidence_metadata: {"id":"C49-A04","actual_definition_kind":"function","enclosing_symbol":"do_wait_for_common","definition_range":"42-68","quote_range":"47-48","quote_supports_claim":true,"consumer_positions":["E13","W02","T06","matrix.gate/slow","DOT.gate/fast/make"]}

## C49-A05 · 旧E12片段实际上属于do_wait循环与无done返回
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
					spin_lock_irq(&x->wait.lock);
				} while (!x->done && timeout);
				__finish_swait(&x->wait, &wait);
				if (!x->done)
					return timeout;
```
evidence_metadata: {"id":"C49-A05","actual_definition_kind":"function","enclosing_symbol":"do_wait_for_common","definition_range":"42-68","quote_range":"59-63","quote_supports_claim":true,"consumer_positions":["E12","T06/T07","matrix.exit"]}

## C49-A06 · 非饱和done消费与timeout ?: 1
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
			if (x->done != UINT_MAX)
				x->done--;
			return timeout ?: 1;
```
evidence_metadata: {"id":"C49-A06","actual_definition_kind":"function","enclosing_symbol":"do_wait_for_common","definition_range":"42-68","quote_range":"65-67","quote_supports_claim":true,"consumer_positions":["T07","matrix.exit"]}

## C49-A07 · public void入口调用long但不返还
[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
```
evidence_metadata: {"id":"C49-A07","actual_definition_kind":"function","enclosing_symbol":"wait_for_completion","definition_range":"53-55","quote_range":"53-55","quote_supports_claim":true,"consumer_positions":["T05/T07","matrix.entry/public-return","DOT.rvoid"]}

## ST50-A01 · 局部timer/task组合结构
[VERIFIED mykernel/time/timer/timer.c::proc_timer_s]
```c
typedef struct process_timer {
	timer_list_s	timer;
	task_s			*task;
} proc_timer_s;
```
evidence_metadata: {"id":"ST50-A01","actual_definition_kind":"typedef","enclosing_symbol":"proc_timer_s","definition_range":"32-35","quote_range":"32-35","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A02 · 回调容器回推及wake请求
[VERIFIED mykernel/time/timer/timer.c::process_timeout]
```c
static void
process_timeout(timer_list_s *t) {
	proc_timer_s *timeout = from_timer(timeout, t, timer);
	wake_up_process(timeout->task);
}
```
evidence_metadata: {"id":"ST50-A02","actual_definition_kind":"function","enclosing_symbol":"process_timeout","definition_range":"41-45","quote_range":"41-45","quote_supports_claim":true,"consumer_positions":["T02","DOT.callback"]}

## ST50-A03 · long参数及局部proc_timer_s/ulong
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
long __sched schedule_timeout(long timeout)
{
	proc_timer_s timer;
	ulong expire;
```
evidence_metadata: {"id":"ST50-A03","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"759-762","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A04 · MAX switch分支
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	switch (timeout)
	{
	case MAX_SCHEDULE_TIMEOUT:
```
evidence_metadata: {"id":"ST50-A04","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"764-766","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A05 · MAX实际schedule再goto out
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
		schedule();
		goto out;
```
evidence_metadata: {"id":"ST50-A05","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"774-775","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A06 · 负timeout分支恢复TASK_RUNNING
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
		if (timeout < 0) {
			// printk(KERN_ERR "schedule_timeout: wrong timeout "
			// 	"value %lx\n", timeout);
			// dump_stack();
			__set_current_state(TASK_RUNNING);
```
evidence_metadata: {"id":"ST50-A06","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"784-788","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A07 · 负值分支goto out
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
			__set_current_state(TASK_RUNNING);
			goto out;
		}
```
evidence_metadata: {"id":"ST50-A07","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"788-790","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A08 · expire使用timeout+jiffies
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	expire = timeout + jiffies;
```
evidence_metadata: {"id":"ST50-A08","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"793-793","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A09 · 有限分支timer设置/请求和注释掉的schedule
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer.task = current;
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```
evidence_metadata: {"id":"ST50-A09","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"795-799","quote_supports_claim":true,"consumer_positions":["T01/T02"]}

## ST50-A10 · 有限路径timer删除与栈销毁包装
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	del_timer_sync(&timer.timer);

	/* Remove the timer from the object tracker */
	destroy_timer_on_stack(&timer.timer);
```
evidence_metadata: {"id":"ST50-A10","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"799-802","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A11 · 统一return保持非负输入原值
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
 out:
	return timeout < 0 ? 0 : timeout;
}
```
evidence_metadata: {"id":"ST50-A11","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"804-806","quote_supports_claim":true,"consumer_positions":["T01/T03"]}

## ST50-A12 · 栈timer宏映射初始化入口
[VERIFIED mykernel/time/timer/timer_macro.h::timer_setup_on_stack]
```c
	#define timer_setup_on_stack(timer, callback, flags)	\
				simple_init_timer_key((timer), (callback), (flags))
```
evidence_metadata: {"id":"ST50-A12","actual_definition_kind":"macro","enclosing_symbol":"timer_setup_on_stack","definition_range":"21-22","quote_range":"21-22","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A13 · 删除sync宏别名
[VERIFIED mykernel/time/timer/timer_macro.h::del_timer_sync]
```c
	#define del_timer_sync	timer_delete_sync
```
evidence_metadata: {"id":"ST50-A13","actual_definition_kind":"macro","enclosing_symbol":"del_timer_sync","definition_range":"25-25","quote_range":"25-25","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A14 · 栈timer销毁包装空函数体
[VERIFIED mykernel/time/timer/timer_macro.h::destroy_timer_on_stack]
```c
	static inline void destroy_timer_on_stack(timer_list_s *timer) { }
```
evidence_metadata: {"id":"ST50-A14","actual_definition_kind":"function","enclosing_symbol":"destroy_timer_on_stack","definition_range":"27-27","quote_range":"27-27","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A15 · 回调中的container_of回推宏
[VERIFIED mykernel/time/timer/timer_macro.h::from_timer]
```c
	#define from_timer(var, callback_timer, timer_fieldname) \
				container_of(callback_timer, typeof(*var), timer_fieldname)
```
evidence_metadata: {"id":"ST50-A15","actual_definition_kind":"macro","enclosing_symbol":"from_timer","definition_range":"6-7","quote_range":"6-7","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A16 · timer_list entry/expires/function/flags成员
[VERIFIED mykernel/time/timer/timer_types.h::struct timer_list]
```c
		HList_s		entry;
		ulong		expires;
		void		(*function)(timer_list_s *);
		u32			flags;
```
evidence_metadata: {"id":"ST50-A16","actual_definition_kind":"struct","enclosing_symbol":"struct timer_list","definition_range":"70-83","quote_range":"75-78","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A17 · 实函数初始化链表节点与function指针
[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
void simple_init_timer_key(timer_list_s *timer,
		void (*func)(timer_list_s *), uint flags)
{
	timer->entry.pprev = NULL;
	timer->function = func;
```
evidence_metadata: {"id":"ST50-A17","actual_definition_kind":"function","enclosing_symbol":"simple_init_timer_key","definition_range":"713-722","quote_range":"713-717","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A18 · sync包装向下调用并返回int
[VERIFIED mykernel/time/timer/timer.c::timer_delete_sync]
```c
int timer_delete_sync(timer_list_s *timer)
{
	return __timer_delete_sync(timer, false);
}
```
evidence_metadata: {"id":"ST50-A18","actual_definition_kind":"function","enclosing_symbol":"timer_delete_sync","definition_range":"693-696","quote_range":"693-696","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A19 · 设置不可中断状态，返回被调long
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout_uninterruptible]
```c
long __sched schedule_timeout_uninterruptible(long timeout)
{
	__set_current_state(TASK_UNINTERRUPTIBLE);
	return schedule_timeout(timeout);
}
```
evidence_metadata: {"id":"ST50-A19","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout_uninterruptible","definition_range":"826-830","quote_range":"826-830","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A20 · msleep取得msecs_to_jiffies返回的ulong
[VERIFIED mykernel/time/timer/timer.c::msleep]
```c
void msleep(uint msecs)
{
	ulong timeout = msecs_to_jiffies(msecs);

```
evidence_metadata: {"id":"ST50-A20","actual_definition_kind":"function","enclosing_symbol":"msleep","definition_range":"852-858","quote_range":"852-855","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A21 · while循环赋回schedule_timeout返回
[VERIFIED mykernel/time/timer/timer.c::msleep]
```c
	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}
```
evidence_metadata: {"id":"ST50-A21","actual_definition_kind":"function","enclosing_symbol":"msleep","definition_range":"852-858","quote_range":"856-858","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A22 · 可中断状态包装对照
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout_interruptible]
```c
long __sched schedule_timeout_interruptible(long timeout)
{
	__set_current_state(TASK_INTERRUPTIBLE);
	return schedule_timeout(timeout);
}
```
evidence_metadata: {"id":"ST50-A22","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout_interruptible","definition_range":"812-816","quote_range":"812-816","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A23 · msecs转换常量分支入口
[VERIFIED mykernel/time/systick/systick.h::msecs_to_jiffies]
```c
		ulong
		msecs_to_jiffies(const uint m) {
			if (__builtin_constant_p(m)) {
				if ((int)m < 0)
					return MAX_JIFFY_OFFSET;
```
evidence_metadata: {"id":"ST50-A23","actual_definition_kind":"function","enclosing_symbol":"msecs_to_jiffies","definition_range":"113-123","quote_range":"114-118","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A24 · msecs转换两条返回路径
[VERIFIED mykernel/time/systick/systick.h::msecs_to_jiffies]
```c
				return _msecs_to_jiffies(m);
			} else {
				return __msecs_to_jiffies(m);
			}
		}
```
evidence_metadata: {"id":"ST50-A24","actual_definition_kind":"function","enclosing_symbol":"msecs_to_jiffies","definition_range":"113-123","quote_range":"119-123","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A25 · ulong定义为unsigned long
[VERIFIED mykernel/include/linux/kernel/types.h::ulong]
```c
		typedef unsigned long		ulong;
```
evidence_metadata: {"id":"ST50-A25","actual_definition_kind":"typedef","enclosing_symbol":"ulong","definition_range":"105-105","quote_range":"105-105","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A26 · MAX_SCHEDULE_TIMEOUT的固定源码定义
[VERIFIED mykernel/sched/task/task_const.h::MAX_SCHEDULE_TIMEOUT]
```c
	#define MAX_SCHEDULE_TIMEOUT	LONG_MAX
```
evidence_metadata: {"id":"ST50-A26","actual_definition_kind":"macro","enclosing_symbol":"MAX_SCHEDULE_TIMEOUT","definition_range":"86-86","quote_range":"86-86","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A27 · TASK_RUNNING的固定源码定义
[VERIFIED mykernel/sched/task/task_const.h::TASK_RUNNING]
```c
	#define TASK_RUNNING			0x00000000
```
evidence_metadata: {"id":"ST50-A27","actual_definition_kind":"macro","enclosing_symbol":"TASK_RUNNING","definition_range":"26-26","quote_range":"26-26","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A28 · TASK_UNINTERRUPTIBLE的固定源码定义
[VERIFIED mykernel/sched/task/task_const.h::TASK_UNINTERRUPTIBLE]
```c
	#define TASK_UNINTERRUPTIBLE	0x00000002
```
evidence_metadata: {"id":"ST50-A28","actual_definition_kind":"macro","enclosing_symbol":"TASK_UNINTERRUPTIBLE","definition_range":"28-28","quote_range":"28-28","quote_supports_claim":true,"consumer_positions":["T03"]}

## ST50-A29 · LONG_MAX的固定源码定义
[VERIFIED mykernel/include/linux/kernel/limits.h::LONG_MAX]
```c
	#define LONG_MAX		((long)(~0UL >> 1))
```
evidence_metadata: {"id":"ST50-A29","actual_definition_kind":"macro","enclosing_symbol":"LONG_MAX","definition_range":"33-33","quote_range":"33-33","quote_supports_claim":true,"consumer_positions":["T01"]}

## ST50-A30 · __mod_timer函数类型int
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
static inline int
__mod_timer(timer_list_s *timer, ulong expires, uint options) {
	ulong clk = 0, flags, bucket_expiry;
	timer_base_s *base, *new_base;
	uint idx = UINT_MAX;
```
evidence_metadata: {"id":"ST50-A30","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"213-217","quote_supports_claim":true,"consumer_positions":["T02"]}

## ST50-A31 · __mod_timer将ret作为返回
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
out_unlock:
	// raw_spin_unlock_irqrestore(&base->lock, flags);
	spin_unlock_irqrestore(&base->lock, flags);

	return ret;
```
evidence_metadata: {"id":"ST50-A31","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"333-337","quote_supports_claim":true,"consumer_positions":["T02"]}

## CONTEXT（不计新增证据）
旧49 E13来自DEBUG下extern声明区，并非do_wait实际定义体；E12错归wrapper，实为do_wait的重锁/循环/返回片段。E01中的struct completion;仅前向声明，实际typedef见C49-A01。jiffies读到的类型是extern声明，故不伪作已审变量定义：
```c
	extern ulong volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;
```
未证明链接布局、实时刻度、内存序或配置编译效果。
