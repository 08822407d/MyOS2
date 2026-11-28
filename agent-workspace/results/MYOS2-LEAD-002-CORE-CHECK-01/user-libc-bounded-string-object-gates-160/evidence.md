# 第160批固定源码证据（新正式窗独立计数）

固定time@a039d9803ade。正式窗只取定义体、真实宏和P4内核活动配置；声明、typedef、include及用户CMake均列普通定位。每窗连续1–5物理行；原TAB/LF/CRLF/NONE保留，NONE围栏分隔LF非源字节。行号只作辅助导航。旧159分类补正另见D00，不计新P6。

## BS160-E001 · strnlenRename

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	#define __strnlen STRNLEN
```

myinitramfs/mylib/myglibc/string/strnlen.c:29-29；blob短12 3525b2bc96b4；行尾 LF。

## BS160-E002 · endInit

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	const char *char_ptr, *end_ptr = str + maxlen;
	const unsigned long int *longword_ptr;
	unsigned long int longword, himagic, lomagic;
```

myinitramfs/mylib/myglibc/string/strnlen.c:35-37；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E003 · zero

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	if (maxlen == 0)
		return 0;
```

myinitramfs/mylib/myglibc/string/strnlen.c:39-40；blob短12 3525b2bc96b4；行尾 LF,LF。

## BS160-E004 · fallback

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	if (end_ptr < str)
		end_ptr = (const char *)~0UL;
```

myinitramfs/mylib/myglibc/string/strnlen.c:42-43；blob短12 3525b2bc96b4；行尾 LF,LF。

## BS160-E005 · prefix

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	for (char_ptr = str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
		 ++char_ptr)
		if (*char_ptr == '\0')
```

myinitramfs/mylib/myglibc/string/strnlen.c:47-49；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E006 · prefixClamp

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			if (char_ptr > end_ptr)
				char_ptr = end_ptr;
			return char_ptr - str;
```

myinitramfs/mylib/myglibc/string/strnlen.c:51-53；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E007 · wordPtr

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	longword_ptr = (unsigned long int *)char_ptr;
```

myinitramfs/mylib/myglibc/string/strnlen.c:59-59；blob短12 3525b2bc96b4；行尾 LF。

## BS160-E008 · magic

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
```

myinitramfs/mylib/myglibc/string/strnlen.c:70-72；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E009 · magic64

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}
```

myinitramfs/mylib/myglibc/string/strnlen.c:75-78；blob短12 3525b2bc96b4；行尾 LF,LF,LF,LF。

## BS160-E010 · wordGate

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	while (longword_ptr < (unsigned long int *)end_ptr)
	{
```

myinitramfs/mylib/myglibc/string/strnlen.c:85-86；blob短12 3525b2bc96b4；行尾 LF,LF。

## BS160-E011 · wordRead

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		longword = *longword_ptr++;

		if ((longword - lomagic) & himagic)
		{
```

myinitramfs/mylib/myglibc/string/strnlen.c:116-119；blob短12 3525b2bc96b4；行尾 LF,LF,LF,LF。

## BS160-E012 · cp

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			const char *cp = (const char *)(longword_ptr - 1);

			char_ptr = cp;
			if (cp[0] == 0)
				break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:123-127；blob短12 3525b2bc96b4；行尾 LF,LF,LF,LF,LF。

## BS160-E013 · cp1

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			char_ptr = cp + 1;
			if (cp[1] == 0)
				break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:128-130；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E014 · cp2

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			char_ptr = cp + 2;
			if (cp[2] == 0)
				break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:131-133；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E015 · cp3

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			char_ptr = cp + 3;
			if (cp[3] == 0)
				break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:134-136；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E016 · cp64

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
			if (sizeof(longword) > 4)
			{
				char_ptr = cp + 4;
				if (cp[4] == 0)
					break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:137-141；blob短12 3525b2bc96b4；行尾 LF,LF,LF,LF,LF。

## BS160-E017 · cp5

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
				char_ptr = cp + 5;
				if (cp[5] == 0)
					break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:142-144；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E018 · cp6

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
				char_ptr = cp + 6;
				if (cp[6] == 0)
					break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:145-147；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E019 · cp7

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
				char_ptr = cp + 7;
				if (cp[7] == 0)
					break;
```

myinitramfs/mylib/myglibc/string/strnlen.c:148-150；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E020 · afterWord

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
		char_ptr = end_ptr;
	}
```

myinitramfs/mylib/myglibc/string/strnlen.c:153-154；blob短12 3525b2bc96b4；行尾 LF,LF。

## BS160-E021 · endClamp

[VERIFIED myinitramfs/mylib/myglibc/string/strnlen.c::__strnlen]
```c
	if (char_ptr > end_ptr)
		char_ptr = end_ptr;
	return char_ptr - str;
```

myinitramfs/mylib/myglibc/string/strnlen.c:156-158；blob短12 3525b2bc96b4；行尾 LF,LF,LF。

## BS160-E022 · strncpyDefault

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	#define STRNCPY strncpy
```

myinitramfs/mylib/myglibc/string/strncpy.c:23-23；blob短12 b661abdab724；行尾 LF。

## BS160-E023 · sizeCall

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	size_t size = __strnlen(s2, n);
```

myinitramfs/mylib/myglibc/string/strncpy.c:29-29；blob短12 b661abdab724；行尾 LF。

## BS160-E024 · zeroCall

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	if (size != n)
		memset(s1 + size, '\0', n - size);
```

myinitramfs/mylib/myglibc/string/strncpy.c:30-31；blob短12 b661abdab724；行尾 LF,LF。

## BS160-E025 · copyReturn

[VERIFIED myinitramfs/mylib/myglibc/string/strncpy.c::STRNCPY]
```c
	return memcpy(s1, s2, size);
```

myinitramfs/mylib/myglibc/string/strncpy.c:32-32；blob短12 b661abdab724；行尾 LF。

## BS160-E026 · setInit

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	long int dstp = (long int)dstpp;

	if (len >= 8)
```

myinitramfs/mylib/myglibc/string/memset.c:24-26；blob短12 bd50b22c66bf；行尾 LF,LF,LF。

## BS160-E027 · setShort

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	while (len > 0)
	{
		((byte *)dstp)[0] = c;
		dstp += 1;
		len -= 1;
```

myinitramfs/mylib/myglibc/string/memset.c:76-80；blob短12 bd50b22c66bf；行尾 LF,LF,LF,LF,LF。

## BS160-E028 · setRet

[VERIFIED myinitramfs/mylib/myglibc/string/memset.c::memset]
```c
	return dstpp;
```

myinitramfs/mylib/myglibc/string/memset.c:83-83；blob短12 bd50b22c66bf；行尾 LF。

## BS160-E029 · memcpyDefault

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	#define MEMCPY memcpy
```

myinitramfs/mylib/myglibc/string/memcpy.c:24-24；blob短12 b7c61fb0c0d4；行尾 LF。

## BS160-E030 · copyInit

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	unsigned long int dstp = (long int)dstpp;
	unsigned long int srcp = (long int)srcpp;
```

myinitramfs/mylib/myglibc/string/memcpy.c:30-31；blob短12 b7c61fb0c0d4；行尾 LF,LF。

## BS160-E031 · copyGate

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	if (len >= OP_T_THRES)
```

myinitramfs/mylib/myglibc/string/memcpy.c:36-36；blob短12 b7c61fb0c0d4；行尾 LF。

## BS160-E032 · tailMacro

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	BYTE_COPY_FWD(dstp, srcp, len);
```

myinitramfs/mylib/myglibc/string/memcpy.c:57-57；blob短12 b7c61fb0c0d4；行尾 LF。

## BS160-E033 · copyRet

[VERIFIED myinitramfs/mylib/myglibc/string/memcpy.c::MEMCPY]
```c
	return dstpp;
```

myinitramfs/mylib/myglibc/string/memcpy.c:59-59；blob短12 b7c61fb0c0d4；行尾 LF。

## BS160-E034 · opType

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::op_t]
```c
	#define op_t unsigned long int
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:63-63；blob短12 61d668720747；行尾 LF。

## BS160-E035 · opSize

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OPSIZ]
```c
	#define OPSIZ (sizeof(op_t))
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:64-64；blob短12 61d668720747；行尾 LF。

## BS160-E036 · byteMacro1

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
	#define BYTE_COPY_FWD(dst_bp, src_bp, nbytes) \
		do                                        \
		{                                         \
			size_t __nbytes = (nbytes);           \
			while (__nbytes > 0)                  \
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:78-82；blob短12 61d668720747；行尾 LF,LF,LF,LF,LF。

## BS160-E037 · byteMacro2

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
			{                                     \
				byte __x = ((byte *)src_bp)[0];   \
				src_bp += 1;                      \
				__nbytes -= 1;                    \
				((byte *)dst_bp)[0] = __x;        \
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:83-87；blob短12 61d668720747；行尾 LF,LF,LF,LF,LF。

## BS160-E038 · byteMacro3

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::BYTE_COPY_FWD]
```c
				dst_bp += 1;                      \
			}                                     \
		} while (0)
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:88-90；blob短12 61d668720747；行尾 LF,LF,LF。

## BS160-E039 · threshold

[VERIFIED myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h::OP_T_THRES]
```c
	#define OP_T_THRES 16
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:199-199；blob短12 61d668720747；行尾 LF。

## BS160-E040 · kernelFlags

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -mcmodel=kernel -fno-pie -fno-pic \
	-ffreestanding -nostartfiles \
	-nostdinc -nostdlib \
```

mykernel/scripts/options_flags.cmake:39-43；blob短12 6019436417ad；行尾 LF,LF,LF,LF,LF。

## BS160-E041 · kernelConfig

[VERIFIED mykernel/scripts/options_flags.cmake::set(CMAKE_C_FLAGS)]
```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-D__x86_64__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
```

mykernel/scripts/options_flags.cmake:58-62；blob短12 6019436417ad；行尾 LF,LF,LF,LF,LF。

## 普通定位（不计正式窗）

### BS160-L01 · internalStrnlen

```text
	extern size_t __strnlen(const char *__string, size_t __maxlen);
```

myinitramfs/mylib/myglibc/include/string.h:13-13；blob短12 962bf46c48d6；行尾 LF。

### BS160-L02 · stringInclude

```text
	#include <string/string.h>
```

myinitramfs/mylib/myglibc/include/string.h:62-62；blob短12 962bf46c48d6；行尾 LF。

### BS160-L03 · memcpyDecl

```text
	extern void *memcpy(void *__dest, const void *__src, size_t __len);
```

myinitramfs/mylib/myglibc/include/string.h:179-179；blob短12 962bf46c48d6；行尾 LF。

### BS160-L04 · publicStrnlen

```text
	extern size_t strnlen(const char *__string, size_t __maxlen);
```

myinitramfs/mylib/myglibc/string/string.h:412-412；blob短12 23305a96a57a；行尾 LF。

### BS160-L05 · memsetDecl

```text
	extern void *memset(void *__s, int __c, size_t __n);
```

myinitramfs/mylib/myglibc/string/string.h:58-58；blob短12 23305a96a57a；行尾 LF。

### BS160-L06 · sizeRoute

```text
	#define __need_size_t
	#define __need_NULL
	#include <stddef.h>
```

myinitramfs/mylib/myglibc/string/string.h:30-32；blob短12 23305a96a57a；行尾 LF,LF,LF。

### BS160-L07 · project

```text
project(MyOS2_Initramfs ASM C)
```

myinitramfs/CMakeLists.txt:3-3；blob短12 6757d21bc64c；行尾 LF。

### BS160-L08 · userFlags

```text
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 \
	-ggdb \
")
```

myinitramfs/CMakeLists.txt:26-29；blob短12 6757d21bc64c；行尾 LF,LF,LF,LF。

### BS160-L09 · externalCompiler

```text
set(MUSL_PATH /home/cheyh/projs/musl/build)
set(CMAKE_C_COMPILER ${MUSL_PATH}/exec/bin/musl-gcc)

set(MUSL_INC_PATH ${MUSL_PATH}/include)
```

myinitramfs/CMakeLists.txt:40-43；blob短12 6757d21bc64c；行尾 LF,LF,LF,LF。

### BS160-L10 · userIncludes

```text
include_directories("${MUSL_INC_PATH}")
add_subdirectory(myinit)
add_subdirectory(myshell)
```

myinitramfs/CMakeLists.txt:46-48；blob短12 6757d21bc64c；行尾 LF,LF,LF。

### BS160-L11 · shellTarget

```text
file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
```

myinitramfs/myshell/CMakeLists.txt:1-4；blob短12 dcdac0564c0e；行尾 LF,LF,LF,LF。

### BS160-L12 · libSub

```text
add_subdirectory(myglibc)
add_subdirectory(csu)
add_subdirectory(util)
```

myinitramfs/mylib/CMakeLists.txt:1-3；blob短12 8f6abc24095a；行尾 LF,LF,NONE。

### BS160-L13 · libTarget

```text
add_library(mylibc STATIC)
```

myinitramfs/mylib/myglibc/CMakeLists.txt:1-1；blob短12 b956957a602a；行尾 LF。

### BS160-L14 · libInclude1

```text
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
```

myinitramfs/mylib/myglibc/CMakeLists.txt:3-7；blob短12 b956957a602a；行尾 LF,LF,LF,LF,LF。

### BS160-L15 · libInclude2

```text
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86
```

myinitramfs/mylib/myglibc/CMakeLists.txt:8-13；blob短12 b956957a602a；行尾 LF,LF,LF,LF,LF,LF。

### BS160-L16 · libString

```text
add_subdirectory(string)
```

myinitramfs/mylib/myglibc/CMakeLists.txt:20-20；blob短12 b956957a602a；行尾 LF。

### BS160-L17 · stringSources

```text
file(GLOB LIBC_STRING_C_SRCS *.c)
file(GLOB LIBC_STRING_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_STRING_C_SRCS} ${LIBC_STRING_ASM_SRCS})
```

myinitramfs/mylib/myglibc/string/CMakeLists.txt:1-3；blob短12 3b2073525ead；行尾 LF,LF,NONE。

### BS160-L18 · byteTypedef

```text
	typedef unsigned char byte;
```

myinitramfs/mylib/myglibc/sysdeps/generic/memcopy.h:67-67；blob短12 61d668720747；行尾 LF。

### BS160-L19 · panicCanary

```text
	this_cpu = smp_processor_id();
```

mykernel/debug/panic.c:79-79；blob短12 2e9d5b195559；行尾 LF。

## D00／R159-01旧证据分类（不计本批P6）

固定旧159 evidence.md@ff7c39dea3dd，blob 0174a1bcc889；旧ID及原窗位置、字节和行尾不变。下表25项仅替换章节内的标签类别，具体唯一物理行和完整85项映射在facts的D00_correction.classification_map。原其余60正式窗保留（E001–E054、E060/E063/E065/E066、E084/E085，其中最后二窗是P4）。原23普通保留，组合48。旧历史85/85/0保留，组合60/60/0，不计本批新P6。

| 旧ID | 旧 evidence 标签物理行 | 源位置 | 类别 |
|---|---:|---|---|
| FA159-E055 | 651 | myinitramfs/mylib/myglibc/string/string.h:405–405 | extern declaration; not function body/ABI evidence |
| FA159-E056 | 660 | myinitramfs/mylib/myglibc/include/stdio.h:70–70 | extern declaration; not function body/ABI evidence |
| FA159-E057 | 669 | myinitramfs/mylib/myglibc/libio/stdio.h:131–131 | extern declaration; not function body/ABI evidence |
| FA159-E058 | 678 | myinitramfs/mylib/myglibc/libio/stdio.h:367–367 | extern declaration; not function body/ABI evidence |
| FA159-E059 | 687 | myinitramfs/mylib/myglibc/bits/types/FILE.h:8–8 | type declaration; not function body/ABI evidence |
| FA159-E061 | 706 | myinitramfs/mylib/myglibc/posix/unistd.h:216–219 | type declaration; not function body/ABI evidence |
| FA159-E062 | 718 | myinitramfs/mylib/myglibc/posix/unistd.h:377–377 | extern declaration; not function body/ABI evidence |
| FA159-E064 | 736 | myinitramfs/mylib/myglibc/posix/bits/types.h:193–193 | type declaration; not function body/ABI evidence |
| FA159-E067 | 763 | myinitramfs/mylib/myglibc/posix/sys/types.h:105–108 | type declaration; not function body/ABI evidence |
| FA159-E068 | 775 | myinitramfs/CMakeLists.txt:26–29 | user CMake text; not C definition/actual linking |
| FA159-E069 | 787 | myinitramfs/CMakeLists.txt:31–33 | user CMake text; not C definition/actual linking |
| FA159-E070 | 798 | myinitramfs/CMakeLists.txt:40–40 | user CMake text; not C definition/actual linking |
| FA159-E071 | 807 | myinitramfs/CMakeLists.txt:41–41 | user CMake text; not C definition/actual linking |
| FA159-E072 | 816 | myinitramfs/CMakeLists.txt:43–43 | user CMake text; not C definition/actual linking |
| FA159-E073 | 825 | myinitramfs/CMakeLists.txt:46–46 | user CMake text; not C definition/actual linking |
| FA159-E074 | 834 | myinitramfs/CMakeLists.txt:47–47 | user CMake text; not C definition/actual linking |
| FA159-E075 | 843 | myinitramfs/CMakeLists.txt:48–48 | user CMake text; not C definition/actual linking |
| FA159-E076 | 852 | myinitramfs/myshell/CMakeLists.txt:4–4 | user CMake text; not C definition/actual linking |
| FA159-E077 | 861 | myinitramfs/mylib/CMakeLists.txt:1–1 | user CMake text; not C definition/actual linking |
| FA159-E078 | 870 | myinitramfs/mylib/myglibc/CMakeLists.txt:1–1 | user CMake text; not C definition/actual linking |
| FA159-E079 | 879 | myinitramfs/mylib/myglibc/CMakeLists.txt:3–7 | user CMake text; not C definition/actual linking |
| FA159-E080 | 892 | myinitramfs/mylib/myglibc/CMakeLists.txt:18–18 | user CMake text; not C definition/actual linking |
| FA159-E081 | 901 | myinitramfs/mylib/myglibc/CMakeLists.txt:20–20 | user CMake text; not C definition/actual linking |
| FA159-E082 | 910 | myinitramfs/mylib/myglibc/stdio/CMakeLists.txt:3–3 | user CMake text; not C definition/actual linking |
| FA159-E083 | 919 | myinitramfs/mylib/myglibc/string/CMakeLists.txt:3–3 | user CMake text; not C definition/actual linking |

## 回核账

新正式窗41/41/0；五核心源39窗另次重开同文，两个P4窗独立双读同文，合107个LF源物理行。18普通位置另次重开，panic金丝雀本轮重复读及time/master反验，合19普通。旧159的85原窗/60组合及48普通另账，不复制其完整正式标签。用户侧声明/构建仅定位；外部ABI和链接未证。完整本地P9及项目运行NOT_RUN。
