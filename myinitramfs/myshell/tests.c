#include "tests.h"


// task: sh
void test_all()
{
	signal(SIGTERM, sigaction_SIGKILL_test);

	int rv = fork();
	if (rv == 0) {
		pid_t pid = getpid();
		signal_test();
	}
}

void signal_test()
{
	pid_t ppid = getppid();
	kill(ppid, SIGTERM);
}

void sigaction_SIGKILL_test(int val)
{
	printf("Captured SIGTERM! Test value: %d\n", val);
}




typedef unsigned long ulong;

// ulong rdgsbase(void) {
// 	ulong gsbase;
// 	asm volatile(	"rdgsbase	%0		\t\n"
// 				:	"=r" (gsbase)
// 				:
// 				:	"memory");
// 	return gsbase;
// }

// ulong rdfsbase(void) {
// 	ulong fsbase;
// 	asm volatile(	"rdfsbase	%0		\t\n"
// 				:	"=r" (fsbase)
// 				:
// 				:	"memory");
// 	return fsbase;
// }


void test_FSGbase()
{
#define rdgsbase() ({									\
			ulong gsbase;								\
			asm volatile(	"rdgsbase	%0		\t\n"	\
						:	"=r" (gsbase)				\
						:								\
						:	"memory");					\
			gsbase;										\
		})

#define rdfsbase() ({									\
			ulong fsbase;								\
			asm volatile(	"rdfsbase	%0		\t\n"	\
						:	"=r" (fsbase)				\
						:								\
						:	"memory");					\
			fsbase;										\
		})


	ulong gsbase = rdgsbase();
	ulong fsbase = rdfsbase();
	printf("gsbase in user space: %p\n", (void *)gsbase);
	printf("fsbase in user space: %p\n", (void *)fsbase);
}

void test_SharedAnon()
{
	size_t alloc_size = 512;
	ulong *mmap_share_test =
			mmap(0, alloc_size, PROT_WRITE,
				MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	memset((void *)mmap_share_test, 0,
		alloc_size * sizeof(*mmap_share_test));
	mmap_share_test[0] = 0;

	int rv = fork();
	
	if (rv == 0) {
		while (1) {
			mmap_share_test[0]++;
		}
	} else {
		ulong last_read = 0;
		while (1) {
			if (mmap_share_test[0] != last_read)
				last_read = mmap_share_test[0];
			sched_yield();
		}
	}
}

int test_PrintDateTime() {
	struct timeval tv;
	struct tm *tm_info;
	char buffer[64];

	// 获取当前时间
	if (gettimeofday(&tv, NULL) == -1) {
		perror("gettimeofday failed");
		return 1;
	}

	// printf("Raw timeval: %ld, %ld\n", tv.tv_sec, tv.tv_usec);

	// 将秒数转换为本地时间
	tm_info = localtime(&tv.tv_sec);

	// 格式化时间为字符串
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

	// 打印日期和微秒
	printf("DateTime Now: %s.%06ld\n", buffer, tv.tv_usec);

	return 0;
}