Linux-5.17.9

初始化过程

  建立内核页映射 init_mem_mapping()
    主要执行流程:
      (init/main.c)start_kernel() => (arch/kernel/setup)setup_arch() => (arch/mm/init)init_mem_mapping() => memory_map_top_down() / memory_map_bottom_up => init_range_memory_mapping() => init_memory_mapping()
    调用栈:
      static void __init init_mem_mapping(void)
      {
                          ...
        if (memblock_bottom_up()) {
          static void __init memory_map_bottom_up(
              unsigned long map_start, unsigned long map_end)
          {
                            ...
            static unsigned long __init init_range_memory_mapping(
                unsigned long r_start, unsigned long r_end)
            {
              for_each_mem_pfn_range(i, MAX_NUMNODES, &start_pfn, 
                  &end_pfn, NULL) {
                unsigned long __ref init_memory_mapping()
              }
            }
                            ...
          }
        } else {
          static void __init memory_map_top_down(
              unsigned long map_start, unsigned long map_end)
          {
                            ...
            static unsigned long __init init_range_memory_mapping()
                            ...
          }
        }
                          ...
      }


API

  建立x86虚拟地址到物理地址的页映射 init_memory_mapping()
    主要执行流程:
      (arch/mm/init.c)init_memory_mapping() => split_mem_range() => kernel_physical_mapping_init() => add_pfn_range_mapped()
    调用栈:
      unsigned long __ref init_memory_mapping(unsigned long start,
          unsigned long end, pgprot_t prot)
      {
                    ...
        // 按2^n个页为一段分割整段内存两端
        static int __meminit split_mem_range(struct map_range *mr,
            int nr_range, unsigned long start, unsigned long end);
                    ...
        // x86映射物理页核心函数
        unsigned long __meminit
        kernel_physical_mapping_init(unsigned long paddr_start, 
            unsigned long paddr_end, unsigned long page_size_mask, 
            pgprot_t prot);
                    ...
        // 将映射的内存段加入统计信息
        static void add_pfn_range_mapped(unsigned long start_pfn, 
            unsigned long end_pfn);
                    ...
      }
