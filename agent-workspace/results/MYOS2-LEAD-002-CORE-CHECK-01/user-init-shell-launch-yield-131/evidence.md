---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
track_id: MYOS2-LEAD-002
followup_id: CORE-USER-INIT-SHELL-LAUNCH-YIELD-131
record_type: phase2_static_source_windows
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
conversation_display_name: MYOS2-A-C08c 代码分析执行
produced_by: GPT-6 (self-reported; backend and effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time（分支名）
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
---
# 第131批新增证据 · 独立于130旧44个来源窗

48条定义体或活动宏短窗与12条独立定位记录。逐条来自固定time的当前GitHub连接器读取，每条连续1至5物理行，保留源内制表符和CRLF；行号仅作导航，只有定义体或活动宏标签作为正式P2引文。注释、extern、配置、分派表项单列为SOURCE_LOCATION类别而不冒充C函数定义体。

## E131-001 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	freopen("/dev/console", "r", stdin);
	freopen("/dev/console", "w", stdout);
	freopen("/dev/console", "w", stderr);
```

物理行 11–13；源码定义体/有效宏短窗。

## E131-002 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	printf("Welcome to MyOS2\n\n");
```

物理行 16–16；源码定义体/有效宏短窗。

## E131-003 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::main]
```c
	return normal_boot(argc, argv);
```

物理行 18–18；源码定义体/有效宏短窗。

## E131-004 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char prog_name[] = "/boot/sh";
```

物理行 24–24；源码定义体/有效宏短窗。

## E131-005 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char *const args[] =
			{ prog_name , "userarg_test_1", "userarg_test_2", NULL };
```

物理行 25–26；源码定义体/有效宏短窗。

## E131-006 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	char *const envs[] =
			{ "userenv_test_1", "userenv_test_2", "userenv_test_3", NULL };
```

物理行 27–28；源码定义体/有效宏短窗。

## E131-007 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	int rv = fork();
```

物理行 30–30；源码定义体/有效宏短窗。

## E131-008 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	if (rv == 0) {
		pid_t pid = getpid();
```

物理行 32–33；源码定义体/有效宏短窗。

## E131-009 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		execve(prog_name, args, envs);
```

物理行 35–35；源码定义体/有效宏短窗。

## E131-010 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
	else {
		pid_t pid = getpid();
		// printf("parent task, %d\n", rv);
```

物理行 37–39；源码定义体/有效宏短窗。

## E131-011 · VERIFIED

[VERIFIED myinitramfs/myinit/initd.c::normal_boot]
```c
		while (1) {
			sched_yield();
		}
```

物理行 40–42；源码定义体/有效宏短窗。

## E131-012 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
	shell_internal_cmd[index].function(argc, argv);
```

物理行 141–141；源码定义体/有效宏短窗。

## E131-013 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	int err = 0;
	if (argc != 2)
		return -1;
```

物理行 351–353；源码定义体/有效宏短窗。

## E131-014 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	char *dirname = argv[1];
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

```

物理行 355–359；源码定义体/有效宏短窗。

## E131-015 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	return err;
```

物理行 360–360；源码定义体/有效宏短窗。

## E131-016 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	int err = 0;
	if (argc != 2)
		return -1;
```

物理行 365–367；源码定义体/有效宏短窗。

## E131-017 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	char *filename = argv[1];
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);
```

物理行 369–372；源码定义体/有效宏短窗。

## E131-018 · VERIFIED

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	return err;
```

物理行 374–374；源码定义体/有效宏短窗。

## E131-019 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOENT]
```c
	#define ENOENT			0x02
```

物理行 424–424；源码定义体/有效宏短窗。

## E131-020 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::EACCES]
```c
	#define EACCES			0x0d
```

物理行 435–435；源码定义体/有效宏短窗。

## E131-021 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOTEMPTY]
```c
	#define ENOTEMPTY		0x42
```

物理行 489–489；源码定义体/有效宏短窗。

## E131-022 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[0] = "Success",
```

物理行 25–25；源码定义体/有效宏短窗。

## E131-023 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[ENOENT] = "No such file or directory",
#endif
```

物理行 39–40；源码定义体/有效宏短窗。

## E131-024 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[EACCES] = "Permission denied",
#endif
```

物理行 127–128；源码定义体/有效宏短窗。

## E131-025 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[ENOTEMPTY] = "Directory not empty",
#endif
```

物理行 541–542；源码定义体/有效宏短窗。

## E131-026 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist.c::__get_errlist]
```c
   if (errnum < 0)
      errnum = -errnum;
   return _sys_errlist_internal[errnum];
```

物理行 27–29；源码定义体/有效宏短窗。

## E131-027 · LOCATION_DECLARATION

[SOURCE_LOCATION_DECLARATION myinitramfs/mylib/myglibc/include/stdio.h::_sys_errlist_internal]
```c
	extern const char *const _sys_errlist_internal[];
```

物理行 184–184；声明、注释、分派条目或配置定位，不计正式引文。

## E131-028 · LOCATION_DECLARATION

[SOURCE_LOCATION_DECLARATION myinitramfs/mylib/myglibc/include/stdio.h::__get_errlist]
```c
	extern const char *__get_errlist(int);
```

物理行 186–186；声明、注释、分派条目或配置定位，不计正式引文。

## E131-029 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
```

物理行 18–18；源码定义体/有效宏短窗。

## E131-030 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	write(stdout->_fileno, buf, rv);
```

物理行 22–22；源码定义体/有效宏短窗。

## E131-031 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	return rv;
```

物理行 25–25；源码定义体/有效宏短窗。

## E131-032 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
```

物理行 783–786；源码定义体/有效宏短窗。

## E131-033 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
```

物理行 805–808；源码定义体/有效宏短窗。

## E131-034 · VERIFIED

[VERIFIED mykernel/arch/x86_64/include/asm/unistd_64.h::__NR_sched_yield]
```c
	#define __NR_sched_yield	24
```

物理行 32–32；源码定义体/有效宏短窗。

## E131-035 · LOCATION_DISPATCH_ENTRY

[SOURCE_LOCATION_DISPATCH_ENTRY mykernel/arch/x86_64/include/asm/syscalls_64.h::sys_sched_yield]
```c
__SYSCALL(__NR_sched_yield, sys_sched_yield)
```

物理行 24–24；声明、注释、分派条目或配置定位，不计正式引文。

## E131-036 · VERIFIED

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::__SYSCALL]
```c
#define __SYSCALL(nr, sym) case nr: return __x64_##sym(regs);
```

物理行 26–26；源码定义体/有效宏短窗。

## E131-037 · VERIFIED

[VERIFIED mykernel/arch/x86_64/entry/syscall_64.c::x64_sys_call]
```c
	switch (nr) {
	#include <asm/syscalls_64.h>
	default: return __x64_sys_ni_syscall(regs);
```

物理行 29–31；源码定义体/有效宏短窗。

## E131-038 · VERIFIED

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		rq_flags_s rf;
		rq_s *rq;

		rq = this_rq_lock_irq(&rf);
```

物理行 26–29；源码定义体/有效宏短窗。

## E131-039 · VERIFIED

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		// schedstat_inc(rq->yld_count);
		current->sched_class->yield_task(rq);

		preempt_disable();
```

物理行 31–34；源码定义体/有效宏短窗。

## E131-040 · LOCATION_COMMENT

[SOURCE_LOCATION_COMMENT mykernel/sched/syscall.c::sched_yield]
```c
		// rq_unlock_irq(rq, &rf);
```

物理行 35–35；声明、注释、分派条目或配置定位，不计正式引文。

## E131-041 · VERIFIED

[VERIFIED mykernel/sched/syscall.c::sched_yield]
```c
		sched_preempt_enable_no_resched();

		schedule();
	// }
	return 0;
```

物理行 36–40；源码定义体/有效宏短窗。

## E131-042 · VERIFIED

[VERIFIED mykernel/sched/scheduler/scheduler_types.h::sched_class]
```c
		void	(*yield_task) (rq_s *rq);
```

物理行 68–68；源码定义体/有效宏短窗。

## E131-043 · VERIFIED

[VERIFIED mykernel/sched/scheduler/myos_rt.c::yield_task_fair_myos]
```c
	resched_curr(rq);
```

物理行 62–62；源码定义体/有效宏短窗。

## E131-044 · VERIFIED

[VERIFIED mykernel/sched/scheduler/myos_rt.c::myos_rt]
```c
DEFINE_SCHED_CLASS(myos_rt) = {
	.yield_task			= yield_task_fair_myos,
	.pick_next_task		= pick_next_task_myos,
```

物理行 65–67；源码定义体/有效宏短窗。

## E131-045 · VERIFIED

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::resched_curr]
```c
	task_s *curr = rq->curr;
	int cpu;
```

物理行 734–735；源码定义体/有效宏短窗。

## E131-046 · VERIFIED

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::resched_curr]
```c
	if (test_tsk_need_resched(curr))
		return;
```

物理行 739–740；源码定义体/有效宏短窗。

## E131-047 · VERIFIED

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::resched_curr]
```c
	cpu = cpu_of(rq);
```

物理行 742–742；源码定义体/有效宏短窗。

## E131-048 · VERIFIED

[VERIFIED mykernel/sched/scheduler/scheduler_core.c::resched_curr]
```c
	if (cpu == smp_processor_id()) {
		set_tsk_need_resched(curr);
		set_preempt_need_resched();
		return;
```

物理行 744–747；源码定义体/有效宏短窗。

## E131-049 · LOCATION_COMMENT

[SOURCE_LOCATION_COMMENT mykernel/sched/scheduler/scheduler_core.c::resched_curr]
```c
	// if (set_nr_and_not_polling(curr))
	// 	smp_send_reschedule(cpu);
	// else
	// 	trace_sched_wake_idle_without_ipi(cpu);
```

物理行 750–753；声明、注释、分派条目或配置定位，不计正式引文。

## E131-050 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 1:
		a1 = ((op_t *)srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
```

物理行 82–86；源码定义体/有效宏短窗。

## E131-051 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
		goto do8; /* No-op.  */
```

物理行 86–89；源码定义体/有效宏短窗。

## E131-052 · VERIFIED

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
do0:
	((op_t *)dstp)[0] = a1;
```

物理行 126–127；源码定义体/有效宏短窗。

## E131-053 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
```

物理行 41–41；声明、注释、分派条目或配置定位，不计正式引文。

## E131-054 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG myinitramfs/CMakeLists.txt::MUSL_PATH]
```c
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)

set(MUSL_INC_PATH ${MUSL_PATH}/include)
set(MUSL_LIB_PATH ${MUSL_PATH}/lib)
```

物理行 40–44；配置、声明或注释定位，不计正式引文。

## E131-055 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG myinitramfs/CMakeLists.txt::boot_targets]
```c
add_subdirectory(myinit)
add_subdirectory(myshell)
```

物理行 47–48；配置、声明或注释定位，不计正式引文。

## E131-056 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG myinitramfs/CMakeLists.txt::boot_install]
```c
install(TARGETS init DESTINATION boot)
install(TARGETS sh DESTINATION boot)
```

物理行 50–51；配置、声明或注释定位，不计正式引文。

## E131-057 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG myinitramfs/myinit/CMakeLists.txt::init]
```c
file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
```

物理行 1–4；配置、声明或注释定位，不计正式引文。

## E131-058 · LOCATION_CONFIG

[SOURCE_LOCATION_CONFIG myinitramfs/myshell/CMakeLists.txt::sh]
```c
file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
```

物理行 1–4；配置、声明或注释定位，不计正式引文。

## E131-059 · LOCATION_COMMENT

[SOURCE_LOCATION_COMMENT myinitramfs/mylib/myglibc/string/string.h::strerror]
```c
	// extern char *strerror(int __errnum) __THROW;
```

物理行 416–416；配置、声明或注释定位，不计正式引文。

## E131-060 · VERIFIED

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

物理行 79–79；活动源码定义短窗。
