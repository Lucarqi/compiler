%{
#include <iostream>
#include <stdlib>
#include "parser.hpp"

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();

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

%type  

%start CompUnit
%%

%%