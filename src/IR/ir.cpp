#include "IR/ir.hpp"
#include <assert.h>
namespace sysy::ir{
//irOP是个变量
irOP::irOP(std::string name):type(Type::var),name(name){}
//irOP是个立即数
irOP::irOP(int value):type(Type::imm),value(value){}
//irOP为空
irOP::irOP():type(Type::null){}
//irOP是变量吗
bool irOP::is_var(){return this->type == Type::var;}
//irOP是局部变量吗，局部变量是以%开头或者$arg参数
bool irOP::is_local_var(){
    return this->is_var() &&
        (this->name.starts_with("%") || this->name.starts_with("$arg"));
}
//irOP是全局变量吗，全局变量是以@开头
bool irOP::is_global_var(){
    return this->is_var()&&
        this->name.starts_with("@");
}
//irOP是立即数吗
bool irOP::is_imm(){return this->type == Type::imm;}
//irOP是空
bool irOP::is_null(){return this->type == Type::null;}
//重载==判断当前和other irOP是否一样的
bool irOP::operator==(const irOP& other)
{
    if(this->type!=other.type) return false;
    if(this->is_var())return this->name == other.name;
    else if(this->is_imm())return this->value == other.value;
    else {
        // ?
        assert(this->is_null());
        return true;
    }
}

//IR生成,irOP()为空方便IR生成
IR::IR(irCODE ircode,irOP dest,irOP op1,irOP op2,irOP op3)
    :ircode(ircode),dest(dest),op1(op1),op2(op2),op3(op3){}
IR::IR(irCODE ircode,irOP dest,irOP op1,irOP op2)
    :ircode(ircode),dest(dest),op1(op1),op2(op2),op3(irOP()){}
IR::IR(irCODE ircode,irOP dest,irOP op1)
    :ircode(ircode),dest(dest),op1(op1),op2(irOP()),op3(irOP()){}
IR::IR(irCODE ircode,irOP dest)
    :ircode(ircode),dest(dest),op1(irOP()),op2(irOP()),op3(irOP()){}
IR::IR(irCODE ircode)
    :ircode(ircode),dest(irOP()),op1(irOP()),op2(irOP()),op3(irOP()){}
//IR输出
void IR::print(std::ostream& out)
{
    switch(this->ircode){
        case irCODE::DATA_BEGIN:print_format("DATA_BEGIN",out);break;
        case irCODE::DATA_WORD:print_format("DATA_WORD",out);break;
        case irCODE::DATA_END:print_format("DATA_END",out);break;
        case irCODE::FUNCTION_BEGIN:print_format("FUNCTION_BEGIN",out);break;
        case irCODE::FUNCTION_END:print_format("FUNCTION_END",out);break;
        case irCODE::RET:print_format("RET",out);break;
        case irCODE::ADD:print_format("ADD",out);break;
        case irCODE::SUB:print_format("SUB",out);break;
        case irCODE::MUL:print_format("MUL",out);break;
        case irCODE::DIV:print_format("DIV",out);break;
        case irCODE::MOD:print_format("MOD",out);break;
        case irCODE::MOV:print_format("MOV",out);break;
    }
    print_irOP(dest,out);
    print_irOP(op1,out);
    print_irOP(op2,out);
    print_irOP(op3,out);
    out<<std::endl;
}
//设置源码位置
void IR::set_filepos()
{
    
}
//遍历irOP输出
void IR::print_irOP(irOP& op,std::ostream& out)
{
    if(op.is_var()) out<<op.name<<'\t';
    else if(op.is_imm()) out<<op.imm<<'\t';
    else if(op.is_null()) out<<'\t';
}
//格式输出
void IR::print_format(std::string code,std::ostream& out)
{
    out<<code<<std::string(16-code.size(),' ');
}
}