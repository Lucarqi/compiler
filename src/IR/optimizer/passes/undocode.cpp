#include "IR/optimizer/passes/undocode.hpp"
#include "config.hpp"
#include "IR/ir.hpp"
#include "asm/generate/context.hpp"
/*
当mov语句中dest只出现1次，即可删去，配合const_assign求值使用
*/
namespace sysy::ir::passes{
    void undocode(IRList& ir){
        sysy::asm_generator::Context ctx(&ir,ir.begin(),std::cerr);
        //设置每条语句时间
        for(auto it=ir.begin();it!=ir.end();it++){
            ctx.set_ir_time(*it);
        }
        std::unordered_map<std::string,int> mov_var_define_time;
        std::unordered_map<std::string,int> mov_var_lastused_time;
        for(auto it=ir.begin();it!=ir.end();it++){
            auto cur = ctx.ir_in_time[&*it];
            if(it->ircode==ir::irCODE::MOV && it->op1.is_imm()
                && it->dest.is_var() && 
                !mov_var_define_time.count(it->dest.name)){
                mov_var_define_time.insert({it->dest.name,cur});
            }
        }
        for(auto it=std::prev(ir.end());it!=ir.begin();it--){
            auto cur = ctx.ir_in_time[&*it];
            if(it->ircode==ir::irCODE::MOV && it->op1.is_imm()
                && it->dest.is_var() && it->dest.name[0]=='%' &&
                !mov_var_lastused_time.count(it->dest.name)){
                if(cur == mov_var_define_time.at(it->dest.name)){
                //删除该条MOV语句
                it=ir.erase(it);
                }
            }
        }
        
    }
}