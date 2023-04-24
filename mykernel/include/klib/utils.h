#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

#include <linux/kernel/types.h>
#include <linux/kernel/stddef.h>

	// bitmap.c
	void bm_set_bit(bitmap_t *, unsigned long idx);
	void bm_clear_bit(bitmap_t *, unsigned long idx);
	unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit);
	unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx);

#endif /* _LIB_UTILS_H_ */