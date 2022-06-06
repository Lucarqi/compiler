#include "ast/node.hpp"
#include "IR/ir.hpp"
#include "parser.hpp"
#include <assert.h>

using namespace sysy::ir;
/*
所有的_eval实现，直接求值，编译器优化部分
*/
namespace sysy{
//Expression
int ast::node::Expression::_eval(ir::Context& ctx)
{
    throw std::runtime_error("cant eval of this");
}
//Number：返回数值
int ast::node::Number::_eval(ir::Context& ctx) {return this->value;}
//BianryExp：返回二元运算的值
int ast::node::BinaryExpr::_eval(ir::Context& ctx)
{
    switch(this->op)
    {
        case PLUS:
            return lh.eval(ctx) + rh.eval(ctx);
            break;
        case MINUS:
            return lh.eval(ctx) - rh.eval(ctx);
            break;
        case MUL:
            return lh.eval(ctx) * rh.eval(ctx);
            break;
        case DIV:
            return lh.eval(ctx) / rh.eval(ctx);
            break;
        case MOD:
            return lh.eval(ctx) % rh.eval(ctx);
            break;
        case EQ:
            return lh.eval(ctx) == rh.eval(ctx);
            break;
        case NQ:
            return lh.eval(ctx) != rh.eval(ctx);
            break;
        case LT:
            return lh.eval(ctx) < rh.eval(ctx);
            break;
        case LEQ:
            return lh.eval(ctx) <= rh.eval(ctx);
            break;
        case GT:
            return lh.eval(ctx) > rh.eval(ctx);
            break;
        case GEQ:
            return lh.eval(ctx) >= rh.eval(ctx);
            break;
        case AND:
            return lh.eval(ctx) && rh.eval(ctx);
            break;
        case OR:
            return lh.eval(ctx) || rh.eval(ctx);
            break;
        default:
            //error,二元运算符号未知，抛出异常
            throw std::runtime_error("Unkonwn OP");
            break;
    }
}
//返回一元运算的值
int ast::node::UnaryExpr::_eval(ir::Context& ctx)
{
    switch(this->op)
    {
        case PLUS:
            return rh.eval(ctx);
            break;
        case MINUS:
            return -rh.eval(ctx);
            break;
        case NOT:
            return !rh.eval(ctx);
            break;
        default:
            //抛出异常，未知操作符号
            throw std::runtime_error("Unkown OP");
            break;
    }
}
//条件表达式的值
int ast::node::ConditionExpr::_eval(ir::Context& ctx)
{   
    return this->value.eval(ctx);
}
int ast::node::ValueExpr::_eval(ir::Context& ctx)
{
    return this->value.eval(ctx);
}
int ast::node::ArrayIdentifier::_eval(ir::Context& ctx){}
int ast::node::AfterInc::_eval(ir::Context& ctx){}
int ast::node::AssignStmt::_eval(ir::Context& ctx){}

/*
所有_eval_run实现，构建IR语句，返回目标寄存器dest
*/
//Expression _eval_run接口
ir::irOP ast::node::Expression::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    //do something
}

//Number节点，运行时直接返回 立即数
ir::irOP ast::node::Number::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    irOP dest = this->value;
    return dest;
}

//返回标识符 寄存器
ir::irOP ast::node::Identifier::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    auto find = ctx.find_symbol(this->name);
    irOP dest = find.name;
    return dest;
}

//二元运算的IR构建，含 虚拟寄存器
ir::irOP ast::node::BinaryExpr::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    ir::irOP dest="%"+std::to_string(ctx.get_id()), lh,rh;
    //立即数或者虚拟寄存器
    lh = this->lh.eval_run(ctx,ir);
    rh = this->rh.eval_run(ctx,ir);
    //首先实现加减乘除
    switch(this->op){
        case PLUS:
            ir.emplace_back(irCODE::ADD,dest,lh,rh);
            break;
        case MINUS:
            ir.emplace_back(irCODE::SUB,dest,lh,rh);
            break;
        case MUL:
            ir.emplace_back(irCODE::MUL,dest,lh,rh);
            break;
        case DIV:
            ir.emplace_back(irCODE::DIV,dest,lh,rh);
            break;
        case MOD:
            ir.emplace_back(irCODE::MOD,dest,lh,rh);
            break;
        default:
            throw std::runtime_error("unKown OP");
            break;
    }
    //返回生成的寄存器
    return dest;
}
//一元表达式生成IR，返回dest寄存器
ir::irOP ast::node::UnaryExpr::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    irOP dest = "%"+std::to_string(ctx.get_id());
    //先实现取正和取负
    switch(this->op){
        case PLUS:
            return this->rh.eval_run(ctx,ir);
            break;
        case MINUS:
            ir.emplace_back(irCODE::SUB,dest,0,rh.eval_run(ctx,ir)); 
            break;
        default:
            throw std::runtime_error("unKown op");
            break;
    }
    return dest;
}
//Stmt语句生成IR接口
ir::irOP ast::node::Stmt::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    this->irGEN(ctx,ir);
    return irOP();
}
//赋值语句，返回当前IR列表最后一个的dest
ir::irOP ast::node::AssignStmt::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    //生成IR
    this->irGEN(ctx,ir);
    //返回dest
    return ir.back().dest;
}
//AfterInc:a++ 翻译为return a ,then: a=a+1
ir::irOP ast::node::AfterInc::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    //寻找标识符a
    auto find = ctx.find_symbol(this->lname.name);
    //处理a=a+1
    auto num = new ast::node::Number(1);
    auto binaryExp = new ast::node::BinaryExpr(this->op,lname,*num);
    auto assign = new ast::node::AssignStmt(lname,*binaryExp);
    assign->line = this->line;
    assign->column = this->column;
    assign->eval_run(ctx,ir);
    delete num;
    delete binaryExp;
    delete assign;
    //return a
    return find.name;
}
//ValueExpr，表达式语句/函数调用
ir::irOP ast::node::ValueExpr::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    return this->value.eval_run(ctx,ir);
}

//函数调用
ir::irOP ast::node::FunctionCall::_eval_run(ir::Context& ctx,ir::IRList& ir){
    //判断调用函数是否存在
    try
    {
        auto find = ctx.find_func(this->name.name);
        //找到
        std::vector<ir::irOP> list;
        //遍历参数列表
        for(int i = 0; i < this->args.args.size() ; ++i)
        {
            list.push_back(this->args.args[i]->eval_run(ctx,ir));
        }
        //生成IR,设置参数
        for(int i = this->args.args.size()-1 ; i>= 0 ; --i)
        {
            ir.emplace_back(irCODE::SET_ARG, i, list[i]);
        }
        //调用CALL
        ir::irOP dest = "%"+std::to_string(ctx.get_id());
        ir.emplace_back(irCODE::CALL, dest, this->name.name);
        return dest;
    }
    catch(error::BaseError& e)
    {
        throw e;
    }
}

ir::irOP ast::node::ArrayIdentifier::_eval_run(ir::Context& ctx,ir::IRList& ir){}
ir::irOP ast::node::ConditionExpr::_eval_run(ir::Context& ctx,ir::IRList& ir){}

}
