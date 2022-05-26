#include "config.hpp"
#include "ast/generate/generate.hpp"
#include "IR/ir.hpp"
#include "IR/generate/generate.hpp"

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
    //生成IR
    auto ir = sysy::ir::generate(root);
    //输出IR
    if(config::print_ir)
    {
        for(auto& i:ir)
        {
            i.print();
        }
    }
    return 0;
}