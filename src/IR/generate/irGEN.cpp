/*
IR中间语言生成
作用域的构建
类型检查
重复定义检查
编译期间求值
*/
#include "ast/node.hpp"
#include "IR/ir.hpp"
#include "IR/generate/context.hpp"
#include "IR/generate/generate.hpp"
#include "parser.hpp"
#include "error/error.hpp"

using namespace sysy::ir;
namespace sysy::ast::node{
//BaseNode
void BaseNode::irGEN(ir::Context& ctx,IRList& ir)
{
    //do something
}
//rootNode：遍历所有节点生成IR
void Root::irGEN(ir::Context& ctx,IRList& ir)
{
    for(auto& i:this->body)
    {
        i->generate_ir(ctx,ir);
    }
}
//变量带初始值
void VarDeclWithInitVal::irGEN(ir::Context& ctx,IRList& ir)
{
    try{
        //检查当前作用域定义过吗
        auto& find = ctx.find_symbol(this->name.name,true);
    }catch(...)
    {
        //没定义过，判断是否是全局作用域
        if(ctx.is_global())
        {
            //添加IR语句
            ir.emplace_back(irCODE::DATA_BEGIN,irOP("@"+this->name.name));
            ir.emplace_back(irCODE::DATA_WORD,irOP(this->value.eval(ctx)));
            ir.emplace_back(irCODE::DATA_END);
            //加入符号表
            ctx.insert_symbol(this->name.name,VarInfo("@"+this->name.name));
            if(this->is_const)
            {
                //加入常量表
                ctx.insert_const(this->name.name,ConstInfo(this->value.eval(ctx)));
            }
        }
        else
        {
            //是个局部变量
            std::string local_re = "%"+std::to_string(ctx.get_id());
            ctx.insert_symbol(this->name.name,VarInfo(local_re);
            //MOV赋值
            ir.emplace_back(irCODE::MOV,irOP(local_re),irOP(this->value.eval(ctx)));
        }
    }
    //定义过，抛出重复定义异常
    throw error::RedefineVar{};
}
//变量的声明
void VarDecl::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    try
    {
        auto& find = ctx.find_symbol(this->name.name,true);
    }
    catch(...)
    {
        if(ctx.is_global())
        {   //全局变量初始化为0
            ir.emplace_back(irCODE::DATA_BEGIN,irOP("@"+this->name.name));
            ir.emplace_back(irCODE::DATA_WORD,irOP(0));
            ir.emplace_back(irCODE::DATA_END);
            ctx.insert_symbol(this->name.name,VarInfo("@"+this->name.name));
        }
        else 
        {
            ctx.insert_symbol(this->name.name,VarInfo("%"+std::to_string(ctx.get_id())));
        }
    }
    throw error::RedefineVar{};
}
/*
函数的定义
需要创建作用域
*/
void FuncDefine::irGEN(ir::Context& ctx,IRList& ir)
{
    //创建作用域
    ctx.create_scope();
    int arg_len = this->args.list.size();
    ir.emplace_back(irCODE::FUNCTION_BEGIN,irOP(),arg_len,this->name.name);
    //处理参数,假设都为单变量，没有处理数组
    for(int i=0;i<arg_len;i++)
    {
        std::string arg_re = "%"+std::to_string(ctx.get_id());
        ir.emplace_back(irCODE::MOV,irOP(arg_re),irOP("$arg"+std::to_string(i)));
        ctx.insert_symbol(args.list[i]->name.name,VarInfo(arg_re)); 
    }
    //处理block
    this->body.generate_ir(ir::Context& ctx,ir::IRList& ir);
    //处理return
    if(this->return_type == INT)
    {
        ir.emplace_back(irCODE::RET,irOP(),0)
    }
    else 
    {
        ir.emplace_back(irCODE::RET);
    }
    ir.emplace_back(irCODE::FUNCTION_END,this->name.name);
    //退出作用域
    ctx.end_scope();
}
//block
void Block::irGEN(ir::Context& ctx,IRList& ir)
{
    //注意此时函数和Block都定义了作用域，即参数在函数定义域不在block中，有可能出现 bug
    ctx.create_scope();
    for(auto i:this->Stmts)
    {
        i->generate_ir(ctx,ir);
    }
    ctx.end_scope();
}
/*
赋值语句
*/

}