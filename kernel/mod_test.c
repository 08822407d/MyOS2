#include "include/proto.h"

#include "include/printk.h"

extern atomic_T boot_counter;

void kmalloc_kfree_test(void);
void kthread_test(void);
unsigned long module_test(unsigned long flag);

unsigned long module_test(unsigned long flag)
{
	// kmalloc_kfree_test();

	atomic_inc(&boot_counter);

	if (flag == 0)
	{
		long long i = 0;
		for ( ; i < 0xFFFFFFF; i++);
		long val = boot_counter.value;
		color_printk(BLACK, GREEN, "Mutex servied core num : - %d -\n", val);
	}

	kthread_test();
}

unsigned long test_task_a(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;

		color_printk(WHITE, BLACK, "-A- ");
	}
}

unsigned long test_task_b(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-B- ");
	}
}

unsigned long test_task_c(unsigned long arg)
{
	while (1)
	{
		unsigned long k = 0;
		for (int i = 0; i < 0x2000; i++)
			for (int j = 0; j < 0x2000; j++)
				k++;
		color_printk(WHITE, BLACK, "-C- ");
	}
}

void kthread_test()
{
	kernel_thread(test_task_a, 0, 0);
	kernel_thread(test_task_b, 0, 0);
	kernel_thread(test_task_c, 0, 0);
}

void kmalloc_kfree_test()
{
	unsigned char * test1 = (char *)kmalloc(0x100);
	unsigned char * test2 = (char *)kmalloc(0x100);
	unsigned char * test3 = (char *)kmalloc(0x100);
	unsigned char * test4 = (char *)kmalloc(0x100);

	kfree(test2);
	kfree(test3);
	kfree(test4);
	unsigned char * test5 = (char *)kmalloc(0x100);
	unsigned char * test6 = (char *)kmalloc(0x100);
	kfree(test1);
	kfree(test6);
	unsigned char * test7 = (char *)kmalloc(0x100);
	unsigned char * test8 = (char *)kmalloc(0x100);
	kfree(test5);
	kfree(test7);
	kfree(test8);
}