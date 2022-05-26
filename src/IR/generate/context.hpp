#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

//作用域定义
namespace sysy::ir{
//变量信息
class VarInfo{
public:
    //数组大小
    std::vector<int> shape;
    //是不是数组
    bool is_array;
    //寄存器名称
    std::string name;
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
};
}