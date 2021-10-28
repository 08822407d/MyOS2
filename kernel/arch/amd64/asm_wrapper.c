#include <sys/types.h>
#include <sys/cdefs.h>

#include "include/arch_proto.h"

/*==============================================================================================*
 *								x86 asm i/o instructions' warpper					 			*
 *==============================================================================================*/
inline __always_inline uint64_t inb(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__(	"inb	%%dx, %%al	\n\t"
							"mfence				\n\t"
						:	"=al"(ret_val)
						:	"d"(port)
						:	"memory");
	ret_val &= 0xFF;
	return ret_val;
}
inline __always_inline uint64_t inw(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__(	"inw	%%dx, %%ax	\n\t"
							"mfence				\n\t"
						:	"=ax"(ret_val)
						:	"d"(port)
						:	"memory");
	ret_val &= 0xFFFF;
	return ret_val;
}
inline __always_inline uint64_t inl(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__(	"inl	%%dx, %%eax	\n\t"
							"mfence				\n\t"
						:	"=eax"(ret_val)
						:	"d"(port)
						:	"memory");
	ret_val &= 0xFFFFFFFF;
	return ret_val;
}

inline __always_inline void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__(	"outb	%0, %%dx	\n\t"
							"mfence				\n\t"
						:
						:	"a"(value),"d"(port)
						:	"memory");
}
inline __always_inline void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__(	"outw	%0, %%dx	\n\t"
							"mfence				\n\t"
						:
						:	"a"(value),"d"(port)
						:	"memory");
}
inline __always_inline void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__(	"outl	%0, %%dx	\n\t"
							"mfence				\n\t"
						:
						:	"a"(value),"d"(port)
						:	"memory");
}

inline __always_inline void insw(uint16_t port, uint16_t * buffer, uint64_t nr)
{
	__asm__ __volatile__(	"cld		\n\t"
							"rep		\n\t"
							"insw		\n\t"
							"mfence		\n\t"
						:
						:	"d"(port),"D"(buffer),"c"(nr)
						:	"memory");
}

inline __always_inline void outsw(uint16_t port, uint16_t * buffer, uint64_t nr)
{
	__asm__ __volatile__(	"cld		\n\t"
							"rep		\n\t"
							"outsw		\n\t"
							"mfence		\n\t"
						:
						:	"d"(port),"S"(buffer),"c"(nr)
						:	"memory");
}

inline __always_inline uint64_t rdmsr(uint64_t msr_addr)
{
	uint32_t tmp0 = 0;
	uint32_t tmp1 = 0;
	__asm__ __volatile__(	"rdmsr		\n\t"
						:	"=d"(tmp0),"=a"(tmp1)
						:	"c"(msr_addr)
						:	"memory");	
	return (unsigned long)tmp0<<32 | tmp1;
}

inline __always_inline void wrmsr(uint64_t msr_addr, uint64_t value)
{
	__asm__ __volatile__(	"wrmsr		\n\t"
						:
						:	"d"(value >> 32),"a"(value & 0xffffffff),"c"(msr_addr)
						:	"memory");	
}

inline __always_inline void io_mfence()
{
	__asm__ __volatile__(	"mfence		\n\t");
}

/*==============================================================================================*
 *								x86 asm special instructions' warpper							*
 *==============================================================================================*/
inline __always_inline void nop()
{
	__asm__ __volatile__(	"nop		\n");
}

inline __always_inline void hlt()
{
	__asm__ __volatile__(	"hlt		\n");
}

inline __always_inline void std()
{
	__asm__ __volatile__(	"std		\n");
}

inline __always_inline void cld()
{
	__asm__ __volatile__(	"cld		\n");
}

inline __always_inline void cli()
{
	__asm__ __volatile__(	"cli		\n");
}

inline __always_inline void sti()
{
	__asm__ __volatile__(	"sti		\n");
}

inline __always_inline void cpuid(unsigned int Mop,
								  unsigned int Sop,
								  unsigned int * a,
								  unsigned int * b,
								  unsigned int * c,
								  unsigned int * d)
{
	__asm__ __volatile__(	"cpuid		\n\t"
						:	"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d)
						:	"0"(Mop),"2"(Sop)
						:	);
}

inline __always_inline void wrgsbase(uint64_t addr)
{
	__asm__ __volatile__(	"wrgsbase	%%rax	\n\t"
						:
						:	"a"(addr)
						:	);
}

inline __always_inline uint64_t rdgsbase()
{
	uint64_t ret_val = 0;
	__asm__ __volatile__(	"rdgsbase	%0		\n\t"
						:	"=r"(ret_val)
						:
						:	);
	return ret_val;
}