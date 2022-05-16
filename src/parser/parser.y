%{
#include <iostream>
#include <stdlib>
#include "parser.hpp"
#include "config.hpp"

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
// 分析验证函数
void _parser_print(const char *s,bool newline = false){}
void yyerror(const char* s)
{
    std::cerr << "location:" << "line is:" <<yylloc.first_line << \
    "column is :" << yylloc.firstcolumn << "error :" << s << std::endl;
    yylex_destroy();
}
// BISON的location操作
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

%}

%locations
%union {
    int token;
    std::string *string;
}

%token <string> INT_VALUE "integer" IDENTIFIER "identifier"
%token <token> IF "if"  ELSE "else" WHILE "while" 
%token <token> BREAK "break" CONTINUE "continue" RETURN "return"
%token <token> CONST "const" INT "int" VOID "void" 
%token <token> LPARENT "(" RPARENT ")" LBRACKET "[" RBRACKET "]" LBRACE "{" RBRACE "}"
%token <token> ASSIGN "=" EQ "==" NQ "!="
%token <token> LT "<" LEQ "<=" GT ">" GEQ ">="
%token <token> AND "&&" OR "||"
%token <token> DOT "." SEMI ";" COMMA ","
%token <token> PLUSPLUS "++" MINUSMINUS "--"
%token <token> PLUS "+" MINUS "-" MUL "*" DIV "/" MOD "%" NOT "!"

%left COMMA
%left ASSIGN
%left EQ NQ LT LEQ GT GEQ
%left PLUS MINUS MUL DIV MOD
%left NOT

%type <token> CompUnit Decl FuncDef ConstStmt VarStmt
%type <token> ConstDecl VarDecl ConstDef VarDef
%type <token> BType
%type <token> DefOne DefArray ConstDefOne ConstDefArray
%type <token> Ident 
%type <token> ArrayInit
%type <token> ArrayName
%type <token> AddExp Exp Cond
%type <token> ArrayInner
%type <token> LOrExp LAndExp EqExp RelExp 
%type <token> MulExp UnaryExp PrimaryExp
%type <token> RelOp AddOp MulOp
%type <token> FuncCall FuncFParams FuncRParams FuncFParam
%type <token> Number Lval
%type <token> ArrayItem
%type <token> Block
%type <token> FuncFParamOne FuncFParamArray
%type <token> BlockItems BlockItem
%type <token> Stmt IfStmt WhileStmt ReturnStmt BreakStmt ContinueStmt AssignStmt

%start CompUnit
%%
CompUnit    : CompUnit Decl
            | CompUnit FuncDef
            | Decl
            | FuncDef
            ;

Decl        : ConstStmt
            | VarStmt
            ;

ConstStmt   : ConstDecl SEMI
            ;

ConstDecl   : CONST BType ConstDef
            | ConstDecl COMMA ConstDef
            ;

BType       : INT
            ;

VarStmt     : VarDecl SEMI
            ;

VarDecl     : BType VarDef
            | VarDecl COMMA VarDef
            ;

VarDef      : DefOne
            | DefArray
            ;

ConstDef    : ConstDefOne
            | ConstDefArray
            ;

DefOne      : Ident ASSIGN AddExp 
            | Ident
            ;

DefArray    : ArrayName ASSIGN ArrayInit
            | ArrayName
            ;

ArrayName   : ArrayName LBRACKET AddExp RBRACKET
            | Ident LBRACKET AddExp RBRACKET
            ;

ArrayInit   : LBRACE ArrayInner RBRACE
            | LBRACE RBRACE
            ;

ArrayInner  : ArrayInner COMMA AddExp
            | ArrayInner COMMA ArrayInit
            | ArrayInit
            | AddExp
            ;

ConstDefOne : Ident ASSIGN AddExp
            | Ident
            ;

ConstDefArray   : ArrayName ASSIGN ArrayInit
                ;

Exp : AddExp
    ;

Cond: LOrExp 
    ;

LOrExp  : LOrExp OR LAndExp
        | LAndExp
        ;

LAndExp : LAndExp AND EqExp
        | EqExp
        ;

EqExp   : EqExp NQ RelExp
        | EqExp EQ RelExp
        | RelExp
        ;

RelExp  : RelExp RelOp AddExp
        | AddExp
        ;

AddExp  : AddExp AddOp MulExp
        | MulExp
        ;

MulExp  : MulExp MulOp UnaryExp
        | UnaryExp
        ;

UnaryExp: UnaryOp UnaryExp
        | FuncCall
        | PrimaryExp
        ;

FuncCall: Ident LPARENT FuncRParams RPARENT
        | Ident LPARENT LPARENT
        ;

PrimaryExp  : Lval
            | Number
            | LPARENT Cond RPARENT
            ;

Lval    : Ident
        | ArrayItem
        ;

ArrayItem   : Lval LBRACKET Exp RBRACKET
            | ArrayItem LBRACKET Exp RBRACKET
            ;

FuncDef : BType Ident LPARENT FuncFParams RPARENT Block
        | BType Ident LPARENT RPARENT Block
        | VOID Ident LPARENT FuncFParams RPARENT Block
        | VOID Ident LPARENT RPARENT Block
        ;

FuncFParams : FuncFParams COMMA FuncFParam
            | FuncFParam
            ;

FuncRParams : FuncRParams COMMA AddExp
            | AddExp
            ;

FuncFParam  : FuncFParamOne
            | FuncFParamArray
            ;

FuncFParamOne   : BType Ident 
                ;

FuncFParamArray : FuncFParamOne LBRACKET RBRACKET
                | FuncFParamArray LBRACKET Exp RBRACKET
                ;

Block   : LBRACE RBRACE
        | LBRACE BlockItems RBRACE
        ;

BlockItems      : BlockItem
                | BlockItems BlockItem
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
        | Exp SEMI
        | SEMI

RelOp   : GT
        | GEQ
        | LT
        | LEQ
        ;

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
Ident       : IDENTIFIER
            ;
Number      : INT_VALUE
            ;
%%
