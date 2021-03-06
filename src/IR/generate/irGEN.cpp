/*
IR中间语言生成
作用域的构建
类型检查
重复定义检查
编译期间求值
*/
#include <assert.h>
#include <unordered_set>
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
        //检查当前作用域定义过吗，检查重复定义
        auto& find = ctx.find_symbol(this->name.name,true);
        std::cerr<<find.name<<std::endl;
    }
    catch(error::BaseError& e)
    {
        //没定义过，判断是否是全局作用域
        if(ctx.is_global())
        {
            //添加IR语句
            ir.emplace_back(irCODE::DATA_BEGIN,"@"+this->name.name);
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
            //全新的局部变量，插入符号表(标识符，VarInfo信息)，直接定义虚拟寄存器
            ctx.insert_symbol(this->name.name,VarInfo("%"+std::to_string(ctx.get_id())));
            //使用Assignment语句创建，此时一定能够在符号表中查得到
            AssignStmt assign(this->name,this->value);
            assign.line = this->line;
            assign.column = this->column;
            assign.generate_ir(ctx,ir);
            if(this->is_const)
            {
                ctx.insert_const(this->name.name,ConstInfo(this->value.eval(ctx)));
            }
        }
        return;
    }
    //定义过，抛出重复定义异常
    throw error::RedefineVar();
}

//变量的声明，加入重复声明判断
void VarDecl::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    try
    {
        auto& find = ctx.find_symbol(this->name.name,true);
    }
    catch(error::BaseError& e)
    {
        //std::cerr<<"error"<<std::endl;
        if(ctx.is_global())
        {   //全局变量初始化为0
            ir.emplace_back(irCODE::DATA_BEGIN,"@"+this->name.name);
            ir.emplace_back(irCODE::DATA_WORD,irOP(0));
            ir.emplace_back(irCODE::DATA_END);
            ctx.insert_symbol(this->name.name,VarInfo("@"+this->name.name));
        }
        else 
        {
            //定义未初始化局部变量，插入符号表，但是不使用Assignment进行赋值
            ctx.insert_symbol(this->name.name,VarInfo("%"+std::to_string(ctx.get_id())));
        }
        return;
    }
    throw error::RedefineVar();
}

/*
函数的定义
需要创建作用域
*/
void FuncDefine::irGEN(ir::Context& ctx,IRList& ir)
{
    std::vector<int> list;
    //创建作用域
    ctx.create_scope();
    int arg_len = this->args.list.size();
    //std::cerr<<arg_len<<std::endl;
    ir.emplace_back(irCODE::FUNCTION_BEGIN,irOP(),arg_len,this->name.name);
    //考虑数组情况
    for(int i=0;i<arg_len;i++)
    {
        //参数是数组吗，能够转换类型
        auto identifier = dynamic_cast<ArrayIdentifier*>(&this->args.list[i]->name);
        if(identifier)
        {
            //保存shape大小
            std::vector<int> shape;
            for(auto i: identifier->shape) shape.push_back(i->eval(ctx));
            std::string arg_re = "%"+std::to_string(ctx.get_id());
            //保存参数，label处加入提示是数组
            ir.emplace_back(irCODE::MOV,irOP(arg_re),irOP("$arg"+std::to_string(i)),"array");
            //添加进入当前函数符号表
            ctx.insert_symbol(identifier->name.name, VarInfo(arg_re,true,shape));
        }
        else
        {
            //单变量
            std::string arg_re = "%"+std::to_string(ctx.get_id());
            ir.emplace_back(irCODE::MOV,irOP(arg_re),irOP("$arg"+std::to_string(i)));
            ctx.insert_symbol(args.list[i]->name.name,VarInfo(arg_re)); 
            list.emplace(list.end(),1);
        }
    }
    //处理block
    this->body.generate_ir(ctx,ir);
    ir.emplace_back(irCODE::FUNCTION_END,this->name.name);
    //添加函数进入函数信息表
    ctx.insert_function(this->name.name, FuncInfo(this->return_type, arg_len, list));
    //退出作用域
    ctx.end_scope();
}

//block
void Block::irGEN(ir::Context& ctx,IRList& ir)
{
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
        ir.emplace_back(ir::irCODE::RET,this->expr->eval_run(ctx,ir));
    else 
        ir.emplace_back(ir::irCODE::RET,ir::irOP());
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
数组赋值在store_runtime里面
*/
void AssignStmt::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    //判断是否是数组
    if(dynamic_cast<ArrayIdentifier*>(&this->lname))
    {
        auto rhs = this->rexpr.eval_run(ctx,ir);
        //使用store_runtime存储数组
        dynamic_cast<ArrayIdentifier*>(&this->lname)->store_runtime(rhs,ctx,ir);
    }
    else 
    {
        auto rh = this->rexpr.eval_run(ctx,ir);
        auto& lh = ctx.find_symbol(this->lname.name);
        //std::cerr<<lh.name<<std::endl;
        if(lh.is_array)
        {
            throw std::runtime_error("lexpr is a array");
        }
        else
        {
            if (rh.is_var() && rh.name[0] == '%' &&
            (lh.name[0] == '%' || lh.name.substr(0, 4) == "$arg") &&
            lh.name[0] != '@') {
                //两个局部变量在循环当中
                if (ctx.in_loop()) {
                    bool lhs_is_loop_var = false, rhs_is_loop_vae = false;
                    for (const auto& i : ctx.loop_var.top()) {
                        if (i == lh.name) lhs_is_loop_var = true;
                        if (i == rh.name) rhs_is_loop_vae = true;
                    }
                    if (lhs_is_loop_var && rhs_is_loop_vae) {
                        lh.name = "%" + std::to_string(ctx.get_id());
                        ir.emplace_back(irCODE::MOV, lh.name, rh);
                    } 
                    else {
                        lh.name = rh.name;
                    }
                } 
                //局部变量间赋值，直接赋值虚拟寄存器，不产生新的虚拟寄存器
                else {
                    lh.name = rh.name;
                }
            } 
            else if (lh.name[0] == '@') {
                //全局变量直接虚拟寄存器赋值
                ir.emplace_back(irCODE::MOV, lh.name, rh);
            } 
            else 
            {
                //非局部变量赋值，更新变量的虚拟寄存器号，保证SSA赋值
                lh.name = "%" + std::to_string(ctx.get_id());
                ir.emplace_back(irCODE::MOV, lh.name, rh);
                //添加进入const_assign，直接求值
                if (config::optimize_level > 0 && rh.is_imm()) {
                    ctx.insert_const_assign(lh.name, rh.value);
                }
            }
        }
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
//valueExpr
void ValueExpr::irGEN(ir::Context& ctx,ir::IRList& ir){
    this->eval_run(ctx,ir);
}

/*
IfStmt的实现，分为cond和thenod和elsedo两个部分
cond部分：实现短路求值，需要短路求值时插入phi_move
thenod和elsedo部分：对其引用前一个作用域的变量在其末尾位置设置phi_move
*/
void IfStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.create_scope();
    std::string id = std::to_string(ctx.get_id());
    //判断语句模块
    auto ret = this->cond.eval_cond_run(ctx,ir);
    //直接跳转
    if(ret.thenop == ir::irCODE::JMP)
    {
        this->thenstmt.generate_ir(ctx,ir);
        return;
    }    
    if(ret.elseop == ir::irCODE::JMP)
    {
        this->elsestmt.generate_ir(ctx,ir);
        return;
    }
    //cond结束，根据elseop跳转到IF_X_ELSE位置
    ir.emplace_back(ret.elseop, "IF_"+id+"_ELSE");
    /*
    先分别生成thenstmt和elsestmt的各种临时变量和ir语句。
    注意这里需要创建作用域并且退出，当需要临时变量时，在其内部作用域创建好，再退出。
    而当需要对当前作用域变量赋值时，会将当前符号表的变量VarInfo中的虚拟寄存器改变，参见Assignment
    语句的赋值，采用的是& lname引用。
    */
    ir::IRList thenlist,elselist;
    ir::Context then_ctx = ctx, else_ctx = ctx;
    then_ctx.create_scope();
    this->thenstmt.generate_ir(then_ctx,thenlist);
    then_ctx.end_scope();
    else_ctx.id = then_ctx.id;
    else_ctx.create_scope();
    this->elsestmt.generate_ir(else_ctx,elselist);
    else_ctx.end_scope();
    //返回当前的ctx
    ctx.id = else_ctx.id;

    ir::IRList end;
    end.emplace_back(ir::irCODE::LABEL, "IF_"+id+"_END");
    //根据thenstmt生成的symbol表进行phi_move变化
    for(int i=0;i < (int)then_ctx.symbol_table.size();i++)
    {
        for(auto &s : then_ctx.symbol_table[i])
        {
            /*
            实现：当在then_do和else_do中对前一个作用域变量赋值是，会改变其VarInfo即虚拟寄存器，而then_do和else_do
            改变的不一样(因为then_ctx.id = ctx.id，而else_ctx.id = then_ctx.id)，
            检查全部作用域，只要变量名称相同，而虚拟寄存器不同，即可认为发生了赋值，需要使用phi_move到新的同一个寄存器，
            并改变变量的寄存器值。
            */
           if(s.second.name!= else_ctx.symbol_table[i].find(s.first)->second.name)
           {
            //找到引用寄存器
            auto &v = ctx.find_symbol(s.first);
            assert(!v.is_array);
            if(v.name[0]== '%') v.name = "%"+std::to_string(ctx.get_id());
            thenlist.emplace_back(ir::irCODE::PHI_MOVE, v.name, ir::irOP(s.second.name));
            thenlist.back().phi_block = end.begin();
            elselist.emplace_back(ir::irCODE::PHI_MOVE, v.name, 
                                  irOP(else_ctx.symbol_table[i].find(s.first)->second.name));
            elselist.back().phi_block = end.begin();
           }
        }
    }
    //当处于循环当中时，循环体内if_else的信息插入
    if (!ctx.loop_break_symbol_snapshot.empty()) {
        auto& br = ctx.loop_break_symbol_snapshot.top();
        auto& then_br = then_ctx.loop_break_symbol_snapshot.top();
        auto& else_br = else_ctx.loop_break_symbol_snapshot.top();
        br.insert(br.end(), then_br.begin(), then_br.end());
        br.insert(br.end(), else_br.begin(), else_br.end());
        auto& co = ctx.loop_continue_symbol_snapshot.top();
        auto& then_co = then_ctx.loop_continue_symbol_snapshot.top();
        auto& else_co = else_ctx.loop_continue_symbol_snapshot.top();
        co.insert(co.end(), then_co.begin(), then_co.end());
        co.insert(co.end(), else_co.begin(), else_co.end());  
    }
    //连接then_do生成的ir
    ir.splice(ir.end(),thenlist);
    //elsestmt不是空的，在then_do末尾添加JMP语句
    if(!elselist.empty()) ir.emplace_back(irCODE::JMP,"IF_"+id+"_END");
    //进入else_do的标签
    ir.emplace_back(irCODE::LABEL,"IF_"+id+"_ELSE");
    ir.splice(ir.end(),elselist);
    ir.splice(ir.end(),end);
    ctx.end_scope();
}

//continue语句生成
void ContinueStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.loop_continue_symbol_snapshot.top().push_back(ctx.symbol_table);
    for (auto& i : ctx.loop_continue_phi_move.top()) {
        ir.emplace_back(
            irCODE::PHI_MOVE, i.second,
            irOP(
                ctx.symbol_table[i.first.first].find(i.first.second)->second.name));
    }
    ir.emplace_back(irCODE::JMP, "LOOP_" + ctx.loop_label.top() + "_CONTINUE");
}
//break语句生成
void BreakStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.loop_break_symbol_snapshot.top().push_back(ctx.symbol_table);
    for (auto& i : ctx.loop_break_phi_move.top()) {
        ir.emplace_back(
            irCODE::PHI_MOVE, i.second,
            irOP(
                ctx.symbol_table[i.first.first].find(i.first.second)->second.name));
    }
    ir.emplace_back(irCODE::JMP, "LOOP_" + ctx.loop_label.top() + "_END");
}

/*
while语句实现，与IfStmt类似
cond部分
*/
void WhileStmt::irGEN(Context& ctx, IRList& ir) {
    ctx.create_scope();
    //进入首先添加循环的标识数字
    ctx.loop_label.push(std::to_string(ctx.get_id()));
    ctx.loop_var.push({});
    // 在 `jeq END`、`break;`、`continue`前均需要插入 PHI_MOV

    // BRFORE
    Context ctx_before = ctx;
    IRList ir_before;

    // COND，包含在while循环当中与IfStmt形式上有所不用
    Context ctx_cond = ctx_before;
    IRList ir_cond;
    ir_cond.emplace_back(irCODE::LABEL,
                        "LOOP_" + ctx.loop_label.top() + "_BEGIN");
    auto cond = this->cond.eval_cond_run(ctx_cond, ir_cond);

    // JMP
    IRList ir_jmp;
    ir_jmp.emplace_back(cond.elseop, "LOOP_" + ctx.loop_label.top() + "_END");

    // DO (fake)，先生成一遍记录所有变量信息
    Context ctx_do_fake = ctx_cond;
    IRList ir_do_fake;
    ctx_do_fake.loop_continue_symbol_snapshot.push({});
    ctx_do_fake.loop_break_symbol_snapshot.push({});
    ctx_do_fake.loop_continue_phi_move.push({});
    ctx_do_fake.loop_break_phi_move.push({});
    this->stmt.generate_ir(ctx_do_fake, ir_do_fake);
    ctx_do_fake.loop_continue_symbol_snapshot.top().push_back(
        ctx_do_fake.symbol_table);

    // Do的symbel表是原来的，此时比较Do(fake)(更新之后)的表，
    // 判断哪些变量被引用了，并把它加入到continue_phi_move当中
    Context ctx_do = ctx_cond;
    IRList ir_do;
    ctx_do.loop_continue_symbol_snapshot.push({});
    ctx_do.loop_break_symbol_snapshot.push({});
    ctx_do.loop_continue_phi_move.push({});
    ctx_do.loop_break_phi_move.push({});
    //保存受影响的main()中的变量，到phi_move表中
    for (int i = 0; i < (int)ctx_do_fake.symbol_table.size(); i++) 
    {
        for (auto& symbol : ctx_do_fake.symbol_table[i]) 
        {
            for (int j = 0;j < (int)ctx_do_fake.loop_continue_symbol_snapshot.top().size(); j++) 
            {
                if (symbol.second.name !=
                        ctx_do_fake.loop_continue_symbol_snapshot.top()[j][i]
                        .find(symbol.first)->second.name) 
                {
                    ctx_do.loop_continue_phi_move.top().insert(
                        {{i, symbol.first}, "%" + std::to_string(ctx_do.get_id())});
                    break;
                }
            }
        }
    }
    //
    for (int i = 0; i < (int)ctx_cond.symbol_table.size(); i++) 
    {
        for (auto& symbol : ctx_cond.symbol_table[i]) 
        {
            for (int j = 0; j < (int)ctx_do_fake.loop_break_symbol_snapshot.top().size();j++) 
            {
                const auto do_name = ctx_do_fake.loop_break_symbol_snapshot.top()[j][i]
                                        .find(symbol.first)->second.name;
                if (symbol.second.name != do_name) 
                {
                    ctx_do.loop_break_phi_move.top().insert(
                        {{i, symbol.first}, symbol.second.name});
                    break;
                }
            }
        }
    }
    //DO模块添加循环标志
    ir_do.emplace_back(irCODE::LABEL, "LOOP_" + ctx.loop_label.top() + "_DO");
    //再生成一遍一样的的ctx_do,ir_do
    this->stmt.generate_ir(ctx_do, ir_do);
    //根据phi_move中保存的信息，在ir_do添加phi_move指令
    for (auto& i : ctx_do.loop_continue_phi_move.top()) {
        ir_do.emplace_back(irCODE::PHI_MOVE, i.second,
                        irOP(ctx_do.symbol_table[i.first.first]
                                    .find(i.first.second)
                                    ->second.name));
    }

    for (auto& i : ctx_do.loop_continue_phi_move.top()) {
        ctx_do.symbol_table[i.first.first].find(i.first.second)->second.name =
            i.second;
    }

    for (auto& i : ctx_do.loop_break_phi_move.top()) {
        ctx_cond.symbol_table[i.first.first].find(i.first.second)->second.name =
            i.second;
    }

    ctx_do.loop_continue_symbol_snapshot.pop();
    ctx_do.loop_break_symbol_snapshot.pop();
    ctx_do.loop_continue_phi_move.pop();
    ctx_do.loop_break_phi_move.pop();

    // CONTINUE
    Context ctx_continue = ctx_do;
    IRList ir_continue;
    // Continue模块的入口标识
    ir_continue.emplace_back(irCODE::LABEL,
                            "LOOP_" + ctx.loop_label.top() + "_CONTINUE");

    ir_cond.clear();
    ir_cond.emplace_back(irCODE::LABEL,
                        "LOOP_" + ctx.loop_label.top() + "_BEGIN");
    //添加phi_move语句，保存phi点
    for (int i = 0; i < (int)ctx_before.symbol_table.size(); i++) 
    {
        for (const auto& symbol_before : ctx_before.symbol_table[i]) 
        {
            const auto& symbo_continue =
                *ctx_continue.symbol_table[i].find(symbol_before.first);
            if (symbol_before.second.name != symbo_continue.second.name) 
            {
                const std::string new_name = "%" + std::to_string(ctx_before.get_id());
                ir_before.emplace_back(irCODE::PHI_MOVE, new_name,
                                    irOP(symbol_before.second.name));
                ir_before.back().phi_block = ir_cond.begin();
                ir_continue.emplace_back(irCODE::PHI_MOVE, new_name,
                                        irOP(symbo_continue.second.name));
                ctx_before.symbol_table[i].find(symbol_before.first)->second.name =
                    new_name;
                ctx_before.loop_var.top().push_back(new_name);
            }
        }
    }
    ir_continue.emplace_back(irCODE::JMP,
                            "LOOP_" + ctx.loop_label.top() + "_BEGIN");
    ir_continue.emplace_back(irCODE::LABEL,
                            "LOOP_" + ctx.loop_label.top() + "_END");
    //上面模拟运行完之后，再根据信息重新生成一遍
    // COND real
    ctx_cond = ctx_before;
    cond = this->cond.eval_cond_run(ctx_cond, ir_cond);
    // JMP real
    ir_jmp.clear();
    ir_jmp.emplace_back(cond.elseop, "LOOP_" + ctx.loop_label.top() + "_END");
    // DO (fake) real
    ctx_do_fake = ctx_cond;
    ir_do_fake.clear();
    ctx_do_fake.loop_continue_symbol_snapshot.push({});
    ctx_do_fake.loop_break_symbol_snapshot.push({});
    ctx_do_fake.loop_continue_phi_move.push({});
    ctx_do_fake.loop_break_phi_move.push({});
    this->stmt.generate_ir(ctx_do_fake, ir_do_fake);
    ctx_do_fake.loop_continue_symbol_snapshot.top().push_back(
        ctx_do_fake.symbol_table);
    // DO real
    ctx_do = ctx_cond;
    ir_do.clear();
    ir_continue.clear();
    IRList end;
    end.emplace_back(irCODE::LABEL, "LOOP_" + ctx.loop_label.top() + "_END");
    ctx_do.loop_continue_symbol_snapshot.push({});
    ctx_do.loop_break_symbol_snapshot.push({});
    ctx_do.loop_continue_phi_move.push({});
    ctx_do.loop_break_phi_move.push({});

    for (int i = 0; i < (int)ctx_do_fake.symbol_table.size(); i++) 
    {
        for (auto& symbol : ctx_do_fake.symbol_table[i]) 
        {
            for (int j = 0;
                j < (int)ctx_do_fake.loop_continue_symbol_snapshot.top().size(); j++) 
            {
                if (symbol.second.name !=
                    ctx_do_fake.loop_continue_symbol_snapshot.top()[j][i]
                        .find(symbol.first)->second.name) 
                {
                    ctx_do.loop_continue_phi_move.top().insert(
                        {{i, symbol.first}, "%" + std::to_string(ctx_do.get_id())});
                    break;
                }
            }
        }
    }
    for (int i = 0; i < (int)ctx_cond.symbol_table.size(); i++) 
    {
        for (auto& symbol : ctx_cond.symbol_table[i]) 
        {
            for (int j = 0; j <(int) ctx_do_fake.loop_break_symbol_snapshot.top().size();
                j++) 
            {
                const auto do_name = 
                    ctx_do_fake.loop_break_symbol_snapshot.top()[j][i].find(symbol.first)->second.name;
                if (symbol.second.name != do_name) 
                {
                    ctx_do.loop_break_phi_move.top().insert(
                        {{i, symbol.first}, symbol.second.name});
                    break;
                }
            }
        }
    }
    ir_do.emplace_back(irCODE::LABEL, "LOOP_" + ctx.loop_label.top() + "_DO");
    this->stmt.generate_ir(ctx_do, ir_do);
    for (auto& i : ctx_do.loop_continue_phi_move.top()) 
    {
        ir_do.emplace_back(irCODE::PHI_MOVE, 
                           i.second,
                           irOP(ctx_do.symbol_table[i.first.first].find(i.first.second)->second.name));
        ir_do.back().phi_block = end.begin();
    }
    for (auto& i : ctx_do.loop_continue_phi_move.top()) {
        ctx_do.symbol_table[i.first.first].find(i.first.second)->second.name =
            i.second;
    }

    for (auto& i : ctx_do.loop_break_phi_move.top()) {
        ctx_cond.symbol_table[i.first.first].find(i.first.second)->second.name =
            i.second;
    }

    ctx_do.loop_continue_symbol_snapshot.pop();
    ctx_do.loop_break_symbol_snapshot.pop();
    ctx_do.loop_continue_phi_move.pop();
    ctx_do.loop_break_phi_move.pop();
    // CONTINUE real
    ctx_continue = ctx_do;
    ir_continue.emplace_back(irCODE::LABEL,
                            "LOOP_" + ctx.loop_label.top() + "_CONTINUE");
    for (int i = 0; i < (int)ctx_before.symbol_table.size(); i++)
    {
        for (const auto& symbol_before : ctx_before.symbol_table[i]) 
        {
            const auto& symbo_continue =
                *ctx_continue.symbol_table[i].find(symbol_before.first);
            if (symbol_before.second.name != symbo_continue.second.name) 
            {
                ir_continue.emplace_back(irCODE::PHI_MOVE, symbol_before.second.name,
                                        irOP(symbo_continue.second.name));
                ir_continue.back().phi_block = ir_cond.begin();
            }
        }
    }
    ir_continue.emplace_back(irCODE::JMP,
                            "LOOP_" + ctx.loop_label.top() + "_BEGIN");
    //假读延长生命周期，将在循环之外的变量的生命周期延长，保证在寄存器覆盖时不会在循环时覆盖
    std::unordered_set<std::string> written;
    for (auto& irs : std::vector<IRList*>{&ir_cond, &ir_jmp, &ir_do}) {
        for (auto& i : *irs) {
        if (i.dest.is_var()) written.insert(i.dest.name);
        i.forEachOp(
            [&](irOP op) {
                if (op.is_var()) {
                if (!written.count(op.name)) {
                    ir_continue.emplace_back(irCODE::NOOP, irOP(), op);
                }
                }
            },
            false);
        }
    }
    //连接所有生成的ir
    ctx = ctx_cond;
    ctx.id = ctx_continue.id;
    ir.splice(ir.end(), ir_before);
    ir.splice(ir.end(), ir_cond);
    ir.splice(ir.end(), ir_jmp);
    ir.splice(ir.end(), ir_do);
    ir.splice(ir.end(), ir_continue);
    ir.splice(ir.end(), end);
    ctx.loop_var.pop();
    ctx.loop_label.pop();
    ctx.end_scope();
}

//数组初始值无显示定义
void ArrayDecl::irGEN(ir::Context& ctx,ir::IRList& ir){
    //数组大小
    std::vector<int> shape;
    //取得数组大小
    for(auto i : this->name.shape) shape.push_back(i->eval(ctx));
    int size=1;
    //总的大小
    for(auto i : shape) size*=i;
    if(ctx.is_global())
    {   //分配一个全局的未初始化的数组，.space值全部为0
        ir.emplace_back(irCODE::DATA_BEGIN,"@&"+this->name.name.name);
        ir.emplace_back(irCODE::DATA_SPACE,size*4);
        ir.emplace_back(irCODE::DATA_END);
        ctx.insert_symbol(this->name.name.name,VarInfo("@&"+this->name.name.name,true,shape));
    }
    else
    {   //分配一个局部的未初始化数组，放在栈中，全部为0
        ctx.insert_symbol(this->name.name.name,
                        VarInfo("%&"+ std::to_string(ctx.get_id()),true,shape));
        ir.push_back(IR(irCODE::MALLOC_IN_STACK,
                    irOP(ctx.find_symbol(this->name.name.name).name),
                    size*4));
    }
}
/*
参数：
that:这个带初始值的数组a[2][3]={...}
v:数组的初始值{....}
shape:数组大小
init_value:存中间值
index:shape的维度
*/
namespace{
    void _ArrayDeclWithInit(ArrayDeclWithInit& that,
    std::vector<ArrayDeclWithInitVal*> v,
    std::vector<Expression*> shape,
    std::vector<int>& init_value,
    int index,
    int total_size,
    Context& ctx,IRList& ir)
    {
        //常量数组，保存值
        const auto output_const = [&](int value, bool is_space = false) {
        //占位空间
        if (is_space) {
            //在全局域中
            if (ctx.is_global()) {
                //将init_value添加0，占位子
                for (int i = 0; i < value; i++) init_value.push_back(0);
                //全局空间分配.space，占位子(全为0)
                ir.emplace_back(irCODE::DATA_SPACE, value * 4);
            } 
            else 
            {   //直接占位子，不生成STORE，栈中默认是0
                for (int i = 0; i < value; i++) {
                    init_value.push_back(0);
                    ir.emplace_back(irCODE::STORE, irOP(),
                                irOP(ctx.find_symbol(that.name.name.name).name),
                                init_value.size() * 4 - 4, 0);
                }
            }
        }//是值 
        else {
            //是全局作用域，添加DATA_WORD
            init_value.push_back(value);
            if (ctx.is_global())
                ir.emplace_back(irCODE::DATA_WORD, value);
            else
            {
                //STORE保存：store ，数组符号，偏移(4位)，值
                ir.emplace_back(irCODE::STORE, irOP(),
                                irOP(ctx.find_symbol(that.name.name.name).name),
                                init_value.size() * 4 - 4, value);
            }
        }
        if((int)init_value.size() > total_size) 
            std::runtime_error("数组初始值空间过大:"+that.name.name.name);
    };
    const auto output = [&](irOP value, bool is_space = false) {
        //与上面类似，传入的是irOP，因为可以是变量
        if (is_space) {
            if (ctx.is_global()) {
                for (int i = 0; i < value.value; i++) init_value.push_back(0);
                ir.emplace_back(irCODE::DATA_SPACE, value.value * 4);
            } 
            else 
            {
                for (int i = 0; i < value.value; i++) 
                {
                    init_value.push_back(0);
                    ir.emplace_back(irCODE::STORE, irOP(),
                                    irOP(ctx.find_symbol(that.name.name.name).name),
                                    init_value.size() * 4 - 4, 0);
                }
            }
        } 
        else {
            //占个位置
            init_value.push_back(0);
            if (ctx.is_global())
                ir.emplace_back(irCODE::DATA_WORD, value);
            else
                ir.emplace_back(irCODE::STORE, irOP(),
                                irOP(ctx.find_symbol(that.name.name.name).name),
                                init_value.size() * 4 - 4, value);
        }
        if((int)init_value.size() > total_size) 
            std::runtime_error("数组初始值空间过大:"+that.name.name.name);
    };
    //到达最大维度，分配完毕
    if (index >= (int)shape.size()) return;
    int size = 1, write_size = 0;
    //当前维度大小:[2][3]就是3，[2][3][4]就是12
    for (auto it = shape.begin() + index; it != shape.end(); it++)
        size *= (*it)->eval(ctx);
    int size_this_shape = size / shape[index]->eval(ctx);
    //遍历list
    for (auto i : v) 
    {
        //当前初始值是个数字
        if (i->is_number) {
            write_size++;
            try {
                if (that.is_const)
                output_const(i->value->eval(ctx));
                else
                output(i->value->eval(ctx));
            } catch (...) {
                //局部数组，存在变量
                output(i->value->eval_run(ctx, ir));
            }
        } 
        else 
        {
            if (write_size % size_this_shape != 0) 
            {   //剩余补0，占位子
                if (that.is_const)
                    output_const(size_this_shape - (write_size % size_this_shape), true);
                else
                    output(size_this_shape - (write_size % size_this_shape), true);
            }
            //进入下一层
            _ArrayDeclWithInit(that, i->list, shape, init_value, index + 1,total_size,
                                    ctx, ir);
            write_size += size_this_shape;
        }
    }//末尾的空间，占位子
    
    if (that.is_const)
        output_const(size - write_size, true);
    else
        output(size - write_size, true);
    }
}
/*
数组带初始值的初始化
*/
void ArrayDeclWithInit::irGEN(ir::Context& ctx,ir::IRList& ir){

    std::vector<int> shape;
    for(auto i : this->name.shape) shape.push_back(i->eval(ctx));
    int size=1;
    //数组总大小
    for(auto i : shape) size*=i;
    //已经存放的数据
    std::vector<int> init_value;
    if(ctx.is_global())
    {
        ir.emplace_back(irCODE::DATA_BEGIN,"@&"+this->name.name.name);
        //生成全局数组
        _ArrayDeclWithInit(*this,this->value.list,this->name.shape,
                            init_value,0,size,ctx,ir);
        ir.emplace_back(irCODE::DATA_END);
        ctx.insert_symbol(this->name.name.name,
                            VarInfo("@&"+this->name.name.name,true,shape));
    }
    else
    {
        //局部数组：%&开头
        ctx.insert_symbol(this->name.name.name,
                            VarInfo("%&"+std::to_string(ctx.get_id()),true,shape));
        //分配在栈中：符号，大小
        ir.emplace_back(irCODE::MALLOC_IN_STACK,
                        irOP(ctx.find_symbol(this->name.name.name).name),
                        size*4);
        //开始分配标志
        //ir.emplace_back(irCODE::CALL,"memset");
        //使用STORE表示压入数据
        _ArrayDeclWithInit(*this,this->value.list,this->name.shape,
                            init_value,0,size,ctx,ir);
    }
}

/*
数组作为左值进行存储，此时维度中可以是变量
*/
void ArrayIdentifier::store_runtime(ir::irOP value,ir::Context& ctx, ir::IRList& ir)
{
    //查找数组寄存器
    auto v = ctx.find_symbol(this->name.name);
    if(v.is_array)
    {
        if(this->shape.size() == v.shape.size())
        {
            if (config::optimize_level > 0) {
                try {
                    int index = 0, size = 4;
                    for (int i = this->shape.size() - 1; i >= 0; i--) {
                        index += this->shape[i]->eval(ctx) * size;
                        size *= v.shape[i];
                    }
                    ir.emplace_back(irCODE::STORE, irOP(), v.name, index, value);
                    return;
                } catch (...) {}
            }
            /*判断维度大小，当数组维度出现变量时，不好检查错误
            for(int i=0;i < (int)v.shape.size();i++){
                if(this->shape[i]->eval(ctx) >= v.shape[i])
                    std::runtime_error(this->name.name+"数组大小超出范围");
            }*/
            //偏移值，和中间size大小
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
                }
            }
            ir.emplace_back(ir::irCODE::STORE,ir::irOP(),v.name,index,value);
        }
        else 
        {
            throw std::runtime_error("数组大小不匹配");
        }
    }
    else 
    {
        throw std::runtime_error(this->name.name + ":不是数组");
    }
}
}