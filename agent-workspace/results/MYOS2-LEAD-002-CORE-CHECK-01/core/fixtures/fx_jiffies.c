/* fx_jiffies: V14 MODEL_ONLY host link model. HPET_handler and do_timer are copied verbatim;
 * the alias is provided by an implicit linker script that contains the exact kernel.lds line
 * 'jiffies = jiffies_64;' (written by run_fixtures.py from the pinned blob). This is a host
 * executable, not the MyOS2 ELF; it explains the arithmetic of a linker alias only.
 * Hand-written: jiffies/jiffies_64 declarations (types as time/systick/systick_api.h; jiffies_64
 * starts at 0 instead of INITIAL_JIFFIES), pt_regs_s, tty/colour stubs, DEBUG_show_jiffies (false,
 * as initialised in hpet.c). Build with -DCONTROL_SEPARATE to give jiffies its own storage
 * (negative control, no linker script).
 */
#include "fx_common.h"

typedef struct pt_regs { int dummy; } pt_regs_s;
#define BLACK 0
#define GREEN 2
static int g_tty_calls;
static void myos_tty_write_color_at(const char *b, size_t n, int fg, int bg, int x, int y)
{ (void)b; (void)n; (void)fg; (void)bg; (void)x; (void)y; g_tty_calls++; }
bool DEBUG_show_jiffies = false;
u64 jiffies_64 = 0;
#ifdef CONTROL_SEPARATE
ulong volatile jiffies = 0;
#else
extern ulong volatile jiffies;
#endif

//@@ORIG time mykernel/time/timekeeping/timekeeping.c func do_timer
//@@ORIG time mykernel/arch/x86_64/kernel/hpet.c func HPET_handler

int main(int argc, char **argv)
{
	g_case = argc > 1 ? argv[1] : "v14";
	u64 j64_0 = jiffies_64;
	ulong j_0 = jiffies;
	HPET_handler(0, NULL);
	/* runtime comparison through volatile integers: a direct &a == &b between two distinct
	 * declarations is folded to false by the compiler and would not observe the linker alias */
	volatile uintptr_t a_j = (uintptr_t)&jiffies, a_j64 = (uintptr_t)&jiffies_64;
	ev("\"ev\":\"one_handler_call\",\"same_address\":%s,\"jiffies_64_delta\":%llu,\"jiffies_delta\":%lu,"
	   "\"sizeof_jiffies\":%zu,\"sizeof_jiffies_64\":%zu,\"tty_calls\":%d",
	   (a_j == a_j64) ? "true" : "false",
	   (unsigned long long)(jiffies_64 - j64_0), (ulong)(jiffies - j_0), sizeof(jiffies), sizeof(jiffies_64),
	   g_tty_calls);
	return 0;
}
