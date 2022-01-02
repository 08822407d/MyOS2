// linux代码参考

// 1.buddy系统
//	mm/page_alloc.c
//	alloc_pages()核心函数:
static __always_inline
struct page *__rmqueue_smallest(struct zone *zone, unsigned int order,
						int migratetype);

// free_pages()核心函数
static inline void __free_one_page(struct page *page,
		unsigned long pfn,
		struct zone *zone, unsigned int order,
		int migratetype, fpi_t fpi_flags);

//	include/asm-generic/memory_model.h
//	pfn和page相互转化宏函数
#define __pfn_to_page(pfn)
#define __page_to_pfn(page)