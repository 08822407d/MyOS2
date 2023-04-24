#include <linux/kernel/types.h>

#include <klib/utils.h>

unsigned bm_get_assigned_bit(bitmap_t * bm, unsigned long idx)
{
	unsigned ret_val = 0;
	size_t unit_idx = idx / (8 * sizeof(bitmap_t));
	bitmap_t mask = 1 << (idx % (8 * sizeof(bitmap_t)));
	bitmap_t unit = *(bm + unit_idx);
	ret_val = unit & mask;
	return ret_val;
}

void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}

void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}

unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long start, unsigned long limit)
{
	unsigned long 	i;
	for (i = start; i < limit; i++)
	{
		size_t idx = i / BITMAP_UNITSIZE;
		size_t off = i % BITMAP_UNITSIZE;
		bitmap_t bm_unit = *(bm + idx);
		if ((bm_unit & (1ULL << off)) == 0)
			break;
	}
	return i;
}