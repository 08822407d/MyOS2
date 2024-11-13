#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/mman.h>


void test_all(void);

void signal_test(void);

void sigaction_SIGKILL_test(int val);