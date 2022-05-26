#include "IR/generate/generate.hpp"

namespace sysy::ir{
IRList generate(sysy::ast::node::Root* root)
{
    IRList ir;
    ir::Context ctx;
    root->generate_ir(ctx,ir);
    return ir;
}
}