#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_

#include <sys/types.h>

	void bm_set_bit(bitmap_t *, unsigned long);
	void bm_clear_bit(bitmap_t *, unsigned long);
	unsigned long bm_get_freebit_idx(bitmap_t *, unsigned long);

#endif /* _LIB_UTILS_H_ */