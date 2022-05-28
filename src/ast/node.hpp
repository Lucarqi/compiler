/*
定义所有的节点信息
*/
#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include <stack>
#include <exception>

#include "config.hpp"
#include "IR/ir.hpp"
#include "error/error.hpp"

namespace sysy{
//INTGER是32位整型数字别名
using INTGER=std::int32_t;
namespace ir{
//作用域定义
class Context;
}
namespace ast::node{
/*
AST基本类定义
*/
class BaseNode {
    public:
    int line,column;
    BaseNode();
    //虚构析函数
    virtual ~BaseNode();
    /*
    虚函数，在子类实现：lock缩进，end对象末尾, 输出流cerr直接在终端显示
    */
    virtual void print(int lock=0, bool end=false, std::ostream& out = std::cerr);
    //输出格式
    void print_format(int lock=0,bool end=false, std::ostream& out=std::cerr);
    //生成IR接口
    void generate_ir(ir::Context& ctx,ir::IRList& ir);
    protected:
    //虚函数，子类实现
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST表达式接口
*/
class Expression:public BaseNode{
    public:
    //获取当前expr的值，接口
    int eval(ir::Context& ctx);
    //获得该对象表示的寄存器信息，接口
    ir::irOP eval_run(ir::Context& ctx,ir::IRList& ir);
    protected:
    //虚函数，子类实现
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST语句接口
继承自Expression，使用eval_run接口
*/
class Stmt: public Expression{
    protected:
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST声明接口
*/
class Decl:public BaseNode{};

/*
AST标识符定义
name:标识符名称
*/
class Identifier:public Expression{
    public:
    std::string name;
    Identifier(const std::string& name);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    //virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST条件表达式定义
value:当前整个表达式
*/
class ConditionExpr: public Expression{
    public:
    Expression& value;
    ConditionExpr(Expression& value);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST二元运算表达式
lh:左值
rh:右值
op:运算操作
*/
class BinaryExpr:public Expression{
    public:
    int op;
    Expression& lh;
    Expression& rh;
    BinaryExpr(int op,Expression& lh,Expression& rh);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST一元运算表达式
op:操作符记号
rh:表达式
*/
class UnaryExpr:public Expression{
    public:
    int op;
    Expression& rh;
    UnaryExpr(int op,Expression& rh);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST数字类定义
value:具体的数字
:传入的是string类型，需要转换
*/
class Number:public Expression{
    public:
    INTGER value;
    Number(const std::string& value);
    Number(INTGER value);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST函数调用语句参数定义
*/
class FunctionCallArgList:public Expression{
    public:
    std::vector<Expression*> args;
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
};

/*
AST函数调用定义
name:调用函数的标识符
args:参数列表(表达式列表)
*/
class FunctionCall:public Expression{
    public:
    Identifier& name;
    FunctionCallArgList& args;
    FunctionCall(Identifier& name,FunctionCallArgList& args);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST语句块定义
Stmts:语句列表
*/
class Block:public Stmt{
    public:
    std::vector<Stmt*> Stmts;
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST等式语句定义
lname:左边的量(标识符)
rexpr:右边的表达式
*/
class AssignStmt:public Stmt
{
    public:
    Identifier& lname;
    Expression& rexpr;
    AssignStmt(Identifier& lname,Expression& rexpr);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST后缀表达式语句
lname:标识符
op:操作
*/
class AfterInc:public Stmt
{
    public:
    Identifier& lname;
    int op;
    AfterInc(Identifier&lname,int op);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST IF语句
cond:条件表达式
thenstmt:真语句(语句块或者语句)
elsestmt:假语句(..)
:需要考虑单if的问题 
*/
class IfStmt:public Stmt
{
    public:
    ConditionExpr& cond;
    Stmt& thenstmt;
    Stmt& elsestmt;
    IfStmt(ConditionExpr& cond,Stmt& thenstmt,Stmt& elsestmt);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST While语句定义
cond:判断语句
stmt:语句块
*/
class WhileStmt:public Stmt
{
    public:
    ConditionExpr& cond;
    Stmt& stmt;
    WhileStmt(ConditionExpr& cond,Stmt& stmt);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST Break语句
没什么东西
*/
class BreakStmt:public Stmt
{
    public:
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST Continue语句
*/
class ContinueStmt:public Stmt
{
    public:
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST Return语句
expr:return返回值，是指针类型 ?
:考虑有返回值
*/
class ReturnStmt:public Stmt
{
    public:
    Expression* expr;
    ReturnStmt(Expression* expr=NULL);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 具体表达式最后的值节点
?
表达式的值
*/
class ValueExpr:public Stmt
{
    public:
    Expression& value;
    ValueExpr(Expression& expr);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 空语句void
*/
class VoidStmt:public Stmt
{
    public:
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 声明语句
list:各种声明句子，如int a=0,b[3]={1,2,3}；list是int之后的语句
type:这个声明的类型
:注意这是一个总的声明类，常量和变量在下面定义
*/
class DeclareStmt:public Decl
{
    public:
    int type;
    std::vector<Decl*> list;
    DeclareStmt(int type);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);  
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 带初始值的变量声明
name:标识符
value:初始值
is_const:判断是否是常量,默认为false
*/
class VarDeclWithInitVal:public Decl
{
    public:
    Identifier& name;
    Expression& value;
    bool is_const;
    VarDeclWithInitVal(Identifier& name,Expression& value,bool is_const=false);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 不带初始值的变量声明
name:标识符
:不需要判断是否是常量
*/
class VarDecl:public Decl
{
    public:
    Identifier& name;
    VarDecl(Identifier& name);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 数组的初始化值声明
is_number:此时是不是数字
value:是数字，值是多少
list:不是数字，是一堆值
:注意这里表示{{1,2},2+3,2/2,{a[1],b[2]}},只有表达式AddExp才是数字
*/
class ArrayDeclWithInitVal:public Expression
{
    public:
    bool is_number;
    // ?指针
    Expression* value;
    std::vector<ArrayDeclWithInitVal*> list;
    ArrayDeclWithInitVal(bool is_number,Expression* value);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
};

/*
AST 数组的标识符定义
name:标识符
shape:数字维度大小
*/
class ArrayIdentifier:public Identifier
{
    public:
    Identifier& name;
    std::vector<Expression*> shape;
    ArrayIdentifier(Identifier& name);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual int _eval(ir::Context& ctx);
    virtual ir::irOP _eval_run(ir::Context& ctx,ir::IRList& ir);
}; 

/*
AST 数组赋值节点
name:数组标识符
value:数组值
is_const:是常量吗
:注意这里是前面数组定义的整体框架，如 a[2][3] = {...}
*/
class ArrayDeclWithInit:public Decl
{
    public:
    ArrayIdentifier& name;
    ArrayDeclWithInitVal& value;
    bool is_const;
    ArrayDeclWithInit(ArrayIdentifier& name,ArrayDeclWithInitVal& value,bool is_const=false);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST 无初始化数组定义
name:数组标识符
*/
class ArrayDecl:public Decl
{
    public:
    ArrayIdentifier& name;
    ArrayDecl(ArrayIdentifier& name);
    virtual void print(int lock=0,bool end=false,std::ostream& out=std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
};

/*
AST函数定义时单个参数定义
type:类型
name:标识符
*/
class FuncArg:public Expression{
    public:
    int type;
    Identifier& name;
    FuncArg(int type,Identifier& name);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
};

/*
AST函数定义时参数列表定义
list:函数参数列表
*/
class FuncArgList:public Expression{
    public:
    std::vector<FuncArg*> list;
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
};

/*
AST函数定义
return_type:返回类型
name:标识符
args:函数参数列表类
body:函数主体类
*/
class FuncDefine:public BaseNode{
    public:
    int return_type;
    Identifier& name;
    FuncArgList& args;
    Block& body;
    FuncDefine(int return_type,Identifier& name,FuncArgList& list,Block& body);
    virtual void print(int lock=0,bool end=false, std::ostream& out= std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);

};

/*
AST 根节点的定义
body:所有函数和全局变量
*/
class Root:public BaseNode{
    public:
    std::vector<BaseNode*> body;
    virtual void print(int lock=0,bool end=true, std::ostream& out= std::cerr);
    virtual void irGEN(ir::Context& ctx,ir::IRList& ir);
    
};
}//sysy::ast::node
}//sysy
