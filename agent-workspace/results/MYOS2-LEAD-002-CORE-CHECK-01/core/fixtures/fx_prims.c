/* fx_prims: host fixture for V12 (arch_atomic_add_test_negative, original x86-64 inline asm) and
 * V13 (arch_spin_trylock and neighbours, original code). Runs only on an x86-64 host.
 * Hand-written: typedef lines (same text as lock_ipc_type_declaration_arch.h) and __READ_ONCE
 * (plain volatile read; the original uses __unqual_scalar_typeof). The contract column for V12 is
 * computed in plain C from the function's own doc comment ("adds @i ... true if negative").
 */
#include "fx_common.h"

typedef struct atomic atomic_t;
typedef struct atomic64 atomic64_t;
typedef struct tspinlock arch_spinlock_t;
#define __READ_ONCE(x) (*(const volatile __typeof__(x) *)&(x))
//@@ORIG time mykernel/arch/x86_64/lock_IPC/atomic/atomic_types_arch.h struct atomic
//@@ORIG time mykernel/arch/x86_64/lock_IPC/atomic/atomic_types_arch.h struct atomic64
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_types_arch.h struct tspinlock
//@@ORIG time mykernel/arch/x86_64/include/asm/alternative.h macro LOCK_PREFIX_HERE
//@@ORIG time mykernel/arch/x86_64/include/asm/alternative.h macro LOCK_PREFIX
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_macro_arch.h macro __ARCH_SPIN_LOCK_UNLOCKED
//@@ORIG time mykernel/arch/x86_64/lock_IPC/atomic/atomic_arch.h func arch_atomic_add_test_negative
//@@ORIG time mykernel/arch/x86_64/lock_IPC/atomic/atomic64_arch.h func arch_atomic64_read
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_init
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_is_locked
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_trylock
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_lock
//@@ORIG time mykernel/arch/x86_64/lock_IPC/spinlock/spinlock_smp_arch.h func arch_spin_unlock

static const char *tf(bool b) { return b ? "true" : "false"; }
static void lk(const char *step, arch_spinlock_t *l, int ret)
{
	ev("\"ev\":\"lock\",\"step\":\"%s\",\"ret\":%d,\"val\":%lld,\"head\":%u,\"tail\":%u,\"is_locked\":%d",
	   step, ret, (long long)l->val.counter, l->head, l->tail, arch_spin_is_locked(l));
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 2;
	g_case = argv[1];
	if (!strcmp(g_case, "v12_add_test_negative")) {
		int vec[][2] = {{-1, 1}, {1, 2}, {2, -1}, {0, 0}, {5, 3}};
		for (unsigned k = 0; k < sizeof(vec) / sizeof(vec[0]); k++) {
			atomic_t v = { .counter = vec[k][0] };
			int i = vec[k][1];
			bool ret = arch_atomic_add_test_negative(i, &v);
			long long add = (long long)vec[k][0] + i, sub = (long long)vec[k][0] - i;
			ev("\"ev\":\"vec\",\"init\":%d,\"i\":%d,\"after\":%d,\"ret\":%s,"
			   "\"contract_after_add\":%lld,\"contract_ret\":%s,\"sub_after\":%lld,\"sub_ret\":%s,"
			   "\"after_matches_contract\":%s,\"ret_matches_contract\":%s,\"after_matches_sub\":%s",
			   vec[k][0], i, v.counter, tf(ret), add, tf(add < 0), sub, tf(sub < 0),
			   tf(v.counter == add), tf(ret == (add < 0)), tf(v.counter == sub));
		}
	} else if (!strcmp(g_case, "v13_trylock")) {
		arch_spinlock_t l;
		memset(&l, 0xa5, sizeof(l));
		arch_spin_init(&l);
		lk("after_init", &l, -1);
		int r1 = arch_spin_trylock(&l);
		lk("after_first_trylock", &l, r1);
		int r2 = arch_spin_trylock(&l);
		lk("after_second_trylock_without_unlock", &l, r2);
		arch_spin_lock(&l);
		lk("after_arch_spin_lock", &l, -1);
		int r3 = arch_spin_trylock(&l);
		lk("trylock_while_held_by_arch_spin_lock", &l, r3);
		arch_spin_unlock(&l);
		lk("after_arch_spin_unlock", &l, -1);
	} else {
		return 2;
	}
	return 0;
}
