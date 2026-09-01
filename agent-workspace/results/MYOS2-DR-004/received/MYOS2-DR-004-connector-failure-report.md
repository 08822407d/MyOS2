未能完成 MYOS2-DR-004 的可信交付。

已确认任务书路径为：

`agent-workspace/tasks/MYOS2-DR-004-debug-instrumentation.md`

但本次 GitHub 连接器将私有仓库的文件内容读取请求错误路由为仓库元数据请求，因而我没有实际取得以下关键内容：

- `master:agent-workspace/conventions.md`
- `master:agent-workspace/repo-map.md`
- `master:agent-workspace/tasks/MYOS2-DR-004-debug-instrumentation.md`
- `time` 分支的实际 HEAD commit 与任务范围内源码

同一连接器的建分支、写文件和创建 PR 动作也未能正确解析。因此，本次没有：

- 创建 `agent/MYOS2-DR-004` 分支；
- 修改仓库中的任何文件；
- 创建 PR；
- 伪造 `base_snapshot`、源码证据或任务书交付物。

由于任务书正文和源码快照均未成功读取，不能负责任地使用“对话中逐文件完整输出”的兜底方式：那会要求猜测交付物清单、格式、分析边界及源码事实，违反用户要求的“严格按任务书执行”和真实记录。
