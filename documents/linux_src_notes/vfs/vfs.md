// open() 内核态入口函数, fs/open.c
static long do_sys_openat2(int dfd, const char __user *filename, struct open_how *how);
//	{
	// ， fs/namei.c
	file_s *do_filp_open(int dfd, struct filename *pathname, const struct open_flags *op);
		//	{
		static file_s *path_openat(struct nameidata *nd, const struct open_flags *op, unsigned flags);
		//	{
			//	路径解析核心， fs/namei.c
			//	大循环里分解跟踪路径的每一段
			static int link_path_walk(const char *name, struct nameidata *nd);
			//	{
				//	在 nd->path 里查询 nd->last 子目录
				static const char *walk_component(struct nameidata *nd, int flags);
				//	{
					//	在rcu和dcache里查找目标子目录
					//	DO
						static struct dentry *lookup_fast(struct nameidata *nd, inode_s **inode, unsigned *seqp);
						//	{
							//	fs/dcache.c
							struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name);
						//	}
					//	IF FAIL, DO
						//	在设备上查找目标子目录
						static struct dentry *lookup_slow(const struct qstr *name, struct dentry *dir, unsigned int flags)
						//	{
							static struct dentry *__lookup_slow(const struct qstr *name, struct dentry *dir, unsigned int flags)
						//	}
					// END IF
					//	进入子目录，设置 nd->path 为子目录
					static const char *step_into(struct nameidata *nd, int flags, struct dentry *dentry, inode_s *inode, unsigned seq);
				//	}
			//	}
		//	}
	//	}
//	}