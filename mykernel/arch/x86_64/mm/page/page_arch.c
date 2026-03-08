#define ARCH_PAGE_DEFINATION
#include "page_arch.h"


#ifdef CONFIG_DEBUG_VIRTUAL
    ulong __phys_addr(ulong x)
    {
        ulong y = x - __START_KERNEL_map;

        /* use the carry flag to determine if x was < __START_KERNEL_map */
        if (unlikely(x > y)) {
            x = y + phys_base;

            VIRTUAL_BUG_ON(y >= KERNEL_IMAGE_SIZE);
        } else {
            x = y + (__START_KERNEL_map - PAGE_OFFSET);

            /* carry flag will be set if starting x was >= PAGE_OFFSET */
            VIRTUAL_BUG_ON((x > y) || !phys_addr_valid(x));
        }

        return x;
    }
    EXPORT_SYMBOL(__phys_addr);

    ulong __phys_addr_symbol(ulong x)
    {
        ulong y = x - __START_KERNEL_map;

        /* only check upper bounds since lower bounds will trigger carry */
        VIRTUAL_BUG_ON(y >= KERNEL_IMAGE_SIZE);

        return y + phys_base;
    }
    EXPORT_SYMBOL(__phys_addr_symbol);
#endif

bool __virt_addr_valid(ulong x)
{
	ulong y = x - __START_KERNEL_map;

	/* use the carry flag to determine if x was < __START_KERNEL_map */
	if (unlikely(x > y)) {
		x = y + phys_base;

		if (y >= KERNEL_IMAGE_SIZE)
			return false;
	} else {
		x = y + (__START_KERNEL_map - PAGE_OFFSET);

		/* carry flag will be set if starting x was >= PAGE_OFFSET */
		if ((x > y) || !phys_addr_valid(x))
			return false;
	}

	return pfn_valid(x >> PAGE_SHIFT);
}
EXPORT_SYMBOL(__virt_addr_valid);