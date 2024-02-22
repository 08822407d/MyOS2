#include <linux/compiler/compiler.h>

#include <asm/current.h>
#include <asm/percpu.h>

struct task_struct;
typedef struct task_struct task_s;


// task_s *get_current(void) {
// 	task_s **curr_pp = this_cpu_ptr(&pcpu_hot.current_task);
// 	return *curr_pp;
// }