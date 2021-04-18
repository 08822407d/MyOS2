#include <lib/utils.h>

void bm_set_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) |= (1ULL << (idx % BITMAP_UNITSIZE));
}

void bm_clear_bit(bitmap_t * bm, unsigned long idx)
{
	*(bm + idx / BITMAP_UNITSIZE) &= ~(1ULL << (idx % BITMAP_UNITSIZE));
}

unsigned long bm_get_freebit_idx(bitmap_t * bm, unsigned long limit)
{
	unsigned long i;
	for (i = 0; i <= limit; i++)
	{
		bitmap_t bm_unit = *(bm + (i / BITMAP_UNITSIZE));
		if ((bm_unit & (1ULL << i % BITMAP_UNITSIZE)) == 0)
			break;
	}

	return i;
}