#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>

#include <obsolete/arch_proto.h>

/*==============================================================================================*
 *								x86 asm i/o instructions' warpper					 			*
 *==============================================================================================*/
inline __always_inline void io_mfence()
{
	asm volatile(	"mfence		\n\t");
}

/*==============================================================================================*
 *								x86 asm special instructions' warpper							*
 *==============================================================================================*/
inline __always_inline void nop()
{
	asm volatile(	"nop		\n");
}

inline __always_inline void hlt()
{
	asm volatile(	"hlt		\n");
}

inline __always_inline void std()
{
	asm volatile(	"std		\n");
}

inline __always_inline void cld()
{
	asm volatile(	"cld		\n");
}

inline __always_inline void cli()
{
	asm volatile(	"cli		\n");
}

inline __always_inline void sti()
{
	asm volatile(	"sti		\n");
}