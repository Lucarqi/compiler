/*
AST node的实现
*/
#include "ast/node.hpp"
#include "parser.hpp"

namespace sysy::ast::node{
namespace{
//当前的IR生成对应AST对象
std::stack<BaseNode*> nodes;
}
/*
BaseNode实现
*/
BaseNode::BaseNode():line(yylloc.first_line),column{yylloc.first_column} {}
BaseNode::~BaseNode() {}
void BaseNode::print(int lock,bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
}
void BaseNode::print_format(int lock,bool end, std::ostream& out)
{
    for(int i=0; i<lock; i++)
    {
        out<< "│   ";
    }
    if(end) out<<"└──";
    else out<<"├──";
}
void BaseNode::generate_ir(ir::Context& ctx,ir::IRList& ir)
{
    try
    {
        nodes.push(this);
        this->irGEN(ctx,ir);
        nodes.pop();
    }
    catch(error::BaseError& e)
    {
        nodes.pop();
        std::cerr <<sysy::config::filename<<":"<<this->line
            <<":"<<this->column<< e.what() << '\n'; 
        abort();
    }
}

/*
Expression
*/
int Expression::eval(ir::Context& ctx)
{
    try
    {
        nodes.push(this);
        auto ret = this->_eval(ctx);
        nodes.pop();
        return ret;
    }
    catch(const std::exception& e)
    {
        nodes.pop();
        std::cerr << e.what() << '\n';
        abort();
    }
}
ir::irOP Expression::eval_run(ir::Context& ctx,ir::IRList& ir)
{
    try{
        nodes.push(this);
        auto ret = this->_eval_run(ctx,ir);
        nodes.pop();
        return ret;
    }catch(error::BaseError& e)
    {
        nodes.pop();
        std::cerr <<sysy::config::filename<<":"<<this->line
            <<":"<<this->column<< e.what() << '\n'; 
        abort();
    }
}

/*
Identifier
*/
Identifier::Identifier(const std::string& name)
    :name(name){}
void Identifier::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Identifier:"<<this->name<<std::endl;
}

/*
conditionexpr
*/
ConditionExpr::ConditionExpr(Expression& value)
    :value(value){}
void ConditionExpr::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"ConditionExpr:"<<std::endl;
    value.print(lock+1,true,out);
}

/*
BinaryExpr
*/
BinaryExpr::BinaryExpr(int op,Expression& lh,Expression& rh)
    :op(op),lh(lh),rh(rh){}
void BinaryExpr::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"BinaryExpr Op:"<<op<<std::endl;
    lh.print(lock+1,false,out);
    rh.print(lock+1,true,out);
}

/*
UnaryExpr
*/
UnaryExpr::UnaryExpr(int op,Expression& rh)
    :op(op),rh(rh){}
void UnaryExpr::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"UnaryExpr Op:"<<op<<std::endl;
    rh.print(lock+1,true,out);
}

/*
Number
*/
Number::Number(const std::string& value)
    :value(std::stoi(value,0,0)){}
Number::Number(INTGER value):value(value){}
void Number::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Number:"<<value<<std::endl;
}

/*
FunctionCallArgList
*/
void FunctionCallArgList::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionCallArgList"<<std::endl;
    for(auto i=args.begin();i!=args.end();i++)
    {
        (*i)->print(lock+1,i+1==args.end(),out);
    }
}

/*
FunctionCall
*/
FunctionCall::FunctionCall(Identifier& name,FunctionCallArgList& args)
    :name(name),args(args){}
void FunctionCall::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionCall"<<std::endl;
    name.print(lock+1,false,out);
    args.print(lock+1,false,out);
}

/*
Block
*/
void Block::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Block"<<std::endl;
    for(auto i=Stmts.begin();i!=Stmts.end();i++)
    {
        (*i)->print(lock+1,i+1==Stmts.end(),out);
    }
}

/*
AssignStmt
*/
AssignStmt::AssignStmt(Identifier& lname,Expression& rexpr)
    :lname(lname),rexpr(rexpr){}
void AssignStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Assignment"<<std::endl;
    lname.print(lock+1,false,out);
    rexpr.print(lock+1,true,out);
}

/*
AfterInc:a++,a--
*/
AfterInc::AfterInc(Identifier& lname,int op)
    :lname(lname),op(op){}
void AfterInc::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"AfterInc Op:"<<op<<std::endl;
    // ?true
    lname.print(lock+1,false,out);
}

/*
ifstatement
*/
IfStmt::IfStmt(ConditionExpr& cond,Stmt& thenstmt,Stmt& elsestmt)
    :cond(cond),thenstmt(thenstmt),elsestmt(elsestmt){}
void IfStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"IfStatement"<<std::endl;
    
    this->print_format(lock+1,false,out);
    out<<"Condition"<<std::endl;
    cond.print(lock+2,false,out);

    this->print_format(lock+1,false,out);
    out<<"Then"<<std::endl;
    thenstmt.print(lock+2,false,out);

    this->print_format(lock+1,false,out);
    out<<"Else"<<std::endl;
    elsestmt.print(lock+2,true,out);
}

/*
WhileStmt
*/
WhileStmt::WhileStmt(ConditionExpr& cond,Stmt& stmt)
    : cond(cond),stmt(stmt) {}
void WhileStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"WhileStatement"<<std::endl;

    this->print_format(lock+1,false,out);
    out<<"Cond"<<std::endl;
    cond.print(lock+2,false,out);

    this->print_format(lock+1,false,out);
    out<<"Do things"<<std::endl;
    // ?true
    stmt.print(lock+2,false,out);
}

/*
Break
*/
void BreakStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Break"<<std::endl;
}

/*
Continue
*/
void ContinueStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Continue"<<std::endl;
}

/*
Return
*/
ReturnStmt::ReturnStmt(Expression* expr)
    :expr(expr) {}
void ReturnStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Return"<<std::endl;
    //判断是否有返回值
    if(expr) expr->print(lock+1,true,out);
}

/*
ValueExpr
*/
ValueExpr::ValueExpr(Expression& value)
    :value(value){}
void ValueExpr::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock, end,out);
    out<<"Eval"<<std::endl;
    value.print(lock+1,true,out);
}

/*
VoidStmt
*/
void VoidStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"VoidStatemmet"<<std::endl;
}

/*
DeclareStmt
*/
DeclareStmt::DeclareStmt(int type)
    :type(type){}
void DeclareStmt::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Declare Type:"<<type<<std::endl;
    for(auto i=list.begin();i!=list.end();i++)
    {
        (*i)->print(lock+1,i+1==list.end(),out);
    }
}

/*
VarDeclWithInitVal
*/
VarDeclWithInitVal::VarDeclWithInitVal(Identifier& name,Expression& value,bool is_const)
    :name(name),value(value),is_const(is_const){}
void VarDeclWithInitVal::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"DeclareWithInit"<<std::endl;
    name.print(lock+1,false,out);
    value.print(lock+1,true,out);
}

/*
VarDecl
*/
VarDecl::VarDecl(Identifier& name):name(name){}
void  VarDecl::print(int lock, bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Declare"<<std::endl;
    name.print(lock+1,true,out);
}

/*
ArrayDeclWithInitVal
*/
ArrayDeclWithInitVal::ArrayDeclWithInitVal(bool is_number,Expression* value)
    :is_number(is_number),value(value){}
void ArrayDeclWithInitVal::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"ArrayDeclareInitValue"<<std::endl;
    if(is_number)
        value->print(lock+1,true,out);
    else 
        for(auto i=list.begin();i!=list.end();i++)
        {
            (*i)->print(lock+1,i+1==list.end(),out);
        }
}

/*
ArrayIdentifier
// 实例化Identifier？
*/
ArrayIdentifier::ArrayIdentifier(Identifier& name)
    :Identifier(name),name(name){}
void ArrayIdentifier::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"ArrayIdentifier"<<std::endl;
    name.print(lock+1,false,out);
    this->print_format(lock+1,true,out);
    out<<"Shape"<<std::endl;
    for(auto i=shape.begin();i!=shape.end();i++)
    {
        (*i)->print(lock+2,i+1==shape.end(),out);
    }
}

/*
ArrayDeclWithInit
*/
ArrayDeclWithInit::ArrayDeclWithInit(ArrayIdentifier& name,ArrayDeclWithInitVal& value,bool is_const)
    :name(name),value(value),is_const(is_const){}
void ArrayDeclWithInit::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"ArrayDeclareWithInit"<<std::endl;
    name.print(lock+1,false,out);
    value.print(lock+1,true,out);
}

/*
ArrayDecl
*/
ArrayDecl::ArrayDecl(ArrayIdentifier& name):name(name){}
void ArrayDecl::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"ArrayDecl"<<std::endl;
    name.print(lock+1,true,out);
}

/*
FunctinArg
*/
FuncArg::FuncArg(int type,Identifier& name)
    :type(type),name(name){}
void FuncArg::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionArgType:"<<type<<std::endl;
    name.print(lock+1,true,out);
}

/*
FunctionArgs
*/
void FuncArgList::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionArgList"<<std::endl;
    for(auto i=list.begin();i!=list.end();i++)
    {
        (*i)->print(lock+1,i+1 == list.end(),out);
    }
}
/*
FunctionDefine
*/
FuncDefine::FuncDefine(int return_type,Identifier& name,
        FuncArgList& args,Block& body)
        : return_type(return_type),name(name),args(args),body(body) {}
void FuncDefine::print(int lock,bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionDefine:"<<std::endl;
    
    this->print_format(lock+1,false,out);
    out<<"Return Type:"<<return_type<<std::endl;

    this->print_format(lock+1,false,out);
    out<<"Name"<<std::endl;
    name.print(lock+2,true,out);

    this->print_format(lock+1,false,out);
    out<<"Args"<<std::endl;
    args.print(lock+2,true,out);

    this->print_format(lock+1,true,out);
    out<<"Body"<<std::endl;
    body.print(lock+2,true,out);
}

/*
root
*/
void Root::print(int lock,bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Root"<<std::endl;
    //out<<this->body.size()<<std::endl;
    for(auto i=body.begin(); i!=body.end();i++)
    {
        (*i)->print(lock+1,i+1 == body.end(),out);
    }
}
}
  
