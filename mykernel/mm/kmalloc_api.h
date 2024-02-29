#ifndef _LINUX_KMALLOC_API_H_
#define _LINUX_KMALLOC_API_H_


	extern void *kmalloc_order(size_t size, gfp_t flags, unsigned int order);

	void *__kmalloc(size_t size, gfp_t flags);
	void *kmalloc(size_t size, gfp_t flags);
	void kfree(const void *objp);

	void myos_preinit_slab(void);
	void myos_init_slab(void);

#endif /* _LINUX_KMALLOC_API_H_ */