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
    }catch(error::BaseError& e)
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
            ctx.insert_symbol(this->name.name,VarInfo(local_re));
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
    catch(error::BaseError& e)
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
    this->body.generate_ir(ctx,ir);
    //处理return
    if(this->return_type == INT)
    {
        ir.emplace_back(irCODE::RET,irOP(),0);
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

//ReturnStmt
void ReturnStmt::irGEN(ir::Context& ctx,IRList& ir)
{
    if(this->expr!=NULL)
        ir.emplace_back(ir::irCODE::RET,irOP(),this->expr->eval_run(ctx,ir));
    else 
        ir.emplace_back(ir::irCODE::RET);
}

//DeclareStmt：多个赋值语句
void DeclareStmt::irGEN(ir::Context& ctx,IRList& ir)
{
    for(auto i:this->list)
        i->generate_ir(ctx,ir);
}

//voidStmt：空语句
void VoidStmt::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    //空过
}


/*
赋值语句，需要判断lname定义
*/
void AssignStmt::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    //UndefindError 交给generate_ir处理
    auto& lh = ctx.find_symbol(this->lname.name);
    auto rh = this->rexpr.eval_run(ctx,ir);
    //
    if(rh.is_var() && (rh.name[0] == '%') && 
        (lh.name[0]=='%'||lh.name.substr(0,4)=="$arg"))
    {
        //直接改变寄存器，优化
        lh.name = rh.name;
    }
    else if(lh.name[0] == '@')
    {
        ir.emplace_back(ir::irCODE::MOV,lh.name,rh);
    }
    else 
    {
        lh.name = "%"+std::to_string(ctx.get_id());
        ir.emplace_back(ir::irCODE::MOV,lh.name,rh);
    }
}

//AfterInc语句状态下的生成IR
void AfterInc::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    auto num = new ast::node::Number(1);
    auto binaryExp = new ast::node::BinaryExpr(this->op,lname,*num);
    auto assign = new ast::node::AssignStmt(lname,*binaryExp);
    assign->line = this->line;
    assign->column = this->column;
    assign->eval_run(ctx,ir);
    delete num;
    delete binaryExp;
    delete assign;
}
void ArrayDecl::irGEN(ir::Context& ctx,ir::IRList& ir){}
void ArrayDeclWithInit::irGEN(ir::Context& ctx,ir::IRList& ir){}
void ValueExpr::irGEN(ir::Context& ctx,ir::IRList& ir){}
void ContinueStmt::irGEN(ir::Context& ctx,ir::IRList& ir){}
void BreakStmt::irGEN(ir::Context& ctx,ir::IRList& ir){}
void WhileStmt::irGEN(ir::Context& ctx,ir::IRList& ir){}
void IfStmt::irGEN(ir::Context& ctx,ir::IRList& ir){}
}