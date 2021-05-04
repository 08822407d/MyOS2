#ifndef _SYS_TYPES_H_
#define	_SYS_TYPES_H_

	#ifndef	int8_t
	typedef	__INT8_TYPE__	__int8_t;
	#define	int8_t			__int8_t
	#endif

	#ifndef	uint8_t
	typedef	__UINT8_TYPE__	__uint8_t;
	#define	uint8_t			__uint8_t
	#endif

	#ifndef	int16_t
	typedef	__INT16_TYPE__	__int16_t;
	#define	int16_t			__int16_t
	#endif

	#ifndef	uint16_t
	typedef	__UINT16_TYPE__	__uint16_t;
	#define	uint16_t		__uint16_t
	#endif

	#ifndef	int32_t
	typedef	__INT32_TYPE__	__int32_t;
	#define	int32_t			__int32_t
	#endif

	#ifndef	uint32_t
	typedef	__UINT32_TYPE__	__uint32_t;
	#define	uint32_t		__uint32_t
	#endif

	#ifndef	int64_t
	typedef	__INT64_TYPE__	__int64_t;
	#define	int64_t			__int64_t
	#endif

	#ifndef	uint64_t
	typedef	__UINT64_TYPE__	__uint64_t;
	#define	uint64_t		__uint64_t
	#endif

	typedef __UINT64_TYPE__	size_t;
	typedef __UINT64_TYPE__ reg_t;
	typedef __UINT64_TYPE__ bitmap_t;
	#define BITMAP_UNITSIZE 64

	typedef void*	phy_addr;
	typedef void*	vir_addr;

#endif /* !_SYS_TYPES_H_ */