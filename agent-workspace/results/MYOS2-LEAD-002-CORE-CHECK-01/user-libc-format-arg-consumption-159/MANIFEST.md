---
task_id: "MYOS2-LEAD-002-CORE-CHECK-01"
track_id: "MYOS2-LEAD-002"
followup_id: "CORE-USER-LIBC-FORMAT-ARG-CONSUMPTION-159"
executor_identity: "MYOS2-LEAD-002-CHAT-EXEC-27"
produced_by: "GPT-6.2（会话自报；实际Sol后台与Extra/high推理档UNATTESTED）"
model_per_owner: "GPT-6.2 Sol＋Extra／高推理，普通执行无需Pro"
date: "2026-11-27"
date_timezone: "Asia/Shanghai"
base_snapshot: "time（分支名）"
read_channel: "connector"
status: "READY_FOR_MAINLINE_REVIEW"
actual_G04: "PENDING_POST_PUBLICATION"
mainline_acceptance: "PENDING"
D00: []
full_local_P9: "NOT_RUN"
source_pin_short12: "a039d9803ade"
lead_pin_short12: "a8e6ab70852e"
execution_parent_short12: "bf0a7ea26dcc"
inputs_read: ["myinitramfs/mylib/myglibc/stdio/vsnprintf.c","myinitramfs/mylib/myglibc/stdio/printf.c","myinitramfs/mylib/myglibc/string/strlen.c","myinitramfs/myshell/myshell.c","myinitramfs/mylib/myglibc/string/string.h","myinitramfs/mylib/myglibc/include/stdio.h","myinitramfs/mylib/myglibc/libio/stdio.h","myinitramfs/mylib/myglibc/bits/types/FILE.h","myinitramfs/mylib/myglibc/bits/types/struct_FILE.h","myinitramfs/mylib/myglibc/posix/unistd.h","myinitramfs/mylib/myglibc/posix/bits/types.h","myinitramfs/mylib/myglibc/bits/typesizes.h","myinitramfs/mylib/myglibc/sysdeps/unix/sysv_linux/x86/bits/wordsize.h","myinitramfs/mylib/myglibc/posix/sys/types.h","myinitramfs/CMakeLists.txt","myinitramfs/myshell/CMakeLists.txt","myinitramfs/mylib/CMakeLists.txt","myinitramfs/mylib/myglibc/CMakeLists.txt","myinitramfs/mylib/myglibc/stdio/CMakeLists.txt","myinitramfs/mylib/myglibc/string/CMakeLists.txt","mykernel/scripts/options_flags.cmake","myinitramfs/mylib/myglibc/include/string.h","myinitramfs/mylib/myglibc/include/unistd.h","myinitramfs/mylib/myglibc/include/bits/types.h","myinitramfs/mylib/myglibc/sys/types.h","mykernel/debug/panic.c","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-compare-123/libc-string-compare.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/libc-string-length-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-length-copy-124/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-string-copy-append-125/libc-string-copy-append.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-memory-copy-fill-126/libc-memory-copy-fill.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-aligned-word-copy-127/libc-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-dest-aligned-word-copy-128/libc-dest-aligned-word-copy.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/shell-datetime-format.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-datetime-format-129/deferred-findings.yaml","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-shell-error-diagnostics-130/shell-error-diagnostics.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/libc-format-capacity.md","agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-capacity-158/deferred-findings.yaml","agent-workspace/lead/MYOS2-LEAD-002/180-user-shell-datetime-format-contract.md","agent-workspace/lead/MYOS2-LEAD-002/181-user-shell-error-diagnostics-contract.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-DATETIME-FORMAT-129-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-D00-127-129-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-SHELL-ERROR-DIAGNOSTICS-130-review.md","agent-workspace/lead/MYOS2-LEAD-002/reviews/CORE-USER-LIBC-FORMAT-CAPACITY-158-review.md","agent-workspace/lead/MYOS2-LEAD-002/222-user-libc-format-arg-consumption-contract.md","agent-workspace/lead/MYOS2-LEAD-002/checkpoints/2026-11-27-format158-reviewed-args159.md","agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","agent-workspace/README.md","agent-workspace/conventions.md","agent-workspace/tasks/00-gpt-task-protocol-v2.md","agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md","agent-workspace/lead/MYOS2-LEAD-002/194-owner-continuity-and-copyable-return.md","agent-workspace/lead/MYOS2-LEAD-002/198-repository-first-compact-relay.md","agent-workspace/lead/MYOS2-LEAD-002/210-mainline-c11-recovery-and-context-thresholds.md"]
open_questions: ["actual sh linking/actual caller and effective ABI/compiler headers or preprocessing","real filesystem or actual input producer, memory, stdout and write/terminal effects","backend Sol/Extra/high effort UNATTESTED; no API or project program run","full local P9, preprocessing, compile/link, project scripts, C/ASM, Graphviz, QEMU/hardware/real IO NOT_RUN"]
startup_selfcheck_quote: "2. **唯一可写区**：`agent-workspace/results/<你的任务号>/`。只许**新增**文件；不改内核源码、不改 tasks/ 任务书、不碰其他任务号的目录、不改本公约。"
branch_canary_quotes: ["\t-m64 -mcmodel=kernel -fno-pie -fno-pic \\","\tthis_cpu = smp_processor_id();"]
self_check:
  verified_claims: 85
  quotes_reconfirmed: 85
  downgraded_to_inferred: 0
per_file_formal_counts:
  MANIFEST.md: 0
  libc-format-arg-consumption.md: 0
  facts-and-boundaries.yaml: 0
  libc-format-arg-consumption-paths.dot: 0
  deferred-findings.yaml: 0
  evidence.md: 85
execution_handoff_receive:
  continued_identity: "MYOS2-A-C11c / EXEC-27"
  previous_package_id: "MYOS2-A-C11b-TO-C11c-158-20261127"
  previous_package_role: "non_execution_source_transfer_artifact; same-seat only"
  formal_release_comment: 7212542733
  entry_date: "2026-11-27"
  entry_precise_time: "UNKNOWN"
  task: "MYOS2-LEAD-002-CORE-CHECK-01"
  phase: 2
  branch: "claude/dazzling-cori-q0dnyt"
  pr: 17
  allowed_dir: "agent-workspace/results/MYOS2-LEAD-002-CORE-CHECK-01/user-libc-format-arg-consumption-159/"
  allowed_files: ["MANIFEST.md","libc-format-arg-consumption.md","facts-and-boundaries.yaml","libc-format-arg-consumption-paths.dot","deferred-findings.yaml","evidence.md"]
  authorization: "Owner current instruction and fixed 222; earlier 00/28 continuity"
  previous_158_state: "PASS_PENDING_LOCAL_WITH_MAINLINE_ADDENDUM; R158-01 closed by mainline; no D00"
  current_suitability: "qualitative sufficient for scoped static model; no quantitative guarantee"
  next_action: "G00 then 159 ten rows, six append-only files, one commit, unique author comment, actual G04, STOP_WRITING; no160"
  backend: "GPT-6.2 self-identified, actual Sol/Extra/high UNATTESTED"
  no_new_seat: true
  no_previous_C11b_reads_reused: true
  ui_rename: "UNATTESTED"
receiver_guidance_load:
  profile: {"project_guidance":"required","meta_agent_guidance":"required","mnemosyne_guidance":"no","mnemosyne_receive_format":"task_local_only","mnemosyne_naming":"required"}
  status: {"refresh_completed":true,"task_local_only":true,"current_task_preserved":true}
  project_rules: [{"path":"agent-workspace/lead/MYOS2-LEAD-002/00-work-order.md","ref":"a8e6ab70852e","blob_short12":"c357eeebcfe5","LF":411,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/README.md","ref":"a8e6ab70852e","blob_short12":"1deef3891013","LF":52,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/conventions.md","ref":"a8e6ab70852e","blob_short12":"ca21465d1319","LF":63,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/tasks/00-gpt-task-protocol-v2.md","ref":"a8e6ab70852e","blob_short12":"ebb0e304cf48","LF":117,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/18-deferred-findings-and-resume.md","ref":"a8e6ab70852e","blob_short12":"4d22d565ab9e","LF":108,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/24-chatgpt-specialist-conversation-separation.md","ref":"a8e6ab70852e","blob_short12":"a0156f00594d","LF":41,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/28-review-and-next-batch-same-turn.md","ref":"a8e6ab70852e","blob_short12":"9a8b0d039cef","LF":45,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/63-github-write-retry-and-readback.md","ref":"a8e6ab70852e","blob_short12":"2dd70d822b3b","LF":79,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/79-work-package-sizing-and-pro-review.md","ref":"a8e6ab70852e","blob_short12":"3b92828411e5","LF":68,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/110-owner-communication-and-delivery-continuity.md","ref":"a8e6ab70852e","blob_short12":"ae8f04daa248","LF":58,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/124-model-routing-and-pro-message-budget.md","ref":"a8e6ab70852e","blob_short12":"c84b0e04c524","LF":65,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/193-mainline-context-thresholds-and-model-availability.md","ref":"a8e6ab70852e","blob_short12":"a66d24912f6f","LF":68,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/194-owner-continuity-and-copyable-return.md","ref":"a8e6ab70852e","blob_short12":"9ad1f8667620","LF":140,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/198-repository-first-compact-relay.md","ref":"a8e6ab70852e","blob_short12":"5f2b12f75534","LF":59,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"},{"path":"agent-workspace/lead/MYOS2-LEAD-002/210-mainline-c11-recovery-and-context-thresholds.md","ref":"a8e6ab70852e","blob_short12":"56dddd4d796d","LF":67,"read":"158 same-seat actual full read, current tree blob identity fresh matched; 79 reuse"}]
  meta_agent: {"master":"b234c6c91a2e","construction":"06b783861368","entry":"881fd8c1076e","effective":"v1.4-r10.266","fixed_ref":"119597a75517","adoption48":"03c90e03c950","text47":"e3c7e284a6c4","common45":"3492039a99ba","model_registry":"0924de22a54d","checkpoint":"54ac6e5fc66c","prior_full_read":"158 same-seat full text47, 48 and decoded common 01–14/16 individually read; model registry/entry read; checkpoint relevant ranges 1–190,1425–1445,1996–2032. Current direct refs and entry/model/checked checkpoint blob fresh equal; current checkpoint head/tail reread, §322 pending not adoption; no new applicable text or foreign work imported"}
  mnemosyne: {"master":"e581eb9df6c4","readme":"04e450fc22f7","receive":"173f2ea983fd","approved_spec":"058bdbbe3ccb","naming":"2d930f07381e","naming_range":"445–584","prior_read":"same seat 158 actual README/receive/spec and naming §7, current tree blob identity all matched"}
  application: "actual scope in current 222 and Owner instruction; 158 §4 relation type and §5 current explicit field consumed; 18 deferred,24/28 roles,63 bounded retry,79 same-seat reuse,110 continuous,124 model routing,198 compact relay,210 mainline 8/10 reminder; no conflicts/changed required text; no new write rights; no UI assertion"
fresh_G00:
  status: "PASS"
  refs: {"lead":"a8e6ab70852e","exec":"bf0a7ea26dcc","time":"a039d9803ade","direct_master":"e8e60cd19540"}
  pr16: "OPEN_READY"
  pr17: "OPEN_DRAFT"
  open_pages: [[17,16],[]]
  comments: {"pages":[100,100,100,13,0],"unique":313,"positive_markers":75,"negative_absent":19,"direct_double_ids":[7212542733,7208914225,7208211734],"prior_311_fingerprint":"8751f1ad7d76d332","baseline_313_fingerprint":"900a961fc8b1f6da","fingerprint_method":"FNV1a64 UTF16 code units over JSON ordered [id,body,created_at,updated_at]"}
  prior_158: {"three_channel_equal":6,"bytes":321481,"LF":12156,"CRLF":258,"one_parent":"abbf15de6fbb","six_additions":true,"old_non_tree":2271}
  exec_tree: {"entries":2674,"non_tree":2277,"tree_short12":"4437f1595a8c"}
  lead_tree: {"entries":2080,"non_tree":1845,"tree_short12":"72e26b4ef940","previous_1842_preserved":true}
  directory_unoccupied: true
  all_author_159_absent: true
  canary_hit_time_miss_master: true
  conventions_match: true
coverage:
  scenarios: 3
  rows: 10
  normal_rows: 8
  stop_rows: ["A159_04 before strlen dereference","B159_03 before signed int negation"]
  requests: [3,2,5]
  model_checks: 716
  graph_edges: 476
  formal_windows: 85
  ordinary_locations: 23
given: ["ASCII, 8-bit byte, 32-bit int/unsigned int, 64-bit long/unsigned long/size_t/ssize_t/pointer; little-endian; int domain [-2147483648,2147483647]; pointer-to-ulong preserves given low bits","fmt valid NUL-terminated array; source/destination distinct, alive, no concurrency; A/B D initialized to 0x7e; C uninitialized local buf has unspecified untouched bytes","valid va_list and exact provided types; C va_start/va_end normally complete; no real ABI proof","normal aligned source word accesses within object with given word access semantics; not portable effective-type guarantee","same-object or one-past formation only; never dereference one-past; stop before first unproved operation or unrepresentable signed operation","C selected in-repo printf/vsnprintf; valid stdout FILE._fileno=7; opaque write normally returns ssize_t 0, ignored, no actual bytes written/read/output given"]
history: "D00=[]; R158-01 closed via 158 review §4, old graph 4 failures retained; R155 and R152 prior closures; R129 closure retained. 159 mainline cumulative designs 8; 8 reminder reached, 10 not reached; C10 historical 11. Mainline READY 195/158 and 21 binding checks not counted as executor results."
P9_reachable:
  1: "no 40-hex literal"
  2: "formal windows 85 re-read bytes and owner/symbol"
  3: "fixed input paths resolved; historic selectors checked"
  4: "physical ranges verified"
  5: "85=85+0; per-file literal count"
  6: "no external source claims or internal retrieval handles"
  7: "time canary hits/master misses"
  8: "exact convention/read_channel"
  9: "NA no code"
  10: "NA no diff"
  11: "no unsupported build/run assertions"
  12: "NA no degraded delivery"
  13: "222 §7 anchors covered"
  full_local: "NOT_RUN"
preparation_incidents: "发表前校正5个strlen长度3返回事件的邻窗锚为FA159-E052；两个停止行明确slen未赋值及仅A159_04挂起strlen；不改事件顺序/结果/图；未更新执行ref。"
files: ["MANIFEST.md","libc-format-arg-consumption.md","facts-and-boundaries.yaml","libc-format-arg-consumption-paths.dot","deferred-findings.yaml","evidence.md"]
---

# 第159批成果清单

本席持续执行，刷新按79有效复用自身158已读未变版本；本次同名profile及status显式登记，不改158。三场景十行，六件追加，唯一作者回执和实际G04尚须发表后复核；主线独审PENDING。
