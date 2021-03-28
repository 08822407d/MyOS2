#include <sys/types.h>

#include "include/arch_proto.h"

inline uint64_t inb(uint16_t port)
{
	register reg_t ret_val = 0;
	__asm__ __volatile__("inb	%%dx"
						 :"=rax"(ret_val)
						 :"rdx"(port)
						 :);
	return ret_val;
}

inline uint64_t inw(uint16_t port)
{
	register reg_t ret_val = 0;
	__asm__ __volatile__("inw	%%dx"
						 :"=rax"(ret_val)
						 :"rdx"(port)
						 :);
	return ret_val;
}


inline uint64_t inl(uint16_t port)
{
	register reg_t ret_val = 0;
	__asm__ __volatile__("inl	%%dx"
						 :"=rax"(ret_val)
						 :"rdx"(port)
						 :);
	return ret_val;
}

inline void outb(uint16_t port, uint8_t value)
{
	__asm__ __volatile__("outb	%%dx"
						 :
						 :"rax"(value),"rdx"(port)
						 :);
}

inline void outw(uint16_t port, uint16_t value)
{
	__asm__ __volatile__("outw	%%dx"
						 :
						 :"rax"(value),"rdx"(port)
						 :);
}

inline void outl(uint16_t port, uint32_t value)
{
	__asm__ __volatile__("outl	%%dx"
						 :
						 :"rax"(value),"rdx"(port)
						 :);
}