# Compiler
## 0.构建编译环境
[参考](https://decaf-lang.github.io/minidecaf-tutorial/docs/step0/riscv_env.html)  
实验平台：WSL(Ubuntu20.04)  
编码平台：vscode  
汇编指令集：armv7
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

1.顺序语句：赋值语句，函数调用，函数定义的实现  (实现)   
2.控制语句：（实现）  
3.数组：（实现）
## 4.优化
## 5.生成arm汇编语言


虚拟机的使用:
+ .s链接成可执行文件
arm-linux-gnueabihf-gcc test.s libsysy.a -o test
注意此时.s必须放在.a上面
+ qemu执行
qemu-arm -L /usr/arm-linux-gnueabihf/ ./test


to do:
1.函数调用：(实现)
寄存器分配还可以再优化下!
1.1如果是dest去寻找寄存器，发现op里面最后时间与自己定义时间一样，直接占了

先将寄存器压栈，再将其放入
注意：push{r0-r11}是先压栈r11再压栈r0，即最后指向r0

2.数组的实现
2.1在生成ir的时候添加ARRAY_LOAD和ARRAY_STORE
在生成arm的时候以r14和r12为保存寄存器(实现)
2.2全局数组的加载和运算(实现)
2.3数组作为参数的传递
与变量类型的形参传递一致，先不实现部分传递即数组定义a[2][3]，只传递a[1]部分，数组传递sp的偏移位置(实现)


3.if跳转
if (cond) {
    thendo 
}
else {
    elsedo
}
if语句实现分为2个部分：
首先：cond部分需要处理短路求值问题
处理AND和OR的情况，其余的<=,>=等只返回一个值1或者0，表示该逻辑的真假(实现)

4.除法和求余运算
除法：首先将r0和r1数据压栈，再在r0和r1上保存需要的数据，再BL "__aeabi_idiv"，最后从r0读取数据(实现)

求余运算：与除法类似，调用BL __aeabi_idivmod
注意：最后返回的是r1才是余数(实现)

5.修正函数返回为void
调用者正常接收，被调用者RET ir::irOP()空(实现)

6.改进：
6.1将参数调用改为r0-r4直接传参，多的才压栈（实现）
6.2优化寄存器分配，循环遍历，将不会再出现的直接从reg上覆盖掉，当寄存器满了，选择最一后用的一个溢出（最后面用到的寄存器溢出）（实现）
（存在的问题：当前是最后一个的，不一定全局是最后一个，有可能出栈调用出错）
6.3寄存器分配还可以再优化下
