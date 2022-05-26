#pragma once
#include <iostream>
#include <list>
#include <IR/generate/context.hpp>

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
};
//IR,中间语言定义
class IR{
    public:
    //源代码位置
    int line,column;
    //操作码
    irCODE ircode;
    //操作数
    irOP op1,op2,op3,dest;
    //IR格式
    IR(irCODE ircode,irOP dest,irOP op1,irOP op2,irOP op3);
    IR(irCODE ircode,irOP dest,irOP op1,irOP op2);
    IR(irCODE ircode,irOP dest,irOP op1);
    IR(irCODE ircode,irOP dest);
    IR(irCODE ircode);
    //IR输出到终端,使用std::cerr
    void print(std::ostream& out=std::cerr);
    //IR对应源码位置
    void set_filepos();
    //遍历irOP输出
    void print_irOP(irOP& op,std::ostream& out=std::cerr);
    //格式输出
    void print_format(std::string name,std::ostream& out=std::cerr);
};
//全部IR的list,.emplace_back添加IR对象
using IRList=std::list<IR>;
}