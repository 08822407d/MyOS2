#ifndef _LINUX_CONST_H_
#define _LINUX_CONST_H_

	// #define min(x, y) ({							\
	// 			typeof(x) _min1 = (x);				\
	// 			typeof(y) _min2 = (y);				\
	// 			(void) (&_min1 == &_min2);			\
	// 			_min1 < _min2 ? _min1 : _min2;		\
	// 		})

	// #define max(x, y) ({							\
	// 			typeof(x) _min1 = (x);				\
	// 			typeof(y) _min2 = (y);				\
	// 			(void) (&_min1 == &_min2);			\
	// 			_min1 > _min2 ? _min1 : _min2;		\
	// 		})

	/*
	* This returns a constant expression while determining if an argument is
	* a constant expression, most importantly without evaluating the argument.
	* Glory to Martin Uecker <Martin.Uecker@med.uni-goettingen.de>
	*/
	#define __is_constexpr(x) \
		(sizeof(int) == sizeof(*(8 ? ((void *)((long)(x) * 0l)) : (int *)8)))

#endif /* _LINUX_CONST_H_ */