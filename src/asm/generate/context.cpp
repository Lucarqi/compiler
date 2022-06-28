#include "asm/generate/context.hpp"



namespace sysy::asm_generator
{
using namespace std;
Context::Context(ir::IRList* irs,ir::IRList::iterator function_begin,std::ostream& out)
    :irs(irs),function_begin(function_begin),out(out){}
//对全局变量和数组的重新命名
string Context::rename(std::string name)
{
    if(name[0] == '@' && name[1] !='&')
        return "_var_"+name.substr(1);
    else 
        return "_array_"+name.substr(2);
}
/*
新的实现思路
*/
void Context::set_ir_time(ir::IR& cur)
{
    ir_in_time.insert({&cur,++time});
}
void Context::set_var_define_time(ir::IR& cur)
{
    int ir_time = ir_in_time[&cur];
    if(cur.dest.is_var() && (!var_define_time.count(cur.dest.name))){
        var_define_time.insert({cur.dest.name,ir_time});
        var_define_time_heap.insert(make_pair(time,cur.dest.name));
    }//数组变量不计入变量定义时间，因为每次数组时都会将其加载到内存中完成
    if(cur.op1.is_var() && (!var_define_time.count(cur.op1.name))
        && cur.op1.name[1]!='&'){
        var_define_time.insert({cur.op1.name,ir_time});
        var_define_time_heap.insert(make_pair(time,cur.op1.name));
    }
    if(cur.op2.is_var() && (!var_define_time.count(cur.op2.name))){
        var_define_time.insert({cur.op2.name,ir_time});
        var_define_time_heap.insert(make_pair(time,cur.op2.name));
    }
    if(cur.op3.is_var() && (!var_define_time.count(cur.op3.name))){
        var_define_time.insert({cur.dest.name,ir_time});
        var_define_time_heap.insert(make_pair(time,cur.op3.name));
    }
}
void Context::set_var_lastused_time(ir::IR& cur)
{
    int time = ir_in_time[&cur];
    if(cur.dest.is_var() && !var_lastused_time.count(cur.dest.name)){
        var_lastused_time.insert({cur.dest.name,time});
        var_lastused_time_heap.insert(make_pair(time,cur.dest.name));
    }//数组变量不计入变量定义时间，因为每次数组时都会将其加载到内存中完成
    if(cur.op1.is_var() && !var_lastused_time.count(cur.op1.name)
        && cur.op1.name[1]!='&'){
        var_lastused_time.insert({cur.op1.name,time});
        var_lastused_time_heap.insert(make_pair(time,cur.op1.name));
    } 
    if(cur.op2.is_var() && !var_lastused_time.count(cur.op2.name)){
        var_lastused_time.insert({cur.op2.name,time});
        var_lastused_time_heap.insert(make_pair(time,cur.op2.name));
    } 
    if(cur.op3.is_var() && !var_lastused_time.count(cur.op3.name)){
        var_lastused_time.insert({cur.op3.name,time});
        var_lastused_time_heap.insert(make_pair(time,cur.op3.name));
    } 
}

//存在可用寄存器
bool Context::find_reg()
{
    for(int i=0;i<reg_num;i++)
    {
        auto flag = avalibel_reg[i];
        if(flag==0) return true; 
    }   
    return false;
}

//变量溢出到栈,偏移以4为单位
void Context::store_var_stack(std::string name,int reg_id,ostream& out)
{
    stack_size += 4;
    var_in_stack.insert({name,stack_size});
    
    string reg = "r"+to_string(reg_id);
    out<<"      push  {"+reg+"}"<<endl;
}

/*
加载局部变量，全局变量，立即数的总接口
返回一个寄存器
*/
string Context::load_reg(ir::irOP op,std::ostream& out)
{
    if(op.is_imm()) return "#"+to_string(op.value);
    else if(op.is_local_var())
    {
        if(var_in_reg.count(op.name)){
            return "r"+to_string(var_in_reg.at(op.name));
        }
        else if(var_in_stack.count(op.name))
        {
            //先将其放在r14上，并清除栈上信息
            int offset = var_in_stack.at(op.name);
            var_in_stack.erase(op.name);
            out<<"      LDR  r14,  [sp,#"+to_string(stack_size-offset)+"]"<<endl;
            //理论上来说，应该栈减小?
            out<<"      POP"<<endl;
            stack_size -= 4;
            string  reg = "r"+to_string(get_reg(op,out));
            out<<"      MOV  "+reg+",  r14"<<endl;
            return reg;
        }
        else //只能是MOV语句的时候，首次出现？
        {
            int i = get_reg(op,out);
            return "r"+to_string(i);
        }
    }
    else if(op.is_global_var())
    {
        return load_global(op,out);
    }
    std::runtime_error(op.name+":不是立即数,不是局部变量，不是全局变量");
}
//加载全局变量到寄存器，以r14为基址，参数全局变量名称，out；
string Context::load_global(ir::irOP op,ostream& out)
{
    string pointer = Context::rename(op.name);
    //在寄存器上
    if(var_in_reg.count(op.name)){
        return "r"+to_string(var_in_reg.at(op.name));
    }
    //在栈上
    else if(var_in_stack.count(op.name)){
        int offset = var_in_reg.at(op.name);
        var_in_stack.erase(op.name);
        out<<"      MOV  r14,  [sp,#"+to_string(stack_size-offset)<<"]"<<endl;
        out<<"      POP"<<endl;
        stack_size -=4;
        int i = get_reg(op,out);
        out<<"      MOV  r"+to_string(i)+",  r14"<<endl;
        return "r"+to_string(i);
    }
    //都不存在，首先加载到r14，再分配一个寄存器
    else 
    {
        out<<"      mov32  r14,  "+pointer<<endl;
        out<<"      LDR  r14,  [r14,#0]"<<endl;
        int i = get_reg(op,out);
        out<<"      MOV  r"+to_string(i)+",  r14"<<endl;
        return "r"+to_string(i);
    }
}

//在var_in_reg和reg_in_var添加信息
void Context::set_var_in_reg(string name,int i){
    if(!find_reg()) throw std::runtime_error("没有可存放寄存器给:"+to_string(i));
    this->var_in_reg.insert({name,i});
    this->reg_in_var.insert({i,name});
    //std::cerr<<name+":reg"+to_string(i)<<endl;
    this->avalibel_reg[i] = 1;
}
//删除var_in_reg信息
void Context::off_var_in_reg(string name,int i){
    this->var_in_reg.erase(name);
    this->reg_in_var.erase(i);
    this->avalibel_reg[i] = 0;
}

/*
循环遍历r0到r11，返回第一个空的寄存器
若reg上有值，判定当前的时间和其最后一次用到的时间比较，次时间更大就直接占用
若reg已经满了，则将最后才用到的reg压栈，并改变stack_size长度，并写入var_in_stack
并且返回当前的寄存器号

这里寄存器分配有问题？
*/
int Context::get_reg(ir::irOP op,ostream& out)
{
    //用于获取lastused_time最大值
    int lastused_time = -1;
    string lastused_name = "";
    for(int i =0; i<reg_num; i++){
        if(avalibel_reg[i]==0){
            set_var_in_reg(op.name,i);
            return i;
        }
        else{
            string name = reg_in_var.at(i);
            int time = var_lastused_time.at(name);
            //定义时间比最后使用时间大，直接覆盖
            if(var_define_time.at(op.name) > time){
                off_var_in_reg(name,i);
                set_var_in_reg(op.name,i);
                return i;
            }//定义时间比最后使用时间小，且我的最后使用时间更小，将其取出压栈
            //如果最后使用时间还比他长的话，不采取行动，有可能12个寄存器都不能用?
            if(time < var_lastused_time.at(op.name) && time > lastused_time){
                lastused_time = time;
                lastused_name = name;
            }
        } 
    }
    if(lastused_time != -1)
    {
        //全部遍历完，都不符合条件，选择一个最后使用的压栈
        int to_stack_reg = var_in_reg.at(lastused_name);
        off_var_in_reg(lastused_name,to_stack_reg);
        set_var_in_reg(op.name,to_stack_reg);
        store_var_stack(op.name,to_stack_reg,out);
        return to_stack_reg;
    }//特殊情况，自己是最后一个使用的且寄存器是满的，加载到r12中，再压入栈中,这里存在问题
    else {
        return 12;
    }
}

//保存全局变量：寄存器号，全局变量名称
void Context::store_global(string reg,ir::irOP op,ostream& out){
    if(!op.is_global_var()) throw std::runtime_error("不是全局变量:"+op.name);
    string pointer = Context::rename(op.name);
    //加载地址
    out<<"      mov32  r14,  "+pointer<<endl;
    out<<"      STR  "+reg+",  [r14,#0]"<<endl;
}

//寄存器值入栈，理论上是连续且从r0开始的?
void Context::call_in(ostream& out)
{
    //遍历reg存入call_in_reg, 将存在的所有reg压栈（从r小到大）
    int max = -1;
    for(int i=0;i<12;i++){
        if(avalibel_reg[i]==1){
            string name = reg_in_var.at(i);
            //std::cerr<<name+"in call_in_var:"+to_string(i)<<endl;
            call_in_reg.insert({name,i});
            off_var_in_reg(name,i);
            max = i;
        }
    }
    int offset = max;
    //寄存器中没有值
    if(offset == -1) return;
    //有一个
    else if(offset == 0){
        out<<"      PUSH  {r0}"<<endl;
        stack_size += 4;
    }//有多个值
    else 
    {
        out<<"      PUSH  {r0-r"+to_string(offset)+"}"<<endl;
        stack_size += (offset+1)*4;
    }
}

/*
函数调用时加载参数，因为已经压栈了，参数在var_in_stack上
注意此时是反向压栈的，即sp最后停在r0上
前4个存入r0-r3,多的压入栈
需要压入栈时，sp先不动，最后统一跳转;stack_size不变
数组传递地址，将地址值压栈
*/
void Context::load_call(ir::irOP op,int offset,ostream& out)
{
    int greate_four = offset-3;
    //是立即数
    if(op.is_imm()){
        if(greate_four <= 0)
        {
            out<<"      MOV  r"+to_string(offset)+",  #"+to_string(op.value)<<endl;
        }//压入栈
        else {
            out<<"      MOV  r14,  #"+to_string(op.value);
            out<<"      STR  r14,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
        }
    }//传递数组参数
    else if(op.name[1]=='&'){
        //局部数组
        if(op.name[0]=='%'){
            int sp_offset = this->stack_size - var_in_stack.at(op.name);
            if(greate_four <= 0)
            {
                out<<"      ADD  r"+to_string(offset)+",  sp,  #"
                    +to_string(sp_offset)<<endl;
            }
            else {
                //首先加载到r14，再从r14加载到指定位置
                out<<"      ADD  r14,  sp"+to_string(sp_offset)<<endl;
                out<<"      STR  r14,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
            }
        }//全局数组
        else {
            //首先加载到r14
            out<<"      mov32  r12,  "+rename(op.name)<<endl;
            if(greate_four <=0){
                out<<"      MOV  r"+to_string(offset)+",  r12"<<endl;
            }
            else {
                out<<"      STR  r12,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
            }
        }
    }
    //在call_in_reg上找
    else if(call_in_reg.count(op.name)){
        if(greate_four <=0 ){
            out<<"      LDR  r"+to_string(offset)+",  [sp,#"
                +to_string(call_in_reg.at(op.name)*4)+"]"<<endl;
        }
        else {
            out<<"      LDR  r14,  [sp,#"+to_string(call_in_reg.at(op.name)*4)+"]"<<endl;
            out<<"      STR  r14,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
        }
    }//在var_in_stack栈中
    else if(var_in_stack.count(op.name)){
        int stack_offset = var_in_stack.at(op.name);
        if(greate_four <=0 ){

            out<<"      LDR  r"+to_string(offset)+",  [sp,#"+to_string(stack_size-stack_offset)
                +"]"<<endl;
        }
        else {
            out<<"      LDR  r14,  [sp,#"+to_string(stack_size-stack_offset)+"]"<<endl;
            out<<"      STR  r14,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
        }
    }
    //全局变量作为形式参数传递，在add()中不会改变其值，此时只可能是全局变量尚未加载
    else if(op.is_global_var()){
        string pointer = Context::rename(op.name);
        out<<"      mov32  r14,  "+pointer<<endl;
        if(greate_four <= 0){   
            out<<"      LDR  r"+to_string(offset)+",  [r14,#0]"<<endl;
        }
        else 
        {
            out<<"      LDR  r14,  [r14,#0]"<<endl;
            out<<"      STR  r14,  [sp,#-"+to_string(greate_four*4)+"]"<<endl;
        }
    }
    else throw std::runtime_error("函数参数不存在:"+op.name);
}

//函数调用结束将寄存器的值还原，理论上是连续还原的
void Context::call_out(ostream& out)
{
    int max = -1;
    //此时已经实际入栈了，现在从call_in_reg入reg
    for(auto i=call_in_reg.begin();i != call_in_reg.end();i++){
        int reg_id = i->second;
        string name = i->first;
        call_in_reg.erase(name);
        set_var_in_reg(name,reg_id);
        if(reg_id > max) max = reg_id;
    }   
    //没有还原的，不可能？
    if(max == -1){

    }//还原r0
    else if(max == 0){
        out<<"      POP  {r0}"<<endl;
        stack_size -=4;
    }
    else 
    {
        out<<"      POP  {r0-r"+to_string(max)+"}"<<endl;
        stack_size -=(max+1)*4;
    }
}


//实现除法
void Context::div_mod(ir::IR& ir,ostream& out)
{
    //将数据加载到reg上
    string op1 = load_reg(ir.op1,out);
    string op2 = load_reg(ir.op2,out);
    //函数调用
    call_in(out);
    //将数据放到r0和r1上
    load_call(ir.op1,0,out);
    load_call(ir.op2,1,out);
    //调用BL __aeabi_idiv
    if(ir.ircode==ir::irCODE::DIV){
        out<<"      BL  __aeabi_idiv"<<endl;
        //获取返回值
        out<<"      MOV  r12,  r0"<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOD){
        out<<"      BL  __aeabi_idivmod"<<endl;
        out<<"      MOV  r12,  r1"<<endl;
    }
    //将压入参数返回
    call_out(out);
    string dest = load_reg(ir.dest,out);
    out<<"      MOV  "+dest+",  r12"<<endl;
    if(ir.dest.is_global_var()){
        this->store_global(dest,ir.dest,out);
    }
}
//当为立即数时，首先加载到r12，其次加载到r14
void Context::cmp(ir::IR& ir,std::ostream& out){
    if(ir.op1.is_imm()&&ir.op2.is_imm()){
        out<<"      MOV  r12,  #"+to_string(ir.op1.value);
        out<<"      MOV  r14,  #"+to_string(ir.op2.value);
        out<<"      CMP  r12,  r14"<<endl;
    }
    else {
        string op1 = load_reg(ir.op1,out);
        string op2 = load_reg(ir.op2,out);
        if(op1.substr(0,1)=="#"){
            out<<"      MOV  r12,  "+op1<<endl;
            out<<"      CMP  r12,  "+op2<<endl;
        }
        else if(op2.substr(0,1)=="#"){
            out<<"      MOV  r12,  "+op2<<endl;
            out<<"      CMP  "+op1+",  r12"<<endl;
        }
        else {
            out<<"      MOV  "+op1+",  "+op2<<endl;
        }
    }
}

//处理moveq、movnq等。实现：存储0或者1，表示前一个cmp是真还是假，用于cond短路求值
void Context::movdo(ir::IR& ir,std::ostream& out){
    string op1 = "#"+to_string(ir.op1.value);
    string op2 = "#"+to_string(ir.op2.value);
    string dest = load_reg(ir.dest,out);
    if(ir.ircode==ir::irCODE::MOVEQ){
        out<<"      MOVEQ  "+dest+",  "+op1<<endl;
        out<<"      MOVNE  "+dest+",  "+op2<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOVGEQ){
        out<<"      MOVGE  "+dest+",  "+op1<<endl;
        out<<"      MOVLT  "+dest+",  "+op2<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOVGT){
        out<<"      MOVGT  "+dest+",  "+op1<<endl;
        out<<"      MOVLE  "+dest+",  "+op2<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOVLEQ){
        out<<"      MOVLE  "+dest+",  "+op1<<endl;
        out<<"      MOVGT  "+dest+",  "+op2<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOVLT){
        out<<"      MOVLT  "+dest+",  "+op1<<endl;
        out<<"      MOVGE  "+dest+",  "+op2<<endl;
    }
    else if(ir.ircode==ir::irCODE::MOVNQ){
        out<<"      MOVNE  "+dest+",  "+op1<<endl;
        out<<"      MOVEQ  "+dest+",  "+op2<<endl;
    }
    else std::runtime_error("不可识别MOV选择赋值操作");
}

//数组分配
void Context::array_build(ir::IR& ir, ostream& out)
{
    this->stack_size += ir.op1.value;
    out<<"      SUB  sp,  sp,  #"+to_string(ir.op1.value)<<endl;
    var_in_stack.insert({ir.dest.name,this->stack_size});
}
//数组压入数据，首先在r14压入数据，再实现压入(实现全局数组)
void Context::array_store(ir::IR& ir,ostream& out)
{
    //局部数组初始值压栈，op2必须是立即数，压栈数值可能是变量、立即数，全局变量不考虑
    if(ir.op2.is_imm()){
        if(ir.op3.is_imm()){
            int offset = this->stack_size - var_in_stack.at(ir.op1.name) + ir.op2.value;
            out<<"      MOV  r14,  #"+to_string(ir.op3.value)<<endl;
            out<<"      STR  r14,  [sp,#"+to_string(offset)+']'<<endl;
        }
        else {
            //加载变量，此时变量一定存在，此时可能存在压栈操作，需要将offset计算调整
            string reg = load_reg(ir.op3,out);
            int offset = this->stack_size - var_in_stack.at(ir.op1.name) + ir.op2.value;
            out<<"      STR  "+reg+",  [sp,#"+to_string(offset)+"]"<<endl;
        }
    }//此时是数组作为左值，op2必须是一个局部变量(因为生成ir时是这样设计的),需要对某一位置存储
    else {
        if(ir.op3.is_imm()){
            string index = load_reg(ir.op2,out);
            if(index=="r12"||index=="r14") 
                throw std::runtime_error(index+":寄存器分配会被覆盖掉");
            out<<"      MOV  r14,  #"+to_string(ir.op3.value)<<endl;
            //局部全局变量的保存
            if(ir.op1.name[0]=='%')
            {   /*
                这里是数组作为形式参数，地址被保存在了%2这种局部量里面，
                因为默认局部变量是直接加载到栈里面的，所以得修改一下，首先在var_in_reg
                里面找，再在var_in_stack里面找
                */
                if(ir.op1.name[1]!='&'){
                    //数组地址在reg_in_var里面
                    if(var_in_reg.count(ir.op1.name)){
                        int reg_id = var_in_reg.at(ir.op1.name);
                        out<<"      MOV  r12,  r"+to_string(reg_id)<<endl;
                        out<<"      STR  r14,  [r12,"+index+"]"<<endl;
                    }//数组地址在var_in_stack里面
                    else if(var_in_stack.count(ir.op1.name)){
                        int offset = this->stack_size - var_in_stack.at(ir.op1.name);
                        out<<"      LDR  r12,  [sp,#"+to_string(offset)+"]"<<endl;
                        out<<"      STR  r14,  [r12,"+index+"]"<<endl;
                    }
                    else throw runtime_error(ir.op1.name+"：形参数组地址加载出错");
                }
                else{
                    int offset = this->stack_size - var_in_stack.at(ir.op1.name);
                    out<<"      ADD  r12,  sp,  #"+to_string(offset)<<endl;
                    out<<"      STR  r14,  [r12,"+index+"]"<<endl;
                } 
            }
            else if(ir.op1.name[0]=='@'){
                out<<"      mov32  r12,  "+rename(ir.op1.name)<<endl;
                out<<"      STR  r14,  [r12,"+index+"]"<<endl;
            }
        }
        else {
            string index = load_reg(ir.op2,out);
            string value = load_reg(ir.op3,out);
            if(index == value) 
                std::runtime_error(ir.op2.name+"和"+ir.op3.name+"分配到同一个寄存器");
            if(index == "r12" || value == "r12")
                std::runtime_error("index或者value分配到r12上,会被数组地址加载覆盖");
            if(ir.op1.name[0]=='%')
            {
                if(ir.op1.name[1]!='&'){
                    //数组地址在var_in_reg上
                    if(var_in_reg.count(ir.op1.name)){
                        int reg_id = var_in_reg.at(ir.op1.name);
                        out<<"      MOV  r12,  r"+to_string(reg_id)<<endl;
                        out<<"      STR  "+value+",  [r12,"+index+"]"<<endl;   
                    }
                    //数组地址在var_in_stack上
                    else if(var_in_stack.count(ir.op1.name)){
                        int offset = this->stack_size - var_in_stack.at(ir.op1.name);
                        out<<"      MOV  r12,  [sp,#"+to_string(offset)+"]"<<endl;
                        out<<"      STR  "+value+",  [r12,"+index+"]"<<endl;
                    }
                    else throw runtime_error(ir.op1.name+":形参数组地址加载出错");
                }
                else {
                    int offset = this->stack_size - var_in_stack.at(ir.op1.name);
                    out<<"      ADD  r12,  sp,  #"+to_string(offset)<<endl;
                    out<<"      STR  "+value+",  [r12,"+index+"]"<<endl;
                }
            }
            else if(ir.op1.name[0]=='@'){
                out<<"      mov32  r12,  "+rename(ir.op1.name)<<endl;
                out<<"      STR  "+value+",  [r12,"+index+"]"<<endl;
            }
        }
    }
}

//加载数组中的某一项
void Context::array_load(ir::IR& ir,ostream& out){
    //全局数组，一定初始化了，没初始化的引用会在生成IR的时候出错
    if(ir.op1.name[0]=='@'){
        string dest = load_reg(ir.dest,out);
        string index = load_reg(ir.op2,out);
        if(dest=="r12"||index=="r12")
            throw std::runtime_error("dest或者index被分配到r12,会被数组地址覆盖");
        if(index==dest)
            throw std::runtime_error(ir.dest.name+"和"+ir.op2.name+"分配到同一个寄存器");
        out<<"      mov32  r12,  "+rename(ir.op1.name)<<endl;
        out<<"      LDR  "+dest+",  [r12,"+index+"]"<<endl;         
    }//局部数组
    else {
        if(!var_in_stack.count(ir.op1.name))
        std::runtime_error(ir.op1.name+"数组未保存");
        string dest = load_reg(ir.dest,out);
        string index = load_reg(ir.op2,out);
        int offset = this->stack_size - this->var_in_stack.at(ir.op1.name);
        out<<"      ADD  r12,  sp,  #"+to_string(offset)<<endl;
        if(dest=="r12"||index=="r12")
            throw std::runtime_error("dest或者index被分配到r12,会被数组地址覆盖");
        if(index == dest) 
            throw std::runtime_error(ir.dest.name+"和"+ir.op2.name+"分配到同一个寄存器");
        out<<"      LDR  "+dest+",  [r12,"+index+"]"<<endl;
    }
}
}//namesapce