/*
参数设置
*/
#pragma once 
#include <iostream>
#include <string>
#include <fstream>

namespace sysy::config{
//输出ast标志
extern bool print_ast;
extern bool print_ir;
//输入文件流
extern FILE* inputfile;
//输出流
extern std::ostream* output;
//输入文件名字
extern std::string filename;
//优化标志
extern int optimize_level;
//具体实现
void parser_arg(int arg, char** argv);
}