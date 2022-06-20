/*
IR中间语言生成
作用域的构建
类型检查
重复定义检查
编译期间求值
*/
#include <assert.h>

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
        std::cerr<<find.name<<std::endl;
    }
    catch(error::BaseError& e)
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
            ctx.insert_symbol(this->name.name,VarInfo("%"+std::to_string(ctx.get_id())));
            //使用Assignment语句创建
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

//变量的声明
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
            ir.emplace_back(irCODE::DATA_BEGIN,irOP("@"+this->name.name));
            ir.emplace_back(irCODE::DATA_WORD,irOP(0));
            ir.emplace_back(irCODE::DATA_END);
            ctx.insert_symbol(this->name.name,VarInfo("@"+this->name.name));
        }
        else 
        {
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
    ir.emplace_back(irCODE::FUNCTION_BEGIN,irOP(),arg_len,irOP(this->name.name));
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
            //保存参数
            ir.emplace_back(irCODE::MOV,irOP(arg_re),irOP("$arg"+std::to_string(i)));
            //添加进入当前函数符号表
            ctx.insert_symbol(identifier->name.name, VarInfo(arg_re,true,shape));
        }
        else
        {
            //单变量
            std::string arg_re = "%"+std::to_string(ctx.get_id());
            ir.emplace_back(irCODE::MOV,irOP(arg_re),irOP("$arg"+std::to_string(i)));
            ctx.insert_symbol(args.list[i]->name.name,VarInfo(arg_re)); 
            //list.emplace(list.end(),1);
        }
    }
    //处理block
    this->body.generate_ir(ctx,ir);
    //处理return
    if(this->return_type == INT)
    {
        ir.emplace_back(irCODE::RET,irOP(),0);
    }
    else //返回类型为void
    {
        ir.emplace_back(irCODE::RET);
        
    }
    ir.emplace_back(irCODE::FUNCTION_END,this->name.name);
    //添加函数进入函数信息表
    ctx.insert_function(this->name.name, FuncInfo(this->return_type, arg_len, list));
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
添加数组支持
*/
void AssignStmt::irGEN(ir::Context& ctx,ir::IRList& ir)
{
    //判断是否是数组
    if(dynamic_cast<ArrayIdentifier*>(&this->lname))
    {
        //计算右式值
        auto rhs = this->rexpr.eval_run(ctx,ir);
        dynamic_cast<ArrayIdentifier*>(&this->lname)->store_runtime(rhs,ctx,ir);
    }
    else 
    {
        //UndefindError 交给generate_ir处理
        auto& lh = ctx.find_symbol(this->lname.name);
        auto rh = this->rexpr.eval_run(ctx,ir);
        if(lh.is_array)
        {
            throw std::runtime_error("lexpr is a array");
        }
        else
        {
            if (rh.is_var() && rh.name[0] == '%' &&
            (lh.name[0] == '%' || lh.name.substr(0, 4) == "$arg") &&
            lh.name[0] != '@') {
                if (ctx.in_loop()) {
                    bool lhs_is_loop_var = false, rhs_is_loop_vae = false;
                    //int lhs_level = -1, rhs_level = -1;
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
                else {
                    lh.name = rh.name;
                }
            } 
            else if (lh.name[0] == '@') {
                ir.emplace_back(irCODE::MOV, lh.name, rh);
            } 
            else 
            {
                lh.name = "%" + std::to_string(ctx.get_id());
                ir.emplace_back(irCODE::MOV, lh.name, rh);
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

//IfStmt
void IfStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.create_scope();
    std::string id = std::to_string(ctx.get_id());
    //判断语句模块
    auto ret = this->cond.eval_cond_run(ctx,ir);
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
    ir.emplace_back(ret.elseop,irOP(),irOP(), "IF_"+id+"_ELSE");
    //先分别生成thenstmt和elsestmt的各种临时变量和ir语句
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
    end.emplace_back(ir::irCODE::LABEL,irOP(),irOP(), "IF_"+id+"_END");
    //根据thenstmt生成的symbol表进行phi_move变化
    for(int i=0;i < (int)then_ctx.symbol_table.size();i++)
    {
        for(auto &s : then_ctx.symbol_table[i])
        {
            /*
            实现：then_ctx = ctx且有新的，else_ctx = ctx且有新的
            then_ctx临时寄存器 = else_ctx临时寄存器时，只有then且elsestmt中都没有对main()中变量的引用。
            只要thenstmt或者elsestmt对main()中变量有引用，都要加phi_move 
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
    //连接修改的then和else
    ir.splice(ir.end(),thenlist);
    //elsestmt不是空的，添加JMP语句
    if(!elselist.empty()) ir.emplace_back(irCODE::JMP, irOP(),irOP(),"IF_"+id+"_END");
    ir.emplace_back(irCODE::LABEL, irOP(),irOP(),"IF_"+id+"_ELSE");
    ir.splice(ir.end(),elselist);
    ir.splice(ir.end(),end);
    ctx.end_scope();
}

//continue语句生成
void ContinueStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.loop_continue_symbol_snapshot.top().push_back(ctx.symbol_table);
    for (auto& i : ctx.loop_continue_phi_move.top()) {
        ir.emplace_back(
            irCODE::MOV, i.second,
            irOP(
                ctx.symbol_table[i.first.first].find(i.first.second)->second.name));
    }
    ir.emplace_back(irCODE::JMP, ".L.LOOP_" + ctx.loop_label.top() + "_CONTINUE");
}
//break语句生成
void BreakStmt::irGEN(ir::Context& ctx,ir::IRList& ir){
    ctx.loop_break_symbol_snapshot.top().push_back(ctx.symbol_table);
    for (auto& i : ctx.loop_break_phi_move.top()) {
        ir.emplace_back(
            irCODE::MOV, i.second,
            irOP(
                ctx.symbol_table[i.first.first].find(i.first.second)->second.name));
    }
    ir.emplace_back(irCODE::JMP, ".L.LOOP_" + ctx.loop_label.top() + "_END");
}

void WhileStmt::irGEN(Context& ctx, IRList& ir) {
    ctx.create_scope();
    ctx.loop_label.push(std::to_string(ctx.get_id()));
    ctx.loop_var.push({});

    // 此处的块与基本快有所不同，见下图
    /*      ┌───────────┐
        COND:│ cmp       │
            ├───────────┤
        JMP:│ // jne DO │
            │ jeq END   │
            ├───────────┤
        DO:│ ...       │
            │ break;    │
            │ continue; │
            ├───────────┤
    CONTINUE:│ jmp COND  │
            ├───────────┤
        END:│           │
            └───────────┘
    */
    // 在 `jne END`、`break;`、`continue`前均需要插入 PHI_MOV

    // BRFORE
    Context ctx_before = ctx;
    IRList ir_before;

    // COND
    Context ctx_cond = ctx_before;
    IRList ir_cond;
    ir_cond.emplace_back(irCODE::LABEL,
                        ".L.LOOP_" + ctx.loop_label.top() + "_BEGIN");
    auto cond = this->cond.eval_cond_run(ctx_cond, ir_cond);

    // JMP
    IRList ir_jmp;
    ir_jmp.emplace_back(cond.elseop, ".L.LOOP_" + ctx.loop_label.top() + "_END");

    //先生成一个symbol表，记录所有的信息
    // DO (fake)
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
    //保存受影响的main()中的变量
    for (int i = 0; i < (int)ctx_do_fake.symbol_table.size(); i++) {
        for (auto& symbol : ctx_do_fake.symbol_table[i]) {
        for (int j = 0;
            j < (int)ctx_do_fake.loop_continue_symbol_snapshot.top().size(); j++) {
            if (symbol.second.name !=
                ctx_do_fake.loop_continue_symbol_snapshot.top()[j][i]
                    .find(symbol.first)
                    ->second.name) {
            ctx_do.loop_continue_phi_move.top().insert(
                {{i, symbol.first}, "%" + std::to_string(ctx_do.get_id())});
            break;
            }
        }
        }
    }
    //类似，目前不清楚?
    for (int i = 0; i < (int)ctx_cond.symbol_table.size(); i++) {
        for (auto& symbol : ctx_cond.symbol_table[i]) {
        for (int j = 0; j < (int)ctx_do_fake.loop_break_symbol_snapshot.top().size();
            j++) {
            const auto do_name = ctx_do_fake.loop_break_symbol_snapshot.top()[j][i]
                                    .find(symbol.first)
                                    ->second.name;
            if (symbol.second.name != do_name) {
            ctx_do.loop_break_phi_move.top().insert(
                {{i, symbol.first}, symbol.second.name});
            break;
            }
        }
        }
    }
    ir_do.emplace_back(irCODE::LABEL, ".L.LOOP_" + ctx.loop_label.top() + "_DO");
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
    ir_continue.emplace_back(irCODE::PHI_MOVE,
                            ".L.LOOP_" + ctx.loop_label.top() + "_CONTINUE");

    ir_cond.clear();
    ir_cond.emplace_back(irCODE::PHI_MOVE,
                        ".L.LOOP_" + ctx.loop_label.top() + "_BEGIN");

    for (int i = 0; i < (int)ctx_before.symbol_table.size(); i++) {
        for (const auto& symbol_before : ctx_before.symbol_table[i]) {
        const auto& symbo_continue =
            *ctx_continue.symbol_table[i].find(symbol_before.first);
        if (symbol_before.second.name != symbo_continue.second.name) {
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
                            ".L.LOOP_" + ctx.loop_label.top() + "_BEGIN");
    ir_continue.emplace_back(irCODE::LABEL,
                            ".L.LOOP_" + ctx.loop_label.top() + "_END");

    //////////////////////////////////////////////////////////////////////

    //前面是准备，保存各种phi_move，生成符号等
    // COND real
    ctx_cond = ctx_before;
    cond = this->cond.eval_cond_run(ctx_cond, ir_cond);

    // JMP real
    ir_jmp.clear();
    ir_jmp.emplace_back(cond.elseop, ".L.LOOP_" + ctx.loop_label.top() + "_END");

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
    //ir_continue.emplace_back(OpCode::NOOP);
    IRList end;
    end.emplace_back(irCODE::LABEL, ".L.LOOP_" + ctx.loop_label.top() + "_END");
    ctx_do.loop_continue_symbol_snapshot.push({});
    ctx_do.loop_break_symbol_snapshot.push({});
    ctx_do.loop_continue_phi_move.push({});
    ctx_do.loop_break_phi_move.push({});
    for (int i = 0; i < (int)ctx_do_fake.symbol_table.size(); i++) {
        for (auto& symbol : ctx_do_fake.symbol_table[i]) {
        for (int j = 0;
            j < (int)ctx_do_fake.loop_continue_symbol_snapshot.top().size(); j++) {
            if (symbol.second.name !=
                ctx_do_fake.loop_continue_symbol_snapshot.top()[j][i]
                    .find(symbol.first)
                    ->second.name) {
            ctx_do.loop_continue_phi_move.top().insert(
                {{i, symbol.first}, "%" + std::to_string(ctx_do.get_id())});
            break;
            }
        }
        }
    }
    for (int i = 0; i < (int)ctx_cond.symbol_table.size(); i++) {
        for (auto& symbol : ctx_cond.symbol_table[i]) {
        for (int j = 0; j <(int) ctx_do_fake.loop_break_symbol_snapshot.top().size();
            j++) {
            const auto do_name = ctx_do_fake.loop_break_symbol_snapshot.top()[j][i]
                                    .find(symbol.first)
                                    ->second.name;
            if (symbol.second.name != do_name) {
            ctx_do.loop_break_phi_move.top().insert(
                {{i, symbol.first}, symbol.second.name});
            break;
            }
        }
        }
    }
    ir_do.emplace_back(irCODE::LABEL, ".L.LOOP_" + ctx.loop_label.top() + "_DO");
    this->stmt.generate_ir(ctx_do, ir_do);
    for (auto& i : ctx_do.loop_continue_phi_move.top()) {
        ir_do.emplace_back(irCODE::PHI_MOVE, i.second,
                        irOP(ctx_do.symbol_table[i.first.first]
                                    .find(i.first.second)
                                    ->second.name));
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
                            ".L.LOOP_" + ctx.loop_label.top() + "_CONTINUE");
    for (int i = 0; i < (int)ctx_before.symbol_table.size(); i++) {
        for (const auto& symbol_before : ctx_before.symbol_table[i]) {
        const auto& symbo_continue =
            *ctx_continue.symbol_table[i].find(symbol_before.first);
        if (symbol_before.second.name != symbo_continue.second.name) {
            ir_continue.emplace_back(irCODE::PHI_MOVE, symbol_before.second.name,
                                    irOP(symbo_continue.second.name));
            ir_continue.back().phi_block = ir_cond.begin();
        }
        }
    }
    ir_continue.emplace_back(irCODE::JMP,
                            ".L.LOOP_" + ctx.loop_label.top() + "_BEGIN");
    //连接ir
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

//无初始值的数组声明
void ArrayDecl::irGEN(ir::Context& ctx,ir::IRList& ir){
    //数组大小
    std::vector<int> shape;
    //取得数组大小
    for(auto i : this->name.shape) shape.push_back(i->eval(ctx));
    int size=1;
    //总的大小
    for(auto i : shape) size*=i;
    if(ctx.is_global())
    {
        ir.emplace_back(irCODE::DATA_BEGIN,"@&"+this->name.name.name);
        ir.emplace_back(irCODE::DATA_SPACE,size*4);
        ir.emplace_back(irCODE::DATA_END);
        ctx.insert_symbol(this->name.name.name,VarInfo("@&"+this->name.name.name,true,shape));
    }
    else
    {
        ctx.insert_symbol(this->name.name.name,
                        VarInfo("&"+ std::to_string(ctx.get_id()),true,shape));
        ir.push_back(IR(irCODE::MALLOC_IN_STACK,
                    irOP(ctx.find_symbol(this->name.name.name).name),
                    size*4));
    }
}
//带初始值的数组声明
//辅助函数
namespace{
    void _ArrayDeclWithInit(ArrayDeclWithInit& that,
    std::vector<ArrayDeclWithInitVal*> v,std::vector<Expression*> shape,
    std::vector<int>& init_value,int index,
    Context& ctx,IRList& ir)
    {
        const auto output_const = [&](int value, bool is_space = false) {
        if (is_space) {
        if (ctx.is_global()) {
            for (int i = 0; i < value; i++) init_value.push_back(0);
            ir.emplace_back(irCODE::DATA_SPACE, value * 4);
        } else {
            for (int i = 0; i < value; i++) {
            init_value.push_back(0);
            }
        }
        } else {
        init_value.push_back(value);
        if (ctx.is_global())
            ir.emplace_back(irCODE::DATA_WORD, value);
        else
            ir.emplace_back(irCODE::STORE, irOP(),
                            irOP(ctx.find_symbol(that.name.name.name).name),
                            init_value.size() * 4 - 4, value);
        }
    };
    const auto output = [&](irOP value, bool is_space = false) {
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
                }
            }
        } 
        else {
            init_value.push_back(0);
            if (ctx.is_global())
                ir.emplace_back(irCODE::DATA_WORD, value);
            else
                ir.emplace_back(irCODE::STORE, irOP(),
                                irOP(ctx.find_symbol(that.name.name.name).name),
                                init_value.size() * 4 - 4, value);
        }
    };

    if (index >= (int)shape.size()) return;
    int size = 1, write_size = 0;
    for (auto it = shape.begin() + index; it != shape.end(); it++)
        size *= (*it)->eval(ctx);
    int size_this_shape = size / shape[index]->eval(ctx);
    for (auto i : v) 
    {
        if (i->is_number) {
            write_size++;
            try {
                if (that.is_const)
                output_const(i->value->eval(ctx));
                else
                output(i->value->eval(ctx));
            } catch (...) {
                output(i->value->eval_run(ctx, ir));
            }
        } 
        else {
        if (write_size % size_this_shape != 0) {
            if (that.is_const)
            output_const(size_this_shape - (write_size % size_this_shape), true);
            else
            output(size_this_shape - (write_size % size_this_shape), true);
        }
        _ArrayDeclWithInit(that, i->list, shape, init_value, index + 1,
                                ctx, ir);
        write_size += size_this_shape;
        }
    }
    if (that.is_const)
        output_const(size - write_size, true);
    else
        output(size - write_size, true);
    }
}
//处理带初始值的
void ArrayDeclWithInit::irGEN(ir::Context& ctx,ir::IRList& ir){
    std::vector<int> shape;
    for(auto i : this->name.shape) shape.push_back(i->eval(ctx));
    int size=1;
    //总的大小
    for(auto i : shape) size*=i;
    std::vector<int> init_value;
    if(ctx.is_global())
    {
        ir.emplace_back(irCODE::DATA_BEGIN,"@&"+this->name.name.name);
        _ArrayDeclWithInit(*this,this->value.list,this->name.shape,
                            init_value,0,ctx,ir);
        ir.emplace_back(irCODE::DATA_END);
        ctx.insert_symbol(this->name.name.name,
                            VarInfo("@&"+this->name.name.name,true,shape));
    }
    else
    {
        //局部数组
        ctx.insert_symbol(this->name.name.name,
                            VarInfo("%&"+std::to_string(ctx.get_id())));
        //需要分配空间拉
        ir.emplace_back(irCODE::MALLOC_IN_STACK,
                        irOP(ctx.find_symbol(this->name.name.name).name),
                        size*4);
        //将分配空间参数压栈 name,0,size
        ir.emplace_back(irCODE::SET_ARG,0,
                        irOP(ctx.find_symbol(this->name.name.name).name));
        ir.emplace_back(irCODE::SET_ARG,1,irOP(0));
        ir.emplace_back(irCODE::SET_ARG,2,size);
        ir.emplace_back(irCODE::CALL,"memset");
        _ArrayDeclWithInit(*this,this->value.list,this->name.shape,
                            init_value,0,ctx,ir);
    }
}

//向数组a[3][4]写入某个值
//value：写入的值
void ArrayIdentifier::store_runtime(ir::irOP value,ir::Context& ctx, ir::IRList& ir)
{
    //查找数组寄存器
    auto v = ctx.find_symbol(this->name.name);
    if(v.is_array)
    {
        if(this->shape.size() == v.shape.size())
        {
            irOP index = "%" + std::to_string(ctx.get_id());
            irOP size = "%" + std::to_string(ctx.get_id());
            ir.emplace_back(
                irCODE::SAL, index, 
                this->shape[this->shape.size()-1]->eval_run(ctx,ir),2
            );
            if(this->shape.size()!=1)
            {
                irOP temp;
                ir.emplace_back(irCODE::MOV,size,4*v.shape[this->shape.size()-1]);
            }
            for (int i = this->shape.size() - 2; i >= 0; i--) {
                irOP tmp = "%" + std::to_string(ctx.get_id());
                irOP tmp2 = "%" + std::to_string(ctx.get_id());
                ir.emplace_back(irCODE::MUL, tmp, size,
                                this->shape[i]->eval_run(ctx, ir));
                ir.emplace_back(irCODE::ADD, tmp2, index, tmp);
                index = tmp2;
                if (i != 0) {
                    irOP tmp = "%" + std::to_string(ctx.get_id());
                    ir.emplace_back(irCODE::MUL, tmp, size, v.shape[i]);
                    size = tmp;
                }
            }
            ir.emplace_back(irCODE::STORE, irOP(), v.name, index, value);
        }
        else 
        {
            throw std::runtime_error("shape is not match");
        }
    }
    else 
    {
        throw std::runtime_error(this->name.name + " is not a array");
    }
}
}