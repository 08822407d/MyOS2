// open() 内核态入口函数, fs/open.c
static long do_sys_openat2(int dfd, const char __user *filename,
			   struct open_how *how);
//						||
//						\/
// ， fs/namei.c
struct file *do_filp_open(int dfd, struct filename *pathname,
		const struct open_flags *op);
//						||
//						\/
static struct file *path_openat(struct nameidata *nd,
			const struct open_flags *op, unsigned flags);
//						||
//						\/
//	路径解析核心， fs/namei.c
static int link_path_walk(const char *name, struct nameidata *nd);
//						||
//						\/
static const char *walk_component(struct nameidata *nd, int flags);
//						||
//						\/