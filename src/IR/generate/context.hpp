#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "IR/ir.hpp"
//作用域定义
namespace sysy::ir{
//变量信息
class VarInfo{
public:
    //寄存器名称
    std::string name;
    //是不是数组
    bool is_array;
    //数组大小
    std::vector<int> shape;
    VarInfo(std::string name, bool is_array=false, std::vector<int> shape={});
};
//常量信息
class ConstInfo{
public:
    std::vector<int> shape;
    std::vector<int> value;
    bool is_array;
    ConstInfo(std::vector<int> shape={}, std::vector<int> value={}, bool is_array=false);
    ConstInfo(int value);
};
//函数信息
class FuncInfo{
public:
    //返回类型
    int returntype;
    //参数个数
    int argnum;
    //参数类型:0为单一变量，1为数组
    std::vector<int> type;
    FuncInfo(int returntype, int argnum, std::vector<int> type={});
};
//context类实现
class Context{
public:
    Context();
    //序号
    unsigned id=1;
    unsigned get_id();
    //定义符号表和常量表
    using SymbolTable = std::vector<std::unordered_map<std::string ,VarInfo>>;
    using ConstTable = std::vector<std::unordered_map<std::string , ConstInfo>>;
    SymbolTable symbol_table={{}};
    ConstTable const_table={{}};
    //添加进入符号表
    void insert_symbol(std::string name,VarInfo value);
    //添加进入常量表
    void insert_const(std::string name,ConstInfo value);

    /*
    对符号表和常量表的查找
    true在当前作用域查找
    false在全部作用域查找
    */
    VarInfo& find_symbol(std::string name,bool top=false);
    ConstInfo& find_const(std::string name,bool top=false);

    //创建当前作用域
    void create_scope();
    //退出当前作用域
    void end_scope();

    //判断是否在全局作用域
    bool is_global();

    /*
    保存函数的返回类型，参数个数，参数类型(单一变量/数组)
    */
    using FunctionTable = std::unordered_map<std::string, FuncInfo>;
    FunctionTable function_table={};
    //添加函数信息
    void insert_function(std::string name, FuncInfo func);
    //查找函数信息
    FuncInfo& find_func(std::string name);
};
}