# MYOS2-DR-003 · 依赖关系图（构建依赖＋初始化顺序＋功能依赖）

```yaml
task_id: MYOS2-DR-003
mode: 普通对话 Pro（GitHub 连接器）
priority: P0
parallel_safe: true
write_zone: agent-workspace/results/MYOS2-DR-003/
prerequisites: 先读 agent-workspace/conventions.md（尤其 §3 词汇表）与 agent-workspace/repo-map.md
```

## 背景

Owner 需要依赖关系图回答两类问题：（a）"要实现功能 X 还必须先有哪些更基础的部分"——为学习实践排序；（b）未来做 menuconfig 式内核裁剪时哪些部分可选、哪些互锁。本任务与 MYOS2-DR-002 并行执行，不依赖其产出——共享词汇表保证两者产出可事后合并。

## 目标

产出三层依赖数据，统一用词汇表节点 ID：

1. **构建依赖**：CMake 目标/子目录之间、头文件包含的粗粒度依赖（子系统级即可，不必到文件级）。
2. **初始化顺序依赖**：从 mykernel/init/（及 arch 启动路径、myloader 交接）提取实际 init 调用序列，标出"谁必须先于谁"，注明证据 `[VERIFIED]`。这是最硬的依赖证据。
3. **功能依赖**：能力节点间的语义依赖（如 mm.kmalloc → mm.page_alloc → mm.early；fs.fat → fs.vfs → block；sched.forkexec → mm.vm_map）。来自源码调用关系的标 `[VERIFIED]`，来自内核常识的标 `[INFERRED]`。

## 交付物（放入 write_zone）

- `MANIFEST.md`（必交）
- `deps.dot` — Graphviz 总图（可分层：build/init/functional 用不同边样式）
- `deps.yaml` — 机器可读边表：

```yaml
- from: mm.kmalloc          # 依赖方
  to: mm.page_alloc         # 被依赖方
  kind: build | init_order | functional
  hardness: hard | soft     # hard=缺它必不可用；soft=缺它降级可用
  evidence: [路径或 INFERRED]
```

- `init-sequence.md` — 启动到 idle/首个用户进程的实际初始化时序叙述（教学价值极高）
- `trim-analysis.md` — 基于图的初步裁剪分析：哪些子系统是不可裁剪的核（强连通/全依赖），哪些是可选叶（如 drivers.rtc、fs.fat、lock.futex），做 menuconfig 的话开关应设在哪一粒度

## 边界

不实现任何裁剪机制；图求"正确的粗粒度"而非"完备的细粒度"——子系统级全覆盖优先，能力级只做重要节点。

## 验收判据

init_order 类边全部有源码证据；deps.yaml 节点命名合法；trim-analysis 至少识别出核集合与三个以上可选叶并给理由。
