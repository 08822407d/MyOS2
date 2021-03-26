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
	page_s * page_alloc(void);
	void page_free(page_s *);

	/* video.c */
	void video_init(void);

	/* exception.c */
	void exception_handler(unsigned int);

	/* functions defined in architecture-dependent files. */
	void prot_init(void);

#endif /* _PROTO_H_ */