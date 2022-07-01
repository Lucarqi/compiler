#include "asm/generate/context.hpp"
#include "asm/generate/generate.hpp"
#include "ast/node.hpp"
#include "IR/ir.hpp"
#include "config.hpp"

using namespace std;

namespace sysy::asm_generator
{
namespace {

//分析函数内部
/*
实现思路：寄存器直接使用，多了就溢出到栈中
*/
void generate_function(ir::IRList& irs,ir::IRList::iterator begin,
                    ir::IRList::iterator end, std::ostream& out)
{
    Context ctx(&irs,begin,out);
    /*
    额外辅助信息
    */
    //有set_args语句吗
    bool has_set_args = false;
    //是否需要传递参数
    bool need_args = false;
    //参数压栈的最大偏移值
    int args_max_offset = -1;
    //数组作为函数参数保存到栈中，其总的个数
    int array_as_param = 0;
    //是否有参数
    bool has_arg = false;
    //遍历保存信息
    for(auto it = begin;it!=end;it++)
    {
        ctx.set_ir_time(*it);
    }
    for(auto it = begin;it!=end;it++)
    {
        ctx.set_var_define_time(*it);
    }
    for(auto it = end;it!=begin;it--)
    {
        ctx.set_var_lastused_time(*it);
    }
    for(auto it = begin;it!=end;it++){
        if(it->ircode==ir::irCODE::CALL){
            ctx.has_call = true;
        }
    }
    for(auto it = begin;it!=end;it++){
        ctx.phi_alloc(*it);
    }
    if(!ctx.phi_in_stack.empty()) ctx.need_prealloc=true;
    //开始翻译
    for(auto i = begin;i!=end;i++)
    {   
        auto& ir = *i;
        //std::cerr<<ctx.ir_in_time[&ir]<<":"<<ir.dest.name<<endl;
        if(ir.ircode==ir::irCODE::FUNCTION_BEGIN)
        {
            out<<".text"<<endl;
            out<<".global   "<<ir.label<<endl;
            out<<".type     "<<ir.label<<",   %function"<<endl;
            out<<ir.label+":"<<endl;
            //只将lr压栈，当前函数的虚拟栈大小不变化。stack_size不变
            out<<"      push  {lr}"<<endl;
            if(ir.op1.value!=0) has_arg = true;
            if(ctx.need_prealloc && !has_arg){
                ctx.pre_alloc(out);
            }
        }
        else if(ir.ircode==ir::irCODE::MOV) 
        {
            //处理参数传递
            if(ir.op1.name.substr(0,4)=="$arg"){
                string name = ir.op1.name;
                int offset = stoi(name.substr(4));
                //在r0-r3中
                if(offset < 4){
                    //是数组，统一保存在var_in_stack中，前期寄存器少分配一个
                    if(ir.label=="array"){
                        array_as_param += 1;
                        ctx.stack_size += 4;
                        out<<"      STR  r"+to_string(offset)+",  [sp,#-"+to_string(ctx.stack_size)+"]"<<endl;
                        ctx.var_in_stack.insert({ir.dest.name,ctx.stack_size});
                    }
                    else {
                        ctx.set_var_in_reg(ir.dest.name,offset);
                    }
                    //跳转sp到正确位置
                    if(offset == 0){
                        int offset = array_as_param*4;
                        out<<"      SUB  sp,  sp,  #"+to_string(offset)<<endl;
                        array_as_param = 0;
                        if(ctx.need_prealloc && has_arg){
                            ctx.pre_alloc(out);
                            has_arg = 0;
                        }
                    }   
                }
                //在栈中
                else {
                    //是数组
                    if(ir.label == "array"){
                        ctx.stack_size += 4;
                        out<<"      LDR  r14,  [sp,#"+to_string((offset-3)*4)+
                        "]"<<endl;
                        out<<"      STR  r14,  [sp,#-"+to_string(ctx.stack_size)<<"]"<<endl;
                        ctx.var_in_stack.insert({ir.op1.name,ctx.stack_size});
                    }
                    else {
                        string reg = ctx.load_reg(ir.dest,out);
                        out<<"      LDR  "+reg+",  [sp,#"+to_string((offset-3)*4)+
                        "]"<<endl;
                    }
                }
            }//正常MOV
            else{
                if(!ir.dest.is_var()) 
                    throw runtime_error(ir.dest.name+":不是变量");
                string dest = ctx.load_reg(ir.dest,out);
                string op1 = ctx.load_reg(ir.op1,out);
                out<<"      MOV  "+dest+",  "+op1<<endl;
                //全局变量的保存
                if(ir.dest.is_global_var()) ctx.store_global(dest,ir.dest,out);
            }
            ctx.clear_phi_global(ir);
        }
        else if(ir.ircode==ir::irCODE::SET_ARG)
        {
            //首次传递参数，将寄存器压栈
            if(!need_args){
                need_args = true;
                has_set_args = true;
                ctx.call_in(out);
                args_max_offset += 1;
                
                ctx.load_call(ir.op1,ir.dest.value,out);
            }
            else {
                ctx.load_call(ir.op1,ir.dest.value,out);
            }
        }
        else if(ir.ircode==ir::irCODE::CALL)
        {
            //不需要任何参数的调用，sp不动
            if(!has_set_args){
                ctx.call_in(out);
            }//有参数入栈，sp改变
            if(args_max_offset > 3){
                
                out<<"      SUB  sp,  sp,  #"+to_string((args_max_offset-3)*4)<<endl;
            }

            has_set_args = false;
            need_args = false;
            out<<"      BL  "+ir.label<<endl;
            //保存结果到r12
            out<<"      MOV  r12,  r0"<<endl;
            //将压入的参数出栈
            if(args_max_offset > 3){
                out<<"      ADD  sp,  sp,  #"+to_string((args_max_offset-3)*4)<<endl;
            }
            args_max_offset = -1;
            //跳转完，需要将压入栈的寄存器出栈，并且保存结果
            ctx.call_out(out);
            string reg = ctx.load_reg(ir.dest,out);
            out<<"      MOV  "+reg+",  r12"<<endl;
        }
        else if(ir.ircode==ir::irCODE::RET)
        {
            string dest;
            //修正返回值为void，返回0值
            if(ir.dest.is_null()){
                dest = "#0";
            }
            else {
                dest = ctx.load_reg(ir.dest,out);
            }
            //sp返回栈底
            out<<"      MOV  r0,  "+dest<<endl;
            out<<"      ADD  sp,  sp,  #"+to_string(ctx.stack_size)<<endl;
            //将lr和fp出栈
            out<<"      POP  {r14}"<<endl;
            //将跳转到pc
            out<<"      MOV  pc,  r14"<<endl;
        }                                 
        else if(ir.ircode==ir::irCODE::ADD)      
        {                                           
            string dest = ctx.load_reg(ir.dest,out);
            string op1 = ctx.load_reg(ir.op1,out);  
            string op2 = ctx.load_reg(ir.op2,out);
            if(op1.substr(0,1)=="#") {
                out<<"      MOV  r14,  "+op1<<endl;
                out<<"      ADD  "+dest+",  r14"+",  "+op2<<endl;
            }
            else {
                out<<"      ADD  "+dest+",  "+op1+",  "+op2<<endl;
            }
            if(ir.dest.is_global_var()) ctx.store_global(dest,ir.dest,out);
            ctx.clear_phi_global(ir);
        }                                           
        else if(ir.ircode==ir::irCODE::SUB)      
        {                                           
            string dest = ctx.load_reg(ir.dest,out);
            string op1 = ctx.load_reg(ir.op1,out);
            string op2 = ctx.load_reg(ir.op2,out);
            
            if(op1.substr(0,1)=="#") {
                out<<"      MOV  r14,  "+op1<<endl;
                out<<"      SUB  "+dest+",  r14"+",  "+op2<<endl;
            }
            else {
                out<<"      SUB  "+dest+",  "+op1+",  "+op2<<endl;
            }  
            
            if(ir.dest.is_global_var()) ctx.store_global(dest,ir.dest,out);
            ctx.clear_phi_global(ir);
        }//注意是立即数时都需要
        else if(ir.ircode==ir::irCODE::MUL)      
        {                                           
            string dest = ctx.load_reg(ir.dest,out);
            string op1 = ctx.load_reg(ir.op1,out);  
            string op2 = ctx.load_reg(ir.op2,out);  
            if(op1.substr(0,1)=="#") {
                out<<"      MOV  r14,  "+op1<<endl;
                op1 = "r14";
            }
            if(op2.substr(0,1)=="#"){
                out<<"      MOV  r12,  "+op2<<endl;
                op2 = "r12";
            }
            out<<"      MUL  "+dest+",  "+op1+",  "+op2<<endl;
            if(ir.dest.is_global_var()) ctx.store_global(dest,ir.dest,out);
            ctx.clear_phi_global(ir);
        } 
        //实现除法和求余
        else if(ir.ircode==ir::irCODE::DIV||ir.ircode==ir::irCODE::MOD)
        {
            ctx.div_mod(ir,out);
            ctx.clear_phi_global(ir);
        }//跳转label
        else if(ir.ircode==ir::irCODE::LABEL){
            out<<ir.label <<":"<<endl;
        }//一系列的直接跳转
        else if(ir.ircode==ir::irCODE::JMP)
        {
            out<<"      B  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JEQ)
        {
            out<<"      BEQ  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JNQ)
        {
            out<<"      BNE  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JLE)
        {
            out<<"      BLE  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JLT)
        {
            out<<"      BLT  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JGE)
        {
            out<<"      BGE  "+ir.label<<endl;
        }
        else if(ir.ircode==ir::irCODE::JGT)
        {
            out<<"      BGT  "+ir.label<<endl;
        }//处理MOVEQ等条件赋值
        else if(ir.ircode==ir::irCODE::MOVEQ||ir.ircode==ir::irCODE::MOVGEQ
        ||ir.ircode==ir::irCODE::MOVGT||ir.ircode==ir::irCODE::MOVLEQ
        ||ir.ircode==ir::irCODE::MOVLT||ir.ircode==ir::irCODE::MOVNQ){
            ctx.movdo(ir,out);
        }//PHI_MOVE多次赋值
        else if(ir.ircode==ir::irCODE::PHI_MOVE){
            ctx.phi_move(ir,out);
            ctx.clear_phi_global(ir);
        }//CMP判断指令，后接B型跳转指令或者MOV型条件判断
        else if(ir.ircode==ir::irCODE::CMP){
            ctx.cmp(ir,out);
            ctx.clear_phi_global(ir);
        }
        /*
        下面实现数组的保存，加载和使用
        */
        //局部数组，开始在栈内分配空间
        else if(ir.ircode==ir::irCODE::MALLOC_IN_STACK)
        {
            ctx.array_build(ir,out);
        }//局部或者全局数组的加载
        else if(ir.ircode==ir::irCODE::LOAD)
        {   
            ctx.array_load(ir,out); 
            
            ctx.clear_phi_global(ir);  
        }//局部或者全局数组值的保存
        else if(ir.ircode==ir::irCODE::STORE)
        {
            ctx.array_store(ir,out);
            ctx.clear_phi_global(ir);
        }
        else if(ir.ircode==ir::irCODE::SAL){
            if(!ir.dest.is_local_var()) throw std::runtime_error(ir.dest.name+":非局部变量");
            string dest = ctx.load_reg(ir.dest,out);
            string op1 = ctx.load_reg(ir.op1,out);
            string op2 = ctx.load_reg(ir.op2,out);
            if(op1[0]=='#') 
            {
                out<<"      MOV  r14,  "+op1<<endl;
                out<<"      LSL  "+dest+",  r14,  "+op2<<endl;
            }
            else {
                out<<"      LSL  "+dest+",  "+op1+",  "+op2<<endl;
            }
            ctx.clear_phi_global(ir);
        }
        else if(ir.ircode==ir::irCODE::NOOP) continue;
    }
}

//生成arm指令，处理全局变量

}
void generate_asm(ir::IRList& irlist,std::ostream& out)
{
    //方便加载全局标识符
    out << R"(
.macro mov32, reg, val
    movw \reg, #:lower16:\val
    movt \reg, #:upper16:\val
.endm
)" << endl;
    //进入函数入口main
    out << R"(
.global __start
__start:
      BL main
      nop
    )"<<endl;
    ir::IRList::iterator function_begin;
    for(auto global = irlist.begin();global!=irlist.end();global++)
    {

        auto& ir= *global;
        if(ir.ircode == ir::irCODE::DATA_BEGIN)
        {
            out<<".data     "<<std::endl;
            out<<".global   "<<Context::rename(ir.label)<<std::endl;
            out<<Context::rename(ir.label)+":"<<endl;
        }
        else if(ir.ircode == ir::irCODE::DATA_WORD)
        {
            out<<".word     "<<ir.dest.value<<std::endl;
        }
        else if(ir.ircode == ir::irCODE::DATA_SPACE)
        {
            out<<".space    "<<ir.dest.value<<std::endl;
        }
        else if(ir.ircode == ir::irCODE::DATA_END)
        {
            //
        }
        //函数分析开始
        else if(ir.ircode == ir::irCODE::FUNCTION_BEGIN)
        {
            function_begin = global;
        }
        //开始分析
        else if(ir.ircode == ir::irCODE::FUNCTION_END)
        {
            generate_function(irlist,function_begin,global,out);
        }
    }
}
}

