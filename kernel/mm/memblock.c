#include <sys/types.h>
#include <sys/limits.h>

#include <string.h>
#include <errno.h>

#include <include/memblock.h>
#include <include/kutils.h>

#define INIT_MEMBLOCK_REGIONS			128
#define INIT_PHYSMEM_REGIONS			4

#ifndef INIT_MEMBLOCK_RESERVED_REGIONS
# define INIT_MEMBLOCK_RESERVED_REGIONS		INIT_MEMBLOCK_REGIONS
#endif

static memblock_region_s memblock_memory_init_regions[INIT_MEMBLOCK_REGIONS] = 
	{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0}};
static memblock_region_s memblock_reserved_init_regions[INIT_MEMBLOCK_RESERVED_REGIONS] =
	{ [0 ... INIT_MEMBLOCK_REGIONS - 1] = {.base = (phys_addr_t)~0, .size = 0}};

memblock_s memblock = {
	.memory.regions		= memblock_memory_init_regions,
	.memory.cnt			= 0,	/* empty dummy entry */
	.memory.max			= INIT_MEMBLOCK_REGIONS,
	.memory.name		= "memory",

	.reserved.regions	= memblock_reserved_init_regions,
	.reserved.cnt		= 0,	/* empty dummy entry */
	.reserved.max		= INIT_MEMBLOCK_RESERVED_REGIONS,
	.reserved.name		= "reserved",

	.bottom_up			= false,
	.current_limit		= (phys_addr_t)MEMBLOCK_ALLOC_ANYWHERE,
};


#define for_each_memblock_type(rgn_idx, memblock_type, rgn_ptr)		\
		for (rgn_idx = 0, rgn_ptr = &memblock_type->regions[0];		\
			rgn_idx < memblock_type->cnt + 1;						\
			rgn_idx++, rgn_ptr = &memblock_type->regions[rgn_idx])

/* adjust *@size so that (@base + *@size) doesn't overflow, return new size */
static inline size_t memblock_cap_size(phys_addr_t base, size_t *size)
{
	return *size = min(*size, PHYS_ADDR_MAX - (size_t)base);
}

/*
 * Address comparison utilities
 */
static unsigned long memblock_addrs_overlap(phys_addr_t base1, size_t size1,
				       						phys_addr_t base2, size_t size2)
{
	return ((base1 < (base2 + size2)) && (base2 < (base1 + size1)));
}

bool memblock_overlaps_region(	memblock_type_s *type,
								phys_addr_t base, size_t size)
{
	unsigned long i;

	memblock_cap_size(base, &size);

	for (i = 0; i < type->cnt; i++)
		if (memblock_addrs_overlap(base, size, type->regions[i].base,
					   type->regions[i].size))
			break;
	return i < type->cnt;
}

/**
 * memblock_merge_regions - merge neighboring compatible regions
 * @type: memblock type to scan
 *
 * Scan @type and merge neighboring compatible regions.
 */
static void memblock_merge_regions(memblock_type_s *type)
{
	int i = 0;

	/* cnt never goes below 1 */
	while (i < type->cnt - 1) {
		memblock_region_s *this = &type->regions[i];
		memblock_region_s *next = &type->regions[i + 1];

		if (this->base + this->size != next->base) {
			// BUG_ON(this->base + this->size > next->base);
			while (this->base + this->size > next->base);
			i++;
			continue;
		}

		this->size += next->size;
		/* move forward from next + 1, index of which is i + 2 */
		memmove(next, next + 1, (type->cnt - (i + 2)) * sizeof(*next));
		type->cnt--;
	}
}

/**
 * memblock_insert_region - insert new memblock region
 * @type:	memblock type to insert into
 * @idx:	index for the insertion point
 * @base:	base address of the new region
 * @size:	size of the new region
 * @flags:	flags of the new region
 *
 * Insert new memblock region [@base, @base + @size) into @type at @idx.
 * @type must already have extra room to accommodate the new region.
 */
static void memblock_insert_region(	memblock_type_s *type,
									int idx, phys_addr_t base,
									size_t size,
									enum memblock_flags flags)
{
	memblock_region_s *rgn = &type->regions[idx];

	while(type->cnt >= type->max);

	memmove(rgn + 1, rgn, (type->cnt - idx) * sizeof(*rgn));
	rgn->base = base;
	rgn->size = size;
	// memblock_set_region_node(rgn, nid);
	type->cnt++;
	type->total_size += size;
}

/**
 * memblock_add_range - add new memblock region
 * @type: memblock type to add new region into
 * @base: base address of the new region
 * @size: size of the new region
 * @nid: nid of the new region
 * @flags: flags of the new region
 *
 * Add new memblock region [@base, @base + @size) into @type.  The new region
 * is allowed to overlap with existing ones - overlaps don't affect already
 * existing regions.  @type is guaranteed to be minimal (all neighbouring
 * compatible regions are merged) after the addition.
 *
 * Return:
 * 0 on success, -errno on failure.
 */
static int memblock_add_range(	memblock_type_s *type,
								phys_addr_t base, size_t size,
								enum memblock_flags flags)
{
	phys_addr_t end = base + memblock_cap_size(base, &size);
	int idx;
	memblock_region_s *rgn;

	if (!size)
		return ENOERR;

	if (type->cnt + 2 > type->max)
		return -ENOMEM;

	/*
	 * 
	 */
	for_each_memblock_type(idx, type, rgn) {
		phys_addr_t rbase = rgn->base;
		phys_addr_t rend = rbase + rgn->size;

		if (rend <= base)
			continue;
		if (rbase >= end || rgn->size == 0)
		{
			memblock_insert_region(type, idx, base, end - base, flags);
			break;
		}
		if (rbase > base) {
			memblock_insert_region(type, idx, base, rbase - base, flags);
			idx++;
			base = min(end, rend);
			if (base = end)
				break;
		}
	}

	memblock_merge_regions(type);
	return ENOERR;
}

/**
 * memblock_add - add new memblock region
 * @base: base address of the new region
 * @size: size of the new region
 *
 * Add new memblock region [@base, @base + @size) to the "memory"
 * type. See memblock_add_range() description for mode details
 *
 * Return:
 * 0 on success, -errno on failure.
 */
int memblock_add(phys_addr_t base, size_t size)
{
	return memblock_add_range(&memblock.memory, base, size, 0);
}