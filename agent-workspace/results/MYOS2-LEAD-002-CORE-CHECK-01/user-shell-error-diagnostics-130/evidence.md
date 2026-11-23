---
task_id: MYOS2-LEAD-002-CORE-CHECK-01
followup_id: CORE-USER-SHELL-ERROR-DIAGNOSTICS-130
executor_identity: MYOS2-LEAD-002-CHAT-EXEC-20
produced_by: GPT-6 (self-reported; backend/effort UNATTESTED)
model_per_owner: GPT-6＋Extra
date: 2026-11-23
base_snapshot: time
read_channel: connector
status: READY_FOR_MAINLINE_REVIEW
mainline_acceptance: PENDING
---
# 第130批新增源码短窗口

从time固定源独立提取；各窗口单定义体连续1–5行，保留制表符、CRLF。历史129的76窗不是本批新增。导航行号不能代替路径+符号。

## E130-001

[VERIFIED myinitramfs/myshell/myshell.c::shell_internal_cmd]
```c
	{"cd",		cd_command},
	{"ls",		ls_command},
	{"pwd",		pwd_command},
	{"cat",		cat_command},
	{"touch",	touch_command},
```

原物理行 46–50。

## E130-002

[VERIFIED myinitramfs/myshell/myshell.c::main]
```c
	test_PrintDateTime();

	printf("This is MyShell v0.01.\n");
	printf("You are login as root.\n");
```

原物理行 68–71。

## E130-003

[VERIFIED myinitramfs/myshell/myshell.c::run_cmd]
```c
void run_cmd(int index, int argc, char **argv)
{
	shell_internal_cmd[index].function(argc, argv);
}
```

原物理行 139–142。

## E130-004

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	i = chdir(path);
	if(!i)
	{
		memset(current_dir, 0, PATH_MAX);
```

原物理行 203–206。

## E130-005

[VERIFIED myinitramfs/myshell/myshell.c::cd_command]
```c
	else
		printf("cd: %s: %s\n", strerror(errno), path);
	return 1;
```

原物理行 209–211。

## E130-006

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	dir = opendir(path);
	if (dir == NULL)
	{
		printf("ls: %s: %s\n", strerror(errno), path);
		return -1;
```

原物理行 224–228。

## E130-007

[VERIFIED myinitramfs/myshell/myshell.c::ls_command]
```c
	{
		printf("ls: %s: %s\n", strerror(errno), path);
		return -1;
```

原物理行 226–228。

## E130-008

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	len = strlen(current_dir);
	i = len + strlen(argv[1]);
	filename = malloc(i + 2);
	memset(filename, 0, i + 2);
	strcpy(filename, current_dir);
```

原物理行 260–264。

## E130-009

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	if(len > 1)
		filename[len] = '/';
	strcat(filename, argv[1]);
```

原物理行 265–267。

## E130-010

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	fd = open(filename, O_RDONLY, 0);	
	if (fd < 0)
	{
		printf("ls: %s: %s\n", strerror(errno), filename);
		return -1;
```

原物理行 270–274。

## E130-011

[VERIFIED myinitramfs/myshell/myshell.c::cat_command]
```c
	{
		printf("ls: %s: %s\n", strerror(errno), filename);
		return -1;
```

原物理行 272–274。

## E130-012

[VERIFIED myinitramfs/myshell/myshell.c::rmdir_command]
```c
	err = rmdir(dirname);
	if (err != 0)
		printf("rmdir: %s: %s\n", strerror(errno), dirname);

	return err;
```

原物理行 356–360。

## E130-013

[VERIFIED myinitramfs/myshell/myshell.c::rm_command]
```c
	err = unlink(filename);
	if (err != 0)
		printf("rm: %s: %s\n", strerror(errno), filename);

	return err;
```

原物理行 370–374。

## E130-014

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOENT]
```c
	// #define ENOENT 0x40000002
```

原物理行 235–235。

## E130-015

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::EACCES]
```c
	// #define EACCES 0x4000000d
```

原物理行 246–246。

## E130-016

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/bits/errno.h::ENOTEMPTY]
```c
	// #define ENOTEMPTY 0x40000042
```

原物理行 300–300。

## E130-017

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
const char *const _sys_errlist_internal[] =
{
	[0] = "Success",
```

原物理行 23–25。

## E130-018

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
#ifdef ENOENT
```

原物理行 34–34。

## E130-019

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[ENOENT] = "No such file or directory",
#endif
```

原物理行 39–40。

## E130-020

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
#ifdef EACCES
```

原物理行 124–124。

## E130-021

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[EACCES] = "Permission denied",
#endif
```

原物理行 127–128。

## E130-022

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
#ifdef ENOTEMPTY
```

原物理行 537–537。

## E130-023

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist_internal.c::_sys_errlist_internal]
```c
	[ENOTEMPTY] = "Directory not empty",
#endif
```

原物理行 541–542。

## E130-024

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist.c::__get_errlist]
```c
   if (errnum < 0)
      errnum = -errnum;
   return _sys_errlist_internal[errnum];
```

原物理行 27–29。

## E130-025

[VERIFIED myinitramfs/mylib/myglibc/stdio/errlist.c::__get_errlist]
```c
   return _sys_errlist_internal[errnum];
   // return NULL;
```

原物理行 29–30。

## E130-026

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
				case 's':		/* String */
					sarg = va_arg(ap, const char *);
					sarg = sarg ? sarg : "(null)";
					slen = strlen(sarg);
					goto is_string;
```

原物理行 783–787。

## E130-027

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
						for (i = slen; i; i--)
						{
							sch = *sarg++;
							EMIT(sch);
						}
```

原物理行 805–809。

## E130-028

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	if (o < n)
		*q = '\0';				/* No overflow */
	else if (n > 0)
		buf[n - 1] = '\0';		/* Overflow - terminate at end of buffer */
```

原物理行 871–874。

## E130-029

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::vsnprintf]
```c
	return o;
```

原物理行 876–876。

## E130-030

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	va_start(ap, fmt);
	rv = vsnprintf(buf, SZ_4K, fmt, ap);
	va_end(ap);
```

原物理行 17–19。

## E130-031

[VERIFIED myinitramfs/mylib/myglibc/stdio/printf.c::printf]
```c
	// buffered IO has unsolved bug, so derectly use the write() function
	write(stdout->_fileno, buf, rv);
	// rv = (fputs(buf, stdout) < 0) ? 0 : rv;
	// fflush(stdout);
```

原物理行 21–24。

## E130-032

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	else if (val == 0)
	{
		ndigits = 1;		/* Zero still requires space */
```

原物理行 91–93。

## E130-033

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
	if (((flags & (FL_MINUS | FL_ZERO)) == FL_ZERO) && (width > ndigits))
	{
		while (width > nchars) {
			EMIT('0');
			width--;
```

原物理行 156–160。

## E130-034

[VERIFIED myinitramfs/mylib/myglibc/stdio/vsnprintf.c::format_int]
```c
		oo--;
		ndigits--;
		if (oo < n)
			*qq = digits[val % base];
		val /= base;
```

原物理行 185–189。

## E130-035

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		BYTE_COPY_FWD(dstp, srcp, (-dstp) % OPSIZ);
```

原物理行 40–40。

## E130-036

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		PAGE_COPY_FWD_MAYBE(dstp, srcp, len, len);
```

原物理行 45–45。

## E130-037

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
		WORD_COPY_FWD(dstp, srcp, len, len);
```

原物理行 51–51。

## E130-038

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	BYTE_COPY_FWD(dstp, srcp, len);

	return dstpp;
```

原物理行 57–59。

## E130-039

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::WORD_COPY_FWD]
```c
		{                                                                     \
			if (src_bp % OPSIZ == 0)                                          \
				_wordcopy_fwd_aligned(dst_bp, src_bp, (nbytes) / OPSIZ);      \
			else                                                              \
				_wordcopy_fwd_dest_aligned(dst_bp, src_bp, (nbytes) / OPSIZ); \
```

原物理行 123–127。

## E130-040

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return;
		a0 = ((op_t *)srcp)[0];
		srcp -= 0 * OPSIZ;
```

原物理行 75–79。

## E130-041

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
	case 1:
		a1 = ((op_t *)srcp)[0];
		srcp -= -1 * OPSIZ;
		dstp -= 0 * OPSIZ;
		len -= 1;
```

原物理行 82–86。

## E130-042

[VERIFIED myinitramfs/mylib/myglibc/string/wordcopy.c::WORDCOPY_FWD_ALIGNED]
```c
		dstp += 8 * OPSIZ;
		len -= 8;
	} while (len != 0);

	/* This is the right position for do0.  Please don't move
```

原物理行 120–124。

## E130-043

[VERIFIED mykernel/scripts/options_flags.cmake::CMAKE_C_FLAGS]
```c
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
```

原物理行 40–42。

## E130-044

[VERIFIED mykernel/debug/panic.c::panic]
```c
	this_cpu = smp_processor_id();
```

原物理行 79–79。
