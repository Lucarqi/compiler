/*
AST node的实现
*/
#include "node.hpp"

namespace sysy::ast::node{
/*
BaseNode实现
*/
BaseNode::BaseNode() {}
BaseNode::~BaseNode() {}
void BaseNode::print(int lock,bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
}
void BaseNode::print_format(int lock,bool end, std::ostream& out)
{
    for(int i=0; i<lock; i++)
    {
        out<< "|   ";
    }
    if(end) out<<"└──";
    else out<<"├──";
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
BinaryExpr::BinaryExpr(Expression& lh,Expression& rh,int op)
    :lh(lh),rh(rh),op(op){}
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
UnaryExpr::UnaryExpr(Expression& rh,int op)
    :rh(rh),op(op){}
void UnaryExpr::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"UnaryExpr Op:"<<op<<std::endl;
    rh.print(lock+1,true,out);
}

/*
FunctionCallArgList
*/
void FunctionCallArgList::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    for(auto i=args.begin();i!=args.end();i++)
    {
        (*i)->print(lock+1,i+1==args.end(),out);
    }
}

/*
FunctionCall
*/
FunctionCall::FunctionCall(Identifier& name,FunctionCallArgList& list)
    :name(name),args(args){}
void FunctionCall::print(int lock,bool end,std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"FunctionCall"<<std::endl;
    name.print(lock+1,false,out);
    args.print(lock+1,false,out);
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
    out<<"Declare Type"<<type<<std::endl;
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
    :name(name),Identifier(name){}
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
void FuncDefine::print(int lock=0,bool end=false, std::ostream& out= std::cerr)
{
    this->print_format(lock,end,out);
    out<<"FunctionDefine:"<<std::endl;
    
    this->print_format(lock+1,false,out);
    out<<"Return-type:"<<return_type<<std::endl;

    this->print_format(lock+1,false,out);
    out<<"Name"<<std::endl;
    name.print(lock+2,false,out);

    this->print_format(lock+1,false,out);
    out<<"Args"<<std::endl;
    args.print(lock+2,false,out);

    this->print_format(lock+1,false,out);
    out<<"Body"<<std::endl;
    body.print(lock+2,false,out);
}

/*
root
*/
void Root::print(int lock,bool end, std::ostream& out)
{
    this->print_format(lock,end,out);
    out<<"Root"<<std::endl;
    for(auto i=body.begin(); i!=body.end();i++)
    {
        (*i)->print(lock+1,i+1 == body.end(),out);
    }
}
}