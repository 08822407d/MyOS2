#include <string.h>

// int strlen(char * s)
// {
//     char * count = s;
//     while(*count++);
//     return count - s;
// }

// int strcmp(char * str1, char * str2)
// {
//    	unsigned char c1, c2;
// 	int res = 0;

// 	do {
// 		c1 = *str1++;
// 		c2 = *str2++;
// 		res = c1 - c2;
// 		if (res)
// 			break;
// 	} while (c1);
// 	return res;
// }

char * strcpy(char * dst, char * src)
{
	__asm__	__volatile__(	"cld					\n\t"
							"1:						\n\t"
							"lodsb					\n\t"
							"stosb					\n\t"
							"testb	%%al,	%%al	\n\t"
							"jne	1b				\n\t"
						:	
						:	"S"(src), "D"(dst)
						:	"ax", "memory"
						);
	return 	dst;
}

/*
		string copy number bytes
*/

char * strncpy(char * dst, char * src, long count)
{
	__asm__	__volatile__(	"cld					\n\t"
							"1:						\n\t"
							"decq	%2				\n\t"
							"js	2f					\n\t"
							"lodsb					\n\t"
							"stosb					\n\t"
							"testb	%%al,	%%al	\n\t"
							"jne	1b				\n\t"
							"rep					\n\t"
							"stosb					\n\t"
							"2:						\n\t"
						:	
						:	"S"(src), "D"(dst), "c"(count)
						:	"ax", "memory"				
						);
	return dst;
}

/*
		string cat Dest + Src
*/

char * strcat(char * dst, char * src)
{
	__asm__	__volatile__(	"cld					\n\t"
							"repne					\n\t"
							"scasb					\n\t"
							"decq	%1				\n\t"
							"1:						\n\t"
							"lodsb					\n\t"
							"stosb					\n\t"
							"testb	%%al,	%%al	\n\t"
							"jne	1b				\n\t"
						:	
						:	"S"(src), "D"(dst), "a"(0), "c"(0xffffffff)
						:	"memory"				
						);
	return dst;
}

/*
		string compare FirstPart and SecondPart
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

int strcmp(char * str1, char * str2)
{
	register int __res;
	__asm__	__volatile__(	"cld					\n\t"
							"1:						\n\t"
							"lodsb					\n\t"
							"scasb					\n\t"
							"jne	2f				\n\t"
							"testb	%%al,	%%al	\n\t"
							"jne	1b				\n\t"
							"xorl	%%eax,	%%eax	\n\t"
							"jmp	3f				\n\t"
							"2:						\n\t"
							"movl	$1,		%%eax	\n\t"
							"jl	3f					\n\t"
							"negl	%%eax			\n\t"
							"3:						\n\t"
						:	"=a"(__res)
						:	"D"(str1), "S"(str2)
						:						
						);
	return __res;
}

/*
		string compare FirstPart and SecondPart with Count Bytes
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

int strncmp(char * str1, char * str2, long count)
{	
	register int __res;
	__asm__	__volatile__(	"cld					\n\t"
							"1:						\n\t"
							"decq	%3				\n\t"
							"js	2f					\n\t"
							"lodsb					\n\t"
							"scasb					\n\t"
							"jne	3f				\n\t"
							"testb	%%al,	%%al	\n\t"
							"jne	1b				\n\t"
							"2:						\n\t"
							"xorl	%%eax,	%%eax	\n\t"
							"jmp	4f				\n\t"
							"3:						\n\t"
							"movl	$1,		%%eax	\n\t"
							"jl	4f					\n\t"
							"negl	%%eax			\n\t"
							"4:						\n\t"
						:	"=a"(__res)
						:	"D"(str1), "S"(str2), "c"(count)
						:	
						);
	return __res;
}

/*

*/

int strlen(char * str)
{
	register int __res;
	__asm__	__volatile__(	"cld			\n\t"
							"repne			\n\t"
							"scasb			\n\t"
							"notl	%0		\n\t"
							"decl	%0		\n\t"
						:	"=c"(__res)
						:	"D"(str), "a"(0), "0"(0xffffffff)
						:	
						);
	return __res;
}