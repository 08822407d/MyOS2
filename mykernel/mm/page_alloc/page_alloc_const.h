#ifndef _LINUX_PAGE_ALLOC_CONST_H_
#define _LINUX_PAGE_ALLOC_CONST_H_

	/* to align the pointer to the (next) page boundary */
	#define PAGE_ALIGN(addr) ALIGN(addr, PAGE_SIZE)

#endif /* _LINUX_PAGE_ALLOC_CONST_H_ */