#ifndef  _AMD64_ASM_H_
#define  _AMD64_ASM_H_   

    #ifdef __ELF__
    #   define _C_LABEL(x)      x 
    #else
    #   ifdef __STDC__
    #       define _C_LABEL(x)  _ ## x
    #   else
    #       define _C_LABEL(x)  _/**/x
    #   endif
    #endif
    #define	_ASM_LABEL(x)	    x

    #ifdef __STDC__
    #   define __CONCAT(x,y)	x ## y
    #       define __STRING(x)	#x
    #else
    #   define __CONCAT(x,y)	x/**/y
    #   define __STRING(x)	    "x"
    #endif

    /* let kernels and others override entrypoint alignment */
    #if !defined(_ALIGN_TEXT) && !defined(_KERNEL)
    #   ifdef _STANDALONE
    #       define _ALIGN_TEXT .align 1
    #   elif defined __ELF__
    #       define _ALIGN_TEXT .align 16
    #   else
    #       define _ALIGN_TEXT .align 4
    #   endif
    #endif

    #define _ENTRY(x) \
        .text; _ALIGN_TEXT; .globl x; .type x,@function; x:
    #define _LABEL(x) \
        .globl x; x:

    #ifdef GPROF
    #   ifdef __ELF__
    #       define _PROF_PROLOGUE	\
                    pushl %ebp; movl %esp,%ebp; call PIC_PLT(__mcount); popl %ebp
    #   else 
    #       define _PROF_PROLOGUE	\
                    pushl %ebp; movl %esp,%ebp; call PIC_PLT(mcount); popl %ebp
    #   endif
    #else
    #   define _PROF_PROLOGUE
    #endif

    #define	ENTRY(y)	_ENTRY(_C_LABEL(y)); _PROF_PROLOGUE
    #define	NENTRY(y)	_ENTRY(_C_LABEL(y))
    #define	ASENTRY(y)	_ENTRY(_ASM_LABEL(y)); _PROF_PROLOGUE
    #define	LABEL(y)	_LABEL(_C_LABEL(y))
    #define	END(y)		.size y, . - y

#endif /* !_AMD64_ASM_H_*/