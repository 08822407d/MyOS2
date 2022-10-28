Linux-5.17.9

memblock是一种早期内存分配器，用于在slab初始化之前分配内存

1.初始化,x86平台用e820结果初始化memblock系统
  e820到memblock
    (init/main.c)start_kernel() => (arch/kernel/setup.c)setup_arch() => (arch/mm/e820.c)e820__memblock_setup()
  标记已占用内存
    (init/main.c)start_kernel() => (arch/kernel/setup.c)setup_arch() => early_reserve_memory()

2.API

  添加内存段 memblock_add_range()
    主要执行流程:
      (mm/memblock.c)memblock_add_range() => memblock_insert_region() => memblock_merge_regions()
    调用栈:
      static int __init_memblock memblock_add_range(struct memblock_type *type,
          phys_addr_t base, phys_addr_t size, int nid, enum memblock_flags flags)
      {
                          ...
        static void __init_memblock memblock_insert_region(struct memblock_type *type,
            int idx, phys_addr_t base, phys_addr_t size, int nid, enum memblock_flags flags)
        {
                            ...
        }
                          ...
        static void __init_memblock memblock_merge_regions(struct memblock_type *type)
        {
                            ...
        }
      }

  标记可用 memblock_add()
    主要执行流程:
      (mm/memblock.c)memblock_add() => memblock_add_range()

  标记已用 memblock_reserve()
    主要执行流程:
      (mm/memblock.c)memblock_reserve() => memblock_add_range()


  申请 memblock_alloc_try_nid()
    主要执行流程:
      (mm/memblock.c)memblock_alloc_try_nid() => memblock_alloc_internal() => memblock_alloc_range_nid() => memblock_find_in_range_node() => __memblock_find_range_bottom_up() / __memblock_find_range_top_down() => memblock_reserve()
    调用栈:
      void * __init memblock_alloc_try_nid(phys_addr_t size, phys_addr_t align,
          phys_addr_t min_addr, phys_addr_t max_addr, int nid)
      {
        static void * __init memblock_alloc_internal( phys_addr_t size, phys_addr_t align,
            phys_addr_t min_addr, phys_addr_t max_addr, int nid, bool exact_nid)
        {
                            ...
          phys_addr_t __init memblock_alloc_range_nid(phys_addr_t size, phys_addr_t align, 
              phys_addr_t start, phys_addr_t end, int nid, bool exact_nid)
          {
                              ...
            static phys_addr_t __init_memblock memblock_find_in_range_node(phys_addr_t size,
                phys_addr_t align, phys_addr_t start, phys_addr_t end, int nid, 
                enum memblock_flags flags)
            {
                                ...
              if (memblock_bottom_up())
                static phys_addr_t __init_memblock
                __memblock_find_range_bottom_up(phys_addr_t start, phys_addr_t end, 
                    phys_addr_t size, phys_addr_t align, int nid, enum memblock_flags flags);
              else
                __memblock_find_range_top_down(phys_addr_t start, phys_addr_t end, 
                    phys_addr_t size, phys_addr_t align, int nid, enum memblock_flags flags);
            }
                              ...
            memblock_reserve();
                              ...
          }
                            ...
        }
                          ...
      }

  释放 memblock_free()
    主要执行流程:
      (mm/memblock.c)memblock_free() => memblock_phys_free() => memblock_remove_range() => memblock_isolate_range() => memblock_insert_region() => memblock_remove_region()
    调用栈: