**Write User Applications for MyOS2**

MyOS2 Kernel supports static linked ELF-64 binaries, that means you can only link your app static, and you need not do magic build operation but just use common compiler and linker command

`gcc -o main main.c -static -llibmycrt.a -llibmylibc.a -IMyOS2/mylib/myglibc/include`

It is obvious that you need to link libraries compatible for MyOS2 Kernel, thus you need compile options `-static -llibmycrt.a -llibmylibc.a -IMyOS2/mylib/myglibc/include`

Or you can have a much simpler way to build by Cmake

---

**ChangeLog**

04-16 2024

    Create this doc
