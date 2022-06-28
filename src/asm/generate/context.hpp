#pragma once
#include <array>
#include <ostream>
#include <unordered_map>
#include <bitset>

#include "ast/node.hpp"
#include "config.hpp"
#include "IR/generate/generate.hpp"
#include "IR/ir.hpp"

namespace sysy::asm_generator
{
//arm生成类
class Context
{
private:
//记录每条语句时间
    int time=0;
public:
ir::IRList* irs;
ir::IRList::iterator function_begin;
std::ostream& out;

Context(ir::IRList* irs,ir::IRList::iterator function_begin,std::ostream& out);
//记录每条ir的时间戳
std::unordered_map<ir::IR*,int> ir_in_time;
//记录每个变量的时间戳timestamp
std::unordered_map<std::string ,int> var_define_time;
//该时间上有多个变量要使用
std::multimap<int , std::string> var_define_time_heap;
//记录每个变量最后使用的时间戳
std::unordered_map<std::string ,int> var_lastused_time;
std::multimap<int, std::string> var_lastused_time_heap;

//当前在寄存器中的变量
std::unordered_map<std::string,int> var_in_reg ;
std::unordered_map<int,std::string> reg_in_var ;

//变量在栈中 : 变量名称：offset
std::unordered_map<std::string,int> var_in_stack ;
//函数调用，在set_arg时，全部压入此数据向
std::unordered_map<std::string ,int> call_in_reg ;
//寄存器可用标志
std::bitset<12> avalibel_reg = 0b000000000000;


//存不存在函数调用
bool has_call = false;
int reg_num=12;
//sp指针偏移
int stack_size=0;


//设置ir_time
void set_ir_time(ir::IR& cur);
//设置var_define_time
void set_var_define_time(ir::IR& cur);
//设置var_lastused_time
void set_var_lastused_time(ir::IR& cur);


int get_reg(ir::irOP op,std::ostream& out);
void set_var_in_reg(std::string name,int i);
void off_var_in_reg(std::string name,int i);
void store_var_stack(std::string name,int reg_id,std::ostream& out);
std::string load_global(ir::irOP op,std::ostream& out);

bool find_reg();
static std::string rename(std::string name);
std::string load_reg(ir::irOP op,std::ostream& out);
void store_global(std::string reg,ir::irOP op,std::ostream& out);
void load_call(ir::irOP op,int args, std::ostream& out);
void call_in(std::ostream& out);

//实现除法和求余运算
void div_mod(ir::IR& ir,std::ostream& out);
//实现CMP指令比较，需要将立即数加载到寄存器中比较
void cmp(ir::IR& ir,std::ostream& out);
//处理moveq、movnq等操作
void movdo(ir::IR& ir, std::ostream& out);
//函数调用结束将寄存器的值还原
void call_out(std::ostream& out);
//数组分配
void array_build(ir::IR& ir, std::ostream& out);
//数据压入数组
void array_store(ir::IR& ir,std::ostream& out);
//数据加载
void array_load(ir::IR& ir,std::ostream& out);
};//class
}//namespace