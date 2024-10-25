#ifndef _MM_MISC_CONST_H_
#define _MM_MISC_CONST_H_


	#ifndef AT_VECTOR_SIZE_ARCH
	#  define AT_VECTOR_SIZE_ARCH	0
	#endif

	#define AT_VECTOR_SIZE_BASE		22 /* NEW_AUX_ENT entries in auxiliary table */
	/* number of "#define AT_.*" above, minus {AT_NULL, AT_IGNORE, AT_NOTELF} */

	#define AT_VECTOR_SIZE			(2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))

	#define INIT_PASID	0

#endif /* _MM_MISC_CONST_H_ */
