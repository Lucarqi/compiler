/*
*/
#include "config.hpp"

namespace sysy::config{

FILE* inputfile = stdin;
std::ostream* output = &std::cout;
bool print_ast = false;
bool print_ir = false;
std::string filename = "<stdin>";

void parser_arg(int arg, char** argv)
{
    int s=0;
    for(int i=1; i < arg; i++)
    {
        if(argv[i][0] == '-')
        {
            if(argv[i] == std::string("-o")) s=1;//-o outfilename
            else if(argv[i] == std::string("-print_ast")) print_ast=true;
            else if(argv[i] == std::string("-print_ir")) print_ir=true;
        }
        else 
        {
            if(s == 1)
            {   //向文件写
                //std::cerr<<argv[i]<<std::endl;
                output = new std::ofstream(argv[i], std::ofstream::out);
            }
            else if(s == 0)
            {   //从文件读
                inputfile = fopen(argv[i], "r");
                filename = argv[i];
            }
            s = 0;
        }
    }
}
}