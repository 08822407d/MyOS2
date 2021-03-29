#include <sys/types.h>

#include "include/arch_proto.h"

inline uint64_t inb(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inb	%%dx, %%al	\n\
						  mfence			\n	"
						 :"=al"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}
inline uint64_t inw(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inw	%%dx, %%ax	\n\
						  mfence			\n	"
						 :"=ax"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}
inline uint64_t inl(uint16_t port)
{
	reg_t ret_val = 0;
	__asm__ __volatile__("inl	%%dx, %%eax	\n\
						  mfence			\n	"
						 :"=eax"(ret_val)
						 :"d"(port)
						 :"memory");
	return ret_val;
}

inline void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__("outb	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}
inline void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__("outw	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}
inline void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__("outl	%0, %%dx	\n\
						  mfence			\n	"
						 :
						 :"a"(value),"d"(port)
						 :"memory");
}
