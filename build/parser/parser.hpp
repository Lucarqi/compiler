/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_LUCAR_COMPILER_BUILD_PARSER_PARSER_HPP_INCLUDED
# define YY_YY_HOME_LUCAR_COMPILER_BUILD_PARSER_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT_VALUE = 258,               /* "integer"  */
    IDENTIFIER = 259,              /* "identifier"  */
    IF = 260,                      /* "if"  */
    ELSE = 261,                    /* "else"  */
    WHILE = 262,                   /* "while"  */
    BREAK = 263,                   /* "break"  */
    CONTINUE = 264,                /* "continue"  */
    RETURN = 265,                  /* "return"  */
    CONST = 266,                   /* "const"  */
    INT = 267,                     /* "int"  */
    VOID = 268,                    /* "void"  */
    LPARENT = 269,                 /* "("  */
    RPARENT = 270,                 /* ")"  */
    LBRACKET = 271,                /* "["  */
    RBRACKET = 272,                /* "]"  */
    LBRACE = 273,                  /* "{"  */
    RBRACE = 274,                  /* "}"  */
    ASSIGN = 275,                  /* "="  */
    EQ = 276,                      /* "=="  */
    NQ = 277,                      /* "!="  */
    LT = 278,                      /* "<"  */
    LEQ = 279,                     /* "<="  */
    GT = 280,                      /* ">"  */
    GEQ = 281,                     /* ">="  */
    AND = 282,                     /* "&&"  */
    OR = 283,                      /* "||"  */
    SEMI = 284,                    /* ";"  */
    COMMA = 285,                   /* ","  */
    PLUSPLUS = 286,                /* "++"  */
    MINUSMINUS = 287,              /* "--"  */
    PLUS = 288,                    /* "+"  */
    MINUS = 289,                   /* "-"  */
    MUL = 290,                     /* "*"  */
    DIV = 291,                     /* "/"  */
    MOD = 292,                     /* "%"  */
    NOT = 293                      /* "!"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 53 "src/parser/parser.y"

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


#line 125 "/home/lucar/compiler/build/parser/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_HOME_LUCAR_COMPILER_BUILD_PARSER_PARSER_HPP_INCLUDED  */
