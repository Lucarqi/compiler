#include <sstream>
#include "ast/generate/generate.hpp"
#include "config.hpp"
#include "IR/generate/generate.hpp"
#include "asm/generate/generate.hpp"
#include "IR/optimizer/optimize.hpp"

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
    if(config::optimize_level > 0){
        sysy::ir::optimize(ir);
    }
    //输出IR
    if(config::print_ir)
    {
        for(auto& i:ir) i.print(std::cerr);
    }
    std::stringstream asmbuffer;
    //生成汇编到文件
    sysy::asm_generator::generate_asm(ir,asmbuffer);
    *config::output << asmbuffer.str();
    if(config::output!=&std::cout) delete config::output;
    return 0;
}