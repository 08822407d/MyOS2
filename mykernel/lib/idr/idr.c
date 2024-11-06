#define IDR_DEFINATION
#include "idr.h"

#include <linux/kernel/export.h>
#include <linux/debug/bug.h>


/**
 * idr_alloc_u32() - Allocate an ID.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @nextid: Pointer to an ID.
 * @max: The maximum ID to allocate (inclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @nextid and @max.
 * Note that @max is inclusive whereas the @end parameter to idr_alloc()
 * is exclusive.  The new ID is assigned to @nextid before the pointer
 * is inserted into the IDR, so if @nextid points into the object pointed
 * to by @ptr, a concurrent lookup will not find an uninitialised ID.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: 0 if an ID was allocated, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.  If an error occurred,
 * @nextid is unchanged.
 */
int idr_alloc_u32(idr_s *idr, void *ptr,
		u32 *nextid, ulong max, gfp_t gfp)
{
	// struct radix_tree_iter iter;
	// void __rcu **slot;
	uint base = idr->idr_base;
	uint id = *nextid;

	// if (WARN_ON_ONCE(!(idr->idr_rt.xa_flags & ROOT_IS_IDR)))
	// 	idr->idr_rt.xa_flags |= IDR_RT_MARKER;

	// id = (id < base) ? 0 : id - base;
	// radix_tree_iter_init(&iter, id);
	// slot = idr_get_free(&idr->idr_rt, &iter, gfp, max - base);
	// if (IS_ERR(slot))
	// 	return PTR_ERR(slot);

	// *nextid = iter.index + base;
	// /* there is a memory barrier inside radix_tree_iter_replace() */
	// radix_tree_iter_replace(&idr->idr_rt, &iter, slot, ptr);
	// radix_tree_iter_tag_clear(&idr->idr_rt, &iter, IDR_FREE);

	for (int slot_idx = id; slot_idx < MYOS_IDR_BUF_SIZE; slot_idx++)
	{
		void *tmp_slot = idr->idr_rt[slot_idx];
		if (tmp_slot == NULL)
		{
			ptr = tmp_slot;
			*nextid = slot_idx;
			return 0;
		}
	}

	return -ENOSPC;
}
EXPORT_SYMBOL_GPL(idr_alloc_u32);

/**
 * idr_alloc() - Allocate an ID.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @start: The minimum ID (inclusive).
 * @end: The maximum ID (exclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @start and @end.  If
 * @end is <= 0, it is treated as one larger than %INT_MAX.  This allows
 * callers to use @start + N as @end as long as N is within integer range.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: The newly allocated ID, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.
 */
int idr_alloc(idr_s *idr, void *ptr,
		int start, int end, gfp_t gfp)
{
	u32 id = start;
	int ret;

	if (WARN_ON_ONCE(start < 0))
		return -EINVAL;

	ret = idr_alloc_u32(idr, ptr, &id, end > 0 ?
				end - 1 : MYOS_IDR_BUF_SIZE, gfp);
	if (ret)
		return ret;

	return id;
}
EXPORT_SYMBOL_GPL(idr_alloc);

/**
 * idr_alloc_cyclic() - Allocate an ID cyclically.
 * @idr: IDR handle.
 * @ptr: Pointer to be associated with the new ID.
 * @start: The minimum ID (inclusive).
 * @end: The maximum ID (exclusive).
 * @gfp: Memory allocation flags.
 *
 * Allocates an unused ID in the range specified by @start and @end.  If
 * @end is <= 0, it is treated as one larger than %INT_MAX.  This allows
 * callers to use @start + N as @end as long as N is within integer range.
 * The search for an unused ID will start at the last ID allocated and will
 * wrap around to @start if no free IDs are found before reaching @end.
 *
 * The caller should provide their own locking to ensure that two
 * concurrent modifications to the IDR are not possible.  Read-only
 * accesses to the IDR may be done under the RCU read lock or may
 * exclude simultaneous writers.
 *
 * Return: The newly allocated ID, -ENOMEM if memory allocation failed,
 * or -ENOSPC if no free IDs could be found.
 */
int idr_alloc_cyclic(idr_s *idr, void *ptr,
		int start, int end, gfp_t gfp)
{
	u32 id = idr->idr_next;
	int err, max = end > 0 ? end - 1 : MYOS_IDR_BUF_SIZE;

	if ((int)id < start)
		id = start;

	err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	if ((err == -ENOSPC) && (id > start)) {
		id = start;
		err = idr_alloc_u32(idr, ptr, &id, max, gfp);
	}
	if (err)
		return err;

	idr->idr_next = id + 1;
	return id;
}
EXPORT_SYMBOL(idr_alloc_cyclic);

/**
 * idr_remove() - Remove an ID from the IDR.
 * @idr: IDR handle.
 * @id: Pointer ID.
 *
 * Removes this ID from the IDR.  If the ID was not previously in the IDR,
 * this function returns %NULL.
 *
 * Since this function modifies the IDR, the caller should provide their
 * own locking to ensure that concurrent modification of the same IDR is
 * not possible.
 *
 * Return: The pointer formerly associated with this ID.
 */
void *idr_remove(idr_s *idr, ulong id)
{
	// return radix_tree_delete_item(&idr->idr_rt, id - idr->idr_base, NULL);
	void *ptr = idr->idr_rt[id];
	idr->idr_rt[id] = NULL;
	return ptr;
}
EXPORT_SYMBOL_GPL(idr_remove);

/**
 * idr_find() - Return pointer for given ID.
 * @idr: IDR handle.
 * @id: Pointer ID.
 *
 * Looks up the pointer associated with this ID.  A %NULL pointer may
 * indicate that @id is not allocated or that the %NULL pointer was
 * associated with this ID.
 *
 * This function can be called under rcu_read_lock(), given that the leaf
 * pointers lifetimes are correctly managed.
 *
 * Return: The pointer associated with this ID.
 */
void *idr_find(const idr_s *idr, ulong id)
{
	// return radix_tree_lookup(&idr->idr_rt, id - idr->idr_base);
	return idr->idr_rt[id];
}
EXPORT_SYMBOL_GPL(idr_find);

/**
 * idr_for_each() - Iterate through all stored pointers.
 * @idr: IDR handle.
 * @fn: Function to be called for each pointer.
 * @data: Data passed to callback function.
 *
 * The callback function will be called for each entry in @idr, passing
 * the ID, the entry and @data.
 *
 * If @fn returns anything other than %0, the iteration stops and that
 * value is returned from this function.
 *
 * idr_for_each() can be called concurrently with idr_alloc() and
 * idr_remove() if protected by RCU.  Newly added entries may not be
 * seen and deleted entries may be seen, but adding and removing entries
 * will not cause other entries to be skipped, nor spurious ones to be seen.
 */
int idr_for_each(const idr_s *idr,
		int (*fn)(int id, void *p, void *data), void *data)
{
	// struct radix_tree_iter iter;
	// void __rcu **slot;
	int base = idr->idr_base;

	// radix_tree_for_each_slot(slot, &idr->idr_rt, &iter, 0) {
	// 	int ret;
	// 	unsigned long id = iter.index + base;

	// 	if (WARN_ON_ONCE(id > INT_MAX))
	// 		break;
	// 	ret = fn(id, rcu_dereference_raw(*slot), data);
	// 	if (ret)
	// 		return ret;
	// }

	return 0;
}
EXPORT_SYMBOL(idr_for_each);

/**
 * idr_get_next_ul() - Find next populated entry.
 * @idr: IDR handle.
 * @nextid: Pointer to an ID.
 *
 * Returns the next populated entry in the tree with an ID greater than
 * or equal to the value pointed to by @nextid.  On exit, @nextid is updated
 * to the ID of the found value.  To use in a loop, the value pointed to by
 * nextid must be incremented by the user.
 */
void *idr_get_next_ul(idr_s *idr, ulong *nextid)
{
	// struct radix_tree_iter iter;
	// void __rcu **slot;
	void *entry = NULL;
	ulong base = idr->idr_base;
	ulong id = *nextid;

	// id = (id < base) ? 0 : id - base;
	// radix_tree_for_each_slot(slot, &idr->idr_rt, &iter, id) {
	// 	entry = rcu_dereference_raw(*slot);
	// 	if (!entry)
	// 		continue;
	// 	if (!xa_is_internal(entry))
	// 		break;
	// 	if (slot != &idr->idr_rt.xa_head && !xa_is_retry(entry))
	// 		break;
	// 	slot = radix_tree_iter_retry(&iter);
	// }
	// if (!slot)
	// 	return NULL;

	// *nextid = iter.index + base;
	return entry;
}
EXPORT_SYMBOL(idr_get_next_ul);

/**
 * idr_get_next() - Find next populated entry.
 * @idr: IDR handle.
 * @nextid: Pointer to an ID.
 *
 * Returns the next populated entry in the tree with an ID greater than
 * or equal to the value pointed to by @nextid.  On exit, @nextid is updated
 * to the ID of the found value.  To use in a loop, the value pointed to by
 * nextid must be incremented by the user.
 */
void *idr_get_next(idr_s *idr, int *nextid)
{
	ulong id = *nextid;
	void *entry = idr_get_next_ul(idr, &id);

	if (WARN_ON_ONCE(id > INT_MAX))
		return NULL;
	*nextid = id;
	return entry;
}
EXPORT_SYMBOL(idr_get_next);

/**
 * idr_replace() - replace pointer for given ID.
 * @idr: IDR handle.
 * @ptr: New pointer to associate with the ID.
 * @id: ID to change.
 *
 * Replace the pointer registered with an ID and return the old value.
 * This function can be called under the RCU read lock concurrently with
 * idr_alloc() and idr_remove() (as long as the ID being removed is not
 * the one being replaced!).
 *
 * Returns: the old value on success.  %-ENOENT indicates that @id was not
 * found.  %-EINVAL indicates that @ptr was not valid.
 */
void *idr_replace(idr_s *idr, void *ptr, ulong id)
{
	// struct radix_tree_node *node;
	// void __rcu **slot = NULL;
	void *entry;

	// id -= idr->idr_base;

	// entry = __radix_tree_lookup(&idr->idr_rt, id, &node, &slot);
	// if (!slot || radix_tree_tag_get(&idr->idr_rt, id, IDR_FREE))
	// 	return ERR_PTR(-ENOENT);

	// __radix_tree_replace(&idr->idr_rt, node, slot, ptr);

	entry = idr->idr_rt[id];
	idr->idr_rt[id] = ptr;

	return entry;
}
EXPORT_SYMBOL(idr_replace);