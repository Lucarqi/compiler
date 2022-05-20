/*
生成AST抽象语法树root
*/
#include "ast/generate/generate.hpp"

extern int yyparse();
extern int yylex_destory();
extern void yyset_lineno(int line);
extern int yycolumn;
void yyset_in(FILE* input);

namespace sysy::ast{
    sysy::ast::node::Root* root = nullptr;
    sysy::ast::node::Root* generate(FILE* input)
    {
        yyset_in(input);
        yyset_lineno(1);
        yycolumn = 1;
        yyparse();
        yylex_destory();
        return root;
    }
}