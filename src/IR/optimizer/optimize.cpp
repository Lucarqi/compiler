#include "IR/optimizer/optimize.hpp"
#include "asm/generate/context.hpp"
#include "config.hpp"
#include "IR/ir.hpp"
#include "IR/optimizer/passes.hpp"

namespace sysy::ir{
void optimize(IRList& ir){
    using namespace sysy::ir::passes;
    unreachablecode(ir);
    undocode(ir);
}
}