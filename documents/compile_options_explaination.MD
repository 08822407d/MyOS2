Compile Options Explainations:

 -m32
 -m64
 -mx32
 -m16
 -miamcu
 https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
	Generate code for a 16-bit, 32-bit or 64-bit environment. The -m32 option sets int, long, and pointer types to 32 bits, and generates code that runs on any i386 system.

	The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture. For Darwin only the -m64 option also turns off the -fno-pic and -mdynamic-no-pic options.

	The -mx32 option sets int, long, and pointer types to 32 bits, and generates code for the x86-64 architecture.

	The -m16 option is the same as -m32, except for that it outputs the .code16gcc assembly directive at the beginning of the assembly output so that the binary can run in 16-bit mode.

	The -miamcu option generates code which conforms to Intel MCU psABI. It requires the -m32 option to be turned on.

 -mcmodel=large
 https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
   Generate code for the large model. This model makes no assumptions about addresses and sizes of sections.

 -fpic
 https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
	Generate position-independent code (PIC) suitable for use in a shared library, if supported for the target machine. Such code accesses all constant addresses through a global offset table (GOT). The dynamic loader resolves the GOT entries when the program starts (the dynamic loader is not part of GCC; it is part of the operating system). If the GOT size for the linked executable exceeds a machine-specific maximum size, you get an error message from the linker indicating that -fpic does not work; in that case, recompile with -fPIC instead. (These maximums are 8k on the SPARC, 28k on AArch64 and 32k on the m68k and RS/6000. The x86 has no such limit.)

 -fPIC
 https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
	If supported for the target machine, emit position-independent code, suitable for dynamic linking and avoiding any limit on the size of the global offset table. This option makes a difference on AArch64, m68k, PowerPC and SPARC.

 -ffreestanding
 https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
   Assert that compilation targets a freestanding environment. This implies -fno-builtin. A freestanding environment is one in which the standard library may not exist, and program startup may not necessarily be at main. The most obvious example is an OS kernel. This is equivalent to -fno-hosted.

 -nostartfiles
 https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
	Do not use the standard system startup files when linking. The standard system libraries are used normally, unless -nostdlib, -nolibc, or -nodefaultlibs is used.

 -nostdinc
 https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
   Do not search the standard system directories for header files. Only the directories explicitly specified with -I, -iquote, -isystem, and/or -idirafter options (and the directory of the current file, if appropriate) are searched.

 -nostdlib
 https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
	Do not use the standard system startup files or libraries when linking. No startup files and only the libraries you specify are passed to the linker, and options specifying linkage of the system libraries, such as -static-libgcc or -shared-libgcc, are ignored.

 -fstack-protector (the opposite of -fno-stack-protector)
 https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
	Emit extra code to check for buffer overflows, such as stack smashing attacks. This is done by adding a guard variable to functions with vulnerable objects. This includes functions that call alloca, and functions with buffers larger than or equal to 8 bytes. The guards are initialized when a function is entered and then checked when the function exits. If a guard check fails, an error message is printed and the program exits. Only variables that are actually allocated on the stack are considered, optimized away variables or variables allocated in registers don’t count.

 -funwind-tables (the opposite of -fno-unwind-tables)
 https://gcc.gnu.org/onlinedocs/gcc-4.2.4/gcc/Code-Gen-Options.html
 	Similar to -fexceptions, except that it will just generate any needed static data, but will not affect the generated code in any other way. You will normally not enable this option; instead, a language processor that needs this handling would enable it on your behalf.

 -mabi=name
 https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
	Generate code for the specified calling convention. Permissible values are ‘sysv’ for the ABI used on GNU/Linux and other systems, and ‘ms’ for the Microsoft ABI. The default is to use the Microsoft ABI when targeting Microsoft Windows and the SysV ABI on all other systems. You can control this behavior for specific functions by using the function attributes ms_abi and sysv_abi. See https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html#Function-Attributes.Calling conventions see https://en.wikipedia.org/wiki/X86_calling_conventions.

 -ffunction-sections
 -fdata-sections
	Place each function or data item into its own section in the output file if the target supports arbitrary sections. The name of the function or the name of the data item determines the section's name in the output file. Use these options on systems where the linker can perform optimizations to improve locality of reference in the instruction space. Most systems using the ELF object format and SPARC processors running Solaris 2 have linkers with such optimizations. AIX may have these optimizations in the future.