#ifndef _KERN_UTILS_H_
#define _KERN_UTILS_H_

	#define min(x, y) ({							\
				typeof(x) _min1 = (x);				\
				typeof(y) _min2 = (y);				\
				(void) (&_min1 == &_min2);			\
				_min1 < _min2 ? _min1 : _min2;		\
			})

#endif /* _KERN_UTILS_H_ */