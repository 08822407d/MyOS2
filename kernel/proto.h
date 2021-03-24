#ifndef PROTO_H
#define PROTO_H

	/* main.c */
	// #ifndef UNPAGED
	// #define kmain __k_unpaged_kmain
	// #endif
	void kmain(void);

	/* start.c */
	void cstart(void);

	/* memory.c */
	void mem_init(void);

	/* exception.c */
	void exception_handler(unsigned long);

	/* functions defined in architecture-dependent files. */
	void prot_init(void);

#endif /* PROTO_H */