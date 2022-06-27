#include "ast/node.hpp"
#include "IR/ir.hpp"
#include "parser.hpp"
#include <assert.h>

using namespace sysy::ir;
/*
所有的_eval实现，编译期间直接求值
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
//常量数组直接求值
int ast::node::ArrayIdentifier::_eval(ir::Context& ctx){
    auto v = ctx.find_const(this->name.name);
    if(v.is_array){
        if(v.shape.size()!=this->shape.size()){
            int index = 0, size = 1;
            for (int i = this->shape.size() + 1; i >= 0; i++) {
                index += this->shape[i]->eval(ctx) * size;
                size *= v.shape[i];
            }
            return v.value[index];
        }
        else {
            throw std::runtime_error(this->name.name + "数组维度大小不匹配");
        }
    }
    else {
        throw std::runtime_error(this->name.name+"：不是数组");
    }
}
//常量直接求值
int ast::node::Identifier::_eval(ir::Context& ctx){
    auto v = ctx.find_const(this->name);
    if (v.is_array) {
        throw std::runtime_error(this->name + " is a array.");
    } 
    else {
        return v.value.front();
    }
}//编译期间求值
int ast::node::AfterInc::_eval(ir::Context& ctx){
    if(dynamic_cast<ast::node::ArrayIdentifier*>(&this->lname)!=nullptr){
        throw std::runtime_error("只有局部变量可以a++直接求值");
    }
    auto val = this->lname.eval(ctx);
    auto v = ctx.find_symbol(this->lname.name);
    if(v.name[0]!='%'||v.is_array){
        throw std::runtime_error("只有局部变量可以a++直接求值");
    }
    int value = this->op==PLUS?val + 1:val - 1;
    return value;
}
int ast::node::AssignStmt::_eval(ir::Context& ctx){
    if(dynamic_cast<ast::node::ArrayIdentifier*>(&this->lname)!=nullptr){
        throw std::runtime_error("只有局部变量可以直接求值");
    }
    auto val = this->rexpr.eval(ctx);
    auto v = ctx.find_symbol(this->lname.name);
    if(v.name[0]!='%'||v.is_array){
        throw std::runtime_error("只有局部变量可以直接求值");
    }
    return val;
}

/*
所有_eval_run实现，构建IR语句，返回目标寄存器dest
*/
//Expression _eval_run接口
ir::irOP ast::node::Expression::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    //do something
    throw std::runtime_error("can't resolve this");
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

//二元运算的IR构建，含虚拟寄存器
ir::irOP ast::node::BinaryExpr::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    ir::irOP dest="%"+std::to_string(ctx.get_id()), lh,rh;
    if(this->op!=AND && this->op!=OR)
    {
        //立即数或者虚拟寄存器irOP
        lh = this->lh.eval_run(ctx,ir);
        rh = this->rh.eval_run(ctx,ir);
    }
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
        // 实现条件判断
        case EQ:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVEQ,dest , 1, 0);
            break;
        case NQ:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVNQ,dest , 1, 0);
            break;
        case GT:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVGT,dest , 1, 0);
            break;
        case LT:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVLT,dest , 1, 0);
            break;
        case GEQ:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVGEQ,dest , 1, 0);
            break;
        case LEQ:
            ir.emplace_back(irCODE::CMP,ir::irOP(), lh, rh);
            ir.emplace_back(irCODE::MOVLEQ,dest , 1, 0);
            break;
        /*处理AND
        返回dest临时寄存器
        */
        case AND:
        {
            //定义label
            std::string label = "Cond_" + std::to_string(ctx.get_id()) + "_End";
            //定义cond条件判断list模块
            ir::IRList end;
            end.emplace_back(ir::irCODE::LABEL,ir::irOP(),ir::irOP(),label);
            //判断左值条件
            auto lhs = this->lh.eval_cond_run(ctx,ir);
            //phi_move，假设为假，0
            ir.emplace_back(ir::irCODE::PHI_MOVE, dest, ir::irOP(0));
            ir.back().phi_block = end.begin();
            ir.emplace_back(lhs.elseop, label);            
            //判断右值条件
            auto rhs = this->rh.eval_run(ctx,ir);
            //phi_move,不假设直接得到
            ir.emplace_back(ir::irCODE::PHI_MOVE, dest , rhs);
            ir.back().phi_block = end.begin();
            //合并endlist
            ir.splice(ir.end(),end);
            break;
        }
        //处理OR
        case OR:
        {
            std::string label = "Cond_" + std::to_string(ctx.get_id()) + "_End";
            ir::IRList end;
            end.emplace_back(ir::irCODE::LABEL, ir::irOP(),ir::irOP(),label);
            auto lhs = this->lh.eval_cond_run(ctx,ir);
            //假设上面判断为真
            ir.emplace_back(ir::irCODE::PHI_MOVE, dest ,ir::irOP(1));
            ir.back().phi_block = end.begin();
            ir.emplace_back(lhs.thenop, label);
            auto rhs = this->rh.eval_run(ctx,ir);
            ir.emplace_back(ir::irCODE::PHI_MOVE, dest, rhs);
            ir.back().phi_block = end.begin();
            ir.splice(ir.end(), end);
            break;
        }
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
            return dest;
            break;
        case NOT:
            //与0判断，=0则设置为1，!=0设置为0
            ir.emplace_back(irCODE::CMP,ir::irOP(),0,rh.eval_run(ctx,ir));
            ir.emplace_back(irCODE::MOVEQ,dest,1,0);
            return dest;
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
    
    return find.name;
}
//ValueExpr，表达式语句/函数调用
ir::irOP ast::node::ValueExpr::_eval_run(ir::Context& ctx,ir::IRList& ir)
{
    return this->value.eval_run(ctx,ir);
}

//函数调用
ir::irOP ast::node::FunctionCall::_eval_run(ir::Context& ctx,ir::IRList& ir){
    /*先不使用判断find_func*/
    std::vector<ir::irOP> list;
    //遍历参数列表
    for(int i = 0; i < (int)this->args.args.size() ; ++i)
    {
        list.push_back(this->args.args[i]->eval_run(ctx,ir));
    }
    //生成IR,反向设置参数
    for(int i = this->args.args.size()-1 ; i>= 0 ; --i)
    {
        ir.emplace_back(irCODE::SET_ARG, i, list[i]);
    }
    //调用CALL
    ir::irOP dest = "%"+std::to_string(ctx.get_id());
    ir.emplace_back(irCODE::CALL, dest, this->name.name);
    return dest;
}

//整个逻辑表达式
ir::irOP ast::node::ConditionExpr::_eval_run(ir::Context& ctx,ir::IRList& ir){
    return  this->value.eval_run(ctx,ir);
}
/*
数组作为右值，此时的维度可以是变量
*/
ir::irOP ast::node::ArrayIdentifier::_eval_run(ir::Context& ctx,ir::IRList& ir){
    //std::cerr<<this->name.name<<std::endl;
    auto v = ctx.find_symbol(this->name.name);
    if(v.is_array)
    {
        //作为右值载入，一定存在
        if(this->shape.size() == v.shape.size()){
            /*判断维度大小，这里不好判断数组维度大小
            for(int i=0;i < (int)v.shape.size();i++){
                if(this->shape[i]->eval(ctx) >= v.shape[i]){
                    throw std::runtime_error(this->name.name+"数组大小超出范围");
                }
            }*/
            irOP dest = "%"+std::to_string(ctx.get_id());
            irOP index = "%"+std::to_string(ctx.get_id());
            irOP size = "%"+std::to_string(ctx.get_id());
            ir.emplace_back(ir::irCODE::SAL,index,
                this->shape[this->shape.size()-1]->eval_run(ctx,ir),2);//乘以4
            if(this->shape.size()!=1){
                irOP temp = "%"+std::to_string(ctx.get_id());
                ir.emplace_back(ir::irCODE::MOV,size,4*v.shape[this->shape.size()-1]);
            }
            for(int i = this->shape.size()-2;i>=0;i--){
                irOP tmp = "%"+std::to_string(ctx.get_id());
                irOP tmp2 = "%"+std::to_string(ctx.get_id());
                ir.emplace_back(ir::irCODE::MUL,tmp,size,this->shape[i]->eval_run(ctx,ir));
                ir.emplace_back(ir::irCODE::ADD,tmp2,index,tmp);
                index = tmp2;
                if(i!=0){
                    irOP tmp = "%"+std::to_string(ctx.get_id());
                    ir.emplace_back(ir::irCODE::MUL,tmp,size,v.shape[i]);
                    size = tmp;
                }
            }
            ir.emplace_back(ir::irCODE::LOAD,dest,v.name,index);
            return dest;
        }
        else 
        {
            std::runtime_error("数组维度不匹配");
        }
    }
    else 
    {
        std::runtime_error(this->name.name+":不是数组");
    }
}

/*
控制流实现部分
*/
//逻辑运算判断(一元运算)，处理1/0值，具体的运算在UnaryExpr处
ast::node::Expression::condResult ast::node::Expression::_eval_cond_run(ir::Context& ctx, IRList& ir)
{
    //注意此时then应该是!=0,即判断为真
    Expression::condResult ret;
    ir.emplace_back(irCODE::CMP, ir::irOP(), this->eval_run(ctx,ir), ir::irOP(0));
    ret.thenop = ir::irCODE::JNQ;
    ret.elseop = ir::irCODE::JEQ;
    return ret;
}

/*二元运算逻辑, 处理1/0值，具体运算在BinaryExpr处
注意AND和OR需要单独处理,实现“短路求值”
*/
ast::node::Expression::condResult ast::node::BinaryExpr::_eval_cond_run(ir::Context& ctx,ir::IRList& ir)
{
    Expression::condResult ret;
    ir::irOP lh,rh;
    if(this->op!=AND && this->op!=OR)
    {
        lh = this->lh.eval_run(ctx,ir);
        rh = this->rh.eval_run(ctx,ir);
    }
    switch(this->op)
    {
        case EQ:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JEQ;
            ret.elseop = ir::irCODE::JNQ;
            break;
        case NQ:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JNQ;
            ret.elseop = ir::irCODE::JEQ;
            break;
        case GT:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JGT;
            ret.elseop = ir::irCODE::JLE;
            break;
        case GEQ:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JGE;
            ret.elseop = ir::irCODE::JLT;
            break;
        case LT:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JLT;
            ret.elseop = ir::irCODE::JGE;
            break;
        case LEQ:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), lh,rh);
            ret.thenop = ir::irCODE::JLE;
            ret.elseop = ir::irCODE::JGT;
            break;
        //处理AND和OR,返回1为真，0为假，与0作比较
        default:
            ir.emplace_back(ir::irCODE::CMP, ir::irOP(), this->eval_run(ctx,ir),ir::irOP(0));
            ret.thenop = ir::irCODE::JNQ;
            ret.elseop = ir::irCODE::JEQ;
            break;
    }
    return ret;
}
}
