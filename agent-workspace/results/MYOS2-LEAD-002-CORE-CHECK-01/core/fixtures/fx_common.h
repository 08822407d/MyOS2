/* Shared host-fixture scaffolding (hand-written; NOT MyOS2 code).
 * Replacement dependencies declared here are listed in evidence.md:
 *   - basic integer typedefs with the kernel's names;
 *   - PREFIX_STATIC_INLINE / PREFIX_STATIC_AWLWAYS_INLINE as in the non-DEBUG branch of
 *     include/linux/compiler/myos_debug_option.h (static inline / static __always_inline);
 *   - READ_ONCE / WRITE_ONCE as plain volatile accesses (the kernel versions add type checks);
 *   - likely/unlikely/barrier as the usual GCC builtins.
 * Event output: one JSON object per line on stdout. STOP() ends the case process on purpose.
 */
#ifndef FX_COMMON_H
#define FX_COMMON_H
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned long ulong;
typedef unsigned int uint;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t s32;
typedef int64_t s64;

#undef __always_inline
#define __always_inline inline __attribute__((__always_inline__))
#define PREFIX_STATIC_INLINE static inline
#define PREFIX_STATIC_AWLWAYS_INLINE static __always_inline
#define READ_ONCE(x) (*(const volatile __typeof__(x) *)&(x))
#define WRITE_ONCE(x, val) do { *(volatile __typeof__(x) *)&(x) = (val); } while (0)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define barrier() __asm__ __volatile__("" ::: "memory")

static const char *g_case = "";
static int g_step_cap = 100;

static void ev(const char *fmt, ...)
{
	va_list ap;
	printf("{\"case\":\"%s\",", g_case);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("}\n");
	fflush(stdout);
}

#define EXIT_STOP_INVARIANT 42
#define EXIT_STOP_STEPCAP 43
#define STOP(code, ...) do { ev(__VA_ARGS__); _exit(code); } while (0)

#endif
