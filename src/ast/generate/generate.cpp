/*
生成AST抽象语法树root
*/
#include "ast/generate/generate.hpp"

//实现在parser.cpp中，语法分析主函数
extern int yyparse();
//实现在lexer.cpp中，清除lexer实例
extern int yylex_destroy();
//实现在lexer.cpp中，设置yylineno
extern void yyset_lineno(int _line_number);
extern int yycolumn;
//实现在lexer.cpp中，设置输入流
void yyset_in(FILE* input);

namespace sysy::ast{
    sysy::ast::node::Root* root = nullptr;
    sysy::ast::node::Root* generate(FILE* input)
    {
        yyset_in(input);
        yyset_lineno(1);
        yycolumn = 1;
        yyparse();
        yylex_destroy();
        return root;
    }
}