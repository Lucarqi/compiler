/*
利用yyparser()生成AST并打印出来
*/
#pragma once
#include "ast/node.hpp"

namespace sysy::ast{
    extern sysy::ast::node::Root* root;
    sysy::ast::node::Root* generate(FILE* input);
}