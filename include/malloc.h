#ifndef __MALLOC_H__
#define __MALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <arch/amd64/types.h>

	void *	mm_create(void * mem, size_t bytes);
	void	mm_destroy(void * mem);
	void *	mm_get(void * mem);
	void *	mm_add_pool(void * mm, void * mem, size_t bytes);
	void	mm_remove_pool(void * mm, void * mem);
	void *	mm_malloc(void * mm, size_t size);
	void *	mm_memalign(void * mm, size_t align, size_t size);
	void *	mm_realloc(void * mm, void * ptr, size_t size);
	void	mm_free(void * mm, void * ptr);
	void	mm_info(void * mm, size_t * mused, size_t * mfree);

	void *	memalign(size_t align, size_t size);
	void	meminfo(size_t * mused, size_t * mfree);

	void	do_init_mem(void);

#ifdef __cplusplus
}
#endif

#endif /* __MALLOC_H__ */
