#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/mman.h>

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


int normal_boot(int argc, const char *argv[])
{
	char prog_name[] = "/boot/sh";
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };

	int rv = fork();
	
	if (rv == 0) {
		pid_t pid = getpid();
		// pid_t ppid = getppid();
		// printf("child task, %d\n", rv);
		execve(prog_name, args, envs);
	}
	else {
		pid_t pid = getpid();
		// pid_t ppid = getppid();
		// printf("parent task, %d\n", rv);
		while (1) {
			sched_yield();
		}
	}
}

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


int main(int argc, const char *argv[])
{
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);


	printf("Welcome to MyOS2\n\n");


	// test_FSGbase();
	// test_SharedAnon();

	return normal_boot(argc, argv);
}