#ifndef _PROTO_H_
#define _PROTO_H_

#include "ktypes.h"

	/* main.c */
	// #ifndef UNPAGED
	// #define kmain __k_unpaged_kmain
	// #endif
	void kmain(void);

	/* start.c */
	void cstart(void);

	/* memory.c */
	void mem_init(void);
	void slab_init();
	Page_s * page_alloc(void);
	void page_free(Page_s *);
	void * kmalloc(size_t size);
	void kfree(void * obj_p);

	/* video.c */
	void video_init(void);

	/* functions defined in architecture-dependent files. */
	void prot_init(void);

#endif /* _PROTO_H_ */