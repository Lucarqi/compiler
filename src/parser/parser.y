%{
#include "ast/node.hpp"
#include "ast/generate/generate.hpp"
#include <iostream>
#include <cstdlib>
#include "parser.hpp"
#include "config.hpp"

//AST抽象语法树root，在generate.hpp中定义
using sysy::ast::root;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();

void yyerror(const char* s)
{
    std::cerr << sysy::config::filename << ":" <<yylloc.first_line << \
    ":" << yylloc.first_column << " error :" << s << std::endl;
    yylex_destroy();
    std::exit(1);
}
// BISON的location操作default定义
#define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do {                                                              \
        if (N) {                                                      \
            (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;    \
            (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
            (Current).last_line    = YYRHSLOC (Rhs, N).last_line;     \
            (Current).last_column  = YYRHSLOC (Rhs, N).last_column;   \
        } else {                                                      \
            (Current).first_line   = (Current).last_line   =          \
              YYRHSLOC (Rhs, 0).last_line;                            \
            (Current).first_column = (Current).last_column =          \
              YYRHSLOC (Rhs, 0).last_column;                          \
        }                                                             \
        yylloc = Current;                                             \
    } while (0)
//bison错误处理默认定义
#define yytnamerr(_yyres, _yystr)                                         \
        ([](char* yyres, const char* yystr) {                             \
            if (*yystr == '"') {                                          \
                if (yyres) return yystpcpy(yyres, yystr + 1) - yyres - 1; \
                else return yystrlen(yystr) - 2;                          \
            } else {                                                      \
                if (yyres) return yystpcpy(yyres, yystr) - yyres;         \
                else return yystrlen(yystr);                              \
            }                                                             \
        })(_yyres, _yystr)
%}

%locations
%union {
    int token;
    std::string *string;
    sysy::ast::node::Identifier* ident;
    sysy::ast::node::Expression* expr;
    sysy::ast::node::Root* root;
    sysy::ast::node::DeclareStmt* declare_stmt;
    sysy::ast::node::FuncDefine* funcdef;
    sysy::ast::node::Decl* decl;
    sysy::ast::node::ArrayDeclWithInitVal* array_init_val;
    sysy::ast::node::ArrayIdentifier* arrayident;
    sysy::ast::node::FunctionCall* functionCall;
    sysy::ast::node::FunctionCallArgList* functionCallArgList;
    sysy::ast::node::FuncArgList* funcdefarglist;
    sysy::ast::node::FuncArg* funcdefarg;
    sysy::ast::node::Block* block;
    sysy::ast::node::Stmt* stmt;
    sysy::ast::node::AssignStmt* assignstmt;
    sysy::ast::node::IfStmt* ifstmt;
    sysy::ast::node::ConditionExpr* condexpr;

}

%token <string> INT_VALUE "integer" IDENTIFIER "identifier"
%token <token> IF "if"  ELSE "else" WHILE "while" 
%token <token> BREAK "break" CONTINUE "continue" RETURN "return"
%token <token> CONST "const" INT "int" VOID "void" 
%token <token> LPARENT "(" RPARENT ")" LBRACKET "[" RBRACKET "]" LBRACE "{" RBRACE "}"
%token <token> ASSIGN "=" EQ "==" NQ "!="
%token <token> LT "<" LEQ "<=" GT ">" GEQ ">="
%token <token> AND "&&" OR "||"
%token <token> SEMI ";" COMMA ","
%token <token> PLUSPLUS "++" MINUSMINUS "--"
%token <token> PLUS "+" MINUS "-" MUL "*" DIV "/" MOD "%" NOT "!"

%left COMMA
%left ASSIGN
%left EQ NQ LT LEQ GT GEQ
%left PLUS MINUS MUL DIV MOD
%left NOT

%type <token> AddOp MulOp RelOp UnaryOp BType
%type <ident> Ident Lval
%type <expr> Number Exp LOrExp LAndExp EqExp AddExp MulExp PrimaryExp RelExp UnaryExp FuncCall
%type <root> CompUnit
%type <declare_stmt> Decl ConstDecl VarDecl ConstStmt VarStmt
%type <decl> VarDef DefOne DefArray ConstDef ConstDefOne ConstDefArray
%type <arrayident> ArrayName ArrayItem
%type <funcdef> FuncDef
%type <funcdefarglist> FuncFParams
%type <funcdefarg> FuncFParam FuncFParamArray FuncFParamOne
%type <array_init_val> ArrayInit ArrayInner
%type <functionCallArgList> FuncRParams
%type <block> Block BlockItems
%type <stmt> BlockItem Stmt AssignStmt AssignStmtNoSEMI IfStmt ReturnStmt WhileStmt BreakStmt ContinueStmt
%type <condexpr> Cond

%start CompUnit
%%
//开始分析
CompUnit    : CompUnit Decl {$$->body.push_back($<decl>2);}
            | CompUnit FuncDef {$$->body.push_back($<funcdef>2);}
            | Decl {root=new sysy::ast::node::Root();$$=root;$$->body.push_back($<decl>1);}
            | FuncDef {root=new sysy::ast::node::Root();$$=root;$$->body.push_back($<funcdef>1);}
            ;
//变量和常量语句
Decl        : ConstStmt
            | VarStmt
            ;
// ... ;
ConstStmt   : ConstDecl SEMI {$$=$1;}
            ;
// const int ... , ...
ConstDecl   : CONST BType ConstDef {$$=new sysy::ast::node::DeclareStmt($2);$$->list.push_back($3);}
            | ConstDecl COMMA ConstDef {$$->list.push_back($3);}
            ;
// int
BType       : INT
            ;
// ... ;
VarStmt     : VarDecl SEMI {$$=$1;}
            ;
// int ... , ...
VarDecl     : BType VarDef {$$=new sysy::ast::node::DeclareStmt($1);$$->list.push_back($2);}
            | VarDecl COMMA VarDef {$$->list.push_back($3);}
            ;
// 单一变量和数组
VarDef      : DefOne
            | DefArray
            ;
// 
ConstDef    : ConstDefOne
            | ConstDefArray
            ;
// a = exp
DefOne      : Ident ASSIGN AddExp {$$=new sysy::ast::node::VarDeclWithInitVal(*$1,*$3);}
            | Ident {$$=new sysy::ast::node::VarDecl(*$1);}
            ;
// a[][] = {}
DefArray    : ArrayName ASSIGN ArrayInit {$$=new sysy::ast::node::ArrayDeclWithInit(*$1,*$3); }
            | ArrayName {$$=new sysy::ast::node::ArrayDecl(*$1);}
            ;
// a[]...[]
ArrayName   : ArrayName LBRACKET AddExp RBRACKET {$$->shape.push_back($3);}
            | Ident LBRACKET AddExp RBRACKET {$$ = new sysy::ast::node::ArrayIdentifier(*$1); $$->shape.push_back($3);}
            ;
// {}
ArrayInit   : LBRACE ArrayInner RBRACE {$$=$2;}
            | LBRACE RBRACE {$$=new sysy::ast::node::ArrayDeclWithInitVal(false, nullptr);}
            ;
// {{},exp,..,}
ArrayInner  : ArrayInner COMMA AddExp {$$=$1; $$->list.push_back(new sysy::ast::node::ArrayDeclWithInitVal(true,$3)); }
            | ArrayInner COMMA ArrayInit {$$=$1; $$->list.push_back($3);}
            | ArrayInit {$$=new sysy::ast::node::ArrayDeclWithInitVal(false,nullptr);$$->list.push_back($1);}
            | AddExp {$$=new sysy::ast::node::ArrayDeclWithInitVal(false,nullptr);$$->list.push_back(new sysy::ast::node::ArrayDeclWithInitVal(true,$1));}
            ;
// 
ConstDefOne : Ident ASSIGN AddExp {$$=new sysy::ast::node::VarDeclWithInitVal(*$1,*$3,true);}
            ;
// 
ConstDefArray   : ArrayName ASSIGN ArrayInit {$$ = new sysy::ast::node::ArrayDeclWithInit(*$1, *$3, true);}
                ;

Exp : AddExp
    ;


LOrExp  : LOrExp OR LAndExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | LAndExp
        ;

LAndExp : LAndExp AND EqExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | EqExp
        ;

EqExp   : EqExp EQ RelExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | EqExp NQ RelExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | RelExp
        ;

RelExp  : RelExp RelOp AddExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | AddExp
        ;

AddExp  : AddExp AddOp MulExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | MulExp
        ;

MulExp  : MulExp MulOp UnaryExp {$$=new sysy::ast::node::BinaryExpr($2,*$1,*$3);}
        | UnaryExp
        ;

UnaryExp: UnaryOp UnaryExp {$$=new sysy::ast::node::UnaryExpr($1,*$2);}
        | FuncCall
        | PrimaryExp
        ;

FuncCall: Ident LPARENT FuncRParams RPARENT {$$=new sysy::ast::node::FunctionCall(*$1,*$3);}
        | Ident LPARENT RPARENT {$$=new sysy::ast::node::FunctionCall(*$1,*(new sysy::ast::node::FunctionCallArgList()));}
        ;

PrimaryExp  : Lval
            | Number
            | LPARENT Cond RPARENT {$$=$2;}
            | AssignStmtNoSEMI
            ;

Lval    : Ident 
        | ArrayItem
        ;

ArrayItem   : Lval LBRACKET Exp RBRACKET {$$=new sysy::ast::node::ArrayIdentifier(*$1);$$->shape.push_back($3);}
            | ArrayItem LBRACKET Exp RBRACKET {$$=$1; $$->shape.push_back($3);}
            ;

FuncDef : BType Ident LPARENT FuncFParams RPARENT Block {$$=new sysy::ast::node::FuncDefine($1,*$2,*$4,*$6);}
        | BType Ident LPARENT RPARENT Block {$$=new sysy::ast::node::FuncDefine($1,*$2,*(new sysy::ast::node::FuncArgList()),*$5);}
        | VOID Ident LPARENT FuncFParams RPARENT Block {$$=new sysy::ast::node::FuncDefine($1,*$2,*$4,*$6);}
        | VOID Ident LPARENT RPARENT Block {$$=new sysy::ast::node::FuncDefine($1,*$2,*(new sysy::ast::node::FuncArgList()),*$5);}
        ;
// (params){}
FuncFParams : FuncFParams COMMA FuncFParam {$$=$1;$$->list.push_back($3);}
            | FuncFParam {$$=new sysy::ast::node::FuncArgList();$$->list.push_back($1);}
            ;
// 函数调用的函数参数
FuncRParams : FuncRParams COMMA AddExp {$$=$1;$$->args.push_back($3);}
            | AddExp {$$=new sysy::ast::node::FunctionCallArgList();$$->args.push_back($1);}
            ;

FuncFParam  : FuncFParamOne
            | FuncFParamArray
            ;

FuncFParamOne   : BType Ident {$$=new sysy::ast::node::FuncArg($1,*$2);}
                ;

//是a[][3]类型，我就实际变为a[1][3]
FuncFParamArray : FuncFParamOne LBRACKET RBRACKET 
                {
                        $$=new sysy::ast::node::FuncArg($1->type,
                        *new sysy::ast::node::ArrayIdentifier(*(new sysy::ast::node::ArrayIdentifier($1->name))));
                        ((sysy::ast::node::ArrayIdentifier*)&($$->name))->shape.push_back(new sysy::ast::node::Number(1));
                }
                | FuncFParamArray LBRACKET Exp RBRACKET
                {
                        $$=$1;
                        ((sysy::ast::node::ArrayIdentifier*)&($$->name))->shape.push_back($3);
                }
                ;

Block   : LBRACE RBRACE {$$=new sysy::ast::node::Block();}
        | LBRACE BlockItems RBRACE {$$=$2;}
        ;

BlockItems      : BlockItem {$$=new sysy::ast::node::Block();$$->Stmts.push_back($1);}
                | BlockItems BlockItem {$$=$1;$$->Stmts.push_back($2);}
                ;

BlockItem       : Decl
                | Stmt
                ;

Stmt    : Block
        | AssignStmt
        | ReturnStmt
        | IfStmt
        | WhileStmt
        | BreakStmt
        | ContinueStmt
        | Exp SEMI {$$=new sysy::ast::node::ValueExpr(*$1);}
        | SEMI {$$=new sysy::ast::node::VoidStmt();}
        ;

AssignStmt      : AssignStmtNoSEMI SEMI {$$=$1;}
                ;

/*这里++a翻译为 a=a+1
a++翻译为AfterInc,即b=a,a=a+1
*/
AssignStmtNoSEMI: Lval ASSIGN AddExp {$$=new sysy::ast::node::AssignStmt(*$1,*$3);}
                | PLUSPLUS Lval 
                {
                        $$=new sysy::ast::node::AssignStmt(*$2,
                        *(new sysy::ast::node::BinaryExpr(PLUS,*$2,*new sysy::ast::node::Number(1))));
                }
                | MINUSMINUS Lval
                {
                        $$=new sysy::ast::node::AssignStmt(*$2,
                        *(new sysy::ast::node::BinaryExpr(MINUS,*$2,*new sysy::ast::node::Number(1))));
                }
                | Lval PLUSPLUS {$$=new sysy::ast::node::AfterInc(*$1,PLUSPLUS);}
                | Lval MINUSMINUS {$$=new sysy::ast::node::AfterInc(*$1,MINUSMINUS);}
                ;

IfStmt  : IF LPARENT Cond RPARENT Stmt {$$=new sysy::ast::node::IfStmt(*$3,*$5,*(new sysy::ast::node::VoidStmt()));}
        | IF LPARENT Cond RPARENT Stmt ELSE Stmt {$$=new sysy::ast::node::IfStmt(*$3,*$5,*$7);}
        ;

ReturnStmt      : RETURN Exp SEMI {$$=new sysy::ast::node::ReturnStmt($2);}
                | RETURN SEMI {$$=new sysy::ast::node::ReturnStmt();}
                ;
        
WhileStmt       : WHILE LPARENT Cond RPARENT Stmt {$$=new sysy::ast::node::WhileStmt(*$3,*$5);}
                ;

BreakStmt       : BREAK SEMI {$$=new sysy::ast::node::BreakStmt();}
                ;

ContinueStmt    : CONTINUE SEMI {$$=new sysy::ast::node::ContinueStmt();}
                ;

RelOp   : GT
        | GEQ
        | LT
        | LEQ
        ;

Cond    : LOrExp;

AddOp   : PLUS
        | MINUS
        ;

MulOp   : MUL
        | DIV
        | MOD
        ;

UnaryOp : PLUS
        | MINUS
        | NOT
        ;
Ident       : IDENTIFIER {$$=new sysy::ast::node::Identifier(*$1);}
            ;
Number      : INT_VALUE {$$=new sysy::ast::node::Number(*$1);}
            ;
%%
