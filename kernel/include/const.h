#ifndef _CONST_H_
#define _CONST_H_

	#define CONST_1K		0x400ULL
	#define SHIFT_1K		10
	#define MASKF_1K(addr)	(addr & ~(CONST_1K - 1))
	#define ALIGN_1K(addr)	(MASK_1K(addr + CONST_1K - 1))

	#define CONST_4K		0x1000ULL
	#define SHIFT_4K		12
	#define MASKF_4K(addr)	(addr & ~(CONST_4K - 1))
	#define ALIGN_4K(addr)	(MASK_4K(addr + CONST_4K - 1))

	#define CONST_1M		0x100000ULL
	#define SHIFT_1M		20
	#define MASKF_1M(addr)	(addr & ~(CONST_1M - 1))
	#define ALIGN_1M(addr)	(MASK_1M(addr + CONST_1M - 1))

	#define CONST_2M		0x200000ULL
	#define SHIFT_2M		21
	#define MASKF_2M(addr)	(addr & ~(CONST_2M - 1))
	#define ALIGN_2M(addr)	(MASK_2M(addr + CONST_2M - 1))

	#define CONST_1G		0x40000000ULL
	#define SHIFT_1G		30
	#define MASKF_1G(addr)	(addr & ~(CONST_1G - 1))
	#define ALIGN_1G(addr)	(MASK_1G(addr + CONST_1G - 1))

	#define CONST_2G		0x80000000ULL
	#define SHIFT_2G		31
	#define MASKF_2G(addr)	(addr & ~(CONST_2G - 1))
	#define ALIGN_2G(addr)	(MASK_2G(addr + CONST_2G - 1))

	#define COSNT_4G		0x100000000ULL
	#define SHIFT_4G		32
	#define MASKF_4G(addr)	(addr & ~(CONST_4G - 1))
	#define ALIGN_4G(addr)	(MASK_4G(addr + CONST_4G - 1))

	#define CONST_1T		0x10000000000ULL
	#define SHIFT_1T		40
	#define MASKF_1T(addr)	(addr & ~(CONST_1T - 1))
	#define ALIGN_1T(addr)	(MASK_1T(addr + CONST_1T - 1))

#endif /* _CONST_H_ */