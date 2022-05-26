#pragma once
#include "ast/node.hpp"
#include "IR/ir.hpp"

namespace sysy::ir{
IRList generate(sysy::ast::node::Root* root);
}