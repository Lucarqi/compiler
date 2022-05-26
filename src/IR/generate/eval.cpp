#include "ast/node.hpp"
#include "parser.hpp"
#include <assert.h>
/*
所有的_eval实现，编译期间求值
*/
namespace sysy{
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
/*
变量标识符
*/

/*
数组标识符
*/
}