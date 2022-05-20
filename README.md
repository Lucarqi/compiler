# Compiler
## 0.构建编译环境
[参考](https://decaf-lang.github.io/minidecaf-tutorial/docs/step0/riscv_env.html)  
实验平台：WSL(Ubuntu20.04)  
编码平台：vscode  
汇编指令集：RISC-V
方式：在WSL上搭建qemu虚拟机，使用riscv64-unknown-elf-gcc在x64平台上交叉编译出RISC-V可执行程序,且指定为x86格式
## 1.编译器框架
词法分析/语法分析-->抽象语法树-->中间代码IR-->risc-v汇编程序生成
## 2.词法分析器和语法分析
CmakeList.txt参考Cmake官方文档FindFLEX和FindBISON节部分