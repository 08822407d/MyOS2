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
            memblock_free_pages(page_s *page, unsigned long pfn, unsigned int order)
            {
                    ......
              void __free_pages_core(page_s *page, unsigned int order)
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
      page_s *
      __alloc_pages(gfp_t gfp, unsigned int order,
          int preferred_nid, nodemask_t *nodemask)
      {
                        ......
        static page_s *
        get_page_from_freelist(gfp_t gfp_mask, unsigned int order,
            int alloc_flags, const struct alloc_context *ac)
        {
                          ......
          static inline page_s
          *rmqueue(struct zone *preferred_zone, struct zone *zone,
              unsigned int order, gfp_t gfp_flags, unsigned int alloc_flags,
              int migratetype)
          {
                            ......
            static __always_inline page_s
            *__rmqueue_smallest(struct zone *zone, unsigned int order, int migratetype)
            {
              get_page_from_free_area();
              del_page_from_free_list();
              expand();
            }
                            ......
          }

          static void prep_new_page(page_s *page, unsigned int order,
              gfp_t gfp_flags, unsigned int alloc_flags)
          {
                            ......
            if (order && (gfp_flags & __GFP_COMP))
              prep_compound_page();
                            ......
          }
                          ......
        }
                        ......
      }


  释放页面
    主要执行流程:
      (mm/page_alloc.c)free_the_page() => __free_pages_ok()
    调用栈:
      static void
      __free_pages_ok(page_s *page, unsigned int order, fpi_t fpi_flags)
      {
                        ......
        static inline void
        __free_one_page(page_s *page, unsigned long pfn, struct zone *zone,
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