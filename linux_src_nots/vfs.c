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
//	大循环里分解跟踪路径的每一段
static int link_path_walk(const char *name, struct nameidata *nd);
//						||
//						\/
static const char *walk_component(struct nameidata *nd, int flags);
//						||
//						\/
// {
	//	在 nd->path 里查询 nd->last 子目录
	//	lookup_fast()核心是在rcu和dcache里查找
	static struct dentry *lookup_fast(struct nameidata *nd,
					struct inode **inode, unsigned *seqp);
	//						||
	//						\/
	//	fs/dcache.c
	struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name);
	//	lookup_fast()查找失败则需要在设备上查找
	static struct dentry *lookup_slow(const struct qstr *name,
				  struct dentry *dir, unsigned int flags)
	//						||
	//						\/
	static struct dentry *__lookup_slow(const struct qstr *name,
				    struct dentry *dir, unsigned int flags)
	//					---------->
	//	进入子目录，设置 nd->path 为子目录
	static const char *step_into(struct nameidata *nd, int flags,
				struct dentry *dentry, struct inode *inode, unsigned seq);
// }