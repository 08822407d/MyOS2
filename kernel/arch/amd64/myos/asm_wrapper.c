#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>

#include <obsolete/arch_proto.h>

/*==============================================================================================*
 *								x86 asm i/o instructions' warpper					 			*
 *==============================================================================================*/
inline __always_inline uint64_t inb(uint16_t port)
{
	reg_t ret_val = 0;
	asm volatile(	"inb	%%dx, %%al	\n\t"
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
	asm volatile(	"inw	%%dx, %%ax	\n\t"
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
	asm volatile(	"inl	%%dx, %%eax	\n\t"
					"mfence				\n\t"
				:	"=eax"(ret_val)
				:	"d"(port)
				:	"memory");
	ret_val &= 0xFFFFFFFF;
	return ret_val;
}

inline __always_inline void outb(uint16_t port, uint8_t value)
{
	asm volatile(	"outb	%0, %%dx	\n\t"
					"mfence				\n\t"
				:
				:	"a"(value),"d"(port)
				:	"memory");
}
inline __always_inline void outw(uint16_t port, uint16_t value)
{
	asm volatile(	"outw	%0, %%dx	\n\t"
					"mfence				\n\t"
				:
				:	"a"(value),"d"(port)
				:	"memory");
}
inline __always_inline void outl(uint16_t port, uint32_t value)
{
	asm volatile(	"outl	%0, %%dx	\n\t"
					"mfence				\n\t"
				:
				:	"a"(value),"d"(port)
				:	"memory");
}

inline __always_inline void insw(uint16_t port, uint16_t * buffer, uint64_t nr)
{
	asm volatile(	"cld		\n\t"
					"rep		\n\t"
					"insw		\n\t"
					"mfence		\n\t"
				:
				:	"d"(port),"D"(buffer),"c"(nr)
				:	"memory");
}

inline __always_inline void outsw(uint16_t port, uint16_t * buffer, uint64_t nr)
{
	asm volatile(	"cld		\n\t"
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
	asm volatile(	"rdmsr		\n\t"
				:	"=d"(tmp0), "=a"(tmp1)
				:	"c"(msr_addr)
				:	"memory");	
	return (unsigned long)tmp0 << 32 | tmp1;
}

inline __always_inline void wrmsr(uint64_t msr_addr, uint64_t value)
{
	asm volatile(	"wrmsr		\n\t"
				:
				:	"d"(value >> 32),
					"a"(value & 0xffffffff),
					"c"(msr_addr)
				:	"memory");	
}

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

inline __always_inline void wrgsbase(uint64_t addr)
{
	asm volatile(	"wrgsbase	%%rax	\n\t"
				:
				:	"a"(addr)
				:	);
}

inline __always_inline uint64_t rdgsbase()
{
	uint64_t ret_val = 0;
	asm volatile(	"rdgsbase	%0		\n\t"
				:	"=r"(ret_val)
				:
				:	);
	return ret_val;
}