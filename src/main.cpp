#include "config.hpp"
#include "ast/generate/generate.hpp"

int main(int arg,char** argv)
{   
    using namespace sysy;
    //处理输入流和输出流
    config::parser_arg(arg,argv);
    auto* root = sysy::ast::generate(config::inputfile);
    //输出AST抽象树
    if(config::print_ast)
    {
        root->print();
    }
    return 0;
}