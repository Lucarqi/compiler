#pragma once
#include <iostream>
#include <list>
#include <functional>
#include <string>
#include "IR/generate/context.hpp"
namespace sysy::ir{
//IRop,中间语言操作对象
class irOP
{
    public:
    enum Type
    {
        var,//变量
        imm,//立即数
        null,//null
    };
    //当前操作对象类型
    Type type;
    //操作对象的名称(与VarInfo关联)
    std::string name;
    //立即数的值
    int value;
    irOP();
    irOP(std::string name);
    irOP(int value);
    bool is_var();
    bool is_local_var();
    bool is_global_var();
    bool is_imm();
    bool is_null();
    bool operator==(const irOP& other);
};
//irCODE,中间语言操作码
enum class irCODE{
    FUNCTION_BEGIN,     //函数开始
    FUNCTION_END,       //函数结束
    RET,                //return/return op1 返回
    MOV,                //dest = op1        赋值
    ADD,                //dest = op1 + op2  加法
    SUB,                //dest = op1 - op2  减法
    MUL,                //dest = op1 * op2  除法
    DIV,                //dest = op1 / op2  乘法
    MOD,                //dest = op1 % op2  取模
    DATA_BEGIN,         //global name
    DATA_WORD,          //global value
    DATA_END,           //global end
    SET_ARG,            //设置函数传递参数
    CALL,               //调用函数
    CMP,                //比较  op1 和 op2
    JMP,                //跳转到    label
    MOVEQ,              //dest=op1 if eq or =op2
    MOVNQ,              //dest=op1 if neq or = op2
    MOVGT,              //dest=op1 if gt or = op2
    MOVLT,              //
    MOVGEQ,             //
    MOVLEQ,             //
    JEQ,                //相等
    JNQ,                //不相等
    JGT,                //大于
    JGE,                //大于等于
    JLT,                //小于
    JLE,                //小于等于
    LABEL,              //label
    PHI_MOVE,           //判断赋值
    SAL,                //左移
    STORE,              //从寄存器保存到内存
    LOAD,               //从内存加载到寄存器
    DATA_SPACE,         //全局变量开辟空间占位子
    MALLOC_IN_STACK,    //为临时数组分配空间 name size
    NOOP,               //假读延长生命周期
};
//IR,中间语言定义
class IR{
    public:
    //源代码位置
    int line,column;
    //操作码
    irCODE ircode;
    //操作数
    irOP dest,op1,op2,op3;
    std::string label;
    //IR格式
    IR(irCODE ircode,irOP dest,irOP op1,irOP op2,irOP op3,
        std::string label = "");
    IR(irCODE ircode,irOP dest,irOP op1,irOP op2,
        std::string label = "");
    IR(irCODE ircode,irOP dest,irOP op1,
        std::string label = "");
    IR(irCODE ircode,irOP dest,
        std::string label = "");
    IR(irCODE ircode,
        std::string label = "");
    //IR输出到终端,使用std::cerr
    void print(std::ostream& out=std::cerr);
    //IR对应源码位置
    void set_filepos();
    //遍历irOP输出
    void print_irOP(irOP& op,std::ostream& out=std::cerr);
    //格式输出
    void print_format(std::string name,std::ostream& out=std::cerr);
    //定义迭代器
    std::list<IR>::iterator phi_block;
    //遍历ir中irOP
    bool some(decltype(&sysy::ir::irOP::is_var) callback,
            bool include_dest = true) const;
    bool some(std::function<bool(const sysy::ir::irOP&)> callback,
                bool include_dest = true) const;
    void forEachOp(std::function<void(const sysy::ir::irOP&)> callback,
                    bool include_dest = true) const;
};
//全部IR的list,.emplace_back添加IR对象
using IRList=std::list<IR>;
}