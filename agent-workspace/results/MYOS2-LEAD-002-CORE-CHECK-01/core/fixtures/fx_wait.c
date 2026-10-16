/* fx_wait: host fixture for V02, V03, V09, V10, V11 (swait / completion / schedule_timeout).
 * Original MyOS2 code is inserted verbatim by build.py at every //@@ORIG line.
 * Hand-written replacements (see evidence.md "替换依赖"):
 *   task_s (only __state + fixture id), current, try_to_wake_up/wake_up_process (record call,
 *   validate pointer, set TASK_RUNNING only), schedule (count; optional scripted notifier),
 *   interrupt-flag/preempt hooks, timer_list_s layout, simple_init_timer_key / __mod_timer /
 *   timer_delete_sync (record only; __mod_timer enforces the 100-step cap), jiffies (fixed, never
 *   ticks), msecs_to_jiffies (identity), __sched (empty), signal_pending/__fatal_signal_pending (0).
 */
#include "fx_common.h"

//@@INCLUDE fx_list.inc.c

/* ---------------- lock: original layout and original ticket asm, irq/preempt hooks stubbed */
typedef struct atomic64 atomic64_t;
typedef struct tspinlock arch_spinlock_t;
typedef struct tspinlock spinlock_t;
//@@ORIG time mykernel/arch/x86_64/lock_IPC/atomic/atomic_types_arch.h struct atomic64
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h struct tspinlock
//@@ORIG time mykernel/arch/x86_64/include/asm/alternative.h macro LOCK_PREFIX_HERE
//@@ORIG time mykernel/arch/x86_64/include/asm/alternative.h macro LOCK_PREFIX
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_macro_arch.h macro __ARCH_SPIN_LOCK_UNLOCKED
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_init
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_lock
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_unlock
static int g_preempt;
#define local_irq_save(flags) do { (flags) = 0x200; } while (0)
#define local_irq_restore(flags) do { (void)(flags); } while (0)
#define local_irq_disable() do { } while (0)
#define local_irq_enable() do { } while (0)
#define preempt_disable() do { g_preempt++; } while (0)
#define preempt_enable() do { g_preempt--; } while (0)
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp.h func raw_spin_lock_irqsave
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp.h func raw_spin_lock_irq
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp.h func raw_spin_unlock_irqrestore
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp.h func raw_spin_unlock_irq
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro raw_spin_lock_init
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro spin_lock_init
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro spin_lock_irqsave
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro spin_unlock_irqrestore
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro spin_lock_irq
//@@ORIG time mykernel/lock_IPC/spinlock/spinlock_smp_macro.h macro spin_unlock_irq

/* ---------------- tasks and state */
typedef struct task task_s;
struct task { uint __state; int id; };
static task_s T_W1 = {0, 1}, T_W2 = {0, 2}, T_N = {0, 9};
static task_s *g_current = &T_W1;
#define current g_current
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_RUNNING
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_INTERRUPTIBLE
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_UNINTERRUPTIBLE
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_WAKEKILL
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_NORMAL
//@@ORIG time mykernel/sched/task/task_const.h macro MAX_SCHEDULE_TIMEOUT
//@@ORIG time mykernel/sched/runqueue/runqueue_macro.h macro __set_current_state
//@@ORIG time mykernel/include/linux/lib/errno.h macro ERESTARTSYS

static spinlock_t *g_watch_lock;
static int g_ttwu_calls, g_sched_calls, g_mod_calls, g_del_calls, g_init_calls;
static long g_first_timeouts[3];
static long g_last_timeout;

int try_to_wake_up(task_s *p, uint state, int wake_flags)
{
	g_ttwu_calls++;
	if (p != &T_W1 && p != &T_W2 && p != &T_N) {
		unsigned long v = (unsigned long)p;
		unsigned long lw = g_watch_lock ? (unsigned long)g_watch_lock->val.counter : 0;
		STOP(EXIT_STOP_INVARIANT,
		     "\"ev\":\"ttwu_invalid_task\",\"call\":%d,\"task_ptr_value\":%lu,"
		     "\"watched_lock_word\":%lu,\"equals_watched_lock_word\":%s",
		     g_ttwu_calls, v, lw, (g_watch_lock && v == lw) ? "true" : "false");
	}
	ev("\"ev\":\"ttwu\",\"call\":%d,\"task\":%d,\"state_before\":%u,\"mask\":%u",
	   g_ttwu_calls, p->id, p->__state, state);
	p->__state = TASK_RUNNING;
	return 0;
}
int wake_up_process(task_s *p) { return try_to_wake_up(p, TASK_NORMAL, 0); }
static int signal_pending(task_s *p) { (void)p; return 0; }
static int __fatal_signal_pending(task_s *p) { (void)p; return 0; }
//@@ORIG time mykernel/lock_IPC/signal/signal.h func signal_pending_state

/* ---------------- swait and completion (original) */
typedef struct swait_queue_head swqueue_hdr_s;
typedef struct swait_queue swqueue_s;
typedef struct completion completion_s;
//@@ORIG time mykernel/kactive/swait/swait_types.h struct swait_queue_head
//@@ORIG time mykernel/kactive/swait/swait_types.h struct swait_queue
//@@ORIG time mykernel/kactive/completion/completion_types.h struct completion
//@@ORIG time mykernel/kactive/swait/swait_macro.h macro __SWAITQUEUE_INITIALIZER
//@@ORIG time mykernel/kactive/swait/swait_macro.h macro DECLARE_SWAITQUEUE
//@@ORIG time mykernel/kactive/swait/swait_macro.h macro init_swait_queue_head
//@@ORIG time mykernel/kactive/swait/swait.c func __init_swait_queue_head
//@@ORIG time mykernel/kactive/swait/swait.c func swake_up_locked
//@@ORIG time mykernel/kactive/swait/swait.c func swake_up_all_locked
//@@ORIG time mykernel/kactive/swait/swait.c func __prepare_to_swait
//@@ORIG time mykernel/kactive/swait/swait.c func __finish_swait
//@@ORIG time mykernel/kactive/completion/completion.c func complete

/* ---------------- schedule / timer replacements, then original schedule_timeout family */
static void (*g_on_schedule)(void);
void schedule(void)
{
	g_sched_calls++;
	ev("\"ev\":\"schedule\",\"call\":%d,\"current\":%d,\"current_state\":%u",
	   g_sched_calls, current->id, current->__state);
	if (g_sched_calls > g_step_cap)
		STOP(EXIT_STOP_STEPCAP, "\"ev\":\"step_cap\",\"where\":\"schedule\",\"cap\":%d", g_step_cap);
	if (g_on_schedule)
		g_on_schedule();
}
typedef struct timer_list timer_list_s;
struct timer_list { void (*function)(timer_list_s *); ulong expires; };
static volatile ulong jiffies = 1000;
#define __sched
void simple_init_timer_key(timer_list_s *t, void (*f)(timer_list_s *), uint flags)
{ (void)flags; g_init_calls++; t->function = f; }
static inline int __mod_timer(timer_list_s *timer, ulong expires, uint options)
{
	(void)options;
	timer->expires = expires;
	g_last_timeout = (long)(expires - jiffies);
	if (g_mod_calls < 3)
		g_first_timeouts[g_mod_calls] = g_last_timeout;
	g_mod_calls++;
	if (g_mod_calls >= g_step_cap)
		STOP(EXIT_STOP_STEPCAP, "\"ev\":\"step_cap\",\"where\":\"__mod_timer\",\"cap\":%d,"
		     "\"first_timeouts\":[%ld,%ld,%ld],\"last_timeout\":%ld,\"sched_calls\":%d,"
		     "\"current_state\":%u", g_step_cap, g_first_timeouts[0], g_first_timeouts[1],
		     g_first_timeouts[2], g_last_timeout, g_sched_calls, current->__state);
	return 0;
}
int timer_delete_sync(timer_list_s *t) { (void)t; g_del_calls++; return 0; }
static ulong msecs_to_jiffies(const uint m) { return m; }
//@@ORIG time mykernel/time/timer/timer_const.h macro MOD_TIMER_NOTPENDING
//@@ORIG time mykernel/time/timer/timer_macro.h macro from_timer
//@@ORIG time mykernel/time/timer/timer_macro.h macro timer_setup_on_stack
//@@ORIG time mykernel/time/timer/timer_macro.h macro del_timer_sync
//@@ORIG time mykernel/time/timer/timer_macro.h func destroy_timer_on_stack
//@@ORIG time mykernel/time/timer/timer.c struct process_timer
//@@ORIG time mykernel/time/timer/timer.c func process_timeout
//@@ORIG time mykernel/time/timer/timer.c func schedule_timeout
//@@ORIG time mykernel/time/timer/timer.c func schedule_timeout_uninterruptible
//@@ORIG time mykernel/time/timer/timer.c func msleep
//@@ORIG time mykernel/kactive/completion/completion.h func do_wait_for_common
//@@ORIG time mykernel/kactive/completion/completion.h func wait_for_common
//@@ORIG time mykernel/kactive/completion/completion.c func wait_for_completion

/* ---------------- observers */
static const char *tf(bool b) { return b ? "true" : "false"; }
static void snap(const char *step, swqueue_hdr_s *q, swqueue_s *a, swqueue_s *b)
{
	List_hdr_s *h = &q->task_list_hdr;
	int walk = fx_walk_len(h);
	ev("\"ev\":\"snap\",\"step\":\"%s\",\"count\":%lu,\"walk_len\":%d,\"count_equals_nodes\":%s,"
	   "\"anchor_self\":%s,\"header_is_empty\":%s,\"a_self\":%s,\"a_occ\":%d,\"b_self\":%s,\"b_occ\":%d,"
	   "\"lock_word\":%lld,\"preempt\":%d",
	   step, h->count, walk, tf(walk >= 0 && (ulong)walk == h->count), tf(h->anchor.next == &h->anchor),
	   tf(list_header_is_empty(h)), a ? tf(list_is_empty_entry(&a->task_list)) : "null",
	   a ? fx_occurrences(h, &a->task_list) : -1, b ? tf(list_is_empty_entry(&b->task_list)) : "null",
	   b ? fx_occurrences(h, &b->task_list) : -1, (long long)q->lock.val.counter, g_preempt);
}
static void layout(swqueue_hdr_s *q)
{
	char *anchor_container = (char *)&q->task_list_hdr.anchor - offsetof(swqueue_s, task_list);
	char *task_field = anchor_container + offsetof(swqueue_s, task);
	ev("\"ev\":\"layout\",\"sizeof_lock\":%zu,\"offsetof_hdr_task_list_hdr\":%zu,"
	   "\"offsetof_waiter_task_list\":%zu,\"anchor_container_task_field_aliases_lock\":%s",
	   sizeof(spinlock_t), offsetof(swqueue_hdr_s, task_list_hdr), offsetof(swqueue_s, task_list),
	   tf(task_field == (char *)&q->lock));
}

static completion_s *g_x;
static void notifier_once(void)
{
	task_s *saved = current;
	g_on_schedule = NULL;
	ev("\"ev\":\"notifier_start\",\"waiter_state\":%u,\"done\":%u,\"count\":%lu,\"walk_len\":%d",
	   saved->__state, g_x->done, g_x->wait.task_list_hdr.count, fx_walk_len(&g_x->wait.task_list_hdr));
	current = &T_N;
	complete(g_x);
	current = saved;
	ev("\"ev\":\"notifier_end\",\"waiter_state\":%u,\"done\":%u,\"count\":%lu,\"walk_len\":%d",
	   saved->__state, g_x->done, g_x->wait.task_list_hdr.count, fx_walk_len(&g_x->wait.task_list_hdr));
}

/* ---------------- cases */
static void v02_prefix(swqueue_hdr_s *q, swqueue_s *w)
{
	init_swait_queue_head(q);
	g_watch_lock = &q->lock;
	layout(q);
	snap("init", q, w, NULL);
	current = &T_W1;
	__prepare_to_swait(q, w);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	snap("after_prepare", q, w, NULL);
	current = &T_N;
	swake_up_locked(q, 0);
	snap("after_wake", q, w, NULL);
	current = &T_W1;
	__finish_swait(q, w);
	snap("after_finish", q, w, NULL);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 2;
	g_case = argv[1];
	if (!strcmp(g_case, "v02_single")) {
		swqueue_hdr_s q;
		current = &T_W1;
		DECLARE_SWAITQUEUE(w);
		v02_prefix(&q, &w);
		ev("\"ev\":\"end\",\"ttwu_calls\":%d", g_ttwu_calls);
	} else if (!strcmp(g_case, "v03_second_wake_direct")) {
		swqueue_hdr_s q;
		current = &T_W1;
		DECLARE_SWAITQUEUE(w);
		v02_prefix(&q, &w);
		current = &T_N;
		ev("\"ev\":\"second_wake_begin\",\"lock_held\":false");
		swake_up_locked(&q, 0);
		snap("after_second_wake", &q, &w, NULL);
	} else if (!strcmp(g_case, "v03_second_wake_via_complete")) {
		completion_s x;
		x.done = 0; init_swait_queue_head(&x.wait);
		g_watch_lock = &x.wait.lock;
		current = &T_W1;
		DECLARE_SWAITQUEUE(w);
		__prepare_to_swait(&x.wait, &w);
		current = &T_N; complete(&x);
		snap("after_first_complete", &x.wait, &w, NULL);
		current = &T_W1; __finish_swait(&x.wait, &w);
		if (x.done) x.done--;
		snap("after_waiter_finish", &x.wait, &w, NULL);
		current = &T_N;
		ev("\"ev\":\"second_complete_begin\",\"lock_held_inside\":true");
		complete(&x);
		snap("after_second_complete", &x.wait, &w, NULL);
	} else if (!strcmp(g_case, "v03_all_two_waiters")) {
		swqueue_hdr_s q;
		init_swait_queue_head(&q); g_watch_lock = &q.lock;
		current = &T_W1;
		DECLARE_SWAITQUEUE(a);
		__prepare_to_swait(&q, &a);
		current = &T_W2;
		DECLARE_SWAITQUEUE(b);
		__prepare_to_swait(&q, &b);
		snap("after_two_prepare", &q, &a, &b);
		current = &T_N;
		swake_up_all_locked(&q);
		snap("after_all", &q, &a, &b);
	} else if (!strcmp(g_case, "v09_schedule_timeout_values")) {
		long in[] = {0, 1, 5, -1, MAX_SCHEDULE_TIMEOUT};
		for (unsigned k = 0; k < sizeof(in) / sizeof(in[0]); k++) {
			int s0 = g_sched_calls, m0 = g_mod_calls, d0 = g_del_calls;
			current = &T_W1;
			__set_current_state(TASK_UNINTERRUPTIBLE);
			long r = schedule_timeout(in[k]);
			ev("\"ev\":\"st\",\"in\":%ld,\"ret\":%ld,\"sched_calls\":%d,\"mod_timer_calls\":%d,"
			   "\"del_calls\":%d,\"state_after\":%u", in[k], r, g_sched_calls - s0,
			   g_mod_calls - m0, g_del_calls - d0, current->__state);
		}
	} else if (!strcmp(g_case, "v09_uninterruptible_wrapper")) {
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		long r = schedule_timeout_uninterruptible(5);
		ev("\"ev\":\"stu\",\"in\":5,\"ret\":%ld,\"sched_calls\":%d,\"state_after\":%u",
		   r, g_sched_calls, current->__state);
	} else if (!strcmp(g_case, "v09_msleep_bounded")) {
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		ev("\"ev\":\"msleep_begin\",\"msecs\":5,\"cap\":%d", g_step_cap);
		msleep(5);
		ev("\"ev\":\"msleep_returned\",\"mod_timer_calls\":%d", g_mod_calls);
	} else if (!strcmp(g_case, "v10_done_preset_fast_path")) {
		completion_s x; x.done = 1; init_swait_queue_head(&x.wait);
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		long r = wait_for_common(&x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
		ev("\"ev\":\"wfc_common\",\"ret_is_max\":%s,\"done_after\":%u,\"sched_calls\":%d,"
		   "\"count\":%lu,\"state_after\":%u", tf(r == MAX_SCHEDULE_TIMEOUT), x.done, g_sched_calls,
		   x.wait.task_list_hdr.count, current->__state);
		x.done = 1;
		wait_for_completion(&x);
		ev("\"ev\":\"wait_for_completion\",\"done_after\":%u,\"sched_calls\":%d,\"count\":%lu",
		   x.done, g_sched_calls, x.wait.task_list_hdr.count);
	} else if (!strcmp(g_case, "v10_infinite_notify_during_schedule")) {
		completion_s x; x.done = 0; init_swait_queue_head(&x.wait);
		g_x = &x; g_on_schedule = notifier_once; g_watch_lock = &x.wait.lock;
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		long r = wait_for_common(&x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
		ev("\"ev\":\"returned\",\"ret_is_max\":%s,\"done_after\":%u,\"sched_calls\":%d,"
		   "\"count\":%lu,\"walk_len\":%d,\"state_after\":%u", tf(r == MAX_SCHEDULE_TIMEOUT), x.done,
		   g_sched_calls, x.wait.task_list_hdr.count, fx_walk_len(&x.wait.task_list_hdr), current->__state);
	} else if (!strcmp(g_case, "v10_finite_timeout_no_notifier")) {
		completion_s x; x.done = 0; init_swait_queue_head(&x.wait);
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		ev("\"ev\":\"begin\",\"timeout\":5,\"cap\":%d", g_step_cap);
		long r = wait_for_common(&x, 5, TASK_UNINTERRUPTIBLE);
		ev("\"ev\":\"returned\",\"ret\":%ld", r);
	} else if (!strcmp(g_case, "v11_wait_then_notify_then_reuse")) {
		completion_s x; x.done = 0; init_swait_queue_head(&x.wait);
		g_x = &x; g_on_schedule = notifier_once; g_watch_lock = &x.wait.lock;
		current = &T_W1; T_W1.__state = TASK_RUNNING;
		wait_for_completion(&x);
		ev("\"ev\":\"waiter_returned\",\"done\":%u,\"count\":%lu,\"walk_len\":%d,\"anchor_self\":%s,"
		   "\"header_is_empty\":%s,\"state\":%u,\"sched_calls\":%d,\"ttwu_calls\":%d", x.done,
		   x.wait.task_list_hdr.count, fx_walk_len(&x.wait.task_list_hdr),
		   tf(x.wait.task_list_hdr.anchor.next == &x.wait.task_list_hdr.anchor),
		   tf(list_header_is_empty(&x.wait.task_list_hdr)), current->__state, g_sched_calls, g_ttwu_calls);
		current = &T_N;
		ev("\"ev\":\"reuse_complete_begin\",\"note\":\"waiter stack object no longer exists\"");
		complete(&x);
		ev("\"ev\":\"reuse_complete_returned\",\"done\":%u,\"count\":%lu", x.done, x.wait.task_list_hdr.count);
	} else {
		return 2;
	}
	return 0;
}
