#ifndef  _AMD64_ASM_H_
#define  _AMD64_ASM_H_   

    #ifdef __ELF__
    #   define _C_LABEL(x)	x 
    #   define _C_LABEL_LABEL(x)  x:
    #else
    #   ifdef __STDC__
    #       define _C_LABEL(x)	_ ## x
    #       define _C_LABEL_LABEL(x)   _ ## x:
    #   else
    #       define _C_LABEL(x)	_/**/x
    #       define _C_LABEL_LABEL(x)   _/**/x:
    #   endif
    #endif
    #define	_ASM_LABEL(x)	x

    #define ENTRY(name)             \
            .global _C_LABEL(name); \
            _C_LABEL_LABEL(name)

#endif /* !_AMD64_ASM_H_*/