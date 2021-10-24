#ifndef _WAIT_H_
#define _WAIT_H_

	int wait4(unsigned long pid, int *status, int options);
	int waitpid(unsigned long pid, int *status, int options);

#endif /* _WAIT_H_ */