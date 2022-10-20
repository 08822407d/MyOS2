// #include <linux/kernel/slab.h>
// #include <linux/fs/fs.h>
// #include <linux/lib/string.h>

// #include <obsolete/glo.h>
// #include <obsolete/printk.h>
// #include <obsolete/arch_proto.h>
// #include <obsolete/device.h>
// #include <obsolete/ide.h>

// extern atomic_T boot_counter;

// void kmalloc_kfree_test(void);
// void kernthd_test(void);
// void disk_drv_test(void);

// unsigned long module_test(unsigned long flag);

// unsigned long module_test(unsigned long flag)
// {
// 	// if (flag == 0)
// 	// {
// 	// 	long long i = 0;
// 	// 	for ( ; i < 0xFFFFFFF; i++);
// 	// 	long val = boot_counter.value;
// 	// 	color_printk(BLACK, GREEN, "Mutex servied core num : - %d -\n", val);
// 	// }

// 	// kmalloc_kfree_test();

// 	kernthd_test();

// 	// disk_drv_test();

// 	// while (!kparam.init_flags.vfs);
	
// 	// char * testfname = "/EFI/BOOT/BOOTX64.EFI";
// 	// char testf_buf[512];
// 	// long pos = 0;
// 	// file_s * testf_fp = open_exec_file(testfname);
// 	// int retval = testf_fp->f_op->read(testf_fp, testf_buf, 512, &pos);

// 	// color_printk(YELLOW, BLACK, "task module_test finished......");
// 	while (1);
// }

// unsigned long test_task_a(unsigned long arg)
// {
// 	while (1)
// 	{
// 		unsigned long k = 0;
// 		for (int i = 0; i < 0x2000; i++)
// 			for (int j = 0; j < 0x2000; j++)
// 				k++;
// 		{
// 			color_printk(WHITE, BLACK, "-");
// 			color_printk(BLACK, RED, " ");
// 			color_printk(WHITE, BLACK, "-");
// 		}
// 	}
// }

// unsigned long test_task_b(unsigned long arg)
// {
// 	while (1)
// 	{
// 		unsigned long k = 0;
// 		for (int i = 0; i < 0x2000; i++)
// 			for (int j = 0; j < 0x2000; j++)
// 				k++;
// 		{
// 			color_printk(WHITE, BLACK, "-");
// 			color_printk(BLACK, GREEN, " ");
// 			color_printk(WHITE, BLACK, "-");
// 		}
// 	}
// }

// unsigned long test_task_c(unsigned long arg)
// {
// 	while (1)
// 	{
// 		unsigned long k = 0;
// 		for (int i = 0; i < 0x2000; i++)
// 			for (int j = 0; j < 0x2000; j++)
// 				k++;
// 		{
// 			color_printk(WHITE, BLACK, "-");
// 			color_printk(BLACK, YELLOW, " ");
// 			color_printk(WHITE, BLACK, "-");
// 		}
// 	}
// }

// unsigned long test_task_d(unsigned long arg)
// {
// 	while (1)
// 	{
// 		unsigned long k = 0;
// 		for (int i = 0; i < 0x2000; i++)
// 			for (int j = 0; j < 0x2000; j++)
// 				k++;
// 		{
// 			color_printk(WHITE, BLACK, "-");
// 			color_printk(BLACK, BLUE, " ");
// 			color_printk(WHITE, BLACK, "-");
// 		}
// 	}
// }

// unsigned long ide_read_test(unsigned long arg)
// {
// 	per_cpudata_s * cpudata_p = curr_cpu;

// 	char * disk_test = (char *)kzalloc(512, GFP_KERNEL);
// 	char * buf = (char *)kzalloc(512 * 3 + 1, GFP_KERNEL);
// 	memset(disk_test, 0, 512);
// 	memset(disk_test, 0, 512 * 3 + 1);

// 	struct Identify_Device_data ide_id;
// 	ATA_master_ops.ioctl(MASTER, SLAVE, ATA_INFO_CMD,
// 					(long)&ide_id);

// 	// ATA_master_ops.transfer(ATA_READ_CMD, 0, 1, disk_test);
// 	color_printk(ORANGE, WHITE, "------\n");	
// 	// for(int i = 0; i < 512; i++)
// 	// {
// 	// 	number(&buf[i * 3], (long)disk_test[i], 16, 2, -1, 65);
// 	// 	buf[i * 3 + 2] = ' ';
// 	// }

// 	// color_printk(ORANGE, BLACK, "%s", buf);
// 	color_printk(ORANGE, WHITE, "------\n");
// 	// kfree(buf);
// }

// void kernthd_test()
// {
// 	myos_kernel_thread(test_task_a, 0, 0);
// 	myos_kernel_thread(test_task_b, 0, 0);
// 	myos_kernel_thread(test_task_c, 0, 0);
// 	myos_kernel_thread(test_task_d, 0, 0);
// }

// void kmalloc_kfree_test()
// {
// 	unsigned char * test1 = (char *)kmalloc(0x100);
// 	unsigned char * test2 = (char *)kmalloc(0x100);
// 	unsigned char * test3 = (char *)kmalloc(0x100);
// 	unsigned char * test4 = (char *)kmalloc(0x100);

// 	kfree(test2);
// 	kfree(test3);
// 	kfree(test4);
// 	unsigned char * test5 = (char *)kmalloc(0x100);
// 	unsigned char * test6 = (char *)kmalloc(0x100);
// 	kfree(test1);
// 	kfree(test6);
// 	unsigned char * test7 = (char *)kmalloc(0x100);
// 	unsigned char * test8 = (char *)kmalloc(0x100);
// 	kfree(test5);
// 	kfree(test7);
// 	kfree(test8);

// 	color_printk(WHITE, BLACK, "kmalloc test finished.\n");
// }

// void disk_drv_test()
// {
// 	myos_kernel_thread(ide_read_test, 0, 0);
// }