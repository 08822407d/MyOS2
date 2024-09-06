2024-09-05:

    Ported musl c-library, now test-purpose initd and shell are static linked to musl.

    Currently support syscalls (only main flags/options):

    file -- open, close, seek,

    dir -- getcwd, chdir, mkdir, rmdir, getdents64

    io -- read, write, writev,

    process -- fork, exec,

    virtmem -- mmap, munmap, brk,


---

2024-06-25:

    AMD64 system call API changed to syscall/sysretq instruction pair.
