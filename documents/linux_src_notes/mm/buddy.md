/* 未完成 */

Linux-5.17.9

buddy是物理内存页分配器

1.初始化，由memblock将可分配物理内存段按页面添加到buddy中，之后memblock失效
  主要执行流程:
    (init/main.c)start_kernel() => mm_init() => (arch/mm)mem_init() => (mm/memblock.c)memblock_free_all() => free_low_memory_core_early()
  调用栈:
    static unsigned long __init free_low_memory_core_early(void)
    {
            ......
      // 将页面标记为PG_reserved
      static void __init memmap_init_reserved_pages(void)
      {
              ......
        for_each_reserved_mem_range
          reserve_bootmem_region();
              ......
      }
      // 获取每个可用内存段并加入buddy
      for_each_free_mem_range
        __init __free_memory_core(phys_addr_t start, phys_addr_t end)
        {
                ......
          static void __init
          __free_pages_memory(unsigned long start, unsigned long end)
          {
                  ......
            void __init
            memblock_free_pages(struct page *page, unsigned long pfn, unsigned int order)
            {
                    ......
              void __free_pages_core(struct page *page, unsigned int order)
              {
                      ......
                // 清除可分配页的保留标记并初始化页面引用计数器
                __ClearPageReserved();
                set_page_count();
                      ......
                // 计算并设置zone中的页面计数
                atomic_long_add();
                __free_pages_ok();
              }
            }
                  ......
          }
                ......
        }
    }


2.API

  申请页面
    主要执行流程:
      (mm/mempolicy)alloc_pages() => (mm/page_alloc.c)__alloc_pages()
    调用栈:

  释放页面
    主要执行流程:
      (mm/page_alloc.c)free_the_page() => __free_pages_ok()
    调用栈:
      static void
      __free_pages_ok(struct page *page, unsigned int order, fpi_t fpi_flags)
      {
                        ......
        static inline void
        __free_one_page(struct page *page, unsigned long pfn, struct zone *zone,
            unsigned int order, int migratetype, fpi_t fpi_flags)
        {
                          ......
          // 找到伙伴页块并判断是否可合并
          __find_buddy_pfn();
          page_is_buddy();
                          ......
          // 加入可分配列表
          add_to_free_list();
                          ......
        }
                        ......
      }