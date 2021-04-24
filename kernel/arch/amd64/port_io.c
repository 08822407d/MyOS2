#include <sys/types.h>
#include <sys/cdefs.h>

#include "include/arch_proto.h"

inline __always_inline uint64_t inb(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inb	%%dx, %%al	\n\
						  mfence			\n	"
						 :"=al"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}
inline __always_inline uint64_t inw(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inw	%%dx, %%ax	\n\
						  mfence			\n	"
						 :"=ax"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}
inline __always_inline uint64_t inl(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inl	%%dx, %%eax	\n\
						  mfence			\n	"
						 :"=eax"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}

inline __always_inline void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__("outb	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}
inline __always_inline void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__("outw	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}
inline __always_inline void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__("outl	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}

inline __always_inline uint64_t rdmsr(uint64_t msr_addr)
{
	uint32_t tmp0 = 0;
	uint32_t tmp1 = 0;
	__asm__ __volatile__("rdmsr		\n\t"
						 :"=d"(tmp0),"=a"(tmp1)
						 :"c"(msr_addr)
						 :"memory");	
	return (unsigned long)tmp0<<32 | tmp1;
}

inline __always_inline void wrmsr(uint64_t msr_addr,uint64_t value)
{
	__asm__ __volatile__("wrmsr		\n\t"
						 :
						 :"d"(value >> 32),"a"(value & 0xffffffff),"c"(msr_addr)
						 :"memory");	
}

inline __always_inline void io_mfence()
{
	__asm__ __volatile__("mfence	\n");
}

/*===========================================================================*
 *							x86 asm instructions' warpper					 *
 *===========================================================================*/
inline __always_inline void nop()
{
	__asm__ __volatile__("nop	\n");
}

inline __always_inline void std()
{
	__asm__ __volatile__("std	\n");
}

inline __always_inline void cld()
{
	__asm__ __volatile__("cld	\n");
}

inline __always_inline void cli()
{
	__asm__ __volatile__("cli	\n");
}

inline __always_inline void sti()
{
	__asm__ __volatile__("sti	\n");
}

inline __always_inline void get_cpuid(unsigned int Mop,
									  unsigned int Sop,
									  unsigned int * a,
									  unsigned int * b,
									  unsigned int * c,
									  unsigned int * d)
{
	__asm__ __volatile__("cpuid	\n\t"
						 :"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d)
						 :"0"(Mop),"2"(Sop)
						 :);
}