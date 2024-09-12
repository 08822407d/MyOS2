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
