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
        assert(this->is_null());
        return true;
    }
}

//IR生成,irOP()为空方便IR生成
IR::IR(irCODE ircode,irOP dest,irOP op1,irOP op2,irOP op3,std::string label)
    :ircode(ircode),dest(dest),op1(op1),op2(op2),op3(op3),label(label){}
IR::IR(irCODE ircode,irOP dest,irOP op1,irOP op2,std::string label)
    :ircode(ircode),dest(dest),op1(op1),op2(op2),op3(irOP()),label(label){}
IR::IR(irCODE ircode,irOP dest,irOP op1,std::string label)
    :ircode(ircode),dest(dest),op1(op1),op2(irOP()),op3(irOP()),label(label){}
IR::IR(irCODE ircode,irOP dest,std::string label)
    :ircode(ircode),dest(dest),op1(irOP()),op2(irOP()),op3(irOP()),label(label){}
IR::IR(irCODE ircode,std::string label)
    :ircode(ircode),dest(irOP()),op1(irOP()),op2(irOP()),op3(irOP()),label(label){}
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
        case irCODE::SET_ARG:print_format("SET_ARG",out);break;
        case irCODE::CALL:print_format("CALL",out);break;
        case irCODE::CMP:print_format("CMP",out);break;
        case irCODE::JMP:print_format("JMP",out);break;
        case irCODE::MOVEQ:print_format("MOVEQ",out);break;
        case irCODE::MOVNQ:print_format("MOVNQ",out);break;
        case irCODE::MOVGT:print_format("MOVGT",out);break;
        case irCODE::MOVLT:print_format("MOVLT",out);break;
        case irCODE::MOVGEQ:print_format("MOVGEQ",out);break;
        case irCODE::MOVLEQ:print_format("MOVLEQ",out);break;
        case irCODE::JEQ:print_format("JEQ",out);break;
        case irCODE::JNQ:print_format("JNQ",out);break;
        case irCODE::JGT:print_format("JGT",out);break;
        case irCODE::JGE:print_format("JGE",out);break;
        case irCODE::JLT:print_format("JLT",out);break;
        case irCODE::JLE:print_format("JLE",out);break;
        case irCODE::LABEL:print_format("LABEL",out);break;
        case irCODE::PHI_MOVE:print_format("PHI_MOVE",out);break;
        case irCODE::SAL:print_format("SAL",out);break;
        case irCODE::STORE:print_format("STORE",out);break;
        case irCODE::DATA_SPACE:print_format("SPACE",out);break;
        case irCODE::MALLOC_IN_STACK:print_format("MALLOC_IN_STACK",out);break;
        case irCODE::LOAD:print_format("LOAD",out);break;
        case irCODE::NOOP:print_format("NOOP",out);break;
        default:
            std::cerr <<"Unkown irCODE"<<std::endl;
    }
    print_irOP(dest,out);
    print_irOP(op1,out);
    print_irOP(op2,out);
    print_irOP(op3,out);
    out<<this->label;
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
    else if(op.is_imm()) out<<op.value<<'\t';
    else if(op.is_null()) out<<'\t';
    
}
//格式输出
void IR::print_format(std::string code,std::ostream& out)
{
    out<<code<<std::string(16-code.size(),' ');
}

bool IR::some(decltype(&sysy::ir::irOP::is_var) callback,
              bool include_dest) const {
  return this->some([callback](irOP op) { return (op.*callback)(); },
                    include_dest);
}
bool IR::some(std::function<bool(const sysy::ir::irOP&)> callback,
              bool include_dest) const {
  return (include_dest && callback(this->dest)) || callback(this->op1) ||
         callback(this->op2) || callback(this->op3);
}
void IR::forEachOp(std::function<void(const sysy::ir::irOP&)> callback,
                   bool include_dest) const {
  this->some(
      [callback](const sysy::ir::irOP& op) {
        callback(op);
        return false;
      },
      include_dest);  
}

}