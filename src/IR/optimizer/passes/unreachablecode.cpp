#include "IR/optimizer/unreachablecode.hpp"
#include "config.hpp"
#include "IR/ir.hpp"

/*
优化去除直接跳转JMP和return语句后面的不会执行语句
*/
namespace sysy::ir::passes{
    void unreachablecode(IRList& ir){
        for(auto it=ir.begin();it!=ir.end();it++){
            if(it->ircode==ir::irCODE::JMP||
                it->ircode==ir::irCODE::RET)
            {
                for(auto next=std::next(it);next!=ir.end();){
                    if(next->ircode!=irCODE::LABEL&&next->ircode!=irCODE::FUNCTION_END){
                        next = ir.erase(next);
                    }
                    else{
                        break;
                    }
                }
            }
        }
    }
}