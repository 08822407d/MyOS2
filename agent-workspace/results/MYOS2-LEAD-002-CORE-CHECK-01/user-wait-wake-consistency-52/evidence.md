---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-WAIT-WAKE-CONSISTENCY-52
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-06
model_per_owner: GPT-6
effort_per_owner: Extra
backend_identity: unknown_or_not_attestable
date: 2026-11-07
source_pin_short12: a039d9803ade
read_channel: GitHub_connector
record_type: source_evidence_ledger
runtime_evidence: NOT_RUN
mainline_acceptance: PENDING
---
# 第52批三专题统一证据账本
固定来源：time@a039d9803ade。各标签只取一个真实源码函数/结构/宏的1–5个原样相邻行；本次全部由连接器重新取得源文件并按行号直接切片。`quote_supports_claim`仅证明该段对限定命题的文字支持，不代替全体定义反事实、配置编译或实际运行。
## WW52-A01 · swait头有lock及带count的List_hdr_s
[VERIFIED mykernel/kactive/swait/swait_types.h::struct swait_queue_head]
```c
	struct swait_queue_head {
		spinlock_t	lock;
		List_hdr_s	task_list_hdr;
	};
```
proof: {"evidence_id":"WW52-A01","source_blob_short12":"5ce44773eda2","actual_definition_kind":"struct","enclosing_symbol":"struct swait_queue_head","definition_range":"7-10","quote_range":"7-10","quote_supports_claim":true,"consumer_positions":["A.object"]}

## WW52-A02 · List_hdr_s独立保存anchor和count
[VERIFIED mykernel/lib/list/double_list_types.h::struct list_hdr]
```c
	struct list_hdr {
		List_s		anchor;
		ulong		count;
	};
```
proof: {"evidence_id":"WW52-A02","source_blob_short12":"2ef93df3fe63","actual_definition_kind":"struct","enclosing_symbol":"struct list_hdr","definition_range":"13-16","quote_range":"13-16","quote_supports_claim":true,"consumer_positions":["A.invariant"]}

## WW52-A03 · 动态头初始化请求锁与counted list
[VERIFIED mykernel/kactive/swait/swait.c::__init_swait_queue_head]
```c
void __init_swait_queue_head(swqueue_hdr_s *q)
{
	spin_lock_init(&q->lock);
	INIT_LIST_HEADER_S(&q->task_list_hdr);
}
```
proof: {"evidence_id":"WW52-A03","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"__init_swait_queue_head","definition_range":"11-15","quote_range":"11-15","quote_supports_claim":true,"consumer_positions":["A.initialization"]}

## WW52-A04 · prepare按节点自指判断并追加到counted tail
[VERIFIED mykernel/kactive/swait/swait.c::__prepare_to_swait]
```c
void __prepare_to_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	wait->task = current;
	if (list_is_empty_entry(&wait->task_list))
		list_header_add_to_tail(&q->task_list_hdr, &wait->task_list);
}
```
proof: {"evidence_id":"WW52-A04","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"__prepare_to_swait","definition_range":"46-50","quote_range":"46-50","quote_supports_claim":true,"consumer_positions":["A.prepare"]}

## WW52-A05 · 入队先调用链表插入再将count加一
[VERIFIED mykernel/lib/list/double_list.h::list_header_add_to_tail]
```c
		list_header_add_to_tail(List_hdr_s *lhdr_p, List_s *l_p) {
			list_add_to_prev(l_p, &lhdr_p->anchor);
			lhdr_p->count++;
		}
```
proof: {"evidence_id":"WW52-A05","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_header_add_to_tail","definition_range":"662-667","quote_range":"664-667","quote_supports_claim":true,"consumer_positions":["A.count"]}

## WW52-A06 · wake是否为空只查header_is_empty
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
void swake_up_locked(swqueue_hdr_s *q, int wake_flags)
{
	swqueue_s *curr;
	if (list_header_is_empty(&q->task_list_hdr))
		return;
```
proof: {"evidence_id":"WW52-A06","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"swake_up_locked","definition_range":"23-31","quote_range":"23-27","quote_supports_claim":true,"consumer_positions":["A.wake_gate"]}

## WW52-A07 · wake取队首、请求唤醒后普通list_del_init
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_locked]
```c
	curr = list_headr_first_container(&q->task_list_hdr, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
```
proof: {"evidence_id":"WW52-A07","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"swake_up_locked","definition_range":"23-31","quote_range":"28-30","quote_supports_claim":true,"consumer_positions":["A.wake_unlink"]}

## WW52-A08 · 普通del_init摘链后自指，无header count修改
[VERIFIED mykernel/lib/list/double_list.h::list_del_init]
```c
		list_del_init(List_s *entry) {
			__list_del_entry(entry);
			INIT_LIST_HEAD(entry);
		}
```
proof: {"evidence_id":"WW52-A08","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_del_init","definition_range":"278-283","quote_range":"280-283","quote_supports_claim":true,"consumer_positions":["A.unlink"]}

## WW52-A09 · header empty只检count等于零
[VERIFIED mykernel/lib/list/double_list.h::list_header_is_empty]
```c
		list_header_is_empty(const List_hdr_s *lhdr_p) {
			return READ_ONCE(lhdr_p->count) == 0;
		}
```
proof: {"evidence_id":"WW52-A09","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_header_is_empty","definition_range":"628-632","quote_range":"630-632","quote_supports_claim":true,"consumer_positions":["A.counter_gate"]}

## WW52-A10 · finish先设RUNNING且仅节点非自指时调用counted删除
[VERIFIED mykernel/kactive/swait/swait.c::__finish_swait]
```c
void __finish_swait(swqueue_hdr_s *q, swqueue_s *wait) {
	__set_current_state(TASK_RUNNING);
	if (!list_is_empty_entry(&wait->task_list))
		list_header_delete_node(&q->task_list_hdr, &wait->task_list);
}
```
proof: {"evidence_id":"WW52-A10","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"__finish_swait","definition_range":"52-56","quote_range":"52-56","quote_supports_claim":true,"consumer_positions":["A.finish"]}

## WW52-A11 · counted删除先按链环实际成员扫描
[VERIFIED mykernel/lib/list/double_list.h::list_header_contains]
```c
		list_header_contains(List_hdr_s *lhdr_p, List_s *l_p) {
			List_s *tmp = NULL;
			list_header_foreach(tmp, lhdr_p) {
				if (tmp == l_p)
					return true;
```
proof: {"evidence_id":"WW52-A11","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_header_contains","definition_range":"634-642","quote_range":"635-639","quote_supports_claim":true,"consumer_positions":["A.member_gate"]}

## WW52-A12 · 成员存在才del_init并count--
[VERIFIED mykernel/lib/list/double_list.h::list_header_delete_node]
```c
		*list_header_delete_node(List_hdr_s *lhdr_p, List_s *l_p) {
			if (list_header_contains(lhdr_p, l_p)) {
				list_del_init(l_p);
				lhdr_p->count--;
				return l_p;
```
proof: {"evidence_id":"WW52-A12","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_header_delete_node","definition_range":"681-691","quote_range":"683-687","quote_supports_claim":true,"consumer_positions":["A.finish_remove"]}

## WW52-A13 · 普通节点next自指作为空/已摘判断
[VERIFIED mykernel/lib/list/double_list.h::list_is_empty_entry]
```c
		list_is_empty_entry(const List_s *head) {
			return READ_ONCE(head->next) == head;
		}
```
proof: {"evidence_id":"WW52-A13","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_is_empty_entry","definition_range":"428-432","quote_range":"430-432","quote_supports_claim":true,"consumer_positions":["A.selflink"]}

## WW52-A14 · 从anchor的next按member回推对象
[VERIFIED mykernel/lib/list/double_list_macro.h::list_headr_first_container]
```c
	#define list_headr_first_container(ptr, type, member) \
				list_first_entry(&((ptr)->anchor), type, member)
```
proof: {"evidence_id":"WW52-A14","source_blob_short12":"985a8262e578","actual_definition_kind":"macro","enclosing_symbol":"list_headr_first_container","definition_range":"334-335","quote_range":"334-335","quote_supports_claim":true,"consumer_positions":["A.first_container"]}

## WW52-A15 · 全部唤醒循环也仅依赖header_is_empty
[VERIFIED mykernel/kactive/swait/swait.c::swake_up_all_locked]
```c
void swake_up_all_locked(swqueue_hdr_s *q) {
	while (!list_header_is_empty(&q->task_list_hdr))
		swake_up_locked(q, 0);
}
```
proof: {"evidence_id":"WW52-A15","source_blob_short12":"b97d3e347bed","actual_definition_kind":"function","enclosing_symbol":"swake_up_all_locked","definition_range":"40-43","quote_range":"40-43","quote_supports_claim":true,"consumer_positions":["A.repeat"]}

## WW52-A16 · complete先增加done再请求swake并解锁
[VERIFIED mykernel/kactive/completion/completion.c::complete]
```c
	spin_lock_irqsave(&x->wait.lock, flags);
	if (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, 0);
	spin_unlock_irqrestore(&x->wait.lock, flags);
```
proof: {"evidence_id":"WW52-A16","source_blob_short12":"422ed6eee18b","actual_definition_kind":"function","enclosing_symbol":"complete","definition_range":"32-40","quote_range":"35-39","quote_supports_claim":true,"consumer_positions":["A.completion_producer"]}

## WW52-A17 · 普通semaphore __up摘队、置flag、发wake请求
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::__up]
```c
			list_first_entry(&sem->wait_list,
				struct semaphore_waiter, list);
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
```
proof: {"evidence_id":"WW52-A17","source_blob_short12":"bab1d368ce97","actual_definition_kind":"function","enclosing_symbol":"__up","definition_range":"203-211","quote_range":"206-210","quote_supports_claim":true,"consumer_positions":["A.contrast"]}

## WW52-A18 · 普通list_del不是counted删除
[VERIFIED mykernel/lib/list/double_list.h::list_del]
```c
		list_del(List_s *entry) {
			__list_del_entry(entry);
			entry->next = LIST_POISON1;
			entry->prev = LIST_POISON2;
		}
```
proof: {"evidence_id":"WW52-A18","source_blob_short12":"6e3e1aa8e724","actual_definition_kind":"function","enclosing_symbol":"list_del","definition_range":"267-273","quote_range":"269-273","quote_supports_claim":true,"consumer_positions":["A.plain_list"]}

## WW52-A19 · completion慢路prepare、状态、解锁、action及重锁
[VERIFIED mykernel/kactive/completion/completion.h::do_wait_for_common]
```c
					__prepare_to_swait(&x->wait, &wait);
					__set_current_state(state);
					spin_unlock_irq(&x->wait.lock);
					timeout = action(timeout);
					spin_lock_irq(&x->wait.lock);
```
proof: {"evidence_id":"WW52-A19","source_blob_short12":"7695d432946a","actual_definition_kind":"function","enclosing_symbol":"do_wait_for_common","definition_range":"42-68","quote_range":"55-59","quote_supports_claim":true,"consumer_positions":["A.wait_consumer"]}

## WW52-A20 · 普通List_s头只有next和prev
[VERIFIED mykernel/lib/list/double_list_types.h::struct list_head]
```c
	struct list_head {
		List_s		*next;
		List_s		*prev;
	};
```
proof: {"evidence_id":"WW52-A20","source_blob_short12":"2ef93df3fe63","actual_definition_kind":"struct","enclosing_symbol":"struct list_head","definition_range":"8-11","quote_range":"8-11","quote_supports_claim":true,"consumer_positions":["A.plain_head"]}

## WW52-A21 · counted header静态宏由自指anchor和count0构成
[VERIFIED mykernel/lib/list/double_list_macro.h::LIST_HEADER_INIT]
```c
	#define LIST_HEADER_INIT(name) {				\
				.anchor	= LIST_INIT(name.anchor),	\
				.count	= 0,						\
			}
```
proof: {"evidence_id":"WW52-A21","source_blob_short12":"985a8262e578","actual_definition_kind":"macro","enclosing_symbol":"LIST_HEADER_INIT","definition_range":"9-12","quote_range":"9-12","quote_supports_claim":true,"consumer_positions":["A.static_init"]}

## WW52-A22 · swait waiter保存task与普通节点
[VERIFIED mykernel/kactive/swait/swait_types.h::struct swait_queue]
```c
	struct swait_queue {
		task_s		*task;
		List_s		task_list;
	};
```
proof: {"evidence_id":"WW52-A22","source_blob_short12":"5ce44773eda2","actual_definition_kind":"struct","enclosing_symbol":"struct swait_queue","definition_range":"12-15","quote_range":"12-15","quote_supports_claim":true,"consumer_positions":["A.waiter"]}

## WW52-B01 · schedule_timeout的栈对象区分timer和task
[VERIFIED mykernel/time/timer/timer.c::proc_timer_s]
```c
typedef struct process_timer {
	timer_list_s	timer;
	task_s			*task;
} proc_timer_s;
```
proof: {"evidence_id":"WW52-B01","source_blob_short12":"753350236555","actual_definition_kind":"typedef","enclosing_symbol":"proc_timer_s","definition_range":"32-35","quote_range":"32-35","quote_supports_claim":true,"consumer_positions":["B.object"]}

## WW52-B02 · 回调根据container_of推回task并请求wake
[VERIFIED mykernel/time/timer/timer.c::process_timeout]
```c
static void
process_timeout(timer_list_s *t) {
	proc_timer_s *timeout = from_timer(timeout, t, timer);
	wake_up_process(timeout->task);
}
```
proof: {"evidence_id":"WW52-B02","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"process_timeout","definition_range":"41-45","quote_range":"41-45","quote_supports_claim":true,"consumer_positions":["B.callback"]}

## WW52-B03 · 栈timer setup宏映射simple_init_timer_key
[VERIFIED mykernel/time/timer/timer_macro.h::timer_setup_on_stack]
```c
	#define timer_setup_on_stack(timer, callback, flags)	\
				simple_init_timer_key((timer), (callback), (flags))
```
proof: {"evidence_id":"WW52-B03","source_blob_short12":"9def7fd021b6","actual_definition_kind":"macro","enclosing_symbol":"timer_setup_on_stack","definition_range":"21-22","quote_range":"21-22","quote_supports_claim":true,"consumer_positions":["B.setup"]}

## WW52-B04 · 初始化pprev/function并处理flags与CPU号
[VERIFIED mykernel/time/timer/timer.c::simple_init_timer_key]
```c
	timer->entry.pprev = NULL;
	timer->function = func;
	if (WARN_ON_ONCE(flags & ~TIMER_INIT_FLAGS))
		flags &= TIMER_INIT_FLAGS;
	timer->flags = flags | smp_processor_id();
```
proof: {"evidence_id":"WW52-B04","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"simple_init_timer_key","definition_range":"713-722","quote_range":"716-720","quote_supports_claim":true,"consumer_positions":["B.init_fields"]}

## WW52-B05 · pending从HList entry unhashed状态推导
[VERIFIED mykernel/time/timer/timer.h::timer_pending]
```c
		timer_pending(const timer_list_s * timer) {
			return !hlist_unhashed_lockless(&timer->entry);
		}
```
proof: {"evidence_id":"WW52-B05","source_blob_short12":"1e96e0e5aad5","actual_definition_kind":"function","enclosing_symbol":"timer_pending","definition_range":"60-64","quote_range":"62-64","quote_supports_claim":true,"consumer_positions":["B.pending"]}

## WW52-B06 · unhashed读取entry.pprev是否为空
[VERIFIED mykernel/lib/list/hash_list.h::hlist_unhashed_lockless]
```c
		hlist_unhashed_lockless(const HList_s *h) {
			return !READ_ONCE(h->pprev);
		}
```
proof: {"evidence_id":"WW52-B06","source_blob_short12":"91330c9d43a6","actual_definition_kind":"function","enclosing_symbol":"hlist_unhashed_lockless","definition_range":"107-111","quote_range":"109-111","quote_supports_claim":true,"consumer_positions":["B.pending_type"]}

## WW52-B07 · nonpending返回零
[VERIFIED mykernel/time/timer/timer.c::detach_if_pending]
```c
	uint idx = timer_get_idx(timer);

	if (!timer_pending(timer))
		return 0;
```
proof: {"evidence_id":"WW52-B07","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"detach_if_pending","definition_range":"155-171","quote_range":"159-162","quote_supports_claim":true,"consumer_positions":["B.detach_notpending"]}

## WW52-B08 · pending经过detach_timer后返回一
[VERIFIED mykernel/time/timer/timer.c::detach_if_pending]
```c
		base->next_expiry_recalc = true;
	}

	detach_timer(timer, clear_pending);
	return 1;
```
proof: {"evidence_id":"WW52-B08","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"detach_if_pending","definition_range":"155-171","quote_range":"166-170","quote_supports_claim":true,"consumer_positions":["B.detach_pending"]}

## WW52-B09 · clear_pending为真时设置pprev=NULL并毒化next
[VERIFIED mykernel/time/timer/timer.h::detach_timer]
```c
			__hlist_del(entry);
			if (clear_pending)
				entry->pprev = NULL;
			entry->next = LIST_POISON2;
```
proof: {"evidence_id":"WW52-B09","source_blob_short12":"1e96e0e5aad5","actual_definition_kind":"function","enclosing_symbol":"detach_timer","definition_range":"67-78","quote_range":"74-77","quote_supports_claim":true,"consumer_positions":["B.detach_flag"]}

## WW52-B10 · NOTPENDING选项使早期pending优化分支不进入
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (!(options & MOD_TIMER_NOTPENDING) && timer_pending(timer)) {
```
proof: {"evidence_id":"WW52-B10","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"227-227","quote_supports_claim":true,"consumer_positions":["B.options_gate"]}

## WW52-B11 · 替代分支检查function非NULL并forward base
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		if (!timer->function)
			goto out_unlock;

		forward_timer_base(base);
```
proof: {"evidence_id":"WW52-B11","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"286-289","quote_supports_claim":true,"consumer_positions":["B.function_gate"]}

## WW52-B12 · 普通登记先detach_if_pending(false)，可转向本CPU base
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	ret = detach_if_pending(timer, base, false);
	if (!ret && (options & MOD_TIMER_PENDING_ONLY))
		goto out_unlock;

	new_base = get_timer_this_cpu_base(timer->flags);
```
proof: {"evidence_id":"WW52-B12","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"292-296","quote_supports_claim":true,"consumer_positions":["B.mod_detach"]}

## WW52-B13 · 不同base时running_timer门决定是否迁移
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
		 */
		if (likely(base->running_timer != timer)) {
			/* See the comment in lock_timer_base() */
			timer->flags |= TIMER_MIGRATING;

```
proof: {"evidence_id":"WW52-B13","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"305-309","quote_supports_claim":true,"consumer_positions":["B.migration_gate"]}

## WW52-B14 · 迁移片段的unlock/relock和flags CPU位写入
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
			spin_unlock(&base->lock);
			base = new_base;
			spin_lock(&base->lock);
			WRITE_ONCE(timer->flags,
				(timer->flags & ~TIMER_BASEMASK) | base->cpu);
```
proof: {"evidence_id":"WW52-B14","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"310-314","quote_supports_claim":true,"consumer_positions":["B.migration"]}

## WW52-B15 · timer.expires被实际赋值
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	timer->expires = expires;
```
proof: {"evidence_id":"WW52-B15","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"321-321","quote_supports_claim":true,"consumer_positions":["B.expires"]}

## WW52-B16 · 按idx/clk选择enqueue或internal_add_timer
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
	if (idx != UINT_MAX && clk == base->clk)
		enqueue_timer(base, timer, idx, bucket_expiry);
	else
		internal_add_timer(base, timer);
```
proof: {"evidence_id":"WW52-B16","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"328-331","quote_supports_claim":true,"consumer_positions":["B.enqueue_choice"]}

## WW52-B17 · mod调用尾返回ret并解锁
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
out_unlock:
	// raw_spin_unlock_irqrestore(&base->lock, flags);
	spin_unlock_irqrestore(&base->lock, flags);

	return ret;
```
proof: {"evidence_id":"WW52-B17","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"333-337","quote_supports_claim":true,"consumer_positions":["B.mod_return"]}

## WW52-B18 · base锁和timer flags重核
[VERIFIED mykernel/time/timer/timer.c::lock_timer_base]
```c
		if (!(tf & TIMER_MIGRATING)) {
			base = get_timer_base(tf);
			// raw_spin_lock_irqsave(&base->lock, *flags);
			spin_lock_irqsave(&base->lock, *flags);
			if (timer->flags == tf)
```
proof: {"evidence_id":"WW52-B18","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"lock_timer_base","definition_range":"184-210","quote_range":"199-203","quote_supports_claim":true,"consumer_positions":["B.base_lock"]}

## WW52-B19 · internally calculates wheel index then enqueue
[VERIFIED mykernel/time/timer/timer.c::internal_add_timer]
```c
internal_add_timer(timer_base_s *base, timer_list_s *timer) {
	ulong bucket_expiry;
	uint idx = calc_wheel_index(timer->expires,
					base->clk, &bucket_expiry);
	enqueue_timer(base, timer, idx, bucket_expiry);
```
proof: {"evidence_id":"WW52-B19","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"internal_add_timer","definition_range":"146-152","quote_range":"147-151","quote_supports_claim":true,"consumer_positions":["B.enqueue_boundary"]}

## WW52-B20 · enqueue实际hlist_add_head和timer_set_idx
[VERIFIED mykernel/time/timer/timer.c::enqueue_timer]
```c
	hlist_add_head(&timer->entry, base->vectors + idx);
	// __set_bit(idx, base->pending_map);
	timer_set_idx(timer, idx);
```
proof: {"evidence_id":"WW52-B20","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"enqueue_timer","definition_range":"119-144","quote_range":"123-125","quote_supports_claim":true,"consumer_positions":["B.enqueue_link"]}

## WW52-B21 · hlist add向head写first及pprev链
[VERIFIED mykernel/lib/list/hash_list.h::hlist_add_to_head]
```c
			WRITE_ONCE(n->next, first);
			if (first)
				WRITE_ONCE(first->pprev, &n->next);
			WRITE_ONCE(h->first, n);
			WRITE_ONCE(n->pprev, &h->first);
```
proof: {"evidence_id":"WW52-B21","source_blob_short12":"91330c9d43a6","actual_definition_kind":"function","enclosing_symbol":"hlist_add_to_head","definition_range":"172-181","quote_range":"176-180","quote_supports_claim":true,"consumer_positions":["B.hlist"]}

## WW52-B22 · 取消尝试初始ret=-1
[VERIFIED mykernel/time/timer/timer.c::__try_to_del_timer_sync]
```c
__try_to_del_timer_sync(timer_list_s *timer, bool shutdown) {
	timer_base_s *base;
	ulong flags;
	int ret = -1;
```
proof: {"evidence_id":"WW52-B22","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__try_to_del_timer_sync","definition_range":"564-583","quote_range":"565-568","quote_supports_claim":true,"consumer_positions":["B.try_init"]}

## WW52-B23 · running_timer非本timer才detach，shutdown为真才置NULL
[VERIFIED mykernel/time/timer/timer.c::__try_to_del_timer_sync]
```c
	if (base->running_timer != timer)
		ret = detach_if_pending(timer, base, true);
	if (shutdown)
		timer->function = NULL;

```
proof: {"evidence_id":"WW52-B23","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__try_to_del_timer_sync","definition_range":"564-583","quote_range":"574-578","quote_supports_claim":true,"consumer_positions":["B.running_gate"]}

## WW52-B24 · 删除循环调用__try并在负结果时进入分支
[VERIFIED mykernel/time/timer/timer.c::__timer_delete_sync]
```c
	do {
		ret = __try_to_del_timer_sync(timer, shutdown);

		if (unlikely(ret < 0)) {
			// del_timer_wait_running(timer);
```
proof: {"evidence_id":"WW52-B24","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__timer_delete_sync","definition_range":"629-649","quote_range":"639-643","quote_supports_claim":true,"consumer_positions":["B.retry"]}

## WW52-B25 · 负结果cpu_relax并重复，非负结束
[VERIFIED mykernel/time/timer/timer.c::__timer_delete_sync]
```c
		if (unlikely(ret < 0)) {
			// del_timer_wait_running(timer);
			cpu_relax();
		}
	} while (ret < 0);
```
proof: {"evidence_id":"WW52-B25","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__timer_delete_sync","definition_range":"629-649","quote_range":"642-646","quote_supports_claim":true,"consumer_positions":["B.retry_end"]}

## WW52-B26 · 调用者timer_delete_sync固定传shutdown=false
[VERIFIED mykernel/time/timer/timer.c::timer_delete_sync]
```c
int timer_delete_sync(timer_list_s *timer)
{
	return __timer_delete_sync(timer, false);
}
```
proof: {"evidence_id":"WW52-B26","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"timer_delete_sync","definition_range":"693-696","quote_range":"693-696","quote_supports_claim":true,"consumer_positions":["B.shutdown_choice"]}

## WW52-B27 · finite caller setup、mod、注释schedule、del请求
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	timer.task = current;
	timer_setup_on_stack(&timer.timer, process_timeout, 0);
	__mod_timer(&timer.timer, expire, MOD_TIMER_NOTPENDING);
	// schedule();
	del_timer_sync(&timer.timer);
```
proof: {"evidence_id":"WW52-B27","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"795-799","quote_supports_claim":true,"consumer_positions":["B.direct_caller"]}

## WW52-B28 · destroy wrapper与原timeout返回
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
	destroy_timer_on_stack(&timer.timer);

 out:
	return timeout < 0 ? 0 : timeout;
```
proof: {"evidence_id":"WW52-B28","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"802-805","quote_supports_claim":true,"consumer_positions":["B.normal_return"]}

## WW52-B29 · mod入口ret初始化零、idx初始化UINT_MAX
[VERIFIED mykernel/time/timer/timer.c::__mod_timer]
```c
__mod_timer(timer_list_s *timer, ulong expires, uint options) {
	ulong clk = 0, flags, bucket_expiry;
	timer_base_s *base, *new_base;
	uint idx = UINT_MAX;
	int ret = 0;
```
proof: {"evidence_id":"WW52-B29","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"__mod_timer","definition_range":"213-338","quote_range":"214-218","quote_supports_claim":true,"consumer_positions":["B.idx_initial"]}

## WW52-C01 · 公开void只传MAX和TASK_UNINTERRUPTIBLE
[VERIFIED mykernel/kactive/completion/completion.c::wait_for_completion]
```c
void wait_for_completion(completion_s *x) {
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
```
proof: {"evidence_id":"WW52-C01","source_blob_short12":"422ed6eee18b","actual_definition_kind":"function","enclosing_symbol":"wait_for_completion","definition_range":"53-55","quote_range":"53-55","quote_supports_claim":true,"consumer_positions":["C.public_void"]}

## WW52-C02 · wrapper将schedule_timeout指针传do_wait并返回long
[VERIFIED mykernel/kactive/completion/completion.h::wait_for_common]
```c
			spin_lock_irq(&x->wait.lock);
			timeout = do_wait_for_common(x, schedule_timeout, timeout, state);
			spin_unlock_irq(&x->wait.lock);

			return timeout;
```
proof: {"evidence_id":"WW52-C02","source_blob_short12":"7695d432946a","actual_definition_kind":"function","enclosing_symbol":"wait_for_common","definition_range":"70-80","quote_range":"75-79","quote_supports_claim":true,"consumer_positions":["C.wrapper"]}

## WW52-C03 · semaphore有限action赋回、重锁再查flag
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.h::___down_common]
```c
				__set_current_state(state);
				spin_unlock_irq(&sem->lock);
				timeout = schedule_timeout(timeout);
				spin_lock_irq(&sem->lock);
				if (waiter.up)
```
proof: {"evidence_id":"WW52-C03","source_blob_short12":"5f4a3fbd2ecb","actual_definition_kind":"function","enclosing_symbol":"___down_common","definition_range":"41-70","quote_range":"55-59","quote_supports_claim":true,"consumer_positions":["C.semaphore"]}

## WW52-C04 · semaphore入口count快路径或慢路
[VERIFIED mykernel/lock_IPC/semaphore/semaphore.c::down_timeout]
```c
	if (likely(sem->count > 0))
		sem->count--;
	else
		result = __down_timeout(sem, timeout);
```
proof: {"evidence_id":"WW52-C04","source_blob_short12":"bab1d368ce97","actual_definition_kind":"function","enclosing_symbol":"down_timeout","definition_range":"135-151","quote_range":"143-146","quote_supports_claim":true,"consumer_positions":["C.down_timeout"]}

## WW52-C05 · MAX分支直接调用schedule
[VERIFIED mykernel/time/timer/timer.c::schedule_timeout]
```c
		schedule();
		goto out;
```
proof: {"evidence_id":"WW52-C05","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"schedule_timeout","definition_range":"759-806","quote_range":"774-775","quote_supports_claim":true,"consumer_positions":["C.max_action"]}

## WW52-C06 · msleep while赋回不可中断schedule_timeout值
[VERIFIED mykernel/time/timer/timer.c::msleep]
```c
	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}
```
proof: {"evidence_id":"WW52-C06","source_blob_short12":"753350236555","actual_definition_kind":"function","enclosing_symbol":"msleep","definition_range":"852-858","quote_range":"856-858","quote_supports_claim":true,"consumer_positions":["C.msleep"]}

## WW52-C07 · request路径发wake请求并等待req.done
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c

	wake_up_process(thread);
	wait_for_completion(&req->done);
```
proof: {"evidence_id":"WW52-C07","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"100-102","quote_supports_claim":true,"consumer_positions":["C.devreq_wait"]}

## WW52-C08 · request等待后读取req->err
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_submit_req]
```c
	kfree((void *)tmp);

	return req->err;
```
proof: {"evidence_id":"WW52-C08","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_submit_req","definition_range":"92-107","quote_range":"104-106","quote_supports_claim":true,"consumer_positions":["C.devreq_return"]}

## WW52-C09 · worker设置req.err、complete后kfree(req)
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_work_loop]
```c
				req->err = handle(req->name, req->mode,
						req->uid, req->gid, req->dev);
				complete(&req->done);
				kfree(req);
				req = next;
```
proof: {"evidence_id":"WW52-C09","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_work_loop","definition_range":"355-377","quote_range":"365-369","quote_supports_claim":true,"consumer_positions":["C.devreq_producer"]}

## WW52-C10 · 挂载错误早退在setup_done通知之前
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
		err = init_mount("devtmpfs", "/", "devtmpfs", DEVTMPFS_MFLAGS);
		if (err)
			goto out;
```
proof: {"evidence_id":"WW52-C10","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfsd","definition_range":"384-403","quote_range":"390-392","quote_supports_claim":true,"consumer_positions":["C.setup_early_exit"]}

## WW52-C11 · worker通知setup_done后进入工作循环
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfsd]
```c
	complete(&setup_done);
	devtmpfs_work_loop();
```
proof: {"evidence_id":"WW52-C11","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfsd","definition_range":"384-403","quote_range":"399-400","quote_supports_claim":true,"consumer_positions":["C.setup_producer"]}

## WW52-C12 · creator只在非ERR线程通道请求等待setup_done
[VERIFIED mykernel/drivers/base/devtmpfs.c::devtmpfs_init]
```c
	thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	if (!IS_ERR(thread)) {
		wait_for_completion(&setup_done);
```
proof: {"evidence_id":"WW52-C12","source_blob_short12":"633678e473c0","actual_definition_kind":"function","enclosing_symbol":"devtmpfs_init","definition_range":"409-445","quote_range":"427-429","quote_supports_claim":true,"consumer_positions":["C.setup_consumer"]}

## WW52-C13 · kthread创建方等待栈done后读取result
[VERIFIED mykernel/sched/forkexec/kthread.c::__kthread_create_on_node]
```c
		 * shortly.
		 */
		wait_for_completion(&done);
	// }
	task = create->result;
```
proof: {"evidence_id":"WW52-C13","source_blob_short12":"4782afc6b3f2","actual_definition_kind":"function","enclosing_symbol":"__kthread_create_on_node","definition_range":"238-288","quote_range":"280-284","quote_supports_claim":true,"consumer_positions":["C.kthread_wait"]}

## WW52-C14 · kthread发布done并调用schedule_preempt_disabled
[VERIFIED mykernel/sched/forkexec/kthread.c::kthread]
```c
	preempt_disable();
	complete(done);
	schedule_preempt_disabled();
```
proof: {"evidence_id":"WW52-C14","source_blob_short12":"4782afc6b3f2","actual_definition_kind":"function","enclosing_symbol":"kthread","definition_range":"164-222","quote_range":"203-205","quote_supports_claim":true,"consumer_positions":["C.kthread_producer"]}

## WW52-C15 · rest_init通知kthreadd_done
[VERIFIED mykernel/init/main.c::rest_init]
```c
	complete(&kthreadd_done);
```
proof: {"evidence_id":"WW52-C15","source_blob_short12":"06a900f8d509","actual_definition_kind":"function","enclosing_symbol":"rest_init","definition_range":"82-129","quote_range":"120-120","quote_supports_claim":true,"consumer_positions":["C.init_producer"]}

## WW52-C16 · kernel_init等待kthreadd_done
[VERIFIED mykernel/init/main.c::kernel_init]
```c
	 * Wait until kthreadd is all set-up.
	 */
	wait_for_completion(&kthreadd_done);
```
proof: {"evidence_id":"WW52-C16","source_blob_short12":"06a900f8d509","actual_definition_kind":"function","enclosing_symbol":"kernel_init","definition_range":"270-365","quote_range":"275-277","quote_supports_claim":true,"consumer_positions":["C.init_consumer"]}

## CONTEXT / 限制
未把DEBUG extern前置声明当作函数体。`__mod_timer`、`schedule_timeout`等“全函数没有某赋值/调用”的结论基于本批读取的完整固定活动体，而不是单条短引文。对象身份和原生类型限定、源码注释与活跃语句区别、编译选项/宏分支不运行验证。
