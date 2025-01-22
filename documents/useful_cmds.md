代码量统计分析

cloc ./ --exclude-dir=.vscode,build,documents,scripts

---

在zsh上构建edk2的命令
make -C BaseTools/
source edksetup.sh BaseTools
build -a X64 -t GCC5 -p ShellPkg/ShellPkg.dsc

---

x86-64上编译musl库，仅静态库，含debug信息
bash ./configure --prefix=/home/cheyh/projs/musl/build --enable-debug --enable-static --disable-shared --exec-prefix=/home/cheyh/projs/musl/build/exec --syslibdir=/home/cheyh/projs/musl/build/sys --disable-optimize

---

动态链接musl-c标准库时，修改用户程序中储存的动态链接器路径(.interp段的内容)。

patchelf --set-interpreter $MUSL_DYN_LD init

如果开发平台默认c库不打算更换为musl-c库，则显然开发时用于链接的musl-c库并不在默认位置，所以生成的用户程序中设定的动态链接器路径指向非默认的库路径(/usr/lib/x86_64-linux-gnu)。因此将.interp段内容修改为$MUSL_DYN_LD(在其他位置设置为/usr/lib/x86_64-linux-gnu)。
