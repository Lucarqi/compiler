#include "IR/generate/context.hpp"
#include <exception>

namespace sysy::ir{
VarInfo::VarInfo(std::string name, bool is_array, std::vector<int> shape)
    :name(name),is_array(is_array),shape(shape){}

ConstInfo::ConstInfo(std::vector<int> shape, std::vector<int> value, bool is_array)
    :shape(shape),value(value),is_array(is_array){}
ConstInfo::ConstInfo(int value)
    :shape({}),value({value}),is_array(false){}

Context::Context(){}
//返回序号
unsigned Context::get_id(){return ++id;}
//添加进入当前符号表，输入name:info对
void Context::insert_symbol(std::string name,VarInfo value){
    symbol_table.back().insert({name,value});
}
void Context::insert_const(std::string name,ConstInfo value){
    const_table.back().insert({name,value});
}
//根据name查找符号表，返回info引用
VarInfo& Context::find_symbol(std::string name,bool top)
{
    for(int i=symbol_table.size()-1;i>=0;i--)
    {
        auto find = symbol_table[i].find(name);
        if(top)
        {
            if(find!=symbol_table[i].end()) return find->second;
            //没找到直接从当前作用域退出
            else break;
        }
        else
        {
            if(find!=symbol_table[i].end()) return find->second;
        }
    }
    //没找到
    throw std::out_of_range("No such const:"+name);
}
//根据标识符，查找ConstInfo
ConstInfo& Context::find_const(std::string name,bool top)
{
    for(int i=const_table.size()-1;i>=0;i--)
    {
        auto find = const_table[i].find(name);
        if(top)
        {
            if(find!=const_table[i].end()) return find->second;  
            else break;
        }
        else 
        {
            if(find!=const_table[i].end()) return find->second;
        }
    }
    //没找到
    throw std::out_of_range("No such const:"+name);
}
//创建当前作用域
void Context::create_scope(){
    symbol_table.push_back({});
    const_table.push_back({});
}
//退出当前作用域
void Context::end_scope()
{
    symbol_table.pop_back();
    const_table.pop_back();
}
//当只剩下一个map时是全局的 
bool Context::is_global(){
    return symbol_table.size()==1 && const_table.size()==1;
}
}