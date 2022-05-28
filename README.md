# Compiler
## 0.构建编译环境
[参考](https://decaf-lang.github.io/minidecaf-tutorial/docs/step0/riscv_env.html)  
实验平台：WSL(Ubuntu20.04)  
编码平台：vscode  
汇编指令集：RISC-V
方式：在WSL上搭建qemu虚拟机，使用riscv64-unknown-elf-gcc在x64平台上交叉编译出RISC-V可执行程序,且指定为x86格式
## 1.编译器框架
词法分析/语法分析-->抽象语法树-->中间代码IR-->riscv汇编程序生成
## 2.词法分析器和语法分析
CmakeList.txt:  参考Cmake官方文档FindFLEX和FindBISON节部分  
lexer.l: 词法分析
parser.y: 语法分析
/ast:包括ast节点的定义，生成ast抽象语法树   
-print_ast:在终端生成AST
## 3.中间语言生成
0.构建作用域，符号表，IR对象类型  
IR：形式上与指令类似  
irCODE：IR操作码  
irOP：IR操作对象，局部变量---%开头，参数---$arg开头，全局变量---@开头  
符号表：分为symbol和const表  
作用域：是符号表以vector形式组织  
IR/generate/context*: 作用域定义  
IR/ir*: IR中间语言定义  

1.顺序语句：赋值语句，函数调用，函数定义的实现  (未验证)   
2.控制语句：（未实现）  
3.数组：（未实现）
## 4.优化
## 5.生成riscv汇编语言