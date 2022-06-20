//生成汇编源码的接口
#pragma once
#include <iostream>
#include "IR/ir.hpp"

namespace sysy::asm_generator{
    void generate_asm(ir::IRList& ir,std::ostream& out);
}