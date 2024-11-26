#ifndef _IDR_CONST_H_
#define _IDR_CONST_H_

#include <limits.h>

    #define MYOS_IDR_BUF_SIZE		32768

	/*
	 * IDA - ID Allocator, use when translation from id to pointer isn't necessary.
	 */
	// #define IDA_CHUNK_SIZE		128	/* 128 bytes per chunk */
	#define IDA_CHUNK_SIZE			MYOS_IDR_BUF_SIZE	/* 128 bytes per chunk */
	#define IDA_BITMAP_LONGS		(IDA_CHUNK_SIZE / sizeof(long))
	#define IDA_BITMAP_BITS 		(IDA_BITMAP_LONGS * sizeof(long) * 8)
	#define IDA_BITMAP_SLOT_BITS	(sizeof(long))

#endif /* _IDR_CONST_H_ */