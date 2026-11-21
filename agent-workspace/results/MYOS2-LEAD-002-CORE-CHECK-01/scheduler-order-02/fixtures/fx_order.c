/* ---- provenance (metadata comment) ----------------------------------------------------------
 * packet: MYOS2-LEAD-002-CORE-CHECK-01 / followup CORE-SCHED-ORDER-02 (scheduler-order-02)
 * executor: Claude Code cloud session (claude.ai/code); model: unknown_or_not_attestable
 * source: new template. Type choices follow core/fixtures/fx_sched.c @ 0d62c4d19711 (frozen,
 *   unchanged); every //@@ORIG line is replaced verbatim from time a039d9803ade by the frozen
 *   recheck-01 build2.Builder.expand. pick_next_task_myos is copied unchanged (comparison, requeue,
 *   accounting order and return are the original text).
 * hand-written here (NOT MyOS2 code): reduced task_struct / runqueue holding only the members the
 *   function touches (__state, se.vruntime, rt.run_list, rt.time_slice, rq->idle, rq->curr, rq->myos)
 *   plus a fixture id; current, need_resched() and jiffies as explicit fixture inputs; BUG_ON records
 *   and stops (exit 42). Observers run only before and after each call; the queue walk compares each
 *   link with the anchor and with the known task nodes by address, never converting an unknown link,
 *   and follows at most 16 links.
 * not taken over from fx_sched.c: its pick() helper that set rq->myos.last_jiffies = jiffies before
 *   every call. Here last_jiffies is set once per scenario; between calls only the original function
 *   updates it.
 * sequence model: in W07/W08 the returned task is made current before the next call. This is not a
 *   context switch, a clock, an interrupt or preemption.
 * --------------------------------------------------------------------------------------------- */
#include "fx_common.h"

//@@INCLUDE fx_list.inc.c

typedef struct task_struct task_s;
typedef struct runqueue rq_s;
typedef struct myos_rq myos_rq_s;
typedef struct sched_entity sched_entity_s;
typedef struct sched_rt_entity sched_rt_entity_s;
//@@ORIG time mykernel/sched/scheduler/scheduler_types.h struct sched_entity
//@@ORIG time mykernel/sched/scheduler/scheduler_types.h struct sched_rt_entity
//@@ORIG time mykernel/arch/x86_64/sched/context/thread_info_types_arch.h struct thread_info
//@@ORIG time mykernel/sched/runqueue/runqueue_types.h struct myos_rq
struct task_struct {
	struct thread_info thread_info;
	uint __state;
	int on_cpu;
	sched_entity_s se;
	sched_rt_entity_s rt;
	int id;
};
struct runqueue {
	task_s *curr;
	task_s *idle;
	myos_rq_s myos;
};

//@@ORIG time mykernel/sched/task/task_const.h macro TASK_RUNNING
//@@ORIG time mykernel/sched/task/task_const.h macro TASK_UNINTERRUPTIBLE

static rq_s rq0;
static task_s *g_current;
#define current g_current
static int g_need_resched;
#define need_resched() (g_need_resched)
static ulong jiffies;
#define BUG_ON(cond) do { if (cond) STOP(EXIT_STOP_INVARIANT, "\"ev\":\"BUG_ON\",\"line\":%d", __LINE__); } while (0)

//@@ORIG time mykernel/sched/scheduler/myos_rt.c func pick_next_task_myos

/* ---------------- fixture inputs and observers (hand-written) */
#define TIME_SLICE 100
static task_s TI, TA, TB, TC, TD;                 /* ids I=0, A=2, B=3, C=4, D=5 */
static task_s *const ALL[] = {&TI, &TA, &TB, &TC, &TD};
#define NALL ((int)(sizeof(ALL) / sizeof(ALL[0])))

static int id_of(const task_s *t)
{
	for (int k = 0; k < NALL; k++)
		if (t == ALL[k])
			return ALL[k]->id;
	return t ? -2 : -1;   /* -2: pointer that is not a fixture task, -1: NULL */
}
static void mk(task_s *t, int id, uint state, u64 vr)
{
	memset(t, 0, sizeof(*t));
	t->id = id; t->__state = state; t->se.vruntime = vr; t->rt.time_slice = TIME_SLICE;
	INIT_LIST_S(&t->rt.run_list);
}
static void enqueue_tail(task_s *t) { list_header_add_to_tail(&rq0.myos.running_lhdr, &t->rt.run_list); }

/* queue snapshot: compare with the anchor first, then identify the link by address among the five
 * fixture nodes; an unknown link is reported and the walk stops without dereferencing it */
static void print_queue(void)
{
	List_hdr_s *h = &rq0.myos.running_lhdr;
	int ids[16], n = 0, unknown = 0, complete = 0;
	List_s *lp = h->anchor.next;
	while (n < 16) {
		if (lp == &h->anchor) { complete = 1; break; }
		int id = -2;
		for (int k = 0; k < NALL; k++)
			if (lp == &ALL[k]->rt.run_list)
				id = ALL[k]->id;
		if (id == -2) { unknown = 1; break; }
		ids[n++] = id;
		lp = lp->next;
	}
	int dup = 0, idle_occ = 0, cur_occ = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++)
			if (ids[i] == ids[j])
				dup = 1;
		if (ids[i] == TI.id) idle_occ++;
		if (current && ids[i] == current->id) cur_occ++;
	}
	printf("\"queue\":[");
	for (int i = 0; i < n; i++) {
		task_s *t = NULL;
		for (int k = 0; k < NALL; k++)
			if (ALL[k]->id == ids[i])
				t = ALL[k];
		printf("%s[%d,%llu]", i ? "," : "", ids[i], (unsigned long long)t->se.vruntime);
	}
	printf("],\"count\":%lu,\"walk_len\":%d,\"walk_complete\":%s,\"unknown_link\":%s,\"duplicate_nodes\":%s,"
	       "\"idle_occurrences\":%d,\"current_occurrences\":%d",
	       h->count, n, complete ? "true" : "false", unknown ? "true" : "false", dup ? "true" : "false", idle_occ, cur_occ);
}

static void snap(const char *where, int step)
{
	printf("{\"case\":\"%s\",\"ev\":\"snap\",\"where\":\"%s\",\"step\":%d,\"jiffies\":%lu,\"last_jiffies\":%lu,"
	       "\"used_external\":%ld,\"need_resched\":%d,\"current\":{\"id\":%d,\"state\":%u,\"vruntime\":%llu,\"time_slice\":%u},"
	       "\"rq_curr\":%d,\"rq_idle\":%d,\"tasks\":[",
	       g_case, where, step, jiffies, rq0.myos.last_jiffies, (long)(jiffies - rq0.myos.last_jiffies), g_need_resched,
	       id_of(current), current->__state, (unsigned long long)current->se.vruntime, current->rt.time_slice,
	       id_of(rq0.curr), id_of(rq0.idle));
	for (int k = 0; k < NALL; k++)
		printf("%s[%d,%llu,%u]", k ? "," : "", ALL[k]->id, (unsigned long long)ALL[k]->se.vruntime, ALL[k]->__state);
	printf("],");
	print_queue();
	printf("}\n");
	fflush(stdout);
}

static void call(int step)
{
	snap("before", step);
	task_s *r = pick_next_task_myos(&rq0, current);
	ev("\"ev\":\"call\",\"step\":%d,\"returned\":%d,\"rq_curr\":%d", step, id_of(r), id_of(rq0.curr));
	snap("after", step);
	if (id_of(r) < 0)
		STOP(EXIT_STOP_INVARIANT, "\"ev\":\"returned_not_a_fixture_task\",\"step\":%d", step);
}

/* one scenario: tasks, queue, current, clock and flag are all explicit inputs */
static void setup(u64 a_vr, uint a_state, ulong jf, int resched, int current_is_idle, u64 idle_vr)
{
	mk(&TI, 0, TASK_RUNNING, idle_vr);
	mk(&TA, 2, a_state, a_vr);
	mk(&TB, 3, TASK_RUNNING, 10);
	mk(&TC, 4, TASK_RUNNING, 20);
	mk(&TD, 5, TASK_RUNNING, 30);
	memset(&rq0, 0, sizeof(rq0));
	INIT_LIST_HEADER_S(&rq0.myos.running_lhdr);
	rq0.idle = &TI;
	rq0.myos.last_jiffies = 100;
	enqueue_tail(&TB);
	enqueue_tail(&TC);
	enqueue_tail(&TD);
	if (current_is_idle) {
		current = &TI;           /* idle is current: not queued a second time */
	} else {
		enqueue_tail(&TI);       /* idle always exists; queued when it is not current */
		current = &TA;
	}
	rq0.curr = current;
	jiffies = jf;
	g_need_resched = resched;
	ev("\"ev\":\"scenario\",\"time_slice\":%d,\"last_jiffies_initial\":%lu,\"jiffies\":%lu,\"need_resched\":%d,"
	   "\"current\":%d,\"a_state\":%u,\"a_vruntime\":%llu,\"idle_vruntime\":%llu",
	   TIME_SLICE, rq0.myos.last_jiffies, jiffies, g_need_resched, id_of(current), a_state,
	   (unsigned long long)a_vr, (unsigned long long)idle_vr);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 2;
	g_case = argv[1];
	int steps = 1;
	if (!strcmp(g_case, "W01")) {
		setup(25, TASK_RUNNING, 100, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W02")) {
		setup(15, TASK_RUNNING, 110, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W03")) {
		setup(15, TASK_RUNNING, 100, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W04")) {
		setup(15, TASK_RUNNING, 105, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W05")) {
		setup(15, TASK_UNINTERRUPTIBLE, 110, 1, 0, 0); call(1);
	} else if (!strcmp(g_case, "W06")) {
		setup(15, TASK_RUNNING, 110, 1, 1, 7); call(1);
	} else if (!strcmp(g_case, "W07")) {
		setup(15, TASK_RUNNING, 110, 1, 0, 0); call(1);
		current = rq0.curr;   /* sequence model: the returned task becomes current; jiffies and last_jiffies untouched */
		g_need_resched = 1;
		ev("\"ev\":\"sequence\",\"before_step\":2,\"current_set_to_returned\":%d,\"jiffies\":%lu,\"need_resched\":%d",
		   id_of(current), jiffies, g_need_resched);
		call(2); steps = 2;
	} else if (!strcmp(g_case, "W08")) {
		setup(15, TASK_RUNNING, 105, 0, 0, 0); call(1);
		current = rq0.curr;
		g_need_resched = 1;   /* same jiffies (105); last_jiffies is whatever the original function left */
		ev("\"ev\":\"sequence\",\"before_step\":2,\"current_set_to_returned\":%d,\"jiffies\":%lu,\"need_resched\":%d",
		   id_of(current), jiffies, g_need_resched);
		call(2); steps = 2;
	} else {
		return 2;
	}
	ev("\"ev\":\"end\",\"steps\":%d", steps);
	return 0;
}
