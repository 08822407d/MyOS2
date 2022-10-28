/* 未完成 */

Linux-5.17.9

mmzone结构用来帮助管理物理内存页,其中之一是页换入换出

1.初始化,用memblock和各段预设最小最大内存页号初始化pg_data_t类型的结构node_data[nid]
  主要执行流程:
    (init/main.c)start_kernel() => (arch/kernel/setup)setup_arch() => (arch/mm/init_64.c)paging_init() => (mm/init.c)zone_sizes_init() => (mm/page_alloc.c)free_area_init() => free_area_init_node()
  调用栈:
    static void __init free_area_init_node(int nid)
    {
            ......
      // 初始化node_data[nid]
      //(计算并填入各段和总计的各种类型的页数目)
      calculate_node_totalpages()
      // 分配和初始化node_data[nid]->node_mam_map
      // (计算管理的页面总数并分配struct page)
      alloc_node_mem_map()
            ......
      // 继续初始化node_data[nid]->node_zones[zone_type]
      // (其他成员赋值或初始化)
      free_area_init_core()
    }